/**
 *******************************************************************************
 * @file arm1176jzf_s_interrupt.c
 * @author Olli Vanhoja
 * @brief Interrupt service routines.
 * @section LICENSE
 * Copyright (c) 2013 - 2015 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
 * Copyright (c) 2012, 2013 Ninjaware Oy,
 *                          Olli Vanhoja <olli.vanhoja@ninjaware.fi>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include <kinit.h>
#include <kerror.h>
#include <kstring.h>
#include <ksched.h>
#include <proc.h>
#include <hal/core.h>

/**
 * Interrupt vectors.
 *
 * @ note This needs to be aligned to 32 bits as the bottom 5 bits of the vector
 * address as set in the control coprocessor must be zero.
 */
__attribute__ ((naked, aligned(32))) static void interrupt_vectors(void)
{
    /*
     * Processor will never jump to bad_exception when reset is hit because
     * interrupt vector offset is set back to 0x0 on reset.
     */
    __asm__ volatile (          /* Event                    Pri LnAddr Mode */
        "b bad_exception\n\t"   /* Reset                    1   8      abt  */
        "b undef_handler\n\t"   /* Undefined instruction    6   0      und  */
        "b interrupt_svc\n\t"   /* Software interrupt       6   0      svc  */
        "b interrupt_pabt\n\t"  /* Prefetch abort           5   4      abt  */
        "b interrupt_dabt\n\t"  /* Data abort               2   8      abt  */
        "b bad_exception\n\t"   /* Unused vector                            */
        "b interrupt_sys\n\t"   /* IRQ                      4   4      irq  */
        "b bad_exception\n\t"   /* FIQ                      3   4      fiq  */
    );
}

__attribute__ ((naked)) void undef_handler(void)
{
    uintptr_t addr;
    uintptr_t lr;
    char buf[80];

    __asm__ volatile (
        "subs %[reg], r14, #4" : [reg] "=r" (addr) :: "r14");
    __asm__ volatile (
        "mov  %[reg], lr" : [reg] "=r" (lr) :: "lr");

    ksprintf(buf, sizeof(buf), "Undefined instruction @ %x, lr: %x\n",
             addr, lr);

    if (!current_thread ||
        thread_flags_is_set(current_thread, SCHED_INSYS_FLAG) ||
        current_thread->curr_mpt != &curproc->mm.mpt /* Probably in interrupt */
       ) {
        /*
         * TODO In some cases we could probably just kill the process/thread
         * and still maintain a stable system even though the issue was in
         * the kernel.
         */
        panic(buf);
    } else {
        const struct ksignal_param sigparm = { .si_code = ILL_ILLOPC };

        enable_interrupt();

        KERROR(KERROR_ERR, "%s", buf);

        /*
         * Kill the current process.
         */
        ksignal_sendsig_fatal(curproc, SIGILL, &sigparm);
        thread_wait();
    }
}

/**
 * Unhandled exception
 */
__attribute__ ((naked)) void bad_exception(void)
{
    panic("bad_exception.");
}

int arm_interrupt_preinit(void)
{
    SUBSYS_INIT("arm_interrupt_preinit");
    kputs(" ...enabling interrupts\n");

    /* Set interrupt base register */
    __asm__ volatile ("mcr p15, 0, %[addr], c12, c0, 0"
            : : [addr]"r" (&interrupt_vectors));
    /* Turn on interrupts */
    __asm__ volatile ("cpsie aif");

    return 0;
}
HW_PREINIT_ENTRY(arm_interrupt_preinit);

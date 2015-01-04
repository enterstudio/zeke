/**
 *******************************************************************************
 * @file    vm.h
 * @author  Olli Vanhoja
 * @brief   VM functions.
 * @section LICENSE
 * Copyright (c) 2014 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

#ifndef _VM_VM_H
#define _VM_VM_H

#ifdef KERNEL_INTERNAL
#include <llist.h>
#include <sys/tree.h>
#include <hal/mmu.h>
#include <klocks.h>
#include <fs/fs.h>

#define VM_PROT_READ    0x1 /*!< Read. */
#define VM_PROT_WRITE   0x2 /*!< Write. */
#define VM_PROT_EXECUTE 0x4 /*!< Execute. */
#define VM_PROT_COW     0x8 /*!< Copy-on-write. */

struct proc_info;

/**
 * VM page table structure.
 */
struct vm_pt {
    RB_ENTRY(vm_pt) entry_;
    mmu_pagetable_t pt;
    int linkcount; /*!< Links to this page table */
};

/* struct ptlist */
RB_HEAD(ptlist, vm_pt);

/**
 * MM struct for processes.
 */
struct vm_mm_struct {
    mmu_pagetable_t mpt;        /*!< Process master page table. */
    /** RB tree of page tables */
    struct ptlist ptlist_head;
#define MM_CODE_REGION  0
#define MM_STACK_REGION 1
#define MM_HEAP_REGION  2
    struct buf * (*regions)[]; /*!< Memory regions of a process.
                                 *   [0] = code         RORO
                                 *   [1] = stack        RWRW
                                 *   [2] = heap/data    RWRW
                                 *   [n] = allocs
                                 */
    int nr_regions;             /*!< Number of regions allocated. */
    mtx_t regions_lock;
};

/* Region insert operations */
#define VM_INSOP_SET_PT  0x01 /*!< Set default page tabe from process vpt. */
#define VM_INSOP_MAP_REG 0x02 /*!< Map the region to the given proc. */

/**
 * Compare vmp_pt rb tree nodes.
 * Compares virtual addresses of two page tables.
 * @param a is the left node.
 * @param b is the right node.
 * @return  If the first argument is smaller than the second, the function
 *          returns a value smaller than zero;
 *          If they are equal, the  function returns zero;
 *          Otherwise, value greater than zero is returned.
 */
int ptlist_compare(struct vm_pt * a, struct vm_pt * b);
RB_PROTOTYPE(ptlist, vm_pt, entry_, ptlist_compare);

/**
 * Get a page table for a given virtual address.
 * @param vaddr         is the virtual address that will be mapped into
 *                      a returned page table.
 * @return Returs a page table where vaddr can be mapped.
 */
struct vm_pt * ptlist_get_pt(struct vm_mm_struct * mm, uintptr_t vaddr);

/**
 * Free ptlist and its page tables.
 */
void ptlist_free(struct ptlist * ptlist_head);

/** @addtogroup copy copyin, copyout, copyinstr
 * Kernel copy functions.
 *
 * The copy functions are designed to copy contiguous data from one address
 * to another from user-space to kernel-space and vice-versa.
 * @{
 */

/**
 * Copy data from user-space to kernel-space.
 * Copy len bytes of data from the user-space address uaddr to the kernel-space
 * address kaddr.
 * @param[in]   uaddr is the source address.
 * @param[out]  kaddr is the target address.
 * @param       len  is the length of source.
 * @return 0 if succeeded; otherwise -EFAULT.
 */
int copyin(const void * uaddr, void * kaddr, size_t len);

/**
 * Copy data from kernel-space to user-space.
 * Copy len bytes of data from the kernel-space address kaddr to the user-space
 * address uaddr.
 * @param[in]   kaddr is the source address.
 * @param[out]  uaddr is the target address.
 * @param       len is the length of source.
 * @return 0 if succeeded; otherwise -EFAULT.
 */
int copyout(const void * kaddr, void * uaddr, size_t len);

/**
 * Copy a string from user-space to kernel-space.
 * Copy a NUL-terminated string, at most len bytes long, from user-space address
 * uaddr to kernel-space address kaddr.
 * @param[in]   uaddr is the source address.
 * @param[out]  kaddr is the target address.
 * @param       len is the maximum number of bytes to be copied.
 * @param[out]  done is the number of bytes actually copied, including the
 *                   terminating NUL, if done is non-NULL.
 * @return  0 if succeeded; or -ENAMETOOLONG if the string is longer than len
 *          bytes; or any of the return values defined for copyin().
 */
int copyinstr(const char * uaddr, char * kaddr, size_t len, size_t * done);

/**
 * Copyin from a process specified by proc argument.
 * Arguments same as for copyin() except proc.
 */
int copyin_proc(struct proc_info * proc, const void * uaddr, void * kaddr,
        size_t len);

/**
 * Copyout to a process specified by proc argument.
 * Arguments same as for copyout() except proc.
 */
int copyout_proc(struct proc_info * proc, const void * kaddr, void * uaddr,
        size_t len);

/**
 * @}
 */

/**
 * Create a new empty general purpose section.
 * Create a new empty buffer that can be inserted as a section/region
 * to a process into regions array. Allocated memory region will be aligned
 * so that vaddr + size will be the upper bound of the region regardless of
 * the page alignment requirements.
 * @param vaddr is the addess of the new section.
 * @param size is the size of the new section.
 * @param prot is a OR'd VM_PROT flags mask.
 */
struct buf * vm_newsect(uintptr_t vaddr, size_t size, int prot);

/**
 * Create a new section to a randomly selected address.
 * Returned section is inserted and mapped to the process if operation succeeds.
 * @param proc is a pointer to the process to be altered.
 * @param size is the size of the new section.
 * @param prot is a OR'd VM_PROT flags mask.
 * @return Returns a new section that is mapped to the given process.
 */
struct buf * vm_rndsect(struct proc_info * proc, size_t size, int prot);

/**
 * Update usr access permissions based on b_uflags.
 * @param region is the region to be updated.
 */
void vm_updateusr_ap(struct buf * region);

/**
 * Reallocate MM regions array to a new size given in new_count.
 * If new_count is smaller than the current size of regions array
 * calling this function will be NOP i.e. this function will only
 * icrease the size of the regions array. Note that not any part of the kernel
 * should take a pointer to the regions array without taking regions_lock and
 * any pointer to a regions array shall be consireded invalid when user doesn't
 * hold lock to a regions_lock controlling that array.
 * @param mm is a pointer to the mm struct to be updated.
 * @param new_count is the new size of the regions array.
 * @return Returns zero if succeed; Otherwise a negative errno is returned.
 */
int realloc_mm_regions(struct vm_mm_struct * mm, int new_count);

/**
 * Insert a reference to a region, set its pt and map.
 * @param region is the region to be inserted, since the region pt attribute is
 *               modified region must be set.
 * @returns value >= 0 succeed and value is the region nr;
 *          value < 0 failed with an error code.
 */
int vm_insert_region(struct proc_info * proc, struct buf * region, int op);

/**
 * Replace a region in process running image.
 * @param proc is a pointer to the PCB.
 * @param region is the region to be inserted.
 * @param region_nr is the region number to be replaced.
 * @param op contains bitwise OR'ed operations/options (VM_INSOP_) instructions
 *        how to handle the insertion.
 * @return Zero if succeed; non-zero error code otherwise.
 */
int vm_replace_region(struct proc_info * proc, struct buf * region,
                      int region_nr, int op);

/**
 * Map a VM region with the given page table.
 * Usually you don't want to use this function but instead you want to use
 * vm_replace_region() or vm_insert_region().
 * @note buf->b_mmu.pt is not respected and is considered as invalid.
 * @param region is a vm region buffer.
 * @param vm_pt is a vm page table.
 * @return Zero if succeed; non-zero error code otherwise.
 */
int vm_map_region(struct buf * region, struct vm_pt * pt);

/**
 * Map a VM region to given a process pointed by proc.
 * Usually you don't want to use this function but instead you want to use
 * vm_replace_region() or vm_insert_region().
 * @param proc is the process struct.
 * @param region is a vm region buffer.
 * @return Zero if succeed; non-zero error code otherwise.
 */
int vm_mapproc_region(struct proc_info * proc, struct buf * region);

/**
 * Check kernel space memory region for accessibility.
 * Check whether operations of the type specified in rw are permitted in the
 * range of virtual addresses given by addr and len.
 * @todo Not implemented properly!
 * @remark Compatible with BSD.
 * @return  Boolean true if the type of access specified by rw is permitted;
 *          Otherwise boolean false.
 */
int kernacc(const void * addr, int len, int rw);

int useracc(const void * addr, size_t len, int rw);
int useracc_proc(const void * addr, size_t len, struct proc_info * proc, int rw);

#endif /* KERNEL_INTERNAL */
#endif /* _VM_VM_H */

/**
 *******************************************************************************
 * @file    exec.c
 * @author  Olli Vanhoja
 * @brief   Execute a file.
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

#define KERNEL_INTERNAL 1
#include <errno.h>
#include <kmalloc.h>
#include <proc.h>
#include <exec.h>

SET_DECLARE(exec_loader, struct exec_loadfn);

/**
 * Execute a file.
 * File can be elf binary, she-bang file, etc.
 * @param file  is the executable file.
 * @param argc  is the argument count.
 * @param argv  contains the argument strings.
 * @param env   contains environmen variables.
 */
int exec_file(file_t * file, char ** argv, char ** env)
{
    struct exec_loadfn ** loader;
    void * old_argv = 0;
    int err, retval = 0;
    uintptr_t vaddr;

    if (!file)
        return -ENOENT;

    /* Set argv */
    old_argv = (void *)curproc->argv;
    curproc->argv = argv;

    SET_FOREACH(loader, exec_loader) {
        err = (*loader)->fn(file, &vaddr);
        if (err == 0 || err != -ENOEXEC)
            break;
    }
    if (err) {
        retval = err;
        goto fail;
    }

    kfree(curproc->env);
    curproc->env = env;

    goto out;
fail:
    curproc->argv = old_argv;
out:
    kfree(old_argv);

    return retval;
}
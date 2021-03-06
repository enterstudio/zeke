/**
 *******************************************************************************
 * @file    procfs.h
 * @author  Olli Vanhoja
 * @brief   Process file system headers.
 * @section LICENSE
 * Copyright (c) 2014 - 2017 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

/**
 * @addtogroup fs
 * @{
 */

/**
 * @addtogroup procfs
 * @{
 */

#pragma once
#ifndef FS_PROCFS_H
#define FS_PROCFS_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/linker_set.h>
#include <fs/fs.h>

#define PROCFS_FSNAME           "procfs"    /*!< Name of the fs. */

#define PROCFS_PERMS            0400        /*!< Default file permissions of
                                             *   an procfs entry.
                                             */

struct procfs_file;

/**
 * Procfs read file function.
 * One per file type.
 * @param[in] spec is the procfs specinfo for the file.
 */
typedef struct procfs_stream * procfs_readfn_t(const struct procfs_file * spec);

typedef ssize_t procfs_writefn_t(const struct procfs_file * spec,
                                 struct procfs_stream * stream,
                                 const uint8_t * buf, size_t bufsize);

typedef void procfs_relefn_t(struct procfs_stream * stream);

/**
 * Procfs specinfo descriptor.
 */
struct procfs_file {
    const char * filename;
    procfs_readfn_t * const readfn;
    procfs_writefn_t * const writefn;
    procfs_relefn_t * const relefn;
    /* dynamic */
    vnode_t * vnode;            /*!< Pointer back to the vnode. */
    void * opt;
    pid_t pid;                  /*!< PID of the process this file is
                                 *   representing. */
};

struct procfs_stream {
    ssize_t bytes;
    char buf[0];
};

#endif /* FS_PROCFS_H */

/**
 * @}
 */

/**
 * @}
 */

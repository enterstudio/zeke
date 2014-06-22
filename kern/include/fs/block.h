/**
 *******************************************************************************
 * @file    block.h
 * @author  Olli Vanhoja
 * @brief   Block device interface headers.
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

#pragma once
#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <fs/fs.h>

#define BDEV_FLAGS_MB_READ      0x01 /*!< Supports multiple block read. */
#define BDEV_FLAGS_MB_WRITE     0x02 /*!< Supports multiple block write. */
#define BDEV_FLAGS_WR_BT_MASK   0x04 /*!< 0 = Write-back; 1 = Write-through */

struct block_dev {
    dev_t dev_id;
    char * drv_name;
    char dev_name[20];

    /*!< Configuration flags for block device handling */
    uint32_t flags;

    size_t block_size;
    ssize_t num_blocks;

    int (*read)(struct block_dev * bdev, off_t offset,
            uint8_t * buf, size_t count);
    int (*write)(struct block_dev * bdev, off_t offset,
            uint8_t * buf, size_t count);
};

int block_read(vnode_t * vnode, const off_t * offset,
        void * vbuf, size_t count);
int block_write(vnode_t * file, const off_t * offset,
        const void * vbuf, size_t count);

#endif /* BLOCK_H */
/**
 *******************************************************************************
 * @file    dehtable.h
 * @author  Olli Vanhoja
 * @brief   Directory Entry Hashtable.
 * @section LICENSE
 * Copyright (c) 2013 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

/** @addtogroup fs
  * @{
  */

#pragma once
#ifndef DEHTABLE_H
#define DEHTABLE_H

#include <fs/fs.h>

#define DEHTABLE_SIZE 16

/**
 * Directory entry.
 */
typedef struct dh_dirent {
    ino_t dh_ino; /*!< File serial number. */
    size_t dh_size; /*!< Size of this directory entry. */
    uint8_t dh_link; /*!< Internal link tag. If 0 chain ends here; otherwise
                      * chain linking continues. */
    char dh_name[1]; /*!< Name of the entry. */
} dh_dirent_t;

/**
 * Directory entry hash table array type.
 */
typedef dh_dirent_t * dh_table_t[DEHTABLE_SIZE];

/**
 * Directory iterator.
 */
typedef struct dh_dir_iter {
    dh_table_t * dir;
    size_t dea_ind;
    size_t ch_ind;
} dh_dir_iter_t;

int dh_link(dh_table_t * dir, vnode_t * vnode, const char * name, size_t name_len);
void dh_destroy_all(dh_table_t * dir);
int dh_lookup(dh_table_t * dir, const char * name, size_t name_len,
                ino_t * vnode_num);
dh_dir_iter_t dh_get_iter(dh_table_t * dir);
dh_dirent_t * dh_iter_next(dh_dir_iter_t * it);

#endif /* DEHTABLE_H */

/**
  * @}
  */

/**
 *******************************************************************************
 * @file    dynmem.h
 * @author  Olli Vanhoja
 * @brief   Dynmem management headers.
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

#pragma once
#ifndef DYNMEM_H
#define DYNMEM_H
#include <hal/mmu.h>

/* TODO
 * - Manage memmap arrays
 * - Reference counting for dynmem allocations & mappings
 * - Offer high level memory mapping & allocation functions
 * - Macros to incr/decr ref count on dynamic memory region
 */

/**
 * Dynmem area starts
 * TODO check if this is ok?
 */
#define DYNMEM_START    0x00020000

/**
 * Dynmem area end
 * TODO should match end of physical memory at least
 * (or higher if paging is allowed later)
 */
#define DYNMEM_END      0x00050000

/**
 * Dynmemmap size.
 * Dynmem memory space in 4kB blocks.
 */
#define DYNMEM_MAPSIZE  ((DYNMEM_END-DYNMEM_START)/1024/4096)

/**
 * Dynmem region definition used for allocating a region.
 */
typedef struct {
    uint32_t size;      /*!< region size in 4kB blocks. */
    uint32_t ap;        /*!< access permission. */
    uint32_t control;   /*!< control settings. */
} mmu_dregion_def;

extern uint32_t dynmemmap[];

/* TODO */
void * dynmem_alloc_region(mmu_dregion_def refdef);
void dynmem_free_region(void * address);
void dynmem_free_region_r(mmu_region_t * region);
mmu_region_t dynmem_get_region(void * p);

#endif /* DYNMEM_H */

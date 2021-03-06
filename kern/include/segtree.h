/**
 *******************************************************************************
 * @file    segtree.c
 * @author  Olli Vanhoja
 * @brief   Generic min/max segment tree.
 * @section LICENSE
 * Copyright (c) 2015 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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
 * @addtogroup libkern
 * @{
 */

/**
 * @addtogroup segtree
 * @{
 */

#pragma once
#ifndef SEGTREE_H
#define SEGTREE_H

#include <stddef.h>

/**
 * Compares two given objects and returns either of them.
 * This function shall implement min/max style comparison.
 * The function should return a or b even if one or both of them are NULL.
 * @param a first object.
 * @param b second object.
 * @return a or b.
 */
typedef void * (*segtcmp_t)(void * a, void * b);

struct segt {
    segtcmp_t cmp;
    size_t n;
    void * arr[0];
};

/**
 * Allocate and initialize a new segment tree.
 * @param n is the size of the new tree.
 * @param cmp is the compare function.
 */
struct segt * segt_init(size_t n, segtcmp_t cmp);

/**
 * Free a segment tree.
 * @note Other resources shall be freed elsewhere, this only frees what
 * segt_init() has allocated.
 * @param s is a pointer to the segment tree.
 */
void segt_free(struct segt * s);

/**
 * Alter a segment tree.
 * @param s is a pointer to the segment tree.
 * @param k is the index.
 * @param x is the new object.
 */
void segt_alt(struct segt * s, size_t k, void * x);

/**
 * Find a value between indices a and b.
 * @param s is a pointer to the segment tree.
 * @param a is the lower bound.
 * @param b is the upper bound.
 */
void * segt_find(struct segt * s, size_t a, size_t b);

#endif /* SEGTREE_H */

/**
 * @}
 */

/**
 * @}
 */

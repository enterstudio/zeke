/**
 *******************************************************************************
 * @file    tty.c
 * @author  Olli Vanhoja
 * @brief   Generic tty.
 * @section LICENSE
 * Copyright (c) 2013 - 2015 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

struct termios;

struct tty {
    struct termios conf;

    void * opt_data;

    void (* setconf)(struct termios * conf);

    ssize_t (*read)(struct tty * tty, off_t blkno, uint8_t * buf,
                    size_t bcount, int oflags);

    ssize_t (*write)(struct tty * tty, off_t blkno, uint8_t * buf,
                     size_t bcount, int oflags);

    /**
     * Overriding ioctl.
     * @note Can be NULL.
     */
    int (*ioctl)(struct dev_info * devnfo, uint32_t request,
                 void * arg, size_t arg_len);
};

/**
 * @param drv_name should be allocated from heap, refereced.
 * @param dev_name device name, copied.
 */
int make_ttydev(struct tty * tty, const char * drv_name, dev_t dev_id,
                const char * dev_name);

#endif /* TTY_H */
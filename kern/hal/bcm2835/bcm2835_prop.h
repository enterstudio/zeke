/**
 *******************************************************************************
 * @file    bcm2835_prop.c
 * @author  Olli Vanhoja
 * @brief   BCM2835 Property interface.
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

#pragma once
#ifndef BCM2835_PROP_H
#define BCM2835_PROP_H

#include <stdint.h>

#define BCM2835_PROP_REQUEST 0x0
#define BCM2835_PROP_TAG_END 0x0

/* VideoCore & HW */
#define BCM2835_PROP_TAG_GET_FIRMWARE           0x00000001
#define BCM2835_PROP_TAG_GET_GET_BOARD_MODEL    0x00010001
#define BCM2835_PROP_TAG_GET_BOARD_REVISION     0x00010002
#define BCM2835_PROP_TAG_GET_MAC_ADDRESS        0x00010003
#define BCM2835_PROP_TAG_GET_BOARD_SERIAL       0x00010004
#define BCM2835_PROP_TAG_GET_ARM_MEMORY         0x00010005
#define BCM2835_PROP_TAG_GET_VC_MEMORY          0x00010006
#define BCM2835_PROP_TAG_GET_CLOCKS             0x00010007

/**
 * Make a property request to BCM2835 VC.
 * @param request is a regularly formatted property request but it doesn't
 *                have to be in any special memory region as the subsystem
 *                will handle that part.
 * @return Returns 0 if succeed and copies response data to request;
 *         Otherwise returns an error code, a negative errno value.
 */
int bcm2835_prop_request(uint32_t * request);

#endif /* BCM2835_PROP_H */

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "specialize.h"
#include "softfloat.h"

unsigned int sting_get_hid() {
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], hartid\n" : [ret] "=r" (ret) : : );
    return ret;
}

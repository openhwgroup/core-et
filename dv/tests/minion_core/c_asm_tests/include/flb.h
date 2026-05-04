/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __FLB_H
#define __FLB_H

#define WAIT_FLB(threads, barrier, result) do \
{ \
    const uint64_t val = ((threads - 1U) << 5U) + barrier; \
    asm volatile ("csrrw %0, flb0, %1" : "=r" (result) : "r" (val)); \
} while (0)

#endif

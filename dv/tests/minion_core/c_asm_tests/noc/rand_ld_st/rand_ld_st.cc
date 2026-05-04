/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 100
#include "macros.h"
#include "minion.h"
#include <inttypes.h>

#define BUF_SIZE 256

#ifdef __cplusplus
extern "C"
#endif

int main()
{

    volatile uint64_t buf[BUF_SIZE];
    uint64_t data = 0;
    int i;
    int idx = (get_hart_id() << 16);

    C_TEST_START;

    for ( i = 0 ; i < ITERS; i++) {
        buf[idx] = data;
        data = buf[idx];
        /*
        __asm__ __volatile__
            (
             "addi t0, %[buff], 0\n"
             "li t1, 0x0000ffffffffffc0\n"
             "and t0, t0, t1\n"
             "li t1, 0x6800000000000001\n"
             "add t0, t0, t1\n"
             "addi t1, zero, 1\n"
             "csrrw t1, 0x81f, t0\n"
             "loop: bne zero, t1, loop\n"
             :
             : [buff] "r" (buf)
             : "t0", "t1"
            );*/

    }

    C_TEST_PASS;
    return 0;
}

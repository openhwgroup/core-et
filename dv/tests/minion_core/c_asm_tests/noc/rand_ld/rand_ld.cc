/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 10
#include "macros.h"
#include "minion.h"
#include <inttypes.h>

#define BUF_SIZE (1 << 20)
#define BUF_SLOT BUF_SIZE/256

#ifdef __cplusplus
extern "C"
#endif


int main()
{
   volatile uint64_t buf[BUF_SIZE];
    C_TEST_START;

    __asm__ __volatile__ (
            "csrr a5, mhartid\n"
            "slli a5, a5, 3\n"
            "add a5, a5, %[buf]\n"
            "li a3, 0x0200\n"
            "li a2, 3\n"
            "loop1: ld a4, 0(a5)\n"
            "addi a2, a2, -1\n"
            "addi a5, a5, 0x200\n"
            "bne zero, a2, loop1\n"
            :
            : [buf] "r" (buf)
            : "a4", "a5", "a2", "a3"
    );
/*
    //uint64_t data=get_hart_id();
    int i;
    int idx  = (get_hart_id() << 10);
    uint64_t* p=&buf[idx];

    for ( i = 0 ; i < ITERS; i++) {
        __asm__ __volatile__(
            "ld a1, 0(%[p])\n"
            :
            : [p] "r" (p)
            : "a1"
        );

        //data++;
        //buf[idx] = data;

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
           );
    }*/


    C_TEST_PASS;
    return 0;
}

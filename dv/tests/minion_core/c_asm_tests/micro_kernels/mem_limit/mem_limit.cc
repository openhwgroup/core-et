/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <cstdio>
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "macros.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) void mtrap_handler();

volatile int mtrap_hits=0;
volatile int err=0;
           
int main()
{
    C_TEST_START;
    uint64_t hart_id = getHartId();
    
    if (hart_id == 0) {
        // configure machine trap vectors
        __asm__ __volatile__ 
            (
             "la t0, mtrap_handler\n"
             "csrw mtvec, t0\n"
             : : : "t0"
             );


        __asm__ __volatile__ (
                              "fxor.pi f0, f0, f0\n"
                              "li      x4, 0x8c00002000\n"
                              "csrrw   x0, 0x87f, x4\n"
                              );
    }
    C_TEST_PASS;
}

//void __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) mtrap_handler() {
void mtrap_handler() {
    uint64_t mcause;

    __asm__ __volatile__
        (
         "csrr %[c], mcause\n"
         : [c] "=r" (mcause)
         );

    if (mcause != 29) {
        C_TEST_FAIL;
    }
}


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

int main()
{
    volatile char __attribute__((aligned(4096))) ptr[4096];
    long long int scp = 0x80000000;
    // Init marker
    C_TEST_START;

    // Test
    __asm__ __volatile__ (
        // Init and set all cachelines to modified
        "addi a1, %[ptr], 0\n"
        "addi a2, zero, 0\n"
        "addi a3, zero, 1024\n"
        "loop_sw:\n"
        "sw   a2, 0(a1)\n"          // Store
        "addi a2, a2, 1\n"          // Inc value
        "addi a1, a1, 4\n"          // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_sw\n"

        // Lock set/ways
        "li   a1, 0\n"        // Lock S/W
        "add  a1, a1, %[scp]\n"
        "addi a3, zero, 16\n"
        "loop_lock:\n"
        "csrw 0x51f, a1\n"          // Lock S/W
        "addi a1, a1, 64\n"         // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_lock\n"

        // Does some operations over scratchpad
        "add  a1, zero, %[scp]\n"
        "add  a2, zero, 0xbe\n"
        "sw   a2, 0(a1)\n"
        "lw   a2, 0(a1)\n"

        // Read all cachelines again
        "addi a1, %[ptr], 0\n"
        "addi a3, zero, 1024\n"
        "loop_lw:\n"
        "lw   a2, 0(a1)\n"          // Load
        "addi a1, a1, 4\n"          // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_lw\n"
      : 
      : [ptr] "r" (ptr), [scp] "r" (scp)
      : "a1", "a2", "a3", "a4"
    );

    // End marker
    C_TEST_PASS;
    return 0;
}

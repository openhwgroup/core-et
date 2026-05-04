/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

int main()
{
    volatile char __attribute__((aligned(4096))) ptr[4096];
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

        // Evict all cachelines
	"li   a1, 0x0400000000000000\n"
	"add  a1, a1, t0\n"
	"ori  a1, a1, 0xF\n" // all the lines
	"li   t6, 64\n" // line stride
	"csrw 0x89F, a1\n"          // Evict CL
	
	
        // Read all cachelines again
        "addi a1, %[ptr], 0\n"
        "addi a3, zero, 1024\n"
        "loop_lw:\n"
        "lw   a2, 0(a1)\n"          // Load
        "addi a1, a1, 4\n"          // Inc ptr
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_lw\n"
      : 
      : [ptr] "r" (ptr)
      : "a1", "a2", "a3", "a4"
    );

    // End marker
    C_TEST_PASS;
    return 0;
}

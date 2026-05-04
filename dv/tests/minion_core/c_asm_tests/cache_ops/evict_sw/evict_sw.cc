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
        "fence\n"

        // Evict all cachelines
        /*"li   a1, 0x3400000000000000\n" // Evict SW
        "li   a2, 0x1000000000000\n"    // 1 way advance
        "li   a4, 0x4000000000000\n"    // Clear way
        "addi a3, zero, 16\n"       // 16 sets
        "loop_esw:\n"
        "csrw 0x51f, a1\n"          // Evict SW (way 0)
        "add  a1, a1, a2\n"
        "csrw 0x51f, a1\n"          // Evict SW (way 1)
        "add  a1, a1, a2\n"
        "csrw 0x51f, a1\n"          // Evict SW (way 2)
        "add  a1, a1, a2\n"
        "csrw 0x51f, a1\n"          // Evict SW (way 3)
        "add  a1, a1, 64\n"         // Move to next set
        "xor  a1, a1, a4\n"         // Move to way 0
        "addi a3, a3, -1\n"         // Inc counter
        "bne  a3, zero, loop_esw\n"*/

	"li t0, 0x040000000000000F\n"
	"li t1, 0x4000\n"	
	"csrw 0x7f9, t1\n" // evict 16 sets way 0
	"add t0, t0, t1\n"
	"csrw 0x7f9, t1\n" // evict 16 sets way 1
	"add t0, t0, t1\n"
	"csrw 0x7f9, t1\n" // evict 16 sets way 2
	"add t0, t0, t1\n"
	"csrw 0x7f9, t1\n" // evict 16 sets way 3
	

        // Read all cachelines again
        "fence\n"
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

    C_TEST_PASS;
    return 0;
}

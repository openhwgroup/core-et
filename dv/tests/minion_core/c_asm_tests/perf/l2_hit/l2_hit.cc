/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   volatile char data[1024 * 64]; //NOTE: increase stack size to get  this test to work

   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi x1, %[data], 0\n"
         "addi x2, zero, 1024\n"
         "add  x2, x2, x2\n"    // x2 -> 2048 offset
         "add  x3, x1, zero\n"  // x3 -> last
         "add  x4, x3, x2\n"    // x4 -> next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x4, 0x0(x3)\n"
         "add  x3, x4, zero\n"  // last = next
         "add  x4, x4, x2\n"    // next = new next
         "sd   x1, 0x0(x3)\n"
         "addi x3, zero, 32\n"
         "loop:\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "ld   x1, 0x0(x1)\n"
         "add  x3, x3, -1\n"
         "bne  x3, zero, loop\n"
         : 
         : [data] "r" (data)
         : "x1", "x3", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"
            );

   C_TEST_PASS;
   return 0;
}

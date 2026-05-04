/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   volatile char data[256 * 64];
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi a1, %[data], 0\n"
         "addi a2, zero, 32\n"
         "loop:\n"
         "ld   s0, 0x0(a1)\n"
         "ld   s1, 0x40(a1)\n"
         "ld   s2, 0x80(a1)\n"
         "ld   s3, 0xc0(a1)\n"
         "ld   s4, 0x100(a1)\n"
         "ld   s5, 0x140(a1)\n"
         "ld   s6, 0x180(a1)\n"
         "ld   s7, 0x1c0(a1)\n"
         "add  a2, a2, -1\n"
         "add  a1, a1, 0x200\n"
         "bne  a2, zero, loop\n"
         : 
         : [data] "r" (data)
         : "a1", "a2", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"
         );

   C_TEST_PASS;
   return 0;
}

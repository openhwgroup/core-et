/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   volatile long long int data[128];
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi a1, %[data], 0\n"
         "addi a2, zero, 256\n"
         "addi a3, zero, 12\n"
         "addi a4, zero, 14\n"
         "addi a5, zero, 53\n"
         "addi a6, zero, 103\n"
         "loop:\n"
         "sb   a3, 0(a1)\n"
         "lw   a7, 0(a1)\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "sb   a4, 0(a1)\n"
         "add  zero, zero, zero\n"
         "lw   a7, 0(a1)\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "sb   a5, 0(a1)\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "lw   a7, 0(a1)\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "sb   a6, 0(a1)\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "add  zero, zero, zero\n"
         "lw   a7, 0(a1)\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop\n"
         : 
         : [data] "r" (data)
         : "a1", "a2"
            );

   C_TEST_PASS;
   return 0;
}

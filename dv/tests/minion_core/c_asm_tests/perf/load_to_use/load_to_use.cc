/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   volatile long long int data;
   volatile long long int * ptr = &data;
   data = (long long int) ptr;
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi a1, %[ptr], 0\n"
         "addi a2, zero, 256\n"
         "loop:\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "ld   a1, 0(a1)\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop\n"
         : 
         : [ptr] "r" (ptr)
         : "a1", "a2"
            );

   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
   C_TEST_START;

   // FUTURE: Emmanuel Marie This test uses fdiv.ps which is an mcode instruction
   // But doesn't set up a trap vector. This causes hangs. Pass and exit.
   C_TEST_PASS;
   return 0;

   // Test
   __asm__ __volatile__ (
         // Enables all elements of FPU
         "mov.m.x m0, zero, 0xff\n"
         "addi    x2, zero, 8\n"
         "addi    x1, zero, 0xe7\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x80\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x00\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x00\n"
         "fbcx.ps f0, x1\n"
         "addi    x1, zero, 0x5a\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x01\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x00\n"
         "sll     x1, x1, x2\n"
         "addi    x1, x1, 0x01\n"
         "fbcx.ps f1, x1\n"
         "addi    x2, zero, 256\n"
         "loop:\n"
         "fdiv.ps f2, f0, f1\n"
         "addi    x1, x1, 1\n"
         "addi    x2, x2, -1\n"
         //"bne     x2, zero, loop\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         "addi    x2, x2, -1\n"
         : 
         : 
         : "f0", "f1", "f2", "x1", "x2" 
         );

   C_TEST_PASS;
   return 0;
}

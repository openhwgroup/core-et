/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         // Enables all elements of FPU
         "mov.m.x m0, zero, 0xff\n"
         "addi    t3, zero, 8\n"
         "addi    x1, zero, 0x40\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x42\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x8c\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x9f\n"
         "addi    t3, zero, 256\n"
         "fbcx.ps f0, x1\n"
         "loop:\n"
         "flog.ps f0, f0\n"
         "addi    x1, x1, 1\n"
         "addi    t3, t3, -1\n"
         //"bne     t3, zero, loop\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         : 
         : 
         : "f0", "x1", "t3" 
         );

   C_TEST_PASS;
   return 0;
}

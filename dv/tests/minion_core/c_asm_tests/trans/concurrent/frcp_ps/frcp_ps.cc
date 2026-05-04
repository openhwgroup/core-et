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
         "li x1, 0xbe737870\n"
         "fbcx.ps f0, x1\n"
         "li x1, 0xDEADBEEF\n"
         "fbcx.ps f2, x1\n"
         "li x1, 0x34071500\n"
         "fbcx.ps f3, x1\n"
         "li x1, 0x34900005\n"
         "fbcx.ps f4, x1\n"
         "li x1, 0xDEADBEEF\n"
         "fbcx.ps f5, x1\n"
         "loop:\n"
         //RTLMIN-5479"fsin.ps f0, f0\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
         "frcp.ps f1, f2\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
         "fexp.ps f1, f0\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
         "frcp.ps f5, f3\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
         "flog.ps f0, f1\n"
         //RTLMIN-5479"frsq.ps f3, f3\n"
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

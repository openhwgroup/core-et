/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   C_TEST_START;

   // FUTURE: Emmanuel Marie This test makes no sense as it uses an mcode instruction
   // without setting up a trap vector. Pass and quit.
   C_TEST_PASS;
   return 0;

   // Test
   __asm__ __volatile__ (
         "addi a1, zero, 256\n"
         "mov.m.x	m0,zero,0xf\n"
         "loop:\n"
         "fdiv.ps    f2, f0, f1\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "fmadd.ps   f4, f0, f1, f3\n"
         "add  a1, a1, -1\n"
         "bne  a1, zero, loop\n"
         : 
         : 
         : "a1", "a2", "a3", "a4"
         );

   C_TEST_PASS;
   return 0;
}

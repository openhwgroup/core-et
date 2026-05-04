/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi a1, zero, 256\n"
         "loop:\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "fmadd.ps   f0, f1, f2, f0\n"
         "add  a1, a1, -1\n"
         "bne  a1, zero, loop\n"
         : 
         : 
         : "a1", "a2", "a3", "a4"
         );

   C_TEST_PASS;
   return 0;
}

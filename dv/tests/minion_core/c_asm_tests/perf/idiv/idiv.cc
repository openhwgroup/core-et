/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "addi a1, zero, 13\n"
         "addi a2, zero, 1\n"
         "addi a3, zero, 256\n"
         "addi a4, zero, 0\n"
         "loop:\n"
         "div  a1, a1, a2\n"
         "add  a4, a1, a1\n"
         "add  a3, a3, -1\n"
         "bne  a3, zero, loop\n"
         : 
         : 
         : "a1", "a2", "a3", "a4"
         );

   C_TEST_PASS;
   return 0;
}

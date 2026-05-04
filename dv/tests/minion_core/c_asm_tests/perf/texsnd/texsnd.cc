/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   C_TEST_START;

   // Test
   __asm__ __volatile__ (
         "mov.m.x m0, zero, 0xff\n"
         "addi a1, zero, 0xde\n"
         "addi a2, zero, 0xad\n"
         "fbci.ps f0, 0xca\n"
         "fbci.ps f1, 0xbe\n"
         // "texsndh a1, a2\n" # FUTURE: Emmanuel Marie unknown instruction
         // "texsnds f0\n" # FUTURE: Emmanuel Marie unknown instruction
         // "texsndt f1\n" # FUTURE: Emmanuel Marie unknown instruction
         // "texrcv  f0, 0x3\n" # FUTURE: Emmanuel Marie unknown instruction
         : 
         : 
         : "a1", "a2", "a3", "a4"
         );

   C_TEST_PASS;
   return 0;
}

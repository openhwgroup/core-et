/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   volatile uint32_t operands[16] __attribute__ ((aligned (4096))) = {

      0x00800001,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,

      0xff7ffffe,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000
   };
     C_TEST_START;
   __asm__ __volatile__ (
         "li x2, 0xffffffffffffffff\n"
         "mova.m.x x2\n"
         "flw.ps f0,  0 (%[operands])\n"
         "flw.ps f1, 32 (%[operands])\n"
         "fmul.ps f2, f0, f1\n"                       
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2"
         );
   C_TEST_PASS;
   return 0;
}

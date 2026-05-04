/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <cstdint>
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

   volatile uint32_t operands[16] __attribute__ ((aligned (4096))) = {
      0x1980007f,
      0x00000000,
      0x00000000,
      0x00000000,
      0xb0000000,
      0x00000000,
      0x00000000,
      0x00000000,

      0x0e7fffff,
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
         "li x3, 0xffffffffffffffff\n"
         "mova.m.x x3\n"
         "flw.ps f0,0(%[operands])\n"
         "flw.ps f1,32(%[operands])\n"
         "FADD.PS f31, f0, f1\n" 


         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
         );
   C_TEST_PASS;
   return 0;
}

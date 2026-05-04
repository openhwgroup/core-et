/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   volatile uint32_t operands[16] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x4b8c4b40,                      
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
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f0,  0 (%[operands])\n"

         "fmsub.s f1, f0, f0, f0, rdn\n"
         "fmsub.s f2, f0, f0, f0, rne\n"
         "fmsub.s f3, f0, f0, f0, rtz\n"
         "fmsub.s f4, f0, f0, f0, rup\n"
         "fmsub.s f5, f0, f0, f0, rmm\n"

         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3"
         );
   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   volatile uint32_t operands[32] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x3dcccccd,
      0x3e4ccccd,
      0x3e99999a,
      0x3ecccccd,
      0x3f000000,
      0x3f19999a,
      0x3f333333,
      0x3f4ccccd
   };
   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f0,  0 (%[operands])\n"
         //"ffrc.ps f1, f0\n"                       
         ////RTLMIN-5479"fsin.ps f8, f1\n"                       
         //RTLMIN-5479"fsin.ps f8, f0\n"                       
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f8"
         );
   C_TEST_PASS;
   return 0;
}

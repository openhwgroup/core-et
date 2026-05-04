/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"


int main() {

   // Test
   volatile uint32_t operands[8]  __attribute__ ((aligned (4096)))= {
      0x33333333,
      0x33333333,
      0x33333333,
      0x33333333,
      0x33333333,
      0x33333333,
      0x33333333,
      0x33333333
   };

   C_TEST_START;
   __asm__ __volatile__ (

         //enable lanes
         "li x4, 0xff\n"
         "mova.m.x x4\n"

         //load operands
        "flw.ps f0,0(%[operands])\n"

         "fcvt.f16.ps f16,f0\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x4", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
            );	

   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>


int main()
{

   C_TEST_START;

   // Test
   static uint32_t operands[88]  __attribute__ ((aligned (16)))= {
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero

      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero

      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      0x0,  // zero

      //-----------------------
      0x0,  // zero
      0x80000000,  // -zero
      0x80000000,   // -zero
      0x0,  // zero

      0x80000000,   // -zero
      0x80000000,  // -zero
      0x0,  // zero
      0x0   // zero

         //-----------------------         


   };

   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 32(%[operands])\n"    
         "flw.ps f2, 64(%[operands])\n"

         "fnmsub.ps f4,f0,f1,f2\n"
         "fnmadd.ps f5,f0,f1,f2\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
         );


   C_TEST_PASS;
   return 0;
}

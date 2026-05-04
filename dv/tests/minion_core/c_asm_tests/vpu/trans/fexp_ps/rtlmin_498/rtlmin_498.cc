/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"


int main() {

   // Test
   volatile uint32_t operands[16]  __attribute__ ((aligned (4096)))= {
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      //-----------------------                
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff // -1.1754942E-38
   };

   C_TEST_START;
   __asm__ __volatile__ (

         //enable lanes
         "li x4, 0xff\n"
         "mova.m.x x4\n"

         //load operands
        "flw.ps f0,0(%[operands])\n"
	"flw.ps f1,32(%[operands])\n"

         "fexp.ps f16,f0\n"
         "fexp.ps f16,f1\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x4", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
            );	

   C_TEST_PASS;
   return 0;
}

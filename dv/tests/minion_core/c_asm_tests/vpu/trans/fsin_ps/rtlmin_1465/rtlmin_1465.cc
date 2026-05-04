/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>



int main()
{
   //   volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   // Test


   static uint32_t operands[24]  __attribute__ ((aligned (16)))= {

      //-----------------------
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN  
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      //-----------------------                
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf

      //------ denormal -------
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff // -1.1754942E-38


   };


   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 32(%[operands])\n"    
         "flw.ps f2, 64(%[operands])\n"


         // Instruction fsin
         //RTLMIN-5479"fsin.ps f4,f0\n"
         //RTLMIN-5479"fsin.ps f5,f1\n"
         //RTLMIN-5479"fsin.ps f6,f2\n"


         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );

   C_TEST_PASS;
   return 0;
}

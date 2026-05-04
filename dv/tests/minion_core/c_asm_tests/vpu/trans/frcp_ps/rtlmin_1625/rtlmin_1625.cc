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

   static uint32_t operands[32]  __attribute__ ((aligned (16)))= {

      //----- denormal --------
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff, // -1.1754942E-38

      //------- NaNs ----------
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7ff00000, // NaN                  
      0xfff00000, // -NaN  
      0x7ff00f01, // NaN                  
      0xfff00f01, // -NaN

      //------ +/-inf ---------
      0x7f800000, // inf
      0xff800000, // -inf
      0x7e800000, // +8.5070592e37
      0xfe800000, // -8.5070592e37
      0x7e800001, // +8.50706e37
      0xfe800001, // -8.50706e37
      0x7e967699, // +1.0e38
      0xfe967699, // -1.0e38


      //------- -F -----------
      0x0,  // zero
      0x80000000,  // -zero
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1 
      0xbfc00000, // -1.5
      0xbf028f5c, // -0.51
      0xcb000000, // -8388608.0
      0xcb8c4b40 // -18388608.0 


   };


   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 32(%[operands])\n"    
         "flw.ps f2, 64(%[operands])\n"
         "flw.ps f3, 96(%[operands])\n"



         // Instruction - mixing src with dst
         "frcp.ps f4,f0\n"
         "frcp.ps f5,f1\n"
         "frcp.ps f6,f2\n"
         "frcp.ps f7,f3\n"


         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );
   C_TEST_PASS;
   return 0;
}

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
   static uint32_t operands[24]  __attribute__ ((aligned (16)))= {

      0x0,  // zero
      0x80000000,  // -zero
      0x3f800000,  // +1.0
      0xbf800000,  // -1.0              
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  

      //-----------------------                
      0x0,  // zero
      0x80000000,  // -zero
      0x3f800000,  // +1.0
      0xbf800000,  // -1.0
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  

      //-----------------------                
      0x0,  // zero
      0x80000000,  // -zero
      0x3f800000,  // +1.0
      0xbf800000,  // -1.0
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  


   };

   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"    // +0.0
         "flw.ps f1, 8(%[operands])\n"    // +1.0
         "flw.ps f2, 16(%[operands])\n"    // qnan
         "flw.ps f3, 20(%[operands])\n"   // -qnan

         //load operands
         "fnmsub.s f4, f1, f0, f3\n"   
         "fnmadd.s f5, f1, f0, f3\n"  
         "fnmsub.s f6, f1, f0, f2\n"  
         "fnmadd.s f7, f1, f0, f2\n"  
         "fmsub.s f8, f1, f0, f3\n"  
         "fmadd.s f9, f1, f0, f3\n"  
         "fmsub.s f10, f1, f0, f3\n"  
         "fmadd.s f11, f1, f0, f3\n"  

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11"
            );




   C_TEST_PASS;
   return 0;
}

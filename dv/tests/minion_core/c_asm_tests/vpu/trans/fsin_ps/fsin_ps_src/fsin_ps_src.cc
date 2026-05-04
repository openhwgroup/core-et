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

   /* static uint32_t operands[32]  __attribute__ ((aligned (16)))= {
      0x3e800000, // 1.0 (f) // 0.25 e
      0x3f028f5c, // 0.51
      0x3f800000, // 1.0
      0x3f028f5c, // 0.51  
      0x3fa00000, // 1.5 0x40000000 //1.25
      0x3fc7ae14, // 1.56
      0x40000000, // 1.5
      0x3fc7ae14, // 1.56
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN    
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000   // -zero
      };*/
   static uint32_t operands[88]  __attribute__ ((aligned (16)))= {
      0x3f800000, // 1.0 
      0x3f028f5c, // 0.51
      0x3f800000, // 1.0
      0x3f028f5c, // 0.51  
      0xbf800000, // -1.0 
      0xbf028f5c, // -0.51
      0xbf800000, // -1.0
      0xbf028f5c, // -0.51  
      //-----------------------
      0x40000000, // 1.5 
      0x3fc7ae14, // 1.56
      0x40000000, // 1.5
      0x3fc7ae14, // 1.56
      0xbfc00000, // -1.5 
      0xbfc7ae14, // -1.56
      0xbfc00000, // -1.5
      0xbfc7ae14, // -1.56
      //-----------------------
      0x41a4cccd, // 20.6           
      0x41a0cccd, // 20.1           
      0x41a4cccd, // 20.6           
      0x41a0cccd, // 20.1   
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1  
      //-----------------------         
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      //-----------------------
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff, // -1.1754942E-38
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
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------                
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------                
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
      0x007fffff, // -1.1754942E-38
      //----------------------- 
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000   // -zero

   };


   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 16(%[operands])\n"    
         "flw.ps f2, 32(%[operands])\n"
         "flw.ps f3, 48(%[operands])\n"
         "flw.ps f4, 64(%[operands])\n"
         "flw.ps f5, 80(%[operands])\n"        
         "flw.ps f6, 96(%[operands])\n"  
         "flw.ps f7, 0(%[operands])\n" 


         // Instruction - circular test, dst becomes src
         //RTLMIN-5479"fsin.ps f1,f0\n"
         //RTLMIN-5479"fsin.ps f2,f1\n"
         //RTLMIN-5479"fsin.ps f3,f2\n"
         //RTLMIN-5479"fsin.ps f4,f3\n"
         //RTLMIN-5479"fsin.ps f5,f4\n"
         /*//RTLMIN-5479"fsin.ps",8,7,"dyn");
           //RTLMIN-5479"fsin.ps",10,9,"dyn");
           //RTLMIN-5479"fsin.ps",11,10,"dyn");
           //RTLMIN-5479"fsin.ps",13,12,"dyn");
           //RTLMIN-5479"fsin.ps",14,13,"dyn");
           //RTLMIN-5479"fsin.ps",16,15,"dyn");
           //RTLMIN-5479"fsin.ps",17,16,"dyn");*/

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );

   C_TEST_PASS;
   return 0;
}

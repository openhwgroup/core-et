/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

#include "computational.h"


int main()
{
   C_TEST_START;

   // Test

   static uint32_t operands[80]  __attribute__ ((aligned (16)))= {
      0x3e800000, // 0.25
      0x3f028f5c, // 0.51
      0x3f800000, // 1.0
      0x3f028f5c, // 0.51  
      0x3fa00000,  //1.25
      0x3fc7ae14, // 1.56
      0x40000000, // 1.5
      0x3fc7ae14, // 1.56
      // --------------------      
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      //-----------------------   
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      // --------------------
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      // --------------------
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN  
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN 
      // --------------------
      0x00011c58, //1.02003E-40
      0x80011c58, // -1.02003E-40
      0x006f1249, //1.02003E-38
      0x806f1249, // -1.02003E-38
      0x00011c58, //1.02003E-40
      0x80011c58, // -1.02003E-40
      0x006f1249, //1.02003E-38
      0x806f1249, // -1.02003E-38
      // --------------------
      0x00011c58, //1.02003E-40 
      0x80011c58, // -1.02003E-40
      0x006f1249, //1.02003E-38
      0x806f1249, // -1.02003E-38
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      // --------------------
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      // --------------------
      0x00011c58, //1.02003E-40 
      0x80011c58, // -1.02003E-40
      0x006f1249, //1.02003E-38
      0x806f1249, // -1.02003E-38
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      // --------------------
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN   
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
         "flw.ps f7, 112(%[operands])\n" 
         "flw.ps f8, 128(%[operands])\n" 
         "flw.ps f9, 144(%[operands])\n"        
         "flw.ps f10, 160(%[operands])\n"  
         "flw.ps f11, 176(%[operands])\n" 
         "flw.ps f12, 192(%[operands])\n"
         "flw.ps f13, 208(%[operands])\n"        
         "flw.ps f14, 224(%[operands])\n"  
         "flw.ps f15, 240(%[operands])\n" 
         "flw.ps f16, 8(%[operands])\n"
         "flw.ps f17, 24(%[operands])\n"
         "flw.ps f18, 40(%[operands])\n"        
         "flw.ps f19, 56(%[operands])\n"  
         "flw.ps f20, 72(%[operands])\n" 
         "flw.ps f21, 88(%[operands])\n"
         "flw.ps f22, 104(%[operands])\n" 
         "flw.ps f23, 120(%[operands])\n"
         "flw.ps f24, 136(%[operands])\n" 
         "flw.ps f25, 152(%[operands])\n"
         "flw.ps f26, 168(%[operands])\n" 
         "flw.ps f27, 184(%[operands])\n"
         "flw.ps f28, 200(%[operands])\n"
         "flw.ps f29, 216(%[operands])\n"
         "flw.ps f30, 232(%[operands])\n"
         "flw.ps f31, 0(%[operands])\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );


   // Instruction - mixing src with dst
   __computational4("fnmsub.s",8,0,1,2);
   __computational4("fnmsub.s",5,4,7,1);
   __computational4("fnmsub.s",2,6,3,5);
   __computational4("fnmsub.s",1,7,2,7);
   __computational4("fnmsub.s",0,5,5,4);
   __computational4("fnmsub.s",7,1,6,0);
   __computational4("fnmsub.s",17,11,9,10);
   __computational4("fnmsub.s",27,20,15,18);

   C_TEST_PASS;
   return 0;
}

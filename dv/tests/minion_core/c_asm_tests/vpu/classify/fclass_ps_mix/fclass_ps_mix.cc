/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test

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
         //enable the 8 lanes
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
         // Instruction - circular test, dst becomes src
         "fclass.ps f1, f0\n"
         "fclass.ps f2, f1\n"
         "fclass.ps f2, f3\n"
         "fclass.ps f3, f3\n"
         "fclass.ps f5, f4\n"
         "fclass.ps f5, f5\n"
         "fclass.ps f5, f6\n"
         "fclass.ps f6, f7\n"
         "fclass.ps f8, f8\n"
         "fclass.ps f10, f9\n"
         "fclass.ps f9, f10\n"
         "fclass.ps f9, f9\n"
         "fclass.ps f11, f11\n"
         "fclass.ps f11, f12\n"
         "fclass.ps f7, f12\n"
         "fclass.ps f12, f12\n"
         "fclass.ps f14, f13\n"
         "fclass.ps f14, f14\n"
         "fclass.ps f16, f15\n"
         "fclass.ps f15, f16\n"
         "fclass.ps f17, f17\n"
         "fclass.ps f17, f18\n"
         "fclass.ps f18, f19\n"
         "fclass.ps f19, f19\n"
         "fclass.ps f20, f20\n"
         "fclass.ps f22, f21\n"
         "fclass.ps f22, f22\n"
         "fclass.ps f24, f23\n"
         "fclass.ps f25, f24\n"
         "fclass.ps f25, f25\n"
         "fclass.ps f26, f25\n"
         "fclass.ps f27, f26\n"
         "fclass.ps f27, f28\n"
         "fclass.ps f29, f28\n"
         "fclass.ps f29, f30\n"
         "fclass.ps f30, f30\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );	


   C_TEST_PASS;
   return 0;




}

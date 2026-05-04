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
         "li t3, 0xff\n"
         "mova.m.x t3\n"

         //load operands to all fp registers
         "flq2 f0, 0(%[operands])\n"
         "flq2 f1, 16(%[operands])\n"    
         "flq2 f2, 32(%[operands])\n"
         "flq2 f3, 48(%[operands])\n"
         "flq2 f4, 64(%[operands])\n"
         "flq2 f5, 80(%[operands])\n"        
         "flq2 f6, 96(%[operands])\n"  
         "flq2 f7, 112(%[operands])\n" 
         "flq2 f8, 128(%[operands])\n"
         "flq2 f9, 144(%[operands])\n"        
         "flq2 f10, 160(%[operands])\n"  
         "flq2 f11, 176(%[operands])\n" 
         "flq2 f12, 192(%[operands])\n"
         "flq2 f13, 208(%[operands])\n"        
         "flq2 f14, 224(%[operands])\n"  
         "flq2 f15, 240(%[operands])\n" 
         "flq2 f16, 8(%[operands])\n"
         "flq2 f17, 24(%[operands])\n"
         "flq2 f18, 40(%[operands])\n"        
         "flq2 f19, 56(%[operands])\n"  
         "flq2 f20, 72(%[operands])\n" 
         "flq2 f21, 88(%[operands])\n"
         "flq2 f22, 104(%[operands])\n" 
         "flq2 f23, 120(%[operands])\n"
         "flq2 f24, 136(%[operands])\n" 
         "flq2 f25, 152(%[operands])\n"
         "flq2 f26, 168(%[operands])\n" 
         "flq2 f27, 184(%[operands])\n"
         "flq2 f28, 200(%[operands])\n"
         "flq2 f29, 216(%[operands])\n"
         "flq2 f30, 232(%[operands])\n"
         "flq2 f31, 0(%[operands])\n"

         "fsq2 f0, 8(%[operands])\n"
         "flq2 f1, 8(%[operands])\n"    
         "flq2 f1, 32(%[operands])\n"
         "fsq2 f3, 48(%[operands])\n"
         "fsq2 f4, 48(%[operands])\n"
         "flq2 f5, 48(%[operands])\n"
         "flq2 f5, 88(%[operands])\n"
         "fsq2 f10, 168(%[operands])\n"    
         "flq2 f15, 168(%[operands])\n"
         "fsq2 f30, 168(%[operands])\n"
         "fsq2 f24, 48(%[operands])\n"
         "flq2 f24, 184(%[operands])\n"
         "fsq2 f25, 184(%[operands])\n"


         : 
         : [operands] "r" (operands)
         : "t3", "t0", "t1", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"
            );



   C_TEST_PASS;

}

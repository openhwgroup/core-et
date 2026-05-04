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
         "flw f0, 0(%[operands])\n"
         "flw f1, 16(%[operands])\n"    
         "flw f2, 32(%[operands])\n"
         "flw f3, 48(%[operands])\n"
         "flw f4, 64(%[operands])\n"
         "flw f5, 80(%[operands])\n"        
         "flw f6, 96(%[operands])\n"  
         "flw f7, 112(%[operands])\n" 
         "flw f8, 128(%[operands])\n"
         "flw f9, 144(%[operands])\n"        
         "flw f10, 160(%[operands])\n"  
         "flw f11, 176(%[operands])\n" 
         "flw f12, 192(%[operands])\n"
         "flw f13, 208(%[operands])\n"        
         "flw f14, 224(%[operands])\n"  
         "flw f15, 240(%[operands])\n" 
         "flw f16, 8(%[operands])\n"
         "flw f17, 24(%[operands])\n"
         "flw f18, 40(%[operands])\n"        
         "flw f19, 56(%[operands])\n"  
         "flw f20, 72(%[operands])\n" 
         "flw f21, 88(%[operands])\n"
         "flw f22, 104(%[operands])\n" 
         "flw f23, 120(%[operands])\n"
         "flw f24, 136(%[operands])\n" 
         "flw f25, 152(%[operands])\n"
         "flw f26, 168(%[operands])\n" 
         "flw f27, 184(%[operands])\n"
         "flw f28, 200(%[operands])\n"
         "flw f29, 216(%[operands])\n"
         "flw f30, 232(%[operands])\n"
         "flw f31, 0(%[operands])\n"

         "fsw f0, 8(%[operands])\n"
         "flw f1, 8(%[operands])\n"    
         "flw f1, 32(%[operands])\n"
         "fsw f3, 48(%[operands])\n"
         "fsw f4, 48(%[operands])\n"
         "flw f5, 48(%[operands])\n"
         "flw f5, 88(%[operands])\n"
         "fsw f10, 168(%[operands])\n"    
         "flw f15, 168(%[operands])\n"
         "fsw f30, 168(%[operands])\n"
         "fsw f24, 48(%[operands])\n"
         "flw f24, 184(%[operands])\n"
         "fsw f25, 184(%[operands])\n"


         : 
         : [operands] "r" (operands)
         : "t3", "t0", "t1", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"
            );



   C_TEST_PASS;

}

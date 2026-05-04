/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"


int main()
{

   //test
   volatile uint32_t operands[88]  __attribute__ ((aligned (4096)))= {
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

   C_TEST_START;
   __asm__ __volatile__ (
         //enable the 4 lanes
         "li x5, 0xff\n"
         "mova.m.x x5\n"
         //load operands
        "flw.ps f0,0(%[operands])\n"
	"flw.ps f1,32(%[operands])\n"
	"flw.ps f2,64(%[operands])\n"
	"flw.ps f3,96(%[operands])\n"
	"flw.ps f4,128(%[operands])\n"
	"flw.ps f5,160(%[operands])\n"
	"flw.ps f6,192(%[operands])\n"
	"flw.ps f7,224(%[operands])\n"
	"flw.ps f8,256(%[operands])\n"
	"flw.ps f9,288(%[operands])\n"
	"flw.ps f10,320(%[operands])\n"
        

         // Same destination
         "fswizz.ps f31, f0, 0x00\n"
         "fswizz.ps f31, f1, 0xf\n"
         "fswizz.ps f31, f2, 0xff\n"
         "fswizz.ps f31, f3, 0x1\n"
         "fswizz.ps f31, f4, 0x2\n"
         "fswizz.ps f31, f5, 0x1\n"
         "fswizz.ps f31, f6, 0x2\n"
         "fswizz.ps f31, f7, 0x0\n"
         "fswizz.ps f31, f8, 0x00\n"
         "fswizz.ps f31, f9, 0xf\n"
         "fswizz.ps f31, f10, 0xff\n"
         // Asm output results to C code
         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x5", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );

   C_TEST_PASS;
   return 0;


}

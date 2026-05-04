/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

 static uint32_t operands[32]  __attribute__ ((aligned (16)))= {
    0x3f800000, // 1.0 
    0x3f028f5c, // 0.51
    0x3f800000, // 1.0
    0x3f028f5c, // 0.51  
    0x40000000, // 1.5 
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

 
      //move and broadcast

    "fmvz.x.ps x0, f0, 0\n"
    "fbcx.ps f8, x0\n"
    "fmvz.x.ps x1, f1, 0\n"
    "fbcx.ps f9, x1\n"
    "fmvz.x.ps x2, f2, 0\n"
    "fbcx.ps f10, x2\n"
    "fmvz.x.ps x3, f3, 0\n"
    "fbcx.ps f11, x3\n"
    "fmvz.x.ps x4, f4, 0\n"
    "fbcx.ps f12, x4\n"
    "fmvz.x.ps x5, f5, 0\n"
    "fbcx.ps f13, x5\n"
    "fmvz.x.ps x6, f6, 0\n"
    "fbcx.ps f14, x6\n"
    "fmvz.x.ps x7, f7, 0\n"
    "fbcx.ps f15, x7\n"

  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2"
  );

  C_TEST_PASS;
  return 0;
}

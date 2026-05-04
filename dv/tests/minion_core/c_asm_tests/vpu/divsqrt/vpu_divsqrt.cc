/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{


  static uint32_t operands[32]  __attribute__ ((aligned (16)))= {
    0x3f800000, // 0.5
    0x3f028f5c, // 0.51
    0x3f800000, // 0.5
    0x3f028f5c, // 0.51
    0x40000000, // 1.5
    0x3fc7ae14, // 1.56
    0x40000000, // 1.5
    0x3fc7ae14, // 1.56      
    0x4b000000, // 8388608.0
    0x4b8c4b40, // 18388608.0
    0x4b000000, // 8388608.0
    0x4b8c4b40, // 18388608.0
    0xc1a4cccd, // 20.6          
    0xc1a0cccd, // 20.1          
    0xc1a4cccd, // 20.6          
    0xc1a0cccd, // 20.1   
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
    "flw.ps f7, 112(%[operands])\n"      
                      
    //RTLMIN-5479"fsin.ps f10, f0\n"
    "fdiv.ps f10, f0, f1\n"
    "fsqrt.ps f11, f2\n" 
    "fclass.ps f10, f0\n"
    "fdiv.s f12, f3, f2\n"
    "fcvt.pw.ps f10, f0\n"
    "fsqrt.s f10, f0\n"
    "fadd.ps f11, f0, f1\n"
    "fsub.ps f12, f0, f1\n"  
    "fdiv.ps f13, f0, f1\n"
    "fsqrt.ps f14, f2\n"
    "fdiv.s f15, f3, f2\n"
    "fsqrt.s f16, f0\n"       
    "fsqrt.ps f17, f2\n"
    "fdiv.s f18, f1, f2\n"    

    // operation with masks
    "mov.m.x mt0, zero, 6\n"    
    "fsqrt.ps f3, f1\n"
    "mov.m.x mt0, zero, 3\n"

  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18"
  );

  C_TEST_PASS;
  return 0;
}

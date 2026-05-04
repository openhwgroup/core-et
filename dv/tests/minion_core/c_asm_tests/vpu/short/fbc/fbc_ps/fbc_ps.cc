/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

 static uint32_t operands[32]  __attribute__ ((aligned (8)))= {
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
    "fbc.ps f0, 0(%[operands])\n"
    "fbc.ps f1, 8(%[operands])\n"    
    "fbc.ps f2, 16(%[operands])\n"
    "fbc.ps f3, 24(%[operands])\n"
    "fbc.ps f4, 32(%[operands])\n"
    "fbc.ps f5, 40(%[operands])\n"        
    "fbc.ps f6, 48(%[operands])\n"  
    "fbc.ps f7, 56(%[operands])\n" 
    "fbc.ps f8, 64(%[operands])\n"
    "fbc.ps f9, 72(%[operands])\n" 
    "fbc.ps f10, 80(%[operands])\n"
    "fbc.ps f11, 88(%[operands])\n"    
    "fbc.ps f12, 96(%[operands])\n"
    "fbc.ps f13, 0(%[operands])\n"
    "fbc.ps f14, 8(%[operands])\n"
    "fbc.ps f15, 16(%[operands])\n"        
    "fbc.ps f16, 24(%[operands])\n"  
    "fbc.ps f17, 32(%[operands])\n" 
    "fbc.ps f18, 40(%[operands])\n"
    "fbc.ps f19, 48(%[operands])\n" 
    "fbc.ps f20, 56(%[operands])\n"
    "fbc.ps f21, 64(%[operands])\n"    
    "fbc.ps f22, 72(%[operands])\n"
    "fbc.ps f23, 80(%[operands])\n"
    "fbc.ps f24, 88(%[operands])\n"
    "fbc.ps f25, 96(%[operands])\n"        
    "fbc.ps f26, 0(%[operands])\n"  
    "fbc.ps f27, 8(%[operands])\n" 
    "fbc.ps f28, 16(%[operands])\n"
    "fbc.ps f29, 24(%[operands])\n" 
    "fbc.ps f30, 32(%[operands])\n"
    "fbc.ps f31, 40(%[operands])\n"

    //enabled lane0-3
    "li x2, 0x0f\n"
    "mova.m.x x2\n" 

    "fbc.ps f0, 8(%[operands])\n"
    "fbc.ps f1, 16(%[operands])\n"    
    "fbc.ps f2, 24(%[operands])\n"
    "fbc.ps f3, 32(%[operands])\n"

    //enabled lane2-5
    "li x2, 0x3c\n"
    "mova.m.x x2\n" 

    "fbc.ps f10, 0(%[operands])\n"
    "fbc.ps f11, 8(%[operands])\n"    
    "fbc.ps f12, 16(%[operands])\n"
    "fbc.ps f13, 24(%[operands])\n"



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

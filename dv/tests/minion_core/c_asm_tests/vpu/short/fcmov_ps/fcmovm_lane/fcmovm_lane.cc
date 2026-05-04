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

 
      //move 

    "fcmovm.ps f8, f1, f0\n"
    "fcmovm.ps f8, f2, f1\n"
    //switching off lane 0
    "li x2, 0xfe\n"
    "mova.m.x x2\n"
	// repeat inst
    "fcmovm.ps f8, f1, f0\n"
    "fcmovm.ps f8, f2, f1\n"
/*    "fcmovm.ps f8, f3, f2\n"
    "fcmovm.ps f8, f4, f3\n"
    "fcmovm.ps f8, f5, f4\n"
    "fcmovm.ps f8, f6, f5\n"
    "fcmovm.ps f8, f7, f6\n" */


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

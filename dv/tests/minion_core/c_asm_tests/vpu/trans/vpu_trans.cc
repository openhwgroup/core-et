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
    "li x2, 0xf\n"
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
    "fexp.ps f10, f0\n"
    "flog.ps f10, f0\n"

    "ffrc.ps f10, f0\n"
    "ffrc.ps f10, f1\n"
    "ffrc.ps f10, f2\n"
    "ffrc.ps f10, f3\n"
    "ffrc.ps f10, f4\n"
    "ffrc.ps f10, f5\n"
    "ffrc.ps f10, f6\n"
    "ffrc.ps f10, f7\n" 
                               
    "fround.ps f10, f0, dyn\n"
    //"fround.ps f10, f1, dyn\n"
    //"fround.ps f10, f2, dyn\n"
    "fround.ps f10, f3, dyn\n"
    "fround.ps f10, f4, dyn\n"
    "fround.ps f10, f5, dyn\n"
    "fround.ps f10, f6, dyn\n"  
    "fround.ps f10, f7, dyn\n"

    "fround.ps f10, f0, rne\n"
    //"fround.ps f10, f1, rne\n"
    //"fround.ps f10, f2, rne\n"
    "fround.ps f10, f3, rne\n"
    "fround.ps f10, f4, rne\n"
    "fround.ps f10, f5, rne\n"
    "fround.ps f10, f6, rne\n"  
    "fround.ps f10, f7, rne\n"    

    "fround.ps f10, f0, rtz\n"
    "fround.ps f10, f1, rtz\n"
    "fround.ps f10, f2, rtz\n"
    "fround.ps f10, f3, rtz\n"
    "fround.ps f10, f4, rtz\n"
    "fround.ps f10, f5, rtz\n"
    "fround.ps f10, f6, rtz\n"  
    "fround.ps f10, f7, rtz\n"    
    
    "fround.ps f10, f0, rdn\n"
    "fround.ps f10, f1, rdn\n"
    "fround.ps f10, f2, rdn\n"
    "fround.ps f10, f3, rdn\n"
    "fround.ps f10, f4, rdn\n"
    "fround.ps f10, f5, rdn\n"
    "fround.ps f10, f6, rdn\n"  
    "fround.ps f10, f7, rdn\n"        
    
    "fround.ps f10, f0, rup\n"
    "fround.ps f10, f1, rup\n"
    "fround.ps f10, f2, rup\n"
    "fround.ps f10, f3, rup\n"
    "fround.ps f10, f4, rup\n"
    "fround.ps f10, f5, rup\n"
    "fround.ps f10, f6, rup\n"  
    "fround.ps f10, f7, rup\n"    
    
    "fround.ps f10, f0, rmm\n"
    "fround.ps f10, f1, rmm\n"
    "fround.ps f10, f2, rmm\n"
    "fround.ps f10, f3, rmm\n"
    "fround.ps f10, f4, rmm\n"
    "fround.ps f10, f5, rmm\n"
    "fround.ps f10, f6, rmm\n"  
    "fround.ps f10, f7, rmm\n"        
    
  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
  );

  C_TEST_PASS;
  return 0;
}

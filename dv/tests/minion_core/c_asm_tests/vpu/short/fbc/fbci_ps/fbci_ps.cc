/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>

int main()
{

 
  __asm__ __volatile__ (

    //enable lanes
    "li x2, 0xff\n"
    "mova.m.x x2\n"

    //load operands
    "fbci.ps f0, 0x0\n"
    "fbci.ps f1, 0x80000\n"    
    "fbci.ps f2, 0x7fc00\n"
    "fbci.ps f3, 0xffc00\n"
    "fbci.ps f4, 0x7f800\n"
    "fbci.ps f5, 0xff800\n"        
    "fbci.ps f6, 0x4b8c4\n"  
    "fbci.ps f7, 0x4b000\n" 
    "fbci.ps f8, 0xc1a0c\n"
    "fbci.ps f9, 0xc1a4c\n" 
    "fbci.ps f10, 0x3fc7a\n"
    "fbci.ps f11, 0x40000\n"    
    "fbci.ps f12, 0x3f028\n"
    "fbci.ps f13, 0x3f800\n"
    "fbci.ps f14, 0xc0000\n"    
    "fbci.ps f15, 0xbf028\n"

    //disabled lane0-3
    "li x2, 0xf0\n"
    "mova.m.x x2\n"

    "fbci.ps f16, 0xbf8\n"     
    "fbci.ps f17, 0x800\n"    
    "fbci.ps f18, 0x7fc\n"
    "fbci.ps f19, 0xffc\n"
    "fbci.ps f20, 0x7f8\n"
    "fbci.ps f21, 0xff8\n"        
    "fbci.ps f22, 0x4b8\n"  
    "fbci.ps f23, 0x4b0\n" 
    "fbci.ps f24, 0xc1a0\n"
    "fbci.ps f25, 0xc1a4\n" 
    "fbci.ps f26, 0x3fc7\n"
    "fbci.ps f27, 0x4000\n"    
    "fbci.ps f28, 0x3f02\n"
    "fbci.ps f29, 0x3f80\n"
    "fbci.ps f30, 0xc000\n"    
    "fbci.ps f31, 0xbf02\n"




  // Asm output results to C code
  : 
  // Asm input data from C code
  : 
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2"
  );

  C_TEST_PASS;
  return 0;
}

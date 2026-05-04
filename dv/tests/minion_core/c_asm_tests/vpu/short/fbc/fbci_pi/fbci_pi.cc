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
    "fbci.pi f0, 0x0\n"
    "fbci.pi f1, 0x80000\n"    
    "fbci.pi f2, 0x7fc00\n"
    "fbci.pi f3, 0xffc00\n"
    "fbci.pi f4, 0x7f800\n"
    "fbci.pi f5, 0xff800\n"        
    "fbci.pi f6, 0x4b8c4\n"  
    "fbci.pi f7, 0x4b000\n" 
    "fbci.pi f8, 0xc1a0c\n"
    "fbci.pi f9, 0xc1a4c\n" 
    "fbci.pi f10, 0x3fc7a\n"
    "fbci.pi f11, 0x40000\n"    
    "fbci.pi f12, 0x3f028\n"
    "fbci.pi f13, 0x3f800\n"
    "fbci.pi f14, 0xc0000\n"    
    "fbci.pi f15, 0xbf028\n"

    //disabled lane0-3
    "li x2, 0xf0\n"
    "mova.m.x x2\n"

    "fbci.pi f16, 0xbf8\n"     
    "fbci.pi f17, 0x800\n"    
    "fbci.pi f18, 0x7fc\n"
    "fbci.pi f19, 0xffc\n"
    "fbci.pi f20, 0x7f8\n"
    "fbci.pi f21, 0xff8\n"        
    "fbci.pi f22, 0x4b8\n"  
    "fbci.pi f23, 0x4b0\n" 
    "fbci.pi f24, 0xc1a0\n"
    "fbci.pi f25, 0xc1a4\n" 
    "fbci.pi f26, 0x3fc7\n"
    "fbci.pi f27, 0x4000\n"    
    "fbci.pi f28, 0x3f02\n"
    "fbci.pi f29, 0x3f80\n"
    "fbci.pi f30, 0xc000\n"    
    "fbci.pi f31, 0xbf02\n"




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

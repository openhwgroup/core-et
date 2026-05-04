/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#include "computational.h"


int main()
{

    // Init marker
   C_TEST_START

    // Test
  static uint32_t operands[32]  __attribute__ ((aligned (16)))= {
    0x0,  
    0x0, 
    0x0, 
    0x0,   
    0x0, 
    0x0, 
    0x0, 
    0x0,   
//-----------------------
    0x40000000, // 2 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
//-----------------------
    0x41a4cccd, // 20.6  A         
    0x41a0cccd, // 20.1  B         
    0x41a4cccd, // 20.6  C         
    0x41a0cccd, // 20.1  D 
    0xc1a4cccd, // -20.6           
    0xc1a0cccd, // -20.1           
    0xc1a4cccd, // -20.6           
    0xc1a0cccd, // -20.1  
//-----------------------
    0x41a4cccd, // 20.6  A          
    0x41a0cccd, // 20.1           
    0x41a4cccd, // 20.6           
    0x41a0cccd, // 20.1   
    0x0, //            
    0x0, //            
    0x0, //            
    0x0 //   


   };
	
  __asm__ __volatile__ (

    //enable lanes
    "li x2, 0xff\n"
    "mova.m.x x2\n"

    //load operands
    "flw.ps f1, 0(%[operands])\n"
    "flw.ps f2, 32(%[operands])\n"    
    "flw.ps f5, 64(%[operands])\n"
    "flw.ps f6, 0(%[operands])\n"
    "flw.ps f7, 96(%[operands])\n"
    "flw.ps f8, 0(%[operands])\n"  

    //disable lanes 0-3
    "li x2, 0xf0\n"
    "mova.m.x x2\n"      
		 //     0123 4567
    //instruction       abcd 0000
    "fmin.ps f5, f1, f2\n"

    //enable lanes 0-3, disable 4-7
    "li x2, 0x0f\n"
    "mova.m.x x2\n"

    "fadd.ps f8, f5, f6\n"

    //enable all lanes 
    "li x2, 0xff\n"
    "mova.m.x x2\n"

   //check matching
    "feq.ps f9, f7, f8\n"

    // self-checking, to be improved
    "fcvt.w.s x3, f7, rtz \n"
    "fcvt.w.s x4, f8, rtz \n"
    "bne x3,x4,fail \n" // Fail out if doesn't match.
  
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
  );

    C_TEST_PASS
    __asm__ __volatile__ (
    "fail: \n");
    C_TEST_FAIL


   C_TEST_PASS;
    return 0;
}

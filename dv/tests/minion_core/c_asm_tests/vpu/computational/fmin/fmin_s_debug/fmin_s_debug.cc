/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "et_postsi_common.h"

#include "computational.h"


int main()
{

    // Init marker
   C_TEST_START

    // Test
  volatile uint32_t operands[32]  __attribute__ ((aligned (4096)))= {
    0x0,  
    0x0, 
    0x0, 
    0x0,   
    0x0, 
    0x0, 
    0x0, 
    0x0,   
//-----------------------
    0x40000000, // 2.0 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
    0x40000000, // 
//----------------------- values stored in f5 at the begining  
    0x41a4cccd, // 20.6   A             lane 0
    0x41a0cccd, // 20.1   B        	lane 1
    0x41a4cccd, // 20.6   C        	lane 2
    0x41a0cccd, // 20.1   D 		lane 3
    0xc1a4cccd, // -20.6  E         	
    0xc1a0cccd, // -20.1  F         
    0xc1a4cccd, // -20.6  G         
    0xc1a0cccd, // -20.1  H		lane 7
//----------------------- values stored in f7, expected to be in f1 and f5 at the end
    0x0,        // 0.0    0       
    0x41a0cccd, // 20.1   B        
    0x41a4cccd, // 20.6   C        
    0x41a0cccd, // 20.1   D
    0xc1a4cccd, // -20.6  E         
    0xc1a0cccd, // -20.1  F         
    0xc1a4cccd, // -20.6  G         
    0xc1a0cccd  // -20.1  H


   };
	
  __asm__ __volatile__ (

    //enable all lanes
    "li x5, 0xff\n"
    "mova.m.x x5\n"

    //load operands
    "flw.ps f1, 0(%[operands])\n"
    "flw.ps f2, 32(%[operands])\n"    
    "flw.ps f5, 64(%[operands])\n"
    "flw.ps f6, 0(%[operands])\n"
    "flw.ps f7, 96(%[operands])\n"
    "flw.ps f8, 0(%[operands])\n"  


    //fmin.s should change only lane 0 and leave other lanes untouched? NO, zeroes out
    //			  lane	0123 4567 
    "fmin.s  f5, f1, f2\n"  //  0000 0000 (output)

    //fadd.ps should  perform addition in each lane
    "fadd.ps f1, f5, f1\n"  //  
  

    //if fmin.s modifies only rf of lane0, this should  match
    "feq.ps f9, f7, f1\n"  // f9 0xffff ffff in all lanes (expected) NO!

    // self-checking, to be improved - checks only lane0!
    "fcvt.w.s x3, f7, rtz \n"
    "fcvt.w.s x4, f5, rtz \n"
    "bne x3,x4,fail \n" // Fail if f5 and f7 don't match.
  
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
  );

    c_test_pass();
    __asm__ __volatile__ (
    "fail: \n");
    C_TEST_FAIL


   c_test_pass();
    return 0;
}

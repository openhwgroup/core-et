/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>



int main()
{
   //   volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;


   volatile uint32_t operands[88]  __attribute__ ((aligned (4096)))= {
      0x00010000, // 17.14: 1.0
      0x000a4000, // 17.14: 10.25
      0x000a0019, // 17.14:
      0x00010000, // 17.14: 1.0  
      0x00010000, // 17.14: 1.0 
      0x00010000, // 17.14: 1.0
      0x00010000, // 17.14: 1.0
      0x00010000, // 17.14: 1.0 
      //-----------------------
      0x00004000, // 15.16: 1.0
      0x0000063e, // 15.16: 0.097 
      0x000003cf, // 15.16: 
      0x00004000, // 15.16: 1.0
      0x00004000, // 15.16: 1.0
      0x00004000, // 15.16: 1.0
      0x00004000, // 15.16: 1.0
      0x00004000, // 15.16: 1.0
      //-----------------------         



   };

   __asm__ __volatile__ (

         //enable lanes
         "li x5, 0xff\n"
         "mova.m.x x5\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 32(%[operands])\n"    
         "flw.ps f2, 8(%[operands])\n"




         // Instruction 
         "frcp_fix.rast f2,f0,f1\n"


         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );
   C_TEST_PASS;
   return 0;
}

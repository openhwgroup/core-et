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


   static uint32_t operands[88]  __attribute__ ((aligned (16)))= {
      0x00010000, // 17.14: 1.0
      0x00004000, // 15.16: 1.0
      0x000a0019, // 1.0
      0x000003cf, // 0.51  
      0xbf800000, // -1.0 
      0xbf028f5c, // -0.51
      0xbf800000, // -1.0
      0xbf028f5c, // -0.51  
      //-----------------------
      0x40000000, // 1.5 
      0x3fc7ae14, // 1.56
      0x40000000, // 1.5
      0x3fc7ae14, // 1.56
      0xbfc00000, // -1.5 
      0xbfc7ae14, // -1.56
      0xbfc00000, // -1.5
      0xbfc7ae14, // -1.56
      //-----------------------
      0x41a4cccd, // 20.6           
      0x41a0cccd, // 20.1           
      0x41a4cccd, // 20.6           
      0x41a0cccd, // 20.1   
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1           
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1  
      //-----------------------         



   };

   __asm__ __volatile__ (

         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         //load operands
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 4(%[operands])\n"    
         "flw.ps f2, 8(%[operands])\n"
         "flw.ps f3, 8(%[operands])\n"
         "flw.ps f4, 12(%[operands])\n"
         "flw.ps f5, 12(%[operands])\n"        




         //enable lanes
         "li x2, 0x01\n"
         "mova.m.x x2\n"

         // Instruction 
         "frcp_fix.rast f2,f0,f1\n"
         "frcp_fix.rast f5,f3,f4\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7"
            );
   C_TEST_PASS;
   return 0;
}

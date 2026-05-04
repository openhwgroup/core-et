/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;


   // Test
   static uint32_t operands[24]  __attribute__ ((aligned (16)))= {

      0x0,  // 
      0x0,  // 
      0x0,  // 
      0x0,  //               
      0x0, //                   
      0x0, //  
      0x0, //                   
      0x0, //   

      //-----------------------                
      0x3f800000,  // 
      0x3f800000,  // 
      0x3f800000,  // 
      0x3f800000,  // 
      0x3f800000, //                
      0x3f800000, // 
      0x3f800000, //                 
      0x3f800000, //  

   };


   // Test
   __asm__ __volatile__ (


         //enable lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"					//Write in mask registers
         //load operands
         "flw.ps f0, 32(%[operands])\n"    // +1.0
         "flw.ps f1, 0(%[operands])\n"     // +0.0


         "fsetm.pi m1, f0\n"

         "fsetm.pi m0, f1\n"

         "fsetm.pi m1, f1\n"


         :
         : [operands] "r" (operands)
         : "t0", "t1", "t2", "t3", "t4", "t5", "t6"
            );



   C_TEST_PASS;
   return 0;
}

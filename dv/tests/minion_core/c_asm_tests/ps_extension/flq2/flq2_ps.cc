/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test
   static uint32_t operands[32]  __attribute__ ((aligned (32)))= {
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0x7f800002, // signaling NaN 
      0xff800002, // -signaling NaN 
      0x7fc00001, //NANQ
      0xffc00001, //-NANQ
      0x1edcba98,
      0x12345678,
      0x12345678,
      0x1edcba98,
      0x69abcdef,
      0x38765432,
      0x7543654a,
      0x3c342985,
      0x15634579,
      0x78967365,
      0x88888888,
      0x88888888,
      0x77777777,
      0x77777777,
      0x44444444,
      0x44444444,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98
   };


   __asm__ __volatile__ (

         //"addi t1, %[operands], 32\n"
         //"addi t2, t1, 14\n"
         //"addi t2, t1, 15\n"
         //"sd t2, (t1)\n"

         //load operands
         //"flq2 f1, (t2)\n"
         "flq2 f1, 0(%[operands])\n"
         "fence\n"
         "fsq2 f1, 64(%[operands])\n"
         "flq2 f2, 64(%[operands])\n"
         : 
         : [operands] "r" (operands)
         : "x2", "t0", "t1", "t2","f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"
         );


   C_TEST_PASS;

}

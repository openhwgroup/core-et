/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"

int main()
{
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

   static uint32_t result[32]  __attribute__ ((aligned (32)))= {
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98
   };


   static uint32_t address[32]  __attribute__ ((aligned (16)))= {
      0x0,
      0x4,
      0x8,
      0xc,
      0x10,
      0x14,
      0x18,
      0x1c,
   };

   C_TEST_START;
   __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "addi t0, %[result], 0\n"

         "famomaxl.ps f0,f1(t0)\n"

         "flw.ps f0, 0(%[operands])\n"
         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
         );



   C_TEST_PASS;
   return 0;
}

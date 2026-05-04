/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>



int main()
{
   C_TEST_START;

   //test
   static long operands[13] = {
      0x40000000, // 2
      0x3f028f5c, // 0.51
      0x3fc7ae14, // 1.56
      0x3F800000, // 1.0
      0xc1a4cccd, // -20.6 --> bug
      0x4b000000, // 8388608.0
      0xff800000, // -inf
      0x7f800000, // inf
      0x7fffffff, //NaN
      0x7fc00001, // signaling NaN
      0xffc00001, // -signaling NaN
      0x0,  // zero
      0x80000000,  // -zero
   };

   __asm__ __volatile__ (
         //enable the 4 lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"
         //load operands
         "flw f1, 0(%[operands])\n"
         "flw f2, 8(%[operands])\n"
         "flw f3, 16(%[operands])\n"
         "flw f4, 24(%[operands])\n"
         "flw f5, 32(%[operands])\n"
         "flw f6, 40(%[operands])\n"
         "flw f7, 48(%[operands])\n"
         "flw f8, 56(%[operands])\n"
         "flw f9, 64(%[operands])\n"
         "flw f10, 72(%[operands])\n"
         "flw f11, 80(%[operands])\n"
         "flw f12, 88(%[operands])\n"
         "flw f13, 96(%[operands])\n"

         "fcvt.sn16.ps f0, f1\n" //32 to 16
         "fcvt.ps.sn16 f14, f0\n" //16 to 32

         "fcvt.sn16.ps f0, f2\n"
         "fcvt.ps.sn16 f15, f0\n"

         "fcvt.sn16.ps f0, f3\n"
         "fcvt.ps.sn16 f16, f0\n"

         "fcvt.sn16.ps f0, f5\n"
         "fcvt.ps.sn16 f17, f0\n"

         "fcvt.sn16.ps f0, f6\n"
         "fcvt.ps.sn16 f18, f0\n"

         "fcvt.sn16.ps f0, f7\n"
         "fcvt.ps.sn16 f19, f0\n"

         "fcvt.sn16.ps f0, f8\n"
         "fcvt.ps.sn16 f20, f0\n"

         "fcvt.sn16.ps f0, f9\n"
         "fcvt.ps.sn16 f21, f0\n"

         "fcvt.sn16.ps f0, f10\n"
         "fcvt.ps.sn16 f22, f0\n"

         "fcvt.sn16.ps f0, f11\n"
         "fcvt.ps.sn16 f23, f0\n"

         "fcvt.sn16.ps f0, f12\n"
         "fcvt.ps.sn16 f24, f0\n"

         "fcvt.sn16.ps f0, f13\n"
         "fcvt.ps.sn16 f25, f0\n"

         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11","f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22","f23","f24","f25"
            );

   C_TEST_PASS;
   return 0;
}

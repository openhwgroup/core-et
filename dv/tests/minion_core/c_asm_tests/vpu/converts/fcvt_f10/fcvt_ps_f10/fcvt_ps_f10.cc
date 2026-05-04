/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>



int main()
{
   C_TEST_START;

   //test
   volatile long operands[10] = {
      /* 0x40000000, // 2
         0x3f028f5c, // 0.51
         0x3fc7ae14, // 1.56
         0x3F800000, // 1.0
         0x41a4cccd, // 20.6
         0x477de800, //65000
         0x3a1fe868, //0.00061
         0x7f800000, // inf
         0x0,  // zero*/
      0x7fffffff, //NaN // bug
      /* 0x7f800020, // bug
         0x7fc00001, // signaling NaN // bug
         0x0,  // zero*/
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


         "fcvt.f10.ps f0, f1\n" //32 to 10
         "fcvt.ps.f10 f11, f0\n" //10 to 32

         "fcvt.f10.ps f0, f2\n"
         "fcvt.ps.f10 f12, f0\n"

         "fcvt.f10.ps f0, f3\n"
         "fcvt.ps.f10 f13, f0\n"

         "fcvt.f10.ps f0, f4\n"
         "fcvt.ps.f10 f14, f0\n"

         "fcvt.f10.ps f0, f5\n"
         "fcvt.ps.f10 f15, f0\n"

         "fcvt.f10.ps f0, f6\n"
         "fcvt.ps.f10 f16, f0\n"

         "fcvt.f10.ps f0, f7\n"
         "fcvt.ps.f10 f17, f0\n"

         "fcvt.f10.ps f0, f8\n"
         "fcvt.ps.f10 f18, f0\n"

         "fcvt.f10.ps f0, f9\n"
         "fcvt.ps.f10 f19, f0\n"

         "fcvt.f10.ps f0, f10\n"
         "fcvt.ps.f10 f20, f0\n"


         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11","f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20"
            );

   C_TEST_PASS
      return 0;
}

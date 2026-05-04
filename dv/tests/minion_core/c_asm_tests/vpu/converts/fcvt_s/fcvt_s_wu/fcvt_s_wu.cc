/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test
   static uint32_t operands[88]  __attribute__ ((aligned (16)))= {
      0x3f800000, // 1.0
      0x3f028f5c, // 0.51
      0x3f800000, // 1.0
      0x3f028f5c, // 0.51
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
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      0xcb000000, // -8388608.0
      0xcb8c4b40, // -18388608.0
      //-----------------------
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff, // -1.1754942E-38
      //-----------------------
      0x7fc00000, // quiet NaN
      0xffc00000, // -quiet NaN
      0x7fc00001, // signaling NaN
      0xffc00001, // -signaling NaN
      0x7fc00000, // quiet NaN
      0xffc00000, // -quiet NaN
      0x7fc00001, // signaling NaN
      0xffc00001, // -signaling NaN
      //-----------------------
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------
      0x7fc00000, // quiet NaN
      0xffc00000, // -quiet NaN
      0x7fc00001, // signaling NaN
      0xffc00001, // -signaling NaN
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------
      0x7fc00000, // quiet NaN
      0xffc00000, // -quiet NaN
      0x7fc00001, // signaling NaN
      0xffc00001, // -signaling NaN
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      //-----------------------
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff, // -1.1754942E-38
      //-----------------------
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000   // -zero

   };

   __asm__ __volatile__ (
         //enable the 4 lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 16(%[operands])\n"
         "flw.ps f2, 32(%[operands])\n"
         "flw.ps f3, 48(%[operands])\n"
         "flw.ps f4, 64(%[operands])\n"
         "flw.ps f5, 80(%[operands])\n"
         "flw.ps f6, 96(%[operands])\n"
         "flw.ps f7, 112(%[operands])\n"
         "flw.ps f8, 128(%[operands])\n"
         "flw.ps f9, 144(%[operands])\n"
         "flw.ps f10, 160(%[operands])\n"
         "flw.ps f11, 176(%[operands])\n"

         // Instructions
         "fcvt.wu.s t0, f0\n" //float to int reg
         "fcvt.s.wu f12, t0\n" //int to float

         "fcvt.wu.s t0, f1\n" //float to int reg
         "fcvt.s.wu f13, t0\n" //int to float

         "fcvt.wu.s t2, f2\n"
         "fcvt.s.wu f14, t2\n"

         "fcvt.wu.s t3, f3\n"
         "fcvt.s.wu f15, t3\n"

         "fcvt.wu.s t4, f4\n"
         "fcvt.s.wu f16, t4\n"

         "fcvt.wu.s t0, f5\n"
         "fcvt.s.wu f17, t0\n"

         "fcvt.wu.s t0, f6\n"
         "fcvt.s.wu f18, t0\n"

         "fcvt.wu.s t0, f7\n"
         "fcvt.s.wu f19, t0\n"

         "fcvt.wu.s t0, f9\n"
         "fcvt.s.wu f20, t0\n"

         "fcvt.wu.s t0, f10\n"
         "fcvt.s.wu f21, t0\n"

         "fcvt.wu.s t0, f11\n"
         "fcvt.s.wu f22, t0\n"

         "fcvt.wu.s t0, f12\n"
         "fcvt.s.wu f26, t0\n"

         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "t0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8"
            );

   C_TEST_PASS;
   return 0;

}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"


int main() {

   // Test
   volatile uint32_t operands[24]  __attribute__ ((aligned (4096)))= {
      0x3d91F98f,
      0x3dA291CE,
      0x3dB7CB9E,
      0x3e11F98F,
      0x3e2291CE,
      0x3e37CB9E,
      0x3e91F98F,
      0x3eA291CE,

      0x3eB01C36,
      0x3eB20A72,
      0x3eB56B4A,
      0x3eB7CB9E,
      0x3eBE3F49,
      0x3fC39642,
      0x3fCD06FA,
      0x3fE7261E,

      0x3fECB136,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000

   };

   C_TEST_START;
   __asm__ __volatile__ (

         //enable lanes
         "li x4, 0xff\n"
         "mova.m.x x4\n"

         //load operands
        //"flw.ps f0,0(%[operands])\n"
        //"flw.ps f1,32(%[operands])\n"
        "flw.ps f2,64(%[operands])\n"

        //"flog.ps f16,f0\n"
        //"flog.ps f16,f1\n"
        "flog.ps f16,f2\n"

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x4", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
            );	

   C_TEST_PASS;
   return 0;
}

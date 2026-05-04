/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"


int main() {

   // Test
   volatile uint32_t operands[56]  __attribute__ ((aligned (4096)))= {
      0x3f000000,
      0x3f400000,
      0x3f400000,
      0x3f700000,
      0x3f780000,
      0xbf7c0000,
      0xbf7e0000,
      0xbf7f0000,

      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,
      0x3f7fffff,

      0x3f000001, 
      0x3f400001,
      0x3f400001, 
      0x3f700001,  
      0x3f780001,  
      0x3f7c0001,  
      0x3f7e0001,
      0x3f7f0001,

      0xbf7f8000,
      0xbf7fc000,
      0xbf7fe000,
      0x3f7ff000,
      0xbf7ff800,
      0xbf7ffc00,
      0xbf7ffe00,
      0xbf7fff00,

      0x3f7f8001,
      0x3f7fc001,
      0x3f7fe001,
      0xbf7ff001,
      0x3f7ff801,
      0x3f7ffc01,
      0x3f7ffe01,
      0x3f7fff01,

      0xbf7fff80,
      0xbf7fffc0,
      0xbf7fffe0,
      0xbf7ffff0,
      0xbf7ffff8,
      0xbf7ffffc,
      0xbf7ffffe,
      0xff7ffffe,

      0x3f7fff81,
      0x3f7fffc1,
      0x3f7fffe1,
      0x3f7ffff1,
      0x3f7ffff9,
      0x3f7ffffd,
      0x3f7fffff,
      0xff7fffff
   };

   C_TEST_START;
   __asm__ __volatile__ (

         //enable lanes
         "li x4, 0xff\n"
         "mova.m.x x4\n"

         //load operands
        "flw.ps f0,0(%[operands])\n"
        "flw.ps f1,32(%[operands])\n"
        "flw.ps f2,64(%[operands])\n"
        "flw.ps f3,96(%[operands])\n"
        "flw.ps f4,128(%[operands])\n"
        "flw.ps f5,160(%[operands])\n"
        "flw.ps f6,192(%[operands])\n"

        "fmadd.ps f31,f0,f1,f2, rne\n"
        "fmadd.ps f31,f0,f1,f2, rtz\n"
        "fmadd.ps f31,f0,f1,f2, rdn\n"
        "fmadd.ps f31,f0,f1,f2, rup\n"
        "fmadd.ps f31,f0,f1,f2, rmm\n"

        "fmadd.ps f31,f3,f1,f4, rne\n"
        "fmadd.ps f31,f3,f1,f4, rtz\n"
        "fmadd.ps f31,f3,f1,f4, rdn\n"
        "fmadd.ps f31,f3,f1,f4, rup\n"
        "fmadd.ps f31,f3,f1,f4, rmm\n"

        "fmadd.ps f31,f5,f1,f6, rne\n"
        "fmadd.ps f31,f5,f1,f6, rtz\n"
        "fmadd.ps f31,f5,f1,f6, rdn\n"
        "fmadd.ps f31,f5,f1,f6, rup\n"
        "fmadd.ps f31,f5,f1,f6, rmm\n"


         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x4", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
            );	

   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[2] __attribute__ ((aligned (16)))= {
    0x12345678,
    0xfedcba98
  };

  __asm__ __volatile__ (

    //load operands
    "flw f0, 0(%[operands])\n"
    "flw f1, 8(%[operands])\n"
    "flw f2, 16(%[operands])\n"

      //wait some time
    "li t3, 0x03\n"
    "mova.m.x t3\n"
    "mova.m.x t3\n"

    "masknot m2, m0\n"
    "maskand m3, m0, m2\n"
    "maskor m4, m0, m3\n"
    "maskxor m5, m0, m4\n"
    "masknot m6, m5\n"

    "li x1, 0x64\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mov.m.x m1, x1, 0x1\n"
    "maskpopc x1,m0\n"
    "maskpopcz x1,m0\n"


    "masknot m2, m0\n"
    "maskand m3, m0, m2\n"
    "maskor m4, m0, m3\n"
    "maskxor m5, m0, m4\n"
    "masknot m6, m5\n"

    "li x1, 0x128\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mov.m.x m1, x1, 0x5\n"
    "maskpopc x1,m1\n"
    "maskpopcz x1,m1\n"

    "masknot m2, m0\n"
    "maskand m3, m0, m2\n"
    "maskor m4, m0, m3\n"
    "maskxor m5, m0, m4\n"
    "masknot m6, m5\n"

    "li x1, 0x123456\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mov.m.x m1, x1, 0xf\n"
    "maskpopc x1,m2\n"
    "maskpopcz x1,m2\n"

    "masknot m2, m0\n"
    "maskand m3, m0, m2\n"
    "maskor m4, m0, m3\n"
    "maskxor m5, m0, m4\n"
    "masknot m6, m5\n"
    "mov.m.x m1, x1, 0xf0\n"
    "maskpopc x1,m3\n"
    "maskpopcz x1,m3\n"

    "li x1, -1\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mova.m.x x1\n"
    "mova.x.m x1\n"
    "mov.m.x m1, x1, 0xff\n"
    "maskpopc x1,m4\n"
    "maskpopcz x1,m4\n"

    "masknot m2, m0\n"
    "maskand m3, m0, m2\n"
    "maskor m4, m0, m3\n"
    "maskxor m5, m0, m4\n"
    "masknot m6, m5\n"


  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31","a5"

  );
  C_TEST_PASS;
  return 0;
}

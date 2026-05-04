/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[20]  __attribute__ ((aligned (16)))= {
    0x412b3333,
    0x412b3333,
    0x412b3333,
    0x412b3333,
    
    0x42480000,
    0x42480000,
    0x42480000,
    0x42480000,
    
    0x40f8902e,
    0x40f8902e,
    0x40f8902e,
    0x40f8902e,
    
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
    //load operands
    "flw f0, 0(%[operands])\n"
    "flw f1, 16(%[operands])\n"
    "flw f2, 32(%[operands])\n"
  : 
  : [operands] "r" (operands)
  : "t3","f0","f1","f2"
  );

  __asm__ __volatile__ (
    //computational 
  "fadd.s f4, f0, f1\n"
  "fsub.s f4, f0, f1\n"
  "fmul.s f5, f0, f1\n"
  "fmadd.s f9, f0, f1, f2\n"
  "fmsub.s f10, f0, f1, f2\n"
  "fnmsub.s f11, f0, f1, f2\n" 
  "fnmadd.s f12, f0, f1, f2\n"
  "fmin.s f4, f0, f1\n"
  "fmax.s f5, f0, f1\n"

  "li t3, 0x12345678\n"  
  "fmv.s.x f5, t3\n"
  "fmv.x.s x1, f4\n"
  "fsgnj.s f5, f0, f1\n"
  "fsgnjn.s f5, f0, f1\n"
  "fsgnjx.s f5, f0, f1\n"
 // "fcvt.wu.s x1, f4\n"
  "fcvt.w.s x1, f4\n"
  "fcvt.s.wu f4, x1\n"
  "fcvt.s.w f4, x1\n"

    //store  
  "fsw f4, 0(a1)\n"
  "fsw f4, 8(a1)\n"
  "fsw f4, 16(a1)\n"

  // Asm output results to C code
  : 
  // Asm input data from C code
  :
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31","a5"
  );

  C_TEST_PASS;

}

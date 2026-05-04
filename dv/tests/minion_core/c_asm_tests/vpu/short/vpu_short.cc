/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[8]  __attribute__ ((aligned (16)))= {
    0x00000002,
    0xf0000002,
    0x00000002,
    0xf0000002,
    0x0f0f0f0f,
    0x0f0f0f0f,
    0x0f0f0f0f,
    0x0f0f0f0f
   };


  __asm__ __volatile__ (
    //enable lanes
    "li x2, 0xff\n"
    "mova.m.x x2\n"
  
    //load operands
    "flw.ps f0, 0(%[operands])\n"
    "flw.ps f1, 16(%[operands])\n"

      //broadcast
    "fbci.ps f4, 0x0\n"
    "fbci.ps f4, 0x1\n"
    "fbci.ps f4, 0xfff\n"
    "fbci.ps f4, 0xfffff\n"
    "fbc.ps  f4, 0(%[operands])\n"
    "fbc.ps  f4, 4(%[operands])\n"
    "fbc.ps  f4, 8(%[operands])\n"
    "fcmovm.ps f9, f0, f1\n"


  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2"
  );

  C_TEST_PASS;
  return 0;
}

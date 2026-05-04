/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[16]  __attribute__ ((aligned (16)))= {
    0x1,
    0x5,
    0xf,
    0x2,
    0xa,
    0x0,
    0xc,
    0x11,
    0xFF00FF00,
    0x00FF00FF,
    0x10101010,
    0xFFFFFFFF,
    0x00000000,
    0xFF000000,
    0x000000FF,
    0x000FF000
   };

  C_TEST_START;



  __asm__ __volatile__ (
    //enable lanes
    "li x11, 0xff\n"
    "mova.m.x x11\n"
  
    //load operands
    "flw.ps f0, 0(%[operands])\n"
    "flw.ps f1, 32(%[operands])\n"
    "flw.ps f3, 16(%[operands])\n"
    "flw.ps f4, 24(%[operands])\n"
    "flw.ps f6, 4(%[operands])\n"
    "flw.ps f8, 0(%[operands])\n"

    "fsra.pi f2, f0, f0\n"

    "fsra.pi f3, f2, f0\n"

    "fsra.pi f5, f3, f0\n"

    "fsra.pi f7, f5, f0\n"

    "fsra.pi f3, f7, f0\n"


  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "x11"
  );

  C_TEST_PASS;
  return 0;
}

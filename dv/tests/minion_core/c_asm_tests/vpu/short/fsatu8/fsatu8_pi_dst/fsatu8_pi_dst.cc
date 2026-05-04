/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[16]  __attribute__ ((aligned (16)))= {
    0x11111111,
    0xF0F0F0F0,
    0x00000000,
    0x0F0F0F0F,
    0x01010101,
    0xFFFFFFFF,
    0xFFFF0000,
    0x0000FFFF,
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
    "flw.ps f2, 16(%[operands])\n"
    "flw.ps f4, 24(%[operands])\n"
    "flw.ps f6, 4(%[operands])\n"
    "flw.ps f8, 0(%[operands])\n"

    "fsatu8.pi f3, f0\n"

    "fsatu8.pi f3, f2\n"

    "fsatu8.pi f5, f1\n"


    "fsatu8.pi f5, f2\n"

    "fsatu8.pi f5, f8\n"


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

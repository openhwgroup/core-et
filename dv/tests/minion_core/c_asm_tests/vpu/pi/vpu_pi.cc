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

    "fmin.pi f2, f1, f0\n"
    "fmax.pi f2, f1, f0\n"
    "fand.pi f2, f1, f0\n"
    "for.pi f2, f1, f0\n"
    "fxor.pi f2, f1, f0\n"
    "fnot.pi f2, f0\n"    
    "fsll.pi f2, f1, f0\n"
    "fsrl.pi f2, f1, f0\n"
    "fsra.pi f2, f1, f0\n"
    "feq.pi f2, f1, f0\n"
    "flt.pi f2, f1, f0\n" 
    "fltu.pi f2, f1, f0\n" 
    "fle.pi f2, f1, f0\n"
    "faddi.pi f2, f1, -2\n"        
    "fandi.pi f2, f1, -2\n"
    "fslli.pi f2, f1, 2\n"
    "fbci.pi f2, 0xffffe\n"
    "fsrli.pi f2, f1, 2\n"
    "fsrai.pi f2, f1, 2\n"
    "fsat8.pi f2, f1\n"         
    "fsatu8.pi f2, f1\n"         
    
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

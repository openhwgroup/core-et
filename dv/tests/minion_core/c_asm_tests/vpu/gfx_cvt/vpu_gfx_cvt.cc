/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;

int main()
{
  *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

  static long operands[10] = {
    0x00003C00,
    0x000003C0,
    0x000001E0,
    0x3F800000,
    0x00FFFFFF,
    0x007FFFFF,
    0x00007FFF,
    0x0000007F,
    0x00000000,
    0x411B1000
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
    "li x2, 128\n"
    "fscsr zero, x2\n"
    "fcvt.ps.f16 f0, f1\n"
    "fcvt.ps.f11 f0, f2\n"
    "fcvt.ps.f10 f0, f3\n"
    "fcvt.ps.un24 f0, f5\n"
    "fcvt.ps.un16 f0, f5\n"
    "fcvt.ps.un8 f0, f5\n"
    "fcvt.ps.un10 f0, f5\n"
    "fcvt.ps.un2 f0, f5\n"
    "fcvt.ps.sn16 f0, f7\n"
    "fcvt.ps.sn8 f0, f8\n"
    "fcvt.f16.ps f0, f1\n"
    "fcvt.f11.ps f0, f2\n"
    "fcvt.f10.ps f0, f3\n"
    "fcvt.un24.ps f0, f9\n"
    "fcvt.un16.ps f0, f9\n"
    "fcvt.un10.ps f0, f9\n"
    "fcvt.un8.ps f0, f4\n"
    "fcvt.un2.ps f0, f4\n"
    "fcvt.sn16.ps f0, f4\n"
    "fcvt.sn8.ps f0, f4\n"
    "fcvt.f16.ps f0, f10\n"
  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10"
  );

  C_TEST_PASS;
  return 0;
}

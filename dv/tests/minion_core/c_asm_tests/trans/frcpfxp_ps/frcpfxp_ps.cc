/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;

int main()
{
   C_TEST_START;
   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

   // Test
   __asm__ __volatile__ (
         // Enables all elements of FPU
         "mov.m.x m0, zero, 0xff\n"
         "addi    x3, zero, 8\n"
         "li      x1, 0x3f7872a6\n"
         "fbcx.ps f0, x1\n"
         "loop:\n"
         "fcvt.ps.rast f1, f0\n"
         "frcp.ps f1, f1\n"
         "csrr t0, mepc\n"
         "fcvt.rast.ps f1, f1\n"
         "frcp_fix.rast f0, f0, f1\n"
         "addi    x1, x1, 1\n"
         "addi    x3, x3, -1\n"
         //"bne     x3, zero, loop\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         "addi    x3, x3, -1\n"
         : 
         : 
         : "f0", "x1", "x3" 
         );

   C_TEST_PASS;
   return 0;
}

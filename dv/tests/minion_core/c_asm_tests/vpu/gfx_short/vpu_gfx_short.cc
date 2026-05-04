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

      //pi    
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
    "fsetm.pi m2, f0\n"
    
      //conversion and move    
    "fcmov.ps f9, f0, f1, f2\n"
    "fcmovm.ps   f9, f0, f1\n"    
    "fmvs.x.ps   x3, f0, 0\n"
    "fmvs.x.ps   x3, f0, 1\n"
    "fmvs.x.ps   x3, f0, 2\n"
    "fmvs.x.ps   x3, f0, 3\n"
    "fmvz.x.ps   x3, f0, 0\n"
    "fmvz.x.ps   x3, f0, 1\n"
    "fmvz.x.ps   x3, f0, 2\n"
    "fmvz.x.ps   x3, f0, 3\n"    
    "fswizz.ps f8, f0, 0\n"

      //cube face
    "cubeface.ps f3, f1, f0\n"
    "cubeface.ps f9, f3, f2\n"
    "cubefaceidx.ps f3, f0, f1\n"
    "cubefaceidx.ps f4, f1, f3\n"
    "cubefaceidx.ps f5, f2, f5\n"
    "cubefaceidx.ps f6, f3, f6\n"
    "cubefaceidx.ps f7, f4, f8\n"
    "cubefaceidx.ps f8, f5, f7\n"
    "cubefaceidx.ps f9, f6, f0\n"
    "cubefaceidx.ps f10, f7, f3\n"
    "cubesgnsc.ps f11, f0, f1\n"
    "cubesgnsc.ps f12, f2, f3\n"
    "cubesgntc.ps f13, f0, f1\n"
    "cubesgntc.ps f14, f2, f3\n"    


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

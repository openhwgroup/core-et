/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[20]  __attribute__ ((aligned (16)))= {
    0x1edcba98,
    0x12345678,
    0x12345678,
    0x1edcba98,
    0x69abcdef,
    0x38765432,
    0x7543654a,
    0x3c342985,
    0x15634579,
    0x78967365,
    0x88888888,
    0x88888888,
    0x77777777,
    0x77777777,
    0x44444444,
    0x44444444,
    0xfedcba98,
    0xfedcba98,
    0xfedcba98,
    0xfedcba98
  };



  for (uint32_t i=0; i<10; i++) 
  {  

    if (i==0) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x03\n"
      : 
      :
      : "t3"
      );
    } else if (i==1) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x0c\n"
      : 
      :
      : "t3"
      );
    }  else if (i==2) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x30\n"
      : 
      :
      : "t3"
      );
    }  else if (i==3) {
      __asm__ __volatile__ (
        "li t3, 0xc0\n"
      : 
      :
      : "t3"
      );
    }  else if (i==4) {
      __asm__ __volatile__ (
        "li t3, 0xff\n"
      : 
      :
      : "t3"
      );
    }  else if (i==5) {
      __asm__ __volatile__ (
        "li t3, 0x33\n"
      : 
      :
      : "t3"
      );
    }   else if (i==6) {
      __asm__ __volatile__ (
        "li t3, 0xc3\n"
      : 
      :
      : "t3"
      );
    } else if (i==7) {
      __asm__ __volatile__ (
        "li t3, 0x3c\n"
      : 
      :
      : "t3"
      );
    } else if (i==8) {
      __asm__ __volatile__ (
        "li t3, 0xcc\n"
      : 
      :
      : "t3"
      );
    } else if (i==9) {
      __asm__ __volatile__ (
        "li t3, 0x0\n"
      : 
      :
      : "t3"
      );
    }


    if (i<4) {
      __asm__ __volatile__ (
        //mask lanes
        "mova.m.x t3\n"

        //load operands
        "flw.ps f0, 0(%[operands])\n"
        "flw.ps f1, 16(%[operands])\n"
        "flw.ps f2, 32(%[operands])\n"
        "flw.ps f3, 0(%[operands])\n"

        "li t3, 0xcc\n"
      : 
      : [operands] "r" (operands)
      : "t3","f0","f1","f2"
      );
    } else if (i==4) {
      __asm__ __volatile__ (
        //mask lanes
        "mova.m.x t3\n"

        //load operands
        "flw.ps f0, 16(%[operands])\n"
        "flw.ps f1, 32(%[operands])\n"
        "flw.ps f2, 0(%[operands])\n"

        "li t3, 0xcc\n"
      : 
      : [operands] "r" (operands)
      : "t3","f0","f1","f2"
      );
    } else {
      __asm__ __volatile__ (
        //mask lanes
        "mova.m.x t3\n"

        //load operands
        "flw.ps f0, 32(%[operands])\n"
        "flw.ps f1, 0(%[operands])\n"
        "flw.ps f2, 16(%[operands])\n"

        "li t3, 0xcc\n"
      : 
      : [operands] "r" (operands)
      : "t3","f0","f1","f2"
      );
    }


    __asm__ __volatile__ (
 

      //broadcast
    "fbci.ps f4, 0x0\n"
    "fbci.ps f4, 0x1\n"
    "fbci.ps f4, 0xfff\n"
    "fbci.ps f4, 0xfffff\n"

      //computational
    "fadd.ps f4, f0, f1\n"
    "fsub.ps f4, f0, f1\n"
    //"fmul.ps f5, f0, f1\n" //failing RM
    "fmin.ps f7, f0, f1\n"
    "fmax.ps f8, f0, f1\n"
    "fmadd.ps f9, f0, f1, f2\n"
    "fmsub.ps f10, f0, f1, f2\n"
    "fnmsub.ps f11, f0, f1, f2\n" 
    "fnmadd.ps f12, f0, f1, f2\n"
    //"fdiv.ps f6, f0, f1\n"
    //"fsqrt.ps f13, f3\n"
    
      //fp compare
    "feq.ps f14, f0, f1\n"
    "flt.ps f14, f0, f1\n"
    "fle.ps f16, f0, f1\n"
    "fclass.ps f17, f0\n"
    "fsetm.pi m2, f0\n"
    "feqm.ps m5, f0, f1\n"
    "fltm.ps m6, f0, f1\n"
    "flem.ps m7, f0, f1\n"
    "fcmov.ps f9, f0, f0, f1\n"
    "fswizz.ps f8, f0, 0\n"
    "fswizz.ps f8, f0, 1\n"
    "fswizz.ps f8, f0, 2\n"
    "fswizz.ps f8, f0, 15\n"
    "fswizz.ps f8, f0, 32\n"
    "fswizz.ps f8, f0, 64\n"
    "fswizz.ps f8, f0, 128\n"
    "fswizz.ps f8, f0, 255\n"
    "fpackreph.pi f8, f0\n"
    "fpackreph.pi f8, f1\n"
    "fpackrepb.pi f8, f0\n"
    "fpackrepb.pi f8, f1\n"

      //fp conversion and move
    
    "fcvt.pw.ps  f17, f3\n"
    "fcvt.pwu.ps f18, f3\n"
    "fcvt.ps.pw  f19, f0\n"
    "fcvt.ps.pwu f20, f0\n"
    "fsgnjn.ps   f23, f1, f0\n"
    "fsgnj.ps    f24, f1, f0\n"
    "fsgnjx.ps   f25, f1, f0\n"
    "fmvs.x.ps   x3, f0, 0\n"
    "fmvs.x.ps   x3, f0, 1\n"
    "fmvs.x.ps   x3, f0, 2\n"
    "fmvs.x.ps   x3, f0, 3\n"
    "fmvz.x.ps   x3, f0, 0\n"
    "fmvz.x.ps   x3, f0, 1\n"
    "fmvz.x.ps   x3, f0, 2\n"
    "fmvz.x.ps   x3, f0, 3\n" 
    
    // Asm output results to C code
    : 
    // Asm input data from C code
    :
    // Registers used in Asm so C knows they change
    : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31","a5"
    );
  }

  C_TEST_PASS;

}

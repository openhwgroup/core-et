/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>

int main()
{

static uint32_t operands[32]  __attribute__ ((aligned (32)))= {
    0xff800000,//-Inf
    0xc1a4cccd, //-20.6
    0x806ce3ee, // -1.0E-38-0
    0x80000000, // -zero
    0x0,  // zero
    0x006ce3ee, //1.0E-38
    0x40000000, //2
    0x7f800000, // +Inf
    0x7f800002, // signaling NaN 
    0xff800002, // -signaling NaN 
    0x7fc00001, //NANQ
    0xffc00001, //-NANQ
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

for (uint32_t i=0; i<14; i++) 
  {  

    if (i==0) {
      __asm__ __volatile__ (
        //enable lanes
        "li x2, 0x00\n"
      : 
      :
      : "x2"
      );
    } else if (i==1) {
      __asm__ __volatile__ (
        //enable lanes
        "li x2, 0x01\n"
      : 
      :
      : "x2"
      );
    }  else if (i==2) {
      __asm__ __volatile__ (
        //enable lanes
        "li x2, 0x02\n"
      : 
      :
      : "x2"
      );
    }  else if (i==3) {
      __asm__ __volatile__ (
        "li x2, 0x04\n"
      : 
      :
      : "x2"
      );
    }  else if (i==4) {
      __asm__ __volatile__ (
        "li x2, 0x08\n"
      : 
      :
      : "x2"
      );
    }  else if (i==5) {
      __asm__ __volatile__ (
        "li x2, 0x10\n"
      : 
      :
      : "x2"
      );
    }   else if (i==6) {
      __asm__ __volatile__ (
        "li x2, 0x20\n"
      : 
      :
      : "x2"
      );
    } else if (i==7) {
      __asm__ __volatile__ (
        "li x2, 0x40\n"
      : 
      :
      : "x2"
      );
    } else if (i==8) {
      __asm__ __volatile__ (
        "li x2, 0x80\n"
      : 
      :
      : "x2"
      );
    } else if (i==9) {
      __asm__ __volatile__ (
        "li x2, 0xff\n"
      : 
      :
      : "x2"
      );
    } else if (i==10) {
      __asm__ __volatile__ (
        "li x2, 0x55\n"
      : 
      :
      : "x2"
      );
    } else if (i==11) {
      __asm__ __volatile__ (
        "li x2, 0xaa\n"
      : 
      :
      : "x2"
      );
    } else if (i==12) {
      __asm__ __volatile__ (
        "li x2, 0x0f\n"
      : 
      :
      : "x2"
      );
    } else if (i==13) {
      __asm__ __volatile__ (
        "li x2, 0xf0\n"
      : 
      :
      : "x2"
      );
    }


      __asm__ __volatile__ (
        "mova.m.x x2\n"
	"li t1, 0xeca86420\n"
	"flw.ps f1, 0(%[operands])\n"
	"addi t2, %[operands], 32\n"

	"fsc32h.ps f1, t1(t2)\n"
	"fence\n"
	"flw.ps f2, 32(%[operands])\n"

      : 
      : [operands] "r" (operands)
      : "f0", "f1", "f2", "t0", "t1", "t2", "t3", "x2"
      );

  }

C_TEST_PASS;

}

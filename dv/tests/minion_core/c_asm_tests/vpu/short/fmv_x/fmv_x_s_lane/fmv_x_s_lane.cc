/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

 static uint32_t operands[32]  __attribute__ ((aligned (16)))= {
    0x3f800000, // 1.0 
    0x3f028f5c, // 0.51
    0x3f800000, // 1.0
    0x3f028f5c, // 0.51  
    0x40000000, // 1.5 
    0x3fc7ae14, // 1.56
    0x40000000, // 1.5
    0x3fc7ae14, // 1.56
    0xc1a4cccd, // -20.6           
    0xc1a0cccd, // -20.1           
    0xc1a4cccd, // -20.6           
    0xc1a0cccd, // -20.1           
    0x4b000000, // 8388608.0
    0x4b8c4b40, // 18388608.0
    0x4b000000, // 8388608.0
    0x4b8c4b40, // 18388608.0
    0x7fc00000, // quiet NaN                  
    0xffc00000, // -quiet NaN                 
    0x7fc00000, // quiet NaN                  
    0xffc00000, // -quiet NaN                 
    0x7f800000, // inf
    0xff800000, // -inf
    0x7f800000, // inf
    0xff800000, // -inf
    0x7fc00001, // signaling NaN                  
    0xffc00001, // -signaling NaN                 
    0x7fc00001, // signaling NaN                  
    0xffc00001, // -signaling NaN    
    0x00000000,  // zero
    0x80000000,  // -zero
    0x00000000,  // zero
    0x80000000   // -zero
   };

for (uint32_t i=0; i<14; i++) 
  {  

    if (i==0) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x00\n"
      : 
      :
      : "t3"
      );
    } else if (i==1) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x01\n"
      : 
      :
      : "t3"
      );
    }  else if (i==2) {
      __asm__ __volatile__ (
        //enable lanes
        "li t3, 0x02\n"
      : 
      :
      : "t3"
      );
    }  else if (i==3) {
      __asm__ __volatile__ (
        "li t3, 0x04\n"
      : 
      :
      : "t3"
      );
    }  else if (i==4) {
      __asm__ __volatile__ (
        "li t3, 0x08\n"
      : 
      :
      : "t3"
      );
    }  else if (i==5) {
      __asm__ __volatile__ (
        "li t3, 0x10\n"
      : 
      :
      : "t3"
      );
    }   else if (i==6) {
      __asm__ __volatile__ (
        "li t3, 0x20\n"
      : 
      :
      : "t3"
      );
    } else if (i==7) {
      __asm__ __volatile__ (
        "li t3, 0x40\n"
      : 
      :
      : "t3"
      );
    } else if (i==8) {
      __asm__ __volatile__ (
        "li t3, 0x80\n"
      : 
      :
      : "t3"
      );
    } else if (i==9) {
      __asm__ __volatile__ (
        "li t3, 0xff\n"
      : 
      :
      : "t3"
      );
    } else if (i==10) {
      __asm__ __volatile__ (
        "li t3, 0x55\n"
      : 
      :
      : "t3"
      );
    } else if (i==11) {
      __asm__ __volatile__ (
        "li t3, 0xaa\n"
      : 
      :
      : "t3"
      );
    } else if (i==12) {
      __asm__ __volatile__ (
        "li t3, 0x0f\n"
      : 
      :
      : "t3"
      );
    } else if (i==13) {
      __asm__ __volatile__ (
        "li t3, 0xf0\n"
      : 
      :
      : "t3"
      );
    }


  __asm__ __volatile__ (

    //enable lanes
    "li t3, 0xff\n"
    "mova.m.x t3\n"

    //load operands
    "flw.ps f0, 0(%[operands])\n"
    "flw.ps f1, 16(%[operands])\n"    
    "flw.ps f2, 32(%[operands])\n"
    "flw.ps f3, 48(%[operands])\n"
    "flw.ps f4, 64(%[operands])\n"
    "flw.ps f5, 80(%[operands])\n"        
    "flw.ps f6, 96(%[operands])\n"  
    "flw.ps f7, 0(%[operands])\n" 

 
      //move 

    "fmv.x.s t0, f4\n"
		"fmv.s.x f4, t0\n"
		"fmv.x.s t0, f1\n"
		"fmv.s.x f1, t0\n"
		"fmv.x.s t0, f2\n"
		"fmv.s.x f2, t0\n"
		"fmv.x.s t0, f3\n"
		"fmv.s.x f3, t0\n"
		"fmv.x.s t0, f4\n"
		"fmv.s.x f4, t0\n"
		"fmv.x.s t0, f5\n"
		"fmv.s.x f5, t0\n"
		"fmv.x.s t0, f6\n"
		"fmv.s.x f6, t0\n"
		"fmv.x.s t0, f7\n"
		"fmv.s.x f7, t0\n"


      : 
      : [operands] "r" (operands)
      : "f0", "f1", "f2", "t0", "t1", "t2", "t3"
      );

  }

C_TEST_PASS;

}

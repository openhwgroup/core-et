/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>

int main()
{

 static uint32_t operands[32]  __attribute__ ((aligned (32)))= {
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
    0x0,  // zero
    0x80000000,  // -zero
    0x0,  // zero
    0x80000000   // -zero
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
        //mask lanes
        "mova.m.x x2\n"

   	"li t0, 0\n"
    	"fcvt.s.w f0, t0\n"
    	"li t1, 0\n"
    	"fcvt.s.w f9, t1\n"

        //load operands
	"fbc.ps f0, 0(%[operands])\n"
	"fbc.ps f1, 8(%[operands])\n"    
	"fbc.ps f2, 16(%[operands])\n"
	"fbc.ps f3, 24(%[operands])\n"
	"fbc.ps f4, 32(%[operands])\n"
	"fbc.ps f5, 40(%[operands])\n"        
	"fbc.ps f6, 48(%[operands])\n"  
	"fbc.ps f7, 56(%[operands])\n" 
	"fbc.ps f8, 64(%[operands])\n"

        "fadd.pi f0, f9, f1\n"
        "fadd.pi f0, f9, f2\n"
        "fadd.pi f0, f9, f3\n"
	"fadd.pi f0, f9, f4\n"
        "fadd.pi f0, f9, f5\n"
        "fadd.pi f0, f9, f6\n"
        "fadd.pi f0, f9, f7\n"
	"fadd.pi f0, f9, f8\n"
      : 
      : [operands] "r" (operands)
      : "x2", "t0", "t1", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"
      );

  }

C_TEST_PASS;

}

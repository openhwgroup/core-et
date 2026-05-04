/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
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

    //load operands
    "flw.ps f0, 0(%[operands])\n"
    "flw.ps f1, 16(%[operands])\n"    
    "flw.ps f2, 32(%[operands])\n"
    "flw.ps f3, 48(%[operands])\n"
    "flw.ps f4, 64(%[operands])\n"
    "flw.ps f5, 80(%[operands])\n"        
    "flw.ps f6, 96(%[operands])\n"  
    "flw.ps f7, 0(%[operands])\n" 

 
      //move and broadcast

    "fmvs.x.ps x0, f0, 0\n"
    "fbcx.ps f8, x0\n"
    "fmvs.x.ps x1, f1, 0\n"
    "fbcx.ps f9, x1\n"
    "fmvs.x.ps x2, f2, 0\n"
    "fbcx.ps f10, x2\n"
    "fmvs.x.ps x3, f3, 0\n"
    "fbcx.ps f11, x3\n"
    "fmvs.x.ps x4, f4, 0\n"
    "fbcx.ps f12, x4\n"
    "fmvs.x.ps x5, f5, 0\n"
    "fbcx.ps f13, x5\n"
    "fmvs.x.ps x6, f6, 0\n"
    "fbcx.ps f14, x6\n"
    "fmvs.x.ps x7, f7, 0\n"
    "fbcx.ps f15, x7\n"

      : 
      : [operands] "r" (operands)
      : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15"
      );

  }

C_TEST_PASS;

}

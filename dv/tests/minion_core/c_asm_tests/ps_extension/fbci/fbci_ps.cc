/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>

int main()
{

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
	"fbci.ps f0, 0x0\n"
	"fbci.ps f1, 0x80000\n"    
	"fbci.ps f2, 0x7fc00\n"
	"fbci.ps f3, 0xffc00\n"
	"fbci.ps f4, 0x7f800\n"
	"fbci.ps f5, 0xff800\n"        
	"fbci.ps f6, 0x4b8c4\n"  
	"fbci.ps f7, 0x4b000\n" 
	"fbci.ps f8, 0xc1a0c\n"
	"fbci.ps f9, 0xc1a4c\n" 
	"fbci.ps f10, 0x3fc7a\n"
	"fbci.ps f11, 0x40000\n"    
	"fbci.ps f12, 0x3f028\n"
	"fbci.ps f13, 0x3f800\n"
	"fbci.ps f14, 0xc0000\n"    
	"fbci.ps f15, 0xbf028\n"
	"fbci.ps f16, 0xbf8\n"     
	"fbci.ps f17, 0x800\n"    
	"fbci.ps f18, 0x7fc\n"
	"fbci.ps f19, 0xffc\n"
	"fbci.ps f20, 0x7f8\n"
	"fbci.ps f21, 0xff8\n"        
	"fbci.ps f22, 0x4b8\n"  
	"fbci.ps f23, 0x4b0\n" 
	"fbci.ps f24, 0xc1a0\n"
	"fbci.ps f25, 0xc1a4\n" 
	"fbci.ps f26, 0x3fc7\n"
	"fbci.ps f27, 0x4000\n"    
	"fbci.ps f28, 0x3f02\n"
	"fbci.ps f29, 0x3f80\n"
	"fbci.ps f30, 0xc000\n"    
	"fbci.ps f31, 0xbf02\n"

      : 
      :
      : "x2", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
      );

  }

C_TEST_PASS;

}

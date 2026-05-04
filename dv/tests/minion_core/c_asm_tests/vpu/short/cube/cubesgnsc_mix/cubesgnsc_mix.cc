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
   static uint32_t operands[88]  __attribute__ ((aligned (16)))= {
      0x3f800000, // 1.0 
      0x3f028f51, // 0.51
      0x3f800002, // 1.0
      0x3f028f53, // 0.51  
      0xbf800004, // -1.0 
      0xbf028f55, // -0.51
      0xbf800006, // -1.0i
      0xbf028f57, // -0.51  
      //-----------------------
      0x40000008, // 1.5 
      0x3fc7ae19, // 1.56
      0x4000000a, // 1.5
      0x3fc7ae1b, // 1.56
      0xbfc0000c, // -1.5 
      0xbfc7ae1d, // -1.56
      0xbfc0000e, // -1.5
      0xbfc7ae1f, // -1.56
      //-----------------------
      0x41a4cccc, // 20.6           
      0x41a0cccc, // 20.1           
      0x41a4cccd, // 20.6           
      0x41a0cccd, // 20.1   
      0xc1a4cccc, // -20.6           
      0xc1a0cccc, // -20.1           
      0xc1a4cccd, // -20.6           
      0xc1a0cccd, // -20.1  
      //-----------------------         
      0x4b000000, // 8388608.0
      0x4b8c4b40, // 18388608.0
      0x4b000001, // 8388608.0
      0x4b8c4b41, // 18388608.0
      0xcb000002, // -8388608.0
      0xcb8c4b42, // -18388608.0
      0xcb000003, // -8388608.0
      0xcb8c4b43, // -18388608.0
      //-----------------------
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x00011111, // 9.7958E-41
      0x007ffffe, // 1.1754942E-38
      0x8001111e, // -9.7958E-41
      0x8000000e, // -1.4E-45
      0x80011111, // -9.7958E-41
      0x007ffffe, // -1.1754942E-38
      //-----------------------
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN  
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN                 
      //-----------------------                
      0x7f800008, // inf
      0xff800009, // -inf
      0x7f80000a, // inf
      0xff80000b, // -inf
      0x0,  // zero
      0x8000000c,  // -zero
      0x0,  // zero
      0x8000000d,   // -zero
      //-----------------------                
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  
      0x0,  // zero
      0x80000000,  // -zero
      0x0,  // zero
      0x80000000,   // -zero
      //-----------------------                
      0x7fc00000, // quiet NaN                  
      0xffc00000, // -quiet NaN 
      0x7fc00001, // signaling NaN                  
      0xffc00001, // -signaling NaN  
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      //-----------------------                
      0x7f800000, // inf
      0xff800000, // -inf
      0x7f800000, // inf
      0xff800000, // -inf
      0x00011111, // 9.7958E-41
      0x00000001, // 1.4E-45
      0x80011111, // -9.7958E-41
      0x007fffff, // -1.1754942E-38
      //----------------------- 
      0x00011111, // 9.7958E-41
      0x007fffff, // 1.1754942E-38
      0x80011111, // -9.7958E-41
      0x80000001, // -1.4E-45
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
            //mask lanes
            "mova.m.x t3\n"

            "li t0, 0\n"
            "fcvt.s.w f0, t0\n"
            "li t1, 0\n"
            "fcvt.s.w f9, t1\n"

            //load operands to all fp registers
            "flw.ps f0, 0(%[operands])\n"
            "flw.ps f1, 16(%[operands])\n"    
            "flw.ps f2, 32(%[operands])\n"
            "flw.ps f3, 48(%[operands])\n"
            "flw.ps f4, 64(%[operands])\n"
            "flw.ps f5, 80(%[operands])\n"        
            "flw.ps f6, 96(%[operands])\n"  
            "flw.ps f7, 112(%[operands])\n" 
            "flw.ps f8, 128(%[operands])\n"
            "flw.ps f9, 144(%[operands])\n"        
            "flw.ps f10, 160(%[operands])\n"  
            "flw.ps f11, 176(%[operands])\n" 
            "flw.ps f12, 192(%[operands])\n"
            "flw.ps f13, 208(%[operands])\n"        
            "flw.ps f14, 224(%[operands])\n"  
            "flw.ps f15, 240(%[operands])\n" 
            "flw.ps f16, 8(%[operands])\n"
            "flw.ps f17, 24(%[operands])\n"
            "flw.ps f18, 40(%[operands])\n"        
            "flw.ps f19, 56(%[operands])\n"  
            "flw.ps f20, 72(%[operands])\n" 
            "flw.ps f21, 88(%[operands])\n"
            "flw.ps f22, 104(%[operands])\n" 
            "flw.ps f23, 120(%[operands])\n"
            "flw.ps f24, 136(%[operands])\n" 
            "flw.ps f25, 152(%[operands])\n"
            "flw.ps f26, 168(%[operands])\n" 
            "flw.ps f27, 184(%[operands])\n"
            "flw.ps f28, 200(%[operands])\n"
            "flw.ps f29, 216(%[operands])\n"
            "flw.ps f30, 232(%[operands])\n"
            "flw.ps f31, 0(%[operands])\n"


            "cubeface.ps f15, f1, f0\n"
            "cubefaceidx.ps f15, f15, f2\n"
            "cubesgnsc.ps f15, f15, f2\n"
            "cubeface.ps f10, f10, f15\n"
            "cubefaceidx.ps f15, f10, f2\n"
            "cubesgnsc.ps f10, f15, f10\n"
            "cubeface.ps f26, f21, f20\n"
            "cubefaceidx.ps f5, f26, f6\n"
            "cubesgnsc.ps f5, f5, f7\n"
            "cubeface.ps f7, f2, f5\n"
            "cubefaceidx.ps f2, f7, f8\n"
            "cubesgnsc.ps f8, f2, f8\n"
            "cubeface.ps f17, f2, f12\n"
            "cubefaceidx.ps f15, f17, f0\n"
            "cubesgnsc.ps f10, f15, f10\n"
            "cubeface.ps f3, f3, f3\n"
            "cubefaceidx.ps f4, f3, f4\n"
            "cubesgnsc.ps f4, f4, f10\n"
            "cubeface.ps f9, f9, f4\n"
            "cubefaceidx.ps f19, f9, f18\n"
            "cubesgnsc.ps f2, f19, f10\n"
            "cubeface.ps f16, f19, f2\n"
            "cubefaceidx.ps f30, f16, f30\n"
            "cubesgnsc.ps f10, f30, f10\n"
            "cubeface.ps f0, f13, f0\n"
            "cubefaceidx.ps f12, f0, f21\n"
            "cubesgnsc.ps f10, f12, f10\n"

            : 
            : [operands] "r" (operands)
            : "t3", "t0", "t1", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9"
               );
   }



   C_TEST_PASS;

}

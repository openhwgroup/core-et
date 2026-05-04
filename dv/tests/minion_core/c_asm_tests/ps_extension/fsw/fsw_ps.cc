/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test
   static uint32_t operands[64]  __attribute__ ((aligned (32))) = {
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
      0xfedcba98,
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
      0x77777777,
      0x44444444,
      0x44444444,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
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

            //"li t0, 0\n"
            //"fcvt.s.w f0, t0\n"
            //"li t1, 0\n"
            //"fcvt.s.w f9, t1\n"

            //load operands
            "flw.ps f1, 0(%[operands])\n"
            "flw.ps f2, 32(%[operands])\n"
            "flw.ps f3, 64(%[operands])\n"
            "flw.ps f4, 96(%[operands])\n"
            "flw.ps f5, 128(%[operands])\n"
            "flw.ps f6, 160(%[operands])\n"
            "flw.ps f7, 192(%[operands])\n"
            "flw.ps f8, 224(%[operands])\n"

            "fadd.pi f9, f1, f1\n"
            "fadd.pi f10, f2, f2\n"
            "fadd.pi f11, f3, f3\n"
            "fadd.pi f12, f4, f4\n"
            "fadd.pi f13, f5, f5\n"
            "fadd.pi f14, f6, f6\n"
            "fadd.pi f15, f7, f7\n"
            "fadd.pi f16, f8, f8\n"

            "fsw.ps f9, 0(%[operands])\n"
            "fsw.ps f10, 32(%[operands])\n"
            "fsw.ps f11, 64(%[operands])\n"
            "fsw.ps f12, 96(%[operands])\n"
            "fsw.ps f13, 128(%[operands])\n"
            "fsw.ps f14, 160(%[operands])\n"
            "fsw.ps f15, 192(%[operands])\n"
            "fsw.ps f16, 224(%[operands])\n"

            /* "flw.ps f9, 0(%[operands])\n"
               "flw.ps f10, 32(%[operands])\n"
               "flw.ps f11, 64(%[operands])\n"
               "flw.ps f12, 96(%[operands])\n"
               "flw.ps f13, 128(%[operands])\n"
               "flw.ps f14, 160(%[operands])\n"
               "flw.ps f15, 192(%[operands])\n"
               "flw.ps f16, 224(%[operands])\n" */

            "feq.ps f0, f1, f1\n"
            "feq.ps f0, f2, f2\n"
            "feq.ps f0, f3, f3\n"
            "feq.ps f0, f4, f4\n"
            "feq.ps f0, f5, f5\n"
            "feq.ps f0, f6, f6\n"
            "feq.ps f0, f7, f7\n"
            "feq.ps f0, f8, f8\n"
            "feq.ps f0, f9, f9\n"
            "feq.ps f0, f10, f10\n"
            "feq.ps f0, f11, f11\n"
            "feq.ps f0, f12, f12\n"
            "feq.ps f0, f13, f13\n"
            "feq.ps f0, f14, f14\n"
            "feq.ps f0, f15, f15\n"
            "feq.ps f0, f16, f16\n"

            "feq.ps f0, f1, f9\n"
            "feq.ps f0, f2, f10\n"
            "feq.ps f0, f3, f11\n"
            "feq.ps f0, f4, f12\n"
            "feq.ps f0, f5, f13\n"
            "feq.ps f0, f6, f14\n"
            "feq.ps f0, f7, f15\n"
            "feq.ps f0, f8, f16\n"
            : 
            : [operands] "r" (operands)
            : "x2", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
               );

   }

   C_TEST_PASS;

}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test
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
            //mask lanes
            "mova.m.x x2\n"

            //load operands
            "flq2 f1, 0(%[operands])\n"
            //"flq2 f2, 8(%[operands])\n"
            //"flq2 f3, 16(%[operands])\n"
            //"flq2 f4, 24(%[operands])\n"
            "fence\n"
            "fsq2 f1, 64(%[operands])\n"
            //"fsq2 f2, 72(%[operands])\n"
            //"fsq2 f3, 80(%[operands])\n"
            //"fsq2 f4, 88(%[operands])\n"
            "fence\n"
            "flq2 f9, 64(%[operands])\n"
            //"flq2 f10, 72(%[operands])\n"
            //"flq2 f11, 80(%[operands])\n"
            //"flq2 f12, 88(%[operands])\n"


            : 
            : [operands] "r" (operands)
            : "x2", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16"
               );

   }

   C_TEST_PASS;

}

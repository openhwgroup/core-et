/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>



int main()
{

   C_TEST_START;

   //test
   static uint32_t operands[32]  __attribute__ ((aligned (32)))= {
      0xcdcdcdcd,
      0x00008000,
      0x00000000,
      0x0fff0fff,
      0xf000f000,
      0xaaaaaaaa,
      0x7fff7fff,
      0xffffffff
   };

   uint64_t addr = 0x8001000000;
   uint64_t tmp;

   uint64_t d_addr = 0x8001100000;
   uint64_t* d_ptr = (uint64_t*)0x8001100000;

   for (uint32_t i=0; i<7; i=i+2) {
     tmp = operands[i+1];
     *d_ptr = (tmp << 32) | operands[i];
     d_ptr++;
   }

   uint64_t f_addr = 0x8001200000;
   uint64_t* f_ptr = (uint64_t*)0x8001200000;

   for (uint32_t i=0; i<7; i=i+2) {
    tmp = (i + 1) * 4;
     *f_ptr = (tmp<<32) | (i * 4);
     f_ptr++;
   }

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
            "mv t1, %[addr]\n"

            "flw.ps f2, 0(%[d_addr])\n"
            "flw.ps f0, 0(%[f_addr])\n"

            //load operands
            "fsch.ps f2, f0(t1)\n"
            "fence\n"

            : 
            : [operands] "r" (operands), [addr] "r" (addr), [d_addr] "r" (d_addr), [f_addr] "r" (f_addr)
            : "f0", "f1", "f2", "f3", "t0", "t1", "t2", "t3"
      );

      d_ptr = (uint64_t*)addr;
      for (uint32_t j=0; j<16; j++) {
         tmp = *d_ptr;
         d_ptr++;
      }

      addr = addr + 100;

   }

   C_TEST_PASS;

}

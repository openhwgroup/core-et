/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "compare.h"

int main()
{
   volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   for (uint32_t j=0; j<10; j++) 
   {  //begin

      if (j==0) {
         __asm__ __volatile__ (
               //enable lanes
               "li t3, 0x03\n"
               : 
               :
               : "t3"
               );
      } else if (j==1) {
         __asm__ __volatile__ (
               //enable lanes
               "li t3, 0x0c\n"
               : 
               :
               : "t3"
               );
      } else if (j==2) {
         __asm__ __volatile__ (
               //enable lanes
               "li t3, 0x30\n"
               : 
               :
               : "t3"
               );
      }  else if (j==3) {
         __asm__ __volatile__ (
               "li t3, 0xc0\n"
               : 
               :
               : "t3"
               );
      }  else if (j==4) {
         __asm__ __volatile__ (
               "li t3, 0xff\n"
               : 
               :
               : "t3"
               );
      }  else if (j==5) {
         __asm__ __volatile__ (
               "li t3, 0x33\n"
               : 
               :
               : "t3"
               );
      }   else if (j==6) {
         __asm__ __volatile__ (
               "li t3, 0xc3\n"
               : 
               :
               : "t3"
               );
      } else if (j==7) {
         __asm__ __volatile__ (
               "li t3, 0x3c\n"
               : 
               :
               : "t3"
               );

      } else if (j==8) {
         __asm__ __volatile__ (
               "li t3, 0xcc\n"
               : 
               :
               : "t3"
               );
      } else if (j==9) {
         __asm__ __volatile__ (
               "li t3, 0x0\n"
               : 
               :
               : "t3"
               );
      } 

      // Test
      __asm__ __volatile__ (

            // Init and set all cachelines to modified
            "addi t0, %[ptr], 0\n"
            "addi t1, zero, 5\n"        // Start value of matrix
            "addi t2, zero, 256\n"      // Loop iterations
            "loop_sw:\n"
            "sw   t1, 0(t0)\n"          // Store
            "addi t1, t1, 1\n"          // Inc value
            "addi t0, t0, 4\n"          // Inc ptr
            "addi t2, t2, -1\n"         // Dec counter
            "bne  t2, zero, loop_sw\n"

            // Setting mask for the lanes
            //"li t3, 0xff\n"
            "mova.m.x t3\n"
            :
            : [ptr] "r" (ptr)
            : "t0", "t1", "t2"
            );

      // Init fp regs
      __asm__ __volatile__ (
            "addi t0, %[ptr], 0\n" : :[ptr] "r" (ptr): "t0"
            );

      uint8_t i=0;
      uint8_t l=1;
      uint8_t p=0;
      while (i<16) {
         __asm__ __volatile__ (
               "flw f0, 0(t0)\n" : : 
               );
         __asm__ __volatile__ (
               "addi t0, t0, 4\n" : : 
               );

         __fcvt("fcvt.ps.pw",l,p);
         i++;
         l++;
      }

      while (i<31) {
         __asm__ __volatile__ (
               "flw f0, 0(t0)\n" : : 
               );
         __asm__ __volatile__ (
               "addi t0, t0, 4\n" : : 
               );

         __fcvt("fcvt.ps.pw",l,p);
         i++;
         l++;
      }


      // Instruction - circular test, dst becomes src
      __compare("feq.ps",3,2,1);
      __compare("feq.ps",5,4,3);
      __compare("feq.ps",7,6,5);
      __compare("feq.ps",9,8,7);
      __compare("feq.ps",11,10,9);
      __compare("feq.ps",13,12,11);
      __compare("feq.ps",15,14,13);
      __compare("feq.ps",17,16,15);
      __compare("feq.ps",19,18,17);
      __compare("feq.ps",21,20,19);
      __compare("feq.ps",23,22,21);
      __compare("feq.ps",25,24,23);
      __compare("feq.ps",27,26,25);
      __compare("feq.ps",29,28,27);
      __compare("feq.ps",31,30,29);



   } // end for

   C_TEST_PASS;
   return 0;

}

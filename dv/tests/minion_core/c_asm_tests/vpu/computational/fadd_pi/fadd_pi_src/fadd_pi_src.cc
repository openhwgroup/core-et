/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

#include "computational.h"


int main()
{
   volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   // Test
   __asm__ __volatile__ (

         // Init and set all cachelines to modified
         "addi t0, %[ptr], 0\n"
         "addi t1, zero, 0\n"        // Start value of matrix
         "addi t2, zero, 256\n"      // Loop iterations
         "loop_sw:\n"
         "sw   t1, 0(t0)\n"          // Store
         "addi t1, t1, 5\n"          // Inc value
         "addi t0, t0, 4\n"          // Inc ptr
         "addi t2, t2, -1\n"         // Dec counter
         "bne  t2, zero, loop_sw\n"

         "li x2, 0xff\n"
         "mova.m.x x2\n" 

         :
         : [ptr] "r" (ptr)
         : "t0", "t1", "t2"
         );

   // Init fp regs
   __asm__ __volatile__ (
         "addi t0, %[ptr], 0\n" : :[ptr] "r" (ptr): "t0"
         );
   uint8_t i=0;
   while (i<16) {
      __feed_fp("flw",i);
      __asm__ __volatile__ (
            "addi t0, t0, 4\n" : : 
            );
      i++;
   }
   while (i<32) {
      __feed_fp("flw",i);
      __asm__ __volatile__ (
            "addi t0, t0, 4\n" : : 
            );
      i++;
   }

   // Instruction - circular test, dst becomes src
   __computational("fadd.pi",2,1,0);
   __computational("fadd.pi",4,3,2);
   __computational("fadd.pi",6,5,4);
   __computational("fadd.pi",8,7,6);
   __computational("fadd.pi",10,9,8);
   __computational("fadd.pi",12,11,10);
   __computational("fadd.pi",14,13,12);
   __computational("fadd.pi",16,15,14);
   __computational("fadd.pi",18,17,16);
   __computational("fadd.pi",20,19,18);
   __computational("fadd.pi",22,21,20);
   __computational("fadd.pi",24,23,22);
   __computational("fadd.pi",27,26,25);
   __computational("fadd.pi",29,28,27);
   __computational("fadd.pi",31,30,29); 


   /*
      __asm__ __volatile__ (
      "fadd.pi f2,f1,f0\n"
      "fadd.pi f4,f3,f2\n"
      "fadd.pi f6,f5,f4\n"
      "fadd.pi f8,f7,f6\n"
      "fadd.pi f10,f9,f8\n"
      "fadd.pi f12,f11,f10\n"
      "fadd.pi f14,f13,f12\n"
      "fadd.pi f16,f15,f14\n"
      "fadd.pi f18,f17,f16\n"
      "fadd.pi f20,f19,f18\n"
      "fadd.pi f22,f21,f20\n"
      "fadd.pi f24,f23,f22\n"
      "fadd.pi f27,f26,f25\n"
      "fadd.pi f29,f28,f27\n"
      "fadd.pi f31,f30,f29\n"

      :
      : 
      : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", \
      "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", \
      "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", \
      "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
      );
      */


   C_TEST_PASS;
   return 0;
}

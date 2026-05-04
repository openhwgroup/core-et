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

   // Instruction - dst becomes src & src is dst
   __computational("fadd.pi",2,1,0);
   __computational("fadd.pi",4,3,2);
   __computational("fadd.pi",4,5,4);
   __computational("fadd.pi",8,7,6);
   __computational("fadd.pi",10,9,8);
   __computational("fadd.pi",10,11,10);
   __computational("fadd.pi",14,13,12);
   __computational("fadd.pi",16,15,14);
   __computational("fadd.pi",16,17,16);
   __computational("fadd.pi",20,19,18);
   __computational("fadd.pi",22,21,20);
   __computational("fadd.pi",22,23,22);
   __computational("fadd.pi",26,25,24);
   __computational("fadd.pi",28,27,26);
   __computational("fadd.pi",28,29,28);
   __computational("fadd.pi",31,30,29);



   C_TEST_PASS;
   return 0;
}

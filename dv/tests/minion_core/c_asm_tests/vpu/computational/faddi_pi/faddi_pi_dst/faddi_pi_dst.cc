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


   __asm__ __volatile__ (

         "faddi.pi f31,f1,-2\n"
         "faddi.pi f31,f2,-15\n"
         "faddi.pi f31,f3,2\n"
         "faddi.pi f31,f4,0xff\n"
         "faddi.pi f31,f5,0xf\n"
         "faddi.pi f31,f6,-2\n"
         "faddi.pi f31,f7,-15\n"
         "faddi.pi f31,f8,0xff\n"
         "faddi.pi f31,f9,0xf\n"
         "faddi.pi f31,f10,-2\n"
         "faddi.pi f31,f11,-15\n"
         "faddi.pi f31,f12,0xff\n"
         "faddi.pi f31,f13,0xf\n"
         "faddi.pi f31,f14,-2\n"
         "faddi.pi f31,f15,-15\n"
         "faddi.pi f31,f16,0xff\n"
         "faddi.pi f31,f17,0xf\n"
         "faddi.pi f31,f18,-2\n"
         "faddi.pi f31,f19,-15\n"
         "faddi.pi f31,f20,0xff\n"
         "faddi.pi f31,f21,0xf\n"
         "faddi.pi f31,f22,-2\n"
         "faddi.pi f31,f23,-15\n"
         "faddi.pi f31,f24,0xff\n"
         "faddi.pi f31,f25,0xf\n"
         "faddi.pi f31,f26,-2\n"
         "faddi.pi f31,f27,-15\n"
         "faddi.pi f31,f28,0xff\n"
         "faddi.pi f31,f29,0xf\n"
         "faddi.pi f31,f30,-2\n"
         );

   C_TEST_PASS;
   return 0;
}

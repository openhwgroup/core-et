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

   // circular test
   __asm__ __volatile__ (

         "faddi.pi f1,f0,-2\n"
         "faddi.pi f2,f1,-15\n"
         "faddi.pi f3,f2,2\n"
         "faddi.pi f4,f3,0xff\n"
         "faddi.pi f5,f4,0x2\n"
         "faddi.pi f6,f5,0xf\n"
         "faddi.pi f7,f6,-2\n"
         "faddi.pi f8,f7,-15\n"
         "faddi.pi f9,f8,0xff\n"
         "faddi.pi f10,f9,0xf\n"
         "faddi.pi f11,f10,-2\n"
         "faddi.pi f12,f11,-15\n"
         "faddi.pi f13,f12,0xff\n"
         "faddi.pi f14,f13,0xf\n"
         "faddi.pi f15,f14,-2\n"
         "faddi.pi f16,f15,-15\n"
         "faddi.pi f17,f16,0xff\n"
         "faddi.pi f18,f17,0xf\n"
         "faddi.pi f19,f18,-2\n"
         "faddi.pi f20,f19,-15\n"
         "faddi.pi f21,f20,0xff\n"
         "faddi.pi f22,f21,0xf\n"
         "faddi.pi f23,f22,-2\n"
         "faddi.pi f24,f23,-15\n"
         "faddi.pi f25,f24,0xff\n"
         "faddi.pi f26,f25,0xf\n"
         "faddi.pi f27,f26,-2\n"
         "faddi.pi f28,f27,-15\n"
         "faddi.pi f29,f28,0xff\n"
         "faddi.pi f30,f29,0xf\n"
         "faddi.pi f31,f30,-2\n"
         );

   C_TEST_PASS;
   return 0;
}

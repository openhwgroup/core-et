/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"

#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   //srand(time(NULL));
   for (int i=1; i<10; i++) {  
      int r = 30999*i + 1234*i;
      int dest = r & 0x7;
      int src1 = (r >> 3) & 0x7; 
      int src2 = (r >> 6) & 0x7;


      // Test
      __asm__ __volatile__ (
            "addi  t0, zero, %3\n"		//SEED
            "li  t1, 0xABCDABCDABCDABCD\n"
            "li  t2, 0x1234567812345678\n"
            "mul t1, t0, t1\n"
            "mul t2, t0, t2\n"
            "add t1, t1, t2\n"			//Keep generated random number in t1

            "mova.m.x t1\n"					//Write in mask registers


            "fsetm.pi m%0, f%1\n"
            "fsetm.pi m%1, f%0\n"
            "fsetm.pi m%0, f%1\n"

            "fsetm.pi m%0, f%1\n"
            "fsetm.pi m%1, f%1\n"
            "fsetm.pi m%0, f%0\n"

            "fsetm.pi m%0, f%1\n"
            "fsetm.pi m%0, f%0\n"
            "fsetm.pi m%0, f%1\n"

            :
            : "n" (dest), "n" (src1), "n" (src2), "n" (i)
            : "t0", "t1", "t2", "t3", "t4", "t5", "t6"
               );

   }

   C_TEST_PASS;
   return 0;
}

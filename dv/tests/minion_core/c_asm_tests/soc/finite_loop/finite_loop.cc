/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef ITERATIONS
  #define ITERATIONS 5
#endif

inline __attribute__((naked)) void mtrap_vector(void)
{
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vector%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         "csrw mepc, s3\n\t"
         "mret\n\t"
         :::
   );
}

inline void setup_mtrap_vector(void)
{
    void (*mtvec_ptr)(void) = &mtrap_vector;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}

int main()
{
   C_TEST_START;
   setup_mtrap_vector();

         // Test
      __asm__ __volatile__ (
            "addi a1, zero, 13\n"
            "addi a2, zero, 1\n"
	    "li a3, %[LOOP_END]\n"
            "addi a4, zero, 0\n"
            "loop:\n"
            "div  a1, a1, a2\n"
            "add  a4, a1, a1\n"
            "add  a3, a3, -1\n"
            "bne  a3, zero, loop\n"            
	    : 
            : [LOOP_END] "i" (ITERATIONS)
            : "a1", "a2", "a3", "a4"
            );

   

   C_TEST_PASS;
   return 0;
}

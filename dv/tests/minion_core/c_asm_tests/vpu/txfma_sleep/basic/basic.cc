/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"

int main()
{
   volatile char __attribute__((aligned(4096))) ptr[4096];

   float * f = (float *) ptr;
   float value = 1.0f;


   for(int vect = 0; vect < 4; vect++)
   {
      for(int lane = 0; lane < 8; lane++)
      {
         * f = value;
         value = value + 0.1f;
         f++;
      }
      value = vect + 1.0f;
   }
   f = (float *) ptr;

   C_TEST_START;
   __asm__ __volatile__ (

         // Enable 8 lanes
         "li x2, 0xff\n"
         "mova.m.x x2\n"

         // Disable TXFMA lanes 2..7
         "csrwi  0x7d1, 1\n"

         // Wait just in case
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"

         // Load to f0..3
         "flw.ps f0, 0(%[f])\n"
         "flw.ps f1, 32(%[f])\n"
         "flw.ps f2, 64(%[f])\n"
         "flw.ps f3, 96(%[f])\n"

         // Execute 1 TXFMA op
         "fadd.ps f0, f0, f1\n"
         //RTLMIN-5479"fsin.ps f1, f1\n"
         "fadd.ps f2, f2, f3\n"
         "fexp.ps f3, f3\n"
         "fsub.ps f0, f0, f2\n"

         // End marker
         :
         : [f] "r" (f)
         :
            );

   // f = (float *) ptr;
   // for(int vect = 0; vect < 4; vect++)
   // {
   //     for(int lane = 0; lane < 8; lane++)
   //     {
   //         printf("%f\n", * f);
   //         f++;
   //     }
   // }

   C_TEST_PASS;
   return 0;
}

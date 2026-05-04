/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

void mult_rtl_fail() {
   uint64_t result;
   
   asm volatile (
       "li     x27, 6\n\t"        // s11 = 6
       "lui    x29, 0x1\n\t"      // t4 = 0x100000
       "addiw  x29, x29, 128\n\t" // t4 = 0x100080
       "mul    x29, x29, x27\n\t" // t4 *= s11
       "mv     %0, x29\n\t"       // move result out
       : "=r"(result)
       :
       : "x27", "x29"
   );
}

void rem_model_fail() {
   uint64_t result;
   
   asm volatile (
       "li     x13, 0x8094ca31b0918c26\n\t"   // a3 = constant
       "addiw  x29, x0, 1\n\t"                // t4 = 1
       "slli   x29, x29, 48\n\t"              // t4 <<= 48
       "addi   x29, x29, -64\n\t"             // t4 -= 64
       "and    x24, x24, x29\n\t"             // s8 &= t4
       "li     x28, 15\n\t"                   // t3 = 15
       "or     x28, x28, x24\n\t"             // t3 |= s8
       "li     x31, 65\n\t"                   // t6 = 65
       "rem    x11, x29, x13\n\t"             // a1 = t4 % a3
       "mv     %0, x11\n\t"                   // output result
       : "=r"(result)
       :
       : "x11", "x13", "x24", "x28", "x29", "x31"
   );
}

int main() {

   // Init marker
   C_TEST_START;

   uint64_t tid;
   uint64_t mid;

   // Get minion ID and thread id
   tid = get_thread_id();
   mid = get_minion_id();

   // Only thread 0 executes this
   if (mid == 0 and tid == 0) {
      mult_rtl_fail();
      rem_model_fail();
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

int main() {

   /*
   // Init marker
   C_TEST_START;
   */

   for (int i = 0; i < 5*1024*1024; i++) {
      /* LCG */
      uint64_t seed = 12;

      uint64_t x;
      __asm__ __volatile__ ("li %[x], 164603309694725029ul" : [x] "=r" (x) : : );

      uint64_t y;
      __asm__ __volatile__ ("li %[y], 14738995463583502973ul" : [y] "=r" (y) : : );

      for (int i = 0; i < 27; i++) {
         for (int b = 0; b < 8; b++) {
            seed = (x * seed) % y;
         }
      }

      /* RV Hint */
      __asm__ __volatile__ (
         "slti x0, %[seed], 0x516"
            :
            : [seed] "r" (seed)
            :
      );

      /* Gold ref computed using sys_emu */
      uint64_t gold;
      __asm__ __volatile__ ("li %[gold], 0xb1faccfe85e59c31" : [gold] "=r" (gold) : : );

      if (seed != gold) {
         C_TEST_FAIL;
      }
   }

   C_TEST_PASS;
   return 0;
}

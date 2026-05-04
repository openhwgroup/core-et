/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "minion.h"

int main()
{
   minion_dcache_t l1d;
   C_TEST_START;

   if (get_hart_id() == 0) {

      // Initialize array
      for (uint32_t i = 0; i < 64; ++i) {
         l1d.cl[i].w[0] = i+1;
      }
      excl_mode(1);
      // Go in L1D split mode
      // Cache should be zeroed out
      mcache_control(1, 0, 0);
      excl_mode(0);

      // Self check
      for (uint32_t i = 0; i < 64; ++i) {
         if (l1d.cl[i].w[0] == i+1) {
            C_TEST_FAIL;
         }
      }

      excl_mode(1);
      // Initialize again
      // we only have 8 sets now
      for (uint32_t i = 0; i < 32; ++i) {
         l1d.cl[i].w[0] = i+1;
      }

      excl_mode(1);
      // Writing L1D split mode if already enabled
      // should not zero anything
      mcache_control(1, 0, 0);
      excl_mode(0);

      // Self check
      for (uint32_t i = 0; i < 32; ++i) {
         if (l1d.cl[i].w[0] != i+1) {
            C_TEST_FAIL;
         }
      }

      excl_mode(1);
      // Go back to shared mode, cache should be zeroed again
      mcache_control(0, 0, 0);
      excl_mode(0);

      // Self check
      for (uint32_t i = 0; i < 64; ++i) {
         if (l1d.cl[i].w[0] == i+1) {
            C_TEST_FAIL;
         }
      }

      C_TEST_PASS;
   }
}

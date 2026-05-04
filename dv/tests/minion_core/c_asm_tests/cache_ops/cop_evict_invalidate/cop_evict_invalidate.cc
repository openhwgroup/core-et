/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

int main()
{
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }


   // Evict cache
   for (unsigned way = 0; way < 16; ++way) {
      for (unsigned set = 0; set < 16; ++set) {
         evict_sw(false, to_L2, way, set);
      }
   }
   WAIT_CACHEOPS;

   // Initialize array second time
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i+1; 
   }

   // EvictVA cache
   for (unsigned i = 0; i < 4; i++) {
      //	      use_mask dst    address                         num_lines  stride  id
      //	      -------- ------ ------------------------------  ---------  ------  --
      evict_va(false,   to_L2, (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
   }
   WAIT_CACHEOPS;

   C_TEST_PASS;
}


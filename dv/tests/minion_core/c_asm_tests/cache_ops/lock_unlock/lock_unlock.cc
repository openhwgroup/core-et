/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 0, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 1, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 2, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 3, set,   0       );
   }
   WAIT_CACHEOPS;

   for (int i = 0; i < 16; i++) {

      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(16*i)), 0,         0,      0 );
   }

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;

   // Unlock L1 lines (FUTURE: Emmanuel Marie numLines not supported yet, so looping manually)
   for (int i = 0; i < 16; i++) {

      //        use_tmask  addr                           num_lines  stride  id
      //        ---------  ---------------------------    ---------  ------  --
      unlock_va(false,     (uint64_t)(void*)(ptr+(16*i)), 0,         0,      0 );
   }

   WAIT_CACHEOPS;

   C_TEST_PASS;
}

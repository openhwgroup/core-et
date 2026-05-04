/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"

int main()
{
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }

   for(int i = 0; i < 4; i++)
   {
      // Evict L1 lines.
      //       use_tmask dst        addr     				             num_lines stride id
      //       --------- ---        ------------------------------  --------- ------ --
      evict_va(false,    to_L2,    (uint64_t)(void*)(ptr+(i*256)), 15,       0,    0 );
      WAIT_CACHEOPS;
   }

   // Initialize array
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }

   for(int i = 0; i < 4; i++)
   {
      // Flush L1 lines
      //       use_tmask dst     addr                            num_lines stride id
      //       --------- ---     -----------------------------   --------- ------ --
      flush_va(false,    to_L2,  (uint64_t)(void*)(ptr+(i*256)), 15,       0,    0 );
      WAIT_CACHEOPS;
   }

   // Lock L1 lines
   for (int i = 0; i < 4; i++)
   {
      //      use_tmask  addr                             num_lines  stride id
      //      ---------  ------------------------------   ---------  ------ --
      lock_va(false,      (uint64_t)(void*)(ptr+(i*256)),  15,        0,     0 );
      WAIT_CACHEOPS;
   }


   // UnLock L1 lines
   for (int i = 0; i < 4; i++)
   {
      //        use_tmask  keep_valid  addr                             num_lines  stride  id
      //        ---------  ----------  ------------------------------   ---------  ------  --
      unlock_va(false,      1,          (uint64_t)(void*)(ptr+(i*256)),  15,        0,     0 );
      WAIT_CACHEOPS;
   }


   for(int i = 0; i < 4; i++)
   {
      // Prefetch L1 lines
      //          use_tmask dst src addr                           num_lines stride id
      //          --------- --- --- -----------------------------  --------- ------ --
      prefetch_va(false,    0,  0, (uint64_t)(void*)(ptr+(i*256)), 15,       0,     0 );
      WAIT_CACHEOPS;
   }

   /*
   // Initialize array
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i;
   }


   for(int i = 0; i < 4; i++)
   {
   // Flush L1 lines
   //       use_tmask dst addr                            num_lines stride id
   //       --------- --- -----------------------------   --------- ------ --
   flush_va(false,    1,  (uint64_t)(void*)(ptr+(i*256)), 15,       0,    0 );
   WAIT_CACHEOPS;
   }


   for(int i = 0; i < 4; i++)
   {
   // Prefetch L1 lines
   //          use_tmask dst  addr                           num_lines stride id
   //          --------- ---  -----------------------------  --------- ------ --
   prefetch_va(false,    0,  (uint64_t)(void*)(ptr+(i*256)), 15,       0,     0 );
   WAIT_CACHEOPS;
   }
   */
   C_TEST_PASS;
}

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


   // Enable scratchpad
   //  WARL  DEscratchpad
   //  ----  ------------
   scp(0x00, 0x1       );
   WAIT_CACHEOPS;

   /*
   // Lock L1 lines
   for (int i = 0; i < 4; i++)
   {
   //      use_tmask  addr                             num_lines  stride id
   //      ---------  ------------------------------   ---------  ------ --
   lock_va(false,      (uint64_t)(void*)(ptr+(i*256)),  11,        64,     0 );
   WAIT_CACHEOPS;
   }
   */
   for(int i = 0; i < 4; i++)
   {
      // Prefetch L1 lines
      //          use_tmask dst     addr                           num_lines stride id
      //          --------- ---     -----------------------------  --------- ------ --
      prefetch_va(false,    to_L1,  (uint64_t)(void*)(ptr+(i*256)), 15,       64,     0 );
      WAIT_PREFETCH_0;
   }

   C_TEST_PASS;
}

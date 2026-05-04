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

   // Lock L1 lines (FUTURE: Emmanuel Marie numLines not supported yet, so looping manually)
   for (int i = 0; i < 16; i++) {

      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(32*i)), 0,         0,      0 );
   }

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;

   lock_va(false,     (uint64_t)(void*)(ptr), 0,         0,      0 );

   WAIT_CACHEOPS;
   /*    // Initialize array
         for (int i = 0; i < 256; ++i) {
         ptr[i] = i;
         }
         */
   /*            // Evict L1 lines.
   //      use_tmask  dst       addr                                      num_lines stride id
   //      ---------  ---       --------------------  --------- ------ --
   evict_va(false,     to_L2,    (uint64_t)(void*)ptr, 15,       64,    0 );
   */

   // Evict L1 lines
   //       use_tmask  dst  way  set  num_lines
   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     to_L2, 0, set,   0       );
   }
   WAIT_CACHEOPS;
   // EvictSW L1 lines.
   //            ---------  ---   ---  --------------------  --------- ------ --
   //            evict_sw(false,     to_L2,   0, (uint64_t)(void*)ptr, 15,       64,    0 );

   //    WAIT_CACHEOPS;








   // Unlock L1 lines (FUTURE: Emmanuel Marie numLines not supported yet, so looping manually)
   for (int i = 0; i < 16; i++) {

      //        use_tmask  addr                           num_lines  stride  id
      //        ---------  ---------------------------    ---------  ------  --
      unlock_va(false,     (uint64_t)(void*)(ptr+(32*i)), 0,         0,      0 );
   }

   WAIT_CACHEOPS;

   // Evict L1 lines
   //       use_tmask  dst  way  set  num_lines
   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     to_L2, 0, set,   0       );
   }
   WAIT_CACHEOPS;


   /*
   // Initialize array
   for (int i = 0; i < 256; ++i) {
   ptr[i] = i+1;
   }

   // Evict L1 lines.
   //      use_tmask  dst   addr                                  num_lines stride id
   //      ---------  ---   --------------------  --------- ------ --
   evict_va(false,     1,    (uint64_t)(void*)ptr, 15,       64,    0 );

   WAIT_CACHEOPS;


   // Read scratchpad and self check
   for (int i = 0; i < 256; ++i) {
   if (ptr[i] != i) {
   test_fail();
   }
   }
   */
   C_TEST_PASS;
}

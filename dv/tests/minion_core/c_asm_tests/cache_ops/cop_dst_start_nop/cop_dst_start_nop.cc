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
   unsigned j = 0;
   unsigned k = 0;

   ////////////////////////// EVICTSW ///////////////////////////////
   // Initialize array
   j++;
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i+j;
   }
   FENCE;

   // Evict cache
   for (unsigned way = 0; way < 4; way++) {
      evict_sw(false, to_L1, way, 0x0, 15);
   }
   WAIT_CACHEOPS;

   ////////////////////////// FLUSHSW ///////////////////////////////
   // Initialize array
   j++;
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i+j;
   }
   FENCE;

   // Evict cache
   for (unsigned way = 0; way < 4; way++) {
      flush_sw(false, to_L1, way, 0x0, 15);
   }
   WAIT_CACHEOPS;

   ////////////////////////// EVICTVA ///////////////////////////////
   for (unsigned dst = 0; dst <  4; dst++)
   {
      j++;
      // Initialize array
      for (int i = 0; i < 1024; ++i) {
         ptr[i] = i+j;
      }
      FENCE;

      // EvictVA cache from L1 to L1
      for (unsigned i = 0; i < 4; i++) {
         //	use_mask dst   address                         num_lines  stride  id
         //	-------- ---   ------------------------------  ---------  ------  --
         evict_va(false,   dst,  (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
         WAIT_CACHEOPS;
      }

   }

   ////////////////////////// FLUSHVA ///////////////////////////////
   for (unsigned dst = 0; dst <  4; dst++)
   {
      k++;
      // Initialize array
      for (int i = 0; i < 1024; ++i) {
         ptr[i] = i+k;
      }
      FENCE;

      // EvictVA cache from L1 to L1
      for (unsigned i = 0; i < 4; i++) {
         //	use_mask dst   address                         num_lines  stride  id
         //	-------- ---   ------------------------------  ---------  ------  --
         flush_va(false,   dst,  (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
         WAIT_CACHEOPS;
      }
   }

   C_TEST_PASS;
}

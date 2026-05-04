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
   unsigned dst1;
   unsigned dst2;
   unsigned dst3;
   unsigned dst4;
   unsigned start3;
   unsigned start4;

   /*
   // Initialize array
   for (int i = 0; i < 1024; ++i) {
   ptr[i] = i;
   }
   FENCE;
   // Evict cache
   for (unsigned way = 0; way < 4; way++) {
   evict_sw(false,  0x2, way, 0x0,   15       );
   WAIT_CACHEOPS;
   }
   */




   ////////////////////////////////// EVICTSW ////////////////////////////////////
   for(dst1 = 1; dst1 < 4; dst1++)
   {
      j++;
      // Initialize array
      for (int i = 0; i < 1024; ++i) {
         ptr[i] = i+j;
      }
      FENCE;

      // Evict cache
      for (unsigned way = 0; way < 4; way++) {
         evict_sw(false,  dst1, way, 0x0,   15       );
      }
      WAIT_CACHEOPS;
   }


   ////////////////////////////////// FLUSHSW ////////////////////////////////////
   for(dst2 = 1; dst2 < 4; dst2++)
   {
      j++;
      // Initialize array
      for (int i = 0; i < 1024; ++i) {
         ptr[i] = i+j;
      }
      FENCE;

      // Evict cache
      for (unsigned way = 0; way < 4; way++) {
         flush_sw(false,  dst2, way, 0x0,   15       );
      }
      WAIT_CACHEOPS;
   }


   ////////////////////////////////// EVICTVA ////////////////////////////////////
   for(start3 = 0; start3 < 3; start3++)
   {
      for(dst3 = start3 + 1; dst3 < 4; dst3++)
      {
         j++;
         // Initialize array
         for (int i = 0; i < 1024; i++) {
            ptr[i] = i+j;
         }
         FENCE;

         // EvictVA cache
         for (unsigned i = 0; i < 4; i++) {
            //	   use_mask dst   address                         num_lines  stride  id
            //	   -------- ---   ------------------------------  ---------  ------  --
            evict_va(false,   dst3,  (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
         }
         WAIT_CACHEOPS;
      }
   }

   ////////////////////////////////// FLUSHVA ////////////////////////////////////
   for(start4 = 0; start4 < 3; start4++)
   {
      for(dst4 = start4 + 1; dst4 < 4; dst4++)
      {
         j++;
         // Initialize array
         for (int i = 0; i < 1024; i++) {
            ptr[i] = i+j;
         }
         FENCE;

         // flushVA cache
         for (unsigned i = 0; i < 4; i++) {
            //	  use_mask  dst   address                         num_lines  stride  id
            //	  --------  ----- ------------------------------  ---------  ------  --
            flush_va(false,   dst4, (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
         }
         WAIT_CACHEOPS;
      }
   }


   C_TEST_PASS;
}

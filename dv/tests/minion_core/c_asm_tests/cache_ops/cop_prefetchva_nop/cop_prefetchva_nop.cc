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

   // --------------------------------------------------------------------------------------------
   // PREFETCH TO L1
   // --------------------------------------------------------------------------------------------

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }

   // EvictSW whole cache to L2
   for (int way = 0; way < 4; way++)
   {
      //       Tensor mask dst  way  set    Num_lines 
      evict_sw(false     , 0x1, way, 0x0,   15       );
      WAIT_CACHEOPS;
   }

   // Prefetch 64 lines from MEM = 11 (3)
   for (int j = 0; j < 4; j++)
   { 
      prefetch_va(false,     3,  0,    (uint64_t)(void*)(ptr + (j*256)),  15,         64,      0 );
      WAIT_PREFETCH_0;
   }

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }
   C_TEST_PASS;
}

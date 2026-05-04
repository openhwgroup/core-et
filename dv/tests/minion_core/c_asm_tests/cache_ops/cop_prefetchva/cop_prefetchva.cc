/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[256];
   // Init marker
   C_TEST_START;

   // Initialize array
   for (int i = 0; i < 256; ++i) {
      ptr[i] = i; 
   }

   // Evict cache
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


   // Prefetch 16 lines 
   prefetch_va(false,     0,  0,    (uint64_t)(void*)(ptr),  15,         64,      0 );
   WAIT_PREFETCH_0;

   C_TEST_PASS;
}

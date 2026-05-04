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

   // Prefetch 64 lines
   for (int j = 0; j < 4; j++)
   { 
      prefetch_va(false,     0,  0,    (uint64_t)(void*)(ptr + (j*256)),  15,         64,      0 );
      WAIT_PREFETCH_0;
   }

   C_TEST_PASS;
}

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

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }


   /////////////////////////////////////////////////////////////////////////////////////////
   // Unlock L1 lines 
   //for (int i = 0; i < 4; i++) {

   //        use_tmask  keepValid  addr                           num_lines  stride  id
   //        ---------  ---------  ---------------------------    ---------  ------  --
   unlock_va(false,     1,         (uint64_t)(void*)(ptr),        15,        64,      0 );
   //}

   WAIT_CACHEOPS;

   // Unlock L1 lines 
   //        use_tmask  keepValid  addr                           num_lines  stride  id
   //        ---------  ---------  ---------------------------    ---------  ------  --
   unlock_va(false,     1,         (uint64_t)(void*)(ptr+(256)),  15,        64,      0 );
   WAIT_CACHEOPS;

   // Unlock L1 lines 
   //        use_tmask  keepValid  addr                           num_lines  stride  id
   //        ---------  ---------  ---------------------------    ---------  ------  --
   unlock_va(false,     1,         (uint64_t)(void*)(ptr+(512)),  15,        64,      0 );
   WAIT_CACHEOPS;

   // Unlock L1 lines 
   //        use_tmask  keepValid  addr                           num_lines  stride  id
   //        ---------  ---------  ---------------------------    ---------  ------  --
   unlock_va(false,     1,         (uint64_t)(void*)(ptr+(768)),  15,        64,      0 );
   WAIT_CACHEOPS;


   C_TEST_PASS;
}



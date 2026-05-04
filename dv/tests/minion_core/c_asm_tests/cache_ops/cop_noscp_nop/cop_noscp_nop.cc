/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

#define BYTES_PER_LINE 64
#define NUM_SETS 16
#define NUM_WAYS 4

int main()
{
   const int data_size = (BYTES_PER_LINE*NUM_SETS*(NUM_WAYS-1))/sizeof(int);
   // L1D$ size is $((BYTES_PER_LINE*NUM_SETS*NUM_WAYS)) Bytes
   volatile int __attribute__((aligned(4096))) ptr[data_size];
   // Init marker
   C_TEST_START;


   // Initialize array
   for (int i = 0; i < data_size; ++i) {
      ptr[i] = i;
   }
   FENCE;

   // Lock L1 lines
   const uint64_t set_size = (BYTES_PER_LINE*NUM_SETS); // Bytes
   const uint64_t data_ptr = (uint64_t)(void*)ptr;
   for (int way = 0; way < (NUM_WAYS-1); way++) {

      uint64_t cop_data_ptr = data_ptr + (way*set_size);

      //      use_tmask  addr          num_lines   stride          id
      //      ---------- ------------- ----------- --------------- --
      lock_va(    false, cop_data_ptr, NUM_SETS-1, BYTES_PER_LINE, 0);
      WAIT_CACHEOPS;
   }

   // Evict whole cache (CHECK WHAT SETs and WAYs WILL BE EVICTED)
   for (unsigned way = 0; way < 4; way++) {
      evict_sw(false,  to_L2, way, 0x0,   15       );
   }
   WAIT_CACHEOPS;

   C_TEST_PASS;
}

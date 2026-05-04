/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random evict_sw sequences.
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#ifndef NUM_ITER
#define NUM_ITER 10
#endif

// Preload random data (~1M)
extern unsigned char _binary_blob_bin_start;

__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:          \n\t"
      "       .incbin     \"data.raw\"  \n\t"
      "       .align      4             \n\t"
      );


int main()
{
    volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );
 
   uint64_t tid, rnd;
   uint64_t ii;

   uint8_t *buffer = (uint8_t *) &_binary_blob_bin_start;

   tid = get_thread_id();
   if (tid == 0) {
      // Setup machine trap vector
      setup_mtrap_vector();
      excl_mode(1);
/**
#ifdef L1D_SPLIT_MODE
      mcache_control(1,0,0);
#endif
#ifdef L1D_SCP_MODE
      mcache_control(1,0,0);
      mcache_control(1,1,0);
#endif
**/
      setup_l1dcache_split_or_scp();  // PRM-8; VERIF-3295
      excl_mode(0);
   } else {
      delay(30);
   }

   // Init marker
   C_TEST_START;

   for (ii = 0; ii < NUM_ITER; ++ii) {
      // Get a random number
      rnd = et_get_rand_dword();

      // Warm up the L1 cache
      warm_l1d(buffer+((rnd%64)*64));

      FENCE;

      bool use_tmask = 0; //rnd % 2;
      if (use_tmask) {
         uint64_t tmask = rnd >> 32;
         __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
      }

      //
      // Do some random EVICT SW
      //
      //       use_tmask  dest             way              set               num_lines          warl
      //       ---------  ---------------  ---------------  ----------------  -----------------  ----
    //evict_sw(use_tmask, ((rnd >> 1)%4),  (rnd >> 4) % 4,  (rnd >> 8) % 16,  (rnd >> 16) % 16,  rnd);
      evict_sw(use_tmask, ((rnd >> 1)%4),  (rnd >> 4) % 4,  (rnd >> 8) % 16,  1,                 rnd);

      WAIT_CACHEOPS;
   }

   C_TEST_PASS;
   return 0;
}


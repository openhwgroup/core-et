/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "esr.h"
#include "cacheops.h"
#include "et_test_common.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define NUM_ITER 3

int main()
{
   uint64_t tid, rnd;
   uint64_t ii;

   uint8_t __attribute__((aligned(4096))) buffer[1024];

   volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );

   tid = get_thread_id();
   if (tid == 0) {
     setup_cache_scp();   
   } else {
      delay(30);
   }

   // Init marker
   C_TEST_START;

   for (ii = 0; ii < NUM_ITER; ++ii) {
      uint8_t *base_addr;

      // Get a random number
      rnd = et_get_rand_dword();
      base_addr = buffer+((rnd%64)*64);

      FENCE;

      // Prepare the tensor mask (50% of the time)
      bool use_tmask = rnd % 2;
      if (use_tmask) {
         uint64_t tmask = rnd >> 32;
         __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
      }

      // Do some random PREFETCH VA
      //          use_tmask  dest             addr                  num_lines          stride             id          warl
      //          ---------  ---------------  --------------------  -----------------  -----------------  ----------  ----
      prefetch_va(use_tmask, ((rnd >> 1)%4),  (uint64_t)base_addr,  (rnd >> 16) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);

      WAIT_CACHEOPS;
   }

   C_TEST_PASS;
   return 0;
}


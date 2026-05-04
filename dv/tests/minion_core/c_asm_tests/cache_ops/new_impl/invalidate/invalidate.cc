/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random prefetch_va sequences.
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 10000
   #ifndef NUM_ITER
   #define NUM_ITER 8
   #endif
#else
   #ifndef NUM_ITER
   #define NUM_ITER 64
   #endif
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

   uint64_t tid, rnd, cinval_event;

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

   #ifdef MINSHIRE_GATESIM_ENABLE 

     volatile uint64_t hart_id = get_hart_id(); 
     volatile uint64_t thread_id  = (hart_id & 1);
     volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
     volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     volatile uint64_t timeout = TIMEOUT;

     if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
       s32_scp_init();      
     }

     if (hart_id == 0) {
   #endif

   for (ii = 0; ii < NUM_ITER; ++ii) {
      uint8_t *base_addr;

      // Get a random number
      cinval_event = et_get_rand_dword();
      uint64_t inval_icache = cinval_event & 1;
      uint64_t inval_TLBs_and_PTWs = (cinval_event >> 1) & 1;      

      // Warm up the L1 cache
      rnd = et_get_rand_dword();
      base_addr = buffer+((rnd%64)*64);
      warm_l1d(base_addr);
      FENCE;

      // CACHE_INVALIDATE      
      //          inval_instr_cache  inval_TLBs_and_PTW  warl
      //          -----------------  ------------------  -----
      cache_invalidate(inval_icache, inval_TLBs_and_PTWs, cinval_event);

      WAIT_CACHEOPS;

      uint64_t res = get_cache_invalidate();
      if (res != 0) {
        C_TEST_FAIL;
        #ifdef MINSHIRE_GATESIM_ENABLE        
        atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
        WFI 
        #endif                       
      }
   }

   #ifdef MINSHIRE_GATESIM_ENABLE
     }

     if (hart_id == 0) {
       atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
     }

     if (shire_id == 32 and thread_id == 0) {
       check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
     }
   #endif

   C_TEST_PASS;
   return 0;
}


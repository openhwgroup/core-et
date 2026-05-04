/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random lock_sw/unlock_sw sequences.
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NUM_ITER 256


// (CYE)
#ifdef SELF_CHECK

#define SIZE 32
#define ALIGN_DATA  32
#define ALIGN_INDEX 16

static uint32_t result[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000
};

inline __attribute__((always_inline)) void __do_load_dw(uint8_t *my_base_addr, uint32_t *my_result) {
    __asm__ __volatile__
        (
         "ld a0, 0(%[my_base_addr])\n"
         "sd a0, 0(%[my_result]) \n"
         :
         : [my_base_addr] "r" (my_base_addr), [my_result] "r" (my_result)
         : "a0", "memory"
        );
}

inline __attribute__((always_inline)) void __do_amoswapl_w(uint8_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       ".equ NEW_VALUE, 0xbeefbeef\n"
       "li a0, NEW_VALUE\n"
       "amoswapl.w a1, a0, (%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a0", "a1", "memory"
      );
}

#endif // ifdef SELF_CHECK



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
   uint64_t tid, rnd;
   uint64_t ii;
   uint8_t *buffer = (uint8_t *) &_binary_blob_bin_start;
   bool  pass = true;

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
      uint8_t *base_addr;

   #ifdef SELF_CHECK

      // (CYE) in order to do self-checking:
      // 1) for first 16 iterations of "sets" (ii==0...15); 
      // 2) don't use random; do lock_sw to all sets (16) and any one of 4 ways of L1D$;
      // 3) add self-checking on data being zero'ed;
      // 4) to self-check the hard-lock and M-state effects: do Evict (should be NO effect);

      // Split_mode: L1D normal cache: T0 has 8 sets (0~7); T1 has 2 sets (14, 15)

      if (ii < 8) {   // SCP_mode: assume only T0 (has 8 sets = 0~7) 
         base_addr = buffer+(ii*64);

         // at first time (ii==0): warm the L1D$:
         if (ii == 0) {
            warm_l1d((base_addr+64*64), 100);  // we prefer warm addresses different from lock_sw addresses
            FENCE;
         }

         // random pick one of 4 ways: 
         rnd = et_get_rand_dword();
         uint64_t way = rnd % L1_WAYS;
   
         //      way      paddr                 warl
         //      -------  --------------------  ----
         lock_sw(way,     (uint64_t)base_addr,  0);
         WAIT_CACHEOPS;

            // (CYE) self-checking: data value of this address should be 0 (after lock_sw):
            __do_load_dw(base_addr, result);
   
            if(result[0] != 0) {    // "lock" should zero out the data 
               et_printf("Test self-check: mismatch: L1 data is non-0\n");  
               pass = false;
               C_TEST_FAIL;
            } 
            //else {
            //   et_printf("Test self-check: match: L1 data is 0\n");
            //}

         // to self-check its "hard-lock" state:
         // do Evict_VA (L1->L2): should be NO effect; 

         //       use_tmask  dest  addr                  num_lines  stride  id   warl
         //       ---------  ----  --------------------  ---------  ------  ---- ----
         evict_va(0,         1,    (uint64_t)base_addr,  0,         0,      0,   rnd);
         WAIT_CACHEOPS;

            // do a "amoswapl.w" (to L2): new data is placed in L2; old data (non-0) is returned to Core;
            FENCE;
            __do_amoswapl_w(base_addr, result); 
            FENCE;
          
            // expect the above returned data to NOT be 0 (meaning Evict not succeed):
            if (result[0] == 0) {   // should NOT be 0 
               et_printf("Test self-check: expect Evict not succeed: mismatch\n");
               pass = false;
               C_TEST_FAIL;
            } 
            //else {
            //   et_printf("Test self-check: expect Evict not succeed: match\n");
            //}
            
            // do a normal Load:
            __do_load_dw(base_addr, result);

            // expect that "hard-lock" and "0" data still exist in L1:
            if(result[0] != 0) {  
               et_printf("Test self-check: expect L1 data=0 after Evict (no effect): mismatch\n");  
               pass = false;
               C_TEST_FAIL;
            } 
            //else {
            //   et_printf("Test self-check: expect L1 data=0 after Evict (no effect): match\n");  
            //}

         if (pass == false) {
            C_TEST_FAIL;
         } 

         continue;   // skip code below and go to next iteration of for(ii) loop
      }  // if (ii < 8)


      // after first 8 iterations are completed, unlock everything and warm up the cache again
      if (ii == 8) {
         for (int jj = 0; jj < L1_WAYS; jj++) {
            //for (int kk = 0; kk < L1_SETS; kk++) {
            for (int kk = 12; kk < 14; kk++) {
               unlock_sw(jj, kk);
            }
         }
         WAIT_CACHEOPS;
         warm_l1d(base_addr, 100);
         FENCE;
      }

   #endif // ifdef SELF_CHECK


      // for (ii >=8) : use random; don't self-checking;

      // Get a random number
      rnd = et_get_rand_dword();
      base_addr = buffer+((rnd%64)*64);

      // Lock 75% of the time, unlock 25% of the time
      if ((rnd >> 62) < 3) {
         //      way                     paddr                 warl
         //      ----------------------  --------------------  ----
         lock_sw((rnd >> 16) % L1_WAYS,  (uint64_t)base_addr,  rnd);
      } else {
         //        way                     set                        warl
         //        ----------------------  -------------------------  ----
         unlock_sw((rnd >> 16) % L1_WAYS,  (uint64_t)base_addr >> 6,  rnd);
      }

      WAIT_CACHEOPS;

      // Every 64 iteration, unlock everything and warm up the cache again
      if ((ii % 64) == 63) {
         for (int jj = 0; jj < L1_WAYS; jj++) {
            for (int kk = 0; kk < L1_SETS; kk++) {
               unlock_sw(jj, kk);
            }
         }
         WAIT_CACHEOPS;
         warm_l1d(base_addr);
         FENCE;
      }
   }

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


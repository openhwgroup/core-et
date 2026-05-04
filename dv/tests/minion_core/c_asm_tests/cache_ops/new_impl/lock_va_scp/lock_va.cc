/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random lock_va/unlock_va sequences.
//
// Note: This test will be in SCP_mode: only 4 sets available for normal cache 
// (T0 has set=12 & 13; T1 has 14 & 15).
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NUM_ITER 64


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

      // Get a random number
      rnd = et_get_rand_dword();

      // Warm up the L1 cache
      base_addr = buffer+((rnd%64)*64);
      warm_l1d(base_addr);
      FENCE;

      bool use_tmask = rnd % 2;
      uint64_t tmask = 0; 
      if (use_tmask) {
         tmask = rnd >> 32;
         __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
      }

      // Do some random LOCK VA
      //      use_tmask   addr                  num_lines          stride             id          warl
      //      ---------   --------------------  -----------------  -----------------  ----------  ----
      lock_va(use_tmask,  (uint64_t)base_addr,  (rnd >> 16) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);
      WAIT_CACHEOPS;


   #ifdef SELF_CHECK
   
      // (CYE) add some self-check: 
      // 1) only when use_tmask==0, or when (use_tmask==1) and (tmask[0]==1'b1);
      // 2) to be simple: only on first line (if num_lines > 1);
      // 3) to check on data=0; 
      // 4) to check on soft-lock state and M state: do Evict (should be able to evict the line out)

      if ((use_tmask == false) || ((use_tmask == true) && ((tmask & 0x1UL) == 1))) {
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

         // to self-check its "soft-lock" state:
         // do Evict_VA (L1->L2): should succeed; 

         //       use_tmask  dest  addr                  num_lines  stride  id   warl
         //       ---------  ----  --------------------  ---------  ------  ---- ----
         evict_va(0,         1,    (uint64_t)base_addr,  0,         0,      0,   rnd);
         WAIT_CACHEOPS;

            // do a "amoswapl.w" (to L2): new data is placed in L2; old data (non-0) is returned to Core;
            FENCE;
            __do_amoswapl_w(base_addr, result); 
            FENCE;
          
            // expect the above returned data to be 0 (meaning Evict succeed):
            if (result[0] != 0) {   // should be 0 
               et_printf("Test self-check: expect L2 data=0 (Evict succeed): mismatch\n");
               pass = false;
               C_TEST_FAIL;
            } 
            //else {
            //   et_printf("Test self-check: expect L2 data=0 (Evict succeed): match\n");
            //}
            
            // do a normal Load:
            __do_load_dw(base_addr, result);

            // expect that "soft-lock" and "0" data was already lost in L1;
            // expect the above Load get the new data from L2;
            if(result[0] != 0xbeefbeef) {  
               et_printf("Test self-check: expect L1 was invalidated after Evict (succeed): mismatch\n");  
               pass = false;
               C_TEST_FAIL;
            } 
            //else {
            //   et_printf("Test self-check: expect L1 was invalidated after Evict (succeed): match\n");  
            //}

         if (pass == false) {
            C_TEST_FAIL;
         } 
      }  // if (use_tmask == false)

   #endif // ifdef SELF_CHECK


      // 50% of the time do some unlocks (different number of lines than what was locked)
      if (rnd >> 63) {
         //        use_tmask   addr                  num_lines          stride             id          warl
         //        ---------   --------------------  -----------------  -----------------  ----------  ----
         unlock_va(use_tmask,  (uint64_t)base_addr,  (rnd >> 48) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);
      }

      // Every 16 iteration, unlock everything
      if ((ii % 16) == 15) {
         for (int jj = 0; jj < L1_WAYS; jj++) {
            for (int kk = 0; kk < L1_SETS; kk++) {
               unlock_sw(jj, kk);
            }
         }
      }
   }

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


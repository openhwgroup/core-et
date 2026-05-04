/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random evict_va sequences.
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

#endif // ifdef SELF_CHECK


// Preload random data (~1M)
extern unsigned char _binary_blob_bin_start[];

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
      rnd = et_get_rand_dword();

      // Warm up the L1 cache
      base_addr = buffer+((rnd%64)*64);
      warm_l1d(base_addr, 100);        // 100% warm
      FENCE;

   #ifdef SELF_CHECK
      // (CYE) to prepare for self-checking (later): 
      // Store some known value to base_addr in L1D$: 16-byte data = 0xa1a1_a1a1_a1a1_a1a1;
      for (int jj = 0; jj < 16; jj++) {   // let's Store 16 bytes (out of 64 bytes cline)
         * (base_addr + jj) = 0xa1;    
      }
      FENCE;
   #endif // ifdef SELF_CHECK


      bool use_tmask = rnd % 2;
      uint64_t tmask;
      if (use_tmask) {
         tmask = rnd >> 32;
         __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
      }

   #ifdef POSTSI_UST
     #ifdef POSTSI_PHASE1
      uint64_t  dest      = 1;   // 1=L2 (Post-Si Phase 1 cannot access NOC/L3)
     #else
      uint64_t  dest      = (rnd >> 1) % 2 + 1;   // 1=L2, 2=L3 (Post-Si Phase 2: cannot access DDR)
     #endif
   #else
      uint64_t  dest      = (rnd >> 1) % 4;
   #endif
      uint64_t  num_lines = (rnd >> 16) % 16;
      uint64_t  stride    = 64 * ((rnd >> 24) % 16) ;
      uint64_t  id        = (rnd >> 3) % 2 ;

      // Do some random EVICT VA
      //       use_tmask  dest             addr                  num_lines          stride             id          warl
      //       ---------  ---------------  --------------------  -----------------  -----------------  ----------  ----
    //evict_va(use_tmask, ((rnd >> 1)%4),  (uint64_t)base_addr,  (rnd >> 16) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);
      evict_va(use_tmask, dest,            (uint64_t)base_addr,  num_lines,         stride,            id,         rnd);

      WAIT_CACHEOPS;


   // Here: make sure macro SELF_CHECK and POSTSI_UST are set together at the same time!
   #ifdef SELF_CHECK

      // (CYE) self-checking: 
      // 1) to be simple: let's only check on Evict L1->L2 and L1->L3;
      // 2) after evict_va: (dirty) data is flushed to L2; L1 is invalidated;  
      // 3) do a "amoswapl.w": new data is placed in L2; (old) data is returned to "rd" (need to check);
      // 4) do a normal load: will obtain the new data from L2 (bcoz L1 is invalidated);
      
      // Evict L1->L2:
      if (((use_tmask == false) || ((tmask & 0x1UL) == 1)) && (dest == 1)) {
         // check that L2 have the L1-evicted (dirty) data:
         __asm__ __volatile__
            (
             ".equ NEW_VALUE, 0xbeefbeef\n"
             "li a0, NEW_VALUE\n"
             "amoswapl.w a1, a0, (%[base_addr])\n"
             "sw a1, 0(%[result])\n"
             :
             : [base_addr] "r" (base_addr), [result] "r" (result)
             : "a0", "a1", "memory"
            );
         
         if (result[0] != 0xa1a1a1a1) {   // should be the old (dirty) data from L2 (after evict L1->L2)
            et_printf("Test self-check: expecte (evicted) data in L2: mismatch\n");
            pass = false;
         } 
         //else {
         //   et_printf("Test self-check: expecte (evicted) data in L2: match\n");
         //}

         // check L1 being invalidated:
         __asm__ __volatile__
            (
             "lw a1, 0(%[base_addr])\n"
             "sw a1, 4(%[result])\n"
             :
             : [base_addr] "r" (base_addr), [result] "r" (result)
             : "a1", "memory"
            );

         if (result[1] != 0xbeefbeef) {   // should be the new data from L2 (after amoswapl.w )
            et_printf("Test self-check: expect L1 was invalidated: mismatch\n");
            pass = false;
         } 
         //else {
         //   et_printf("Test self-check: expect L1 was invalidated: match\n");
         //}

         if (pass == false) {
            #ifdef MINSHIRE_GATESIM_ENABLE        
            atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
            WFI 
            #endif                 
            C_TEST_FAIL;
            break;
         }
      }  // if(... (dest == 1)) ... do self-checking


      // Evict L1->L3:
      if (((use_tmask == false) || ((tmask & 0x1UL) == 1)) && (dest == 2)) {
         // check that L3 have L1-evicted (dirty) data:
         __asm__ __volatile__
            (
             ".equ NEW_VALUE, 0xbeefbeef\n"
             "li a0, NEW_VALUE\n"
             "amoswapg.w a1, a0, (%[base_addr])\n"
             "sw a1, 0(%[result])\n"
             :
             : [base_addr] "r" (base_addr), [result] "r" (result)
             : "a0", "a1", "memory"
            );
         
         if (result[0] != 0xa1a1a1a1) {   // should be the old (dirty) data from L3 (after evict L1->L3)
            et_printf("Test self-check: expect (evicted) data in L3: mismatch\n");
            pass = false;
         } 
         //else {
         //   et_printf("Test self-check: expect (evicted) data in L3: match\n");
         //}

         // check L1 being invalidated:
         __asm__ __volatile__
            (
             "lw a1, 0(%[base_addr])\n"
             "sw a1, 4(%[result])\n"
             :
             : [base_addr] "r" (base_addr), [result] "r" (result)
             : "a1", "memory"
            );

         if (result[1] != 0xbeefbeef) {   // should be the new data from L3 (after amoswapg.w )
            et_printf("Test self-check: expect L1 was invalidated: mismatch\n");
            pass = false;
         } 
         //else {
         //   et_printf("Test self-check: expect L1 was invalidated: match\n");
         //}

         if (pass == false) {
            #ifdef MINSHIRE_GATESIM_ENABLE        
            atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
            WFI       
            #endif       
            C_TEST_FAIL;
            break;
         }
      }  // if(... (dest == 2)) ... do self-checking

   #endif  // ifdef SELF_CHECK


   } // for (ii = 0 ...)


   #ifdef MINSHIRE_GATESIM_ENABLE
     }

     if (hart_id == 0) {
       if (pass == false) {
         atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);       
       } else {
         atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
       }
     }

     if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
       check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
     }
   #endif

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


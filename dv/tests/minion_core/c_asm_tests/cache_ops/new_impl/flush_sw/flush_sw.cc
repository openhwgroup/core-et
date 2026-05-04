/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random flush_sw sequences.
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


// (CYE) my own version of function warm_l1d: more precise based on different Thread
inline void __attribute__((always_inline)) my_warm_l1d (uint8_t *base, uint64_t fill_rate = 75) {
   l1d_mode mode = get_l1d_mode();
   int num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 40 : 16;

   uint64_t hart_id = get_hart_id(); 
   uint64_t thread_id  = (hart_id & 1);

   if (thread_id == 0) {
       num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 32 :     // T0 has 8 sets (0...7) 
                                          8;       // T0 has 2 sets (12, 13)
   } else if (thread_id == 1) {
       num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 8  :     // T1 has 2 sets (14, 15) 
                                          8;       // T1 has 2 sets (14, 15)
   }

   //
   // For each cache line, store something
   // By default, 75% of the lines are actually accessed
   //
   for (int ii = 0; ii < num_lines; ++ii) {
      if ((et_get_rand_dword() % 100) < fill_rate) {
         * (base + (64*ii)) = ii;            // Store 1 byte (8-bit)
      }
   }
   FENCE;
}


#ifdef SELF_CHECK

inline uint64_t __attribute__((always_inline)) find_set_from_addr(uint64_t addr, l1d_mode mode, uint64_t thread_id)
{
   uint64_t set;
   if (mode == l1d_shared) {
      set = (addr >> 6) % 16;        // set=0~15; addr[9:6]
   } 
   else if (mode == l1d_split) {
      if (thread_id == 0) {
         set = (addr >> 6) % 8;      // set=0~7;  addr[8:6]
      } else {
         set = (addr >> 6) % 2 + 14; // set=14~15; addr[6]
      }
   }
   else {   // l1d_scp
      if (thread_id == 0) {
         set = (addr >> 6) % 2 + 12; // set=12~13; addr[6]
      } else {
         set = (addr >> 6) % 2 + 14; // set=14~15; addr[6]
      }
   } 
   return set;
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

inline __attribute__((always_inline)) void __do_amoswapg_w(uint8_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       ".equ NEW_VALUE, 0xbeefbeef\n"
       "li a0, NEW_VALUE\n"
       "amoswapg.w a1, a0, (%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a0", "a1", "memory"
      );
}

inline __attribute__((always_inline)) void __do_load_w(uint8_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       "lw a1, 0(%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a1", "memory"
      );
}

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

 #ifdef SELF_CHECK
   l1d_mode mode = get_l1d_mode();
   //uint64_t thread_id = get_thread_id();  // already declared previously as "tid"; 
 #endif

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
      // base address:
      base_addr = buffer+((rnd%64)*64);

   // Here: make sure macro SELF_CHECK and POSTSI_UST are set together at the same time!
   #ifdef SELF_CHECK

      // do self-checking on first loop ii==0: try to be more predictable:
      if (ii < 10) {
         // (CYE) some setup in order to make self-checking (later):
         // Don't warm the entire cache (to avoid unpredicted capacity eviction or overwrite).
         // Store some known value to the first base_addr: 16-byte data = 0xa1a1_a1a1_a1a1_a1a1; 
         // Note: not sure which "way" it goes into; 
         for (int jj = 0; jj < 16; jj++) {   // let's Store double-word, 16 bytes (out of 64 bytes cline)
            * (base_addr + jj) = 0xa1;    
         }
         FENCE;

       #ifdef POSTSI_PHASE1
         uint64_t my_dest = 1;   // 1=L2 (Post-Si Phase 1 cannot access NOC/L3)
       #else
         uint64_t my_dest = (rnd >> 1) % 2 + 1;   // 1=L2, 2=L3 (Post-Si Phase 2: cannot access DDR)
       #endif

         // find out "set" value from a given address:
         uint64_t set;
         //set = find_set_from_addr((uint64_t)base_addr, mode, thread_id);  
         set = find_set_from_addr((uint64_t)base_addr, mode, tid);  
         
         // do FLUSH_SW: to ALL 4 ways (of the above "set"):
         for (uint64_t way = 0; way < 4; way++) {
            //       use_tmask  dest             way              set               num_lines          warl
            //       ---------  ---------------  ---------------  ----------------  -----------------  ----
            flush_sw(0,         my_dest,         way,             set,              0,                 rnd);
         }
         WAIT_CACHEOPS;

         // (CYE) self-checking:
         // 1) to be simple: let's only check on Flush L1->L2 and L1->L3;
         // 2) after flush_sw: (dirty) data is flushed to L2; L1 is still valid (with clean data);  
         // 3) do a "amoswapl.w": new data is placed in L2; (old) data is returned to "rd" (need to check);
         // 4) do a normal load: will obtain the "old" data from L1 (bcoz L1 is still valid);

         // Flush L1->L2:
         if (my_dest == 1) {
            // 1) do a "amoswapl.w": new data is placed in L2; (old) data is returned to Core;
            FENCE;
            __do_amoswapl_w(base_addr, result); 
            FENCE;
 
            // check that L2 returned the (dirty) data flushed from L1:
            if (result[0] != 0xa1a1a1a1) {   // should be the old (dirty) data from L2 (after flush L1->L2)
               et_printf("Test self-check: expect (flushed) data in L2: mismatch\n");
               pass = false;
            } 
            //else {
            //   et_printf("Test self-check: expect (flushed) data in L2: match\n");
            //}

            // 2) do a normal Load: 
            __do_load_w(base_addr, result+1); 

            // check L1 being still valid (with old data):
            if (result[1] != 0xa1a1a1a1) {   // expect "old" data 
               et_printf("Test self-check: expect L1 was still valie: mismatch\n");
               pass = false;
            } 
            //else {
            //   et_printf("Test self-check: expect L1 was still valid: match\n");
            //}

            if (pass == false) {
               #ifdef MINSHIRE_GATESIM_ENABLE        
               atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
               WFI 
               #endif                    
               C_TEST_FAIL;
               break;
            }
         } // if (my_dest == 1)

         // Flush L1->L3:
         if (my_dest == 2) {
            // 1) do a "amoswapg.w": new data is placed in L3; (old) data is returned to Core;
            FENCE;
            __do_amoswapg_w(base_addr, result); 
            FENCE;
         
            // check that L3 returned the (dirty) data flushed from L1:
            if (result[0] != 0xa1a1a1a1) {   // should be the old (dirty) data from L3 (after flush L1->L3)
               et_printf("Test self-check: expect (flushed) data in L3: mismatch\n");
               pass = false;
            } 
            //else {
            //   et_printf("Test self-check: expect (flushed) data in L3: match\n");
            //}

            // 2) do a normal Load: 
            __do_load_w(base_addr, result+1); 

            // check L1 being still valide (with old data):
            if (result[1] != 0xa1a1a1a1) {   // expect "old" data 
               et_printf("Test self-check: expect L1 was still valid: mismatch\n");
               pass = false;
            } 
            //else {
            //   et_printf("Test self-check: expect L1 was still valid: match\n");
            //}

            if (pass == false) {
               #ifdef MINSHIRE_GATESIM_ENABLE        
               atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
               WFI 
               #endif      
               C_TEST_FAIL;
               break;
            }
         } // if (my_dest == 2)

         continue;   // skip code below and go to next iteration of for() loop
      } // if (ii < 10)

   #endif // ifdef SELF_CHECK


      // for the rest: (ii>10): no self-checking, use random ways and tmask, etc.

      // Warm up the L1 cache
      //warm_l1d(base_addr, 100);   // 100% warm
      my_warm_l1d(base_addr, 100);   // 100% warm
      FENCE;

      bool use_tmask = rnd % 2;
      if (use_tmask) {
         uint64_t tmask = rnd >> 32;
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
      uint64_t  way       = (rnd >> 4) % 4;
      uint64_t  set       = (rnd >> 8) % 16; 

      //
      // Do some random FLUSH SW
      //
      //       use_tmask  dest             way              set               num_lines          warl
      //       ---------  ---------------  ---------------  ----------------  -----------------  ----
    //flush_sw(use_tmask, ((rnd >> 1)%4),  (rnd >> 4) % 4,  (rnd >> 8) % 16,  (rnd >> 16) % 16,  rnd);
      flush_sw(use_tmask, dest,            way,             set,              num_lines,         rnd);

      WAIT_CACHEOPS;
      clear_l1d();
   }


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


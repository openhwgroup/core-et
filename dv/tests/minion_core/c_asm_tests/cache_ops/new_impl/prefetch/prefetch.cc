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


#ifdef SELF_CHECK

#define SIZE 32
//#define ALIGN_DATA  32
#define ALIGN_DATA  512
#define ALIGN_INDEX 16

static uint32_t operands[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
      0xa5a50001,          
      0xa5a50002,           
      0xa5a50003,           
      0xa5a50004,
      0xa5a50005,
      0xa5a50006,
      0xa5a50007,
      0xa5a50008,                 
      0xa5a50009,                
      0xa5a5000a,                   
      0xa5a5000b,                 
      0xa5a5000c,
      0xa5a5000d,
      0xa5a5000e,
      0xa5a5000f,
      0xa5a50010,                
      0xa5a50011,                
      0xa5a50012,                 
      0xa5a50013,                
      0xa5a50014,   
      0xa5a50015,
      0xa5a50016,
      0xa5a50017,
      0xa5a50018,
      0xa5a50019,
      0xa5a5001a,
      0xa5a5001b,
      0xa5a5001c,
      0xa5a5001d,
      0xa5a5001e,
      0xa5a5001f,
      0xa5a50020 
};

static uint32_t expected[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
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

static uint32_t result[2]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0x00000000,
   0x00000000
};


inline __attribute__((always_inline)) void __save_operand_in_expected(uint32_t *my_operand, uint32_t *my_expected) {
   __asm__ __volatile__
      (
       "lw a0, 0(%[my_operand])\n"
       "sw a0, 0(%[my_expected])\n"
       :
       : [my_operand] "r" (my_operand), [my_expected] "r" (my_expected)
       : "a0", "memory"
      );
}


// amoswapl.w: atomic_swap in L2: new data = 0xbeef0002; old data returned goes into my_result;
//inline __attribute__((always_inline)) void __do_amoswapl_w(uint8_t *my_addr, uint32_t *my_result) {
inline __attribute__((always_inline)) void __do_amoswapl_w(uint32_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       ".equ NEW_VALUE, 0xbeef0002\n"
       "li a0, NEW_VALUE\n"
       "amoswapl.w a1, a0, (%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a0", "a1", "memory"
      );
}

// amoswapg.w: atomic_swap in L3: new data = 0xbeef0003; old data returned goes into my_result;
//inline __attribute__((always_inline)) void __do_amoswapg_w(uint8_t *my_addr, uint32_t *my_result) {
inline __attribute__((always_inline)) void __do_amoswapg_w(uint32_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       ".equ NEW_VALUE, 0xbeef0003\n"
       "li a0, NEW_VALUE\n"
       "amoswapg.w a1, a0, (%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a0", "a1", "memory"
      );
}

//inline __attribute__((always_inline)) void __do_load_w(uint8_t *my_addr, uint32_t *my_result) {
inline __attribute__((always_inline)) void __do_load_w(uint32_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       "lw a1, 0(%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a1", "memory"
      );
}


//   This function evicts all cache lines present in the L1 dcache: all to L2;
inline void __attribute__((always_inline)) clear_l1d_to_l2() {
   for (int w = 0; w < L1_WAYS; ++w) {
      evict_sw(false, to_L2, w, 0, 15);
   }
   WAIT_CACHEOPS;
}

//   This function evicts all cache lines present in the L1 dcache: all to L3;
inline void __attribute__((always_inline)) clear_l1d_to_l3() {
   for (int w = 0; w < L1_WAYS; ++w) {
      evict_sw(false, to_L3, w, 0, 15);
   }
   WAIT_CACHEOPS;
}

//   This function evicts all cache lines present in the L1 dcache: all to Mem;
inline void __attribute__((always_inline)) clear_l1d_to_mem() {
   for (int w = 0; w < L1_WAYS; ++w) {
      evict_sw(false, to_Mem, w, 0, 15);
   }
   WAIT_CACHEOPS;
}


// restore orignal data value back to a certain address: for "operands[]": 
inline __attribute__((always_inline)) void __restore_orig_data(uint32_t *my_addr) {
         __asm__ __volatile__
            (
             ".equ ORIG_VALUE, 0xa5a50001\n"
             "li a0, ORIG_VALUE\n"
             "sw a0, 0(%[my_addr])\n"
             :
             : [my_addr] "r" (my_addr)
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


   #ifdef SELF_CHECK
      // first of all: save all the operands[] into expected[]:
      uint64_t jj;
      for (jj = 0; jj < SIZE; jj++) {
         __save_operand_in_expected(operands+jj, expected+jj);
      }
      // important: must clear entire L1D$: Evict (invalidate) all lines; 
      #ifdef POSTSI_PHASE1
        clear_l1d_to_l2();  // for Post-Si_Phase1
      #else
        clear_l1d_to_l3();  // for Post-Si_Phase2
      #endif
      WAIT_CACHEOPS;
   #endif


   for (ii = 0; ii < NUM_ITER; ++ii) {
      uint8_t *base_addr;

      // Get a random number
      rnd = et_get_rand_dword();
      base_addr = buffer+((rnd%64)*64);
      FENCE;


   #ifdef SELF_CHECK

      // do self-checking on first loop ii==0: try to be simpler and more predictable:
      // note: if use "base_addr", the data value will be unknown;
      //       so change to use "operands" (known data).
      if (ii < 6) {
       #ifdef POSTSI_PHASE1
         uint64_t my_dest = 0;   // 0=L1 (Post-Si Phase 1 cannot access NOC/L3)
       #else
         uint64_t my_dest = ii % 3;  // 0=L1, 1=L2, 2=L3 (Post-Si Phase 2 cannot access DDR)(ignore dest=Mem)
       #endif

         //          use_tmask  dest       addr                      num_lines  stride   id   warl
         //          ---------  ---------  ------------------------  ---------- -------  ---  ----
       //prefetch_va(0,         my_dest,   (uint64_t)base_addr,      0,         0 ,      0,   rnd);
         prefetch_va(0,         my_dest,   (uint64_t)(operands+ii),  0,         0 ,      0,   rnd);
         WAIT_CACHEOPS;

         // Prefetch_to_L1:
         if (my_dest == 0) {
            // 1) do a "amoswapl.w" (to L2): new data (0xbeef0002) is placed in L2 (bypassing L1);
            FENCE;
            //__do_amoswapl_w(base_addr, result);  // it returns "old" data (value don't know)  
            __do_amoswapl_w(operands+ii, result);  // it returns "old" data (operands[ii]) 
            FENCE;

            // 2) do a normal Load:
            //__do_load_w(base_addr, result+1);
            __do_load_w(operands+ii, result+1);

            // expect the Load return "old" data from L1 (not "new" data from L2);
            // should NOT get "new" data from L2 (0xbeef0002);
            //if (result[1] != 0xa5a50001) {   // should get "old" data from L1
            if (result[1] != expected[ii]) { 
               et_printf("Test self-check: Pref_L1: expect L1 was already valid: mismatch\n");
               pass = false;
            }
            else {
               et_printf("Test self-check: Pref_L1: expect L1 was already valid: match\n");
            }
         } // if (my_dest == 0)


         // Prefetch_to_L2:
         if (my_dest == 1) {
            // 1) do a "amoswapl.w" (to L2): new data (0xbeef0002) is placed in L2 (bypassing L1);
            FENCE;
            //__do_amoswapl_w(base_addr, result);  // it returns "old" data (value don't know)  
            __do_amoswapl_w(operands+ii, result);  // it returns "old" data (operands[ii]) 
            FENCE;

            // expect the above return "old" data from L2: 
            //if (result[0] != 0xa5a50001) {   // should get "old" data from L2 
            if (result[0] != expected[ii]) { 
               et_printf("Test self-check: Pref_L2: expect L2 was already valid: mismatch\n");
               pass = false;
            }
            else {
               et_printf("Test self-check: Pref_L2: expect L2 was already valid: match\n");
            }

            // 2) do a normal Load:
            //__do_load_w(base_addr, result+1);
            __do_load_w(operands+ii, result+1);

            // expect the Load return "new" data from L2: since L1 was invalid;
            if (result[1] != 0xbeef0002) {   // should get "new" data from L2
               et_printf("Test self-check: Pref_L2: expect L1 was invalid: mismatch\n");
               pass = false;
            }
            else {
               et_printf("Test self-check: Pref_L2: expect L1 was invalid: match\n");
            }
         } // if (my_dest == 1)


         // Prefetch_to_L3:
         if (my_dest == 2) {
            // 1) do a "amoswapg.w" (to L3): new data (0xbeef0003) is placed in L3 (bypassing L1 & L2);
            FENCE;
            //__do_amoswapg_w(base_addr, result);  // it returns "old" data (value don't know)  
            __do_amoswapg_w(operands+ii, result);  // it returns "old" data (operands[ii]) 
            FENCE;

            // expect the above return "old" data from L3: 
            //if (result[0] != 0xa5a50001) {   // should get "old" data from L3 
            if (result[0] != expected[ii]) { 
               et_printf("Test self-check: Pref_L3: expect L3 was already valid: mismatch\n");
               pass = false;
            }
            else {
               et_printf("Test self-check: Pref_L3: expect L3 was already valid: match\n");
            }

            // 2) do a normal Load:
            //__do_load_w(base_addr, result+1);
            __do_load_w(operands+ii, result+1);

            // expect the Load return "new" data from L3: since L1 & L2 was invalid;
            if (result[1] != 0xbeef0003) {   // should get "new" data from L3
               et_printf("Test self-check: Pref_L3: expect L1 & L2 was invalid: mismatch\n");
               pass = false;
            }
            else {
               et_printf("Test self-check: Pref_L3: expect L1 & L2 was invalid: match\n");
            }
         } // if (my_dest == 1)


         if (pass == false) {
            #ifdef MINSHIRE_GATESIM_ENABLE        
            atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
            WFI 
            #endif                 
            C_TEST_FAIL;
            break;
         }

         // note: important: restore the original data value for operands[0]:
         __restore_orig_data(operands); 
         FENCE;

       #ifdef POSTSI_PHASE1
         clear_l1d_to_l2();    // for Post-Si_Phase1
       #else
         clear_l1d_to_l3();    // for Post-Si_Phase2
       #endif

         WAIT_CACHEOPS;

         continue;   // skip code below and go to next iteration of for() loop
      } // if (ii < 3)

   #endif // ifdef SELF_CHECK


      // for the rest: (ii>=3): no self-checking, use more random (tmask, dest, num_lines, etc.)

      // Prepare the tensor mask (50% of the time)
      bool use_tmask = rnd % 2;
      if (use_tmask) {
         uint64_t tmask = rnd >> 32;
         __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
      }

   #ifdef POSTSI_UST
     #ifdef POSTSI_PHASE1
      uint64_t  dest      = 0;   // 0=L1 (Post-Si Phase 1 cannot access NOC/L3)
     #else
      uint64_t  dest      = (rnd >> 1) % 2;   // 0=L1, 1=L2 (Post-Si Phase 2: cannot access DDR)
     #endif
   #else
      uint64_t  dest      = (rnd >> 1) % 4;   // 2=L3, 3=Mem (meaning NOP)
   #endif
      uint64_t  num_lines = (rnd >> 16) % 16;
      uint64_t  stride    = 64 * ((rnd >> 24) % 16) ;
      uint64_t  id        = (rnd >> 3) % 2 ;

      // Do some random PREFETCH VA
      //          use_tmask  dest             addr                  num_lines          stride             id          warl
      //          ---------  ---------------  --------------------  -----------------  -----------------  ----------  ----
    //prefetch_va(use_tmask, ((rnd >> 1)%4),  (uint64_t)base_addr,  (rnd >> 16) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);
      prefetch_va(use_tmask, dest,            (uint64_t)base_addr,  num_lines,         stride,            id,         rnd);

      WAIT_CACHEOPS;

   }  // for()


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


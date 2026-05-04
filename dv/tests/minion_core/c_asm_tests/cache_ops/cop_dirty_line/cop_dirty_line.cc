/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "et_test_common.h"
#include "minion.h"
#define TIMEOUT 500



/**
inline __attribute__((always_inline)) void coh_byp_store_local_hw(uint64_t addr, uint16_t st_data) {
    __asm__ __volatile__
        (
         //"addi x3, x0, st_data\n"     // only st_data[11:0]
         //"li x3, st_data\n"
         //"la t2, st_data\n"
         "addi t2, zero, 1024\n"         // only [11:0]
         "shl t2, 0(%[addr]) \n"
         :
         : [addr] "r" (addr), [st_data] "r" (st_data)
         : "t2", "t3", "memory"
        );
}
**/


inline __attribute__((always_inline)) void atomic_add_local_w(uint64_t addr) {
    __asm__ __volatile__
        (
         "addi t2, zero, 1024\n"         // only [11:0]; add 1024 on existing value
         "amoaddl.w t3, t2, 0(%[addr]) \n"
         :
         : [addr] "r" (addr)
         : "t2", "t3", "memory"
        );
}




int main()
{

   //static  int ptr[1024]  __attribute__((aligned(4096))) ;
   //static  uint16_t  st_data;

   #ifdef MINSHIRE_GATESIM_ENABLE
   volatile uint64_t hart_id = get_hart_id();
   volatile uint64_t thread_id  = (hart_id & 1);
   volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
   volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   volatile uint64_t timeout = TIMEOUT;
   if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
     s32_scp_init();
     give_credit(0, 0, 0, 0);
   }
   if (hart_id == 0) {
     wait_for_credit(0);
   }
   #endif

   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];

   C_TEST_START;

   // Test 1: Evict_SW dirty line;

   // Initialize array: Fill in L1D and Store values:
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i;
   }

   // Evict cache: L1->L2: after Evict, the line is invalid in L1D$;
   for (unsigned way = 0; way < 4; ++way) {
      for (unsigned set = 0; set < 16; ++set) {
         evict_sw(false, 0x1, way, set, 0);
      }
   }
   WAIT_CACHEOPS;
   FENCE; 

   // (Charles 4/08/2020) to self-check: 
   // direct Write/Store new values to L2$ (bypass L1D); then normal cached Load;
   // since it's invalid in L1D, the Load will bring the new data from L2$; 
   
   // a) direct Write/Store new/different values to L2$ (bypass L1D); -> change to below:
   // a) Atomic addition Word to Local L2$: add 1024 upon the existing value;
   for (int i = 0; i < 1024; ++i) {
      //coh_byp_store_local_hw( (uint64_t)(void*)(ptr+(i*1)), (1024+i) ); 
      atomic_add_local_w( (uint64_t)(void*)(ptr+(i*1)) ); 
   }
   FENCE;
 
   // b) normal cached Load to read from either L1D$ or L2$: 
   //    check that its return data should be the new/different value from L2$;
   for (int i = 0; i < 1024; i++) {
      if (ptr[i] != (1024+i)) {
         et_printf("Evict_SW: Test self-checking: Mismatch\n");
   #ifdef MINSHIRE_GATESIM_ENABLE         
         atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
   #endif                   
         C_TEST_FAIL;
      }
   }
   FENCE; 



   // Test 2: Flush_SW dirty line;

   // Initialize array: 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i+1;
   }

   // Flush cache: L1->L2: after Flush, the line is still valid in L1D$ (M->E);
   for (unsigned way = 0; way < 4; ++way) {
      for (unsigned set = 0; set < 16; ++set) {
         flush_sw(false, 0x1, way, set, 0);
      }
   }
   WAIT_CACHEOPS;
   FENCE; 

   // (Charles 4/08/2020) to self-check: 

   // a) Atomic addition Word to Local L2$: add 1024 upon the existing value; 
   for (int i = 0; i < 1024; ++i) {
      atomic_add_local_w( (uint64_t)(void*)(ptr+(i*1)) ); 
   }
   FENCE; 

   // b) normal cached Load to read from either L1D$ or L2$: 
   //    check that its return data should be the old value from L1D$;
   for (int i = 0; i < 1024; i++) {
      if (ptr[i] != (i+1)) {
         et_printf("Flush_SW: Test self-checking: Mismatch\n");
   #ifdef MINSHIRE_GATESIM_ENABLE         
         atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
   #endif                   
         C_TEST_FAIL;
      }
   }
   FENCE; 


   // Test 3: Evict_VA dirty line;

   // Initialize array
   for (int i = 0; i < 1024; i++) {
      ptr[i] = i+2;
   }

   // EvictVA cache: L1->L2: after Evict, the line is invalid in L1D$;
   for (unsigned i = 0; i < 4; i++) {
      //	      use_mask dst   address                         num_lines  stride  id
      //	      -------- ---   ------------------------------  ---------  ------  --
      evict_va(false,   0x01, (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
   }
   WAIT_CACHEOPS;
   FENCE; 

   // (Charles 4/08/2020) to self-check: 

   // a) Atomic addition Word to Local L2$: add 1024 upon the existing value; 
   for (int i = 0; i < 1024; ++i) {
      atomic_add_local_w( (uint64_t)(void*)(ptr+(i*1)) ); 
   }
   FENCE; 

   // b) normal cached Load to read from either L1D$ or L2$: 
   //    check that its return data should be the new value from L2$;
   for (int i = 0; i < 1024; i++) {
      if (ptr[i] != (i+2+1024)) {
         et_printf("Evict_VA: Test self-checking: Mismatch\n");
   #ifdef MINSHIRE_GATESIM_ENABLE         
         atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
   #endif                   
         C_TEST_FAIL;
      }
   }
   FENCE; 



   // Test 4: Flush_VA dirty line;

   // Initialize array
   for (int i = 0; i < 1024; i++) {
      ptr[i] = i+3;
   }

   // FlushVA cache: L1->L2: after Flush, the line is still valid in L1D$ (M->E);
   for (unsigned i = 0; i < 4; i++) {
      //	      use_mask dst   address                         num_lines  stride  id
      //	      -------- ---   ------------------------------  ---------  ------  --
      flush_va(false,   0x01, (uint64_t)(void*)(ptr+(i*256)), 15,        64,     0  );
   }
   WAIT_CACHEOPS;
   FENCE; 


   // (Charles 4/08/2020) to self-check: 

   // a) Atomic addition Word to Local L2$: add 1024 upon the existing value; 
   for (int i = 0; i < 1024; ++i) {
      atomic_add_local_w( (uint64_t)(void*)(ptr+(i*1)) ); 
   }
   FENCE; 

   // b) normal cached Load to read from either L1D$ or L2$: 
   //    check that its return data should be the old value from L1D$;
   for (int i = 0; i < 1024; i++) {
      if (ptr[i] != (i+3)) {
         et_printf("Flush_VA: Test self-checking: Mismatch\n");
   #ifdef MINSHIRE_GATESIM_ENABLE         
         atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
   #endif                  
         C_TEST_FAIL;     
      }
   }
   FENCE; 



   #ifdef MINSHIRE_GATESIM_ENABLE
   if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
     check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
   } else if (hart_id == 0) {
     atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
   }  
   #endif

   C_TEST_PASS;
}

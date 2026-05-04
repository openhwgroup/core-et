/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

#ifdef MINSHIRE_GATESIM_ENABLE
#include "minion.h"
#include "et_test_common.h"
#define TIMEOUT 10000
#endif

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   #ifdef MINSHIRE_GATESIM_ENABLE 

     volatile uint64_t hart_id = get_hart_id(); 
     volatile uint64_t thread_id  = (hart_id & 1);
     volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
     volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     volatile uint64_t timeout = TIMEOUT;

     if (shire_id == 32) {
       if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
         s32_scp_init();
         give_credit(0, -1, -1, 1);
         give_credit(32, -1, -1, 1);
       }             
     }

     wait_for_credit(1);  

     if (shire_id == 0) {
   #endif

   // --------------------------------------------------------------------------------------------
   // PREFETCH TO L1
   // --------------------------------------------------------------------------------------------

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }

   // EvictSW whole cache to L2
   for (int way = 0; way < 4; way++)
   {
      //       Tensor mask dst  way  set    Num_lines 
      evict_sw(false     , 0x1, way, 0x0,   15       );
      WAIT_CACHEOPS;
   }

   // Prefetch 64 lines
   for (int j = 0; j < 4; j++)
   { 
      prefetch_va(false,     0,  0,    (uint64_t)(void*)(ptr + (j*256)),  15,         64,      0 );
      WAIT_PREFETCH_0;
   }

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }


   // --------------------------------------------------------------------------------------------
   // PREFETCH TO L2
   // --------------------------------------------------------------------------------------------

   // EvictSW whole cache to L3
   for (int way = 0; way < 4; way++)
   {
      //       Tensor mask dst  way  set    Num_lines 
      evict_sw(false     , 0x2, way, 0x0,   15       );
      WAIT_CACHEOPS;
   }

   // Prefetch 64 lines to L2
   for (int j = 0; j < 4; j++)
   { 
      prefetch_va(false,     1,  0,    (uint64_t)(void*)(ptr + (j*256)),  15,         64,      0 );
      WAIT_PREFETCH_0;
   }

   // Initialize array 
   for (int i = 0; i < 1024; ++i) {
      ptr[i] = i; 
   }


   // --------------------------------------------------------------------------------------------
   // PREFETCH TO L3
   // --------------------------------------------------------------------------------------------

   // EvictSW whole cache to MEM
   for (int way = 0; way < 4; way++)
   {
      //       Tensor mask dst  way  set    Num_lines 
      evict_sw(false     , 0x3, way, 0x0,   15       );
      WAIT_CACHEOPS;
   }

   // Prefetch 64 lines to L3
   for (int j = 0; j < 4; j++)
   { 
      prefetch_va(false,     2,  0,    (uint64_t)(void*)(ptr + (j*256)),  15,         64,      0 );
      WAIT_PREFETCH_0;
   }

   // Initialize array 
   for (int i = 0; i < 512; ++i) {
      ptr[i] = i; 
   }

   #ifdef MINSHIRE_GATESIM_ENABLE
     }

     if (shire_id == 0 and thread_id == 1) {
       give_credit(0, minion_id, 0, 1);
     }

     if (shire_id == 0 and thread_id == 0) {
       wait_for_credit(1); // to ensure that t1 is successfully finished
       atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
     }

     if (shire_id == 32 and thread_id == 0) {
       check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
     }
   #endif
      
   C_TEST_PASS;
}

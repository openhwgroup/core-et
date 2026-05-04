/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/****
 * prefetch_to_l2_M   (M stands for multi or all shires involved in prefetching)
 *This is a prefetch to L2 test
 *Stride=64 gaurantees all lines prefetched through same prefetchOP, are loaded from all Mem-Shire/MC(each req loads from a different Mem-shire)
 *Each shires will prefetch from all Mem-Shire/MC (all num_lines of each prefetchOP from ALL MC)
 * This test should generate MAX NOC/MESH traffic all accross SOC 
 * ****/

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "common.h"
#include "cacheops.h"

#ifdef __cplusplus
extern "C"
#endif

#define N_TIMES 1 

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   //volatile int __attribute__((aligned(4096))) ptr[256];
   // Init marker
   C_TEST_START;

   // long data;
   uint64_t hart_id = getHartId();
   //uint64_t thread_id = get_thread_id(); // (hart_id & 1)
   uint64_t shire_id = ((hart_id>>6) & 0x3F);
   uint64_t minion_id = ((hart_id>>1) & 0x1F);

   /**
   // Initialize array
   for (int i = 0; i < 256; ++i) {
      ptr[i] = i; 
   }

   // Evict cache
   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 0, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 1, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 2, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x1, 3, set,   0       );
   }
   WAIT_CACHEOPS;
   **/

   /***************Prefetch 16 lines (ESPERANTO CACHE CONTROL EXTENSION) **************************
   prefetch_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
   dst = 01 (Prefetch into L2)
   addr = VA
   num_lines = 15
   Stride=64 (implictly read from the X31[47:6] 
      //Case1: each req will go to a unique Mem-Shire based on stride 0x40 (or 64) . Bits (starting from 0) 
   ID = 0 (implictly read from the X31[0]
   ***************************/
   if ((hart_id & 1) == 1) //Only Thread1 (or prefetch threads to do prefetch)
   {
	   for(int i = 0; i < N_TIMES ; i++) //N_TIMES = # of times the thread1 performs the prefetch CSR
	   {

		   // VA Address to prefetch from
		   //                                |39-32= 0x80|    |31-28= 0xa|   |27-23 |             |22-18 |            |17-10 = i factor|  |9-6= MC/MS/L2Bank|  
		   volatile uint64_t VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<10) + (0x0 << 6));

		   prefetch_va(false,     1,   (uint64_t)(VA),  15,         64,      0 );
		   WAIT_PREFETCH_0;
	   }
   }
   C_TEST_PASS;
}

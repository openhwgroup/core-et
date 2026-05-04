/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/****
 * prefetch_to_l2_s512_M   (M stands for multi or all shires involved in prefetching)
 *This is a prefetch to L2 test (S512 stands for stride of 512)
 *Stride=512 gaurantees all lines prefetched through same prefetchOP, are loaded from same Mem-Shire (and BOTH MCs of the mem-shire)
 *all even numbered requests go to MC0 (same mem-shire) and all odd numbered requests go to MC1(same mem-shire)
 *Each shires tries to prefetch from Mem-Shire/MC closest to it 
 *in the first configuration, the shires selected based on LEAST AMOUNT OF HOPS to get to the Mem-Shire
 *Both 
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
   C_TEST_START;

   uint64_t hart_id = getHartId();
   //uint64_t thread_id = get_thread_id(); // (hart_id & 1)
   uint64_t shire_id = ((hart_id>>6) & 0x3F);
   uint64_t minion_id = ((hart_id>>1) & 0x1F);

   /***************Prefetch 16 lines (ESPERANTO CACHE CONTROL EXTENSION) **************************
   prefetch_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
   dst = 01 (Prefetch into L2)
   addr = VA
   num_lines = 15
   Stride=64 (implictly read from the X31[47:6] 
      //Case2: each req will go to ONLY to  ONE Mem-Shire based on stride 0x200 (or 512).Bit8-0 will be untouched in VA 
      //But every even request will be to MC0 (of the same mem-shire) and odd request will be to MC1 (of same mem-shire)
   ID = 0 (implictly read from the X31[0]
   ***************************/
   if ((hart_id & 1) == 1) //Only Thread1 (or prefetch threads to do prefetch)
   {
	   for(int i = 0; i < N_TIMES ; i++) //N_TIMES = # of times the thread1 performs the prefetch CSR
	   {

		   volatile uint64_t VA;	
		   // VA Address to prefetch from
		   //                                |39-32= 0x80|    |31-28= 0xa|   |27-23 |             |22-18 |            |17-13 = i factor|  |9-6= MC/MS/L2Bank|  
		                                   
		   /****** Prefetches from these shires should get data always form mem-sire 0*******/
		   if ( (shire_id == 0) || (shire_id == 1) || (shire_id == 16) || (shire_id == 8) || (shire_id == 12) || (shire_id == 24))
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x0 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 1*******/
		   else if ( (shire_id == 9) || (shire_id == 17) || (shire_id == 25) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x1 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 2*******/
		   else if ( (shire_id == 2) || (shire_id == 18) || (shire_id == 26) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x2 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 3*******/
		   else if ( (shire_id == 10) || (shire_id == 3) || (shire_id == 11) || (shire_id == 19) || (shire_id == 27) || (shire_id == 32))
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x3 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 4*******/
		   else if ( (shire_id == 5) || (shire_id == 20) || (shire_id == 28) || (shire_id == 4) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x4 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 5*******/
		   else if ( (shire_id == 13) || (shire_id == 21) || (shire_id == 29) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x5 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 6*******/
		   else if ( (shire_id == 6) || (shire_id == 22) || (shire_id == 30) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x6 << 6));
		   }
		   /****** Prefetches from these shires should get data always form mem-sire 7*******/
		   else if ( (shire_id == 14) || (shire_id == 15) || (shire_id == 23) || (shire_id == 7) || (shire_id == 31) )
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x7 << 6));
		   }
		   else //defualt just use mem-shire 0
		   {
			   VA = (uint64_t)(((0x80ULL)<<32) + ((0xaU)<<28U) + (shire_id << 23) + (minion_id << 18) + ((0x01*i)<<13) + (0x0 << 6));
		   }

                   
		   // Prefetch performed by one minion from each shire based on VA formed
		   prefetch_va(false,     1,   (uint64_t)(VA),  15,         512,      0 );
		   WAIT_PREFETCH_0;
	   }
   }
   C_TEST_PASS;
}

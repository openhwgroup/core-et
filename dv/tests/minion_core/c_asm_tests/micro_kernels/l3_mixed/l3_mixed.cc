/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 30
#include <inttypes.h>
#include "common.h"
#include "addr.h" 

#ifdef __cplusplus
extern "C"
#endif

//Group1 are minions 0..15
//Group2 are minions 16..31

// Limit Group1 bw to 2GB/sec; Group2 bw to 1GB/sec;
#define CYCLES_BETWEEN_REQUESTS_G1 5
#define CYCLES_BETWEEN_REQUESTS_G2 6
    
// Ucache control offset for CacheOp_Max
#define UCACHE_CNTRL_CACHEOP_MAX_OFFSET 6
// Ucache control offset for CacheOp_RepRate
#define UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET 2

// Prefetch VA offset for the field Tensor Mask
#define PREFETCH_VA_TENSOR_MASK_OFFSET 63
// Prefetch VA offset for the field DestLevel
#define PREFETCH_VA_DESTINATION_LEVEL_OFFSET 58
// Prefetch VA offset for the field Virtual Address
#define PREFETCH_VA_VIRTUAL_ADDRESS 6
// Prefetch VA offset for the field Number of Lines -1
#define PREFETCH_VA_N_LINES_OFFSET 0

//Prefetch VA mask for offset bits
#define PREFETCH_VA_MASK_OFFSET 0xFFFFFFFFC0

#define ALL_MINIONS_MASK 0xFFFFFFFF
           
// TensorWait on all previous cacheops event it
#define TENSOR_WAIT_ON_PREFETCH0 4

int main()
{
  uint64_t cstart, cend, cdiff;
  uint64_t hart_id = getHartId();
  uint64_t minion_id = ((hart_id>>1) & 0x1F);
  uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  uint64_t fcc_counters_code = 0x018;
  long data = 0;
  int baseindex = ((shire_id % 4) * 32 + minion_id)*100;
  //long STRIDE = 0x40;
  long SIZE = 0x7; // 8th 64b elemnt in a cache line (8-1)

  C_TEST_START;
  

  if( ((hart_id & 1) == 0) && (minion_id<=15)) { // Group 1 minions hitting in l3
    uint64_t cache_cntrl_g1 = (CYCLES_BETWEEN_REQUESTS_G1 << UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET);
    configure_cacheop(cache_cntrl_g1);
  
    // Shire 0-3 prefetch data to L3
    if (shire_id<=3){
      // Prefetch random indices and write some data
      for(int i=0; i<ITERS; ++i){ 
        store((long(randindices[baseindex+i])), (baseindex+i));
        fence();

	    long cache_line_end_addr =  long(randindices[baseindex+i]) + SIZE;
 
        //__evictVAs((long*)(randindices[baseindex+i]), EVICT_L3, 1, STRIDE);
        //__evictVA((long*)(randindices[baseindex+i]), EVICT_L3, 0);

        long regular_line =  long(randindices[baseindex+i]) & ~0x3f;
	    long split_line   =  long(cache_line_end_addr) & ~0x3f;

        __evictVA((long*)(regular_line), EVICT_L3, 0);

        /*Special case to handle evicts in case there was a split_line access*/
	    if (long(regular_line) != (long(cache_line_end_addr) & ~0x3f)){
        	__evictVA((long*)(split_line), EVICT_L3, 0);
	    }
      }
    } else { // Other minions prefetch random indices only
      for(int i=0; i<ITERS; ++i){
        load((long(&randindices[baseindex+i])), data); 
      }
    }

    __cacheOpWait(); //6 is to wait any cache op to finish

    // Synchronize with FCC
    uint64_t shire_ESR = ((1UL << 32) + (26U << 17) + (fcc_counters_code << 3));
    uint64_t minionMask = ALL_MINIONS_MASK ^ (1<<minion_id);

    for (uint64_t s=0; s<NUM_SHIRES; s++){
      uint64_t addr = shire_ESR     
                        | (s << 22);
      if (s == shire_id){
        store(addr, minionMask);
      } else {
        store(addr, ALL_MINIONS_MASK);
      }
    }

    for (int i=0; i < 1023; i++){
      waitFCC(0);
    }

    cstart = et_get_cycle_count();
    // L3 hits
    for (int i=0; i<ITERS; i++){
      load((long(randindices[baseindex+i])), data);
      data++;
    }
    cend = et_get_cycle_count();
    cdiff = cend - cstart;
    print_cycles(cdiff);

  } else if( ((hart_id & 1) == 0) && (minion_id>=16) && (minion_id<=31)) { // Group 2 minions missing in l3
    uint64_t cache_cntrl_g2 = (CYCLES_BETWEEN_REQUESTS_G2 << UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET);
    configure_cacheop(cache_cntrl_g2); 

    // Prefetch random indices and write some data
    for(int i=0; i<ITERS; ++i){ 
      store((long(randindices[baseindex+i])), (baseindex+i));
      fence();
      __evictVA((long*)(randindices[baseindex+i]), EVICT_MEM, 0);
    }

    __cacheOpWait(); //6 is to wait any cache op to finish

    // Synchronize with FCC
    uint64_t shire_ESR = ((1UL << 32) + (26U << 17) + (fcc_counters_code << 3));
    uint64_t minionMask = ALL_MINIONS_MASK ^ (1<<minion_id);

    for (uint64_t s=0; s<NUM_SHIRES; s++){
      uint64_t addr = shire_ESR     
                        | (s << 22);
      if (s == shire_id){
        store(addr, minionMask);
      } else {
        store(addr, ALL_MINIONS_MASK);
      }
    }

    for (int i=0; i < 1023; i++){
      waitFCC(0);
    }

    cstart = et_get_cycle_count();
    // L3 misses
    for (int i=0; i<ITERS; i++){
      load((long(randindices[baseindex+i])), data);
      data++;
    }
    cend = et_get_cycle_count();
    cdiff = cend - cstart;
    print_cycles(cdiff);

  }

  C_TEST_PASS;

  return 0;
}

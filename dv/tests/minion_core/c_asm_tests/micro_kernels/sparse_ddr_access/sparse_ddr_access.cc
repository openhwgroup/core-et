/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "addr.h" 

#ifdef __cplusplus
extern "C"
#endif

#define ITERS 32
#define ALL_MINIONS_MASK 0xFFFFFFFF
 
int main()
{
  uint64_t cstart, cend, cdiff;
  uint64_t hart_id = getHartId();
  uint64_t thread_id  = (hart_id & 1);
  uint64_t minion_id = ((hart_id>>1) & 0x1F);
  uint64_t shire_id  = ((hart_id>>6) & 0x3F);  

  uint64_t fcc_counters_code = 0x018;
  long data = 0;
  int baseindex = (shire_id * 32 + minion_id)*ITERS;

  C_TEST_START;

  if((thread_id == 0) && (shire_id != 32)){   
    // Prefetch random indices and write some data
    for(int i=0; i<ITERS; ++i){
      // load((long(&randindices[baseindex+i])), data); 
      store((long(randindices[baseindex+i])), (baseindex+i));
      fence();
      __evictVA((long*)(randindices[baseindex+i]),EVICT_MEM,0);
    }

    __cacheOpWait(); //6 is to wait any cache op to finish

    // shire_ESR bits
    // 32 31 30   29..22 21 20 19 18 17 16.3 2 1 0
    //  1  0  0 shire_id  1  1  0  1  0  Reg 0 0 0
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

    for (int i=0; i<ITERS; i++){
      load((long(randindices[baseindex+i])), data);
      data++;
    }

    cend = et_get_cycle_count();
    cdiff = cend - cstart;
    print_cycles(cdiff);
  }

  C_TEST_PASS;
  // End marker
  return 0;
}



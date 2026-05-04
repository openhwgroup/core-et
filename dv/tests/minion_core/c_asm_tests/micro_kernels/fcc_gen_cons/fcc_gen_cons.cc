/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif

#define CREDITS 10

#define MASTER_SHIRE 32
#define MASTER_MINION 0
#define COMPUTE_THREAD 0

#define NUM_THREADS 2

#define ALL_MINIONS_MASK 0xFFFFFFFF

#define FCC_COUNTERS 4
#define THREAD_COUNTERS 2
           

int main()
{
    uint64_t cstart, cend, cdiff;
    uint64_t hart_id = getHartId();
    uint64_t thread_id  = (hart_id & 1);
    uint64_t minion_id = ((hart_id>>1) & 0x1F);
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    uint64_t fcc_counters_code [FCC_COUNTERS];
    fcc_counters_code[0] = 0x018;
    fcc_counters_code[1] = 0x019;
    fcc_counters_code[2] = 0x01a;
    fcc_counters_code[3] = 0x01b;
  
    C_TEST_START;

    cstart = et_get_cycle_count();

    // Test1
    // FCC are set by thread0, minion0, shire32
    if( (thread_id == COMPUTE_THREAD) && (minion_id == MASTER_MINION) && (shire_id == MASTER_SHIRE)) {
      // shire_ESR bits
      // 32 31 30   29..22 21 20 19 18 17 16.3 2 1 0
      //  1  0  0 shire_id  1  1  0  1  0  Reg 0 0 0
      uint64_t shire_ESR = ((1UL << 32) + (26U << 17)); 
      for (int s=0; s<NUM_SHIRES-1; s++){
        for (int cnt=0; cnt<FCC_COUNTERS; cnt++){
          uint64_t addr = shire_ESR | (s << 22) | (fcc_counters_code[cnt] << 3);
          for (int c = 0; c<CREDITS; c++){
            store(addr, ALL_MINIONS_MASK);
          }
        }
      }
    } 
    else if (shire_id < NUM_SHIRES-1){
      for (int t=0; t<THREAD_COUNTERS; t++){
        for (int c = 0; c<CREDITS; c++){
          waitFCC(t);
        }
      }
    }

    // Test2
    // The same but 0 and 32 swapp the rolls
    if( (thread_id == COMPUTE_THREAD) && (minion_id == MASTER_MINION) && (shire_id == 0)) {
      uint64_t shire_ESR = ((1UL << 32) + (26U << 17)); 
      for (int s=1; s<=NUM_SHIRES-1; s++){
        for (int cnt=0; cnt<FCC_COUNTERS; cnt++){
          uint64_t addr = shire_ESR | (s << 22) | (fcc_counters_code[cnt] << 3);
          for (int c = 0; c<CREDITS; c++){
            store(addr, ALL_MINIONS_MASK);
          }
        }
      }
    } 
    else if ((shire_id != 0) && (shire_id <= NUM_SHIRES-1)){
      for (int t=0; t<THREAD_COUNTERS; t++){
        for (int c = 0; c<CREDITS; c++){
          waitFCC(t);
        }
      }
    }

    // Test3
    // The same as the first block Shire 0 sends credit to itseld as well
    if( (thread_id == COMPUTE_THREAD) && (minion_id == MASTER_MINION) && (shire_id == MASTER_SHIRE)) {
      uint64_t shire_ESR = ((1UL << 32) + (26U << 17)); 
      for (int s=0; s <= NUM_SHIRES-1; s++){
        for (int cnt=0; cnt<FCC_COUNTERS; cnt++){
          uint64_t addr = shire_ESR | (s << 22) | (fcc_counters_code[cnt] << 3);
          for (int c = 0; c<CREDITS; c++){
            store(addr, ALL_MINIONS_MASK);
          }
        }
      }
    } 

    for (int t=0; t<THREAD_COUNTERS; t++){
      for (int c = 0; c<CREDITS; c++){
        waitFCC(t);
      }
    }


    cend = et_get_cycle_count();
    cdiff = cend - cstart;
    print_cycles(cdiff);

    C_TEST_PASS;
    // End marker
    return 0;
}

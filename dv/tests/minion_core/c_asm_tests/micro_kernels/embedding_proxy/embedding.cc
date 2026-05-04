/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "addr.h"

#define ITERS 32
#define ALL_MINIONS_MASK 0xFFFFFFFF

#ifdef __cplusplus
extern "C"
#endif

// Function that returns shire id
unsigned int get_hart_id_shadow()
{
    int ret;
    __asm__ __volatile__ (
      "csrr %[ret], 0xcd0\n" //u-mode hartid shadow
      : [ret] "=r" (ret)
      :
      :
    );
    return ret;
}

int main()
{
  uint64_t hart_id = get_hart_id_shadow();
  uint64_t thread_id  = (hart_id & 1);
  uint64_t minion_id = ((hart_id>>1) & 0x1F);
  uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  uint64_t fcc_counters_code_t0 = 0x018;
  uint64_t fcc_counters_code_t1 = 0x01a;

  // shire_ESR bits
  // 32 31 30   29..22 21 20 19 18 17 16.3 2 1 0
  //  1  0  0 shire_id  1  1  0  1  0  Reg 0 0 0
  uint64_t shire_ESR_t0 = ((1UL << 32) + (26U << 17) + (fcc_counters_code_t0 << 3));
  uint64_t shire_ESR_t1 = ((1UL << 32) + (26U << 17) + (fcc_counters_code_t1 << 3));
  uint64_t minionMask = ALL_MINIONS_MASK ^ (1<<minion_id);

  long data = 0;
  int baseindex = (shire_id * 32 + minion_id)*ITERS;

  // M0, S0, T0 sets page tables and wake up all the other minions;
  if(hart_id == 0){
    for (uint64_t s=0; s<NUM_SHIRES; s++){
      uint64_t addr_t0 = shire_ESR_t0 | (s << 22);
      uint64_t addr_t1 = shire_ESR_t1 | (s << 22);
      if (s == shire_id){
        store(addr_t0, minionMask);
      } else {
        store(addr_t0, ALL_MINIONS_MASK);
      }
      store(addr_t1, ALL_MINIONS_MASK);
    }
  }

  // __asm__ __volatile__
  //   (
  //    // jump to s-code just afther the mret
  //    "li t0, 0x1000\n"
  //    "csrrc x0, mstatus, t0\n"
  //    "li t0, 0x800\n"
  //    "csrrs x0, mstatus, t0\n"
  //    "la t0, 1f\n"
  //    "csrrw zero, mepc, t0\n"
  //    "mret\n"
  //    "1:\n"
  //    : : : "t0"
  //      );


  if(thread_id == 0){
    // Prefetch random indices and write some data
    for(int i=0; i<ITERS; ++i){
      // load((long(&randindices[baseindex+i])), data);
      store((long(randindices[baseindex+i])), (baseindex+i));
      fence();
      __evictVA((long*)(randindices[baseindex+i]),EVICT_MEM,0);
    }
    __cacheOpWait(); //6 is to wait any cache op to finish

    for (uint64_t s=0; s<NUM_SHIRES; s++){
      uint64_t addr = shire_ESR_t0
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

    for (int i=0; i<ITERS; i++){
      load((long(randindices[baseindex+i])), data);
      data++;
    }
  }

  C_TEST_PASS;
  // End marker
  return 0;
}

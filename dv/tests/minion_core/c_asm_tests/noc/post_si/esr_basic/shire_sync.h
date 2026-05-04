/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _SHIRE_SYNC_
#define _SHIRE_SYNC_

#ifndef NUM_MINIONS_PER_SHIRE
   #define NUM_MINIONS_PER_SHIRE 32
#endif

#ifndef NUM_THREADS_PER_MINION
   #define NUM_THREADS_PER_MINION 2
#endif

#ifndef NUM_HART_PER_SHIRE
   #define NUM_HART_PER_SHIRE NUM_MINIONS_PER_SHIRE*NUM_THREADS_PER_MINION
#endif

#ifndef MAX_L3_SHIRES
   #define MAX_L3_SHIRES 32 // 32 Compute Shires only, Master Shire L3 is not addressable
#endif

#ifndef MAX_SCP_SHIRES
   #define MAX_SCP_SHIRES 33 // 32 Compute Shires, 1 Master Shire
#endif

#ifndef MAX_ESR_SHIRES
   #define MAX_ESR_SHIRES 33 // 32 Compute Shires, 1 Master Shire
#endif

#ifndef MAX_ACTIVE_SHIRES
   #define MAX_ACTIVE_SHIRES 33 // 32 Compute Shires, 1 Master Shire
#endif

#ifndef MAX_HARTS_SOC
   #define MAX_HARTS_SOC MAX_ACTIVE_SHIRES*NUM_HART_PER_SHIRE
#endif

#ifndef SHIRE_MASK
   #define SHIRE_MASK 0x1
#endif

#ifndef MINION_MASK
   #define MINION_MASK 0x1
#endif

#ifndef THREAD_MASK
   #define THREAD_MASK 0x1
#endif


uint64_t get_base_hart(uint64_t local_active_hart) {
   uint64_t shire_mask = SHIRE_MASK;
   uint64_t idx;

   idx = 0;
   uint64_t base_shire = 0;
   while (base_shire == 0) {
      base_shire = shire_mask & 0x1;
      shire_mask >>= 1;
      idx++;
   }
   base_shire = idx-1;

   return (base_shire * NUM_HART_PER_SHIRE) + local_active_hart;
}

uint64_t get_next_shire_id(uint64_t sid) {
   uint64_t shire_mask = SHIRE_MASK;
   uint64_t tmp = shire_mask >> (sid+1);
   uint64_t ret = sid+1;

   if (tmp == 0) {
      tmp = shire_mask;
      ret = 0;
   }

   while ((tmp & 0x1) == 0) {
      tmp = tmp >> 1;
      ret++;
   }

   return ret;
}

uint64_t get_num_local_active_harts() {
   const uint64_t threads_mask = THREAD_MASK & 0x3;
   uint64_t minions_mask = MINION_MASK;
   uint64_t ret = 0;
   while (minions_mask != 0) {
      if (minions_mask & 0x1) {
         ret++;
      }
      minions_mask >>= 1;
   }
   if (threads_mask == 3) return ret*2;
   else return ret;
}
#endif

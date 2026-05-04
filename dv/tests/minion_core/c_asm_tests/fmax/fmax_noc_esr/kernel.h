/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _NOC_FMAX_ESR_KERNEL_
#define _NOC_FMAX_ESR_KERNEL_ 1

#include "noc_power_virus_kernel.h"

#define FMAX_NOC_ESR_INIT_SEED 4518123878435467083
#define BASE_IPI_REDIRECT_FILTER_ADDR 0x80F40088

#define SEQ_READS 2
#define SEQ_WRITES 1

inline uint64_t get_base_esr_shire(uint64_t esr_base_addr, uint64_t shire_id) {
   // bits [29:22] correspond to the Shire ID
   esr_base_addr += (shire_id << 22);
   return esr_base_addr;
}

uint64_t fmax_noc_esr_thread1_kernel(volatile uint64_t *esr_ptr, uint64_t gold_val) {
   for (int i = 0; i < 5; i++) {
      for (int w = 0; w < SEQ_WRITES; w++) {
         *esr_ptr = gold_val;
      }
      for (int r = 0; r < SEQ_READS; r++) {
         uint64_t read_val = *esr_ptr;
         if (read_val != gold_val) return 1;
      }
   }
   return 0;
}

uint64_t fmax_noc_esr_thread1(int loop_size) {
   int hid = get_hart_id();
   uint64_t local_mid = (hid>>1)%32;
   uint64_t res = 0;

   uint64_t *esr_ptr = (uint64_t*) get_base_esr_shire(BASE_IPI_REDIRECT_FILTER_ADDR, local_mid);
   for (int i = 0; i < loop_size; i++) {
      res = fmax_noc_esr_thread1_kernel(esr_ptr, local_mid+1);
      if (res != 0) return res;
   }
   return res;
}

uint64_t fmax_noc_esr(int loop_size) {

   uint64_t res;
   uint64_t hid = get_hart_id();
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();

   uint64_t tid = hid & 0x1;

   uint64_t seed = FMAX_NOC_ESR_INIT_SEED+hid;
   noc_pv_init_rand_data(seed);

   soc_l2_scp *soc_scp = (soc_l2_scp*) 0x000080000000;

   uint64_t init_bank = mid%4;
   uint64_t base_addr0 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank];
   if (tid == 0) {
      setup_cache_scp();
      uint64_t base_addr1 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank + 16];
      noc_load_store(loop_size, base_addr0, base_addr1);
      res = noc_pv_self_check(seed);
   }
   else {
      res = fmax_noc_esr_thread1(loop_size);
   }
   return res;
}
#endif

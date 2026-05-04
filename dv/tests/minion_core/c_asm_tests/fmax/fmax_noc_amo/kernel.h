/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _NOC_FMAX_AMO_KERNEL_
#define _NOC_FMAX_AMO_KERNEL_ 1

#include "noc_power_virus_kernel.h"

#define FMAX_NOC_AMO_INIT_SEED 7941466007057077024

uint64_t amo_mem[34][4][32][CL_NELEMS] __attribute__ ((aligned (8192)));

uint64_t fmax_noc_amo_init_rand_data(uint64_t seed) {

   int hid = get_hart_id();
   int mid = hid >> 1;
   int local_mid = mid%32;

   int sid = get_shire_id();
   int tgt_sid = local_mid;

   for (int bank = 0; bank < 4; bank++) {
      for (uint64_t chunk = 0; chunk < CL_NELEMS; chunk++) {
         seed = noc_pv_lcg(seed);
         amo_mem[sid][bank][tgt_sid][chunk] = seed;
      }
   }
   noc_pv_evict_whole_l1();
   return seed;
}

uint64_t fmax_noc_amo_self_check(uint64_t seed) {

   int hid = get_hart_id();
   int mid = hid >> 1;
   int local_mid = mid%32;

   int sid = get_shire_id();
   int tgt_sid = local_mid;

   for (int bank = 0; bank < 4; bank++) {
      for (uint64_t chunk = 0; chunk < CL_NELEMS; chunk++) {
         uint64_t read_data = amo_mem[sid][bank][tgt_sid][chunk];
         seed = noc_pv_lcg(seed);
         if (read_data != seed) return 1;
      }
   }
   return 0;
}

uint64_t fmax_noc_amo_read(uint64_t* addr) {
   uint64_t ret;
   __asm__ __volatile__(
      "amoorg.d %[ret], x0, (%[addr])\n"
      : [ret] "=r" (ret)
      : [addr] "r" (addr)
      :
   );
   return ret;
}

void fmax_noc_amo_write(uint64_t* addr, uint64_t data) {
   __asm__ __volatile__(
      "amoswapg.d x0, %[data], (%[addr])\n"
      :
      : [data] "r" (data),
        [addr] "r" (addr)
      :
   );
}

void fmax_noc_amo_thread1(int loop_size) {
   int hid = get_hart_id();
   int mid = hid >> 1;
   int local_mid = mid%32;

   int sid = get_shire_id();
   int tgt_sid = local_mid;

   for (int i = 0; i < loop_size; i++) {
      for (uint64_t chunk = 0; chunk < CL_NELEMS; chunk++) {
         for (int bank = 0; bank < 4; bank++) {
            uint64_t read_data = fmax_noc_amo_read(&amo_mem[sid][bank][tgt_sid][chunk]);
            fmax_noc_amo_write(&amo_mem[sid][bank][tgt_sid][chunk], read_data);
         }
      }
   }
}

uint64_t fmax_noc_amo(int loop_size) {

   uint64_t res = 0;
   uint64_t hid = get_hart_id();
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();

   uint64_t tid = hid & 0x1;

   uint64_t seed = FMAX_NOC_AMO_INIT_SEED+hid;

   soc_l2_scp *soc_scp = (soc_l2_scp*) 0x000080000000;

   uint64_t init_bank = mid%4;
   uint64_t base_addr0 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank];
   if (tid == 0) {
      noc_pv_init_rand_data(seed);
      setup_cache_scp();
      uint64_t base_addr1 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank + 16];
      noc_load_store(loop_size, base_addr0, base_addr1);
      res = noc_pv_self_check(seed);
   }
   else {
      fmax_noc_amo_init_rand_data(seed);
      fmax_noc_amo_thread1(loop_size/3);
      res = fmax_noc_amo_self_check(seed);
   }
   return res;
}
#endif

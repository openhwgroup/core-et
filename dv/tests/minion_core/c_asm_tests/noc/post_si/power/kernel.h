/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _NOC_POWER_VIRUS_KERNEL_
#define _NOC_POWER_VIRUS_KERNEL_ 1

#define NOC_PV_INIT_SEED 4589732368491540805
#define NOC_PV_ELEMS_CACHE 64/sizeof(uint64_t)
#include "clear_l2_scp.h"

#define CL_NELEMS 64/sizeof(uint64_t)
typedef uint64_t scp_mem[34][2][64][CL_NELEMS];

typedef struct soc_l2_scp {
   scp_mem mem;
   uint64_t padding[(0x0800000-sizeof(scp_mem))/sizeof(uint64_t)];
} soc_l2_scp;

uint64_t noc_pv_lcg(uint64_t seed) {
   uint64_t x;
   __asm__ __volatile__ ("li %[x], 164603309694725029ul" : [x] "=r" (x) : : );

   uint64_t y;
   __asm__ __volatile__ ("li %[y], 14738995463583502973ul" : [y] "=r" (y) : : );

   return (x * seed) % y;
}

void noc_pv_evict_whole_l1() {
   excl_mode(1);
   //       use_tmask, dst, way, set, num_lines, warl
   evict_sw(        0,   1,   0,   0,       0xf,    0);
   evict_sw(        0,   1,   1,   0,       0xf,    0);
   evict_sw(        0,   1,   2,   0,       0xf,    0);
   evict_sw(        0,   1,   3,   0,       0xf,    0);
   FENCE;
   WAIT_CACHEOPS;
   excl_mode(0);
}

uint64_t noc_pv_init_rand_data(uint64_t seed) {
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();
   uint64_t hid = get_hart_id();
   uint64_t tid = hid & 0x1;

   soc_l2_scp *soc_scp = (soc_l2_scp*) 0x000080000000;

   for (int i = 0; i < 64; i++) {
      for (int j = 0; j < 8; j++) {
         seed = noc_pv_lcg(seed);
         soc_scp[mid].mem[sid][tid][i][j] = seed;
      }
   }
   noc_pv_evict_whole_l1();
   return seed;
}

uint64_t noc_pv_self_check(uint64_t seed) {
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();
   uint64_t hid = get_hart_id();
   uint64_t tid = hid & 0x1;

   soc_l2_scp *soc_scp = (soc_l2_scp*) 0x000080000000;

   for (int i = 0; i < 64; i++) {
      for (int j = 0; j < 8; j++) {
         uint64_t read_data = soc_scp[mid].mem[sid][tid][i][j];
         seed = noc_pv_lcg(seed);
         if (read_data != seed) return 0x1;
      }
   }
   return 0x0;
}

void noc_load_store(int loop_size, uint64_t base_addr0, uint64_t base_addr1) {
   uint64_t tl_enc_0 = 0xf + base_addr0;
   uint64_t tl_enc_1 = 0x20000000000000f + base_addr1;
   
   uint64_t ts_scp_enc_0 = 0x79000000000000 + base_addr0;
   uint64_t ts_scp_enc_1 = 0x1079000000000000 + base_addr1;
   
   // Set TL Stride, TL ID and TS Stride
   const uint64_t x31_enc = 0x40; // Stride(x31[47:6]) == 1 and TL ID(x31[0]) == 0
   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      :
      : [x31_enc] "r" (x31_enc)
      : "x31"
   );

   for (int i = 0; i < loop_size; i++) {

      __asm__ __volatile__ ("csrw tensor_load, %[tl_enc_0]\n" : : [tl_enc_0] "r" (tl_enc_0) : );
      WAIT_TENSOR_LOAD_0;
      __asm__ __volatile__ ("csrw tensor_store, %[ts_scp_enc_0]\n" : : [ts_scp_enc_0] "r" (ts_scp_enc_0) : );
      
      /*****************************************************************************************************************/
      
      __asm__ __volatile__ ("csrw tensor_load, %[tl_enc_1]\n" : : [tl_enc_1] "r" (tl_enc_1) : );
      WAIT_TENSOR_LOAD_0;
      __asm__ __volatile__ ("csrw tensor_store, %[ts_scp_enc_1]\n" : : [ts_scp_enc_1] "r" (ts_scp_enc_1) : );

   }
   WAIT_TENSOR_STORE;
}

inline void noc_pv_load_fp(uint64_t base_addr) {
   __asm__ __volatile__ (
      "flq2 f0, 0(%[base_addr])\n"
      "flq2 f2, 64(%[base_addr])\n"
      "flq2 f4, 128(%[base_addr])\n"
      "flq2 f6, 192(%[base_addr])\n"
      "flq2 f8, 256(%[base_addr])\n"
      "flq2 f10, 320(%[base_addr])\n"
      "flq2 f12, 384(%[base_addr])\n"
      "flq2 f14, 448(%[base_addr])\n"
      "flq2 f16, 512(%[base_addr])\n"
      "flq2 f18, 576(%[base_addr])\n"
      "flq2 f20, 640(%[base_addr])\n"
      "flq2 f22, 704(%[base_addr])\n"
      "flq2 f24, 768(%[base_addr])\n"
      "flq2 f26, 832(%[base_addr])\n"
      "flq2 f28, 896(%[base_addr])\n"
      "flq2 f30, 960(%[base_addr])\n"
      "flq2 f1, 32(%[base_addr])\n"
      "flq2 f3, 96(%[base_addr])\n"
      "flq2 f5, 160(%[base_addr])\n"
      "flq2 f7, 224(%[base_addr])\n"
      "flq2 f9, 288(%[base_addr])\n"
      "flq2 f11, 352(%[base_addr])\n"
      "flq2 f13, 416(%[base_addr])\n"
      "flq2 f15, 480(%[base_addr])\n"
      "flq2 f17, 544(%[base_addr])\n"
      "flq2 f19, 608(%[base_addr])\n"
      "flq2 f21, 672(%[base_addr])\n"
      "flq2 f23, 736(%[base_addr])\n"
      "flq2 f25, 800(%[base_addr])\n"
      "flq2 f27, 864(%[base_addr])\n"
      "flq2 f29, 928(%[base_addr])\n"
      "flq2 f31, 992(%[base_addr])\n"
      :
      : [base_addr] "r" (base_addr)
      :
   );
}

inline void noc_pv_store_fp(uint64_t base_addr) {
   __asm__ __volatile__ (
      "fsq2 f0, 0(%[base_addr])\n"
      "fsq2 f2, 64(%[base_addr])\n"
      "fsq2 f4, 128(%[base_addr])\n"
      "fsq2 f6, 192(%[base_addr])\n"
      "fsq2 f8, 256(%[base_addr])\n"
      "fsq2 f10, 320(%[base_addr])\n"
      "fsq2 f12, 384(%[base_addr])\n"
      "fsq2 f14, 448(%[base_addr])\n"
      "fsq2 f16, 512(%[base_addr])\n"
      "fsq2 f18, 576(%[base_addr])\n"
      "fsq2 f20, 640(%[base_addr])\n"
      "fsq2 f22, 704(%[base_addr])\n"
      "fsq2 f24, 768(%[base_addr])\n"
      "fsq2 f26, 832(%[base_addr])\n"
      "fsq2 f28, 896(%[base_addr])\n"
      "fsq2 f30, 960(%[base_addr])\n"
      "fsq2 f1, 32(%[base_addr])\n"
      "fsq2 f3, 96(%[base_addr])\n"
      "fsq2 f5, 160(%[base_addr])\n"
      "fsq2 f7, 224(%[base_addr])\n"
      "fsq2 f9, 288(%[base_addr])\n"
      "fsq2 f11, 352(%[base_addr])\n"
      "fsq2 f13, 416(%[base_addr])\n"
      "fsq2 f15, 480(%[base_addr])\n"
      "fsq2 f17, 544(%[base_addr])\n"
      "fsq2 f19, 608(%[base_addr])\n"
      "fsq2 f21, 672(%[base_addr])\n"
      "fsq2 f23, 736(%[base_addr])\n"
      "fsq2 f25, 800(%[base_addr])\n"
      "fsq2 f27, 864(%[base_addr])\n"
      "fsq2 f29, 928(%[base_addr])\n"
      "fsq2 f31, 992(%[base_addr])\n"
      :
      : [base_addr] "r" (base_addr)
      :
   );
}

void noc_pv_load_store_fp(int loop_size, uint64_t base_addr0) {
   for (int i = 0; i < loop_size; i++) {
      noc_pv_load_fp(base_addr0);
      noc_pv_store_fp(base_addr0);
   }
}

uint64_t noc_power_virus(int loop_size) {

   uint64_t hid = get_hart_id();
   uint64_t mid = get_minion_id();
   uint64_t sid = get_shire_id();

   uint64_t tid = hid & 0x1;

   uint64_t seed = NOC_PV_INIT_SEED+hid;
   noc_pv_init_rand_data(seed);

   soc_l2_scp *soc_scp = (soc_l2_scp*) 0x000080000000;

   uint64_t init_bank = mid%4;
   uint64_t base_addr0 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank];
   if ((hid&0x1) == 0) {
      setup_cache_scp();
      uint64_t base_addr1 = (uint64_t) soc_scp[mid].mem[sid][tid][init_bank + 16];
      noc_load_store(loop_size, base_addr0, base_addr1);
   }
   else {
      noc_pv_load_store_fp(loop_size/2, base_addr0);
   }

   return noc_pv_self_check(seed);
}
#endif

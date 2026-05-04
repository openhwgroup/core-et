/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _SHIRE_CACHE_POWER_VIRUS_KERNEL_
#define _SHIRE_CACHE_POWER_VIRUS_KERNEL_ 1

#define SC_PV_INIT_SEED 3609532315644332150

#define SC_PV_ELEMS_CACHE 64/sizeof(uint64_t)

// M0 mode L2 SCP size is 2.5MB
#define SC_PV_L2_SCP_SIZE 2621440
#define SC_PV_SCP_SIZE_PER_HART SC_PV_L2_SCP_SIZE/64
#define SC_PV_SCP_LINES_PER_HART SC_PV_SCP_SIZE_PER_HART/64
uint64_t (*sc_pv_local_scp)[SC_PV_SCP_LINES_PER_HART][SC_PV_ELEMS_CACHE] = (uint64_t(*)[SC_PV_SCP_LINES_PER_HART][SC_PV_ELEMS_CACHE]) 0xbf800000;

#define L1_SCP_NLINES 48

uint64_t sc_pv_lcg(uint64_t seed) {
   uint64_t x;
   __asm__ __volatile__ ("li %[x], 164603309694725029ul" : [x] "=r" (x) : : );

   uint64_t y;
   __asm__ __volatile__ ("li %[y], 14738995463583502973ul" : [y] "=r" (y) : : );

   return (x * seed) % y;
}

uint64_t sc_pv_self_check(uint64_t scp[][SC_PV_ELEMS_CACHE], uint64_t seed) {
   for (uint64_t i = 0; i < 16; i++) {
      for (uint64_t j = 0; j < SC_PV_ELEMS_CACHE; j++) {
         seed = sc_pv_lcg(seed);
      }
   }
   for (uint64_t i = 16; i < SC_PV_SCP_LINES_PER_HART; i++) {
      for (uint64_t j = 0; j < SC_PV_ELEMS_CACHE; j++) {
         uint64_t read_data = scp[i][j];
         seed = sc_pv_lcg(seed);
         if (read_data != seed) return 0x1;
      }
   }
   return 0x0;
}

#include "clear_l2_scp.h"

void sc_pv_evict_coalescing_buffer() {
   #include "evict_coalescing_buffer.h"
}

void sc_pv_evict_whole_l1() {
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

uint64_t sc_pv_init_rand_data(uint64_t scp[][SC_PV_ELEMS_CACHE], uint64_t seed) {
   for (uint64_t i = 0; i < SC_PV_SCP_LINES_PER_HART; i++) {
      for (uint64_t j = 0; j < SC_PV_ELEMS_CACHE; j++) {
         seed = sc_pv_lcg(seed);
         scp[i][j] = seed;
      }
   }
   sc_pv_evict_whole_l1();
   return seed;
}

void sc_pv_load_store_l1scp_0(uint64_t scp[][SC_PV_ELEMS_CACHE]) {
   const uint64_t tl_enc_0 = 0xf;
   const uint64_t tl_enc_1 = 0x20000000000000f;
   const uint64_t tl_enc_2 = 0x40000000000000f;
   
   const uint64_t ts_scp_enc_0 = 0x79000000000000;
   const uint64_t ts_scp_enc_1 = 0x1079000000000000;
   const uint64_t ts_scp_enc_2 = 0x2079000000000000;

   // Set TL Stride, TL ID and TS Stride
   const uint64_t x31_enc = 64; // Stride(x31[47:6]) == 1 and TL ID(x31[0]) == 0
   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      :
      : [x31_enc] "r" (x31_enc)
      : "x31"
   );

   uint64_t tl_enc;
   uint64_t ts_fromscp_enc;

   /*****************************************************************************************************************/

   tl_enc = tl_enc_0 + ((uint64_t) scp[16]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_0 + ((uint64_t) scp[0]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/

   tl_enc = tl_enc_1 + ((uint64_t) scp[32]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_1 + ((uint64_t) scp[16]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/

   tl_enc = tl_enc_2 + ((uint64_t) scp[48]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_2 + ((uint64_t) scp[32]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/
}

void sc_pv_load_store_l1scp_1(uint64_t scp[][SC_PV_ELEMS_CACHE]) {
   const uint64_t tl_enc_0 = 0xf;
   const uint64_t tl_enc_1 = 0x20000000000000f;
   const uint64_t tl_enc_2 = 0x40000000000000f;
   
   const uint64_t ts_scp_enc_0 = 0x79000000000000;
   const uint64_t ts_scp_enc_1 = 0x1079000000000000;
   const uint64_t ts_scp_enc_2 = 0x2079000000000000;

   // Set TL Stride, TL ID and TS Stride
   const uint64_t x31_enc = 64; // Stride(x31[47:6]) == 1 and TL ID(x31[0]) == 0
   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      :
      : [x31_enc] "r" (x31_enc)
      : "x31"
   );

   uint64_t tl_enc;
   uint64_t ts_fromscp_enc;

   /*****************************************************************************************************************/

   tl_enc = tl_enc_0 + ((uint64_t) scp[32]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_0 + ((uint64_t) scp[48]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/

   tl_enc = tl_enc_1 + ((uint64_t) scp[16]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_1 + ((uint64_t) scp[32]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/

   tl_enc = tl_enc_2 + ((uint64_t) scp[0]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;

   ts_fromscp_enc = ts_scp_enc_2 + ((uint64_t) scp[16]);
   __asm__ __volatile__ ("csrw tensor_store, %[ts_fromscp_enc]\n" : : [ts_fromscp_enc] "r" (ts_fromscp_enc) : );

   /*****************************************************************************************************************/
}

inline void sc_pv_load_fp(uint64_t base_addr) {
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

inline void sc_pv_store_fp(uint64_t base_addr) {
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

void sc_pv_load_store_fp_0(uint64_t scp[][SC_PV_ELEMS_CACHE]) {
   sc_pv_load_fp((uint64_t) scp[16]);
   sc_pv_store_fp((uint64_t) scp[0]);

   sc_pv_load_fp((uint64_t) scp[32]);
   sc_pv_store_fp((uint64_t) scp[16]);

   sc_pv_load_fp((uint64_t) scp[48]);
   sc_pv_store_fp((uint64_t) scp[32]);
}

void sc_pv_load_store_fp_1(uint64_t scp[][SC_PV_ELEMS_CACHE]) {
   sc_pv_load_fp((uint64_t) scp[32]);
   sc_pv_store_fp((uint64_t) scp[48]);

   sc_pv_load_fp((uint64_t) scp[16]);
   sc_pv_store_fp((uint64_t) scp[32]);

   sc_pv_load_fp((uint64_t) scp[0]);
   sc_pv_store_fp((uint64_t) scp[16]);
}

uint64_t shire_cache_power_virus_thread0(int loop_size) {
   const int num_iter_0 = (((int)(SC_PV_SCP_LINES_PER_HART/L1_SCP_NLINES))*L1_SCP_NLINES);
   const int num_iter_1 = num_iter_0 - L1_SCP_NLINES;

   uint64_t hid = get_hart_id();
   uint64_t local_hid = hid%64;

   uint64_t seed = SC_PV_INIT_SEED+hid;
   sc_pv_init_rand_data(sc_pv_local_scp[local_hid], seed);

   setup_cache_scp();
   for (int it = 0; it < loop_size; it++) {
      int i = 0;
      while (i < num_iter_0) {
         sc_pv_load_store_l1scp_0(&sc_pv_local_scp[local_hid][i]);
         i+=L1_SCP_NLINES;
      }
      WAIT_TENSOR_STORE;
      //sc_pv_evict_coalescing_buffer();
   
      i = num_iter_1;
      while (i >= 0) {
         sc_pv_load_store_l1scp_1(&sc_pv_local_scp[local_hid][i]);
         i-=L1_SCP_NLINES;
      }
      WAIT_TENSOR_STORE;
      //sc_pv_evict_coalescing_buffer();
   }
   setup_cache_shared();

   return sc_pv_self_check(sc_pv_local_scp[local_hid], seed);
}

uint64_t shire_cache_power_virus_thread1(int loop_size_1) {
   const int num_iter_0 = (((int)(SC_PV_SCP_LINES_PER_HART/L1_SCP_NLINES))*L1_SCP_NLINES);
   const int num_iter_1 = num_iter_0 - L1_SCP_NLINES;

   uint64_t hid = get_hart_id();
   uint64_t local_hid = hid%64;

   uint64_t seed = SC_PV_INIT_SEED+hid;
   sc_pv_init_rand_data(sc_pv_local_scp[local_hid], seed);

   for (int it = 0; it < loop_size_1; it++) {
      int i = 0;
      while (i < num_iter_0) {
         sc_pv_load_store_fp_0(&sc_pv_local_scp[local_hid][i]);
         i+=L1_SCP_NLINES;
      }
   
      i = num_iter_1;
      while (i >= 0) {
         sc_pv_load_store_fp_1(&sc_pv_local_scp[local_hid][i]);
         i-=L1_SCP_NLINES;
      }
   }
   return sc_pv_self_check(sc_pv_local_scp[local_hid], seed);
}

uint64_t shire_cache_power_virus(int loop_size) {
   int loop_size_1 = (int) (loop_size/3);

   uint64_t res;
   uint64_t hid = get_hart_id();
   if ((hid&0x1) == 0) {
      res = shire_cache_power_virus_thread0(loop_size);
   }
   else {
      res = shire_cache_power_virus_thread1(loop_size_1);
   }
   return res;
}
#endif

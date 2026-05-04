/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#define SC_NOC_DRAM_SIZE (uint64_t) 28*1024*1024*1024
#define SC_NOC_DRAM_CACHE_LINES SC_NOC_DRAM_SIZE/64
#define SC_NOC_CACHE_LINES SC_NOC_DRAM_CACHE_LINES/2176
#define SC_NOC_ELEMS_CACHE 64/sizeof(uint64_t)

uint64_t sc_noc_lcg(uint64_t seed) {
   uint64_t x;
   __asm__ __volatile__ ("li %[x], 164603309694725029ul" : [x] "=r" (x) : : );

   uint64_t y;
   __asm__ __volatile__ ("li %[y], 14738995463583502973ul" : [y] "=r" (y) : : );

   return (x * seed) % y;
}

uint64_t sc_noc_init_rand_data(uint64_t mem[][SC_NOC_ELEMS_CACHE], uint64_t seed) {
   for (uint64_t j = 0; j < SC_NOC_ELEMS_CACHE; j++) {
      for (uint64_t i = 0; i < SC_NOC_CACHE_LINES; i++) {
         seed = sc_noc_lcg(seed);
         mem[i][j] = seed;
      }
   }
   return seed;
}

uint64_t sc_noc_read_and_check(uint64_t mem[][SC_NOC_ELEMS_CACHE], uint64_t seed) {
   for (uint64_t j = 0; j < SC_NOC_ELEMS_CACHE; j++) {
      for (uint64_t i = 0; i < SC_NOC_CACHE_LINES; i++) {
         uint64_t read_data = mem[i][j];
         seed = sc_noc_lcg(seed);
         if (read_data != seed) return 1;
      }
   }
   return 0;
}

#define SC_NOC_INIT_SEED 3609532315644332150

uint64_t sc_noc_power_virus(int loop_size) {
   uint64_t hid = get_hart_id();
   uint64_t seed = SC_NOC_INIT_SEED+hid;

   if ((hid&0x1) == 0) {
      setup_cache_scp();
   }
   /*
    * Address 0x8100000000 maps to the begining of the:
    * - DRAM Low Memory subregion: [0x81_0000_0000, 0x87_FFFF_FFFF]
    */
   uint64_t (*sc_noc_pv_glbl_mem)[SC_NOC_CACHE_LINES][SC_NOC_ELEMS_CACHE] = (uint64_t (*)[SC_NOC_CACHE_LINES][SC_NOC_ELEMS_CACHE]) 0x8100000000;
   sc_noc_init_rand_data(sc_noc_pv_glbl_mem[hid], seed);

   uint64_t res = 0;
   for (int i = 0; i < loop_size; i++) {
      res = sc_noc_read_and_check(sc_noc_pv_glbl_mem[hid], seed);
      if (res != 0) return res;
   }
   return res;
}

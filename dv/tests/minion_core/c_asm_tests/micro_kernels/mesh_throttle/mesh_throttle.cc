/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "mesh_throttle.h"


//Group1 are minions 0..7
//Group2 are minions 8..15

#define N_LINES_PREFETCH_G1 256ULL
#define N_LINES_PREFETCH_G2 512ULL

#define MAX_CACHE_LINES 16

#define CYCLES_BETWEEN_REQUESTS_G1 6
#define CYCLES_BETWEEN_REQUESTS_G2 5
// G1 repeat rate 1req/64cycles
// G2 repeat rate 1 req/32cycles

#define MAX_OUT_REQUESTS 32

// Prefetch VA offset for the field Tensor Mask
#define PREFETCH_VA_TENSOR_MASK_OFFSET 63
// Prefetch VA offset for the field DestLevel
#define PREFETCH_VA_DESTINATION_LEVEL_OFFSET 58
// Prefetch VA offset for the field Number of Lines -1
#define PREFETCH_VA_N_LINES_OFFSET 0

// Ucache control offset for CacheOp_Max
#define UCACHE_CNTRL_CACHEOP_MAX_OFFSET 6
// Ucache control offset for CacheOp_RepRate
#define UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET 2

// TensorWait on all previous cacheops event it
#define TENSOR_WAIT_ON_PREFETCH0 4

#define dest_L2 1ULL
#define dest_MEM 3ULL

#define ACTIVE_MINIONS_MASK 0x0000FFFF
#define ACTIVE_MINIONS 16

int main()
{
    uint64_t cstart, cend, cdiff;
    uint64_t hart_id = getHartId();
    uint64_t minion_id = ((hart_id>>1) & 0x1F);
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    uint64_t fcc_counters_code = 0x018;
    uint64_t minionMask = ACTIVE_MINIONS_MASK ^ (1<<minion_id);
    uint64_t shire_ESR = ((1UL << 32) + (shire_id << 22) + (26U << 17) + (fcc_counters_code << 3));

    uint64_t conf;

    change_to_user_mode();

    if( ((hart_id & 1) == 0) && (minion_id<=7) && shire_id==0) { // Group 1 minions
      uint64_t cache_cntrl_g1 = ((MAX_OUT_REQUESTS-1) << UCACHE_CNTRL_CACHEOP_MAX_OFFSET)
                                | (CYCLES_BETWEEN_REQUESTS_G1 << UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET);
      configure_cacheop(cache_cntrl_g1);

      uint64_t base_addr_g1 = 0x8200100000;
      uint64_t start_addr =  base_addr_g1  + minion_id * N_LINES_PREFETCH_G1 * N_BYTES_CL; //each minion reads 256 CL * 64 Bytes/CL

      // Write some data in mem locations that will be prefetched
      uint64_t addr = start_addr;
      uint64_t data = minion_id * N_LINES_PREFETCH_G1;
      for(long long unsigned int i=0; i<N_LINES_PREFETCH_G1; ++i){
        store((long(addr)), (data+i));
        fence();
        __evictVA((long*)(addr), dest_MEM, 0);
        addr += N_BYTES_CL;
      }
      __cacheOpWait(); //6 is to wait any cache op to finish

      // Synchronize with FCC
      store(shire_ESR, minionMask);
      for (int i=0; i < ACTIVE_MINIONS-1; i++){
        waitFCC(0);
      }

      uint64_t conf_g1 = (dest_L2 << PREFETCH_VA_DESTINATION_LEVEL_OFFSET)
                         | ((MAX_CACHE_LINES-1) << PREFETCH_VA_N_LINES_OFFSET);

      int prefetch_req1 = N_LINES_PREFETCH_G1 / MAX_CACHE_LINES;

      cstart = et_get_cycle_count();

      for (int i=0; i < prefetch_req1; i++){
        conf = conf_g1 | (start_addr &  ~(0x3F));
        prefetch(conf, N_BYTES_CL, 0);
        start_addr += MAX_CACHE_LINES * N_BYTES_CL;
      }

      tensorWait(TENSOR_WAIT_ON_PREFETCH0);

      cend = et_get_cycle_count();
      cdiff = cend - cstart;
      print_cycles(cdiff);

    } else if( ((hart_id & 1) == 0) && (minion_id>=8) && (minion_id<=15) && shire_id==0) { // Group 2 minions
      uint64_t cache_cntrl_g2 = ((MAX_OUT_REQUESTS-1) << UCACHE_CNTRL_CACHEOP_MAX_OFFSET)
                                | (CYCLES_BETWEEN_REQUESTS_G2 << UCACHE_CNTRL_CACHEOP_REPRATE_OFFSET);
      configure_cacheop(cache_cntrl_g2);

      uint64_t base_addr_g2 = 0x8300200000;
      uint64_t start_addr =  base_addr_g2  + minion_id * N_LINES_PREFETCH_G2 * N_BYTES_CL; //each minion reads 512 CL * 64 Bytes/CL

      // Write some data in mem locations that will be prefetched
      uint64_t addr = start_addr;
      uint64_t data = minion_id * N_LINES_PREFETCH_G2;
      for(long long unsigned int i=0; i<N_LINES_PREFETCH_G2; ++i){
        store((long(addr)), (data+i));
        fence();
        __evictVA((long*)(addr), dest_MEM, 0);
        addr += N_BYTES_CL;
      }
      __cacheOpWait(); //6 is to wait any cache op to finish

      // Synchronize with FCC
      store(shire_ESR, minionMask);
      for (int i=0; i < ACTIVE_MINIONS-1; i++){
        waitFCC(0);
      }

      uint64_t conf_g2 = (dest_L2 << PREFETCH_VA_DESTINATION_LEVEL_OFFSET)
                         | ((MAX_CACHE_LINES-1) << PREFETCH_VA_N_LINES_OFFSET);

      int prefetch_req2 = N_LINES_PREFETCH_G2 / MAX_CACHE_LINES;

      cstart = et_get_cycle_count();

      for (int i=0; i < prefetch_req2; i++){
        conf = conf_g2 | (start_addr &  ~(0x3F));
        prefetch(conf, N_BYTES_CL, 0);
        start_addr += MAX_CACHE_LINES * N_BYTES_CL;
      }

      tensorWait(TENSOR_WAIT_ON_PREFETCH0);
      cend = et_get_cycle_count();
      cdiff = cend - cstart;
      print_cycles(cdiff);
    }

    fence();
    wfi();
    // End marker
    return 0;
}

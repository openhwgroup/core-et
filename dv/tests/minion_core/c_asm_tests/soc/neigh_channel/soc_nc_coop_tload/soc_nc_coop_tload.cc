/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "tensors.h"

volatile int __attribute__((aligned(4096))) __attribute__((section(".address_array"))) ptr[1024];
#define NUM_ITER 10
#define NEIGH0_IPI_REDIRECT_PC 0x80100040

#define SET_N_BITS(n) ((1ULL << (n)) - 1ULL)


int main()
{
   uint64_t mid, tid, sid, nid;

   // Number of Minions per Neigh that are cooperating
   uint64_t ncoop_minions = 8;
   // Number of Neighs per Shire that are cooperating
   uint64_t ncoop_neighs  = 1;
   // Cooperation ID
   uint64_t coop_id       = 1;

    // Init marker
    C_TEST_START;

    volatile uint64_t *esr_poll_value;

    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );

    // Get minion ID and thread id
    mid = (get_minion_id())%8;
    tid = get_thread_id();
    nid = get_neigh_id();
    sid = get_shire_id();    

    if ((tid == 0) && (mid < ncoop_minions) && (nid < ncoop_neighs)) {
      enable_shire_coop(sid);
      setup_cache_scp();
      // Setup cooperation mask
      tensor_coop(SET_N_BITS(ncoop_neighs), SET_N_BITS(ncoop_minions), coop_id);

      // Set configurations
      et_tensor_load_conf_t load_conf_matrixA;
    
      // Matrix A
      load_conf_matrixA.use_tmask      = false;
      load_conf_matrixA.use_coop       = true;
      load_conf_matrixA.use_tenb       = false;
      load_conf_matrixA.dst_start      = 0;
      load_conf_matrixA.transformation = 0x0UL;
      load_conf_matrixA.rd_l2scp       = 0x0U;
      load_conf_matrixA.addr           = (uint64_t)(void*)(ptr);
      load_conf_matrixA.offset         = 0;
      load_conf_matrixA.num_lines      = 15;
      load_conf_matrixA.stride         = 64;
      load_conf_matrixA.id             = 0;  
  
      for ( int i = 0; i < NUM_ITER; i++) {
          // Load matrix A
          et_tensor_load (&load_conf_matrixA);
          WAIT_TENSOR_LOAD_0;
      }

    }
    // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}


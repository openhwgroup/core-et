/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"
#include "../../tensors/tensor_load/tensor_load.h"
#include "perf_cntr_diag.h"
#include "utils.h"

#ifndef LOOP_SIZE
    #define LOOP_SIZE 7
#endif

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 10000
   #ifndef NUM_ITER
   #define NUM_ITER 8
   #endif
#else
   #ifndef NUM_ITER
   #define NUM_ITER 64
   #endif
#endif


#define BLOB_SIZE 1048576 // 1M
#define CL_BLOB_SIZE 1048576/64
// Tensor Load accesses multiple addresses. To avoid acessing beyond the BLOB
// reduce the size of the BLOB to the half
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start_tload;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start_tload:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);


int main() {

   // Init marker
   C_TEST_START;
   uint64_t mid, tid;
   et_tensor_load_conf_t  load_conf_matrix, load_conf_matrix_b;

   // Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   PmuCounter<3>::configure(pmu_event_NOCOUNT);
   PmuCounter<4>::configure(pmu_event_NOCOUNT);
   PmuCounter<7>::configure(pmu_event_NOCOUNT);

   PmuCounter<3>::write(0);
   PmuCounter<4>::write(0);
   PmuCounter<7>::write(0);

   PmuCounter<3>::configure(pmu_event_L2_MISS_REQ_REJ);
   PmuCounter<4>::configure(pmu_event_L2_EVICT_REQ_REJ);
   PmuCounter<7>::configure(pmu_neigh_event_ET_REQ_FIFO);

   // Only thread 0 executes this
   if (tid == 0) {
      volatile cl_data_t *buffer = &_binary_blob_bin_start_tload;
      setup_cache_scp();

      for (int i = 0; i < LOOP_SIZE; i++) {
         uint64_t ptr = (uint64_t)(void*)buffer[(mid*mid*i)%TL_BLOB_SIZE].data[0];
         do_tensor_load_common(&load_conf_matrix, (void*) ptr, false);
         uint64_t load_op = i%7;

         if (i > 6) {
             load_conf_matrix.use_tmask = et_get_rand_word(0,1) ? true : false;
             if (load_conf_matrix.use_tmask) {
                 uint64_t tmask = et_get_rand_word();
                 tensor_mask(0x0, tmask);
             }
         }

         switch (load_op) {
            case 0:
               do_tensor_load(&load_conf_matrix);
               break;
            case 1:
               load_conf_matrix.use_tmask = 0;
               do_tensor_load(&load_conf_matrix);
               WAIT_TENSOR_LOAD_0;

               do_tensor_load_common(&load_conf_matrix_b, (void*) ptr, false);
               load_conf_matrix_b.num_lines = (load_conf_matrix_b.num_lines & 0xC) - 1;
               setup_tensor_load_B(&load_conf_matrix_b);

               do_tensorFMA(&load_conf_matrix, &load_conf_matrix_b);
               WAIT_TENSOR_FMA;

               tensor_error_check();
               break;
            case 2:
               do_tensor_load_interleave8(&load_conf_matrix);
               break;
            case 3:
               do_tensor_load_interleave16(&load_conf_matrix);
               break;
            case 4:
               do_tensor_load_transpose8(&load_conf_matrix);
               break;
            case 5:
               do_tensor_load_transpose16(&load_conf_matrix);
               break;
            case 6:
               do_tensor_load_transpose32(&load_conf_matrix);
               break;
            default:
               do_tensor_load(&load_conf_matrix);
         }
         FENCE;
         WAIT_TENSOR_LOAD_0;
         tensor_error_check();
      }
      do_tensor_load_illegal_config_0(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
      do_tensor_load_illegal_config_1(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;

   } else {

      // Reserve full-cache space in the stack (64 bytes per cacheline, 16 sets, 4 ways)
      uint8_t data[(64*16*4)+64];
      uint64_t cl_buffer = (uint64_t) data;
      uint64_t ii;
      uint64_t rnd;


      for (ii = 0; ii < NUM_ITER; ++ii) {
         uint8_t *base_addr;

         // Get a random number
         rnd = et_get_rand_dword();

         // Warm up the L1 cache
         base_addr = (uint8_t*) ((cl_buffer+64)%64);
         warm_l1d(base_addr, 100);        // 100% warm
         FENCE;


         bool use_tmask = rnd % 2;
         uint64_t tmask;
         if (use_tmask) {
            tmask = rnd >> 32;
            __asm__ __volatile__ ( "csrw 0x805, %[n]\n" : :[n] "r" (tmask) : );
         }

         uint64_t  dest      = 3;   // 1=L2 (Post-Si Phase 1 cannot access NOC/L3)
         uint64_t  num_lines = (rnd >> 16) % 16;
         uint64_t  stride    = 64 * ((rnd >> 24) % 16) ;
         uint64_t  id        = (rnd >> 3) % 2 ;

         // Do some random EVICT VA
         //       use_tmask  dest             addr                  num_lines          stride             id          warl
         //       ---------  ---------------  --------------------  -----------------  -----------------  ----------  ----
       //evict_va(use_tmask, ((rnd >> 1)%4),  (uint64_t)base_addr,  (rnd >> 16) % 16,  64*((rnd>>24)%16), (rnd>>3)%2, rnd);
         evict_va(use_tmask, dest,            (uint64_t)base_addr,  num_lines,         stride,            id,         rnd);

         WAIT_CACHEOPS;

      } // for (ii = 0 ...)

   }

   PmuCounter<3>::read();
   PmuCounter<4>::read();
   PmuCounter<7>::read();

   // End marker
   C_TEST_PASS;
   return 0;
}

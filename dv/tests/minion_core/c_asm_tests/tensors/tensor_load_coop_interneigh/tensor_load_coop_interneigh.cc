/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Directed test that exercices the basic functionality
//               of the Esperanto tensor cooperative load inter-neigh feature 
// Note: 
//       - Machine trap handler is configured in this test in such way that the test would branch to DEAD, should 
//        an exception occur.  

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"
#include "tensor_load.h"

#define SET_N_BITS(n) ((1ULL << (n)) - 1ULL)

#ifndef LOOP_SIZE
    #define LOOP_SIZE 7
#endif

#ifdef SELF_CHECK
   #include "tl_selfchecking.cpp"
#endif

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      12            \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );

int main() {

   uint64_t mid, tid, sid, nid;
   et_tensor_load_conf_t  load_conf_matrix;
   #ifndef SELF_CHECK
   et_tensor_load_conf_t  load_conf_matrix_b;
   #endif

   volatile cl_data_t *ptr = &_binary_blob_bin_start;

   // Number of Minions per Neigh that are cooperating
   uint64_t ncoop_minions = 8;
   // Number of Neighs per Shire that are cooperating
   uint64_t ncoop_neighs  = 4;
   // Cooperation ID
   uint64_t coop_id       = 0;

   // Init marker
   C_TEST_START;

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   #ifdef SELF_CHECK
   uint64_t selfcheck_tmask = 0;
   uint8_t pre_data[L1SCP_SIZE_BYTES] __attribute__((aligned(64)));
   uint8_t post_data[L1SCP_SIZE_BYTES]  __attribute__((aligned(64)));
   #endif

   //1.0 Get minion ID and thread id
   mid = (get_minion_id())%8;
   tid = get_thread_id();
   nid = get_neigh_id();
   sid = get_shire_id();

   if ((tid == 0) && (mid < ncoop_minions) && (nid < ncoop_neighs)) {
      enable_shire_coop(sid);
      setup_cache_scp();
      // Setup cooperation mask
      tensor_coop(SET_N_BITS(ncoop_neighs), SET_N_BITS(ncoop_minions), coop_id);

      // Do the tensor load
      for (int i = 0; i < LOOP_SIZE; i++) {
         do_tensor_load_common(&load_conf_matrix, (void*) ptr, true);
         uint64_t load_op = i%7;

         #ifdef SELF_CHECK
         dump_whole_l1scp((uint64_t*) pre_data);
         #endif

         switch (load_op) {
            case 0:
               do_tensor_load(&load_conf_matrix);
               break;
            case 1:
               load_conf_matrix.use_tmask = 0;
               do_tensor_load(&load_conf_matrix);
               WAIT_TENSOR_LOAD_0;

               #ifndef SELF_CHECK
               //do_tensor_load_common(&load_conf_matrix_b, (void*) ptr, false);
               do_tensor_load_common(&load_conf_matrix_b, (void*) ptr, true);     // (Charles) coop = true (for TenB too)
               load_conf_matrix_b.num_lines = (load_conf_matrix_b.num_lines & 0xC) - 1;
               setup_tensor_load_B(&load_conf_matrix_b);

               do_tensorFMA(&load_conf_matrix, &load_conf_matrix_b);
               WAIT_TENSOR_FMA;

               tensor_error_check();
               #endif
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
         #ifdef SELF_CHECK
         dump_whole_l1scp((uint64_t*) post_data);
         uint64_t ret = tl_selfchecking(&load_conf_matrix, selfcheck_tmask, (uint64_t*) pre_data, (uint64_t*) post_data);
         if (ret != PASS) C_TEST_FAIL;
         #endif
      }
      do_tensor_load_illegal_config_0(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
      do_tensor_load_illegal_config_1(&load_conf_matrix);
      FENCE;
      WAIT_TENSOR_LOAD_0;
   }
   C_TEST_PASS;
}

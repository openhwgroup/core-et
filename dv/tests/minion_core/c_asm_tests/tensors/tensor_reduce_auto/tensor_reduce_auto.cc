/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// This test exercises tensor reduce auto
// Run with at least 8 Minions
//
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#ifndef NUM_MINIONS
#define NUM_MINIONS 8
#endif
#include "tr_selfcheck.h"

#ifndef NUM_ITER
   #define NUM_ITER 128
#endif

#ifndef MAX_TREE
   #define MAX_TREE 2
#endif

int main()
{
   uint64_t mid, tid;
   C_TEST_START;

   tid = get_thread_id();
   mid = get_minion_id();

   #ifdef POSTSI_PARAM
      mid = mid % 8;  // have to assume the test is always neigh-based;   
   #endif

   // Setup machine trap vector
   // Normally we do not expect any interrupt
   setup_mtrap_vector();

   uint64_t start_reg  = 0;
   uint64_t num_reg    = 1;
   uint64_t tree_depth = 0;
   uint64_t operation  = 0;

   if (tid == 0) {
      uint64_t ptr = get_ptr(mid);
      init_fp_regs((void*) ptr);
      for (int ii = 0; ii < NUM_ITER; ii++) {
         FENCE;

         #ifdef SELF_CHECK
            dump_vpu_rf((uint32_t*) ptr);
            wait_for_everyone();
         #endif

         tensor_reduce_auto(start_reg, operation, num_reg, tree_depth);
         WAIT_TENSOR_REDUCE;
         tensor_error_check();

         #ifdef SELF_CHECK
            self_check_tensor_reduce(mid, start_reg, operation, num_reg, tree_depth, TR_AUTO);
         #endif

         start_reg  = (start_reg  + 1) % 32;
         num_reg    = (num_reg    + 1) % 96;
         tree_depth = (tree_depth + 1) % MAX_TREE;
         operation  = (operation == 0 || operation == 4) ? (operation + 2)     :
                                                          ((operation + 1) % 9);

         // Charles: temp: illegal Ops:
         #ifdef REDUCE_ILLEGAL_OP
            if (ii == 10) {
               operation = 1;
            }
            else if (ii == 20) {
               operation = 5;
            }
            else if (ii == 30) {
               operation = 10;
            }
         #endif
      }
   }
   C_TEST_PASS;

   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#ifndef NUM_MINIONS
#define NUM_MINIONS 8
#endif
#include "tr_selfcheck.h"

int main()
{
   uint64_t mid, tid;
   C_TEST_START;

   // Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();


   if (tid == 0) {
      uint64_t ptr = get_ptr(mid);

      // load some VPU registers with FLQ instructions from the first array
      init_fp_regs((void*) ptr);
      #ifdef SELF_CHECK
         dump_vpu_rf((uint32_t*) ptr);
         wait_for_everyone();
      #endif
      FENCE;

      uint64_t start_reg = 0;
      uint64_t operation = FGET;
      uint64_t num_reg   = 6;
      uint64_t partnerID = 0;
      uint64_t action    = 3;

      // Charles: temp: illegal_Op:  
      #ifdef REDUCE_ILLEGAL_OP
         operation = 1;
      #endif

      tensor_reduce(start_reg, operation, num_reg, partnerID, action);
      //FENCE;
      WAIT_TENSOR_REDUCE;
      tensor_error_check();

      #ifdef SELF_CHECK
         self_check_tensor_reduce(mid, start_reg, operation, num_reg, partnerID, action);
      #endif

   }
   // End marker
   C_TEST_PASS;

   return 0;
}

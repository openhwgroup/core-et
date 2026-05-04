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
   // Normally we do not expect any interrupt
   setup_mtrap_vector();

   uint64_t start_reg = 0;
   uint64_t num_reg   = 1;
   uint64_t partnerID;
   uint64_t operation = FGET;

   if (tid == 0) {
      uint64_t ptr = get_ptr(mid);
      init_fp_regs((void*) ptr);
      FENCE;

      #ifdef SELF_CHECK
         uint64_t action;
         dump_vpu_rf((uint32_t*) ptr);
         wait_for_everyone();
      #endif

      // Normal Send / Receive Tensor Reduce
      if ((mid % 2) == 0) {
         // Sends to MinionID + 1
         partnerID = mid + 1;
         #ifdef SELF_CHECK
            action = TR_SND;
         #endif
         tensor_reduce_send(start_reg, num_reg, partnerID);
      }
      else {
         // Receives from MinionID - 1
         partnerID = mid - 1;
         #ifdef SELF_CHECK
            action = TR_RCV;
         #endif

         // Charles: temp: illegal_Op:
         #ifdef REDUCE_ILLEGAL_OP
         operation = 1;
         #endif

         //tensor_reduce_recv(start_reg, FGET, num_reg, partnerID);
         tensor_reduce_recv(start_reg, operation, num_reg, partnerID);
      }
      WAIT_TENSOR_REDUCE;
      tensor_error_check();

      #ifdef SELF_CHECK
         self_check_tensor_reduce(mid, start_reg, operation, num_reg, partnerID, action);
      #endif
   }
   C_TEST_PASS;

   return 0;
}

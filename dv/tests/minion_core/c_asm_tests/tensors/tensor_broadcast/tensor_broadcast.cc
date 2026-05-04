/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"


// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

int main()
{
   volatile cl_data_t *ptr = &_binary_blob_bin_start;

   uint64_t tid;
   C_TEST_START;

   // Get Thread ID
   tid = get_thread_id();

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   // Only thread 0 executes this
   if (tid == 0) {

      int tmp = 0;
      char* addr = (char*) ptr;
      
      // Init and set all cachelines to modified
      for (int i = 0; i < 256; i++) {
         *addr = tmp;
         tmp++;
         addr += 4;
      }
      FENCE;
      
      // load some VPU registers with FLQ instructions from the first array
      init_fp_regs((void*) &ptr[0]);
      FENCE;

      uint64_t start_reg  = 0;
      uint64_t operation  = 0;
      uint64_t num_reg    = 6;
      uint64_t tree_depth = 0;

      tensor_broadcast(start_reg, operation, num_reg, tree_depth);
      //FENCE;
      WAIT_TENSOR_REDUCE;
      tensor_error_check();
   }
   // End marker
   C_TEST_PASS;

   return 0;
}

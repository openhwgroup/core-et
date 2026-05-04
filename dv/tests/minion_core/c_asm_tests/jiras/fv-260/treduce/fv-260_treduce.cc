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

#define BLOB_SIZE 1048576
#define TR_BLOB_SIZE (BLOB_SIZE/2)
// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

uint64_t get_ptr(uint64_t mid) {
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t idx = (mid*mid) % (TR_BLOB_SIZE/sizeof(cl_data_t));
   uint64_t ptr = (uint64_t)(void*)buffer[idx].data[0];
   return ptr;
}

int main() {
   uint64_t tid, mid;
   C_TEST_START;

   tid = get_thread_id();
   mid = get_minion_id();

   uint64_t start_reg  = 0;
   uint64_t tree_depth = 0;

   if (tid == 0) {
      // Test all the combinations of Tensor Reduce Number of registers
      for (uint64_t num_reg = 0; num_reg <= 0x7f; num_reg++) {
         uint64_t ptr = get_ptr(mid);
         init_fp_regs((void*) ptr);
         // Test all the Tensor Reduce operations
         uint64_t operation  = 0;
         do {
            tensor_reduce_auto(start_reg, operation, num_reg, tree_depth);
            operation  = (operation == 0 || operation == 4) ? (operation + 2)     :
                                                             ((operation + 1) % 9);
         } while (operation != 0);
      }
   }

   WAIT_TENSOR_REDUCE;

   C_TEST_PASS;

   return 0;
}

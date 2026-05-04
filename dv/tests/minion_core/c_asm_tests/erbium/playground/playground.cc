/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#define BLOB_SIZE 1048576 // 1M
#define CL_BLOB_SIZE (BLOB_SIZE/sizeof(cl_data_t))
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)

// Preload random data (~1M)
extern uint64_t _binary_blob_bin_start[];
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

int main() {

   // Init marker
   C_TEST_START;

   uint64_t tid;
   //uint64_t mid;

   // Get minion ID and thread id
   tid = get_thread_id();
   //mid = get_minion_id();

   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();

   // Only thread 0 executes this
   if (tid == 0) {
      volatile uint64_t buffer = _binary_blob_bin_start[0];
      FENCE;

      buffer = buffer + 1;
      FENCE;
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "et_test_common.h"
#include "minion.h"

#define NUM_LINES 15
#define NELEM     ((NUM_LINES*64)/8) // NUM_LINES lines, 64 bytes per line, 8 bytes per uint64_t

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

#define BLOB_SIZE 1048576 // 1M
#define CL_BLOB_SIZE (BLOB_SIZE/sizeof(cl_data_t))
// Tensor Load accesses multiple addresses. To avoid acessing beyond the BLOB
// reduce the size of the BLOB to the half
#define TL_BLOB_SIZE (CL_BLOB_SIZE/2)

// This function loads 15 cache lines data from *ptr to the scratchpad region (starting from scp line 0)
void do_tensor_load(void* ptr) {
   // Tensor Load configuration
   et_tensor_load_conf_t  load_conf_matrix;

   load_conf_matrix.use_tmask      = false;                  // xs[   63]: Not used currently
   load_conf_matrix.use_coop       = false;                  // xs[   62]: No cooperation
   load_conf_matrix.transformation = 0x0UL;                  // xs[61:59]: In a Tensor Load must be 0
   load_conf_matrix.dst_start      = 0;                      // xs[58:53]: TO: SCP Cache line (0..47)
   load_conf_matrix.use_tenb       = 0;                      // xs[   52]: In a Tensor Load must be 0
   load_conf_matrix.addr           = (uint64_t) ptr;         // xs[47: 6]: FROM: Full Virtual Address
   load_conf_matrix.offset         = 0;                      // xs[ 5: 4]: WARL(0)
   load_conf_matrix.num_lines      = NUM_LINES;              // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)

   load_conf_matrix.stride         = 64;                     // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
   load_conf_matrix.id             = 0;                      // x31[0]: Tensor Operation ID

   et_tensor_load (&load_conf_matrix);
}

void self_check(uint64_t* pre, uint64_t* post) {
   for (int i = 0; i < NELEM; i++) {
      if (pre[i] != post[i]) {
         C_TEST_FAIL;
      }
   }
}

int main() {
   volatile uint64_t __attribute__((aligned(4096))) tmp_buff[NELEM];
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t mid, tid;

   // Init marker
   C_TEST_START;

   mid = get_minion_id();
   tid = get_thread_id();

   // Setup machine trap vector
   // Normally we do not expect any interrupt
   setup_mtrap_vector();

   // Only thread 0 executes this
   if (tid == 0) {
      uint64_t ptr = (uint64_t)(void*)buffer[(mid*mid)%TL_BLOB_SIZE].data[0];

      // Setup the L1 Cache to Scratchpad mode
      setup_cache_scp();

      // First load into the SCP data that makes sense
      // The only way to load data into SCP is using a TensorLoad
      do_tensor_load((void*) ptr);
      WAIT_TENSOR_LOAD_0;
      
      // Then store the data back from the L1 SCP region to the L2 

      //               entry_stride  start_scp_entry  NumLines    addr                          stride
      //               ------------  ---------------  ----------  ----------------------------  ------
      tensor_store_scp(          0,               0,  NUM_LINES,  (uint64_t)(void*)(tmp_buff),     64);
      WAIT_TENSOR_STORE;

      // Check if there was any error in the Tensor Operations
      tensor_error_check();
      // Check if the values we stored back matches the loaded ones
      self_check((uint64_t*) ptr, (uint64_t*) tmp_buff);
   }

   C_TEST_PASS;
}

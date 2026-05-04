/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"

// Preload random data (~1M)
extern cl_aligned_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      8             \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );

#define TIMA8A32 0b011
void tfma_kernel() {
    tensor_fma(0, 0x3, 0xf, 0xf, 0, 1, 1, 1, 0, 0, 0, TIMA8A32, 1);
    WAIT_TENSOR_FMA;
    uint64_t tenc_loc = 1;
    for (int b_num_col = 0; b_num_col <= 0x3; b_num_col++) {
        for (int a_num_rows = 0; a_num_rows <= 0xf; a_num_rows++) {
            for (int a_num_cols = 0; a_num_cols <= 0xf; a_num_cols++) {
                uint64_t hint = (b_num_col << 16) | (a_num_rows << 8) | a_num_cols;
                __asm__ __volatile__ (
                    "add x0, x0, %[hint]\n"
                    :
                    : [hint] "r" (hint)
                    :
                );
                //         use_tmask, b_num_col, a_num_rows, a_num_cols, offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a,   opcode, first_pass
                tensor_fma(        0, b_num_col, a_num_rows, a_num_cols,      0, tenc_loc,             1,             1,        0,         0,         0, TIMA8A32,          0);
            }
        }
    }
    WAIT_TENSOR_FMA;
}

int main() {

   uint64_t mid, tid;

   volatile cl_aligned_data_t *buffer = &_binary_blob_bin_start;

   //1.0 Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Init marker
   C_TEST_START;

   if (tid == 0) {
      // Enable scratchpad
      setup_cache_scp();

      FENCE;

      //          use_tmask  use_coop  dst_start  transformation  scp_sel   addr                                   offset  num_lines  stride   id
      //          ---------  --------  ---------  --------------  --------  --------------------------------------  ------  ---------  ------   --
      tensor_load(   false,    false,         0,              0,        0,   (uint64_t)(void*)buffer[mid].data[0],      0,        15,     64,    0);
      tensor_load(   false,    false,        16,              0,        0,  (uint64_t)(void*)buffer[mid].data[16],      0,        15,     64,    0);
      tensor_load(   false,    false,        32,              0,        0,  (uint64_t)(void*)buffer[mid].data[32],      0,        15,     64,    0);

      WAIT_TENSOR_LOAD_0;

      tfma_kernel();
   }
   C_TEST_PASS;
   return 0;
}

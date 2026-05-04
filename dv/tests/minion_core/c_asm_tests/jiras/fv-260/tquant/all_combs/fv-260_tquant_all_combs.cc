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

void tquant_kernel() {
    for (uint64_t t0 = 1; t0 <= 10; t0++) {

        for (uint64_t t1 = 1; t1 <= 10; t1++) {

            for (uint64_t t2 = 1; t2 <= 10; t2++) {

                uint64_t base_enc = t0 + (t1 << 4) + (t2 << 8);
                for (uint64_t cols = 0; cols <= 0x3; cols++) {
                    for (uint64_t rows = 0; rows <= 0xf; rows++) {
                        uint64_t hint = (cols << 8) + rows;
                        __asm__ __volatile__ (
                            "slti x3, %[hint], 127\n"
                            :
                            : [hint] "r" (hint)
                            : "x3"
                        );
                        uint64_t csr_enc = base_enc + (cols << 55) + (rows << 51);
                        __asm__ __volatile__ (
                            "csrw tensor_quant, %[csr_enc]\n"
                            :
                            : [csr_enc] "r" (csr_enc)
                            :
                        );
                        WAIT_TENSOR_QUANT;
                    }
                }
            }
        }
    }

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

      tquant_kernel();
   }
   C_TEST_PASS;
   return 0;
}

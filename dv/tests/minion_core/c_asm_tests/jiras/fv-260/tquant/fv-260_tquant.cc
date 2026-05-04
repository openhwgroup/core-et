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

void init_fp_regs(uint64_t addr) {
    __asm__ __volatile__(
        "flq2 f0,    0(%[addr])\n"
        "flq2 f1,   32(%[addr])\n"
        "flq2 f2,   64(%[addr])\n"
        "flq2 f3,   96(%[addr])\n"
        "flq2 f4,  128(%[addr])\n"
        "flq2 f5,  160(%[addr])\n"
        "flq2 f6,  192(%[addr])\n"
        "flq2 f7,  224(%[addr])\n"
        "flq2 f8,  256(%[addr])\n"
        "flq2 f9,  288(%[addr])\n"
        "flq2 f10, 320(%[addr])\n"
        "flq2 f11, 352(%[addr])\n"
        "flq2 f12, 384(%[addr])\n"
        "flq2 f13, 416(%[addr])\n"
        "flq2 f14, 448(%[addr])\n"
        "flq2 f15, 480(%[addr])\n"
        "flq2 f16, 512(%[addr])\n"
        "flq2 f17, 544(%[addr])\n"
        "flq2 f18, 576(%[addr])\n"
        "flq2 f19, 608(%[addr])\n"
        "flq2 f20, 640(%[addr])\n"
        "flq2 f21, 672(%[addr])\n"
        "flq2 f22, 704(%[addr])\n"
        "flq2 f23, 736(%[addr])\n"
        "flq2 f24, 768(%[addr])\n"
        "flq2 f25, 800(%[addr])\n"
        "flq2 f26, 832(%[addr])\n"
        "flq2 f27, 864(%[addr])\n"
        "flq2 f28, 896(%[addr])\n"
        "flq2 f29, 928(%[addr])\n"
        "flq2 f30, 960(%[addr])\n"
        "flq2 f31, 992(%[addr])\n"
        :
        : [addr] "r" (addr)
        :
    );
}

void tquant_kernel(uint64_t addr) {
    uint64_t base_enc = 0x5A030B0A7281A661;
    for (uint64_t cols = 0; cols <= 0x3; cols++) {
        for (uint64_t rows = 0; rows <= 0xf; rows++) {
            if ((cols == 0) and (rows == 0)) continue;
            if ((cols == 0) and (rows == 7)) continue;

            if ((cols == 1) and (rows == 0)) continue;
            if ((cols == 1) and (rows == 7)) continue;

            if ((cols == 2) and (rows == 1)) continue;
            if ((cols == 2) and (rows == 3)) continue;
            if ((cols == 2) and (rows == 7)) continue;

            if ((cols == 3) and (rows == 1)) continue;
            if ((cols == 3) and (rows == 3)) continue;
            if ((cols == 3) and (rows == 7)) continue;

            uint64_t hint = (cols << 8) + rows;
            __asm__ __volatile__ (
                "add x0, x0, %[hint]\n"
                :
                : [hint] "r" (hint)
                :
            );
            init_fp_regs(addr);
            uint64_t csr_enc = base_enc + (cols << 55) + (rows << 51);
            __asm__ __volatile__ (
                "csrw tensor_quant, %[csr_enc]\n"
                :
                : [csr_enc] "r" (csr_enc)
                :
            );
        }
    }
    WAIT_TENSOR_QUANT;

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

      //          use_tmask  use_coop  dst_start  transformation  scp_sel   addr                                    offset  num_lines  stride   id
      //          ---------  --------  ---------  --------------  --------  ---------------------------------------  ------  ---------  ------   --
      tensor_load(   false,    false,         0,              0,        0,   (uint64_t)(void*)&buffer[mid].data[0],      0,        15,     64,    0);
      tensor_load(   false,    false,        16,              0,        0,  (uint64_t)(void*)&buffer[mid].data[16],      0,        15,     64,    0);
      tensor_load(   false,    false,        32,              0,        0,  (uint64_t)(void*)&buffer[mid].data[32],      0,        15,     64,    0);

      WAIT_TENSOR_LOAD_0

      uint64_t addr = (uint64_t)(void*)&buffer[mid].data[0];
      tquant_kernel(addr);
   }
   C_TEST_PASS;
   return 0;
}

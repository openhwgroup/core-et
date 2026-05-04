/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#include "common.h"

void tq_tfma(uint64_t x31, uint64_t tq_enc, uint64_t tfma_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_quant, %[tq_enc]\n"
         "csrrw x0, tensor_fma, %[tfma_enc]\n"
         :
         : [x31] "r" (x31), [tq_enc] "r" (tq_enc), [tfma_enc] "r" (tfma_enc)
         : "x31"
   );
   WAIT_TENSOR_QUANT;
   WAIT_TENSOR_FMA;
}

void tq_tr(uint64_t x31, uint64_t tq_enc, uint64_t tr_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_quant, %[tq_enc]\n"
         "csrrw x0, tensor_reduce, %[tr_enc]\n"
         :
         : [x31] "r" (x31), [tq_enc] "r" (tq_enc), [tr_enc] "r" (tr_enc)
         : "x31"
   );
   WAIT_TENSOR_QUANT;
   WAIT_TENSOR_REDUCE;
}

void tq_ts(uint64_t x31, uint64_t tq_enc, uint64_t ts_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_quant, %[tq_enc]\n"
         "csrrw x0, tensor_store, %[ts_enc]\n"
         :
         : [x31] "r" (x31), [tq_enc] "r" (tq_enc), [ts_enc] "r" (ts_enc)
         : "x31"
   );
   WAIT_TENSOR_QUANT;
   WAIT_TENSOR_STORE;
}

uint64_t get_tq_prod_conf(uint64_t num_regs) {
   uint64_t start_reg = 0;
   uint64_t col = (num_regs & 0x1) << 1;
   uint64_t row = num_regs >> 1;
   uint64_t scp_loc = 0;
   uint64_t csr_enc = ((start_reg  & 0x1F) << 57)       |
                      ((col        & 0x3)  << 55)       |
                      ((row        & 0xF)  << 51)       |
                      ((scp_loc    & 0x3F) << 45)       |
                      ((LAST       & 0xF)  << 36)       |
                      ((LAST       & 0xF)  << 32)       |
                      ((LAST       & 0xF)  << 28)       |
                      ((LAST       & 0xF)  << 24)       |
                      ((LAST       & 0xF)  << 20)       |
                      ((LAST       & 0xF)  << 16)       |
                      ((LAST       & 0xF)  << 12)       |
                      ((LAST       & 0xF)  << 8)        |
                      ((LAST       & 0xF)  << 4)        |
                      ((INT32_TO_FP32 & 0xF)  << 0);
   return csr_enc;
}

void ProducerQuantConsumerTX(uint64_t ptr) {
   uint64_t x31 = 0x40;
   uint64_t tfma_enc = get_tfma_conf();
   uint64_t tr_enc = get_tr_conf();
   uint64_t ts_enc = get_ts_conf();

   for (uint64_t num_regs = 0; num_regs < 32; num_regs++) {
      init_fp_regs((void*) ptr);
      uint64_t tq_enc = get_tq_prod_conf(num_regs);

      /* TQUANT -> TFMA */
      tq_tfma(x31, tq_enc, tfma_enc);

      /* TQUANT -> TR */
      tq_tr(x31, tq_enc, tr_enc);

      /* TQUANT -> TS */
      tq_ts(x31, tq_enc, ts_enc);
   }
}

int main()
{
   C_TEST_START;

   uint64_t mid = get_minion_id();
   uint64_t tid = get_thread_id();


   if (tid == 0) {
      // Enable Scratchpad
      setup_cache_scp();

      uint64_t ptr = get_ptr(mid);
      fill_l1scp((uint64_t(*)[ELEMS_CACHE]) ptr);
      FENCE;

      ProducerQuantConsumerTX(ptr);
   }
   C_TEST_PASS;

   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "et_test_common.h"

#define TR_SND   0b00
#define TR_RCV   0b01
#define TR_BCAST 0b10
#define TR_AUTO  0b11

#define LAST (uint64_t) 0
#define INT32_TO_FP32 (uint64_t) 1

#define TFMA32 (uint64_t) 0

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

#define ELEMS_CACHE 64/sizeof(uint64_t)
void fill_l1scp(uint64_t ptr[][ELEMS_CACHE]) {
   uint64_t tl_enc;
   const uint64_t tl_enc_0 = 0xf;
   const uint64_t tl_enc_1 = 0x20000000000000f;
   const uint64_t tl_enc_2 = 0x40000000000000f;

   const uint64_t x31_enc = 64; // Stride(x31[47:6]) == 1 and TL ID(x31[0]) == 0
   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      :
      : [x31_enc] "r" (x31_enc)
      : "x31"
   );

   tl_enc = tl_enc_0 + ((uint64_t) ptr[0]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );
   tl_enc = tl_enc_1 + ((uint64_t) ptr[16]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );
   tl_enc = tl_enc_2 + ((uint64_t) ptr[32]);
   __asm__ __volatile__ ("csrw tensor_load, %[tl_enc]\n" : : [tl_enc] "r" (tl_enc) : );

   WAIT_TENSOR_LOAD_0;
}

uint64_t get_ptr(uint64_t mid) {
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t idx = (mid*mid) % (TR_BLOB_SIZE/sizeof(cl_data_t));
   uint64_t ptr = (uint64_t)(void*)buffer[idx].data[0];
   return ptr;
}

uint64_t get_tfma_conf(uint64_t arows, uint64_t bcols) {
   uint64_t use_tmask = 0;
   uint64_t b_num_col = bcols;
   uint64_t a_num_rows = arows;
   uint64_t a_num_cols = 0;
   uint64_t offset = 0x0;
   uint64_t tenc_loc = 1;
   uint64_t tena_unsigned = 1;
   uint64_t tenb_unsigned = 1;
   uint64_t tenb_loc = 0;
   uint64_t scp_loc_b = 0;
   uint64_t scp_loc_a = 0;
   uint64_t first_pass = 0;

   uint64_t csr_enc = (((uint64_t)use_tmask & 1) << 63)       |
                      ((b_num_col & 0x3) << 55)               |
                      ((a_num_rows & 0xF) << 51)              |
                      ((a_num_cols & 0xF) << 47)              |
                      ((offset & 0xF) << 43)                  |
                      (((uint64_t) tenc_loc & 1) << 23)       |
                      (((uint64_t) tena_unsigned & 1) << 22)  |
                      (((uint64_t) tenb_unsigned & 1) << 21)  |
                      (((uint64_t) tenb_loc & 1) << 20)       |
                      ((scp_loc_b & 0xFF) << 12)              |
                      ((scp_loc_a & 0xFF) << 4)               |
                      ((TFMA32 & 0x7) << 1)                   |
                      ((uint64_t)first_pass & 1);
   return csr_enc;
}

uint64_t get_tq_conf() {
   uint64_t transf0 = INT32_TO_FP32;
   uint64_t scp_loc = 0; /* not used */
   uint64_t row = 0xf;
   uint64_t col = 0x3;
   uint64_t start_reg = 0;
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
                      ((transf0    & 0xF)  << 0);
   return csr_enc;
}

uint64_t get_tr_conf() {
   uint64_t start_reg = 0;
   uint64_t operation = FADD;
   uint64_t num_reg = 32;
   uint64_t action = TR_AUTO;
   uint64_t csr_enc = ((start_reg & 0x1F       ) << 57) |
                      ((operation & 0xF        ) << 24) |
                      ((num_reg   & 0xFF       ) << 16) |
                      ((action    & 0x3        )      );
   return csr_enc;
}

uint32_t scr[32][8] __attribute__ ((aligned (64))) = {0};
uint64_t get_ts_conf() {

   uint64_t reg_stride = 0;
   uint64_t start_reg = 0;
   uint64_t cols = 0x3;
   uint64_t addr = (uint64_t) scr;
   uint64_t Arows = 0xf;
   uint64_t coop_store = 0;
   
   uint64_t csr_enc = ((reg_stride     & 0x3 ) << 62) |
                      ((start_reg      & 0x1F) << 57) |
                      ((cols           & 0x3 ) << 55) |
                      ((addr & 0xFFFFFFFFFFF0)      ) |
                      ((Arows          & 0xF ) << 51) |
                      ((coop_store     & 0x3 ) << 49);
   return csr_enc;
}

void tfma_tq(uint64_t x31, uint64_t tfma_enc, uint64_t tq_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_fma, %[tfma_enc]\n"
         "csrrw x0, tensor_quant, %[tq_enc]\n"
         :
         : [x31] "r" (x31), [tfma_enc] "r" (tfma_enc), [tq_enc] "r" (tq_enc)
         : "x31"
   );
   WAIT_TENSOR_FMA;
   WAIT_TENSOR_QUANT;
}

void tfma_tr(uint64_t x31, uint64_t tfma_enc, uint64_t tr_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_fma, %[tfma_enc]\n"
         "csrrw x0, tensor_reduce, %[tr_enc]\n"
         :
         : [x31] "r" (x31), [tfma_enc] "r" (tfma_enc), [tr_enc] "r" (tr_enc)
         : "x31"
   );
   WAIT_TENSOR_FMA;
   WAIT_TENSOR_REDUCE;
}

void tfma_ts(uint64_t x31, uint64_t tfma_enc, uint64_t ts_enc) {
   __asm__ __volatile__ (
         "add x31, x0, %[x31]\n"
         "csrrw x0, tensor_fma, %[tfma_enc]\n"
         "csrrw x0, tensor_store, %[ts_enc]\n"
         :
         : [x31] "r" (x31), [tfma_enc] "r" (tfma_enc), [ts_enc] "r" (ts_enc)
         : "x31"
   );
   WAIT_TENSOR_FMA;
   WAIT_TENSOR_STORE;
}

void ProducerTFMAConsumerTX(uint64_t ptr) {
   uint64_t x31 = 0x40;
   uint64_t tq_enc = get_tq_conf();
   uint64_t tr_enc = get_tr_conf();
   uint64_t ts_enc = get_ts_conf();

   for (uint64_t arows = 0; arows < 16; arows++) {
      for (uint64_t bcols = 0; bcols < 4; bcols += 2) {
         init_fp_regs((void*) ptr);
         uint64_t tfma_enc = get_tfma_conf(arows, bcols);

         /* TFMA -> TQ */
         tfma_tq(x31, tfma_enc, tq_enc);

         /* TFMA -> TR */
         tfma_tr(x31, tfma_enc, tr_enc);

         /* TFMA -> TS */
         tfma_ts(x31, tfma_enc, ts_enc);
      }
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

      ProducerTFMAConsumerTX(ptr);
   }
   C_TEST_PASS;

   return 0;
}

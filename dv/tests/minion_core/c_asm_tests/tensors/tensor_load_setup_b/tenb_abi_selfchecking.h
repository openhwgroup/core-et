/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

typedef struct tensor_data {
   uint64_t csr_enc;       // snp_info[0]
   uint64_t tensor_mask;   // snp_info[8]
   uint64_t x31_val;       // snp_info[16]
   uint64_t pre_data_ptr;  // snp_info[24]
   uint64_t post_data_ptr; // snp_info[32]
   uint64_t pre_vpurf_ptr; // snp_info[40]
   uint64_t tl_tenb_csr;   // snp_info[48]
   uint64_t minion_id;     // snp_info[56]
} tensor_data;

tensor_data get_tfma_info(uint64_t tfma_tenb_csr_enc, uint64_t tl_tenb_csr_enc, uint64_t x31_val, uint64_t * pre_vpurf, uint64_t * post_vpurf, uint64_t * l1scp) {

   tensor_data tfma_info;
   uint64_t tensor_mask_val;

   __asm__ __volatile__ ( "csrrs %[TM_VAL], tensor_mask, x0\n" : [TM_VAL] "=r" (tensor_mask_val) : );

   tfma_info.csr_enc = tfma_tenb_csr_enc;
   tfma_info.tensor_mask = tensor_mask_val;
   tfma_info.x31_val = x31_val;
   tfma_info.pre_data_ptr = (uint64_t) l1scp;
   tfma_info.post_data_ptr = (uint64_t) post_vpurf;
   tfma_info.pre_vpurf_ptr = (uint64_t) pre_vpurf;
   tfma_info.tl_tenb_csr = tl_tenb_csr_enc;

   return tfma_info;
}

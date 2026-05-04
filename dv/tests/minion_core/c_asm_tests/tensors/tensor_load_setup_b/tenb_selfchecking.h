/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "tenb_abi_selfchecking.h"
#include "diags_selfchecking_common.h"

#define TFMA32    0
#define TFMA16A32 1
#define TIMA8A32  3

#define FMADD_S(C, C0, A, B) \
            __asm__ volatile("fmadd.s %[FC], %[FA], %[FB], %[FC0], rne" \
               : [FC]  "=f" (C)     \
               : [FA]  "f"  (A),    \
                 [FB]  "f"  (B),    \
                 [FC0] "f"  (C0)   \
               : /* No clobbers */ \
            );

#define FMUL_S(C, A, B) \
            __asm__ volatile("fmul.s %[FC], %[FA], %[FB], rne" \
               : [FC]  "=f" (C)     \
               : [FA]  "f"  (A),    \
                 [FB]  "f"  (B)     \
               : /* No clobbers */ \
            );

void do_fma32(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf, uint64_t tl_tenb_csr) {
   const uint32_t elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(float);
   float (*sec_l1scp)[elems_per_scp_line] = (float (*)[elems_per_scp_line]) pre_l1scp;

   const uint64_t usemsk = (sm_get_bits(csr_enc, 63, 63) == 1);
   const uint64_t bcols  = (sm_get_bits(csr_enc, 56, 55) + 1) * 4;
   const uint64_t arows  =  sm_get_bits(csr_enc, 54, 51) + 1;
   const uint64_t acols  =  sm_get_bits(csr_enc, 50, 47) + 1;
   const uint64_t offset =  sm_get_bits(csr_enc, 46, 43); // first column inside A row
   const uint64_t l1scpb = (sm_get_bits(csr_enc, 20, 20) == 0);
   const uint64_t bstart =  sm_get_bits(csr_enc, 17, 12); // start of B data in L1 scratchpad
   const uint64_t astart =  sm_get_bits(csr_enc,  9,  4); // start of A data in L1 scratchpad
   const uint64_t ignore_prev_val = sm_get_bits(csr_enc, 0, 0);

   // TensorLoad TenB info
   const uint64_t vaddrB  = sm_get_bits(tl_tenb_csr, 47, 6) * 64;
   //const uint64_t countB  = sm_get_bits(tl_tenb_csr,  3, 0) + 1;
   const uint64_t strideB = sm_get_bits(x31, 47, 6) * 64;

   // VPU RF
   float (*vpurf)[LANES_PER_VPU_REG] = (float (*)[LANES_PER_VPU_REG]) pre_vpurf;

   uint64_t k = 0;
   if (ignore_prev_val) {
      // If first pass, the first iteration must be a FMUL.S instead of FMADD.S
      float* tmp = l1scpb ? sec_l1scp[(bstart+k)%48] : (float*) (vaddrB + k*strideB);
      for (uint64_t i = 0; i < arows; i++) {
         if (usemsk && (sm_get_bits(tensor_mask, i, i) == 0)) {
            // Even if the mask is not set, first-pass sets the register to 0
            for (uint64_t j = 0; j < bcols; j++) {
               vpurf[i*2+j/8][j%8] = 0;
            }
            continue;
         }
         const float a = sec_l1scp[(astart+i)%48][(offset+k)%16];
         for (uint64_t j = 0; j < bcols; j++ ) {
            const float b  = tmp[j];
            float c;
            FMUL_S(c, a, b)
            vpurf[i*2+j/8][j%8] = c;
         }
      }
      k++;
   }

   while (k < acols) {
      float* tmp = l1scpb ? sec_l1scp[(bstart+k)%48] : (float*) (vaddrB + k*strideB);
      for (uint64_t i = 0; i < arows; i++) {
         if (usemsk && (sm_get_bits(tensor_mask, i, i) == 0)) {
            continue;
         }
         const float a = sec_l1scp[(astart+i)%48][(offset+k)%16];
         for (uint64_t j = 0; j < bcols; j++ ) {
            const float c0 = vpurf[i*2+j/8][j%8];
            const float b  = tmp[j];
            float c;
            FMADD_S(c, c0, a, b)
            vpurf[i*2+j/8][j%8] = c;
         }
      }
      k++;
   }
}

int execute_tfma(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp, uint64_t* pre_vpurf, uint64_t tl_tenb_csr) {
   const uint64_t config = sm_get_bits(csr_enc, 3, 1);
   const uint64_t tenb = sm_get_bits(csr_enc, 20, 20);
   if (tenb == 1) {
      const uint64_t tfma_bits = sm_get_bits(csr_enc, 50, 47);
      const uint64_t tl_bits = sm_get_bits(tl_tenb_csr, 3, 0);
      if (tfma_bits != tl_bits) return SKIP;
   }
   switch (config) {
      case TFMA32:
         do_fma32(csr_enc, x31, tensor_mask, pre_l1scp, pre_vpurf, tl_tenb_csr);
         return COMPARE;

      case TFMA16A32:
         C_TEST_FAIL // Not implemented
         return SKIP;

      case TIMA8A32:
         C_TEST_FAIL // Not implemented
         return SKIP;

      default:
         return SKIP;
   }
}

int tfma_selfchecking(tensor_data* snp_info) {
   uint64_t csr_enc, x31_val, tensor_mask, tl_tenb_csr;
   uint64_t *pre_data;
   uint64_t *pre_vpurf;
   uint64_t *post_data;

   csr_enc     = snp_info->csr_enc;
   tensor_mask = snp_info->tensor_mask;
   x31_val     = snp_info->x31_val;
   pre_data    = (uint64_t *) snp_info->pre_data_ptr;  //L1SCP
   pre_vpurf   = (uint64_t *) snp_info->pre_vpurf_ptr; //VPU RF before tensor op
   post_data   = (uint64_t *) snp_info->post_data_ptr; //VPU RF after tensor op
   tl_tenb_csr = snp_info->tl_tenb_csr;

   int action = execute_tfma(csr_enc, x31_val, tensor_mask, pre_data, pre_vpurf, tl_tenb_csr);
   if (action == COMPARE) return compare_vpurf(pre_vpurf, post_data);
   else return PASS;
}

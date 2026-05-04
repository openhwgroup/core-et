
#include "tensor_load_l2.h"

int tl_l2_selfchecking(tensor_data* snp_info) {

   uint64_t csr_enc, x31_val, tensor_mask;

   csr_enc     = snp_info->csr_enc;
   tensor_mask = snp_info->tensor_mask;
   x31_val     = snp_info->x31_val;


   const uint64_t usemsk   = sm_get_bits(csr_enc, 63, 63);

   const uint64_t l2_scp_h = sm_get_bits(csr_enc, 62, 48);
   const uint64_t l2_scp_l = sm_get_bits(csr_enc,  5,  4);
   const uint64_t dst      = ((l2_scp_h * 4) + l2_scp_l);

   const uint64_t VA       = sm_get_bits(csr_enc, 47, 6) * 64;
   const uint64_t count    = sm_get_bits(csr_enc,  3, 0) + 1;
   const uint64_t stride   = sm_get_bits(x31_val, 47, 6) * 64;

   const uint32_t elems_per_scp_line = L2SCP_LINE_SIZE_BYTES / sizeof(uint64_t);
   const uint64_t * scp_addr = (uint64_t*) 0xbf800000;
   uint64_t (*l2_scp)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) scp_addr;

   for (int i = 0; i < count; i++) {
       if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
           uint64_t * golden_data = (uint64_t*) (VA + i*stride);
           uint64_t * rtl_data = l2_scp[(dst + i) % L2SCP_NUM_LINES];
           for (int elem = 0; elem < elems_per_scp_line; elem++) {
              if ((rtl_data[elem]) != (golden_data[elem])) return NOT_PASS;
           }
       }
   }
   return PASS;
}


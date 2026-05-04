
#include <assert.h>
#include <iostream>
#include <vector>
#include "et_soc.h"
#include "snp_sequence.h"
#include "snp_seq_tload.h"
#include "snp_seq_tfma.h"

#define TFMA32    0
#define TFMA16A32 1
#define TIMA8A32  3

#define NOT_TENB 0
#define IS_TENB  1

////////////////////////////////////////////////////////////////////////////
//
//  Tensor FMA (Any Variant)
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
uint32_t get_tfma_fregs_mask(CpuResource * cpu_resource, unum_t tfma_csr_val) {
   uint32_t fregs_mask = 0;
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   // Only TFMA32 and TFMA16A32 can read from the normal RV FP regs
   unum_t valid_config = config == TFMA32 || config == TFMA16A32;
   if (valid_config) {
      unum_t bcols  = (snp_get_bits(cpu_resource, tfma_csr_val, 56, 55) + 1) * 4;
      unum_t arows  = snp_get_bits(cpu_resource, tfma_csr_val, 54, 51) + 1;
      unum_t first_pass = snp_get_bits(cpu_resource, tfma_csr_val, 0, 0);

      if (!first_pass) {
         for (int i = 0; i < arows; i++) {
            for (int j = 0; j < bcols; j++) {
               unum_t fs1 = (i*2) + (j/8);
               fregs_mask |= (1 << fs1);
            }
         }
      }
   }
   return fregs_mask;
}
unum_t get_tfma_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tfma_csr_val) {
   unum_t unpr_mask = snp_get_unpr_fregs_mask(cpu_resource);
   unum_t fregs_mask = get_tfma_fregs_mask(cpu_resource, tfma_csr_val);
   return (fregs_mask & unpr_mask);
}
unum_t get_tfma_ten_waits(CpuResource * cpu_resource, unum_t tfma_csr_val) {
   unum_t ret = 0;

   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t valid_config = (config == TFMA32) || (config == TFMA16A32) || (config == TIMA8A32);
   unum_t tenc2rf = (snp_get_bits(cpu_resource, tfma_csr_val, 23, 23) == 1);
   if (valid_config) {
      ret = ret | READS_L1SCP;
      if (((config == TIMA8A32) && tenc2rf) || (config != TIMA8A32)) {
         ret = ret | WRITES_VPU_RF;
      }
   }
   return ret;
}
unum_t get_mem_size_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t x31_val) {
   // TFMA does not read from memory directly
   return 0;
}
void selfcheck_a_scp_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t mult = (config == 0) ? 1 : (config == 1) ? 2 : 4;

   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      unum_t arows  = snp_get_bits(cpu_resource, tfma_csr_val, 54, 51) + 1;
      unum_t acols  = (snp_get_bits(cpu_resource, tfma_csr_val, 50, 47) + 1) * mult;
      unum_t astart = snp_get_bits(cpu_resource, tfma_csr_val,  9,  4);

      for (int k = 0; k < acols; k += mult) {
         for (int i = 0; i < arows; i++) {
            unum_t scp_line = (astart+i)%48;
            assert(scp_head <= scp_line && scp_line <= scp_tail);
         }
      }
   }
}
void selfcheck_b_scp_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t l1scpb = (snp_get_bits(cpu_resource, tfma_csr_val, 20, 20) == 0);
   unum_t mult = (config == 0) ? 1 : (config == 1) ? 2 : 4;

   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      unum_t acols  = (snp_get_bits(cpu_resource, tfma_csr_val, 50, 47) + 1) * mult;
      unum_t bstart = snp_get_bits(cpu_resource, tfma_csr_val, 17, 12);

      for (int k = 0; k < acols; k += mult) {
         if (l1scpb) {
            unum_t scp_line = (bstart+k/mult)%48;
            assert(scp_head <= scp_line && scp_line <= scp_tail);
         }
      }
   }
}
void selfcheck_vpurf_boundaries_tfma(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t rf_head, unum_t rf_tail) {
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t mult = (config == 0) ? 1 : (config == 1) ? 2 : 4;
   unum_t tenc2rf = (snp_get_bits(cpu_resource, tfma_csr_val, 23, 23) == 1);

   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      unum_t bcols  = (snp_get_bits(cpu_resource, tfma_csr_val, 56, 55) + 1) * 4;
      unum_t arows  = snp_get_bits(cpu_resource, tfma_csr_val, 54, 51) + 1;
      unum_t acols  = (snp_get_bits(cpu_resource, tfma_csr_val, 50, 47) + 1) * mult;

      for (int k = 0; k < acols; k += mult) {
         for (int i = 0; i < arows; i++) {
            for (int j = 0; j < bcols; j++) {
               if (config == TIMA8A32) {
                  if (tenc2rf && ((k+4) == acols)) {
                     unum_t reg = i*2+j/8;
                     assert(rf_head <= reg && reg <= rf_tail);
                  }
               }
               else {
                  unum_t reg = i*2+j/8;
                  assert(rf_head <= reg && reg <= rf_tail);
               }
            }
         }
      }
   }
}
void selfcheck_set_tfma(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val, unum_t selfcheck, unum_t set) {
   // Make sure that at least one of the two options are set
   assert(selfcheck || set);

   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t l1scpb = (snp_get_bits(cpu_resource, tfma_csr_val, 20, 20) == 0);
   unum_t mult = (config == 0) ? 1 : (config == 1) ? 2 : 4;
   unum_t tenc2rf = (snp_get_bits(cpu_resource, tfma_csr_val, 23, 23) == 1);

   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      unum_t bcols  = (snp_get_bits(cpu_resource, tfma_csr_val, 56, 55) + 1) * 4;
      unum_t arows  = snp_get_bits(cpu_resource, tfma_csr_val, 54, 51) + 1;
      unum_t acols  = (snp_get_bits(cpu_resource, tfma_csr_val, 50, 47) + 1) * mult;
      unum_t bstart = snp_get_bits(cpu_resource, tfma_csr_val, 17, 12);
      unum_t astart = snp_get_bits(cpu_resource, tfma_csr_val,  9,  4);

      for (int k = 0; k < acols; k += mult) {
         if (l1scpb) {
            if (selfcheck) assert(l1_scp[(bstart+k/mult)%48] != L1SCP_MODIFIED);
            if (set) l1_scp[(bstart+k/mult)%48] = L1SCP_SHARED;
         }
         for (int i = 0; i < arows; i++) {
            if (selfcheck) assert(l1_scp[(astart+i)%48] != L1SCP_MODIFIED);
            if (set) l1_scp[(astart+i)%48] = L1SCP_SHARED;
            for (int j = 0; j < bcols; j++) {
               if (config == TIMA8A32) {
                  if (tenc2rf && ((k+4) == acols)) {
                     if (selfcheck) {
                        assert(vpu_rf[i*2+j/8] != VPU_RF_SHARED);
                        assert(vpu_rf[i*2+j/8] != VPU_RF_EXCLUSIVE);
                     }
                     if (set) vpu_rf[i*2+j/8] = VPU_RF_MODIFIED;
                  }
               }
               else {
                  if (selfcheck) {
                     assert(vpu_rf[i*2+j/8] != VPU_RF_SHARED);
                     assert(vpu_rf[i*2+j/8] != VPU_RF_EXCLUSIVE);
                  }
                  if (set) vpu_rf[i*2+j/8] = VPU_RF_MODIFIED;
               }
            }
         }
      }
   }
}
void set_tfma_l1scp_vpurf(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val) {
   selfcheck_set_tfma(cpu_resource, l1_scp, vpu_rf, tfma_csr_val, 0, 1);
}
void selfcheck_tfma(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tfma_csr_val) {
   selfcheck_set_tfma(cpu_resource, l1_scp, vpu_rf, tfma_csr_val, 1, 0);
}
unum_t constrain_tfma_a_read_lines(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t ret = tfma_csr_val;
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      unum_t astart = snp_urandrange(cpu_resource, scp_head, scp_tail+1);
      unum_t arows  = snp_urandrange(cpu_resource, 0, (scp_tail-astart)+1);
      ret = snp_set_bits(cpu_resource, ret, 54, 51, arows);
      ret = snp_set_bits(cpu_resource, ret,  9,  4, astart);
      selfcheck_a_scp_boundaries_tfma(cpu_resource, ret, scp_head, scp_tail);
   }
   return ret;
}
unum_t constrain_tfma_b_read_lines(CpuResource * cpu_resource, unum_t tfma_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t ret = tfma_csr_val;
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   unum_t l1scpb = (snp_get_bits(cpu_resource, tfma_csr_val, 20, 20) == 0);
   if (valid_config && l1scpb) {
      unum_t scp_lines = scp_tail - scp_head + 1;
      unum_t bstart = snp_urandrange(cpu_resource, scp_head, scp_tail+1);
      unum_t acols  = snp_urandrange(cpu_resource, 0, (scp_tail-bstart)+1);
      ret = snp_set_bits(cpu_resource, ret, 50, 47, acols);
      ret = snp_set_bits(cpu_resource, ret, 17, 12, bstart);
      selfcheck_b_scp_boundaries_tfma(cpu_resource, ret, scp_head, scp_tail);
   }
   return ret;
}
unum_t constrain_tfma_vpu_write_rf(CpuResource * cpu_resource, unum_t tfma_csr_value, unum_t rf_head, unum_t rf_tail) {
   unum_t arows = snp_get_bits(cpu_resource, tfma_csr_value, 54, 51) + 1;
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_value, 3, 1);
   unum_t tenc2rf = (snp_get_bits(cpu_resource, tfma_csr_value, 23, 23) == 1);

   unum_t rf_min = rf_head + (arows-1)*2;

   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {
      if ((config == TIMA8A32 && tenc2rf) || config != TIMA8A32) {
         if (rf_min > rf_tail) {
            arows = 1 + ((rf_tail - rf_head)/2);
            tfma_csr_value = snp_set_bits(cpu_resource, tfma_csr_value, 54, 51, arows - 1);
            rf_min = rf_head + (arows-1)*2;
            assert(rf_min <= rf_tail);
         }
      }

      unum_t margin = rf_tail - rf_min;
      unum_t bcols; // Only 2 bits, max value is 0b11
      switch (margin) {
         case 0:
            bcols = 0;
            break;
         case 1:
            bcols = snp_urandrange(cpu_resource, 0, 2);
            break;
         default:
            bcols = snp_urandrange(cpu_resource, 0, 4);
            break;
      }

      tfma_csr_value = snp_set_bits(cpu_resource, tfma_csr_value, 56, 55, bcols);
      selfcheck_vpurf_boundaries_tfma(cpu_resource, tfma_csr_value, rf_head, rf_tail);
   }
   return tfma_csr_value;
}
unum_t snp_seq_tfma_base(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops, unum_t tenb) {
   unum_t tfma_csr_val;
   if (tenb) tfma_csr_val = snp_get_tfma_tenb_wrapper(cpu_resource);
   else      tfma_csr_val = snp_get_tfma_wrapper(cpu_resource);
   unum_t config = snp_get_bits(cpu_resource, tfma_csr_val, 3, 1);
   unum_t l1scpb = (snp_get_bits(cpu_resource, tfma_csr_val, 20, 20) == 0);
   unum_t valid_config = config == TFMA32 || config == TFMA16A32 || config == TIMA8A32;
   if (valid_config) {

      data_region vpu_rf_region;
      // Make sure there is enough space in the VPU RF
      if (vpu_rf[0] == VPU_RF_NOT_USED || vpu_rf[0] == VPU_RF_MODIFIED) {
         vpu_rf_region = get_empty_vpu_tfma_range(cpu_resource, vpu_rf, VPU_RF_MODIFIED, ten_seq, idx, num_ops);
      }
      else {
         //If there is no space left, force TFMA to be TIMA and write into TenC
         tfma_csr_val = snp_set_bits(cpu_resource, tfma_csr_val, 3, 1, 3); // TIMA8A32
         tfma_csr_val = snp_set_bits(cpu_resource, tfma_csr_val, 23, 23, 0); // tenc2rf
      }

      data_region scp_region;
      // Matrix A
      scp_region = get_empty_l1scp_range(cpu_resource, l1_scp, L1SCP_SHARED);
      tfma_csr_val = constrain_tfma_a_read_lines(cpu_resource, tfma_csr_val, scp_region.head, scp_region.tail);
      if (l1scpb) {
         // Matrix B
         scp_region = get_empty_l1scp_range(cpu_resource, l1_scp, L1SCP_SHARED);
         tfma_csr_val = constrain_tfma_b_read_lines(cpu_resource, tfma_csr_val, scp_region.head, scp_region.tail);
      }
      tfma_csr_val = constrain_tfma_vpu_write_rf(cpu_resource, tfma_csr_val, vpu_rf_region.head, vpu_rf_region.tail);

      // Self Check there is no potential L1SCP collisions and set the lines in the L1SCP/VPU RF models
      selfcheck_set_tfma(cpu_resource, l1_scp, vpu_rf, tfma_csr_val, 1, 1);
   }

   return tfma_csr_val;
}
unum_t snp_seq_tfma_wrapper(CpuResource * cpu_resource, unum_t tl_csr_val) {
   unum_t l1_scp[L1SCP_SIZE];
   unum_t vpu_rf[VPU_RF_SIZE];
   init_l1scp(l1_scp);
   init_vpu_rf(vpu_rf);
   set_tload_l1scp(cpu_resource, l1_scp, tl_csr_val);
   unum_t stub[] = {TFMA_OP};
   return snp_seq_tfma_base(cpu_resource, l1_scp, vpu_rf, stub, 0, 1, NOT_TENB);
}

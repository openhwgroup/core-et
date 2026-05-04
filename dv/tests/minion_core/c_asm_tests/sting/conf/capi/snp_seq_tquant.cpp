
#include <assert.h>
#include <iostream>
#include <vector>
#include "et_soc.h"
#include "snp_sequence.h"
#include "snp_seq_tquant.h"

#define LAST 0
#define INT32_TO_FP32 1
#define FP32_TO_INT32 2
#define RELU 3
#define INT32_ADD_ROW 4
#define INT32_ADD_COL 5
#define FP32_MUL_ROW 6
#define FP32_MUL_COL 7
#define SATINT8 8
#define SATUINT8 9
#define PACK128B 10


////////////////////////////////////////////////////////////////////////////
//
//  Tensor Quant (Any Transformation)
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
uint32_t get_tquant_fregs_mask(CpuResource * cpu_resource, unum_t tquant_csr_val) {
   uint32_t fregs_mask = 0;
   unum_t fstart = snp_get_bits(cpu_resource, tquant_csr_val, 61, 57);
   unum_t cols   = (snp_get_bits(cpu_resource, tquant_csr_val, 56, 55) + 1) * 4;
   unum_t rows   = snp_get_bits(cpu_resource, tquant_csr_val, 54, 51) + 1;
   unum_t line   = snp_get_bits(cpu_resource, tquant_csr_val, 50, 45);

   for (int k = 0; k < 10; k++) {
      unum_t fs1;
      unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k);

      switch (trans) {
      case LAST:
         break;

      case INT32_TO_FP32:
      case FP32_TO_INT32:
      case RELU:
      case SATINT8:
      case SATUINT8:
      case INT32_ADD_ROW:
      case INT32_ADD_COL:
      case FP32_MUL_ROW:
      case FP32_MUL_COL:
      case PACK128B:
         for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
               fs1 = (fstart + i*2 + j/8) % 32;
               fregs_mask |= (1 << fs1);
            }
         }
         break;
      }
   }
   return fregs_mask;
}
unum_t get_tquant_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tquant_csr_val) {
   unum_t unpr_mask = snp_get_unpr_fregs_mask(cpu_resource);
   unum_t fregs_mask = get_tquant_fregs_mask(cpu_resource, tquant_csr_val);
   return (fregs_mask & unpr_mask);
}
unum_t get_tquant_ten_waits(CpuResource * cpu_resource, unum_t tquant_csr_val) {
   unum_t ret = 0;
   unum_t cols   = (snp_get_bits(cpu_resource, tquant_csr_val, 56, 55) + 1) * 4;
   unum_t rows   = snp_get_bits(cpu_resource, tquant_csr_val, 54, 51) + 1;
   unum_t line   = snp_get_bits(cpu_resource, tquant_csr_val, 50, 45);

   for (int k = 0; k < 10; k++) {
      unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k);
      switch (trans) {
      case LAST:
         if (k > 0) {
            // TQUANT always prefetches L1SCP
            ret = ret | READS_L1SCP;
         }
         return ret;

      case INT32_TO_FP32:
      case FP32_TO_INT32:
      case RELU:
      case SATINT8:
      case SATUINT8:
      case PACK128B:
         ret = ret | READS_VPU_RF;
         ret = ret | WRITES_VPU_RF;
         break;

      case INT32_ADD_ROW:
      case INT32_ADD_COL:
      case FP32_MUL_ROW:
      case FP32_MUL_COL:
         ret = ret | READS_L1SCP;
         ret = ret | WRITES_VPU_RF;
         break;
      }
   }
   // TQUANT always prefetches L1SCP
   ret = ret | READS_L1SCP;
   return ret;
}
unum_t tquant_is_a_nop(CpuResource * cpu_resource, unum_t tquant_csr_val) {
   unum_t ten_waits = get_tquant_ten_waits(cpu_resource, tquant_csr_val);
   unum_t reads_l1scp = (ten_waits & READS_L1SCP) != 0;
   unum_t writes_vpurf = (ten_waits & WRITES_VPU_RF) != 0;
   unum_t is_nop = (!reads_l1scp) && (!writes_vpurf);
   return is_nop;
}
unum_t get_mem_size_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t x31_val) {
   // TQUANT does not read from memory
   return 0;
}
void selfcheck_scp_boundaries_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t cols   = (snp_get_bits(cpu_resource, tquant_csr_val, 56, 55) + 1) * 4;
   unum_t rows   = snp_get_bits(cpu_resource, tquant_csr_val, 54, 51) + 1;
   unum_t line   = snp_get_bits(cpu_resource, tquant_csr_val, 50, 45);

   for (int k = 0; k < 10; k++) {
      unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k);
      switch (trans) {
      case LAST:
         return;
      case INT32_TO_FP32:
      case FP32_TO_INT32:
      case RELU:
      case SATINT8:
      case SATUINT8:
         break;

      case INT32_ADD_ROW:
      case INT32_ADD_COL:
      case FP32_MUL_ROW:
      case FP32_MUL_COL:
         for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
               assert(scp_head <= line && line <= scp_tail);
            }
         }
         line = (line + 1) % 48;
         break;

      case PACK128B:
         break;
      }
   }
}
void selfcheck_vpurf_boundaries_tquant(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t rf_head, unum_t rf_tail) {
   unum_t fstart = snp_get_bits(cpu_resource, tquant_csr_val, 61, 57);
   unum_t cols   = (snp_get_bits(cpu_resource, tquant_csr_val, 56, 55) + 1) * 4;
   unum_t rows   = snp_get_bits(cpu_resource, tquant_csr_val, 54, 51) + 1;

   for (int k = 0; k < 10; k++) {
       unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k);
       switch (trans) {
       case LAST:
           return;
       case INT32_TO_FP32:
       case FP32_TO_INT32:
       case RELU:
       case SATINT8:
       case SATUINT8:
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < cols; j++) {
                   unum_t reg = (fstart + i*2 + j/ 8) % 32;
                   assert(rf_head <= reg && reg <= rf_tail);
               }
           }
           break;
       case INT32_ADD_ROW:
       case INT32_ADD_COL:
       case FP32_MUL_ROW:
       case FP32_MUL_COL:
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < cols; j++) {
                  unum_t reg = (fstart + i*2 + j/8) % 32;
                  assert(rf_head <= reg && reg <= rf_tail);
               }
           }
           break;

        case PACK128B:
           for (int i = 0; i < rows; i++) {
              for (int j = 0; j < cols; j++) {
                 unum_t reg = (fstart + i*2) % 32;
                 assert(rf_head <= reg && reg <= rf_tail);
              }
           }
           break;
       }
   }
}
void selfcheck_set_tquant(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val, unum_t selfcheck, unum_t set) {
   // Make sure that at least one of the two options are set
   assert(selfcheck || set);
   unum_t fstart = snp_get_bits(cpu_resource, tquant_csr_val, 61, 57);
   unum_t cols   = (snp_get_bits(cpu_resource, tquant_csr_val, 56, 55) + 1) * 4;
   unum_t rows   = snp_get_bits(cpu_resource, tquant_csr_val, 54, 51) + 1;
   unum_t line   = snp_get_bits(cpu_resource, tquant_csr_val, 50, 45);
 
   for (int k = 0; k < 10; k++) {
       unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k);
       switch (trans) {
       case LAST:
           return;
       case INT32_TO_FP32:
       case FP32_TO_INT32:
       case RELU:
       case SATINT8:
       case SATUINT8:
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < cols; j++) {
                   unum_t reg = (fstart + i*2 + j/ 8) % 32;
                   if (selfcheck) {
                      assert(vpu_rf[reg] != VPU_RF_SHARED);
                      assert(vpu_rf[reg] != VPU_RF_EXCLUSIVE);
                   }
                   if (set) vpu_rf[reg] = VPU_RF_MODIFIED;
               }
           }
           break;
   
       case INT32_ADD_ROW:
       case INT32_ADD_COL:
       case FP32_MUL_ROW:
       case FP32_MUL_COL:
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < cols; j++) {
                   unum_t reg = (fstart + i*2 + j/8) % 32;
                   if (selfcheck) {
                      assert(vpu_rf[reg] != VPU_RF_SHARED);
                      assert(vpu_rf[reg] != VPU_RF_EXCLUSIVE);
                      assert(l1_scp[line] != L1SCP_MODIFIED);
                   }
                   if (set) {
                      vpu_rf[reg] = VPU_RF_MODIFIED;
                      l1_scp[line] = L1SCP_SHARED;
                   }
               }
           }
           line = (line + 1) % 48;
           break;
   
       case PACK128B:
           for (int i = 0; i < rows; i++) {
               for (int j = 0; j < cols; j++) {
                   unum_t reg = (fstart + i*2) % 32;
                   if (selfcheck) {
                      assert(vpu_rf[reg] != VPU_RF_SHARED);
                      assert(vpu_rf[reg] != VPU_RF_EXCLUSIVE);
                      assert(vpu_rf[(reg+ j/8) % 32] != VPU_RF_SHARED);
                      assert(vpu_rf[(reg+ j/8) % 32] != VPU_RF_EXCLUSIVE);
                   }
                   if (set) {
                      vpu_rf[reg] = VPU_RF_MODIFIED;
                      vpu_rf[(reg+ j/8) % 32] = VPU_RF_MODIFIED;
                   }
               }
           }
           break;
       }
   }
   // Model VPU prefetching behavior
   if (selfcheck) assert(l1_scp[line] != L1SCP_MODIFIED);
   if (set) l1_scp[line] = L1SCP_SHARED;
}
void set_tquant_l1scp_vpurf(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val) {
   selfcheck_set_tquant(cpu_resource, l1_scp, vpu_rf, tquant_csr_val, 0, 1);
}
void selfcheck_tquant(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t tquant_csr_val) {
   selfcheck_set_tquant(cpu_resource, l1_scp, vpu_rf, tquant_csr_val, 1, 0);
}
unum_t constrain_tquant_write_vpu_rf(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t rf_head, unum_t rf_tail) {
   unum_t ret = tquant_csr_val;
   unum_t trans0 = snp_get_bits(cpu_resource, tquant_csr_val, 3, 0);
   if (trans0 != LAST) {
      unum_t fstart   = snp_urandrange(cpu_resource, rf_head, rf_tail+1);
      unum_t num_regs = rf_tail - fstart + 1;
      num_regs        = et_min(num_regs, 16);
      num_regs        = snp_urandrange(cpu_resource, 0, num_regs);
      unum_t rows     = num_regs/2;

      unum_t cols = 0;
      if ((fstart + (rows*2)) < rf_tail) {
         cols = snp_urandrange(cpu_resource, 0, 4);
      }
      else {
         cols = snp_urandrange(cpu_resource, 0, 2);
      }


      ret = snp_set_bits(cpu_resource, ret, 61, 57, fstart);
      ret = snp_set_bits(cpu_resource, ret, 56, 55, cols);
      ret = snp_set_bits(cpu_resource, ret, 54, 51, rows);
   }
   selfcheck_vpurf_boundaries_tquant(cpu_resource, ret, rf_head, rf_tail);
   return ret;
}
unum_t get_tquant_trans_only_vpurf(CpuResource * cpu_resource) {
   unum_t trans = snp_urandrange(cpu_resource, 1, 11-4);
   if (trans > 3) trans += 4;
   assert(INT32_ADD_ROW != trans ||
          INT32_ADD_COL != trans ||
          FP32_MUL_ROW  != trans ||
          FP32_MUL_COL  != trans);
   return trans; 
}
unum_t constrain_tquant_read_l1scp(CpuResource * cpu_resource, unum_t tquant_csr_val, unum_t scp_head, unum_t scp_tail) {
   unum_t ret = tquant_csr_val;
   unum_t line = snp_urandrange(cpu_resource, scp_head, scp_tail+1);
   ret = snp_set_bits(cpu_resource, ret, 50, 45, line);

   // RTL does some L1SCP prefetching, even if the data is not consumed. This triggers L1SB errors
   // To avoid this, we are assuming that we always prefetch one more L1SCP line
   unum_t one_line_only = (line == scp_tail);
   if (!one_line_only) {
      scp_tail = scp_tail - 1;
   }

   for (int k = 0; k < 10; k++) {
      unum_t trans = snp_get_bits(cpu_resource, tquant_csr_val, 4*k+3, 4*k); 
      switch (trans) {
         case LAST:
            break;
         case INT32_ADD_ROW:
         case FP32_MUL_ROW:
         case INT32_ADD_COL:
         case FP32_MUL_COL:
            if ((one_line_only) || (line > scp_tail)) {
               // If the L1SCP line is above the L1SCP limit, then we change it to a
               // trans that only reads/write the VPU RF
               trans = get_tquant_trans_only_vpurf(cpu_resource);
               ret = snp_set_bits(cpu_resource, ret, 4*k+3, 4*k, trans);
            }
            // PRM-10 wraps around 48, but we are not considering L1SCP regions that can wrap around
            line = line + 1;
            break;
         default:
            break;
      }
   }
   selfcheck_scp_boundaries_tquant(cpu_resource, ret, scp_head, scp_tail);
   return ret;
}
unum_t snp_seq_tquant_base(CpuResource * cpu_resource, unum_t *l1_scp, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops) {
   unum_t tquant_csr_val = snp_get_tquant_wrapper(cpu_resource);
   unum_t trans0 = snp_get_bits(cpu_resource, tquant_csr_val, 3, 0);
   if (trans0 != LAST) {
      data_region vpu_rf_region = get_empty_vpu_rf_range(cpu_resource, vpu_rf, VPU_RF_MODIFIED, ten_seq, idx, num_ops);
      data_region scp_region = get_empty_l1scp_range(cpu_resource, l1_scp, L1SCP_SHARED);
      tquant_csr_val = constrain_tquant_write_vpu_rf(cpu_resource, tquant_csr_val, vpu_rf_region.head, vpu_rf_region.tail);
      tquant_csr_val = constrain_tquant_read_l1scp(cpu_resource, tquant_csr_val, scp_region.head, scp_region.tail);
   }
   selfcheck_set_tquant(cpu_resource, l1_scp, vpu_rf, tquant_csr_val, 1, 1);
   return tquant_csr_val;
}

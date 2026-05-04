
#include <assert.h>
#include <iostream>
#include <vector>
#include "et_soc.h"
#include "snp_sequence.h"
#include "snp_seq_tstore.h"

////////////////////////////////////////////////////////////////////////////
//
//  Tensor Store (Any Combination)
//  /!\ Not intended to be called from a Sting Snippet
//
////////////////////////////////////////////////////////////////////////////
uint32_t get_tstore_fregs_mask(CpuResource * cpu_resource, unum_t tstore_csr_val) {
   uint32_t fregs_mask = 0;
   unum_t fstep  = snp_get_bits(cpu_resource, tstore_csr_val, 63, 62) + 1;
   unum_t fstart = snp_get_bits(cpu_resource, tstore_csr_val, 61, 57);
   unum_t bytes  = (snp_get_bits(cpu_resource, tstore_csr_val, 56, 55) + 1) * 16;
   unum_t rows   = snp_get_bits(cpu_resource, tstore_csr_val, 54, 51) + 1;

   if (bytes == 16) {
      for (int i = 0; i < rows; i++) {
         unum_t fs1 = (fstart + i*fstep) % 32;
         fregs_mask |= (1 << fs1);
      }
   } else if (bytes == 32) {
      for (int i = 0; i < rows; i++) {
         unum_t fs1 = (fstart + i*fstep) % 32;
         fregs_mask |= (1 << fs1);
      }
   } else if (bytes == 64) {
      for (int i = 0; i < rows; i++) {
         unum_t fs1 = (fstart + 2*i*fstep) % 32;
         unum_t fs2 = (fstart + (2*i + 1)*fstep) % 32;
         fregs_mask |= (1 << fs1);
         fregs_mask |= (1 << fs2);
      }
   }
}
unum_t get_tstore_ow_unpr_fregs(CpuResource * cpu_resource, unum_t tstore_csr_val) {
   unum_t unpr_mask = snp_get_unpr_fregs_mask(cpu_resource);
   unum_t fregs_mask = get_tstore_fregs_mask(cpu_resource, tstore_csr_val);
   return (fregs_mask & unpr_mask);
}
unum_t get_tstore_ten_waits(CpuResource * cpu_resource, unum_t tstore_csr_val) {
   unum_t ret = 0;
   unum_t valid_config = snp_tstore_is_valid_config(cpu_resource, tstore_csr_val);
   if (valid_config) ret = ret | READS_VPU_RF | WRITES_TO_MEM;
   return ret;
}
unum_t get_mem_size_tstore(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t x31_val) {
   unum_t mem_size = 0;
   unum_t valid_config = snp_tstore_is_valid_config(cpu_resource, tstore_csr_val);
   if (valid_config) {
      unum_t bytes  = (snp_get_bits(cpu_resource, tstore_csr_val, 56, 55) + 1) * 16;
      unum_t rows   = snp_get_bits(cpu_resource, tstore_csr_val, 54, 51) + 1;
      unum_t stride = snp_get_bits(cpu_resource, x31_val, 47, 4) * 16;
      // Max 'i' value from the loop
      unum_t i = rows - 1;
      if (bytes == 16) {
         mem_size = ((i*stride) & (0xfffffffffffffff0)) + 16;
      } else if (bytes == 32) {
         mem_size = ((i*stride) & (0xffffffffffffffe0)) + 32;
      } else if (bytes == 64) {
         mem_size = (((i*stride) + 32) & (0xffffffffffffffc0)) + 32;
      }
      // To avoid sharing the same cache line between different operations
      // We align the memory size to 64 bytes
      mem_size = ((mem_size%64) != 0) ? (mem_size & 0xffffffffffffffc0) + 64 : mem_size;
      // As the memory address could not be aligned to 64 bytes, we add 64 extra bytes just in case
      mem_size = mem_size + 64;
   }
   return mem_size;
}
unum_t selfcheck_set_tstore(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t tstore_csr_val, unum_t selfcheck, unum_t set) {
   unum_t fstep  = snp_get_bits(cpu_resource, tstore_csr_val, 63, 62) + 1;
   unum_t fstart = snp_get_bits(cpu_resource, tstore_csr_val, 61, 57);
   unum_t bytes  = (snp_get_bits(cpu_resource, tstore_csr_val, 56, 55) + 1) * 16;
   unum_t rows   = snp_get_bits(cpu_resource, tstore_csr_val, 54, 51) + 1;

   if (bytes == 16) {
      for (int i = 0; i < rows; i++) {
         unum_t reg = (fstart + i*fstep) % 32;
         if (selfcheck) {
            assert(vpu_rf[reg] != VPU_RF_MODIFIED);
            assert(vpu_rf[reg] != VPU_RF_EXCLUSIVE);
         }
         if (set) vpu_rf[reg] = VPU_RF_SHARED;
      }
   } else if (bytes == 32) {
      for (int i = 0; i < rows; i++) {
         unum_t reg = (fstart + i*fstep) % 32;
         if (selfcheck) {
            assert(vpu_rf[reg] != VPU_RF_MODIFIED);
            assert(vpu_rf[reg] != VPU_RF_EXCLUSIVE);
         }
         if (set) vpu_rf[reg] = VPU_RF_SHARED;
      }
   } else if (bytes == 64) {
      for (int i = 0; i < rows; i++) {
         unum_t reg0 = (fstart + 2*i*fstep) % 32;
         unum_t reg1 = (fstart + (2*i + 1)*fstep) % 32;
         if (selfcheck) {
            assert(vpu_rf[reg0] != VPU_RF_MODIFIED);
            assert(vpu_rf[reg1] != VPU_RF_MODIFIED);
            assert(vpu_rf[reg0] != VPU_RF_EXCLUSIVE);
            assert(vpu_rf[reg1] != VPU_RF_EXCLUSIVE);
         }
         if (set) {
            vpu_rf[reg0] = VPU_RF_SHARED;
            vpu_rf[reg1] = VPU_RF_SHARED;
         }
      }
   }
}
unum_t selfcheck_boundaries_tstore(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t head, unum_t tail) {
   unum_t fstep  = snp_get_bits(cpu_resource, tstore_csr_val, 63, 62) + 1;
   unum_t fstart = snp_get_bits(cpu_resource, tstore_csr_val, 61, 57);
   unum_t bytes  = (snp_get_bits(cpu_resource, tstore_csr_val, 56, 55) + 1) * 16;
   unum_t rows   = snp_get_bits(cpu_resource, tstore_csr_val, 54, 51) + 1;

   if (bytes == 16) {
      for (int i = 0; i < rows; i++) {
         unum_t reg = (fstart + i*fstep) % 32;
         assert(reg >= head);
         assert(reg <= tail);
      }
   } else if (bytes == 32) {
      for (int i = 0; i < rows; i++) {
         unum_t reg = (fstart + i*fstep) % 32;
         assert(reg >= head);
         assert(reg <= tail);
      }
   } else if (bytes == 64) {
      for (int i = 0; i < rows; i++) {
         unum_t reg0 = (fstart + 2*i*fstep) % 32;
         unum_t reg1 = (fstart + (2*i + 1)*fstep) % 32;
         assert(reg0 >= head);
         assert(reg0 <= tail);
         assert(reg1 >= head);
         assert(reg1 <= tail);
      }
   }
}
unum_t snp_tstore_is_valid_config(CpuResource * cpu_resource, unum_t csr_value) {
   unum_t coop_mode = snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t bytes     = snp_get_bits(cpu_resource, csr_value, 56, 55);
   if (coop_mode == 0) {
      if (bytes == 0b00 || bytes == 0b01 || bytes == 0b11) return 1;
   }
   else if (coop_mode == 1) {
      if (bytes == 0b00 || bytes == 0b01) return 1;
   }
   else if (coop_mode == 3) {
      if (bytes == 0b00) return 1;
   }
   return 0;
}
unum_t constrain_tstore_vpu_read_rf(CpuResource * cpu_resource, unum_t tstore_csr_val, unum_t rf_head, unum_t rf_tail) {
   unum_t ret = tstore_csr_val;
   unum_t valid_config = snp_tstore_is_valid_config(cpu_resource, tstore_csr_val);
   if (valid_config) {
      unum_t fstep = snp_get_bits(cpu_resource, tstore_csr_val, 63, 62) + 1;
      unum_t bytes = (snp_get_bits(cpu_resource, tstore_csr_val, 56, 55) + 1) * 16;

      unum_t fstart = snp_urandrange(cpu_resource, rf_head, rf_tail+1);

      if (bytes == 16)      rf_tail = et_min(rf_tail, fstart + 15 * fstep);
      else if (bytes == 32) rf_tail = et_min(rf_tail, fstart + 15 * fstep);
      else if (bytes == 64) rf_tail = et_min(rf_tail, fstart + 15 * 2 * fstep + fstep);

      rf_tail = snp_urandrange(cpu_resource, fstart, rf_tail + 1);
      if (bytes == 64) {
         unum_t margin = rf_tail-fstart;
         if (margin == 0) {
            // If there is only one register available and the config modifies
            // two register (64 bytes), we must constrain the config
            unum_t tmp = snp_urandrange(cpu_resource, 0, 2);
            switch (tmp) {
               case 0:
                  bytes = 16;
                  ret = snp_set_bits(cpu_resource, ret, 56, 55, 0);
                  break;
               case 1:
                  bytes = 32;
                  ret = snp_set_bits(cpu_resource, ret, 56, 55, 1);
                  break;
            }
         }
         else if (margin < fstep) {
            // If the fstep is smaller than the number of registers we are working on
            // we need to constrain fstep, as in 64bytes, we read two registers, one at X
            // and the next one at X+fstep every loop iteration
            fstep = snp_urandrange(cpu_resource, 1, margin+1);
            ret = snp_set_bits(cpu_resource, ret, 63, 62, fstep-1);
         }
      }

      unum_t rows;
      if (bytes == 16)      rows = (rf_tail - fstart)/fstep;
      else if (bytes == 32) rows = (rf_tail - fstart)/fstep;
      else if (bytes == 64) rows = (rf_tail - fstart - fstep)/(fstep*2);

      ret = snp_set_bits(cpu_resource, ret, 61, 57, fstart);
      ret = snp_set_bits(cpu_resource, ret, 54, 51, rows);
      selfcheck_boundaries_tstore(cpu_resource, ret, fstart, rf_tail);
   }
   return ret;
}
unum_t snp_seq_tstore_base(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t *ten_seq, unum_t idx, unum_t num_ops) {
   unum_t tstore_csr_val = snp_get_tstore_wrapper(cpu_resource);
   unum_t valid_config = snp_tstore_is_valid_config(cpu_resource, tstore_csr_val);
   if (valid_config) {
      data_region vpu_rf_region = get_empty_vpu_rf_range(cpu_resource, vpu_rf, VPU_RF_SHARED, ten_seq, idx, num_ops);
      tstore_csr_val = constrain_tstore_vpu_read_rf(cpu_resource, tstore_csr_val, vpu_rf_region.head, vpu_rf_region.tail);

      // Self Check there is no potential L1SCP collisions and set the lines in the L1SCP/VPU RF models
      selfcheck_set_tstore(cpu_resource, vpu_rf, tstore_csr_val, 1, 1);
   }
   return tstore_csr_val;
}


#include <assert.h>
#include <iostream>
#include <vector>
#include "et_soc.h"
#include "snp_sequence.h"
#include "snp_seq_treduce.h"
#include "snp_tensor_reduce_auto.h"

#define FADD 0
#define FMAX 2
#define FMIN 3
#define IADD 4
#define IMAX 6
#define IMIN 7
#define FGET 8

#define REDUCE_AUTO_OP 3
#define REDUCE_BROADCAST_OP 2

#define IS_REDUCE_AUTO 1
#define IS_REDUCE_BROADCAST 0

#define pwrtwo(x) (1 << (x))
#define MAX_TREE_LVL 10

uint32_t get_treduce_fregs_mask(CpuResource * cpu_resource, unum_t treduce_csr_val) {
   uint32_t fregs_mask = 0;
   unum_t valid_config = snp_treduce_is_valid_op(cpu_resource, treduce_csr_val);
   if (valid_config) {
      unum_t vpurf_op_type = snp_get_vpurf_op_type(cpu_resource, treduce_csr_val);
      unum_t reads_fregs = 0;
      if (vpurf_op_type == VPU_RF_MODIFIED) {
         // Receiver reads the RV FP regs if the operation is not FGET
         unum_t op = snp_get_bits(cpu_resource, treduce_csr_val, 27, 24);
         reads_fregs = (op != FGET);
      }
      else {
         // Sender always reads the RV FP regs if the operation is valid
         reads_fregs = 1;
      }
      if (reads_fregs) {
         unum_t lcpuid = snp_cpu_id(cpu_resource);
         unum_t mhartid = snp_get_phycpu(cpu_resource, lcpuid);
         unum_t minionid = mhartid / 2; // two harts per core

         unum_t depth = snp_get_bits(cpu_resource, treduce_csr_val, 6, 3);
         unum_t takes_part = minionid % pwrtwo(depth+1);

         if ((takes_part == 0) || (takes_part == pwrtwo(depth))) {
            unum_t fstart = snp_get_bits(cpu_resource, treduce_csr_val, 61, 57);
            unum_t fnum = snp_get_bits(cpu_resource, treduce_csr_val, 22, 16);

            for (int i = 0; i < fnum; i++) {
               unum_t fs1 = (fstart+i)%32;
               fregs_mask |= (1 << fs1);
            }
         }
      }
   }
   return fregs_mask;
}
unum_t get_treduce_ow_unpr_fregs(CpuResource * cpu_resource, unum_t treduce_csr_val) {
   unum_t unpr_mask = snp_get_unpr_fregs_mask(cpu_resource);
   unum_t fregs_mask = get_treduce_fregs_mask(cpu_resource, treduce_csr_val);
   return (fregs_mask & unpr_mask);
}
unum_t get_treduce_ten_waits(CpuResource * cpu_resource, unum_t treduce_csr_val) {
   unum_t ret = 0;
   unum_t cpuid = snp_cpu_id(cpu_resource);
   unum_t valid_config = snp_treduce_is_valid_op(cpu_resource, treduce_csr_val);
   unum_t in_reduce = snp_in_reduce(cpu_resource, treduce_csr_val, cpuid);
   if (valid_config && in_reduce) {
      unum_t vpurf_op_type = snp_get_vpurf_op_type(cpu_resource, treduce_csr_val);
      if (vpurf_op_type == VPU_RF_MODIFIED) {
         ret = ret | WRITES_VPU_RF;
      }
      else if (vpurf_op_type == VPU_RF_SHARED) {
         ret = ret | READS_VPU_RF;
      }
   }
   return ret;
}
unum_t get_mem_size_treduce(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t x31_val) {
   // TREDUCE does not read from memory
   return 0;
}
unum_t snp_treduce_is_valid_op(CpuResource * cpu_resource, unum_t treduce_csr_val) {
   unum_t op = snp_get_bits(cpu_resource, treduce_csr_val, 27, 24);
   unum_t valid_op = (op == FADD) || (op == FMAX) || (op == FMIN) || (op == IADD) || \
                     (op == IMAX) || (op == IMIN) || (op == FGET);
   return valid_op;
}
unum_t snp_get_vpurf_op_type(CpuResource * cpu_resource, unum_t treduce_csr_val) {
   unum_t is_receiver;
   unum_t reduce_type = snp_get_bits(cpu_resource, treduce_csr_val, 1, 0);

   unum_t cpuid = snp_cpu_id(cpu_resource);

   unum_t tree_lvl = snp_get_bits(cpu_resource, treduce_csr_val, 6, 3);
   unum_t hart_id   = snp_get_phycpu(cpu_resource, cpuid); // HART ID
   unum_t minion_id = hart_id >> 1; // Minion ID
   if (reduce_type == REDUCE_AUTO_OP) {
      is_receiver = snp_is_tensor_reduce_receiver(cpu_resource, tree_lvl, minion_id, IS_REDUCE_AUTO);
   }
   else {
      assert(reduce_type == REDUCE_BROADCAST_OP);
      is_receiver = snp_is_tensor_reduce_receiver(cpu_resource, tree_lvl, minion_id, IS_REDUCE_BROADCAST);
   }

   if (is_receiver) return VPU_RF_MODIFIED;
   else             return VPU_RF_SHARED;
}
void selfcheck_boundaries_treduce(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t head, unum_t tail) {
   unum_t depth = snp_get_bits(cpu_resource, treduce_csr_val, 6, 3);
   
   unum_t lcpuid = snp_cpu_id(cpu_resource);
   unum_t mhartid = snp_get_phycpu(cpu_resource, lcpuid);
   unum_t minionid = mhartid / 2; // two harts per core

   unum_t takes_part = minionid % pwrtwo(depth+1);
   if ((takes_part == 0) || (takes_part == pwrtwo(depth))) {
      unum_t fstart = snp_get_bits(cpu_resource, treduce_csr_val, 61, 57);
      unum_t fnum = snp_get_bits(cpu_resource, treduce_csr_val, 22, 16);
      for (int i = 0; i < fnum; i++) {
         unum_t k = (fstart+i)%32;
         assert((head <= k) && (k <= tail)); 
      }
   }
}
void selfcheck_set_treduce(CpuResource * cpu_resource, unum_t *vpu_rf, unum_t treduce_csr_val, unum_t selfcheck, unum_t set) {
   unum_t lcpuid = snp_cpu_id(cpu_resource);
   unum_t mhartid = snp_get_phycpu(cpu_resource, lcpuid);
   unum_t minionid = mhartid / 2; // two harts per core

   unum_t depth = snp_get_bits(cpu_resource, treduce_csr_val, 6, 3);
   unum_t takes_part = minionid % pwrtwo(depth+1);

   if ((takes_part == 0) || (takes_part == pwrtwo(depth))) {
      unum_t fstart = snp_get_bits(cpu_resource, treduce_csr_val, 61, 57);
      unum_t fnum = snp_get_bits(cpu_resource, treduce_csr_val, 22, 16);

      unum_t vpurf_op_type = snp_get_vpurf_op_type(cpu_resource, treduce_csr_val);
      unum_t vpurf_avoid_op_type;
      if (vpurf_op_type == VPU_RF_MODIFIED) {
         vpurf_avoid_op_type = VPU_RF_SHARED;
      }
      else {
         assert(vpurf_op_type == VPU_RF_SHARED);
         vpurf_avoid_op_type = VPU_RF_MODIFIED;
      }

      for (int i = 0; i < fnum; i++) {
         unum_t k = (fstart+i)%32;
         if (selfcheck) {
            assert(vpu_rf[k] != vpurf_avoid_op_type);
            assert(vpu_rf[k] != VPU_RF_EXCLUSIVE);
         }
         if (set) vpu_rf[k] = vpurf_op_type;
      }
   }
}
unum_t constrain_treduce_vpu_rf(CpuResource * cpu_resource, unum_t treduce_csr_val, unum_t rf_head, unum_t rf_tail) {
   unum_t ret = treduce_csr_val;
   unum_t valid_config = snp_treduce_is_valid_op(cpu_resource, treduce_csr_val);
   if (valid_config) {
      unum_t fnum = snp_get_bits(cpu_resource, ret, 22, 16);
      unum_t max_size = rf_tail - rf_head + 1;
      assert(max_size >= fnum);
      max_size = max_size - fnum;
      unum_t fstart = snp_urandrange(cpu_resource, 0, max_size + 1) + rf_head;
      ret = snp_set_bits(cpu_resource, ret, 61, 57, fstart);
      selfcheck_boundaries_treduce(cpu_resource, ret, rf_head, rf_tail);
   }
   return ret;
}
unum_t glbl_snp_seq_treduce_base(CpuResource * cpu_resource, unum_t *vpu_rf) {
   unum_t cpuid = snp_cpu_id(cpu_resource);
   unum_t treduce_csr_val = glbl_snp_get_treduce_auto_bcast_wrapper(cpu_resource);

   // Global Calls must be done by every thread
   unum_t fnum = snp_urandrange_glbl(cpu_resource, 0, TREDUCE_MAX_REGS);

   unum_t valid_config = snp_treduce_is_valid_op(cpu_resource, treduce_csr_val);
   unum_t in_reduce = snp_in_reduce(cpu_resource, treduce_csr_val, cpuid);
   if (valid_config && in_reduce) {
      // We constrain the number of registers just in case
      treduce_csr_val = snp_set_bits(cpu_resource, treduce_csr_val, 22, 16, fnum);
      // In a tensor reduce one minion reads the VPU RF while the other writes it
      unum_t op_type = snp_get_vpurf_op_type(cpu_resource, treduce_csr_val);
      data_region vpu_rf_region = get_empty_vpu_reduce_range(cpu_resource, vpu_rf, fnum, op_type);
      treduce_csr_val = constrain_treduce_vpu_rf(cpu_resource, treduce_csr_val, vpu_rf_region.head, vpu_rf_region.tail);

      // Self Check there is no potential VPU RF collisions and set the lines in the VPU RF model
      selfcheck_set_treduce(cpu_resource, vpu_rf, treduce_csr_val, 1, 1);
   }

   return treduce_csr_val;
}

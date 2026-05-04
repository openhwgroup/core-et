
#include <vector>
#include <iostream>
#include <assert.h>
#include "snp_seq_enc.h"

unum_t get_rand_elem_from_vec(CpuResource * cpu_resource, const std::vector<unum_t> &ops) {
   unum_t rand;
   if (ops.size() == 1) rand = 0;
   else rand = snp_urandrange(cpu_resource, 0, ops.size());
   return ops[rand];
}

unum_t get_r_type_ins(CpuResource * cpu_resource, const std::vector<unum_t> &funct7, const unum_t xs2, const unum_t xs1, const std::vector<unum_t> &funct3, const unum_t xd_xs3, const unum_t opcode) {
   unum_t ret = 0;
   ret = snp_set_bits(cpu_resource, ret, 31, 25, get_rand_elem_from_vec(cpu_resource, funct7));
   ret = snp_set_bits(cpu_resource, ret, 24, 20, xs2);
   ret = snp_set_bits(cpu_resource, ret, 19, 15, xs1);
   ret = snp_set_bits(cpu_resource, ret, 14, 12, get_rand_elem_from_vec(cpu_resource,funct3));
   ret = snp_set_bits(cpu_resource, ret, 11,  7, xd_xs3);
   ret = snp_set_bits(cpu_resource, ret,  6,  0, opcode);
   return ret;
}

unum_t get_rt_type_ins(CpuResource * cpu_resource, const std::vector<unum_t> &funct12, const unum_t xs1, const std::vector<unum_t> &funct3, const unum_t xd, const unum_t opcode) {
   unum_t ret = 0;
   ret = snp_set_bits(cpu_resource, ret, 31, 20, get_rand_elem_from_vec(cpu_resource, funct12));
   ret = snp_set_bits(cpu_resource, ret, 19, 15, xs1);
   ret = snp_set_bits(cpu_resource, ret, 14, 12, get_rand_elem_from_vec(cpu_resource, funct3));
   ret = snp_set_bits(cpu_resource, ret, 11,  7, xd);
   ret = snp_set_bits(cpu_resource, ret,  6,  0, opcode);
   return ret;
}

unum_t get_rf_type_ins(CpuResource * cpu_resource, const unum_t fs3, const std::vector<unum_t> &funct2, const unum_t fs2, const unum_t fs1, const std::vector<unum_t> &funct3, const unum_t fd, const unum_t opcode) {
   unum_t ret = 0;
   ret = snp_set_bits(cpu_resource, ret, 31, 27, fs3);
   ret = snp_set_bits(cpu_resource, ret, 26, 25, get_rand_elem_from_vec(cpu_resource, funct2));
   ret = snp_set_bits(cpu_resource, ret, 24, 20, fs2);
   ret = snp_set_bits(cpu_resource, ret, 19, 15, fs1);
   ret = snp_set_bits(cpu_resource, ret, 14, 12, get_rand_elem_from_vec(cpu_resource, funct3));
   ret = snp_set_bits(cpu_resource, ret, 11,  7, fd);
   ret = snp_set_bits(cpu_resource, ret,  6,  0, opcode);
   return ret;
}

unum_t get_j_type_ins(CpuResource * cpu_resource, const unum_t imm, const unum_t fd, const unum_t opcode) {
   unum_t ret = 0;
   ret = snp_set_bits(cpu_resource, ret, 31, 12, imm);
   ret = snp_set_bits(cpu_resource, ret, 11, 7, fd);
   ret = snp_set_bits(cpu_resource, ret,  6,  0, opcode);
   return ret;
}

unum_t snp_init_et_ins_conf(CpuResource * cpu_resource, unum_t vpu_rf_ptr) {
   unum_t *vpu_rf = (unum_t*) vpu_rf_ptr;
   et_ins_conf *et_ins_regs = (et_ins_conf*) calloc(1, sizeof(et_ins_conf));
   // FP Destination
   std::vector<unum_t> target_regs {28, 29};
   et_ins_regs->fd = get_rand_elem_from_vec(cpu_resource, target_regs);
   for (int i = 0; i < target_regs.size(); i++) {
      int reg = target_regs[i];
      // fd must be a non-used register because HW do not manage dependencies with TensorOps and PS
      assert(vpu_rf[reg] == VPU_RF_NOT_USED);
      vpu_rf[reg] = VPU_RF_EXCLUSIVE;
   }
   return (unum_t) et_ins_regs;
}

unum_t free_et_ins_conf_ptr(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   free(et_ins_regs);
   return 0;
}

unum_t get_free_fsx(CpuResource * cpu_resource, unum_t *vpu_rf) {
   // Get the potential free regions
   std::vector<data_region> fsx_regions;
   set_empty_data_ranges(fsx_regions, vpu_rf, VPU_RF_SIZE, VPU_RF_NOT_USED, VPU_RF_SHARED, 1);
   data_region fsx_region = fsx_regions[snp_urandrange(cpu_resource, 0, fsx_regions.size())];
   // Select a register from the region
   unum_t fsx = snp_urandrange(cpu_resource, fsx_region.head, fsx_region.tail + 1);
   // Assert that is available and mark it in the VPU RF model
   assert((vpu_rf[fsx] == VPU_RF_NOT_USED) || (vpu_rf[fsx] == VPU_RF_SHARED));
   vpu_rf[fsx] = VPU_RF_SHARED;

   return fsx;
}

unum_t get_free_aligned_ba_reg(CpuResource * cpu_resource) {
   std::vector<unum_t> ba_regs {26, 28};
   return get_rand_elem_from_vec(cpu_resource, ba_regs);
}

unum_t snp_set_et_ins_conf(CpuResource * cpu_resource, unum_t vpu_rf_ptr, unum_t et_ins_conf_ptr) {
   unum_t *vpu_rf = (unum_t*) vpu_rf_ptr;
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;

   // FP Source
   et_ins_regs->fs1 = get_free_fsx(cpu_resource, vpu_rf);
   et_ins_regs->fs2 = get_free_fsx(cpu_resource, vpu_rf);
   et_ins_regs->fs3 = get_free_fsx(cpu_resource, vpu_rf);

   // ba_reg
   et_ins_regs->aligned_ba_reg = get_free_aligned_ba_reg(cpu_resource);

   return (unum_t) et_ins_regs;
}

unum_t get_et_ins_fs1_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   return et_ins_regs->fs1;
}

unum_t get_et_ins_fs2_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   return et_ins_regs->fs2;
}

unum_t get_et_ins_fs3_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   return et_ins_regs->fs3;
}

unum_t get_et_ins_aligned_ba_reg(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   return et_ins_regs->aligned_ba_reg;
}

unum_t et_ins_requires_fs1(CpuResource * cpu_resource, unum_t et_ins_ptr) {
   return 1;
}

unum_t et_ins_requires_fs2(CpuResource * cpu_resource, unum_t et_ins_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_ptr;
   return (et_ins_regs->fs1 != et_ins_regs->fs2);
}

unum_t et_ins_requires_fs3(CpuResource * cpu_resource, unum_t et_ins_conf_ptr) {
   et_ins_conf * et_ins_regs = (et_ins_conf*) et_ins_conf_ptr;
   return (et_ins_regs->fs1 != et_ins_regs->fs3) && (et_ins_regs->fs2 != et_ins_regs->fs3);
}





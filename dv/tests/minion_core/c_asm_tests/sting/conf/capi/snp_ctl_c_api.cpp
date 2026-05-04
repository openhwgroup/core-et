
#include "snp_ctl_c_api.h"
#include "snp_common.h"
#include "et_soc.h"
#include "board_conf.h"
#include "lib_snip_transform.h"
#include "snp_sequence.h"

unum_t snp_ctl_set_valid_transformation(CpuResource * cpu_resource, unum_t csr_value) {
   unum_t rand_value = snp_get_bits(cpu_resource, csr_value, 61, 59);
   switch(rand_value%6) {
      case 0 :
         rand_value = 0b000;
         break;
      case 1 :
         rand_value = 0b001;
         break;
      case 2 :
         rand_value = 0b010;
         break;
      case 3 :
         rand_value = 0b101;
         break;
      case 4 :
         rand_value = 0b110;
         break;
      case 5 :
         rand_value = 0b111;
         break;
   }
   return snp_set_bits(cpu_resource, csr_value, 61, 59, rand_value);
}

unum_t snp_ctl_set_invalid_transformation(CpuResource * cpu_resource, unum_t csr_value) {
   unum_t rand_value = snp_get_bits(cpu_resource, csr_value, 61, 59);
   switch(rand_value%2) {
      case 0 :
         rand_value = 0b011;
         break;
      case 1 :
         rand_value = 0b100;
         break;
   }
   return snp_set_bits(cpu_resource, csr_value, 61, 59, rand_value);
}

unum_t glbl_snp_get_coop_neigh_mask(CpuResource * cpu_resource) {

   // Compute which Neighs are active from the same shire
   unum_t cpuid     = snp_cpu_id(cpu_resource);
   unum_t shire_id  = snp_get_shire_id(cpu_resource, cpuid);
   unum_t neigh_map[4] = {0};
   unum_t n_active_neighs = 0;

   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
      if (tmp_shire == shire_id) {
          unum_t tmp_neigh = snp_get_neigh_id(cpu_resource, i);
          if (neigh_map[tmp_neigh] == 0) {
              neigh_map[tmp_neigh] = 1;
              n_active_neighs++;
          }
      }
   }

   // Compute a random mask only considering the active neighs
   unum_t mask = (( 1 << n_active_neighs) - 1);
   unum_t rand_mask = snp_urandrange_glbl(cpu_resource, 1, mask + 1);
   unum_t ret_mask = 0;

   for (unum_t i = 0; i < 4; i++) {
       if (neigh_map[i] == 1) {
           unum_t is_set = rand_mask & 0b0001;
           if (is_set == 1) ret_mask = ret_mask | (1 << i);
           rand_mask = rand_mask >> 1;
       }
   }
   return ret_mask;
}

unum_t snp_get_coop_neigh_mask_all_active(CpuResource * cpu_resource) {

   // Compute which Neighs are active from the same shire
   unum_t cpuid     = snp_cpu_id(cpu_resource);
   unum_t shire_id  = snp_get_shire_id(cpu_resource, cpuid);
   unum_t neigh_map[4] = {0};

   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
      if (tmp_shire == shire_id) {
          unum_t tmp_neigh = snp_get_neigh_id(cpu_resource, i);
          if (neigh_map[tmp_neigh] == 0) {
              neigh_map[tmp_neigh] = 1;
          }
      }
   }

   // Compute a random mask only considering the active neighs
   unum_t ret_mask = 0;
   for (unum_t i = 0; i < 4; i++) {
       if (neigh_map[i] == 1) {
           ret_mask = ret_mask | (1 << i);
       }
   }
   return ret_mask;
}

unum_t snp_invert_coop_neigh_mask(CpuResource * cpu_resource, unum_t rand_neigh_mask){
   unum_t ret = 0;
   // Compute which Neighs are active from the same shire
   unum_t cpuid     = snp_cpu_id(cpu_resource);
   unum_t shire_id  = snp_get_shire_id(cpu_resource, cpuid);
   unum_t neigh_map[4] = {0};

   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
      if (tmp_shire == shire_id) {
          unum_t tmp_neigh = snp_get_neigh_id(cpu_resource, i);
          if (neigh_map[tmp_neigh] == 0) {
              neigh_map[tmp_neigh] = 1;
          }
      }
   }

   unum_t mask = 0;
   for (int i = 3; i >= 0; i--) {
       mask = mask << 1;
       if (neigh_map[i] == 1) mask += 1;
   }

   ret = ~rand_neigh_mask;
   ret = ret & mask;
   return ret;
}

unum_t glbl_snp_get_coop_minion_mask(CpuResource * cpu_resource) {
   // Compute which Neighs are active from the same shire
   unum_t cpuid     = snp_cpu_id(cpu_resource);
   unum_t shire_id  = snp_get_shire_id(cpu_resource, cpuid);
   unum_t neigh_id  = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t minion_map[8] = {0};
   unum_t n_active_minions = 0;

   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
       unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
       unum_t tmp_neigh = snp_get_neigh_id(cpu_resource, i);
       if ((tmp_shire == shire_id) && (tmp_neigh == neigh_id)){
           unum_t tmp_minion = snp_get_minion_id(cpu_resource, i);
           if (minion_map[tmp_minion] == 0) {
               minion_map[tmp_minion] = 1;
               n_active_minions++;
           }
       }
   }

   // Compute a random mask only considering the active minions
   unum_t mask = (( 1 << n_active_minions) - 1);
   unum_t rand_mask = snp_urandrange_glbl(cpu_resource, 1, mask + 1);
   unum_t ret_mask = 0;

   for (unum_t i = 0; i < 8; i++) {
       if (minion_map[i] == 1) {
           unum_t is_set = rand_mask & 1;
           if (is_set == 1) ret_mask = ret_mask | (1 << i);
           rand_mask = rand_mask >> 1;
       }
   }
   return ret_mask;
}

unum_t snp_get_coop_minion_mask_all_active(CpuResource * cpu_resource) {
   // Compute which Neighs are active from the same shire
   unum_t cpuid     = snp_cpu_id(cpu_resource);
   unum_t shire_id  = snp_get_shire_id(cpu_resource, cpuid);
   unum_t neigh_id  = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t minion_map[8] = {0};

   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
       unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
       unum_t tmp_neigh = snp_get_neigh_id(cpu_resource, i);
       if ((tmp_shire == shire_id) && (tmp_neigh == neigh_id)){
           unum_t tmp_minion = snp_get_minion_id(cpu_resource, i);
           if (minion_map[tmp_minion] == 0) {
               minion_map[tmp_minion] = 1;
           }
       }
   }

   // Compute a random mask only considering the active minions
   unum_t ret_mask = 0;

   for (unum_t i = 0; i < 8; i++) {
       if (minion_map[i] == 1) {
           ret_mask = ret_mask | (1 << i);
       }
   }
   return ret_mask;
}

unum_t snp_tensor_mask(CpuResource * cpu_resource, unum_t num_cpus, unum_t rand_val, unum_t rand_neigh_mask, unum_t rand_minion_mask){

   unum_t rand_coop_minion = 0x11;
   unum_t rand_coop_neigh  = 0x0f;

   unum_t ret = 0;

   rand_val = rand_val & 0xfffffffffff000e0;

   unum_t rand_neigh_minion = ((rand_neigh_mask << 16) + (rand_minion_mask << 8));
   ret = rand_val | rand_neigh_minion;
   ret = snp_set_bits(cpu_resource, ret, 4, 0, 0);
   return ret;
}

unum_t snp_cpu_in_mask(CpuResource * cpu_resource, unum_t cpuId, unum_t coop_mask){
    coop_mask = coop_mask & 0x00fff00; // Mask out irrelevant bits
    cpuId = snp_get_phycpu(cpu_resource, cpuId);
    if (cpuId%2 == 1) return 0;
    // Convert from HART# to Minion#
    cpuId = cpuId >> 1;
    // TL Coop does not go beyond the same shire
    cpuId = cpuId % 32; // 32 minions per shire
    unum_t minion_id = cpuId%8;
    unum_t neigh_id  = cpuId/8;
    unum_t neigh_mask  = (1 << minion_id) <<  8;
    unum_t minion_mask = (1 <<  neigh_id) << 16;
    unum_t in_neigh = coop_mask & neigh_mask;
    unum_t in_minion = coop_mask & minion_mask;
    return (in_neigh != 0) && (in_minion != 0);
}

unum_t glbl_snp_get_tfma_tenb_value(CpuResource * cpu_resource, unum_t tenb_csr_value, unum_t is_coop) {
   unum_t rand_value = snp_randnum(cpu_resource);
   unum_t tenb_lines = snp_get_bits(cpu_resource, tenb_csr_value, 3, 0);

   if (is_coop == 0) {
       // In order to test tensor_error[6], we will force a wrong configuration
       unum_t wrong_pairing_bias = snp_urandrange_glbl(cpu_resource, 0, 27);
       if (wrong_pairing_bias == 0) {
          tenb_lines = ~tenb_lines;
       }
   }

   unum_t rand_tfma = rand_value%3;
   rand_tfma = (rand_tfma == 2) ? 3 : rand_tfma;
   rand_value = snp_set_bits(cpu_resource, rand_value, 50, 47, tenb_lines);
   rand_value = snp_set_bits(cpu_resource, rand_value, 20, 20,          1);
   rand_value = snp_set_bits(cpu_resource, rand_value,  3,  1,  rand_tfma);
   return rand_value;
}

unum_t glbl_snp_get_tload_base(CpuResource * cpu_resource, unum_t tl_csr_val, unum_t tenb, unum_t coop) {
   tl_csr_val    = snp_and(cpu_resource, tl_csr_val,  0xbfef00000000003f);
   if (tenb) {
      tl_csr_val = snp_set_bits(cpu_resource, tl_csr_val, 52, 52, 1);
   }
   else {
      tl_csr_val = snp_set_bits(cpu_resource, tl_csr_val, 52, 52, 0);
   }
   // Tensor Load Transformation Bias to reduce the number of wrong transformations
   unum_t  tl_trans_bias;
   if (coop == NON_COOP) tl_trans_bias = snp_urandrange(cpu_resource, 0, 27);
   else                  tl_trans_bias = snp_urandrange_glbl(cpu_resource, 0, 27);
   if (tl_trans_bias == 0) {
      tl_csr_val = snp_ctl_set_invalid_transformation(cpu_resource, tl_csr_val);
   }
   else {
      tl_csr_val = snp_ctl_set_valid_transformation(cpu_resource, tl_csr_val);
   }
   return tl_csr_val;
}

unum_t snp_get_tload_wrapper(CpuResource * cpu_resource) {
   unum_t  tl_csr_val_rand = snp_randnum(cpu_resource);
   // Function is called as NON_COOP, in this case, no global rand() is called, so it is safe to not add the
   // glbl_* prefix to the function
   unum_t  tl_csr_val = glbl_snp_get_tload_base(cpu_resource, tl_csr_val_rand, NON_TENB, NON_COOP);
   return tl_csr_val;
}

unum_t glbl_snp_get_coop_tload_wrapper(CpuResource * cpu_resource) {
   unum_t  tl_csr_val_rand = snp_randnum_glbl(cpu_resource);
   unum_t  tl_csr_val = glbl_snp_get_tload_base(cpu_resource, tl_csr_val_rand, NON_TENB, COOP);
   return tl_csr_val;
}

unum_t snp_get_tload_tenb_wrapper(CpuResource * cpu_resource) {
   unum_t  tl_csr_val_rand = snp_randnum(cpu_resource);
   // Function is called as NON_COOP, in this case, no global rand() is called, so it is safe to not add the
   // glbl_* prefix to the function
   unum_t  tl_csr_val = glbl_snp_get_tload_base(cpu_resource, tl_csr_val_rand, TENB, NON_COOP);
   return tl_csr_val;
}

unum_t snp_get_coop_tload_tenb_wrapper(CpuResource * cpu_resource) {
   unum_t  tl_csr_val_rand = snp_randnum_glbl(cpu_resource);
   unum_t  tl_csr_val = glbl_snp_get_tload_base(cpu_resource, tl_csr_val_rand, TENB, COOP);
   return tl_csr_val;
}

unum_t glbl_snp_get_tload_x31_wrapper(CpuResource * cpu_resource, unum_t max_cacheline_stride) {
   unum_t tl_x31_rand   = snp_randnum(cpu_resource);
   unum_t tl_x31_masked = snp_and(cpu_resource, tl_x31_rand, 0xffff00000000003f);

   unum_t tl_mem_stride = snp_urandrange_glbl(cpu_resource, 0, max_cacheline_stride);
   tl_mem_stride = snp_lshift(cpu_resource, tl_mem_stride, 6);

   unum_t tl_x31_val = snp_or(cpu_resource, tl_x31_masked, tl_mem_stride);
   return tl_x31_val;
}

unum_t snp_get_tl_pre_ten_waits(CpuResource * cpu_resource, unum_t tl_enc) {
   csr_op tl;
   tl.op_type = TLOAD_OP;
   tl.csr_val = tl_enc;
   return get_tensor_waits_from_op(cpu_resource, tl);
}

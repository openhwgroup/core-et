
#include "snp_cts.h"
#include "snp_common.h"
#include "et_soc.h"
#include "lib_snip_transform.h"
#include "board_conf.h"
#include <set>

unum_t snp_cts_set_valid_config(CpuResource * cpu_resource, unum_t csr_value, unum_t rand_init) {
   csr_value = snp_set_bits(cpu_resource, csr_value, 61, 57, rand_init);
   unum_t coop_mode  = snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t rand_value = snp_get_bits(cpu_resource, csr_value, 56, 55);
   if (coop_mode == 0) {
      rand_value = rand_value % 3;
      switch(rand_value) {
         case 0 :
            rand_value = 0b00;
            break;
         case 1 :
            rand_value = 0b01;
            break;
         case 2 :
            rand_value = 0b11;
            break;
      }
   }
   else if (coop_mode == 1) {
      rand_value = rand_value % 2;
      switch(rand_value) {
         case 0 :
            rand_value = 0b00;
            break;
         case 1 :
            rand_value = 0b01;
            break;
      }
   }
   else if (coop_mode == 3) {
      rand_value = 0;
   }
   return snp_set_bits(cpu_resource, csr_value, 56, 55, rand_value);
}

unum_t snp_cts_set_invalid_config(CpuResource * cpu_resource, unum_t csr_value, unum_t rand_init) {
   csr_value = snp_set_bits(cpu_resource, csr_value, 61, 57, rand_init);
   unum_t coop_mode  = snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t rand_value = snp_get_bits(cpu_resource, csr_value, 56, 55);
   if (coop_mode == 0) {
      rand_value = 2;
   }
   else if (coop_mode == 1) {
      rand_value = rand_value % 2;
      switch(rand_value) {
         case 0 :
            rand_value = 0b10;
            break;
         case 1 :
            rand_value = 0b11;
            break;
      }
   }
   else if (coop_mode == 3) {
      rand_value = rand_value % 3;
      switch(rand_value) {
         case 0 :
            rand_value = 0b01;
            break;
         case 1 :
            rand_value = 0b10;
            break;
         case 2 :
            rand_value = 0b11;
            break;
      }
   }
   return snp_set_bits(cpu_resource, csr_value, 56, 55, rand_value);
}

unum_t snp_align_coop_addr(CpuResource * cpu_resource, unum_t csr_value, unum_t addr) {

    unum_t lcpu_id = snp_cpu_id(cpu_resource);
    unum_t minion_id = snp_get_minion_id(cpu_resource, lcpu_id);
    unum_t coop_num = snp_get_bits(cpu_resource, csr_value, 50, 49);
    unum_t pair_mode = snp_get_bits(cpu_resource, csr_value, 56, 55);
    unum_t byte_offset = 0;

    if (coop_num == 1) {
        if (pair_mode == 0) { // Pair-128
            addr = addr & 0xFFFFFFFFFFFFFFE0;
            byte_offset = (minion_id%2) * 0x10;
        }
        else if (pair_mode == 1) { // Pair-256
            addr = addr & 0xFFFFFFFFFFFFFFC0;
            byte_offset = (minion_id%2) * 0x20;
        }
    }
    else if (coop_num == 3) {
        if (pair_mode == 0) { // Quad-128
            addr = addr & 0xFFFFFFFFFFFFFFC0;
            byte_offset = (minion_id%4) * 0x10;
        }
    }
    // For the other cases, keep the address as it is
    return addr + byte_offset;
}

unum_t snp_get_ts_stride(CpuResource * cpu_resource, unum_t csr_value, unum_t stride) {
   unum_t m_size   =  snp_get_bits(cpu_resource, csr_value, 56, 55);
   unum_t coop_way =  snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t bits;
   if (coop_way == 0) {
      if (m_size == 0) bits = 128;
      else if (m_size == 1) bits = 256;
      else if (m_size == 3) bits = 512;
   }
   else if (coop_way == 1) {
      if (m_size == 0) bits = 256;
      else if (m_size == 1) bits = 512;
   }
   else if (coop_way == 3) {
      if (m_size == 0) bits = 512;
   }

   switch(bits) {
      case 128 :
         stride = stride << 4;
         break;
      case 256 :
         stride = stride << 5;
         break;
      case 512 :
         stride = stride << 6;
         break;
   }
   return stride;
}

unum_t glbl_snp_get_min_cpu_cts(CpuResource * cpu_resource, unum_t num_cpus, unum_t csr_value) {
   num_cpus = num_cpus/2;
   unum_t coop_way =  snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t ncoop = 1;
   if (coop_way == 1) ncoop = 2;
   else if (coop_way == 3) ncoop = 4;
   unum_t coop_sets = num_cpus/ncoop;
   unum_t selected_set = snp_urandrange_glbl(cpu_resource, 0, coop_sets);
   return (selected_set * ncoop)*2;
}

unum_t snp_get_max_cpu_cts(CpuResource * cpu_resource, unum_t min_cpu, unum_t csr_value) {
   min_cpu = min_cpu/2;
   unum_t coop_way =  snp_get_bits(cpu_resource, csr_value, 50, 49);
   unum_t ncoop = 1;
   if (coop_way == 1) ncoop = 2;
   else if (coop_way == 3) ncoop = 4;
   return (min_cpu + ncoop)*2;
}

unum_t snp_get_cts_base_minion(CpuResource * cpu_resource, unum_t cts_way) {
    unum_t lcpu_id = snp_cpu_id(cpu_resource);
    unum_t mid = snp_get_minion_id(cpu_resource, lcpu_id);
    unum_t base_mid;
    if (cts_way == 0) {
        // One Minion
        base_mid = mid;
    }
    else if (cts_way == 1) {
        // Two Minions
        base_mid = mid & 0xfffffffffffffffe;
    }
    else if (cts_way == 3) {
        // Four Minions
        base_mid = mid & 0xfffffffffffffffc;
    }
    else {
        return -1;
    }
    return base_mid;
}

unum_t glbl_snp_set_rand_cts_way(CpuResource * cpu_resource, unum_t csr_value) {
    unum_t minion_map[1024] = {0};
    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        unum_t hart_id = (unum_t)BOARD_CONF.LCPUID_TO_PCPUID_MAP[i];
        if ((hart_id%2) == 0) {
            unum_t minion_id = hart_id >> 1;
            minion_map[minion_id] = 1;
        }
    }

    unum_t two_way  = 0;
    unum_t four_way = 0;
    unum_t base_mid;
    unum_t active_count;

    /** Checking if 2-way is possible **/
    base_mid = snp_get_cts_base_minion(cpu_resource, 1);
    active_count = 0;
    for (int i = 0; i < 2; i++) {
        if (minion_map[base_mid + i] == 1) active_count++;
    }
    two_way = (active_count == 2) ? 1 : 0;

    /** Checking if 4-way is possible **/
    base_mid = snp_get_cts_base_minion(cpu_resource, 3);
    active_count = 0;
    for (int i = 0; i < 4; i++) {
        if (minion_map[base_mid + i] == 1) active_count++;
    }
    four_way = (active_count == 4) ? 1 : 0;

    /** Randomly choose one way **/
    unum_t rand_way   = snp_get_cts_way(cpu_resource, csr_value);
    unum_t rand_sel   = snp_urandrange_glbl(cpu_resource, 0, 27);
    unum_t rand_sel_1 = snp_urandrange_glbl(cpu_resource, 0,  2);
    if (rand_sel == 0) {
        rand_way = 2; // 1/27 of the times, choose a bad config (3-way)
    }
    else {
        // If both ways are possible, chose a random one
        if ((two_way == 1) && (four_way == 1)) {
            rand_way = (rand_sel_1 == 0) ? 1 : 3;
        }
        else if (two_way  == 1) rand_way = 1; // 2-way
        else if (four_way == 1) rand_way = 3; // 4-way
    }
    return snp_set_bits(cpu_resource, csr_value, 50, 49, rand_way);
}

unum_t snp_get_cts_way(CpuResource * cpu_resource, unum_t csr_value) {
    return snp_get_bits(cpu_resource, csr_value, 50, 49);
}

unum_t snp_in_cts(CpuResource * cpu_resource, unum_t csr_value) {

    /** BIAS the execution to rarely execute Tensor Store in Thread 1 **/
    unum_t lcpu_id = snp_cpu_id(cpu_resource);
    unum_t phy_tid = snp_get_thread_id(cpu_resource, lcpu_id);
    unum_t th1_bias = snp_urandrange(cpu_resource, 0, 27);
    if (phy_tid == 1) {
        if (th1_bias == 0) return 1;
        else               return 0;
    }
    /******************************************************************/

    unum_t minion_map[1024] = {0};
    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        unum_t hart_id = (unum_t)BOARD_CONF.LCPUID_TO_PCPUID_MAP[i];
        if ((hart_id%2) == 0) {
            unum_t minion_id = hart_id >> 1;
            minion_map[minion_id] = 1;
        }
    }

    unum_t cts_way = snp_get_cts_way(cpu_resource, csr_value);
    unum_t base_mid = snp_get_cts_base_minion(cpu_resource, cts_way);

    if (base_mid == -1) return 0;

    unum_t active_count = 0;
    for (int i = 0; i <= cts_way; i++) {
        if (minion_map[base_mid + i] == 1) active_count++;
    }
    return (active_count == (cts_way + 1));
}

unum_t snp_get_tstore_wrapper(CpuResource * cpu_resource) {
   unum_t  ts_csr_val_rand = snp_randnum(cpu_resource);
   unum_t  ts_csr_val_base = snp_and(cpu_resource, ts_csr_val_rand, 0xfff800000000000f);

   unum_t  rand_init       = snp_urandrange(cpu_resource, 0, 32);

   // Tensor Store Transformation Bias to reduce the number of wrong transformations
   unum_t  ts_csr_valid    = snp_cts_set_valid_config(cpu_resource, ts_csr_val_base, rand_init);
   unum_t  ts_csr_invalid  = snp_cts_set_invalid_config(cpu_resource, ts_csr_val_base, rand_init);

   unum_t ts_csr_val;
   unum_t  config_bias     = snp_urandrange(cpu_resource, 0, 27);
   if (config_bias == 0) ts_csr_val = ts_csr_invalid;
   else                  ts_csr_val = ts_csr_valid;

   return ts_csr_val;
}

unum_t glbl_snp_get_coop_tstore_wrapper(CpuResource * cpu_resource) {
   unum_t  ts_csr_val_rand = snp_randnum_glbl(cpu_resource);
   unum_t  ts_csr_val_mask = snp_and(cpu_resource, ts_csr_val_rand, 0xfff800000000000f);
   unum_t  ts_csr_val_base = glbl_snp_set_rand_cts_way(cpu_resource, ts_csr_val_mask);

   unum_t  rand_init       = snp_urandrange_glbl(cpu_resource, 0, 32);

   // Tensor Store Transformation Bias to reduce the number of wrong transformations
   unum_t  ts_csr_valid    = snp_cts_set_valid_config(cpu_resource, ts_csr_val_base, rand_init);
   unum_t  ts_csr_invalid  = snp_cts_set_invalid_config(cpu_resource, ts_csr_val_base, rand_init);

   unum_t ts_csr_val;
   unum_t  config_bias     = snp_urandrange_glbl(cpu_resource, 0, 27);
   if (config_bias == 0) ts_csr_val = ts_csr_invalid;
   else                  ts_csr_val = ts_csr_valid;

   return ts_csr_val;
}

unum_t snp_get_tstore_scp_wrapper(CpuResource * cpu_resource) {
   unum_t  ts_csr_val_rand  = snp_randnum(cpu_resource);
   unum_t  ts_csr_val       = snp_and(cpu_resource, ts_csr_val_rand, 0xfffe00000000003f);
   ts_csr_val = snp_set_bits(cpu_resource, ts_csr_val, 48, 48, 1);
   return ts_csr_val;
}

#define TLOAD_TENA  0
#define TLOAD_INT8  1
#define TLOAD_INT16 2
#define TLOAD_TRA8  5
#define TLOAD_TRA16 6
#define TLOAD_TRA32 7

unum_t snp_get_tstore_from_tlcsr(CpuResource * cpu_resource, unum_t tlcsr) {

   unum_t ret;

   const std::set<unum_t> tl_valid_configs {TLOAD_TENA, TLOAD_INT8, TLOAD_INT16, TLOAD_TRA8, TLOAD_TRA16, TLOAD_TRA32};
   unum_t tl_config = snp_get_bits(cpu_resource, tlcsr, 61, 59);
   unum_t is_tenb = snp_get_bits(cpu_resource, tlcsr, 52, 52);
   if (!is_tenb && tl_valid_configs.count(tl_config)) {
      unum_t start_scpline = snp_get_bits(cpu_resource, tlcsr, 58, 53);
      unum_t num_scplines = snp_get_bits(cpu_resource, tlcsr, 3, 0);

      ret = snp_get_tstore_scp_wrapper(cpu_resource);
      ret = snp_set_bits(cpu_resource, ret, 63, 62, 0);
      ret = snp_set_bits(cpu_resource, ret, 61, 56, start_scpline);
      ret = snp_set_bits(cpu_resource, ret, 54, 51, num_scplines);
   }
   else {
      // Return a normal TensorStore with invalid config
      unum_t rand_init       = snp_urandrange(cpu_resource, 0, 32);
      unum_t ts_csr_val_rand = snp_randnum(cpu_resource);
      unum_t ts_csr_val_base = snp_and(cpu_resource, ts_csr_val_rand, 0xfff800000000000f);
      ret = snp_cts_set_invalid_config(cpu_resource, ts_csr_val_base, rand_init);
      ret = snp_set_bits(cpu_resource, ret, 63, 62, 0);
   }
   return ret;
}

unum_t snp_get_ts_scp_start_line(CpuResource * cpu_resource, unum_t tscsr) {
   return snp_get_bits(cpu_resource, tscsr, 61, 56);
}

unum_t snp_get_cts_base_lcpuid(CpuResource * cpu_resource, unum_t ts_csr_val) {
   unum_t cts_way = snp_get_cts_way(cpu_resource, ts_csr_val);
   unum_t base_minion_id = snp_get_cts_base_minion(cpu_resource, cts_way);
   if (base_minion_id != -1) {
      unum_t base_hart_id = base_minion_id << 1;
      for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
         unum_t phy_hartid = snp_get_phycpu(cpu_resource, i);
         if (phy_hartid == base_hart_id) return i;
      }
   }
   return snp_cpu_id(cpu_resource);
}

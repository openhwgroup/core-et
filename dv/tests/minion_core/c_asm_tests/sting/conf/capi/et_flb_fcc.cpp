#include "et_soc.h"
#include "snp_common.h"
#include "board_conf.h"
#include <assert.h>

#define ESR_SHIRE_REGION 0x80340000ULL
#define ESR_REGION_PROT_SHIFT  30
#define ESR_REGION_SHIRE_SHIFT 22

#define ESR_SHIRE(prot, shire, name) \
   (((unum_t)ESR_SHIRE_REGION) + \
     (((unum_t)prot) << ESR_REGION_PROT_SHIFT) + \
     (((unum_t)shire) << ESR_REGION_SHIRE_SHIFT) + \
      ((unum_t)name))

#define PRV_U 0
#define ESR_SHIRE_FLB0 0x00100

unum_t snp_get_flb_addr(CpuResource * cpu_resource, unum_t barrier_num) {
    barrier_num = ESR_SHIRE_FLB0 + (barrier_num << 3);
    unum_t addr = ESR_SHIRE(PRV_U, 0xFF, barrier_num);
    return addr;
}

unum_t snp_get_flb_match_val(CpuResource * cpu_resource) {
    unum_t cpuid = snp_cpu_id(cpu_resource);
    unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);

    unum_t num_harts_my_shire = 0;

    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
        if (shire_id == tmp_shire) {
            num_harts_my_shire++;
        }
    }
    return num_harts_my_shire/2;
}

unum_t snp_get_fcc_addr(CpuResource * cpu_resource, int shire_id, unum_t thread_id, unum_t counter_num) {
   unum_t incr;
   unum_t esr_addr;

   assert((counter_num >= 0) && (counter_num <= 1));
   assert((thread_id   >= 0) && (thread_id   <= 1));

   if (thread_id == 0) {
      if (counter_num == 0) {
         esr_addr = 0x803400C0;
      }
      else {
         esr_addr = 0x803400C8;
      }
   }
   else {
      if (counter_num == 0) {
         esr_addr = 0x803400D0;
      }
      else {
         esr_addr = 0x803400D8;
      }
   }

   if (shire_id == -1) incr = 0x3fc00000; // local shire
   else                incr = shire_id * 0x400000;

   return esr_addr + incr;
}

unum_t snp_get_fcc_addr_local_shire(CpuResource * cpu_resource, unum_t thread_id, unum_t counter_num) {
    int shire_id;
    unum_t cpuid = snp_cpu_id(cpu_resource);
    unum_t sid_bias = snp_urandrange(cpu_resource, 0, 2); // returns 0 or 1
    if (sid_bias == 0) shire_id = snp_get_shire_id(cpu_resource, cpuid);
    else               shire_id = -1;
    return snp_get_fcc_addr(cpu_resource, shire_id, thread_id, counter_num);
}

unum_t snp_get_fcc_addr_local_shire_same_tid(CpuResource * cpu_resource, unum_t counter_num) {
    unum_t cpuid = snp_cpu_id(cpu_resource);
    unum_t hart_id = snp_get_phycpu(cpu_resource, cpuid);
    unum_t thread_id = hart_id%2;
    return snp_get_fcc_addr_local_shire(cpu_resource, thread_id, counter_num);
}

unum_t snp_is_reset_cpu(CpuResource * cpu_resource, unum_t cpuid) {
    unum_t hart_id = snp_get_phycpu(cpu_resource, cpuid);
    unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);
    unum_t min_id = 2048;
    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        unum_t tmp_shire = snp_get_shire_id(cpu_resource, i);
        if (shire_id == tmp_shire) {
            unum_t tmp_hart = snp_get_phycpu(cpu_resource, i);
            min_id = (tmp_hart < min_id) ? tmp_hart : min_id;
        }
    }
    if ((min_id >> 1) == (hart_id >> 1)) return 1;
    return 0;
}

unum_t glbl_snp_get_flb_barrier_num (CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num) {
    unum_t ret;
    unum_t barrier_select  = snp_urandrange_glbl(cpu_resource, 0,16) << 1;
    unum_t hartid = snp_get_phycpu(cpu_resource, cpuid);
    unum_t threadid = hartid%2;
    ret = barrier_select + threadid;
    if (ret == eot_flb_num) {
        ret = (ret + 2) % 32;
    }
    return ret;
}

unum_t snp_get_fcc_mask(CpuResource * cpu_resource, unum_t shire_id, unum_t thread_id) {
    uint64_t ret = 0;
    unum_t cpuid = snp_cpu_id(cpu_resource);

    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
        if (i != cpuid) {
            unum_t tmp_sid = snp_get_shire_id(cpu_resource, i);
            if (tmp_sid == shire_id) {
               unum_t tmp_hid = snp_get_phycpu_shire(cpu_resource, i);
               unum_t tmp_tid = tmp_hid%2;
               if (tmp_tid == thread_id) {
                   uint64_t tmp_mid = tmp_hid >> 1;
                   uint64_t mid_mask = ((uint64_t) 1) << tmp_mid;
                   ret = ret | mid_mask;
               }
            }
        }
    }
    assert((0xffffffff00000000 & ret) == 0);
    return ret;
}

unum_t snp_get_fcc_mask_local_shire_same_tid(CpuResource * cpu_resource) {
    unum_t cpuid = snp_cpu_id(cpu_resource);

    unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);

    unum_t hart_id = snp_get_phycpu(cpu_resource, cpuid);
    unum_t thread_id = hart_id%2;

    return snp_get_fcc_mask(cpu_resource, shire_id, thread_id);
}

unum_t snp_get_flb_csr_value_cov(CpuResource * cpu_resource, unum_t barrier_num, unum_t match_val) {
    unum_t rand = snp_randnum(cpu_resource);

    // Set the Barrier Num
    rand = snp_set_bits(cpu_resource, rand, 4, 0, barrier_num);

    // Set the Match Value
    rand = snp_set_bits(cpu_resource, rand, 12, 5, match_val - 1);

    return rand;
}

unum_t snp_get_flb_csr_value(CpuResource * cpu_resource, unum_t barrier_num) {
    unum_t match_val = snp_get_flb_match_val(cpu_resource);
    return snp_get_flb_csr_value_cov(cpu_resource, barrier_num, match_val);
}

unum_t snp_get_num_flb_cov(CpuResource * cpu_resource, unum_t flb_match_val) {
    unum_t size = BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP/2;
    flb_match_val = (flb_match_val < size) ? size : flb_match_val;

    unum_t num_flb[size];
    unum_t cpuid = snp_cpu_id(cpu_resource)/2;

    unum_t count = 0;
    int i;
    for (i = 0; i < size; i++) {
        num_flb[i] = 1;
        count++;
    }

    i = 0;
    while (count < flb_match_val) {
       unum_t bias = snp_urandrange_glbl(cpu_resource, 0, size);
       if (bias == 0) {
           count++;
           num_flb[i]++;
       }
       i++;
       i = i % size;
    }

    return num_flb[cpuid];
}

unum_t snp_get_cpuy_fcc_overflow(CpuResource * cpu_resource, unum_t cpux) {
    unum_t ret;
    if ((cpux%2) == 0) {
        ret = cpux+1;
    }
    else {
        ret = cpux-1;
    }
    return ret;
}

unum_t snp_get_fcc_overflow_mask(CpuResource * cpu_resource, unum_t cpuid) {
    return snp_get_minion_mask(cpu_resource, cpuid);
}

unum_t snp_get_fcc_total_players(CpuResource * cpu_resource, unum_t per_shire_participants_mask) {
    unum_t total_players = 0;
    for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
       unum_t phy_cpu_shire = snp_get_phycpu_shire(cpu_resource, i);
       if (snp_bit_set_in_mask(cpu_resource, phy_cpu_shire, per_shire_participants_mask)) total_players++;
    }
    return total_players;
}

unum_t glbl_snp_get_fcc_master(CpuResource * cpu_resource, unum_t per_shire_participants_mask) {
   std::set<unum_t> participants;
   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t phy_cpu_shire = snp_get_phycpu_shire(cpu_resource, i);
      if (snp_bit_set_in_mask(cpu_resource, phy_cpu_shire, per_shire_participants_mask)) participants.insert(i);
   }

   unum_t master = 0;
   if (participants.size() > 0) {
      unum_t victim = snp_urandrange_glbl(cpu_resource, 0, participants.size());
      std::set<unum_t>::iterator iter = participants.begin();
      std::advance(iter, victim);
      master = *iter;
   }
   return master;
}

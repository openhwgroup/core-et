
#include "snp_ipi.h"
#include "et_soc.h"
#include "snp_common.h"
#include "board_conf.h"
#include "et_flb_fcc.h"

unum_t snp_get_ipi_trigger_mask(CpuResource * cpu_resource, unum_t cpuid) {
    return snp_get_minion_mask(cpu_resource, cpuid);
}

// As IPI Redirect is at Neigh level,
// We need to synchronise all the HARTS but at Neigh level
// To do that, we need to get an FLB barrier number that does overlap between Neighs
// "cpuid" must be the Sting Logical CPU ID
unum_t glb_dev_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num) {
   unum_t neigh_zero  = snp_urandrange_glbl(cpu_resource,  0,  8);
   unum_t neigh_one   = snp_urandrange_glbl(cpu_resource,  8, 16);
   unum_t neigh_two   = snp_urandrange_glbl(cpu_resource, 16, 24);
   unum_t neigh_three = snp_urandrange_glbl(cpu_resource, 24, 32);

   unum_t flb_num;
   unum_t neigh_id = snp_get_neigh_id(cpu_resource, cpuid);
   switch (neigh_id) {
      case 0:
         if (eot_flb_num == neigh_zero) {
             neigh_zero = neigh_zero + 1;
             neigh_zero = (neigh_zero == 8) ? 0 : neigh_zero;
         }
         flb_num =  neigh_zero;
         break;
      case 1:
         if (eot_flb_num == neigh_one) {
             neigh_one = neigh_one + 1;
             neigh_one = (neigh_one == 16) ? 8 : neigh_one;
         }
         flb_num =  neigh_one;
         break;
      case 2:
         if (eot_flb_num == neigh_two) {
             neigh_two = neigh_two + 1;
             neigh_two = (neigh_two == 24) ? 16 : neigh_two;
         }
         flb_num =  neigh_two;
         break;
      case 3:
         if (eot_flb_num == neigh_three) {
             neigh_three = neigh_three + 1;
             neigh_three = (neigh_three == 32) ? 24 : neigh_three;
         }
         flb_num =  neigh_three;
         break;
   }
   return flb_num;
}

// As IPI Redirect is at Neigh level,
// We need to synchronise all the HARTS but at Neigh level
// To do that, we need specify the match value for FLB
// However, because we support sparse builds, we need to compute the match value
// given a Sting Logical CPU ID
unum_t dev_snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t cpuid) {
   unum_t neigh_id = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);

   unum_t ret = 0;
   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_neigh_id = snp_get_neigh_id(cpu_resource, i);
      unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
      if ((neigh_id == tmp_neigh_id) && (shire_id == tmp_shire_id)) {
         ret = ret + 1;
      }
   }

   return ret;
}

unum_t snp_get_ipi_shire_id(CpuResource * cpu_resource, unum_t cpuid) {
   unum_t bias = snp_urandrange(cpu_resource, 0, 2);
   unum_t shire_id;
   if (bias == 0) {
      shire_id = snp_get_shire_id(cpu_resource, cpuid);
   }
   else {
      shire_id = 0xFF;
   }
   return shire_id;
}

unum_t snp_get_ipi_redirect_pc_addr(CpuResource * cpu_resource, unum_t cpuid) {
   // Base ESR Addr for IPI_REDIRECT_PC
   unum_t base_esr_addr = 0x80100040;

   // Compute the Neigh and Shire id given a Sting Logical CPU ID
   unum_t neigh_id = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t shire_id = snp_get_ipi_shire_id(cpu_resource, cpuid);

   // Set the Shire ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 29, 22, shire_id);
   // Set the Neigh ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 19, 16, neigh_id);

   return base_esr_addr;
}

// As IPI redirect is at Neigh level, only one guy from each neigh should set
// the redirect PC. This function tries to compute given a Sting Logical CPU ID
// if the thread should write into the IPI_REDIRECT_PC or not
unum_t snp_is_set_redirect_pc(CpuResource * cpu_resource, unum_t cpuid) {
   unum_t neigh_id = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);
   unum_t minion_id =  snp_get_phycpu(cpu_resource, cpuid);

   // Compute the active minion of the same shire and neigh as 'cpuid' that has the lowest HART ID
   unum_t min_hart = 2048; // MAX HARTS in the chip
   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_neigh_id = snp_get_neigh_id(cpu_resource, i);
      unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
      if ((neigh_id == tmp_neigh_id) && (shire_id == tmp_shire_id)) {
         unum_t tmp_minion_id = snp_get_phycpu(cpu_resource, i);
         min_hart = (min_hart > tmp_minion_id) ? tmp_minion_id : min_hart;
      }
   }

   return (min_hart == minion_id);
}

// Given a Sting Logical CPU ID and a mask, sets the bit corresponding to the Physical CPU ID
// in the mask. For example, H0 will set bit 0, H1 bit 1...
unum_t snp_set_ipi_redirect_mask(CpuResource * cpu_resource, unum_t cpuid, unum_t mask) {
   unum_t hart_id = snp_get_phycpu(cpu_resource, cpuid);
   mask = snp_set_bits(cpu_resource, mask, hart_id, hart_id, 1);
   return mask;
}

// As IPI redirect Filter is at Shire level, when we send an IPI redirect trigger, the filter should only
// take into consideration the active Minions in the same Shire.
unum_t snp_get_ipi_redirect_filter(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode) {

   // and_mode 0: IPI_filter will be all the active minions and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 1: IPI_filter will be all bits set           and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 2: IPI_filter will be all the active minions and IPI redirect trigger will write all bits set to 1

   unum_t filter = 0;

   if (and_mode == 1) {
      filter = 0xffffffffffffffff;
   }
   else {
      unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);
      for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
         unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
         if (shire_id == tmp_shire_id) {
            filter = snp_set_ipi_redirect_mask(cpu_resource, i, filter);
         }
      }
   }

   return filter;
}

unum_t dev_snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode) {

   // and_mode 0: IPI_filter will be all the active minions and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 1: IPI_filter will be all bits set           and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 2: IPI_filter will be all the active minions and IPI redirect trigger will write all bits set to 1

   unum_t neigh_id = snp_get_neigh_id(cpu_resource, cpuid);
   unum_t filter = 0;
   unum_t bias;

   if (and_mode == 1) {
      // If AND_MODE is 1, we must set only the active minions in the Neigh
      bias = 0;
   }
   else {
      // If AND_MODE is 0 or 2, the IPI_FILTER already contains only the active minions, therefore
      // we can play a little bit more and set in the trigger only the active minions or all bits
      // of the same Neigh set to 1
      bias = snp_urandrange(cpu_resource, 0, 2);
   }

   if (bias == 0) {
      unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);

      for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
         unum_t tmp_neigh_id = snp_get_neigh_id(cpu_resource, i);
         unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
         if ((cpuid != i) && (shire_id == tmp_shire_id) && (neigh_id == tmp_neigh_id)) {
            filter = snp_set_ipi_redirect_mask(cpu_resource, i, filter);
         }
      }
   }
   else {
      unum_t base_bit = neigh_id*16;
      filter = snp_set_bits(cpu_resource, 0, base_bit + 15, base_bit, 0xffff);
      unum_t my_phy_cpuid = snp_get_phycpu(cpu_resource, cpuid);
      filter = snp_set_bits(cpu_resource, filter, my_phy_cpuid, my_phy_cpuid, 0);
   }

   return filter;
}

unum_t snp_get_ipi_redirect_filter_addr(CpuResource * cpu_resource, unum_t cpuid) {
   // Base ESR Addr for IPI_REDIRECT_FILTER
   unum_t base_esr_addr = 0x80F40088;

   // Compute the Shire id given a Sting Logical CPU ID
   unum_t shire_id = snp_get_ipi_shire_id(cpu_resource, cpuid);

   // Set the Shire ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 29, 22, shire_id);

   return base_esr_addr;
}

unum_t snp_get_redirect_trigger_addr(CpuResource * cpu_resource, unum_t cpuid) {
   // Base ESR Addr for IPI_REDIRECT_TRIGGER
   unum_t base_esr_addr = 0x80340080;

   // Compute the Neigh and Shire id given a Sting Logical CPU ID
   unum_t shire_id = snp_get_ipi_shire_id(cpu_resource, cpuid);

   // Set the Shire ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 29, 22, shire_id);

   return base_esr_addr;
}

// As IPI redirect is at Neigh level, only one guy from each Shire should set
// the redirect filter. This function tries to compute given a Sting Logical CPU ID
// if the thread should write into the IPI_REDIRECT_FILTER or not
unum_t snp_is_set_redirect_filter(CpuResource * cpu_resource, unum_t cpuid) {
   unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);
   unum_t minion_id =  snp_get_phycpu(cpu_resource, cpuid);

   // Compute the active minion of the same shire and neigh as 'cpuid' that has the lowest HART ID
   unum_t min_hart = 2048; // MAX HARTS in the chip
   for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
      unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
      if (shire_id == tmp_shire_id) {
         unum_t tmp_minion_id = snp_get_phycpu(cpu_resource, i);
         min_hart = (min_hart > tmp_minion_id) ? tmp_minion_id : min_hart;
      }
   }

   return (min_hart == minion_id);
}

unum_t snp_get_trigger_addr(CpuResource * cpu_resource, unum_t cpuid) {
   // Base ESR Addr for IPI_REDIRECT_TRIGGER
   unum_t base_esr_addr = 0x80F40090;

   // Compute the Neigh and Shire id given a Sting Logical CPU ID
   unum_t shire_id = snp_get_ipi_shire_id(cpu_resource, cpuid);

   // Set the Shire ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 29, 22, shire_id);

   return base_esr_addr;
}

unum_t snp_get_trigger_clear_addr(CpuResource * cpu_resource, unum_t cpuid) {
   // Base ESR Addr for IPI_TRIGGER_CLEAR
   unum_t base_esr_addr = 0x80F40098;

   // Compute the Neigh and Shire id given a Sting Logical CPU ID
   unum_t shire_id = snp_get_ipi_shire_id(cpu_resource, cpuid);

   // Set the Shire ID
   base_esr_addr = snp_set_bits(cpu_resource, base_esr_addr, 29, 22, shire_id);

   return base_esr_addr;
}

unum_t dev_snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t cpuid) {
   return dev_snp_get_ipi_redirect_trigger_value(cpu_resource, cpuid, 1);
}




unum_t glb_rand_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t cpuid, unum_t eot_flb_num) {
   return glbl_snp_get_flb_barrier_num (cpu_resource, cpuid, eot_flb_num);
}

unum_t rand_snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t cpuid) {
   return snp_get_flb_match_val(cpu_resource);
}

unum_t rand_snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t cpuid, unum_t and_mode) {

   // and_mode 0: IPI_filter will be all the active minions and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 1: IPI_filter will be all bits set           and IPI redirect trigger will write only the active minions in the Neigh
   // and_mode 2: IPI_filter will be all the active minions and IPI redirect trigger will write all bits set to 1

   unum_t filter = 0;
   unum_t bias;

   if (and_mode == 1) {
      // If AND_MODE is 1, we must set only the active minions in the Neigh
      bias = 0;
   }
   else {
      // If AND_MODE is 0 or 2, the IPI_FILTER already contains only the active minions, therefore
      // we can play a little bit more and set in the trigger only the active minions or all bits
      bias = snp_urandrange(cpu_resource, 0, 2);
   }

   if (bias == 0) {
      /* /!\ This part of the code is very dependent with the behavior of snp_get_fcc_mask(cpu_resource, cpuid); */
      unum_t shire_id = snp_get_shire_id(cpu_resource, cpuid);
      for (unum_t i = 0; i < BOARD_CONF.NUM_LCPUID_TO_PCPUID_MAP; i++) {
         unum_t tmp_shire_id = snp_get_shire_id(cpu_resource, i);
         if ((cpuid != i) && (shire_id == tmp_shire_id)) {
            filter = snp_set_ipi_redirect_mask(cpu_resource, i, filter);
         }
      }
   }
   else {
      filter = 0xffffffffffffffff;
      unum_t my_phy_cpuid = snp_get_phycpu(cpu_resource, cpuid);
      filter = snp_set_bits(cpu_resource, filter, my_phy_cpuid, my_phy_cpuid, 0);
   }

   unum_t hart_parity = snp_get_thread_id(cpu_resource, cpuid);
   if (hart_parity == 0) {
      filter = filter & 0x5555555555555555;
   }
   else {
      filter = filter & 0xaaaaaaaaaaaaaaaa;
   }

   return filter;
}

unum_t rand_snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t cpuid) {
   return rand_snp_get_ipi_redirect_trigger_value(cpu_resource, cpuid, 1);
}

unum_t glb_snp_get_ipi_flb_num(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid, unum_t eot_flb_num) {
   if (is_sting_rand == 1) {
      return glb_rand_snp_get_ipi_flb_num(cpu_resource, cpuid, eot_flb_num);
   }
   else {
      return glb_dev_snp_get_ipi_flb_num(cpu_resource, cpuid, eot_flb_num);
   }
}

unum_t snp_get_ipi_flb_match_val(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid) {
   if (is_sting_rand == 1) {
      return rand_snp_get_ipi_flb_match_val(cpu_resource, cpuid);
   }
   else {
      return dev_snp_get_ipi_flb_match_val(cpu_resource, cpuid);
   }
}

unum_t snp_get_ipi_redirect_trigger_value(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid, unum_t and_mode) {
   if (is_sting_rand == 1) {
      return rand_snp_get_ipi_redirect_trigger_value(cpu_resource, cpuid, and_mode);
   }
   else {
      return dev_snp_get_ipi_redirect_trigger_value(cpu_resource, cpuid, and_mode);
   }
}

unum_t snp_get_ipi_trigger_value(CpuResource * cpu_resource, unum_t is_sting_rand, unum_t cpuid) {
   if (is_sting_rand == 1) {
      return rand_snp_get_ipi_trigger_value(cpu_resource, cpuid);
   }
   else {
      return dev_snp_get_ipi_trigger_value(cpu_resource, cpuid);
   }
}

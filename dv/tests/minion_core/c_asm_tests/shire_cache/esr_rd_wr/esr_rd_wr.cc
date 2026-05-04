/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test reads and writes shire cache esrs
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "shire_cache_rand.h"


//#define SIZE 4096
//#define ALIGN_DATA 4096 
//#define ALIGN_INDEX 16
//
//static uint8_t buffer[SIZE]  __attribute__ ((aligned (ALIGN_DATA)));


// Floating point register usage
// f0 zero
// f1 golden
// f2 read back
// f3 compare result (1 if equal)
// f4 write temp



int main()
{
   // Init marker
   C_TEST_START;
   const int NUM_BANKS = 4;
   const uint32_t esr_addr_revision_id = 0x4;
   const uint64_t revision_id_mask = 0x00000000ffffffff;
   const uint32_t esr_addr_sc_idx_cop_sm_data0 = 0x8;
   const uint32_t esr_addr_sc_perfmon_p0_qual = 0x1b;
   
   const uint8_t sub_region_shire_cache = 0x0;
   const uint8_t sub_region_shire_other = 0x2;

   uint64_t err_log_info_wrvals[NUM_BANKS];
   uint64_t perfmon_p0_qual_wrvals[NUM_BANKS];
   uint64_t perfmon_p0_qual_wrmask = 0x0003ffffffffffff;
   uint64_t made_up_wr_val = 0x0001000200030004;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);

   uint64_t read_val;
   uint64_t exp_val;
   
   et_printf_long("TEST: BCK 1a\n");
   delay(150);
   et_printf_long("TEST: BCK 1b \n");



   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();

      et_printf_long("TEST: inside esr_rd_wr\n");
      

      // Read esr_shire_cache_revision_id: This one is read only. shire_other region (one per shire, not one per bank)
      exp_val = 0x000900000001;
      read_val = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, sub_region_shire_other, esr_addr_revision_id, 0xf);
      
      et_printf_long("TEST: read 0x%lx from revision_id\n",read_val);
      if ((read_val & revision_id_mask) != (exp_val & revision_id_mask)) {
        pass = false;
      }
      
      
      //For each bank, write unique values to sc_idx_cop_sm_data0 (0x8) and sc_perfmon_p0_qual (0x1b)
      int bank;
      for (bank = 0; bank < NUM_BANKS; bank++) {
        err_log_info_wrvals[bank] = made_up_wr_val;
        made_up_wr_val += 0x0102030401020304;
        perfmon_p0_qual_wrvals[bank] = made_up_wr_val;
        made_up_wr_val += 0x0403020104030201;
        et_printf_long("TEST: writing registers in shire cache bank %d\n",bank);
        write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_idx_cop_sm_data0, err_log_info_wrvals[bank], bank);
        write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_perfmon_p0_qual, perfmon_p0_qual_wrvals[bank], bank);

      }
      
      //Now read the values back and check them!
      for (bank = 0; bank < NUM_BANKS; bank++) {
        et_printf_long("TEST: reading registers in shire cache bank %d\n",bank);

        read_val = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_idx_cop_sm_data0, bank);
        exp_val = err_log_info_wrvals[bank];
        et_printf_long("TEST: read 0x%lx from sc_idx_cop_sm_data0. Expected 0x%lx\n",read_val,exp_val);
        if (read_val != exp_val) {
          pass = false;
        }
        read_val = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_perfmon_p0_qual, bank);
        exp_val = perfmon_p0_qual_wrvals[bank];
        et_printf_long("TEST: read 0x%lx from perfmon_p0_qual. Expected 0x%lx\n",read_val,exp_val);
        if ((read_val & perfmon_p0_qual_wrmask) != (exp_val & perfmon_p0_qual_wrmask)) {
          pass = false;
        }
        
      }
   }

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


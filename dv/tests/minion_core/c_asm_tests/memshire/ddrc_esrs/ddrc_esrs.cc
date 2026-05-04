/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Read DDRC ESRs
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "esr.h"
#include <cstdio>
#ifdef POSTSI_UST
   #include "print_dbg.h"
#endif

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 10000
   #ifndef NUM_ITER
   #define NUM_ITER 1
   #endif
#else
   #ifndef NUM_ITER
   #define NUM_ITER 1
   #endif
#endif


// (CYE)
#ifdef SELF_CHECK

#define SIZE 4096
#define ALIGN_DATA 4096 
#define ALIGN_INDEX 16

//static uint64_t buffer[SIZE]  __attribute__ ((aligned (ALIGN_DATA)));

#endif // ifdef SELF_CHECK


// Which memshires should this test target. this can be passed with cflag tgt_memshires_mask
#ifndef TGT_MEMSHIRES_TEST_MASK
  #define TGT_MEMSHIRES_TEST_MASK 0x01
#endif


int main()
{
  // Init marker
  C_TEST_START;

   bool  pass = true;
   uint32_t hart_id = getHartId();
   uint32_t thread_id = hart_id & 1;
   uint32_t shire_id  = ((hart_id>>6) & 0x3F);
   uint32_t minion_id  = ((hart_id>>1) & 0x1F);
   uint8_t memshireEnable = TGT_MEMSHIRES_TEST_MASK;
#ifdef POSTSI_UST
   print_dbg("TEST: memshireEnable = %x shire_id = %x minion_id = %x hart_id = %x \n",memshireEnable,shire_id,minion_id,hart_id);
#else
   et_printf_long("TEST: memshireEnable = %x shire_id = %x minion_id = %x hart_id = %x \n",memshireEnable,shire_id,minion_id,hart_id);
#endif

 
   if( (thread_id == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
#ifdef POSTSI_UST
    print_dbg("TEST: shire %x inside ddrc_esrs\n",shire_id);
#else
    et_printf_long("TEST: shire %x inside ddrc_esrs\n",shire_id);
#endif

      
      // !NOTE! Minion can't read any memshire registers because the PP must be 0x2. But
      // it can read some of the DDRC registers so that's what we're doing here.
      
      int ii;
      const int NUM_MEMSHIRES = 8;
      uint64_t ddrc_scratch_wrvals[NUM_MEMSHIRES];
      uint64_t ddrc_perfmon_ctl_wrvals[NUM_MEMSHIRES];
      uint64_t ddrc_perfmon_ctl_wrmask = 0x00007fff;
      uint64_t made_up_write_values;
      uint64_t read_val;
      uint64_t exp_val;
      made_up_write_values = 0x0001000200030004 + shire_id; //just make it a bit different

      // write some values to a couple of ddrc registers in each memshire:
      for (ii = 0; ii < NUM_MEMSHIRES; ii++) {
        // check the memshire enable.
        if (((memshireEnable >> ii) & 0x1) == 1) {
          ddrc_scratch_wrvals[ii] = made_up_write_values;
          made_up_write_values += 0x0102030401020304;
          ddrc_perfmon_ctl_wrvals[ii] = made_up_write_values;
          made_up_write_values += 0x0403020104030201;
#ifdef POSTSI_UST
          print_dbg("TEST: writing registers in memshire %d ddrc. ddrc_scratch_wrval = %x ddrc_perfmon_ctl_wrval = %x\n",ii,ddrc_scratch_wrvals[ii],ddrc_perfmon_ctl_wrvals[ii]);
#else
          et_printf_long("TEST: writing registers in memshire %d ddrc. ddrc_scratch_wrval = %x ddrc_perfmon_ctl_wrval = %x\n",ii,ddrc_scratch_wrvals[ii],ddrc_perfmon_ctl_wrvals[ii]);
#endif
          //
          //offset ends up being bits ?:3 of the addr. For DDR make sure bit 6 of the offset is set (bit 9 of addr selects ddr)
          //DDRC scratch (RW)
          write_esr_new(PP_MACHINE, (232 + ii), REGION_OTHER, 0x0, 0x4e, ddrc_scratch_wrvals[ii], 0x00);
          //DDRC Perfmon Ctl Status (bits 30:0 are RW)
          write_esr_new(PP_MACHINE, (232 + ii), REGION_OTHER, 0x0, 0x50, ddrc_perfmon_ctl_wrvals[ii], 0x00);
        }
      }
      
      // read the registers back and check the values
      for (ii = 0; ii < NUM_MEMSHIRES; ii++) {
        // check the memshire enable.
        if (((memshireEnable >> ii) & 0x1) == 1) {
#ifdef POSTSI_UST
          print_dbg("TEST: reading registers in memshire %d ddrc\n",ii);
#else
          et_printf_long("TEST: reading registers in memshire %d ddrc\n",ii);
#endif
          //DDRC scratch (RW)
          read_val = read_esr_new(PP_MACHINE, (232 + ii), REGION_OTHER, 0x0, 0x4e, 0x00);
#ifdef POSTSI_UST
          print_dbg("TEST: read 0x%lx from ddrc scratch\n",read_val);
#else
          et_printf_long("TEST: read 0x%lx from ddrc scratch\n",read_val);
#endif
          exp_val = ddrc_scratch_wrvals[ii];
          if (read_val != exp_val) {
#ifdef POSTSI_UST
            print_dbg("TEST: ERROR. on memshire %x saw a mismatch between the ddrc scratch expected value 0x%x and the read value 0x%x\n",ii,exp_val,read_val);
#else
            et_printf_long("TEST: ERROR. on memshire %x saw a mismatch between the ddrc scratch expected value 0x%x and the read value 0x%x\n",ii,exp_val,read_val);
#endif
            pass = false;
          }
          
          //DDRC Perfmon Ctl Status (bits 30:0 are RW)
          read_val = read_esr_new(PP_MACHINE, (232 + ii), REGION_OTHER, 0x0, 0x50, 0x00);
          exp_val = ddrc_perfmon_ctl_wrvals[ii];
          // Just check the writeable bits
          if ((read_val & ddrc_perfmon_ctl_wrmask) != (exp_val & ddrc_perfmon_ctl_wrmask)) {
#ifdef POSTSI_UST
            print_dbg("TEST: ERROR. on memshire %x saw a mismatch between the ddrc_perfmon_ctl_wrmask expected value 0x%x and the read value 0x%x (using mask 0x%x)\n",ii,exp_val,read_val,ddrc_perfmon_ctl_wrmask);
#else
            et_printf_long("TEST: ERROR. on memshire %x saw a mismatch between the ddrc_perfmon_ctl_wrmask expected value 0x%x and the read value 0x%x (using mask 0x%x)\n",ii,exp_val,read_val,ddrc_perfmon_ctl_wrmask);
#endif
            pass = false;
          }
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


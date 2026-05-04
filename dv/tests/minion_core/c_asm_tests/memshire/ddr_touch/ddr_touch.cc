/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does reads and writes to all memshires/ddrs
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "esr.h"
#include "shire_cache_rand.h"
#ifdef POSTSI_UST
   #include "print_dbg.h"
#endif

// Which memshires should this test target. this can be passed with cflag tgt_memshires_mask
#ifndef TGT_MEMSHIRES_TEST_MASK
  #define TGT_MEMSHIRES_TEST_MASK 0x1
#endif


int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t thread_id = hart_id & 1;
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);
   uint64_t my_base_addr;

   uint64_t writeData64;
//   uint64_t read_val;
   const uint64_t testData64 = 0x0000aaaa3333bbbb;
   // If this test takes too long to run on all the real ddrs in vcs so here's a mask to run on just some:
   // (Note! Memshires 0 and 4 have the PLLs. Don't enable 1 but not 0 for example.)
   //uint8_t memshireEnable[8] = {1,1,1,1,1,1,1,1};
   uint8_t memshireEnable = TGT_MEMSHIRES_TEST_MASK;
   
      // only run this once per shire.
   if ((thread_id == 0) && (minion_id == 0)) {  
     // Main memory
     my_base_addr = (1UL << 39) + (shire_id<<20) + (hart_id<<0);
     
     
     setM0MaskFF();
     initFRegs_0();
     
     uint64_t fail;
     uint64_t addr64;
     
     uint64_t ii;
     uint64_t jj;
     for (ii = 0; ii < 8; ii++) { //loop for each memshire
        if (((memshireEnable >> ii) & 0x1) == 1) {
          for(jj = 0; jj < 2; jj++) { // loop for each ddrc in a memshire
            addr64 = (uint64_t)my_base_addr | (ii << 6) | (jj << 9);
            writeData64 = (testData64 << (ii + jj)) | shire_id;
#ifdef POSTSI_UST
            print_dbg("TEST: shire %d hart %d writing to memshire %d addr 0x%lx\n",shire_id,hart_id,ii,addr64);
#else
            et_printf_long("TEST: shire %d hart %d writing to memshire %d addr 0x%lx\n",shire_id,hart_id,ii,addr64);
#endif
            sc_rand_write_word(addr64,writeData64);
            sc_rand_evict_L3(addr64, 0x0);
            fail = sc_rand_read_and_check_word(addr64,writeData64);
            if (fail != 0) {
#ifdef POSTSI_UST
              print_dbg("TEST: Error: Failed compare for address 0x%lx in memshire %d\n", addr64,ii);
#else
              et_printf_long("TEST: Error: Failed compare for address 0x%lx in memshire %d\n", addr64,ii);
#endif
              pass = false;
            }
          }
        }
      }
   }
   if (pass == false) {
#ifdef POSTSI_UST
     print_dbg("TEST: FAILED: shire %x hart 0x%lx\n", shire_id, hart_id);
#else
     et_printf_long("TEST: FAILED: shire %x hart 0x%lx\n", shire_id, hart_id);
#endif
      C_TEST_FAIL;
   } else {
#ifdef POSTSI_UST
     print_dbg("TEST: PASSED: shire %x hart 0x%lx\n", shire_id, hart_id);
#else
     et_printf_long("TEST: PASSED: shire %x hart 0x%lx\n", shire_id, hart_id);
#endif
      C_TEST_PASS;
   }
   return 0;
}


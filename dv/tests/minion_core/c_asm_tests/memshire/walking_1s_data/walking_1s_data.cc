/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series stores and loads in a walking 1s pattern
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
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
   uint64_t my_addr;
   uint64_t base_addr;
   uint64_t  write_data;
   const uint64_t one64 = 0x1;
   uint64_t addr_mask = (uint64_t)0xfffffffff8;

   uint8_t memshireEnable = TGT_MEMSHIRES_TEST_MASK;

   //a way to speed up the test for now:
   uint64_t increment; 
#ifdef POSTSI_UST
   increment = 1;
#else
   increment = 32;
#endif


   delay(150);
   uint8_t ddrc_num = 0;  // for each memshire there's a ddrc 0 or 1 (addr[9])

   //only run this once per shire
   if( (thread_id == 0 ) && (minion_id == 0)  ) {
     setM0MaskFF();
     initFRegs();
     uint64_t compare;
#ifdef POSTSI_UST
     print_dbg("TEST: shire_id %x reporting for duty! memshireEnable = %x\n",shire_id,memshireEnable);
#else
     et_printf_long("TEST: shire_id %x reporting for duty! memshireEnable = %x\n",shire_id,memshireEnable);
#endif
      
     uint8_t memshire;

     for (memshire=0; memshire < 8; memshire++) { //loop for each memshire
       if (((memshireEnable >> memshire) & 0x1) == 1) {
#ifdef POSTSI_UST
         print_dbg("TEST: shire %d walking 1s on memshire %d\n",shire_id,memshire);
#else
         et_printf_long("TEST: shire %d walking 1s on memshire %d\n",shire_id,memshire);
#endif
         
         uint32_t ii;
         uint32_t jj;

         // loop for each ddrc in a memshire
         for (jj = 0; jj < 2; jj++) {
           ddrc_num = jj;
           base_addr = (1UL << 39) + (ddrc_num << 9) + (memshire << 6);
           
           for (ii = 0; ii < 512; ii+=increment) {
             my_addr = (base_addr + ii/8) & addr_mask;
             write_data = (one64 << (ii%64));
#ifdef POSTSI_UST
             //print_dbg("TEST: Write address = 0x%lx   data=0x%lx\n", my_addr, write_data);
#else             
             et_printf_long("TEST: Write address = 0x%lx   data=0x%lx\n", my_addr, write_data);
#endif
             sc_write_global_word(my_addr, write_data);
	     sc_rand_evict_L3(my_addr, 0x0);

#ifdef POSTSI_UST
             //print_dbg("TEST: Read and check address = 0x%lx\n", my_addr);
#else
             et_printf_long("TEST: Read and check address = 0x%lx\n", my_addr);
#endif
             sc_read_global(my_addr);
             sc_set_golden_word(my_addr, write_data);
             compare = sc_check_word(my_addr);
             if (compare != 0) {
#ifdef POSTSI_UST
               print_dbg("TEST: Error: Failed compare for address 0x%lx from shire %d to memshire %d\n", my_addr,shire_id,memshire);
#else
               et_printf_long("TEST: Error: Failed compare for address 0x%lx from shire %d to memshire %d\n", my_addr,shire_id,memshire);
#endif
               pass = false;
             }
           }
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


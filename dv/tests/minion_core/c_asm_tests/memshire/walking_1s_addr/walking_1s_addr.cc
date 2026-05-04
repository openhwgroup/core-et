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
// For this test the default is 01, because it takes a while to run.
#ifndef TGT_MEMSHIRES_TEST_MASK
  #define TGT_MEMSHIRES_TEST_MASK 0x01
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

   uint8_t memshireEnable = TGT_MEMSHIRES_TEST_MASK;

   uint8_t num_offsets = 26;
   uint8_t offsets[27] = { 
     //0, not doing byte accesses
     //1, not doing halfword accesses
     2,
     3,
     4,
     5,
     // 6,  // memshire
     // 7,  // memshire
     // 8,  // memshire
      9,  // ddrc 
     10,
     11,
     // 12, // lands on top of code
     13,
     14,
     15,
     16,
     17,
     18,
     19,
     20,
     21,
     22,
     // 23, // Lands on top of code
     24,
     25,
     26,
     27,
     28,
     29,
     30,
     //31, avoid hitting SMB space 0x80_8000_0000
     32,    // 0x81...
     33     // 0x82...
     // 34, // 0x84...   // Not accessible with 16 GB DDR
     // 35, // 0x88...   // Reserved
     // 36, // 0x90...   // Reserved
     // 37, // 0xA0...   // Reserved
     // 38  // 0xC0...   // Also traps
   };

   

   delay(150);

   //only run this once per shire
   if( (thread_id == 0 ) && (minion_id == 0)  ) {
     setM0MaskFF();
     initFRegs();
     uint64_t compare;
#ifdef POSTSI_UST
      print_dbg("TEST: shire_id %lx reporting for duty! memshireEnable = %lx\n",shire_id,memshireEnable);
#else
      et_printf_long("TEST: shire_id %x reporting for duty! memshireEnable = %x\n",shire_id,memshireEnable);
#endif
      
     uint8_t memshire;
     for (memshire=0; memshire < 8; memshire++) { //loop for each memshire
       if (((memshireEnable >> memshire) & 0x1) == 1) {
#ifdef POSTSI_UST
         print_dbg("TEST: shire %lx walking 1s on memshire %lx\n",shire_id,memshire);
#else
         et_printf_long("TEST: shire %lx walking 1s on memshire %lx\n",shire_id,memshire);
#endif
         uint8_t ii;
         // For each address write a word
         for (ii = 0; ii < num_offsets; ii++) {
           my_addr = (1UL << 39) + (1UL << offsets[ii]) + (memshire << 6);
#ifdef POSTSI_UST
           print_dbg("TEST: Write address = 0x%lx\n", my_addr);
#else
           et_printf_long("TEST: Write address = 0x%lx\n", my_addr);
#endif
           sc_write_global_word(my_addr, ii+1);
           sc_rand_evict_L3(my_addr, 0x0);
	 }
         // For each address check the word
         for (ii = 0; ii < num_offsets; ii++) {
           
           my_addr =  (1UL << 39) + (1UL << offsets[ii]) + (memshire << 6);
#ifdef POSTSI_UST
           print_dbg("TEST: Read and check address = 0x%lx\n", my_addr);
#else
           et_printf_long("TEST: Read and check address = 0x%lx\n", my_addr);
#endif
           sc_read_global(my_addr);
           sc_set_golden_word(my_addr, ii+1);
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

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


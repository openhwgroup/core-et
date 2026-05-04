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

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x3F);


   uint64_t word_offset_max;

   delay(100);

   //Only run one instance of this test
   if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
     

      // Note! As written, this test will not stomp on SMB space, but be careful when modifying
      // it to cover more memory.
      //Default range: Low OS Subregion: 0x80_8000_0000 to 0x80_FFFF_FFFF
      //Alternate range: Low Memory Subregion: 0x83_8000_0000 to 0x83_FFFF_FFFF

      // Write/read every Xth cacheline (note that this will affect which memshires are accessed)
      uint8_t cacheline_stride = 2; 
      // How many cachelines to write/read for this test (length of the test)
      uint8_t cachelines_to_test = 16;
      // Using word accesses, write/read all words in each cacheline or just the lowest?
      uint8_t test_all_words = 0;
      // Pause for a long time before writing back values to test the ddr refresh
      uint8_t pause_for_refresh = 1;
#ifdef POSTSI_UST
      print_dbg("TEST: incr: cachelines_to_test = %d cacheline_stride = %d test_all_words = %d\n",cachelines_to_test,cacheline_stride,test_all_words);
#else
      et_printf_long("TEST: incr: cachelines_to_test = %d cacheline_stride = %d test_all_words = %d\n",cachelines_to_test,cacheline_stride,test_all_words);
#endif

      //ASSUMPTION: The ddr has been initialized already for each memshire
      
      uint64_t writeData64;

      const uint64_t testData64 = 0x66aa33bb;

  
      uint64_t ii;

      uint64_t tmpAddr64;

      uint64_t word_offset;
      uint64_t base_addr;
      uint64_t compare;

      base_addr = (1UL << 39);

      if (test_all_words == 1) {
        word_offset_max = 7;
      } else {
        word_offset_max = 0;
      }
      
      // First write the first words of each cacheline, etc...
      for (word_offset = 0; word_offset <= word_offset_max; word_offset++) {
        tmpAddr64 = base_addr + (word_offset << 2);
        
        for (ii = 0; ii < cachelines_to_test; ii++) {
         
          writeData64 = testData64 + ii + (word_offset <<16); 
#ifdef POSTSI_UST
          //print_dbg("TEST: incr: writing 0x%lx to addr 0x%lx\n",writeData64,tmpAddr64);
#else
          et_printf_long("TEST: incr: writing 0x%lx to addr 0x%lx\n",writeData64,tmpAddr64);
#endif
          sc_rand_write_global_word(tmpAddr64, writeData64);
          sc_rand_evict_L3(tmpAddr64, 0x0);
          // Increment address by "stride" number of cachelines
          tmpAddr64 = tmpAddr64 + (cacheline_stride << 6);
        }
      }
      
      // Optional pause for ddr refresh before reading back values
      if (pause_for_refresh == 1) {
#ifdef POSTSI_UST
        print_dbg("TEST: incr: waiting for a while\n");
#else
        et_printf_long("TEST: incr: waiting for a while\n");
#endif
        delay(210);
#ifdef POSTSI_UST
        print_dbg("TEST: incr: done waiting for a while\n");
#else
        et_printf_long("TEST: incr: done waiting for a while\n");
#endif
      }
      
      // Read them all back and check
      for (word_offset = 0; word_offset <= word_offset_max; word_offset++) {
        tmpAddr64 = base_addr + (word_offset << 3);
        
        for (ii = 0; ii < cachelines_to_test; ii++) {
        
          writeData64 = testData64 + ii + (word_offset <<16); //expected data
          sc_read_global(tmpAddr64);
          sc_set_golden_word(tmpAddr64,writeData64);
          compare = sc_check_word(tmpAddr64);
          if (compare != 0) {
#ifdef POSTSI_UST
            print_dbg("TEST: Error: Failed compare for address 0x%lx\n",tmpAddr64);
#else
            et_printf_long("TEST: Error: Failed compare for address 0x%lx\n",tmpAddr64);
#endif
            pass = false;
          }
         
          // Increment address by "stride" number of cachelines
          tmpAddr64 = tmpAddr64 + (cacheline_stride << 6);
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


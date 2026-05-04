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
   //uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);
   uint64_t my_addr;

   uint8_t num_offsets = 25;
   uint8_t offsets[25] = {
     0,
     2,
     3,
     4,
     5,
     // 6,  // bank
     // 7,  // bank
     // 8,  // sub-bank
     // 9,  // sub-bank
     10,
     11,
     12,
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
     //31,  // Avoid SMB buffer space
     //32,    // 0x81...
     //33     // 0x82...
     // 34, // 0x84...   // Not accessible with 16 GB DDR
     // 35, // 0x88...   // Reserved
     // 36, // 0x90...   // Reserved
     // 37, // 0xA0...   // Reserved
     // 38  // 0xC0...   // Also traps
   };



   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
      uint64_t compare;

      uint8_t bank;
      uint8_t sub_bank;
      for (bank = 0; bank < 4; bank++) {
         for (sub_bank = 0; sub_bank < 4; sub_bank++) {
            uint64_t bank_addr = (sub_bank << 8) + (bank << 6);  

            // I do not want any of the lines to get evicted by a capacity eviction between 
            // the write and the read.  All reads should be hits.  
            // Therefore, break the offsets into smaller batches.
            uint8_t jj;
            for (jj = 0; jj < 8; jj++) {
               uint8_t ii;
               // For each address zero the line
               for (ii = 0; ii < num_offsets; ii++) {
                  if (ii%8 == jj) {
                     my_addr = bank_addr + (1UL << 39) + (1UL << offsets[ii]);
                     et_printf_long("TEST: Clear address = 0x%lx\n", my_addr);
                     sc_zero_line_l2(my_addr);
                  }
               }
               // For each address write a word
               for (ii = 0; ii < num_offsets; ii++) {
                  if (ii%8 == jj) {
                     my_addr = bank_addr + (1UL << 39) + (1UL << offsets[ii]);
                     et_printf_long("TEST: Write address = 0x%lx\n", my_addr);
                     sc_write_word(my_addr, ii+1);
                  }
               }
               // For each address check the word
               for (ii = 0; ii < num_offsets; ii++) {
                  if (ii%8 == jj) {
                     my_addr = bank_addr + (1UL << 39) + (1UL << offsets[ii]);
                     et_printf_long("TEST: Read and check address = 0x%lx\n", my_addr);
                     sc_read(my_addr);
                     sc_set_golden_word(my_addr, ii+1);
                     compare = sc_check_word(my_addr);
                     if (compare != 0) {
                        et_printf_long("TEST: Error: Failed compare for address 0x%lx\n", my_addr);
                        pass = false;
                     }
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


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
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);
   //uint64_t my_addr;

   uint8_t num_addr = 18;
   uint64_t addrs[18] = {
      (1UL << 39) | ( 1 << 15) | (shire_id << 6) | (0 << 11) | (0 << 13),  // l3 bank 0, sub-bank 0 
      (1UL << 39) | ( 2 << 15) | (shire_id << 6) | (0 << 11) | (1 << 13),  // l3 bank 0, sub-bank 1 
      (1UL << 39) | ( 3 << 15) | (shire_id << 6) | (0 << 11) | (2 << 13),  // l3 bank 0, sub-bank 2 
      (1UL << 39) | ( 4 << 15) | (shire_id << 6) | (0 << 11) | (3 << 13),  // l3 bank 0, sub-bank 3 
      (1UL << 39) | ( 5 << 15) | (shire_id << 6) | (1 << 11) | (0 << 13),  // l3 bank 1, sub-bank 0 
      (1UL << 39) | ( 6 << 15) | (shire_id << 6) | (1 << 11) | (1 << 13),  // l3 bank 1, sub-bank 1 
      (1UL << 39) | ( 7 << 15) | (shire_id << 6) | (1 << 11) | (2 << 13),  // l3 bank 1, sub-bank 2 
      (1UL << 39) | ( 8 << 15) | (shire_id << 6) | (1 << 11) | (3 << 13),  // l3 bank 1, sub-bank 3 
      (1UL << 39) | ( 9 << 15) | (shire_id << 6) | (2 << 11) | (0 << 13),  // l3 bank 2, sub-bank 0 
      (1UL << 39) | (10 << 15) | (shire_id << 6) | (2 << 11) | (1 << 13),  // l3 bank 2, sub-bank 1 
      (1UL << 39) | (11 << 15) | (shire_id << 6) | (2 << 11) | (2 << 13),  // l3 bank 2, sub-bank 2 
      (1UL << 39) | (12 << 15) | (shire_id << 6) | (2 << 11) | (3 << 13),  // l3 bank 2, sub-bank 3 
      (1UL << 39) | (13 << 15) | (shire_id << 6) | (3 << 11) | (0 << 13),  // l3 bank 3, sub-bank 0 
      (1UL << 39) | (14 << 15) | (shire_id << 6) | (3 << 11) | (1 << 13),  // l3 bank 3, sub-bank 1 
      (1UL << 39) | (15 << 15) | (shire_id << 6) | (3 << 11) | (2 << 13),  // l3 bank 3, sub-bank 2 
      (1UL << 39) | (16 << 15) | (shire_id << 6) | (3 << 11) | (3 << 13),  // l3 bank 3, sub-bank 3 
      (1UL << 39) | (17 << 15) | (shire_id << 6) | (3 << 11) | (3 << 13) | (1 << 23),    // other l3
      (1UL << 39) | (18 << 15) | (shire_id << 6) | (3 << 11) | (3 << 13) | (1 << 25)     // other l3
   };

   // write the lines
   // start l2 flush
   // idx_cop read should still be valid
   // read back should still match

   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
      uint64_t compare;
      uint64_t my_addr;
      cache_line_t cache_line;

      // for each address idx_cop_write
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];

         cache_line.valid = 1;
         cache_line.qw0_dw0 = 1 | (ii << 8);
         cache_line.qw0_dw1 = 2 | (ii << 8);
         cache_line.qw1_dw0 = 3 | (ii << 8);
         cache_line.qw1_dw1 = 4 | (ii << 8);
         cache_line.qw2_dw0 = 5 | (ii << 8);
         cache_line.qw2_dw1 = 6 | (ii << 8);
         cache_line.qw3_dw0 = 7 | (ii << 8);
         cache_line.qw3_dw1 = 8 | (ii << 8);

         sc_idx_cop_write_dbg(shire_id, my_addr, cache_line, IDX_L3, 1);
      }

      // l3 flush
      et_printf_long("TEST: IDX_L3_FLUSH\n");
      for ( int bank = 0; bank < 4; bank++) {
         sc_idx_cop_go(shire_id, bank, IDX_L3_FLUSH); 
      }

      // idx_cop read should be invalid
      // read back shouldn't match
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];
         cache_line.valid = 1;
         cache_line.qw0_dw0 = 1 | (ii << 8);
         cache_line.qw0_dw1 = 2 | (ii << 8);
         cache_line.qw1_dw0 = 3 | (ii << 8);
         cache_line.qw1_dw1 = 4 | (ii << 8);
         cache_line.qw2_dw0 = 5 | (ii << 8);
         cache_line.qw2_dw1 = 6 | (ii << 8);
         cache_line.qw3_dw0 = 7 | (ii << 8);
         cache_line.qw3_dw1 = 8 | (ii << 8);

         cache_line_t read_line; 
         read_line = sc_idx_cop_read_dbg(shire_id, my_addr, IDX_L3); 

         compare = sc_compare_line(cache_line, read_line);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx Failed read_idx_cop for address 0x%lx\n", hart_id, my_addr);
            pass = false;
         }

         cache_line.valid = 1;
         cache_line.qw0_dw0 = 1 | (ii << 8);
         cache_line.qw0_dw1 = 2 | (ii << 8);
         cache_line.qw1_dw0 = 3 | (ii << 8);
         cache_line.qw1_dw1 = 4 | (ii << 8);
         cache_line.qw2_dw0 = 5 | (ii << 8);
         cache_line.qw2_dw1 = 6 | (ii << 8);
         cache_line.qw3_dw0 = 7 | (ii << 8);
         cache_line.qw3_dw1 = 8 | (ii << 8);

         compare = sc_read_global_and_check_line(my_addr, cache_line);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx read did not match expected %lx\n", hart_id, my_addr);
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


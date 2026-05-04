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

   uint8_t num_addr = 20;
   uint64_t addrs[20] = {
     (1UL << 39) | ( 1 << 10) |(0 << 6) | (0 << 8),  // l2 bank 0, sub-bank 0 
     (1UL << 39) | ( 2 << 10) |(0 << 6) | (1 << 8),  // l2 bank 0, sub-bank 1 
     (1UL << 39) | ( 3 << 10) |(0 << 6) | (2 << 8),  // l2 bank 0, sub-bank 2 
     (1UL << 39) | ( 4 << 10) |(0 << 6) | (3 << 8),  // l2 bank 0, sub-bank 3 
     (1UL << 39) | ( 5 << 10) |(1 << 6) | (0 << 8),  // l2 bank 1, sub-bank 0 
     (1UL << 39) | ( 6 << 10) |(1 << 6) | (1 << 8),  // l2 bank 1, sub-bank 1 
     (1UL << 39) | ( 7 << 10) |(1 << 6) | (2 << 8),  // l2 bank 1, sub-bank 2 
     (1UL << 39) | ( 8 << 10) |(1 << 6) | (3 << 8),  // l2 bank 1, sub-bank 3 
     (1UL << 39) | ( 9 << 10) |(2 << 6) | (0 << 8),  // l2 bank 2, sub-bank 0 
     (1UL << 39) | (10 << 10) |(2 << 6) | (1 << 8),  // l2 bank 2, sub-bank 1 
     (1UL << 39) | (11 << 10) |(2 << 6) | (2 << 8),  // l2 bank 2, sub-bank 2 
     (1UL << 39) | (12 << 10) |(2 << 6) | (3 << 8),  // l2 bank 2, sub-bank 3 
     (1UL << 39) | (13 << 10) |(3 << 6) | (0 << 8),  // l2 bank 3, sub-bank 0 
     (1UL << 39) | (14 << 10) |(3 << 6) | (1 << 8),  // l2 bank 3, sub-bank 1 
     (1UL << 39) | (15 << 10) |(3 << 6) | (2 << 8),  // l2 bank 3, sub-bank 2 
     (1UL << 39) | (16 << 10) |(3 << 6) | (3 << 8),  // l2 bank 3, sub-bank 3 
     (1UL << 39) | (17 << 10) |(1 << 12),  // other l2 addr
     (1UL << 39) | (18 << 10) |(1 << 18),  // other l2 addr
     (1UL << 39) | (19 << 10) |(1 << 24),  // other l2 addr
     (1UL << 39) | (20 << 10) |(1 << 30)   // other l2 addr
   };

   // write_around the lines
   // read back should still match
   // cb_inv
   // read back should be invalid

   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
      uint64_t compare;
      uint64_t my_addr;
      cache_line_t cache_line;

      // idx_cop_write to clear out X in tag ram
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];
         cache_line.valid = 1;
         cache_line.qw0_dw0 = 0;
         cache_line.qw0_dw1 = 0;
         cache_line.qw1_dw0 = 0;
         cache_line.qw1_dw1 = 0;
         cache_line.qw2_dw0 = 0;
         cache_line.qw2_dw1 = 0;
         cache_line.qw3_dw0 = 0;
         cache_line.qw3_dw1 = 0;

         sc_idx_cop_write_dbg(shire_id, my_addr, cache_line, IDX_L2, 1); 
      }

      // l2 invalidate
      et_printf_long("TEST: IDX_L2_EVICT\n");
      for ( int bank = 0; bank < 4; bank++) {
         sc_idx_cop_go(shire_id, bank, IDX_L2_EVICT); 
      }

      // for each address perform write around
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];

         // Do write around
         sc_write_around(my_addr, ii+1);
      }

      // idx_cop read should be valid
      // read back should match
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];
         cache_line.valid = 1;
         cache_line.qw0_dw0 = ((ii+1UL)<<32) | (ii+1);
         cache_line.qw0_dw1 = ((ii+1UL)<<32) | (ii+1);
         cache_line.qw1_dw0 = 0;
         cache_line.qw1_dw1 = 0;
         cache_line.qw2_dw0 = 0;
         cache_line.qw2_dw1 = 0;
         cache_line.qw3_dw0 = 0;
         cache_line.qw3_dw1 = 0;

         cache_line_t read_line; 
         read_line = sc_idx_cop_read_dbg(shire_id, my_addr); 

         compare = sc_compare_line(cache_line, read_line, 0x1);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx Failed read_idx_cop for address 0x%lx\n", hart_id, my_addr);
            pass = false;
         } else {
            et_printf_long("TEST: Hart: 0x%lx idx_cop_read_dbg matched expected %lx\n", hart_id, my_addr);
         }
      }

      // CB Inv
      et_printf_long("TEST: IDX_CB_INV\n");
      for ( int bank = 0; bank < 4; bank++) {
         sc_idx_cop_go(shire_id, bank, IDX_CB_INV); 
      }

      // idx_cop read should be invalid
      // read back should match zero
      for ( int ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];
         cache_line.valid = 0;
         cache_line.qw0_dw0 = 0;
         cache_line.qw0_dw1 = 0;
         cache_line.qw1_dw0 = 0;
         cache_line.qw1_dw1 = 0;
         cache_line.qw2_dw0 = 0;
         cache_line.qw2_dw1 = 0;
         cache_line.qw3_dw0 = 0;
         cache_line.qw3_dw1 = 0;

         cache_line_t read_line; 
         read_line = sc_idx_cop_read_dbg(shire_id, my_addr); 

         compare = sc_compare_line(cache_line, read_line);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx Failed read_idx_cop for address 0x%lx\n", hart_id, my_addr);
            pass = false;
         } else {
            et_printf_long("TEST: Hart: 0x%lx idx_cop_read_dbg matched expected %lx\n", hart_id, my_addr);
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


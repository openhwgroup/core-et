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

   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();

      uint64_t my_addr = (1UL << 31) | (shire_id<<23) | (0 << 6) | (0 << 8);  // l2 bank 0, sub-bank 0 
      uint64_t compare;
      cache_line_t cache_line;

      sc_idx_cop_go(shire_id, 0, IDX_SCP_ZERO); 

      sc_rand_write_word(my_addr + 0*8, 1); 
      sc_rand_write_word(my_addr + 1*8, 2); 
      sc_rand_write_word(my_addr + 2*8, 3); 
      sc_rand_write_word(my_addr + 3*8, 4); 
      for ( int k = 0; k < 4; k++) {
         compare = sc_rand_read_and_check_byte(my_addr + k*8, 0);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx Failed compare for first address 0x%lx\n", hart_id, my_addr + k*8);
            pass = false;
         } else {
            et_printf_long("TEST: Hart: 0x%lx read matched expected %lx\n", hart_id, my_addr + k*8);
         }
      }
      sc_rand_write_word(my_addr + 4*8, 5); 
      sc_rand_write_word(my_addr + 5*8, 6); 
      sc_rand_write_word(my_addr + 6*8, 7); 
      sc_rand_write_word(my_addr + 7*8, 8); 
      for ( int k = 4; k < 8; k++) {
         compare = sc_rand_read_and_check_byte(my_addr + k*8, 0);
         if (compare != 0) {
            et_printf_long("TEST: Error: Hart: 0x%lx Failed compare for first address 0x%lx\n", hart_id, my_addr + k*8);
            pass = false;
         } else {
            et_printf_long("TEST: Hart: 0x%lx read matched expected %lx\n", hart_id, my_addr + k*8);
         }
      }

      cache_line.valid = 1;
      cache_line.qw0_dw0 = 1;
      cache_line.qw0_dw1 = 2;
      cache_line.qw1_dw0 = 3;
      cache_line.qw1_dw1 = 4;
      cache_line.qw2_dw0 = 5;
      cache_line.qw2_dw1 = 6;
      cache_line.qw3_dw0 = 7;
      cache_line.qw3_dw1 = 8;

      compare = sc_read_and_check_line(my_addr, cache_line);
      if (compare != 0) {
         et_printf_long("TEST: Error: Hart: 0x%lx Failed compare for address 0x%lx\n", hart_id, my_addr);
         pass = false;
      } else {
         et_printf_long("TEST: Hart: 0x%lx read matched expected %lx\n", hart_id, my_addr);
      }
   }

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


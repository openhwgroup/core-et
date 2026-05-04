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

   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
      uint64_t compare;
      uint64_t my_addr;
      my_addr = (1UL << 39);

      cache_line_t cache_line; 
      cache_line.valid = 1;
      cache_line.qw0_dw0 = 1;
      cache_line.qw0_dw1 = 2;
      cache_line.qw1_dw0 = 3;
      cache_line.qw1_dw1 = 4;
      cache_line.qw2_dw0 = 5;
      cache_line.qw2_dw1 = 6;
      cache_line.qw3_dw0 = 7;
      cache_line.qw3_dw1 = 8;

      sc_idx_cop_write_dbg(shire_id, my_addr, cache_line); 



      compare = sc_read_and_check_line(my_addr, cache_line);
      if (compare != 0) {
         et_printf_long("TEST: Error: Hart: 0x%lx Failed compare for address 0x%lx\n", hart_id, my_addr);
         pass = false;
      }

      cache_line_t read_line; 
      read_line = sc_idx_cop_read_dbg(shire_id, my_addr); 

      compare = sc_compare_line(cache_line, read_line);
      if (compare != 0) {
         et_printf_long("TEST: Error: Hart: 0x%lx Failed read_idx_cop for address 0x%lx\n", hart_id, my_addr);
         pass = false;
      }
   }


   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


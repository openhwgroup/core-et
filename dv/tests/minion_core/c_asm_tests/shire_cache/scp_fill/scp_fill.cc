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


#define OP_LIST 32
#define RUN_OPS 50

int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);

   uint8_t num_addr = 48;
   uint64_t srcs[48] = {
      // local scp address
      // Scratchpad + Shire_id       + minion            + index     + sub_bank + bank
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (0 << 10) + (3 << 8) + (3 << 6),
      // remote
      // Scratchpad + Shire_id           + minion            + index     + sub_bank + bank
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 18) + (1 << 10) + (3 << 8) + (3 << 6),
      // L3
      // memory   + index + sub_bank + bank 
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (0 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (1 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (2 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (3 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (0 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (1 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (2 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (3 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (0 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (1 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (2 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (3 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (0 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (1 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (2 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 18) + (1UL << 10) + (3 << 8) + (3 << 6)
   };

   uint64_t dsts[48] = {
      // scp address
      // Scratchpad + Shire_id       + minion            + index                 + sub_bank + bank
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 11) + (1 << 10) + (3 << 8) + (3 << 6),
      // Scratchpad + Shire_id       + minion            + index                 + sub_bank + bank
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 12) + (1 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 13) + (1 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 14) + (1 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 15) + (1 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 12) + (1 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 13) + (1 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 14) + (1 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 15) + (1 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 12) + (1 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 13) + (1 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 14) + (1 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 15) + (1 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 12) + (1 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 13) + (1 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 14) + (1 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 15) + (1 << 10) + (3 << 8) + (3 << 6),
      // Scratchpad + Shire_id       + minion            + index                 + sub_bank + bank
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 16) + (1 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 17) + (1 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 17) + (1 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 17) + (1 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 18) + (1 << 17) + (1 << 10) + (3 << 8) + (3 << 6)
   };


   if( ((hart_id & 1) == 0 )) {
      uint64_t my_addr;
      uint64_t src_addr;
      uint64_t dst_addr;

      setM0MaskFF();
      initFRegs();
      uint64_t compare;

      uint32_t data;
      volatile uint64_t conf;

      // For each address zero the line
      // For each address write a word
      data = 1;
      uint8_t ii;
      for (ii = 0; ii < num_addr; ii++) {
         my_addr = srcs[ii];
         et_printf_long("TEST: Clear address = 0x%lx 0x%lx\n", my_addr, data);
         sc_zero_line_l2(my_addr);
         sc_write_word(my_addr, data);
         data++;
      }

      // l2 flush
      et_printf_long("TEST: IDX_L2_FLUSH\n");
      for ( int bank = 0; bank < 4; bank++) {
         sc_idx_cop_go(shire_id, bank, IDX_L2_FLUSH); 
      }
      
      // For each address scp fill
      data = 1;
      for (ii = 0; ii < num_addr; ii++) {
         src_addr = srcs[ii];
         dst_addr = dsts[ii];
         et_printf_long("TEST: SCP Fill address = from 0x%lx to 0x%lx\n", src_addr, dst_addr);
         conf = src_addr | ((dst_addr>>(2ULL+6ULL)) << 48ULL) | (((dst_addr >> 6ULL) & 0x3) << 4); 
         tensorLoadL2SCP(conf, N_BYTES_CL);
         data++;
      }

      // For each address check the word
      data = 1;
      for (ii = 0; ii < num_addr; ii++) {
         my_addr = dsts[ii]; 
         et_printf_long("TEST: Read and check address = 0x%lx\n", my_addr);
         sc_read(my_addr);
         sc_set_golden_word(my_addr, data);
         compare = sc_check_word(my_addr);
         if (compare != 0) {
            et_printf_long("TEST: Error: Failed compare for address 0x%lx\n", my_addr);
            pass = false;
         }
         data++;
      }
   }


   if (pass == false) {
      et_printf_long("TEST: FAILED: hart 0x%lx\n", hart_id);
      C_TEST_FAIL;
   } else {
      et_printf_long("TEST: PASSED: hart 0x%lx\n", hart_id);
      C_TEST_PASS;
   }
   return 0;
}


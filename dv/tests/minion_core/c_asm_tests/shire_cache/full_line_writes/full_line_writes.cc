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

inline __attribute__((always_inline)) void sc_write_l1_word(uint64_t my_addr, uint64_t my_data) {
    my_addr = my_addr & ~0x3; 

    __asm__ __volatile__
        (
         "sw %[my_data], 0 (%[my_addr])\n"
         :
         :  [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         : "t0", "memory"
        );
}

int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);

   uint8_t num_addr = 48;
   uint64_t addrs[48] = {
      // local scp
      // Scratchpad + Shire_id       + minion            + hart            + index                 + sub_bank + bank
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (0 << 10) + (3 << 8) + (3 << 6),
      // remote scp
      // Scratchpad + Shire_id       + minion            + hart            + index                 + sub_bank + bank
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (0 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (1 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (2 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (3 << 8) + (0 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (0 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (1 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (2 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (3 << 8) + (1 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (0 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (1 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (2 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (3 << 8) + (2 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (0 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (1 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (2 << 8) + (3 << 6),
      (1U << 31U) + ((shire_id+1) << 23) + (minion_id << 19) + (hart_id << 18) + (2 << 10) + (3 << 8) + (3 << 6),
      // memory
      // memory   + index          + sub_bank + bank 
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (0 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (1 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (2 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (3 << 8) + (0 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (0 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (1 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (2 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (3 << 8) + (1 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (0 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (1 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (2 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (3 << 8) + (2 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (0 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (1 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (2 << 8) + (3 << 6),
      (1UL << 39) + (shire_id << 23) + (minion_id << 19) + (hart_id << 18) + (1UL << 10) + (3 << 8) + (3 << 6)
   };


   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      uint64_t my_addr;

      setM0MaskFF();
      initFRegs();
      uint64_t compare;
      uint32_t data;

      // For each address zero the line
      // For each address write a word
      data = 1;
      uint8_t ii;
      for (ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii];
         et_printf_long("TEST: Clear address = 0x%lx 0x%lx\n", my_addr, data);
         sc_zero_line_l2(my_addr);
         et_printf_long("TEST: Write address = 0x%lx 0x%lx\n", my_addr, data);
         sc_write_l1_word(my_addr, data);
         evict_va(0, 1, my_addr); 
         WAIT_CACHEOPS;
         data++;
      }
      // For each address check the word
      data = 1;
      for (ii = 0; ii < num_addr; ii++) {
         my_addr = addrs[ii]; 
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


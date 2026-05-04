/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "macros.h"
#include "l3_utils.h"

/* Includes required for 'sc_zero_line_l2()' function */
#include "cacheops.h"
#include "tensors.h"
#include "shire_cache_rand.h"
/* ************************************************** */

//PRM-19 mentions that SCP shire id bits are [6:0] wide
//ShireId bits correspond to bits[30:23] of the address
//However, the bit[30] is a duplicate of bit[29]
//For this reason IOSHIRE ID 254 is translated to 126
#define MAXION_SHIRE_ID 126

uint64_t* get_data_ptr(uint64_t hart_id, uint64_t bank_id) {

   const uint64_t scp_sh0 = 0x080000000;
   return (uint64_t*) (scp_sh0 + (MAXION_SHIRE_ID << 23) + (hart_id << 8) + (bank_id << 6));
}

int main() {
   uint64_t hart_id   = get_hart_id();
   uint64_t sid = ((hart_id>>6) & 0x3F);

   uint64_t bank = hart_id%4;
   uint64_t bank_iters = 0;

   while (bank_iters < 4) {
      uint64_t* addr = get_data_ptr(hart_id, bank);

      // Clean the line
      sc_zero_line_l2((uint64_t) addr);

      bank_iters++;
   }
   WAIT_CACHEOPS;

   bank_iters = 0;
   while (bank_iters < 4) {
      uint64_t* addr = get_data_ptr(hart_id, bank);

      // Write the line
      uint32_t write_data = (hart_id * sid) + bank_iters + 1;
      write_32b_to_l3((uint32_t*) addr, write_data);

      bank_iters++;
      bank = (bank == 3) ? 0 : bank + 1;
   }

   bank_iters = 0;
   while (bank_iters < 4) {
      uint64_t* addr = get_data_ptr(hart_id, bank);
      uint32_t golden_data = (hart_id * sid) + bank_iters + 1;

      // Read the line
      uint32_t read_data = read_32b_from_l3((uint32_t*) addr);

      // Self-Check
      if (read_data != golden_data) {
         C_TEST_FAIL;
         break;
      }

      bank_iters++;
      bank = (bank == 3) ? 0 : bank + 1;
   }

   C_TEST_PASS;
}

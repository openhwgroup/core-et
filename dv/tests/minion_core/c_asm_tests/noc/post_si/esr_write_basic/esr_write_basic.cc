/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "et_test_common.h"
#include "macros.h"
#include "fcc.h"
#include "shire_sync.h"
#include "print_dbg.h"

#define FCC_NUM_COUNTERS 2

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0x80000001
#endif

void set_shires_array(const uint64_t shires_mask, uint16_t * shires_array) {
   uint64_t mask = shires_mask;
   for (int i = 0; i < MAX_ESR_SHIRES; i++) {
      shires_array[i] = mask & 0x01;
      mask = mask >> 1;
   }
}

uint64_t get_num_shires(const uint64_t shires_mask) {
   uint64_t ret = 0;
   uint64_t mask = shires_mask;
   while (mask != 0) {
      ret = ret + (mask & 0x01);
      mask = mask >> 1;
   }
   return ret;
}

void check_shires_mask(uint64_t shire_id, const uint64_t shires_mask) {
   // Sanity check to see if shire_id is part of the shire_mask
   uint64_t check = (1 << shire_id) & shires_mask;
   if (check == 0) {
      C_TEST_FAIL;
   }
   // Sanity check to see if the shires_mask does not contain more shires than MAX_ESR_SHIRES
   if (shires_mask > (((uint64_t)1 << MAX_ESR_SHIRES)-1)) {
      C_TEST_FAIL;
   }
}

uint64_t get_next_shire(const uint16_t * shires_array, uint64_t shire_id) {
   shire_id = (shire_id + 1) % MAX_ESR_SHIRES;

   uint16_t found = 0;
   while (!found) {
      if (shires_array[shire_id] != 0) found = 1;
      else shire_id = (shire_id + 1) % MAX_ESR_SHIRES;
   }
   return shire_id;
}

uint64_t get_fcc_addr(int64_t shire_id, uint64_t thread_id, uint64_t counter_num) {
   uint64_t incr;
   uint64_t esr_addr;

   if ((counter_num > 1) || (thread_id > 1)) {
      C_TEST_FAIL;
      return 0xbadc0de;
   }

   if (thread_id == 0) {
      if (counter_num == 0) {
         esr_addr = 0x803400C0;
      }
      else {
         esr_addr = 0x803400C8;
      }
   }
   else {
      if (counter_num == 0) {
         esr_addr = 0x803400D0;
      }
      else {
         esr_addr = 0x803400D8;
      }
   }

   if (shire_id == -1) incr = 0x3fc00000; // local shire
   else                incr = shire_id * 0x400000;

   return esr_addr + incr;
}



int main() {
   static_assert(NOC_TGT_SHIRE_MASK == SHIRE_MASK, "NOC_TGT_SHIRE_MASK must be equal to SHIRE_MASK");
   const uint64_t hart_id = get_hart_id();
   const uint64_t thread_id = hart_id & 0x01;
   const uint64_t minion_id = (hart_id >> 1) & 0x1f;
   const uint64_t sid = ((hart_id>>6) & 0x3F);
   const uint64_t num_shires = get_num_shires(SHIRE_MASK);
   const uint64_t fcc_mask = 1 << minion_id;

   // Sanity check to make sure my shire is in the SHIRE_MASK
   check_shires_mask(sid, SHIRE_MASK);

   uint16_t shires_array[MAX_ESR_SHIRES];
   set_shires_array(SHIRE_MASK, shires_array);

   uint64_t fcc_sid = sid;
   for (uint64_t c = 0; c < FCC_NUM_COUNTERS; c++) {

      // Sanity check to make sure 'get_next_shire()' function works properly
      if (sid != fcc_sid) C_TEST_FAIL;

      for (uint64_t i = 0; i < num_shires; i++) {
         uint64_t *fcc_addr = (uint64_t*) get_fcc_addr(fcc_sid, thread_id, c);

#ifdef DEBUG
         et_printf_long("H%d TEST: Writing to S%d FCC Counter %d\n", hart_id, fcc_sid, c);
#endif
#ifdef POSTSI_DEBUG
         print_dbg("H%d TEST: Writing to S%d FCC Counter %d\n", hart_id, fcc_sid, c);
#endif

         // Send FCC credit
         *fcc_addr = fcc_mask;

         fcc_sid = get_next_shire(shires_array, fcc_sid);
      }
   }


   // Self-check: make sure we receive all the expected FCC credits
   for (uint64_t c = 0; c < FCC_NUM_COUNTERS; c++) {
      for (uint64_t i = 0; i < num_shires; i++) {
         wait_for_credit(c);
      }
   }

   // Sanity check to make sure there is no remaining FCC credits after the test
   for (uint64_t c = 0; c < FCC_NUM_COUNTERS; c++) {
      uint64_t fcc_credits = get_credit_count_nb(c);
      if (fcc_credits != 0) C_TEST_FAIL;
   }

   C_TEST_PASS;
}

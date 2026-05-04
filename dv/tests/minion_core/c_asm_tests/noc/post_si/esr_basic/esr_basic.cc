/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "macros.h"
#include "et_test_common.h"
#include "shire_sync.h"
#include "fcc.h"
#include "print_dbg.h"

#define SEQUENCE_SIZE 8

#define BASE_IPI_REDIRECT_FILTER_ADDR 0x80F40088

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xFFFFFFFF
#endif

#define FCC_COUNT_NUM 0

inline uint64_t get_base_esr_shire(uint64_t esr_base_addr, uint64_t shire_id) {
   if (shire_id >= MAX_ESR_SHIRES) {
      C_TEST_FAIL;
      return 0xbadc0de;
   }
   // bits [29:22] correspond to the Shire ID
   esr_base_addr += (shire_id << 22);
   return esr_base_addr;
}

void gen_minion_shire_esr_traffic(uint64_t sid) {
   uint64_t iters = 0;

   while (iters < MAX_ESR_SHIRES) {
      uint64_t sid_w = sid;
      for (int i = 0; i < SEQUENCE_SIZE; i++) {
         if ((NOC_TGT_SHIRE_MASK >> sid_w) & 0x1) {
            uint64_t gold_esr_value = (sid_w*sid_w*sid_w)-1;

            uint64_t esr_addr = get_base_esr_shire(BASE_IPI_REDIRECT_FILTER_ADDR, sid_w);
#ifdef DEBUG
            et_printf_long("TEST: Writing data 0x%lx to S%d addr 0x%lx\n", gold_esr_value, sid_w, esr_addr);
#endif
#ifdef POSTSI_DEBUG
            print_dbg("TEST: Writing data 0x%x to S%d addr 0x%x\n", gold_esr_value, sid_w, esr_addr);
#endif
            uint64_t *esr_ptr = (uint64_t*) esr_addr;
            *esr_ptr = gold_esr_value;
         }
         sid_w = (sid_w == (MAX_ESR_SHIRES-1)) ? 0 : sid_w + 1;
      }

      uint64_t sid_r = sid;
      for (int i = 0; i < SEQUENCE_SIZE; i++) {
         if ((NOC_TGT_SHIRE_MASK >> sid_r) & 0x1) {
            uint64_t gold_esr_value = (sid_r*sid_r*sid_r)-1;

            uint64_t esr_addr = get_base_esr_shire(BASE_IPI_REDIRECT_FILTER_ADDR, sid_r);
            uint64_t *esr_ptr = (uint64_t*) esr_addr;
            uint64_t read_esr_value = *esr_ptr;
#ifdef DEBUG
            et_printf_long("TEST: Read data 0x%lx from S%d addr 0x%lx\n", read_esr_value, sid_r, esr_addr);
#endif
#ifdef POSTSI_DEBUG
            print_dbg("TEST: Read data 0x%x to S%d addr 0x%x\n", read_esr_value, sid_r, esr_addr);
#endif
            if (read_esr_value != gold_esr_value) C_TEST_FAIL;
         }
         sid_r = (sid_r == (MAX_ESR_SHIRES-1)) ? 0 : sid_r + 1;
      }

      if (sid_r != sid_w) C_TEST_FAIL;
      sid = sid_r;
      iters += SEQUENCE_SIZE;
   }
}


int main() {
   uint64_t hart_id = get_hart_id();
   uint64_t local_hart_id = (hart_id & 0x3F);

   if (local_hart_id == LOCAL_ACTIVE_HART) {
      uint64_t sid = ((hart_id>>6) & 0x3F);
      uint64_t mid = local_hart_id >> 1;
      uint64_t tid = (hart_id & 0x1);

      // Base HART is the HART that starts the computation
      uint64_t base_hart = get_base_hart(LOCAL_ACTIVE_HART);

      if (base_hart == hart_id) {
         // Base HART must send an FCC to itself to start the computation
         give_credit(sid, mid, tid, FCC_COUNT_NUM);
      }

      // Everyone waits until is its turn (aka when it receives an FCC credit)
      wait_for_credit(FCC_COUNT_NUM);

#ifdef DEBUG
      et_printf_long("H%d TEST: Sending ESR Traffic\n", hart_id);
#endif
#ifdef POSTSI_DEBUG
      print_dbg("H%d TEST: Sending ESR Traffic\n", hart_id);
#endif

      // Do the computation
      gen_minion_shire_esr_traffic(sid);

      // Computation done, time to send an FCC to the next minion of another Shire
      uint8_t next_sid = get_next_shire_id(sid);

#ifdef DEBUG
      et_printf_long("H%d TEST: Sending FCC to S%d\n", hart_id, next_sid);
#endif
#ifdef POSTSI_DEBUG
      print_dbg("H%d TEST: Sending FCC to S%d\n", hart_id, next_sid);
#endif

      give_credit(next_sid, mid, tid, FCC_COUNT_NUM);

      // Base HART will receive an extra credit at the very end from the last Minion of the chain
      if (base_hart == hart_id) {
         wait_for_credit(FCC_COUNT_NUM);
      }
   }

   C_TEST_PASS;
}

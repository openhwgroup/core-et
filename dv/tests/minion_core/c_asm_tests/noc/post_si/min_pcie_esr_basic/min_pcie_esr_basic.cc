/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "et_test_common.h"
#include "macros.h"
#include "fcc.h"
#include "pshire_esr.h"
#include "shire_sync.h"

#define FCC_COUNT_NUM 0

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#ifndef SYS_EMU_ONLY
   #define READ_ESR_PSHIRE(ADDR) read_pshire_esr((ADDR))
   #define WRITE_ESR_PSHIRE(ADDR, VALUE) write_pshire_esr((ADDR), (VALUE))
#else
   uint64_t esr_stub;
   #define READ_ESR_PSHIRE(ADDR) esr_stub;
   #define WRITE_ESR_PSHIRE(ADDR, VALUE) esr_stub = (uint64_t)(VALUE);
#endif

void gen_pshire_esr_traffic() {
   uint64_t golden_value = 0x00ABCDEF;
   uint64_t test_addr = 0x7F80001008;

#ifdef DEBUG
   et_printf_long("TEST: Write and read address = 0x%lx\n", test_addr);
#endif
#ifdef POSTSI_DEBUG
   print_dbg("TEST: Write and read address = 0x%x\n", test_addr);
#endif

   WRITE_ESR_PSHIRE(test_addr, golden_value);

   uint64_t read_value = READ_ESR_PSHIRE(test_addr);

   if (read_value != golden_value) C_TEST_FAIL;

   golden_value = 0xA5FF005A;
   test_addr = 0x7F80001018;

#ifdef DEBUG
   et_printf_long("TEST: Write and read address = 0x%lx\n", test_addr);
#endif
#ifdef POSTSI_DEBUG
   print_dbg("TEST: Write and read address = 0x%x\n", test_addr);
#endif

   WRITE_ESR_PSHIRE(test_addr, golden_value);

   read_value = READ_ESR_PSHIRE(test_addr);

   if (read_value != golden_value) C_TEST_FAIL;

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
      et_printf_long("H%d TEST: Sending PShire ESR traffic\n", hart_id);
#endif
#ifdef POSTSI_DEBUG
      print_dbg("H%d TEST: Sending PShire ESR traffic\n", hart_id);
#endif

      // Do the computation
      gen_pshire_esr_traffic();

      // Computation done, time to send an FCC to the next minion of another Shire
      uint8_t next_sid = get_next_shire_id(sid);
      give_credit(next_sid, mid, tid, FCC_COUNT_NUM);

      // Base HART will receive an extra credit at the very end from the last Minion of the chain
      if (base_hart == hart_id) {
         wait_for_credit(FCC_COUNT_NUM);
      }
   }

   C_TEST_PASS;
}

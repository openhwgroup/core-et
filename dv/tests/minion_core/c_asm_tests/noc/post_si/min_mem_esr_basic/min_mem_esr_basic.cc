/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "macros.h"
#include "fcc.h"
#include "mem_esr.h"
#include "shire_sync.h"
#include "print_dbg.h"

#define FCC_COUNT_NUM 0

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#define NUM_MEM_SHIRES 8

#ifndef SYS_EMU_ONLY
   #define READ_ESR_DDRC_SCRATCH(SID) read_ddrc_esr(PP_MACHINE, (SID), 0x00E)
   #define WRITE_ESR_DDRC_SCRATCH(SID, VALUE) write_ddrc_esr(PP_MACHINE, (SID), 0x00E, (VALUE))
#else
   uint64_t esr_stub[NUM_MEM_SHIRES];
   #define READ_ESR_DDRC_SCRATCH(SID) esr_stub[(SID)%8];
   #define WRITE_ESR_DDRC_SCRATCH(SID, VALUE) esr_stub[(SID)%8] = (uint64_t)(VALUE);
#endif

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xff
#endif

const uint8_t mem_sid[NUM_MEM_SHIRES] = {232, 233, 234, 235, 236, 237, 238, 239}; // 8 Mem Shires

void gen_mem_shire_esr_traffic() {
   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      WRITE_ESR_DDRC_SCRATCH(mem_sid[i], mem_sid[i]);
   }
   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      uint64_t golden_value = mem_sid[i];
      uint64_t read_value = READ_ESR_DDRC_SCRATCH(mem_sid[i]);
      if (read_value != golden_value) C_TEST_FAIL;
   }
}

int main() {
   static_assert(0 < NOC_TGT_SHIRE_MASK && NOC_TGT_SHIRE_MASK <= 0xff, "NOC_TGT_SHIRE_MASK (mem shire mask) must be between 0x1 and 0xFF");
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

#ifdef POSTSI_DEBUG
      print_dbg("H%d TEST: Sending ESR Traffic\n", hart_id);
#endif

      // Do the computation
      gen_mem_shire_esr_traffic();

      // Computation done, time to send an FCC to the next minion of another Shire
      uint8_t next_sid = get_next_shire_id(sid);

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

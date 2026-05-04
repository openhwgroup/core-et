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
#include "shire_sync.h"
#include "fcc.h"

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#define FCC_COUNT_NUM 0
#define NEIGH0_IPI_REDIRECT_PC 0x80100040

// Shire Cache Sepc: L3 address decoding defines that shire bits are [10:6]
// Bank bits is [12:11]. We need to align the data to 8KB to make sure the base address maps to Shire 0, Bank 0
#define ALIGN_DATA 8192
// 'data' size is 8192 Bytes
uint64_t data[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));

// Floating point register usage
// f0 zero
// f1 golden
// f2 read back
// f3 compare result (1 if equal)
// f4 write temp

int main()
{
   volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t local_hart_id = (hart_id & 0x3F);
   uint64_t my_addr;

   if (local_hart_id == LOCAL_ACTIVE_HART) {
      uint64_t sid = ((hart_id>>6) & 0x3F);
      uint64_t mid = local_hart_id >> 1;
      uint64_t tid = (hart_id & 0x1);

      // Random offset to not always write at the same portion of the cache line
      // As only one minion writes to this cache line at a time, it does not matter
      // if multiple minions have the same offset
      const uint64_t offset = sid/8;

      // Base HART is the HART that starts the computation
      uint64_t base_hart = get_base_hart(LOCAL_ACTIVE_HART);

      if (base_hart == hart_id) {
         // Base HART must send an FCC to itself to start the computation
         give_credit(sid, mid, tid, FCC_COUNT_NUM);
      }

      // Everyone waits until is its turn (aka when it receives an FCC credit)
      wait_for_credit(FCC_COUNT_NUM);

      // Do the computation
      setM0MaskFF();
      initFRegs();
      uint64_t compare;

      uint8_t bank;
      for (bank = 0; bank < 4; bank++) {

         // I do not want any of the lines to get evicted by a capacity eviction between 
         // the write and the read.  All reads should be hits.  
         // Therefore, break the offsets into smaller batches.
         uint8_t tgt_shire;
         for (tgt_shire = 0; tgt_shire < MAX_L3_SHIRES; tgt_shire++) {
            my_addr = (uint64_t) &data[bank][tgt_shire][offset];
#ifdef DEBUG
            et_printf_long("H%d TEST: Clear address = 0x%lx\n", hart_id, my_addr);
#endif
            sc_zero_line_l3(my_addr);
         }
         WAIT_CACHEOPS;
         for (tgt_shire = 0; tgt_shire < MAX_L3_SHIRES; tgt_shire++) {
            my_addr = (uint64_t) &data[bank][tgt_shire][offset];
#ifdef DEBUG
            et_printf_long("H%d TEST: Write address = 0x%lx\n", hart_id, my_addr);
#endif
            sc_write_global_word(my_addr, tgt_shire);
         }
         for (tgt_shire = 0; tgt_shire < MAX_L3_SHIRES; tgt_shire++) {
            my_addr = (uint64_t) &data[bank][tgt_shire][offset];
#ifdef DEBUG
            et_printf_long("H%d TEST: Read and check address = 0x%lx\n", hart_id, my_addr);
#endif
            sc_read_global(my_addr);
            sc_set_golden_word(my_addr, tgt_shire);
            compare = sc_check_word(my_addr);
            if (compare != 0) {
#ifdef DEBUG
               et_printf_long("H%d TEST: Error: Failed compare for address 0x%lx\n", hart_id, my_addr);
#endif
               pass = false;
               break;
            }
         }
         if (pass == false) break;
      }

      // Computation done, time to send an FCC to the next minion of another Shire
      uint8_t next_sid = get_next_shire_id(sid);
      give_credit(next_sid, mid, tid, FCC_COUNT_NUM);

      // Base HART will receive an extra credit at the very end from the last Minion of the chain
      if (base_hart == hart_id) {
         wait_for_credit(FCC_COUNT_NUM);
      }
   }

   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


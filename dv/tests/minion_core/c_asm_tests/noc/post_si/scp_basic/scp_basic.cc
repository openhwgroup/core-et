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
#include "print_dbg.h"

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xFFFFFFFF
#endif

#define FCC_COUNT_NUM 0

// Floating point register usage
// f0 zero
// f1 golden
// f2 read back
// f3 compare result (1 if equal)
// f4 write temp

int main()
{
   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t local_hart_id = (hart_id & 0x3F);
   uint64_t my_addr;
   uint64_t max_addr = 0x240f;


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

      // Do the computation
      setM0MaskFF();
      initFRegs();
      uint64_t compare;

      uint8_t bank;
      for (bank = 0; bank < 4; bank++) {
         uint64_t bank_addr = bank << 6;  

         // I do not want any of the lines to get evicted by a capacity eviction between 
         // the write and the read.  All reads should be hits.  
         // Therefore, break the offsets into smaller batches.
         uint8_t tgt_shire;
         for (tgt_shire = 0; tgt_shire < MAX_SCP_SHIRES; tgt_shire++) {
            if ((NOC_TGT_SHIRE_MASK >> tgt_shire) & 0x1) {
               my_addr = bank_addr + (1UL << 31) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Clear address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST:  Clear address = 0x%x\n", hart_id, my_addr);
#endif
               sc_zero_line_l2(my_addr);
               my_addr = bank_addr + (1UL << 31) + (max_addr << 8) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Clear address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST:  Clear address = 0x%x\n", hart_id, my_addr);
#endif
               sc_zero_line_l2(my_addr);
            }
         }
         WAIT_CACHEOPS;
         for (tgt_shire = 0; tgt_shire < MAX_SCP_SHIRES; tgt_shire++) {
            if ((NOC_TGT_SHIRE_MASK >> tgt_shire) & 0x1) {
               my_addr = bank_addr + (1UL << 31) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Write address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST: Write address = 0x%x\n", hart_id, my_addr);
#endif
               sc_write_global_word(my_addr, tgt_shire);
               my_addr = bank_addr + (1UL << 31) + (max_addr << 8) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Write address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST: Write address = 0x%x\n", hart_id, my_addr);
#endif
               sc_write_word(my_addr, (tgt_shire << 8));
            }
         }
         for (tgt_shire = 0; tgt_shire < MAX_SCP_SHIRES; tgt_shire++) {
            if ((NOC_TGT_SHIRE_MASK >> tgt_shire) & 0x1) {
               my_addr = bank_addr + (1UL << 31) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Read and check address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST: Read and check address = 0x%x\n", hart_id, my_addr);
#endif
               sc_read(my_addr);
               sc_set_golden_word(my_addr, tgt_shire);
               compare = sc_check_word(my_addr);
               if (compare != 0) {
#ifdef DEBUG
                  et_printf_long("H%d TEST: Error: Failed compare for address 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
                  print_dbg("H%d TEST: Error: Failed compare for address 0x%x\n", hart_id, my_addr);
#endif
                  pass = false;
                  break;
               }
               my_addr = bank_addr + (1UL << 31) + (max_addr << 8) + (tgt_shire << 23);
#ifdef DEBUG
               et_printf_long("H%d TEST: Read and check address = 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST: Read and check address = 0x%x\n", hart_id, my_addr);
#endif
               sc_read(my_addr);
               sc_set_golden_word(my_addr, (tgt_shire << 8));
               compare = sc_check_word(my_addr);
               if (compare != 0) {
#ifdef DEBUG
                  et_printf_long("H%d TEST: Error: Failed compare for address 0x%lx\n", hart_id, my_addr);
#endif
#ifdef POSTSI_DEBUG
                  print_dbg("H%d TEST: Error: Failed compare for address 0x%x\n", hart_id, my_addr);
#endif
                  pass = false;
                  break;
               }
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


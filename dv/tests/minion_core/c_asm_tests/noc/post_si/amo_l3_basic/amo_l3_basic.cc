/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "macros.h"
#include "et_test_common.h"
#include "amo_ops.h"
#include "shire_sync.h"
#include "fcc.h"
#include "print_dbg.h"

#define FCC_COUNT_NUM 0
#define SEQUENCE_SIZE 8

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xFFFFFFFF
#endif

//Shire Cache Sepc: L3 address decoding defines that shire bits are [10:6] 
// Bank bits is [12:11]. We need to align the data to 8KB to make sure the base address maps to Shire 0, Bank 0
#define ALIGN_DATA 8192

// Each 'datax_y' size is 8192 Bytes
uint64_t data0_0[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data0_1[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data1_0[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data1_1[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data2_0[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data2_1[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data3_0[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));
uint64_t data3_1[4][MAX_L3_SHIRES][8] __attribute__ ((aligned (ALIGN_DATA)));

uint64_t* get_data_ptr(uint64_t hart_id, uint64_t bank_id) {
   uint64_t thread_id = hart_id & 0x1;
   uint64_t neigh_id = (hart_id >> 4) & 0x3; 
   if (neigh_id == 0) {
      if (thread_id == 0) return &data0_0[bank_id][0][0];
      else                return &data0_1[bank_id][0][0];
   }
   else if (neigh_id == 1) {
      if (thread_id == 0) return &data1_0[bank_id][0][0];
      else                return &data1_1[bank_id][0][0];
   }
   else if (neigh_id == 2) {
      if (thread_id == 0) return &data2_0[bank_id][0][0];
      else                return &data2_1[bank_id][0][0];
   }
   else if (neigh_id == 3) {
      if (thread_id == 0) return &data3_0[bank_id][0][0];
      else                return &data3_1[bank_id][0][0];
   }
   else {
      C_TEST_FAIL;
      return (uint64_t*) 0xbadc0de;
   }
}

int main() {
   uint64_t hart_id   = get_hart_id();
   uint64_t local_hart_id = (hart_id & 0x3F);
   uint64_t minion_id = ((hart_id>>1) & 0x3F);

   uint64_t sid = ((hart_id>>6) & 0x3F);
   uint64_t mid = local_hart_id >> 1;
   uint64_t tid = (hart_id & 0x1);
   // Base HART is the HART that starts the computation
   uint64_t base_hart = get_base_hart(local_hart_id);
   if (base_hart == hart_id) {
      // Base HART must send an FCC to itself to start the computation
      give_credit(sid, mid, tid, FCC_COUNT_NUM);
   }

   // Everyone waits until is its turn (aka when it receives an FCC credit)
   wait_for_credit(FCC_COUNT_NUM);

   uint64_t data_id = minion_id & 0x7;

   uint64_t bank = hart_id%4;
   uint64_t bank_iters = 0;

   while (bank_iters < 4) {

      uint64_t sid = ((hart_id>>6) % MAX_L3_SHIRES);
      uint64_t write_data;
      uint64_t iters = 0;
      uint64_t *data_ptr = get_data_ptr(hart_id, bank);
      uint64_t (*my_data)[8] = (uint64_t (*)[8]) data_ptr;

      while (iters < MAX_L3_SHIRES) {
         uint64_t sid_w = sid;
         for (int i = 0; i < SEQUENCE_SIZE; i++) {
            if ((NOC_TGT_SHIRE_MASK >> sid_w) & 0x1) {
               uint64_t *addr = &my_data[sid_w][data_id];
               write_data = (hart_id * sid_w) + 1;

#ifdef DEBUG
               et_printf_long("H%d TEST: Write address = 0x%lx\n", hart_id, addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST:  Write address = 0x%x\n", hart_id, addr);
#endif

               // ATOMIC WRITE
               amo_write(addr, write_data);
            }

            sid_w = (sid_w == (MAX_L3_SHIRES-1)) ? 0 : sid_w + 1;
         }

         uint64_t sid_r = sid;
         for (int i = 0; i < SEQUENCE_SIZE; i++) {
            if ((NOC_TGT_SHIRE_MASK >> sid_r) & 0x1) {
               uint64_t *addr = &my_data[sid_r][data_id];
               write_data = (hart_id * sid_r) + 1;

#ifdef DEBUG
               et_printf_long("H%d TEST: Read address = 0x%lx\n", hart_id, addr);
#endif
#ifdef POSTSI_DEBUG
               print_dbg("H%d TEST:  Read address = 0x%x\n", hart_id, addr);
#endif

               // ATOMIC READ
               uint64_t read_data = amo_read(addr);

               // COMPARE READ VALUE AGAINST THE WRITTEN VALUE
               if (read_data != write_data) C_TEST_FAIL;
            }
            sid_r = (sid_r == (MAX_L3_SHIRES-1)) ? 0 : sid_r + 1;
         }

         if (sid_r != sid_w) C_TEST_FAIL;
         sid = sid_r;
         iters += SEQUENCE_SIZE;
      }
      bank_iters++;
      bank = (bank == 3) ? 0 : bank + 1;
   }

   // Computation done, time to send an FCC to the next minion of another Shire
   uint8_t next_sid = get_next_shire_id(sid);
   give_credit(next_sid, mid, tid, FCC_COUNT_NUM);

   // Base HART will receive an extra credit at the very end from the last Minion of the chain
   if (base_hart == hart_id) {
      wait_for_credit(FCC_COUNT_NUM);
   }

   C_TEST_PASS;
}

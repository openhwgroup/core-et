/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdint.h>
#include "et_test_common.h"
#include "minion.h"
#include "macros.h"
#include "print_dbg.h"

#include "shire_sync.h"
#include "sc_utils.h"
#include "amo_ops.h"

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xff
#endif

// Bits [8:6] of L3 addresses will map to the Mem Shires
// We need to align the data to 512 Bytes to make sure the base address maps to Mem Shire 0
#define ALIGN_DATA 512

#define NUM_MEM_SHIRES 8
#define BYTES_PER_CACHE_LINE 64

typedef struct cache_line {
   uint8_t bytes[BYTES_PER_CACHE_LINE];
} cache_line;

cache_line data[MAX_HARTS_SOC][NUM_MEM_SHIRES] __attribute__ ((aligned (ALIGN_DATA)));

#define GET_DATA_PTR(HID, MEM_SID) (uint64_t*) &data[(HID)][(MEM_SID)].bytes[0]

int main() {
   static_assert(0 < NOC_TGT_SHIRE_MASK && NOC_TGT_SHIRE_MASK <= 0xff, "NOC_TGT_SHIRE_MASK (mem shire mask) must be between 0x1 and 0xFF");
   uint64_t hart_id = get_hart_id();

   // Disable L3 to force AMO request to go directly to MemShire
   disable_l3(hart_id, LOCAL_ACTIVE_HART);

   // Do AMO write
   uint64_t mem_sid = hart_id % NUM_MEM_SHIRES;
   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      if ((NOC_TGT_SHIRE_MASK >> mem_sid) & 0x1) {
         uint64_t* addr = GET_DATA_PTR(hart_id, mem_sid);
         uint64_t  write_value = (hart_id+i)*-1;
#ifdef DEBUG
         et_printf_long("H%d TEST: AMO write to S%d addr 0x%lx\n", hart_id, mem_sid, addr);
#endif
#ifdef POSTSI_DEBUG
         print_dbg("H%d TEST: AMO write to S%d addr 0x%x\n", hart_id, mem_sid, addr);
#endif
         amo_write(addr, write_value);
      }
      mem_sid = (mem_sid + 1) % NUM_MEM_SHIRES;
   }

   // Do AMO read and self-check
   mem_sid = hart_id % NUM_MEM_SHIRES;
   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      if ((NOC_TGT_SHIRE_MASK >> mem_sid) & 0x1) {
         uint64_t* addr = GET_DATA_PTR(hart_id, mem_sid);
         uint64_t  gold_value = (hart_id+i)*-1;
#ifdef DEBUG
         et_printf_long("H%d TEST: AMO read to S%d addr 0x%lx\n", hart_id, mem_sid, addr);
#endif
#ifdef POSTSI_DEBUG
         print_dbg("H%d TEST: AMO read from S%d addr 0x%x\n", hart_id, mem_sid, addr);
#endif
         uint64_t  read_value = amo_read(addr);

         // Self-check
         if (gold_value != read_value) C_TEST_FAIL;
      }
      mem_sid = (mem_sid + 1) % NUM_MEM_SHIRES;
   }

   C_TEST_PASS;
}

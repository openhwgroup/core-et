/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "et_test_common.h"
#include "macros.h"
#include "cacheops.h"
#include "sc_utils.h"
#include "print_dbg.h"

#ifndef LOCAL_ACTIVE_HART
   #define LOCAL_ACTIVE_HART 0
#endif

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xFFFFFFFF
#endif

// Bits [8:6] of L3 addresses will map to the Mem Shires
// We need to align the data to 512 Bytes to make sure the base address maps to Mem Shire 0
#define ALIGN_DATA 512

#define NUM_MEM_SHIRES 8
#define BYTES_PER_CACHE_LINE 64

#define EVICT_VA_MEM(ADDR) evict_va(0, 0x3, (uint64_t) (ADDR), NUM_MEM_SHIRES-1, 64, 0, 0)

typedef struct cache_line {
   uint8_t bytes[BYTES_PER_CACHE_LINE];
} cache_line;

cache_line data[MAX_HARTS_SOC][NUM_MEM_SHIRES] __attribute__ ((aligned (ALIGN_DATA)));

uint64_t* get_data_ptr(uint64_t hart_id, uint64_t mem_shire_id) {
   if ((mem_shire_id < NUM_MEM_SHIRES) && (hart_id < MAX_HARTS_SOC)) {
      return (uint64_t*) &data[hart_id][mem_shire_id].bytes[0];
   }
   else {
      C_TEST_FAIL;
      return (uint64_t*) 0xbadc0de;
   }
}

int main() {
   uint64_t hart_id = get_hart_id();

#ifdef DISABLE_L3
   disable_l3(hart_id, LOCAL_ACTIVE_HART);
#endif

   uint64_t mem_sid = hart_id % NUM_MEM_SHIRES;

   EVICT_VA_MEM(get_data_ptr(hart_id, 0));
   WAIT_CACHEOPS;
   FENCE;

   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      if ((NOC_TGT_SHIRE_MASK >> i) & 0x1) {
         uint64_t* addr = get_data_ptr(hart_id, mem_sid);
         uint64_t  write_value = (hart_id+i)*-1;
         *addr = write_value;
         mem_sid = (mem_sid + 1) % NUM_MEM_SHIRES;
      }
   }

   FENCE;
   EVICT_VA_MEM(get_data_ptr(hart_id, 0));
   WAIT_CACHEOPS;
   FENCE;

   mem_sid = hart_id % NUM_MEM_SHIRES;
   for (int i = 0; i < NUM_MEM_SHIRES; i++) {
      if ((NOC_TGT_SHIRE_MASK >> i) & 0x1) {
         uint64_t  golden_value = (hart_id+i)*-1;
         uint64_t* addr = get_data_ptr(hart_id, mem_sid);
         uint64_t  read_value = *addr;
         if (read_value != golden_value) C_TEST_FAIL;
         mem_sid = (mem_sid + 1) % NUM_MEM_SHIRES;
      }
   }

   FENCE;
   EVICT_VA_MEM(get_data_ptr(hart_id, 0));
   WAIT_CACHEOPS;

   C_TEST_PASS;
}

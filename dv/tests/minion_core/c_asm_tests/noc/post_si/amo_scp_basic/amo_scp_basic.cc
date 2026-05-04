/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "minion.h"
#include "et_test_common.h"
#include "macros.h"
#include "amo_ops.h"
#include "print_dbg.h"

/* Includes required for 'sc_zero_line_l2()' function */
#include "cacheops.h"
#include "tensors.h"
#include "shire_sync.h"
#include "shire_cache_rand.h"
/* ************************************************** */

#define SEQUENCE_SIZE 8

#ifndef NOC_TGT_SHIRE_MASK
   #define NOC_TGT_SHIRE_MASK 0xFFFFFFFF
#endif

uint64_t* get_data_ptr(uint64_t hart_id, uint64_t shire_id, uint64_t bank_id) {

   const uint64_t scp_sh0 = 0x080000000;

   if ((bank_id < 4) && (shire_id < MAX_SCP_SHIRES)) {
      return (uint64_t*) (scp_sh0 + (shire_id << 23) + (hart_id << 8) + (bank_id << 6));
   }
   else {
      C_TEST_FAIL;
      return (uint64_t*) 0xbadc0de;
   }
}

inline uint64_t get_next_scp_shire_id(uint64_t shire_id) {
   return (shire_id == (MAX_SCP_SHIRES-1)) ? 0 : shire_id + 1;
}

inline uint64_t* get_addr_to_shireid(uint64_t* base_addr, uint64_t shire_id) {
   return (uint64_t*) (((uint64_t) base_addr) + (shire_id << 23));
}

inline uint64_t* get_addr_to_shireid_offset(uint64_t* base_addr, uint64_t shire_id, uint64_t hart_id) {
   // This version adds offset within the cache line in order to not only target cache aligned addresses
   uint64_t shire_scp_base_addr = (uint64_t) get_addr_to_shireid(base_addr, shire_id);
   uint64_t cl_offset = ((hart_id+shire_id)%8) << 3;
   return (uint64_t*) (shire_scp_base_addr + cl_offset);
}

int main() {
   uint64_t hart_id   = get_hart_id();

   uint64_t bank = hart_id%4;
   uint64_t bank_iters = 0;

   while (bank_iters < 4) {

      uint64_t sid = ((hart_id>>6) % MAX_SCP_SHIRES);
      uint64_t write_data;
      uint64_t iters = 0;
      uint64_t* base_addr = get_data_ptr(hart_id, 0, bank);

      while (iters < MAX_SCP_SHIRES) {
         // Initialize L2 SCP region
         uint64_t sid_w = sid;
         for (int i = 0; i < SEQUENCE_SIZE; i++) {
            uint64_t *addr = get_addr_to_shireid(base_addr, sid_w);
            sc_zero_line_l2((uint64_t) addr);
            sid_w = get_next_scp_shire_id(sid_w);
         }
         WAIT_CACHEOPS;

         sid_w = sid;
         for (int i = 0; i < SEQUENCE_SIZE; i++) {
            if ((NOC_TGT_SHIRE_MASK >> sid_w) & 0x1) {
               uint64_t *addr = get_addr_to_shireid_offset(base_addr, sid_w, hart_id);
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

            sid_w = get_next_scp_shire_id(sid_w);
         }

         uint64_t sid_r = sid;
         for (int i = 0; i < SEQUENCE_SIZE; i++) {
            if ((NOC_TGT_SHIRE_MASK >> sid_r) & 0x1) {
               uint64_t *addr = get_addr_to_shireid_offset(base_addr, sid_r, hart_id);
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
            sid_r = get_next_scp_shire_id(sid_r);
         }

         if (sid_r != sid_w) C_TEST_FAIL;
         sid = sid_r;
         iters += SEQUENCE_SIZE;
      }
      bank_iters++;
      bank = (bank == 3) ? 0 : bank + 1;
   }

   C_TEST_PASS;
}

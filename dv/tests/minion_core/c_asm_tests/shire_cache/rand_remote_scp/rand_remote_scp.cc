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


#define OP_LIST 32
#define RUN_OPS 50

int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);
   uint64_t my_addr;

   // Hart_id includes minion.
   uint64_t offset = ((hart_id & 0xf) <<4);

   // Main memory
   my_addr = 
         (1U << 31U) + // Scratchpad
         ((shire_id+1) << 23) + // Shire_id
         (minion_id << 19) + // minion
         (hart_id << 18) + // hart
         (0 << 11) + // index
         (0 << 9) + // sub_bank
         (0 << 6);  // bank

   if (hart_id == 0 ) {
      // For zero the line
      sc_zero_line_l2(my_addr);
      et_printf_long("TEST: hart 0x%lx zeroed line \n", hart_id);
   }

   SC_OP_t write_b_op   = {sc_rand_write_byte,            "write_byte"};
   SC_OP_t read_b_op    = {sc_rand_read_and_check_byte,   "read_byte"};
   SC_OP_t write_w_op   = {sc_rand_write_word,            "write_word"};
   SC_OP_t read_w_op    = {sc_rand_read_and_check_word,   "read_word"};
   SC_OP_t atomic_w_op  = {sc_rand_atomic_and_check_word, "atomic_word"};
   SC_OP_t read_qw      = {sc_rand_read_global_and_check_qw,     "read_qw"};
   SC_OP_t write_around = {sc_rand_write_around,                 "write_around"};
   SC_OP_t evict_l2     = {sc_rand_evict_L2, "evict_l2"};
   SC_OP_t nop          = {sc_rand_nop, "nop"};

   // Create a pool of options for this hart
   uint64_t ops_in_pool = 8;
   SC_OP_t ops_pool[8];
   // give hart_id 62 the upper qw
   if (hart_id == 62) {
      uint8_t nn = 0;
      my_addr = (1UL << 39) + (shire_id<<8) + 48;
      ops_pool[nn++] = write_around;
      ops_pool[nn++] = write_around;
      ops_pool[nn++] = write_around;
      ops_pool[nn++] = write_around;
      ops_pool[nn++] = read_qw;
      ops_pool[nn++] = read_qw;
      ops_pool[nn++] = read_qw;
      ops_pool[nn++] = read_qw;
   }
   // give hart_id 52, 54, 56, 58, 60, the next lower words
   else if ((hart_id == 52) || 
            (hart_id == 54) ||
            (hart_id == 56) ||
            (hart_id == 58) ||
            (hart_id == 60)) {
      uint8_t nn = 0; 
      switch (hart_id) {
         case 52:
            my_addr = (1UL << 39) + (shire_id<<8) + 28;
            break;
         case 54:
            my_addr = (1UL << 39) + (shire_id<<8) + 32;
            break;
         case 56:
            my_addr = (1UL << 39) + (shire_id<<8) + 36;
            break;
         case 58:
            my_addr = (1UL << 39) + (shire_id<<8) + 40;
            break;
         case 60:
            my_addr = (1UL << 39) + (shire_id<<8) + 44;
            break;
         }
      ops_pool[nn++] = write_w_op;
      ops_pool[nn++] = write_w_op;
      ops_pool[nn++] = write_w_op;
      ops_pool[nn++] = read_w_op;
      ops_pool[nn++] = atomic_w_op;
      ops_pool[nn++] = atomic_w_op;
      ops_pool[nn++] = atomic_w_op;
      ops_pool[nn++] = atomic_w_op;
   }
   // remaining even harts
   else if ((hart_id & 0x1) == 0) {
      uint8_t nn = 0;
      my_addr = (1UL << 39) + (shire_id<<8) + (hart_id >> 1);
      ops_pool[nn++] = write_b_op;
      ops_pool[nn++] = write_b_op;
      ops_pool[nn++] = write_b_op;
      ops_pool[nn++] = write_b_op;
      ops_pool[nn++] = read_b_op;
      ops_pool[nn++] = read_b_op;
      ops_pool[nn++] = read_b_op;
      ops_pool[nn++] = read_b_op;
   }
   // odd harts
   else {
      uint8_t nn = 0;
      // odd harts don't have a vpu.  No self-checks
      // so don't modify the data. 
      my_addr = (1UL << 39) + (shire_id<<8) + (hart_id >> 1);
      switch (hart_id) {
         case 53:
            my_addr = (1UL << 39) + (shire_id<<8) + 28;
            break;
         case 55:
            my_addr = (1UL << 39) + (shire_id<<8) + 32;
            break;
         case 57:
            my_addr = (1UL << 39) + (shire_id<<8) + 36;
            break;
         case 59:
            my_addr = (1UL << 39) + (shire_id<<8) + 40;
            break;
         case 61:
            my_addr = (1UL << 39) + (shire_id<<8) + 44;
            break;
         }
      ops_pool[nn++] = evict_l2;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
      ops_pool[nn++] = nop;
   }

   // Create a random list from the pool of options
   SC_OP_t ops[OP_LIST];

   uint64_t ii;
   et_printf_long("TEST: Hart: 0x%lx Selected address = 0x%lx\n", hart_id,  my_addr);
   for (ii = 0; ii < OP_LIST; ii++) {
      ops[ii] = ops_pool[et_get_rand_word() % ops_in_pool];
      et_printf_long("TEST: Hart: 0x%lx Op %d %s \n", hart_id, ii, ops[ii].opName);
   }

   // Each core does a different subset in order to sweep the lists across each other
   uint64_t ops_in_list = OP_LIST;
   uint64_t op_ptr = 0;
   //if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0)  ) {
   // Only hart 0 in the core has a vpu. 
   // Odd harts can be used for evictions and such. 
   setM0MaskFF();
   initFRegs_0();
   
   uint64_t fail;
   SC_OP_t  selected;


   // Run the ops
   for (ii = 0; ii < RUN_OPS; ii++) {
      selected = ops[op_ptr++]; 
      // wrap the op_ptr if off the end of the list
      if (op_ptr >= ops_in_list) {
         op_ptr = 0; 
      }
      et_printf_long("TEST: Hart: 0x%lx Op %s address = 0x%lx\n", hart_id, selected.opName, my_addr);
      fail = (*selected.op)(my_addr, (ii & 0xf) | offset);
      if (fail != 0) {
         et_printf_long("TEST: Error: Hart: 0x%lx Failed compare for address 0x%lx\n", hart_id, my_addr);
         pass = false;
      }
   }

   if (pass == false) {
      et_printf_long("TEST: FAILED: hart 0x%lx\n", hart_id);
      C_TEST_FAIL;
   } else {
      et_printf_long("TEST: PASSED: hart 0x%lx\n", hart_id);
      C_TEST_PASS;
   }
   return 0;
}


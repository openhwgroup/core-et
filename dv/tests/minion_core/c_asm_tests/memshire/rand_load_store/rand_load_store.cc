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
#include "esr.h"
#include "shire_cache_rand.h"


#define ESR_SC_REQQ_CTL              0x1

                                                                           

// Preload random data (~1M)
extern unsigned char _binary_blob_bin_start;

__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:          \n\t"
      "       .incbin     \"data.raw\"  \n\t"
      "       .align      4             \n\t"
      );


int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   //uint64_t minion_id  = ((hart_id>>1) & 0x3F);
   uint64_t my_addr;

   uint8_t  shr_cach_sub_region = 0x00;    // Shire_Cache ESR ([19:17]=3'b000)
   uint32_t  esr_addr;              
   uint64_t  esr_wr_data;

   delay(100);

   esr_addr = ESR_SC_REQQ_CTL;
   esr_wr_data =
      (21 << 7) |   // l3 reqq entries
      (1  << 14) |  // l2 bypass
      (0  << 16);   // remote l3 enable
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, 0xf);

   // Hart_id includes minion.
   uint64_t offset = (hart_id<<4);

   // Main memory
   my_addr = (1UL << 39) + (shire_id<<8) + (hart_id<<0);
   if (hart_id == 0 ) {
      // For zero the line
      sc_zero_line_l2(my_addr);
      et_printf_long("TEST: hart 0x%lx zeroed line \n", hart_id);
   }

   SC_OP_t write_op = {sc_rand_write_byte,          "write"};
   SC_OP_t read_op  = {sc_rand_read_and_check_byte, "read"};

   // Create a pool of options for this hart
   uint64_t ops_in_pool = 2;
   SC_OP_t ops_pool[2];
   ops_pool[0] = write_op;
   ops_pool[1] = read_op;

   // Create a random list from the pool of options
   SC_OP_t ops[100];

   uint64_t ii;
   for (ii = 0; ii < 100; ii++) {
      ops[ii] = ops_pool[et_get_rand_word() % ops_in_pool];
   }
   for (ii = 0; ii < 100; ii++) {
      et_printf_long("TEST: Op %d %s \n", ii, ops[ii].opName);
   }

   // Each core does a different subset in order to sweep the lists across each other
   uint64_t ops_in_list = 20+hart_id;
   uint64_t num_ops = 500;
   uint64_t op_ptr = 0;
   //if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0)  ) {
   // Only hart 0 in the core has a vpu. 
   // Odd harts can be used for evictions and such. 
   if ((hart_id & 1) == 0 ) {
      setM0MaskFF();
      initFRegs_0();
      
      uint64_t fail;
      SC_OP_t  selected_op;


      // Run the ops
      for (ii = 0; ii < num_ops; ii++) {
         selected_op = ops[op_ptr++]; 
         // wrap the op_ptr if off the end of the list
         if (op_ptr >= ops_in_list) {
            op_ptr = 0; 
         }
         et_printf_long("TEST: Op %s address = 0x%lx\n", selected_op.opName, my_addr);
         fail = (*selected_op.op)(my_addr, ii | offset);
         if (fail != 0) {
            et_printf_long("TEST: Error: Failed compare for address 0x%lx\n", my_addr);
            pass = false;
         }
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


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Directed test that exercices the basic functionality
//            of the Esperanto tensor cooperative store feature 
// Note: 
//      - These tests focuses on operations that which addresses fall into the same cache line
//      - Machine trap handler is configured in this test in such way that the test would branch to DEAD, should 
//      an exception occur.  
//      - Message size is set to DWORD and always contain the sender minion id.
// 

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"

// Macros for setting the minion mask for the coop loads
#define SET_N_BITS(n) ((1ULL << (n)) - 1ULL)
#define SET_N_K_BITS(n, k) ((SET_N_BITS(n) >> k) << k)
#define NEIGH0_IPI_REDIRECT_PC 0x80100040
#define TENSOR_COOP_WAY  0x4UL

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;

__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
   );

void common_params(uint64_t addr, uint64_t ncoop_minions, uint64_t row_size) {
   uint64_t reg_stride    = 0;
   uint64_t start_reg     = 0;
   uint64_t nrows         = 0;
   uint64_t stride        = 0x0;
   tensor_store(reg_stride, start_reg, row_size, nrows, addr, ncoop_minions, stride);
}

void oneway_coop(uint64_t mid, uint64_t cl_align_addr, uint64_t row_size) {
   //compute address where to store
   uint64_t addr          = cl_align_addr + (mid * (0x10 * (row_size + 1)));
   uint64_t ncoop_minions = 0;
   common_params(addr, ncoop_minions, row_size);
}

void mixed_oneway_coop(uint64_t mid, uint64_t cl_align_addr) {
      // Mixing one-way operations with multiple sizes
      // 1x32 Bytes + 2x16 Bytes
      uint64_t row_size;
      cl_align_addr = cl_align_addr + ((mid / 3) * 0x40);
      if ((mid % 3) == 0) {
         row_size = 1; //32 Bytes
      }
      else {
         row_size = 0; //16 Bytes
         cl_align_addr = cl_align_addr + 0x10;
      }
      oneway_coop((mid%3), cl_align_addr, row_size);
}

void twoway_coop(uint64_t mid, uint64_t cl_align_addr, uint64_t row_size, bool mask_addr) {
   //compute address where to store
   uint64_t addr = cl_align_addr + (mid * (0x10 * (row_size + 1)));
   if (mask_addr) { 
      // The spec says that lower bits will be ignored
      // 'mask_addr' controls whether set the lower bits to 0 or not
      addr = addr & 0xffffffffffffffe0;
   }
   // Pair-128 cooperation mode
   uint64_t ncoop_minions = 1; // 2 Minions
   // Starting at an even mid (minion id)
   // addr[4:0] bits are ignored internally
   common_params(addr, ncoop_minions, row_size);
}

void mixed_twoway_oneway(uint64_t mid, uint64_t cl_align_addr, uint64_t row_size_1way, bool mask_addr) {

   // Number of minions touching same cache line
   uint64_t n_minions_cl = 4;
   cl_align_addr = cl_align_addr + (0x40 * (mid/n_minions_cl));
   mid = mid % n_minions_cl;
   // The 4th minion is doing nothing if the row_size_1way is 1 (256 bits)
   if ((row_size_1way == 1) && (mid != 3)) {
      if ((mid == 0) || (mid == 1)) {
         twoway_coop(mid, cl_align_addr, 0, mask_addr);
      }
      else {
         mid = mid - 2;
         uint64_t addr = cl_align_addr + (2 * 0x10);
         oneway_coop(mid, addr, row_size_1way);
      }
   }
}

void fourway_coop(uint64_t mid, uint64_t cl_align_addr, bool wrong_addr) {
   //compute address where to store
   uint64_t ncoop_minions = 3; // 4 Minions
   uint64_t row_size      = 0; // 16 Bytes
   uint64_t addr          = cl_align_addr + (mid * (0x10 * (row_size + 1)));

   if (wrong_addr && ((mid != 0) || (mid != 4))) {
      addr = addr + 0x80;
   }

   common_params(addr, ncoop_minions, row_size);
}


volatile uint64_t rand_init __attribute__((section(".rand_init_array"))) ;
      
volatile uint64_t addr __attribute__((section(".address_array")));

int main()
{
   uint64_t mid, tid, sid;
   bool bad_addr;
   
   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   volatile uint64_t *esr_poll_value;

   // Wait for the debugger to write this ESR
   esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
   while ( *esr_poll_value != 0xFE );

   // Init marker
   C_TEST_START;
   
   // Setup machine trap vector
   // Normally we do not expect any interrut
   setup_mtrap_vector();
   
   //1.0 Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();
   sid = get_shire_id();

   enable_shire_coop(sid);

   //***************************************************************//
   // This represents a cache line. Each cell is the 'row_size'.    //
   // the 'row_size' can be 0 (16Bytes), 1 (32Bytes) or 3 (64Bytes) //
   // In th following example the 'row_size' is 0, which is 16Bytes //
   //                                                               //
   //  -----------------------------                                //
   //  |      |      |      |      |                                //
   //  -----------------------------                                //
   //                                                               //
   // Minions ID are M[0-7].                                        //
   //                                                               //
   // For representing the cooperation between minions we use:      //
   //    * Non-coop:      <MX>                                      //
   //    * Two-way coop:  <MX | MY>                                 //
   //    * Four-way coop: <MX | MY | MZ | MK>                       //
   //***************************************************************//
   bool mask_addr;
   uint64_t row_size;
   if (tid == 0) {
      rand_init = et_get_rand_word(0,1024);
      addr = (uint64_t)(void*)buffer[rand_init].data[0];
      // load some VPU registers with FLQ instructions from the first array
      init_fp_regs((void*) addr);
      FENCE;

      // 1-Way
      // -----------------------------
      // | <M0> | <M1> | <M2> | <M3> |
      // -----------------------------
      row_size = 0; //16 Bytes
      oneway_coop(mid, addr, row_size);

      // -----------------------------
      // |     <M0>    | <M2> | <M3> |
      // -----------------------------
      // Mix 32 Bytes and 16 Bytes
      mixed_oneway_coop(mid, addr);
      
      // -----------------------------
      // |     <M0>    |     <M1>    |
      // -----------------------------
      row_size = 1; //32 Bytes
      oneway_coop(mid, addr, row_size);

      // -----------------------------
      // |           <M0>            |
      // -----------------------------
      row_size = 3; //64 Bytes
      oneway_coop(mid, addr, row_size);

      // 2-Way
      // -----------------------------
      // | <M0  |  M1> | <M2  |  M3> |
      // -----------------------------
      row_size  = 0;
      mask_addr = 0;
      twoway_coop(mid, addr, row_size, mask_addr);

      // -----------------------------
      // |     <M0     |      M1>    |
      // -----------------------------
      row_size  = 1;
      mask_addr = 0;
      twoway_coop(mid, addr, row_size, mask_addr);

      // -----------------------------
      // | <M0  |  M1> | <M2> | <M3> |
      // -----------------------------
      row_size  = 0;
      mask_addr = 0;
      mixed_twoway_oneway(mid, addr, row_size, mask_addr);

      // -----------------------------
      // | <M0  |  M1> |     <M2>    |
      // -----------------------------
      row_size  = 1;
      mask_addr = 0;
      mixed_twoway_oneway(mid, addr, row_size, mask_addr);

      // 4-way
      // -----------------------------
      // | <M0  |  M1  |  M2  |  M3> |
      // -----------------------------
      bad_addr = false;
      for (int i = 0; i < 5; i++) {
          fourway_coop(mid, addr, bad_addr);
      }

      // -----------------------------
      // | <M0  |  M1  |  M2  |  M3> |
      // -----------------------------
      buffer = (cl_data_t*) 0x0080080D00;
      addr = (uint64_t)(void*)buffer[0].data[0];
      bad_addr = false;
      for (int i = 0; i < 5; i++) {
          fourway_coop(mid, addr, bad_addr);
      }

      tensor_error_check();
   }
   C_TEST_PASS;
   return 0;
}



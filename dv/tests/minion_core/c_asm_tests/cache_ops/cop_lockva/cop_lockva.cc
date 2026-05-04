/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "cacheops.h"


int main()
{
   volatile char __attribute__((aligned(4096))) ptr[4096];
   // Init marker
   C_TEST_START;

   // Only thread 0 executes this
   if (get_hart_id() == 0) {

      //      use_tmask  addr                     num_lines  stride  id
      //      ---------  -----------------------  ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr),  15,        64,     0 );
      WAIT_CACHEOPS;

      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(256)),  15,        64,     0 );
      WAIT_CACHEOPS;

      //      use_tmask  addr                           num_lines  stride  id
      //      ---------  ---------------------------    ---------  ------  --
      lock_va(false,     (uint64_t)(void*)(ptr+(512)),  15,        64,     0 );
      WAIT_CACHEOPS;

      // Initialize array 
      for (int i = 0; i < 1024; ++i) {
         ptr[i] = i; 
      }
      FENCE;

      // Evict whole cache (CHECK WHAT SETs and WAYs WILL BE EVICTED)
      for (unsigned way = 0; way < 4; way++) {
         evict_sw(false,  to_L2, way, 0x0,   15       );
      }
      WAIT_CACHEOPS;

      /*
      // Test
      __asm__ __volatile__ (
      // Init and set all cachelines to modified
      "addi t0, %[ptr], 0\n"
      "addi t1, zero, 0\n"        // Start value of matrix
      "addi t2, zero, 1024\n"      // Loop iterations
      "loop_sw:\n"
      "sw   t1, 0(t0)\n"          // Store
      "addi t1, t1, 1\n"          // Inc value
      "addi t0, t0, 4\n"          // Inc ptr
      "addi t2, t2, -1\n"         // Dec counter
      "bne  t2, zero, loop_sw\n"

      // Lock VA and then try to evict some of them 
      "addi t0, zero, 0\n"
      "slli t0, t0, 60\n"
      "addi t1, zero, 0\n"		  // Lock Way0
      "slli t1, t1, 48\n"
      "or   t0, t0, t1\n"
      //        "addi t1, %[ptr], 0\n"	  // Virtual address
      //        "slli t1, t1, 6\n"
      "or   t0, t0, %[ptr]\n"
      "addi t0, t0, 15\n"		  // NumLines up to 16
      "csrw 0x51f, t0\n"			  // LockVa 
      "add x31, zero, 64\n"

      "fence\n"

      // Now try to evictVA same lines?
      "addi t0, zero, 7\n"       // EvictVA. Should this evict data from L1 to L2? 
      "slli t0, t0, 60\n"
      "addi t1, zero, 1\n"		  // At this moment only works for start_level=L1 destination_level= L2
      "slli t1, t1, 58\n"
      "or   t0, t0, t1\n"
      //        "addi t1, %[ptr], 0\n"	  // Virtual address
      //        "slli t1, t1, 6\n"
      "or   t0, t0, %[ptr]\n"
      "addi t0, t0, 15\n"		  // NumLines up to 16 
      "csrw 0x51f, t0\n"
      "add x31, zero, 64\n"

      "fence\n"
      */
      /* 
         "addi x31, zero, 64\n"    //LockVA 
         "ori  t0, %[ptr], 15\n"
         "csrw 0x51f, t0\n"

         "fence\n"		*/

      /*
      // Read all cachelines again.
      "addi t0, %[ptr], 0\n"
      "addi t2, zero, 1024\n"
      "addi t3, zero, 0\n"        // self check vector
      "loop_lw:\n"
      "lw   t1, 0(t0)\n"          // Load
      "sub  t4, t1, t3\n"
      "bne  t4, zero, fail\n"
      "addi t1, t1, 1\n"          // Inc value
      "addi t3, t3, 1\n"          // Inc self check vector
      "addi t0, t0, 4\n"          // Inc ptr
      "addi t2, t2, -1\n"         // Dec counter
      "bne  t2, zero, loop_lw\n"
      "j pass\n"
      "fail:\n"
      "slli zero, zero, 1\n"
      "pass:\n"	*/
      /*
         :
         : [ptr] "r" (ptr)
         : "t0", "t1", "t2", "t3", "t4"
         );
         */
   }

   // End marker
   C_TEST_PASS;
   return 0;
}

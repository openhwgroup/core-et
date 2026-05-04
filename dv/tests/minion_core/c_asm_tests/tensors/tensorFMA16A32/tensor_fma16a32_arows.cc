/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[256];
   // Init marker
   C_TEST_START;


   // Only thread 0 executes this
   if (get_thread_id() == 0) {

      __asm__ __volatile__ (
            // Init and set all cachelines to modified
            "addi t0, %[ptr], 0\n"
            "li t1, 0x3C00\n"        // Start value of matrix
            "addi t2, zero, 256\n"      // Loop iterations
            "loop_sw:\n"
            "sw   t1, 0(t0)\n"          // Store
            "addi t1, t1, 1\n"          // Inc value
            "addi t0, t0, 4\n"          // Inc ptr
            "addi t2, t2, -1\n"         // Dec counter
            "bne  t2, zero, loop_sw\n"

            "fence\n"

            "li x11, 0xff\n"
            "mova.m.x x11\n"

            "fence\n"
            :
            : [ptr] "r" (ptr)
            : "t0", "t1", "t2", "x11"
               );

      FENCE;

      // Evict L1 lines
      for (unsigned way = 0; way < 4; ++way) {
         //       use_tmask  dst  way  set  num_lines
         evict_sw(false,     0x1, way, 0,   15       );
      }

      // Tensor Wait 0
      __asm__ __volatile__ ( "csrsi 0x830, 0\n" : : );


      // Enable Scratchpad
      __asm__ __volatile__ ( "csrsi 0x810, 1\n" : : );

      //          use_tmask  use_coop  transformation  dst_start  scp_sel   addr                   offset  num_lines  stride   id
      //          ---------  --------  --------------  ---------  --------  --------------------   ------  ---------  ------   --
      tensor_load(false,     false,    0,              0,         0,        (uint64_t)(void*)ptr,  0,      15,        64,      0 );

      // Tensor Wait 0
      __asm__ __volatile__ ( "csrw 0x830, x0\n" : : );


      for (unsigned arows=0; arows < 8; arows++) {
         //         use_tmask  b_num_col  a_num_rows  a_num_cols  offset  tenc_loc  tenb_unsigned tena_unsigned tenb_loc  scp_loc_b  scp_loc_a  opcode  first pass
         //         ---------  ---------  ----------  ----------  ------  ---------  ------------ ------------- -------   ---------  ---------  ------  ----------
	 tensor_fma(false,     3,         arows,          3,          0,      0,         0,           0,           0,          1,         0,         1,       true);

         FENCE;
      }

   }

   C_TEST_PASS;
   return 0;
}

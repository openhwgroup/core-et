/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef MINSHIRE_GATESIM_ENABLE
#include "et_test_common.h"
#define TIMEOUT 2000
#endif

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"

int main()
{
   //volatile char __attribute__((aligned(4096))) ptr[4096];
   volatile int __attribute__((aligned(4096))) ptr[1024];
   // Init marker
   C_TEST_START;

   #ifdef MINSHIRE_GATESIM_ENABLE
     volatile uint32_t hart_id   = get_hart_id();    
     volatile uint32_t thread_id = (hart_id & 1);
     volatile uint32_t minion_id = ((hart_id>>1) & 0x1F);      
     volatile uint32_t shire_id  = ((hart_id>>6) & 0x3F);
     volatile uint32_t timeout   = TIMEOUT;

     if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
       s32_scp_init();
       give_credit(0, -1, -1, 1);
       give_credit(32, -1, -1, 1);
     }
   
     wait_for_credit(1);  

     if (shire_id == 0) {
   #endif 
   // Only thread 0 executes this
   if (get_thread_id() == 0) {

      __asm__ __volatile__ (
            // Init and set all cachelines to modified
            "addi t0, %[ptr], 0\n"
            "li t1, 0x5\n"        // Start value of matrix
            "addi t2, zero, 1024\n"      // Loop iterations
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

      WAIT_CACHEOPS;


      // Enable Scratchpad
      setup_cache_scp();

      //          use_tmask  use_coop  transformation  dst_start  scp_sel   addr                   offset  num_lines  stride   id
      //          ---------  --------  --------------  ---------  --------  --------------------   ------  ---------  ------   --
      tensor_load(false,     false,    0,              0,         0,        (uint64_t)(void*)ptr,  0,      15,        64,      0 );

      // Tensor Wait 0
      WAIT_TENSOR_LOAD_0;



      //         use_tmask  b_num_col  a_num_rows  a_num_cols  offset  tenc_loc tenb_unsigned  tena_unsigned  tenb_loc scp_loc_b  scp_loc_a  opcode  first pass
      //         ---------  ---------  ----------  ----------  ------  -------- -------------  -------------  -------- ---------  ---------  ------  ----------
      tensor_fma(false,     3,         7,          7,          0,      0,             0,             0,           0,       0,         0,         3,      true);

      WAIT_TENSOR_FMA;

   }

   #ifdef MINSHIRE_GATESIM_ENABLE

     }

     hart_id   = get_hart_id();    
     thread_id = (hart_id & 1);
     minion_id = ((hart_id>>1) & 0x1F); 
     shire_id  = ((hart_id>>6) & 0x3F);

     if (shire_id == 0 and thread_id == 0) {
         atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
     }

     if (shire_id == 32 and thread_id == 0) {
         check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
     }
   #endif 

   C_TEST_PASS;
   return 0;
}

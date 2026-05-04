/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"



#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

int main()
{

   static uint32_t operands[32]  __attribute__ ((aligned (32)))= {
      0x00000000,           
      0x00000001,          
      0x00000002,           
      0x00000003,           
      0x00000004,
      0x00000005,
      0x00000006,
      0x00000007,
      0x00000008,                 
      0x00000009,                
      0x0000000a,                   
      0x0000000b,                 
      0x0000000c,
      0x0000000d,
      0x0000000e,
      0x0000000f,
      0x00000010,                
      0x00000012,                 
      0x00000013,                
      0x00000014,   
      0x00000015,
      0x00000016,
      0x00000017,
      0x00000018,
      0x00000019,
      0x0000001a,
      0x0000001b,
      0x0000001c,
      0x0000001d,
      0x0000001e,
      0x0000001f,
      0x00000020 
   };

   static uint64_t err_cnt[1] __attribute__ ((aligned (64))) = {0};


   //volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;


   // (CYE) only run the following code in ONE Hart (otherwise self-check would fail):
   
   //uint64_t shire_id = get_shire_id();
   //uint64_t neigh_id = get_neigh_id();
   //uint64_t minion_id = get_minion_id();
   //uint64_t thread_id = get_thread_id();
   uint64_t hart_id = get_hart_id();

   // note: updated in order to run on any Minion (decided by +MINION_MASK=xxx setting in "minion_diags.py"):
   if ((hart_id % 2) == 0) {

      #ifdef POSTSI_UST
        // need to initialize the 2 cachline address (%[operands]) in L3$ (by flush from L1->L3):
        // flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl) 
        flush_va(0, 2, (uint64_t)operands, 1, 64, 0, 0);
        WAIT_CACHEOPS; 
      #endif

      for (int i=1; i<10; i++) {  
         //int r = 999*i + 34*i;

         // Test
         __asm__ __volatile__ (

            // (CYE) reg t0 is used for address of %[operands]; t1 as a random number;
            //       reg t4 is used as atomic new source data; reg t2 as atomic returned data (orignal value);
            //       reg t3 is used to save expected value of atomic result (in L2 or L3);  
            //       reg s11 is used as error_count; reg s10 is used as a mask 0xffffffff (const);         

            "addi t0, %[operands], 0\n"
            "li   t1, 0x6831732103AC3FDE\n"
            "li   t6, 0x1\n"
            "li   s11, 0\n"     // error_count
            "fence \n"

            "call amoswapgW_global_scalar_test%=\n"
            "call amoswapgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOSWAPG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoandgW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOANDG_W returning data to self-check AMOSWAPG_W result
            "call amoandgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOANDG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoorgW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOORG_W returning data to self-check AMOANDG_W result
            "call amoorgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOORG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoxorgW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOXORG_W returning data to self-check AMOORG_W result
            "call amoxorgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOXORG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoaddgW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOADDG_W returning data to self-check AMOXORG_W result 
            "call amoaddgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOADDG_W expected result 
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomingW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOMING_W returning data to self-check AMOADDG_W result 
            "call amomingW_global_scalar_expected_result%=\n"  // (CYE) generate AMOMING_W expected result 
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxgW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOMAXG_W returning data to self-check AMOMING_W result 
            "call amomaxgW_global_scalar_expected_result%=\n"  // (CYE) generate AMOMAXG_W expected result 
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominugW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOMINUG_W returning data to self-check AMOMAXG_W result 
            "call amominugW_global_scalar_expected_result%=\n"  // (CYE) generate AMOMINUG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxugW_global_scalar_test%=\n"
            "call comm_amo_gW_global_scalar_selfcheck%=\n" // (CYE) use AMOMAXUG_W returning data to self-check AMOMINUG_W result
            "call amomaxugW_global_scalar_expected_result%=\n"  // (CYE) generate AMOMAXUG_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            // (CYE) do an extra AMOORG_W(0) to self-check AMOMAXUG_W result
            "call amomaxugW_global_scalar_selfcheck%=\n" 

            "call end%=\n"



            // AMOSWAPG.W
            "amoswapgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoswapg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOSWAPG_W expected result in L3 address (t0): to be used by self-check later
            "amoswapgW_global_scalar_expected_result%=:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOANDG.W
            "amoandgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoandg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOANDG_W expected result in L3 address (t0): to be used by self-check later
            "amoandgW_global_scalar_expected_result%=:\n"
            "and t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOORG.W
            "amoorgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoorg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOORG_W expected result in L3 address (t0): to be used by self-check later
            "amoorgW_global_scalar_expected_result%=:\n"
            "or t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOXORG.W
            "amoxorgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoxorg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOXORG_W expected result in L3 address (t0): to be used by self-check later
            "amoxorgW_global_scalar_expected_result%=:\n"
            "xor t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOADDG.W
            "amoaddgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoaddg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOADDG_W expected result in L3 address (t0): to be used by self-check later
            "amoaddgW_global_scalar_expected_result%=:\n"
            "add t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOMING.W
            "amomingW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoming.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMING_W expected result in L3 address (t0): to be used by self-check later
            "amomingW_global_scalar_expected_result%=:\n"
            "li s10, 0xffffffff\n"
            "and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            "and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "ble t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMAXG.W
            "amomaxgW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxg.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMAXG_W expected result in L3 address (t0): to be used by self-check later
            "amomaxgW_global_scalar_expected_result%=:\n"
            "li s10, 0xffffffff\n"
            "and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            "and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bgt t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMINUG.W
            "amominugW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominug.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMINUG_W expected result in L3 address (t0): to be used by self-check later;
            "amominugW_global_scalar_expected_result%=:\n"
            "li s10, 0xffffffff\n"
            "and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            "and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bleu t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMAXUG.W
            "amomaxugW_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxug.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMAXUG_W expected result in L3 address (t0): to be used by self-check later;
            "amomaxugW_global_scalar_expected_result%=:\n"
            "li s10, 0xffffffff\n"
            "and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            "and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bgtu t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // (CYE) self-check result of AMOMAXUG_W (above): do an extra AMOORG_W(0) returning data (t2):
            "amomaxugW_global_scalar_selfcheck%=:\n"
            "amoorg.w t2, zero, (t0)\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"


            // (CYE) common/generic self-check result of previous atomic: use a subsequent atomic returning data (in reg t2):
            "comm_amo_gW_global_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"


            "delay%=: \n"
            "li t5, 32\n"
            "loop_delay%=: \n"
            "add t5, t5, -1\n"
            "bne t5, zero, loop_delay%=\n"
            "ret\n"

            "end%=: \n"
            "nop\n"
            :
            : [operands] "r" (operands) //, "n" (r)
            : "t0", "t1", "t2", "t3", "t4", "t5", "t6", "s11"
         );  // end of __asm__ __volatile__ 


         // check s11 -> err_cnt:
         __asm__ __volatile__ (
            //"bgtz s11, L2\n"
            //"j L3\n"
            //"L2:\n"
            "sw s11, 0(%[err_cnt])\n"
            //"L3:\n"
            :
            : [err_cnt] "r" (err_cnt)
            : "s11", "memory"
         );

         if (err_cnt[0] > 0) {
            et_printf("Test self-check: err_cnt > 0\n"); 
            C_TEST_FAIL;
         }
         //else {
         //   et_printf("Test self-check: no_err\n"); 
         //}

      } // for()

   } // if (hart_id == 0)

   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
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

   // (CYE) only run the following code in hart_id==0 (in order to do self-check):
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
            "li   s11, 0\n"    // (CYE) s11 = error_count
            "fence \n"

            "call amoswapgD_global_scalar_test%=\n"
            "call amoswapgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOSWAPG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoandgD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n"  // (CYE) use AMOANDG_D returning data to self-check AMOSWAPG_D result
            "call amoandgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOANDG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoorgD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOORG_D returning data to self-check AMOANDG_D result
            "call amoorgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOORG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoxorgD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOXORG_D returning data to self-check AMOORG_D result
            "call amoxorgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOXORG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoaddgD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOADDG_D returning data to self-check AMOXORG_D result
            "call amoaddgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOADDG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomingD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOMING_D returning data to self-check AMOADDG_D result
            "call amomingD_global_scalar_expected_result%=\n"  // (CYE) generate AMOMING_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxgD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOMAXG_D returning data to self-check AMOMING_D result 
            "call amomaxgD_global_scalar_expected_result%=\n"  // (CYE) generate AMOMAXG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominugD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOMING_D returning data to self-check AMOMAXG_D result 
            "call amominugD_global_scalar_expected_result%=\n" // (CYE) generate AMOMINUG_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxugD_global_scalar_test%=\n"
            "call comm_amo_gD_global_scalar_selfcheck%=\n" // (CYE) use AMOMAXUG_D returning data to self-check AMOMINUG_D result
            "call amomaxugD_global_scalar_expected_result%=\n"
            "call delay%=\n"
            "addi t6, t6, 1\n"

            // (CYE) do an extra AMOORG_D(0) to self-check AMOMAXUG_D result
            "call amomaxugD_global_scalar_selfcheck%=\n" 

            "call end%=\n"


            // AMOSWAPG.D
            "amoswapgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoswapg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOSWAPG_D expected result in L3 address (t0): to be used by self-check later)
            "amoswapgD_global_scalar_expected_result%=:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOANDG.D
            "amoandgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoandg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOANDG_D expected result in L3 address (t0): to be used by self-check later)
            "amoandgD_global_scalar_expected_result%=:\n"
            "and t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOORG.D
            "amoorgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoorg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOORG_D expected result in L3 address (t0): to be used by self-check later)
            "amoorgD_global_scalar_expected_result%=:\n"
            "or t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOXORG.D
            "amoxorgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoxorg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOXORG_D expected result in L3 address (t0): to be used by self-check later)
            "amoxorgD_global_scalar_expected_result%=:\n"
            "xor t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOADDG.D
            "amoaddgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoaddg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOADDG_D expected result in L3 address (t0): to be used by self-check later)
            "amoaddgD_global_scalar_expected_result%=:\n"
            "add t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOMING.D
            "amomingD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoming.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMING_D expected result in L3 address (t0): to be used by self-check later)
            "amomingD_global_scalar_expected_result%=:\n"
            //"li s10, 0xffffffff\n"
            //"and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            //"and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "ble t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMAXG.D
            "amomaxgD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxg.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMAXG_D expected result in L3 address (t0): to be used by self-check later)
            "amomaxgD_global_scalar_expected_result%=:\n"
            //"li s10, 0xffffffff\n"
            //"and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            //"and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bgtu t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMINUG.D
            "amominugD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominug.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMINUG_D expected result in L3 address (t0): to be used by self-check later;
            "amominugD_global_scalar_expected_result%=:\n"
            //"li s10, 0xffffffff\n"
            //"and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            //"and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bleu t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // AMOMAXUG.D
            "amomaxugD_global_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxug.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOMAXUG_D expected result in L3 address (t0): to be used by self-check later;
            "amomaxugD_global_scalar_expected_result%=:\n"
            //"li s10, 0xffffffff\n"
            //"and t4, t4, s10\n"    // only acquire lower 32 bits (1-word)
            //"and t2, t2, s10\n"    // only acquire lower 32 bits (1-word)
            "bgtu t4, t2, 1f\n" 
            "mv t3, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "j 2f\n"
            "1:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "2:\n"
            "nop\n"
            "ret\n"


            // (CYE) self-check result of AMOMAXUG_D (above): do an extra AMOORG_D(0) returning data (t2):
            "amomaxugD_global_scalar_selfcheck%=:\n"
            "amoorg.d t2, zero, (t0)\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"


            // (CYE) common/generic self-check result of preivous atomic: use a subsequent atomic returning data (t2):
            "comm_amo_gD_global_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
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
            : [operands] "r" (operands)//, "n" (r)
            : "t0", "t1", "t2", "t3", "t4", "t5", "t6", "s11"
      );


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

   } // for(int i=1 ...)

   } // if (hart_id == 0)

   C_TEST_PASS;
   return 0;
}

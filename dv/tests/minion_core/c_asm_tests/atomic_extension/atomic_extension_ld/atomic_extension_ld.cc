/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "minion.h"
#include "et_test_common.h"


#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

#define ERROR_COUNT s11


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

   if ((hart_id % 2) == 0) {

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
            "li   s11, 0\n"    // s11 = error_count
            "fence \n"

            "call amoswaplD_local_scalar_test%=\n"
            "call amoswaplD_local_scalar_expected_result%=\n"  // generate AMOSWAPL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoandlD_local_scalar_test%=\n"
            //"call amoswaplD_local_scalar_selfcheck%=\n"  // (CYE) use AMOANDL_D returning data to self-check AMOSWAPL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n"  // (CYE) use AMOANDL_D returning data to self-check AMOSWAPL_D result
            "call amoandlD_local_scalar_expected_result%=\n"  // generate AMOANDL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoorlD_local_scalar_test%=\n"
            //"call amoandlD_local_scalar_selfcheck%=\n" // (CYE) use AMOORL_D returning data to self-check AMOANDL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOORL_D returning data to self-check AMOANDL_D result
            "call amoorlD_local_scalar_expected_result%=\n"  // generate AMOORL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoxorlD_local_scalar_test%=\n"
            //"call amoorlD_local_scalar_selfcheck%=\n" // (CYE) use AMOXORL_D returning data to self-check AMOORL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOXORL_D returning data to self-check AMOORL_D result
            "call amoxorlD_local_scalar_expected_result%=\n"  // generate AMOXORL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoaddlD_local_scalar_test%=\n"
            //"call amoxorlD_local_scalar_selfcheck%=\n" // (CYE) use AMOADDL_D returning data to self-check AMOXORL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOADDL_D returning data to self-check AMOXORL_D result
            "call amoaddlD_local_scalar_expected_result%=\n"  // generate AMOADDL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominlD_local_scalar_test%=\n"
            //"call amoaddlD_local_scalar_selfcheck%=\n" // (CYE) use AMOMINL_D returning data to self-check AMOADDL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOMINL_D returning data to self-check AMOADDL_D result
            "call amominlD_local_scalar_expected_result%=\n"  // generate AMOMINL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxlD_local_scalar_test%=\n"
            //"call amominlD_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXL_D returning data to self-check AMOMINL_D result 
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXL_D returning data to self-check AMOMINL_D result 
            "call amomaxlD_local_scalar_expected_result%=\n"  // generate AMOMAXL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominulD_local_scalar_test%=\n"
            //"call amomaxlD_local_scalar_selfcheck%=\n"  // (CYE) use AMOMINUL_D returning data to self-check AMOMAXL_D result   
            "call comm_amo_lD_local_scalar_selfcheck%=\n"  // (CYE) use AMOMINUL_D returning data to self-check AMOMAXL_D result 
            "call amominulD_local_scalar_expected_result%=\n" // generate AMOMINUL_D expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxulD_local_scalar_test%=\n"
            //"call amominulD_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXUL_D returning data to self-check AMOMINUL_D result
            "call comm_amo_lD_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXUL_D returning data to self-check AMOMINUL_D result
            "call amomaxulD_local_scalar_expected_result%=\n"
            "call delay%=\n"
            "addi t6, t6, 1\n"

            // (CYE) do an extra AMOORL_D(0) to self-check AMOMAXUL_D result
            "call amomaxulD_local_scalar_selfcheck%=\n" 

            "call end%=\n"


            // AMOSWAPL.D
            "amoswaplD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoswapl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOSWAPL_D expected result in L2 address (t0): to be used by self-check later)
            "amoswaplD_local_scalar_expected_result%=:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOANDL.D
            "amoandlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoandl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOSWAPL_D (above): use AMOANDL_D returning data (t2):
            "amoswaplD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOANDL_D expected result in L2 address (t0): to be used by self-check later)
            "amoandlD_local_scalar_expected_result%=:\n"
            "and t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOORL.D
            "amoorlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoorl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOANDL_D (above): use AMOORL_D returning data (t2):
            "amoandlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOORL_D expected result in L2 address (t0): to be used by self-check later)
            "amoorlD_local_scalar_expected_result%=:\n"
            "or t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOXORL.D
            "amoxorlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoxorl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOORL_D (above): use AMOXORL_D returning data (t2):
            "amoorlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOXORL_D expected result in L2 address (t0): to be used by self-check later)
            "amoxorlD_local_scalar_expected_result%=:\n"
            "xor t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOADDL.D
            "amoaddlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoaddl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOXORL_D (above): use AMOADDL_D returning data (t2):
            "amoxorlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOADDL_D expected result in L2 address (t0): to be used by self-check later)
            "amoaddlD_local_scalar_expected_result%=:\n"
            "add t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOMINL.D
            "amominlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOADDL_D (above): use AMOMINL_D returning data (t2):
            "amoaddlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMINL_D expected result in L2 address (t0): to be used by self-check later)
            "amominlD_local_scalar_expected_result%=:\n"
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


            // AMOMAXL.D
            "amomaxlD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxl.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMINL_D (above): use AMOMAXL_D returning data (t2):
            "amominlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMAXL_D expected result in L2 address (t0): to be used by self-check later)
            "amomaxlD_local_scalar_expected_result%=:\n"
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


            // AMOMINUL.D
            "amominulD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominul.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMAXL_D (above): use AMOMINUL_D returning data (t2):
            "amomaxlD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMINUL_D expected result in L2 address (t0): to be used by self-check later;
            "amominulD_local_scalar_expected_result%=:\n"
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


            // AMOMAXUL.D
            "amomaxulD_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxul.d t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMINUL_D (above): use AMOMAXUL_D returning data (t2):
            "amominulD_local_scalar_selfcheck%=:\n"
            //"li s10, 0xffffffff\n" 
            //"and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMAXUL_D expected result in L2 address (t0): to be used by self-check later;
            "amomaxulD_local_scalar_expected_result%=:\n"
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


            // (CYE) self-check result of AMOMAXUL_D (above): do an extra AMOORL_D(0) returning data (t2):
            "amomaxulD_local_scalar_selfcheck%=:\n"
            "amoorl.d t2, zero, (t0)\n"
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
            "comm_amo_lD_local_scalar_selfcheck%=:\n"
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

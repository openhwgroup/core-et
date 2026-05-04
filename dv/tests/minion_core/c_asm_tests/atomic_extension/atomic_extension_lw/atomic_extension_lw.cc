/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "minion.h"
#include "et_test_common.h"


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

   //int  err_cnt = 0;

   // (CYE) only run the following code in hart_id==0 (in order to do self-check):
   uint64_t hart_id = get_hart_id();

   if ((hart_id %2) == 0) {   // change to use % modulo: easy to port to Minion #1, #2, #3, etc.

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
            "li   s11, 0\n"
            "fence \n"

            "call amoswaplW_local_scalar_test%=\n"
            "call amoswaplW_local_scalar_expected_result%=\n"  // generate AMOSWAPL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoandlW_local_scalar_test%=\n"
            //"call amoswaplW_local_scalar_selfcheck%=\n" // (CYE) use AMOANDL_W returning data to self-check AMOSWAPL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOANDL_W returning data to self-check AMOSWAPL_W result
            "call amoandlW_local_scalar_expected_result%=\n"  // generate AMOANDL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoorlW_local_scalar_test%=\n"
            //"call amoandlW_local_scalar_selfcheck%=\n" // (CYE) use AMOORL_W returning data to self-check AMOANDL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOORL_W returning data to self-check AMOANDL_W result
            "call amoorlW_local_scalar_expected_result%=\n"  // generate AMOORL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoxorlW_local_scalar_test%=\n"
            //"call amoorlW_local_scalar_selfcheck%=\n" // (CYE) use AMOXORL_W returning data to self-check AMOORL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOXORL_W returning data to self-check AMOORL_W result
            "call amoxorlW_local_scalar_expected_result%=\n"  // generate AMOXORL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amoaddlW_local_scalar_test%=\n"
            //"call amoxorlW_local_scalar_selfcheck%=\n" // (CYE) use AMOADDL_W returning data to self-check AMOXORL_W result 
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOADDL_W returning data to self-check AMOXORL_W result 
            "call amoaddlW_local_scalar_expected_result%=\n"  // generate AMOADDL_W expected result 
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominlW_local_scalar_test%=\n"
            //"call amoaddlW_local_scalar_selfcheck%=\n" // (CYE) use AMOMINL_W returning data to self-check AMOADDL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOMINL_W returning data to self-check AMOADDL_W result
            "call amominlW_local_scalar_expected_result%=\n"  // generate AMOMINL_W expected result 
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxlW_local_scalar_test%=\n"
            //"call amominlW_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXL_W returning data to self-check AMOMINL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXL_W returning data to self-check AMOMINL_W result
            "call amomaxlW_local_scalar_expected_result%=\n"  // generate AMOMAXL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amominulW_local_scalar_test%=\n"
            //"call amomaxlW_local_scalar_selfcheck%=\n" // (CYE) use AMOMINUL_W returning data to self-check AMOMAXL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOMINUL_W returning data to self-check AMOMAXL_W result
            "call amominulW_local_scalar_expected_result%=\n"  // generate AMOMINUL_W expected result
            "call delay%=\n"
            "addi t6, t6, 1\n"

            "call amomaxulW_local_scalar_test%=\n"
            //"call amominulW_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXUL_W returning data to self-check AMOMINUL_W result
            "call comm_amo_lW_local_scalar_selfcheck%=\n" // (CYE) use AMOMAXUL_W returning data to self-check AMOMINUL_W result
            "call amomaxulW_local_scalar_expected_result%=\n"
            "call delay%=\n"
            "addi t6, t6, 1\n"

            // (CYE) do an extra AMOORL_W(0) to self-check AMOMAXUL_W result
            "call amomaxulW_local_scalar_selfcheck%=\n" 

            "call end%=\n"




            // AMOSWAPL.W
            "amoswaplW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoswapl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

            // (CYE) figure out AMOSWAPL_W expected result in L2 address (t0): to be used by self-check later)
            "amoswaplW_local_scalar_expected_result%=:\n"
            "mv t3, t4\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOANDL.W
            "amoandlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoandl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOSWAPL_W (above): use AMOANDL_W returning data (t2):
            "amoswaplW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOANDL_W expected result in L2 address (t0): to be used by self-check later)
            "amoandlW_local_scalar_expected_result%=:\n"
            "and t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOORL.W
            "amoorlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoorl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOANDL_W (above): use AMOORL_W returning data (t2):
            "amoandlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOORL_W expected result in L2 address (t0): to be used by self-check later)
            "amoorlW_local_scalar_expected_result%=:\n"
            "or t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOXORL.W
            "amoxorlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoxorl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOORL_W (above): use AMOXORL_W returning data (t2):
            "amoorlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOXORL_W expected result in L2 address (t0): to be used by self-check later)
            "amoxorlW_local_scalar_expected_result%=:\n"
            "xor t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOADDL.W
            "amoaddlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amoaddl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOXORL_W (above): use AMOADDL_W returning data (t2):
            "amoxorlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOADDL_W expected result in L2 address (t0): to be used by self-check later)
            "amoaddlW_local_scalar_expected_result%=:\n"
            "add t3, t4, t2\n"     // (CYE) saved in t3 (to be used by self-check later)
            "nop\n"
            "ret\n"


            // AMOMINL.W
            "amominlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOADDL_W (above): use AMOMINL_W returning data (t2):
            "amoaddlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMINL_W expected result in L2 address (t0): to be used by self-check later)
            "amominlW_local_scalar_expected_result%=:\n"
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


            // AMOMAXL.W
            "amomaxlW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxl.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMINL_W (above): use AMOMAXL_W returning data (t2):
            "amominlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMAXL_W expected result in L2 address (t0): to be used by self-check later)
            "amomaxlW_local_scalar_expected_result%=:\n"
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


            // AMOMINUL.W
            "amominulW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amominul.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMAXL_W (above): use AMOMINUXL_W returning data (t2):
            "amomaxlW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMINUL_W expected result in L2 address (t0): to be used by self-check later;
            "amominulW_local_scalar_expected_result%=:\n"
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


            // AMOMAXUL.W
            "amomaxulW_local_scalar_test%=:\n"
            "div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "amomaxul.w t2, t4, (t0)\n"
            "nop\n"
            "ret\n"

           /***
            // (CYE) self-check result of AMOMINUL_W (above): use AMOMAXUL_W returning data (t2):
            "amominulW_local_scalar_selfcheck%=:\n"
            "li s10, 0xffffffff\n" 
            "and t3, t3, s10\n"      // only acquire lower 32 bits (1-word)
            "bne t2, t3, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"     // error_count
            "2:\n" 
            "nop\n"
            "ret\n"
           ***/

            // (CYE) figure out AMOMAXUL_W expected result in L2 address (t0): to be used by self-check later;
            "amomaxulW_local_scalar_expected_result%=:\n"
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


            // (CYE) self-check result of AMOMAXUL_W (above): do an extra AMOORL_W(0) returning data (t2):
            "amomaxulW_local_scalar_selfcheck%=:\n"
            "amoorl.w t2, zero, (t0)\n"
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
            "comm_amo_lW_local_scalar_selfcheck%=:\n"
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

   }  // for() 

   } // if (hart_id == 0)

   C_TEST_PASS;
   return 0;
}

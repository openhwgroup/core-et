/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "minion.h"
#include "et_test_common.h"


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


   static uint32_t exp_result[32]  __attribute__ ((aligned (32)))= {
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000
   };

   static uint32_t rtl_result[32]  __attribute__ ((aligned (32)))= {
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000,
      0x00000000
   };



   //volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   // (CYE) only run the following code in hart_id==0 (in order to do self-check):
   uint64_t hart_id = get_hart_id();

   if ((hart_id % 2) ==  0) {

   for (int i=1; i<10; i++) {  
      int r = 30999*i + 1234*i;

     /*********
      // Test
      __asm__ __volatile__ (
            "addi t0, %[operands], 0\n"
            "li   t1, %1\n"
            "fence \n"
            "amoandl.w t2, t1, (t0)\n"
            :
            : [operands] "r" (operands), "n" (r)
            : "t0", "t1", "t2", "memory" 
            );
     **********/
      // (CYE) changed to code below: functional equivalent and to use register for "r" itself (instead of immediate value):

      // Test
      __asm__ __volatile__ (
            // (CYE) for self-check later: first step: load (save) operands[0] orignal value into a0:
            "lw a0, 0(%[operands])\n"

            "addi t0, %[operands], 0\n"
            "fence \n"
            "amoandl.w t2, %1, (t0)\n"
            :
            : [operands] "r" (operands), "r" (r)
            : "a0", "t0", "t2", "memory"
            );


      //---------------- (CYE) self-checking: --------------------------
      FENCE;

      // check #2: new value in L2: 
      __asm__ __volatile__ 
         (
          // expected result = "r" & t2 (original operands[i]); then store into exp_result[]
          "and a0, %1, t2\n"
          "sw a0, 0(%[exp_result])\n"

          // do an atomic_or_0 to read the current (t0) content in L2: (bypass L1): 
          // then store into rtl_results[]
          "amoorl.w a1, zero, (t0)\n"  
          "sw a1, 0(%[rtl_result])\n"

           :
           : [exp_result] "r" (exp_result), "r" (r), [rtl_result] "r" (rtl_result)
           : "t0", "t2", "a0", "a1", "memory"
         );
      FENCE;

      if (rtl_result[0] != exp_result[0]) {
         et_printf("Test self-check: new value in L2: mismatch\n"); 
         C_TEST_FAIL;
      }
      //else {
      //   et_printf("Test self-check: new value in L2: match\n"); 
      //}


      // clean up: evict_va the cline (operands[]) from L1: (to avoid problem in next iteration of for() loop)
      FENCE;
      evict_va(false, 0x1, (uint64_t)(void*)operands, 0, 0, 0, 0);
      WAIT_CACHEOPS;
      FENCE;

   } // for ()
   
   }  // if (hart_id == 0)

   C_TEST_PASS;
   return 0;
}

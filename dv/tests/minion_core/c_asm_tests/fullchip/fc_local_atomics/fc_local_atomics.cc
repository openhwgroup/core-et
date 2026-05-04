/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef MINSHIRE_GATESIM_ENABLE
#include "et_test_common.h"
#define TIMEOUT 1000
#endif

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "fcc.h"

inline void __attribute__((always_inline)) evict_l1_to_l2() {

   for (int w = 0; w < 4; ++w) {
      evict_sw(false, 0x1, w, 0, 15, 0);
   }
   WAIT_CACHEOPS;
}

uint8_t minion_id(void)
{ 
  uint64_t hart_id = get_hart_id();
          
  return ((hart_id>>1) & 0x1F);
}

int main()
{
   static uint32_t predicted_data[8]  __attribute__ ((aligned (32)));

   static uint32_t operands[8]  __attribute__ ((aligned (32)))= {
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      /*0x7f800002, // signaling NaN
      0xff800002, // -signaling NaN
      0x7fc00001, //NANQ
      0xffc00001, //-NANQ
      0x1edcba98,
      0x12345678,
      0x12345678,
      0x1edcba98,
      0x69abcdef,
      0x38765432,
      0x7543654a,
      0x3c342985,
      0x15634579,
      0x78967365,
      0x88888888,
      0x88888888,
      0x77777777,
      0x77777777,
      0x44444444,
      0x44444444,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98*/
   };

   static uint32_t result[8]  __attribute__ ((aligned (32)))= {
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98
   };

   static uint32_t address[8]  __attribute__ ((aligned (16)))= {
      0x0,
      0x4,
      0x8,
      0xc,
      0x10,
      0x14,
      0x18,
      0x1c
   };

   C_TEST_START;

  #ifdef MINSHIRE_GATESIM_ENABLE
   volatile uint64_t hart_id = get_hart_id(); 
   volatile uint64_t thread_id  = (hart_id & 1);
   volatile uint64_t minion_index = ((hart_id>>1) & 0x1F);      
   volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   volatile uint64_t timeout = TIMEOUT;
   if (shire_id == 32) {
     if ((shire_id == 32) && (minion_index == 0) && (thread_id == 0)) {  
       s32_scp_init();
       give_credit(0, -1, -1, 1);
       give_credit(32, -1, -1, 1);
     }         
   }
   wait_for_credit(1);  

   if (shire_id == 0){
  #endif  


   if (minion_id() == 0){
      /* Wait master to evict result*/
      wait_for_credit(0);

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "addi t0, %[result], 0\n"

         "famoaddl.pi f0,f1(t0)\n"

         //"fswl.ps f0, 0(%[operands])\n"
         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
      );

      /* Tell master minion to check data */
      give_credit(0, 1, 0, 0);

   }
   /* minion 0*/
   else if (minion_id() == 1){
      /* Calculate predicted data */
      for ( int i = 0; i < 8; i ++)
         predicted_data[i] = operands[i] + 0xfedcba98;

      /* Evict operands from caches*/
      evict_l1_to_l2();

      /* Tell minion 0 to write result to L3*/
      give_credit(0, 0, 0, 0);

      /* Wait credit */
      wait_for_credit(0);

      /* Check data in L2*/
      for ( int i = 0; i < 8; i ++)
         if (result[i] != predicted_data[i]) {
            #ifdef MINSHIRE_GATESIM_ENABLE
               if (shire_id == 0 and thread_id == 0) {
                  atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + minion_index*8); 
               }
            #endif 
            C_TEST_FAIL;
         }
   }

 #ifdef MINSHIRE_GATESIM_ENABLE
   }  

   if (shire_id == 0 and thread_id == 0) {
     atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_index*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
   }

   if (shire_id == 32 and thread_id == 0) {
     check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_index*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
   }
 #endif  

   C_TEST_PASS;
   return 0;
}

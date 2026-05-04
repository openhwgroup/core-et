/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "fcc.h"

inline void __attribute__((always_inline)) evict_l2_to_l3() {

   for (int w = 0; w < 8; ++w) {
      evict_sw(false, 0x2, w, 0, 15, 0);
   }
   WAIT_CACHEOPS;
}

uint8_t check_master(void)
{ 
  uint64_t hart_id = get_hart_id();
          
  if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     if (shire_id == 32 && minion_id == 0) return 1;  
     else return 0;
  }   
  return 0;
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

   if (!check_master()){
      /* Wait master to evict result*/
      wait_for_credit(0);

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "addi t0, %[result], 0\n"

         "famoaddg.pi f0,f1(t0)\n"

         //"fswl.ps f0, 0(%[operands])\n"
         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
      );

      /* Tell master minion to check data */
      give_credit(32, 0, 0, 0);

   }
   /* shire 0*/
   else {
      /* Calculate predicted data */
      for ( int i = 0; i < 8; i ++)
         predicted_data[i] = operands[i] + 0xfedcba98;

      /* Evict operands from caches*/
      evict_l2_to_l3();
      /* Tell minion 0 to write result to L3*/
      give_credit(0, 0, 0, 0);

      /* Wait credit */
      wait_for_credit(0);
      /* Check dummy data in L3*/
      for ( int i = 0; i < 8; i ++)
         if (result[i] != predicted_data[i]) C_TEST_FAIL;

   }
   C_TEST_PASS;
   return 0;
}

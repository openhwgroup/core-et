/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "fcc.h"

#define SCP_S32_ADDRESS 0x0090000000 

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

   static uint32_t operands[16]  __attribute__ ((aligned (32)))= {
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000 // +Inf
   };

   static uint32_t result[16]  __attribute__ ((aligned (32)))= {
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
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

   //scratchpad addresses in SHIRE32
   uint32_t *result_sc;
   uint32_t *operands_sc;
 
   result_sc =(uint32_t*)SCP_S32_ADDRESS;
   operands_sc = (uint32_t*)(SCP_S32_ADDRESS + 256);


   if (!check_master()){

      /* Wait master to evict result*/
      wait_for_credit(0);

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x1, 0xff\n"
         "mova.m.x x1\n"

         "flw.ps f2, 0(%[operands])\n"
         "fswg.ps f2, 0(%[operands_sc])\n"

         "flw.ps f3, 0(%[result])\n"
         "fswg.ps f3, 0(%[result_sc])\n"

         "flw.ps f0, 0(%[operands_sc])\n"
         "flw.ps f1, 0(%[address])\n"
         "addi t0, %[result_sc], 0\n"

         "famoaddg.pi f0,f1(t0)\n"

         :
         : [operands_sc] "r" (operands_sc), [address] "r" (address), [result_sc] "r" (result_sc), [operands] "r" (operands), [result] "r" (result)
         : "t0", "f0", "f1", "f2", "f3"
      );

      /* Tell master minion to check data */
      give_credit(32, 0, 0, 0);

   }
   /* shire 32*/
   else {
      /* Initialize SCP bank 0*/
      uint64_t *scp_esr = (uint64_t*) 0x80F00030;
      *scp_esr = 0x00000901;

      /* Calculate predicted data */
      for ( int i = 0; i < 8; i ++)
         predicted_data[i] = operands[i] + 0xfedcba98;

      /* Tell minion 0 to write result to L3*/
      give_credit(0, 0, 0, 0);

      /* Wait credit */
      wait_for_credit(0);

      /* Check dummy data in L3*/
      for ( int i = 0; i < 8; i ++)
         if (result_sc[i] != predicted_data[i]) C_TEST_FAIL;

   }
   C_TEST_PASS;
   return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef MINSHIRE_GATESIM_ENABLE
#include "et_test_common.h"
#include "minion.h"
#define TIMEOUT 2000
#endif

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
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
   // Test
   __asm__ __volatile__ (
         // Enables all elements of FPU
         "mov.m.x m0, zero, 0xff\n"
         "addi    t3, zero, 8\n"
         "addi    x1, zero, 0xe7\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x80\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x00\n"
         "sll     x1, x1, t3\n"
         "addi    x1, x1, 0x00\n"
         "addi    t3, zero, 256\n"
         "fbcx.ps f1, x1\n"
         "loop:\n"
         "fexp.ps f1, f1\n"
         "addi    x1, x1, 1\n"
         "addi    t3, t3, -1\n"
         //"bne     t3, zero, loop\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         "addi    t3, t3, -1\n"
         : 
         : 
         : "f0", "x1", "t3" 
         );

   #ifdef MINSHIRE_GATESIM_ENABLE

     }

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

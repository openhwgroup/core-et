/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test exercise basic Fast Credit Counter functionality
//               Should be run on a full Minion Shire
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "common.h"

#define NUM_ITER 20

#define MPROT_BASE 0x80D00020
#define MPROT(shire, neigh) ((MPROT_BASE) | ((shire)<<22) | ((neigh)<<16))

volatile uint32_t check_var = 0;


int main()
{
   
   int hid = get_hart_id();

   int rand_neigh[]         = {0, 2, 0, 3, 1, 0, 1, 1, 3, 3, 0, 2, 2, 0, 3, 0};
   int rand_bcast_neigh[]   = {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0};
   int rand_current_shire[] = {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0};
   int rand_repeat[]        = {0, 1, 2, 4, 3, 5, 5, 5, 4, 6, 1, 6, 3, 3, 2, 3};
   
   int max_ptr = (sizeof(rand_neigh)/sizeof(int));
   int ptr=hid % max_ptr;
   

  // shire_ESR bits
  // 32 31 30   29..22 21 20 19 18 17 16.3 2 1 0
  //  1  0  0 shire_id  1  1  0  1  0  Reg 0 0 0   
   uint64_t addr_thread_0_shire_0  = (1UL << 32) + (0x1AUL << 17) + (0x018UL << 3); //shire_id = 0 fcc_counter_0==0x018
   uint64_t addr_thread_1_shire_0  = (1UL << 32) + (0x1AUL << 17) + (0x01AUL << 3); //shire_id = 0 fcc_counter_3==0x01A

   // Init marker
   C_TEST_START;

   for ( int i = 0; i < NUM_ITER; i++) {

     int shire = rand_current_shire[ptr] ? 0xFF : 0;
     int neigh = rand_bcast_neigh[ptr] ? 0xF : rand_neigh[ptr];

     //READ();
     volatile uint64_t *esr = (volatile uint64_t*) MPROT(shire, 0);
     uint64_t val = *esr;

      if (i==0) {
        if (val != 0x13) { //check that MPROT has the reset value at the beginning
          C_TEST_FAIL;
        }
      }

     //give credit to all threads in shire 0 
     store(addr_thread_0_shire_0,  0xFFFFFFFF); // give credit to all minions inside shire
     store(addr_thread_1_shire_0,  0xFFFFFFFF); // give credit to all minions inside shire
     
     //wait all threads to finish ESR read
     for (int c = 0; c<64; c++){
      waitFCC(0);
     }

     //WRITE();
     esr = (volatile uint64_t*) MPROT(shire, neigh);
     for ( int j=0; j < rand_repeat[ptr]; j++)
       *esr = (++val) & 0xFFFFFFF7;   //Set prot[3] == 0

    //give credit to all threads in shire 0
     store(addr_thread_0_shire_0,  0xFFFFFFFF); // give credit to all minions inside shire
     store(addr_thread_1_shire_0,  0xFFFFFFFF); // give credit to all minions inside shire
     
     //wait all threads to finish ESR write
     for (int c = 0; c<64; c++){
      waitFCC(0);
     }

     ptr++;
     if (ptr == max_ptr) ptr = 0;
   }

   if (get_credit_count_nb(0) != 0) {
            C_TEST_FAIL;
         }
   
   C_TEST_PASS;
   return 0;
}





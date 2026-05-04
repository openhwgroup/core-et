/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#ifdef MINSHIRE_GATESIM_ENABLE
  #include "et_test_common.h"
  #include "test_common.h"
#endif 

#include "test_compute.h"
#include "test_sync.h"
#include "shire.h"
#include "utils.h"

#ifdef MINSHIRE_GATESIM_ENABLE
  #define TIMEOUT 15000
#endif 


// This is the entry point for the test
void test_entry_point()
{
    volatile uint32_t hart_id = get_hart_id();    
    volatile uint32_t thread_id  = (hart_id & 1);
    volatile uint32_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint32_t shire_id  = ((hart_id>>6) & 0x3F);
    

    #ifdef MINSHIRE_GATESIM_ENABLE
      volatile uint32_t timeout = TIMEOUT;

      if ((shire_id == 32) && (minion_id == 16) && (thread_id == 0)) {  
        //s32_scp_init();
        //local SCP zero initialization
        setM0MaskFF();
        initFRegs_0();
        uint64_t conf = (3ULL << 55) | (15ULL << 51) | (((2U)<<30U)  + (127ULL << 23)); 
        tensorStore( conf , 64);

        give_credit(32, -1, -1, 1);
      }

      if (shire_id == 32) {  
        wait_for_credit(1);
      } 
    #endif 


    // Selects type of function based on position
    // Compute minions
    if((shire_id < 32) && (thread_id == 0))
        test_compute(shire_id, minion_id);
    //else if((shire_id < 32) && (thread_id == 1) && (minion_id < 16))
    //    test_helper_activation(shire_id, minion_id);
    //else if((shire_id < 32) && (thread_id == 1) && (minion_id < 24))
    //    test_helper_weights(shire_id, minion_id);
    //else if((shire_id < 32) && (thread_id == 1) && (minion_id < 28))
    //    test_helper_drain(shire_id, minion_id);
    //else if((shire_id < 32) && (thread_id == 1) && (minion_id < 29))
    //    test_helper_code(shire_id, minion_id);
    else if(shire_id == 32)
        test_sync(minion_id, thread_id);

    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 0 and thread_id == 0) {
          atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
      }
  
      if (shire_id == 32 and thread_id == 0) {
          check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
      }
    #endif 
}


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef MINSHIRE_GATESIM_ENABLE
  #include "et_test_common.h"
  #define TIMEOUT 25000
#endif 

// Local
#include "shire.h"
#include "test_common.h"
#include "test_compute.h"
#include "test_helper_activation.h"
#include "test_helper_weights.h"
#include "test_helper_code.h"
#include "test_helper_drain.h"
#include "test_sync.h"

// This is the entry point for the test
void test_entry_point()
{
    // Gets hart id and other ids
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
        give_credit(0, -1, -1, 1);
      }

        wait_for_credit(1);
    #endif 

    // Selects type of function based on position
    // Compute minions

    int shire_thr = N_SHIRES_COMPUTE;
    if((shire_id < shire_thr) && (thread_id == 0))
        test_compute(shire_id, minion_id);
    else if((shire_id < shire_thr) && (thread_id == 1) && (HELPER_ACTIVATION_THREADS & (1 << minion_id)))
        test_helper_activation(shire_id, minion_id);
    else if((shire_id < shire_thr) && (thread_id == 1) && (HELPER_WEIGHTS_THREADS    & (1 << minion_id)))
        test_helper_weights(shire_id, minion_id);
    else if((shire_id < shire_thr) && (thread_id == 1) && (HELPER_DRAIN_THREADS      & (1 << minion_id)))
        test_helper_drain(shire_id, minion_id);
    else if((shire_id < shire_thr) && (thread_id == 1) && (HELPER_CODE_THREADS       & (1 << minion_id)))
        test_helper_code(shire_id, minion_id);
    else if(shire_id == 32) {
        test_sync(minion_id, thread_id);
    }

    #ifdef MINSHIRE_GATESIM_ENABLE

      if (shire_id == 0 and thread_id == 0) {
          wait_for_credit(1); // to ensure that t1 is successfully finished
          atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary    
      }

      if (shire_id == 0 and thread_id == 1) {
          give_credit(0, minion_id, 0, 1); 
      }

  
      if (shire_id == 32 and thread_id == 0) {
          check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary
          check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + (minion_id-16)*8); //only minions 16..31 in S32 execute test_entry_point(), so we need additional synchronization for minions 0..15
          // Wait to prevent master minions finish the test before compute minions
          for(int i=0; i<5000; i++) NOP;                      
      }
    #endif     
    // Return to the FW U-mode trampoline
}


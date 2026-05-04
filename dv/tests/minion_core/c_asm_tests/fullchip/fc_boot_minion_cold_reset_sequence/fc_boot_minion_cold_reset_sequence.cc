/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "minion.h"
#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "mm_minion_cold_reset_sequence.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 100000
#endif

void dram_rw(){
  uint64_t     regVal;
  uint64_t     *pRegs64;

  for(int i=0; i<10; i++) {
      pRegs64 = (uint64_t *)(0x8100000000+0x87f00+i*8);
      regVal = pRegs64[0];

      regVal= 0x0001020304050607;
      pRegs64[0]=regVal;

      regVal = pRegs64[0];

      if (regVal != 0x0001020304050607 ) {
          #ifdef MINSHIRE_GATESIM_ENABLE
            volatile uint64_t hart_id = get_hart_id(); 
            volatile uint64_t thread_id  = (hart_id & 1);
            volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
            volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

            if (shire_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 8*minion_id);
          #endif
          C_TEST_FAIL;
      }/* End if*/
  }
}

static inline void __attribute__((always_inline)) enable_local_threads(uint64_t thread_mask)
{
   uint8_t  enable_neigh;
   uint64_t reg_value;
   uint64_t thread_disable_mask;
   uint64_t thread0_mask = 0;
   uint64_t thread1_mask = 0;
   uint64_t shire_config;

   thread_disable_mask = thread_mask ^ 0xfffffffffffffffful;

   if(thread_mask && 0x0ffff) enable_neigh = 1;
   if((thread_mask >> 16) && 0x0ffff) enable_neigh = enable_neigh + 2;
   if((thread_mask >> 32) && 0x0ffff) enable_neigh = enable_neigh + 4;
   if((thread_mask >> 48) && 0x0ffff) enable_neigh = enable_neigh + 8;

   shire_config = read_esr(PP_MACHINE, LOCAL_SHIRE_ID, REGION_OTHER, SHIRE_OTHER_CONFIG);
   reg_value = (shire_config & 0xffffffffffffe1fful) | (enable_neigh << 9);

   //enable neighs
   write_esr(PP_MACHINE, LOCAL_SHIRE_ID, REGION_OTHER, SHIRE_OTHER_CONFIG, reg_value);

   //enable threads
   for(int j = 0; j < 64; j++) {
      if(j%2 == 0) thread0_mask |= thread_disable_mask & (1 << (j/2));
      else thread1_mask |= thread_disable_mask & (1 << (j/2));
   }
   write_esr(PP_MACHINE, LOCAL_SHIRE_ID, REGION_OTHER, SHIRE_OTHER_THREAD0_DISABLE, thread0_mask);
   write_esr(PP_MACHINE, LOCAL_SHIRE_ID, REGION_OTHER, SHIRE_OTHER_THREAD1_DISABLE, thread1_mask);

}

int main()
{
    C_TEST_START;

    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {

      config_compute_shire_dlls(0x0ffffffff);

      pll_normalize_and_turn_of_normalization();

      update_minion_pll_freq(500, 0);

      enable_minion_threads(0x01, 0xffffffffffffffff);

      update_self_pll_freq_full(MODE_750MHz);    

      enable_local_threads(0xffffffffffffffff);
    }

    #ifdef MINSHIRE_GATESIM_ENABLE
      volatile uint64_t timeout = TIMEOUT;

      if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
        s32_scp_init();
        give_credit(0, -1, -1, 1);
        give_credit(32, -1, -1, 1);
      }         

      wait_for_credit(1);  
    #endif 

    dram_rw();

    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 0 and thread_id == 0) {
        atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
      }

      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
      }
    #endif
      
    C_TEST_PASS;
}

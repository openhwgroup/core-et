/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "minion.h"
#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "cacheops.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "pll_config.h"
#include "mm_minion_cold_reset_sequence.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 1000
#endif

inline __attribute__((always_inline)) void tensorWait(long id) {
    __asm__ __volatile__
        (
         " csrw 0x830, %[id]\n"
         :
         : [id] "r" (id)
         : "memory"
        );
}

void dram_rw(int iter){
  uint64_t     regVal;
  uint64_t     *pRegs64;

  pRegs64 = (uint64_t *)(0x8100000000+0x87f00+iter*0x40);
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

int main()
{
    C_TEST_START;

    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;
    uint64_t   reg_value;
    uint8_t*   syncAddr =  (uint8_t*) 0x20009300;
    uint8_t    num_iter;
    lvdpllMode new_mode;
    uint8_t    way = 0;

    #ifdef MINSHIRE_GATESIM_ENABLE
      volatile uint64_t timeout_gls = TIMEOUT;

      if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
        s32_scp_init();
        give_credit(0, -1, -1, 1);
        give_credit(32, -1, -1, 1);
      }         

      wait_for_credit(1);  
    #endif 

    num_iter = *syncAddr;
    syncAddr++;      

    if ((shire_id == 32) && (minion_id == 0)) {
       if(thread_id == 0) {
          dram_rw(100);
          //1GHz with normalization on
          pll_normalize_and_turn_of_normalization();
          
          //750MHz
          update_minion_pll_freq(750, 0);
          while(!(read_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));

          for(int i=1; i<num_iter; i++) {
             new_mode = lvdpllMode(*syncAddr);
             syncAddr++;
             for(int j=0; j<100; j++) dram_rw(j);
             update_minion_pll_freq(new_mode*25+275, 0);
             while(!(read_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
          }
       }
       else C_TEST_PASS;
    }
    else {
       for(int i=1; i<num_iter; i++) {
          for(int j=0; j<300; j++) dram_rw(j);
          if((i%5) == 0) {
             flush_sw(0, to_Mem, way%4, 0, 0xf);
             tensorWait(6);
             way++;
          }
       }
    }

    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 0 and thread_id == 0) {
        atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
      }

      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout_gls, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
      }
    #endif
      
    C_TEST_PASS;
}

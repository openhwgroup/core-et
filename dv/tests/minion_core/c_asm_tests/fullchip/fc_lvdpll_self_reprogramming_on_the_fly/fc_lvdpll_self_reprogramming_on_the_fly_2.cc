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
#include "pll_config.h"
#include "fc_utils.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 1000
#endif

void dram_rw(int num_iter){
  uint64_t     regVal;
  uint64_t     *pRegs64;

  for(int i=0; i<num_iter; i++) {
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

int main()
{
    C_TEST_START;

    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;
    uint32_t    timeout;
    uint8_t     mode1;
    uint8_t     mode2;
    uint8_t     mode3;

    #ifdef MINSHIRE_GATESIM_ENABLE
      volatile uint64_t timeout_gls = TIMEOUT;

      if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
        s32_scp_init();
        give_credit(0, -1, -1, 1);
        give_credit(32, -1, -1, 1);
      }         

      wait_for_credit(1);  
    #endif 

    timeout = 300;
    while(*baseAddr != 0xbabe) {
        timeout--;
        if(timeout == 0)  {
          #ifdef MINSHIRE_GATESIM_ENABLE
            if (shire_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 8*minion_id);
          #endif   
          C_TEST_FAIL;
        }
    }

    baseAddr = baseAddr + 1;
    mode1 = *baseAddr;
    baseAddr = baseAddr + 1;
    mode2 = *baseAddr;
    baseAddr = baseAddr + 1;
    mode3 = *baseAddr;

    baseAddr = baseAddr + 1;
    *baseAddr = 0xbabebeef;

    if ((shire_id == 32 || shire_id == 0) && (minion_id == 0) && (thread_id == 0)) {
      dram_rw(100);
      pll_mode_config_no_dll(255, mode1);
      dram_rw(100); 
      pll_mode_config_no_dll(255, mode2);
      dram_rw(100);
      pll_mode_config_no_dll(255, mode3);
      dram_rw(100);
    }
    else dram_rw(1000);

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

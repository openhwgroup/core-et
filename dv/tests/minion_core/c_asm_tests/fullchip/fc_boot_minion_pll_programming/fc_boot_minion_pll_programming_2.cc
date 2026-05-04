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

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 1000
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

void config_compute_shire_dlls()
{
   uint64_t reg_value;

   // Auto-config register set dll_enable and get reset deasserted of the DLL
   reg_value =  SHIRE_OTHER_DLL_AUTO_CONFIG_PCLK_SEL
             | (0x1 << SHIRE_OTHER_DLL_AUTO_CONFIG_DLL_EN_OFF);
   broadcast_req (PP_MACHINE,       REGION_OTHER, SHIRE_OTHER_DLL_AUTO_CONFIG,           0x2ffffffff, reg_value);

   for ( uint8_t i = 0; i <= 31; i ++){
      // Wait until DLL is locked
      while(!(read_esr(PP_MACHINE, i, REGION_OTHER, SHIRE_DLL_READ_DATA) & 0x20000));
   }

}

int main()
{
    C_TEST_START;

    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
      pll_mode_config_no_dll(255, 19); //750MHz    

      //enable rest of local threads
      write_esr(PP_MACHINE, 255, REGION_OTHER, SHIRE_OTHER_CONFIG, 0x001f20);
      write_esr(PP_MACHINE, 255, REGION_OTHER, SHIRE_OTHER_THREAD0_DISABLE, 0x00000000ul);
      write_esr(PP_MACHINE, 255, REGION_OTHER, SHIRE_OTHER_THREAD1_DISABLE, 0x00000000ul);

      config_compute_shire_dlls();

      //enable neigh
      write_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_OTHER_CONFIG, 0x001f00);
      //enable thread 0
      write_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_OTHER_THREAD0_DISABLE, 0xfffffffe);
    }
    else if ((shire_id == 0) && (minion_id == 0) && (thread_id == 0)) {
      pll_mode_config_no_dll(255, 19); //750MHz      

      //enable rest of local threads
      write_esr(PP_MACHINE, 255, REGION_OTHER, SHIRE_OTHER_THREAD0_DISABLE, 0x00000000ul);
      write_esr(PP_MACHINE, 255, REGION_OTHER, SHIRE_OTHER_THREAD1_DISABLE, 0x00000000ul);
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

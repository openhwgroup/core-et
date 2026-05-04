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

void pll_config_multiple_write_broadcast(uint32_t reg_first, uint32_t reg_num)
{
  uint64_t reg_value;

  // Enable PLL auto config
  reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (reg_num << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (reg_first << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
  broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x2ffffffff, reg_value);
  
  // Start PLL auto config
  reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (reg_num << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (reg_first << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_WRITE_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_RUN_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
  broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x2ffffffff, reg_value);
  
  // Wait for the PLL configuration to finish
  //while((read_esr(PP_MACHINE, shire_id, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x10000));
  for ( uint8_t i = 0; i <= 31; i ++){
      // Wait for the PLL configuration to finish
      while((read_esr(PP_MACHINE, i, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x10000));
  }
  
  // Stop the PLL auto config
  reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (reg_num << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (reg_first << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF);
  broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x2ffffffff, reg_value);
}

void pll_mode_config_broadcast(uint8_t mode)
{
   uint64_t reg_value;
   
   // Select 1 GHz from step_clock, Bits[2:0] = 3'b011. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x2ffffffff, 0xb);

   // Write PLL registers 0x00 to 0x0f. PLL registers can be downloaded in chunks of 16 registers
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[3];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[2];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[1];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[0];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[7];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[6];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[5];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[4];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_1, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[11];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[10];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[9];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[8];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_2, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[15];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[14];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[13];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[12];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_3, 0x2ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x00,0xf);
   
   // Write PLL registers 0x10 to 0x1a.
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[19];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[18];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[17];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[16];          
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[23];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[22];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[21];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[20];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_1, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[26];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[25];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[24];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_2, 0x2ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x10,0xa);

   // Write PLL registers 0x20 to 0x24.         
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[30];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[29];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[28];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[27];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, reg_value);
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[31];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_1, 0x2ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x20,0x4);

   // Write PLL registers 0x27 to 0x28.(DATA_1 and DATA_2 and DATA_3 not changed)
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[33];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[32];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x27,0x1);

   // Write PLL registers 0x38 to 0x39.(DATA_1 and DATA_2 and DATA_3 not changed)
   reg_value = 0;
   reg_value = reg_value | gs_lvdpll_settings[(mode-1)].values[35];
   reg_value = (reg_value << 16) | gs_lvdpll_settings[(mode-1)].values[34];
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x38,0x1);
   
   // Write PLL strobe register 0x38 to load previous configuration
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x2ffffffff, 0x0000000000000001);
   pll_config_multiple_write_broadcast(0x38,0x0);
  
   
   // Wait until PLL is locked to change clock mux
   for ( uint8_t i = 0; i <= 31; i ++){
      while(!(read_esr(PP_MACHINE, i, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
  }
   
   // Select PLL[0] output. Bits[2:0]=3'b100. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x2ffffffff, 0xc);

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

      pll_mode_config_broadcast(9); //500MHz

      //enable neighs
      write_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_OTHER_CONFIG, 0x001f00);
      //enable threads
      write_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_OTHER_THREAD0_DISABLE, 0x00000000ul);
      write_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_OTHER_THREAD1_DISABLE, 0x00000000ul);
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

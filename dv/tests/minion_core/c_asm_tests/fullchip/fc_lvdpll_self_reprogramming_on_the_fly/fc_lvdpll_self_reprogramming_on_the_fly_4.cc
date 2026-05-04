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
#include "dvfs_movellus_lvdpll_modes_config.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 1000
#endif

#define ESR_FLB_REG_00 0x80340100ULL

volatile uint8_t     current_mode;

static uint8_t quadrants[6][6] = {
       { 1, 1, 1, 2, 0, 0 },
       { 1, 1, 1, 2, 2, 2 },
       { 1, 1, 1, 2, 2, 2 },
       { 3, 3, 3, 4, 4, 4 },
       { 3, 3, 3, 4, 4, 4 },
       { 3, 3, 0, 0, 4, 4 }
};

static uint8_t mesh[6][6] = {
       { 0, 16, 12, 4, 253, 254 },
       { 1, 8, 24, 28, 20, 5 },
       { 9, 17, 25, 29, 21, 13 },
       { 2, 18, 26, 30, 22, 6 },
       { 10, 3, 27, 31, 7, 14 },
       { 11, 19, 32, 33, 23, 15 }
};

volatile uint32_t current_frq;

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

inline void __attribute__((always_inline)) wait_busy(uint8_t quadrant)
{
  for ( uint8_t i = 0; i <= 5; i ++){
  	for ( uint8_t j = 0; j <= 5; j ++){
      // Wait for the PLL configuration to finish
      if(quadrants[i][j] == quadrant) while((read_esr(PP_MACHINE, mesh[i][j], REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x10000));
    }
  }
}

inline __attribute__((always_inline)) void init_barrier(uint64_t barrier, uint64_t value = 0) {
    volatile uint64_t *flb_esr = (uint64_t *) (ESR_FLB_REG_00 + (barrier<<3));
    *flb_esr = value;
    FENCE;
}

inline __attribute__((always_inline)) uint64_t join_barrier(uint64_t barrier, uint64_t players) {
    uint64_t match = players - 1;
    uint64_t csr_enc = ((match   & 0xFF) << 5 ) |
                       ((barrier & 0x1F)      );
    uint64_t last_entry;
    asm volatile("csrrw %[last_entry], 0x820, %[csr_enc]\n"
                 : [last_entry]"=r"(last_entry)
                 : [csr_enc]"r"(csr_enc)
                 :
                );
    return last_entry;
}

inline void __attribute__((always_inline)) pll_config_multiple_write_broadcast(uint32_t reg_first, uint32_t reg_num, uint8_t quadrant)
{
  uint64_t reg_value;
  
  // Start PLL auto config
  reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (reg_num << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (reg_first << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_WRITE_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_RUN_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
  broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x2ffffffff, reg_value);
  
  // Wait for the PLL configuration to finish
  //give_credit_local(0, 1, 0);
  //give_credit_local(1, 0, 0);
  //give_credit_local(1, 1, 0);
  //wait_busy(quadrant);
  //uint64_t is_last = join_barrier(0, 4);
  //if (!is_last) {
  //    wait_for_credit(0);
  //}
  for(int i=1; i<=reg_num; i++)
  	for(int j=0; j<30; j++) NOP;
  
  // Stop the PLL auto config
  reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (reg_num << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (reg_first << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
  broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x2ffffffff, reg_value);
}

inline void __attribute__((always_inline)) wait_pll_lock(uint8_t quadrant)
{
  for ( uint8_t i = 0; i <= 5; i ++){
  	for ( uint8_t j = 0; j <= 5; j ++){
      // Wait for the PLL configuration to finish
      if(quadrants[i][j] == quadrant) while(!(read_esr(PP_MACHINE, mesh[i][j], REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
    }
  }
}

void pll_mode_config_broadcast(uint8_t mode, uint8_t quadrant)
{
   uint64_t reg_value;
   uint8_t  reg_num = 0;
   uint8_t  reg_2_update = 0;
   uint8_t  reg_3_update = 0;
   uint8_t  reg_b_update = 0;
   uint8_t  reg_e_update = 0;
   uint8_t  reg_20_update = 0;
   
   // Select 1 GHz from step_clock, Bits[2:0] = 3'b011. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x0ffffffff, 0xb);

   if(et_dvfs_lvdpll_settings[(mode-1)].quick_values[0] != et_dvfs_lvdpll_settings[(current_mode-1)].quick_values[0]) reg_2_update = 1;
   if(et_dvfs_lvdpll_settings[(mode-1)].quick_values[1] != et_dvfs_lvdpll_settings[(current_mode-1)].quick_values[1]) reg_3_update = 1;
   //if(et_dvfs_lvdpll_settings[(mode-1)].quick_values[2] != et_dvfs_lvdpll_settings[(current_mode-1)].quick_values[2]) reg_b_update = 1;
   if(et_dvfs_lvdpll_settings[(mode-1)].quick_values[3] != et_dvfs_lvdpll_settings[(current_mode-1)].quick_values[3]) reg_e_update = 1;
   //if(et_dvfs_lvdpll_settings[(mode-1)].quick_values[7] != et_dvfs_lvdpll_settings[(current_mode-1)].quick_values[7]) reg_20_update = 1;

   // Enable PLL auto config
   reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (0x0 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (0x02 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x0ffffffff, reg_value);

   if(reg_2_update == 1 && reg_3_update == 1) {
      reg_value = 0;
      reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[1];
      reg_value = (reg_value << 16) | et_dvfs_lvdpll_settings[(mode-1)].quick_values[0];
      broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
      pll_config_multiple_write_broadcast(0x02,0x1,quadrant);
   }
   else if(reg_2_update == 1) {
      reg_value = 0;
      reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[0];
      broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
      pll_config_multiple_write_broadcast(0x02,0x0,quadrant);
   }
   else if(reg_3_update == 1) {
      reg_value = 0;
      reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[1];
      broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
      pll_config_multiple_write_broadcast(0x03,0x0,quadrant);
   }
   
   //if(reg_b_update == 1 && reg_e_update == 1) {
   //   reg_value = 0;
   //   reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[3];
   //   reg_value = (reg_value << 16) | 0x0;
   //   reg_value = (reg_value << 16) | 0xc;
   //   reg_value = (reg_value << 16) | et_dvfs_lvdpll_settings[(mode-1)].quick_values[2];          
   //   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   //   pll_config_multiple_write_broadcast(0x0b,0x3,quadrant);
   //}
   //else if(reg_b_update == 1) {
   //   reg_value = 0;
   //   reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[2];
   //   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   //   pll_config_multiple_write_broadcast(0x0b,0x0,quadrant);
   //}
   //else 
   if(reg_e_update == 1) {
      reg_value = 0;
      reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[3];
      broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
      pll_config_multiple_write_broadcast(0x0e,0x0,quadrant);
   }

   //if(reg_20_update == 1) {
   //   reg_value = 0;
   //   reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[7];
   //   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   //   pll_config_multiple_write_broadcast(0x20,0x0,quadrant);
   //}
   
   
   // Write PLL strobe register 0x38 to load previous configuration
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, 0x0000000000000001);
   pll_config_multiple_write_broadcast(0x38,0x0,quadrant);
  
   
   // Wait until PLL is locked to change clock mux
   give_credit_local(0, 1, 0);
   give_credit_local(1, 0, 0);
   give_credit_local(1, 1, 0);
   wait_pll_lock(quadrant);
   uint64_t is_last = join_barrier(0, 4);
   if (!is_last) wait_for_credit(0);

   //for ( uint8_t i = 0; i <= 31; i ++){
   //   while(!(read_esr(PP_MACHINE, i, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
   //}
   
   // Select PLL[0] output. Bits[2:0]=3'b100. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x0ffffffff, 0xc);

   // Stop the PLL auto config
   reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL;
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x0ffffffff, reg_value);

   current_mode = mode;
 
}

void pll_mode_config_broadcast_fcw(uint8_t mode, uint8_t up_down, uint8_t jump, uint8_t quadrant)
{
   uint64_t reg_value;
   uint32_t new_freq;
   uint16_t fcw_int;
   uint16_t fcw_frac;
   uint16_t post_div;

   if(up_down == 1) {
      new_freq = current_frq + jump * 25;
   }
   else {
      new_freq = current_frq - jump * 25;
   }

   post_div = et_dvfs_lvdpll_settings[(mode-1)].quick_values[3];

   fcw_int = (new_freq * post_div) / 100;
   fcw_frac = ((65536 * ( new_freq * post_div)) - (65536 * (100 * fcw_int ))) / 100;

   current_frq = new_freq;
   
   // Select 1 GHz from step_clock, Bits[2:0] = 3'b011. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x0ffffffff, 0xb);

   // Enable PLL auto config
   reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
            | (0x0 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
            | (0x02 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
            | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x0ffffffff, reg_value);

   // Write PLL registers 0x00 to 0x0f. PLL registers can be downloaded in chunks of 16 registers
   reg_value = 0;
   reg_value = reg_value | fcw_frac;
   reg_value = (reg_value << 16) | fcw_int;
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   pll_config_multiple_write_broadcast(0x02,0x1,quadrant);
   
   // Write PLL registers 0x10 to 0x1a.
   //reg_value = 0;
   //reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[11];          
   //broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   //pll_config_multiple_write_broadcast(0x0b,0x0,quadrant);

   // Write PLL registers 0x20. PLL registers can be downloaded in chunks of 16 registers
   //reg_value = 0;
   ////reg_value = reg_value | et_dvfs_lvdpll_settings[(mode-1)].quick_values[32];
   //broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
   //pll_config_multiple_write_broadcast(0x20,0x0,quadrant);
   
   
   // Write PLL strobe register 0x38 to load previous configuration
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, 0x0000000000000001);
   pll_config_multiple_write_broadcast(0x38,0x0,quadrant);
  
   
   // Wait until PLL is locked to change clock mux
   give_credit_local(0, 1, 0);
   give_credit_local(1, 0, 0);
   give_credit_local(1, 1, 0);
   wait_pll_lock(quadrant);
   uint64_t is_last = join_barrier(0, 4);
   if (!is_last) wait_for_credit(0);

   //for ( uint8_t i = 0; i <= 31; i ++){
   //   while(!(read_esr(PP_MACHINE, i, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
   //}
   
   // Select PLL[0] output. Bits[2:0]=3'b100. Bit 3 to '1' to go with DLL output
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_CTRL_CLOCKMUX, 0x0ffffffff, 0xc);

   // Stop the PLL auto config
   reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL;
   broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x0ffffffff, reg_value);
 
}

int main()
{
    C_TEST_START;

    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;
    uint8_t     jump;
    uint64_t    reg_value;
    uint8_t*   syncAddr =  (uint8_t*) 0x20009300;
    uint8_t    num_iter;
    uint8_t    new_mode;
    uint8_t    up_down;

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

    if ((shire_id == 32) && (minion_id == 0 || minion_id == 1)) {
       if(minion_id == 0 && thread_id == 0) {
          dram_rw(100);
          //1GHz with normalization on
          pll_mode_config_broadcast(29, 1);
          // Enable PLL auto config
          reg_value =  SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL
                 | (0x0 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF)
                 | (0x02 << SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF)
                 | (0x1 << SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF);
          broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_AUTO_CONFIG, 0x0ffffffff, reg_value);
          // Turn of auto-normalization 
          reg_value = 0x81b8;      
          broadcast_req (PP_MACHINE, REGION_OTHER, SHIRE_OTHER_PLL_CONFIG_DATA_0, 0x0ffffffff, reg_value);
          pll_config_multiple_write_broadcast(0x00,0x0,0);
          
          //750MHz
          pll_mode_config_broadcast(19, 1);

          current_frq = 750;
          current_mode = 19;

          for(int i=1; i<num_iter; i++) {
          	 new_mode = *syncAddr;
       	  	 syncAddr++;
             dram_rw(100); 
             pll_mode_config_broadcast(new_mode, 1);
          }
       }
       else {
       	 for(int i=0; i<(num_iter+1); i++) {
            wait_for_credit(0);
            if(minion_id == 0 && thread_id == 1) wait_pll_lock(2);
            if(minion_id == 1 && thread_id == 0) wait_pll_lock(3);
            if(minion_id == 1 && thread_id == 1) wait_pll_lock(4);
            uint64_t is_last = join_barrier(0, 4);
            if (is_last) give_credit_local(0, 0, 0);
         }  
       }
    }
    else dram_rw(num_iter*250);

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

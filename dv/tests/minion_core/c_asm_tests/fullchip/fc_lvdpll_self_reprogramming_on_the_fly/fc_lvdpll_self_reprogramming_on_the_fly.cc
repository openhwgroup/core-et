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

    if ((shire_id == 32 || shire_id == 0) && (minion_id == 0) && (thread_id == 0)) {
      dram_rw(100);
      pll_mode_config_no_dll(255, 15); //650MHz
      dram_rw(100); 
      pll_mode_config_no_dll(255, 11); //550MHz
      dram_rw(100);
      pll_mode_config_no_dll(255, 15); //650MHz
      dram_rw(100);
    }
    else dram_rw(1000);

    C_TEST_PASS;
}

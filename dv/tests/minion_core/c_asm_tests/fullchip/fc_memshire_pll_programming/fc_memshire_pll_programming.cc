/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

int main()
{
    uint64_t regVal;
    uint64_t *pRegs64;

    C_TEST_START;

    for(int i=0; i<10; i++) {
      pRegs64 = (uint64_t *)(R_L3_DRAM_BASEADDR+0x87f00+i*8);
      regVal = pRegs64[0];

      regVal= 0x0001020304050607;
      pRegs64[0]=regVal;

      regVal = pRegs64[0];

      if (regVal != 0x0001020304050607 ) {
          C_TEST_FAIL;
      }/* End if*/
    }

    C_TEST_PASS;

} /*esr_rw_Test() */

/*****     < EOF >     *****/
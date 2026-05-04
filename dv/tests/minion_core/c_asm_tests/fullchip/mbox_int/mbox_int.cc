/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "mbox_intr.h"
#include "hal_device.h"

uint64_t baseAddr = R_PU_TRG_MMIN_BASEADDR;
//check if master minion
uint8_t check_master(void)
{ 
   uint64_t hart_id = get_hart_id();
    
   if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);

     if (shire_id == 32 && minion_id == 0) return 1; 
     else return 0;
   }
   return 0;
}

int main()
{
    C_TEST_START;

    PTR_Mbox_intr mbox_int_regs;
    mbox_int_regs = ( PTR_Mbox_intr ) baseAddr;
    /* Wait for SP to send credit*/
    if(check_master()){
      wait_for_credit(0);
      mbox_int_regs->ipi_trigger_set = 0xf ; 
    }
    C_TEST_PASS;
}

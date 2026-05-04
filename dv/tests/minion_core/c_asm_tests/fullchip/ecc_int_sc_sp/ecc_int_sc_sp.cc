/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "esr_rw.h"
#include "et_test_common.h"


#define ESR_SC_ERR_LOG_CTL     0xB

int main()
{

	volatile uint64_t shire_id = get_shire_id();
	volatile uint64_t* pAddr;
	volatile uint64_t readData;
	long long int addr = 0x8000000000;

    C_TEST_START;

    if (shire_id == 0) {

    	wait_for_credit(0);

    	/* Configure and enable interrupt on Minion Shire Cache */ 
    	//write_esr(PP_MACHINE, shire_id, REGION_SHIRE_CACHE, ESR_SC_ERR_LOG_CTL, 0x2); 

       pAddr = (uint64_t*)addr;

       *pAddr = 0xBABEBABEBABEBABE;

       readData = *pAddr;

       evict_va(false, to_L2, addr, 0, 0, 0, 0);
       WAIT_CACHEOPS;
       delay(100);
    
       __asm__ __volatile__ (             
           "slti t0, t0, 801\n"      // magic instruction to trigger double bit error injection       
          "nop\n"
          "nop\n"
          "nop\n"
          "nop\n"
          "nop\n"
         :
         : 
         : "t0"
       );
       
       readData = *pAddr;
    }

    C_TEST_PASS;
}

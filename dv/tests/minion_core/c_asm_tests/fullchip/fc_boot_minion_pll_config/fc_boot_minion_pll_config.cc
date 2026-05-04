/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef MINSHIRE_GATESIM_ENABLE
   #include "minion.h"
   #define TIMEOUT 1000
#endif

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



int main()
{
    C_TEST_START;

    uart_send_data(0x01234567);

  #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t hart_id = get_hart_id(); 
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t timeout = TIMEOUT;

    if (shire_id == 32) {
      C_TEST_START;

      if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
        s32_scp_init();
        give_credit(0, -1, -1, 1);
        give_credit(32, -1, -1, 1);
      }         
    }

    wait_for_credit(1);  

    if (shire_id == 0 and thread_id == 0) {
      atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
    }

    if (shire_id == 32 and thread_id == 0) {
      check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
    }
  #endif  

    C_TEST_PASS;
}

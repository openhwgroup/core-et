/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


// Turn off shires 0 and 1 according to power spec

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
#include "minion.h"
#include "common.h"
#include "safe_gate.h"

#define TIMEOUT 5000


EXTERN_C void  __attribute__ ((interrupt)) bad_trap(){
  et_printf("\nBad Trap\n");
  C_TEST_FAIL;
}

EXTERN_C void mtrap_vector_ipi();

int main()
{
  C_TEST_START

  volatile uint64_t hart_id = getHartId();
  //volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  #ifdef MINSHIRE_GATESIM_ENABLE
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t timeout = TIMEOUT;
  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
    s32_scp_init();
  }
  #endif

  if (((shire_id == 0) || (shire_id == 1)) && ((minion_id == 0) || (minion_id == 1))) {
    // configure trap vector
    // and enable interrupts
    __asm__ __volatile__ 
    (  
      "la t0, mtrap_vector_ipi\n"
      "ori t0, t0, 1\n"  // Here we set "vector mode" so that msip is on baase + 4 x 3 
      "csrw mtvec, t0\n" // assign routine base address
      "csrwi mie, 8\n"  // enable msip (msie)
      "csrsi mstatus, 8\n"  // set MIE to '1'
     : : : "t0"
    );
    WFI //wait for ipi
  }

  #ifdef MINSHIRE_GATESIM_ENABLE
  if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
    check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);        
  }
  #endif	
  C_TEST_PASS

}
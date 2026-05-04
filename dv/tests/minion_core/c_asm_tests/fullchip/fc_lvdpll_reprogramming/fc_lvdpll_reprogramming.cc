/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "hal_device.h"
#include "macros.h" 
#include "common.h"
#include "fc_lvdpll_reprogramming.h"


#define MASTER_SHIRE 32
#define MASTER_MINION 0
#define MASTER_THREAD 0
#define IPI_TRIGGER 0x80F40090 >> 3
#define IPI_TRIGGER_CLEAR 0x80F40098 >> 3
#define ALL_MINIONS_MASK 0xFFFFFFFFFFFFFFFF
#define EXP_CAUSE ((1ULL<<63) | 3)
#define NUM_IPI_MINIONS 16
#define NUM_THREADS 2
#define NUM_TEST_SHIRES 2
#define SHIRE_TEST_0 0
#define SHIRE_TEST_1 15


int main() {
  volatile uint64_t hart_id = getHartId();
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
  uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

  C_TEST_START;
  
  if (shire_id == SHIRE_TEST_0) {

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

    dram_rw();
    
    //Notify SP to reprogram PLL
    *baseAddr = 0xbabe;

    STALL //wait for ipi
    et_printf("\nInterrupt happened\n");

    dram_rw();
  }

  C_TEST_PASS;
  return 0;
}


void irq_handler()
{

  et_printf("\nInterrupt routine started\n");

  volatile uint64_t cause = 0;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  // error in receiving an exception other than page fault
  if ( cause != EXP_CAUSE) {
    et_printf("\nWrong Cause\n");
    C_TEST_FAIL;
  }
  else {
    volatile uint64_t hart_id = getHartId();
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    // clear the interrupt
    write_esr(PP_MACHINE, shire_id, REGION_OTHER, IPI_TRIGGER_CLEAR, 0x1 << hart_id);
    FENCE
  }
}

void bad_trap(){
  et_printf("\nBad Trap\n");
  C_TEST_FAIL;
}

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
          C_TEST_FAIL;
      }/* End if*/
  }
}

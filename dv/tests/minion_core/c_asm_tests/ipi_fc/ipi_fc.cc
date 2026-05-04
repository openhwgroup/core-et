/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "ipi_fc.h"


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
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

  C_TEST_START;
  
  if (shire_id == MASTER_SHIRE) {
    if ((minion_id == MASTER_MINION) && (thread_id == MASTER_THREAD)) {
      // wait for all minions to enable interrupts
      for (int i=0; i<NUM_IPI_MINIONS*NUM_THREADS*NUM_TEST_SHIRES; i++)
        wait_for_credit(0);
      // trigger msip for all minions in shire0
      write_esr(PP_MACHINE, SHIRE_TEST_0, REGION_OTHER, IPI_TRIGGER, ALL_MINIONS_MASK);
      write_esr(PP_MACHINE, SHIRE_TEST_1, REGION_OTHER, IPI_TRIGGER, ALL_MINIONS_MASK);
    } 
  } else if (((shire_id == SHIRE_TEST_0) || (shire_id == SHIRE_TEST_1)) && (minion_id < NUM_IPI_MINIONS)) {
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
    give_credit(32, 0, 0, 0);
    STALL //wait for ipi
    et_printf("\nInterrupt happened\n");
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

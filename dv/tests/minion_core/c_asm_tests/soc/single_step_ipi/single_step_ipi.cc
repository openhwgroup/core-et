/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "minion.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include "single_step_ipi.h"

void fail_trap() {
  C_TEST_FAIL;
}

inline void delay(int d) { 
  while (d-- > 0) {
    __asm__ __volatile__ ("nop");
  } 
}

inline void wait_debugger() {
  /*
   * Debugger setups the minion an sets mcsrtch to 2
   */
  uint64_t mcause = 0;
  do {
    __asm__ __volatile__ 
      (
       "csrr %[mcause], mcause\n"
       : [mcause] "=r" (mcause)
       :
       :
       );
  } while(mcause != 2);
  __asm__ __volatile__ ("csrwi mcause, 0");
}


inline void send_ipi(int ipi_type) {

  if (ipi_type == IPI_SET) {
    volatile uint64_t *addr = reinterpret_cast<uint64_t*>(0x80F40090ULL);
    const uint64_t hart_triggered = 0;
    const uint64_t mask = 1 << hart_triggered;
    *addr = mask;
  } else {
    volatile uint64_t *addr = reinterpret_cast<uint64_t*>(0x80F40098ULL);
    const uint64_t hart_triggered = 0;
    const uint64_t mask = 1 << hart_triggered;
    *addr = mask;
  }
}

void minion0_code () {
  /*
   * We are single steping here
   * Just do useless nops while single step and 
   * minion1 'bombards' us with IPIs
   * Set mcratch to 1 to notify that the single step is done
   */
  constexpr int SINGLE_STEP_DELAY = 16;

  delay(SINGLE_STEP_DELAY);
  __asm__ __volatile__ ("csrwi mscratch, 1");

}

void minion1_code () {
  /*
   * This minion set MSIP to 1 and 0 a couple of times
   */

  constexpr int NUM_IPIs = 1024;
  for(int i = 0; i < NUM_IPIs; ++i) {
    send_ipi(IPI_SET);
    FENCE;
    send_ipi(IPI_CLEAR);
    FENCE;
  }
}

void setup() {

  // Initialize registers
  __asm__ __volatile__ 
    ("csrwi mscratch, 0\n"
     "csrsi mie, 0x8\n" // Enable sw-interrupts throgh IPI
     :
     :
     :
     );
  
  // Set trap vector
  void (*mtvec_ptr)(void) = &fail_trap;
  __asm__ __volatile__ 
    (
     "csrw mtval, t0\n\t"
     "csrw stval, t0\n\t"
     "mv t0, %[mtvec]\n\t"
     "csrw mtvec, t0\n\t"
     :
     :[mtvec] "r" (mtvec_ptr)
     :
     );

  
}

int main() {
  
  C_TEST_START;
  
  uint64_t minion_id = get_minion_id();
  setup();
  
  // Wait for halt
  wait_debugger();
  
  /*
   * Minion0: Single step
   * minion1: IPI
   */
  if (minion_id == 0)
    minion0_code();
  else
    minion1_code();
  
  
  C_TEST_PASS;
  return 0;
}

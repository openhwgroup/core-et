/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <stdio.h>
#include <stdlib.h>

#include "minion.h"
#include "macros.h"
#include "dcause_priority.h"

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
   * Debugger setups the minion an sets mcause to 2
   */
  
  __asm__ volatile ("csrwi mcause, 1"); // Set mcause to 1 to notifiy that we reached the spin loop
  
  uint64_t mcause = 0;
  do {
    __asm__ __volatile__ 
      (
       "csrr %[mcause], mcause\n" 
       : [mcause] "=r" (mcause)
       :
       :
       );
  } while (mcause != 2); // Loop until the debugger notifies that it end the setup
  
  __asm__ __volatile__ ("csrwi mcause, 0"); // Reset the CSR to its default value
}


__attribute__((always_inline))
inline void hart0_code () {
  __asm__ __volatile__ ("ebreak\n" // First ebreak just to notify that we have started
			//"ebreak\n" // Test 0: HW breakpoint + ebreak + halt req + single step
			//"nop\n"    // Test 1: HW breakpoint + halt req + single step
			"ebreak\n" // Test 2: HW breakpoint + ebreak + single step
			//"ebreak\n" // Test 3: ebreak + halt req + single step
			//"nop\n"    // Test 4: halt req + single step
			"nop\n"    // Test 5: single step
			"ebreak\n" // Test 6: HW breakpoint + ebreak
			"ebreak\n" // Test 7: ebreak + single step
			:::);
  
  __asm__ __volatile__ ("nop"); // Extra nop
}

void setup() {

  // Initialize registers
  __asm__ __volatile__ 
    ("csrwi mscratch, 0\n"
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
  
  uint64_t hart_id = get_hart_id();
  setup();
  
  // Wait for halt
  wait_debugger();
  
  /*
   * Hart 0 will execute multiple ebreaks/nops and the debugger will generate other events (haltreq, hw breakpoint,...)
   * to test the that the trap cause priority is respected
   */
  if (hart_id == 0)
    hart0_code();
  else
    C_TEST_PASS
  
  
  C_TEST_PASS;
  return 0;
}

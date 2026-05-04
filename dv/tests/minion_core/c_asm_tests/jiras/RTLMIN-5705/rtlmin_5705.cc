/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "rtlmin_5705.h"

int main() {
  C_TEST_START;
  unsigned id = get_hart_id();
  
  // neigh 1, core 0, thread 0 will control the test
  // minions in neigh 0 will be the ones being tested
  // other minions will do nothing
  if (id == CONTROL_HART) test_control();
  else if (id < THREADS_PER_NEIGH) run_tests(id);
  else { // other minions
    C_TEST_PASS;
  }

}

void test_control(){
  // for each small test, wait for EXPECTED_CYCLES and go to next one (by sending msip)
  
  __asm__ __volatile__ ("csrwi mhpmevent3, 1"); // to count cycles
  for (unsigned i = 0; i < NR_TESTS; i++) {
    // wait for notification from N0 to start
    // set validation3, just for waveform debug purposes
    __asm__ __volatile__
      (
       "li t0, -1\n"
       "csrw validation3, t0\n"
       ".rept %[n_credits]\n" 
       "csrw fcc, x0\n"
       ".endr\n"
       "csrw validation3, %[d]\n"
       : : [d] "r" (i), [n_credits] "i" (THREADS_PER_NEIGH) : "t0");

    CRED_INC_N0; // tell neigh 0 to start looping
    
    // start counting

    __asm__ __volatile__ ("csrwi mhpmcounter3, 0"); // reset nr cycles
    uint64_t cycles;
    do {
      __asm__ __volatile__ ("csrr %[cycles], mhpmcounter3" : [cycles] "=r" (cycles));
    } while ( cycles < EXPECTED_CYCLES);
    
    // trigger msip in n0, so that it advances to the next iterations
    TRIGGER_MSIP_N0;
  }

  C_TEST_PASS;
}

void run_tests(unsigned id){
 
  // before starting, setup trap handler (for MSIP)
  __asm__ __volatile__ 
    (
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     "csrwi mie, 8\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
     );
  

  // run all the tests... it will end the infinite loop because of the 'msip'
  // end of loop address has been saved in mscratch
  for (unsigned i = 0; i < NR_TESTS; i++) {
    test_wrapper::fnc(id, i);
  }
  C_TEST_PASS;
}


void trap_handler(){
  // set mepc = mscratch 
  __asm__ __volatile__
    (
     "csrr t0, mscratch\n"
     "csrw mepc, t0\n"
     : : : "t0"
     );
  // clear msip
  unsigned id = get_hart_id();
  CLEAR_MSIP(id);
  __asm__ __volatile__ ("fence");
  
}


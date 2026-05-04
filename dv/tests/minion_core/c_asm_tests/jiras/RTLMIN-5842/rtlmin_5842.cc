/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"

#define EXPECTED_CYCLES 8000
#define CREDIT_INC_AD_T0   0x803400C0
#define CREDIT_INC_AD_T1   0x803400D0
#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))

extern "C" void   __attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) trap_handler();


void test_muldiv(){
  // before starting, setup trap handler (for MSIP)
  __asm__ __volatile__ 
    (
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     "csrwi mie, 8\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
     );
  

  
  uint64_t res1, res2;
  uint64_t d[4] = {1,2,3,4};

  // sync with h2
  *((volatile uint64_t*) CREDIT_INC_AD_T0   ) = 0x2;
  __asm__ __volatile__("csrw fcc, x0\n");

  // and loop
  while(true){
    __asm__ __volatile__
      (
       ".rept 256\n"
       "mul %0, %2, %3\n"
       "div %1, %4, %5 \n"
       ".endr\n"
       : "=&r" (res1), "=&r" (res2)
       : "r" (d[0]), "r" (d[1]), "r" (d[2]), "r" (d[3])
       );
    d[0]=d[0] + res1;
    d[1]=d[1] + res2;
    d[2]++;
    d[3]++;
  }
  
}
void test_control(){
  // for each small test, wait for EXPECTED_CYCLES and go to next one (by sending msip)
  __asm__ __volatile__ ("csrwi mhpmevent3, 1"); // to count cycles

    // wait for notification harts 0 and 1, and tell them to start
  __asm__ __volatile__
      (
       "csrw fcc, x0\n"
       "csrw fcc, x0\n"       
       );
  *((volatile uint64_t*) CREDIT_INC_AD_T0   ) = 0x1;  
  *((volatile uint64_t*) CREDIT_INC_AD_T1    ) = 0x1;
  
  __asm__ __volatile__ ("csrwi mhpmcounter3, 0"); // reset nr cycles
  uint64_t cycles;
  do {
    __asm__ __volatile__ ("csrr %[cycles], mhpmcounter3" : [cycles] "=r" (cycles));
  } while ( cycles < EXPECTED_CYCLES);
  
  // trigger msip to tell minion 0 to stop looping
  *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 0x3;
  
  C_TEST_PASS;
}


void trap_handler(){
  // clears the interrupt and ends the test
  unsigned id = get_hart_id() % 64;
  *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1<<id;
  __asm__ __volatile__ ("fence");
  C_TEST_PASS
}


int main() {
  C_TEST_START;
  unsigned id = get_hart_id();

  switch(id) {
  case 0:
  case 1:
    test_muldiv();
    break;
  case 2:
    test_control();
    break;
  default:
    break;
  }
  C_TEST_PASS;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "trap_delegate.h"

volatile int mhandler_hits=0;
volatile int shandler_hits=0;
volatile int err=0;

int main(){
  C_TEST_START;

  unsigned id = get_hart_id();
  if ((id % 2) == 0){
    // configure machine and supervisor trap vectors
    __asm__ __volatile__ 
     (
      "la t0, machine_handler\n"
      "csrw mtvec, t0\n"
      "la t0, supervisor_handler\n"
      "csrw stvec, t0\n"
      : : : "t0"
     );

    // Switches to U-mode
    m_to_u_mode();

    for (int i=0; i<ITERATIONS; i++){
    //for (int i=0; i<1; i++){
      __asm__ __volatile__ 
      (
       "ecall\n"
       "nop\n"
      );
    }

    if (mhandler_hits != 1){
      err=3;
      C_TEST_FAIL;
    }

    if (shandler_hits != ITERATIONS-1){
      err=4;
      C_TEST_FAIL;
    }
  }

  C_TEST_PASS;

  return 0;
}
unsigned int get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], mhartid\n"
      : [ret] "=r" (ret)
      : 
      :
    );
    return ret;
}

// Jump thread to user mode
void m_to_u_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t0"
     );
}

void machine_handler()
{
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  if (cause == CAUSE_USER_ECALL) {
    mhandler_hits++;

    // Delegate "user ecall" exception to S-mode
    __asm__ __volatile__ 
    (
     "li t0, 0x100\n"
     "csrw medeleg, t0\n"
     : : : "t0"
    );  

    /**
    // Delegate "user ecall" exception to M-mode
    __asm__ __volatile__ 
    (
     "li t0, 0x1800\n"
     "csrw medeleg, t0\n"
     : : : "t0"
    );  
    **/


    // Increase PC to skip ecall
    __asm__ __volatile__ 
      (
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       : : : "t0"
      );
  /**
  }else if (cause == CAUSE_MACHINE_ECALL) {
    mhandler_hits++;

    
    // Delegate "user ecall" exception to S-mode
    __asm__ __volatile__ 
    (
     "li t0, 0x100\n"
     "csrw medeleg, t0\n"
     : : : "t0"
    );  
   
    // Increase PC to skip ecall
    __asm__ __volatile__ 
      (
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       : : : "t0"
      );
    **/

  } else if (cause == CAUSE_SUPERVISOR_ECALL){
    // return to m-mode
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );

    /**
    // Delegate "user ecall" exception to U-mode
    __asm__ __volatile__ 
    (
     "li t0, 0x000\n"
     "csrw medeleg, t0\n"
     : : : "t0"
    );  
    **/

    // Increase PC to skip ecall and the loop
    __asm__ __volatile__ 
      (
       "csrr t0, sepc\n"
       "addi t0, t0, 16\n"
       "csrw mepc, t0\n"
       : : : "t0"
      );

  } else {
    err=1;
    C_TEST_FAIL;
  }
}

void supervisor_handler()
{
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], scause\n" : [cause] "=r" (cause));
  
  if (cause == CAUSE_USER_ECALL) {
    shandler_hits++;

    if (shandler_hits == (ITERATIONS -1)){
    __asm__ __volatile__ 
      (
       "ecall\n"
       "nop\n"
      );
    } else {
      // Increase PC to skip ecall
      __asm__ __volatile__ 
        (
         "csrr t0, sepc\n"
         "addi t0, t0, 4\n"
         "csrw sepc, t0\n"
         : : : "t0"
        );
    }
  } else {
    err=2;
    C_TEST_FAIL;
  }
}


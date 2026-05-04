/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "mtip.h"

// t0: will loop, wfi in each iteration. When awaken by timer interrupt (programmed by t2) it increments the number of received interrupts, gives credits to t2 and wfi again.
// t1: will be awakening to the timer interrupt together with t0. It does not modify the state and just wfi again
// t2: programs the timer that will interrupt t0/t1, and waits for fcc credits. t0 gives it credits when it processes the interrupt 

int main(){
  C_TEST_START;

  unsigned id = get_hart_id();
  switch(id)
    {
    case 0: test_t0(); break;
    case 1: test_t1(); break;
    case 2: test_t2(); break;
    default:
      C_TEST_PASS; break;
    }

  return 0;
}
volatile int irq_cnt=0;
volatile int err=0;

void test_t0(){
  irq_cnt = 0;

  __asm__ __volatile__ ("csrw validation3, x0\n");
  
  // configure trap vector
  // and enable interrupts
  __asm__ __volatile__ 
    (
     "la t0, mtrap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     "li t0, 0x80\n"
     "csrw mie, t0\n"
     "csrsi mstatus, 0x08\n"
     : : : "t0"
     );
  
  CRED_INC; // inc fcc for thread2

  for ( int i = 0; i< ITERATIONS; i++) {
    __asm__ __volatile__ ("wfi\n");

    CRED_INC; // inc fcc for thread2
  }

  // Disable interrupts
  /*  __asm__ __volatile__ 
    (
     "csrw mie, 0x0\n"
     "li t0, 0x88\n"
     "csrc mstatus, t0\n"
     : : : "t0"
     );*/

  if (irq_cnt == ITERATIONS) {
    C_TEST_PASS;
  } 
  else {
    C_TEST_FAIL;
  }  
}

void test_t1(){
  
  for ( int i = 0; i< ITERATIONS; i++) {
    __asm__ __volatile__ ("wfi\n");
    for ( int j = 0; j<100; j++) {
      __asm__ __volatile__ ("add x0, x0, 0\n");
    }
  }

  C_TEST_PASS;
 
}


void test_t2(){

  uint64_t curr_time, prog_time;
  uint64_t tick = 0x10;

  // FCC wait
  __asm__ __volatile__ ("csrw fcc, x0\n");

  for ( int i = 0; i< ITERATIONS; i++) {
    curr_time = get_timer_ticks();
    prog_time = curr_time + tick;
    set_timer_ticks(prog_time);

    // Program the ESR shire (local_target) 
    SET_MTIME_TARGET;

    __asm__ __volatile__ ("csrw fcc, x0\n");

  }

  if (err) {
    C_TEST_FAIL;
  }
  else {
    C_TEST_PASS;
  }
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

void bad_trap(){
  err=1;
  C_TEST_FAIL;
}

void irq_handler()
{
  uint64_t cause;
  unsigned id = get_hart_id();

  if (id==0){
    __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
    
    // error in receiving an exception other than page fault
    if (cause != EXP_CAUSE) {
      err=1;
      C_TEST_FAIL;
    } else {
      // Write max value to mtimecmp 
      set_timer_ticks(0xFFFFFFFF);    
      CLEAR_MTIME_TARGET;

      // set val3 to irq cnt (to see in fsdb)
      dump_irq_count(++irq_cnt);    
    }
  }
}

void dump_irq_count(int cnt){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], validation3\n" : [v] "=r" (v));
  v &= ~0x000000000000FFFFULL;
  v |= cnt & 0xFFFF;
  __asm__ __volatile__ ("csrw validation3, %[v]\n" : : [v] "r" (v));
}

uint64_t get_timer_ticks()
{
    int ret;
    uint64_t taddr = R_PU_RVTIMER;
    __asm__ __volatile__ (
       "li t0, %[taddr]\n"
       "ld %[ret], 0(t0)\n"
      : [ret] "=r" (ret)
      : [taddr] "i" (taddr)
      : "t0"
    );
    return ret;
}

void set_timer_ticks(int cnt)
{
    uint64_t taddr = R_PU_RVTIMER;
    __asm__ __volatile__ (
       "li t0, %[taddr]\n"
       "mv t1, %[cnt]\n"
       "sd t1, 8(t0)\n"
      :
      : [cnt] "r" (cnt),
        [taddr] "i" (taddr)
      : "t0", "t1"
    );
}


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "msip.h"
#include "et_postsi_common.h"

// t1: will loop, waiting for FCC and trigger MSIP for t0
// t0: will inc FCC for t1, then loop waiting for interrupts to be
//     triggered while doing ld/sd, csr stall or spinlock. once the
//     interrupt is received, it will inc fcc for t1 so that it can trigger the next one

int main(){
  C_TEST_START;

  unsigned id = get_hart_id();
  switch(id)
    {
    case 0: test_t0(); break;
    case 1: test_t1(); break;
    default:
      c_test_pass(); break;
    }

  return 0;
}
volatile int irq_cnt=0;
volatile int stalling = 0;
volatile int err=0;
uint8_t mem_ptr[64*4];

void test_t0(){
  stalling = irq_cnt = 0;

  __asm__ __volatile__ ("csrw mscratch, x0\n");
  
  // configure trap vector
  // and enable interrupts
  __asm__ __volatile__ 
    (
     "la t0, mtrap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     "csrwi mie, 8\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
     );
  
  CRED_INC; // inc fcc for thread1

  int it=0;
  for(;;){
    if (err) { C_TEST_FAIL; }
    else {
      dump_wait_cnt(it);
      switch(it%4)
        {
        case 0:
          dump_wait_type(0);
          // loads
          __asm__ __volatile__ (
                                "ld t5, %[irq_cnt]\n" // save current irq cnt in t5
                                "2: li t0, 64*4\n"
                                "li t3, 0x0000FFFFFFFFFFFF\n"
                                "mv t4,%[ptr]\n"                              
                                "1:addi t0, t0, -64\n"
                                "ld x0, 0(t4)\n"
                                "sd t0, 0(t4)\n"
                                "addi t4, t4, 64\n"
                                "csrr t2, mscratch\n"
                                "mv t1, t0\n"                              
                                "slli t1, t1, 48\n"
                                "and t2, t2, t3\n"
                                "or t2,t1,t2\n"
                                "csrw mscratch, t2\n"
                                "bne t0, zero, 1b\n"
                                "ld t0,%[irq_cnt]\n" // retrieve irq cnt in t0
                                "beq t5, t0, 2b\n"    //and compare with initial. loop until it changes
                                :
                                : [ptr] "r" (mem_ptr), [irq_cnt] "m" (irq_cnt)
                                : "t0", "t1", "t2", "t3", "t4", "t5");
          break;
        case 1:
        case 2:
          dump_wait_type(1);
          // stall CSR
          __asm__ __volatile__
            ( "csrw stall, x0\n"
              "csrw stall, x0\n"
              "csrw stall, x0\n"
              "csrw stall, x0\n"
              "nop\n");
          break;
          
        case 3:
          dump_wait_type(2);
          // stall with spinlock
          stalling = 2;
          __asm__ __volatile__ ("1: j 1b\nnop\n");;
          break;
        default:
          break;
        }
      it++;
    }
  }
  
}

void test_t1(){
  int  delay[] = { 10, 5, 20, 30, 12, 16, 40, 16};
  int delay_ptr = 0;
  for ( int i = 0; i< ITERATIONS; i++) {
    
    // FCC wait
    __asm__ __volatile__ ("csrw fcc, x0\n");

    if (err) {
      C_TEST_FAIL;
    }

    if ( irq_cnt > i) {
      C_TEST_FAIL;
    }
    
    // trigger msip
    TRIGGER_MSIP;

    //delay
    int d = delay[delay_ptr];
    while(d-->=0){
      __asm__ __volatile__ ("nop\nnop\nnop\nnop\n");
    }

    delay_ptr++;
    delay_ptr%=8;
  }
  c_test_pass();
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
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  // error in receiving an exception other than page fault
  if ( cause != EXP_CAUSE) {
    err=1;
    C_TEST_FAIL;
  }
  else{
    // clear the interrupt
    CLEAR_MSIP;
    
    // set val3 to irq cnt (to see in fsdb)
    dump_irq_count(++irq_cnt);
    
    // notify t1
    if (irq_cnt != ITERATIONS)
      CRED_INC; // inc fcc for thread1
    
    __asm__ __volatile__ ("fence\n");
    
    if ( irq_cnt == ITERATIONS) {
      for(;;) c_test_pass();
    }
    
    // if done with stalling, set mepc to +4, so that we skip the 'stall'
    if (--stalling == 0){
      __asm__ __volatile__
        (
         "csrr t0, mepc\n"
         "addi t0, t0, 4\n"
         "csrw mepc, t0\n"
         : : : "t0");                    
      
    }
  }
}



void dump_irq_count(int cnt){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], mscratch\n" : [v] "=r" (v));
  v&=~0x000000000000FFFFULL;
  v|=cnt & 0xFFFF;
  __asm__ __volatile__ ("csrw mscratch, %[v]\n" : : [v] "r" (v));
}


void dump_wait_type(int t){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], mscratch\n" : [v] "=r" (v));
  v&=~0x00000000FFFF0000ULL;
  v|=(t&0xFFFF)<<16;
  __asm__ __volatile__ ("csrw mscratch, %[v]\n" : : [v] "r" (v));
}


void dump_wait_cnt(int c){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], mscratch\n" : [v] "=r" (v));
  v&=~0x0000FFFF00000000ULL;
  v|=uint64_t(c&0xFFFF)<<32ULL;
  __asm__ __volatile__ ("csrw mscratch, %[v]\n" : : [v] "r" (v));
}

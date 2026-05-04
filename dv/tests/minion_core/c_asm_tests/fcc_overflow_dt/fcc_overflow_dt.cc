/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "fcc_overflow_dt.h"
#include "et_postsi_common.h"

// We test four different cases, in each of them different counter is overflown (T0C0, T0C1, T1C0, T1C1)
// One thread is stalling, while all the others are giving it credits until it overflows. 
// Each threads send the credit to t1 when they are done with fcc increments to t0. t1 then awakes the first thread with the interrupt. 
// The awaken thread check that fcc overflow bit is correctly set in TensorError

int main(){
  C_TEST_START;

  unsigned id = get_hart_id();
  switch(id)
    {
    case 0: test_t0(); break;
    case 1: test_t1(); break;
    default:
      test_others(); break;
    }

  return 0;
}

volatile int irq_cnt=0;
volatile int err=0;

void enable_interrupts(){
  // configure trap vector and enable interrupts
  __asm__ __volatile__ 
    (
     "la t0, mtrap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     "csrwi mie, 8\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
     );
}

void produce_fcc_overflow(int cnt){
 
  if (cnt == 0){
    for (int j=0; j<CREDIT_LOOP; j++){
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0;
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0; 
      CRED_INC_T0_C0;
    }
  } 
  else if (cnt == 1){
   for (int j=0; j<CREDIT_LOOP; j++){
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1;
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1; 
      CRED_INC_T0_C1;
    }
  }
  else if (cnt == 2){
   for (int j=0; j<CREDIT_LOOP; j++){
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0;
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0; 
      CRED_INC_T1_C0;
    }
  }
  else if (cnt == 3){
   for (int j=0; j<CREDIT_LOOP; j++){
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1;
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1; 
      CRED_INC_T1_C1;
    }
  }
}

void receive_fcc_overflow(int sync_t){

  if (sync_t == 0){
    CRED_INC_T0_C0;
  } else {
    CRED_INC_T1_C0;
  }
  
  // wait for interrupt
  __asm__ __volatile__ ("csrw stall, x0\n");

  // wait for fccs to complete before checking the status
  __asm__ __volatile__ ("fence\n");

  uint64_t terror;
  __asm__ __volatile__ ("csrr %[terror], tensor_error\n" : [terror] "=r" (terror));
  
  // fcc overflow not detected
  if ( terror != TERROR_FCC_OVERFLOW) {
    err=3;
    C_TEST_FAIL;
  }

  // Clear error status
  __asm__ __volatile__ ("csrw tensor_error, x0\n");
        
  if (sync_t == 0){
    CRED_INC_T0_C0;
  } else {
    CRED_INC_T1_C0;
  }
}

void test_t0(){

  // Set interrupt routine 
  enable_interrupts();
 
  // Sync with t1
  CRED_INC_T1_C0;
  __asm__ __volatile__ ("csrw fcc, x0\n");

  __asm__ __volatile__ ("csrw validation3, x0\n");

  for ( int i = 0; i< ITERATIONS; i++) {
    switch(i%4)
    {
      case 0:
        // Overflow T0 C0
        dump_event_type(0);
        receive_fcc_overflow(1);
        break;
      case 1:
        // Overflow T0 C1
        dump_event_type(1);
        receive_fcc_overflow(1);
        break;
      case 2:
        // Overflow T1 C0
        __asm__ __volatile__ ("csrw fcc, x0\n");
        dump_event_type(2);
        CRED_INC_ALL_BUT_M0_EVEN;
        CRED_INC_ALL_BUT_M0_ODD;
        produce_fcc_overflow(2);

        // Additional 16 credits to overflow
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 
        CRED_INC_T1_C0; 

        for (int j =0; j<(MINION_PER_SHIRE-2); j++){
          __asm__ __volatile__ ("csrw fcc, x0\n");
        }
        TRIGGER_MSIP_T1;  
        __asm__ __volatile__ ("csrw fcc, x0\n");
        break;
      case 3:
        // Overflow T1 C1
        dump_event_type(3);
        CRED_INC_ALL_BUT_M0_EVEN;
        CRED_INC_ALL_BUT_M0_ODD;
        __asm__ __volatile__ ("csrw fcc, x0\n");
        produce_fcc_overflow(3);

        // Additional 16 credits to overflow
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1;         
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1;         
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 
        CRED_INC_T1_C1; 

        for (int j =0; j<(MINION_PER_SHIRE-2); j++){
          __asm__ __volatile__ ("csrw fcc, x0\n");
        }
        TRIGGER_MSIP_T1;  
        __asm__ __volatile__ ("csrw fcc, x0\n");
        break;  
      default:
        break;
      }
  }   

  c_test_pass();
 
}

void test_t1(){

  // Set interrupt routine 
  enable_interrupts();
 
  // Sync with t0
  __asm__ __volatile__ ("csrw fcc, x0\n");
  CRED_INC_T0_C0; 
 
  for ( int i = 0; i< ITERATIONS; i++) {
    switch(i%4)
    {
      case 0:
        // Overflow T0 C0
       __asm__ __volatile__ ("csrw fcc, x0\n");
        CRED_INC_ALL_BUT_M0_EVEN;
        CRED_INC_ALL_BUT_M0_ODD;
        produce_fcc_overflow(0);
        
        // Additional 16 credits to overflow
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 
        CRED_INC_T0_C0; 

        for (int j =0; j<(MINION_PER_SHIRE-2); j++){
          __asm__ __volatile__ ("csrw fcc, x0\n");
        }
        TRIGGER_MSIP_T0;  
        __asm__ __volatile__ ("csrw fcc, x0\n");
        break;
      case 1:
        // Overflow T0 C1
       __asm__ __volatile__ ("csrw fcc, x0\n");
        CRED_INC_ALL_BUT_M0_EVEN;
        CRED_INC_ALL_BUT_M0_ODD;
        produce_fcc_overflow(1);

        // Additional 16 credits to overflow
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 
        CRED_INC_T0_C1; 

        for (int j =0; j<(MINION_PER_SHIRE-2); j++){
          __asm__ __volatile__ ("csrw fcc, x0\n");
        }
        TRIGGER_MSIP_T0;  
        __asm__ __volatile__ ("csrw fcc, x0\n");
        break;
      case 2:
      case 3:
        // Overflow T1 C0
        // Overflow T1 C1
        receive_fcc_overflow(0);
        break;  
      default:
        break;
      }
  }   

  c_test_pass();
}

void test_others(){

  for ( int i = 0; i< ITERATIONS; i++) {
    switch(i%4)
    {
      case 0:
        // Overflow T0 C0
        __asm__ __volatile__ ("csrw fcc, x0\n");
        produce_fcc_overflow(0);
        CRED_INC_T1_C0;
        break;
      case 1:
        // Overflow T0 C1
        __asm__ __volatile__ ("csrw fcc, x0\n");
        produce_fcc_overflow(1);
        CRED_INC_T1_C0;
        break;
      case 2:
        // Overflow T1 C0
        __asm__ __volatile__ ("csrw fcc, x0\n");
        produce_fcc_overflow(2);
        CRED_INC_T0_C0;
        break;
      case 3:
        // Overflow T1 C1
        __asm__ __volatile__ ("csrw fcc, x0\n");
        produce_fcc_overflow(3);
        CRED_INC_T0_C0;
        break;  
      default:
        break;
      }
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
    unsigned id = get_hart_id();
    switch(id)
      {
      case 0: CLEAR_MSIP_T0; break;
      case 1: CLEAR_MSIP_T1; break;
      default:
        break;
    }            
  }
}

void dump_event_type(int t){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], validation3\n" : [v] "=r" (v));
  v&=~0x000000000000FFFFULL;
  v|=t & 0xFFFF;
  __asm__ __volatile__ ("csrw validation3, %[v]\n" : : [v] "r" (v));
}




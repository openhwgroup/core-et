/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "seip_deleg.h"
#include "et_postsi_common.h"

// t0: will program the trap vector, delegates external interrupts to supervisor, 
// then change to supervisor mode, then loops, stalling in each iteration. 
// When awaken by external interrupt (programmed by t1) it jumps to machine mode
// Then, in machine interrupt handler, it forces a supervisor external interrupt.
// enables supervisor interrupts, and clears the hook in validation registers
// After that, it jumps to supervisor interrupt handler, which first clears the
// supervisor external interrupt by jumping again to machine mode (ecall)
// When cleared, it calls back to supervisor handler, which counts how
// many interrupts we have handled.
// Then it gives credits to t1 and stalls again.
// t1: programs the external device that will interrupt t0, 
/// and waits for fcc credits. 
// t0 gives it credits when it processes the interrupt 

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
volatile int err=0;

void test_t0()
{
  irq_cnt = 0;

  __asm__ __volatile__ ("csrw validation3, x0\n");
  
  // configure machine trap vector
  // and enable machine-mode interrupts
  __asm__ __volatile__ 
    (
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     "li t0, 0x800\n"
     "csrw mie, t0\n"
     "csrsi mstatus, 0x08\n"
     : : : "t0"
    );

  // delegate supervisor external interrupts
  __asm__ __volatile__ 
     (
      "li t0, 0x200\n"
      "csrw mideleg, t0\n"
      : : : "t0"
     );  

  // jump to supervisor mode
  m_to_s_mode();

  // configure supervisor trap vector
  __asm__ __volatile__ 
    (
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw stvec, t0\n"
     : : : "t0"
    );

  // and enable supervisor interrupts
  __asm__ __volatile__ 
    (
     "li t0, 0x200\n"
     "csrw sie, t0\n"
     "csrsi sstatus, 0x02\n"
     : : : "t0"
    );

  // inc fcc for thread1
  CRED_INC; 

  for (int i = 0; i< ITERATIONS; i++) {
    // stall, wait for external interrupt 
    __asm__ __volatile__ ("wfi\n");
    // inc fcc for thread1
    CRED_INC; 
  }

  if (irq_cnt == ITERATIONS) {
    c_test_pass();
  } 
  else {
    C_TEST_FAIL;
  }  
}

void test_t1()
{
  // FCC wait
  __asm__ __volatile__ ("csrw fcc, x0\n");

  for (int i = 0; i< ITERATIONS; i++) {
    set_external_interrupt(true);
    __asm__ __volatile__ ("csrw fcc, x0\n");
  }

  if (err) {
    C_TEST_FAIL;
  }
  else {
    c_test_pass();
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

// Jump thread to supervisor mode
void m_to_s_mode() 
{
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6800\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to supervisor-mode
     : 
     : 
     : "t0"
     );
}

void bad_trap(){
  err=1;
  C_TEST_FAIL;
}

void machine_handler()
{
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  // error in receiving an exception other than page fault
  if (cause == CAUSE_SUPERVISOR_ECALL) {
    // clear supervisor external interrupts
    __asm__ __volatile__ 
      (
       //"csrr t1, mip\n"
       "li t0, 0x200\n"
       //"or t0, t0, t1\n"
       "csrc mip, t0\n"
       : : : "t0"
      );

    // Increase PC to skip ecall
    __asm__ __volatile__ 
      (
       //"csrr t1, mip\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       //"or t0, t0, t1\n"
       "csrw mepc, t0\n"
       : : : "t0"
      );

  } else if (cause == CAUSE_MACHINE_EXTERNAL_INTR) {
    // clear external interrupts
    set_external_interrupt(false);

    // set supervisor external interrupts
    __asm__ __volatile__ 
      (
       //"csrr t1, mip\n"
       "li t0, 0x200\n"
       //"or t0, t0, t1\n"
       "csrs mip, t0\n"
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

  // Check cause using scause
  __asm__ __volatile__ ("csrr %[cause], scause\n" : [cause] "=r" (cause));
  
  // error in receiving an exception other than page fault
  if (cause != CAUSE_SUPERVISOR_EXTERNAL_INTR) {
    err=1;
    C_TEST_FAIL;
  } else {
    // call m-handler to clear the mip register (must be done in M-mode)
    __asm__ __volatile__ 
      (
       "ecall\n"
      );

    // increase counter
    dump_irq_count(++irq_cnt);    
  }
}

void dump_irq_count(int cnt){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], validation3\n" : [v] "=r" (v));
  v &= ~0x000000000000FFFFULL;
  v |= cnt & 0xFFFF;
  __asm__ __volatile__ ("csrw validation3, %[v]\n" : : [v] "r" (v));
}

void set_external_interrupt(bool inject)
{
    uint64_t opcode = ((uint64_t) 0x5 << 56);
    uint64_t set = (uint64_t) (inject ? 1 : 0);
    uint64_t clear = set << 55;
    uint64_t val1 = opcode | clear;
    uint64_t shire_id = (get_hart_id() >> 6);
    uint64_t minion_id = (get_hart_id() && 0x3F);
    uint64_t shire_mask = 1 << shire_id;
    val1 |= shire_mask;
    uint64_t minion_mask = 1 << minion_id;
    uint64_t val2 = minion_mask;
    if (inject) {
      // Set thread 0 of minion if inject
      val2 = val2 >> 1;
    }

  __asm__ __volatile__ ("csrw validation2, %[val2]\n" : : [val2] "r" (val2));
  __asm__ __volatile__ ("csrw validation1, %[val1]\n" : : [val1] "r" (val1));
}


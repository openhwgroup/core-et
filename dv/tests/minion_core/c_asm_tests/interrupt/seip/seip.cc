/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "seip.h"
#include "et_postsi_common.h"

// t0: will loop, stalling in each iteration. 
// When awaken by external interrupt (programmed by t1) it increments 
// the number of received interrupts
// Then it gives credits to t1 and stalls again.
// t1: programs the external device that will interrupt t0, 
/// and waits for fcc credits. 
// t0 gives it credits when it processes the interrupt 

int main(){
  C_TEST_START;

  unsigned id = get_mhart_id();
  switch(id)
  {
    case 0: test_t0(); break;
    case 1: test_t1(); break;
    default:
      while (true) {
        // interate on TEST_PASS as interrupt reaches all minions and
        // wakes them up
        c_test_pass();
      }
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
     "li t0, 0x800\n"
     "csrw mie, t0\n"
     "csrsi mstatus, 0x08\n"
     : : : "t0"
    );

  // enable shadow registers for hartid and sleep txfma
  __asm__ __volatile__ ("csrwi menable_shadows, 0x1");

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
  
  CRED_INC; // inc fcc for thread1

  for (int i = 0; i< ITERATIONS; i++) {
    // Stall, wait for external interrupt 
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

void test_t1(){

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

unsigned int get_mhart_id()
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

unsigned int get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], hartid\n"
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
  __asm__ __volatile__ ("csrr %[cause], scause\n" : [cause] "=r" (cause));
  
  // error in receiving an exception other than page fault
  if (cause != EXP_CAUSE) {
    err=1;
    C_TEST_FAIL;
  } else {
    // clear external interrupts
    set_external_interrupt(false);
    // set val3 to irq cnt (to see in fsdb)
    dump_irq_count(++irq_cnt);    
  }
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
    uint64_t supervisor = ((uint64_t) 0x2 << 53);
    uint64_t val1 = opcode | clear | supervisor;
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


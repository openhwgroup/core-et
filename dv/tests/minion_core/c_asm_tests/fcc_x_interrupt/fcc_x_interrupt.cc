/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "fcc_x_interrupt.h"
#include "et_postsi_common.h"

#include "et_test_common.h"
#include "fcc.h"


// Hart 0:
// Try to do CSR write to FCC and is stalled/blocked (waiting for FCC credit);
// when awaken by external interrupt (programmed by Minion 1 Hart 2), it is cleared from
// its "stalled/blocked" state, it increments the number of received interrupts; 
// after interrupt trap handling, it replays CSR-write-FCC instruction and will stall again 
// (if FCC credit still unavailable).
//
// Hart 2: 
// after some delay (to make sure Hart 0 already did CSR write FCC first);
// programs the external device that will interrupt Hart 0; 
// then after another delay (could be shorter or longer),
// gives FCC credits to Hart 0;
//



int main(){
  C_TEST_START;

  unsigned hart_id = get_hart_id();
  switch(hart_id)
    {
  #ifdef CSRW_FCC_TH1
    case 1: test_hart_csrw_fcc_stall(); break;    // minion 0 thread 1 
    case 0: test_hart_send_interrupt(); break;    // minion 0 thread 0
  #else
    case 0: test_hart_csrw_fcc_stall(); break;    // minion 0 thread 0
    case 1: test_hart_send_interrupt(); break;    // minion 0 thread 1
  #endif
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


// minion 0 thread 0
void test_hart_csrw_fcc_stall(){
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
    FENCE;

     // do CSR write to wait for FCC credit:
     //__asm__ __volatile__ ("csrw fcc, x0\n");
   #ifdef CSRW_FCC1
     wait_for_credit(1);  // credit_counter_num = 1 (FCC1)
   #else
     wait_for_credit(0);  // credit_counter_num = 0 (FCC0)
   #endif
     c_test_pass();
}


// minion 1 thread 0
void test_hart_send_interrupt(){

  // program external device to interrup Hart0;
  // and then send FCC credit to Hart0;

     // add some delay: wait after Hart0 completes its configure trap vector and enable interrupts;
     delay(50);
   #ifndef WITHOUT_INTERRUPT
     set_external_interrupt(true);
   #endif

   #ifdef LONGER_DELAY_B4_GIVE_CREDIT
     delay(100);
   #else
     delay(20);
   #endif

     // ESR write to FCC to increment credit for thread 0:
   #ifdef CSRW_FCC_TH1
    #ifdef CSRW_FCC1
     give_credit_local(0, 1, 1);   // minion=0 thread=1 credit_counter_num=1
    #else 
     give_credit_local(0, 1, 0);   // minion=0 thread=1 credit_counter_num=0
    #endif
   #else
    #ifdef CSRW_FCC1
     give_credit_local(0, 0, 1);   // minion=0 thread=0 credit_counter_num=1
    #else 
     give_credit_local(0, 0, 0);   // minion=0 thread=0 credit_counter_num=0
    #endif
   #endif

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

void dump_irq_count(int cnt){
  uint64_t v;
  __asm__ __volatile__ ("csrr %[v], validation3\n" : [v] "=r" (v));
  v &= ~0x000000000000FFFFULL;
  v |= cnt & 0xFFFF;
  __asm__ __volatile__ ("csrw validation3, %[v]\n" : : [v] "r" (v));
}

void set_external_interrupt(bool inject)
{
  volatile uint64_t hart_id = get_hart_id();    // getHartId();
  //volatile uint64_t thread_id = (hart_id & 0x1);
  //volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);  

    uint64_t opcode = ((uint64_t) 0x5 << 56);     // validation1[63:56]: opcode=0x5 DIAG_IRQ_INJ
    uint64_t set = (uint64_t) (inject ? 1 : 0);   
    uint64_t clear = set << 55;                   // validation1[55]: raise/clear     
    uint64_t val1 = opcode | clear;
    //uint64_t shire_id = (get_hart_id() >> 6) & 0x3F;
    uint64_t shire_mask = 1 << shire_id;
    val1 |= shire_mask;

    uint64_t hart_id_per_shire = hart_id & 0x3F;   // 64 harts per shire 
    uint64_t hart_id_per_shire_mask = 1 << hart_id_per_shire;
    uint64_t val2 = hart_id_per_shire_mask;
    if (inject) {
      // send interrupt to "the other Thread" of the same minion:
      #ifdef CSRW_FCC_TH1    // meaning: Th1 doing CSR-write-FCC-stall, Th0 sending interrupt
        val2 = val2 << 1;
      #else                  // meaning: Th0 doing CSR-write-FCC-stall, Th1 sending interrupt
        val2 = val2 >> 1;
      #endif
    }

  __asm__ __volatile__ ("csrw validation2, %[val2]\n" : : [val2] "r" (val2));
  __asm__ __volatile__ ("csrw validation1, %[val1]\n" : : [val1] "r" (val1));
}


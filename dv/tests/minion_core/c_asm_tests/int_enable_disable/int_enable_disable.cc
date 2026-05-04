/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "esr_defines.h"
#include "int_enable_disable.h"


////////////////////////////////////////////////////////////////////////////////
// Test will have thread0 receiving interrupts and thread1 sending msip to t0.
// Thread0 will also trigger SSIP to itself, and change delegation and interrupt
// enable CSRs continuously, randomly.
//
// The same is then repeated exchanging thread0 and thread1
////////////////////////////////////////////////////////////////////////////////

int main() {
  C_TEST_START;
  unsigned prv = 3;
  enable_shadows();
  unsigned id = get_hart_id();
  __asm__ __volatile__ ("csrwi frm, 6\n"); // set invalid RM to have xctps
  
  for ( unsigned receiver = 0; receiver < 2; receiver++) {
    // always start test in m.mode
    while(prv != 3) privilege_up(prv);
    
    if ( id % 2 == receiver) {
      // receiver thread
      install_trap_handler(mtrap_handler_receiver, strap_handler_receiver);
      test_receiver(prv);
    } else {
      // sender thread
      install_trap_handler(mtrap_handler_sender, strap_handler_sender);
      test_sender();
    }
    
  }
  
  C_TEST_PASS;
}


void test_receiver(unsigned &prv){
  if (peek_fcc(0)> 0) {
    mstatus_mie_sie(false, false);
    C_TEST_FAIL
  }
  unsigned id = get_hart_id();
  unsigned rand_ptr = id;
  uint64_t debug = 0;
  // sync with other thread
  inc_fcc0( (id % 64)^1);
  WAIT_FCC0;

  do {
    debug &=0xFFFFFFFF;
    set_debug(debug, prv);
    RANDOM_EXEC(PROB_ILLEGAL_INSN / 3,  illegal_insn0(), 9);
    RANDOM_EXEC(PROB_CHANGE_PRV, privilege_up(prv, debug), 0);
    RANDOM_EXEC(PROB_CHANGE_PRV, privilege_down(prv, debug), 1);
    
    switch(prv){
    case 3:
      // trigger ssip
      RANDOM_EXEC(PROB_SET_SSIP, set_mip_ssip(true), 2);
      // change delegation of ssip
      RANDOM_EXEC(PROB_DELEG_SSIP, deleg_ssip(get_rand_bit(rand_ptr)), 3);
      // change interrupt enables
      RANDOM_EXEC(PROB_CHANGE_IE, mstatus_mie_sie(get_rand_bit(rand_ptr), get_rand_bit(rand_ptr)), 4);
      RANDOM_EXEC(PROB_CHANGE_IE, set_mie(get_rand_bit(rand_ptr), get_rand_bit(rand_ptr)), 5);
      break;
      
    case 1:
      RANDOM_EXEC(PROB_SET_SSIP, set_sip_ssip(true),6);
      RANDOM_EXEC(PROB_CHANGE_IE, sstatus_sie(get_rand_bit(rand_ptr)), 7);
      RANDOM_EXEC(PROB_CHANGE_IE, set_sie(get_rand_bit(rand_ptr)),8);
      break;
    }
    RANDOM_EXEC(PROB_ILLEGAL_INSN / 3,  illegal_insn1(), 10);
    RANDOM_EXEC(PROB_ILLEGAL_INSN / 3,  illegal_insn2(), 11);

    __asm__ __volatile__ (".rept 32\nnop\n.endr");
    
    set_debug(debug++, prv);
  } while (peek_fcc(0) == 0);

  // clear and disable interrupts, disable delegation
  while(prv != 3) privilege_up(prv);
  clear_msip();
  set_mip_ssip(false);
  deleg_ssip(false);
  // enable interrupts
  mstatus_mie_sie(false, false);

  // and sync with other thread
  WAIT_FCC0;
}

void test_sender(){
  unsigned id = get_hart_id();
  unsigned rand_ptr = id;
  unsigned other_thread = (id % 64) ^ 1;
  // clear all interrupts and disable them before starting
  clear_msip();
  set_mip_ssip(false);
  mstatus_mie_sie(true, true);


  // sync with other thread
  WAIT_FCC0;
  inc_fcc0(other_thread);
  
  // the sender will just continuously trigger interrupts to the other thread
  for ( uint64_t it=0, debug=0; it < ITERATIONS; it++, debug++) {
    debug = debug & 0xFFFFFFFF;
    for (uint64_t m = 0; m < MSIP_PER_ITERATION; m++, debug+=0x100000000ULL) {
      set_debug(debug, 3);
      trigger_msip(other_thread);
      do{ 
        random_delay(get_rand_percent(rand_ptr) >> 2);
      } while( peek_msip(other_thread));
      random_delay(get_rand_percent(rand_ptr) >> 2);
    }
  }
  // and nofity other thread we are done
  inc_fcc0(other_thread);
}

////////////////////////////////////////////////////////////////////////////////
// trap handlers
////////////////////////////////////////////////////////////////////////////////
void mtrap_handler_sender(){
  register uint64_t x11 asm("x11");
  mtrap_handler(false, x11);
}

void mtrap_handler_receiver(){
  register uint64_t x11 asm("x11");
  mtrap_handler(true, x11);
}

void strap_handler_sender(){
  strap_handler(false);
}

void strap_handler_receiver(){
  strap_handler(true);
}


void mtrap_handler(bool interrupts_allowed, uint64_t ecall_param) {
  uint64_t cause, status, epc;
  unsigned prv;
  // read cause, status and mepc
  __asm__ __volatile__ (
                        "csrr %[epc], mepc\n"
                        "csrr %[cause], mcause\n"
                        "csrr %[status], mstatus\n"
                        : [cause] "=r" (cause), [status] "=r" (status), [epc] "=r" (epc)
                        :
                        : 
                        );


  // error if an interrupt is received, and they were not allowed
  bool is_interrupt = (cause >> 63) == 1;
  if ( is_interrupt && !interrupts_allowed) {
    mstatus_mie_sie(false, false);
    C_TEST_FAIL;
  }

  // for xcpts, jump back to epc + 4, for interrupts, to epc
  if (!is_interrupt) {
    epc+=4;
    __asm__ __volatile ( "csrw mepc, %[epc]" : : [epc] "r" (epc));
  }
  

  switch(cause)
    {
      // for ecalls, select a new privilege mode

    case CAUSE_USER_ECALL:
    case CAUSE_SUPERVISOR_ECALL:
    case CAUSE_MACHINE_ECALL:
      prv = (status>>11)&3; // prv = mpp
      // note => ecall_param==0 is inc prv, ecall_param==1 is dec prv
      switch (prv){
      case 0: prv = ecall_param ? 0 : 1; break;
      case 1: prv = ecall_param ? 0 : 3; break;
      case 3: prv = ecall_param ? 1 : 3; break;
      }
      status &= ~( 3ULL << 11);
      status|= prv << 11;
      __asm__ __volatile__ ("csrw mstatus, %[status]\n" : : [status] "r" (status));
      break;

    case CAUSE_MSIP:
      clear_msip();
      break;
    case CAUSE_SSIP:
      set_mip_ssip(false);
      break;

    case CAUSE_ILLEGAL: // nothing to do, just skip (mepc+=4)
      break;
      
    // other causes => error
    default:
      mstatus_mie_sie(false, false);
      C_TEST_FAIL;
      return;
    }

}




void strap_handler(bool interrupts_allowed) {
  uint64_t cause, status, epc;
  // read cause, status and epc
  __asm__ __volatile__ (
                        "csrr %[epc], sepc\n"
                        "csrr %[cause], scause\n"
                        "csrr %[status], sstatus\n"
                        : [cause] "=r" (cause), [status] "=r" (status), [epc] "=r" (epc)
                        :
                        : 
                        );


  // error if an interrupt is received, and they were not allowed
  unsigned prv = 1;
  bool is_interrupt = (cause >> 63) == 1;
  if ( is_interrupt && !interrupts_allowed) {
    privilege_up(prv);
    mstatus_mie_sie(false, false);
    C_TEST_FAIL;
    while(true);
  }

  // for xcpts, jump back to epc + 4, for interrupts, to epc
  if (!is_interrupt) {
    epc+=4;
    __asm__ __volatile ( "csrw sepc, %[epc]" : : [epc] "r" (epc));
  }
  

  switch(cause)
    {
    case CAUSE_SSIP:
      set_sip_ssip(false);
      break;
    // other causes => error
    default:
      privilege_up(prv);
      mstatus_mie_sie(false, false);
      C_TEST_FAIL;
      while(true);
    }

}


////////////////////////////////////////////////////////////////////////////////
// helper routines
////////////////////////////////////////////////////////////////////////////////


void install_trap_handler(void (*m)(),void (*s)()){
  __asm__ __volatile__ ("csrw mtvec, %[m]\n"
                        "csrw stvec, %[s]\n"
                        : : [m] "r" (m), [s] "r" (s));
}

void mstatus_mie_sie(bool mie, bool sie){
  if (mie) __asm__ __volatile__ ("csrsi mstatus, 8");
  else     __asm__ __volatile__ ("csrci mstatus, 8");

  if (sie) __asm__ __volatile__ ("csrsi mstatus, 2");
  else     __asm__ __volatile__ ("csrci mstatus, 2");
}

void sstatus_sie(bool sie){
  if (sie) __asm__ __volatile__ ("csrsi sstatus, 2");
  else     __asm__ __volatile__ ("csrci sstatus, 2");
}

void set_mie(bool msie, bool ssie) {
  if (msie) __asm__ __volatile__ ("csrsi mie, 8");
  else      __asm__ __volatile__ ("csrci mie, 8");

  if (ssie) __asm__ __volatile__ ("csrsi mie, 2");
  else      __asm__ __volatile__ ("csrci mie, 2");
}

void set_mie(uint64_t x) {
  set_mie((x & 8) != 0, (x & 2) != 0);
}

void set_sie(bool ssie) {
  if (ssie) __asm__ __volatile__ ("csrsi sie, 2");
  else      __asm__ __volatile__ ("csrci sie, 2");
}
uint64_t get_mie() {
  uint64_t ie;
  __asm__ __volatile__ ("csrr %[ie], mie" : [ie] "=r" (ie));
  return (ie & 0xA) != 0;
}

bool get_sie() {
  uint64_t ie;
  __asm__ __volatile__ ("csrr %[ie], sie" : [ie] "=r" (ie));
  return (ie & 2) != 0;
}

void enable_shadows(){
  __asm__ __volatile__ ("csrwi menable_shadows, 0xf\n");
}

uint64_t peek_fcc(uint64_t cnt) {
  uint64_t fcc_nb;
  __asm__ __volatile__ ("csrr %[d], fccnb" : [d] "=r" (fcc_nb));
  return (fcc_nb >> (16*cnt)) &0xFFFF;
}

void clear_msip(){
  volatile uint64_t* clear = (volatile uint64_t*) ESR_SHIRE(0xFF, IPI_TRIGGER_CLEAR);
  uint64_t mask = 1ULL<< (get_hart_id() % 64);
  *clear = mask;
  __asm__ __volatile__ ("fence");
}

void trigger_msip(unsigned thread) {
  volatile uint64_t* trigger = (volatile uint64_t*) ESR_SHIRE(0xFF, IPI_TRIGGER);
  *trigger = 1ULL<<thread;
}


bool peek_msip(unsigned thread) {
  volatile uint64_t* trigger = (volatile uint64_t*) ESR_SHIRE(0xFF, IPI_TRIGGER);
  return ((*trigger) & (1ULL<<thread)) != 0;
}

void inc_fcc0(unsigned thread){
  volatile uint64_t* trigger = ( thread % 2 == 0) ?
    (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_0) :
    (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_2);
  *trigger = 1ULL<<(thread>>1);
  __asm__ __volatile__ ("fence");
}

void set_sip_ssip(bool en){
  if (en) __asm__ __volatile__ ("csrsi sip, 2");
  else __asm__ __volatile__ ("csrci sip, 2");
}

void set_mip_ssip(bool en){
  if (en) __asm__ __volatile__ ("csrsi mip, 2");
  else __asm__ __volatile__ ("csrci mip, 2");
}

void privilege_up(unsigned &prv, uint64_t debug){
  if (prv != 3) { //nothing to do if already in M mode
    // using ecall with x11 = 0
    prv = (prv == 0) ? 1 : 3;
    __asm__ __volatile__ 
      ( 
       "li x11, 0\n"
       "ecall\n"
       "nop\n"
       : : : "x11"
	);
    set_debug(debug, prv);
  }
}

void deleg_ssip(bool en) {
  if (en) __asm__ __volatile__ ("csrsi mideleg, 2");
  else __asm__ __volatile__ ("csrci mideleg, 2");
}



void privilege_down(unsigned &prv, uint64_t debug){
  // using ecall with x11 = 1
  if ( prv != 0 ) {
    prv = (prv == 3)? 1 : 0;
    __asm__ __volatile__ 
      ( 
       "li x11, 1\n"
       "ecall\n"
       "nop\n"
       : : : "x11"
	);
    set_debug(debug, prv);
  }
}

void set_debug(uint64_t d, unsigned prv) {
  __asm__ __volatile__ ("csrw validation3, %[d]\n" : : [d] "r" (d | (uint64_t(prv)<< 48)) );
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

void illegal_insn0() {
  __asm__ __volatile__ (".word 0\nnop\n"); //invalid opcode
}
  
void illegal_insn1() {
  __asm__ __volatile__ ("csrw 0, x0\nnop\n"); //invalid CSR
}

void illegal_insn2() {
  __asm__ __volatile__ ("fadd.s f0, f0, f0, dyn\nnop\n"); //invalid RM
}

void random_delay(unsigned d){
  for(; d!=0; d--){
    __asm__  __volatile__
      (".rept 8\n"
       "nop\n"
       ".endr\n"
       );
  }
}

////////////////////////////////////////////////////////////////////////////////
// randon number generation
////////////////////////////////////////////////////////////////////////////////


unsigned rand_vector[] =
  {
   64,71,92,84,88,29,84,69,60,90,19,15,76,39,49,64,49,73,31,4,10,34,4,1,81,97,100,61,88,37,23,99,90,58,69,51,33,27,0,19,49,82,53,99,59,41,3,
   62,100,16,65,93,31,21,67,66,86,36,65,32,94,47,71,53,54,52,47,22,72,85,65,9,7,34,2,14,83,67,61,93,60,35,81,79,13,27,74,95,26,91,58,76,84,95,
   63,45,52,69,87,78,52,14,17,66,49,42,94,48,85,28,87,85,90,23,36,77,35,97,57,71,77,86,77,16,85,22,74,87,33,72,59,12,61,35,68,84,11,68,62,84,
   53,70,99,1,61,13,31,80,23,12,85,40,30,96,72,18,9,60,7,73,5,54,88,84,58,26,50,77,73,21,26,99,20,62,94,11,67,68,66,98,89,42,44,29,26,92,36,
   16,11,61,61,35,70,2,1,55,78,52,76,88,26,83,79,91,65,21,28,69,78,52,66,18,8,93,93,88,61,17,87,77,26,67,78,21,42,3,18,29,92,84,27,17,73,76,18,
   10,78,4,61,63,5,69,33,84,98,60,89,41,86,94,39,6,16,96,56,56,99,61,34,65,86,10,39,8,38,81,20,25,5,41,74,5,2,13,11,78,29,57,78,0,47,14,23,
   92,42,94,5,95,65,41,27,54,6,15,27,59,92,9,94,14,11,16,80,80,66,58,28,16,17,46,53,49,66,27,1,77,89,95,10,13,50,13,11,26,69,82,62,9,34,97,64,
   12,65,81,55,97,5,90,77,53,44,18,57,28,17,37,74,37,20,95,54,27,31,21,90,60,84,35,65,65,17,33,31,22,21,67,70,27,96,94,71,11,57,84,93,66,64,
   15,33,12,84,38,15,43,53,86,95,35,54,10,93,63,36,71,65,98,71,98,27,100,96,92,85,58,89,0,16,97,80,27,6,42,48,84,49,46,90,34,32,31,61,42,33,
   38,67,1,86,57,90,52,71,39,83,81,29,26,21,47,41,13,59,52,11,30,81,22,8,68,22,52,68,23,79,78,36,37,77,21,97,43,18,74,95,9,96,7,82,60,4,57,52,
   67,87,100,14,4,95,21,85,31,4,71,4,41,79,6,82,79,6,9,79,28,19,98,31,38,30,14,10,50,28,52,75,1,35,19,23,12,80,43,36,1,44,40,90,53,67,32,27,86,
   5,85,69,64,68,9,12,44,20,36,74,29,97,13,45,93,8,41,89,1,23,22,7,81,60,34,35,65,60,85,99,50,72,34,50,16,57,3,60,66,48,6,32,76,78,12,0,91,
   57,7,13,53,46,39,30,16,83,73,0,100,69,64,44,51,71,86,14,56,51,42,56,26,35,32,10,90,56,3,59,89,49,71,75,64,19,44,48,26,27,61,0,1,79,44,16,65,
   73,53,47,45,76,99,54,86,43,82,18,58,16,39,32,100,64,27,9,73,20,58,44,14,71,58,30,58,74,2,42,19,92,78,51,19,83,93,42,25,40,40,39,45,74,96,
   76,38,54,54,20,57,18,36,70,66,5,15,11,23,59,66,35,32,39,39,90,3,32,82,88,90,85,88,37,49,65,27,37,2,17,74,24,5,2,65,72,69,100,54,24,91,32,
   87,43,6,2,45,8,83,83,87,30,13,93,77,87,32,18,39,10,16,97,98,19,17,16,58,72,1,63,0,51,7,36,27,5,92,47,7,48,84,86,38,43,93,52,15,86,65,20,96,
   21,66,39,86,79,96,7,80,12,69,48,31,74,89,10,31,51,40,75,50,44,88,6,97,29,59,60,87,58,27,98,7,19,18,48,57,77,2,51,17,8,72,73,0,22,47,2,41,
   52,11,86,24,73,9,95,3,34,40,3,44,40,41,43,56,9,21,40,48,4,6,54,55,6,18,71,55,23,5,56,95,40,36,62,93,55,40,88,74,66,40,99,0,32,67,80,6,43,53,
   73,22,20,49,7,55,89,69,59,96,98,63,14,0,5,20,68,97,82,89,91,18,18,32,65,77,51,67,65,31,88,75,75,65,70,1,67,4,52,93,20,5,80,43,73,12,70,38,
   22,22,58,66,46,17,5,74,70,62,95,41,58,13,71,72,87,88,19,93,78,6,10,66,56,55,45,95,98,68,72,32,52,68,35,39,16,44,67,31,54,85,54,59,5,45,90,
   0,41,79,56,47,0,1,77,58,8,86,58,57,13,85,49,99,81,22,77,39,63,52,38,67,6,98,82,36,24,82,95,72,42,69,3,79,66,69,19,65,78,80,18,49,37,62,75,
   73,20,95,61,79,89,59,50,34,41,97,20,79,70,63,39,84,97,11,82,76,56,53,9,3
  };

unsigned get_rand_percent(unsigned &ptr){
  ptr%=sizeof(rand_vector)/sizeof(unsigned);
  return rand_vector[ptr++];
}
unsigned get_rand_bit(unsigned &ptr){
  return get_rand_percent(ptr) & 1;
}


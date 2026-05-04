/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "rtlmin_2321.h"

// test to check that MTVAL/STVAL is set properly when
// there is a fetch access or page fault in the second
// half of the instruction
// Test sets up the following pages with 1 to 1 PA to VA mapping:
//  1) 0x8000000000ULL
//  2) 0x8040000000ULL (with no exec permission)
//  3) 0x8080000000ULL
//
// it will jump at start of page 2 - 2B => page fault
// it will jump at end of page 3 - 2B => page fault


void inline setup_trap_vector() {
  // configure trap vector
  __asm__ __volatile__ 
    (
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}

void inline to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}



void to_u_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0x1800\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t0"
     );
}

void flush_l1() {
  for ( int way=0; way < 4; way ++){
    uint64_t flush =
      (1ULL<<58) | //to L2
      (0<<14) | // starting at set 0
      (way<<6)|
      0xF;   // the whole 16 sets of the way
    __asm__ __volatile__ ( "csrw 0x7fb, %[v]\n" : : [v] "r" (flush)  );
  }
  
  // and tensor wait
  __asm__ __volatile__ ( "csrwi 0x830, 6\n");
  
}

void write_code(){
  // write something in the addresses to cause a page fault
  // anything that is not a RVC instruction will do (2 LSB =3)
  *((volatile uint64_t*) PAGE_FAULT_ADDRESS0) = 0xDEADFFFF;
  *((volatile uint64_t*) PAGE_FAULT_ADDRESS1) = 0xDEADFFFF;

}


volatile uint64_t exp_cause;
volatile uint64_t exp_tval;
uint64_t jump_ad;

int main(){
  C_TEST_START;

  // test setup
  setup_trap_vector();
  setup_vmem();
  write_code();
  
  flush_l1();
  to_u_mode();

  // first test: access fault
  asm("pf_test0:");
  exp_cause = CAUSE_FETCH_PAGE_FAULT;
  exp_tval = PAGE_FAULT_ADDRESS0 + 2;
  jump_ad = PAGE_FAULT_ADDRESS0;
  __asm__ __volatile__
    (
     "jalr x0, %[dest],0"
     : 
     : [dest] "r" (jump_ad)
     : "t0");
  
  // second test: page fault
  asm("pf_test1:");
  exp_cause = CAUSE_FETCH_PAGE_FAULT;
  exp_tval = PAGE_FAULT_ADDRESS1 + 2;
  jump_ad = PAGE_FAULT_ADDRESS1;
  __asm__ __volatile__
    (
     "jalr x0, %[dest],0"
     : 
     : [dest] "r" (jump_ad)
     : );
  
  asm("test_end:");

  to_m_mode();
  C_TEST_PASS;
  return 0;
}


void trap_handler()
{
  //read mcause and mtval
  uint64_t mcause, mtval;
  __asm__ __volatile__
    (
     "csrr %[mcause], mcause\n" 
     "csrr %[mtval],  mtval\n"
     : [mcause] "=r" (mcause), [mtval] "=r" (mtval) );


  // if cause is ecall, it's a jump to 'm mode'
  // set mepc to +4, and mpp to 3
  if (mcause == CAUSE_USER_ECALL) {
    __asm__ __volatile__
      (
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );
  }
  else {
    if (mcause != exp_cause) {
      C_TEST_FAIL;
    }
    
    if (exp_tval != mtval) {
      C_TEST_FAIL;
    }
    
    
    // set mepc before the mret
    if ( mtval == PAGE_FAULT_ADDRESS0) {
      __asm__ __volatile__
        (
         "la t0, pf_test1\n"
         "csrw mepc, t0\n"
         : : : "t0"
         );
    }
    else {
      __asm__ __volatile__
        (
         "la t0, test_end\n"
         "csrw mepc, t0\n"
         : : : "t0"
         );
    }
  }
  
}

uint64_t table0[512] __attribute__ ((aligned (1<<12))) = {0};
uint64_t table1[512] __attribute__ ((aligned (1<<12))) = {0};

void setup_vmem(){
  uintptr_t ads[]= { 0x8000000000ULL, // for regular code
                     0x8040000000ULL,  // code but no exec permission
                     0x8080000000ULL  // regular code again
  };

  uint64_t access_bits[] = { 0xDF,
                             0xD7,
                             0xDF};
                           
  // mapping ads one to one
  for ( long unsigned int i = 0; i < sizeof(ads)/sizeof(uintptr_t); i++){
    // root
    uint64_t ad = ads[i];
    
    int vpn = ad >> (12+9*3);
    uintptr_t table1_ad = (uintptr_t) table1;
    table0[vpn] = 0x1 | ((table1_ad>>12)<<10);
    
    vpn = (ad >> (12 + 9*2)) & 0x1FF;
    table1[vpn] = access_bits[i] | ((ad>>12)<<10);
  }
  
  uintptr_t satp = (uintptr_t) table0;
  satp>>=12;
  satp|= ( 9ULL<<60);
  __asm__ __volatile__ ( "csrw satp, %[v]\n" : : [v] "r" (satp));

  __asm__ __volatile__ ( "fence\n");  

  // and flush the tables
   for ( int way=0; way < 4; way ++){
    uint64_t flush =
      (1ULL<<58) | //to L2
      (0<<14) | // starting at set 0
      (way<<6)|
      0xF;   // the whole 16 sets of the way
    __asm__ __volatile__ ( "csrw 0x7fb, %[v]\n" : : [v] "r" (flush)  );
  }

  // and tensor wait
  __asm__ __volatile__ ( "csrwi 0x830, 6\n");
  
}

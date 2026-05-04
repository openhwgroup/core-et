/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "verif_511.h"

int main(){
  C_TEST_START;

  // setup vmem
  // 2 1G pages with one to one VA to PA mapping:
  //   0x8000000000 for code
  //   0x8080000000 for data
  setup_vmem();

  // go to user mode to use virtual mem
  __asm__ __volatile__
    (
     // configure trap vector and move to user mode
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     "li t0, 0x1800\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mode
     "1:\n"                     // label to referr to user-mode
     :
     :
     : "t0"
     );



  // the test itself: vmem accesses crossing boundaries
  // because these are the first accesses, they will have a TLB miss
  // each access increments the address by 4k, which is the default
  // page size (so each access has a new miss)
  __asm__ __volatile__
    (
     "li x31, 0xFF\n"
     "mova.m.x x31\n"
     "li x31, %[base]\n"
     "li x30, 4096\n"

     "ld x29, 60(x31)\n"
     "add x31, x31, x30\n"

     "sd x29, 62(x31)\n"
     "add x31, x31, x30\n"

     "flw.ps f0, 48(x31)\n"
     "add x31, x31, x30\n"

     "fsw.ps f0, 44(x31)\n"
     "add x31, x31, x30\n"

     "flq2 f0, 40(x31)\n"
     "add x31, x31, x30\n"


     "fsq2 f0, 48(x31)\n"

     :
     : [base] "i" (BASE)
     : "x31", "x30", "x29", "f0"
     );


  // ecall used to switch to m-mode, and the be able to use the wfi of C_TEST_PASS
  __asm__ __volatile__ ("ecall\n");

}


void trap_handler()
{
  uint64_t cause;
__asm__ __volatile__
  (
   "csrr %[cause], mcause\n"
   : [cause] "=r" (cause));

  if (cause == CAUSE_USER_ECALL) {
    C_TEST_PASS;
  }
  else {
    C_TEST_FAIL;
  }
}



uint64_t table0[512] __attribute__ ((aligned (1<<12))) = {0};
uint64_t table1[512] __attribute__ ((aligned (1<<12))) = {0};

void setup_vmem(){
  uintptr_t ads[]= { 0x8000000000, // for code
                     0x8080000000  // for data
  };

  // mapping ads one to one
  for ( long unsigned int i = 0; i < sizeof(ads)/sizeof(uintptr_t); i++){
    // root
    uint64_t ad = ads[i];

    int vpn = ad >> (12+9*3);
    uintptr_t table1_ad = (uintptr_t) table1;
    table0[vpn] = 0x1 | ((table1_ad>>12)<<10);

    vpn = (ad >> (12 + 9*2)) & 0x1FF;
    table1[vpn] = 0xDF | ((ad>>12)<<10);
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

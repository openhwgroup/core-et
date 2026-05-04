/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include <stdlib.h>
#include "rtlmin_851.h"
#include "utils.h"

#define BASE 0x8080000000

// gather + exceptions test:
// defining the following 2 1G pages (with PA being the same as VA):
//  0x8000000000  // where code is
//  0x8080000000  // GSC destination
//
// Different iterations (offsets and mask defined in test_config.cc) are performed.
// Accesses are in 0x8080000000+offset[lane]
//  - if offset[lane] is negative and lane not masked => will trap to page fault
//  - in the trap handler, offset is made positive and instruction retried (starting at gsc_progress)


int find_first_negative(int32_t *offsets, uint8_t mask, int start){
  mask>>=start;
  for ( int i = start; i < 8; i++) {
    if (offsets[i] < 0 && (mask&1) == 1) return i;
    mask>>=1;
  }
  return -1;
}

void test_gather(int test_nr){
  const uint32_t *base = (const uint32_t*) BASE;
  int32_t offsets[8]  __attribute__ ((aligned (32)));  //aligned to 32B (to avoid RTLMIN-2329)

  // save test nr in validation3 (to see in waveforms) and 
  // copy of offsets (because they will be modified by different threads)
  __asm__ __volatile__
    (
     "csrw validation3, %[test_nr]\n"
     "flq2 f0, %[in]\n"
     "fsq2 f0, %[out]\n"
     : [out] "=m" (offsets)
     : [in] "m" (test_offsets[test_nr]), [test_nr] "r" (test_nr)
     : "f0"
     );
  
  
  // set x5 to lane expected to fail
  // set x7 to mask
  // f0 contains the offsets
  // note: these registers will be used by the trap handler
  
  int fail = find_first_negative(offsets, test_masks[test_nr], 0);
  
  __asm__ __volatile__
    (
     "mv x5, %[fail]\n"
     "mv x6, %[mask]\n"
     "mova.m.x x6\n"
     "flq2 f0,0(%[offsets])\n"
     "fscw.ps f1,f0(%[base]) "
     //"fgw.ps f1,f0(%[base]) "
     :
     : [offsets] "r" (offsets),  [mask] "r" (test_masks[test_nr] ), [fail] "r" (fail), [base] "r" (base)
     : "f0", "f1", "x5", "x6", "x7"
     );
}


// ecall used to switch to m-mode, and the be able to use the wfi of C_TEST_PASS
void inline  pass() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
  C_TEST_PASS;
}

int main(){
  C_TEST_START;

  // setup vmem
  setup_vmem();
  
  __asm__ __volatile__ 
    (
     // configure trap vector and move to user mode
     "la t0, test_mtrap_vector\n"
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

  for ( uint64_t i = 0; i < NR_TESTS; i++) {
    test_gather(i);
  }

  pass();
  return 0;
}




uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t *regs)
{
  // if getting here because of ecall... set epc+=4, mpp=3 and return
  // this will just switch to m-mode => so that we can call C_TEST_PASS
  if (cause == CAUSE_USER_ECALL) {
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );
    return epc+4;
  }


  
  // error in receiving an exception other than page fault
  if ( cause  != CAUSE_STORE_PF) {
    et_printf("ERROR: trap cause is different than page fault, expecting ");
    char str_cause_reported[128];
    char str_cause_expected[128];
    itoa(cause,str_cause_reported);
    itoa(CAUSE_STORE_PF, str_cause_expected);
    et_printf(str_cause_expected);
    et_printf(" but reported ");
    et_printf(str_cause_reported);
    et_printf("\n");
    C_TEST_FAIL;
  }

  // check that gsc progress is the same as x5
  uint64_t progress;
  __asm__ __volatile__ ( "csrr %[progress], gsc_progress\n" : [progress] "=r" (progress));
  if (progress != regs[5]) {
    char str_gsc_progress_reported[128];
    char str_gsc_progress_expected[128];
    itoa(progress, str_gsc_progress_reported);
    itoa(regs[5], str_gsc_progress_expected);
    et_printf("ERROR: gsc_progress is incorrect. Expecting ");
    et_printf(str_gsc_progress_expected);
    et_printf(" but reported ");
    et_printf(str_gsc_progress_reported);
    et_printf("\n");
    C_TEST_FAIL;
  }

  // make the offset positive (and the ones before negative)
  int32_t *offsets = (int32_t*) &(regs[32]); // pointing to f0
  for ( uint64_t i = 0; i< progress; i++)
    offsets[i] = -0x1000;
  offsets[progress] = -offsets[progress];
  regs[5] = find_first_negative(offsets, regs[6], progress);
   
  // return same epc to retry instruction at progress
  return epc;
}


uint64_t table0[512] __attribute__ ((aligned (1<<12))) = {0};
uint64_t table1[512]  __attribute__ ((aligned (1<<12)))= {0};

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

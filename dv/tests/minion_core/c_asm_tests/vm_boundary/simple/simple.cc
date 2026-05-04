/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "vm_boundary_common.h"
#include "macros.h"


// specify section for functions we want to run from VM
void simple() LAST_PG_FNC_ATTS;
uint8_t buffer[512] LAST_PG_ATTS = {0};
EXTERN_C void nop_func() LAST_PG_FNC_ATTS;

// test entry point
int main(){
  C_TEST_START;

  // setup vmem and start u-mode
  setup_vmem_sv48();
  to_u_mode();

  
  // simple test: run code located in the last valid 1G  page
  LONG_CALL(simple);

  pass();
  return 0;
}

void nop_func() {
  __asm__ __volatile__ ("nop");
}

void simple(){
  // code executed in the last page
  // data read and written from the last page
  for ( int i = 0 ; i< 512; i++)
    buffer[i]++;

  // do a jalr to check the destination register is properly written
  // calling nop_func, which will do nothing and return
  __asm__ __volatile__
      (
       "la t0, nop_func\n"
       "jalr x1, t0\n"
       :
       :
       : "t0","x1" );

  // and just some dummy padding
  __asm__ __volatile__ ("nop\nnop\nnop\n");
  
}


//trap handler
uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t mtval, uint64_t *regs)
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


  // in this simple test, no exceptions other than the ecall are expected... so test fail
  C_TEST_FAIL;
  
  // just use these parameters, because with -Werror we get a compilation error if we don't
  regs[0]++;
  mtval++;
 
  // return same epc to retry instruction at progress
  return epc;
}


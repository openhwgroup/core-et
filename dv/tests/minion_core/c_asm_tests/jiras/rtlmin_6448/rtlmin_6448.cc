/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "rtlmin_6448.h"

////////////////////////////////////////////////////////////////////////////////
// test to reproduce RTLMIN-6448: a bad mtval is set when there is a fetch
// breakpoint exception at the same time of a fetch access fault found in the
// second half of the instruction, but not in the first one.
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// test entry point: setups trap handler and debug CSRs to have a fetch BP at the
// end of the DRAM - 2, and address that has a fetch access fault in the second
// half of the instruction, but not int the first half, which is the situation 
// described in RTLMIN-6448
////////////////////////////////////////////////////////////////////////////////
int main(){
  C_TEST_START;

  __asm__ __volatile__
    (
     // setup trap handler
     "la t0, trap_handler\n"
     "csrw mtvec, t0\n"
     // prepare debug registers
     // 0x5B: action=0 (ebreak xcpt);  match=0 (full match of tdata2); match on m/s/u and load/store (but not exec)   
     "li t0, 0x5c\n"
     "csrw tdata1, t0\n"
     "csrw tdata2, %[tdata2]"
     :
     : [tdata2] "r" (BREAKPOINT_AD)
     : "t0"
     );
  
  LONG_CALL(test_code);
}

////////////////////////////////////////////////////////////////////////////////
// trap handler: the test is expected to get here.
// it just checks mtval and mcause and finishes
////////////////////////////////////////////////////////////////////////////////

void test_code(){
  __asm__ __volatile__ 
    (
     // compressed nop in order not to have PC aligned to 32bits, but 16
     ".byte 0x01, 0x00\n" 
     // N-1 nops to fill .hi_text, except for the last 2 bytes
     ".rept %0\n"
     "nop\n"
     ".endr\n"
     //first half of a non-compressed instruction
     ".byte 0x03, 0xff\n" 
     : : "i" (TEST_INSTRUCTIONS -1 )
     );

}

////////////////////////////////////////////////////////////////////////////////
// trap handler: the test is expected to get here.
// it just checks mtval and mcause and finishes
////////////////////////////////////////////////////////////////////////////////
__attribute__ ((aligned (1<<12))) __attribute__ ((interrupt)) 
void trap_handler()
{
  uint64_t mcause, mtval;
__asm__ __volatile__
  (
   "csrr %0, mcause\n"
   "csrr %1, mtval\n"
   :  "=r" (mcause), "=r" (mtval) );

  if (mcause == CAUSE_BREAKPOINT && mtval == BREAKPOINT_AD) {
    C_TEST_PASS;
  }
  else {
    C_TEST_FAIL;
  }
}

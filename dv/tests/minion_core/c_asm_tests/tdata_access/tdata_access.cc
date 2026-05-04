/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "et_test_common.h"
#include "macros.h"
#include "minion.h"
#include "tdata_access.hpp"


// Test that tdata ara only accessible throught


void setup_trap_vector() {
  //configure trap vector
  __asm__ __volatile__
    (
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}


__attribute__ ((always_inline))
inline void setup_debug() {
  // Setup DCSR so ebreaks traps to debug
  // We will use them to communicate with the debuger

  /*
   * The debug expects certain parameters on x31
   * The result is always returned on mscratch
   */
  /*__asm__ __volatile__
    (
    "csrrwi x0, fcc, 0\n" //read from CREDIT COUNTER 0
    );
  */

  /* Long delay until the debugger halts the thread
   * A better way could be using the FCC from the debugger
   * but BEMU expects the FCC from the minion (instr retired)
   * That's not a big deal since the checker is disabled on debug test
   * but makes more complicate to debug since there will be not log for the
   * retired insturctions
   */
  int64_t d = 3*512;
  while(d-- > 0){
    __asm__ __volatile__("nop\n");
  }


}

void setup_tests() {
  /*
   * Setup the minion to run the tests
   */

  setup_trap_vector();

  __asm__ volatile ("csrw tdata2, zero\n"); // Initialize tdata2

}


// DEBUGER: enable DMODE or DMODE + ACTION
void test0() {

  uint64_t tdata1_configs[2] = { ENABLE_DMODE, ENABLE_DMODE | ENABLE_ACTION };

  for (size_t i = 0; i < LENGHT(tdata1_configs); ++i) {

    uint64_t tdata1_config = tdata1_configs[i];

    // Ask debugger to enable DMODE
    __asm__ volatile ("mv x31, %[VALUE]\n"
		      "ebreak\n"
		      : [VALUE] "=r" (tdata1_config)
		      :
		      :);

    tdata1_t prev_tdata1 = read_tdata1();
    tdata1_t next_tdata1;
    uint64_t prev_tdata2 = read_tdata2();
    uint64_t next_tdata2;

    //memset(&next_tdata1, 0xFF, sizeof(tdata1_t)); // Set all the fields to 1
    next_tdata1.load = 1;
    next_tdata1.store = 1;
    next_tdata1.u = 1;
    next_tdata1.execute = 1;
    next_tdata1.s = 1;
    next_tdata1.m = 1;
    next_tdata1.match = 1;
    next_tdata1.action = 1;
    next_tdata1.timing = 1;
    next_tdata1.maskmax = 0x3f;
    next_tdata1.ttype = 0xf;
    next_tdata1.dmode = 0; // Try to disable dmode

    // Try to change the tdata1 values while we tdata1.dmode==1
    write_tdata1(next_tdata1);

    hint(0x0);

    if ( prev_tdata1 != read_tdata1()) {
      // Tdata1 should only be accesible by debug mode
      C_TEST_FAIL;
    }

    // Try to change the tdata2 values while tdata1.mode == 1
    next_tdata2 = 0xCACACACAULL;
    write_tdata2(next_tdata2);

    hint(0x1);

    if ( prev_tdata2 != read_tdata2() ) {
      // tdata2 should only be accesible by debug mode
      C_TEST_FAIL;
    }

  }

}

// FROM TEST 0
// DISABLE DMODE
// Test that action is 0
// Test that action can not be enabled
// Test that we can't enable dmode
void test1() {
  uint64_t tdata1_configs[2] = { 0ULL, ENABLE_ACTION};

  for (size_t i = 0; i < LENGHT(tdata1_configs); ++i) {

    uint64_t tdata1_config = tdata1_configs[i];

    // Ask debugger to disable DMODE
    __asm__ volatile ("mv x31, %[VALUE]\n"
		      "ebreak\n"
		      : [VALUE] "=r" (tdata1_config)
		      :
		      :);

    tdata1_t prev_tdata1 = read_tdata1();
    tdata1_t next_tdata1;
    uint64_t prev_tdata2 = read_tdata2();
    uint64_t next_tdata2;

    tdata1_t current_tdata1;
    uint64_t current_tdata2;

    hint(0x10);

    if (prev_tdata1.action == 1) {
      // C test should be
      C_TEST_FAIL;
    }

    // Try to enable dmode
    bzero(&next_tdata1, sizeof(next_tdata1));
    next_tdata1.dmode = 1;

    write_tdata1(next_tdata1);
    current_tdata1 = read_tdata1();

    hint(0x11);

    if (current_tdata1.dmode) {
      // Dmode should be 0
      C_TEST_FAIL;
    }

    // Try to enable action
    next_tdata1.dmode = 0;
    next_tdata1.action = 1;

    write_tdata1(next_tdata1);
    current_tdata1 = read_tdata1();

    hint(0x12);

    if (current_tdata1.action) {
      // action should be 0
      C_TEST_FAIL;
    }

    // Try to enable dmode + action
    next_tdata1.dmode = 1;
    next_tdata1.action = 1;

    write_tdata1(next_tdata1);
    current_tdata1 = read_tdata1();

    hint(0x13);

    if (current_tdata1.dmode || current_tdata1.action) {
      // Dmode and action should be 0
      C_TEST_FAIL;
    }

    prev_tdata1 = read_tdata1();
    prev_tdata2 = read_tdata2();

    // Try to write tdata1
    next_tdata1.load = 1;             // load is cleaned on every iteration when we try to enable dmode
    next_tdata2 = i+1; // Change it on every iteration
    write_tdata1(next_tdata1);
    write_tdata2(next_tdata2);

    current_tdata1 = read_tdata1();
    current_tdata2 = read_tdata2();

    hint(0x14);

    if ( current_tdata1 == prev_tdata1) {
      // tdata1 and tdata2 should be accessible
      C_TEST_FAIL;
    }

    hint(0x15);

    if (current_tdata2 == prev_tdata2) {
      C_TEST_FAIL;
    }

  }
}


void hart_done() {
  /*
   * Notify the debugger that the hart is done
   */
  uint64_t value = HART_DONE;
  __asm__ volatile ("mv x31, %[VALUE]\n"
		    "ebreak\n"
		    : [VALUE] "=r" (value)
		    :
		    :);
}

//////////////////////////////////////////////////////////////////////
// TEST ENTRY POINT
//////////////////////////////////////////////////////////////////////

int main(){

  C_TEST_START;

  //uint64_t hart = get_hart_id();
  //uint64_t thread_id = hart & 0x1;

  setup_debug();
  setup_tests();

  // Tests
  test0();
  test1();

  // Done
  hart_done();
  C_TEST_PASS;

}




uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  (void)cause;
  (void)epc;
  (void)tval;
  (void)hart;

  // It should not trap..
  C_TEST_FAIL;

  return epc+4;
}

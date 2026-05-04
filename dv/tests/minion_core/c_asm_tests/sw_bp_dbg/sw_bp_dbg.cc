/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include <stdlib.h>
#include "minion.h"
#include "sw_bp_dbg.h"


// The test setups the DCSR to test ebreaks
// ebreak should trap to debug mode or trap to mcode 
// based on the configuration on DCSR

uint64_t priv_mod[NUM_HARTS];
bool expects_trap_to_debug[NUM_HARTS];
volatile uint64_t done_all_operations[NUM_HARTS] __attribute__((section(".done_array")));

bool check_ecall(uint64_t cause, uint64_t priv_mod){

  return (((priv_mod == 0) && (cause == CAUSE_USER_ECALL)) ||
         ((priv_mod == 1) && (cause == CAUSE_SERVICE_ECALL)) ||
         ((priv_mod == 3) && (cause == CAUSE_MACHINE_ECALL)));
}

void setup_trap_vector() {
     //configure trap vector
       __asm__ __volatile__
           (
             "la t0, test_mtrap_vector\n"
             "csrw mtvec, t0\n"
             : : : "t0");
}

__attribute__ ((always_inline))
inline void setup_debug(uint64_t hart) {
  // Setup DCSR so ebreaks traps to debug
 
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
  
  set_flb(hart, 0);
  FENCE;
  
  uint64_t d = 3*512; 
  while(d-- > 0){
    __asm__ __volatile__("nop\n");
  }
  

}

void done_debugger(uint64_t hart) {
  set_flb(hart, 1);
  FENCE;
}


//////////////////////////////////////////////////////////////////////
// TEST ENTRY POINT
//////////////////////////////////////////////////////////////////////

constexpr uint64_t MMODE = 3;
constexpr uint64_t SMODE = 1;
constexpr uint64_t UMODE = 0;

int main(){

  C_TEST_START;
  
  uint64_t hart = get_hart_id();
  uint64_t mscratch = 0;
  priv_mod[hart] = MMODE;
  expects_trap_to_debug[hart] = true;
  
  //////////////////////////////////////////////////////////////////////
  // local variables
  //////////////////////////////////////////////////////////////////////

  setup_debug(hart);
  setup_trap_vector();
  
  //////////////////////////////////////////////////////////////////////
  // Check SW breakpoint in every priv mode.
  //////////////////////////////////////////////////////////////////////
  set_mscratch(0);
  
  // Trap in U-mode
  m_to_u_mode(priv_mod[hart]);
  ebreak();
  to_m_mode(priv_mod[hart]);
  
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_DEBUG != mscratch)
    C_TEST_FAIL;

  // Trap in S-mode
  m_to_s_mode(priv_mod[hart]);
  ebreak();
  to_m_mode(priv_mod[hart]);
  
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_DEBUG != mscratch)
    C_TEST_FAIL;
    
  // Trap in M-mode
  ebreak();
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_DEBUG != mscratch)
    C_TEST_FAIL;
  
  ebreak_done();
  
  expects_trap_to_debug[hart] = false;
  //////////////////////////////////////////////////////////////////////
  // Check SW breakpoint (trap) in every priv mode.
  //////////////////////////////////////////////////////////////////////

  set_mscratch(0);
  
  // Trap in U-mode
  m_to_u_mode(priv_mod[hart]);
  ebreak();
  to_m_mode(priv_mod[hart]);
  
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_MHANDLER != mscratch)
    C_TEST_FAIL;

  // Trap in S-mode
  m_to_s_mode(priv_mod[hart]);
  ebreak();
  to_m_mode(priv_mod[hart]);
  
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_MHANDLER != mscratch)
    C_TEST_FAIL;
    
  // Trap in M-mode
  ebreak();
  mscratch = get_mscratch();
  set_mscratch(0);
  if(TRAPS_TO_MHANDLER != mscratch)
    C_TEST_FAIL;
  
  done_debugger(hart);
  
   // Done
  C_TEST_PASS;
}




uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  (void)tval;
  if (check_ecall(cause, priv_mod[hart])) {
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );

    return epc+4;//erase + 4 tommorrow before push Trap return to epc+4 automatically
  } else {
    if (expects_trap_to_debug[hart] || cause != CAUSE_BREAKPOINT) {
      C_TEST_FAIL;
    } else {
      set_mscratch(TRAPS_TO_MHANDLER);
      return epc+4;
    }
    return epc;
  }
}


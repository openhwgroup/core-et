/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "minion.h"

#include "xcpt_gen_funcs.hpp"

/* The test sets a trap handler an 
 *
 *
 *
 *
 *
 */

xcpt_gen_t xcpt_list[] = { // S-Mode xcpts
                          {.cause = xcpt_cause_t::ILLEGAL_INSTR, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::BREAKPOINT, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_LD, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_ST, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::LD_AF, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::ST_AF, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::S_ECALL, .priv=priv_lv_t::SMODE },
                          //{.cause = xcpt_cause_t::BUS_ERROR, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::M_CODE, .priv=priv_lv_t::SMODE },
                          {.cause = xcpt_cause_t::FETCH_AF, .priv=priv_lv_t::SMODE },
                          //{.cause = xcpt_cause_t::FETCH_BUS_ERROR, .priv=priv_lv_t::SMODE },
                          // // M-Mode xcpts
                          {.cause = xcpt_cause_t::ILLEGAL_INSTR, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::BREAKPOINT, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_LD, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_ST, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::LD_AF, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::ST_AF, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::M_ECALL, .priv=priv_lv_t::MMODE },
                          //{.cause = xcpt_cause_t::BUS_ERROR, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::M_CODE, .priv=priv_lv_t::MMODE },
                          {.cause = xcpt_cause_t::FETCH_AF, .priv=priv_lv_t::MMODE },
                          //{.cause = xcpt_cause_t::FETCH_BUS_ERROR, .priv=priv_lv_t::MMODE },
                          // // U-Mode xcpts
                          {.cause = xcpt_cause_t::ILLEGAL_INSTR, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::BREAKPOINT, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_LD, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::MISALIGNED_ST, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::LD_AF, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::ST_AF, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::U_ECALL, .priv=priv_lv_t::UMODE },
                          //{.cause = xcpt_cause_t::BUS_ERROR, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::M_CODE, .priv=priv_lv_t::UMODE },
                          {.cause = xcpt_cause_t::FETCH_AF, .priv=priv_lv_t::UMODE },
                          //{.cause = xcpt_cause_t::FETCH_BUS_ERROR, .priv=priv_lv_t::UMODE }
                           
                           
                           
};

size_t xcpt_list_len = LEN(xcpt_list);
uint32_t num_0_te_xcpt_traps = 0;
uint32_t num_1_te_xcpt_traps = 0;

void setup_trap_vector() {
     //configure trap vector
  __asm__ __volatile__
    (
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     : : : "t0");
}

void enable_shadows() {
  // Enable user-mode hartid CSR
  __asm__ __volatile__ 
    (
     "csrsi menable_shadows, 0x1\n"
     );
}

void local_itoa(int a, char *b)
{
    int i, i1;
    char c;
  
    if (a==0) {
        b[0]='0';
        b[1]=0;
        return;
    }
  
    i=0;
    while (a>0) {
        b[i]=(a%10)+'0';
        a=a/10;
        i++;
    }
  
    for (i1=0; i1<i/2; i1++) {
        c=b[i1];
        b[i1]=b[i-i1-1];
        b[i-i1-1]=c;
    }
    b[i]=0;
}

int main(){
  
  const uint64_t hart_id = get_hart_id();
  
  // Setup trap handler
  enable_shadows();
  setup_trap_vector();
  
  // Iterate over all the list and generate the exception
  for (const xcpt_gen_t &xcpt_op : xcpt_list) {
    // Jump to desired priv mode
    if ( xcpt_op.priv != hart_priv_mode[hart_id] ) {
      change_priv_to(hart_id, xcpt_op.priv);
    }
    // Call handler to generate expction
    uint64_t offset = static_cast<uint64_t>(xcpt_op.cause);
    xcpt_gen_funcs[offset]();
  }
  FENCE;
  char snum[9];

  if ((hart_id % 2) ==  0) {
      et_printf("HARTID: ");
      local_itoa(hart_id, snum);
      et_printf(snum);
      et_printf("\nNumber of Traps observed: ");     
      local_itoa(num_0_te_xcpt_traps, snum);
      et_printf(snum);
      et_printf("\n");
      if (num_0_te_xcpt_traps < xcpt_list_len) {
          // If traps generated were not observed, fail the test.
          C_TEST_FAIL;
      }
  }
  else {
      et_printf("HARTID: ");
      local_itoa(hart_id, snum);
      et_printf(snum);
      et_printf("\nNumber of Traps observed: ");     
      local_itoa(num_1_te_xcpt_traps, snum);
      et_printf(snum);
      et_printf("\n");  
      if (num_1_te_xcpt_traps < xcpt_list_len) {
          // If traps generated were not observed, fail the test.          
          C_TEST_FAIL;
      }
  }      
  et_printf("\n");
  
  C_TEST_PASS;  
}

bool check_ecall(uint64_t cause) {
  constexpr uint64_t CAUSE_USER_ECALL    = 0x8;
  constexpr uint64_t CAUSE_SERVICE_ECALL = 0x9;
  constexpr uint64_t CAUSE_MACHINE_ECALL = 0xB;
  
  return CAUSE_USER_ECALL == cause ||
    CAUSE_SERVICE_ECALL == cause ||
    CAUSE_MACHINE_ECALL == cause;
}

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  constexpr uint64_t CAUSE_BREAKPOINT = 0x3;
  constexpr uint64_t CAUSE_FETCH_AF = 0x1;
  constexpr uint64_t CAUSE_FETCH_BUS_ERROR = 0x19;

  uint64_t hart_id = get_hart_id();
  
  (void)tval;
  if ((hart_id % 2) == 0) {
      num_0_te_xcpt_traps++;
  }
  else {
      num_1_te_xcpt_traps++;
  }
  
  // if cause is ecall, it's a jump to 'm mode'
  // set mepc to +4, and mpp to 3
  if (check_ecall(cause)) {
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       :
       :
       : "t0" );

    return epc+4;// ecall doesn't increment the EPC. we need to do it manually
    
  } else if (cause == CAUSE_BREAKPOINT) {
    // Remove breakpoint configuration to avoid more traps
    __asm __volatile__
      (
       "csrw tdata2, zero\n"
       "li x31, 0x3FF\n"
       "csrc tdata1, x31\n"
       :
       :
       : "x31" );
    return epc;
  } else if ( cause == CAUSE_FETCH_AF || cause == CAUSE_FETCH_BUS_ERROR) {
    const uint64_t return_addr = hart_return_addr[hart]; // FETCH xcpt needs a valid PC to return. Otherwise we will generate the same exception
    hart_return_addr[hart] = 0; //Clear the address to avoid uninitialized addresses
    FENCE;
    return return_addr;    
  } else {
    // Skip instrucctions and continue (handlers should have a fence/nop after the failing instruction)
    return epc+4;
  }
  return epc;
}

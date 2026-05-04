/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "mbox_intr.h"
#include "hal_device.h" 
#include "mbox_rdwr_twoway.h"
#include "fcc_alt.h"

#define EXP_CAUSE ((1ULL<<63) | 3)
#define IPI_TRIGGER_CLEAR 0x80F40098 >> 3

uint8_t test_status = 0;

//check if master minion
uint8_t check_master(void)
{ 
   uint64_t hart_id = get_hart_id();
    
   if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);

     if (shire_id == 32 && minion_id == 0) return 1; 
     else return 0;
   }
   return 0;
}


int main()
{
    C_TEST_START;
    /* Wait for SP to send credit*/
    if(check_master()){    
      uint64_t baseAddrMboxIntTrg = R_PU_TRG_MMIN_BASEADDR;
      uintptr_t baseAddr =  R_PU_MBOX_MM_SP_BASEADDR;
      uintptr_t movingAddr = baseAddr + R_PU_MBOX_MM_SP_SIZE/2;
      volatile uint64_t* addr64;
      PTR_Mbox_intr mbox_int_regs;
      mbox_int_regs = ( PTR_Mbox_intr ) baseAddrMboxIntTrg;
    // Setup machine trap vector in Master Shire 
    // and enable interrupts
      __asm__ __volatile__ 
      (
        "la t0, mtrap_vector_ipi\n"
        "ori t0, t0, 1\n"  // Here we set "vector mode" so that msip is on baase + 4 x 3 
        "csrw mtvec, t0\n" // assign routine base address
        "csrwi mie, 8\n"  // enable msip (msie)
        "csrsi mstatus, 8\n"  // set MIE to '1'
      : : : "t0"
      );

      int8_t status=wait_for_credit(0);
      if (status<0) {
          test_status++;
          goto test_end;
      }
      addr64  = (uint64_t*)baseAddr;
      *addr64 = movingAddr; //store middle of the range as address for verification of write
      addr64  = (uint64_t*)movingAddr;
      *addr64 = (uint64_t) 0xdeadbeefdeadbeef;
      mbox_int_regs->ipi_trigger_set = 0xf ;
      __asm__ __volatile__ ("fence\n");
      STALL //wait for ipi
      addr64 = (uint64_t*)(baseAddr);
      movingAddr = (uintptr_t)(*addr64);
      addr64 = (uint64_t*)movingAddr;
      if (*addr64 == 0xdeadbeefdeadbeef) { test_status=0;}
      else test_status++;
       
    }
test_end:
    if (check_master() && test_status) {
      C_TEST_FAIL;
    }
    else C_TEST_PASS;

}

void irq_handler()
{

//  et_printf("\nInterrupt routine started\n");

  volatile uint64_t cause = 0;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  // error in receiving an exception other than page fault
  if ( cause != EXP_CAUSE) {
//    et_printf("\nWrong Cause\n");
    test_status++;
  }
  else {
    volatile uint64_t hart_id = get_hart_id();
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    // clear the interrupt
    write_esr(PP_MACHINE, shire_id, REGION_OTHER, IPI_TRIGGER_CLEAR, 0x1 << hart_id);
    FENCE
  }
}

void bad_trap(){
//  et_printf("\nBad Trap\n");
  test_status++;
}

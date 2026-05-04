/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This test tries to whether there is a path to all Mailbox endpoints
// from the Master Minion. We do not test read/writes, only check for the 
// exceptions on accesses.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "fcc.h"
#include "esr.h"
#include "minion_noc_err_int.h"

#define MPROT_ADDR 0x004

#define CAUSE_BUS_ERROR          ((1ULL<<63) | 23)
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#define TIMEOUT 500


typedef struct Mbox_t {
    intptr_t baseAddr;
    uint8_t  isAccessible;
} Mbox_t;


Mbox_t MboxTable[] = { {R_PU_MBOX_PC_MX_BASEADDR, 0},
                       {R_PU_MBOX_MM_MX_BASEADDR, 1},
                       {R_PU_MBOX_PC_MM_BASEADDR, 1},
                       {R_PU_MBOX_MX_SP_BASEADDR, 0},
                       {R_PU_MBOX_MM_SP_BASEADDR, 1},
                       {R_PU_MBOX_PC_SP_BASEADDR, 0},
                       {R_PU_MBOX_SPARE_BASEADDR, 0},
                       {0,0} 
                     };

volatile uint8_t wasAccessFault;

int main()
{
    uint64_t shire_id = get_shire_id();

    #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t hart_id = get_hart_id();
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    //volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t timeout = TIMEOUT;
    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
      s32_scp_init();
      give_credit(0, 0, 0, 0);
    }
    if (hart_id == 0) {
      wait_for_credit(0);
    }
    #endif
    
    uint64_t esrVal = 0x0000000000000001; //set mprot[0]=1
    volatile uint64_t* neighESR;

    /* Wait for SP to send credit*/
    wait_for_credit(0);

    C_TEST_START;
    if (shire_id == 0) {
        neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);

        //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
        neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );

        //write mprot ESR with 0x01 value (mprot[0] = 1 is what we want
        *neighESR = esrVal;
    }

    if (shire_id == 32) 
    {
        Mbox_t *pMbox = MboxTable;
        __asm__ __volatile__ 
        (
        //set trap vector
        "la t0, trap_vector\n"
        "csrw mtvec, t0\n"
        //enable interrupt
        "li t0, 1\n"
        "slli t0, t0, 23\n"
        "csrs mie, t0\n"
        "csrsi mstatus, 8\n"
        : : : "t0"
        );

        uint64_t *pAddr64;
        uint64_t readData64;
        while (pMbox->baseAddr != 0) {
          wasAccessFault = 0;
          pAddr64 = (uint64_t*)(pMbox->baseAddr);
          *pAddr64 = 0xdeafbeefdeadbabe;
          __asm__ __volatile__ ("fence\n");
          for (int i=0; i<20; i++) {  //wait a bit until a potential interrupt is asserted
              __asm__ __volatile__ ("nop\n");
          }
          if (wasAccessFault) { 
              if (pMbox->isAccessible) {
                  et_printf("MBOX_ACCESS: ERROR - UNEXPECTED NON WORKING PATH");
                  C_TEST_FAIL;
              }
              else {
                  et_printf("MBOX_ACCESS: EXPECTED NON WORKING PATH");
              }
          }
          else { 
              if (pMbox->isAccessible) {
                  readData64 = *pAddr64;
                  if (readData64 != 0xdeafbeefdeadbabe) {
                      et_printf("MBOX_ACCESS: ERROR - DATA MISSMATCH");
                      C_TEST_FAIL;
                  }
              }
              else {
                  et_printf("MBOX_ACCESS: ERROR - EXPECTED WORKING PATH");
                  C_TEST_FAIL;
              }
          }
       pMbox++;
       }
        
    } //end if shire==32

    #ifdef MINSHIRE_GATESIM_ENABLE
    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
      check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
    } else if (hart_id == 0) {
      atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
    }  
    #endif

    C_TEST_PASS;
} 


void machine_handler()
{
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  
  if (cause == CAUSE_BUS_ERROR){
    //et_printf("\nTRAP HANDLER - BUS ERROR\n");
    //was bus error interrupt, clean the IRQ
    __asm__ __volatile__
    (
      "li t0, 1\n"
      "slli t0, t0, 23\n"
      "csrc mip, t0\n"
       : : : "t0"
    );
    wasAccessFault = 1;
        
  } else if (cause == CAUSE_LOAD_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - LOAD ACCESS FAULT\n");
    wasAccessFault =1;
    
  } else if (cause ==CAUSE_STORE_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - STORE ACCESS FAULT\n");
    wasAccessFault = 1;
    
  } 
  //et_printf("\nTRAP HANDLER - LEAVE\n");
  
  __asm__ __volatile__
  (
   "li t0, 0x1800\n" //return to -M- mode
   "csrs mstatus, t0\n"
   "csrr t0, mepc\n"
   "addi t0, t0, 4\n"
   "csrw mepc, t0\n"
   :
   :
   : "t0" ); 

    
}

void bad_trap(){
  et_printf("\nBad Trap\n");
  C_TEST_FAIL;
}




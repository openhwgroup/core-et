/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// This test tries to access a single register of each perpheral in IO_PU region.
//
// The assumption is as follows: 
//     * No Bus Error generated (or timeout) means we have accessibility to a particular location
//     
// We test three protection modes from MPROT: user, supervisor and machine.
// 
// * Since this is Master Minion test, we do not test total-ban on accesses to this region (mprot[1:0] = b'10 (MODE_NOBODY)) *
// * This we do from Compute Minion test *
//
// We count number of failures i.e. access which is not allowed from a particular mode, for example:
//
//     * mprot[1:0]=b'11 would generate a fault on access from USER or SUPERVISOR mode, and so on.
//
// This is the table of expected failures:
//  mprot[1:0]=    b'00     b'01    b'10   b'11
// 
//  mode 
//  USER           pass     fail    n/a    fail
//  SUPERVISOR     pass     pass    n/a    fail
//  MACHINE        pass     pass    n/a    pass
//
//

#include "hal_device.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "acc_io_pu_ms.h"
#include "regs.h"
#include "fc_utils.h"

#define REGTEST_END_OF_LIST    0xEEEEEEEE

#define TOTAL_NUMBER_OF_EXPECTED_FAULTS 36

#define CAUSE_BUS_ERROR           ((1ULL<<63) | 23)
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#define MODE_USER       0
#define MODE_SUPERVISOR 1
#define MODE_MACHINE    3
#define MODE_NOBODY     2

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 10000
#endif

volatile uint8_t wasAccessFault;
volatile uint8_t wasBusError;
volatile uint8_t caughtFailures;
volatile uint8_t mode =  MODE_MACHINE;

#define MPROT_ADDR 0x004
void set_mprot_privileges( int prv_mode ){
    uint64_t esrVal;
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
    //neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );
    //
    esrVal = *neighESR;
    esrVal = esrVal & 0xFFFFFFFFFFFFFFFC;
    esrVal = esrVal | prv_mode;
    *neighESR = esrVal;
} 

uint16_t sweep_registers()
{
  IPREGS_t     *pRegs = arr_ip;
  uint16_t failures;
  uintptr_t regAddr; 

  volatile uint8_t* addr8;
  volatile uint16_t* addr16;
  volatile uint32_t* addr32;
  volatile uint64_t* addr64;
  
  failures = 0;
  while( pRegs->base_address != REGTEST_END_OF_LIST ) {
      regAddr = pRegs->base_address + pRegs->reg_offset;
      switch(pRegs->data_width) {
        case (0x08):
          addr8 = (uint8_t*)(regAddr);
          *addr8 = 0xde;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x10):
          addr16 = (uint16_t*)(regAddr);
          *addr16 = 0xdead;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x20):
          addr32 = (uint32_t*)(regAddr);
          *addr32 = 0xdeadbeef;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;          

        case (0x40):
          addr64 = (uint64_t*)(regAddr);
          *addr64 = 0xbaaddeadbabebeef;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;                    
      } //end switch 
    pRegs++;
  } //end while
  return failures;
}




void machine_handler()
{
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  
  if (cause == CAUSE_BUS_ERROR){
    //et_printf("\nTRAP HANDLER - BUS ERROR\n");
    wasBusError = 1;
        
  } else if (cause == CAUSE_LOAD_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - LOAD ACCESS FAULT\n");
    wasAccessFault =1;
    
  } else if (cause ==CAUSE_STORE_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - STORE ACCESS FAULT\n");
    wasAccessFault = 1;
    
  } 
  //et_printf("\nTRAP HANDLER - LEAVE\n");
  
 
 if (mode == MODE_USER) {
      __asm__ __volatile__
      (
       "li t0, 0x0000\n" //return to -U- mode
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" );
  }

  else if (mode == MODE_SUPERVISOR) {
      __asm__ __volatile__
      (
       "li t0, 0x0800\n" //return to -S- mode
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" ); 
  }

  else if (mode == MODE_MACHINE) {
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

    
}

void bad_trap(){
  et_printf("\nBad Trap\n");
  #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t shire_id   = get_shire_id();
    volatile uint64_t hart_id    = get_hart_id();    
    volatile uint64_t thread_id  = (hart_id & 1);
    if (shire_id == 0 and thread_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 4*hart_id);
  #endif
  C_TEST_FAIL;
}


void m_to_u_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to refer to user-mode
     : 
     : 
     : "t0"
     );
}

void m_to_s_mode() 
{
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6800\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to supervisor-mode entry point
     "mret\n"                   // go to supervisor mod
     "1:\n"                     // label to refer to supervisor-mode
     : 
     : 
     : "t0"
     );
}

void to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

int test_access_all_modes(void)
{
    uint8_t failures=0;
    mode = MODE_USER;
    m_to_u_mode();
    failures += sweep_registers();
    to_m_mode();
    mode=MODE_SUPERVISOR;
    m_to_s_mode();
    failures += sweep_registers();
    mode=MODE_MACHINE;
    to_m_mode();
    failures += sweep_registers();
    return failures; 
}


int main()
{


  uint64_t shire_id = get_shire_id();
  
  C_TEST_START;

   __asm__ __volatile__ 
    (
     //set trap vector
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
      //enable interrupt
     "li t0, 1\n"
     "slli t0, t0, 23\n"
     "csrs mie, t0\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
    );

  #ifdef MINSHIRE_GATESIM_ENABLE
    fc_send_mbox_sync_to_sp(); //defined in fc_utils.h 
    volatile uint64_t hart_id = get_hart_id();    
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    volatile uint64_t timeout = TIMEOUT;

    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
      s32_scp_init();
      give_credit(0, -1, -1, 1);
      give_credit(32, -1, -1, 1);
    }
    wait_for_credit(1);  

    if (shire_id == 0) {
  #else
    if (shire_id == 32) {
    fc_send_mbox_sync_to_sp();
    wait_for_credit(0);
  #endif 
     wasAccessFault = 0;
     wasBusError = 0;
     caughtFailures =0;
     
     set_mprot_privileges(MODE_USER);
     caughtFailures += test_access_all_modes();
     
     to_m_mode();
     set_mprot_privileges(MODE_SUPERVISOR);
     caughtFailures += test_access_all_modes();
     
     to_m_mode();
     set_mprot_privileges(MODE_MACHINE);
     caughtFailures += test_access_all_modes();

     if (wasBusError) {
       #ifdef MINSHIRE_GATESIM_ENABLE
         if (shire_id == 0 and thread_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 4*hart_id);
       #endif
       C_TEST_FAIL;
     }
     if (caughtFailures == TOTAL_NUMBER_OF_EXPECTED_FAULTS) {
       #ifdef MINSHIRE_GATESIM_ENABLE
         if (shire_id == 0 and thread_id == 0) atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + hart_id*4);
       #endif   
       C_TEST_PASS;
     }
     else {
       #ifdef MINSHIRE_GATESIM_ENABLE
         if (shire_id == 0 and thread_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 4*hart_id);
       #endif
       C_TEST_FAIL;
     }
  } // if shireid == 0
  else  {
    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + (hart_id%64)*4);        
      }
    #endif
    C_TEST_PASS;
  }
}

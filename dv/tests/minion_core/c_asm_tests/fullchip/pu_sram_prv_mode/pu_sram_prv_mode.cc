/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "pu_sram_prv_mode.h"
#include "fc_utils.h"

#define ldOffset  0x1000
#define WORD_OFFSET 4
#define MPROT_ADDR 0x004

volatile uint8_t excpt_expected = 0;


// Jump thread to supervisor mode
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

void to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

void machine_handler()
{
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  if ((cause==CAUSE_USER_ECALL) || (cause==CAUSE_SUPERVISOR_ECALL) || (cause==CAUSE_MACHINE_ECALL) ) {
    __asm__ __volatile__
      (
       "li t0, 0x1800\n"
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" );
  } 
  else {
    if(excpt_expected == 0) {
        C_TEST_FAIL;
    }
    else excpt_expected = 0;
    __asm__ __volatile__
      (
       //"li t0, 0x1800\n"
       //"csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" );
  }
}

void bad_trap(){
  C_TEST_FAIL;
}

void set_privileges( int m_prot ){
    uint64_t esrVal;
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    esrVal = *neighESR;
    esrVal = esrVal & 0xFFFFFFFFFFFFFFFC;
    esrVal = esrVal | m_prot;
    *neighESR = esrVal;
}

int main()
{
    volatile uint64_t shire_id = get_shire_id();

    volatile uint32_t* pAddr[3];
    volatile uintptr_t baseAddresses[] = {R_PU_SRAM_LO_BASEADDR, 
                                          R_PU_SRAM_MID_BASEADDR,
                                          R_PU_SRAM_HI_BASEADDR,
                                          0 };                             
     
    volatile uintptr_t mbox_sizes[] = { R_PU_SRAM_LO_SIZE, 
                                        R_PU_SRAM_MID_SIZE,
                                        R_PU_SRAM_HI_SIZE  }; 

    volatile uint32_t readData[3];

    const uint32_t testData[] = {0xdeadbeef, 0xabcdef01, 0x12345678};
    
    volatile uint8_t i=0;
    volatile uint8_t     exception_expected;

    C_TEST_START;

    // Set machine trap handler
    __asm__ __volatile__ 
    (
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     : : : "t0"
    );

    if (shire_id == 32) {
        fc_send_mbox_sync_to_sp(); //defined in fc_utils.h
        pAddr[0] = (uint32_t*)(baseAddresses[i] + 128);                               //start of range
        pAddr[1] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2 + 128);             //mid of range
        pAddr[2] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET - 128); //end of range            
    } else { //shire_id == 0
        pAddr[0] = (uint32_t*)(baseAddresses[i]);                                     //start of range
        pAddr[1] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2);                   //mid of range
        pAddr[2] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET);       //end of range
    } 

    for(int m=0; m<4; m++) {

        set_privileges(m);
        FENCE;
        
        if(m == 2) exception_expected = 1;
        else exception_expected = 0;
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        *pAddr[0] = testData[0] + m;
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        readData[0] = *pAddr[0];
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if (readData[0] != (testData[0] + m) && exception_expected == 0){
          C_TEST_FAIL;  
        } 

        exception_expected = 0;

        m_to_s_mode();
        
        if(m > 1) exception_expected = 1;
        else exception_expected = 0;
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        *pAddr[1] = testData[1] + m;
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        readData[1] = *pAddr[1]; 
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if (readData[1] != (testData[1] + m) && exception_expected == 0){
          C_TEST_FAIL;  
        } 

        exception_expected = 0;

        to_m_mode();

        m_to_u_mode();

        if(m > 0) exception_expected = 1;
        else exception_expected = 0;
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        *pAddr[2] = testData[2] + m;
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if(exception_expected == 1) excpt_expected = 1;
        else excpt_expected = 0;
        readData[2] = *pAddr[2]; 
        if(excpt_expected != 0) {
          C_TEST_FAIL;  
        } 
        if (readData[2] != (testData[2] + m) && exception_expected == 0){
          C_TEST_FAIL;  
        } 

        to_m_mode();
    }

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/

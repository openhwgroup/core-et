/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$ 
* @version $Release$ 
* @date $Date$ 
* @author 
*
* @brief esr_sp_rw.c Main function for all TCs 
*
* Setup SoC to enable TC run 
*/ 
/** 
 *  @Component    Test Function Configuration 
 * 
 *  @Filename     esr_sp_rw.c 
 * 
 *  @Description  Contains Test Function
 * 
 *//*======================================================================== */ 



/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */ 

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#include "esr_rw.h"

#define CAUSE_USER_ECALL         0x8
#define CAUSE_SUPERVISOR_ECALL   0x9
#define CAUSE_MACHINE_ECALL      0xb
#define CAUSE_BUS_ERROR          ((1ULL<<63) | 23)

extern "C" void  __attribute__ ((interrupt)) machine_handler();
extern "C" void  __attribute__ ((interrupt)) bad_trap();

inline void m_to_s_mode() __attribute__((always_inline));
inline void m_to_u_mode() __attribute__((always_inline));
inline void to_m_mode() __attribute__((always_inline));

volatile uint8_t excpt_expexted = 0;
volatile uint8_t excpt_happened = 0;

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
    if(excpt_expexted == 0) {
        C_TEST_FAIL;
    }
    else {
    	excpt_expexted = 0;
    	excpt_happened = 1;
    }
    __asm__ __volatile__
    (
      "li t0, 1\n"
      "slli t0, t0, 23\n"
      "csrc mip, t0\n"
       : : : "t0"
    );
    
    if (cause == CAUSE_BUS_ERROR){
      __asm__ __volatile__
        (
         //"li t0, 0x1800\n"
         //"csrs mstatus, t0\n"
         "csrr t0, 0x7d6\n"
         "addi t0, t0, 4\n"
         "csrw mepc, t0\n"
         :
         :
         : "t0" );
    }
    else {
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
}

void bad_trap(){
  C_TEST_FAIL;
}

/* esr_rw Test Function */
int main()
{
    uint64_t    first_val;
    uint64_t    second_val;
    uint64_t    expected_val;
    uint8_t     exception_expected;
    volatile uint64_t shire_id = get_shire_id();

    ESR_REG_DESC_t *pRegs;

    C_TEST_START;

    if (shire_id == 32) {

        // Set machine trap handler
        __asm__ __volatile__ 
        (
        "la t0, trap_vector\n"
        //"ori t0, t0, 1\n"
        "csrw mtvec, t0\n"
        //enable interrupt
        "li t0, 1\n"
        "slli t0, t0, 23\n"
        "csrs mie, t0\n"
        "csrsi mstatus, 8\n"
        : : : "t0"
        );

        for(int i=0; i<3; i++) {

            pRegs    = cache_Regs;

            while( pRegs->addr != 0xFFFFFFFFFFFF ) {
                
                exception_expected = 0;
                if(pRegs->PP == PP_MESSAGES) exception_expected = 1;
                else if(i == 1) {
                    if(pRegs->PP > PP_SUPERVISOR) exception_expected = 1;
                }
                else if(i == 2) {
                    if(pRegs->PP > PP_USER) exception_expected = 1;
                }

                if(exception_expected == 1) excpt_expexted = 1;
                if(pRegs->has_resetVal == 0) write_esr_new(pRegs->PP, 0, pRegs->region, pRegs->SUBREGION_ID, pRegs->addr, 0x0, 0);
                if(exception_expected == 1) excpt_expexted = 1;
                first_val = read_esr_new(pRegs->PP, 0, pRegs->region, pRegs->SUBREGION_ID, pRegs->addr, 0);
                if(exception_expected == 1) excpt_expexted = 1;
                write_esr_new(pRegs->PP, 0, pRegs->region, pRegs->SUBREGION_ID, pRegs->addr, (0x5A5A5A5A5A5A5A5A & pRegs->w_mask) | (first_val & (pRegs->w_mask ^ pRegs->r_mask)), 0);
                if(exception_expected == 1) excpt_expexted = 1;
                second_val = read_esr_new(pRegs->PP, 0, pRegs->region, pRegs->SUBREGION_ID, pRegs->addr, 0);

                if(pRegs->rw_access == RO) {
                     expected_val = first_val;
                }
                else if(pRegs->rw_access == RW) {
                    expected_val = 0x5A5A5A5A5A5A5A5A & pRegs->r_mask;
                }
                else if(pRegs->rw_access == RW1C) {
                    expected_val = first_val & 0xA5A5A5A5A5A5A5A5;
                }
                else if(pRegs->rw_access == WO) {
                    expected_val = 0x0;
                }
                else if(pRegs->rw_access == RC) {
                    expected_val = 0x5A5A5A5A5A5A5A5A & pRegs->r_mask;
                }
                else expected_val = 0;
    
                if(second_val != expected_val && exception_expected == 0 && pRegs->has_readVal == 0) {
                    C_TEST_FAIL;
                }

                if (excpt_happened == 0 && exception_expected == 1) {
                	C_TEST_FAIL;
                }
                else excpt_happened = 0;

                pRegs ++;
           
            }

            if(i==0) m_to_s_mode();
            else if(i==1) {
                to_m_mode();
                m_to_u_mode();
            }
        }
        to_m_mode();
    }

    C_TEST_PASS;

} /* esr_sp_rw_TestFunction() */

/*****     < EOF >     *****/

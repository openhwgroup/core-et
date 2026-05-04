/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "switch_modes.h"

#define MODE_USER       0
#define MODE_SUPERVISOR 1
#define MODE_MACHINE    3
#define MODE_NOBODY     2

volatile uint8_t mode =  MODE_MACHINE;

void machine_handler() {
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
 
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


int main()
{

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

  
   C_TEST_START;

   mode = MODE_USER;
   m_to_u_mode();

   for(int i=0; i<10; i++) {
       NOP;
   }

   mode = MODE_MACHINE;
   to_m_mode();

   for(int i=0; i<10; i++) {
       NOP;
   }

   mode = MODE_SUPERVISOR;
   m_to_s_mode();

   for(int i=0; i<10; i++) {
       NOP;
   }

   mode = MODE_MACHINE;
   to_m_mode();

   C_TEST_PASS;
}

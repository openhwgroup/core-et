/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "pmc_access.h"

volatile int trap_cnt=0;
volatile int err=0;

int main(){
  C_TEST_START;

  uint32_t pmc_access_scenario [ITERATIONS] = {206869365, 315911045, 181959928, 85541324 , 293866976,
                                               69115968 , 194300347, 234356350,	53275276 , 22642256 ,
                                               359622292, 66913317 , 172394855,	208275434, 299890637,
                                               103190799, 191834392, 342486995,	239016743,319186168 }; 

  unsigned id = get_hart_id();

  int expected_traps;
  uint32_t tmp;

  if ((id % 2) == 0) {
     // Set machine trap handler
     __asm__ __volatile__ 
     (
      "la t0, trap_vector\n"
      "ori t0, t0, 1\n"
      "csrw mtvec, t0\n"
      : : : "t0"
     );
 
     // Test accesses from S-mode
     for (int i=0; i<ITERATIONS; i++){
       // set access scenario
       __asm__ __volatile__ ("csrw 0x306, %[v]\n" : : [v] "r" (pmc_access_scenario[i]));

       m_to_s_mode();
       access_all_pmcs();
       to_m_mode();

       // count the number of expected traps and compare with the occurred traps
       expected_traps = 0;
       tmp = pmc_access_scenario[i];
       for (int j=0; j<PMC_LAST_IMPLEMENTED; j++){
         if (j!=1) { // time is not implemented
           if (!(tmp & 0x1)) expected_traps++;
         }
         tmp = tmp >> 1;
       }
       expected_traps += PMC_NON_IMPLEMENTED;

       if (expected_traps != trap_cnt){
         err=2;
         C_TEST_FAIL;
       }
       trap_cnt = 0;
     }

    // Test accesses from U-mode
    for (int i=0; i<ITERATIONS; i++){
       // set access scenario
       __asm__ __volatile__ ("csrw 0x306, %[v]\n" : : [v] "r" (pmc_access_scenario[i]));
       m_to_s_mode();
       __asm__ __volatile__ ("csrw 0x106, %[v]\n" : : [v] "r" (pmc_access_scenario[i]));
       to_m_mode();

       m_to_u_mode();
       access_all_pmcs();
       to_m_mode();

       // count the number of expected traps and compare with the occurred traps
       expected_traps = 0;
       tmp = pmc_access_scenario[i];
       for (int j=0; j<PMC_LAST_IMPLEMENTED; j++){
         if (j!=1) { // time is not implemented
           if (!(tmp & 0x1)) expected_traps++;
         }
         tmp = tmp >> 1;
       }
       expected_traps += PMC_NON_IMPLEMENTED;

       if (expected_traps != trap_cnt){
         err=3;
         C_TEST_FAIL;
       }
       trap_cnt = 0;
     }

  }
 
  C_TEST_PASS;
  return 0;
}

void access_all_pmcs(){
  // access pmcs in order
  uint32_t pmc_value;
  __asm__ __volatile__ ("csrr %[val], 0xc00\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc01\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc02\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc03\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc04\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc05\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc06\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc07\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc08\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc09\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0a\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0b\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0c\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0d\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0e\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc0f\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc10\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc11\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc12\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc13\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc14\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc15\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc16\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc17\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc18\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc19\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1a\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1b\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1c\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1d\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1e\n" : [val] "=r" (pmc_value));
  __asm__ __volatile__ ("csrr %[val], 0xc1f\n" : [val] "=r" (pmc_value));
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
  else if (cause == CAUSE_ILLEGAL_PMC_ACCESS) {
   trap_cnt++;
    
   // Increase PC to skip current instr
    __asm__ __volatile__ 
      (
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       : : : "t0"
      );

  } 
  else {
    err=1;
    C_TEST_FAIL;
  }
}

void bad_trap(){
  err=1;
  C_TEST_FAIL;
}

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

unsigned int get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], mhartid\n"
      : [ret] "=r" (ret)
      : 
      :
    );
    return ret;
}

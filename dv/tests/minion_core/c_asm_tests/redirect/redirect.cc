/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "redirect.h"

//using s11 as scratch to store return point
volatile uint64_t *fcc[] = { (uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_0),
                             (uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_2)};

volatile uint64_t *redirect_pc = (uint64_t*) ESR_NEIGH(0xFF,0, IPI_REDIRECT_PC);
volatile uint64_t *redirect_trigger = (uint64_t*) ESR_SHIRE(0xFF,IPI_REDIRECT_MASK);
volatile uint64_t *redirect_filter = (uint64_t*) ESR_SHIRE(0xFF,IPI_REDIRECT_FILTER);

void init(unsigned id){
  if (id == 0) {
    // simple initialization, only for id 0
    *redirect_filter = (uint64_t) -1;
  }
  __asm__ __volatile__
    (
     "la t0, mtrap_handler\n"
     "csrw mtvec, t0\n"
     "li t0, 1<<16\n"
     "csrs mie, t0\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
     );

}

// 2 steps of test => first t0 receiving and t1 sending; next t0 sending and t1 receiving
int main(){
  C_TEST_START;

  unsigned id = get_hart_id();
  init(id);

  
  switch(id)
    {
    case 0:
      // test t1 sending, t0 receiving
      test_receive(id);
      // test t0 sending, t1 receiving
      test_send(id);
      break;
    case 1:
      // test t1 sending, t0 receiving
      test_send(id);
      // test t0 sending, t1 receiving
      test_receive(id);
      break;
    default:
      break;
    }

  C_TEST_PASS;
  return 0;
}


// different redirect functions
REDIRECT_DEST_FN(0)
REDIRECT_DEST_FN(1)
REDIRECT_DEST_FN(2)
REDIRECT_DEST_FN(3)
REDIRECT_DEST_FN(4)
REDIRECT_DEST_FN(5)
REDIRECT_DEST_FN(6)
REDIRECT_DEST_FN(7)

static uintptr_t destinations [] =  {(uintptr_t) redirect_destination0,
                                     (uintptr_t) redirect_destination1,
                                     (uintptr_t) redirect_destination2,
                                     (uintptr_t) redirect_destination3,
                                     (uintptr_t) redirect_destination4,
                                     (uintptr_t) redirect_destination5,
                                     (uintptr_t) redirect_destination6,
                                     (uintptr_t) redirect_destination7,
};



void test_receive(unsigned id){

  for ( int m = 0; m < 3; m++ ) {
    for ( int i = 0; i < ITERATIONS; i++) {
      // notify other thread we are ready to receive
      switch(m)
        {
        case 0: // in m-mode
        case 1: // in s-mode
          switch(i%4)
            {
            case 0: REDIRECT_WAIT("wfi\n"); break;
            case 1: REDIRECT_WAIT("csrw stall, x0\n");break;
            default:
              REDIRECT_WAIT("2: nop\n"
                            "j 2b\n");
              break;
            }
          break;
        case 2: // in u-mode
          switch(i %2)
            {
            case 0: REDIRECT_WAIT("csrw stall,x0\n");break;
            case 1: REDIRECT_WAIT("2: nop\n"
                                  "j 2b\n");
              break;
            }
          break;
        }
    
    }

    // do an ecall => M mode will jump to S, S to U, and U to M again
    __asm__ __volatile__ ("ecall\n");
  }
  

}


void test_send(unsigned id){
  uint64_t dest_ptr = 0;
  // mask set to the other thread
  uint64_t mask = 1 << (id ^ 1);
  for ( int m = 0; m < 3; m++ ) {
    for ( int i = 0; i < ITERATIONS; i++) {
      // wait for other thread to be ready to receive
      WAIT_FCC0;
      // send trigger
      *redirect_pc = destinations[dest_ptr++];
      *redirect_trigger = mask;
      dest_ptr%= sizeof(destinations)/sizeof(uintptr_t);
    }
  }
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

void mtrap_handler() {

  // read cause, status and set mepc+=4
  uint64_t cause, status;
  __asm__ __volatile__ (
                        "csrr %[cause], mepc\n"
                        "addi %[cause], %[cause], 4\n"
                        "csrw mepc, %[cause]\n"
                        "csrr %[cause], mcause\n"
                        "csrr %[status], mstatus\n"
                        : [cause] "=r" (cause), [status] "=r" (status)
                        :
                        : 
                        );



  int next_prv = 0;
  switch(cause)
    {
      // for ecalls, select next prv mode
    case CAUSE_MACHINE_ECALL: next_prv = 1; break;
    case CAUSE_SUPERVISOR_ECALL: next_prv = 0; break;
    case CAUSE_USER_ECALL: next_prv = 3; break;
      // bad redirect => clear and return to 's11'
    case CAUSE_BAD_RED:
      __asm__ __volatile__
        (
         "mv t0, s11\n"
         "csrw mepc,t0\n"
         "li t0, 1<<16\n"
         "csrc mip, t0\n"
         : : : "t0", "s11"
         );
      return;
      break;
      
      // other causes => error
    default:
      C_TEST_FAIL;
      return;
    }

  // set mstatus.mpp ( only applies for ecalls)
  status &= ~(0x3ULL<<11);
  status |= next_prv << 11;
  __asm__ __volatile__ ("csrw mstatus, %[status]\n" : : [status] "r" (status));

}


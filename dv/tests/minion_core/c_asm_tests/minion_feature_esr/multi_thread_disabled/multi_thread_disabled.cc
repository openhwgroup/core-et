/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "et_postsi_common.h"

#define MINION_FEATURE_ADDR      0x80F40000
#define ALL_ONES                 0xFFFFFFFFFFFFFFFF
#define NSHIRES                  32
#define T0_IDLE_CYCLES           10000
#define TRAP_ON_GFX_BIT          (1<<0)
#define TRAP_ON_ML_BIT           (1<<1)
#define TRAP_ON_UCACHEOP_BIT     (1<<2)
#define DISABLE_USCP_BIT         (1<<3)
#define DISABLE_MTHREADING_BIT   (1<<4)
#define DISABLE_ULOCK_UNLOCK_BIT (1<<5)

#define CREDIT_INC_T0C0_AD        0x803400C0
#define CRED_INC_T0_C0 do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD   ) = 1; } while(0)
#define CREDIT_INC_T1C0_AD        0x803400D0

unsigned int get_hart_id();
volatile int err=0;

void test_t0();
void test_odd_t();

int main()
{
  // Init marker
  C_TEST_START;

  unsigned id = get_hart_id();
  
  if (id == 0) {
    test_t0();
  }
  else if ((id % 2) == 1) {
    test_odd_t();
  }
  else {
    c_test_pass();  
  }
  return 0;
}

void test_odd_t(){
  __asm__ __volatile__ ("csrw fcc, x0\n");

  // Send fcc credits to M0T0
  CRED_INC_T0_C0;
}

void test_t0(){

  // Disable multithreading
  __asm__ __volatile__ (
    "li t1, %[addr]\n"
    "ld t0, 0(t1)\n"
    "ori t0, t0, %[bit]\n"
    "sd t0, 0(t1)\n"
    "fence\n"
    :
    : [addr] "i" (MINION_FEATURE_ADDR),
      [bit] "i"(DISABLE_MTHREADING_BIT)   
    : "t0", "t1"
  );

  // Send fccs to all odd threads
  uint64_t esr_addr = CREDIT_INC_T1C0_AD | (0xFF<<22);
  __asm__ __volatile__ ( 
    "li t0, 0xffffffff\n" 
    "sd t0, 0(%[addr])\n" 
    : 
    : [addr] "r" (esr_addr)
    : "t0");

  for (int i = 0; i < T0_IDLE_CYCLES; ++i)
    asm volatile ("nop\n");

  uint64_t regval;
  asm volatile ("csrr %[regval], fccnb\n" : [regval] "=r" (regval));
  if (regval == 0)
  {
      c_test_pass();
  }
  else
  {
    C_TEST_FAIL
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

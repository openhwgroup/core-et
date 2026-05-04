/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"

#define MINION_FEATURE_ADDR      0x80F40000
#define ALL_ONES                 0xFFFFFFFFFFFFFFFF

#define TRAP_ON_GFX_BIT          (1<<0)
#define TRAP_ON_ML_BIT           (1<<1)
#define TRAP_ON_UCACHEOP_BIT     (1<<2)
#define DISABLE_USCP_BIT         (1<<3)
#define DISABLE_MTHREADING_BIT   (1<<4)
#define DISABLE_ULOCK_UNLOCK_BIT (1<<5)

#define ILLEGAL_INST_CAUSE       0x2
#define ITERATIONS 100

#define CSR_MASK                 ~(0x1fff<<7)
#define UCACHE_CNTRL_INST        (0x81061073 & CSR_MASK)

unsigned int get_hart_id();
volatile int exp_cnt=0;
volatile int err=0;
volatile int __attribute__((aligned(4096))) ptr[1024];

__attribute__((aligned(4096))) __attribute__ ((interrupt))
void trap_handler()
{
  uint64_t cause;
  uint32_t inst;

  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  // Shouldn't happen in this scenario   
  if ( cause == ILLEGAL_INST_CAUSE) {
    __asm__ __volatile__ (
                "csrr t0, mepc\n"
                "addi t0, t0, 4\n"
                "csrw mepc, t0\n"
                : : : "t0"
    );

    __asm__ __volatile__ ("csrr %[inst], mtval\n" : [inst] "=r" (inst));
    inst = inst & CSR_MASK;
    if (inst == UCACHE_CNTRL_INST){ 
      exp_cnt++;
    }
    else {
      err=1;
      C_TEST_FAIL;
    }
  }
  else{
    err=1;
    C_TEST_FAIL;
  }
}

int main()
{
    // Init marker
    C_TEST_START;
 
    unsigned id = get_hart_id();
    
    if ((id % 2) == 0){
      exp_cnt = 0;
      void (*mtvec_ptr)(void) = &trap_handler;

      // Configure trap vector and enable lock/unlock bit
      __asm__ __volatile__ (
          "mv t0, %[mtvec]\n"
          "csrw mtvec, t0\n"
          "li t0, %[ones]\n"
          "xori t0, t0, %[bit]\n"
          "li t1, %[addr]\n"
          "ld t2, 0(t1)\n"
          "and t2, t2, t0\n" 
          "sd t2, 0(t1)\n"
          "fence\n"
        :
        : [addr] "i" (MINION_FEATURE_ADDR),
          [bit] "i"(DISABLE_USCP_BIT),
          [ones] "i"(ALL_ONES),
          [mtvec] "r" (mtvec_ptr)
        : "t0", "t1", "t2");

      // Set to split mode
      //              d1split    scp_en
      //              --------   ------
      mcache_control(    1,        0   );
      FENCE;       // VERIF-3295 (PRM-8)

      for (int i=0; i<ITERATIONS; i++){
        switch(i%2)
          {
          case 0:
            //             scp_en   cacheop_rate    cacheop_max  warl = 0 
            //            --------  -------------   -----------  -------- 
            ucache_control( true,         0,             0              );
            break;
          case 1:
            //             scp_en   cacheop_rate    cacheop_max  warl = 0 
            //            --------  -------------   -----------  -------- 
            ucache_control( false,         0,             0              );
            break;
          default:
            break;
          }
        FENCE;       // VERIF-3295 (PRM-8)
      }

      if (exp_cnt>0){
        err=2;
        C_TEST_FAIL;
      } 
    }

    C_TEST_PASS;
    return 0;
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



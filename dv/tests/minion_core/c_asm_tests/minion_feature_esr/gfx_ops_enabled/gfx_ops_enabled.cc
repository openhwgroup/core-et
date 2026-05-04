/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#define MINION_FEATURE_ADDR      0x80F40000
#define ALL_ONES                 0xFFFFFFFFFFFFFFFF

#define TRAP_ON_GFX_BIT          (1<<0)
#define TRAP_ON_ML_BIT           (1<<1)
#define TRAP_ON_UCACHEOP_BIT     (1<<2)
#define DISABLE_USCP_BIT         (1<<3)
#define DISABLE_MTHREADING_BIT   (1<<4)
#define DISABLE_ULOCK_UNLOCK_BIT (1<<5)

#define ILLEGAL_INST_CAUSE       0x2
#define ITERATIONS               5*28

#define CSR_MASK                 ~(0x1fff<<7)
#define FCVT_PS_F11_INST	 (0xd09000fb & CSR_MASK) 
#define FCVT_PS_F10_INST	 (0xd08000fb & CSR_MASK) 
#define FCVT_PS_UN24_INST	 (0xd10000fb & CSR_MASK) 
#define FCVT_PS_UN16_INST	 (0xd11000fb & CSR_MASK) 
#define FCVT_PS_UN10_INST	 (0xd12000fb & CSR_MASK) 
#define FCVT_PS_UN8_INST	 (0xd13000fb & CSR_MASK) 
#define FCVT_PS_UN2_INST	 (0xd17000fb & CSR_MASK) 
#define FCVT_PS_SN16_INST	 (0xd19000fb & CSR_MASK) 
#define FCVT_PS_SN8_INST	 (0xd1b000fb & CSR_MASK) 
#define FCVT_F11_PS_INST	 (0xd88000fb & CSR_MASK) 
#define FCVT_F10_PS_INST	 (0xd8b000fb & CSR_MASK) 
#define FCVT_UN24_PS_INST	 (0xd90000fb & CSR_MASK) 
#define FCVT_UN16_PS_INST	 (0xd91000fb & CSR_MASK) 
#define FCVT_UN10_PS_INST	 (0xd92000fb & CSR_MASK) 
#define FCVT_UN8_PS_INST	 (0xd93000fb & CSR_MASK) 
#define FCVT_UN2_PS_INST	 (0xd97000fb & CSR_MASK) 
#define FCVT_SN16_PS_INST	 (0xd99000fb & CSR_MASK) 
#define FCVT_SN8_PS_INST	 (0xd9b000fb & CSR_MASK) 
#define CUBEFACE_PS_INST	 (0x8800817b & CSR_MASK) 
#define CUBEFACEIDX_PS_INST	 (0x8800917b & CSR_MASK) 
#define CUBESGNSC_PS_INST	 (0x8800a17b & CSR_MASK) 
#define CUBESGNTC_PS_INST	 (0x8800b17b & CSR_MASK) 
#define BITMIXB_INST	         (0x8000f13b & CSR_MASK) 
#define FCVT_PS_RAST_INST	 (0xd02000fb & CSR_MASK) 
#define FCVT_RAST_PS_INST	 (0xc02000fb & CSR_MASK) 
#define FRCP_FIX_RAST_INST	 (0x3000817b & CSR_MASK) 
#define MASKPOPC_RAST_INST	 (0x5e18007b & CSR_MASK) 
#define TEX_SEND_INST	         (0x807027f3 & CSR_MASK) 

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
    if ( (inst == FCVT_PS_F11_INST) || (inst == FCVT_PS_F10_INST) || (inst == FCVT_PS_UN24_INST) || (inst == FCVT_PS_UN16_INST) || (inst == FCVT_PS_UN10_INST) ||
         (inst == FCVT_PS_UN8_INST) || (inst == FCVT_PS_UN2_INST) || (inst == FCVT_PS_SN16_INST) || (inst == FCVT_PS_SN8_INST) || (inst == FCVT_F11_PS_INST) ||
         (inst == FCVT_F10_PS_INST) || (inst == FCVT_UN24_PS_INST) || (inst == FCVT_UN16_PS_INST) || (inst == FCVT_UN10_PS_INST) || (inst == FCVT_UN8_PS_INST) ||
         (inst == FCVT_UN2_PS_INST) || (inst == FCVT_SN16_PS_INST) || (inst == FCVT_SN8_PS_INST) || (inst == CUBEFACE_PS_INST) || (inst == CUBEFACEIDX_PS_INST) ||
         (inst == CUBESGNSC_PS_INST) || (inst == CUBESGNTC_PS_INST) || (inst == BITMIXB_INST) || (inst == FCVT_PS_RAST_INST) || (inst == FCVT_RAST_PS_INST) ||
         (inst == FRCP_FIX_RAST_INST) || (inst == MASKPOPC_RAST_INST) || (inst == TEX_SEND_INST) 
       )
    {     
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
          [bit] "i"(TRAP_ON_GFX_BIT),
          [ones] "i"(ALL_ONES),
          [mtvec] "r" (mtvec_ptr)
        : "t0", "t1", "t2");

      uint64_t regvalue;

      for (int i=0; i<ITERATIONS; i++){
        switch(i%28)
          {
          case 0:
            asm volatile ("fcvt.ps.f11 f1, f0\n");
            break;
          case 1:
            asm volatile ("fcvt.ps.f10 f1, f0\n");
            break;
          case 2:
            asm volatile ("fcvt.ps.un24 f1, f0\n");
            break;
          case 3:
            asm volatile ("fcvt.ps.un16 f1, f0\n");
            break;
          case 4:
            asm volatile ("fcvt.ps.un10 f1, f0\n");
            break;
          case 5:
            asm volatile ("fcvt.ps.un8 f1, f0\n");
            break;
          case 6:
            asm volatile ("fcvt.ps.un2 f1, f0\n");
            break;
          case 7:
            asm volatile ("fcvt.ps.sn16 f1, f0\n");
            break;
          case 8:
            asm volatile ("fcvt.ps.sn8 f1, f0\n");
            break;
          case 9:
            asm volatile ("fcvt.f11.ps f1, f0\n");
            break;
          case 10:
            asm volatile ("fcvt.f10.ps f1, f0\n");
            break;
          case 11:
            asm volatile ("fcvt.un24.ps f1, f0\n");
            break;
          case 12:
            asm volatile ("fcvt.un16.ps f1, f0\n");
            break;
          case 13:
            asm volatile ("fcvt.un10.ps f1, f0\n");
            break;
          case 14:
            asm volatile ("fcvt.un8.ps f1, f0\n");
            break;
          case 15:
            asm volatile ("fcvt.un2.ps f1, f0\n");
            break;
          case 16:
            asm volatile ("fcvt.sn16.ps f1, f0\n");
            break;
          case 17:
            asm volatile ("fcvt.sn8.ps f1, f0\n");
            break;
          case 18:
            asm volatile ("cubeface.ps f2, f1, f0\n");
            break;
          case 19:
            asm volatile ("cubefaceidx.ps f2, f1, f0\n");
            break;
          case 20:
            asm volatile ("cubesgnsc.ps f2, f1, f0\n");
            break;
          case 21:
            asm volatile ("cubesgntc.ps f2, f1, f0\n");
            break;
          case 22:
            asm volatile ("bitmixb x2, x1, x0\n");
            break;
          case 23:
            asm volatile ("fcvt.ps.rast f1, f0\n");
            break;
          case 24:
            asm volatile ("fcvt.rast.ps f1, f0\n");
            break;
          case 25:
            asm volatile ("frcp_fix.rast f2, f1, f0\n");
            break;
          case 26:
            asm volatile ("maskpopc.rast x0, m0, m1, 2\n");           
            break;
          case 27:
            __asm__ __volatile__ ("csrr %[regvalue], tex_send\n" : [regvalue] "=r" (regvalue));
            break;
          default:
            break;
          }
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



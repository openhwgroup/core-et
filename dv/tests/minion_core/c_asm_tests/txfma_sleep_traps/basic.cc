/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

#define ITERATIONS 100

#define STRING(x) #x

#define CSRWI(dest, data)  \
  __asm__ __volatile__ ( "li t0, %[d]\n" \
                         "csrw " STRING(dest) ", t0\n" \
                         : : [d] "i" (data) : "t0" );

#define CSRW(dest, data)  \
  __asm__ __volatile__ ( "csrw " STRING(dest) ", %[d]\n" \
                         : : [d] "r" (data): );

#define CSRR(csr, data)  \
  __asm__ __volatile__ ( "csrr %[d]," STRING(csr) "\n" \
                         : [d] "=r" (data) : : );


typedef struct  {
  int others: 1;
  int trans: 1;
  int tensor_reduce_fp32: 1;
  int tensor_quant: 1;
  int tensorfma_fp16_fp32:1;
} __attribute__ ((packed)) trap_conf_t;

#define TEST( what, sleep, has_to_trap)                                 \
  do {                                                                  \
    /* set debug info in validation CSR */                              \
    /* (to help debug with waveforms) */                                \
    CSRW(0x8d0, i);   /*iter*/                                          \
    CSRW(0x8d1, has_to_trap && sleep); /*trap expected*/                \
    CSRW(0x8d2, 0);  /*has trapped*/                                    \
    /* and do the test */                                               \
    what;                                                               \
    /* check results (from validation2) */                              \
    CSRR(0x8d2, trapped);                                               \
    if ( trapped != ((has_to_trap && sleep)? 1: 0))                     \
      die();                                                            \
  } while (0)

#define F1SRC(opcode)                                                   \
  do {                                                                  \
    __asm__ __volatile__ (                                              \
                          opcode " f0, f1\n"                            \
                          : : : "f0" );                                 \
  } while(0)

#define F2SRC(opcode)                                                   \
  do {                                                                  \
    __asm__ __volatile__ (                                              \
                          opcode " f0, f1, f0\n"                     \
                          : : : "f0" );                                 \
  } while(0)


#define F3SRC(opcode)                                                   \
  do {                                                                  \
    __asm__ __volatile__ (                                              \
                          opcode " f0, f1, f0, f2\n"                    \
                          : : : "f0" );                                 \
  } while(0)

#define I2SRC(opcode)  __asm__ __volatile__ ( opcode " x0, x1, x2\n")

#define TENSORFMA_FP16    CSRWI(0x801, 1)
#define TENSORFMA_FP32    CSRWI(0x801, 0)
#define TENSORFMA_I8      CSRWI(0x801, 3)
#define TENSOR_REDUCE_F   CSRWI(0x800, 0 ) //fadd
#define TENSOR_REDUCE_I   CSRWI(0x800, 4<<24) //iadd
#define TENSOR_QUANT      CSRWI(0x806, 0)
#define FRCP_PS F1SRC("FRCP.PS")
#define FRSQ_PS F1SRC("FRSQ.PS")
#define FLOG_PS F1SRC("FLOG.PS")
#define FEXP_PS F1SRC("FEXP.PS")
#define FSIN_PS F1SRC("FSIN.PS")
#define FADD_PS F2SRC("fadd.ps")
#define FMADD_PS F3SRC("fmadd.ps")
#define FMIN_PI F2SRC("fadd.pi")
#define FMAXU_PI F2SRC("fadd.pi")
#define ADD I2SRC("add")
#define SUB I2SRC("sub")


void die() {
  // force test to end with timeout
  for(;;);
}

extern "C" void mtrap_vector();

int rand_seed = 0xDEAD;
int randNR (){
  /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
  int bit  = ((rand_seed >> 0) ^ (rand_seed >> 2) ^ (rand_seed >> 3) ^ (rand_seed >> 5) ) & 1;
  rand_seed =  (rand_seed >> 1) | (bit << 15);
  return rand_seed;
}

int main()
{
  uint64_t sleep;
  int trapped;
  union {
    uint64_t  val;
    trap_conf_t str;
  } conf;
  trap_conf_t *trap_conf = &(conf.str);
  
  // config trap vector
  CSRW( mtvec, mtrap_vector);

  
  for ( int i = 0; i < ITERATIONS; i++) {
    
    // set txfma sleep
    sleep = randNR() % 4 == 0 ? 0 : 1;
    CSRW(0x7d1, sleep);  
   
    // set trap configuration
    conf.val = randNR();
    CSRW(0x7d4, conf.val);


    // and do the test...
    switch ( randNR() % 17 )
      {
        //instructions that may have to trap
      case  0: TEST( TENSORFMA_FP16,    sleep, trap_conf->tensorfma_fp16_fp32); break;
      case  1: TEST( TENSORFMA_FP32,    sleep, trap_conf->tensorfma_fp16_fp32); break;
      case  2: TEST( TENSOR_REDUCE_F,   sleep, trap_conf->tensor_reduce_fp32); break;
      case  3: TEST( TENSOR_QUANT,      sleep, trap_conf->tensor_quant); break;
      case  4: TEST( FRCP_PS,           sleep, trap_conf->trans); break;
      case  5: TEST( FRSQ_PS,           sleep, trap_conf->trans); break;
      case  6: TEST( FLOG_PS,           sleep, trap_conf->trans); break;
      case  7: TEST( FEXP_PS,           sleep, trap_conf->trans); break;
      case  8: TEST( FSIN_PS,           sleep, trap_conf->trans); break;
      case  9: TEST( FADD_PS,           sleep, trap_conf->others); break;
      case 10: TEST( FMADD_PS,          sleep, trap_conf->others); break;
        
        //instruction that never have to trap
      case 11: TEST( TENSORFMA_I8,     sleep, 0); break;
      case 12: TEST( TENSOR_REDUCE_I,  sleep, 0); break;
      case 13: TEST( NOP,              sleep, 0); break;
      case 14: TEST( FMIN_PI,          sleep, 0); break;
      case 15: TEST( FMAXU_PI,         sleep, 0); break;
      case 16: TEST( ADD,              sleep, 0); break;


      }
  }
  
  // End marker
  C_TEST_PASS;
  
  return 0;
}

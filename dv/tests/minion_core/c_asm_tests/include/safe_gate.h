/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


// Interrput routine for safe power off according to power spec

#ifndef __SAFE_GATE_H
#define __SAFE_GATE_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "common.h"
#include "fcc.h"

#define EXP_CAUSE ((1ULL<<63) | 3)
#define ACOUNTER_BASE_ADDR 0x0000008100000000
#define ACOUNTER_SIZE 64
#define IPI_TRIGGER_CLEAR 0x80F40098 >> 3
#define MIN_PER_S 32

inline __attribute__((always_inline)) uint64_t atomic_read_inc(uint64_t address)
{
   uint64_t rcv_data = 0;
   __asm__ __volatile__ (
               "li a5, 0x1\n\t"
               "amoaddg.d %[result], a5, 0(%[addr])\n\t"
               : [result] "=r" (rcv_data)
               : [addr] "r" (address)
               : "a5"
   );

   return rcv_data;
}

EXTERN_C void  __attribute__ ((interrupt)) safe_gate() {
  // Power spec safe off rouitne

  // disable interrupts
  __asm__ __volatile__ 
  (
    "csrci mstatus, 8\n"  // set MIE to '0', disable interrupts    	
    "csrwi mie, 0\n"      // disable all types of interrupts

  );
  FENCE

  volatile uint64_t hart_id = getHartId();
  volatile uint64_t thread_id  = (hart_id & 1);
  volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
  volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);  

  // clear the interrupt
  write_esr(PP_MACHINE, 0xFF, REGION_OTHER, IPI_TRIGGER_CLEAR, 0x1 << hart_id);

  // Even and odd thread don't do the same code
  if (!thread_id) {
    volatile uint64_t acounter_addr = minion_id * ACOUNTER_SIZE + (ACOUNTER_BASE_ADDR + shire_id * ACOUNTER_SIZE * MIN_PER_S);
    volatile uint64_t check_data;
    // Clear L1
    clear_l1d();
    // Set atomic counter to 1
    // Evict style          
    //store(acounter_addr, 1);
    //FENCE
    //evict_va(0, to_Mem, acounter_addr);
    //WAIT_CACHEOPS
    // Atomic style
    check_data = atomic_read_inc(acounter_addr);
    if (check_data != 0) {
      #ifdef  MINSHIRE_GATESIM_ENABLE
      if (hart_id == 0) {
        atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR); 
      }
      #endif
      C_TEST_FAIL; 	
    }  

    // Disable msg ports and dropping messages
    __asm__ __volatile__ 
    (
      "csrci 0x9cc, 1\n"
      "csrci 0x9cd, 1\n"
      "csrci 0x9ce, 1\n" 
      "csrci 0x9cf, 1\n"        
    );   
    // Consumes all the FCCs
    volatile uint16_t cred_num;
    for (int i=0; i < 2; i++) {
    cred_num = get_credit_count_nb(i);
    	for (int j=0; j < cred_num; j++) {
      	wait_for_credit(i);
    	}   	
    }

    //Issues a TensorWait CSRRW per each valid event id one can provide through the "xs" register
    __asm__ __volatile__ ( "csrwi tensor_wait, 0\n");  //TensorLoad_0
    __asm__ __volatile__ ( "csrwi tensor_wait, 1\n");  //TensorLoad_1
    __asm__ __volatile__ ( "csrwi tensor_wait, 2\n");  //TensorLoad_2_0
    __asm__ __volatile__ ( "csrwi tensor_wait, 3\n");  //TensorLoad_2_1
    __asm__ __volatile__ ( "csrwi tensor_wait, 4\n");  //Prefetch_0
    __asm__ __volatile__ ( "csrwi tensor_wait, 5\n");  //Prefetch_1
    __asm__ __volatile__ ( "csrwi tensor_wait, 6\n");  //CacheOp
    FENCE
    __asm__ __volatile__ ( "csrwi tensor_wait, 7\n");  //TensorFMA
    __asm__ __volatile__ ( "csrwi tensor_wait, 8\n");  //TensorStore
    __asm__ __volatile__ ( "csrwi tensor_wait, 9\n");  //TensorReduce
    __asm__ __volatile__ ( "csrwi tensor_wait, 10\n");  //TensorQuant

    // Set atomic counter to 2
    // Evict style       
    //store(acounter_addr, 2);
    //FENCE
    //evict_va(0, to_Mem, acounter_addr);
    //WAIT_CACHEOPS
    // Atomic style
    check_data = atomic_read_inc(acounter_addr);
    if (check_data != 1) {
      #ifdef MINSHIRE_GATESIM_ENABLE
      if (hart_id == 0) {
        atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR); 
      }
      #endif       
      C_TEST_FAIL; 	
    }     

    // Clear TensorError ESR
    __asm__ __volatile__ ("csrw tensor_error, x0\n");
    FENCE

    //Flush the DCache again
    clear_l1d();

    // Set atomic counter to 3
    //store(acounter_addr, 3);
    //FENCE
    //evict_va(0, to_Mem, acounter_addr);
    //WAIT_CACHEOPS
    // Atimic style
    check_data = atomic_read_inc(acounter_addr);
    if (check_data != 2) {
      #ifdef MINSHIRE_GATESIM_ENABLE
      if (hart_id == 0) {
        atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR); 
      }
      #endif  
      C_TEST_FAIL; 	
    }     

    #ifdef MINSHIRE_GATESIM_ENABLE
    if (hart_id == 0) {
      atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
    }
    #endif

    WFI
  } else {

    // Consumes all the FCCs
    volatile uint16_t cred_num;
    for (int i=0; i < 2; i++) {
    cred_num = get_credit_count_nb(i);
    	for (int j=0; j < cred_num; j++) {
      	wait_for_credit(i);
      }
    }  

    WFI
  }
}

#endif
/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "tensors.h"
#include "esr.h"

#define NUM_ITER 20
#define NEIGH0_IPI_REDIRECT_PC 0x80100040

int main()
{
    uint64_t tid, sid;
    // Init marker
    C_TEST_START;

    volatile uint64_t *esr_poll_value;

    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );

    // Get thread and shire ID
    tid = get_thread_id();
    sid = get_shire_id();    

    if (tid == 0 && sid == 0) {
      // Set M0 Mask FF
      __asm__ __volatile__
      (
       "mov.m.x m0, zero, 0xff\n"
       :
       : 
       : "memory"
      );

      // initFRegs      
      __asm__ __volatile__
        (
         "fbci.ps f0,0\n"
         "fbci.ps f1,1\n"
         "fbci.ps f2,2\n"
         "fbci.ps f3,3\n"
         "fbci.ps f4,4\n"
         "fbci.ps f5,5\n"
         "fbci.ps f6,6\n"
         "fbci.ps f7,7\n"
         "fbci.ps f8,8\n"
         "fbci.ps f9,9\n"
         "fbci.ps f10,10\n"
         "fbci.ps f11,11\n"
         "fbci.ps f12,12\n"
         "fbci.ps f13,13\n"
         "fbci.ps f14,14\n"
         "fbci.ps f15,15\n"
         "fbci.ps f16,16\n"
         "fbci.ps f17,17\n"
         "fbci.ps f18,18\n"
         "fbci.ps f19,19\n"
         "fbci.ps f20,20\n"
         "fbci.ps f21,21\n"
         "fbci.ps f22,22\n"
         "fbci.ps f23,23\n"
         "fbci.ps f24,24\n"
         "fbci.ps f25,25\n"
         "fbci.ps f26,26\n"
         "fbci.ps f27,27\n"
         "fbci.ps f28,28\n"
         "fbci.ps f29,29\n"
         "fbci.ps f30,30\n"
         "fbci.ps f31,31\n"
         :
         : 
         : "memory"
        );

      setup_cache_scp();

      // Set configurations
      uint64_t start_regs[NUM_ITER] = {0,0,0,0,0,0,0,0,0,0,1,2,30,5,6,7,8,9,16,18};
      uint64_t operations[NUM_ITER] = {TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
			       TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
			       TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,
			       TENSOR_REDUCE_OP_FMIN,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IMAX,
			       TENSOR_REDUCE_OP_IMIN,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,TENSOR_REDUCE_OP_FMIN};
      uint64_t num_regs[NUM_ITER] = {32,32,32,32,32,32,32,32,32,32,10,15,10,10,16,8,1,5,3,26};
      uint64_t levels[NUM_ITER] = {0,1,2,3,4,4,3,2,1,0,3,2,3,4,3,2,1,3,4,3};
      uint64_t actions[NUM_ITER] = {3,3,3,3,3,2,2,2,2,2,3,2,3,3,2,2,2,2,3,3};    

      for ( int i = 0; i < NUM_ITER; i++) {
          uint64_t start_reg = start_regs[i];
	  uint64_t operation = operations[i];
	  uint64_t num_reg = num_regs[i];
	  uint64_t level = levels[i];
	  uint64_t action = actions[i];
          uint64_t mask = (start_reg << 57) | (operation << 24) |
	                  (num_reg << 16) | (level << 3) | action;
          // Tensor reduce
          __asm__ __volatile__
            (
             "  csrw 0x800,  %[conf]\n"
             :
             : [conf] "r" (mask)
             : "memory"
            );
       }    
       WAIT_TENSOR_REDUCE;                  
    }
    // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}


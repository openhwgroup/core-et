/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"
#include "stdio.h"
#include "tensors.h"
#include "et_test_common.h"

volatile int __attribute__((aligned(4096))) ptr[1024];
#define NUM_ITER 10
#define NEIGH0_IPI_REDIRECT_PC 0x80100040

inline __attribute__((always_inline)) void store(long addr, long value) {

    __asm__ __volatile__
        (
         "  sd %[value], 0(%[addr])\n"
         :
         : [addr] "r" (addr), [value] "r" (value)
         : "memory"
        );
}

int main()
{
    // Init marker
    C_TEST_START;
    uint64_t tid;
    tid = get_thread_id();
    //unsigned id = get_hart_id();

    // Set configurations
    et_tensor_load_conf_t load_conf_matrixA;
    et_tensor_load_conf_t load_conf_matrixB;
    et_tensor_fma_conf_t fma_conf;
    
    // Matrix A
    load_conf_matrixA.use_tmask      = false;
    load_conf_matrixA.use_coop       = false;
    load_conf_matrixA.use_tenb       = false;
    load_conf_matrixA.dst_start      = 0;
    load_conf_matrixA.transformation = 0x0UL;
    load_conf_matrixA.rd_l2scp       = 0x0U;
    load_conf_matrixA.addr           = (uint64_t)(void*)(ptr);
    load_conf_matrixA.offset         = 0;
    load_conf_matrixA.num_lines      = 15;
    load_conf_matrixA.stride         = 64;
    load_conf_matrixA.id             = 0;  
   
    // Matrix B
    load_conf_matrixB.use_tmask      = false;
    load_conf_matrixB.use_coop       = false;
    load_conf_matrixB.use_tenb       = true;
    load_conf_matrixB.dst_start      = 128;
    load_conf_matrixB.transformation = 0x0UL;
    load_conf_matrixB.rd_l2scp       = 0x0U;
    load_conf_matrixB.addr           = (uint64_t)(void*)(ptr+512);
    load_conf_matrixB.offset         = 0;
    load_conf_matrixB.num_lines      = 15;
    load_conf_matrixB.stride         = 64;
    load_conf_matrixB.id             = 0;
  
    // Tensor FMA configuration
    fma_conf.use_tmask   = false;
    fma_conf.b_num_col   = 3;
    fma_conf.a_num_rows  = 15;
    fma_conf.a_num_cols  = 15;
    fma_conf.offset      = 0;
    fma_conf.use_tenb    = 0x1UL;
    fma_conf.scp_loc_b   = 0x128; 
    fma_conf.scp_loc_a   = 0x0;
    fma_conf.opcode      = 3;
    fma_conf.first_pass  = true;

    //if ((id % 64) == 0) {
    if (tid == 0) {
      // Dummy operations to clean unknown values from debug signals
      
      // Enable SCP
      setup_cache_scp();

      // Dummy tfma     
      // Load matrices A and B
      et_tensor_load (&load_conf_matrixA);
      WAIT_TENSOR_LOAD_0;
      et_tensor_load (&load_conf_matrixB);
      //WAIT_TENSOR_LOAD_1;
      et_tensor_fma(&fma_conf);  
      WAIT_TENSOR_FMA;

      // Dummy tensor quant
      //           start_reg col row scp_loc transf9 transf8 transf7 transf6 transf5 transf4 transf3 transf2 transf1 transf0                      
      //           --------- --- --- ------  ------- ------- ------- ------- ------- ------- ------- ------- ------- -------
      tensor_quant(    0,     0,  0,   0,       0,      0,      0,      0,      0,      0,      0,      0,       0,     1  );

      // Dummy tensor reduce  
      uint64_t start_reg = 0;
      uint64_t operation = FGET;
      uint64_t num_reg   = 6;
      uint64_t partnerID = 0;
      uint64_t action    = 3;
      tensor_reduce(start_reg, operation, num_reg, partnerID, action);
     
      // signal to configure debug
      store(NEIGH0_IPI_REDIRECT_PC, 0xEC);
    }

    volatile uint64_t *esr_poll_value;

    // Wait for the debugger to write this ESR
    esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
    while ( *esr_poll_value != 0xFE );

    //if ((id % 64) == 0) {
    if (tid == 0) {
    
      for ( int i = 0; i < NUM_ITER; i++) {
          // Load matrices A and B
          et_tensor_load (&load_conf_matrixA);
          WAIT_TENSOR_LOAD_0;
          et_tensor_load (&load_conf_matrixB);
          //WAIT_TENSOR_LOAD_1;

          et_tensor_fma(&fma_conf);  
          WAIT_TENSOR_FMA;

          fma_conf.a_num_rows--;
      }

    }
    // Finish minion test (it will perform a WFI)
    C_TEST_PASS;
    return 0;
}


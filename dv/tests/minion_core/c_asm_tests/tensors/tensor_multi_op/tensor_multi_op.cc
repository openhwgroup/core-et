/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Directed test that exercices the basic functionality
//               of the Esperanto tensor extensions (Load/FMA) 
//               through basic tensor memory and computation instructions.
// 
// Note: 
//       - Machine trap handler is configured in this test in such way that the test would branch to DEAD, should 
//        an exception occur.  
//       - Message size is set to DWORD and always contain the sender minion id.
// FUTURE:
//       Randomize
//          - use_tenb 
//          - Add more tensor loads to increase the umber of outstanding loads (maximum 4 outstanding tensor_loads)
//          - Fetch tensor data first to L2 and then do an tensor load to L1
//          - Add T1 load to TENB or random instructions for T1 (load misses perhaps)
//          


#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"
#include "et_test_common.h"

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      12            \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );

int main()
{

   uint64_t mid, tid;

   volatile cl_data_t *buffer = &_binary_blob_bin_start;
   uint64_t tensor_scp_way    = 0;

   // Init marker
   C_TEST_START;

   //1.0 Get minion ID and thread id
   mid = get_minion_id();
   tid = get_thread_id();

   // Get random way for scratchpad allocation in D1SHARED mode
   tensor_scp_way = (uint64_t) et_get_rand_word(0,3);

   // Tensor load matrix A dimensions
   uint64_t tensor_num_lines  = 8;

   //
   // Tensor Load configuration
   //
   et_tensor_load_conf_t  load_conf_matrixA;
   et_tensor_load_conf_t  load_conf_matrixB;

   // Matrix A
   load_conf_matrixA.use_tmask      = false;
   load_conf_matrixA.use_coop       = false;
   load_conf_matrixA.use_tenb       = false;
   load_conf_matrixA.dst_start      = 0;
   load_conf_matrixA.transformation = 0x0UL;
   load_conf_matrixA.rd_l2scp       = 0x0U;
   load_conf_matrixA.addr           = (uint64_t)(void*)buffer[mid].data[0];
   load_conf_matrixA.offset         = 0;
   load_conf_matrixA.num_lines      = tensor_num_lines-1;
   load_conf_matrixA.stride         = 64;
   load_conf_matrixA.id             = 0;

   // Matrix B
   load_conf_matrixB.use_tmask      = false;
   load_conf_matrixB.use_coop       = false;
   load_conf_matrixB.use_tenb       = true;
   load_conf_matrixB.dst_start      = ((tensor_scp_way+1)%4)*0x10UL;
   load_conf_matrixB.transformation = 0x0UL;
   load_conf_matrixB.rd_l2scp       = 0x0U;
   load_conf_matrixB.addr           = (uint64_t)(void*)buffer[mid].data[0] + tensor_num_lines*0x40;
   load_conf_matrixB.offset         = 0;
   load_conf_matrixB.num_lines      = tensor_num_lines-1;
   load_conf_matrixB.stride         = 64;
   load_conf_matrixB.id             = 0;

   //
   // Tensor FMA configuration
   //
   et_tensor_fma_conf_t fma_conf;

   fma_conf.use_tmask   = false;
   fma_conf.b_num_col   = 3;
   fma_conf.a_num_rows  = tensor_num_lines-1;
   fma_conf.a_num_cols  = tensor_num_lines-1;
   fma_conf.offset      = 0;
   fma_conf.use_tenb    = 0x1UL;
   fma_conf.scp_loc_b   = ((tensor_scp_way+1)%4)*0x10UL; 
   fma_conf.scp_loc_a   = 0x0;
   fma_conf.opcode      = 3;

   // Clear FP regs before doing the FMA
   fma_conf.first_pass  = true;


   if (tid == 0)
   {
      // Load some random value to the FP regs since the FMA operation
      // is configured to clear the regs before it starts (fma_conf.first_pass = true)
      init_fp_regs((void *) buffer[mid].data);
    
     /*** 
      // 
      // Shared Mode
      mcache_control(0,0,0);
      FENCE;
      WAIT_CACHEOPS;

      // Clear the L1 to avoid following locks to fail
      clear_l1d();
      WAIT_CACHEOPS;

      // D1Split Mode
      mcache_control(1,0,0);
      WAIT_CACHEOPS;
      FENCE;   // PRM-8; VERIF-3295

      mcache_control(1,1,0);
      WAIT_CACHEOPS;
     ***/
      setup_cache_scp();

      // Setup tensor mask (unsused for now)
      tensor_mask (0x0, 0x0);	

      // Load matrices A and B
      et_tensor_load (&load_conf_matrixA);
      et_tensor_load (&load_conf_matrixB);


      // Have to wait for the tensor loads to finish before issuing the tensor FMA 
      // since the tensor load is a csr write and retires immediately before the load(s) finishes.
      WAIT_TENSOR_LOAD_0;
      WAIT_TENSOR_LOAD_1;
      
      // FMA 
      // Note: FMA always blocks until TENB load finishes, hence we do not need another tensor fence
      et_tensor_fma(&fma_conf);
      
      // Wait for the FMA to finish before jumping to pass otherwise the checker may exit immediately
      WAIT_TENSOR_FMA;

      C_TEST_PASS;
    } 
    else 
    {
      C_TEST_PASS;
    }

   return 0;
}

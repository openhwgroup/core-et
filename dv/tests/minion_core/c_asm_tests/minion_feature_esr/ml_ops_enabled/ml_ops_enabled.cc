/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "tensors.h"
#include "ml_ops_enabled.h"
#include "et_postsi_common.h"
#include "et_test_common.h"
#include "cacheops.h"


volatile int exp_cnt=0;
volatile int err=0;
volatile int __attribute__((aligned(4096))) ptr[1024];

void trap_handler()
{
  uint64_t cause;
  uint32_t inst;
  unsigned int hart_id = get_hart_id();

  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));
  
  // Shouldn't happen in this scenario   
  if (cause == ILLEGAL_INST_CAUSE) {
    __asm__ __volatile__ (
                "csrr t0, mepc\n"
                "addi t0, t0, 4\n"
                "csrw mepc, t0\n"
                : : : "t0"
    );

    __asm__ __volatile__ ("csrr %[inst], mtval\n" : [inst] "=r" (inst));
    inst = inst & CSR_MASK;
    if ( (inst == TENSOR_REDUCE_INST) || (inst == TENSOR_FMA_INST) || (inst == TENSOR_CONV_SIZE_INST) || (inst == TENSOR_CONV_CTRL_INST) || 
         (inst == TENSOR_COOP_INST) || (inst == TENSOR_MASK_INST) || (inst == TENSOR_QUANT_INST) || (inst == TENSOR_ERROR_INST) ||
         (inst == FLB_INST) || (inst == FCC_INST) || (inst == STALL_INST) || (inst == TENSOR_WAIT_INST) ||  
         (inst == TENSOR_LOAD_L2SCP_INST) || (inst == FCCNB_INST) || (inst == TENSOR_LOAD_INST) || (inst == TENSOR_STORE_INST)){ 
      exp_cnt++;
    }    else {
      err=1;
      C_TEST_FAIL;
    }
  } 
  else if (cause == MSIP_CAUSE) {
    // (Charles) Why need to mepc+4 ? Shoud not. (mepc has already +4.)
    /**** 
    __asm__ __volatile__ (
                "csrr t0, mepc\n"
                "addi t0, t0, 4\n"
                "csrw mepc, t0\n"
                : : : "t0"
    );
    ****/

    //CLEAR_MSIP;  // clear IPI of Hart 0
    NEW_CLEAR_MSIP(hart_id);  // clear IPI of this Hart
    __asm__ __volatile__ ("fence\n");   // important: to ensure that the (above) CLEAR_MSIP is completed (see VERIF-3962) 
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

  unsigned int hart_id = get_hart_id();
  
  // Charles: change to work for pairs like {M0, M1}, {M2, M3}, {M4, M5}, etc.
  // Minion ID are specified by +MINION_MASK=xxx (in file "minion_diags.py");
  if ((hart_id % 4) == 0)    // was: if((id % 64) == 0) 
    test_t0(hart_id);
  else if ((hart_id % 4) == 2)   // was: else if ((id % 64) == 2)
    test_t2(hart_id);
  else {
    c_test_pass();
  }
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

// for Hart 0, 4, 8, etc.
void test_t0(unsigned int hart_id) {
  exp_cnt = 0;
  void (*mtvec_ptr)(void) = &trap_handler;
  unsigned int min_id = hart_id / 2;

  // Configure trap vector and enable lock/unlock bit
  // also program MINION_FEATURE ESR, set TRAP_ON_ML_BIT = 0; 
  __asm__ __volatile__ (
  "mv t0, %[mtvec]\n"
  "csrw mtvec, t0\n"
  "csrwi mie, 8\n"
  "csrsi mstatus, 8\n"
  "li t0, %[ones]\n"
  "xori t0, t0, %[bit]\n"
  "li t1, %[addr]\n"
  "ld t2, 0(t1)\n"
  "and t2, t2, t0\n" 
  "sd t2, 0(t1)\n"
  "fence\n"
  :
  : [addr] "i" (MINION_FEATURE_ADDR),
    [bit] "i"(TRAP_ON_ML_BIT),
    [ones] "i"(ALL_ONES),
    [mtvec] "r" (mtvec_ptr)
  : "t0", "t1", "t2");

  uint64_t regval;

  et_tensor_load_l2scp_conf_t l2scp_conf;
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

  // Enable SCP
  __asm__ __volatile__ ("csrsi mcache_control, 0x1\n");
  __asm__ __volatile__ ("fence\n");                     // VERIF-3295 (PRM-8)
  __asm__ __volatile__ ("csrsi mcache_control, 0x2\n");


  // (after setting TRAP_ON_ML_BIT=0 is completed)
  // increment FCC to H2, to notify that H2 can start doing Tensor Ops;
  //CRED_INC_T2_C0;   // change to code below:
  NEW_CRED_INC_LOCAL((min_id+1));     // give FCC credit to Partner #(min_id+1) (Thread 0); 

  for (int i=0; i<ITERATIONS; i++){
    switch(i%16)
      {
      case 0:
        //CRED_INC_T2_C0;  // order sync between TensorReduce Sender & Receiver is unnecessary (could be removed)

        //                start_reg num_reg partnerID                          
        //                --------- ------- ---------
        //tensor_reduce_send(   0,       1,      1     );
        tensor_reduce_send(   0,       1,   min_id+1 );
        break;
      case 1:
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

        // Load matrices A and B
        et_tensor_load (&load_conf_matrixA);
        et_tensor_load (&load_conf_matrixB);
        WAIT_TENSOR_LOAD_0;
        WAIT_TENSOR_LOAD_1;
        et_tensor_fma(&fma_conf);         
        break;
      case 2:
        //               srow nrow scol ncol                           
        //               ---- ---- ---- ----
        convolution_size( 1,   16,  1,   16);
        break;
      case 3:
        //               row_start col_start                           
        //               --------- ---------
        convolution_ctrl(    0,       0     );
        break;
      case 4:
        //           neigh_mask minion_mask coop_id                           
        //           ---------- ----------- -------
        tensor_coop(    0,           0,       5    );
        break;
      case 5:
        // Read tensor mask
        __asm__ __volatile__ ("csrr %[regval], tensor_mask\n" : [regval] "=r" (regval));
        break;
      case 6:
        //           start_reg col row scp_loc transf9 transf8 transf7 transf6 transf5 transf4 transf3 transf2 transf1 transf0                      
        //           --------- --- --- ------  ------- ------- ------- ------- ------- ------- ------- ------- ------- -------
        tensor_quant(    0,     0,  0,   0,       0,      0,      0,      0,      0,      0,      0,      0,       0,     0   );
        break;
      case 7:
        // Read tensor error
        __asm__ __volatile__ ("csrr %[regval], tensor_error\n" : [regval] "=r" (regval));
        break;
      case 8:
        // Write to flb
        __asm__ __volatile__ ("csrw 0x820, x0\n");
        break;
      case 9:
        // Give credit, then block on fcc
        //CRED_INC_T0_C0;   // change to code below: 
        NEW_CRED_INC_LOCAL(min_id);     // give FCC credit to this Minion itself (Thread 0); 
        __asm__ __volatile__ ("csrw fcc, x0\n");
        break;
      case 10:
        //CRED_INC_T2_C0;   // change to code below:
        NEW_CRED_INC_LOCAL((min_id+1));     // give FCC credit to Partner #(min_id+1) (Thread 0); 
        // Stall
        __asm__ __volatile__ ("csrw stall, x0\n");
        break;
      case 11:
        // Tensor wait
        __asm__ __volatile__ ("csrwi tensor_wait, 0x6\n");
        break;
      case 12:
        // Tensor Load L2scp
        l2scp_conf.use_tmask = 0;                               // xs[   63]: Not used currently
        l2scp_conf.num_lines = 0;                               // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)
        l2scp_conf.stride    = 64;                              // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
        l2scp_conf.id        = 0;                               // x31[0]: Tensor Operation ID
        l2scp_conf.addr      = (uint64_t) ptr;                  // xs[47: 6]: FROM: Full Virtual Address
        l2scp_conf.dst_start = 0;                               // xs[62:48] & xs[5:4]: TO: L2 SCP Cache line (0.(2^17)-1)

        #ifdef POSTSI_UST
          // (CYE) need to initialize the cachline address (%[ptr]) in L3$ (by flush from L1->L3):
          // flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl) 
          flush_va(0, 2, (uint64_t)ptr, 0, 0, 0, 0);
          WAIT_CACHEOPS; 
        #endif

        et_tensor_load_l2scp (&l2scp_conf);            
        break;
      case 13:
        // Call fccnb
        asm volatile ("csrr %[regval], fccnb\n" : [regval] "=r" (regval));
        break;
      case 14: 
        et_tensor_load (&load_conf_matrixA);  
        break;
      case 15:
        //           reg_stride start_reg cols Arows              addr                coop_store stride                       
        //           ---------- --------- ---- ----- -------------------------------- ---------- ------ 
        tensor_store(    0,        0,      0,   15,  (uint64_t)(void*)(ptr+(8*(i-1))),    0,      64   );            
        break;
      default:
        break;
    }
  }

  if (exp_cnt>0){
    err=2;
    C_TEST_FAIL;
  } 
 
  c_test_pass();
}

// for Hart 2, 6, 10, etc.
void test_t2(unsigned int hart_id) {
  unsigned int min_id = hart_id / 2;

  // H2 must wait for FCC (from H0 after setting TRAP_ON_ML_BIT=0 completed), before starting Tensor Ops:
  __asm__ __volatile__ ("csrw fcc, x0\n");

  for (int i=0; i<ITERATIONS; i++){
    switch(i%16)
      {
      case 0:
        //__asm__ __volatile__ ("csrw fcc, x0\n");   // order sync between TensorReduce Sender & Receiver is unnecessary (could be removed)

        //                 start_reg operation num_reg partnerID                          
        //                 --------- --------- ------- ---------
        //tensor_reduce_recv(   0,         0,       1,      0     );
        tensor_reduce_recv(   0,         0,       1,   min_id-1 );
        break;
      case 10:
        __asm__ __volatile__ ("csrw fcc, x0\n");
        // delay
        for (int j=0; j<100; j++){
          __asm__ __volatile__ ("nop\n");
        }
        // IPI to Hart 0:
        //TRIGGER_MSIP;    // send IPI to Hart 0;
        NEW_TRIGGER_MSIP((hart_id-2));    // send IPI to the other Hart 
        break;
      default:
        break;
    }
  }

  c_test_pass();
}


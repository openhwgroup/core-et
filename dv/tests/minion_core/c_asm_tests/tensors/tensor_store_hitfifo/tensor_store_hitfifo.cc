/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "et_test_common.h"
#include "minion.h"
#include "common.h"
#include "tensor_filter_data.h"
#include "tensor_act_data.h"

#ifndef NUM_MINIONS
#define NUM_MINIONS 8
#endif
#include "ts_selfcheck.h"

// Preload random data (~1M)
extern cl_data_t _binary_blob_bin_start;

__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
   );



// Initializes the L2 scp
static inline void init_l2_scp(uint32_t shire_id, uint32_t minion_id)
{
    // Each minion writes 1024 CL, to write a total of 2MBytes of L2Scp
    uint64_t l2_scp_addr = 0x0080000000UL + (shire_id << 23) + (minion_id * 1024 * 64);
    uint64_t offset = (minion_id & 0x3) * 0x100; //256

    __asm__ __volatile__ (
        // Init FP registers
        // Each minion stores 1024 cachelines. First 8 write activation type of data, other filter
        "li       x31, 8\n"
        "blt      %[minion_id], x31, load_act\n"
        "addi     x31, %[filter_data], 0\n"
        "j        do_loads\n"
        "load_act:\n"
        "addi     x31, %[act_data], 0\n"
        "do_loads:\n"
        "addi     t0, x31, 0x400\n"  // t0 is limit
        "add      x31, x31, %[offset]\n"
        "flw.ps   f0,  0x0 (x31)\n"
        "flw.ps   f1,  0x20(x31)\n"
        "flw.ps   f2,  0x40(x31)\n"
        "flw.ps   f3,  0x60(x31)\n"
        "flw.ps   f4,  0x80(x31)\n"
        "flw.ps   f5,  0xA0(x31)\n"
        "flw.ps   f6,  0xC0(x31)\n"
        "flw.ps   f7,  0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset0\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset0:\n"
        "flw.ps   f8,  0x0 (x31)\n"
        "flw.ps   f9,  0x20(x31)\n"
        "flw.ps   f10, 0x40(x31)\n"
        "flw.ps   f11, 0x60(x31)\n"
        "flw.ps   f12, 0x80(x31)\n"
        "flw.ps   f13, 0xA0(x31)\n"
        "flw.ps   f14, 0xC0(x31)\n"
        "flw.ps   f15, 0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset1\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset1:\n"
        "flw.ps   f16, 0x0 (x31)\n"
        "flw.ps   f17, 0x20(x31)\n"
        "flw.ps   f18, 0x40(x31)\n"
        "flw.ps   f19, 0x60(x31)\n"
        "flw.ps   f20, 0x80(x31)\n"
        "flw.ps   f21, 0xA0(x31)\n"
        "flw.ps   f22, 0xC0(x31)\n"
        "flw.ps   f23, 0xE0(x31)\n"
        "addi     x31, x31, 0x100\n"
        "bne      x31, t0, skip_offset2\n"
        "li       t1, -0x400\n"
        "add      x31, x31, t1\n"
        "skip_offset2:\n"
        "flw.ps   f24, 0x0 (x31)\n"
        "flw.ps   f25, 0x20(x31)\n"
        "flw.ps   f26, 0x40(x31)\n"
        "flw.ps   f27, 0x60(x31)\n"
        "flw.ps   f28, 0x80(x31)\n"
        "flw.ps   f29, 0xA0(x31)\n"
        "flw.ps   f30, 0xC0(x31)\n"
        "flw.ps   f31, 0xE0(x31)\n"

        // do 64 tstores of 16 cachelines
        "addi     t0, zero, 64\n"
        "li       t1, (3 << 55) + (15 << 51)\n"
        "add      t1, t1, %[l2_scp_addr] \n"
        "li       x31, 0x40\n"
        "loop:\n"
        "csrw     0x87F, t1\n"
        "addi     t1, t1, 64 * 16\n" // Advance 1024 bytes
        "addi     t0, t0, -1\n"
        "bne      t0, zero, loop\n"
        // Twait for tstores at the end
        "csrwi    0x830, 8\n"
      :
      :
        [l2_scp_addr] "r" (l2_scp_addr),
        [filter_data] "r" (filter_data),
        [act_data] "r" (act_data),
        [minion_id] "r" (minion_id),
        [offset] "r" (offset)
      : "t0", "t1", "t2", "x31"
    );

}

int main() { //try to fill tensor store fifo with coop of 1

  //Init marker
  C_TEST_START;

  // Get minion ID and thread id
  uint64_t tid = get_thread_id();
  uint64_t mid = get_minion_id();
  uint64_t sid = get_shire_id();
  volatile cl_data_t *buffer = &_binary_blob_bin_start;
  uint64_t addr     = (uint64_t)(void*)buffer[0].data[0];
  uint64_t addr2    = (uint64_t)(void*)buffer + (sid << 23) + (mid * 1024 * 64);
  uint64_t aux_addr = (uint64_t)(void*)buffer[0].data[0];
  uint64_t row_size = 1;
   
  // Setup machine trap vector
  // Normally we do not expect any interrut
  setup_mtrap_vector();

  
  if (tid==0 && sid ==0)
  {
    enable_shire_coop(sid);
    setM0MaskFF();
    initFRegs();
    //All minion but one is enough. I do in this way to do the tensor_stroes at the same time .
    setup_cache_scp();
    //init_l2_scp(sid, mid);


    addr += (mid * (0x10 * (row_size + 1)));
    /*reg_stride, start_reg, row_size, nrows, addr, ncoop_minions, stride*/
    tensor_store(0, 0, 1, 15, addr, 1, 256);
    WAIT_TENSOR_STORE;
    
    /* bool use_tmask, bool use_coop, uint64_t dst_start, uint64_t transformation, */
    /* uint64_t use_tenb, uint64_t addr, uint64_t offset,uint64_t num_lines, uint64_t stride, uint64_t id*/
    tensor_load(0, 0, 6, 0, 0, addr2, 0, 2, 0, 0);
    WAIT_TENSOR_LOAD_0;

    addr = aux_addr;
    addr += (mid * (0x10 * (row_size + 1)));
    /*reg_stride, start_reg, row_size, nrows, addr, ncoop_minions, stride*/
    tensor_store(0, 0, 1, 15, addr, 1, 256);
    WAIT_TENSOR_STORE;
    
    /* bool use_tmask, bool use_coop, uint64_t dst_start, uint64_t transformation, */
    /* uint64_t use_tenb, uint64_t addr, uint64_t offset,uint64_t num_lines, uint64_t stride, uint64_t id*/
    tensor_load(0, 0, 6, 0, 0, addr2+64, 0, 2, 0, 0);
    WAIT_TENSOR_LOAD_0;

    addr = aux_addr;
    addr += (mid * (0x10 * (row_size + 1)));
    /*reg_stride, start_reg, row_size, nrows, addr, ncoop_minions, stride*/
    tensor_store(0, 0, 1, 15, addr, 1, 256);
    WAIT_TENSOR_STORE;
    
    /* bool use_tmask, bool use_coop, uint64_t dst_start, uint64_t transformation, */
    /* uint64_t use_tenb, uint64_t addr, uint64_t offset,uint64_t num_lines, uint64_t stride, uint64_t id*/
    tensor_load(0, 0, 6, 0, 0, addr2+128, 0, 2, 0, 0);
    WAIT_TENSOR_LOAD_0;

    addr = aux_addr;    
    addr += (mid * (0x10 * (row_size + 1)));
    /*reg_stride, start_reg, row_size, nrows, addr, ncoop_minions, stride*/
    tensor_store(0, 0, 1, 15, addr, 1, 256);

    WAIT_TENSOR_STORE;
    /* bool use_tmask, bool use_coop, uint64_t dst_start, uint64_t transformation, */
    /* uint64_t use_tenb, uint64_t addr, uint64_t offset,uint64_t num_lines, uint64_t stride, uint64_t id*/
    tensor_load(0, 0, 6, 0, 0, addr2+192, 0, 2, 0, 0);
    WAIT_TENSOR_LOAD_0;
    

    // Check if there was any error in the Tensor Operations
    tensor_error_check();

    #ifdef SELF_CHECK
       selfcheck_ts(0, 0, 1, 15, addr, 256);
    #endif
  }
     
  C_TEST_PASS;
  
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"
//#include "perf_event_monitor.h"

#ifdef __cplusplus
extern "C"
#endif

#define TENSOR_LOAD_WAIT_0 0
#define TENSOR_FMA_WAIT 7

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
           
// Tensor_FMA16A32_Latency test. Makes latency sweeps for typical FP16 FMA uses
// Author: emizan
// Cases tested
// 1. Tensor A Row Sweep (1..16), set A columns to 16, B columns to 16
// 2. Tensor A Column Sweep (1..16), set A rows to 16, B columns to 16.
// 3. Tensor B Column Sweep (0,4,8,12), set A rows and Columns to 16
// 4. Tensor Mask Bit vector sweep, just shift in 1's consecutively with tensor_mask instruction
// 5. Same as 4, use convolution_ctrl and convolutions_size instructions to set mask bit vector.

int main()
{
    uint64_t hart_id = getHartId();
    volatile float data[1024];

//    Event checking (currently not compiling)
//    Perf *perf;
//    perf->event_check_count_precise(0, 0, 40);

    if ( hart_id == 0) {
	setM0MaskFF();
	initFRegs();
    
        // Enable SCP
	mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);
		
	for (uint16_t i=0; i < 1024; i++) {
	    data[i] = (float) i;
	}
        
	// Use different variables for tensor_load and tensor_fma
	// Each loop iteration has a tensor load and a tensor FMA
	// We have added waits between them since we always write and read from the same SCP line.
	// Both Tensors of Tensor FMA access the same SCP line.
	bool use_tmask = true;
	bool use_coop = false;
	bool load_trans = 0;
	bool scp_start = 0;
	bool l2_scp = 0;
        uint64_t addr = (uint64_t) data;
	uint64_t offset = 0;
        uint64_t num_lines = 15;
	uint64_t stride = 0x40;
	uint64_t id = 0;

	bool fma_use_tmask = false;
        uint64_t b_num_cols = 3;
	uint64_t a_num_rows = 15;
	uint64_t a_num_cols = 15;
	uint64_t fma_offset = 0;
	bool tenc_loc = 0;
	bool tenb_unsigned = 0;
	bool tena_unsigned = 0;
        bool tenb_loc = 0;
	uint64_t scp_loc_a = 0;
	uint64_t scp_loc_b = 0;
	uint64_t opcode = 1;
	bool first_pass = 1;
		    
	
	// Tensor A Row sweep
	use_tmask = false;
	for (uint64_t a_row=0; a_row <= a_num_rows; a_row++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_row, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor A Column sweep
	for (uint64_t a_col=0; a_col <= a_num_cols; a_col++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_col, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor B Column sweep
	for (uint64_t b_col=0; b_col <= b_num_cols; b_col++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_col, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor mask bit vector sweep with tensor_mask instruction
	fma_use_tmask = true;
	use_tmask = true;
	uint64_t mask = 0;
	for (uint64_t m=0; m <= 16; m++) {
	    tensor_mask(0, mask);
	    mask = (mask << 1) | 1;
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor mask bit vector sweep with convolution_ctrl and convolution_size instructions
	uint64_t conv_row_start = 0;
	uint64_t conv_col_start = 0;
	uint64_t conv_row_step = 1;
	uint64_t conv_col_step = 1;
	convolution_ctrl(conv_row_start, conv_col_start);
	for (uint64_t rc_val=0; rc_val <=16; rc_val++) {
	    convolution_size(conv_row_step, rc_val, conv_col_step, rc_val);
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}
   }

   C_TEST_PASS;
   // End marker
   return 0;
}

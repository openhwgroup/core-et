/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10

// Tensor_IMA8A32_Latency_TenB test. Makes latency sweeps for typical INT8 FMA uses
// Tensor A is in the SCP, Tensor B is in TenB. Both early and late TenB load cases are tested
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
    volatile uint32_t data[1024];

    if ( hart_id == 0) {
	setM0MaskFF();
	initFRegs();
    
        // Enable SCP
	mcache_control(1,0,0);
    fence();              // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);
	
	// Test 1: Just one transformation
        // Do it with and witout tensor wait
	
	for (uint16_t i=0; i < 1024; i++) {
	    data[i] = i;
	}

	// Use different variables for tensor_load and tensor_fma
        // Each loop iteration has a tensor load and a tensor FMA
        // We have added waits between them since we always write and read from the same SCP line.
        // Both Tensors of Tensor FMA access the same SCP line.
	bool use_tmask = false;
	bool use_coop = false;
	bool load_trans = 0;
	bool scp_start = 0;
	bool use_tenb_0 = 0;
        uint64_t addr = (uint64_t) data;
	uint64_t offset = 0;
        uint64_t num_lines = 15;
	uint64_t stride = 0x40;
	uint64_t id = 0;
	bool use_tenb_1 = 1;

	bool fma_use_tmask = false;
        uint64_t b_num_cols = 3;
	uint64_t a_num_rows = 15;
	uint64_t a_num_cols = 15;
	uint64_t fma_offset = 0;
	bool tenc_loc = 0;
	bool tenb_unsigned = 0;
	bool tena_unsigned = 0;
        bool tenb_loc = 1;
	uint64_t scp_loc_a = 0;
	uint64_t scp_loc_b = 0;
	uint64_t opcode = 3;
	bool first_pass = 1;
     
	// Tensor A Row sweep late TL1
	for (uint64_t a_row=0; a_row <= a_num_rows; a_row++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_fma(fma_use_tmask, b_num_cols, a_row, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor A Row sweep early TL1
	for (uint64_t a_row=0; a_row <= a_num_rows; a_row++) {
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_row, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor A column sweep late TL1
	for (uint64_t a_col=0; a_col <= a_num_cols; a_col++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, a_col, stride, id);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_col, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor A Column sweep early TL1
	for (uint64_t a_col=0; a_col <= a_num_cols; a_col++) {
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, a_col, stride, id);
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_col, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor B Column sweep late TL1
	for (uint64_t b_col=0; b_col <= b_num_cols; b_col++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_fma(fma_use_tmask, b_col, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Tensor B Column sweep early TL1
	for (uint64_t b_col=0; b_col <= b_num_cols; b_col++) {
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
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
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
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
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset,  num_lines, stride, id);
            tensor_load(use_tmask, use_coop, scp_start, load_trans, use_tenb_0, addr, offset, num_lines, stride, id);
            tensorWait(TENSOR_LOAD_WAIT_0);
            tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
            tensorWait(TENSOR_FMA_WAIT);
        }

	unsigned long functional_error = get_tensor_error();

	if (functional_error != 0) {
            C_TEST_FAIL;
        }	
   }

   C_TEST_PASS;
   // End marker
   return 0;
}

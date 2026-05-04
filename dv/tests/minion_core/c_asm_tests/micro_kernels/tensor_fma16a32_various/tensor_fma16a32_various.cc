/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define TENSOR_LOAD_WAIT_0 0
#define TENSOR_FMA_WAIT 7

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
          

int main()
{  
    uint64_t hart_id = getHartId();
    
    
    if ( hart_id == 0) {
	setM0MaskFF();
	initFRegs();
    
        // Enable SCP
	mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);
	// Try some different A row / A columns / B columns combinations

	volatile float __attribute__((aligned(4096))) data[1024];
	for (uint16_t i=0; i < 1024; i++) {
	    data[i] = (float) i;
	}

	bool use_tmask = false;
	bool use_coop = false;
	bool load_trans = 0;
	uint64_t scp_start = 0;
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
       
	// Try many different row / col combinations
	volatile uint16_t a_rows[15] = {1,2,4,6,8,14,13,14,15,5,9,3,15,14,13};
	volatile uint16_t a_cols[15] = {1,2,4,8,7,14,11,3,14,9,10,8,15,14,13};
        volatile uint16_t b_cols[15] = {0,0,1,2,2,3,3,1,0,2,1,3,3,3,3};
		
	for (uint64_t i=0; i < 15; i++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);	
	    tensor_fma(fma_use_tmask, b_cols[i], a_rows[i], a_cols[i], fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}
	
	// // Start from a different SCP line -- use waits after the FMA because loads keep on bringing new values	
	for (uint64_t block=0; block < 128; block = block + 16) {	    
	    scp_loc_a = block % 16;
	    scp_loc_b = block % 16 + 2;
	    tensor_load(use_tmask, use_coop, scp_loc_a, load_trans, l2_scp, addr + block, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_load(use_tmask, use_coop, scp_loc_b, load_trans, l2_scp, addr + block, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);	    
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Start from a different SCP line -- do not use waits after the FMA because load A keep on bringing the same values
        // and load B always loads to a different area.
	for (uint64_t block=0; block < 128; block = block+16) {	    
	    scp_loc_a = block % 16;
	    scp_loc_b = (block + 16) % 32;
	    tensor_load(use_tmask, use_coop, scp_loc_a, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_load(use_tmask, use_coop, scp_loc_b + 16, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b + 16, scp_loc_a, opcode, first_pass);
	}
	tensorWait(TENSOR_FMA_WAIT);
		
	//Clear FP registers
	first_pass = 0;
	scp_start = 0;
	scp_loc_a = 0;
	scp_loc_b = 0;
	for (uint64_t i=0; i < 15; i++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);	
	    tensor_fma(fma_use_tmask, b_cols[i], a_rows[i], a_cols[i], fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Run similar tests when TensorB is in TenB
	tenb_loc = 1;
	bool use_tenb_1 = 1;
	first_pass = 1;
	scp_start = 0;
	scp_loc_a = 0;
	scp_loc_b = 0;

	for (uint64_t i=0; i < 15; i++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, a_cols[i], stride, id);  
	    tensorWait(TENSOR_LOAD_WAIT_0);	
	    tensor_fma(fma_use_tmask, b_cols[i], a_rows[i], a_cols[i], fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Start from a different SCP line -- use waits after the FMA because loads keep on bringing values to the same SCP area
	for (uint64_t block=0; block < 128; block = block + 16) {	    
	    scp_loc_a = block % 16 + 6;	    
	    tensor_load(use_tmask, use_coop, scp_loc_b, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_load(use_tmask, use_coop, scp_loc_a, load_trans, l2_scp, addr + block, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);	    
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}

	// Start from a different SCP line -- do not use waits because load brings values to a different SCP area on every iteration.
	scp_loc_a = 0;
	for (uint64_t block=0; block < 128; block = block + 16) {	    
	    scp_loc_a = (block + 16) % 32;
	    tensor_load(use_tmask, use_coop, scp_loc_b, load_trans, use_tenb_1, addr, offset, num_lines, stride, id);
	    tensor_load(use_tmask, use_coop, scp_loc_a + 16, load_trans, l2_scp, addr + block, offset, num_lines, stride, id);
	    tensorWait(TENSOR_LOAD_WAIT_0);	    
	    tensor_fma(fma_use_tmask, b_num_cols, a_num_rows, a_num_cols, fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a + 16, opcode, first_pass);	  
	}

	// Clear FP registers
	first_pass = 0;
	scp_start = 0;
	scp_loc_a = 0;
	scp_loc_b = 0;
	for (uint64_t i=0; i < 15; i++) {
	    tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
	    tensor_load(0, use_coop, 0, load_trans, use_tenb_1, addr, offset, a_cols[i], stride, id);  
	    tensorWait(TENSOR_LOAD_WAIT_0);	
	    tensor_fma(fma_use_tmask, b_cols[i], a_rows[i], a_cols[i], fma_offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass);
	    tensorWait(TENSOR_FMA_WAIT);
	}
	
   }

   C_TEST_PASS;
   // End marker
   return 0;
}

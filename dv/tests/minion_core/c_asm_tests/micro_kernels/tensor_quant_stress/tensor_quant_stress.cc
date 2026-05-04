/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10

// Tensor_Quant_Stress test
// Author: emizan
// It tests various less common setup of the tensor quant instruction. Used to check both latency and correctness
// Cases testsed
// 1. 6 rows / 3 cols w/o SCP access (6/3 are just arbitrary)
// 2. 6 rows / 3 cols w SCP access
// 3. Start from a non-0 register to make sure we wrap-around
// 4. Start from a non-0 SCP line
// 5. Combine 3 and 4.
// 6. A single transformation 10 times (RELU arbitrarily selected)
// 7. Have valid transformation after LAST_TRANS to make sure we correctly terminate the instruction

int main()
{
    uint64_t hart_id = getHartId();
    uint64_t start_reg;
    uint64_t rows;
    uint64_t cols;
    uint64_t scp_loc;
    volatile uint32_t data[1024];

    if ( hart_id == 0) {
	setM0MaskFF();
	initFRegs();
		
        // Enable SCP
        mcache_control(1,0,0);
        fence();               // VERIF-3295 (PRM-8)
        mcache_control(1,1,0);
        
        for (uint16_t i=0; i < 1024; i++) {
            data[i] = i;
        }
        
        bool use_tmask = false;
        bool use_coop = false;
        bool load_trans = 0;
        bool scp_start = 0;
        bool l2_scp = 0;
        uint64_t addr = (uint64_t) data;
        uint64_t offset = 0;
        uint64_t num_lines = 15;
        uint64_t stride = 0x40;
        uint64_t id = 0;
    
	// Load data to SCP
        tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
        tensorWait(TENSOR_LOAD_WAIT_0);

	// 6 rows, 3 cols, 4 scratchpad trans
	start_reg = 0;
	scp_loc = 0;
	rows = 5;
        cols = 2;
        tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_FP32_MUL_ROW, QUANT_FP32_MUL_COL, QUANT_INT32_ADD_COL, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);

	// 6 rows, 3 cols, 4 non-scratchpad trans
	rows = 5;
        cols = 2;
        tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_SATUINT8, QUANT_SATINT8, QUANT_PACK_128B, QUANT_RELU);
	tensorWait(TENSOR_QUANT_WAIT);

	// Start from reg 16
	start_reg = 16;
	rows = 15;
        cols = 3;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_INT32_ADD_COL, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);

	// Same as last one, change scratchpad line, start from reg 0
	start_reg = 0;
	scp_loc = 10;
        tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_SATUINT8, QUANT_SATINT8, QUANT_PACK_128B, QUANT_RELU);
	tensorWait(TENSOR_QUANT_WAIT);

        // Start not from 0 reg and not from 0 scp line
	start_reg = 4;
	scp_loc = 8;
        tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_SATUINT8, QUANT_SATINT8, QUANT_PACK_128B, QUANT_RELU);
	tensorWait(TENSOR_QUANT_WAIT);

	// Case with a  single transformation
	start_reg = 2;
	scp_loc = 1;
        tensor_quant(start_reg, cols, rows, scp_loc, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU, QUANT_RELU);
	tensorWait(TENSOR_QUANT_WAIT);

	// Make sure we do not go past first LAST_TRANS
	start_reg = 0;
	scp_loc = 0;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_INT32_ADD_ROW, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_INT32_ADD_ROW, QUANT_SATUINT8, QUANT_SATINT8, QUANT_PACK_128B, QUANT_LAST_TRANS);
	tensorWait(TENSOR_QUANT_WAIT);

	unsigned long functional_error = get_tensor_error();
	
	if (functional_error != 0) {
	    C_TEST_FAIL;
	}

   }

   C_TEST_PASS;

   return 0;
}

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
        
        // Test 1: Just one transformation
        // Do it with and witout tensor wait
        
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
    
	// Load something to the scratchpad
        tensor_load(use_tmask, use_coop, scp_start, load_trans, l2_scp, addr, offset, num_lines, stride, id);
        tensorWait(TENSOR_LOAD_WAIT_0);		

	start_reg = 0;
	rows = 15;
	cols = 3;
	scp_loc = 0;
	
	// Quant + tensor wait. Just one transformation, try all of them.
	for (uint64_t t=0; t <= 10; t++) {     	    
	    tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		         QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, t);		
	    tensorWait(TENSOR_QUANT_WAIT);
        }

	// Quant w/o tensor wait. Just one transformation, try all of them.
	for (uint64_t t=0; t <= 10; t++) {     	
	    tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		         QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, t);			    
        }
	        
	// Super minimal case, just one row and column
	rows = 0;
	cols = 0;       
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_INT32_TO_FP32);
		
	tensorWait(TENSOR_QUANT_WAIT);

	// Now run worst-case scenario of "ALL transformations	
	rows = 15;
	cols = 3;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_PACK_128B, QUANT_SATUINT8, QUANT_SATINT8, QUANT_RELU,
		     QUANT_INT32_ADD_COL, QUANT_INT32_ADD_ROW, QUANT_FP32_TO_INT32, QUANT_FP32_MUL_COL, QUANT_FP32_MUL_ROW, QUANT_INT32_TO_FP32);
	tensorWait(TENSOR_QUANT_WAIT);
       
	// Most typical scenario in resnet:
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_PACK_128B, QUANT_SATUINT8, QUANT_FP32_TO_INT32, QUANT_FP32_MUL_ROW, QUANT_INT32_TO_FP32, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);

        // Scale this down to fewer rows.
	rows = 7;
	cols = 3;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_PACK_128B, QUANT_SATUINT8, QUANT_FP32_TO_INT32, QUANT_FP32_MUL_ROW, QUANT_INT32_TO_FP32, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);

	// Scale this down to fewer columns
	rows = 15;
	cols = 1;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_PACK_128B, QUANT_SATUINT8, QUANT_FP32_TO_INT32, QUANT_FP32_MUL_ROW, QUANT_INT32_TO_FP32, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);

        // Scale this down to fewer columns
	rows = 15;
	cols = 0;
	tensor_quant(start_reg, cols, rows, scp_loc, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, QUANT_LAST_TRANS, 
		     QUANT_PACK_128B, QUANT_SATUINT8, QUANT_FP32_TO_INT32, QUANT_FP32_MUL_ROW, QUANT_INT32_TO_FP32, QUANT_INT32_ADD_ROW);
	tensorWait(TENSOR_QUANT_WAIT);	

	unsigned long functional_error = get_tensor_error();
	
	if (functional_error != 0) {
	    C_TEST_FAIL;
	    return -1;
	}
   }

   C_TEST_PASS;
   // End marker
   return 0;
}

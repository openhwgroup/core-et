/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define ITERATIONS 40
           
int main()
{
    //uint64_t operation = TENSOR_REDUCE_OP_FGET;
    //float data[1024];

    setM0MaskFF();
    initFRegs();
    uint64_t hart_id = getHartId();

    if ( hart_id < 2048)  {

	// Enable SCP
	mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);  
	
	uint64_t start_regs[ITERATIONS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,30,5,6,7,8,9,16,18,20,24,3,31,26,14,19,27,4,11};
	uint64_t operations[ITERATIONS] = {TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
					   TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
					   TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
					   TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
					   TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
					   TENSOR_REDUCE_OP_FMAX,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,
					   TENSOR_REDUCE_OP_FMIN,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IMAX,
					   TENSOR_REDUCE_OP_IMIN,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,TENSOR_REDUCE_OP_FMIN,
					   TENSOR_REDUCE_OP_FMIN,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_IMAX,
					   TENSOR_REDUCE_OP_FMIN,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IMAX,TENSOR_REDUCE_OP_IMIN};
	uint64_t num_regs[ITERATIONS] = {32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
					 10,15,10,10,16,8,1,5,3,26,8,2,3,4,20,21,7,11,2,28};
	uint64_t levels[ITERATIONS] = {0,1,2,3,4,5,6,7,8,9,9,8,7,6,5,4,3,2,1,0,
				       6,3,7,2,5,1,9,0,8,3,4,2,6,3,8,4,7,3,5,2};
	uint64_t actions[ITERATIONS] = {3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,
					3,2,3,3,2,2,2,2,3,3,3,2,3,3,2,2,3,2,3,3};
	
	for (uint64_t i=0; i < ITERATIONS; i++) {
	    uint64_t start_reg = start_regs[i];
	    uint64_t operation = operations[i];
	    uint64_t num_reg = num_regs[i];
	    uint64_t level = levels[i];
	    uint64_t action = actions[i];
	    uint64_t mask = (start_reg << 57) | (operation << 24) |
		(num_reg << 16) | (level << 3) | action;
	    _reduce(mask);
	}
	tensorWait(TENSOR_REDUCE_WAIT);
	
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

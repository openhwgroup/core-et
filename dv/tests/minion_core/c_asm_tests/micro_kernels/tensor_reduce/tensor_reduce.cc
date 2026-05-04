/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#define ITERATIONS 20
           
int main()
{
    //uint64_t operation = TENSOR_REDUCE_OP_FGET;
    //float data[1024];

    setM0MaskFF();
    initFRegs();
    
    // Enable SCP
    mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
    mcache_control(1,1,0);  
   
    uint64_t start_regs[ITERATIONS] = {0,0,0,0,0,0,0,0,0,0,1,2,30,5,6,7,8,9,16,18};
    uint64_t operations[ITERATIONS] = {TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
			       TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,
			       TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FGET,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,
			       TENSOR_REDUCE_OP_FMIN,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IADD,TENSOR_REDUCE_OP_IMAX,
			       TENSOR_REDUCE_OP_IMIN,TENSOR_REDUCE_OP_FADD,TENSOR_REDUCE_OP_FMAX,TENSOR_REDUCE_OP_FMIN};
    uint64_t num_regs[ITERATIONS] = {32,32,32,32,32,32,32,32,32,32,10,15,10,10,16,8,1,5,3,26};
    uint64_t levels[ITERATIONS] = {0,1,2,3,4,4,3,2,1,0,3,2,3,4,3,2,1,3,4,3};
    uint64_t actions[ITERATIONS] = {3,3,3,3,3,2,2,2,2,2,3,2,3,3,2,2,2,2,3,3};

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

    C_TEST_PASS;
    // End marker
    return 0;
}

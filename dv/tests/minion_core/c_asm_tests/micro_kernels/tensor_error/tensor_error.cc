/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define ITERATIONS 20
           
int main()
{
    uint64_t hart_id = getHartId();
    uint64_t minion_id = hart_id >> 1;
    uint64_t num_expected_errors = 3 + (hart_id == 0);
    uint64_t num_errors = 0;

    setM0MaskFF();
    initFRegs();
    
    // Enable SCP
    mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
    mcache_control(1,1,0);  

    // ERROR 1: Tensor reduce to yourself.
    uint64_t start_reg = 0;
    uint64_t num_reg = 1;
    tensor_reduce_send(start_reg, num_reg, minion_id);
   
    tensorWait(TENSOR_REDUCE_WAIT);

    unsigned long functional_error = get_tensor_error();
    if (functional_error == (1 << TENSOR_ERROR_REDUCE)) {
       num_errors++;
    }

    CLEAR_TENSOR_ERROR;

    // ERROR 2: Illegal Tensor Load Transformation
    volatile uint32_t data[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint64_t data_array = (uint64_t) data;
   
    uint64_t illegal_trans = 3;
    tensor_load(0, 0, 0, illegal_trans, 0, data_array, 0, 0, 0, 0);
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    functional_error = get_tensor_error();
    if (functional_error == (1 << TENSOR_ERROR_LOAD_TRANSFORM)) {
       num_errors++;
    }

    CLEAR_TENSOR_ERROR;

    // ERROR 3: Memory fault protection
    data_array = 0;
    tensor_load(0, 0, 0, 0, 0, data_array, 0, 0, 0, 0);
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    functional_error = get_tensor_error();
    if (functional_error == (1 << TENSOR_ERROR_MEM_FAULT)) {
       num_errors++;
    }

    CLEAR_TENSOR_ERROR;

    // ERROR 4 TBD : SCP DISABLED
    // ERROR 5 TBD : Illegal TL1 / FMA pairing
    // ERROR 6 TBD : LockSW failed
    // ERROR 7 TBD : Invalid Tensor Store Coop/Size
  
    // ERROR 8: FCC overflow
    // Hart 0 overflows only here.

    uint64_t addr = 0x80340000UL;
    uint64_t shire_id = 0UL;
    uint64_t fcc_counter0 = 0x0C0UL;
    addr |= (shire_id << 22);
    addr |= fcc_counter0;
    
    // For FC divide 2120 by 32.  
    if (hart_id != 0) {
        uint64_t thr = 1;
        for (uint64_t i=0; i <= 2120; i++) {
     	    store(addr,thr);
        } 
    }

    else {
     	while (get_tensor_error() == 0);
    }
    
    functional_error = get_tensor_error();
    if (functional_error == (1 << TENSOR_ERROR_FCC_OVERFLOW)) {
        num_errors++;
    }

    CLEAR_TENSOR_ERROR;
    
    if (num_errors != num_expected_errors) {
	C_TEST_FAIL;
	return -1;
    }

    C_TEST_PASS;
    // End marker
    return 0;
}

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
    volatile float data[1024];

    // For the time being keep it to 1 shire
    // Then add more traffic to see how it scales.
    if ((hart_id & 0x1F) == 0 && hart_id < 2048) {
	setM0MaskFF();
	initFRegs();
    
        // Enable SCP
	mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);
		
	for (uint16_t i=0; i < 1024; i++) {
	    data[i] = (float) i;
	}

	uint64_t reg_stride = 0;
	uint64_t start_reg = 0;
	uint64_t row_size = 3;
	uint64_t num_rows = 15;
	uint64_t coop_minions = 0;
	uint64_t addr = (uint64_t) data;	
	uint64_t stride = 0x40;
	
	// Basic test
	for (uint16_t i=0; i < 20; i++) {
	   tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);
        }

        tensorWait(TENSOR_STORE_WAIT);

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

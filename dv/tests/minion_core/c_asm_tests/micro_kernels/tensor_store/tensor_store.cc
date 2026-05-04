/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "et_test_common.h"

#ifdef __cplusplus
extern "C"
#endif

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
           
// Preload random data (~1M)
cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      12            \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );


int main()
{
    uint64_t hart_id = getHartId();
    volatile float data[1024];

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

	uint64_t reg_stride = 0;
	uint64_t start_reg = 0;
	uint64_t row_size = 3;
	uint64_t num_rows = 15;
	uint64_t coop_minions = 0;
	uint64_t addr = (uint64_t) data;	
	uint64_t stride = 0x40;
	
	// Basic test
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);

	// Smaller row sizes
	row_size = 0;
	__asm__ __volatile__("mov.m.x m0, zero, 0x01\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,1\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x02\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,2\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x04\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,3\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x08\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,4\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x10\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,5\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x20\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,6\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x40\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,7\n");
	__asm__ __volatile__("mov.m.x m0, zero, 0x80\n":: : "memory");
        __asm__ __volatile__("fbci.ps f0,8\n");
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);
	
	row_size = 1;
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);

	row_size = 3;
	// Row sweep
	for (uint64_t i=0; i < num_rows; i++) {
	    tensor_store(reg_stride, start_reg, row_size, i, addr, coop_minions, stride);
	}
	
	// Change start reg	
	for (uint64_t reg=0; reg < 32; reg++) {
	    tensor_store(reg_stride, reg, row_size, num_rows, addr, coop_minions, stride);	    
	}

	// Change reg stride	
	for (reg_stride=0; reg_stride < 4; reg_stride++) {
	    tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);
	}
	
	// 0 stride to test worst latency. All the rows go to the same location so they need to serialize
	stride = 0;
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);

	// What happens for smaller stride ? 
	stride = 0x20;
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);

        tensorWait(TENSOR_STORE_WAIT);

	volatile cl_data_t *ts_data = &_binary_blob_bin_start;

	bool use_tmask = false;
        bool use_coop = false;
        bool load_trans = 0;
        bool scp_start = 0;
	bool tenb = 0;
	uint64_t offset = 0;
	uint64_t num_lines = 15;
	stride = 0x40;
	uint64_t id = 0;

	addr = (uint64_t) ts_data;
	tensor_load(use_tmask, use_coop, scp_start, load_trans, tenb, addr, offset, num_lines, stride, id);
	tensor_load(use_tmask, use_coop, scp_start+16, load_trans, tenb, addr + 0x400, offset, num_lines, stride, id);
	tensor_load(use_tmask, use_coop, scp_start+32, load_trans, tenb, addr + 0x800, offset, num_lines, stride, id);
	tensorWait(TENSOR_LOAD_WAIT_0);

	// Tensor Store from SCP
	uint64_t scp_stride = 3;
	uint64_t start_line = 22;
	num_rows = 8;
	stride = 0x40;
	tensor_store_scp(scp_stride, start_line, num_rows, addr, stride);	

	// Go through num rows
	scp_stride = 0;
	start_line = 0;
        num_rows = 16;        
	for (uint64_t i=0; i < num_rows; i++) {
	    tensor_store_scp(scp_stride, start_line, i, addr, stride);
	}


	// Go through start line
	for (start_line=0; start_line < 64; start_line++) {
	    tensor_store_scp(scp_stride, start_line, num_rows, addr, stride);
	}       

	// Change scp stride     
	for (scp_stride=0; scp_stride < 4; scp_stride++) {
	    tensor_store_scp(scp_stride, start_line, num_rows, addr, stride);
	}

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

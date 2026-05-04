/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <inttypes.h>
#include "common.h"
#include "flb.h"
#include "tensors.h"
#include "et_test_common.h"
#include "cacheops.h"

#ifdef __cplusplus
extern "C"
#endif

// Tensor Load NOC / L3 / DDDR stress test
// Random tensor load 0 (SCP) and  and 1 (TenB) followed by FMA
// We swizzle the addresses so we access the DDR or the L3 in every iteration
// That way we get as many cache line requests as possible
// We use only 4 shires and 4 iterations for the VCS test.
// For Zebu we run on all shires, and run many iterations

// Preload random data (~3M)
cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      25            \n\t"
        "       .incbin     \"in.raw\"    \n\t"
        "       .align      25            \n\t"
        "       .incbin     \"out.raw\"   \n\t"
    );

#define NUM_ITER 4
#define TOTAL_MINIONS 128

// Swizzle the address offset (16-bit field in a 64-bit value)
// Do not use a loop as you just do it 4 times
static uint64_t swizzle(uint64_t val)
{
    uint16_t val_16  = (uint16_t) (val >> 6);
    uint16_t bit = (val_16 >> 0) ^ (val_16 >> 2) ^ (val_16 >> 3) ^ (val_16 >> 5);
    val_16 = (val_16 >> 1) | (uint16_t) (bit << 15);
    val = (val_16 << 6) & 0x1FFFC0ULL;
    return val;
}


int main()
{
    uint64_t hart_id = getHartId();
    uint64_t minion_id = hart_id >> 1;
    uint64_t shire_id = (hart_id >> 6) & 0x3f;
    
    if (hart_id >= TOTAL_MINIONS * 2) {
        C_TEST_PASS;
        return 0;
    }
  
    if (hart_id & 1) {
        C_TEST_PASS; 
	return 0;
    }
    setM0MaskFF();
    initFRegs();
    
    setup_cache_scp();
    enable_shire_coop(shire_id);

    // Variables for included auto-generated files
    uint64_t TL0_COOP_CSR, TL0_IS_COOP, TL0_TMASK, TL0_CODE, TL0_SCP_START_LINE;
    uint64_t TL0_TENB, TL0_ADDR, TL0_OFFSET, TL0_NUM_LINES, TL0_STRIDE;

    uint64_t TL1_COOP_CSR, TL1_IS_COOP, TL1_TMASK, TL1_CODE, TL1_SCP_START_LINE;
    uint64_t TL1_TENB, TL1_ADDR, TL1_OFFSET, TL1_NUM_LINES, TL1_STRIDE;
    
    uint64_t TFMA_TMASK, TFMA_BCOLS, TFMA_AROWS, TFMA_ACOLS, TFMA_ASTART_COL, TFMA_TENB;
    uint64_t TFMA_SCP_START_LINEA, TFMA_SCP_START_LINEB, TFMA_CLEAR_RF, TFMA_TYPE;
    uint64_t TFMA_USE_TENC, TFMA_UNSIGNEDA, TFMA_UNSIGNEDB;
    
    #include "tl0_configs.h"
    #include "tl1_configs.h"
    #include "tfma_configs.h"

    volatile cl_data_t *in_data = &_binary_blob_bin_start;

    // 0x2000000 = 2 ^ 25 (see blob inline above)
    volatile uint64_t base_src_addr = (uint64_t) in_data;
    volatile uint64_t base_dst_addr = (uint64_t) in_data + 0x2000000;
   
    // Fill up the SCP to avoid X's (SCP lines are random)
    tensor_load(0, 0, 0 /*start line*/, 0, 0, // regular tl0 on scp
	base_src_addr, 0, 15 /*num lines*/, 0x40, 0);
    
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    tensor_load(0, 0, 16 /*start line*/, 0, 0, // regular tl0 on scp
	base_src_addr, 0, 15 /*num lines*/, 0x40, 0);
	
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    tensor_load(0, 0, 32 /*start line*/, 0, 0, // regular tl0 on scp
	base_src_addr, 0, 15 /*num lines*/, 0x40, 0);
	
    // Do not use masks (to maximize number of cache line accesses
    // Do not use TenC
    tensorWait(TENSOR_LOAD_WAIT_0);
    TL0_TMASK = 0;
    TL1_TMASK = 0;
    TFMA_TMASK = 0;
    TFMA_CLEAR_RF = 1;
    TFMA_USE_TENC = 1;
    for (uint64_t iter=0; iter < NUM_ITER; iter++) {		

	//=== Actual kernel body:

	// Tensor Load 0
	tensor_coop(TL0_COOP_CSR);		
	tensor_load(TL0_TMASK, TL0_IS_COOP, TL0_SCP_START_LINE, TL0_CODE, TL0_TENB,
	            base_src_addr + TL0_ADDR, TL0_OFFSET, TL0_NUM_LINES, TL0_STRIDE, 0);

	// Tensor Load 1 -- Tenb is 1
	tensor_coop(TL1_COOP_CSR);
	tensor_load(TL1_TMASK, TL1_IS_COOP, TL1_SCP_START_LINE, TL1_CODE, TL1_TENB,
		    base_src_addr + TL1_ADDR, TL1_OFFSET, TL1_NUM_LINES, TL1_STRIDE, 1);

	tensorWait(TENSOR_LOAD_WAIT_0);
	
	// Tensor FMA    
	tensor_fma(TFMA_TMASK, TFMA_BCOLS, TFMA_AROWS, TFMA_ACOLS, TFMA_ASTART_COL, TFMA_USE_TENC,
		   TFMA_UNSIGNEDA, TFMA_UNSIGNEDB, TFMA_TENB, TFMA_SCP_START_LINEB, TFMA_SCP_START_LINEA,
		   TFMA_TYPE, TFMA_CLEAR_RF);       	
	
	// Modify addresses
	TL0_ADDR = swizzle(TL0_ADDR);
        TL1_ADDR = swizzle(TL1_ADDR);

	// Set the TL0 start line so that is does not overwrite whatever is read by the FMA
	TL0_SCP_START_LINE =  ((TFMA_SCP_START_LINEA % 48) + 16 ) % 48;
    }

    // Store the data with tensor stores
    
    uint64_t ts_reg_stride = 0;
    uint64_t ts_start_reg = 0;
    uint64_t ts_num_cols = 3;
    uint64_t ts_addr = base_dst_addr + (minion_id * 1024);
    uint64_t ts_num_rows = 15;
    uint64_t ts_coop = 0;
    uint64_t ts_stride = 0x40;

    tensor_store(ts_reg_stride, ts_start_reg, ts_num_cols, ts_num_rows, ts_addr, ts_coop, ts_stride);

    tensorWait(TENSOR_STORE_WAIT);

    // Synchronization for all minions in a shire.
    uint64_t barrier_result;
    WAIT_FLB(32, 0, barrier_result);
    if (barrier_result == 1) {

	// The last minion to reach this barrier flushes the CB and sends a credit to all others to continue
	// Having more than one minions flush the CB at the same time may end up in having one of the flushes
	// dropped by the shire cache
	uint64_t cb_flush_addr = 0x8031E100UL;
	//uint64_t shire_id = (hart_id >> 6) & 0x3F;
	uint64_t flush_cb_opcode = 0x0A01UL;
	cb_flush_addr |= (shire_id << 22);
	store(cb_flush_addr, flush_cb_opcode);

	__asm__ __volatile__ ("fence\n");
	// You will need to poll each bank separately
	uint64_t cb_busy = 0;
        while (cb_busy != 0x4) {              
            uint64_t cb_busy_bank[4];
            cb_flush_addr = cb_flush_addr & 0xFFFFFFFFFFFE1FFF;
            for (uint64_t b=0; b < 4; b++) {             
                uint64_t cb_flush_addr_bank = cb_flush_addr | (b << 13);
                uint64_t *cb_flush_addr_bank_addr = (uint64_t *) cb_flush_addr_bank;                
                cb_busy_bank[b] = ((*cb_flush_addr_bank_addr) >> 24) & 0x4;             
            }
            cb_busy = cb_busy_bank[0] | cb_busy_bank[1] | cb_busy_bank[2] | cb_busy_bank[3];
        }
       	
	uint64_t target_min_mask = 0xFFFFFFFFUL;
	target_min_mask = target_min_mask & (~(1ULL << (minion_id & 0x1f)));
	uint64_t addr1 = 0x80340000UL;
	uint64_t fcc_counter1 = 0x0C0UL;
	addr1 |= (shire_id << 22);
	addr1 |= fcc_counter1;
	store(addr1, target_min_mask);
    } else {
	waitFCC(0);
    }

    __asm__ __volatile__ ("fence\n");

    // Need to evict ts_addr to memory with evict va.
    evict_va(0, 3, ts_addr, 15, 0x40, 0, 0);
    WAIT_CACHEOPS;
   
    __asm__ __volatile__ ("fence\n");
    uint64_t functional_error = get_tensor_error();
    if (functional_error != 0) {
        C_TEST_FAIL;
    } else {
	C_TEST_PASS;
    }
}

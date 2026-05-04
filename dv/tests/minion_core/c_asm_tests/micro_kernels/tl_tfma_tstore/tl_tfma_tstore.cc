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

// Coop Tensor Load + FMA + Coop Tstore NOC / L3 / DDR stress test
// Random tensor load 0 (SCP) and  and 1 (TenB) followed by FMA
// On every iteration we load next set of constrained random parameters
// for tensor loads and FMAs. Every new coop load uses a different coop id
// When we are close to using all the available id's we add a barrier so
// that we make sure all coop loads are done and the ids can get recycled.
// After the FMA there is a tensor store either from the RF or from SCP.
// Each shire always writes to the same block but minion ordering may change (see TSTORE_ADDR offsets)
// We use only 5 shires and 4 iterations for the VCS test.
// For Zebu we run on all shires, and run many iterations
// At the end we store data to memory so that we can run a CRC check on Zebu.
// CRC check is not used in VCS because it takes a long time and it is not needed 
// since we have cosim/BEMU.

// The randomized parameters are passed using arrays.
// There are array entried for every iteration of the loop
// and within each iteration parameters for each minion.

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

#include "tl0_configs.h"
#include "tl1_configs.h"
#include "tfma_configs.h"
#include "tstore_configs.h"

#define TL_COOP_CSR_IDX 0
#define TL_IS_COOP_IDX 1
#define TL_TMASK_IDX 2
#define TL_CODE_IDX 3
#define TL_SCP_START_LINE_IDX 4
#define TL_TENB_IDX 5 
#define TL_ADDR_IDX 6 
#define TL_OFFSET_IDX 7
#define TL_NUM_LINES_IDX 8
#define TL_STRIDE_IDX 9
#define TL_PARAMS 10

#define TFMA_TMASK 0 
#define TFMA_BCOLS 1
#define TFMA_AROWS 2
#define TFMA_ACOLS 3
#define TFMA_ASTART_COL 4 
#define TFMA_TENB 5
#define TFMA_SCP_START_LINEA 6
#define TFMA_SCP_START_LINEB 7 
#define TFMA_CLEAR_RF 8
#define TFMA_TYPE 9
#define TFMA_USE_TENC 10
#define TFMA_UNSIGNEDA 11
#define TFMA_UNSIGNEDB 12
#define TFMA_PARAMS 13

#define TSTORE_USE_SCP 0
#define TSTORE_COOP_MASK 1
#define TSTORE_START_LINE 2
#define TSTORE_START_REG TSTORE_START_LINE
#define TSTORE_REG_LINE_STRIDE 3
#define TSTORE_NUM_ROWS 4
#define TSTORE_ROW_SIZE 5
#define TSTORE_ADDR 6
#define TSTORE_STRIDE 7
#define TSTORE_PARAMS 8

#define TOTAL_MINIONS 128
#define NUM_ITER 4
#define NUM_ITER_FOR_BARRIER 4

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

    uint64_t tl_minion_idx = minion_id * TL_PARAMS;
    uint64_t tfma_minion_idx = minion_id * TFMA_PARAMS;
    uint64_t tstore_minion_idx = minion_id * TSTORE_PARAMS;

    uint64_t tl0_coop_csr = tl0_configs[tl_minion_idx + TL_COOP_CSR_IDX];
    uint64_t tl0_is_coop = tl0_configs[tl_minion_idx + TL_IS_COOP_IDX];
    uint64_t tl0_tmask =  tl0_configs[tl_minion_idx + TL_TMASK_IDX];
    uint64_t tl0_code = tl0_configs[tl_minion_idx + TL_CODE_IDX];
    uint64_t tl0_scp_start_line = tl0_configs[tl_minion_idx + TL_SCP_START_LINE_IDX];
    uint64_t tl0_tenb = tl0_configs[tl_minion_idx + TL_TENB_IDX];
    uint64_t tl0_addr = tl0_configs[tl_minion_idx + TL_ADDR_IDX];
    uint64_t tl0_offset = tl0_configs[tl_minion_idx + TL_OFFSET_IDX];
    uint64_t tl0_num_lines = tl0_configs[tl_minion_idx + TL_NUM_LINES_IDX];
    uint64_t tl0_stride = tl0_configs[tl_minion_idx + TL_STRIDE_IDX];   
    
    uint64_t tl1_coop_csr = 0;
    uint64_t tl1_is_coop = 0;
    uint64_t tl1_tmask =  0;
    uint64_t tl1_code = 0;
    uint64_t tl1_scp_start_line = 0;
    uint64_t tl1_tenb = 0;
    uint64_t tl1_addr = 0;
    uint64_t tl1_offset = 0;
    uint64_t tl1_num_lines = 0;
    uint64_t tl1_stride = 0;   
    uint64_t tfma_bcols = 0; 
    uint64_t tfma_arows = 0;
    uint64_t tfma_acols = 0; 
    uint64_t tfma_astart_col = 0; 
    uint64_t tfma_tenb = 0;
    uint64_t tfma_scp_start_linea = 0; 
    uint64_t tfma_scp_start_lineb = 0;
    uint64_t tfma_type = 0;
    uint64_t tfma_unsigneda = 0;
    uint64_t tfma_unsignedb = 0;

    // Set out_data far from in_data so that no overwriting happens
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
    // Write always to the RF and do not add TenC
    tensorWait(TENSOR_LOAD_WAIT_0);
    tl0_tmask = 0;
    tl1_tmask = 0;
    uint64_t tfma_tmask = 0;
    uint64_t tfma_clear_rf = 1;
    uint64_t tfma_use_tenc = 1;
    uint64_t tstore_iter_idx = 0;

    for (uint64_t iter=0; iter < NUM_ITER; iter++) {		

	tstore_iter_idx = iter * TSTORE_PARAMS * TOTAL_MINIONS;

	// === Actual kernel body:
	// Index in arrays is Iteration idx + Minion Idx + Param Idx:
	// Using smaller types will reduce cache misses
	tl1_coop_csr = tl1_configs[tl_minion_idx  + TL_COOP_CSR_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_is_coop = tl1_configs[tl_minion_idx  + TL_IS_COOP_IDX+ iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_code = tl1_configs[tl_minion_idx  + TL_CODE_IDX+ iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_scp_start_line = tl1_configs[tl_minion_idx  + TL_SCP_START_LINE_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_tenb = tl1_configs[tl_minion_idx  + TL_TENB_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_addr = tl1_configs[tl_minion_idx  + TL_ADDR_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_offset = tl1_configs[tl_minion_idx  + TL_OFFSET_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_num_lines = tl1_configs[tl_minion_idx  + TL_NUM_LINES_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	tl1_stride = tl1_configs[tl_minion_idx  + TL_STRIDE_IDX + iter * TL_PARAMS * TOTAL_MINIONS];
	 
	tfma_bcols = tfma_configs[tfma_minion_idx  + TFMA_BCOLS + iter * TFMA_PARAMS * TOTAL_MINIONS]; 
	tfma_arows = tfma_configs[tfma_minion_idx  + TFMA_AROWS + iter * TFMA_PARAMS * TOTAL_MINIONS]; 
	tfma_acols = tfma_configs[tfma_minion_idx  + TFMA_ACOLS + iter * TFMA_PARAMS * TOTAL_MINIONS]; 
	tfma_astart_col = tfma_configs[tfma_minion_idx  + TFMA_ASTART_COL + iter * TFMA_PARAMS * TOTAL_MINIONS]; 
	tfma_tenb = tfma_configs[tfma_minion_idx  + TFMA_TENB + iter * TFMA_PARAMS * TOTAL_MINIONS];
	tfma_scp_start_linea = tfma_configs[tfma_minion_idx  + TFMA_SCP_START_LINEA + iter * TFMA_PARAMS] * TOTAL_MINIONS; 
	tfma_scp_start_lineb = tfma_configs[tfma_minion_idx  + TFMA_SCP_START_LINEB + iter * TFMA_PARAMS * TOTAL_MINIONS];
	tfma_type = tfma_configs[tfma_minion_idx  + TFMA_TYPE + iter * TFMA_PARAMS * TOTAL_MINIONS];
	tfma_unsigneda = tfma_configs[tfma_minion_idx  + TFMA_UNSIGNEDA + iter * TFMA_PARAMS * TOTAL_MINIONS];
	tfma_unsignedb = tfma_configs[tfma_minion_idx  + TFMA_UNSIGNEDB + iter * TFMA_PARAMS * TOTAL_MINIONS];    
	
	tensor_coop(tl0_coop_csr);		
	tensor_load(tl0_tmask, tl0_is_coop, tl0_scp_start_line, tl0_code, tl0_tenb,
	            base_src_addr + tl0_addr, tl0_offset, tl0_num_lines, tl0_stride, 0);
	
	if (iter < NUM_ITER) {
	    tl0_coop_csr = tl0_configs[tl_minion_idx  + TL_COOP_CSR_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_is_coop = tl0_configs[tl_minion_idx  + TL_IS_COOP_IDX+ (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_code = tl0_configs[tl_minion_idx  + TL_CODE_IDX+ (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    // Since start lines are random make sure that the next TL0 does not overwrite the lines read by current FMA
	    tl0_scp_start_line = tfma_scp_start_linea + 16;
	    tl0_tenb = tl0_configs[tl_minion_idx  + TL_TENB_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_addr = tl0_configs[tl_minion_idx  + TL_ADDR_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_offset = tl0_configs[tl_minion_idx  + TL_OFFSET_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_num_lines = tl0_configs[tl_minion_idx  + TL_NUM_LINES_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	    tl0_stride = tl0_configs[tl_minion_idx  + TL_STRIDE_IDX + (iter + 1) * TL_PARAMS * TOTAL_MINIONS];
	}

	// Tensor Load 1 -- Tenb is 1
	tensor_coop(tl1_coop_csr);
	tensor_load(tl1_tmask, tl1_is_coop, tl1_scp_start_line, tl1_code, tl1_tenb,
	            base_src_addr + tl1_addr, tl1_offset, tl1_num_lines, tl1_stride, 1);

	tensorWait(TENSOR_LOAD_WAIT_0);
	
	// Tensor FMA    
	tensor_fma(tfma_tmask, tfma_bcols, tfma_arows, tfma_acols, tfma_astart_col, tfma_tenb,
		   tfma_unsigneda, tfma_unsignedb, tfma_use_tenc, tfma_scp_start_lineb, tfma_scp_start_linea,
		   tfma_type, tfma_clear_rf);

	// Tensor Store
	if (tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_USE_SCP] == 0) {
	    tensor_store(tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_REG_LINE_STRIDE],
			 tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_START_REG],
			 tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_ROW_SIZE],
			 tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_NUM_ROWS],
			 base_dst_addr + tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_ADDR],
			 tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_COOP_MASK],
			 tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_STRIDE]);
	} else {
	    tensorWait(TENSOR_FMA_WAIT);
	    tensor_store_scp(tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_REG_LINE_STRIDE],
			     tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_START_LINE],			  
			     tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_NUM_ROWS],
			     base_dst_addr + tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_ADDR],			  
			     tstore_configs[tstore_iter_idx + tstore_minion_idx + TSTORE_STRIDE]);
	}
	tensorWait(TENSOR_STORE_WAIT);

	// Barrier / FCC sync serves for both coop loads (coop id recycling) and SC CB flush   
	uint64_t barrier_result;
	WAIT_FLB(32, 0, barrier_result);
	if (barrier_result == 1) {
	    
	    // The last minion to reach this barrier flushes the CB and sends a credit to all others to continue
	    // Having more than one minions flush the CB at the same time may end up in having one of the flushes
	    // dropped by the shire cache
	    uint64_t cb_flush_addr = 0x8031E100UL;       
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
    }

    __asm__ __volatile__ ("fence\n");
    uint64_t functional_error = get_tensor_error();
    if (functional_error != 0) {
        C_TEST_FAIL;
    } else {	
	C_TEST_PASS;
    }

    // Sys-emu Zebu -- here you should do a CRC check, again FLB sync and the last does the CRC check for the whole shire.   

}

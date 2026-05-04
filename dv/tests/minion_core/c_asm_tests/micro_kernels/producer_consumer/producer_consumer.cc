/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <inttypes.h>
#include "common.h"
#include "flb.h"
#include "tensors.h"
#include "et_test_common.h"

#ifdef __cplusplus
extern "C"
#endif

// Preload random data (~2M)
cl_data_t _binary_blob_bin_start;

__asm__(
        ".section .data\n\t"
        "_binary_blob_bin_start:          \n\t"
        "       .align      12            \n\t"
        "       .incbin     \"data.raw\"  \n\t"
    );        

#include "tfma_configs.h"
#include "reduce_configs.h"
#include "tload_tstore_mappings.h"

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

#define REDUCE_START_REG_IDX 0
#define REDUCE_OP_IDX 1
#define REDUCE_NUM_REGS_IDX 2
#define REDUCE_PARTNER_IDX 3
#define REDUCE_ACTION_IDX 4
#define REDUCE_PARAMS 5

#define STORE_IDX 0
#define LOAD_IDX 1
#define PAIR_IDX 2

#define MAPPINGS_PARAMS 3
#define TOTAL_MINIONS 128
#define NUM_ITER 4 


// Randomized producer-consumer test
// Part 1: A producer makes an FMA followed by a reduce
// Part 2: The receiver of the reduce does a tensor store and how becomes producer
// Part 3: The producer of part 2 wakes up the consumer with a credit inc, and the consumer does a tensor load
//         from the same location
// Betwen parts 1 and 2 there is no need for synchronization. Reduces communicate using the send/receive types
// Between parts 2 and 3 the CB is flushed by the last minion in each shire to do its tensor store (we use an FLB for that purpose)
// and then each producer minion in a shire does a cridit inc.
// Randomized paramters:
// 1. FMA parameters (FMAs load both inputs from the scratchpad). See file tfma_configs.h
// 2. Reduce parameters (Reduce pairs stay the same across iterations). See file reduce_configs.h
// 3. Tensor store / load pairs, and the location where each producer writes and each consumer reads in buffer. See file tload_tstore_mappings.h
//
// TBD: Randomize number of credits to send/receive -- this will increase FCC activity in the NOC.
// TBD: Randomize Tensor load / Tensor Store fields
int main()
{
    uint64_t hart_id = getHartId();
    uint64_t minion_id = hart_id >> 1;
    //uint64_t shire_id = (hart_id >> 6) & 0x3f;

    if (hart_id >= TOTAL_MINIONS * 2) {
	C_TEST_PASS;
        return 0;
    }
    if (hart_id & 1) {
	C_TEST_PASS;	
	return 0;
    }

    setM0MaskFF();
    setup_cache_scp();

    volatile cl_data_t *data = &_binary_blob_bin_start;
    volatile uint64_t base_src_addr = (uint64_t) data;

    // Fill up the SCP to avoid X's (SCP lines are random)
    tensor_load(0, 0, 0 /*start line*/, 0, 0, // regular tl0 on scp
        base_src_addr + 0x100000 * NUM_ITER, 0, 15 /*num lines*/, 0x40, 0);
    
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    tensor_load(0, 0, 16 /*start line*/, 0, 0, // regular tl0 on scp
        base_src_addr + 0x100000 * NUM_ITER, 0, 15 /*num lines*/, 0x40, 0);
        
    tensorWait(TENSOR_LOAD_WAIT_0);
    
    tensor_load(0, 0, 32 /*start line*/, 0, 0, // regular tl0 on scp
        base_src_addr + 0x100000 * NUM_ITER, 0, 15 /*num lines*/, 0x40, 0);
        
    // Do not use masks (to maximize number of cache line accesses
    uint64_t tfma_minion_idx = minion_id * TFMA_PARAMS;
    uint64_t reduce_minion_idx = minion_id * REDUCE_PARAMS;
    uint64_t mappings_minion_idx = minion_id * MAPPINGS_PARAMS;
    uint64_t tfma_iter_idx = 0;
    uint64_t reduce_iter_idx = 0;
    uint64_t mappings_iter_idx = 0;
    uint64_t tfma_tmask = 0;
    uint64_t tfma_clear_rf = 1;
    uint64_t tfma_use_tenc = 1;
    
    uint64_t store_addr = 0;
    uint64_t load_addr = 0;

    for (uint64_t iter=0; iter < NUM_ITER; iter++) {

	tensorWait(TENSOR_LOAD_WAIT_0);

	tfma_iter_idx = iter * TFMA_PARAMS * TOTAL_MINIONS;
	reduce_iter_idx = iter * REDUCE_PARAMS * TOTAL_MINIONS;
	mappings_iter_idx = iter * MAPPINGS_PARAMS * TOTAL_MINIONS;

	tensor_fma(tfma_tmask,
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_BCOLS],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_AROWS],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_ACOLS],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_ASTART_COL],
		   tfma_use_tenc,
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_UNSIGNEDA],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_UNSIGNEDB],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_TENB],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_SCP_START_LINEB],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_SCP_START_LINEA],
		   tfma_configs[tfma_iter_idx + tfma_minion_idx + TFMA_TYPE],
		   tfma_clear_rf);
	
	tensor_reduce(reduce_configs[reduce_iter_idx + reduce_minion_idx + REDUCE_START_REG_IDX],
		      reduce_configs[reduce_iter_idx + reduce_minion_idx + REDUCE_OP_IDX],
		      reduce_configs[reduce_iter_idx + reduce_minion_idx + REDUCE_NUM_REGS_IDX],
		      reduce_configs[reduce_iter_idx + reduce_minion_idx + REDUCE_PARTNER_IDX],
		      reduce_configs[reduce_iter_idx + reduce_minion_idx + REDUCE_ACTION_IDX]);

	// STORE_IDX is the index inside buffer where this minion will store its Reg file
	store_addr = base_src_addr + iter * 0x100000 + 0x400 * tload_tstore_mappings[mappings_iter_idx + mappings_minion_idx + STORE_IDX];

	tensor_store(0, // reg_stride
		     0, // start_reg
		     3, // row_size
		     15, // num_rows
		     store_addr,
		     0, // coop minions
		     0x40); // stride
		     
	tensorWait(TENSOR_STORE_WAIT);
  
	// Synchronization for all minions in a shire.
	uint64_t barrier_result;
	WAIT_FLB(32, 0, barrier_result);
	if (barrier_result == 1) {
	    
	    // The last minion to reach this barrier flushes the CB and sends a credit to all others to continue
	    // Having more than one minions flush the CB at the same time may end up in having one of the flushes
	    // dropped by the shire cache
	    uint64_t cb_flush_addr = 0x8031E100UL;
	    uint64_t shire_id = (hart_id >> 6) & 0x3F;
	    uint64_t flush_cb_opcode = 0x0A01UL;
	    cb_flush_addr |= (shire_id << 22);
	    store(cb_flush_addr, flush_cb_opcode);
	    
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
	    target_min_mask = target_min_mask & (~(1 << (minion_id & 0x1F)));
	    uint64_t addr1 = 0x80340000UL;
	    uint64_t fcc_counter1 = 0x0C8UL;
	    addr1 |= (shire_id << 22);
	    addr1 |= fcc_counter1;	    
	    store(addr1, target_min_mask);
	} else {
	    waitFCC(1);
	}

	// Add a fence since there are 2 consecutive stores and they need to be ordered
	__asm__ __volatile__ ("fence\n");
	    
	// Do not allow the load consumer to access the buffer until the store is done
	uint64_t target_minion_id = tload_tstore_mappings[mappings_iter_idx + mappings_minion_idx + PAIR_IDX];
	uint64_t target_shire_id = (target_minion_id >> 5) & 0x1F;
	uint64_t target_minion_mask = 1 << (target_minion_id & 0x1F); 
	uint64_t addr = 0x80340000UL;
	uint64_t fcc_counter0 = 0x0C0UL;
	addr |= (target_shire_id << 22);
	addr |= fcc_counter0;
	
	// You can send many credits here -- need to set entry in the mappnigs file
	store(addr, target_minion_mask);
	
	// You can wait for many credits here -- need to set entry in the mappings file
	waitFCC(0);  
	
	// LOAD_IDX is the index inside buffer where this minion will load into its SCP for the next iteration
	// The last iteration does not matter
	uint64_t tl_iter = (iter == NUM_ITER -1) ? iter : iter + 1;
	load_addr = base_src_addr + iter * 0x100000 + 0x400 * tload_tstore_mappings[mappings_iter_idx + mappings_minion_idx + LOAD_IDX];
	
	tensor_load(0, // use_tmask, 
		    0, // use_coop,
		    tfma_configs[tl_iter * TFMA_PARAMS * TOTAL_MINIONS + tfma_minion_idx + TFMA_SCP_START_LINEA], // scp_start_line
		    0, // load_trans, 
		    0, // use_tenb, 
		    load_addr, 
		    0, // offset, 
		    15, // tl_num_rows, 
		    0x40, // stride, 
		    0); //id)	
    }

    tensorWait(TENSOR_LOAD_WAIT_0);

    unsigned long functional_error = get_tensor_error();
    
    if (functional_error != 0) {
	C_TEST_FAIL;
	return -1;
    }
  
    C_TEST_PASS;
    
    return 0;
}

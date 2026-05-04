/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"
#include "esr.h"
#include "tensors.h"

#ifdef __cplusplus
extern "C"
#endif

#define TENSOR_LOAD_WAIT_0 0
#define TENSOR_FMA_WAIT 7

#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
           

int main()
{
    uint64_t hart_id = getHartId();    
    uint64_t minion_id = hart_id >> 1;
    uint64_t shire_id = (hart_id >> 6);

    if (hart_id == 0 || hart_id == 2 || hart_id == 4 || hart_id == 6) {
	setM0MaskFF();
	initFRegs();
    
        // Enable SCP
	mcache_control(1,0,0);
    fence();               // VERIF-3295 (PRM-8)
	mcache_control(1,1,0);
		
	// Enable coop mode	
	enable_shire_coop(shire_id);

	uint64_t reg_stride = 0;
	uint64_t start_reg = 0;
	uint64_t row_size = 0;
	uint64_t num_rows = 15;
	uint64_t coop_minions = 3;
	uint64_t addr =  0x0000008000402000;		
	uint64_t stride = 0x40;
	
	/**TENSOR_STORE_COOP (pair-128 , coop_minions= 3 +1 ) requirement :
	   minion0 store address (5:0 bits of address = 0); minion1 (m0_addr + 16); minion2 (m0_addr + 32); minion3 (m0_addr + 48) **/
	tensor_store(reg_stride, start_reg, row_size, num_rows, (addr + ((minion_id % 4) * 16)), coop_minions, stride);
	tensorWait(8);	
	
	unsigned long functional_error = get_tensor_error();
        
        if (functional_error != 0) {
            C_TEST_FAIL;
            return -1;
        }

   }

    if (hart_id == 0 || hart_id == 2) {

	uint64_t reg_stride = 0;
	uint64_t start_reg = 0;
	uint64_t row_size = 0;
	uint64_t num_rows = 15;
	uint64_t coop_minions = 1;		
	uint64_t addr1 =  0x0000008000d02000;	
	uint64_t stride = 0x40;

	//tensor_store(reg_stride, start_reg, row_size, num_rows, addr + minion_id * 1024, coop_minions, stride);
	/**TENSOR_STORE_COOP (pair-128 , coop_minions= 1 +1 ) requirement :
	   minion0 store address (4:0 bits of address = 0); minion1 (m0_addr + 16);**/
	tensor_store(reg_stride, start_reg, row_size, num_rows, (addr1 + ((minion_id % 2) * 16)), coop_minions, stride);
	tensorWait(8);	

	row_size = 1;
	uint64_t addr2 =  0x0000008000f02000;	
	/**TENSOR_STORE_COOP (pair-256 , coop_minions= 1 +1) requirement :
	   minion0 store address (5:0 bits of address = 0); minion1 (m0_addr + 32);**/
	tensor_store(reg_stride, start_reg, row_size, num_rows, (addr2 + ((minion_id % 2) * 32)), coop_minions, stride);
	tensorWait(8);	

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

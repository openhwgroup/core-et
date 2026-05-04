/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"
#include <inttypes.h>
#include "common.h"

#define ldOffset  0x1000
#define WORD_OFFSET 4

// #define N_LINES_WRITE 256ULL
// #define N_LINES_X_STORE 16ULL
#define N_LINES_RW  16ULL
#define N_TIMES 4ULL

//n blocks:
//each block is 128 b, a CL = 64 B => 4 blocks
#define N_BLOCKS 4ULL

#define SP_INDEX 34ULL

int main()
{

    volatile uint64_t hart_id   = getHartId();
    //volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    volatile uint64_t shire_id_remote =126; //maxion SCP     //32 - shire_id;
    volatile uint64_t shire_id_offset;
    volatile long     init_value;
    volatile uint64_t start_destination;
    volatile uint64_t conf_base;

    volatile uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

    C_TEST_START;
    
    setup_cache_scp();

    setM0MaskFF();


    if( (hart_id & 1) == 0) {
      
//WRITE TO REMOTE SCP
      init_value = shire_id << 24;
      initFRegs_2(init_value);

      *baseAddr = 0xbabe;

      shire_id_offset   = shire_id;
      start_destination = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore(conf, N_BYTES_CL);
        start_destination += N_LINES_RW*N_BYTES_CL;
      }

//READ FROM REMOTE SCP (data that was previously written by current shire)  ---> to L2SCP
      // tensorLoadL2SCP
      // usemsk = (xs[63] == 1);
      // dst    = (xs[62:48] * 4 + xs[5:4]);
      // VA     = xs[47:6] * 64;
      // count  = xs[3:0] + 1;
      // stride = x31[47:6] * 64;
      // for (i = 0; i < count; i++) {
      //     if (!usemsk || tensor_mask[i] == 1) {
      //         L2Scp[(dst + i) % L2ScpLines][*] = MEM(VA + i*stride, 512);    
      // }
      volatile uint64_t VA;
      volatile uint64_t l2_scp_line; // in Mode 0, 2.5MB is given to Scratchpad in the shire cache. 2.5MB would be ~40K cache lines. the l2_scp_line needs to be updated accordingly to be able to accomodate lines requested by all minions (32 in total accessing SCP of same shire) 
      volatile uint64_t conf;

      shire_id_offset = shire_id; //source offset
      VA              = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //source
      shire_id_offset = 0; //destination offset
      l2_scp_line     = shire_id_offset*N_TIMES*N_LINES_RW;//*N_BYTES_CL;

      for(uint64_t i = 0; i < N_TIMES ; i++){
        conf = (N_LINES_RW-1) | VA | ((l2_scp_line>>2ULL) << 48ULL);
        tensorLoadL2SCP(conf, N_BYTES_CL);
        VA += N_LINES_RW*N_BYTES_CL;
        l2_scp_line += N_LINES_RW;
      }

//CHECK PHASE
      volatile uint64_t testData;
      volatile uint64_t readData;
      volatile uint64_t* pAddr;
      volatile uintptr_t baseAddresses;

//check_0: local SCB read loadL2 data from other compute shire(data previously written by current shire)
      shire_id_offset = 0;
      baseAddresses =  ((2U)<<30U)  + (shire_id << 23) + (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);

      for (uint64_t k=0; k<N_TIMES; k++) {
        for (int i=0; i<32; i++) {

          testData =(uint64_t)(shire_id<<24) + (uint64_t)i; 
          testData <<= 32;
          testData +=(uint64_t)(shire_id<<24) + (uint64_t)i; 

          for (int j=0; j<4; j++) {
            pAddr = (uint64_t*)(baseAddresses + k*32*32 +  i*32 + j*8); 
            readData = *pAddr; 
            if (readData != testData){
              C_TEST_FAIL;  
            } 
          }
        }
      }            

         

    }


// Check if there was any error in the Tensor Operations
    tensor_error_check();

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/

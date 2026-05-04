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



inline void __attribute__((always_inline)) self_check_scp(uint64_t shire_id, uint64_t address, uint64_t pattern, uint64_t num, uint64_t format)
{
  volatile uint64_t testData;
  volatile uint64_t readData;
  volatile uint64_t* pAddr;
  volatile uintptr_t baseAddresses;

  for (uint64_t k=0; k<num; k++) {
    for (int i=0; i<32; i++) {

      baseAddresses = address + (uint64_t)(k*32*32) +  (uint64_t)(i*32);

      if(format == 0){
        baseAddresses =  ((2U)<<30U)  + (shire_id << 23) + baseAddresses;
      }else{
        baseAddresses =  ((3U)<<30U)  + ((shire_id & 0x60) << 23) + ((shire_id & 0x1F) << 6) + ((baseAddresses & 0x7FFFC0) << 5) + (baseAddresses & 0x3F);
      }

      testData =  pattern + (uint64_t)i; 
      testData <<= 32;
      testData += pattern + (uint64_t)i; 

      for (int j=0; j<4; j++) {
        pAddr = (uint64_t*)(baseAddresses + j*8); 
        readData = *pAddr; 
        if (readData != testData){
          C_TEST_FAIL;  
        } 
      }
    }
  }
}


int main()
{

    volatile uint64_t hart_id   = getHartId();
    //volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    volatile uint64_t shire_id_remote = 32 - shire_id;
    volatile uint64_t shire_id_offset;
    volatile long     init_value;
    volatile uint64_t start_destination;
    volatile uint64_t conf_base;

    volatile uint64_t VA;
    volatile uint64_t l2_scp_line; // in Mode 0, 2.5MB is given to Scratchpad in the shire cache. 2.5MB would be ~40K cache lines. the l2_scp_line needs to be updated accordingly to be able to accomodate lines requested by all minions (32 in total accessing SCP of same shire) 
    volatile uint64_t conf;

    volatile uint64_t testData;
    volatile uint64_t readData;
    volatile uint64_t* pAddr;
    volatile uintptr_t baseAddresses;

    volatile uint64_t address;
    volatile uint64_t pattern;

    uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

    C_TEST_START;
    
    setup_cache_scp(); 

    setM0MaskFF();


    if( (hart_id & 1) == 0) {
      
//local SCP zero initialization
      initFRegs_0();

      *baseAddr = 0xbabe;

      shire_id_offset   = 2;
      start_destination = ((2U)<<30U)  + (127ULL << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore( conf , N_BYTES_CL);
        start_destination+=N_LINES_RW*N_BYTES_CL;
      } 

      shire_id_offset   = 637;
      start_destination = ((2U)<<30U)  + (127ULL << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore( conf , N_BYTES_CL);
        start_destination+=N_LINES_RW*N_BYTES_CL;
      }    

//SCP address format 0
  //SCP START ADDESS IN RANGE
    //WRITE TO REMOTE SCP
      init_value = shire_id << 24;
      initFRegs_2(init_value);

      shire_id_offset   = 0;//destination offset
      start_destination = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore(conf, N_BYTES_CL);
        start_destination += N_LINES_RW*N_BYTES_CL;
      }

    //READ FROM REMOTE SCP (data that was previously written by current shire)  ---> to L2SCP
      shire_id_offset = 0; //source offset
      VA              = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //source
      shire_id_offset = 1; //destination offset
      l2_scp_line     = shire_id_offset*N_TIMES*N_LINES_RW;//*N_BYTES_CL;

      for(uint64_t i = 0; i < N_TIMES ; i++){
        conf = (N_LINES_RW-1) | VA | ((l2_scp_line>>2ULL) << 48ULL);
        tensorLoadL2SCP(conf, N_BYTES_CL);
        VA += N_LINES_RW*N_BYTES_CL;
        l2_scp_line += N_LINES_RW;
      }

  //SCP END ADDESS IN RANGE
    //WRITE TO REMOTE SCP
      init_value = (shire_id << 24) + (170ULL << 16);  // 170 == xAA
      initFRegs_2(init_value);

      shire_id_offset   = 639;//destination offset
      start_destination = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        uint64_t conf = conf_base | start_destination; 
        tensorStore(conf, N_BYTES_CL);
        start_destination += N_LINES_RW*N_BYTES_CL;
      }

    //READ FROM REMOTE SCP (data that was previously written by current shire)  ---> to L2SCP
      shire_id_offset = 639; //source offset
      VA              = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //source
      shire_id_offset = 638; //destination offset
      l2_scp_line     = shire_id_offset*N_TIMES*N_LINES_RW;//*N_BYTES_CL;

      for(uint64_t i = 0; i < N_TIMES ; i++){
        conf = (N_LINES_RW-1) | VA | ((l2_scp_line>>2ULL) << 48ULL);
        tensorLoadL2SCP(conf, N_BYTES_CL);
        VA += N_LINES_RW*N_BYTES_CL;
        l2_scp_line += N_LINES_RW;
      }

//give and wait for credit, all shires and SP to finish with remote transactions before check phase
      give_credit((int)shire_id_remote, 0, 0, 0);  //int shire_id, int minion_id, int thread_id, int credit_counter_num
      wait_for_credit(0);

//check_1: local SCB read written(tensorStore) data from other compute shire
      shire_id_offset = 0; 
      address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
      pattern = (uint64_t)(shire_id_remote<<24); 
      self_check_scp(shire_id, address, pattern, N_TIMES, 1); // source shire_id, address, pattern, num, format
//1
      shire_id_offset = 639; 
      address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
      pattern = (uint64_t)(shire_id_remote<<24)  + (uint64_t)(170ULL << 16); 
      self_check_scp(shire_id, address, pattern, N_TIMES, 1); // source shire_id, address, pattern, num, format

//check_0: local SCB read loadL2 data from other compute shire(data previously written by current shire)
      shire_id_offset = 1; 
      address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
      pattern = (uint64_t)(shire_id<<24); 
      self_check_scp(127ULL, address, pattern, N_TIMES, 1); // source shire_id, address, pattern, num, format
//1
      shire_id_offset = 638; 
      address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
      pattern = (uint64_t)(shire_id<<24)  + (uint64_t)(170ULL << 16); 
      self_check_scp(127ULL, address, pattern, N_TIMES, 1); // source shire_id, address, pattern, num, format

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// //SCP address format 1
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   //SCP START ADDESS IN RANGE
//     //WRITE TO REMOTE SCP
//       init_value = (shire_id << 24) + (85ULL << 16);  // 85 == x55
//       initFRegs_2(init_value);

//       shire_id_offset   = 0;//destination offset
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       //start_destination = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
//       conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

//       for(uint64_t i = 0 ; i < N_TIMES ; i++){
//         start_destination =  ((3U)<<30U)  + ((shire_id_remote & 0x60) << 23) + ((shire_id_remote & 0x1F) << 6) + ((address & 0x7FFFC0) << 5) + (address & 0x3F);
//         uint64_t conf = conf_base | start_destination; 
//         tensorStore(conf, N_BYTES_CL);
//         address += N_LINES_RW*N_BYTES_CL;
//       }

//     //READ FROM REMOTE SCP (data that was previously written by current shire)  ---> to L2SCP
//       shire_id_offset = 0; //source offset
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       //VA              = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //source
//       shire_id_offset = 1; //destination offset
//       l2_scp_line     = shire_id_offset*N_TIMES*N_LINES_RW;//*N_BYTES_CL;

//       for(uint64_t i = 0; i < N_TIMES ; i++){
//         VA =  ((3U)<<30U)  + ((shire_id_remote & 0x60) << 23) + ((shire_id_remote & 0x1F) << 6) + ((address & 0x7FFFC0) << 5) + (address & 0x3F); 
//         conf = (N_LINES_RW-1) | VA | ((l2_scp_line>>2ULL) << 48ULL);
//         tensorLoadL2SCP(conf, N_BYTES_CL);
//         address += N_LINES_RW*N_BYTES_CL;
//         l2_scp_line += N_LINES_RW;
//       }

//   //SCP END ADDESS IN RANGE
//     //WRITE TO REMOTE SCP
//       init_value = (shire_id << 24)+ (150ULL << 16);  // 150 == x96 
//       initFRegs_2(init_value);

//       shire_id_offset   = 639;//destination offset
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       //start_destination = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
//       conf_base         = ((N_BLOCKS-1) << 55) | ((N_LINES_RW-1) << 51); //n blocks: //each block is 128 b, a CL = 64 B => 4 blocks

//       for(uint64_t i = 0 ; i < N_TIMES ; i++){
//         start_destination =  ((3U)<<30U)  + ((shire_id_remote & 0x60) << 23) + ((shire_id_remote & 0x1F) << 6) + ((address & 0x7FFFC0) << 5) + (address & 0x3F);
//         uint64_t conf = conf_base | start_destination; 
//         tensorStore(conf, N_BYTES_CL);
//         address += N_LINES_RW*N_BYTES_CL;
//       }

//     //READ FROM REMOTE SCP (data that was previously written by current shire)  ---> to L2SCP
//       shire_id_offset = 639; //source offset
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       //VA              = ((2U)<<30U)  + (shire_id_remote << 23) +  (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL); //source
//       shire_id_offset = 638; //destination offset
//       l2_scp_line     = shire_id_offset*N_TIMES*N_LINES_RW;//*N_BYTES_CL;

//       for(uint64_t i = 0; i < N_TIMES ; i++){
//         VA = ((3U)<<30U)  + ((shire_id_remote & 0x60) << 23) + ((shire_id_remote & 0x1F) << 6) + ((address & 0x7FFFC0) << 5) + (address & 0x3F);
//         conf = (N_LINES_RW-1) | VA | ((l2_scp_line>>2ULL) << 48ULL);
//         tensorLoadL2SCP(conf, N_BYTES_CL);
//         address += N_LINES_RW*N_BYTES_CL;
//         l2_scp_line += N_LINES_RW;
//       }

// //give and wait for credit, all shires and SP to finish with remote transactions before check phase
//       give_credit((int)shire_id_remote, 0, 0, 0);  //int shire_id, int minion_id, int thread_id, int credit_counter_num
//       wait_for_credit(0);


// //check_1: local SCB read written(tensorStore) data from other compute shire
//       shire_id_offset = 0; 
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       pattern = (uint64_t)(shire_id<<24) + (uint64_t)(85ULL << 16);
//       self_check_scp(shire_id, address, pattern, N_TIMES, 0); // source shire_id, address, pattern, num, format

//       shire_id_offset = 639; 
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       pattern = (uint64_t)(shire_id_remote<<24)  + (uint64_t)(150ULL << 16); 
//       self_check_scp(shire_id, address, pattern, N_TIMES, 0); // source shire_id, address, pattern, num, format
// //1
// //check_0: local SCB read loadL2 data from other compute shire(data previously written by current shire)
//       shire_id_offset = 1; 
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       pattern = (uint64_t)(shire_id<<24) + (uint64_t)(85ULL << 16);
//       self_check_scp(127ULL, address, pattern, N_TIMES, 0); // source shire_id, address, pattern, num, format

//       shire_id_offset = 638; 
//       address = (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);
//       pattern = (uint64_t)(shire_id<<24)  + (uint64_t)(150ULL << 16); 
//       self_check_scp(127ULL, address, pattern, N_TIMES, 0); // source shire_id, address, pattern, num, format
// //1

//check_2: local SCB read written(tensorStore) data from SP
//

      shire_id_offset = 2; 
      baseAddresses =  ((2U)<<30U)  + (shire_id << 23) + (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);


      wait_for_credit(1);
      
      testData =(uint64_t)(SP_INDEX<<24) + (uint64_t)(shire_id<<16); 
      testData <<= 32;
      testData +=(uint64_t)(SP_INDEX<<24) + (uint64_t)(shire_id<<16); 

      for (int j=0; j<4; j++) {
        pAddr = (uint64_t*)(baseAddresses + j*8); 
        readData = *pAddr; 
        if (readData != testData){
          C_TEST_FAIL;  
        } 
      }


      shire_id_offset = 637; 
      baseAddresses =  ((2U)<<30U)  + (shire_id << 23) + (shire_id_offset*N_TIMES*N_LINES_RW*N_BYTES_CL);

      wait_for_credit(1);
      testData =(uint64_t)(SP_INDEX<<24) + (uint64_t)(shire_id<<16) + (uint64_t)(180ULL<<8); 
      testData <<= 32;
      testData +=(uint64_t)(SP_INDEX<<24) + (uint64_t)(shire_id<<16) + (uint64_t)(180ULL<<8); 

      for (int j=0; j<4; j++) {
        pAddr = (uint64_t*)(baseAddresses + j*8); 
        readData = *pAddr; 
        if (readData != testData){
          C_TEST_FAIL;  
        } 
      }
     
    }


// Check if there was any error in the Tensor Operations
    tensor_error_check();

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/




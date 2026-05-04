/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_WRITE 16ULL
#define N_TIMES 32


           
int main()
{
    C_TEST_START; 
    bool passing = true;
    uint64_t hart_id = getHartId();

    uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    if( ((hart_id & 1) == 0) && (shire_id == 0)) {
      setM0MaskFF();
      initFRegs();
      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t start_destination =  ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_TIMES*N_LINES_WRITE*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      //n blocks:
      //each block is 128 b, a CL = 64 B => 4 blocks
      uint64_t n_blocks = 4;
      uint64_t conf_base =   ( (n_blocks-1) << 55)
                     | ( (N_LINES_WRITE-1) << 51);

      
      for(int i = 0; i < N_TIMES ; i++){
        uint64_t conf=conf_base|start_destination;
        uint64_t *addr; 
        addr = (uint64_t*) start_destination;
        tensorStore(conf, N_BYTES_CL);        
        start_destination+=N_LINES_WRITE*N_BYTES_CL;
        tensorWait(8);

        uint64_t data = 0; 
        uint64_t exp_data = 0;       
        
        for(int j = 0; j < 32; j++) {

          if (j < 8) {
            exp_data = (j << 12) | (j << 8) | (j << 4) | j; //(j << 44) | (j << 40) | (j << 36) | (j << 32) |
            exp_data = (exp_data << 32) | (j << 12) | (j << 8) | (j << 4) | j;
          }
          else if (j < 0x10) {
            exp_data = (j << 12) | (j << 8) | (j << 4) | ((j+1) & 0xffff);
            exp_data = (exp_data << 32) | (j << 12) | (j << 8) | (j << 4) | ((j+1) & 0xffff);
          }
          else if (j < 0x18) {
            exp_data = (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | (j & 0xf);
            exp_data = (exp_data << 32) | (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | (j & 0xf);
          }
          else {
            exp_data = (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | ((j+1) & 0xf);
            exp_data = (exp_data << 32) | (1 << 16) | ((j & 0xf) << 12) | ((j & 0xf) << 8) | ((j & 0xf) << 4) | ((j+1) & 0xf);
          }

          data = *addr;
          addr+=0x4;

          if (exp_data != data) {
            passing = false;
          }
        }
      }
    }

    if(passing){
      C_TEST_PASS;
    }
    else {
      C_TEST_FAIL;
    }
    // End marker
    return 0;
}

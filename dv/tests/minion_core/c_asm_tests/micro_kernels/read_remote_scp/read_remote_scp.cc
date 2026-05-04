/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_READ 16ULL
#define N_TIMES 64 
           
int main()
{
    uint64_t hart_id = getHartId();

      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    if( (hart_id & 1) == 0 ) {


      setM0MaskFF();
      initFRegs();
      uint64_t start_destination =  ((2U)<<30U)  + (minion_id << 23) +  (shire_id*N_TIMES*N_LINES_READ*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      //n blocks:
      //each block is 128 b, a CL = 64 B => 4 blocks
      uint64_t n_blocks = 4;
      uint64_t conf_base =   ( (n_blocks-1) << 55)
                     | ( (N_LINES_READ-1) << 51);

      
      for(int i = 0; i < N_TIMES ; i++){
        uint64_t conf=conf_base|start_destination;
        tensorStore(conf, N_BYTES_CL);
        start_destination+=N_LINES_READ*N_BYTES_CL;
      }



      uint64_t start_source =  ((2U)<<30U)  + (minion_id << 23) +  (shire_id*N_TIMES*N_LINES_READ*N_BYTES_CL); //each minion reads 1024 CL * 64 Bytes/CL

      conf_base =  2ULL << 58ULL
                     | (N_LINES_READ-1);

    

      for(int i = 0; i < N_TIMES ; i++){
        uint64_t conf=conf_base|start_source;
        prefetch(conf, N_BYTES_CL, 0);
        start_source+=N_LINES_READ*N_BYTES_CL;
      }
    }

    C_TEST_PASS;
    // End marker
    return 0;
}

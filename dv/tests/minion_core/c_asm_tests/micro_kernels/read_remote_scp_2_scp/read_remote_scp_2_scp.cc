/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_READ  16ULL
#define N_TIMES 64
           
int main()
{
    uint64_t hart_id = getHartId();

    if( (hart_id & 1) == 0) {

      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t shire_id  = ((hart_id>>6) & 0x3F);




      //INIT THE L2 SCP

      setM0MaskFF();
      initFRegs();
      uint64_t start_destination =  ((2U)<<30U)  + (minion_id << 23) +  (shire_id*N_TIMES*N_LINES_READ*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL

      //n blocks:
      //each block is 128 b, a CL = 64 B => 4 blocks
      uint64_t n_blocks = 4;
      uint64_t conf_base_init =   ( ((n_blocks-1) << 55))
                     | ( (N_LINES_READ-1) << 51);
      for(uint64_t i = 0 ; i < N_TIMES ; i++){
        tensorStore(conf_base_init | (start_destination) , N_BYTES_CL);
        start_destination+=N_LINES_READ*N_BYTES_CL;
      }

      //READ FROM REMOTE SCP


      uint64_t VA = ((2U)<<30U)  + (minion_id << 23) +  (shire_id*N_TIMES*N_LINES_READ*N_BYTES_CL);

      // in Mode 0, 2.5MB is given to Scratchpad in the shire cache. 
      // 2.5MB would be ~40K cache lines. the l2_scp_line needs to be updated accordingly to be able to accomodate lines requested by all minions (32 in total accessing SCP of same shire)
      //uint64_t l2_scp_line = (32+minion_id)*N_LINES_READ*N_TIMES;
      uint64_t l2_scp_line = (minion_id)*N_LINES_READ*N_TIMES;
      uint64_t conf_base =   (N_LINES_READ-1);

      for(int i = 0; i < N_TIMES ; i++){
        uint64_t conf=conf_base|VA| ( (l2_scp_line>>2ULL)   << 48ULL);
        tensorLoadL2SCP(conf, N_BYTES_CL);
        VA+=N_LINES_READ*N_BYTES_CL;
        l2_scp_line+=N_LINES_READ;
      }
    }

    C_TEST_PASS;
    // End marker
    return 0;
}

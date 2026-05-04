/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_WRITE 256ULL
#define N_LINES_X_STORE 16ULL
#define N_STORES 16ULL
           
int main()
{
    uint64_t hart_id = getHartId();

    if( (hart_id & 1) == 0) {
      setM0MaskFF();
      initFRegs();

      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t shire_id  = ((hart_id>>6) & 0x3F);
      uint64_t start_destination =  ((2U)<<30U)  + (minion_id << 23) +  (shire_id*N_LINES_WRITE*N_BYTES_CL); //each minion writes 1024 CL * 64 Bytes/CL
      //n blocks:
      //each block is 128 b, a CL = 64 B => 4 blocks
      uint64_t n_blocks = 4;
      uint64_t conf_base =   ( ((n_blocks-1) << 55))
                     | ( (N_LINES_X_STORE-1) << 51);
      for(uint64_t i = 0 ; i < N_STORES ; i++){
        tensorStore(conf_base | (start_destination) , N_BYTES_CL);
        start_destination+=N_LINES_X_STORE*N_BYTES_CL;
      }

    }

    C_TEST_PASS;
    // End marker
    return 0;
}

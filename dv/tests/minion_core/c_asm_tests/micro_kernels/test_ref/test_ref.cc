/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif



#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10
           
int main()
{
    uint64_t hart_id = getHartId();
    //todo init A
    //
    //
    if( (hart_id & 1) == 0) {


      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t shire_id  = ((hart_id>>6) & 0x3F);

      uint64_t VA = ((2U)<<30U)  + (shire_id << 23) +  (minion_id*N_LINES_X_MINION*N_BYTES_CL);
      uint64_t l2_scp_line = (minion_id*N_LINES_OP)+ N_LINES_DST;

      uint64_t conf =  ( (l2_scp_line>>2ULL)   << 48)
                     | VA 
                     | N_LINES_OP;

      for(int i = 0 ; i < N_TIMES ; i++){
        tensorLoadL2SCP(conf, N_BYTES_CL);
      }


    }

    fence();
    wfi();
    // End marker
    return 0;
}

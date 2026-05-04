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
    if( hart_id  == 0) {
       volatile int __attribute__((aligned(4096))) ptr[1024];

       // Initialize array
       for (int i = 0; i < 1024; ++i) {
          ptr[i] = i;
       }

       uint64_t cstart, cend, cdiff;
       cstart = et_get_cycle_count();
       // Evict cache
       for (unsigned set = 0; set < 16; ++set) {
          evict_sw_no_mask(false,     0x1, 0, set,   0       );
       }
       WAIT_CACHEOPS;

       for (unsigned set = 0; set < 16; ++set) {
          evict_sw_no_mask(false,     0x1, 1, set,   0       );
       }
       WAIT_CACHEOPS;

       for (unsigned set = 0; set < 16; ++set) {
          evict_sw_no_mask(false,     0x1, 2, set,   0       );
       }
       WAIT_CACHEOPS;

       for (unsigned set = 0; set < 16; ++set) {
          evict_sw_no_mask(false,     0x1, 3, set,   0       );
       }
       WAIT_CACHEOPS;
       cend = et_get_cycle_count();
       cdiff = cend - cstart;
       print_cycles(cdiff);

    }

    C_TEST_PASS;
    // End marker
    return 0;
}

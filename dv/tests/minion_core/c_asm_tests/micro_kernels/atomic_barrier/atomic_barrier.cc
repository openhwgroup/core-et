/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
#endif



#define N_LINES_X_MINION 1024ULL
#define N_LINES_DST 512ULL
#define N_LINES_OP 512ULL
#define N_TIMES 10

long barrier __attribute__((aligned(512))) = 512;
long *p_barrier=&barrier;

int main()
{
    uint64_t hart_id = getHartId();

    if(((hart_id & 1) == 0) && (hart_id < 1024))
      __syncthreads(p_barrier);

    C_TEST_PASS;
    // End marker
    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 10
#include <inttypes.h>
#include "common.h"

#define BUF_SIZE (1 << 22) 
#define BUF_SLOT BUF_SIZE/256

#ifdef __cplusplus
extern "C"
#endif
// functions to deal with CSR regs
void stub(){

}
uint64_t buf[BUF_SIZE] __attribute__((aligned(65536)));
           
int main()
{
  //for(int iter=0;iter<1000;+iter) forceDelay();
  if(getHartId()%2 == 0){
    int idx = ((getHartId()>>1)) << 3;
    long data = 0;

    uint64_t cstart, cend, cdiff;
    cstart = et_get_cycle_count();
    for(int i=0;i<ITERS;++i){
      load((long(&buf[idx])),data);
      data++;
      idx+=0x10000;
    }
    cend = et_get_cycle_count();
    cdiff = cend - cstart;
    print_cycles(cdiff);
  }
  
  C_TEST_PASS;
  return 0;
}

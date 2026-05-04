/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define ITERS 10
#include <inttypes.h>
#include "common.h"

#define BUF_SIZE (1 << 22) 
#define BUF_SLOT BUF_SIZE/256

#define SC_IDX_COP_SM_CTL (0x6ULL << 3ULL)
#define BANK_FULL (0xFULL << 13ULL)
#define BANK_0 (0x0ULL << 13ULL)
#define BANK_1 (0x1ULL << 13ULL)
#define BANK_2 (0x2ULL << 13ULL)
#define BANK_3 (0x3ULL << 13ULL)
#define SHIRE_CACHE_ESR (24ULL << 17ULL)
#define N_SHIRES (0x0ULL << 22ULL)
#define PRIVILEGE_MODE (3ULL <<30ULL)
#define ESR_CONST (1ULL << 32ULL)

#define L2_EVICT (3<<8)



#ifdef __cplusplus
extern "C"
#endif
// functions to deal with CSR regs
void stub(){

}
uint64_t buf[BUF_SIZE] __attribute__((aligned(65536)));
           
int main()
{

    uint64_t esr_addr_w = 0;
    uint64_t esr_addr_0 = 0;
    uint64_t esr_addr_1 = 0;
    uint64_t esr_addr_2 = 0;
    uint64_t esr_addr_3 = 0;

    esr_addr_w = ESR_CONST | PRIVILEGE_MODE | N_SHIRES | SHIRE_CACHE_ESR | BANK_FULL | SC_IDX_COP_SM_CTL;
    esr_addr_0 = ESR_CONST | PRIVILEGE_MODE | N_SHIRES | SHIRE_CACHE_ESR | BANK_0 | SC_IDX_COP_SM_CTL;
    esr_addr_1 = ESR_CONST | PRIVILEGE_MODE | N_SHIRES | SHIRE_CACHE_ESR | BANK_1 | SC_IDX_COP_SM_CTL;
    esr_addr_2 = ESR_CONST | PRIVILEGE_MODE | N_SHIRES | SHIRE_CACHE_ESR | BANK_2 | SC_IDX_COP_SM_CTL;
    esr_addr_3 = ESR_CONST | PRIVILEGE_MODE | N_SHIRES | SHIRE_CACHE_ESR | BANK_3 | SC_IDX_COP_SM_CTL;

    volatile uint64_t* esr_addr_p = reinterpret_cast<volatile uint64_t*> (esr_addr_w);
    uint64_t esr_data = 0;
    uint64_t w0 = 0,w1 = 0,w2 = 0,w3 = 0;
    esr_data = L2_EVICT | 1;//1 on bit 0 means go

  //for(int iter=0;iter<1000;+iter) forceDelay();
  if(getHartId()%2 == 0){
    int idx = ((getHartId()>>1)) << 3;
    long data = 0;

    uint64_t cstart, cend, cdiff;

    //READ TO MEM - L3 ALL MISS
    for(int i=0;i<ITERS;++i){
      load((long(&buf[idx])),data);
      data++;
      idx+=0x10000;
    }

    //EVICT ALL L2////////////////
    *esr_addr_p = esr_data;
     fence();
     while(w0!=4 or w1!=4 or w2!=4 or w3!=4){
       if(w0!=4){
       	w0 = *(reinterpret_cast<volatile uint64_t*> (esr_addr_0));
       	w0 = ((w0 >> 24) & 0x0FULL);
       }
      if(w1!=4){
       	w1 = *(reinterpret_cast<volatile uint64_t*> (esr_addr_1));
       	w1 = ((w1 >> 24) & 0x0FULL);
      }
      if(w2!=4){
       	w2 = *(reinterpret_cast<volatile uint64_t*> (esr_addr_2));
       	w2 = ((w2 >> 24) & 0x0FULL);
      }
      if(w3!=4){
       	w3 = *(reinterpret_cast<volatile uint64_t*> (esr_addr_3));
       	w3 = ((w3 >> 24) & 0x0FULL);
      }
     }
     //////////////////////////////////////////////
    
    //READ TO MEM - L3 ALL HITS
    idx = ((getHartId()>>1)) << 3;
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

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif

#define CREDINC0_ESR  0x803400C0L //[29:22] replaced by specific shire
#define CREDINC1_ESR  0x803400C8L
#define CREDINC2_ESR  0x803400D0L
#define CREDINC3_ESR  0x803400D8L

#define FCC_CREDITINC_0 0x18UL
#define FCC_CREDITINC_1 0x19UL
#define FCC_CREDITINC_2 0x1AUL
#define FCC_CREDITINC_3 0x1BUL

#define SHIRE_0_MASK    0x00000001UL
#define ALL_SHIRE_MASK  0xFFFFFFFFUL

#define MINION_0_MASK   0x00000001UL
#define ALL_MINION_MASK 0xFFFFFFFFUL

#define USER_MODE_ADDR       (0x8035FFF8ULL)
#define SUPERVISOR_MODE_ADDR (0x8075FFF8ULL)
#define MACHINE_MODE_ADDR    (0x80F5FFF8ULL)
#define UARCH_MODE_ADDR      (0x80B5FFF8ULL)

#define USER_MODE        0x0L
#define SUPERVISOR_MODE  0x1L
#define MACHINE_MODE     0x2L
#define UARCH_MODE       0x3L

#define ET_LINK_ADDR 0x8035FFF0UL
#define BROADCAST_ADDR (ET_LINK_ADDR)
#define SHIRE_ESR_ADR_REGION 0x1AUL

void broadcast (long esr2broadcast, long shire2broadcast, long data)
{
  //It's assumed we are always are in usermode
  volatile uint64_t* addr_broadcast;
  long data2 = ((USER_MODE<<59)|((SHIRE_ESR_ADR_REGION<<54)|(esr2broadcast << 40))|shire2broadcast);

  addr_broadcast  = reinterpret_cast<volatile uint64_t*> BROADCAST_ADDR;  
  *addr_broadcast = data;
  addr_broadcast  = reinterpret_cast<volatile uint64_t*> USER_MODE_ADDR;  
  *addr_broadcast = data2;
}

// Send IPIs (through ESR writes) ML-163          
int main()
{
    uint64_t hart_id = getHartId();
    
    if( (hart_id & 1) == 0) {
      uint64_t minion_id = ((hart_id>>1) & 0x1F);
      uint64_t shire_id  = ((hart_id>>6) & 0x3F);

      if (shire_id == 32) {
        if(minion_id ==0) {
          for(int i=0; i<1000; i++) {
            broadcast(FCC_CREDITINC_0, ALL_SHIRE_MASK, ALL_MINION_MASK);
            fence();
          }
        }
      }
      else {
        for (int i=0; i< 1000; i++) {
          waitFCC(0);
        }
      }
    }
    
    C_TEST_PASS;
    // End marker
    return 0;
}

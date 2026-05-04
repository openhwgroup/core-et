/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>

#include "soc_common.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"

#define NEIGH0_IPI_REDIRECT_PC 0x80100040

#define L1_TAG_OFFSET 10

uint32_t base[(5<<10)/sizeof(uint32_t)];
           
int main()
{
  //  uint8_t base[512];
    
    //C_TEST_START;
    volatile uint64_t *esr_poll_value;
    volatile uint64_t *esr_write;
    uint64_t a = 0;
    bool passing = true;
    volatile uint64_t *ptr[5];
    uint64_t hart_id = getHartId();
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    uint64_t minion_id  = ((hart_id>>1) & 0x3F);





    if( ((hart_id & 1) == 0 ) && (shire_id == 0) && (minion_id == 0) ) {
      
      // Wait for the debugger to write this ESR
      esr_poll_value = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 
      esr_write      = (volatile uint64_t*) NEIGH0_IPI_REDIRECT_PC; 

      uint64_t toto;
      for ( int bank = 0; bank< 4; bank++) {
        
        // Polling ESR
        while ( *esr_poll_value != 0xFE );   

        //Load 5 pointers (4 ways +1)
        for ( int i = 0; i< 5; i++) {
          ptr[i] = (volatile uint64_t*) &(base[(i<<L1_TAG_OFFSET)/ sizeof(uint32_t) ]);
          ptr[i] = (volatile uint64_t*) ((uint64_t)ptr[i] | (bank <<6)); 
        }

        //Randomize array
        for (int ii = 0; ii < 5; ++ii) {
          toto = et_get_rand_dword();
          *ptr[ii]=  toto;
        }

        
        //10 times 5 consecutive reads to L1 --> 1 to L2
        for( int iter=0;iter<10;iter++) {
          for ( int i = 0; i< 5; i++) a+=*ptr[i];
        }
        
        // Handshake to ESR
        *esr_write = 0xF0;

      } // Bank iter
    } // Minion

    if(passing){
      C_TEST_PASS;
    }
    // End marker
    return 0;
}


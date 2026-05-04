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

//#ifdef SELF_CHECK
   inline __attribute__((always_inline)) void __do_amoswapl_w(uint64_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       ".equ NEW_VALUE, 0xbeefbeef\n"
       "li a0, NEW_VALUE\n"
       "amoswapl.w a1, a0, (%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a0", "a1", "memory"
      );
    } 

    inline __attribute__((always_inline)) void __do_load_w(uint64_t *my_addr, uint32_t *my_result) {
   __asm__ __volatile__
      (
       "lw a1, 0(%[my_addr])\n"
       "sw a1, 0(%[my_result])\n"
       :
       : [my_addr] "r" (my_addr), [my_result] "r" (my_result)
       : "a1", "memory"
      );
    }

//#endif
           
int main()
{
    bool  pass = true;
    uint32_t result[2]  = {      0x00000000,      0x00000000    };
    uint64_t hart_id = getHartId();
    C_TEST_START;
    //todo init A
    //
    //
    if( hart_id  == 0) {
       
       uint64_t *base_addr;
       uint64_t cstart, cend, cdiff;

       // Store some known value: 16-byte data = 0xa1a1_a1a1_a1a1_a1a1;
       for (uint64_t set = 0; set < 16; ++set) {
          base_addr = (uint64_t*) ( 0x8000000000 | set << 6 );
          for (int jj = 0; jj < 16; jj++) {   // let's Store double-word, 16 bytes (out of 64 bytes cline)
            * (base_addr + jj) = 0xa1a1a1a1;    
          }
          FENCE;
       }


       // Evict cache 1, ensure that l1 is clean
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

      //#ifdef SELF_CHECK
      for (uint64_t set = 0; set < 16; ++set) {
          
          base_addr = (uint64_t*) ( 0x8000000000 | set << 6 );
          __do_amoswapl_w(base_addr, result); 
          FENCE;

          // check L2 returned the (dirty) data evicted from L1:
          if (result[0] != 0xa1a1a1a1) {   // should be the old (dirty) data from L2 (after evict L1->L2)
            et_printf("Test self-check: expect (evicted) data in L2: mismatch\n");
            pass = false;
          } 

          __do_load_w(base_addr, result+1); 

          if (result[1] != 0xbeefbeef) {   // should be the new data from L2 (after amoswapl.w )
            et_printf("Test self-check: expect L1 was invalidated: mismatch\n");
            pass = false;
          } 

          if (pass == false) {                    
            C_TEST_FAIL;                        
            break;
          }
       }
      // #endif //SELF_CHECK
    


       cstart = et_get_cycle_count();
       // Evict cache 2, take real time
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

    if (pass == false) {
       C_TEST_FAIL;
    } else {
       C_TEST_PASS;
    }
    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of L1D cache warm-up followed by
//               random lock_va/unlock_va sequences.
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"

#ifdef MINSHIRE_GATESIM_ENABLE
   #define TIMEOUT 10000
   #ifndef NUM_ITER
   #define NUM_ITER 1
   #endif
#else
   #ifndef NUM_ITER
   #define NUM_ITER 1
   #endif
#endif


// (CYE)
#ifdef SELF_CHECK

#define SIZE 4096
#define ALIGN_DATA 4096 
#define ALIGN_INDEX 16

static uint64_t buffer[SIZE]  __attribute__ ((aligned (ALIGN_DATA)));


inline __attribute__((always_inline)) void __do_klv_exp(uint64_t *my_addr) {
    __asm__ __volatile__
        (


         // store freg to memory (packed single)
         // Goes to L1
         "fsw.ps f1, (%[my_addr])\n"

         // repeat 10 times
         "li t5,10\n"
         "repeat_a:\n"

         // 32 bit vector store to local memory (atomic)
         // half line partial writes 
         "fswl.ps f2, (%[my_addr])\n"
         "fswl.ps f3, (%[my_addr])\n"
         "fswl.ps f4, (%[my_addr])\n"
         "fswl.ps f5, (%[my_addr])\n"
         "addi t5, t5, -1\n"
         "bne t5, zero, repeat_a\n"

         // 32 bit vector scatter to local memory (atomic)
         // x8 32 bit partial writes.  
         "fscwl.ps f6, f0 (%[my_addr])\n"

         // 16 bit integer store to local memory (atomic)
         // half-word partial write.
         "shl      t0, (%[my_addr])\n"

         // 32 bit vector store to global memory (atomic)
         // half line write forward
         "fswg.ps f16, (%[my_addr])\n"
         "fswg.ps f17, (%[my_addr])\n"
         "fswg.ps f18, (%[my_addr])\n"
         "fswg.ps f19, (%[my_addr])\n"
    
         // 32 bit vector scatter to global memory (atomic)
         // x8 32 bit partial write forwards.  
         "fscwg.ps f20, f0 (%[my_addr])\n"

         // 16 bit integer store to global memory (atomic)
         // half word parial write forward
         "shg      t0,  (%[my_addr])\n"

         // store word to L1 cache
         "sw t0, 0 (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr)
         : "t0", "memory"
        );
}

inline __attribute__((always_inline)) void __do_klv_exp2(uint64_t *my_addr) {
    __asm__ __volatile__
        (
         "li      t0, 0x5a\n"
         "sw t0, 0 (%[my_addr])\n"
         :
         : [my_addr] "r" (my_addr)
         : "t0", "memory"
        );
}

#endif // ifdef SELF_CHECK


int main()
{
   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();
   //uint64_t shire_id  = ((hart_id>>6) & 0x3F);
   uint64_t minion_id  = ((hart_id>>1) & 0x1F);

   if( ((hart_id & 1) == 0 ) && (minion_id == 0)  ) {
      setM0MaskFF();
      initFRegs();
      uint64_t ii;
      for (ii = 0; ii < NUM_ITER; ++ii) {
         uint64_t *base_addr;
         base_addr = buffer;

        // L1 stores and evicts
        uint64_t jj;
	uint64_t addr1 = (uint64_t) buffer;	
        for (jj = 0; jj < 8; ++jj) {
          __do_klv_exp2(base_addr);
          evict_va(0, 1, addr1); 
          WAIT_CACHEOPS;
        }

        // PS stores
         __do_klv_exp(base_addr);

	uint64_t reg_stride = 0;
	uint64_t start_reg = 0;
	uint64_t row_size = 3;
	uint64_t num_rows = 15;
	uint64_t coop_minions = 0;
	uint64_t addr = (uint64_t) buffer;	
	uint64_t stride = 0x40;
	
        // tensor stores
	tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);

      }
   }


   if (pass == false) {
      C_TEST_FAIL;
   } else {
      C_TEST_PASS;
   }
   return 0;
}


/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "et_test_common.h"
#include "cacheops.h"

#ifdef __cplusplus
#endif

// operands: new data for F_Atomic_Swap:
static uint32_t operands[32]  __attribute__ ((aligned (64)))= {
   0xff800000, //-Inf
   0xc1a4cccd, //-20.6
   0x806ce3ee, // -1.0E-38-0
   0x80000000, // -zero
   0x0,        // zero
   0x006ce3ee, //1.0E-38
   0x40000000, //2
   0x7f800000, // +Inf
   0x7f800002, // signaling NaN
   0xff800002, // -signaling NaN
   0x7fc00001, //NANQ
   0xffc00001, //-NANQ
   0x1edcba98,
   0x12345678,
   0x12345678,
   0x1edcba98,
   0x69abcdef,
   0x38765432,
   0x7543654a,
   0x3c342985,
   0x15634579,
   0x78967365,
   0x88888888,
   0x88888888,
   0x77777777,
   0x77777777,
   0x44444444,
   0x44444444,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98
};

// original mem content: 
static uint32_t base_addr[32]  __attribute__ ((aligned (64)))= {
   0xfedcba00,
   0xfedcba01,
   0xfedcba02,
   0xfedcba03,
   0xfedcba04,
   0xfedcba05,
   0xfedcba06,
   0xfedcba07,
   0xfedcba08,
   0xfedcba09,
   0xfedcba0a,
   0xfedcba0b,
   0xfedcba0c,
   0xfedcba0d,
   0xfedcba0e,
   0xfedcba0f,
   0xfedcba10,
   0xfedcba11,
   0xfedcba12,
   0xfedcba13,
   0xfedcba14,
   0xfedcba15,
   0xfedcba16,
   0xfedcba17,
   0xfedcba18,
   0xfedcba19,
   0xfedcba1a,
   0xfedcba1b,
   0xfedcba1c,
   0xfedcba1d,
   0xfedcba1e,
   0xfedcba1f
};

// addr offset: for each 32-bit word (total 8 words):
static uint32_t addr_offset[32]  __attribute__ ((aligned (16)))= {
   0x0,
   0x4,
   0x8,
   0xc,
   0x10,
   0x14,
   0x18,
   0x1c,
};

/***
static uint32_t result[32]  __attribute__ ((aligned (64)))= {
   0x0,
   0x0,
   0x0,
   0x0,
   0x0,
   0x0,
   0x0,
   0x0
};
***/


inline __attribute__((always_inline)) void __famoswapg_pi(uint32_t *base_addr, uint32_t *addr_offset, uint32_t *operands, uint32_t *result) {
    __asm__ __volatile__
        (
         "li x11, 0xff\n"
         "mova.m.x x11\n"
         "flwg.ps f0, 0(%[operands])\n"
         "flwg.ps f1, 0(%[addr_offset])\n"
         "addi t0, %[base_addr], 0\n"
         "famoswapg.pi f0,f1(t0)\n"
         "flwg.ps f0, 0(t0)\n"
         "fswg.ps f0, 0(%[result]) \n"
         :
         : [operands] "r" (operands), [addr_offset] "r" (addr_offset), [base_addr] "r" (base_addr), [result] "r" (result)
         : "t0", "f0", "f1", "memory"
        );
}

inline __attribute__((always_inline)) long getHartId() {
    long  thread;
    __asm__ __volatile__
        (
         "  csrr %[tid], mhartid\n" // software thread
         : [tid] "=r" (thread)
         :
         :
        );
    return thread;
}

int main()
{
    uint32_t result[32]  __attribute__ ((aligned (64)));  // must align to 64-byte (cacheline-size)
    uint64_t hart_id = getHartId();
    bool pass = true;

    C_TEST_START;

/**** // no longer needed; UST flow set FLUSH_TO_L3=True for this test; safer to expend to multi Harts;
    #ifdef POSTSI_UST
      // need to Evict the cachline address (%[base_addr]) L1 -> L3 (which invalidates L1 & L2):
      const int to_l3 = 0x2;
      const int flush_lines = 2; // famoswapg.pi operation could cross the cache line boundary, so we will flush two cache lines just in case
      const uint64_t addr = (uint64_t)base_addr;
      //       use_tmask,   dst, addr,     num_lines, stride, id, warl
      flush_va(        0, to_l3, addr, flush_lines-1,     64,  0,    0);
      WAIT_CACHEOPS;
    #endif
****/

    for(int k = 0; k < 4; k++) {
       __famoswapg_pi(base_addr+k*8, addr_offset, operands+k*8, result+k*8);
       FENCE;

       for(int i = 0; i < 8; i++) {
          if(result[k*8+i] != operands[k*8+i]) {
             //et_printf_long("Test self-check: Mismatch (k=%d i=%d)\n", k, i);
             pass = false;
             break;
          }
       }
       if(pass == false) {
          et_printf_long("Failed: hart_id=%d k=%d\n", hart_id, k);
          C_TEST_FAIL;
       } 
       //else {
       //   et_printf_long("Pass: hart_id=%d k=%d\n", hart_id, k);
       //}

       // clean up for next loop iteration:
       // evict result[] data out of L1$ (to L3): for 2 cachelines; 
       FENCE;
       evict_va(false, 0x2, (uint64_t)(void*)(result), 1, 0x40, 0, 0);
       WAIT_CACHEOPS;
       FENCE; 

    } // for(int k = 0; ...)

    C_TEST_PASS;
    // End marker
    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include <cstdint>
#include <cstddef>
#include "minion.h"
#include "et_test_common.h"

constexpr size_t OPS_SIZE = 32;

static uint32_t operands[OPS_SIZE]  __attribute__ ((aligned (32)))= {
    0x00000000,           
    0x00000001,          
    0x00000002,           
    0x00000003,           
    0x00000004,
    0x00000005,
    0x00000006,
    0x00000007,
    0x00000008,                 
    0x00000009,                
    0x0000000a,                   
    0x0000000b,                 
    0x0000000c,
    0x0000000d,
    0x0000000e,
    0x0000000f,
    0x00000010,                
    0x00000012,                 
    0x00000013,                
    0x00000014,   
    0x00000015,
    0x00000016,
    0x00000017,
    0x00000018,
    0x00000019,
      0x0000001a,
      0x0000001b,
      0x0000001c,
      0x0000001d,
      0x0000001e,
      0x0000001f,
      0x00000020 
};


static uint32_t exp_result[OPS_SIZE]  __attribute__ ((aligned (32)))= {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};

static uint32_t rtl_result[OPS_SIZE]  __attribute__ ((aligned (32)))= {
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
};

template<typename T>
T amoaddl_w(T * ptr, T val)
{
    T out;
    asm volatile
        ("amoaddl.w %[out], %[val], (%[ptr])"
         : [out] "=r" (out), [ptr] "+r" (ptr) // Cant use "+m". Compiler does " 48(t0) where (t0) is the only valid syntax
         : [val] "r" (val)
         : "memory");
    return out;
}

int main()
{
   C_TEST_START;

   // (CYE) only run the following code in hart_id==0 (in order to do self-check):
   uint64_t hart_id = get_hart_id();

   if ((hart_id % 2) != 0) C_TEST_PASS;

   #pragma GCC unroll OPS_SIZE
   for (size_t elem = 0 ; elem < OPS_SIZE; ++elem)
   {
       uint32_t r = 30999*elem + 1234*elem;
       const uint32_t zero = 0;
       auto res = amoaddl_w(&operands[elem], r);
       exp_result[elem] = res + r;
       res = amoaddl_w(&operands[elem], zero);
       rtl_result[elem] = res;
       if (rtl_result[elem] != exp_result[elem]) {
           et_printf("Test self-check: new value in L2: mismatch\n"); 
           C_TEST_FAIL;
       }
       FENCE;
       evict_va(false, 0x1, (uint64_t)(void*)operands, 0, 0, 0, 0);
       WAIT_CACHEOPS;
       FENCE;
   }

   C_TEST_PASS;
   return 0;
}

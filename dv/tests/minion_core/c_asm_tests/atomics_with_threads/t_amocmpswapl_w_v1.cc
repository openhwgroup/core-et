/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "et_test_common.h"

#ifdef __cplusplus
#endif

#define SIZE 32
#define ALIGN_DATA  32
#define ALIGN_INDEX 16

static uint32_t operands[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0xff800000,//-Inf
   0xc1a4cccd, //-20.6
   0x806ce3ee, // -1.0E-38-0
   0x80000000, // -zero
   0x0,  // zero
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
   0x44444444,
   0x44444444,
   0x44444444,
   0x44444444
};

/*
static uint32_t base_addr[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98,
   0xfedcba98
};
*/

static uint32_t base_addr[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
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




inline __attribute__((always_inline)) void __before_amocmpswapl_w(uint32_t *operands, uint32_t *base_addr) {
    __asm__ __volatile__
        (
         "lw t1, 0(%[operands])\n"
         // note: special: x31 is the expected value for amocmpswap to compare against to;
         //"amoorl.w x31, x0, (%[base_addr])\n" // Read the value with AMOOR to avoid bringing the line into the L1D$
         // Charles: testing: change to regular load:
         "lw x31, 0(%[base_addr])\n"
         :
         : [operands] "r" (operands), [base_addr] "r" (base_addr)
         : "t1", "x31", "memory"
        );
}

inline __attribute__((always_inline)) void __do_amocmpswapl_w(uint32_t *base_addr) {
    __asm__ __volatile__
        (
         //"add x31, x31, x0\n"   // (to be activated) workaround to an RTL bug (see VERIF-3948 and related email)           
         "amocmpswapl.w t2, t1, (%[base_addr])\n"   // note: it will compare the old data (in mem) against x31 value
         :
         : [base_addr] "r" (base_addr)
         : "x31", "t1", "t2", "memory"
        );
}

inline __attribute__((always_inline)) void __after_amocmpswapl_w(uint32_t *result, uint32_t *base_addr) {
    __asm__ __volatile__
        (
         "amoorl.w t3, x0, (%[base_addr])\n" // Read the value with AMOOR to avoid bringing the line into the L1D$
         "sw t3, 0(%[result]) \n"
         :
         : [base_addr] "r" (base_addr), [result] "r" (result)
         : "t3", "memory"
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

    // The test tries to write a value ('operands') to a new memory position ('base_addr') using the 'amocmpswapl.w' operation
    // Although multiple threads write into the same memory position using the 'amocmpswapl.w' operation, this test is thread safe
    // All the threads loads the default value in 'base_addr' to X31 using the 'amoorl.w' to avoid bring the line to the L1D$.
    // This is to force the 'amocmpswapl.w' to swap the value in memory, as what we load into the X31 is the actual value we will compare to.
    // If another thread already swapped the value using the 'amocmpswapl.w' between your load to X31 and your 'amocmpswapl.w',
    // the 'amocmpswapl.w' will not swap the value but this is fine as the self-check intent is to check that at least one HART swapped
    // the value using the 'amocmpswapl.w' operation. The self-check explicitly reads the value in 'base_addr' and checks
    // if it matches the reference one ('operands')

    uint64_t hart_id = getHartId();
    uint32_t result[SIZE];

    C_TEST_START;
    if((hart_id%NUM_HARTS_PER_SHIRE) == 0) delay(et_get_rand_word(0, 100));
    for(int i = 0;i < SIZE;i++) {
      __before_amocmpswapl_w(operands + i, base_addr + i);
      __do_amocmpswapl_w(base_addr + i);
      __after_amocmpswapl_w(result + i, base_addr + i);
      if(result[i] != operands[i]) {
        et_printf_long("H%d Test self-check: Mismatch\n", hart_id);
        C_TEST_FAIL;
        break;
      }
      //else {
      //  et_printf_long("H%d Test self-check: Match\n", hart_id);
      //}
    }
    C_TEST_PASS;

    return 0;
}

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "et_test_common.h"

#ifdef __cplusplus
#endif

#define SIZE 32
#define ALIGN_DATA  32
#define ALIGN_INDEX 16
#define TIMEOUT 500

static uint64_t operands[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
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

static uint64_t base_addr[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
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


inline __attribute__((always_inline)) void __before_amocmpswapl_d(uint64_t *operands, uint64_t* base_addr) {
    __asm__ __volatile__
        (
         "ld t1, 0(%[operands])\n"
         // note: special: x31 is the expected value for amocmpswap to compare against to;
         "amoorl.d x31, x0, (%[base_addr])\n" // Read the value with AMOOR to avoid bringing the line into the L1D$
         :
         : [operands] "r" (operands), [base_addr] "r" (base_addr)
         : "t1", "x31", "memory"
        );
}

inline __attribute__((always_inline)) void __do_amocmpswapl_d(uint64_t* base_addr) {
    __asm__ __volatile__
        (
         "add x31, x31, x0\n"   // workaround to an RTL bug (see VERIF-3948 and RTLMIN-6469)           
         "amocmpswapl.d t2, t1, (%[base_addr])\n"   // note: it will compare the old data (in mem) against x31 value
         :
         : [base_addr] "r" (base_addr)
         : "t1", "t2", "memory"
        );
}

inline __attribute__((always_inline)) void __after_amocmpswapl_d(uint64_t *result, uint64_t* base_addr) {
    __asm__ __volatile__
        (
         "amoorl.d t3, x0, (%[base_addr])\n" // Read the value with AMOOR to avoid bringing the line into the L1D$
         "sd t3, 0(%[result]) \n"
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
    uint64_t hart_id = getHartId();
    uint64_t result[SIZE];
    bool pass = true;
    #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t timeout = TIMEOUT;
    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
      s32_scp_init();
      give_credit(0, 0, 0, 0);
    }
    if (hart_id == 0) {
      wait_for_credit(0);
    }
    #endif

    C_TEST_START;
    #ifdef MINSHIRE_GATESIM_ENABLE
    if (hart_id == 0) {  
    #endif
    if((hart_id%NUM_HARTS_PER_SHIRE) == 0) delay(et_get_rand_word(0, 100));
    for(int i = 0;i < SIZE;i++) {
      __before_amocmpswapl_d(operands + i, base_addr + i);
      __do_amocmpswapl_d(base_addr + i);
      __after_amocmpswapl_d(result + i, base_addr + i);
      if(result[i] != operands[i]) {
        et_printf_long("H%d Test self-check: Mismatch\n", hart_id);
        pass = false;
        break;
      }
    }
    if(pass) {
      #ifdef MINSHIRE_GATESIM_ENABLE
      if (hart_id == 0) {
        atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR);
      }
      #endif
      C_TEST_PASS;
    }else {
      #ifdef MINSHIRE_GATESIM_ENABLE
      if (hart_id == 0) {
        atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR);
      }
      #endif
      C_TEST_FAIL;
    }
    #ifdef MINSHIRE_GATESIM_ENABLE
    }
    else {  // for other harts: 
      C_TEST_PASS;
    }
    #endif

    #ifdef MINSHIRE_GATESIM_ENABLE
    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
      check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR);
    }
    C_TEST_PASS
    #endif

    return 0;
}

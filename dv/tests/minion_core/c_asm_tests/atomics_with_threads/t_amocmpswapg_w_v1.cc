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

static uint32_t result[SIZE]  __attribute__ ((aligned (ALIGN_DATA)))= {
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

void evict() {
   // Evict cache: must from L1->L3:
   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x2, 0, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x2, 1, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x2, 2, set,   0       );
   }
   WAIT_CACHEOPS;

   for (unsigned set = 0; set < 16; ++set) {
      evict_sw(false,     0x2, 3, set,   0       );
   }
   WAIT_CACHEOPS;
}

inline __attribute__((always_inline)) void __before_amocmpswapg_w(uint32_t *operands) {
    __asm__ __volatile__
        (
         "lw x1, 0(%[operands])\n"
         //"lw x31, 0(%[base_addr])\n"    // note: x31 will be used by evict_va 
         "lw x4, 0(%[base_addr])\n"
         :
         : [operands] "r" (operands), [base_addr] "r" (base_addr)
         : "x1", "x31", "memory"
        );
}

inline __attribute__((always_inline)) void __do_amocmpswapg_w() {
    __asm__ __volatile__
        (
         "addi x31, x4, 0\n"
         "amocmpswapg.w t3, x1, (%[base_addr])\n"
         :
         : [base_addr] "r" (base_addr)
         : "x1", "t3", "memory"
        );
}

inline __attribute__((always_inline)) void __after_amocmpswapg_w(uint32_t *result) {
    __asm__ __volatile__
        (
         "lw x3, 0(%[base_addr])\n"
         "sw x3, 0(%[result]) \n"
         :
         : [base_addr] "r" (base_addr), [result] "r" (result)
         : "x3", "memory"
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

void int_to_char(char *dst, int num){
  char tmp_buffer[20];
  int pos = 0;
  int i;

  // Deal with negative values
  if (num < 0) {
    *(dst++)='-';
    num=-num;
  }

  do {
    tmp_buffer[pos] = '0' + num % 10;
    num = num/10;
    pos++;
  } while(num!=0);

  for (i=0;i<pos;i++){
    dst[i]=tmp_buffer[pos-i-1];
  }

  dst[i]='\n';
  dst[i+1]='\0';
}

void print_number (uint64_t num){
    char str[50] = "";
    int_to_char(&str[0], num);
    et_printf(str);
}


// inline void __attribute__((always_inline)) evict_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)


int main()
{
    uint64_t hart_id = getHartId();
    bool pass = true;

    C_TEST_START;
    if(hart_id == 0 || hart_id == 2 || hart_id == 4 || hart_id == 6) {
      if(hart_id == 0) delay(et_get_rand_word(0, 100));

      //for(int i = 0; i < 2; i++) {
      for(int i = 0; i < SIZE; i++) {
        __before_amocmpswapg_w(operands + i);

        // Charles: added evict(): evict all clines from L1D:
        FENCE;
        //evict();
        evict_va(false, 0x2, (uint64_t)(void*)base_addr, 0, 0, 0, 0);  // Problem: would cause Exception. Why? 
        WAIT_CACHEOPS;
        FENCE;

        __do_amocmpswapg_w();

        FENCE;
        //evict();
        //FENCE;

        __after_amocmpswapg_w(result + i);

        if(result[i] != operands[i]) {
          et_printf("Test self-check: Mismatch\n");  //print_number(result[i]); //not working???
          pass = false;
          break;
        }
      }
      if(pass) {
        if(hart_id == 0) et_printf("Pass: hart_id = 0\n");
        if(hart_id == 2) et_printf("Pass: hart_id = 2\n");
        if(hart_id == 4) et_printf("Pass: hart_id = 4\n");
        if(hart_id == 6) et_printf("Pass: hart_id = 6\n");
      }else {
        if(hart_id == 0) et_printf("Fail: hart_id = 0\n");
        if(hart_id == 2) et_printf("Fail: hart_id = 2\n");
        if(hart_id == 4) et_printf("Fail: hart_id = 4\n");
        if(hart_id == 6) et_printf("Fail: hart_id = 6\n");
        C_TEST_FAIL;
      }
    }

    C_TEST_PASS;
    // End marker
    return 0;
}

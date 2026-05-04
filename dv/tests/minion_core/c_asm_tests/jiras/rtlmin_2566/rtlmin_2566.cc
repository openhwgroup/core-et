/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>

// TEST IDEA:
// there are 8 minions:
//   - thread 1 is doing long operations (accesses to L3, div, uncacheds)
//   - thread 0 wait for FCC, do a load/store in L2 in some shared data, and inc FCC for the next minion t0
//
//  all minions have thread1 running, and only minion 0 has thread0 awake ( other minions will be waken up by
//  minion-1)


#define ITERATIONS 100

#define FCC_T0 (0x803400C0 | (0xFF<<22))
#define FCC_T1 (0x803400D0 | (0xFF<<22))

#define INC_FCC_T0(x) *((volatile uint64_t*) FCC_T0) = x
#define INC_FCC_T1(x) *((volatile uint64_t*) FCC_T1) = x
#define WAIT_FCC  __asm__ __volatile__ ( "csrrw x0, 0x821, x0\n")

#define TEST_AREA_WORDS 64

inline uint64_t get_hart_id()
{
    int64_t ret;
    __asm__ __volatile__ ( "csrr %[ret], mhartid\n" : [ret] "=r" (ret));
    return ret;
}

inline uint64_t hint_fcc()
{
    int64_t ret;
    __asm__ __volatile__ ( "csrr %[ret], 0xcc0\n" : [ret] "=r" (ret));
    return ret;
}

uint32_t t1_area[]     __attribute__ ((aligned (64))) =
  {
   0x2ad5156a, 0xab5855a, 0xc2ad6156, 0xb0abd855, 0x6c2a3615, 0x1b0a8d85, 
   0x46c22361, 0x11b088d8, 0xc46ce236, 0x711b388d, 0x9c46ce23, 0x6711b388, 
   0xd9c4ece2, 0xf6717b38, 0x3d9c1ece, 0xf6787b3, 0x43d921ec, 0x90f6487b, 
   0xa43d521e, 0x290f9487, 0x4a43a521, 0x52902948, 0x94a44a52, 0x25299294, 
   0xc94ae4a5, 0xf2527929, 0xbc94de4a, 0xef25f792, 0x7bc93de4, 0x1ef28f79, 
   0xc7bce3de, 0x71ef38f7, 0x9c7bce3d, 0x671e338f, 0x99c74ce3, 0x26711338, 
   0x99c4ce, 0x2678133, 0x4099204c, 0x1026813, 0x84094204, 0xa1025081, 
   0xa8405420, 0xaa105508, 0xaa84d542, 0x6aa13550, 0x1aa8d54, 0x86aa4355, 
   0x21aa10d5, 0x86a435, 0x821ac10d, 0xe086f043, 0xf8217c10, 0x3e089f04, 
   0xcf8267c1, 0xb3e0d9f0, 0xecf8767c, 0xbb3edd9f, 0xeecff767, 0xfbb37dd9, 
   0x3eec9f76, 0x4fbba7dd, 0xd3eee9f7, 0xf4fbfa7d};


volatile uint32_t t0_word[]  __attribute__ ((aligned (64))) =
  { 0xFF55AADE,
    0x12345678,
    0x0,
    0xFFFFFFFF,
    0xDEADBEEF,
    0x01247654,
    0xABCD0123,
    0xF0E1D2C3
  };

    
#ifdef RTLMIN_2598
volatile bool t0_done[32] = {0};
#endif

int main(){
  C_TEST_START;
  
  __asm__ __volatile__
    (
     "li t0, 0xff\n"
     "mova.m.x t0\n"
     : : : "t0");

  uint64_t hart_id = get_hart_id();
  uint64_t minion_id = (hart_id >> 1) & 0x1F; // 0..31 inside the same shire
  uint64_t next_minion_id, fcc_write_mask;

  if (hart_id % 2 == 0) {
    //threads 0
    if (minion_id % 8 == 7 ) next_minion_id = (minion_id & ~0x7ULL); // min 0 of the same neigh
    else next_minion_id = minion_id + 1; //next min in the neigh
    fcc_write_mask = 1<<next_minion_id;

    //if it is minion 7 of the neigh, write FCC to start min0
    if (minion_id % 8 == 7 ) INC_FCC_T0(fcc_write_mask);


    // each minion will do some stores, start the next minion, and wait to be started again
    for ( uint64_t i = 0; i < ITERATIONS; i++) {
      WAIT_FCC;

      __asm__ __volatile__
        (
#ifdef USE_ATOMICS      
         "amoxorl.w t0, %[it], (%[ptr])\n"
#else
         "flwl.ps f0, (%[ptr])\n"
         "fnot.pi f0, f0\n"
         "fswl.ps f0, (%[ptr])\n"
         "fence\n"
#endif
         :
         : [ptr] "r" (&t0_word), [it] "r" (i)
         : "t0", "f0"
         );

      INC_FCC_T0(fcc_write_mask);
    }
    
    //and tell thread 1 to stop once done
    INC_FCC_T1(1<<minion_id);
#ifdef RTLMIN_2598
    t0_done[minion_id] = true;
#endif
  }
  else {
    //threads 1
    int  timeout = ITERATIONS*10;
    while(
#ifdef RTLMIN_2598
          !t0_done[minion_id]
#else
          !hint_fcc()
#endif
          ) {
      
      if (--timeout == 0) {
        C_TEST_FAIL;
      }
      
      //loop and read from t1_area ( because it is using amo in L3, it will take a while to retire)
      __asm__ __volatile__
        (
         "mv t0, %[ptr]\n"
         "li t1, %[SIZE]\n"
         "li t2, 0x803400C0\n"
         // do loads from ESR.. should take a while
         "1:\n"
#ifdef RTLMIN_2679
         "ld t3, 0(t2)\n"
         "ld t3, 0(t2)\n"
         "ld t3, 0(t2)\n"
         "ld t3, 0(t2)\n"
#else
         "ld x0, 0(t2)\n"
         "ld x0, 0(t2)\n"
         "ld x0, 0(t2)\n"
         "ld x0, 0(t2)\n"
#endif
         
         // and make a div which takes ~64 cycles
         "div x0, t1,t1\n"
         // do access to L3.. should take a while longer
#ifdef USE_ATOMICS
         "amoorg.w x0, x0, (t0)\n"
         "addi t0, t0, 4\n"         
         "addi t1, t1, -1\n"
#else
         
         "flwl.ps f0, (t0)\n"
         "addi t0, t0, 32\n"
         "addi t1, t1, -8\n"         
#endif
         "fence\n"
         "bne t1, zero, 1b\n"
         : 
         : [ptr] "r" (t1_area), [SIZE] "i" (TEST_AREA_WORDS)
         : "t0", "t1", "t2", "t3");
    }
  }
  
  
  C_TEST_PASS;
  return 0;
}


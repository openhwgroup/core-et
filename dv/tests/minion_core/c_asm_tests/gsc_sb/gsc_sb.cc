/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "gsc_sb.h"
#include "esr_defines.h"

/*******************************************************************************

*******************************************************************************/

extern uint8_t buffer[128][16][64];


int main()
{
  C_TEST_START;

  if (get_hart_id() % 2 == 0) test_t0();
  else test_t1();

  
    
  C_TEST_PASS;
  return 0;
} 


void test_t0(){
  volatile uint32_t offsets[8];
  uint8_t masks[] = { 0xFF, 0x55, 0x1, 0x80, 0x88, 0x0, 0xAA, 0x12, 0x0f, 0xf0};
  unsigned mask_ptr = 0;
  
  for ( int i = 0; i < ITERATIONS; i++) {
    
    // set mask
    set_mask( masks[mask_ptr++] );
    mask_ptr %= sizeof(masks);

    for ( unsigned int k = 0 ; k < REPEATS; k++ ){
      set_progress(  (k+1) % 8 );      
      // set offsets
      for ( unsigned j = 0 ; j < 8; j++ ) {
        switch ( k & 0x3 ) {
        case 0: offsets[j] = 0; break;           // offsets = 0
        case 1: offsets[j] = j*64; break;        // different lines, but different set
        case 2: offsets[j] = j*64*16; break;     // different lines, but same set
        case 3: offsets[j] = j*3 + j *32; break; //misaligneds
        }
      }
      __asm__ __volatile__ ( "" : : : "memory");
      set_debug(k%8);
      uint8_t *base  __attribute__ ((aligned (8))) = buffer[k % 8][ (REPEATS-1-k) % 16];
      evict_va ( (uintptr_t) offsets);
      __asm__ __volatile__
        (
         "flq2 f0,%[offsets]\n"
         "fgw.ps f1,f0(%[base])\n"
         : : [base] "r" (base),  [offsets] "m" (offsets) : "memory" );
      
    }
  }
  inc_fcc0(1 ^ (get_hart_id() % 64));
}

void test_t1(){
  while( peek_fcc(0) == 0) {
    for ( int i = 0; i < 64; i++) {
      uint8_t *src = &(buffer[i][0][0]);
      uint8_t *dst = &(buffer[i+1][0][0]);
      __asm__ __volatile__
        (
         "flq2 f0, 0(%[src])\n"
         "fsq2 f0, 0(%[dst])\n"
         :
         : [src] "r" (src), [dst] "r" (dst)
         : "memory"
         );
    }
  }
}


void evict_va(uintptr_t ad){
   uint64_t conf =  1ULL<<58 | (ad & (~0x3fULL));
  __asm__ __volatile__ ( "csrw evict_va, %[v]\n" : : [v] "r" (conf)  );
  __asm__ __volatile__ ( "csrwi tensor_wait, 6\n");
}

void set_progress(unsigned p)  {
  __asm__ __volatile__ ( "csrw gsc_progress, %[p]\n" : : [p] "r" (p));
}


void evict_l1(){
   for ( int way=0; way < 4; way ++){
    uint64_t evict =
      (3ULL<<58) | //to mem
      (0<<14) | // starting at set 0
      (way<<6)|
      0xF;   // the whole 16 sets of the way
    __asm__ __volatile__ ( "csrw evict_sw, %[v]\n" : : [v] "r" (evict)  );
  }
   __asm__ __volatile__ ( "csrwi tensor_wait, 6\n");
}

void set_debug(uint64_t d){
  __asm__ __volatile__ ("csrw validation3, %[d]\n" : : [d] "r" (d) );
}

void set_mask(uint8_t m){
  __asm__ __volatile__ ("mova.m.x %[mask]\n": : [mask] "r" (m));
}

void delay(){
  __asm__ __volatile__
    (
     ".rept 1024\n"
     "nop\n"
     ".endr"
     );

}

uint64_t get_hart_id()
{
  uint64_t ret;
  __asm__ __volatile__
    (
     "csrr %[ret], mhartid\n"
     : [ret] "=r" (ret)
     );
  return ret;
}


uint64_t peek_fcc(uint64_t cnt) {
  uint64_t fcc_nb;
  __asm__ __volatile__ ("csrr %[d], fccnb" : [d] "=r" (fcc_nb));
  return (fcc_nb >> (16*cnt)) &0xFFFF;
}

void inc_fcc0(unsigned thread){
  volatile uint64_t* trigger = ( thread % 2 == 0) ?
    (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_0) :
    (volatile uint64_t*) ESR_SHIRE(0xFF, FCC_CREDINC_2);
  *trigger = 1ULL<<(thread>>1);
  __asm__ __volatile__ ("fence");
}

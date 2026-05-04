/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <inttypes.h>
#include "gsc_sb_int.h"
#include "esr_defines.h"


/*******************************************************************************

*******************************************************************************/

extern uint8_t buffer[128][16][64];


int main()
{
  C_TEST_START;
  
  setup_trap_vector();
  
  // Main loop
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

    // (Charles) change to unsigned int for "k":
    for ( unsigned int k = 0 ; k < REPEATS; k++ ){
      set_progress(  (k+1) % 8 );      
      // set offsets
      #pragma GCC unroll 8
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

      //(Charles) temp: avoid using "(~k)":
      uint8_t *base  __attribute__ ((aligned (8))) = buffer[k % 8][(REPEATS-1-k) % 16];

      evict_va ( (uintptr_t) offsets);

      #pragma GCC unroll 64
      for (int j = 0; j < 16; ++j) {
	     __asm__ __volatile__
     	  (
     	   "flq2 f0,%[offsets]\n"
     	   "fgw.ps f1,f0(%[base])\n"
     	   : : [base] "r" (base),  [offsets] "m" (offsets) : "memory" );
      }
      
    }
  }
  inc_fcc0(1 ^ (get_hart_id() % 64));
}

void test_t1(){
  // uint64_t it = 0;
  while( peek_fcc(0) == 0) {
    // Trigger interrupt on thread 0
    uint64_t hart = get_hart_id()-1;
    trigger_interrupt(hart);
    
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

void trigger_interrupt(uint64_t hart) {
  volatile uint64_t * addr = (volatile uint64_t*) ESR_SHIRE(0xFF, IPI_TRIGGER);
  uint64_t mask = 1ULL << hart;
  *addr = mask;
  //FENCE;
}

void clear_interrupt(uint64_t hart) {
  volatile uint64_t * addr = (volatile uint64_t*) ESR_SHIRE(0xFF, IPI_TRIGGER_CLEAR);
  uint64_t mask = 1ULL << hart;
  *addr = mask;
  FENCE;
}

void setup_trap_vector() {
     //configure trap vector
  uint64_t hart_id = get_hart_id();
  uint64_t thread_id = hart_id & 0x1;
       __asm__ __volatile__
           (
             "la t0, test_mtrap_vector\n"
             "csrw mtvec, t0\n"
             : : : "t0");
       
       // Configure interrupts
       if (thread_id == 0) {
	 __asm__ volatile 
	   (
	    "addi x29, x0, 1\n"
	    "slli x29, x29, 3\n"
	    "csrr x30, mie\n"    // Read mie
	    "or x31, x30, x29\n" // Set bit 3 (msi)
	    "csrw mie, x31\n"    // Set mie
	    "csrr x31, mstatus\n"
	    "or x31, x31, x29\n"
	    "csrw mstatus, x31\n" // Set mstatus mie (enable mmode interrupts)
	    :
	    :
	    : "x31", "x30", "x29"
	    );
       }
}

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart)
{
  (void)tval;
  constexpr uint64_t MSW_INTERRUPT_CODE = 3;
  
  bool     interrupt  = cause >> 63;
  uint64_t cause_code = cause & 0xFF;
  
  if (not (interrupt && cause_code == MSW_INTERRUPT_CODE && (hart & 0x1)==0 )) {
    C_TEST_FAIL;
  } else {
    clear_interrupt(hart);
  }
  return epc;
}


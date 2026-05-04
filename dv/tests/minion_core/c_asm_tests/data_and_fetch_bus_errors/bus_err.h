/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _BUS_ERR_H_
#define _BUS_ERR_H_

#include "esr_defines.h"


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define MBUSADDR_CSR_IN_BEMU       // comment if the BEMU doesn't have mbusaddr csr implemented
#define DATA_AND_FETCH_ERRORS      // comment to have bus errors mainly from fetch, not in data
#define REPLACE_END_OF_RANGE_FN    // comment to disable replacing the function at the boundary of a valid address
#define LOAD_AND_STORE_DATA_ERRORS //comment to have only stores (arch monitor doesn't handle properly bus errors)

#define CAUSE_FETCH_BUS_ERROR 0x19
#define CAUSE_MACHINE_ECALL 0xB
#define CAUSE_BUS_ERR ((1ULL<<63) | 23)

#define ITERATIONS 500

#define BUF_SIZE (1<<12)

#ifdef DATA_AND_FETCH_ERRORS
// regular testing (data errors, fetch errors.. in different forms)
#define GOOD_PTR_PROB 4 // for data operations, use good base ptr 1 out of 4 times
#define IRQ_NOT_ENABLED_PROB 8 // disable bus err probabilty 1 out of 8 times
#define DATA_NO_REPEAT_PROB 4 // repeat load/store in a loop 
#define DATA_MAX_REPEAT 8
#define DATA_BOUNDARY_PROB 8

#else
// only fetch errors
#define GOOD_PTR_PROB 1 // for data operations, use good base ptr 1 out of 4 times
#define IRQ_NOT_ENABLED_PROB 8 // disable bus err probabilty 1 out of 8 times
#define DATA_NO_REPEAT_PROB 4 // repeat load/store in a loop 
#define DATA_MAX_REPEAT 8
#define DATA_BOUNDARY_PROB 8

#endif

#define DATA_BUS_ERROR_ADDRESS_BASE  0x8800000000ULL
#define FETCH_BUS_ERROR_ADDRESS_BASE 0x8C00012000ULL
#define FETCH_ERR_BOUNDARY_ADDRESS 0x87fffffe00ULL // must be the same as the .end_of_dram section address (in the Makefile)


#define END_OF(fn) void end_of_##fn(){}
#define FN_SIZE(fn) ((uintptr_t) &end_of_##fn - (uintptr_t) &fn)

inline unsigned int get_hart_id() __attribute__((always_inline));
uint8_t rand_byte(unsigned &ptr) __attribute__((noinline));

// functions in the boundary of the end of the DRAM (note, no symbols can be used)
// all will be copied in the last 512B of the DRAM
#define FETCH_ERR_BOUNDARY_FN_DECL(name)                            \
  void name() __attribute__ ((naked)) __attribute__ ((naked)) __attribute__ ((noinline)) __attribute__((no_reorder)) ; \
  void end_of_##name() __attribute__((no_reorder)) 

FETCH_ERR_BOUNDARY_FN_DECL(nop_slide);
FETCH_ERR_BOUNDARY_FN_DECL(jump_back);
FETCH_ERR_BOUNDARY_FN_DECL(jump_back2);
FETCH_ERR_BOUNDARY_FN_DECL(split);
FETCH_ERR_BOUNDARY_FN_DECL(long1);
FETCH_ERR_BOUNDARY_FN_DECL(long2);
FETCH_ERR_BOUNDARY_FN_DECL(long3);



void copy_function(void (*src)(), unsigned n);
void evict_l1();

EXTERN_C void  __attribute__ ((interrupt)) __attribute__((aligned(4096)))  mtrap_handler();

#define LOAD(insn) for ( unsigned i = 0; i < repeat; i++)  {          \
    DEBUG(debug | (i << 16));                                         \
    __asm__ volatile (insn " x0, %[d]\n" : : [d] "m" (base_ptr[i]));  \
  }

#define _STORE(insn, payload)                                           \
  do {                                                                  \
    uint8_t saved[32];                                                  \
    if (in_boundary) {                                                  \
      /*modifying a zone where other minions might execture from */     \
    uint32_t *cond[] = { &copying, &executing };                        \
    uint32_t exp_cond[] = {0, 0};                                       \
    wait_ncond(cond, exp_cond, 2);                                      \
    amo_swap(&copying, 1);                                              \
    __asm__ __volatile__                                                \
        (                                                               \
         "li t0, %[ad]\n"                                               \
         "flq2 f0, 0(t0)\n"                                             \
         "fsq2 f0, %[saved]\n"                                          \
         :                                                              \
         : [ad] "i" (DATA_BUS_ERROR_ADDRESS_BASE-32),                   \
           [saved] "m" (saved)                                          \
         : "memory", "t0"                                               \
        );                                                              \
      unlock_mutex();                                                   \
    }                                                                   \
    for ( unsigned i = 0; i < repeat; i++)  {                           \
      DEBUG(debug | (i << 16));                                         \
      __asm__ volatile (insn " " payload "\n"                           \
                        : : [d] "r" (&(base_ptr[i])) : "memory");       \
    }                                                                   \
    if (in_boundary) {                                                  \
      __asm__ __volatile__                                              \
        (                                                               \
         "li t0, %[ad]\n"                                               \
         "flq2 f0, %[saved]\n"                                          \
         "fsq2 f0, 0(t0)\n"                                             \
         :                                                              \
         : [ad] "i" (DATA_BUS_ERROR_ADDRESS_BASE-32),                   \
           [saved] "m" (saved)                                          \
         : "memory", "t0"                                               \
        );                                                              \
      evict_l1();                                                       \
      lock_mutex();                                                     \
      amo_swap(&copying, 0);                                            \
      unlock_mutex();                                                   \
    }                                                                   \
  } while(0);


#define STORE(insn) _STORE(insn, "x0, 0(%[d])")

#define FLOAD(insn) for ( unsigned i = 0; i < repeat; i++) {   \
    DEBUG(debug | (i << 16));                                        \
    __asm__ volatile (insn " f0, %[d]\n" : : [d] "m" (base_ptr[i])); \
  }

#define FSTORE(insn) _STORE(insn,  "f0, 0(%[d])")

#define GATHER(insn) for ( unsigned i = 0; i < repeat; i++) {           \
    DEBUG(debug | (i << 16));                                           \
  __asm__ volatile (insn " f0, f1(%[d])\n" : : [d] "r" ( &(base_ptr[i]))); \
}

#define SCATTER(insn) _STORE(insn, "f0, f1(%[d])")


#define BAD_ESR(insn) for ( unsigned i = 0; i < repeat; i++) {  \
    DEBUG(debug | (i << 16));                                   \
    __asm__ volatile (insn " x0, %[d]\n" : : [d] "m" (bad_esr[i])); \
  }

#define DEBUG(x) __asm__ __volatile__ ("csrw validation3, %[d]\n" : : [d] "r" (x) )



// for synchronization
void lock_mutex();
void unlock_mutex();
void wait_cond(uint32_t *w, uint32_t e, bool keep_lock = true);
void wait_ncond(uint32_t **w, uint32_t *e, uint32_t count, bool keep_lock = true);
uint32_t amo_swap(uint32_t *m, uint32_t s);
uint32_t amo_add(uint32_t *m, uint32_t s);
uint32_t amo_or(uint32_t *m, uint32_t s);
uint32_t amo_cas(uint32_t *m, uint32_t old_data, uint32_t new_data);

#endif

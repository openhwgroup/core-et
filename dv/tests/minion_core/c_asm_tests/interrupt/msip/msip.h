/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _MSIP_H_
#define _MSIP_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif


#define ITERATIONS 100
#define EXP_CAUSE ((1ULL<<63) | 3)

#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))
#define CREDIT_INC_AD   0x803400D0

#define TRIGGER_MSIP do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 1; } while(0)
#define CLEAR_MSIP   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1; } while(0)
#define CRED_INC     do { *((volatile uint64_t*) CREDIT_INC_AD   ) = 1; } while(0)

EXTERN_C void  __attribute__ ((interrupt)) irq_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
EXTERN_C void mtrap_vector();
inline unsigned int get_hart_id() __attribute__((always_inline));
void test_t0();
void test_t1();
inline void dump_irq_count(int cnt);
inline void dump_wait_type(int t);
inline void dump_wait_cnt(int c);




#endif

/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _FCC_OVERFLOW_H_
#define _FCC_OVERFLOW_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 4
#define EXP_CAUSE ((1ULL<<63) | 3)

#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))
#define CREDIT_INC_T0C0_AD   0x803400C0
#define CREDIT_INC_T0C1_AD   0x803400C8
#define CREDIT_INC_T1C0_AD   0x803400D0
#define CREDIT_INC_T1C1_AD   0x803400D8

#define TRIGGER_MSIP_T0 do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 1; } while(0)
#define CLEAR_MSIP_T0   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1; } while(0)
#define TRIGGER_MSIP_T1 do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 2; } while(0)
#define CLEAR_MSIP_T1   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 2; } while(0)
#define CRED_INC_T0_C0 do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD   ) = 1; } while(0)
#define CRED_INC_T0_C1 do { *((volatile uint64_t*) CREDIT_INC_T0C1_AD   ) = 1; } while(0)
#define CRED_INC_T1_C0 do { *((volatile uint64_t*) CREDIT_INC_T1C0_AD   ) = 1; } while(0)
#define CRED_INC_T1_C1 do { *((volatile uint64_t*) CREDIT_INC_T1C1_AD   ) = 1; } while(0)
#define CRED_INC_ALL_BUT_M0_EVEN do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD   ) = 0xFFFFFFFE; } while(0)
#define CRED_INC_ALL_BUT_M0_ODD do { *((volatile uint64_t*) CREDIT_INC_T1C0_AD   ) = 0XFFFFFFFE; } while(0)

#define MAX_CREDITS 0xFFFF
//#define CREDIT_PER_THREAD 1041 = 52 * 20 
#define CREDIT_LOOP 52
#define TERROR_FCC_OVERFLOW 0x8
#define MINION_PER_SHIRE 64

EXTERN_C void  __attribute__ ((interrupt)) irq_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();
EXTERN_C void mtrap_vector();
inline unsigned int get_hart_id() __attribute__((always_inline));
void test_t0();
void test_t1();
void test_others();
void inc_cnt(int cnt);
void produce_fcc_overflow(int cnt);
void receive_fcc_overflow(int sync_t);
void enable_interrupts();
inline void dump_event_type(int t);




#endif

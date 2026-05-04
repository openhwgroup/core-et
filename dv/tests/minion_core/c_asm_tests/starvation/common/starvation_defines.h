/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _STARVATION_DEFINES_H_
#define _STARVATION_DEFINES_H_

#include "macros.h"
#include "minion.h"
//#include "et_test_common.h"
#include "et_postsi_common.h"
#include <inttypes.h>
#include <utility>
#include <array>

#ifdef POSTSI_FC
#define EXPECTED_CYCLES 12000 //long enough to get timeout if a thread does not retire instructions (plusarg timeout is 3000)
#else
#define EXPECTED_CYCLES 8000 //long enough to get timeout if a thread does not retire instructions (plusarg timeout is 3000)
#endif

////////////////////////////////////////////////////////////////////////////////
// other constants and macros
////////////////////////////////////////////////////////////////////////////////
#define CONTROL_HART 0
#define UNUSED_HART 1
#define THREADS_PER_NEIGH 16


#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))
#define CREDIT_INC_AD_T0    0x803400C0
#define CREDIT_INC_AD_T1    0x803400D0
#define CREDIT_INC_AD_END   0x803400C8

#define CLEAR_MSIP(id)  do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1ull<<id; } while(0)

////////////////////////////////////////////////////////////////////////////////
// macros for function attributes
////////////////////////////////////////////////////////////////////////////////
#define NO_INLINE  __attribute__ ((noinline))   __attribute__ ((aligned (1<<6)))
#define ALWAYS_INLINE  __attribute__((always_inline)) inline
#define NAKED __attribute__ ((naked))
#define ALIGNED __attribute__ ((aligned (64))) 
////////////////////////////////////////////////////////////////////////////////
// macros for inline assembly
////////////////////////////////////////////////////////////////////////////////
#define TEST_START "la t0, 999f\ncsrw mscratch, t0\ncsrw fcc, x0\nli x31, 0\n"
#define TEST_END "999: add %[cnt], x31, x0\n"
#define TEST_REG_MODS "t0"


using run_func_t = void (*) (unsigned int);
using iter_func_t = void (*)();
#endif

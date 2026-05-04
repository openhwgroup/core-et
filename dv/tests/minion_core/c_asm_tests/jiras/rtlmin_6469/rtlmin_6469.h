/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _RTLMIN_6469_
#define _RTLMIN_6469_

#define INLINE  __attribute__ ((always_inline))
#define CL_ALIGN __attribute__ ((aligned(64)))


// constants to select how x31 is written. These constants need to match
// the .equs defined in rtlmin_6469.S

static constexpr size_t X31_ALU = 0;            // x31 from an alu instruction
static constexpr size_t X31_DCACHE_MISS = 1;    // x31 from a dcache load, with miss
static constexpr size_t X31_DCACHE_HIT = 2;     // x31 from a dcache load, with hit
static constexpr size_t X31_DIV_MUL = 3;        // x31 from the mul/div output
static constexpr size_t X31_FP = 4;             // x31 from the VPU
static constexpr size_t X31_DCACHE_MISS_WA = 5; // failing case: adding workaround
static constexpr size_t X31_DIV_MUL_WA = 6;     // failing case: adding workaround
#ifdef TEST_WORKAROUND
static constexpr size_t X31_NR_SRCS = 7;
#else
static constexpr size_t X31_NR_SRCS = 5;
#endif


#endif

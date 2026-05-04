/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PSHIRE_ESR_
#define _PSHIRE_ESR_

#include "esr.h"


inline volatile uint64_t* __attribute__((always_inline)) pshire_esr_address(uint64_t address)
{
    volatile uint64_t *p = (uint64_t *) ( address);
    return p;
}

inline uint64_t __attribute__((always_inline)) read_pshire_esr(uint64_t address)
{
    volatile uint64_t *p = pshire_esr_address( address);
    return *p;
}

inline void __attribute__((always_inline)) write_pshire_esr(uint64_t address, uint64_t value)
{
    volatile uint64_t *p = pshire_esr_address( address);
    *p = value;
}


#endif

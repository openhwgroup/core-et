/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MEM_ESR_
#define _MEM_ESR_

#include "esr.h"

#define MEM_SHIRE_ESR 0
#define DDRC_ESR 1

inline volatile uint64_t* __attribute__((always_inline)) mem_esr_address(esr_protection_t pp, uint8_t shire_id, uint8_t d, uint32_t address)
{
    volatile uint64_t *p = (uint64_t *) (  ESR_MEMORY_REGION
                         | ((uint64_t)(pp       & 0x03) << 30)
                         | ((uint64_t)(shire_id & 0xff) << 22)
                         | ((uint64_t)(d        & 0x01) <<  9)
                         | ((uint64_t)(address  & 0x3f) <<  3));
    return p;
}


inline uint64_t __attribute__((always_inline)) read_memshire_esr(esr_protection_t pp, uint8_t shire_id, uint32_t address)
{
    volatile uint64_t *p = mem_esr_address(pp, shire_id, MEM_SHIRE_ESR, address);
    return *p;
}

inline void __attribute__((always_inline)) write_memshire_esr(esr_protection_t pp, uint8_t shire_id, uint32_t address, uint64_t value)
{
    volatile uint64_t *p = mem_esr_address(pp, shire_id, MEM_SHIRE_ESR, address);
    *p = value;
}

inline uint64_t __attribute__((always_inline)) read_ddrc_esr(esr_protection_t pp, uint8_t shire_id, uint32_t address)
{
    volatile uint64_t *p = mem_esr_address(pp, shire_id, DDRC_ESR, address);
    return *p;
}

inline void __attribute__((always_inline)) write_ddrc_esr(esr_protection_t pp, uint8_t shire_id, uint32_t address, uint64_t value)
{
    volatile uint64_t *p = mem_esr_address(pp, shire_id, DDRC_ESR, address);
    *p = value;
}
#endif

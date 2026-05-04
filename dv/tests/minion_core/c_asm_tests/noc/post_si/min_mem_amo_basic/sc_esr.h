/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _SC_ESR_
#define _SC_ESR_

#include "esr.h"

#define ALL_BANKS 0x0f
#define SC_ESR_MEMORY_REGION (ESR_MEMORY_REGION | (0x03 << 20))

#define SC_REQQ_CTL 0x001

inline volatile uint64_t* __attribute__((always_inline)) sc_esr_address(esr_protection_t pp, uint8_t shire_id, uint8_t b, uint32_t address)
{
    volatile uint64_t *p = (uint64_t *) (  SC_ESR_MEMORY_REGION
                         | ((uint64_t)(pp       &  0x03) << 30)
                         | ((uint64_t)(shire_id &  0xff) << 22)
                         | ((uint64_t)(b        &  0x0f) << 13)
                         | ((uint64_t)(address  & 0x3ff) <<  3));
    return p;
}


inline uint64_t __attribute__((always_inline)) read_sc_esr(esr_protection_t pp, uint8_t shire_id, uint8_t banks, uint32_t address)
{
    volatile uint64_t *p = sc_esr_address(pp, shire_id, banks, address);
    return *p;
}

inline void __attribute__((always_inline)) write_sc_esr(esr_protection_t pp, uint8_t shire_id, uint8_t banks, uint32_t address, uint64_t value)
{
    volatile uint64_t *p = sc_esr_address(pp, shire_id, banks, address);
    *p = value;
}

inline void __attribute__((always_inline)) set_esr_sc_remote_l3_enable(uint8_t shire_id, uint64_t value) {
   const uint8_t BANK0 = 0;
   uint64_t old = read_sc_esr(PP_MACHINE,  shire_id, BANK0, SC_REQQ_CTL);
   old = old & 0xfffffffffffeffff;
   value = (value & 0x1) << 16; // sc_remote_l3_enable corresponds to bit[16]
   value = old | value;
   write_sc_esr(PP_MACHINE, shire_id, ALL_BANKS, SC_REQQ_CTL, value);
}
#endif

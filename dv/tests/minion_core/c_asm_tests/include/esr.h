/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



// see PRM-19: Memory Map, PMA and PMP (Ch. 2.4 ESR Region, 2.4.1 Minion Shire ESR Map)
// see $RTLROOT/shire/esr/scripts/Minion\ ESR\ Registers.xlsx


#ifndef __ESR_H
#define __ESR_H

#include "esr_defines.h"
#ifdef __cplusplus
   #include <cstdint>
#endif

// PCIE base addresses
#ifndef R_PCIE_ESR_BASEADDR
#define R_PCIE_ESR_BASEADDR       0x0058200000
#endif

#ifndef R_PCIE_USRESR_BASEADDR
#define R_PCIE_USRESR_BASEADDR    0x7f80000000
#endif

#ifndef R_PCIE_NOPCIESR_BASEADDR
#define R_PCIE_NOPCIESR_BASEADDR  0x7f80001000
#endif

typedef enum
{
    PP_USER       = 0,
    PP_SUPERVISOR = 1,
    PP_MESSAGES   = 2,
    PP_MACHINE    = 3
} esr_protection_t;

typedef enum
{
    REGION_MINION        = 0,    // HART ESR
    REGION_MEMSHIRE      = 0,    // Memshire ESR
    REGION_NEIGHBOURHOOD = 1,    // Neighbor ESR
    REGION_DDRC          = 1,    // DDRC ESR
    REGION_TBOX          = 2,    //
    REGION_OTHER         = 3,    // Shire Cache ESR and Shire Other ESR
    REGION_PSHIRE_ESR    = 4,    //
    REGION_PSHIRE_USRESR = 5,
    REGION_PSHIRE_NOPCIESR =6
} esr_reg_t;

#ifndef ESR_MEMORY_REGION    // [32]=1
#define ESR_MEMORY_REGION 0x80000000UL
#endif

inline volatile uint64_t* __attribute__((always_inline)) esr_address(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address)
{
    volatile uint64_t *p = (uint64_t *) (  ESR_MEMORY_REGION
                         | ((uint64_t)(pp       & 0x03    ) << 30)
                         | ((uint64_t)(shire_id & 0xff    ) << 22)
                         | ((uint64_t)(region   & 0x03    ) << 20)
                         | ((uint64_t)(address  & 0x01ffff) <<  3));
    return p;
}


inline uint64_t __attribute__((always_inline)) read_esr(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address)
{
    volatile uint64_t *p = esr_address(pp, shire_id, region, address);
    return *p;
}

inline void __attribute__((always_inline)) write_esr(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address, uint64_t value)
{
    volatile uint64_t *p = esr_address(pp, shire_id, region, address);
    *p = value;
}


// new functions
inline volatile uint64_t* __attribute__((always_inline)) esr_address_new(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint8_t subregion, uint32_t address, uint8_t bnk_or_thrd /*=0x0*/)
{
    uint64_t final_addr = (  ESR_MEMORY_REGION
                         | ((uint64_t)(pp       & 0x03    ) << 30)     // 2-bit [31:30]
                         | ((uint64_t)(shire_id & 0xff    ) << 22)     // 8-bit [29:22]
                         | ((uint64_t)(region   & 0x03    ) << 20)     // 2-bit [21:20]
                          );

    if(region == REGION_MINION) {
        // subregion = Minion# (7-bit);
        // bnk_or_thrd = Thread_id (1-bit)
        final_addr = ( final_addr
                     | ((uint64_t)(subregion   & 0x7f    ) << 13)     // 7-bit [19:13]
                     | ((uint64_t)(bnk_or_thrd & 0x01    ) << 12)     // 1-bit [12]
                     | ((uint64_t)(address     & 0x01ff  ) <<  3));   // 9-bit [11:3]

    } else if(region == REGION_NEIGHBOURHOOD) {
        // subregion = Neighbor# (4-bit) [19:16];
        final_addr = ( final_addr
                     | ((uint64_t)(subregion   & 0x0f    ) << 16)     // 4-bit [19:16]
                     | ((uint64_t)(address     & 0x01fff ) <<  3));   // 13-bit [15:3]

    } else if(region == REGION_OTHER) {     //
        // subregion: 3-bit
        if(subregion == 0x00) {   // Shire Cache ESR or Debug
            // shire_cache_bank#: 4-bit [16:13]
            final_addr = ( final_addr
                         | ((uint64_t)(subregion   & 0x07    ) << 17)     // 3-bit [19:17]
                         | ((uint64_t)(bnk_or_thrd & 0x0f    ) << 13)     // 3-bit [16:13]
                         | ((uint64_t)(address     & 0x03ff  ) <<  3));   // 10-bit [12:3]
        } else {
            final_addr = ( final_addr
                         | ((uint64_t)(subregion   & 0x07    ) << 17)     // 3-bit [19:17]
                         | ((uint64_t)(address     & 0x03fff ) <<  3));   // 14-bit [16:3]
        }
    }

    volatile uint64_t *p = (uint64_t *)(final_addr);
    return p;
}

inline uint64_t __attribute__((always_inline)) read_esr_new(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint8_t subregion, uint32_t address, uint8_t bnk_or_thrd /*=0x0*/)
{
    volatile uint64_t *p = esr_address_new(pp, shire_id, region, subregion, address, bnk_or_thrd);
    return *p;
}

inline void __attribute__((always_inline)) write_esr_new(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint8_t subregion, uint32_t address, uint64_t value, uint8_t bnk_or_thrd /*=0x0*/)
{
    volatile uint64_t *p = esr_address_new(pp, shire_id, region, subregion, address, bnk_or_thrd);
    *p = value;
}


/* PSHIRE ESR Functions */
/* Use this functions for PCIE ESR read/write */
inline uint32_t __attribute__((always_inline)) ps_read_esr(uint64_t region_base, uint32_t address)
{
    volatile uint32_t *p = (uint32_t *)(region_base | address);
    return *p;
}

inline void __attribute__((always_inline)) ps_write_esr(uint64_t region_base, uint32_t address, uint32_t value)
{
    volatile uint32_t *p = (uint32_t *)(region_base | address);
    *p = value;
}

// The folowing function is used only for PSHIRE ESR tests using tables in dv/common/sw/ip/inc/esr_rw.h
inline volatile uint32_t* __attribute__((always_inline)) ps_esr_address(esr_reg_t region, uint32_t address)
{
    uint64_t region_addr;

    switch (region) {
        case REGION_PSHIRE_ESR:
            region_addr = R_PCIE_ESR_BASEADDR;
            break;
        case REGION_PSHIRE_USRESR:
            region_addr = R_PCIE_USRESR_BASEADDR;
            break;
        case REGION_PSHIRE_NOPCIESR:
            region_addr = R_PCIE_NOPCIESR_BASEADDR;
            break;
        default:
            region_addr = 0;
            break;
    }

    volatile uint32_t *p = (uint32_t *)(region_addr | address);
    return p;
}

/* MEMSHIRE ESR Functions */
inline volatile uint64_t* __attribute__((always_inline)) ms_esr_address_dv(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address)
{
    uint64_t esr_address = (            0x01ul<< 32)|
                           ((pp       & 0x03ul) << 30)|
                           ((shire_id & 0xfful) << 22)|
                           ((region   & 0x01ul) <<  9)|
                           ((address  & 0x3ful) <<  3);

    volatile uint64_t *p = (uint64_t *)esr_address;
    return p;
}

inline uint64_t __attribute__((always_inline)) ms_read_esr_dv(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address)
{
    volatile uint64_t *p = ms_esr_address_dv(pp, shire_id, region, address);
    return *p;
}

inline void __attribute__((always_inline)) ms_write_esr_dv(esr_protection_t pp, uint8_t shire_id, esr_reg_t region, uint32_t address, uint64_t value)
{
    volatile uint64_t *p = ms_esr_address_dv(pp, shire_id, region, address);
    *p = value;
}
#endif // ! __ESR_H

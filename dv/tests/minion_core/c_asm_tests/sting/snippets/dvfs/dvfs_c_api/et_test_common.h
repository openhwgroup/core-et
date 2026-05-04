/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __ET_TEST_COMMON_H
#define __ET_TEST_COMMON_H

#include <inttypes.h>
#include "macros.h"
#include "esr.h"

inline void __attribute__((always_inline)) broadcast_req(esr_protection_t pp, esr_reg_t region, uint32_t address, uint64_t shire_mask, uint64_t value)
{
   volatile uint64_t * BC_ESR_ADDR = (uint64_t *) 0x8035FFF0;
   *BC_ESR_ADDR = value;
   volatile uint64_t * BC_REQ_ADDR = (uint64_t *) (0x8035FFF8 | (uint64_t(pp) << 30));
   *BC_REQ_ADDR =  ((uint64_t(pp & 0x03)         << 59) |
                    (uint64_t(region & 0x03)     << 57) |
                    (uint64_t(address & 0x1ffff) << 40) |
                    (shire_mask & 0xffffffffff));
}
#endif

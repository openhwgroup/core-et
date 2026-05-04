/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#define MEM_REQ_DELAY (5500/2) // It takes twice the cycles to resolve...
#define CYCLES_PER_JUMP (4)
#define WAIT_REQ_CYCLES (MEM_REQ_DELAY/CYCLES_PER_JUMP)

#define NUM_HARTS_STUCK (3)
#define HELPER_HART  (6)
#define CSR_HART     (4)
#define FP_HART      (2)
#define INT_HART     (0)


#define FCC_REG (0)
#define HELPER_HART_FCC_MASK (1 << (HELPER_HART/2))

#define IPI_MASK (1ULL << FP_HART  | \
                  1ULL << INT_HART | \
                  1ULL << CSR_HART)
#define MSIP_CAUSE (1ULL << 63 | 0x3)

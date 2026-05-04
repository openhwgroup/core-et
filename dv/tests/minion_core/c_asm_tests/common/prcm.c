/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include <stdio.h>
#include "macros.h"
#include "minion.h"
#include "fcc.h"
#include "erbium_regs.h"

/* Thread 0 */
#define CREDINC0 0x803400C0
#define CREDINC1 0x803400C8

/* Thread 1 */
#define CREDINC2 0x803400D0
#define CREDINC3 0x803400D8

#define MINION_MASK 0xFFFFFFFFFFFFFFFFULL

void program_ring_osc() {
    volatile uint32_t* RING_OSC = (uint32_t*) 0x2000098;
    ring_osc_reg_t reg = {
        .field = {
            .en         = 1,
            .divby2_sel = 0,
            .trm        = 27
        }
    };
    *RING_OSC = reg.value;
    FENCE;
}

void program_cpu_divider() {
    volatile uint32_t* CPU_DIVIDER = (uint32_t*) 0x20000A0;
    cpu_divider_reg_t reg = {
        .field = {
            .count      = 0,
            .div_enable = 0
        }
    };
    *CPU_DIVIDER = reg.value;
    FENCE;
}

void program_system_divider() {
    volatile uint32_t* SYSTEM_DIVIDER = (uint32_t*) 0x20000A8;
    system_divider_reg_t reg = {
        .field = {
            .count      = 3,
            .div_enable = 0
        }
    };
    *SYSTEM_DIVIDER = reg.value;
    FENCE;
}

void program_periph_divider() {
    volatile uint32_t* PERIPH_DIVIDER = (uint32_t*) 0x20000B0;
    periph_divider_reg_t reg = {
        .field = {
            .count      = 0xF,
            .div_enable = 1
        }
    };
    *PERIPH_DIVIDER = reg.value;
    FENCE;
}

void program_prcm() {
    if (get_hart_id() == 0) {
        program_ring_osc();
        program_cpu_divider();
        program_system_divider();
        program_periph_divider();

        volatile uint64_t *fcc_th0 = (uint64_t*) CREDINC0;
        volatile uint64_t *fcc_th1 = (uint64_t*) CREDINC2;

        *fcc_th0 = MINION_MASK;
        *fcc_th1 = MINION_MASK;
    }
    wait_for_credit(0);
}

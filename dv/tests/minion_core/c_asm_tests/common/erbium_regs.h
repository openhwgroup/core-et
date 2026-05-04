/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>

typedef union {
    uint32_t value; // full 32‑bit access

    struct {
        uint32_t en             :  1; // bit 0 — Master oscillator enable
        uint32_t divby2_sel     :  1; // bit 1 — Divide‑by‑2 frequency select
        uint32_t trm            :  5; // bits 6:2 — Frequency trim code
        uint32_t dbg_en         :  1; // bit 7 — Debug override enable
        uint32_t dbg_anachip_en :  1; // bit 8 — Debug enable for analog bias block (anachip)
        uint32_t dbg_rochip_en  :  1; // bit 9 — Debug enable for ring oscillator core (rochip)
        uint32_t dbg_sah_en_b   :  1; // bit 10 — Debug SAH enable (active low)
        uint32_t reserved       : 21; // bits 31:11
    } field;
} ring_osc_reg_t;

typedef union {
    uint32_t value; // full 32‑bit access

    struct {
        uint32_t count      : 4;  // bits 3:0 — clock divider (output clk = 2 × count)
        uint32_t div_enable : 1;  // bit 4   — enable divider
        uint32_t reserved   : 27; // bits 31:5
    } field;
} cpu_divider_reg_t;

typedef union {
    uint32_t value; // full 32‑bit access

    struct {
        uint32_t count      : 4;  // bits 3:0 — clock divider (output clk = 2 × count)
        uint32_t div_enable : 1;  // bit 4   — enable divider
        uint32_t reserved   : 27; // bits 31:5
    } field;
} system_divider_reg_t;

typedef union {
    uint32_t value; // full 32‑bit access

    struct {
        uint32_t count      : 4;  // bits 3:0 — clock divider (output clk = 2 × count)
        uint32_t div_enable : 1;  // bit 4   — enable divider
        uint32_t reserved   : 27; // bits 31:5
    } field;
} periph_divider_reg_t;

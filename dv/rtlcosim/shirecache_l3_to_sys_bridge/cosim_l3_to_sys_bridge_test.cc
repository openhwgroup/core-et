// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_l3_to_sys_bridge vs shirecache_l3_to_sys_bridge.
// NumL3SlavePorts=4. Same clock for both domains.

#include "Vcosim_l3_to_sys_bridge_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vcosim_l3_to_sys_bridge_tb;

// Opcode constants (from etlink_pkg)
static constexpr uint8_t ReqWrite      = 0;
static constexpr uint8_t ReqRead       = 1;
static constexpr uint8_t ReqAtomic     = 5;
static constexpr uint8_t ReqFlush      = 8;
static constexpr uint8_t ReqFlushToMem = 9;
static constexpr uint8_t ReqEvict      = 10;
static constexpr uint8_t ReqEvictToMem = 11;
static constexpr uint8_t ReqPrefetch   = 15;

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("l3_ar_ready", d->orig_l3_ar_ready_o, d->new_l3_ar_ready_o);
    sim.compare("l3_aw_ready", d->orig_l3_aw_ready_o, d->new_l3_aw_ready_o);
    sim.compare("l3_w_ready",  d->orig_l3_w_ready_o,  d->new_l3_w_ready_o);
    sim.compare("l3_r_valid",  d->orig_l3_r_valid_o,  d->new_l3_r_valid_o);
    sim.compare("l3_b_valid",  d->orig_l3_b_valid_o,  d->new_l3_b_valid_o);
    sim.compare("sys_ar_valid", d->orig_sys_ar_valid_o, d->new_sys_ar_valid_o);
    if (d->orig_sys_ar_valid_o && d->new_sys_ar_valid_o) {
        sim.compare("sys_ar_id", d->orig_sys_ar_id_o, d->new_sys_ar_id_o);
    }
    sim.compare("sys_aw_valid", d->orig_sys_aw_valid_o, d->new_sys_aw_valid_o);
    if (d->orig_sys_aw_valid_o && d->new_sys_aw_valid_o) {
        sim.compare("sys_aw_id",   d->orig_sys_aw_id_o,   d->new_sys_aw_id_o);
        sim.compare("sys_aw_user", d->orig_sys_aw_user_o, d->new_sys_aw_user_o);
    }
    sim.compare("sys_w_valid",  d->orig_sys_w_valid_o,  d->new_sys_w_valid_o);
    if (d->orig_sys_w_valid_o && d->new_sys_w_valid_o) {
        sim.compare("sys_w_last", d->orig_sys_w_last_o, d->new_sys_w_last_o);
    }
    sim.compare("sys_r_ready",  d->orig_sys_r_ready_o,  d->new_sys_r_ready_o);
    sim.compare("sys_b_ready",  d->orig_sys_b_ready_o,  d->new_sys_b_ready_o);
    // L3 R/B field comparison (port 0)
    if (d->orig_l3_r_valid_o & d->new_l3_r_valid_o & 1) {
        sim.compare("l3_r0_id",   d->orig_l3_r0_id_o,   d->new_l3_r0_id_o);
        sim.compare("l3_r0_resp", d->orig_l3_r0_resp_o, d->new_l3_r0_resp_o);
    }
    if (d->orig_l3_b_valid_o & d->new_l3_b_valid_o & 1) {
        sim.compare("l3_b0_id",   d->orig_l3_b0_id_o,   d->new_l3_b0_id_o);
        sim.compare("l3_b0_resp", d->orig_l3_b0_resp_o, d->new_l3_b0_resp_o);
    }
}

static void tick_cmp(CosimCtrl<DUT>& sim) { sim.tick(); compare_all(sim); }

static void clear(DUT* d) {
    d->l3_ar_valid_i = 0;
    d->l3_aw_valid_i = 0;
    d->l3_w_valid_i  = 0;
    d->l3_r_ready_i  = 0xF;
    d->l3_b_ready_i  = 0xF;

    d->p0_ar_id_i    = 0; d->p0_ar_addr_i  = 0; d->p0_ar_len_i   = 0;
    d->p0_ar_size_i  = 0; d->p0_ar_burst_i = 1; d->p0_ar_lock_i  = 0;
    d->p0_ar_cache_i = 0; d->p0_ar_prot_i  = 0; d->p0_ar_qos_i   = 0;
    d->p0_ar_user_i  = 0;
    d->p1_ar_id_i    = 0; d->p1_ar_addr_i  = 0;

    d->p0_aw_id_i    = 0; d->p0_aw_addr_i  = 0; d->p0_aw_len_i   = 0;
    d->p0_aw_size_i  = 0; d->p0_aw_burst_i = 1; d->p0_aw_lock_i  = 0;
    d->p0_aw_cache_i = 0; d->p0_aw_prot_i  = 0; d->p0_aw_qos_i   = 0;
    d->p0_aw_user_i  = 0;
    d->p1_aw_id_i    = 0; d->p1_aw_addr_i  = 0; d->p1_aw_user_i  = 0;

    d->p0_w_strb_i   = 0; d->p0_w_last_i   = 1;

    d->sys_ar_ready_i = 1;
    d->sys_aw_ready_i = 1;
    d->sys_w_ready_i  = 1;
    d->sys_r_valid_i  = 0;
    d->sys_r_id_i     = 0;
    d->sys_r_resp_i   = 0;
    d->sys_r_last_i   = 1;
    d->sys_b_valid_i  = 0;
    d->sys_b_id_i     = 0;
    d->sys_b_resp_i   = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 500000;
    clear(d);

    sim.reset(10);
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 1: idle ────────────────────────────────────────
    printf("phase 1: idle\n");
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 2: AR read from port 0 ────────────────────────
    printf("phase 2: AR read port 0\n");
    d->l3_ar_valid_i = 0x1;
    d->p0_ar_id_i    = 42;
    d->p0_ar_addr_i  = 0x1000;
    d->p0_ar_size_i  = 6;
    d->p0_ar_qos_i   = 3;
    d->p0_ar_burst_i = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 3: AW+W write from port 0 ─────────────────────
    printf("phase 3: AW+W write port 0\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 100;
    d->p0_aw_addr_i  = 0x2000;
    d->p0_aw_size_i  = 6;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 4: auto-ack Flush ──────────────────────────────
    printf("phase 4: auto-ack Flush\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 200;
    d->p0_aw_addr_i  = 0x3000;
    d->p0_aw_user_i  = ReqFlush;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 4b: auto-ack Evict ─────────────────────────────
    printf("phase 4b: auto-ack Evict\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 201;
    d->p0_aw_addr_i  = 0x3100;
    d->p0_aw_user_i  = ReqEvict;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 4c: auto-ack Prefetch ──────────────────────────
    printf("phase 4c: auto-ack Prefetch\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 202;
    d->p0_aw_addr_i  = 0x3200;
    d->p0_aw_user_i  = ReqPrefetch;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 5: R response ──────────────────────────────────
    printf("phase 5: R response\n");
    d->l3_ar_valid_i = 0x1;
    d->p0_ar_id_i    = 300;
    d->p0_ar_addr_i  = 0x5000;
    d->p0_ar_size_i  = 6;
    d->p0_ar_burst_i = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // Push R response (port 0, reqq_id from first alloc)
    d->sys_r_valid_i = 1;
    d->sys_r_id_i    = (0 << 6) | 0;  // port 0, reqq entry 0
    d->sys_r_resp_i  = 0;
    d->sys_r_last_i  = 1;
    tick_cmp(sim);
    d->sys_r_valid_i = 0;
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 6: B response ──────────────────────────────────
    printf("phase 6: B response\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 400;
    d->p0_aw_addr_i  = 0x6000;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    d->sys_b_valid_i = 1;
    d->sys_b_id_i    = (0 << 6) | 0;
    d->sys_b_resp_i  = 0;
    tick_cmp(sim);
    d->sys_b_valid_i = 0;
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 7: multi-port ──────────────────────────────────
    printf("phase 7: multi-port AR\n");
    d->l3_ar_valid_i = 0x3;
    d->p0_ar_id_i    = 500;
    d->p0_ar_addr_i  = 0x7000;
    d->p0_ar_size_i  = 6;
    d->p0_ar_burst_i = 1;
    d->p1_ar_id_i    = 501;
    d->p1_ar_addr_i  = 0x7100;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 30; i++) tick_cmp(sim);

    // Multi-port AW
    printf("phase 7b: multi-port AW\n");
    d->l3_aw_valid_i = 0x3;
    d->l3_w_valid_i  = 0x3;
    d->p0_aw_id_i    = 510;
    d->p0_aw_addr_i  = 0x7200;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_aw_burst_i = 1;
    d->p0_w_last_i   = 1;
    d->p1_aw_id_i    = 511;
    d->p1_aw_addr_i  = 0x7300;
    d->p1_aw_user_i  = ReqWrite;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 30; i++) tick_cmp(sim);

    // ── Phase 8: thorough random (1000 cycles) ────────────────
    // All 4 ports active (ports 2/3 reuse port 0/1 data in TB).
    // Mixed opcodes including auto-ack (Flush/Evict/Prefetch).
    // L3 R/B backpressure. Sys response IDs kept low to hit allocated reqq entries.
    printf("phase 8: random (1000 cycles)\n");
    uint32_t seed = 0xBEEF1234;
    // AW opcodes: Write(0), Flush(8), FlushToMem(9), Evict(10), EvictToMem(11), Prefetch(15), Atomic(5)
    static const uint8_t aw_opcodes[] = {0, 8, 9, 10, 11, 15, 5, 0};
    for (int i = 0; i < 1000; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

        // All 4 ports active
        d->l3_ar_valid_i = seed & 0xF;
        d->l3_aw_valid_i = (seed >> 4) & 0xF;
        d->l3_w_valid_i  = d->l3_aw_valid_i;
        // L3 R/B backpressure (75% ready)
        d->l3_r_ready_i  = ((seed >> 8) & 3) != 0 ? 0xF : 0x0;
        d->l3_b_ready_i  = ((seed >> 10) & 3) != 0 ? 0xF : 0x0;

        // Port 0 AR — random ID and address
        d->p0_ar_id_i    = (seed >> 12) & 0x7FFFF;
        d->p0_ar_addr_i  = ((uint64_t)(seed >> 14)) << 6;
        d->p0_ar_size_i  = 6;
        d->p0_ar_burst_i = 1;

        // Port 1 AR — different random
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        d->p1_ar_id_i    = (seed >> 0) & 0x7FFFF;
        d->p1_ar_addr_i  = ((uint64_t)(seed >> 2)) << 6;

        // Port 0 AW — mixed opcodes including auto-ack
        uint8_t opcode = aw_opcodes[(seed >> 4) & 7];
        d->p0_aw_id_i    = (seed >> 7) & 0x7FFFF;
        d->p0_aw_addr_i  = ((uint64_t)(seed >> 9)) << 6;
        d->p0_aw_user_i  = opcode;
        d->p0_aw_burst_i = 1;
        d->p0_w_last_i   = 1;

        // Port 1 AW — different opcode mix
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        uint8_t opcode1 = aw_opcodes[(seed >> 0) & 7];
        d->p1_aw_id_i    = (seed >> 3) & 0x7FFFF;
        d->p1_aw_addr_i  = ((uint64_t)(seed >> 5)) << 6;
        d->p1_aw_user_i  = opcode1;

        // Sys backpressure (50%)
        d->sys_ar_ready_i = (seed >> 7) & 1;
        d->sys_aw_ready_i = (seed >> 8) & 1;
        d->sys_w_ready_i  = (seed >> 9) & 1;

        // Sys responses — keep IDs in low range {port_id[1:0], reqq_id[5:0]}
        // so they can actually match allocated reqq entries
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        d->sys_r_valid_i = (seed & 0x7) == 0;  // ~12.5% of cycles
        d->sys_r_id_i    = seed & 0xFF;  // low 8 bits: {port_id[1:0], reqq_id[5:0]}
        d->sys_r_resp_i  = (seed >> 8) & 1 ? 2 : 0;  // occasional SlvErr
        d->sys_r_last_i  = 1;
        d->sys_b_valid_i = (seed & 0x38) == 0;  // ~12.5%
        d->sys_b_id_i    = (seed >> 10) & 0xFF;
        d->sys_b_resp_i  = (seed >> 18) & 1 ? 2 : 0;

        tick_cmp(sim);
    }

    // Drain with all readies high
    clear(d);
    d->l3_r_ready_i = 0xF;
    d->l3_b_ready_i = 0xF;
    d->sys_ar_ready_i = 1;
    d->sys_aw_ready_i = 1;
    d->sys_w_ready_i  = 1;
    for (int i = 0; i < 50; i++) tick_cmp(sim);

    return sim.finish();
}

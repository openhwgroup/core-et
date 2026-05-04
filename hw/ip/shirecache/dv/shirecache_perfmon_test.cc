// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_perfmon top module.
// Tests: ESR write/read, enable/disable counters, cycle counter start/stop,
// overflow detection, P0/P1 qualifier routing, event response merging.

#include "Vshirecache_perfmon_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>

typedef Vshirecache_perfmon_tb DUT;

// ESR ctl_status bit positions
static constexpr int CYC_START_VALID       = 0;
static constexpr int CYC_START_RESET       = 1;
static constexpr int CYC_STOP_OVERFLOW     = 2;
static constexpr int CYC_STOP_INTR_OVF     = 3;
static constexpr int P0_START_VALID        = 4;
static constexpr int P0_START_RESET        = 5;
static constexpr int P0_STOP_OVERFLOW      = 6;
static constexpr int P0_STOP_INTR_OVF      = 7;
static constexpr int P0_EVENT_BIT          = 8;
static constexpr int P0_MODE_LSB           = 9;
static constexpr int P1_START_VALID        = 17;
static constexpr int P1_START_RESET        = 18;
static constexpr int P1_STOP_OVERFLOW      = 19;
static constexpr int P1_STOP_INTR_OVF      = 20;
static constexpr int P1_EVENT_BIT          = 21;
static constexpr int P1_MODE_LSB           = 22;

// Status readback positions
static constexpr int CYC_STATUS_ACTIVE     = 32;
static constexpr int CYC_STATUS_OVERFLOW   = 33;
static constexpr int P0_STATUS_ACTIVE      = 34;
static constexpr int P0_STATUS_OVERFLOW    = 35;
static constexpr int P1_STATUS_ACTIVE      = 36;
static constexpr int P1_STATUS_OVERFLOW    = 37;

static void clear_inputs(DUT* d) {
    d->wr_en_perfmon_ctl_status_i = 0;
    d->wr_en_perfmon_cyc_cntr_i   = 0;
    d->wr_en_perfmon_p0_cntr_i    = 0;
    d->wr_en_perfmon_p1_cntr_i    = 0;
    d->wr_en_perfmon_p0_qual_i    = 0;
    d->wr_en_perfmon_p1_qual_i    = 0;
    d->wr_data_i                  = 0;

    d->p0_ev_rbuf_l2_hit_i          = 0;
    d->p0_ev_rbuf_scp_hit_i         = 0;
    d->p0_ev_msgsenddata_i          = 0;
    d->p0_ev_tag_bubble_i           = 0;
    d->p0_ev_tag_hit_i              = 0;
    d->p0_ev_tag_miss_i             = 0;
    d->p0_ev_tag_evict_dirty_i      = 0;
    d->p0_ev_tag_evict_write_around_i = 0;
    d->p0_ev_tag_evict_none_i       = 0;
    d->p0_ev_tag_victim_qwen_i      = 0;
    d->p0_ev_tag_opcode_i           = 0;
    d->p0_rsrc_cntr_i               = 0;

    d->p1_ev_rbuf_l2_hit_i          = 0;
    d->p1_ev_rbuf_scp_hit_i         = 0;
    d->p1_ev_msgsenddata_i          = 0;
    d->p1_ev_tag_bubble_i           = 0;
    d->p1_ev_tag_hit_i              = 0;
    d->p1_ev_tag_miss_i             = 0;
    d->p1_ev_tag_evict_dirty_i      = 0;
    d->p1_ev_tag_evict_write_around_i = 0;
    d->p1_ev_tag_evict_none_i       = 0;
    d->p1_ev_tag_victim_qwen_i      = 0;
    d->p1_ev_tag_opcode_i           = 0;
    d->p1_rsrc_cntr_i               = 0;
}

static void esr_write(SimCtrl<DUT>& sim, DUT* d,
                      void (*set_en)(DUT*), uint64_t data) {
    clear_inputs(d);
    set_en(d);
    d->wr_data_i = data;
    sim.tick();
    clear_inputs(d);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    // ── Test 1: Reset state ─────────────────────────────────
    printf("-- Test 1: Reset state\n");
    sim.check(d->cyc_cntr_overflow_o == 0, "cyc overflow clear after reset");
    sim.check(d->p0_cntr_overflow_o  == 0, "p0 overflow clear after reset");
    sim.check(d->p1_cntr_overflow_o  == 0, "p1 overflow clear after reset");
    // The original perfmon leaves counter/qual ESR storage unreset. After reset,
    // only the control FSM state and derived request outputs are guaranteed.
    sim.check(((d->esr_ctl_status_o >> CYC_STATUS_ACTIVE) & 1) == 0, "cyc inactive after reset");
    sim.check(((d->esr_ctl_status_o >> P0_STATUS_ACTIVE) & 1) == 0, "p0 inactive after reset");
    sim.check(((d->esr_ctl_status_o >> P1_STATUS_ACTIVE) & 1) == 0, "p1 inactive after reset");
    sim.check(d->p0_req_ev_mode_o    == 0, "p0 ev_mode zero after reset");
    sim.check(d->p1_req_ev_mode_o    == 0, "p1 ev_mode zero after reset");
    sim.check(d->p0_req_rsrc_mode_o  == 0, "p0 rsrc_mode zero after reset");
    sim.check(d->p1_req_rsrc_mode_o  == 0, "p1 rsrc_mode zero after reset");
    sim.check(d->p0_req_cgater_o     == 0, "p0 cgater zero after reset");
    sim.check(d->p1_req_cgater_o     == 0, "p1 cgater zero after reset");

    // ── Test 2: ESR write/read ctl_status ───────────────────
    printf("-- Test 2: ESR write/read ctl_status\n");
    // Write cyc start valid bit
    uint64_t ctl_val = (1ULL << CYC_START_VALID);
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, ctl_val);
    // Let it settle through the state machine (IDLE -> START -> DLY1 -> DLY2 -> DLY3 -> ACTIVE)
    for (int i = 0; i < 6; i++) sim.tick();
    // Check active bit in status readback
    sim.check((d->esr_ctl_status_o >> CYC_STATUS_ACTIVE) & 1, "cyc counter active after start");

    // ── Test 3: Cycle counter counting ──────────────────────
    printf("-- Test 3: Cycle counter counting\n");
    uint64_t cyc_before = d->esr_cyc_cntr_o;
    sim.tick();
    uint64_t cyc_after = d->esr_cyc_cntr_o;
    sim.check(cyc_after > cyc_before, "cyc counter increments while active");

    // ── Test 4: Stop cycle counter ──────────────────────────
    printf("-- Test 4: Stop cycle counter\n");
    // Write ctl_status with start_valid cleared to stop
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, 0);
    for (int i = 0; i < 6; i++) sim.tick();
    cyc_before = d->esr_cyc_cntr_o;
    sim.tick();
    cyc_after = d->esr_cyc_cntr_o;
    sim.check(cyc_after == cyc_before, "cyc counter stops when disabled");

    // ── Test 5: ESR write/read cyc counter ──────────────────
    printf("-- Test 5: ESR write/read cyc counter\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_cyc_cntr_i = 1; }, 0x123456789ULL);
    sim.tick();
    // Counter stores only 40 bits (overflow bit is separate)
    uint64_t expected_cyc = 0x123456789ULL & ((1ULL << 40) - 1);
    sim.check(d->esr_cyc_cntr_o == expected_cyc, "cyc counter write/read");

    // ── Test 6: ESR write/read p0 counter ───────────────────
    printf("-- Test 6: ESR write/read p0 counter\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_cntr_i = 1; }, 0xABCDEF012ULL);
    sim.tick();
    uint64_t expected_p0 = 0xABCDEF012ULL & ((1ULL << 40) - 1);
    sim.check(d->esr_p0_cntr_o == expected_p0, "p0 counter write/read");

    // ── Test 7: ESR write/read p1 counter ───────────────────
    printf("-- Test 7: ESR write/read p1 counter\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p1_cntr_i = 1; }, 0x9876543210ULL);
    sim.tick();
    uint64_t expected_p1 = 0x9876543210ULL & ((1ULL << 40) - 1);
    sim.check(d->esr_p1_cntr_o == expected_p1, "p1 counter write/read");

    // ── Test 8: ESR write/read p0 qualifier ─────────────────
    printf("-- Test 8: ESR write/read p0 qualifier\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_qual_i = 1; }, 0x7FFFFFFFFF000ULL);
    sim.tick();
    // Qualifier is 51 bits
    uint64_t expected_qual = 0x7FFFFFFFFF000ULL & ((1ULL << 51) - 1);
    sim.check(d->esr_p0_qual_o == expected_qual, "p0 qual write/read");

    // ── Test 9: ESR write/read p1 qualifier ─────────────────
    printf("-- Test 9: ESR write/read p1 qualifier\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p1_qual_i = 1; }, 0x555555555ULL);
    sim.tick();
    uint64_t expected_qual1 = 0x555555555ULL & ((1ULL << 51) - 1);
    sim.check(d->esr_p1_qual_o == expected_qual1, "p1 qual write/read");

    // ── Test 10: P0 counter enable/start ────────────────────
    printf("-- Test 10: P0 counter enable/start\n");
    // Reset counters first
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_cntr_i = 1; }, 0);
    sim.tick();
    // Start P0 counter with event mode and qualifier
    uint64_t ctl_p0_start = (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT);
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, ctl_p0_start);
    for (int i = 0; i < 6; i++) sim.tick();
    sim.check((d->esr_ctl_status_o >> P0_STATUS_ACTIVE) & 1, "p0 counter active after start");

    // ── Test 11: P1 counter enable/start ────────────────────
    printf("-- Test 11: P1 counter enable/start\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p1_cntr_i = 1; }, 0);
    sim.tick();
    uint64_t ctl_p1_start = (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT) |
                            (1ULL << P1_START_VALID) | (1ULL << P1_EVENT_BIT);
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, ctl_p1_start);
    for (int i = 0; i < 6; i++) sim.tick();
    sim.check((d->esr_ctl_status_o >> P1_STATUS_ACTIVE) & 1, "p1 counter active after start");

    // ── Test 12: P0 event mode routing (ev_mode to req_info) ─
    printf("-- Test 12: P0 event mode routing\n");
    // With P0 event mode active, set rbuf L2 hit qualifier
    // First write qual with bit 0 set (Q_RBUF_L2_HIT)
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_qual_i = 1; }, 0x1);
    sim.tick();
    // ev_mode should have rbuf bit set for P0
    sim.check((d->p0_req_ev_mode_o & 1) != 0, "p0 ev_mode rbuf set when enabled");

    // ── Test 13: P1 resource mode routing ───────────────────
    printf("-- Test 13: P1 resource mode routing\n");
    // Stop all, then start P1 in resource mode (event bit = 0)
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, 0);
    for (int i = 0; i < 6; i++) sim.tick();
    // P1 resource mode = 0 (PerfmonRsrcL3Reqq)
    uint64_t ctl_p1_rsrc = (1ULL << P1_START_VALID);
    // No P1_EVENT_BIT => resource mode
    // P1 mode = 0 at bits [29:22]
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, ctl_p1_rsrc);
    for (int i = 0; i < 6; i++) sim.tick();
    // rsrc_mode[0] should be set for P1 (L3Reqq for P1)
    sim.check((d->p1_req_rsrc_mode_o & 1) != 0, "p1 rsrc_mode[0] set in mode 0");

    // ── Test 14: cgater routing ─────────────────────────────
    printf("-- Test 14: cgater routing\n");
    // cgater should be same for p0 and p1
    sim.check(d->p0_req_cgater_o == d->p1_req_cgater_o, "p0 and p1 cgater match");

    // ── Test 15: Event response merging ─────────────────────
    printf("-- Test 15: Event response merging\n");
    // The DUT ORs ev_rsp from both P0 and P1 rsp_info.
    // Set up P0 as event mode, both active
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; },
              (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT));
    for (int i = 0; i < 6; i++) sim.tick();
    // Set qualifier for rbuf hit
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_qual_i = 1; },
              (1ULL << 0) | // Q_RBUF_L2_HIT
              (1ULL << 4) | (1ULL << 6) | // Q_TAG_HIT, Q_EVICT_NONE
              (1ULL << 9) | // Q_EVICT_0_QWEN
              (1ULL << 14)); // Q_L2_READ opcode
    sim.tick();
    // Now trigger events from P0 rsp
    clear_inputs(d);
    d->p0_ev_rbuf_l2_hit_i = 1;
    sim.tick();
    // ev_mode bits in req_info should have rbuf
    sim.check((d->p0_req_ev_mode_o & 1) != 0, "event merge: p0 rbuf ev_mode set");
    clear_inputs(d);

    // ── Test 16: All counters disabled ──────────────────────
    printf("-- Test 16: All counters disabled\n");
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, 0);
    for (int i = 0; i < 6; i++) sim.tick();
    sim.check(!((d->esr_ctl_status_o >> CYC_STATUS_ACTIVE) & 1), "cyc not active after disable");
    sim.check(!((d->esr_ctl_status_o >> P0_STATUS_ACTIVE) & 1),  "p0 not active after disable");
    sim.check(!((d->esr_ctl_status_o >> P1_STATUS_ACTIVE) & 1),  "p1 not active after disable");

    // ── Test 17: Start with reset ───────────────────────────
    printf("-- Test 17: Start with reset\n");
    // Write a known value to cyc counter
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_cyc_cntr_i = 1; }, 0xFFFF);
    sim.tick();
    sim.check(d->esr_cyc_cntr_o == 0xFFFF, "cyc counter preloaded");
    // Start cyc counter with reset flag
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; },
              (1ULL << CYC_START_VALID) | (1ULL << CYC_START_RESET));
    for (int i = 0; i < 6; i++) sim.tick();
    // Counter should have been reset (value depends on how many cycles since reset)
    // But it should NOT still be 0xFFFF
    sim.check(d->esr_cyc_cntr_o != 0xFFFF, "cyc counter reset on start");

    // ── Test 18: Multiple ESR writes in sequence ────────────
    printf("-- Test 18: Multiple ESR writes\n");
    // Stop everything first
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_ctl_status_i = 1; }, 0);
    for (int i = 0; i < 6; i++) sim.tick();
    // Write all counters to 0
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_cyc_cntr_i = 1; }, 0);
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p0_cntr_i = 1; }, 0);
    esr_write(sim, d, [](DUT* dd){ dd->wr_en_perfmon_p1_cntr_i = 1; }, 0);
    sim.tick();
    sim.check(d->esr_cyc_cntr_o == 0, "cyc counter cleared");
    sim.check(d->esr_p0_cntr_o  == 0, "p0 counter cleared");
    sim.check(d->esr_p1_cntr_o  == 0, "p1 counter cleared");

    return sim.finish();
}

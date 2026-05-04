// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_perfmon vs shirecache_bank_perfmon
//
// Sequential module with clocked perfmon_ctl and combinational perfmon_qual.
// Drives identical stimulus to both, compares all outputs cycle-by-cycle.

#include "Vcosim_perfmon_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_perfmon_tb;

// ESR ctl_status bit positions
static constexpr int CYC_START_VALID   = 0;
static constexpr int CYC_START_RESET   = 1;
static constexpr int P0_START_VALID    = 4;
static constexpr int P0_START_RESET    = 5;
static constexpr int P0_EVENT_BIT      = 8;
static constexpr int P0_MODE_LSB       = 9;
static constexpr int P1_START_VALID    = 17;
static constexpr int P1_START_RESET    = 18;
static constexpr int P1_EVENT_BIT      = 21;
static constexpr int P1_MODE_LSB      = 22;

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

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("p0_req_cgater",    d->orig_p0_req_cgater_o,    d->new_p0_req_cgater_o);
    sim.compare("p0_req_ev_mode",   d->orig_p0_req_ev_mode_o,   d->new_p0_req_ev_mode_o);
    sim.compare("p0_req_rsrc_mode", d->orig_p0_req_rsrc_mode_o, d->new_p0_req_rsrc_mode_o);
    sim.compare("p1_req_cgater",    d->orig_p1_req_cgater_o,    d->new_p1_req_cgater_o);
    sim.compare("p1_req_ev_mode",   d->orig_p1_req_ev_mode_o,   d->new_p1_req_ev_mode_o);
    sim.compare("p1_req_rsrc_mode", d->orig_p1_req_rsrc_mode_o, d->new_p1_req_rsrc_mode_o);
    sim.compare("cyc_overflow",     d->orig_cyc_overflow_o,     d->new_cyc_overflow_o);
    sim.compare("p0_overflow",      d->orig_p0_overflow_o,      d->new_p0_overflow_o);
    sim.compare("p1_overflow",      d->orig_p1_overflow_o,      d->new_p1_overflow_o);
    sim.compare("esr_ctl_status",   d->orig_esr_ctl_status_o,   d->new_esr_ctl_status_o);
    sim.compare("esr_cyc_cntr",     d->orig_esr_cyc_cntr_o,     d->new_esr_cyc_cntr_o);
    sim.compare("esr_p0_cntr",      d->orig_esr_p0_cntr_o,      d->new_esr_p0_cntr_o);
    sim.compare("esr_p1_cntr",      d->orig_esr_p1_cntr_o,      d->new_esr_p1_cntr_o);
    sim.compare("esr_p0_qual",      d->orig_esr_p0_qual_o,      d->new_esr_p0_qual_o);
    sim.compare("esr_p1_qual",      d->orig_esr_p1_qual_o,      d->new_esr_p1_qual_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_all(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Idle state after reset ─────────────────────
    printf("-- Phase 1: Idle state after reset\n");
    compare_all(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 2: Write qualifiers ───────────────────────────
    printf("-- Phase 2: Write P0 and P1 qualifiers\n");
    clear_inputs(d);
    d->wr_en_perfmon_p0_qual_i = 1;
    d->wr_data_i = 0x7FFFFFFFFFFFF; // all 51 bits set
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    d->wr_en_perfmon_p1_qual_i = 1;
    d->wr_data_i = 0x5555555555555ULL;
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    // ── Phase 3: Write counters ─────────────────────────────
    printf("-- Phase 3: Write counters\n");
    d->wr_en_perfmon_cyc_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    d->wr_en_perfmon_p0_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    d->wr_en_perfmon_p1_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    // ── Phase 4: Start cycle counter ────────────────────────
    printf("-- Phase 4: Start cycle counter\n");
    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = (1ULL << CYC_START_VALID) | (1ULL << CYC_START_RESET);
    tick_compare(sim);
    clear_inputs(d);
    // Wait for state machine to reach ACTIVE
    for (int i = 0; i < 8; i++) tick_compare(sim);

    // ── Phase 5: Cycle counter incrementing ─────────────────
    printf("-- Phase 5: Cycle counter incrementing\n");
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ── Phase 6: Start P0 counter in event mode ─────────────
    printf("-- Phase 6: Start P0 event counter\n");
    d->wr_en_perfmon_p0_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = (1ULL << CYC_START_VALID) |
                   (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT) |
                   (1ULL << P0_START_RESET);
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) tick_compare(sim);

    // Trigger a P0 event: rbuf L2 hit
    printf("-- Phase 6b: P0 event trigger\n");
    d->p0_ev_rbuf_l2_hit_i = 1;
    d->p0_ev_tag_hit_i     = 1;
    d->p0_ev_tag_evict_none_i = 1;
    d->p0_ev_tag_victim_qwen_i = 0;
    d->p0_ev_tag_opcode_i = 0x00; // L2Read
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 7: Start P1 counter in resource mode ──────────
    printf("-- Phase 7: Start P1 resource counter\n");
    d->wr_en_perfmon_p1_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    // P1 resource mode (no P1_EVENT_BIT), mode=0
    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = (1ULL << CYC_START_VALID) |
                   (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT) |
                   (1ULL << P1_START_VALID) | (1ULL << P1_START_RESET);
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) tick_compare(sim);

    // Drive P1 resource counter
    d->p1_rsrc_cntr_i = 5;
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 8: Stop all counters ──────────────────────────
    printf("-- Phase 8: Stop all counters\n");
    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) tick_compare(sim);

    // ── Phase 9: Write counter values and read back ─────────
    printf("-- Phase 9: Counter write/readback\n");
    d->wr_en_perfmon_cyc_cntr_i = 1;
    d->wr_data_i = 0xDEADBEEF;
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    d->wr_en_perfmon_p0_cntr_i = 1;
    d->wr_data_i = 0xCAFEBABE;
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    d->wr_en_perfmon_p1_cntr_i = 1;
    d->wr_data_i = 0x12345678;
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    // ── Phase 10: Both P0 and P1 events simultaneously ──────
    printf("-- Phase 10: Simultaneous P0 and P1 events\n");
    // Start both in event mode
    d->wr_en_perfmon_p0_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);
    d->wr_en_perfmon_p1_cntr_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);

    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = (1ULL << CYC_START_VALID) |
                   (1ULL << P0_START_VALID) | (1ULL << P0_EVENT_BIT) | (1ULL << P0_START_RESET) |
                   (1ULL << P1_START_VALID) | (1ULL << P1_EVENT_BIT) | (1ULL << P1_START_RESET);
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) tick_compare(sim);

    // Both P0 and P1 get the same events on P0 rsp
    d->p0_ev_tag_bubble_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // P1 rsp event
    d->p1_ev_msgsenddata_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 11: Qualifier changes while active ────────────
    printf("-- Phase 11: Qualifier changes while active\n");
    d->wr_en_perfmon_p0_qual_i = 1;
    d->wr_data_i = 0x3FF; // low 10 bits
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    d->wr_en_perfmon_p1_qual_i = 1;
    d->wr_data_i = 0xFFC00; // bits 10-19
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);

    // ── Phase 12: Extended idle ─────────────────────────────
    printf("-- Phase 12: Extended idle\n");
    d->wr_en_perfmon_ctl_status_i = 1;
    d->wr_data_i = 0;
    tick_compare(sim);
    clear_inputs(d);
    for (int i = 0; i < 10; i++) tick_compare(sim);

    return sim.finish();
}

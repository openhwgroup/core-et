// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_perfmon_qual vs shirecache_bank_perfmon_qual
//
// Purely combinational module. Set inputs, tick once, compare outputs.
// Tests both IsP0=1 and IsP0=0 instances simultaneously.

#include "Vcosim_perfmon_qual_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_perfmon_qual_tb;

// ── Opcode values (reqq_opcode_e) ────────────────────────
static constexpr uint8_t L2Read          = 0x00;
static constexpr uint8_t L2Write         = 0x01;
static constexpr uint8_t L2WritePartial  = 0x02;
static constexpr uint8_t L2WriteAround   = 0x04;
static constexpr uint8_t L2Lock          = 0x05;
static constexpr uint8_t L2Unlock        = 0x06;
static constexpr uint8_t L2UnlockInv     = 0x07;
static constexpr uint8_t L2Flush         = 0x08;
static constexpr uint8_t L2Evict         = 0x09;
static constexpr uint8_t L2Prefetch      = 0x0a;
static constexpr uint8_t L2Atomic        = 0x0b;
static constexpr uint8_t L2Fill          = 0x0d;
static constexpr uint8_t L2Scrub         = 0x0e;
static constexpr uint8_t L3Read          = 0x10;
static constexpr uint8_t L3Write         = 0x11;
static constexpr uint8_t L3Flush         = 0x14;
static constexpr uint8_t L3FlushWData    = 0x15;
static constexpr uint8_t L3Evict         = 0x16;
static constexpr uint8_t L3EvictWData    = 0x17;
static constexpr uint8_t L3Prefetch      = 0x18;
static constexpr uint8_t L3Atomic        = 0x19;
static constexpr uint8_t L3Fill          = 0x1b;
static constexpr uint8_t L3Scrub         = 0x1c;
static constexpr uint8_t ScpRead         = 0x1d;
static constexpr uint8_t ScpWrite        = 0x1e;
static constexpr uint8_t ScpAtomic       = 0x20;
static constexpr uint8_t ScpFill         = 0x21;
static constexpr uint8_t ScpScrub        = 0x22;
static constexpr uint8_t ScpZero         = 0x23;
static constexpr uint8_t IdxRead         = 0x26;
static constexpr uint8_t IdxWrite        = 0x27;
static constexpr uint8_t IdxAllInv       = 0x28;
static constexpr uint8_t IdxL2Inv        = 0x29;
static constexpr uint8_t IdxL2Flush      = 0x2a;
static constexpr uint8_t IdxL2Evict      = 0x2b;
static constexpr uint8_t IdxL3Inv        = 0x2c;
static constexpr uint8_t IdxL3Flush      = 0x2d;
static constexpr uint8_t IdxL3Evict      = 0x2e;

// All valid opcodes for match_opcode testing
static const uint8_t ALL_OPCODES[] = {
    L2Read, L2Write, L2WriteAround, L2Lock, L2Unlock, L2UnlockInv,
    L2Flush, L2Evict, L2Prefetch, L2Atomic, L2Fill, L2Scrub,
    L3Read, L3Write, L3Flush, L3FlushWData, L3Evict, L3EvictWData,
    L3Prefetch, L3Atomic, L3Fill, L3Scrub,
    ScpRead, ScpWrite, ScpFill, ScpScrub, ScpZero, ScpAtomic,
    IdxRead, IdxWrite, IdxAllInv, IdxL2Inv, IdxL2Flush, IdxL2Evict,
    IdxL3Inv, IdxL3Flush, IdxL3Evict
};
static constexpr int NUM_OPCODES = sizeof(ALL_OPCODES) / sizeof(ALL_OPCODES[0]);

// Qualifier bit positions for opcode groups
static constexpr int Q_L2_READ         = 14;
static constexpr int Q_L3_READ         = 26;
static constexpr int Q_SCP_READ        = 36;
static constexpr int Q_IDX_READ        = 42;

// Qualifier bit offsets for individual opcodes within each group
// L2: READ(14), WRITE(15), WRITE_AROUND(16), LOCK(17), UNLOCK(18),
//     UNLOCKINV(19), FLUSH(20), EVICT(21), PREFETCH(22), ATOMIC(23),
//     FILL(24), SCRUB(25)
// L3: READ(26), WRITE(27), FLUSH(28), FLUSHWDATA(29), EVICT(30),
//     EVICTWDATA(31), PREFETCH(32), ATOMIC(33), FILL(34), SCRUB(35)
// SCP: READ(36), WRITE(37), FILL(38), SCRUB(39), ZERO(40), ATOMIC(41)
// IDX: READ(42), WRITE(43), ALL_INV(44), L2_INV(45), L2_FLUSH(46),
//      L2_EVICT(47), L3_INV(48), L3_FLUSH(49), L3_EVICT(50)

// Resource qualifier encoding positions within perfmon_qual[23:0]
static constexpr int OPER_LSB   = 0;
static constexpr int MIN_LSB    = 8;   // OPER_LSB + OPER_SIZE(8)
static constexpr int MAX_LSB    = 16;  // MIN_LSB + MIN_SIZE(8)
static constexpr int ACC_OPER   = 0;
static constexpr int CNT_OPER   = 1;
static constexpr int MAX_OPER   = 2;

// ── Helpers ──────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->perfmon_enable_i     = 0;
    d->perfmon_event_i      = 0;
    d->perfmon_mode_i       = 0;
    d->perfmon_qual_i       = 0;
    d->esr_sc_perfmon_cntr_i = 0;

    d->ev_rbuf_l2_hit_i            = 0;
    d->ev_rbuf_scp_hit_i           = 0;
    d->ev_msgsenddata_i            = 0;
    d->ev_tag_bubble_i             = 0;
    d->ev_tag_hit_i                = 0;
    d->ev_tag_miss_i               = 0;
    d->ev_tag_evict_dirty_i        = 0;
    d->ev_tag_evict_write_around_i = 0;
    d->ev_tag_evict_none_i         = 0;
    d->ev_tag_victim_qwen_i        = 0;
    d->ev_tag_opcode_i             = 0;
    d->rsrc_cntr_i                 = 0;
}

// Set perfmon_qual_i (51 bits) from a 64-bit value
static void set_qual(DUT* d, uint64_t val) {
    d->perfmon_qual_i = val & 0x7FFFFFFFFFFFFULL;  // mask to 51 bits
}

// Set esr_sc_perfmon_cntr_i (41 bits) from a 64-bit value
static void set_cntr(DUT* d, uint64_t val) {
    d->esr_sc_perfmon_cntr_i = val & 0x1FFFFFFFFFFULL;  // mask to 41 bits
}

// Build a qualifier with resource fields (oper, min, max) in bits [23:0]
// and opcode/event qualifier bits in [50:24]
static uint64_t make_rsrc_qual(int oper, int min_val, int max_val) {
    uint64_t q = 0;
    q |= ((uint64_t)oper    & 0xFF) << OPER_LSB;
    q |= ((uint64_t)min_val & 0xFF) << MIN_LSB;
    q |= ((uint64_t)max_val & 0xFF) << MAX_LSB;
    return q;
}

// ── Compare helpers ──────────────────────────────────────

static void compare_p0(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("p0_req_cgater",
                (uint32_t)d->orig_p0_req_cgater_o,
                (uint32_t)d->new_p0_req_cgater_o);
    sim.compare("p0_req_ev_mode",
                (uint32_t)d->orig_p0_req_ev_mode_o,
                (uint32_t)d->new_p0_req_ev_mode_o);
    sim.compare("p0_req_rsrc_mode",
                (uint32_t)d->orig_p0_req_rsrc_mode_o,
                (uint32_t)d->new_p0_req_rsrc_mode_o);
    sim.compare("p0_update_cntr",
                (uint32_t)d->orig_p0_update_cntr_o,
                (uint32_t)d->new_p0_update_cntr_o);
    sim.compare("p0_update_cntr_val",
                (uint64_t)d->orig_p0_update_cntr_val_o,
                (uint64_t)d->new_p0_update_cntr_val_o);
}

static void compare_p1(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("p1_req_cgater",
                (uint32_t)d->orig_p1_req_cgater_o,
                (uint32_t)d->new_p1_req_cgater_o);
    sim.compare("p1_req_ev_mode",
                (uint32_t)d->orig_p1_req_ev_mode_o,
                (uint32_t)d->new_p1_req_ev_mode_o);
    sim.compare("p1_req_rsrc_mode",
                (uint32_t)d->orig_p1_req_rsrc_mode_o,
                (uint32_t)d->new_p1_req_rsrc_mode_o);
    sim.compare("p1_update_cntr",
                (uint32_t)d->orig_p1_update_cntr_o,
                (uint32_t)d->new_p1_update_cntr_o);
    sim.compare("p1_update_cntr_val",
                (uint64_t)d->orig_p1_update_cntr_val_o,
                (uint64_t)d->new_p1_update_cntr_val_o);
}

static void compare_all(CosimCtrl<DUT>& sim) {
    compare_p0(sim);
    compare_p1(sim);
}

// ── Main ─────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // ── Phase 1: All disabled ────────────────────────────
    printf("phase 1: all disabled\n");
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 2: Event mode — rbuf L2 hit only ──────────
    printf("phase 2: event mode — rbuf L2 hit\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;  // event mode
    // Q_RBUF_L2_HIT = bit 0
    set_qual(d, 1ULL << 0);
    d->ev_rbuf_l2_hit_i = 1;
    set_cntr(d, 100);
    sim.tick();
    compare_all(sim);

    // ── Phase 3: Event mode — rbuf SCP hit only ─────────
    printf("phase 3: event mode — rbuf SCP hit\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_RBUF_SCP_HIT = bit 1
    set_qual(d, 1ULL << 1);
    d->ev_rbuf_scp_hit_i = 1;
    set_cntr(d, 200);
    sim.tick();
    compare_all(sim);

    // ── Phase 4: Event mode — msgsenddata ────────────────
    printf("phase 4: event mode — msgsenddata\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_MSGSENDDATA = bit 2
    set_qual(d, 1ULL << 2);
    d->ev_msgsenddata_i = 1;
    set_cntr(d, 300);
    sim.tick();
    compare_all(sim);

    // ── Phase 5: Event mode — tag bubble ─────────────────
    printf("phase 5: event mode — tag bubble\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_BUBBLE = bit 3
    set_qual(d, 1ULL << 3);
    d->ev_tag_bubble_i = 1;
    set_cntr(d, 400);
    sim.tick();
    compare_all(sim);

    // ── Phase 6: Event mode — tag hit + evict dirty + 0 qwen + L2Read ──
    printf("phase 6: event mode — full tag event\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_0_QWEN(9) | Q_L2_READ(14)
    set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 9) | (1ULL << 14));
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;  // 0 qwen
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 500);
    sim.tick();
    compare_all(sim);

    // ── Phase 7: Event mode — tag miss + evict none + 4 qwen + L3Write ──
    printf("phase 7: event mode — tag miss/evict none/4 qwen\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_MISS(5) | Q_EVICT_NONE(6) | Q_EVICT_4_QWEN(13) | Q_L3_WRITE(27)
    set_qual(d, (1ULL << 5) | (1ULL << 6) | (1ULL << 13) | (1ULL << 27));
    d->ev_tag_miss_i          = 1;
    d->ev_tag_evict_none_i    = 1;
    d->ev_tag_victim_qwen_i   = 0xF;  // 4 qwen
    d->ev_tag_opcode_i        = L3Write;
    set_cntr(d, 600);
    sim.tick();
    compare_all(sim);

    // ── Phase 8: Event mode — write_around evict + 2 qwen + ScpRead ──
    printf("phase 8: event mode — write_around evict\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_HIT(4) | Q_EVICT_WRITE_AROUND(8) | Q_EVICT_2_QWEN(11) | Q_SCP_READ(36)
    set_qual(d, (1ULL << 4) | (1ULL << 8) | (1ULL << 11) | (1ULL << 36));
    d->ev_tag_hit_i                 = 1;
    d->ev_tag_evict_write_around_i  = 1;
    d->ev_tag_victim_qwen_i         = 0x3;  // 2 qwen
    d->ev_tag_opcode_i              = ScpRead;
    set_cntr(d, 700);
    sim.tick();
    compare_all(sim);

    // ── Phase 9: Event mode — test each qwen count ──────
    printf("phase 9: event mode — each qwen count\n");
    for (int qcnt = 0; qcnt <= 4; qcnt++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_{qcnt}_QWEN(9+qcnt) | Q_L2_READ(14)
        set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << (9 + qcnt)) | (1ULL << 14));
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        // Set qwen with correct popcount
        uint8_t qwen_vals[] = {0x0, 0x1, 0x3, 0x7, 0xF};
        d->ev_tag_victim_qwen_i  = qwen_vals[qcnt];
        d->ev_tag_opcode_i       = L2Read;
        set_cntr(d, 800 + qcnt);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 10: Event mode — test each opcode ─────────
    printf("phase 10: event mode — each opcode\n");
    // Map opcode to qualifier bit position
    struct OpcodeQual { uint8_t opcode; int qual_bit; };
    static const OpcodeQual opcode_map[] = {
        {L2Read,         14}, {L2Write,       15}, {L2WriteAround, 16},
        {L2Lock,         17}, {L2Unlock,      18}, {L2UnlockInv,   19},
        {L2Flush,        20}, {L2Evict,       21}, {L2Prefetch,    22},
        {L2Atomic,       23}, {L2Fill,        24}, {L2Scrub,       25},
        {L3Read,         26}, {L3Write,       27}, {L3Flush,       28},
        {L3FlushWData,   29}, {L3Evict,       30}, {L3EvictWData,  31},
        {L3Prefetch,     32}, {L3Atomic,      33}, {L3Fill,        34},
        {L3Scrub,        35},
        {ScpRead,        36}, {ScpWrite,      37}, {ScpFill,       38},
        {ScpScrub,       39}, {ScpZero,       40}, {ScpAtomic,     41},
        {IdxRead,        42}, {IdxWrite,      43}, {IdxAllInv,     44},
        {IdxL2Inv,       45}, {IdxL2Flush,    46}, {IdxL2Evict,    47},
        {IdxL3Inv,       48}, {IdxL3Flush,    49}, {IdxL3Evict,    50},
    };
    static constexpr int NUM_OPC_MAP = sizeof(opcode_map) / sizeof(opcode_map[0]);

    for (int i = 0; i < NUM_OPC_MAP; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_0_QWEN(9) | opcode qual bit
        uint64_t qual = (1ULL << 4) | (1ULL << 7) | (1ULL << 9) |
                        (1ULL << opcode_map[i].qual_bit);
        set_qual(d, qual);
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        d->ev_tag_victim_qwen_i  = 0x0;  // 0 qwen
        d->ev_tag_opcode_i       = opcode_map[i].opcode;
        set_cntr(d, 1000 + i);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 11: Event mode — opcode mismatch (no qual bit) ──
    printf("phase 11: event mode — opcode mismatch\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_0_QWEN(9) but no opcode qual bit
    set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 9));
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;  // opcode set but qual bit 14 not set
    set_cntr(d, 2000);
    sim.tick();
    compare_all(sim);

    // ── Phase 12: Event mode — unknown opcode (default case) ──
    printf("phase 12: event mode — unknown opcode\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // All opcode qual bits set, but use an unmatched opcode
    // Set all opcode qual bits (14..50)
    uint64_t all_opc_bits = 0;
    for (int b = 14; b <= 50; b++) all_opc_bits |= (1ULL << b);
    set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 9) | all_opc_bits);
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = 0x03;  // L2WritePartial_2nd_pass — not in match list
    set_cntr(d, 2100);
    sim.tick();
    compare_all(sim);

    // ── Phase 13: Event mode — multiple events simultaneously ──
    printf("phase 13: event mode — multiple events\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_RBUF_L2_HIT(0) | Q_MSGSENDDATA(2) | Q_TAG_BUBBLE(3) |
    // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_0_QWEN(9) | Q_L2_READ(14)
    set_qual(d, (1ULL << 0) | (1ULL << 2) | (1ULL << 3) |
               (1ULL << 4) | (1ULL << 7) | (1ULL << 9) | (1ULL << 14));
    d->ev_rbuf_l2_hit_i     = 1;
    d->ev_msgsenddata_i     = 1;
    d->ev_tag_bubble_i      = 1;
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 3000);
    sim.tick();
    compare_all(sim);

    // ── Phase 14: Event mode disabled (enable=0) ─────────
    printf("phase 14: event mode disabled\n");
    clear_inputs(d);
    d->perfmon_enable_i = 0;  // disabled
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 0) | (1ULL << 4) | (1ULL << 7) | (1ULL << 9) | (1ULL << 14));
    d->ev_rbuf_l2_hit_i     = 1;
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 3100);
    sim.tick();
    compare_all(sim);

    // ── Phase 15: Resource mode — ACC with value in range ──
    printf("phase 15: resource mode — ACC in range\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;  // resource mode
    d->perfmon_mode_i   = 0;  // MODE_0
    // ACC_OPER, min=2, max=10
    set_qual(d, make_rsrc_qual(ACC_OPER, 2, 10));
    d->rsrc_cntr_i = 5;
    set_cntr(d, 100);
    sim.tick();
    compare_all(sim);

    // ── Phase 16: Resource mode — ACC below min (no update) ──
    printf("phase 16: resource mode — ACC below min\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 5, 10));
    d->rsrc_cntr_i = 3;  // below min=5
    set_cntr(d, 200);
    sim.tick();
    compare_all(sim);

    // ── Phase 17: Resource mode — ACC above max (no update) ──
    printf("phase 17: resource mode — ACC above max\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 2, 5));
    d->rsrc_cntr_i = 7;  // above max=5
    set_cntr(d, 300);
    sim.tick();
    compare_all(sim);

    // ── Phase 18: Resource mode — CNT in range ──────────
    printf("phase 18: resource mode — CNT in range\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(CNT_OPER, 0, 127));
    d->rsrc_cntr_i = 42;
    set_cntr(d, 400);
    sim.tick();
    compare_all(sim);

    // ── Phase 19: Resource mode — CNT out of range ──────
    printf("phase 19: resource mode — CNT out of range\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(CNT_OPER, 50, 60));
    d->rsrc_cntr_i = 30;  // below min
    set_cntr(d, 500);
    sim.tick();
    compare_all(sim);

    // ── Phase 20: Resource mode — MAX, new > current ────
    printf("phase 20: resource mode — MAX, new > current\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(MAX_OPER, 0, 127));
    d->rsrc_cntr_i = 50;
    set_cntr(d, 30);  // current counter = 30, rsrc = 50 > 30
    sim.tick();
    compare_all(sim);

    // ── Phase 21: Resource mode — MAX, new <= current ───
    printf("phase 21: resource mode — MAX, new <= current\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(MAX_OPER, 0, 127));
    d->rsrc_cntr_i = 10;
    set_cntr(d, 50);  // current counter = 50, rsrc = 10 <= 50
    sim.tick();
    compare_all(sim);

    // ── Phase 22: Resource mode — MAX with range check ──
    printf("phase 22: resource mode — MAX with range check\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(MAX_OPER, 5, 100));
    d->rsrc_cntr_i = 80;
    set_cntr(d, 50);  // in range and > current
    sim.tick();
    compare_all(sim);

    // ── Phase 23: Resource mode — P0 mode sweep ─────────
    printf("phase 23: resource mode — P0 mode sweep\n");
    for (int mode = 0; mode <= 4; mode++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = mode;
        set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
        d->rsrc_cntr_i = 10;
        set_cntr(d, 100 + mode);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 24: Resource mode — P1 mode sweep ─────────
    // (Same inputs; compare_all checks P1 too)
    printf("phase 24: resource mode — P1 mode sweep (verify rsrc_mode)\n");
    for (int mode = 0; mode <= 4; mode++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = mode;
        set_qual(d, make_rsrc_qual(CNT_OPER, 0, 127));
        d->rsrc_cntr_i = 20;
        set_cntr(d, 200 + mode);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 25: Resource mode disabled (event=1) ──────
    printf("phase 25: resource mode disabled (event=1)\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;  // event mode, so rsrc_enable is 0
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
    d->rsrc_cntr_i = 100;
    set_cntr(d, 500);
    sim.tick();
    compare_all(sim);

    // ── Phase 26: Resource mode — ACC at exact min boundary ──
    printf("phase 26: resource mode — ACC at min boundary\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 5, 10));
    d->rsrc_cntr_i = 5;  // exactly at min
    set_cntr(d, 100);
    sim.tick();
    compare_all(sim);

    // ── Phase 27: Resource mode — ACC at exact max boundary ──
    printf("phase 27: resource mode — ACC at max boundary\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 5, 10));
    d->rsrc_cntr_i = 10;  // exactly at max
    set_cntr(d, 200);
    sim.tick();
    compare_all(sim);

    // ── Phase 28: Resource mode — ACC one below min ─────
    printf("phase 28: resource mode — ACC one below min\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 5, 10));
    d->rsrc_cntr_i = 4;
    set_cntr(d, 300);
    sim.tick();
    compare_all(sim);

    // ── Phase 29: Resource mode — ACC one above max ─────
    printf("phase 29: resource mode — ACC one above max\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 5, 10));
    d->rsrc_cntr_i = 11;
    set_cntr(d, 400);
    sim.tick();
    compare_all(sim);

    // ── Phase 30: Event mode — tag hit no evict qualifier ──
    printf("phase 30: event mode — hitmiss without evict\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Q_TAG_HIT(4) only — no evict, no qwen, no opcode qual bits
    set_qual(d, (1ULL << 4));
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 5000);
    sim.tick();
    compare_all(sim);

    // ── Phase 31: Event mode — all eviction types ───────
    printf("phase 31: event mode — each eviction type\n");
    // dirty
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 9) | (1ULL << 14));
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 5100);
    sim.tick();
    compare_all(sim);

    // write_around
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 4) | (1ULL << 8) | (1ULL << 9) | (1ULL << 14));
    d->ev_tag_hit_i                 = 1;
    d->ev_tag_evict_write_around_i  = 1;
    d->ev_tag_victim_qwen_i         = 0x0;
    d->ev_tag_opcode_i              = L2Read;
    set_cntr(d, 5200);
    sim.tick();
    compare_all(sim);

    // none
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 4) | (1ULL << 6) | (1ULL << 9) | (1ULL << 14));
    d->ev_tag_hit_i          = 1;
    d->ev_tag_evict_none_i   = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 5300);
    sim.tick();
    compare_all(sim);

    // ── Phase 32: Event mode — all qwen values ──────────
    printf("phase 32: event mode — all 16 qwen values\n");
    for (uint8_t qw = 0; qw < 16; qw++) {
        // Determine popcount
        int pcnt = __builtin_popcount(qw);
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        // Q_TAG_HIT(4) | Q_EVICT_DIRTY(7) | Q_EVICT_{pcnt}_QWEN(9+pcnt) | Q_L2_READ(14)
        set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << (9 + pcnt)) | (1ULL << 14));
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        d->ev_tag_victim_qwen_i  = qw;
        d->ev_tag_opcode_i       = L2Read;
        set_cntr(d, 6000 + qw);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 33: Resource mode — rsrc_cntr = 0 ─────────
    printf("phase 33: resource mode — zero counter\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 0));
    d->rsrc_cntr_i = 0;
    set_cntr(d, 0);
    sim.tick();
    compare_all(sim);

    // ── Phase 34: Resource mode — max value rsrc_cntr ───
    printf("phase 34: resource mode — max rsrc_cntr\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
    d->rsrc_cntr_i = 127;
    set_cntr(d, 0x1FFFFFFFFULL);  // large counter
    sim.tick();
    compare_all(sim);

    // ── Phase 35: Resource mode — MAX with max counter ──
    printf("phase 35: resource mode — MAX at max counter value\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(MAX_OPER, 0, 127));
    d->rsrc_cntr_i = 127;
    set_cntr(d, 126);  // current < rsrc
    sim.tick();
    compare_all(sim);

    // ── Phase 36: Resource mode — MAX at equal ──────────
    printf("phase 36: resource mode — MAX at equal\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(MAX_OPER, 0, 127));
    d->rsrc_cntr_i = 50;
    set_cntr(d, 50);  // current == rsrc, should not update
    sim.tick();
    compare_all(sim);

    // ── Phase 37: Both disabled ─────────────────────────
    printf("phase 37: both disabled\n");
    clear_inputs(d);
    d->perfmon_enable_i = 0;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
    d->rsrc_cntr_i = 100;
    d->ev_rbuf_l2_hit_i = 1;
    set_cntr(d, 1000);
    sim.tick();
    compare_all(sim);

    // ── Phase 38: Event + resource combined check ───────
    // Event mode: only event path fires, resource path should be silent
    printf("phase 38: event mode — no resource path\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;  // event mode
    set_qual(d, (1ULL << 0) | make_rsrc_qual(ACC_OPER, 0, 127));
    d->ev_rbuf_l2_hit_i = 1;
    d->rsrc_cntr_i = 50;  // ignored because event=1
    set_cntr(d, 2000);
    sim.tick();
    compare_all(sim);

    // ── Phase 39: Resource mode — mode 5+ (unmapped) ────
    printf("phase 39: resource mode — mode > 4 (no rsrc_mode bit)\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 5;  // no rsrc_mode assigned to mode 5
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
    d->rsrc_cntr_i = 10;
    set_cntr(d, 3000);
    sim.tick();
    compare_all(sim);

    // ── Phase 40: Random-ish sweep ──────────────────────
    printf("phase 40: randomized sweep\n");
    uint32_t seed = 0xDEADBEEF;
    for (int i = 0; i < 50; i++) {
        seed = seed * 1103515245 + 12345;
        clear_inputs(d);
        d->perfmon_enable_i = (seed >> 0) & 1;
        d->perfmon_event_i  = (seed >> 1) & 1;
        d->perfmon_mode_i   = (seed >> 2) & 0x7;

        uint64_t qual_val = ((uint64_t)(seed >> 5)) |
                            ((uint64_t)((seed * 2654435761u) & 0x7FFFF) << 32);
        set_qual(d, qual_val);

        d->rsrc_cntr_i = (seed >> 10) & 0x7F;
        set_cntr(d, ((uint64_t)(seed >> 3) * 17) & 0x1FFFFFFFFULL);

        d->ev_rbuf_l2_hit_i            = (seed >> 17) & 1;
        d->ev_rbuf_scp_hit_i           = (seed >> 18) & 1;
        d->ev_msgsenddata_i            = (seed >> 19) & 1;
        d->ev_tag_bubble_i             = (seed >> 20) & 1;
        d->ev_tag_hit_i                = (seed >> 21) & 1;
        d->ev_tag_miss_i               = (seed >> 22) & 1;
        d->ev_tag_evict_dirty_i        = (seed >> 23) & 1;
        d->ev_tag_evict_write_around_i = (seed >> 24) & 1;
        d->ev_tag_evict_none_i         = (seed >> 25) & 1;
        d->ev_tag_victim_qwen_i        = (seed >> 26) & 0xF;
        d->ev_tag_opcode_i             = ALL_OPCODES[(seed >> 14) % NUM_OPCODES];

        sim.tick();
        compare_all(sim);
    }

    // ── Phase 41: Event mode — 1 qwen, 3 qwen edge cases ──
    printf("phase 41: event mode — qwen edge cases\n");
    // 1 qwen with various patterns
    uint8_t one_qwen[] = {0x1, 0x2, 0x4, 0x8};
    for (int i = 0; i < 4; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 10) | (1ULL << 14)); // Q_EVICT_1_QWEN
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        d->ev_tag_victim_qwen_i  = one_qwen[i];
        d->ev_tag_opcode_i       = L2Read;
        set_cntr(d, 7000 + i);
        sim.tick();
        compare_all(sim);
    }
    // 3 qwen with various patterns
    uint8_t three_qwen[] = {0x7, 0xB, 0xD, 0xE};
    for (int i = 0; i < 4; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 12) | (1ULL << 14)); // Q_EVICT_3_QWEN
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        d->ev_tag_victim_qwen_i  = three_qwen[i];
        d->ev_tag_opcode_i       = L2Read;
        set_cntr(d, 7100 + i);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 42: Resource mode — all three opers with same data ──
    printf("phase 42: resource mode — all opers side by side\n");
    for (int oper = 0; oper <= 2; oper++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = 2;  // MODE_2
        set_qual(d, make_rsrc_qual(oper, 0, 127));
        d->rsrc_cntr_i = 42;
        set_cntr(d, 30);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 43: Counter overflow behavior ─────────────
    printf("phase 43: counter near overflow\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    set_qual(d, make_rsrc_qual(ACC_OPER, 0, 127));
    d->rsrc_cntr_i = 127;
    set_cntr(d, 0x1FFFFFFFFFFULL);  // max 41-bit value
    sim.tick();
    compare_all(sim);

    // ── Phase 44: Event mode — rbuf both hits simultaneously ──
    printf("phase 44: event mode — both rbuf hits\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 0) | (1ULL << 1));
    d->ev_rbuf_l2_hit_i  = 1;
    d->ev_rbuf_scp_hit_i = 1;
    set_cntr(d, 8000);
    sim.tick();
    compare_all(sim);

    // ── Phase 45: Event mode — tag miss with no hit ─────
    printf("phase 45: event mode — tag miss only\n");
    clear_inputs(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual(d, (1ULL << 5) | (1ULL << 7) | (1ULL << 9) | (1ULL << 14));
    d->ev_tag_miss_i         = 1;
    d->ev_tag_hit_i          = 0;
    d->ev_tag_evict_dirty_i  = 1;
    d->ev_tag_victim_qwen_i  = 0x0;
    d->ev_tag_opcode_i       = L2Read;
    set_cntr(d, 8100);
    sim.tick();
    compare_all(sim);

    // ── Phase 46: Event mode — idx opcodes ──────────────
    printf("phase 46: event mode — idx opcodes\n");
    uint8_t idx_opcodes[] = {IdxRead, IdxWrite, IdxAllInv, IdxL2Inv,
                             IdxL2Flush, IdxL2Evict, IdxL3Inv,
                             IdxL3Flush, IdxL3Evict};
    int idx_qual_bits[]   = {42, 43, 44, 45, 46, 47, 48, 49, 50};
    for (int i = 0; i < 9; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual(d, (1ULL << 4) | (1ULL << 7) | (1ULL << 9) |
                    (1ULL << idx_qual_bits[i]));
        d->ev_tag_hit_i          = 1;
        d->ev_tag_evict_dirty_i  = 1;
        d->ev_tag_victim_qwen_i  = 0x0;
        d->ev_tag_opcode_i       = idx_opcodes[i];
        set_cntr(d, 9000 + i);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 47: Resource mode — mode sweep for P1 specifically ──
    printf("phase 47: resource mode — P1 mode sweep with MAX\n");
    for (int mode = 0; mode <= 4; mode++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = mode;
        set_qual(d, make_rsrc_qual(MAX_OPER, 0, 127));
        d->rsrc_cntr_i = 60;
        set_cntr(d, 40);  // rsrc > current
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 48: Scratchpad opcodes ────────────────────
    printf("phase 48: event mode — scp opcodes\n");
    uint8_t scp_opcodes[] = {ScpRead, ScpWrite, ScpFill, ScpScrub, ScpZero, ScpAtomic};
    int scp_qual_bits[]   = {36, 37, 38, 39, 40, 41};
    for (int i = 0; i < 6; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual(d, (1ULL << 5) | (1ULL << 6) | (1ULL << 13) |
                    (1ULL << scp_qual_bits[i]));
        d->ev_tag_miss_i         = 1;
        d->ev_tag_evict_none_i   = 1;
        d->ev_tag_victim_qwen_i  = 0xF;  // 4 qwen
        d->ev_tag_opcode_i       = scp_opcodes[i];
        set_cntr(d, 9500 + i);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 49: L3 opcodes with various evict combos ──
    printf("phase 49: event mode — L3 opcodes with evict combos\n");
    uint8_t l3_opcodes[] = {L3Read, L3Write, L3Flush, L3FlushWData,
                            L3Evict, L3EvictWData, L3Prefetch, L3Atomic,
                            L3Fill, L3Scrub};
    int l3_qual_bits[]   = {26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
    for (int i = 0; i < 10; i++) {
        clear_inputs(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        // Alternate between evict types
        int evict_bit = 6 + (i % 3);  // NONE(6), DIRTY(7), WRITE_AROUND(8)
        set_qual(d, (1ULL << 4) | (1ULL << evict_bit) | (1ULL << 9) |
                    (1ULL << l3_qual_bits[i]));
        d->ev_tag_hit_i          = 1;
        if (evict_bit == 6) d->ev_tag_evict_none_i = 1;
        else if (evict_bit == 7) d->ev_tag_evict_dirty_i = 1;
        else d->ev_tag_evict_write_around_i = 1;
        d->ev_tag_victim_qwen_i  = 0x0;
        d->ev_tag_opcode_i       = l3_opcodes[i];
        set_cntr(d, 10000 + i);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 50: More random sweep ─────────────────────
    printf("phase 50: extended random sweep\n");
    seed = 0xCAFEBABE;
    for (int i = 0; i < 50; i++) {
        seed = seed * 1103515245 + 12345;
        clear_inputs(d);
        d->perfmon_enable_i = (seed >> 0) & 1;
        d->perfmon_event_i  = (seed >> 1) & 1;
        d->perfmon_mode_i   = (seed >> 2) & 0x7;

        uint64_t qual_val = ((uint64_t)(seed >> 5)) |
                            ((uint64_t)((seed * 2654435761u) & 0x7FFFF) << 32);
        set_qual(d, qual_val);

        d->rsrc_cntr_i = (seed >> 10) & 0x7F;
        set_cntr(d, ((uint64_t)(seed >> 7) * 31) & 0x1FFFFFFFFULL);

        d->ev_rbuf_l2_hit_i            = (seed >> 17) & 1;
        d->ev_rbuf_scp_hit_i           = (seed >> 18) & 1;
        d->ev_msgsenddata_i            = (seed >> 19) & 1;
        d->ev_tag_bubble_i             = (seed >> 20) & 1;
        d->ev_tag_hit_i                = (seed >> 21) & 1;
        d->ev_tag_miss_i               = (seed >> 22) & 1;
        d->ev_tag_evict_dirty_i        = (seed >> 23) & 1;
        d->ev_tag_evict_write_around_i = (seed >> 24) & 1;
        d->ev_tag_evict_none_i         = (seed >> 25) & 1;
        d->ev_tag_victim_qwen_i        = (seed >> 26) & 0xF;
        d->ev_tag_opcode_i             = ALL_OPCODES[(seed >> 14) % NUM_OPCODES];

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_perfmon_qual.
// Tests resource mode (acc/cnt/max), event mode (all qualifiers), IsP0 vs IsP1.

#include "Vshirecache_perfmon_qual_tb.h"
#include "sim_ctrl.h"

typedef Vshirecache_perfmon_qual_tb DUT;

// Opcode values (from shirecache_pkg reqq_opcode_e)
static const int L2Read       = 0x00;
static const int L2Write      = 0x01;
static const int L3Read       = 0x10;
static const int ScpRead      = 0x1d;
static const int IdxRead      = 0x26;

// Resource operation encodings (from RTL localparams)
static const int ACC_OPER = 0;
static const int CNT_OPER = 1;
static const int MAX_OPER = 2;

// Qualifier bit positions
static const int Q_RBUF_L2_HIT  = 0;
static const int Q_RBUF_SCP_HIT = 1;
static const int Q_MSGSENDDATA  = 2;
static const int Q_TAG_BUBBLE   = 3;
static const int Q_TAG_HIT      = 4;
static const int Q_TAG_MISS     = 5;
static const int Q_EVICT_NONE   = 6;
static const int Q_EVICT_DIRTY  = 7;
static const int Q_L2_READ      = 14;
static const int Q_L2_WRITE     = 15;
static const int Q_L3_READ      = 26;
static const int Q_SCP_READ     = 36;
static const int Q_IDX_READ     = 42;
static const int Q_EVICT_0_QWEN = 9;
static const int Q_EVICT_1_QWEN = 10;
static const int Q_EVICT_2_QWEN = 11;
static const int Q_EVICT_4_QWEN = 13;

// Resource qualifier field positions
static const int OPER_LSB  = 0;
static const int OPER_SIZE = 8;
static const int MIN_LSB   = OPER_LSB + OPER_SIZE;  // 8
static const int MIN_SIZE  = 8;   // PerfmonIncrSize(7) + 1
static const int MAX_LSB   = MIN_LSB + MIN_SIZE;     // 16
static const int MAX_SIZE  = 8;

static void clear(DUT* d) {
    d->perfmon_enable_i = 0;
    d->perfmon_event_i  = 0;
    d->perfmon_mode_i   = 0;
    d->perfmon_qual_i = 0;
    d->esr_sc_perfmon_cntr_i = 0;
    d->ev_rbuf_l2_hit_i         = 0;
    d->ev_rbuf_scp_hit_i        = 0;
    d->ev_msgsenddata_i         = 0;
    d->ev_tag_bubble_i          = 0;
    d->ev_tag_hit_i             = 0;
    d->ev_tag_miss_i            = 0;
    d->ev_tag_evict_dirty_i     = 0;
    d->ev_tag_evict_write_around_i = 0;
    d->ev_tag_evict_none_i      = 0;
    d->ev_tag_victim_qwen_i     = 0;
    d->ev_tag_opcode_i          = 0;
    d->rsrc_cntr_i              = 0;
}

// Set a qualifier bit in the 51-bit perfmon_qual field (QData = uint64_t)
static void set_qual_bit(DUT* d, int bit) {
    d->perfmon_qual_i |= (1ULL << bit);
}

// Set resource qualifier operation + min/max bounds
static void set_rsrc_qual(DUT* d, int oper, int min_val, int max_val) {
    d->perfmon_qual_i = (uint64_t)oper;  // OPER at bits [7:0]
    d->perfmon_qual_i |= ((uint64_t)min_val << MIN_LSB);
    d->perfmon_qual_i |= ((uint64_t)max_val << MAX_LSB);
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset();

    // ── Test 1: Disabled — no update ───────────────────────
    printf("── Test 1: Disabled\n");
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "P0: no update when disabled");
    sim.check(d->p1_update_cntr_o == 0, "P1: no update when disabled");

    // ── Test 2: Resource mode — ACC_OPER ───────────────────
    printf("── Test 2: Resource ACC_OPER\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;  // resource mode
    d->perfmon_mode_i   = 0;  // MODE_0
    set_rsrc_qual(d, ACC_OPER, 0, 127);  // min=0, max=127
    d->rsrc_cntr_i = 5;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "ACC: update=1");
    // Counter value should be old + 5
    sim.check((d->p0_update_cntr_val_o & 0x7F) == 5, "ACC: cntr += 5");

    // ── Test 3: Resource mode — CNT_OPER ───────────────────
    printf("── Test 3: Resource CNT_OPER\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    set_rsrc_qual(d, CNT_OPER, 0, 127);
    d->rsrc_cntr_i = 42;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "CNT: update=1");
    sim.check((d->p0_update_cntr_val_o & 0x7F) == 1, "CNT: cntr += 1");

    // ── Test 4: Resource mode — MAX_OPER ───────────────────
    printf("── Test 4: Resource MAX_OPER\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    set_rsrc_qual(d, MAX_OPER, 0, 127);
    d->rsrc_cntr_i = 10;
    d->esr_sc_perfmon_cntr_i = 5;  // current max = 5
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "MAX: update=1 (10>5)");
    sim.check((d->p0_update_cntr_val_o & 0x7F) == 10, "MAX: cntr = 10");

    // ── Test 5: MAX_OPER — not exceeding ───────────────────
    printf("── Test 5: MAX_OPER not exceeding\n");
    d->rsrc_cntr_i = 3;  // 3 < 5
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "MAX: no update (3<5)");

    // ── Test 6: Resource — below min bound ─────────────────
    printf("── Test 6: Resource below min bound\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 0;
    set_rsrc_qual(d, ACC_OPER, 10, 127);  // min=10
    d->rsrc_cntr_i = 5;  // 5 < 10
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "ACC: no update below min");

    // ── Test 7: Resource — above max bound ─────────────────
    printf("── Test 7: Resource above max bound\n");
    set_rsrc_qual(d, ACC_OPER, 0, 3);  // max=3
    d->rsrc_cntr_i = 5;  // 5 > 3
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "ACC: no update above max");

    // ── Test 8: P0 resource modes ──────────────────────────
    printf("── Test 8: P0 resource mode selection\n");
    for (int mode = 0; mode <= 4; mode++) {
        clear(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = mode;
        sim.tick();
        for (int b = 0; b < 5; b++) {
            int expected = (b == mode) ? 1 : 0;
            char msg[64];
            snprintf(msg, sizeof(msg), "P0: mode=%d → rsrc_mode[%d]=%d", mode, b, expected);
            sim.check(((d->p0_req_rsrc_mode_o >> b) & 1) == expected, msg);
        }
    }

    // ── Test 9: P1 resource modes ──────────────────────────
    printf("── Test 9: P1 resource mode selection\n");
    for (int mode = 0; mode <= 4; mode++) {
        clear(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 0;
        d->perfmon_mode_i   = mode;
        sim.tick();
        for (int b = 0; b < 5; b++) {
            int expected = (b == mode) ? 1 : 0;
            char msg[64];
            snprintf(msg, sizeof(msg), "P1: mode=%d → rsrc_mode[%d]=%d", mode, b, expected);
            sim.check(((d->p1_req_rsrc_mode_o >> b) & 1) == expected, msg);
        }
    }

    // ── Test 10: Event mode — RBUF L2 hit ──────────────────
    printf("── Test 10: Event RBUF L2 hit\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;  // event mode
    set_qual_bit(d, Q_RBUF_L2_HIT);
    d->ev_rbuf_l2_hit_i = 1;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "RBUF L2 hit: update=1");

    // ── Test 11: Event mode — RBUF SCP hit ─────────────────
    printf("── Test 11: Event RBUF SCP hit\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual_bit(d, Q_RBUF_SCP_HIT);
    d->ev_rbuf_scp_hit_i = 1;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "RBUF SCP hit: update=1");

    // ── Test 12: Event mode — MsgSendData ──────────────────
    printf("── Test 12: Event MsgSendData\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual_bit(d, Q_MSGSENDDATA);
    d->ev_msgsenddata_i = 1;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "MsgSendData: update=1");

    // ── Test 13: Event mode — Tag bubble ───────────────────
    printf("── Test 13: Event Tag bubble\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual_bit(d, Q_TAG_BUBBLE);
    d->ev_tag_bubble_i = 1;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "Tag bubble: update=1");

    // ── Test 14: Event mode — Tag hit + evict + qwen + opcode ───
    printf("── Test 14: Full tag event qualification\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    // Set all required qualifier bits for a full tag event
    set_qual_bit(d, Q_TAG_HIT);
    set_qual_bit(d, Q_EVICT_NONE);
    set_qual_bit(d, Q_EVICT_0_QWEN);  // 0 qwen = popcount 0
    set_qual_bit(d, Q_L2_READ);
    // Set matching responses
    d->ev_tag_hit_i        = 1;
    d->ev_tag_evict_none_i = 1;
    d->ev_tag_victim_qwen_i = 0;  // popcount=0
    d->ev_tag_opcode_i     = L2Read;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "Full tag event: update=1");

    // ── Test 15: Tag event — opcode mismatch ───────────────
    printf("── Test 15: Tag event opcode mismatch\n");
    d->ev_tag_opcode_i = L3Read;  // doesn't match Q_L2_READ
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "Opcode mismatch: no update");

    // ── Test 16: Tag event — hit/miss mismatch ─────────────
    printf("── Test 16: Tag event hit/miss mismatch\n");
    d->ev_tag_opcode_i = L2Read;  // match
    d->ev_tag_hit_i    = 0;       // no hit
    d->ev_tag_miss_i   = 1;       // miss instead
    sim.tick();
    // Q_TAG_HIT is set but hit=0, Q_TAG_MISS not set
    sim.check(d->p0_update_cntr_o == 0, "Hit/miss mismatch: no update");

    // ── Test 17: Tag event — eviction type mismatch ────────
    printf("── Test 17: Tag event eviction mismatch\n");
    d->ev_tag_hit_i        = 1;
    d->ev_tag_miss_i       = 0;
    d->ev_tag_evict_none_i = 0;
    d->ev_tag_evict_dirty_i = 1;  // dirty but Q_EVICT_DIRTY not set
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "Eviction mismatch: no update");

    // ── Test 18: Tag event — qwen count mismatch ───────────
    printf("── Test 18: Tag event qwen count mismatch\n");
    d->ev_tag_evict_dirty_i = 0;
    d->ev_tag_evict_none_i  = 1;
    d->ev_tag_victim_qwen_i = 0xF;  // popcount=4, but Q_EVICT_4_QWEN not set
    sim.tick();
    sim.check(d->p0_update_cntr_o == 0, "Qwen count mismatch: no update");

    // ── Test 19: Multiple events increment ─────────────────
    printf("── Test 19: Multiple events increment sum\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual_bit(d, Q_RBUF_L2_HIT);
    set_qual_bit(d, Q_MSGSENDDATA);
    d->ev_rbuf_l2_hit_i = 1;
    d->ev_msgsenddata_i = 1;
    sim.tick();
    sim.check(d->p0_update_cntr_o == 1, "Multiple events: update=1");
    sim.check((d->p0_update_cntr_val_o & 0x7F) == 2, "Two events → cntr += 2");

    // ── Test 20: Event mode ev_mode bits ───────────────────
    printf("── Test 20: ev_mode bits\n");
    clear(d);
    d->perfmon_enable_i = 1;
    d->perfmon_event_i  = 1;
    set_qual_bit(d, Q_RBUF_L2_HIT);
    sim.tick();
    sim.check((d->p0_req_ev_mode_o & 1) == 1, "ev_mode[RBUF] set");
    sim.check((d->p0_req_ev_mode_o >> 2) & 1, "ev_mode[TAG] always set in event mode");

    // ── Test 21: Opcode match — various opcodes ────────────
    printf("── Test 21: Opcode match for various opcodes\n");
    struct { int opcode; int qual_bit; const char* name; } opcode_tests[] = {
        {L2Read,  Q_L2_READ,  "L2Read"},
        {L2Write, Q_L2_WRITE, "L2Write"},
        {L3Read,  Q_L3_READ,  "L3Read"},
        {ScpRead, Q_SCP_READ, "ScpRead"},
        {IdxRead, Q_IDX_READ, "IdxRead"},
    };
    for (auto& t : opcode_tests) {
        clear(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual_bit(d, Q_TAG_HIT);
        set_qual_bit(d, Q_EVICT_NONE);
        set_qual_bit(d, Q_EVICT_0_QWEN);
        set_qual_bit(d, t.qual_bit);
        d->ev_tag_hit_i        = 1;
        d->ev_tag_evict_none_i = 1;
        d->ev_tag_victim_qwen_i = 0;
        d->ev_tag_opcode_i     = t.opcode;
        sim.tick();
        char msg[64];
        snprintf(msg, sizeof(msg), "%s opcode match", t.name);
        sim.check(d->p0_update_cntr_o == 1, msg);
    }

    // ── Test 22: Qwen count variations ─────────────────────
    printf("── Test 22: Qwen count variations\n");
    struct { int qwen; int expected_count; int qual_bit; } qwen_tests[] = {
        {0x0, 0, Q_EVICT_0_QWEN},
        {0x1, 1, Q_EVICT_1_QWEN},
        {0x5, 2, Q_EVICT_2_QWEN},
        {0xF, 4, Q_EVICT_4_QWEN},
    };
    for (auto& t : qwen_tests) {
        clear(d);
        d->perfmon_enable_i = 1;
        d->perfmon_event_i  = 1;
        set_qual_bit(d, Q_TAG_HIT);
        set_qual_bit(d, Q_EVICT_NONE);
        set_qual_bit(d, t.qual_bit);
        set_qual_bit(d, Q_L2_READ);
        d->ev_tag_hit_i         = 1;
        d->ev_tag_evict_none_i  = 1;
        d->ev_tag_victim_qwen_i = t.qwen;
        d->ev_tag_opcode_i      = L2Read;
        sim.tick();
        char msg[64];
        snprintf(msg, sizeof(msg), "Qwen 0x%x count=%d", t.qwen, t.expected_count);
        sim.check(d->p0_update_cntr_o == 1, msg);
    }

    return sim.finish();
}

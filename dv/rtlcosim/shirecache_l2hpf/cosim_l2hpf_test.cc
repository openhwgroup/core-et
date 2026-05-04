// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_l2hpf vs shirecache_bank_l2hpf

#include "Vcosim_l2hpf_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_l2hpf_tb;

// ── Opcode constants (reqq_opcode_e) ─────────────────────────
static constexpr int L2Read          = 0x00;
static constexpr int L2Write         = 0x01;
static constexpr int L2WritePartial  = 0x02;
static constexpr int L3Read          = 0x10;
static constexpr int L3Write         = 0x11;
static constexpr int ScpRead         = 0x20;

// ── reqq_orig_opcode_e values ────────────────────────────────
static constexpr int OrigRead        = 0x00;
static constexpr int OrigWrite       = 0x01;
static constexpr int OrigReadCoop    = 0x04;
static constexpr int OrigMsgSendData = 0x05;

// ── err_rsp_e values ─────────────────────────────────────────
static constexpr int ErrNone         = 0x00;
static constexpr int ErrTagEcc       = 0x01;
static constexpr int ErrDataEcc      = 0x02;

// ── etlink_pkg req_opcode_e ──────────────────────────────────
static constexpr int ReqWrite        = 0;
static constexpr int ReqRead         = 1;

// ── Constants ────────────────────────────────────────────────
static constexpr int NUM_ENTRIES     = 64;

// ── Helpers ──────────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->pipe_tag_rsp_valid_i       = 0;
    d->pipe_tag_rsp_opcode_i      = 0;
    d->pipe_tag_rsp_reqq_id_i     = 0;
    d->pipe_tag_rsp_orig_address_i = 0;
    d->pipe_tag_rsp_hit_i         = 0;
    d->pipe_tag_rsp_victim_i      = 0;
    d->pipe_tag_rsp_err_rsp_i     = ErrNone;

    d->rs_idx0_i          = 0;
    d->rs_source0_i       = 0;
    d->rs_tag_id0_i       = 0;
    d->rs_orig_opcode0_i  = OrigRead;

    d->rs_idx1_i          = 1;
    d->rs_source1_i       = 0;
    d->rs_tag_id1_i       = 0;
    d->rs_orig_opcode1_i  = OrigRead;
}

// ── Compare all outputs ──────────────────────────────────────

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("l2hpf_req_valid",
                (uint32_t)d->orig_l2hpf_req_valid_o,
                (uint32_t)d->new_l2hpf_req_valid_o);

    // Compare info fields when either side is valid
    if (d->orig_l2hpf_req_valid_o || d->new_l2hpf_req_valid_o) {
        sim.compare("source",
                    (uint32_t)d->orig_source_o,
                    (uint32_t)d->new_source_o);
        sim.compare("id",
                    (uint32_t)d->orig_id_o,
                    (uint32_t)d->new_id_o);

        sim.compare("address",
                    (uint64_t)d->orig_address_o,
                    (uint64_t)d->new_address_o);

        sim.compare("opcode",
                    (uint32_t)d->orig_opcode_o,
                    (uint32_t)d->new_opcode_o);
        sim.compare("hit",
                    (uint32_t)d->orig_hit_o,
                    (uint32_t)d->new_hit_o);
    }
}

// Set the 40-bit address input (split across two 32-bit words)
static void set_address(DUT* d, uint64_t addr) {
    d->pipe_tag_rsp_orig_address_i = addr & 0xFFFFFFFFFFULL;
}

// ── Main ─────────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Reset + idle ────────────────────────────────
    printf("phase 1: reset + idle\n");
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 2: L2Read hit, no victim, no error ─────────────
    printf("phase 2: L2Read hit, no victim, no error\n");
    clear_inputs(d);
    d->rs_idx0_i          = 5;
    d->rs_source0_i       = 0x3;
    d->rs_tag_id0_i       = 0xAB;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 5;
    set_address(d, 0x00DEADBEEF40ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrNone;
    sim.tick();
    compare_all(sim);

    // Let output register propagate, then clear
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 3: L2Read hit, with error (hit=1) ──────────────
    printf("phase 3: L2Read hit, with error (err != ErrNone)\n");
    clear_inputs(d);
    d->rs_idx0_i          = 10;
    d->rs_source0_i       = 0x7;
    d->rs_tag_id0_i       = 0x42;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 10;
    set_address(d, 0x00CAFE000080ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrTagEcc;  // != ErrNone => hit=1
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 4: L2Read miss ─────────────────────────────────
    printf("phase 4: L2Read miss\n");
    clear_inputs(d);
    d->rs_idx0_i          = 20;
    d->rs_source0_i       = 0x1;
    d->rs_tag_id0_i       = 0x11;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 20;
    set_address(d, 0x0012345678C0ULL);
    d->pipe_tag_rsp_hit_i     = 0;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrNone;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 5: L2Write OrigWrite ───────────────────────────
    printf("phase 5: L2Write with OrigWrite\n");
    clear_inputs(d);
    d->rs_idx0_i          = 30;
    d->rs_source0_i       = 0x5;
    d->rs_tag_id0_i       = 0x22;
    d->rs_orig_opcode0_i  = OrigWrite;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Write;
    d->pipe_tag_rsp_reqq_id_i = 30;
    set_address(d, 0x00ABCDEF0100ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrDataEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 6: L2WritePartial OrigReadCoop ─────────────────
    printf("phase 6: L2WritePartial with OrigReadCoop\n");
    clear_inputs(d);
    d->rs_idx0_i          = 40;
    d->rs_source0_i       = 0x9;
    d->rs_tag_id0_i       = 0x33;
    d->rs_orig_opcode0_i  = OrigReadCoop;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2WritePartial;
    d->pipe_tag_rsp_reqq_id_i = 40;
    set_address(d, 0x0099887766C0ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrTagEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 7: Non-L2 opcode (should NOT generate valid) ───
    printf("phase 7: non-L2 opcode (L3Read)\n");
    clear_inputs(d);
    d->rs_idx0_i          = 50;
    d->rs_source0_i       = 0x2;
    d->rs_tag_id0_i       = 0x44;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L3Read;
    d->pipe_tag_rsp_reqq_id_i = 50;
    set_address(d, 0x0011223344C0ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrNone;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 8: Another non-L2 opcode (ScpRead) ────────────
    printf("phase 8: non-L2 opcode (ScpRead)\n");
    clear_inputs(d);
    d->rs_idx0_i          = 55;
    d->rs_source0_i       = 0x4;
    d->rs_tag_id0_i       = 0x55;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = ScpRead;
    d->pipe_tag_rsp_reqq_id_i = 55;
    set_address(d, 0x00AABBCCDD00ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrTagEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 9: victim=1 (hit should be 0 in output) ────────
    printf("phase 9: victim=1 forces hit=0\n");
    clear_inputs(d);
    d->rs_idx0_i          = 15;
    d->rs_source0_i       = 0xA;
    d->rs_tag_id0_i       = 0x66;
    d->rs_orig_opcode0_i  = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 15;
    set_address(d, 0x00FEED0000C0ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 1;
    d->pipe_tag_rsp_err_rsp_i = ErrTagEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 10: err_rsp=ErrNone with hit=1 (hit should be 0)
    printf("phase 10: err_rsp=ErrNone with hit=1 => output hit=0\n");
    clear_inputs(d);
    d->rs_idx0_i          = 25;
    d->rs_source0_i       = 0xB;
    d->rs_tag_id0_i       = 0x77;
    d->rs_orig_opcode0_i  = OrigWrite;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Write;
    d->pipe_tag_rsp_reqq_id_i = 25;
    set_address(d, 0x00BABE000100ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrNone;  // ErrNone => hit=0
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 11: Use second reqq entry via idx1 ─────────────
    printf("phase 11: use second reqq entry\n");
    clear_inputs(d);
    d->rs_idx1_i          = 35;
    d->rs_source1_i       = 0xC;
    d->rs_tag_id1_i       = 0x88;
    d->rs_orig_opcode1_i  = OrigReadCoop;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 35;
    set_address(d, 0x00F0F0F0F040ULL);
    d->pipe_tag_rsp_hit_i     = 0;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrDataEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 12: OrigMsgSendData (not Read/ReadCoop => ReqWrite)
    printf("phase 12: OrigMsgSendData => opcode=ReqWrite\n");
    clear_inputs(d);
    d->rs_idx0_i          = 45;
    d->rs_source0_i       = 0xD;
    d->rs_tag_id0_i       = 0x99;
    d->rs_orig_opcode0_i  = OrigMsgSendData;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Write;
    d->pipe_tag_rsp_reqq_id_i = 45;
    set_address(d, 0x00A0A0A0A080ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrTagEcc;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 13: Back-to-back valid pulses ──────────────────
    printf("phase 13: back-to-back valid pulses\n");
    for (int i = 0; i < 8; i++) {
        clear_inputs(d);
        d->rs_idx0_i          = i;
        d->rs_source0_i       = i & 0xF;
        d->rs_tag_id0_i       = 0xA0 + i;
        d->rs_orig_opcode0_i  = (i & 1) ? OrigWrite : OrigRead;

        d->pipe_tag_rsp_valid_i   = 1;
        d->pipe_tag_rsp_opcode_i  = (i & 1) ? L2Write : L2Read;
        d->pipe_tag_rsp_reqq_id_i = i;
        set_address(d, 0x0010000000C0ULL + (uint64_t)(i * 0x40));
        d->pipe_tag_rsp_hit_i     = (i & 2) ? 1 : 0;
        d->pipe_tag_rsp_victim_i  = 0;
        d->pipe_tag_rsp_err_rsp_i = (i & 2) ? ErrTagEcc : ErrNone;
        sim.tick();
        compare_all(sim);
    }
    // Drain
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 14: Sweep all L2 opcodes with various conditions
    printf("phase 14: sweep L2 opcodes\n");
    int l2_opcodes[] = { L2Read, L2Write, L2WritePartial };
    int orig_opcodes[] = { OrigRead, OrigWrite, OrigReadCoop, OrigMsgSendData };
    int errs[] = { ErrNone, ErrTagEcc, ErrDataEcc };

    int idx = 0;
    for (int opc = 0; opc < 3; opc++) {
        for (int oopc = 0; oopc < 4; oopc++) {
            for (int e = 0; e < 3; e++) {
                for (int hit = 0; hit < 2; hit++) {
                    for (int vic = 0; vic < 2; vic++) {
                        clear_inputs(d);
                        int entry = idx % NUM_ENTRIES;
                        d->rs_idx0_i          = entry;
                        d->rs_source0_i       = (idx * 3) & 0xF;
                        d->rs_tag_id0_i       = (idx * 7) & 0xFF;
                        d->rs_orig_opcode0_i  = orig_opcodes[oopc];

                        d->pipe_tag_rsp_valid_i   = 1;
                        d->pipe_tag_rsp_opcode_i  = l2_opcodes[opc];
                        d->pipe_tag_rsp_reqq_id_i = entry;
                        set_address(d, 0x0020000000C0ULL + (uint64_t)(idx * 0x40));
                        d->pipe_tag_rsp_hit_i     = hit;
                        d->pipe_tag_rsp_victim_i  = vic;
                        d->pipe_tag_rsp_err_rsp_i = errs[e];
                        sim.tick();
                        compare_all(sim);
                        idx++;
                    }
                }
            }
        }
    }
    // Drain
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 15: No valid, confirm output goes to 0 ─────────
    printf("phase 15: idle drain\n");
    clear_inputs(d);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 16: Valid pulse then long idle ──────────────────
    printf("phase 16: single pulse then idle\n");
    clear_inputs(d);
    d->rs_idx0_i          = 63;
    d->rs_source0_i       = 0xF;
    d->rs_tag_id0_i       = 0xFF;
    d->rs_orig_opcode0_i  = OrigReadCoop;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2WritePartial;
    d->pipe_tag_rsp_reqq_id_i = 63;
    set_address(d, 0x00FFFFFFFFC0ULL);
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrDataEcc;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

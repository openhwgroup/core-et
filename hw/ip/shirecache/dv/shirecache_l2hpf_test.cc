// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_l2hpf (L2HpfImplemented=1).
// Tests opcode filtering, reqq state extraction, hit calculation, register behavior.

#include "Vshirecache_l2hpf_tb.h"
#include "sim_ctrl.h"

typedef Vshirecache_l2hpf_tb DUT;

// Opcode values from shirecache_pkg
static const int L2Read          = 0x00;
static const int L2Write         = 0x01;
static const int L2WritePartial  = 0x02;
static const int L2WriteAround   = 0x04;
static const int L3Read          = 0x0C;
static const int ScpRead         = 0x18;

// reqq_orig_opcode_e
static const int OrigRead     = 0x00;
static const int OrigWrite    = 0x01;
static const int OrigReadCoop = 0x04;

// err_rsp_e
static const int ErrNone  = 0x00;
static const int ErrSbe   = 0x01;

// etlink req_opcode_e
static const int ReqWrite = 0;
static const int ReqRead  = 1;

static void clear(DUT* d) {
    d->pipe_tag_rsp_valid_i        = 0;
    d->pipe_tag_rsp_opcode_i       = 0;
    d->pipe_tag_rsp_reqq_id_i      = 0;
    d->pipe_tag_rsp_orig_address_i = 0;
    d->pipe_tag_rsp_hit_i          = 0;
    d->pipe_tag_rsp_victim_i       = 0;
    d->pipe_tag_rsp_err_rsp_i      = 0;
    d->rs_idx_i        = 0;
    d->rs_source_i     = 0;
    d->rs_tag_id_i     = 0;
    d->rs_orig_opcode_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset();

    // ── Test 1: No valid response — output stays 0 ─────────
    printf("── Test 1: No valid → output=0\n");
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 0, "no valid → req_valid=0");

    // ── Test 2: L2Read generates valid request ─────────────
    printf("── Test 2: L2Read → valid request\n");
    d->rs_idx_i         = 5;
    d->rs_source_i      = 0x3;
    d->rs_tag_id_i      = 0x42;
    d->rs_orig_opcode_i = OrigRead;

    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_opcode_i  = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 5;
    d->pipe_tag_rsp_orig_address_i = 0xABCD1000;
    d->pipe_tag_rsp_hit_i     = 1;
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrSbe;  // != ErrNone
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 1, "L2Read → req valid");
    sim.check(d->l2hpf_req_source_o == 0x3, "source from reqq_state");
    sim.check(d->l2hpf_req_id_o == 0x42, "id from reqq_state tag_id");
    sim.check(d->l2hpf_req_opcode_o == ReqRead, "OrigRead → ReqRead");
    sim.check(d->l2hpf_req_hit_o == 1, "hit=1 (hit & ~victim & err!=None)");

    // ── Test 3: L2WritePartial also valid ──────────────────
    printf("── Test 3: L2WritePartial → valid\n");
    d->pipe_tag_rsp_opcode_i = L2WritePartial;
    d->rs_orig_opcode_i = OrigWrite;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 1, "L2WritePartial → valid");
    sim.check(d->l2hpf_req_opcode_o == ReqWrite, "OrigWrite → ReqWrite");

    // ── Test 4: L2Write also valid ─────────────────────────
    printf("── Test 4: L2Write → valid\n");
    d->pipe_tag_rsp_opcode_i = L2Write;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 1, "L2Write → valid");

    // ── Test 5: L2WriteAround NOT valid ────────────────────
    printf("── Test 5: L2WriteAround → NOT valid\n");
    d->pipe_tag_rsp_opcode_i = L2WriteAround;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 0, "L2WriteAround → not valid");

    // ── Test 6: L3Read NOT valid ───────────────────────────
    printf("── Test 6: L3Read → NOT valid\n");
    d->pipe_tag_rsp_opcode_i = L3Read;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 0, "L3Read → not valid");

    // ── Test 7: ScpRead NOT valid ──────────────────────────
    printf("── Test 7: ScpRead → NOT valid\n");
    d->pipe_tag_rsp_opcode_i = ScpRead;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 0, "ScpRead → not valid");

    // ── Test 8: OrigReadCoop → ReqRead ─────────────────────
    printf("── Test 8: OrigReadCoop → ReqRead\n");
    d->pipe_tag_rsp_opcode_i = L2Read;
    d->rs_orig_opcode_i = OrigReadCoop;
    sim.tick();
    sim.check(d->l2hpf_req_opcode_o == ReqRead, "OrigReadCoop → ReqRead");

    // ── Test 9: Hit=0 → hit output=0 ──────────────────────
    printf("── Test 9: hit=0 → output hit=0\n");
    d->pipe_tag_rsp_hit_i = 0;
    sim.tick();
    sim.check(d->l2hpf_req_hit_o == 0, "hit=0 → output=0");

    // ── Test 10: victim=1 → hit output=0 ──────────────────
    printf("── Test 10: victim=1 → hit=0\n");
    d->pipe_tag_rsp_hit_i    = 1;
    d->pipe_tag_rsp_victim_i = 1;
    sim.tick();
    sim.check(d->l2hpf_req_hit_o == 0, "victim=1 → hit=0");

    // ── Test 11: err_rsp=ErrNone → hit=0 ──────────────────
    printf("── Test 11: err_rsp=ErrNone → hit=0\n");
    d->pipe_tag_rsp_victim_i  = 0;
    d->pipe_tag_rsp_err_rsp_i = ErrNone;
    sim.tick();
    sim.check(d->l2hpf_req_hit_o == 0, "err=None → hit=0");

    // ── Test 12: Register hold — output persists ───────────
    printf("── Test 12: Register hold behavior\n");
    clear(d);
    // Setup a valid request
    d->rs_idx_i = 10;
    d->rs_source_i = 0xF;
    d->rs_tag_id_i = 0x77;
    d->rs_orig_opcode_i = OrigRead;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_opcode_i = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 10;
    d->pipe_tag_rsp_orig_address_i = 0x12345678;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_err_rsp_i = ErrSbe;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 1, "valid after stimulus");

    // Deassert valid, req_en = old valid (1) so still enabled
    d->pipe_tag_rsp_valid_i = 0;
    sim.tick();
    // req_valid_d will be 0, so output should go to 0
    sim.check(d->l2hpf_req_valid_o == 0, "valid deasserted next cycle");

    // ── Test 13: Different reqq entry ──────────────────────
    printf("── Test 13: Different reqq entry\n");
    clear(d);
    d->rs_idx_i = 63;
    d->rs_source_i = 0x2;
    d->rs_tag_id_i = 0xFF;
    d->rs_orig_opcode_i = OrigWrite;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_opcode_i = L2Write;
    d->pipe_tag_rsp_reqq_id_i = 63;
    d->pipe_tag_rsp_orig_address_i = 0;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_err_rsp_i = ErrSbe;
    sim.tick();
    sim.check(d->l2hpf_req_valid_o == 1, "entry 63 valid");
    sim.check(d->l2hpf_req_source_o == 0x2, "entry 63 source");
    sim.check(d->l2hpf_req_id_o == 0xFF, "entry 63 tag_id");
    sim.check(d->l2hpf_req_opcode_o == ReqWrite, "OrigWrite → ReqWrite");

    // ── Test 14: Address passthrough ───────────────────────
    printf("── Test 14: Address passthrough\n");
    clear(d);
    d->rs_idx_i = 0;
    d->rs_orig_opcode_i = OrigRead;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_opcode_i = L2Read;
    d->pipe_tag_rsp_reqq_id_i = 0;
    d->pipe_tag_rsp_orig_address_i = 0x87654321;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_err_rsp_i = ErrSbe;
    sim.tick();
    // Address is 40 bits; we only set low 32. Check those.
    uint64_t addr = (uint64_t)d->l2hpf_req_address_o;
    sim.check((addr & 0xFFFFFFFF) == 0x87654321, "address passthrough");

    return sim.finish();
}

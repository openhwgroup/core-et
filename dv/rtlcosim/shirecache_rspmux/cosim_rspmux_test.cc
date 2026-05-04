// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_rspmux + shire_cache_bank_rspmux_xbar_arb
//        vs shirecache_bank_rspmux

#include "Vcosim_rspmux_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

using DUT = Vcosim_rspmux_tb;

// ── Constants ─────────────────────────────────────────────────

// Ports = 5 (NumNeigh=4 + RboxPerShire=1)
static constexpr int NUM_PORTS = 5;
// ReqqDepth = 64
static constexpr int NUM_ENTRIES = 64;

// reqq_orig_opcode_e values
static constexpr int OrigRead          = 0x00;
static constexpr int OrigReadCoop      = 0x04;
static constexpr int OrigMsgSendData   = 0x05;
static constexpr int OrigScpReadCoop   = 0x0a;

// pipe_data_rsp_type_e values
static constexpr int NormalRsp  = 0;
static constexpr int VictimData = 1;

// reqq_source_e values
static constexpr int ReqqSrcL2  = 0;
static constexpr int ReqqSrcL3  = 1;

// ── Helpers ───────────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->bank_rsp_ready_i      = 0x1F;  // all 5 ports ready
    d->bank_rsp_2_credits_i  = 0x1F;  // all 5 ports have 2+ credits

    d->pipe_rbuf_rd_rsp_valid_i = 0;
    memset(&d->pipe_rbuf_rsp_data_i, 0, sizeof(d->pipe_rbuf_rsp_data_i));
    d->pipe_rbuf_rsp_trans_id_i = 0;
    d->pipe_rbuf_rsp_reqq_id_i  = 0;
    d->pipe_rbuf_rsp_l3_source_i = 0;

    d->pipe_data_rsp_valid_i = 0;
    memset(&d->pipe_data_rsp_data_i, 0, sizeof(d->pipe_data_rsp_data_i));
    d->pipe_data_rsp_trans_id_i    = 0;
    d->pipe_data_rsp_reqq_id_i     = 0;
    d->pipe_data_rsp_type_i        = NormalRsp;
    d->pipe_data_rsp_tag_sbe_i     = 0;
    d->pipe_data_rsp_qwen_i        = 0;
    d->pipe_data_rsp_data_index_i  = 0;
    d->pipe_data_rsp_sbe_i         = 0;
    d->pipe_data_rsp_dbe_i         = 0;
    d->pipe_data_rsp_mod_trans_id_i  = 0;
    d->pipe_data_rsp_mod_reqq_id_i   = 0;
    d->pipe_data_rsp_mod_dataq_id_i  = 0;
    d->pipe_data_rsp_mod_addr_space_i = 0;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    d->pipe_data_rsp_mod_which_mesh_i  = 0;
    d->pipe_data_rsp_mod_disable_mesh_bypass_i = 0;
    d->pipe_data_rsp_mod_redirect_i = 0;
    d->pipe_data_rsp_mod_port_id_i  = 0;
    d->pipe_data_rsp_mod_rsp_type_i = NormalRsp;

    d->to_l3_mesh_rsp_valid_i    = 0;
    d->to_l3_mesh_rsp_trans_id_i = 0;
    d->to_l3_mesh_rsp_reqq_id_i  = 0;
    d->to_l3_mesh_rsp_dataq_id_i = 0;
    memset(&d->to_l3_mesh_rsp_data_i, 0, sizeof(d->to_l3_mesh_rsp_data_i));
    d->to_l3_mesh_rsp_zero_data_i = 0;
    d->to_l3_mesh_rsp_err_i       = 0;

    d->to_sys_mesh_rsp_valid_i    = 0;
    d->to_sys_mesh_rsp_trans_id_i = 0;
    d->to_sys_mesh_rsp_reqq_id_i  = 0;
    d->to_sys_mesh_rsp_dataq_id_i = 0;
    memset(&d->to_sys_mesh_rsp_data_i, 0, sizeof(d->to_sys_mesh_rsp_data_i));
    d->to_sys_mesh_rsp_zero_data_i = 0;
    d->to_sys_mesh_rsp_err_i       = 0;

    d->reqq_rspmux_valid_nodata_i = 0;
    d->reqq_rspmux_trans_id_i     = 0;
    d->reqq_rspmux_reqq_id_i      = 0;
    memset(&d->reqq_rspmux_data_i, 0, sizeof(d->reqq_rspmux_data_i));
    d->reqq_rspmux_with_data_i    = 0;
    d->reqq_rspmux_zero_data_i    = 0;
    d->reqq_rspmux_qwen_i         = 0;

    d->dataq_rspmux_valid_i       = 0;
    d->dataq_rspmux_trans_id_i    = 0;
    d->dataq_rspmux_reqq_id_i     = 0;
    memset(&d->dataq_rspmux_data_i, 0, sizeof(d->dataq_rspmux_data_i));
    d->dataq_rspmux_with_data_i   = 0;
    d->dataq_rspmux_zero_data_i   = 0;
    d->dataq_rspmux_qwen_i        = 0;

    d->esr_err_rsp_enable_i       = 0;
    d->esr_err_int_sbe_enable_i   = 0;
    d->esr_err_int_dbe_enable_i   = 0;

    // Default reqq state entries: entry 0 at index 0, entry 1 at index 1
    d->rs_idx0_i            = 0;
    d->rs_port_id0_i        = 0;
    d->rs_orig_opcode0_i    = OrigRead;
    d->rs_tag_id0_i         = 0;
    d->rs_source0_i         = 0;
    d->rs_err_detected0_i   = 0;
    d->rs_err_zero_data0_i  = 0;
    d->rs_traced0_i         = 0;
    d->rs_size0_i           = 0;
    d->rs_rspmux_qwen0_i    = 0xF;
    d->rs_readcoop_dest0_i  = 0;

    d->rs_idx1_i            = 1;
    d->rs_port_id1_i        = 1;
    d->rs_orig_opcode1_i    = OrigRead;
    d->rs_tag_id1_i         = 0;
    d->rs_source1_i         = 0;
    d->rs_err_detected1_i   = 0;
    d->rs_err_zero_data1_i  = 0;
    d->rs_traced1_i         = 0;
    d->rs_size1_i           = 0;
    d->rs_rspmux_qwen1_i    = 0xF;
    d->rs_readcoop_dest1_i  = 0;
}

static void fill_data(VlWide<16>& w, uint32_t seed) {
    for (int i = 0; i < 16; i++)
        w[i] = seed * 0x01010101u + i * 0x13579BDFu;
}

// ── Compare helpers ───────────────────────────────────────────

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("bank_rsp_valid",
                (uint32_t)d->orig_bank_rsp_valid_o,
                (uint32_t)d->new_bank_rsp_valid_o);

    // Only compare xbar fields when at least one bank_rsp is valid
    if (d->orig_bank_rsp_valid_o || d->new_bank_rsp_valid_o) {
        sim.compare("bank_rsp_port_id",
                    (uint32_t)d->orig_bank_rsp_port_id_o,
                    (uint32_t)d->new_bank_rsp_port_id_o);
        sim.compare("bank_rsp_src_is_uc",
                    (uint32_t)d->orig_bank_rsp_src_is_uc_o,
                    (uint32_t)d->new_bank_rsp_src_is_uc_o);
        sim.compare("bank_rsp_id",
                    (uint32_t)d->orig_bank_rsp_id_o,
                    (uint32_t)d->new_bank_rsp_id_o);
        sim.compare("bank_rsp_dest",
                    (uint32_t)d->orig_bank_rsp_dest_o,
                    (uint32_t)d->new_bank_rsp_dest_o);
        sim.compare("bank_rsp_wdata",
                    (uint32_t)d->orig_bank_rsp_wdata_o,
                    (uint32_t)d->new_bank_rsp_wdata_o);
        sim.compare("bank_rsp_opcode",
                    (uint32_t)d->orig_bank_rsp_opcode_o,
                    (uint32_t)d->new_bank_rsp_opcode_o);
        sim.compare("bank_rsp_size",
                    (uint32_t)d->orig_bank_rsp_size_o,
                    (uint32_t)d->new_bank_rsp_size_o);
        sim.compare("bank_rsp_qwen",
                    (uint32_t)d->orig_bank_rsp_qwen_o,
                    (uint32_t)d->new_bank_rsp_qwen_o);
    }

    sim.compare("pipe_rbuf_rsp_ready",
                (uint32_t)d->orig_pipe_rbuf_rsp_ready_o,
                (uint32_t)d->new_pipe_rbuf_rsp_ready_o);
    sim.compare("pipe_data_rsp_ready",
                (uint32_t)d->orig_pipe_data_rsp_ready_o,
                (uint32_t)d->new_pipe_data_rsp_ready_o);
    sim.compare("to_l3_mesh_rsp_ready",
                (uint32_t)d->orig_to_l3_mesh_rsp_ready_o,
                (uint32_t)d->new_to_l3_mesh_rsp_ready_o);
    sim.compare("to_sys_mesh_rsp_ready",
                (uint32_t)d->orig_to_sys_mesh_rsp_ready_o,
                (uint32_t)d->new_to_sys_mesh_rsp_ready_o);
    sim.compare("reqq_rspmux_ready_nodata",
                (uint32_t)d->orig_reqq_rspmux_ready_nodata_o,
                (uint32_t)d->new_reqq_rspmux_ready_nodata_o);
    sim.compare("dataq_rspmux_ready",
                (uint32_t)d->orig_dataq_rspmux_ready_o,
                (uint32_t)d->new_dataq_rspmux_ready_o);
    sim.compare("a_rspmux_rsp_is_valid",
                (uint32_t)d->orig_a_rspmux_rsp_is_valid_o,
                (uint32_t)d->new_a_rspmux_rsp_is_valid_o);

    // rspmux_sent_valid_hot — compare the full 64-bit vector
    sim.compare("rspmux_sent_valid_hot",
                (uint64_t)d->orig_rspmux_sent_valid_hot_o,
                (uint64_t)d->new_rspmux_sent_valid_hot_o);

    sim.compare("trace_sent_valid",
                (uint32_t)d->orig_rspmux_trace_sent_valid_o,
                (uint32_t)d->new_rspmux_trace_sent_valid_o);
    sim.compare("trace_sent_reqq_id",
                (uint32_t)d->orig_rspmux_trace_sent_reqq_id_o,
                (uint32_t)d->new_rspmux_trace_sent_reqq_id_o);
    sim.compare("trace_sent_traced",
                (uint32_t)d->orig_rspmux_trace_sent_traced_o,
                (uint32_t)d->new_rspmux_trace_sent_traced_o);
}

// ── Main ──────────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Reset + idle ─────────────────────────────────
    printf("phase 1: reset + idle\n");
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);

    // ── Phase 2: Source 0 (rbuf read rsp) ─────────────────────
    printf("phase 2: source 0 (rbuf)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 5;
    d->rs_port_id0_i     = 2;
    d->rs_tag_id0_i      = 0xAB;
    d->rs_source0_i      = 0x3;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;  // SIZE_LINE

    d->pipe_rbuf_rd_rsp_valid_i = 1;
    d->pipe_rbuf_rsp_reqq_id_i  = 5;
    d->pipe_rbuf_rsp_trans_id_i = 0x42;
    d->pipe_rbuf_rsp_l3_source_i = 0;
    fill_data(d->pipe_rbuf_rsp_data_i, 0xAA);
    sim.tick();
    compare_all(sim);
    // Let it drain through FIFO
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 3: Source 1 (pipe data_rsp NormalRsp, L2) ───────
    printf("phase 3: source 1 (pipe data_rsp NormalRsp)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 10;
    d->rs_port_id0_i     = 1;
    d->rs_tag_id0_i      = 0x42;
    d->rs_source0_i      = 0x5;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->pipe_data_rsp_valid_i       = 1;
    d->pipe_data_rsp_reqq_id_i     = 10;
    d->pipe_data_rsp_trans_id_i    = 0x55;
    d->pipe_data_rsp_type_i        = NormalRsp;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    d->pipe_data_rsp_mod_rsp_type_i    = NormalRsp;
    d->pipe_data_rsp_mod_port_id_i     = 1;
    fill_data(d->pipe_data_rsp_data_i, 0xBB);
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 4: Source 1 filtered (VictimRsp) ────────────────
    printf("phase 4: source 1 filtered (VictimRsp)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 10;
    d->rs_port_id0_i     = 1;

    d->pipe_data_rsp_valid_i       = 1;
    d->pipe_data_rsp_reqq_id_i     = 10;
    d->pipe_data_rsp_type_i        = VictimData;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    d->pipe_data_rsp_mod_rsp_type_i    = VictimData;  // filtered
    d->pipe_data_rsp_mod_port_id_i     = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 5: Source 1 filtered (L3 source) ────────────────
    printf("phase 5: source 1 filtered (L3 source)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 10;
    d->rs_port_id0_i     = 1;

    d->pipe_data_rsp_valid_i       = 1;
    d->pipe_data_rsp_reqq_id_i     = 10;
    d->pipe_data_rsp_type_i        = NormalRsp;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;  // filtered
    d->pipe_data_rsp_mod_rsp_type_i    = NormalRsp;
    d->pipe_data_rsp_mod_port_id_i     = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 6: Source 2 (l3_mesh_rsp) ───────────────────────
    printf("phase 6: source 2 (l3_mesh_rsp)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 20;
    d->rs_port_id0_i     = 3;
    d->rs_tag_id0_i      = 0x11;
    d->rs_source0_i      = 0x7;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->to_l3_mesh_rsp_valid_i    = 1;
    d->to_l3_mesh_rsp_reqq_id_i  = 20;
    d->to_l3_mesh_rsp_trans_id_i = 0x66;
    fill_data(d->to_l3_mesh_rsp_data_i, 0xCC);
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 7: Source 3 (sys_mesh_rsp) ──────────────────────
    printf("phase 7: source 3 (sys_mesh_rsp)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 30;
    d->rs_port_id0_i     = 4;
    d->rs_tag_id0_i      = 0x22;
    d->rs_source0_i      = 0x9;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->to_sys_mesh_rsp_valid_i    = 1;
    d->to_sys_mesh_rsp_reqq_id_i  = 30;
    d->to_sys_mesh_rsp_trans_id_i = 0x77;
    fill_data(d->to_sys_mesh_rsp_data_i, 0xDD);
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 8: Source 4 (reqq nodata) ───────────────────────
    printf("phase 8: source 4 (reqq nodata)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 40;
    d->rs_port_id0_i     = 0;
    d->rs_tag_id0_i      = 0x33;
    d->rs_source0_i      = 0x2;
    d->rs_rspmux_qwen0_i = 0x0;
    d->rs_size0_i        = 6;

    d->reqq_rspmux_valid_nodata_i = 1;
    d->reqq_rspmux_reqq_id_i      = 40;
    d->reqq_rspmux_trans_id_i     = 0x88;
    d->reqq_rspmux_with_data_i    = 0;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 9: Source 5 (dataq wdata) ───────────────────────
    printf("phase 9: source 5 (dataq wdata)\n");
    clear_inputs(d);
    d->rs_idx0_i         = 50;
    d->rs_port_id0_i     = 2;
    d->rs_tag_id0_i      = 0x44;
    d->rs_source0_i      = 0x1;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->dataq_rspmux_valid_i    = 1;
    d->dataq_rspmux_reqq_id_i  = 50;
    d->dataq_rspmux_trans_id_i = 0x99;
    fill_data(d->dataq_rspmux_data_i, 0xEE);
    d->dataq_rspmux_with_data_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 10: Multiple sources simultaneously ─────────────
    printf("phase 10: multiple sources\n");
    clear_inputs(d);
    // Source 0 on port 0
    d->rs_idx0_i         = 0;
    d->rs_port_id0_i     = 0;
    d->rs_tag_id0_i      = 0xA1;
    d->rs_source0_i      = 0x1;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;
    // Source 5 on port 1
    d->rs_idx1_i         = 1;
    d->rs_port_id1_i     = 1;
    d->rs_tag_id1_i      = 0xB2;
    d->rs_source1_i      = 0x2;
    d->rs_rspmux_qwen1_i = 0xF;
    d->rs_size1_i        = 6;

    d->pipe_rbuf_rd_rsp_valid_i = 1;
    d->pipe_rbuf_rsp_reqq_id_i  = 0;
    d->pipe_rbuf_rsp_trans_id_i = 0xA0;
    fill_data(d->pipe_rbuf_rsp_data_i, 0x10);

    d->dataq_rspmux_valid_i    = 1;
    d->dataq_rspmux_reqq_id_i  = 1;
    d->dataq_rspmux_trans_id_i = 0xB0;
    fill_data(d->dataq_rspmux_data_i, 0x20);
    d->dataq_rspmux_with_data_i = 1;

    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 6; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 11: Backpressure (bank_rsp_ready=0) ─────────────
    printf("phase 11: backpressure\n");
    clear_inputs(d);
    d->rs_idx0_i         = 2;
    d->rs_port_id0_i     = 0;
    d->rs_tag_id0_i      = 0x55;
    d->rs_source0_i      = 0x3;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->bank_rsp_ready_i     = 0;  // no ports ready
    d->bank_rsp_2_credits_i = 0;

    d->pipe_rbuf_rd_rsp_valid_i = 1;
    d->pipe_rbuf_rsp_reqq_id_i  = 2;
    d->pipe_rbuf_rsp_trans_id_i = 0xC1;
    fill_data(d->pipe_rbuf_rsp_data_i, 0x30);
    sim.tick();
    compare_all(sim);
    // Fill the FIFO
    sim.tick();
    compare_all(sim);
    sim.tick();
    compare_all(sim);
    // Release backpressure
    d->pipe_rbuf_rd_rsp_valid_i = 0;
    d->bank_rsp_ready_i     = 0x1F;
    d->bank_rsp_2_credits_i = 0x1F;
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 12: Broadcast response (ReadCoop) ───────────────
    printf("phase 12: broadcast (ReadCoop)\n");
    clear_inputs(d);
    d->rs_idx0_i            = 3;
    d->rs_port_id0_i        = 0;
    d->rs_orig_opcode0_i    = OrigReadCoop;
    d->rs_readcoop_dest0_i  = 0x07;  // ports 0, 1, 2
    d->rs_tag_id0_i         = 0x66;
    d->rs_source0_i         = 0x4;
    d->rs_rspmux_qwen0_i    = 0xF;
    d->rs_size0_i           = 6;

    d->pipe_rbuf_rd_rsp_valid_i = 1;
    d->pipe_rbuf_rsp_reqq_id_i  = 3;
    d->pipe_rbuf_rsp_trans_id_i = 0xD1;
    fill_data(d->pipe_rbuf_rsp_data_i, 0x40);
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 6; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 13: Credit-based flow control ───────────────────
    printf("phase 13: credit-based flow control\n");
    clear_inputs(d);
    d->rs_idx0_i         = 4;
    d->rs_port_id0_i     = 0;
    d->rs_tag_id0_i      = 0x77;
    d->rs_source0_i      = 0x5;
    d->rs_rspmux_qwen0_i = 0xF;
    d->rs_size0_i        = 6;

    d->bank_rsp_ready_i     = 0x01;  // only port 0 has 1 credit
    d->bank_rsp_2_credits_i = 0x00;  // no 2-credit ports

    d->to_l3_mesh_rsp_valid_i    = 1;
    d->to_l3_mesh_rsp_reqq_id_i  = 4;
    d->to_l3_mesh_rsp_trans_id_i = 0xE1;
    fill_data(d->to_l3_mesh_rsp_data_i, 0x50);
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->bank_rsp_ready_i     = 0x1F;
    d->bank_rsp_2_credits_i = 0x1F;
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 14: Sequential responses from each source ───────
    printf("phase 14: sequential responses\n");
    for (int src = 0; src < 6; src++) {
        clear_inputs(d);
        d->rs_idx0_i         = src;
        d->rs_port_id0_i     = src % NUM_PORTS;
        d->rs_tag_id0_i      = 0x10 + src;
        d->rs_source0_i      = src;
        d->rs_rspmux_qwen0_i = 0xF;
        d->rs_size0_i        = 6;

        switch (src) {
        case 0:
            d->pipe_rbuf_rd_rsp_valid_i = 1;
            d->pipe_rbuf_rsp_reqq_id_i  = 0;
            d->pipe_rbuf_rsp_trans_id_i = 0xF0;
            break;
        case 1:
            d->pipe_data_rsp_valid_i       = 1;
            d->pipe_data_rsp_reqq_id_i     = 1;
            d->pipe_data_rsp_trans_id_i    = 0xF1;
            d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
            d->pipe_data_rsp_mod_rsp_type_i    = NormalRsp;
            d->pipe_data_rsp_mod_port_id_i     = 1;
            break;
        case 2:
            d->to_l3_mesh_rsp_valid_i    = 1;
            d->to_l3_mesh_rsp_reqq_id_i  = 2;
            d->to_l3_mesh_rsp_trans_id_i = 0xF2;
            break;
        case 3:
            d->to_sys_mesh_rsp_valid_i    = 1;
            d->to_sys_mesh_rsp_reqq_id_i  = 3;
            d->to_sys_mesh_rsp_trans_id_i = 0xF3;
            break;
        case 4:
            d->reqq_rspmux_valid_nodata_i = 1;
            d->reqq_rspmux_reqq_id_i      = 4;
            d->reqq_rspmux_trans_id_i     = 0xF4;
            d->reqq_rspmux_with_data_i    = 0;
            break;
        case 5:
            d->dataq_rspmux_valid_i    = 1;
            d->dataq_rspmux_reqq_id_i  = 5 % NUM_PORTS;  // use valid port index
            // Reassign rs_idx0 to match
            d->rs_idx0_i = 5 % NUM_PORTS;
            d->rs_port_id0_i = 5 % NUM_PORTS;
            d->dataq_rspmux_trans_id_i = 0xF5;
            d->dataq_rspmux_with_data_i = 1;
            break;
        }
        sim.tick();
        compare_all(sim);
        // Drain
        clear_inputs(d);
        for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }
    }

    // ── Phase 15: Error response (err_detected) ───────────────
    printf("phase 15: error response\n");
    clear_inputs(d);
    d->rs_idx0_i            = 7;
    d->rs_port_id0_i        = 0;
    d->rs_err_detected0_i   = 1;
    d->rs_err_zero_data0_i  = 1;
    d->rs_tag_id0_i         = 0x88;
    d->rs_source0_i         = 0x6;
    d->rs_rspmux_qwen0_i    = 0xF;
    d->rs_size0_i           = 6;
    d->esr_err_rsp_enable_i = 1;

    d->reqq_rspmux_valid_nodata_i = 1;
    d->reqq_rspmux_reqq_id_i      = 7;
    d->reqq_rspmux_trans_id_i     = 0xAA;
    d->reqq_rspmux_with_data_i    = 0;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 16: MsgSendData response ────────────────────────
    printf("phase 16: MsgSendData response\n");
    clear_inputs(d);
    d->rs_idx0_i            = 8;
    d->rs_port_id0_i        = 2;
    d->rs_orig_opcode0_i    = OrigMsgSendData;
    d->rs_tag_id0_i         = 0x99;
    d->rs_source0_i         = 0x7;
    d->rs_rspmux_qwen0_i    = 0xF;
    d->rs_size0_i           = 6;

    d->reqq_rspmux_valid_nodata_i = 1;
    d->reqq_rspmux_reqq_id_i      = 8;
    d->reqq_rspmux_trans_id_i     = 0xBB;
    d->reqq_rspmux_with_data_i    = 0;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) { sim.tick(); compare_all(sim); }

    // ── Phase 17: ECC SBE filtering ───────────────────────────
    printf("phase 17: ECC SBE filtering\n");
    clear_inputs(d);
    d->rs_idx0_i         = 10;
    d->rs_port_id0_i     = 1;
    d->esr_err_int_sbe_enable_i = 1;

    d->pipe_data_rsp_valid_i       = 1;
    d->pipe_data_rsp_reqq_id_i     = 10;
    d->pipe_data_rsp_sbe_i         = 0x01;  // SBE detected
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    d->pipe_data_rsp_mod_rsp_type_i    = NormalRsp;
    d->pipe_data_rsp_mod_port_id_i     = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 18: ECC DBE filtering ───────────────────────────
    printf("phase 18: ECC DBE filtering\n");
    clear_inputs(d);
    d->rs_idx0_i         = 10;
    d->rs_port_id0_i     = 1;
    d->esr_err_int_dbe_enable_i = 1;

    d->pipe_data_rsp_valid_i       = 1;
    d->pipe_data_rsp_reqq_id_i     = 10;
    d->pipe_data_rsp_dbe_i         = 0x01;  // DBE detected
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    d->pipe_data_rsp_mod_rsp_type_i    = NormalRsp;
    d->pipe_data_rsp_mod_port_id_i     = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);

    // ── Phase 19: Random stimulus ─────────────────────────────
    printf("phase 19: random stimulus (500 cycles)\n");
    srand(12345);
    for (int cycle = 0; cycle < 500; cycle++) {
        clear_inputs(d);

        // Random reqq state entries
        int idx0 = rand() % NUM_ENTRIES;
        int idx1 = rand() % NUM_ENTRIES;
        if (idx1 == idx0) idx1 = (idx0 + 1) % NUM_ENTRIES;
        d->rs_idx0_i         = idx0;
        d->rs_port_id0_i     = rand() % NUM_PORTS;
        d->rs_orig_opcode0_i = (rand() % 8 == 0) ? OrigReadCoop :
                               (rand() % 8 == 1) ? OrigMsgSendData : OrigRead;
        d->rs_tag_id0_i      = rand() & 0xFF;
        d->rs_source0_i      = rand() & 0xF;
        d->rs_err_detected0_i = (rand() % 16 == 0);
        d->rs_err_zero_data0_i = d->rs_err_detected0_i && (rand() % 2);
        d->rs_traced0_i      = (rand() % 8 == 0);
        d->rs_size0_i        = rand() & 0x7;
        d->rs_rspmux_qwen0_i = rand() & 0xF;
        d->rs_readcoop_dest0_i = rand() & 0xF;

        d->rs_idx1_i         = idx1;
        d->rs_port_id1_i     = rand() % NUM_PORTS;
        d->rs_orig_opcode1_i = OrigRead;
        d->rs_tag_id1_i      = rand() & 0xFF;
        d->rs_source1_i      = rand() & 0xF;
        d->rs_rspmux_qwen1_i = rand() & 0xF;
        d->rs_size1_i        = rand() & 0x7;

        d->esr_err_rsp_enable_i     = (rand() % 4 == 0);
        d->esr_err_int_sbe_enable_i = (rand() % 8 == 0);
        d->esr_err_int_dbe_enable_i = (rand() % 8 == 0);

        d->bank_rsp_ready_i     = rand() & 0x1F;
        d->bank_rsp_2_credits_i = rand() & 0x1F;

        // Randomly activate sources
        if (rand() % 4 == 0) {
            d->pipe_rbuf_rd_rsp_valid_i = 1;
            d->pipe_rbuf_rsp_reqq_id_i  = idx0;
            d->pipe_rbuf_rsp_trans_id_i = rand();
            fill_data(d->pipe_rbuf_rsp_data_i, rand());
        }
        if (rand() % 4 == 0) {
            d->pipe_data_rsp_valid_i       = 1;
            d->pipe_data_rsp_reqq_id_i     = idx0;
            d->pipe_data_rsp_trans_id_i    = rand();
            d->pipe_data_rsp_type_i        = rand() % 2;
            d->pipe_data_rsp_sbe_i         = (rand() % 16 == 0) ? 1 : 0;
            d->pipe_data_rsp_dbe_i         = (rand() % 16 == 0) ? 1 : 0;
            d->pipe_data_rsp_mod_reqq_source_i = rand() % 3;
            d->pipe_data_rsp_mod_rsp_type_i    = rand() % 2;
            d->pipe_data_rsp_mod_port_id_i     = d->rs_port_id0_i;
        }
        if (rand() % 4 == 0) {
            d->to_l3_mesh_rsp_valid_i    = 1;
            d->to_l3_mesh_rsp_reqq_id_i  = idx0;
            d->to_l3_mesh_rsp_trans_id_i = rand();
        }
        if (rand() % 4 == 0) {
            d->to_sys_mesh_rsp_valid_i    = 1;
            d->to_sys_mesh_rsp_reqq_id_i  = idx1;
            d->to_sys_mesh_rsp_trans_id_i = rand();
        }
        if (rand() % 4 == 0) {
            d->reqq_rspmux_valid_nodata_i = 1;
            d->reqq_rspmux_reqq_id_i      = idx0;
            d->reqq_rspmux_trans_id_i     = rand();
            d->reqq_rspmux_with_data_i    = rand() % 2;
        }
        if (rand() % 4 == 0) {
            d->dataq_rspmux_valid_i    = 1;
            d->dataq_rspmux_reqq_id_i  = idx1;
            d->dataq_rspmux_trans_id_i = rand();
            d->dataq_rspmux_with_data_i = 1;
        }

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

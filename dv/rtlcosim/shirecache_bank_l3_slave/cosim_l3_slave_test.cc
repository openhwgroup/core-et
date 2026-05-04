// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_l3_slave vs shirecache_bank_l3_slave

#include "Vcosim_l3_slave_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cassert>

using DUT = Vcosim_l3_slave_tb;

// reqq_source_e values
static constexpr int ReqqSrcL2 = 0;
static constexpr int ReqqSrcL3 = 1;

// pipe_data_rsp_type_e
static constexpr int NormalRsp  = 0;
static constexpr int VictimData = 1;

// reqq_orig_opcode_e values
static constexpr int OrigRead    = 0x00;
static constexpr int OrigScpRead = 0x0a;

// AxiQosMemHighPriority
static constexpr int QosHighPrio = 1;

// -- Helpers --

static void fill_data(VlWide<16>& w, uint32_t seed) {
    for (int i = 0; i < 16; i++)
        w[i] = seed * 0x01010101u + i * 0x13579BDFu;
}

static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                          const uint32_t* orig, const uint32_t* newv, int words) {
    bool match = memcmp(orig, newv, words * sizeof(uint32_t)) == 0;
    sim.comparisons++;
    if (!match) {
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] %s:", (unsigned long)sim.sim_time, name);
        for (int i = words-1; i >= 0; i--)
            printf(" [%d]=%08x/%08x", i, orig[i], newv[i]);
        printf("\n");
    }
}

static void clear_inputs(DUT* d) {
    // Request
    d->req_valid_i    = 0;
    d->req_trans_id_i = 0;
    d->req_id_i       = 0;
    d->req_port_id_i  = 0;
    d->req_source_i   = 0;
    d->req_opcode_i   = 0;
    d->req_address_i  = 0;
    d->req_size_i     = 6; // 64B line
    d->req_qos_i      = 0;
    d->req_wdata_i    = 0;
    d->req_qwen_i     = 0xF;
    memset(&d->req_data_i, 0, sizeof(d->req_data_i));
    d->req_ben_en_i   = 0;
    d->req_ben_i      = 0;

    // Handshake
    d->rsp_ready_i     = 1;
    d->bank_l3_ready_i = 1;

    // Source 0: pipe data rsp
    d->pipe_data_rsp_valid_i      = 0;
    d->pipe_data_rsp_trans_id_i   = 0;
    d->pipe_data_rsp_reqq_id_i    = 0;
    d->pipe_data_rsp_type_i       = NormalRsp;
    d->pipe_data_rsp_tag_sbe_i    = 0;
    memset(&d->pipe_data_rsp_data_i, 0, sizeof(d->pipe_data_rsp_data_i));
    d->pipe_data_rsp_qwen_i       = 0;
    d->pipe_data_rsp_data_index_i = 0;
    d->pipe_data_rsp_sbe_i        = 0;
    d->pipe_data_rsp_dbe_i        = 0;
    d->pipe_data_rsp_mod_trans_id_i           = 0;
    d->pipe_data_rsp_mod_reqq_id_i            = 0;
    d->pipe_data_rsp_mod_dataq_id_i           = 0;
    d->pipe_data_rsp_mod_addr_space_i         = 0;
    d->pipe_data_rsp_mod_reqq_source_i        = ReqqSrcL3;
    d->pipe_data_rsp_mod_which_mesh_i         = 0;
    d->pipe_data_rsp_mod_disable_mesh_bypass_i = 0;
    d->pipe_data_rsp_mod_redirect_i           = 0;
    d->pipe_data_rsp_mod_port_id_i            = 0;
    d->pipe_data_rsp_mod_rsp_type_i           = NormalRsp;

    // Source 1: to_sys mesh rsp
    d->to_sys_rsp_valid_i     = 0;
    d->to_sys_rsp_trans_id_i  = 0;
    d->to_sys_rsp_reqq_id_i   = 0;
    d->to_sys_rsp_dataq_id_i  = 0;
    memset(&d->to_sys_rsp_data_i, 0, sizeof(d->to_sys_rsp_data_i));
    d->to_sys_rsp_zero_data_i = 0;
    d->to_sys_rsp_err_i       = 0;

    // Source 2: reqq rspmux nodata
    d->reqq_rspmux_valid_i     = 0;
    d->reqq_rspmux_trans_id_i  = 0;
    d->reqq_rspmux_reqq_id_i   = 0;
    memset(&d->reqq_rspmux_data_i, 0, sizeof(d->reqq_rspmux_data_i));
    d->reqq_rspmux_with_data_i = 0;
    d->reqq_rspmux_zero_data_i = 0;
    d->reqq_rspmux_qwen_i      = 0;

    // Source 3: dataq rspmux
    d->dataq_rspmux_valid_i     = 0;
    d->dataq_rspmux_trans_id_i  = 0;
    d->dataq_rspmux_reqq_id_i   = 0;
    memset(&d->dataq_rspmux_data_i, 0, sizeof(d->dataq_rspmux_data_i));
    d->dataq_rspmux_with_data_i = 0;
    d->dataq_rspmux_zero_data_i = 0;
    d->dataq_rspmux_qwen_i      = 0;

    // Source 4: rbuf read rsp
    d->rbuf_rsp_valid_i     = 0;
    d->rbuf_rsp_trans_id_i  = 0;
    d->rbuf_rsp_reqq_id_i   = 0;
    d->rbuf_rsp_l3_source_i = 0;
    memset(&d->rbuf_rsp_data_i, 0, sizeof(d->rbuf_rsp_data_i));

    // ESR
    d->esr_err_rsp_enable_i  = 0;
    d->esr_err_int_enable_i  = 0;

    // Reqq state entries
    d->rs_idx0_i          = 0;
    d->rs_tag_id0_i       = 0;
    d->rs_source0_i       = 0;
    d->rs_port_id0_i      = 0;
    d->rs_orig_opcode0_i  = OrigRead;
    d->rs_err_detected0_i = 0;
    d->rs_err_zero_data0_i = 0;

    d->rs_idx1_i          = 1;
    d->rs_tag_id1_i       = 0;
    d->rs_source1_i       = 0;
    d->rs_port_id1_i      = 1;
    d->rs_orig_opcode1_i  = OrigRead;
    d->rs_err_detected1_i = 0;
    d->rs_err_zero_data1_i = 0;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("req_ready",
                (uint32_t)d->orig_req_ready_o,
                (uint32_t)d->new_req_ready_o);
    sim.compare("bank_l3_valid",
                (uint32_t)d->orig_bank_l3_valid_o,
                (uint32_t)d->new_bank_l3_valid_o);
    if (d->orig_bank_l3_valid_o || d->new_bank_l3_valid_o) {
        sim.compare("bank_l3_address",
                    (uint64_t)d->orig_bank_l3_address_o,
                    (uint64_t)d->new_bank_l3_address_o);
        // Data/ben only valid when wdata/ben_en=1 (original en_ff_gated_struct
        // power-gates data registers when don't-care)
        if (d->req_wdata_i && d->req_qwen_i)
            compare_wide(sim, "bank_l3_data",
                         d->orig_bank_l3_data_o.data(),
                         d->new_bank_l3_data_o.data(), 16);
        if (d->req_ben_en_i)
            sim.compare("bank_l3_ben",
                        (uint64_t)d->orig_bank_l3_ben_o,
                        (uint64_t)d->new_bank_l3_ben_o);
    }
    sim.compare("pipe_data_rsp_ready",
                (uint32_t)d->orig_pipe_data_rsp_ready_o,
                (uint32_t)d->new_pipe_data_rsp_ready_o);
    sim.compare("to_sys_rsp_ready",
                (uint32_t)d->orig_to_sys_rsp_ready_o,
                (uint32_t)d->new_to_sys_rsp_ready_o);
    sim.compare("reqq_rspmux_ready",
                (uint32_t)d->orig_reqq_rspmux_ready_o,
                (uint32_t)d->new_reqq_rspmux_ready_o);
    sim.compare("dataq_rspmux_ready",
                (uint32_t)d->orig_dataq_rspmux_ready_o,
                (uint32_t)d->new_dataq_rspmux_ready_o);
    sim.compare("rbuf_rsp_ready",
                (uint32_t)d->orig_rbuf_rsp_ready_o,
                (uint32_t)d->new_rbuf_rsp_ready_o);
    sim.compare("rsp_valid",
                (uint32_t)d->orig_rsp_valid_o,
                (uint32_t)d->new_rsp_valid_o);
    if (d->orig_rsp_valid_o || d->new_rsp_valid_o) {
        sim.compare("rsp_wdata",
                    (uint32_t)d->orig_rsp_wdata_o,
                    (uint32_t)d->new_rsp_wdata_o);
        sim.compare("rsp_err",
                    (uint32_t)d->orig_rsp_err_o,
                    (uint32_t)d->new_rsp_err_o);
        // rsp data only valid when output wdata=1 (original en_ff_gated_struct
        // power-gates data register when wdata=0)
        if (d->orig_rsp_wdata_o)
            compare_wide(sim, "rsp_data",
                         d->orig_rsp_data_o.data(),
                         d->new_rsp_data_o.data(), 16);
    }
    sim.compare("rspmux_sent_valid",
                (uint32_t)d->orig_rspmux_sent_valid_o,
                (uint32_t)d->new_rspmux_sent_valid_o);
    if (d->orig_rspmux_sent_valid_o || d->new_rspmux_sent_valid_o) {
        sim.compare("rspmux_sent_id",
                    (uint32_t)d->orig_rspmux_sent_id_o,
                    (uint32_t)d->new_rspmux_sent_id_o);
    }
    sim.compare("a_rspmux_rsp_is_valid",
                (uint32_t)d->orig_a_rspmux_rsp_is_valid_o,
                (uint32_t)d->new_a_rspmux_rsp_is_valid_o);
}

static void tick_cmp(CosimCtrl<DUT>& sim) { sim.tick(); compare_all(sim); }

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset(5);

    // ================================================================
    // Phase 0: Idle cycles
    // ================================================================
    printf("Phase 0: idle cycles\n");
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ================================================================
    // Phase 1: Single request through input FIFO
    // ================================================================
    printf("Phase 1: single L3 request\n");
    clear_inputs(d);
    d->req_valid_i   = 1;
    d->req_trans_id_i = 0xAA;
    d->req_id_i       = 0x55;
    d->req_port_id_i  = 1;
    d->req_source_i   = 3;
    d->req_opcode_i   = 0;  // Read
    d->req_address_i  = 0x1000;
    d->req_size_i     = 6;
    d->req_qos_i      = QosHighPrio;
    d->req_wdata_i    = 0;
    d->req_qwen_i     = 0xF;
    d->req_ben_en_i   = 0;
    tick_cmp(sim);

    // Deassert request, let it flow through
    d->req_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 2: Response from source 0 (pipe data rsp, NormalRsp, L3)
    // ================================================================
    printf("Phase 2: pipe data rsp (source 0)\n");
    clear_inputs(d);
    d->rs_idx0_i          = 2;
    d->rs_tag_id0_i       = 0x42;
    d->rs_source0_i       = 5;
    d->rs_port_id0_i      = 1;
    d->rs_orig_opcode0_i  = OrigRead;
    d->rs_err_detected0_i = 0;
    d->rs_err_zero_data0_i = 0;

    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_trans_id_i = 0xBB;
    d->pipe_data_rsp_reqq_id_i  = 2;
    d->pipe_data_rsp_type_i     = NormalRsp;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;
    d->pipe_data_rsp_mod_redirect_i    = 0;
    fill_data(d->pipe_data_rsp_data_i, 0x11);
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 3: Response from source 1 (to_sys mesh rsp)
    // ================================================================
    printf("Phase 3: to_sys mesh rsp (source 1)\n");
    clear_inputs(d);
    d->rs_idx0_i     = 3;
    d->rs_tag_id0_i  = 0x10;
    d->rs_source0_i  = 2;
    d->rs_port_id0_i = 0;
    d->rs_orig_opcode0_i = OrigRead;

    d->to_sys_rsp_valid_i    = 1;
    d->to_sys_rsp_trans_id_i = 0xCC;
    d->to_sys_rsp_reqq_id_i  = 3;
    fill_data(d->to_sys_rsp_data_i, 0x22);
    tick_cmp(sim);

    d->to_sys_rsp_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 4: Response from source 2 (reqq nodata)
    // ================================================================
    printf("Phase 4: reqq nodata (source 2)\n");
    clear_inputs(d);
    d->rs_idx0_i     = 4;
    d->rs_tag_id0_i  = 0x20;
    d->rs_source0_i  = 1;
    d->rs_port_id0_i = 2;
    d->rs_orig_opcode0_i = OrigRead;

    d->reqq_rspmux_valid_i     = 1;
    d->reqq_rspmux_trans_id_i  = 0xDD;
    d->reqq_rspmux_reqq_id_i   = 4;
    d->reqq_rspmux_with_data_i = 1;
    tick_cmp(sim);

    d->reqq_rspmux_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 5: Response from source 3 (dataq rspmux)
    // ================================================================
    printf("Phase 5: dataq rspmux (source 3)\n");
    clear_inputs(d);
    d->rs_idx0_i     = 5;
    d->rs_tag_id0_i  = 0x30;
    d->rs_source0_i  = 7;
    d->rs_port_id0_i = 0;
    d->rs_orig_opcode0_i = OrigRead;

    d->dataq_rspmux_valid_i    = 1;
    d->dataq_rspmux_trans_id_i = 0xEE;
    d->dataq_rspmux_reqq_id_i  = 5;
    fill_data(d->dataq_rspmux_data_i, 0x33);
    tick_cmp(sim);

    d->dataq_rspmux_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 6: Response from source 4 (rbuf read rsp, l3_source=1)
    // ================================================================
    printf("Phase 6: rbuf read rsp (source 4)\n");
    clear_inputs(d);
    d->rs_idx0_i     = 6;
    d->rs_tag_id0_i  = 0x40;
    d->rs_source0_i  = 9;
    d->rs_port_id0_i = 1;
    d->rs_orig_opcode0_i = OrigRead;

    d->rbuf_rsp_valid_i     = 1;
    d->rbuf_rsp_trans_id_i  = 0xFF;
    d->rbuf_rsp_reqq_id_i   = 6;
    d->rbuf_rsp_l3_source_i = 1;
    fill_data(d->rbuf_rsp_data_i, 0x44);
    tick_cmp(sim);

    d->rbuf_rsp_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 7: Round-robin arbitration (multiple sources)
    // ================================================================
    printf("Phase 7: round-robin arbitration\n");
    clear_inputs(d);
    d->rs_idx0_i     = 0;
    d->rs_tag_id0_i  = 0x50;
    d->rs_source0_i  = 1;
    d->rs_port_id0_i = 0;
    d->rs_orig_opcode0_i = OrigRead;
    d->rs_idx1_i     = 1;
    d->rs_tag_id1_i  = 0x60;
    d->rs_source1_i  = 2;
    d->rs_port_id1_i = 1;
    d->rs_orig_opcode1_i = OrigRead;

    // Present sources 1 and 2 simultaneously
    d->to_sys_rsp_valid_i    = 1;
    d->to_sys_rsp_trans_id_i = 0x11;
    d->to_sys_rsp_reqq_id_i  = 0;
    fill_data(d->to_sys_rsp_data_i, 0xAA);

    d->reqq_rspmux_valid_i     = 1;
    d->reqq_rspmux_trans_id_i  = 0x22;
    d->reqq_rspmux_reqq_id_i   = 1;
    d->reqq_rspmux_with_data_i = 0;

    for (int i = 0; i < 6; i++) tick_cmp(sim);

    d->to_sys_rsp_valid_i  = 0;
    d->reqq_rspmux_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 8: Error-zero gating
    // ================================================================
    printf("Phase 8: error-zero gating\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->rs_idx0_i     = 7;
    d->rs_tag_id0_i  = 0x70;
    d->rs_source0_i  = 3;
    d->rs_port_id0_i = 0;
    d->rs_orig_opcode0_i  = OrigRead;
    d->rs_err_detected0_i = 1;
    d->rs_err_zero_data0_i = 1;

    // Source 1 with error zero data
    d->to_sys_rsp_valid_i    = 1;
    d->to_sys_rsp_trans_id_i = 0x33;
    d->to_sys_rsp_reqq_id_i  = 7;
    fill_data(d->to_sys_rsp_data_i, 0xBB);
    tick_cmp(sim);

    d->to_sys_rsp_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 9: Backpressure (rsp_ready=0 fills response FIFO)
    // ================================================================
    printf("Phase 9: backpressure\n");
    clear_inputs(d);
    d->rsp_ready_i = 0;

    d->rs_idx0_i     = 8;
    d->rs_tag_id0_i  = 0x80;
    d->rs_source0_i  = 4;
    d->rs_port_id0_i = 0;
    d->rs_orig_opcode0_i = OrigRead;

    // Push responses while consumer is stalled
    d->to_sys_rsp_valid_i    = 1;
    d->to_sys_rsp_trans_id_i = 0x44;
    d->to_sys_rsp_reqq_id_i  = 8;
    fill_data(d->to_sys_rsp_data_i, 0xCC);

    for (int i = 0; i < 6; i++) tick_cmp(sim);

    // Release backpressure
    d->rsp_ready_i = 1;
    d->to_sys_rsp_valid_i = 0;
    for (int i = 0; i < 6; i++) tick_cmp(sim);

    // ================================================================
    // Phase 10: rbuf rsp with l3_source=0 (should not be valid)
    // ================================================================
    printf("Phase 10: rbuf rsp l3_source=0 (filtered)\n");
    clear_inputs(d);
    d->rbuf_rsp_valid_i     = 1;
    d->rbuf_rsp_trans_id_i  = 0x55;
    d->rbuf_rsp_reqq_id_i   = 0;
    d->rbuf_rsp_l3_source_i = 0;  // NOT l3 source
    fill_data(d->rbuf_rsp_data_i, 0xDD);
    tick_cmp(sim);

    d->rbuf_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 11: pipe data rsp filtered (VictimData type)
    // ================================================================
    printf("Phase 11: pipe data rsp VictimData (filtered)\n");
    clear_inputs(d);
    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_type_i     = VictimData;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 12: pipe data rsp filtered (reqq_source != L3)
    // ================================================================
    printf("Phase 12: pipe data rsp non-L3 source (filtered)\n");
    clear_inputs(d);
    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_type_i     = NormalRsp;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL2;
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 13: pipe data rsp filtered (redirect=1)
    // ================================================================
    printf("Phase 13: pipe data rsp redirect (filtered)\n");
    clear_inputs(d);
    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_type_i     = NormalRsp;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;
    d->pipe_data_rsp_mod_redirect_i    = 1;
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 14: ECC SBE filter
    // ================================================================
    printf("Phase 14: ECC SBE filter\n");
    clear_inputs(d);
    d->esr_err_int_enable_i = 0x01;  // bit 0 = SBE enable
    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_type_i     = NormalRsp;
    d->pipe_data_rsp_sbe_i      = 1;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 15: ECC DBE filter
    // ================================================================
    printf("Phase 15: ECC DBE filter\n");
    clear_inputs(d);
    d->esr_err_int_enable_i = 0x02;  // bit 1 = DBE enable
    d->pipe_data_rsp_valid_i    = 1;
    d->pipe_data_rsp_type_i     = NormalRsp;
    d->pipe_data_rsp_dbe_i      = 1;
    d->pipe_data_rsp_mod_reqq_source_i = ReqqSrcL3;
    tick_cmp(sim);

    d->pipe_data_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    // ================================================================
    // Phase 16: Request FIFO backpressure (bank_l3_ready=0)
    // ================================================================
    printf("Phase 16: request FIFO backpressure\n");
    clear_inputs(d);
    d->bank_l3_ready_i = 0;

    d->req_valid_i    = 1;
    d->req_trans_id_i = 0x77;
    d->req_id_i       = 0x11;
    d->req_opcode_i   = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // Release
    d->bank_l3_ready_i = 1;
    d->req_valid_i     = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 17: 500-cycle random stimulus
    // ================================================================
    printf("Phase 17: random stimulus (500 cycles)\n");
    srand(42);
    for (int cyc = 0; cyc < 500; cyc++) {
        clear_inputs(d);

        // Random request
        d->req_valid_i    = rand() & 1;
        d->req_trans_id_i = rand() & 0xFF;
        d->req_id_i       = rand() & 0x1FF;
        d->req_port_id_i  = rand() & 3;
        d->req_source_i   = rand() & 0x3FF;
        d->req_opcode_i   = rand() % 17;
        { uint64_t a = (uint64_t)rand() | ((uint64_t)rand() << 32);
          d->req_address_i = a & ((1ULL << 40) - 1); }
        d->req_size_i     = rand() & 7;
        d->req_qos_i      = rand() & 0xF;
        // Force data-valid enables so original en_ff_gated_struct stores all data
        d->req_wdata_i    = 1;
        d->req_qwen_i     = 0xF;
        d->req_ben_en_i   = 1;
        { uint64_t b = (uint64_t)rand() | ((uint64_t)rand() << 32);
          d->req_ben_i = b; }
        fill_data(d->req_data_i, rand());

        // Random handshake
        d->rsp_ready_i     = (rand() % 4) != 0;
        d->bank_l3_ready_i = (rand() % 4) != 0;

        // Random source 0
        d->pipe_data_rsp_valid_i       = rand() & 1;
        d->pipe_data_rsp_trans_id_i    = rand() & 0xFF;
        d->pipe_data_rsp_reqq_id_i     = rand() & 0x3F;
        d->pipe_data_rsp_type_i        = rand() & 1;
        d->pipe_data_rsp_sbe_i         = rand() & 0xFF;
        d->pipe_data_rsp_dbe_i         = rand() & 0xFF;
        d->pipe_data_rsp_mod_reqq_source_i = rand() & 3;
        d->pipe_data_rsp_mod_redirect_i    = rand() & 1;
        fill_data(d->pipe_data_rsp_data_i, rand());

        // Random source 1
        d->to_sys_rsp_valid_i    = rand() & 1;
        d->to_sys_rsp_trans_id_i = rand() & 0xFF;
        d->to_sys_rsp_reqq_id_i  = rand() & 0x3F;
        fill_data(d->to_sys_rsp_data_i, rand());

        // Random source 2
        d->reqq_rspmux_valid_i     = rand() & 1;
        d->reqq_rspmux_trans_id_i  = rand() & 0xFF;
        d->reqq_rspmux_reqq_id_i   = rand() & 0x3F;
        d->reqq_rspmux_with_data_i = rand() & 1;
        fill_data(d->reqq_rspmux_data_i, rand());

        // Random source 3
        d->dataq_rspmux_valid_i    = rand() & 1;
        d->dataq_rspmux_trans_id_i = rand() & 0xFF;
        d->dataq_rspmux_reqq_id_i  = rand() & 0x3F;
        fill_data(d->dataq_rspmux_data_i, rand());

        // Random source 4
        d->rbuf_rsp_valid_i     = rand() & 1;
        d->rbuf_rsp_trans_id_i  = rand() & 0xFF;
        d->rbuf_rsp_reqq_id_i   = rand() & 0x3F;
        d->rbuf_rsp_l3_source_i = rand() & 1;
        fill_data(d->rbuf_rsp_data_i, rand());

        // Random ESR
        d->esr_err_rsp_enable_i = rand() & 1;
        d->esr_err_int_enable_i = rand() & 0xFF;

        // Random reqq state
        int idx0 = rand() & 0x3F;
        int idx1 = rand() & 0x3F;
        // Avoid both entries targeting the same index
        if (idx1 == idx0) idx1 = (idx0 + 1) & 0x3F;
        d->rs_idx0_i           = idx0;
        d->rs_tag_id0_i        = rand() & 0xFF;
        d->rs_source0_i        = rand() & 0xF;
        d->rs_port_id0_i       = rand() & 7;
        d->rs_orig_opcode0_i   = rand() & 0x1F;
        d->rs_err_detected0_i  = rand() & 1;
        d->rs_err_zero_data0_i = rand() & 1;

        d->rs_idx1_i           = idx1;
        d->rs_tag_id1_i        = rand() & 0xFF;
        d->rs_source1_i        = rand() & 0xF;
        d->rs_port_id1_i       = rand() & 7;
        d->rs_orig_opcode1_i   = rand() & 0x1F;
        d->rs_err_detected1_i  = rand() & 1;
        d->rs_err_zero_data1_i = rand() & 1;

        tick_cmp(sim);
    }

    // ================================================================
    // Phase 18: Request data passthrough with known pattern
    // ================================================================
    printf("Phase 18: request data passthrough\n");
    clear_inputs(d);
    d->req_valid_i    = 1;
    d->req_trans_id_i = 0x99;
    d->req_id_i       = 0x77;
    d->req_port_id_i  = 2;
    d->req_source_i   = 0x1A;
    d->req_opcode_i   = 0;
    d->req_address_i  = 0xDEADBEEF00ULL & ((1ULL << 40) - 1);
    d->req_size_i     = 6;
    d->req_qos_i      = QosHighPrio;
    d->req_wdata_i    = 0;
    d->req_qwen_i     = 0xF;
    d->req_ben_en_i   = 1;
    d->req_ben_i      = 0xFFFFFFFFFFFFFFFFULL;
    fill_data(d->req_data_i, 0xA5);
    d->bank_l3_ready_i = 1;
    d->rsp_ready_i     = 1;

    // Wait for bank_l3_valid and check data output
    for (int i = 0; i < 6; i++) {
        tick_cmp(sim);
        if (d->orig_bank_l3_valid_o) break;
    }
    d->req_valid_i = 0;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // ================================================================
    // Phase 19: Response data passthrough with non-zero data
    // ================================================================
    printf("Phase 19: response data passthrough\n");
    clear_inputs(d);
    d->rsp_ready_i = 0;  // Hold response visible

    d->rs_idx0_i     = 10;
    d->rs_tag_id0_i  = 0xAB;
    d->rs_source0_i  = 6;
    d->rs_port_id0_i = 1;
    d->rs_orig_opcode0_i = OrigRead;

    d->to_sys_rsp_valid_i    = 1;
    d->to_sys_rsp_trans_id_i = 0x88;
    d->to_sys_rsp_reqq_id_i  = 10;
    fill_data(d->to_sys_rsp_data_i, 0xC3);
    tick_cmp(sim);

    d->to_sys_rsp_valid_i = 0;
    // Keep rsp_ready=0 to hold response at output
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    // Release and drain
    d->rsp_ready_i = 1;
    for (int i = 0; i < 4; i++) tick_cmp(sim);

    return sim.finish();
}

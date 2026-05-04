// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_mesh_master vs shirecache_mesh_master.
// NumBanks=4, NumPorts=1, ToL3=1. Same clock for both domains.

#include "Vcosim_mesh_master_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vcosim_mesh_master_tb;

// Opcode constants (from etlink_pkg)
static constexpr uint8_t ReqWrite     = 0;
static constexpr uint8_t ReqRead      = 1;
static constexpr uint8_t ReqFlush     = 8;
static constexpr uint8_t ReqAtomicRsp = 16;

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("bank_req_ready", d->orig_bank_req_ready_o, d->new_bank_req_ready_o);
    sim.compare("bank_rsp_valid", d->orig_bank_rsp_valid_o, d->new_bank_rsp_valid_o);
    sim.compare("axi_ar_valid",   d->orig_axi_ar_valid_o,   d->new_axi_ar_valid_o);
    sim.compare("axi_aw_valid",   d->orig_axi_aw_valid_o,   d->new_axi_aw_valid_o);
    sim.compare("axi_w_valid",    d->orig_axi_w_valid_o,    d->new_axi_w_valid_o);
    sim.compare("axi_r_ready",    d->orig_axi_r_ready_o,    d->new_axi_r_ready_o);
    sim.compare("axi_b_ready",    d->orig_axi_b_ready_o,    d->new_axi_b_ready_o);
}

static void tick_cmp(CosimCtrl<DUT>& sim) { sim.tick(); compare_all(sim); }

static void clear(DUT* d) {
    d->bank_req_valid_i = 0;
    d->bank_clk_en_i    = 0xF;
    d->bank_rsp_ready_i = 0xF;
    d->axi_ar_ready_i   = 1;
    d->axi_aw_ready_i   = 1;
    d->axi_w_ready_i    = 1;
    d->axi_r_valid_i    = 0;
    d->axi_b_valid_i    = 0;
    d->axi_r_id_i       = 0;
    d->axi_r_resp_i     = 0;
    d->axi_b_id_i       = 0;
    d->axi_b_resp_i     = 0;
    d->swizzle_ctl_i    = 0;
    // Clear all bank request fields
    d->b0_req_trans_id_i = 0; d->b0_req_id_i = 0; d->b0_req_port_id_i = 0;
    d->b0_req_source_i = 0; d->b0_req_opcode_i = 0; d->b0_req_address_i = 0;
    d->b0_req_size_i = 0; d->b0_req_qos_i = 0; d->b0_req_wdata_i = 0;
    d->b0_req_qwen_i = 0; d->b0_req_ben_en_i = 0; d->b0_req_ben_i = 0;
    memset(&d->b0_req_data_i, 0, sizeof(d->b0_req_data_i));
    d->b1_req_trans_id_i = 0; d->b1_req_id_i = 0; d->b1_req_port_id_i = 0;
    d->b1_req_source_i = 0; d->b1_req_opcode_i = 0; d->b1_req_address_i = 0;
    d->b1_req_size_i = 0; d->b1_req_qos_i = 0; d->b1_req_wdata_i = 0;
    d->b1_req_qwen_i = 0; d->b1_req_ben_en_i = 0; d->b1_req_ben_i = 0;
    memset(&d->b1_req_data_i, 0, sizeof(d->b1_req_data_i));
    d->b2_req_trans_id_i = 0; d->b2_req_id_i = 0; d->b2_req_port_id_i = 0;
    d->b2_req_source_i = 0; d->b2_req_opcode_i = 0; d->b2_req_address_i = 0;
    d->b2_req_size_i = 0; d->b2_req_qos_i = 0; d->b2_req_wdata_i = 0;
    d->b2_req_qwen_i = 0; d->b2_req_ben_en_i = 0; d->b2_req_ben_i = 0;
    memset(&d->b2_req_data_i, 0, sizeof(d->b2_req_data_i));
    d->b3_req_trans_id_i = 0; d->b3_req_id_i = 0; d->b3_req_port_id_i = 0;
    d->b3_req_source_i = 0; d->b3_req_opcode_i = 0; d->b3_req_address_i = 0;
    d->b3_req_size_i = 0; d->b3_req_qos_i = 0; d->b3_req_wdata_i = 0;
    d->b3_req_qwen_i = 0; d->b3_req_ben_en_i = 0; d->b3_req_ben_i = 0;
    memset(&d->b3_req_data_i, 0, sizeof(d->b3_req_data_i));
    memset(&d->axi_r_data_i, 0, sizeof(d->axi_r_data_i));
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 500000;
    clear(d);

    sim.reset(10);
    // Let both modules settle
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    // ── Phase 1: idle ────────────────────────────────────────
    printf("phase 1: idle\n");
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 2: single read request from bank 0 ─────────────
    printf("phase 2: read from bank 0\n");
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 5;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x1000;
    d->b0_req_size_i    = 6;
    d->b0_req_qos_i     = 2;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 3: write request from bank 0 ───────────────────
    printf("phase 3: write from bank 0\n");
    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 7;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x2000;
    d->b0_req_size_i    = 6;
    d->b0_req_qos_i     = 1;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0xF;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 4: R response ──────────────────────────────────
    printf("phase 4: R response\n");
    // First send a read
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 10;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x3000;
    d->b0_req_size_i    = 6;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // Push R response
    d->axi_r_valid_i = 1;
    d->axi_r_id_i    = (0 << 6) | 10;
    d->axi_r_resp_i  = 0;  // OKAY
    d->axi_r_data_i[0] = 0xCAFEBABE;
    tick_cmp(sim);
    d->axi_r_valid_i = 0;
    memset(&d->axi_r_data_i, 0, sizeof(d->axi_r_data_i));
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 5: B response ──────────────────────────────────
    printf("phase 5: B response\n");
    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 11;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x4000;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0xF;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    d->axi_b_valid_i = 1;
    d->axi_b_id_i    = (0 << 6) | 11;
    d->axi_b_resp_i  = 0;
    tick_cmp(sim);
    d->axi_b_valid_i = 0;
    for (int i = 0; i < 15; i++) tick_cmp(sim);

    // ── Phase 6: backpressure ────────────────────────────────
    printf("phase 6: backpressure\n");
    d->axi_ar_ready_i = 0;
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 20;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x5000;
    d->b0_req_size_i    = 6;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 10; i++) tick_cmp(sim);
    d->axi_ar_ready_i = 1;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // Bank rsp backpressure
    d->bank_rsp_ready_i = 0;
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 21;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x5100;
    d->b0_req_size_i    = 6;
    d->bank_req_valid_i = 0x1;
    tick_cmp(sim);
    clear(d);
    d->bank_rsp_ready_i = 0;
    for (int i = 0; i < 15; i++) tick_cmp(sim);
    d->axi_r_valid_i = 1;
    d->axi_r_id_i = (0 << 6) | 21;
    d->axi_r_resp_i = 0;
    tick_cmp(sim);
    d->axi_r_valid_i = 0;
    for (int i = 0; i < 10; i++) tick_cmp(sim);
    d->bank_rsp_ready_i = 0xF;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    // ── Phase 7: multiple banks ──────────────────────────────
    printf("phase 7: multiple banks\n");
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 30;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x6000;
    d->b0_req_size_i    = 6;
    d->b1_req_opcode_i  = ReqRead;
    d->b1_req_id_i      = 31;
    d->b1_req_source_i  = 1;
    d->b1_req_address_i = 0x6100;
    d->b1_req_size_i    = 6;
    d->bank_req_valid_i = 0x3;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 20; i++) tick_cmp(sim);

    // ── Phase 8: all 4 banks ─────────────────────────────────
    printf("phase 8: all 4 banks\n");
    d->b0_req_opcode_i = ReqRead; d->b0_req_id_i = 40; d->b0_req_source_i = 0;
    d->b0_req_address_i = 0x7000; d->b0_req_size_i = 6;
    d->b1_req_opcode_i = ReqRead; d->b1_req_id_i = 41; d->b1_req_source_i = 1;
    d->b1_req_address_i = 0x7100; d->b1_req_size_i = 6;
    d->b2_req_opcode_i = ReqRead; d->b2_req_id_i = 42; d->b2_req_source_i = 2;
    d->b2_req_address_i = 0x7200; d->b2_req_size_i = 6;
    d->b3_req_opcode_i = ReqRead; d->b3_req_id_i = 43; d->b3_req_source_i = 3;
    d->b3_req_address_i = 0x7300; d->b3_req_size_i = 6;
    d->bank_req_valid_i = 0xF;
    tick_cmp(sim);
    clear(d);
    for (int i = 0; i < 30; i++) tick_cmp(sim);

    // ── Phase 9: random (500 cycles) ─────────────────────────
    printf("phase 9: random (500 cycles)\n");
    uint32_t seed = 0xBEEF1234;
    for (int i = 0; i < 500; i++) {
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

        // Full pressure: prim_fifo_async now correctly supports Depth=6
        // (count-based full detection matching original vcfifo behavior).
        d->bank_req_valid_i = seed & 0xF;
        d->bank_clk_en_i    = 0xF;
        d->bank_rsp_ready_i = 0xF;
        d->axi_ar_ready_i   = (seed >> 4) & 1;
        d->axi_aw_ready_i   = (seed >> 5) & 1;
        d->axi_w_ready_i    = (seed >> 6) & 1;

        // Bank 0
        d->b0_req_opcode_i  = (seed >> 7) & 1;  // Read or Write
        d->b0_req_id_i      = (seed >> 8) & 0x3F;
        d->b0_req_source_i  = 0;
        d->b0_req_address_i = ((uint64_t)(seed >> 10)) << 6;
        d->b0_req_size_i    = 6;
        d->b0_req_wdata_i   = !((seed >> 7) & 1);
        d->b0_req_qwen_i    = 0xF;

        // Bank 1
        d->b1_req_opcode_i  = (seed >> 14) & 1;
        d->b1_req_id_i      = (seed >> 15) & 0x3F;
        d->b1_req_source_i  = 1;
        d->b1_req_address_i = ((uint64_t)(seed >> 17)) << 6;
        d->b1_req_size_i    = 6;
        d->b1_req_wdata_i   = !((seed >> 14) & 1);
        d->b1_req_qwen_i    = 0xF;

        // Bank 2
        d->b2_req_opcode_i  = (seed >> 20) & 1;
        d->b2_req_id_i      = (seed >> 21) & 0x3F;
        d->b2_req_source_i  = 2;
        d->b2_req_address_i = ((uint64_t)(seed >> 23)) << 6;
        d->b2_req_size_i    = 6;

        // Bank 3
        d->b3_req_opcode_i  = (seed >> 26) & 1;
        d->b3_req_id_i      = (seed >> 27) & 0x1F;
        d->b3_req_source_i  = 3;
        d->b3_req_address_i = ((uint64_t)(seed >> 28)) << 6;
        d->b3_req_size_i    = 6;

        // AXI responses (occasional)
        seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;
        d->axi_r_valid_i = (seed & 0x7) == 0;  // ~12.5% chance
        d->axi_r_id_i    = (seed >> 3) & 0x1FF;
        d->axi_r_resp_i  = 0;
        d->axi_b_valid_i = (seed & 0x38) == 0;  // ~12.5% chance
        d->axi_b_id_i    = (seed >> 6) & 0x1FF;
        d->axi_b_resp_i  = 0;

        tick_cmp(sim);
    }

    // Drain
    clear(d);
    for (int i = 0; i < 30; i++) tick_cmp(sim);

    return sim.finish();
}

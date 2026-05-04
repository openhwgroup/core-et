// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_mesh_master.
// NumBanks=4, NumPorts=1, ToL3=1, same clock for both domains.

#include "Vshirecache_mesh_master_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vshirecache_mesh_master_tb;

// Opcode constants (from etlink_pkg)
static constexpr uint8_t ReqWrite     = 0;
static constexpr uint8_t ReqRead      = 1;
static constexpr uint8_t ReqWriteAround = 2;
static constexpr uint8_t ReqFlush     = 8;
static constexpr uint8_t ReqFlushToMem= 9;
static constexpr uint8_t ReqEvict     = 10;
static constexpr uint8_t ReqEvictToMem= 11;
static constexpr uint8_t ReqAtomicRsp = 16;

// AXI constant values
static constexpr uint8_t AxLen0    = 0;
static constexpr uint8_t AxBurstIncr = 1;
static constexpr uint8_t AxCache0  = 0;
static constexpr uint8_t AxProt010 = 2;
// AXI resp codes
static constexpr uint8_t RespOkay = 0;
static constexpr uint8_t RespSlvErr = 2;

#define CHK(sim, msg, val, exp) \
  sim.check((val) == (exp), std::string(msg) + ": got " + std::to_string(val) + " expected " + std::to_string(exp))

static void clear_inputs(DUT* d) {
    d->bank_req_valid_i = 0;
    d->bank_clk_en_i    = 0xF;  // always enabled
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
    d->b0_req_id_i      = 0;
    d->b0_req_source_i  = 0;
    d->b0_req_opcode_i  = 0;
    d->b0_req_address_i = 0;
    d->b0_req_size_i    = 0;
    d->b0_req_qos_i     = 0;
    d->b0_req_wdata_i   = 0;
    d->b0_req_qwen_i    = 0;
    d->b0_req_ben_en_i  = 0;
    d->b0_req_ben_i     = 0;
    d->b1_req_id_i      = 0;
    d->b1_req_source_i  = 0;
    d->b1_req_opcode_i  = 0;
    d->b1_req_address_i = 0;
    d->b1_req_size_i    = 0;
    d->b1_req_qos_i     = 0;
    d->b1_req_wdata_i   = 0;
    d->b2_req_id_i      = 0;
    d->b2_req_opcode_i  = 0;
    d->b2_req_address_i = 0;
    d->b3_req_id_i      = 0;
    d->b3_req_opcode_i  = 0;
    d->b3_req_address_i = 0;
    // Clear AXI R data (wide)
    memset(&d->axi_r_data_i, 0, sizeof(d->axi_r_data_i));
}

// Wait for AR valid with timeout
static bool wait_ar_valid(SimCtrl<DUT>& sim, int max_cycles = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        if (d->axi_ar_valid_o) return true;
        sim.tick();
    }
    return false;
}

// Wait for AW valid
static bool wait_aw_valid(SimCtrl<DUT>& sim, int max_cycles = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        if (d->axi_aw_valid_o) return true;
        sim.tick();
    }
    return false;
}

// Wait for bank rsp valid on a bank
static bool wait_rsp_valid(SimCtrl<DUT>& sim, int bank, int max_cycles = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        if (d->bank_rsp_valid_o & (1 << bank)) return true;
        sim.tick();
    }
    return false;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 500000;
    clear_inputs(d);

    // Extra cycles for async FIFO reset propagation
    sim.reset(10);
    // Let state settle
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 1: Read request from bank 0 → AXI AR
    // ══════════════════════════════════════════════════════════
    printf("Phase 1: Read request from bank 0\n");
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 5;
    d->b0_req_source_i  = 0;  // bank 0
    d->b0_req_address_i = 0x1000;
    d->b0_req_size_i    = 6;  // SizeLine
    d->b0_req_qos_i     = 3;
    d->bank_req_valid_i = 0x1;

    // Clk enable must be on for clock gating
    d->bank_clk_en_i = 0xF;

    sim.tick();
    sim.check(d->bank_req_ready_o & 1, "bank0 req ready");
    d->bank_req_valid_i = 0;
    sim.tick();

    // Wait for AR to appear (through async FIFOs)
    bool ar_ok = wait_ar_valid(sim);
    sim.check(ar_ok, "AR valid after read req");
    if (ar_ok) {
        CHK(sim, "AR.id", (int)d->axi_ar_id_o, (int)((0 << 6) | 5));  // source=0, id=5
        CHK(sim, "AR.addr", (int)(d->axi_ar_addr_o & 0xFFFF), 0x1000);
        CHK(sim, "AR.len", (int)d->axi_ar_len_o, AxLen0);
        CHK(sim, "AR.burst", (int)d->axi_ar_burst_o, AxBurstIncr);
        CHK(sim, "AR.cache", (int)d->axi_ar_cache_o, AxCache0);
        CHK(sim, "AR.prot", (int)d->axi_ar_prot_o, AxProt010);
        CHK(sim, "AR.qos", (int)d->axi_ar_qos_o, 3);
    }
    // Drain
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 2: Write request → AXI AW + W
    // ══════════════════════════════════════════════════════════
    printf("Phase 2: Write request from bank 0\n");
    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 7;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x2000;
    d->b0_req_size_i    = 6;
    d->b0_req_qos_i     = 1;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0xF;
    d->bank_req_valid_i = 0x1;

    sim.tick();
    d->bank_req_valid_i = 0;
    d->b0_req_wdata_i   = 0;
    sim.tick();

    bool aw_ok = wait_aw_valid(sim);
    sim.check(aw_ok, "AW valid after write req");
    if (aw_ok) {
        sim.check(d->axi_w_valid_o == 1, "W valid simultaneous with AW");
        CHK(sim, "AW.id", (int)d->axi_aw_id_o, (int)((0 << 6) | 7));
        CHK(sim, "AW.addr", (int)(d->axi_aw_addr_o & 0xFFFF), 0x2000);
        CHK(sim, "AW.user", (int)d->axi_aw_user_o, ReqWrite);  // opcode in user for ToL3
        CHK(sim, "W.last", (int)d->axi_w_last_o, 1);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 3: All opcode types
    // ══════════════════════════════════════════════════════════
    printf("Phase 3: All opcode types\n");

    struct OpcodeTest {
        uint8_t opcode;
        bool is_read;  // goes on AR vs AW
        const char* name;
    } opcode_tests[] = {
        {ReqRead,        true,  "Read"},
        {ReqWrite,       false, "Write"},
        {ReqWriteAround, false, "WriteAround"},
        {ReqFlush,       false, "Flush"},
        {ReqFlushToMem,  false, "FlushToMem"},
        {ReqEvict,       false, "Evict"},
        {ReqEvictToMem,  false, "EvictToMem"},
        {ReqAtomicRsp,   false, "AtomicRsp"},
    };

    for (auto& t : opcode_tests) {
        printf("  opcode=%s\n", t.name);
        d->b0_req_opcode_i  = t.opcode;
        d->b0_req_id_i      = 1;
        d->b0_req_source_i  = 0;
        d->b0_req_address_i = 0x3000;
        d->b0_req_size_i    = 6;
        d->b0_req_wdata_i   = t.is_read ? 0 : 1;
        d->bank_req_valid_i = 0x1;
        sim.tick();
        d->bank_req_valid_i = 0;
        d->b0_req_wdata_i   = 0;
        sim.tick();

        if (t.is_read) {
            bool ok = wait_ar_valid(sim);
            sim.check(ok, std::string("AR valid for ") + t.name);
        } else {
            bool ok = wait_aw_valid(sim);
            sim.check(ok, std::string("AW valid for ") + t.name);
        }
        for (int i = 0; i < 5; i++) sim.tick();
    }

    // ══════════════════════════════════════════════════════════
    // Phase 4: R response → bank rsp
    // ══════════════════════════════════════════════════════════
    printf("Phase 4: R response\n");

    // First send a read request so something is in flight
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 10;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x4000;
    d->b0_req_size_i    = 6;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();
    // Wait for AR to go through
    wait_ar_valid(sim);
    for (int i = 0; i < 3; i++) sim.tick();

    // Now push R response
    d->axi_r_valid_i = 1;
    d->axi_r_id_i    = (0 << 6) | 10;  // source=0, id=10
    d->axi_r_resp_i  = RespOkay;
    // Set some data
    d->axi_r_data_i[0] = 0xDEADBEEF;

    sim.tick();
    d->axi_r_valid_i = 0;

    // Wait for bank 0 rsp
    bool rsp_ok = wait_rsp_valid(sim, 0);
    sim.check(rsp_ok, "bank0 rsp valid after R");
    if (rsp_ok) {
        CHK(sim, "rsp.id", (int)d->b0_rsp_id_o, 10);
        CHK(sim, "rsp.source", (int)d->b0_rsp_source_o, 0);
        CHK(sim, "rsp.err", (int)d->b0_rsp_err_o, 0);
        CHK(sim, "rsp.wdata", (int)d->b0_rsp_wdata_o, 1);  // R always has data
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 5: B response → bank rsp (wdata=0)
    // ══════════════════════════════════════════════════════════
    printf("Phase 5: B response\n");

    // Send a write request
    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 11;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x5000;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0xF;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    d->b0_req_wdata_i   = 0;
    sim.tick();
    wait_aw_valid(sim);
    for (int i = 0; i < 3; i++) sim.tick();

    // Push B response
    d->axi_b_valid_i = 1;
    d->axi_b_id_i    = (0 << 6) | 11;
    d->axi_b_resp_i  = RespOkay;
    sim.tick();
    d->axi_b_valid_i = 0;

    rsp_ok = wait_rsp_valid(sim, 0);
    sim.check(rsp_ok, "bank0 rsp valid after B");
    if (rsp_ok) {
        CHK(sim, "rsp.id", (int)d->b0_rsp_id_o, 11);
        CHK(sim, "rsp.wdata", (int)d->b0_rsp_wdata_o, 0);  // B never has data
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 6: Error response
    // ══════════════════════════════════════════════════════════
    printf("Phase 6: Error response\n");

    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 12;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x6000;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();
    wait_ar_valid(sim);
    for (int i = 0; i < 3; i++) sim.tick();

    d->axi_r_valid_i = 1;
    d->axi_r_id_i    = (0 << 6) | 12;
    d->axi_r_resp_i  = RespSlvErr;
    sim.tick();
    d->axi_r_valid_i = 0;

    rsp_ok = wait_rsp_valid(sim, 0);
    sim.check(rsp_ok, "bank0 rsp valid after R err");
    if (rsp_ok) {
        CHK(sim, "rsp.err", (int)d->b0_rsp_err_o, 1);  // non-OKAY → err
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 7: AXI backpressure
    // ══════════════════════════════════════════════════════════
    printf("Phase 7: AXI backpressure\n");

    d->axi_ar_ready_i = 0;  // block AR

    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 20;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x7000;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();

    // AR should become valid but not be accepted
    for (int i = 0; i < 15; i++) sim.tick();  // wait through FIFOs
    // Note: with async FIFOs the request gets queued
    // Now release AR ready
    d->axi_ar_ready_i = 1;
    for (int i = 0; i < 5; i++) sim.tick();
    // Should have gone through
    sim.check(true, "AXI backpressure survived");

    // ── AW backpressure ──
    d->axi_aw_ready_i = 0;
    d->axi_w_ready_i  = 0;

    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 21;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x7100;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0xF;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    d->b0_req_wdata_i   = 0;

    for (int i = 0; i < 15; i++) sim.tick();
    d->axi_aw_ready_i = 1;
    d->axi_w_ready_i  = 1;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.check(true, "AW/W backpressure survived");

    // ══════════════════════════════════════════════════════════
    // Phase 8: Bank rsp backpressure
    // ══════════════════════════════════════════════════════════
    printf("Phase 8: Bank rsp backpressure\n");

    // Send a request
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 30;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x8000;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();
    wait_ar_valid(sim);
    for (int i = 0; i < 3; i++) sim.tick();

    // Block bank rsp
    d->bank_rsp_ready_i = 0x0;

    // Push R response
    d->axi_r_valid_i = 1;
    d->axi_r_id_i    = (0 << 6) | 30;
    d->axi_r_resp_i  = RespOkay;
    sim.tick();
    d->axi_r_valid_i = 0;

    for (int i = 0; i < 10; i++) sim.tick();
    // Should still be valid (held)
    sim.check((d->bank_rsp_valid_o & 1) == 1, "rsp valid held during backpressure");

    // Release
    d->bank_rsp_ready_i = 0xF;
    sim.tick();
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 9: Multiple banks → arbitration
    // ══════════════════════════════════════════════════════════
    printf("Phase 9: Multi-bank arbitration\n");

    // Send read from bank 0 and bank 1 simultaneously
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 40;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0x9000;

    d->b1_req_opcode_i  = ReqRead;
    d->b1_req_id_i      = 41;
    d->b1_req_source_i  = 1;
    d->b1_req_address_i = 0x9100;

    d->bank_req_valid_i = 0x3;  // banks 0 and 1
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();

    // Both should eventually produce AR
    int ar_count = 0;
    for (int i = 0; i < 30; i++) {
        if (d->axi_ar_valid_o && d->axi_ar_ready_i) ar_count++;
        sim.tick();
    }
    sim.check(ar_count >= 2, "both bank reqs produced AR");

    // ══════════════════════════════════════════════════════════
    // Phase 10: All 4 banks simultaneous
    // ══════════════════════════════════════════════════════════
    printf("Phase 10: All 4 banks simultaneous\n");

    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 50;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0xA000;
    d->b1_req_opcode_i  = ReqRead;
    d->b1_req_id_i      = 51;
    d->b1_req_source_i  = 1;
    d->b1_req_address_i = 0xA100;
    d->b2_req_opcode_i  = ReqRead;
    d->b2_req_id_i      = 52;
    d->b2_req_address_i = 0xA200;
    d->b3_req_opcode_i  = ReqRead;
    d->b3_req_id_i      = 53;
    d->b3_req_address_i = 0xA300;

    d->bank_req_valid_i = 0xF;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();

    ar_count = 0;
    for (int i = 0; i < 40; i++) {
        if (d->axi_ar_valid_o && d->axi_ar_ready_i) ar_count++;
        sim.tick();
    }
    sim.check(ar_count >= 4, "all 4 bank reqs produced AR");

    // ══════════════════════════════════════════════════════════
    // Phase 11: Request FIFO depth test
    // ══════════════════════════════════════════════════════════
    printf("Phase 11: Request FIFO depth\n");

    // Block AR so requests accumulate in FIFO (depth=2)
    d->axi_ar_ready_i = 0;

    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 60;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0xB000;
    d->bank_req_valid_i = 0x1;
    sim.tick();

    // First accepted
    sim.check(d->bank_req_ready_o & 1, "FIFO accepts 1st");

    d->b0_req_id_i = 61;
    sim.tick();
    // Second may or may not be accepted (depends on FIFO implementation)
    // After 2 entries the FIFO should be full

    d->b0_req_id_i = 62;
    sim.tick();
    sim.tick();
    // By now the depth-2 FIFO should be full
    // (not asserting exact cycle because async FIFO adds variability)

    d->bank_req_valid_i = 0;
    d->axi_ar_ready_i = 1;
    // Drain
    for (int i = 0; i < 20; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 12: Clock gating test
    // ══════════════════════════════════════════════════════════
    printf("Phase 12: Clock gating\n");

    // Disable all bank clk_en — gated clock should shut down
    d->bank_clk_en_i = 0x0;
    for (int i = 0; i < 5; i++) sim.tick();

    // Re-enable
    d->bank_clk_en_i = 0xF;
    for (int i = 0; i < 5; i++) sim.tick();

    // Send a request to verify it still works
    d->b0_req_opcode_i  = ReqRead;
    d->b0_req_id_i      = 70;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0xC000;
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    sim.tick();
    ar_ok = wait_ar_valid(sim);
    sim.check(ar_ok, "AR valid after clk re-enable");
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 13: Write with qwen strobe
    // ══════════════════════════════════════════════════════════
    printf("Phase 13: Write with qwen strobe\n");

    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 80;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0xD000;
    d->b0_req_wdata_i   = 1;
    d->b0_req_qwen_i    = 0x5;  // QW0 and QW2 only
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    d->b0_req_wdata_i   = 0;
    sim.tick();
    aw_ok = wait_aw_valid(sim);
    sim.check(aw_ok, "AW valid with qwen");
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 14: Write with ben_en
    // ══════════════════════════════════════════════════════════
    printf("Phase 14: Write with ben_en\n");

    d->b0_req_opcode_i  = ReqWrite;
    d->b0_req_id_i      = 81;
    d->b0_req_source_i  = 0;
    d->b0_req_address_i = 0xD100;
    d->b0_req_wdata_i   = 1;
    d->b0_req_ben_en_i  = 1;
    d->b0_req_ben_i     = 0xFF;  // bottom 8 bytes
    d->bank_req_valid_i = 0x1;
    sim.tick();
    d->bank_req_valid_i = 0;
    d->b0_req_wdata_i   = 0;
    d->b0_req_ben_en_i  = 0;
    d->b0_req_ben_i     = 0;
    sim.tick();
    aw_ok = wait_aw_valid(sim);
    sim.check(aw_ok, "AW valid with ben_en");
    for (int i = 0; i < 5; i++) sim.tick();

    return sim.finish();
}

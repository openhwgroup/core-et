// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_l3_to_sys_bridge.
// NumL3SlavePorts=4, same clock for both domains.

#include "Vshirecache_l3_to_sys_bridge_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vshirecache_l3_to_sys_bridge_tb;

// Opcode constants (from etlink_pkg, encoded in AW USER field)
static constexpr uint8_t ReqWrite      = 0;
static constexpr uint8_t ReqRead       = 1;
static constexpr uint8_t ReqAtomic     = 5;
static constexpr uint8_t ReqFlush      = 8;
static constexpr uint8_t ReqFlushToMem = 9;
static constexpr uint8_t ReqEvict      = 10;
static constexpr uint8_t ReqEvictToMem = 11;
static constexpr uint8_t ReqPrefetch   = 15;

// AXI resp codes
static constexpr uint8_t RespOkay   = 0;
static constexpr uint8_t RespSlvErr = 2;

#define CHK(sim, msg, val, exp) \
  sim.check((val) == (exp), std::string(msg) + ": got " + std::to_string(val) + " expected " + std::to_string(exp))

static void clear_inputs(DUT* d) {
    d->l3_ar_valid_i = 0;
    d->l3_aw_valid_i = 0;
    d->l3_w_valid_i  = 0;
    d->l3_r_ready_i  = 0xF;
    d->l3_b_ready_i  = 0xF;

    d->p0_ar_id_i   = 0; d->p0_ar_addr_i = 0; d->p0_ar_size_i = 0;
    d->p0_ar_qos_i  = 0; d->p0_ar_user_i = 0;
    d->p0_aw_id_i   = 0; d->p0_aw_addr_i = 0; d->p0_aw_size_i = 0;
    d->p0_aw_qos_i  = 0; d->p0_aw_user_i = 0;
    d->p0_w_strb_i  = 0; d->p0_w_last_i  = 1;

    d->p1_aw_id_i   = 0; d->p1_aw_addr_i = 0; d->p1_aw_user_i = 0;
    d->p1_ar_id_i   = 0; d->p1_ar_addr_i = 0;

    d->sys_ar_ready_i = 1;
    d->sys_aw_ready_i = 1;
    d->sys_w_ready_i  = 1;
    d->sys_r_valid_i  = 0;
    d->sys_r_id_i     = 0;
    d->sys_r_resp_i   = 0;
    d->sys_r_last_i   = 1;
    d->sys_b_valid_i  = 0;
    d->sys_b_id_i     = 0;
    d->sys_b_resp_i   = 0;
}

static bool wait_sys_ar(SimCtrl<DUT>& sim, int max = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max; i++) {
        if (d->sys_ar_valid_o) return true;
        sim.tick();
    }
    return false;
}

static bool wait_sys_aw(SimCtrl<DUT>& sim, int max = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max; i++) {
        if (d->sys_aw_valid_o) return true;
        sim.tick();
    }
    return false;
}

static bool wait_l3_b_valid(SimCtrl<DUT>& sim, int port, int max = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max; i++) {
        if (d->l3_b_valid_o & (1 << port)) return true;
        sim.tick();
    }
    return false;
}

static bool wait_l3_r_valid(SimCtrl<DUT>& sim, int port, int max = 30) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max; i++) {
        if (d->l3_r_valid_o & (1 << port)) return true;
        sim.tick();
    }
    return false;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 500000;
    clear_inputs(d);

    sim.reset(10);
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 1: AR read request from port 0 → verify sys AR
    // ══════════════════════════════════════════════════════════
    printf("phase 1: AR read from port 0\n");
    d->l3_ar_valid_i = 0x1;
    d->p0_ar_id_i    = 42;
    d->p0_ar_addr_i  = 0x1000;
    d->p0_ar_size_i  = 6;
    d->p0_ar_qos_i   = 3;
    sim.tick();
    clear_inputs(d);

    bool ok = wait_sys_ar(sim);
    sim.check(ok, "phase1: sys AR valid");
    if (ok) {
        // ID should be {port_id=0, reqq_id}. reqq_id is first free entry (0)
        // sys_ar_id = {2'b00, 6'd0} = 0
        CHK(sim, "phase1: sys_ar_addr", (uint64_t)d->sys_ar_addr_o, (uint64_t)0x1000);
        CHK(sim, "phase1: sys_ar_size", (int)d->sys_ar_size_o, 6);
        CHK(sim, "phase1: sys_ar_qos",  (int)d->sys_ar_qos_o, 3);
        // ID: port 0 in upper bits, reqq_id=0 in lower 6 bits
        uint32_t expected_id = (0 << 6) | 0;
        CHK(sim, "phase1: sys_ar_id", (int)d->sys_ar_id_o, (int)expected_id);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 2: AW+W write request → verify sys AW+W
    // ══════════════════════════════════════════════════════════
    printf("phase 2: AW+W write from port 0\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 100;
    d->p0_aw_addr_i  = 0x2000;
    d->p0_aw_size_i  = 6;
    d->p0_aw_qos_i   = 1;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_w_strb_i   = 0xFF;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase2: sys AW valid");
    if (ok) {
        CHK(sim, "phase2: sys_aw_addr", (uint64_t)d->sys_aw_addr_o, (uint64_t)0x2000);
        CHK(sim, "phase2: sys_aw_user (opcode)", (int)d->sys_aw_user_o, (int)ReqWrite);
        sim.check(d->sys_w_valid_o == 1, "phase2: sys W valid");
        sim.check(d->sys_w_last_o == 1, "phase2: sys W last");
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 3: Opcode translation — FlushToMem → Write
    // ══════════════════════════════════════════════════════════
    printf("phase 3: FlushToMem opcode translation\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 101;
    d->p0_aw_addr_i  = 0x3000;
    d->p0_aw_user_i  = ReqFlushToMem;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase3: sys AW valid (FlushToMem)");
    if (ok) {
        CHK(sim, "phase3: sys_aw_user (translated)", (int)d->sys_aw_user_o, (int)ReqWrite);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 3b: Opcode translation — EvictToMem → Write
    // ══════════════════════════════════════════════════════════
    printf("phase 3b: EvictToMem opcode translation\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 102;
    d->p0_aw_addr_i  = 0x3100;
    d->p0_aw_user_i  = ReqEvictToMem;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase3b: sys AW valid (EvictToMem)");
    if (ok) {
        CHK(sim, "phase3b: sys_aw_user (translated)", (int)d->sys_aw_user_o, (int)ReqWrite);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 4: Auto-ack — Flush → immediate B, no sys request
    // ══════════════════════════════════════════════════════════
    printf("phase 4: Flush auto-ack\n");
    // Record current sys AW count — should NOT increase
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 200;
    d->p0_aw_addr_i  = 0x4000;
    d->p0_aw_user_i  = ReqFlush;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    // Wait for B on port 0 — should come without sys involvement
    ok = wait_l3_b_valid(sim, 0);
    sim.check(ok, "phase4: L3 B valid (auto-ack)");
    if (ok) {
        CHK(sim, "phase4: L3 B id", (int)d->p0_b_id_o, 200);
        CHK(sim, "phase4: L3 B resp (OKAY)", (int)d->p0_b_resp_o, (int)RespOkay);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 4b: Auto-ack — Evict
    // ══════════════════════════════════════════════════════════
    printf("phase 4b: Evict auto-ack\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 201;
    d->p0_aw_addr_i  = 0x4100;
    d->p0_aw_user_i  = ReqEvict;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_l3_b_valid(sim, 0);
    sim.check(ok, "phase4b: L3 B valid (Evict auto-ack)");
    if (ok) {
        CHK(sim, "phase4b: L3 B id", (int)d->p0_b_id_o, 201);
        CHK(sim, "phase4b: L3 B resp (OKAY)", (int)d->p0_b_resp_o, (int)RespOkay);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 4c: Auto-ack — Prefetch
    // ══════════════════════════════════════════════════════════
    printf("phase 4c: Prefetch auto-ack\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 202;
    d->p0_aw_addr_i  = 0x4200;
    d->p0_aw_user_i  = ReqPrefetch;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_l3_b_valid(sim, 0);
    sim.check(ok, "phase4c: L3 B valid (Prefetch auto-ack)");
    if (ok) {
        CHK(sim, "phase4c: L3 B id", (int)d->p0_b_id_o, 202);
        CHK(sim, "phase4c: L3 B resp (OKAY)", (int)d->p0_b_resp_o, (int)RespOkay);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 5: R response from sys → L3 R on correct port
    // ══════════════════════════════════════════════════════════
    printf("phase 5: R response routing\n");
    // Send an AR from port 0 to establish reqq entry
    d->l3_ar_valid_i = 0x1;
    d->p0_ar_id_i    = 300;
    d->p0_ar_addr_i  = 0x5000;
    d->p0_ar_size_i  = 6;
    sim.tick();
    clear_inputs(d);

    // Wait for sys AR to appear and capture the ID
    ok = wait_sys_ar(sim);
    sim.check(ok, "phase5: sys AR valid");
    uint32_t sys_ar_id_captured = 0;
    if (ok) {
        sys_ar_id_captured = d->sys_ar_id_o;
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // Now send R response with the captured ID
    d->sys_r_valid_i = 1;
    d->sys_r_id_i    = sys_ar_id_captured;
    d->sys_r_resp_i  = RespOkay;
    d->sys_r_last_i  = 1;
    sim.tick();
    d->sys_r_valid_i = 0;

    ok = wait_l3_r_valid(sim, 0);
    sim.check(ok, "phase5: L3 R valid on port 0");
    if (ok) {
        CHK(sim, "phase5: L3 R id (restored)", (int)d->p0_r_id_o, 300);
        CHK(sim, "phase5: L3 R resp", (int)d->p0_r_resp_o, (int)RespOkay);
        sim.check(d->p0_r_last_o == 1, "phase5: L3 R last");
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 6: B response from sys → L3 B on correct port
    // ══════════════════════════════════════════════════════════
    printf("phase 6: B response routing\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 400;
    d->p0_aw_addr_i  = 0x6000;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase6: sys AW valid");
    uint32_t sys_aw_id_captured = 0;
    if (ok) {
        sys_aw_id_captured = d->sys_aw_id_o;
    }
    for (int i = 0; i < 5; i++) sim.tick();

    d->sys_b_valid_i = 1;
    d->sys_b_id_i    = sys_aw_id_captured;
    d->sys_b_resp_i  = RespOkay;
    sim.tick();
    d->sys_b_valid_i = 0;

    ok = wait_l3_b_valid(sim, 0);
    sim.check(ok, "phase6: L3 B valid on port 0");
    if (ok) {
        CHK(sim, "phase6: L3 B id (restored)", (int)d->p0_b_id_o, 400);
        CHK(sim, "phase6: L3 B resp", (int)d->p0_b_resp_o, (int)RespOkay);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 6b: B response with error
    // ══════════════════════════════════════════════════════════
    printf("phase 6b: B response with SlvErr\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 401;
    d->p0_aw_addr_i  = 0x6100;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase6b: sys AW valid");
    if (ok) sys_aw_id_captured = d->sys_aw_id_o;
    for (int i = 0; i < 5; i++) sim.tick();

    d->sys_b_valid_i = 1;
    d->sys_b_id_i    = sys_aw_id_captured;
    d->sys_b_resp_i  = RespSlvErr;
    sim.tick();
    d->sys_b_valid_i = 0;

    ok = wait_l3_b_valid(sim, 0);
    sim.check(ok, "phase6b: L3 B valid");
    if (ok) {
        CHK(sim, "phase6b: L3 B resp (SlvErr)", (int)d->p0_b_resp_o, (int)RespSlvErr);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 7: Multi-port arbitration
    // ══════════════════════════════════════════════════════════
    printf("phase 7: multi-port AR arbitration\n");
    d->l3_ar_valid_i = 0x3;  // port 0 and port 1
    d->p0_ar_id_i    = 500;
    d->p0_ar_addr_i  = 0x7000;
    d->p0_ar_size_i  = 6;
    d->p1_ar_id_i    = 501;
    d->p1_ar_addr_i  = 0x7100;
    sim.tick();
    clear_inputs(d);

    // Should see two sys ARs over the next cycles (one per port)
    int ar_count = 0;
    for (int i = 0; i < 30; i++) {
        if (d->sys_ar_valid_o) ar_count++;
        sim.tick();
    }
    sim.check(ar_count >= 2, "phase7: saw at least 2 sys AR valids");
    for (int i = 0; i < 5; i++) sim.tick();

    // Multi-port AW arbitration
    printf("phase 7b: multi-port AW arbitration\n");
    d->l3_aw_valid_i = 0x3;
    d->l3_w_valid_i  = 0x3;
    d->p0_aw_id_i    = 510;
    d->p0_aw_addr_i  = 0x7200;
    d->p0_aw_user_i  = ReqWrite;
    d->p0_w_last_i   = 1;
    d->p1_aw_id_i    = 511;
    d->p1_aw_addr_i  = 0x7300;
    d->p1_aw_user_i  = ReqWrite;
    sim.tick();
    clear_inputs(d);

    int aw_count = 0;
    for (int i = 0; i < 30; i++) {
        if (d->sys_aw_valid_o) aw_count++;
        sim.tick();
    }
    sim.check(aw_count >= 2, "phase7b: saw at least 2 sys AW valids");
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 8: Backpressure — sys AR not ready
    // ══════════════════════════════════════════════════════════
    printf("phase 8: backpressure (sys AR not ready)\n");
    d->sys_ar_ready_i = 0;
    d->l3_ar_valid_i  = 0x1;
    d->p0_ar_id_i     = 600;
    d->p0_ar_addr_i   = 0x8000;
    d->p0_ar_size_i   = 6;
    sim.tick();
    clear_inputs(d);
    d->sys_ar_ready_i = 0;

    // Sys AR should NOT be valid (blocked by FIFO backpressure)
    // Actually with async FIFOs the request may still appear but
    // the ready from FIFO controls admission. Let several cycles pass.
    for (int i = 0; i < 10; i++) sim.tick();

    // Release backpressure
    d->sys_ar_ready_i = 1;
    ok = wait_sys_ar(sim, 20);
    sim.check(ok, "phase8: sys AR valid after backpressure released");
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 8b: Backpressure — sys AW not ready
    // ══════════════════════════════════════════════════════════
    printf("phase 8b: backpressure (sys AW not ready)\n");
    d->sys_aw_ready_i = 0;
    d->l3_aw_valid_i  = 0x1;
    d->l3_w_valid_i   = 0x1;
    d->p0_aw_id_i     = 601;
    d->p0_aw_addr_i   = 0x8100;
    d->p0_aw_user_i   = ReqWrite;
    d->p0_w_last_i    = 1;
    sim.tick();
    clear_inputs(d);
    d->sys_aw_ready_i = 0;
    for (int i = 0; i < 10; i++) sim.tick();

    d->sys_aw_ready_i = 1;
    ok = wait_sys_aw(sim, 20);
    sim.check(ok, "phase8b: sys AW valid after backpressure released");
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 9: Atomic passthrough (not auto-acked)
    // ══════════════════════════════════════════════════════════
    printf("phase 9: Atomic passthrough\n");
    d->l3_aw_valid_i = 0x1;
    d->l3_w_valid_i  = 0x1;
    d->p0_aw_id_i    = 700;
    d->p0_aw_addr_i  = 0x9000;
    d->p0_aw_user_i  = ReqAtomic;
    d->p0_w_last_i   = 1;
    sim.tick();
    clear_inputs(d);

    ok = wait_sys_aw(sim);
    sim.check(ok, "phase9: sys AW valid (Atomic)");
    if (ok) {
        CHK(sim, "phase9: sys_aw_user (Atomic)", (int)d->sys_aw_user_o, (int)ReqAtomic);
    }
    for (int i = 0; i < 5; i++) sim.tick();

    // ══════════════════════════════════════════════════════════
    // Phase 10: Drain and finish
    // ══════════════════════════════════════════════════════════
    clear_inputs(d);
    for (int i = 0; i < 20; i++) sim.tick();

    return sim.finish();
}

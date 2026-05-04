// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bist_mbx.
// Tests two parameterizations: RamDelay=1 (no blocking) and RamDelay=3 (blocking).

#include "Vshirecache_bist_mbx_tb.h"
#include "sim_ctrl.h"

typedef Vshirecache_bist_mbx_tb DUT;

static void clear(DUT* d) {
    d->mbx_impl_i    = 1;
    d->mbist_sel_i   = 0;
    d->mbist_rd_en_i = 0;
    d->mbist_wr_en_i = 0;
    d->mbist_addr_i  = 0;
    d->mbist_wdata_i = 0;
    d->d1_mbx_rdata_i = 0;
    d->d3_mbx_rdata_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset();

    // ── Test 1: Feedthrough — RamDelay=1 ─────────────────────
    printf("── Test 1: Feedthrough (RamDelay=1)\n");
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0xA;
    d->mbist_wdata_i = 0xBEEF;
    sim.tick();
    sim.check(d->d1_mbx_sel_o    == 1,      "d1: sel passthrough");
    sim.check(d->d1_mbx_rd_en_o  == 1,      "d1: rd_en passthrough (no block)");
    sim.check(d->d1_mbx_addr_o   == 0xA,    "d1: addr passthrough");
    sim.check(d->d1_mbx_wdata_o  == 0xBEEF, "d1: wdata passthrough");

    // ── Test 2: Write enable passthrough ────────────────────
    printf("── Test 2: Write enable passthrough\n");
    clear(d);
    d->mbist_sel_i   = 1;
    d->mbist_wr_en_i = 1;
    d->mbist_addr_i  = 0x5;
    d->mbist_wdata_i = 0x1234;
    sim.tick();
    sim.check(d->d1_mbx_wr_en_o == 1,      "d1: wr_en passthrough");
    sim.check(d->d1_mbx_addr_o  == 0x5,    "d1: addr=5");
    sim.check(d->d1_mbx_wdata_o == 0x1234, "d1: wdata=0x1234");

    // ── Test 3: mbx_impl=1 rdata passthrough ────────────────
    printf("── Test 3: rdata with mbx_impl=1\n");
    clear(d);
    d->mbx_impl_i     = 1;
    d->d1_mbx_rdata_i = 0xCAFE;
    sim.tick();
    sim.check(d->d1_mbist_rdata_o == 0xCAFE, "d1: rdata passthrough when impl=1");

    // ── Test 4: mbx_impl=0 rdata zero ──────────────────────
    printf("── Test 4: rdata with mbx_impl=0\n");
    d->mbx_impl_i     = 0;
    d->d1_mbx_rdata_i = 0xCAFE;
    sim.tick();
    sim.check(d->d1_mbist_rdata_o == 0, "d1: rdata=0 when impl=0");

    // ── Test 5: sel=0 — no access ──────────────────────────
    printf("── Test 5: sel=0 disables access\n");
    clear(d);
    d->mbist_sel_i   = 0;
    d->mbist_rd_en_i = 1;
    sim.tick();
    sim.check(d->d1_mbx_sel_o   == 0, "d1: sel=0 → mbx_sel=0");
    // rd_en blocked because sel=0 for blocking logic input but
    // actual rd_en output = mbist_rd_en & ~block_access
    // For d1 (RamDelay=1), block_access=0 so rd_en passes through
    sim.check(d->d1_mbx_rd_en_o == 1, "d1: rd_en not blocked by sel (feedthrough)");

    // ── Test 6: RamDelay=3 — blocking pattern ───────────────
    // Note: after tick(), Verilator shows post-posedge state.
    // For RamDelay=3 with continuous sel+rd_en, access passes every 3rd cycle.
    // After the first tick, blocking is already active (FF captured).
    printf("── Test 6: RamDelay=3 blocking pattern\n");
    clear(d);
    sim.reset();
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x3;
    sim.tick();
    sim.check(d->d3_mbx_sel_o  == 1, "d3: sel always passes");
    sim.check(d->d3_mbx_addr_o == 3, "d3: addr always passes");
    // After tick 1: blocking shift reg = 01, blocked
    sim.check(d->d3_mbx_rd_en_o == 0, "d3: tick 1 → blocked (shift reg filling)");

    sim.tick();
    // After tick 2: shift reg = 10, still blocked
    sim.check(d->d3_mbx_rd_en_o == 0, "d3: tick 2 → still blocked");

    sim.tick();
    // After tick 3: shift reg = 00, unblocked → new access triggers re-block
    // But at the end of this tick, we see the re-blocked state
    // Actually: vec_active drains to 00, block_access=0, rd_en passes
    // Then active_d0=1 again, vec_active_d={0,1}=01, captured at NEXT posedge
    // So at end of this tick: vec_active is still 00 (drain just happened)
    // Wait, I need to re-trace. After posedge of tick 3:
    // vec_active_q was 10 before. vec_active_d = {vec_active_q[0],0} = {0,0} = 00
    // After posedge: vec_active_q = 00, block_access = 0
    // Comb re-eval: active_d0 = 1&1&~0 = 1, vec_active_d = {0,1} = 01
    // But FF won't capture until next posedge
    // So: block_access = 0, rd_en_o = 1
    sim.check(d->d3_mbx_rd_en_o == 1, "d3: tick 3 → unblocked");

    // ── Test 7: RamDelay=3 — repeating pattern ────────────
    printf("── Test 7: RamDelay=3 repeating pattern\n");
    sim.tick();
    sim.check(d->d3_mbx_rd_en_o == 0, "d3: tick 4 → re-blocked");
    sim.tick();
    sim.check(d->d3_mbx_rd_en_o == 0, "d3: tick 5 → still blocked");
    sim.tick();
    sim.check(d->d3_mbx_rd_en_o == 1, "d3: tick 6 → unblocked again");

    // ── Test 8: RamDelay=3 — write also triggers blocking ──
    printf("── Test 8: RamDelay=3 write blocking\n");
    clear(d);
    sim.reset();
    d->mbist_sel_i   = 1;
    d->mbist_wr_en_i = 1;
    d->mbist_addr_i  = 0x7;
    d->mbist_wdata_i = 0x9999;
    sim.tick();
    sim.check(d->d3_mbx_wdata_o == 0x9999, "d3: wdata passes");
    sim.check(d->d3_mbx_wr_en_o == 0, "d3: tick 1 write → blocked");
    sim.tick();
    sim.check(d->d3_mbx_wr_en_o == 0, "d3: tick 2 write → still blocked");
    sim.tick();
    sim.check(d->d3_mbx_wr_en_o == 1, "d3: tick 3 write → unblocked");

    // ── Test 9: RamDelay=3 — deassert clears blocking ──────
    printf("── Test 9: Deassert clears blocking\n");
    clear(d);
    sim.reset();
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    sim.tick(); // triggers blocking
    // Deassert and wait for shift register to drain
    d->mbist_sel_i   = 0;
    d->mbist_rd_en_i = 0;
    sim.tick(); sim.tick(); sim.tick();
    // Re-assert: should be unblocked now
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    sim.tick();
    // After this tick: just re-triggered, so blocked again
    // But shift reg was clear before re-assert, so first access went through
    // and now it's blocked again
    sim.check(d->d3_mbx_rd_en_o == 0, "d3: re-access triggers fresh block");

    // ── Test 10: RamDelay=1 never blocks ───────────────────
    printf("── Test 10: RamDelay=1 never blocks (burst)\n");
    clear(d);
    sim.reset();
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    for (int i = 0; i < 5; i++) {
        d->mbist_addr_i = i;
        sim.tick();
        sim.check(d->d1_mbx_rd_en_o == 1, "d1: burst access always passes");
    }

    // ── Test 11: RamDelay=3 rdata with impl ────────────────
    printf("── Test 11: RamDelay=3 rdata gating\n");
    clear(d);
    d->mbx_impl_i     = 1;
    d->d3_mbx_rdata_i = 0xABCD;
    sim.tick();
    sim.check(d->d3_mbist_rdata_o == 0xABCD, "d3: rdata pass when impl=1");
    d->mbx_impl_i = 0;
    sim.tick();
    sim.check(d->d3_mbist_rdata_o == 0, "d3: rdata=0 when impl=0");

    return sim.finish();
}

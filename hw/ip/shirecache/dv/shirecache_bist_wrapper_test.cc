// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bist_wrapper.
// Tests one-hot select decode, mbist_on register, per-memory routing, rdata mux.

#include "Vshirecache_bist_wrapper_tb.h"
#include "sim_ctrl.h"

typedef Vshirecache_bist_wrapper_tb DUT;

static void clear(DUT* d) {
    d->mbi_implemented_const_i = 1;
    d->dft_mbist_en_i = 0;
    d->mbist_en_i     = 0;
    d->mbist_sel_i    = 0;
    d->mbist_rd_en_i  = 0;
    d->mbist_wr_en_i  = 0;
    d->mbist_addr_i   = 0;
    d->mbist_wdata_lo_i = 0;
    d->pipe_rsp_mbs_rdata_lo_i   = 0;
    d->pipe_rsp_mbt_rdata_lo_i   = 0;
    d->pipe_rsp_mbd_rdata_lo_i   = 0;
    d->icache_rsp_mbi_rdata_lo_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset();

    // ── Test 1: mbist_on register — disabled ────────────────
    printf("── Test 1: mbist_on disabled\n");
    sim.tick();
    sim.check(d->mbist_on_o == 0, "mbist_on=0 when en=0, dft=0");
    sim.check(d->pipe_req_mbist_on_o == 0, "pipe_req mbist_on=0");
    sim.check(d->icache_req_mbist_on_o == 0, "icache_req mbist_on=0");

    // ── Test 2: mbist_on via mbist_en ──────────────────────
    printf("── Test 2: mbist_on via mbist_en\n");
    d->mbist_en_i = 1;
    sim.tick();
    sim.check(d->mbist_on_o == 1, "mbist_on=1 via mbist_en");
    sim.check(d->pipe_req_mbist_on_o == 1, "pipe_req mbist_on=1");
    sim.check(d->icache_req_mbist_on_o == 1, "icache_req mbist_on=1");

    // ── Test 3: mbist_on via dft_mbist_en ──────────────────
    printf("── Test 3: mbist_on via dft_mbist_en\n");
    clear(d);
    d->dft_mbist_en_i = 1;
    sim.tick();
    sim.check(d->mbist_on_o == 1, "mbist_on=1 via dft");

    // ── Test 4: mbist_on deassert ──────────────────────────
    printf("── Test 4: mbist_on deassert\n");
    clear(d);
    sim.tick();
    sim.check(d->mbist_on_o == 0, "mbist_on=0 after deassert");

    // ── Test 5: Select MBS (sel=1) — check sel routing ─────
    // Note: sub-mbx instances have RamDelay=2, so rd_en/wr_en
    // show blocked state after first tick. Sel always passes through.
    printf("── Test 5: Select MBS (sel=1)\n");
    clear(d);
    d->mbist_en_i    = 1;
    d->mbist_sel_i   = 1;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x55;
    d->mbist_wdata_lo_i = 0xAA;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o   == 1, "MBS sel=1");
    sim.check(d->pipe_req_mbt_sel_o   == 0, "MBT sel=0");
    sim.check(d->pipe_req_mbd_sel_o   == 0, "MBD sel=0");
    sim.check(d->icache_req_mbi_sel_o == 0, "MBI sel=0");
    // rd_en blocked after first tick (RamDelay=2 blocking), check on 2nd tick
    sim.tick();
    sim.check(d->pipe_req_mbs_rd_en_o == 1, "MBS rd_en=1 (after blocking clears)");

    // ── Test 6: Select MBT (sel=2) ─────────────────────────
    printf("── Test 6: Select MBT (sel=2)\n");
    d->mbist_sel_i = 2;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o   == 0, "MBS sel=0");
    sim.check(d->pipe_req_mbt_sel_o   == 1, "MBT sel=1");
    sim.check(d->pipe_req_mbd_sel_o   == 0, "MBD sel=0");
    sim.check(d->icache_req_mbi_sel_o == 0, "MBI sel=0");

    // ── Test 7: Select MBD (sel=3) ─────────────────────────
    printf("── Test 7: Select MBD (sel=3)\n");
    d->mbist_sel_i = 3;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o   == 0, "MBS sel=0");
    sim.check(d->pipe_req_mbt_sel_o   == 0, "MBT sel=0");
    sim.check(d->pipe_req_mbd_sel_o   == 1, "MBD sel=1");
    sim.check(d->icache_req_mbi_sel_o == 0, "MBI sel=0");

    // ── Test 8: Select MBI (sel=4) ─────────────────────────
    printf("── Test 8: Select MBI (sel=4)\n");
    d->mbist_sel_i = 4;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o   == 0, "MBS sel=0");
    sim.check(d->pipe_req_mbt_sel_o   == 0, "MBT sel=0");
    sim.check(d->pipe_req_mbd_sel_o   == 0, "MBD sel=0");
    sim.check(d->icache_req_mbi_sel_o == 1, "MBI sel=1");

    // ── Test 9: sel=0 (reserved) ───────────────────────────
    printf("── Test 9: sel=0 (none selected)\n");
    d->mbist_sel_i = 0;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o   == 0, "sel=0: MBS=0");
    sim.check(d->pipe_req_mbt_sel_o   == 0, "sel=0: MBT=0");
    sim.check(d->pipe_req_mbd_sel_o   == 0, "sel=0: MBD=0");
    sim.check(d->icache_req_mbi_sel_o == 0, "sel=0: MBI=0");

    // ── Test 10: mbist_en=0 blocks select ──────────────────
    printf("── Test 10: mbist_en=0 blocks select\n");
    d->mbist_en_i  = 0;
    d->mbist_sel_i = 1;
    sim.tick();
    sim.check(d->pipe_req_mbs_sel_o == 0, "en=0: MBS sel blocked");

    // ── Test 11: Read data mux — MBS ───────────────────────
    printf("── Test 11: Read data mux — MBS\n");
    clear(d);
    d->mbist_en_i  = 1;
    d->mbist_sel_i = 1;
    d->pipe_rsp_mbs_rdata_lo_i = 0xDEAD;
    sim.tick();
    sim.check(d->mbist_rdata_lo_o == 0xDEAD, "MBS rdata mux");

    // ── Test 12: Read data mux — MBT ───────────────────────
    printf("── Test 12: Read data mux — MBT\n");
    d->mbist_sel_i = 2;
    d->pipe_rsp_mbs_rdata_lo_i = 0;
    d->pipe_rsp_mbt_rdata_lo_i = 0xBEEF;
    sim.tick();
    sim.check(d->mbist_rdata_lo_o == 0xBEEF, "MBT rdata mux");

    // ── Test 13: Read data mux — MBD ───────────────────────
    printf("── Test 13: Read data mux — MBD\n");
    d->mbist_sel_i = 3;
    d->pipe_rsp_mbt_rdata_lo_i = 0;
    d->pipe_rsp_mbd_rdata_lo_i = 0xFACE;
    sim.tick();
    sim.check(d->mbist_rdata_lo_o == 0xFACE, "MBD rdata mux");

    // ── Test 14: Read data mux — MBI ───────────────────────
    printf("── Test 14: Read data mux — MBI\n");
    d->mbist_sel_i = 4;
    d->pipe_rsp_mbd_rdata_lo_i = 0;
    d->icache_rsp_mbi_rdata_lo_i = 0xC0DE;
    sim.tick();
    sim.check(d->mbist_rdata_lo_o == 0xC0DE, "MBI rdata mux");

    // ── Test 15: Write routing ─────────────────────────────
    printf("── Test 15: Write routing to MBS\n");
    clear(d);
    sim.reset();  // clear blocking state
    d->mbist_en_i       = 1;
    d->mbist_sel_i      = 1;
    d->mbist_wr_en_i    = 1;
    d->mbist_addr_i     = 0x42;
    d->mbist_wdata_lo_i = 0x5678;
    sim.tick();  // first tick: blocked by RamDelay=2
    sim.tick();  // second tick: blocking clears
    sim.check(d->pipe_req_mbs_wr_en_o == 1, "MBS wr_en routed");
    sim.check(d->pipe_req_mbs_wdata_lo_o == 0x5678, "MBS wdata routed");
    // wr_en is shared (not gated per-memory by sel); only sel determines target
    sim.check(d->pipe_req_mbs_sel_o == 1, "MBS sel=1 during write");
    sim.check(d->pipe_req_mbt_sel_o == 0, "MBT sel=0 during MBS write");
    sim.check(d->pipe_req_mbd_sel_o == 0, "MBD sel=0 during MBS write");

    // ── Test 16: Address routing per memory ────────────────
    printf("── Test 16: Address routing\n");
    d->mbist_addr_i = 0x123;
    sim.tick();
    // MBS addr is truncated to MbsAddrSize (11 bits)
    sim.check(d->pipe_req_mbs_addr_o == 0x123, "MBS addr");

    return sim.finish();
}

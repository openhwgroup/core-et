// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bist_wrapper vs shirecache_bist_wrapper

#include "Vcosim_bist_wrapper_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_bist_wrapper_tb;

// MBX sel IDs (matching both original and new)
static constexpr int MBS_SEL_ID = 1;
static constexpr int MBT_SEL_ID = 2;
static constexpr int MBD_SEL_ID = 3;
static constexpr int MBI_SEL_ID = 4;

// ── Helpers ──────────────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->mbi_implemented_const_i = 1;
    d->dft_mbist_en_i          = 0;
    d->mbist_en_i              = 0;
    d->mbist_sel_i             = 0;
    d->mbist_rd_en_i           = 0;
    d->mbist_wr_en_i           = 0;
    d->mbist_addr_i            = 0;
    d->mbist_wdata_lo_i        = 0;
    d->rsp_mbs_rdata_lo_i      = 0;
    d->rsp_mbt_rdata_lo_i      = 0;
    d->rsp_mbd_rdata_lo_i      = 0;
    d->rsp_mbi_rdata_lo_i      = 0;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("mbist_rdata_lo",
                (uint32_t)d->orig_mbist_rdata_lo_o,
                (uint32_t)d->new_mbist_rdata_lo_o);
    sim.compare("mbist_on",
                (uint32_t)d->orig_mbist_on_o,
                (uint32_t)d->new_mbist_on_o);

    // MBS
    sim.compare("mbs_sel",
                (uint32_t)d->orig_mbs_sel_o,
                (uint32_t)d->new_mbs_sel_o);
    sim.compare("mbs_rd_en",
                (uint32_t)d->orig_mbs_rd_en_o,
                (uint32_t)d->new_mbs_rd_en_o);
    sim.compare("mbs_wr_en",
                (uint32_t)d->orig_mbs_wr_en_o,
                (uint32_t)d->new_mbs_wr_en_o);
    sim.compare("mbs_addr",
                (uint32_t)d->orig_mbs_addr_o,
                (uint32_t)d->new_mbs_addr_o);
    sim.compare("mbs_wdata_lo",
                (uint32_t)d->orig_mbs_wdata_lo_o,
                (uint32_t)d->new_mbs_wdata_lo_o);

    // MBT
    sim.compare("mbt_sel",
                (uint32_t)d->orig_mbt_sel_o,
                (uint32_t)d->new_mbt_sel_o);
    sim.compare("mbt_rd_en",
                (uint32_t)d->orig_mbt_rd_en_o,
                (uint32_t)d->new_mbt_rd_en_o);
    sim.compare("mbt_wr_en",
                (uint32_t)d->orig_mbt_wr_en_o,
                (uint32_t)d->new_mbt_wr_en_o);
    sim.compare("mbt_addr",
                (uint32_t)d->orig_mbt_addr_o,
                (uint32_t)d->new_mbt_addr_o);
    sim.compare("mbt_wdata_lo",
                (uint32_t)d->orig_mbt_wdata_lo_o,
                (uint32_t)d->new_mbt_wdata_lo_o);

    // MBD
    sim.compare("mbd_sel",
                (uint32_t)d->orig_mbd_sel_o,
                (uint32_t)d->new_mbd_sel_o);
    sim.compare("mbd_rd_en",
                (uint32_t)d->orig_mbd_rd_en_o,
                (uint32_t)d->new_mbd_rd_en_o);
    sim.compare("mbd_wr_en",
                (uint32_t)d->orig_mbd_wr_en_o,
                (uint32_t)d->new_mbd_wr_en_o);
    sim.compare("mbd_addr",
                (uint32_t)d->orig_mbd_addr_o,
                (uint32_t)d->new_mbd_addr_o);
    sim.compare("mbd_wdata_lo",
                (uint32_t)d->orig_mbd_wdata_lo_o,
                (uint32_t)d->new_mbd_wdata_lo_o);

    // MBI
    sim.compare("mbi_sel",
                (uint32_t)d->orig_mbi_sel_o,
                (uint32_t)d->new_mbi_sel_o);
    sim.compare("mbi_rd_en",
                (uint32_t)d->orig_mbi_rd_en_o,
                (uint32_t)d->new_mbi_rd_en_o);
    sim.compare("mbi_wr_en",
                (uint32_t)d->orig_mbi_wr_en_o,
                (uint32_t)d->new_mbi_wr_en_o);
    sim.compare("mbi_addr",
                (uint32_t)d->orig_mbi_addr_o,
                (uint32_t)d->new_mbi_addr_o);
    sim.compare("mbi_wdata_lo",
                (uint32_t)d->orig_mbi_wdata_lo_o,
                (uint32_t)d->new_mbi_wdata_lo_o);
}

// ── Main ─────────────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Reset + idle ────────────────────────────────────
    printf("phase 1: reset + idle\n");
    clear_inputs(d);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 2: Enable mbist_on via mbist_en ────────────────────
    printf("phase 2: enable mbist_on via mbist_en\n");
    clear_inputs(d);
    d->mbist_en_i = 1;
    sim.tick();
    compare_all(sim);

    // After 1 cycle mbist_on should be registered
    sim.tick();
    compare_all(sim);

    // Disable mbist_en
    d->mbist_en_i = 0;
    sim.tick();
    compare_all(sim);

    // mbist_on should deassert after one more cycle
    sim.tick();
    compare_all(sim);

    // ── Phase 3: Enable mbist_on via dft_mbist_en ────────────────
    printf("phase 3: enable mbist_on via dft_mbist_en\n");
    clear_inputs(d);
    d->dft_mbist_en_i = 1;
    sim.tick();
    compare_all(sim);

    sim.tick();
    compare_all(sim);

    d->dft_mbist_en_i = 0;
    sim.tick();
    compare_all(sim);

    sim.tick();
    compare_all(sim);

    // ── Phase 4: Select MBS — read access ────────────────────────
    printf("phase 4: select MBS - read access\n");
    clear_inputs(d);
    d->mbist_en_i    = 1;
    d->mbist_sel_i   = MBS_SEL_ID;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x15;
    d->rsp_mbs_rdata_lo_i = 0xDEADBEEF;
    sim.tick();
    compare_all(sim);

    // Keep sel+rd_en to test blocking
    for (int i = 0; i < 4; i++) {
        d->rsp_mbs_rdata_lo_i = 0xCAFE0000 | i;
        sim.tick();
        compare_all(sim);
    }

    // Deassert
    clear_inputs(d);
    for (int i = 0; i < 4; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 5: Select MBT — write access ───────────────────────
    printf("phase 5: select MBT - write access\n");
    clear_inputs(d);
    d->mbist_en_i     = 1;
    d->mbist_sel_i    = MBT_SEL_ID;
    d->mbist_wr_en_i  = 1;
    d->mbist_addr_i   = 0x0A;
    d->mbist_wdata_lo_i = 0x12345678;
    sim.tick();
    compare_all(sim);

    // Keep asserting to test blocking
    for (int i = 0; i < 4; i++) {
        d->mbist_wdata_lo_i = 0xAAAA0000 | i;
        sim.tick();
        compare_all(sim);
    }

    clear_inputs(d);
    for (int i = 0; i < 4; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 6: Select MBD — read + write ───────────────────────
    printf("phase 6: select MBD - read + write\n");
    for (int i = 0; i < 8; i++) {
        clear_inputs(d);
        d->mbist_en_i  = 1;
        d->mbist_sel_i = MBD_SEL_ID;
        d->mbist_addr_i = i & 0x1F;
        if (i & 1) {
            d->mbist_wr_en_i    = 1;
            d->mbist_wdata_lo_i = 0xBBBB0000 | i;
        } else {
            d->mbist_rd_en_i      = 1;
            d->rsp_mbd_rdata_lo_i = 0xCCCC0000 | i;
        }
        sim.tick();
        compare_all(sim);
    }

    clear_inputs(d);
    for (int i = 0; i < 4; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 7: Select MBI — read with mbi_implemented=1 ────────
    printf("phase 7: select MBI - read (mbi_implemented=1)\n");
    clear_inputs(d);
    d->mbi_implemented_const_i = 1;
    d->mbist_en_i    = 1;
    d->mbist_sel_i   = MBI_SEL_ID;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x03;
    d->rsp_mbi_rdata_lo_i = 0x99887766;
    sim.tick();
    compare_all(sim);

    for (int i = 0; i < 4; i++) {
        d->rsp_mbi_rdata_lo_i = 0x11220000 | i;
        sim.tick();
        compare_all(sim);
    }

    clear_inputs(d);
    for (int i = 0; i < 4; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 8: MBI with mbi_implemented=0 (rdata should be 0) ─
    printf("phase 8: select MBI - mbi_implemented=0\n");
    clear_inputs(d);
    d->mbi_implemented_const_i = 0;
    d->mbist_en_i    = 1;
    d->mbist_sel_i   = MBI_SEL_ID;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x07;
    d->rsp_mbi_rdata_lo_i = 0xFFFFFFFF;
    sim.tick();
    compare_all(sim);

    // Also write
    d->mbist_rd_en_i    = 0;
    d->mbist_wr_en_i    = 1;
    d->mbist_wdata_lo_i = 0x55AA55AA;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    d->mbi_implemented_const_i = 0;
    for (int i = 0; i < 4; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 9: sel=0 (reserved, no access) ─────────────────────
    printf("phase 9: sel=0 (no access)\n");
    clear_inputs(d);
    d->mbist_en_i    = 1;
    d->mbist_sel_i   = 0;  // reserved
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x0F;
    d->rsp_mbs_rdata_lo_i = 0xFFFFFFFF;
    d->rsp_mbt_rdata_lo_i = 0xFFFFFFFF;
    d->rsp_mbd_rdata_lo_i = 0xFFFFFFFF;
    d->rsp_mbi_rdata_lo_i = 0xFFFFFFFF;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 10: mbist_en=0 with sel (should not select) ────────
    printf("phase 10: mbist_en=0 with sel set\n");
    clear_inputs(d);
    d->mbist_en_i    = 0;
    d->mbist_sel_i   = MBS_SEL_ID;
    d->mbist_rd_en_i = 1;
    d->mbist_addr_i  = 0x05;
    d->rsp_mbs_rdata_lo_i = 0xAAAAAAAA;
    sim.tick();
    compare_all(sim);

    clear_inputs(d);
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 11: Cycle through all sel IDs ──────────────────────
    printf("phase 11: cycle through all sel IDs\n");
    for (int sel = 0; sel < 5; sel++) {
        clear_inputs(d);
        d->mbist_en_i    = 1;
        d->mbist_sel_i   = sel;
        d->mbist_rd_en_i = 1;
        d->mbist_addr_i  = sel * 3;
        d->mbist_wdata_lo_i = static_cast<uint32_t>(sel) * 0x11111111u;
        d->rsp_mbs_rdata_lo_i = 0xAA000000 | sel;
        d->rsp_mbt_rdata_lo_i = 0xBB000000 | sel;
        d->rsp_mbd_rdata_lo_i = 0xCC000000 | sel;
        d->rsp_mbi_rdata_lo_i = 0xDD000000 | sel;
        sim.tick();
        compare_all(sim);

        // Wait for blocking to clear
        clear_inputs(d);
        for (int j = 0; j < 3; j++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // ── Phase 12: Read data mux — verify rdata switches by sel ───
    printf("phase 12: rdata mux by sel\n");
    for (int sel = 1; sel <= 4; sel++) {
        clear_inputs(d);
        d->mbist_en_i    = 1;
        d->mbist_sel_i   = sel;
        d->mbist_rd_en_i = 1;
        d->mbist_addr_i  = 0x01;
        d->rsp_mbs_rdata_lo_i = 0x10000000;
        d->rsp_mbt_rdata_lo_i = 0x20000000;
        d->rsp_mbd_rdata_lo_i = 0x30000000;
        d->rsp_mbi_rdata_lo_i = 0x40000000;
        sim.tick();
        compare_all(sim);

        // Drain blocking
        clear_inputs(d);
        for (int j = 0; j < 3; j++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // ── Phase 13: Random stimulus ────────────────────────────────
    printf("phase 13: random stimulus (300 cycles)\n");
    srand(42);
    for (int cycle = 0; cycle < 300; cycle++) {
        d->mbi_implemented_const_i = (rand() & 0x7) ? 1 : 0;
        d->dft_mbist_en_i          = (rand() & 0xF) == 0 ? 1 : 0;
        d->mbist_en_i              = (rand() & 0x3) ? 1 : 0;
        d->mbist_sel_i             = rand() % 5;
        d->mbist_rd_en_i           = rand() & 1;
        d->mbist_wr_en_i           = rand() & 1;
        d->mbist_addr_i            = rand() & 0x7FFF;  // 15-bit
        d->mbist_wdata_lo_i        = rand();
        d->rsp_mbs_rdata_lo_i      = rand();
        d->rsp_mbt_rdata_lo_i      = rand();
        d->rsp_mbd_rdata_lo_i      = rand();
        d->rsp_mbi_rdata_lo_i      = rand();
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 14: Drain and final idle ───────────────────────────
    printf("phase 14: drain and final idle\n");
    clear_inputs(d);
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

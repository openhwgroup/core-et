// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_pipe_cbuf vs shirecache_pipe_cbuf

#include "Vcosim_cbuf_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>

using DUT = Vcosim_cbuf_tb;

static constexpr uint8_t OP_INSTALL    = 0;
static constexpr uint8_t OP_MRU_UPDATE = 1;
static constexpr uint8_t OP_CLEAR      = 2;
static constexpr uint8_t OP_REPLACE    = 3;
static constexpr uint8_t OP_BYPASS     = 4;

static uint64_t make_addr(uint32_t id) {
    return ((uint64_t)id) << 8;
}

static void drive_req(DUT* dut, uint8_t op, uint64_t inst, uint64_t clr,
                      bool rsp, uint8_t id = 0) {
    dut->req_valid_i = 1;
    dut->req_opcode_i = op;
    dut->req_rsp_required_i = rsp;
    dut->req_install_address_i = inst;
    dut->req_clear_address_i = clr;
    dut->req_reqq_id_i = id;
}

static void no_req(DUT* dut) {
    dut->req_valid_i = 0;
    dut->invalidate_req_valid_i = 0;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    sim.compare("rsp_valid",          dut->orig_rsp_valid_o,          dut->new_rsp_valid_o);
    if (dut->orig_rsp_valid_o || dut->new_rsp_valid_o) {
        sim.compare("rsp_victim",         dut->orig_rsp_victim_o,         dut->new_rsp_victim_o);
        sim.compare("rsp_victim_address", dut->orig_rsp_victim_address_o, dut->new_rsp_victim_address_o);
    }
    sim.compare("state_valid", dut->orig_state_valid_o, dut->new_state_valid_o);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* dut = sim.dut.get();

    dut->esr_cbuf_entry_enable_i = 0xFFFFFFFF;
    dut->my_bank_id_i = 0;
    no_req(dut);

    sim.reset();

    // ── Phase 1: Install 32 entries with responses ──────
    printf("cosim phase 1: install 32 entries\n");
    for (int i = 0; i < 32; i++) {
        drive_req(dut, OP_INSTALL, make_addr(0x100 + i), 0, true, i & 0x3F);
        sim.tick();
        compare_all(sim);
        no_req(dut);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 2: Install 32 more (all cause victims) ────
    printf("cosim phase 2: install 32 more (victims)\n");
    for (int i = 0; i < 32; i++) {
        drive_req(dut, OP_INSTALL, make_addr(0x200 + i), 0, true, i & 0x3F);
        sim.tick();
        compare_all(sim);
        no_req(dut);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 3: Clear all and refill ───────────────────
    printf("cosim phase 3: invalidate and refill\n");
    dut->invalidate_req_valid_i = 1;
    sim.tick();
    compare_all(sim);
    dut->invalidate_req_valid_i = 0;
    no_req(dut);
    sim.tick();
    compare_all(sim);

    for (int i = 0; i < 32; i++) {
        drive_req(dut, OP_INSTALL, make_addr(0x300 + i), 0, false);
        sim.tick();
        compare_all(sim);
    }
    no_req(dut);
    sim.tick();
    compare_all(sim);

    // ── Phase 4: MRU updates ────────────────────────────
    printf("cosim phase 4: MRU updates\n");
    for (int i = 0; i < 32; i++) {
        drive_req(dut, OP_MRU_UPDATE, make_addr(0x300 + i), 0, false);
        sim.tick();
        compare_all(sim);
    }
    no_req(dut);
    sim.tick();
    compare_all(sim);

    // ── Phase 5: Clear and replace ──────────────────────
    printf("cosim phase 5: clear and replace\n");
    for (int i = 0; i < 16; i++) {
        drive_req(dut, OP_CLEAR, 0, make_addr(0x300 + i), false);
        sim.tick();
        compare_all(sim);
    }
    for (int i = 0; i < 16; i++) {
        // Replace: install new addr into slot of old addr
        drive_req(dut, OP_REPLACE, make_addr(0x400 + i),
                  make_addr(0x300 + 16 + i), true, i & 0x3F);
        sim.tick();
        compare_all(sim);
        no_req(dut);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 6: Bypass operations ──────────────────────
    printf("cosim phase 6: bypass\n");
    for (int i = 0; i < 8; i++) {
        drive_req(dut, OP_BYPASS, make_addr(0x500 + i), 0, true, i);
        sim.tick();
        compare_all(sim);
        no_req(dut);
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 7: Mixed operations with entry mask ───────
    printf("cosim phase 7: mixed with 4-entry mask\n");
    dut->invalidate_req_valid_i = 1;
    sim.tick();
    compare_all(sim);
    dut->invalidate_req_valid_i = 0;
    no_req(dut);
    sim.tick();
    compare_all(sim);
    dut->esr_cbuf_entry_enable_i = 0xF; // only 4 entries
    for (int i = 0; i < 8; i++) {
        drive_req(dut, OP_INSTALL, make_addr(0x600 + i), 0, true, i);
        sim.tick();
        compare_all(sim);
        no_req(dut);
        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}

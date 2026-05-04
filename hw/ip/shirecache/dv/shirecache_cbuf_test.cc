// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_cbuf — Verilator test via SV wrapper.

#include "Vshirecache_cbuf_tb.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_cbuf_tb;

static constexpr uint8_t OP_INSTALL    = 0;
static constexpr uint8_t OP_MRU_UPDATE = 1;
static constexpr uint8_t OP_CLEAR      = 2;
static constexpr uint8_t OP_REPLACE    = 3;
static constexpr uint8_t OP_BYPASS     = 4;

// Line-aligned address: [39:8] = id, [7:0] = 0
static uint64_t make_addr(uint32_t id) {
    return ((uint64_t)id) << 8;
}

static void drive_req(DUT* dut, uint8_t op, uint64_t install_addr, uint64_t clear_addr,
                      bool rsp_req, uint8_t reqq_id = 0) {
    dut->req_valid_i = 1;
    dut->req_opcode_i = op;
    dut->req_rsp_required_i = rsp_req;
    dut->req_install_address_i = install_addr;
    dut->req_clear_address_i = clear_addr;
    dut->req_reqq_id_i = reqq_id;
}

static void no_req(DUT* dut) {
    dut->req_valid_i = 0;
    dut->invalidate_req_valid_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;
    auto* dut = sim.dut.get();

    dut->esr_cbuf_entry_enable_i = 0xFFFFFFFF;
    dut->my_bank_id_i = 0;
    no_req(dut);

    sim.reset();

    // Helper: drive a request for one cycle.
    // After this returns, rsp_valid_o holds the registered response.
    // Call no_req + tick to advance past the response.
    auto drive_one = [&](uint8_t op, uint64_t inst, uint64_t clr,
                         bool rsp, uint8_t id = 0) {
        drive_req(dut, op, inst, clr, rsp, id);
        sim.tick(); // posedge: request processed, response registered in rsp_q
        no_req(dut); // deassert inputs (next tick will clear the response)
    };

    // ── Test 1: Install into empty buffer ───────────────
    printf("test 1: install into empty buffer\n");
    drive_one(OP_INSTALL, make_addr(0x100), 0, true, 1);
    sim.check(dut->rsp_valid_o == 1, "rsp valid after install");
    sim.check(dut->rsp_victim_o == 0, "no victim on first install");

    // ── Test 2: Fill buffer to 32 entries ───────────────
    printf("test 2: fill buffer to capacity\n");
    for (int i = 1; i < 32; i++) {
        drive_one(OP_INSTALL, make_addr(0x100 + i), 0, false);
    }
    sim.check(dut->state_valid_o == 0xFFFFFFFF, "all entries valid");

    // ── Test 3: Install when full → victim ──────────────
    printf("test 3: install when full causes victim\n");
    drive_one(OP_INSTALL, make_addr(0x200), 0, true, 2);
    sim.check(dut->rsp_valid_o == 1, "rsp valid");
    sim.check(dut->rsp_victim_o == 1, "victim when full");
    sim.check(dut->rsp_victim_address_o != 0, "victim address non-zero");

    // ── Test 4: Clear an entry ──────────────────────────
    printf("test 4: clear entry frees slot\n");
    drive_one(OP_CLEAR, 0, make_addr(0x200), false);
    drive_one(OP_INSTALL, make_addr(0x300), 0, true, 3);
    sim.check(dut->rsp_valid_o == 1, "rsp valid after clear+install");
    sim.check(dut->rsp_victim_o == 0, "no victim after clear");

    // ── Test 5: MRU update ──────────────────────────────
    printf("test 5: MRU update\n");
    drive_one(OP_MRU_UPDATE, make_addr(0x300), 0, false);
    sim.check(dut->state_valid_o == 0xFFFFFFFF, "all entries still valid");

    // ── Test 6: Bypass ──────────────────────────────────
    printf("test 6: bypass\n");
    drive_one(OP_BYPASS, make_addr(0x400), 0, true, 4);
    sim.check(dut->rsp_valid_o == 1, "bypass rsp valid");
    sim.check(dut->rsp_victim_o == 0, "bypass no victim");

    // ── Test 7: Replace (matching) ──────────────────────
    printf("test 7: replace matching entry\n");
    drive_one(OP_REPLACE, make_addr(0x400), make_addr(0x300), true, 5);
    sim.check(dut->rsp_valid_o == 1, "replace rsp valid");
    sim.check(dut->rsp_victim_o == 0, "replace-hit no victim");

    // ── Test 8: Invalidate all ──────────────────────────
    printf("test 8: invalidate all\n");
    dut->invalidate_req_valid_i = 1;
    no_req(dut);
    dut->invalidate_req_valid_i = 1;
    sim.tick();
    dut->invalidate_req_valid_i = 0;
    sim.tick();
    sim.check(dut->state_valid_o == 0, "all entries cleared");
    drive_one(OP_INSTALL, make_addr(0x500), 0, true, 6);
    sim.check(dut->rsp_valid_o == 1, "rsp valid after inv+install");
    sim.check(dut->rsp_victim_o == 0, "no victim after invalidate");

    // ── Test 9: No rsp when rsp_required=0 ──────────────
    printf("test 9: no response when not required\n");
    no_req(dut);
    sim.tick(); sim.tick();
    drive_one(OP_INSTALL, make_addr(0x600), 0, false);
    sim.check(dut->rsp_valid_o == 0, "no rsp when rsp_required=0");

    // ── Test 10: Reqq ID pass-through ───────────────────
    printf("test 10: reqq_id pass-through\n");
    drive_one(OP_INSTALL, make_addr(0x700), 0, true, 42);
    sim.check(dut->rsp_reqq_id_o == 42, "reqq_id=42 in response");

    // ── Test 11: Entry enable mask ──────────────────────
    printf("test 11: entry enable mask\n");
    dut->invalidate_req_valid_i = 1;
    sim.tick();
    dut->invalidate_req_valid_i = 0;
    no_req(dut);
    sim.tick();
    dut->esr_cbuf_entry_enable_i = 0x3;
    drive_one(OP_INSTALL, make_addr(0x800), 0, false);
    drive_one(OP_INSTALL, make_addr(0x801), 0, false);
    drive_one(OP_INSTALL, make_addr(0x802), 0, true, 7);
    sim.check(dut->rsp_valid_o == 1, "rsp valid with mask");
    sim.check(dut->rsp_victim_o == 1, "victim with 2-entry mask");
    dut->esr_cbuf_entry_enable_i = 0xFFFFFFFF;

    return sim.finish();
}

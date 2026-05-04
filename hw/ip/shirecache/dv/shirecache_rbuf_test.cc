// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_rbuf — Verilator test via SV wrapper.

#include "Vshirecache_rbuf_tb.h"
#include "sim_ctrl.h"
#include <cstring>

using DUT = Vshirecache_rbuf_tb;

static uint64_t make_addr(uint32_t id) { return ((uint64_t)id) << 8; }

static void idle(DUT* dut) {
    dut->invalidate_i     = 0;
    dut->install_valid_i  = 0;
    dut->evict_valid_i    = 0;
    dut->data_valid_i     = 0;
    dut->clear_mem_hot_i  = 0;
    dut->clear_pend_hot_i = 0;
    dut->rd_req_valid_i   = 0;
    dut->rd_rsp_ready_i   = 1;
}

// Install an entry and wait for data arrival. Returns when entry is installed.
static void do_install(SimCtrl<DUT>& sim, DUT* dut, uint64_t addr,
                       uint32_t data_word) {
    // Step 1: Issue install
    dut->install_valid_i   = 1;
    dut->install_scp_i     = 0;
    dut->install_address_i = addr;
    sim.tick();
    idle(dut);

    // Step 2: Drive data_valid for several cycles to cover the pipeline
    // For ram_delay=2, dc is at d5 (5 ticks after install).
    // We drive data continuously so it's available when dc fires.
    dut->data_valid_i   = 1;
    dut->data_scp_i     = 0;
    dut->data_address_i = addr;
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = data_word;

    // For ram_delay=2, dc is 6 ticks after install. Drive data for 8 to be safe.
    for (int i = 0; i < 8; i++) sim.tick();
    idle(dut);
    sim.tick();
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* dut = sim.dut.get();

    dut->esr_ram_delay_i      = 2;
    dut->esr_l2_rbuf_enable_i = 1;
    dut->my_bank_id_i         = 0;
    idle(dut);
    sim.reset();

    // ── Test 1: Install + data arrival ──────────────────
    printf("test 1: install and data arrival\n");
    do_install(sim, dut, make_addr(0x100), 0xCAFEBABE);
    sim.check((dut->state_valid_o & 0xFF) != 0, "entry valid after install");

    // ── Test 2: Read from rbuf via FIFO ─────────────────
    printf("test 2: read from rbuf\n");
    {
        dut->rd_rsp_ready_i   = 0; // hold rsp so we can check it
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = 0;
        sim.tick(); // FIFO captures push
        dut->rd_req_valid_i = 0;
        sim.tick(); // FIFO output valid
        sim.check(dut->rd_rsp_valid_o == 1, "rd_rsp valid");
        sim.check(dut->rd_rsp_data_o[0] == 0xCAFEBABE, "rd_rsp data matches");
        // Pop the response
        dut->rd_rsp_ready_i = 1;
        sim.tick();
        idle(dut);
    }

    // ── Test 3: Invalidate all ──────────────────────────
    printf("test 3: invalidate all\n");
    {
        dut->invalidate_i = 1;
        sim.tick();
        idle(dut);
        sim.tick();
        sim.check(dut->state_valid_o == 0, "all entries cleared");
    }

    // ── Test 4: Fill buffer (8 entries) ─────────────────
    printf("test 4: fill buffer\n");
    for (int i = 0; i < 8; i++) {
        do_install(sim, dut, make_addr(0x200 + i), 0xBEEF0000 + i);
    }
    sim.check(dut->state_valid_o == 0xFF, "all 8 entries valid");

    // ── Test 5: Install when full → eviction ────────────
    printf("test 5: install when full causes eviction\n");
    {
        dut->install_valid_i   = 1;
        dut->install_scp_i     = 0;
        dut->install_address_i = make_addr(0x300);
        sim.tick();
        sim.check(dut->state_evict_o == 1, "eviction on full install");
        idle(dut);
        sim.tick();
    }

    // ── Test 6: Evict clears entry ──────────────────────
    printf("test 6: evict clears matching entry\n");
    {
        dut->invalidate_i = 1;
        sim.tick();
        idle(dut);
        // Clear any stale pending installs from test 5
        dut->clear_pend_hot_i = 0xFF;
        sim.tick();
        idle(dut);
        for (int i = 0; i < 10; i++) sim.tick(); // let pipeline drain
        do_install(sim, dut, make_addr(0x400), 0x12345678);
        uint8_t valid_before = dut->state_valid_o;
        sim.check(valid_before != 0, "entry valid before evict");

        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 0;
        dut->evict_address_i = make_addr(0x400);
        sim.tick();
        idle(dut);
        sim.tick();
        sim.check(dut->state_valid_o == 0, "entry cleared after evict");
    }

    // ── Test 7: Clear mem valid hot ─────────────────────
    printf("test 7: clear mem valid hot\n");
    {
        dut->invalidate_i = 1;
        sim.tick();
        idle(dut);
        dut->clear_pend_hot_i = 0xFF;
        sim.tick();
        idle(dut);
        for (int i = 0; i < 10; i++) sim.tick();
        do_install(sim, dut, make_addr(0x500), 0xAAAAAAAA);
        // Wait for any residual pipeline activity
        for (int i = 0; i < 4; i++) sim.tick();

        sim.check(dut->state_valid_o != 0, "entry valid before clear");
        dut->clear_mem_hot_i = 0xFF; // clear all
        sim.tick();
        idle(dut);
        sim.check(dut->state_valid_o == 0, "entry cleared by hot vector");
    }

    // ── Test 8: FIFO backpressure ───────────────────────
    printf("test 8: FIFO backpressure\n");
    {
        dut->invalidate_i = 1;
        sim.tick();
        idle(dut);
        dut->clear_pend_hot_i = 0xFF;
        sim.tick();
        idle(dut);
        for (int i = 0; i < 10; i++) sim.tick();
        do_install(sim, dut, make_addr(0x600), 0x55555555);

        // Fill FIFO (depth=4) without popping
        dut->rd_rsp_ready_i = 0;
        for (int i = 0; i < 4; i++) {
            dut->rd_req_valid_i   = 1;
            dut->rd_req_rbuf_id_i = 0;
            sim.tick();
            dut->rd_req_valid_i = 0;
            sim.tick();
        }
        sim.check(dut->rd_req_ready_o == 0, "rd_req blocked when FIFO full");

        // Drain
        dut->rd_rsp_ready_i = 1;
        for (int i = 0; i < 6; i++) sim.tick();
        sim.check(dut->rd_rsp_valid_o == 0, "FIFO drained");
    }

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Comprehensive cosim: shire_cache_pipe_rbuf vs shirecache_pipe_rbuf
//
// Covers: ram_delay 2/3/4, SCP entries, squash-by-evict during pending pipeline,
// concurrent install+read MRU, FIFO backpressure, clear_pending_install at
// various pipeline stages, mixed L2/SCP, invalidate with in-flight installs.

#include "Vcosim_rbuf_tb.h"
#include "cosim_ctrl.h"
#include <cstring>

using DUT = Vcosim_rbuf_tb;

static uint64_t make_addr(uint32_t id) { return ((uint64_t)id) << 8; }

static void idle(DUT* dut) {
    dut->invalidate_i      = 0;
    dut->install_valid_i   = 0;
    dut->evict_valid_i     = 0;
    dut->data_valid_i      = 0;
    dut->clear_mem_hot_i   = 0;
    dut->clear_pend_hot_i  = 0;
    dut->rd_req_valid_i    = 0;
    dut->rd_rsp_ready_i    = 1;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    sim.compare("rd_req_ready",     (uint8_t)dut->orig_rd_req_ready_o,     (uint8_t)dut->new_rd_req_ready_o);
    sim.compare("rd_rsp_valid",     (uint8_t)dut->orig_rd_rsp_valid_o,     (uint8_t)dut->new_rd_rsp_valid_o);
    sim.compare("state_valid",      dut->orig_state_valid_o,               dut->new_state_valid_o);
    sim.compare("state_evict",      (uint8_t)dut->orig_state_evict_o,      (uint8_t)dut->new_state_evict_o);
    if (dut->orig_state_evict_o || dut->new_state_evict_o) {
        sim.compare("evict_id",     dut->orig_state_evict_id_o,   dut->new_state_evict_id_o);
        sim.compare("evict_addr",   dut->orig_state_evict_addr_o, dut->new_state_evict_addr_o);
    }
    sim.compare("pend_inst",        (uint8_t)dut->orig_state_pend_install_o, (uint8_t)dut->new_state_pend_install_o);
    if (dut->orig_rd_rsp_valid_o && dut->new_rd_rsp_valid_o) {
        for (int i = 0; i < 16; i++)
            sim.compare("rsp_data_w" + std::to_string(i),
                        dut->orig_rd_rsp_data_o[i], dut->new_rd_rsp_data_o[i]);
    }
}

// Tick N times, comparing every cycle
static void tick_compare(CosimCtrl<DUT>& sim, int n) {
    for (int i = 0; i < n; i++) { sim.tick(); compare_all(sim); }
}

// Full install sequence: issue install, drive data for enough cycles, wait for completion
static void do_install(CosimCtrl<DUT>& sim, DUT* dut, uint64_t addr,
                       uint32_t data_word, bool scp = false, uint8_t reqq_id = 0) {
    dut->install_valid_i   = 1;
    dut->install_scp_i     = scp;
    dut->install_address_i = addr;
    dut->install_reqq_id_i = reqq_id;
    sim.tick(); compare_all(sim);
    idle(dut);
    dut->data_valid_i   = 1;
    dut->data_scp_i     = scp;
    dut->data_address_i = addr;
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = data_word;
    tick_compare(sim, 10); // enough for any ram_delay
    idle(dut);
    sim.tick(); compare_all(sim);
}

// Reset to clean state
static void full_reset(CosimCtrl<DUT>& sim, DUT* dut) {
    dut->invalidate_i = 1;
    sim.tick(); compare_all(sim);
    idle(dut);
    dut->clear_pend_hot_i = 0xFF;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 12);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 5000000;
    auto* dut = sim.dut.get();

    // ════════════════════════════════════════════════════════
    // PART A: RAM delay = 2
    // ════════════════════════════════════════════════════════
    printf("=== PART A: ram_delay=2 ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_l2_rbuf_enable_i  = 1;
    dut->esr_scp_rbuf_enable_i = 0;
    dut->my_bank_id_i          = 0;
    idle(dut);
    sim.reset();

    // A1: Install 8 entries, read them all
    printf("  A1: install 8 + read all\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x100 + i), 0xA1000000 + i);
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // A2: Install overflow — 8 more on top of full buffer
    printf("  A2: install overflow (8 more)\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x200 + i), 0xA2000000 + i);

    // A3: Evict all current entries
    printf("  A3: evict all\n");
    for (int i = 0; i < 8; i++) {
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 0;
        dut->evict_address_i = make_addr(0x200 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
    }

    // A4: Install then squash pending by eviction at each pipeline stage
    // For ram_delay=2, dc at d5 (5 ticks). Evict must arrive before dc.
    printf("  A4: squash pending install by eviction\n");
    full_reset(sim, dut);
    for (int delay_ticks = 1; delay_ticks <= 4; delay_ticks++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0x300 + delay_ticks);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, delay_ticks);
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 0;
        dut->evict_address_i = make_addr(0x300 + delay_ticks);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 12);
    }

    // A5: Clear pending at different pipeline stages
    // For ram_delay=2, dc at d5 (5 ticks). Clear must arrive before dc.
    printf("  A5: clear pending install at various stages\n");
    full_reset(sim, dut);
    for (int delay_ticks = 1; delay_ticks <= 4; delay_ticks++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0x400 + delay_ticks);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, delay_ticks);
        dut->clear_pend_hot_i = 0xFF;
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 12);
    }

    // A6: Concurrent install + rd_req MRU update
    printf("  A6: concurrent install + read MRU\n");
    full_reset(sim, dut);
    // Install 4 entries
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x500 + i), 0xA6000000 + i);
    // Install 5th entry while simultaneously reading entry 0 (MRU update)
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0x504);
    dut->rd_req_valid_i    = 1;
    dut->rd_req_rbuf_id_i  = 0;
    sim.tick(); compare_all(sim);
    idle(dut);
    dut->data_valid_i   = 1;
    dut->data_address_i = make_addr(0x504);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xA6000004;
    tick_compare(sim, 10);
    idle(dut);
    tick_compare(sim, 3);

    // A7: FIFO backpressure — fill FIFO, stall, drain
    printf("  A7: FIFO backpressure\n");
    full_reset(sim, dut);
    do_install(sim, dut, make_addr(0x600), 0xA7CAFE00);
    dut->rd_rsp_ready_i = 0; // stall consumer
    for (int i = 0; i < 4; i++) {
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
    }
    // FIFO should be full now — compare rd_req_ready
    sim.tick(); compare_all(sim);
    // Drain one at a time
    for (int i = 0; i < 4; i++) {
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 0;
        sim.tick(); compare_all(sim);
    }
    idle(dut);
    tick_compare(sim, 3);

    // A8: Invalidate with in-flight pending install
    printf("  A8: invalidate with in-flight install\n");
    full_reset(sim, dut);
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0x700);
    sim.tick(); compare_all(sim);
    idle(dut);
    sim.tick(); compare_all(sim);
    dut->invalidate_i = 1;
    sim.tick(); compare_all(sim);
    idle(dut);
    // Data still arrives — pending install should still complete
    dut->data_valid_i   = 1;
    dut->data_address_i = make_addr(0x700);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xA8000000;
    tick_compare(sim, 10);
    idle(dut);
    tick_compare(sim, 3);

    // A9: Clear mem valid hot for individual entries
    printf("  A9: selective clear_mem_hot\n");
    full_reset(sim, dut);
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x800 + i), 0xA9000000 + i);
    // Clear every other entry
    dut->clear_mem_hot_i = 0x55;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    // Clear remaining
    dut->clear_mem_hot_i = 0xAA;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);

    // ════════════════════════════════════════════════════════
    // PART B: RAM delay = 3
    // ════════════════════════════════════════════════════════
    printf("=== PART B: ram_delay=3 ===\n");
    dut->esr_ram_delay_i = 3;
    full_reset(sim, dut);

    // B1: Install 8 + read
    printf("  B1: install 8 + read\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0xB00 + i), 0xB1000000 + i);
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // B2: Overflow + evict
    printf("  B2: overflow + evict\n");
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xC00 + i), 0xB2000000 + i);
    for (int i = 0; i < 4; i++) {
        dut->evict_valid_i   = 1;
        dut->evict_address_i = make_addr(0xC00 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
    }

    // B3: Squash pending by eviction (ram_delay=3, dc at d7 → max delay 6)
    printf("  B3: squash pending\n");
    full_reset(sim, dut);
    for (int d = 1; d <= 6; d++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0xD00 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, d);
        dut->evict_valid_i   = 1;
        dut->evict_address_i = make_addr(0xD00 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 12);
    }

    // ════════════════════════════════════════════════════════
    // PART C: RAM delay = 4
    // ════════════════════════════════════════════════════════
    printf("=== PART C: ram_delay=4 ===\n");
    dut->esr_ram_delay_i = 4;
    full_reset(sim, dut);

    // C1: Install 8 + read
    printf("  C1: install 8 + read\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0xE00 + i), 0xC1000000 + i);
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // C2: Squash pending (ram_delay=4, dc at d7 → max delay 6)
    printf("  C2: squash pending\n");
    full_reset(sim, dut);
    for (int d = 1; d <= 6; d++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0xF00 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, d);
        dut->evict_valid_i   = 1;
        dut->evict_address_i = make_addr(0xF00 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 14);
    }

    // ════════════════════════════════════════════════════════
    // PART D: SCP entries (both L2 and SCP enabled)
    // ════════════════════════════════════════════════════════
    printf("=== PART D: SCP entries ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_l2_rbuf_enable_i  = 1;
    dut->esr_scp_rbuf_enable_i = 1;
    full_reset(sim, dut);

    // D1: Install 4 L2 + 4 SCP at same addresses (separate namespaces)
    printf("  D1: mixed L2 + SCP installs\n");
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x1000 + i), 0xD1000000 + i, false);
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x1000 + i), 0xD1100000 + i, true);

    // D2: Read all entries
    printf("  D2: read all\n");
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // D3: Evict only SCP entries
    printf("  D3: evict SCP entries\n");
    for (int i = 0; i < 4; i++) {
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 1;
        dut->evict_address_i = make_addr(0x1000 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
    }
    // L2 entries should still be valid
    tick_compare(sim, 3);

    // D4: Overflow with mixed L2/SCP
    printf("  D4: overflow with mixed types\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x1100 + i), 0xD4000000 + i, (i & 1));

    // ════════════════════════════════════════════════════════
    // PART E: Edge cases
    // ════════════════════════════════════════════════════════
    printf("=== PART E: edge cases ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_scp_rbuf_enable_i = 0;
    full_reset(sim, dut);

    // E1: Rapid back-to-back installs (no idle between)
    printf("  E1: back-to-back installs\n");
    for (int i = 0; i < 4; i++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0x2000 + i);
        sim.tick(); compare_all(sim);
    }
    idle(dut);
    dut->data_valid_i = 1;
    dut->data_address_i = make_addr(0x2000);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xE1000000;
    tick_compare(sim, 15);
    idle(dut);
    tick_compare(sim, 3);

    // E2: Different bank_id
    printf("  E2: different bank_id\n");
    full_reset(sim, dut);
    dut->my_bank_id_i = 3;
    do_install(sim, dut, make_addr(0x3000), 0xE2000000);
    dut->rd_rsp_ready_i   = 0;
    dut->rd_req_valid_i   = 1;
    dut->rd_req_rbuf_id_i = 0;
    sim.tick(); compare_all(sim);
    dut->rd_req_valid_i = 0;
    sim.tick(); compare_all(sim);
    dut->rd_rsp_ready_i = 1;
    sim.tick(); compare_all(sim);
    dut->my_bank_id_i = 0;

    // E3: Install duplicate address (should be treated as MRU, not double install)
    printf("  E3: duplicate address install\n");
    full_reset(sim, dut);
    do_install(sim, dut, make_addr(0x4000), 0xE3000001);
    // Install same address again — should match and NOT create new entry
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0x4000);
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 10);

    // ════════════════════════════════════════════════════════
    // PART F: Data content verification
    // ════════════════════════════════════════════════════════
    printf("=== PART F: data content ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_scp_rbuf_enable_i = 0;
    full_reset(sim, dut);

    // F1: Install 8 entries with distinct data, read ALL and verify content
    printf("  F1: install 8 with distinct data, read all back\n");
    for (int i = 0; i < 8; i++) {
        uint32_t pattern = 0xF1000000 | (i << 16) | (i * 0x1111);
        do_install(sim, dut, make_addr(0x5000 + i), pattern);
    }
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        // Hold rsp and compare data
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // F2: Overflow install — read new data to verify replacement
    printf("  F2: overflow then read replacement data\n");
    // Install 4 more to replace LRU entries
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x5100 + i), 0xF2000000 + i);
    // Read all 8 entries — some have new data, some old
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // ════════════════════════════════════════════════════════
    // PART G: LRU ordering verification
    // ════════════════════════════════════════════════════════
    printf("=== PART G: LRU ordering ===\n");
    full_reset(sim, dut);

    // G1: Install 8, then 9th triggers eviction of LRU
    printf("  G1: LRU victim is oldest entry\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x6000 + i), 0x61000000 + i);
    // 9th install replaces LRU
    do_install(sim, dut, make_addr(0x6100), 0x61100000);

    // G2: MRU promotion via reads changes eviction order
    printf("  G2: MRU promotion changes victim\n");
    full_reset(sim, dut);
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x6200 + i), 0x62000000 + i);
    // Touch entries 0,1 via reads to promote them to MRU
    for (int i = 0; i < 2; i++) {
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }
    // Install 2 more — should evict entries 2,3 (oldest untouched), not 0,1
    for (int i = 0; i < 2; i++)
        do_install(sim, dut, make_addr(0x6300 + i), 0x63000000 + i);
    // Read all to verify data content
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // G3: Multiple MRU promotions then overflow
    printf("  G3: multiple MRU promotions\n");
    full_reset(sim, dut);
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x6400 + i), 0x64000000 + i);
    // Touch entries 0,1,2,3 via reads to promote them
    for (int i = 0; i < 4; i++) {
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }
    // Install 4 more — should evict entries 4,5,6,7 (oldest untouched)
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x6500 + i), 0x65000000 + i);

    // ════════════════════════════════════════════════════════
    // PART H: Simultaneous operations
    // ════════════════════════════════════════════════════════
    printf("=== PART H: simultaneous ops ===\n");
    full_reset(sim, dut);

    // H1: clear_mem_hot + clear_pend_hot simultaneously
    printf("  H1: simultaneous clear_mem + clear_pend\n");
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x7000 + i), 0x71000000 + i);
    // Start a pending install then immediately clear both mem and pending
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0x7100);
    sim.tick(); compare_all(sim);
    idle(dut);
    // Simultaneously clear mem entries 0,1 and all pending
    dut->clear_mem_hot_i  = 0x03;
    dut->clear_pend_hot_i = 0xFF;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 12);

    // H2: Evict of a valid entry while another entry has pending install
    printf("  H2: evict valid + pending install\n");
    full_reset(sim, dut);
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0x7200 + i), 0x72000000 + i);
    // Start pending install for NEW address, then evict a DIFFERENT valid entry
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0x7300);
    sim.tick(); compare_all(sim);
    idle(dut);
    sim.tick(); compare_all(sim);
    // Evict entry at addr 0x7200 (different from pending addr 0x7300)
    dut->evict_valid_i   = 1;
    dut->evict_address_i = make_addr(0x7200);
    sim.tick(); compare_all(sim);
    idle(dut);
    // Provide data for the pending install
    dut->data_valid_i   = 1;
    dut->data_address_i = make_addr(0x7300);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0x73000000;
    tick_compare(sim, 10);
    idle(dut);
    tick_compare(sim, 3);
    // Read back to verify data
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // H3: Read request during eviction cycle
    printf("  H3: read during eviction\n");
    full_reset(sim, dut);
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0x7400 + i), 0x73000000 + i);
    // Simultaneously read entry 3 and evict entry 5
    dut->rd_req_valid_i   = 1;
    dut->rd_req_rbuf_id_i = 3;
    dut->evict_valid_i    = 1;
    dut->evict_address_i  = make_addr(0x7405);
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    dut->rd_rsp_ready_i = 1;
    tick_compare(sim, 2);

    // ════════════════════════════════════════════════════════
    // PART I: Stress — rapid mixed operations
    // ════════════════════════════════════════════════════════
    printf("=== PART I: stress ===\n");
    full_reset(sim, dut);

    // I1: Alternating install and evict
    printf("  I1: alternating install/evict\n");
    for (int round = 0; round < 3; round++) {
        // Install 8
        for (int i = 0; i < 8; i++)
            do_install(sim, dut, make_addr(0x8000 + round*0x100 + i), 0x80000000 + round*0x100 + i);
        // Read all
        for (int i = 0; i < 8; i++) {
            dut->rd_rsp_ready_i   = 0;
            dut->rd_req_valid_i   = 1;
            dut->rd_req_rbuf_id_i = i;
            sim.tick(); compare_all(sim);
            dut->rd_req_valid_i = 0;
            sim.tick(); compare_all(sim);
            dut->rd_rsp_ready_i = 1;
            sim.tick(); compare_all(sim);
        }
        // Evict 4
        for (int i = 0; i < 4; i++) {
            dut->evict_valid_i   = 1;
            dut->evict_address_i = make_addr(0x8000 + round*0x100 + i);
            sim.tick(); compare_all(sim);
            idle(dut);
            sim.tick(); compare_all(sim);
        }
    }

    // I2: FIFO stress — rapid push/pop interleaving
    printf("  I2: FIFO push/pop interleave\n");
    full_reset(sim, dut);
    do_install(sim, dut, make_addr(0x9000), 0x90000000);
    for (int i = 0; i < 16; i++) {
        // Push
        dut->rd_rsp_ready_i   = (i % 3 != 0); // stall every 3rd
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
    }
    // Drain remaining
    dut->rd_rsp_ready_i = 1;
    tick_compare(sim, 10);

    // I3: All ram_delays with overflow + read + evict
    printf("  I3: all delays with full cycle\n");
    for (int delay = 2; delay <= 4; delay++) {
        dut->esr_ram_delay_i = delay;
        full_reset(sim, dut);
        // Fill
        for (int i = 0; i < 8; i++)
            do_install(sim, dut, make_addr(0xA000 + delay*0x100 + i), 0xA0000000 + delay*0x100 + i);
        // Overflow 4
        for (int i = 0; i < 4; i++)
            do_install(sim, dut, make_addr(0xA000 + delay*0x100 + 0x80 + i), 0xA0800000 + delay*0x100 + i);
        // Read all
        for (int i = 0; i < 8; i++) {
            dut->rd_rsp_ready_i   = 0;
            dut->rd_req_valid_i   = 1;
            dut->rd_req_rbuf_id_i = i;
            sim.tick(); compare_all(sim);
            dut->rd_req_valid_i = 0;
            sim.tick(); compare_all(sim);
            dut->rd_rsp_ready_i = 1;
            sim.tick(); compare_all(sim);
        }
        // Evict all
        for (int i = 0; i < 8; i++) {
            // We don't know exact addresses after LRU replacement,
            // so invalidate instead
        }
        dut->invalidate_i = 1;
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 3);
    }

    // ════════════════════════════════════════════════════════
    // PART J: SCP squash-by-evict
    // ════════════════════════════════════════════════════════
    printf("=== PART J: SCP squash ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_l2_rbuf_enable_i  = 1;
    dut->esr_scp_rbuf_enable_i = 1;
    full_reset(sim, dut);

    // J1: Install SCP entry, squash by SCP evict at each pipeline stage
    printf("  J1: SCP squash by evict (delay=2)\n");
    for (int d = 1; d <= 4; d++) {
        dut->install_valid_i   = 1;
        dut->install_scp_i     = 1;
        dut->install_address_i = make_addr(0xB000 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, d);
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 1;
        dut->evict_address_i = make_addr(0xB000 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 12);
    }

    // J2: SCP squash with ram_delay=3
    printf("  J2: SCP squash (delay=3)\n");
    dut->esr_ram_delay_i = 3;
    full_reset(sim, dut);
    for (int d = 1; d <= 6; d++) {
        dut->install_valid_i   = 1;
        dut->install_scp_i     = 1;
        dut->install_address_i = make_addr(0xB100 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, d);
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 1;
        dut->evict_address_i = make_addr(0xB100 + d);
        sim.tick(); compare_all(sim);
        idle(dut);
        tick_compare(sim, 12);
    }

    // J3: L2 evict does NOT squash SCP pending install (different namespace)
    printf("  J3: L2 evict ignores SCP pending\n");
    dut->esr_ram_delay_i = 2;
    full_reset(sim, dut);
    dut->install_valid_i   = 1;
    dut->install_scp_i     = 1;
    dut->install_address_i = make_addr(0xB200);
    sim.tick(); compare_all(sim);
    idle(dut);
    sim.tick(); compare_all(sim);
    // L2 evict at same address — should NOT squash the SCP pending
    dut->evict_valid_i   = 1;
    dut->evict_scp_i     = 0; // L2, not SCP
    dut->evict_address_i = make_addr(0xB200);
    sim.tick(); compare_all(sim);
    idle(dut);
    // SCP pending should still complete with data
    dut->data_valid_i   = 1;
    dut->data_scp_i     = 1;
    dut->data_address_i = make_addr(0xB200);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xB2000000;
    tick_compare(sim, 10);
    idle(dut);
    tick_compare(sim, 3);

    // ════════════════════════════════════════════════════════
    // PART K: SCP+L2 coexistence under eviction
    // ════════════════════════════════════════════════════════
    printf("=== PART K: SCP+L2 coexistence ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_scp_rbuf_enable_i = 1;
    full_reset(sim, dut);

    // K1: Install 4 L2 + 4 SCP at same addrs, evict SCP, read L2 to verify data
    printf("  K1: evict SCP, verify L2 data survives\n");
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xC000 + i), 0xC0000000 + i, false);
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xC000 + i), 0xC0100000 + i, true);
    // Evict all SCP entries
    for (int i = 0; i < 4; i++) {
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 1;
        dut->evict_address_i = make_addr(0xC000 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
    }
    // Read all entries — L2 entries should have original data
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // K2: Evict L2, verify SCP survives
    printf("  K2: evict L2, verify SCP data survives\n");
    full_reset(sim, dut);
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xC100 + i), 0xC1000000 + i, false);
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xC100 + i), 0xC1100000 + i, true);
    // Evict L2 entries
    for (int i = 0; i < 4; i++) {
        dut->evict_valid_i   = 1;
        dut->evict_scp_i     = 0;
        dut->evict_address_i = make_addr(0xC100 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
    }
    // Read all — SCP entries should have data
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // ════════════════════════════════════════════════════════
    // PART L: Data readback after MRU overflow
    // ════════════════════════════════════════════════════════
    printf("=== PART L: data after MRU overflow ===\n");
    dut->esr_scp_rbuf_enable_i = 0;
    full_reset(sim, dut);

    // L1: Install 8, promote 0-3 via reads, overflow 4 — verify surviving data
    printf("  L1: promote then overflow, verify survivors\n");
    for (int i = 0; i < 8; i++)
        do_install(sim, dut, make_addr(0xD000 + i), 0xD0000000 + i);
    // Promote 0,1,2,3
    for (int i = 0; i < 4; i++) {
        dut->rd_rsp_ready_i = 1;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        idle(dut);
        sim.tick(); compare_all(sim);
        sim.tick(); compare_all(sim);
    }
    // Overflow 4 — should evict 4,5,6,7
    for (int i = 0; i < 4; i++)
        do_install(sim, dut, make_addr(0xD100 + i), 0xD1000000 + i);
    // Read ALL 8 and compare data
    for (int i = 0; i < 8; i++) {
        dut->rd_rsp_ready_i   = 0;
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = i;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
        sim.tick(); compare_all(sim);
        dut->rd_rsp_ready_i = 1;
        sim.tick(); compare_all(sim);
    }

    // ════════════════════════════════════════════════════════
    // PART M: FIFO exact boundary + invalidate with FIFO data
    // ════════════════════════════════════════════════════════
    printf("=== PART M: FIFO boundaries ===\n");
    full_reset(sim, dut);

    // M1: Exact FIFO depth boundary — push exactly 4, verify 5th blocked
    printf("  M1: exact FIFO depth boundary\n");
    do_install(sim, dut, make_addr(0xE000), 0xE0000000);
    dut->rd_rsp_ready_i = 0;
    for (int i = 0; i < 4; i++) {
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
    }
    // 5th push — should be rejected (rd_req_ready = 0)
    dut->rd_req_valid_i   = 1;
    dut->rd_req_rbuf_id_i = 0;
    sim.tick(); compare_all(sim);
    dut->rd_req_valid_i = 0;
    sim.tick(); compare_all(sim);
    // Pop one, verify next push accepted
    dut->rd_rsp_ready_i = 1;
    sim.tick(); compare_all(sim);
    dut->rd_rsp_ready_i = 0;
    sim.tick(); compare_all(sim);
    dut->rd_req_valid_i   = 1;
    dut->rd_req_rbuf_id_i = 0;
    sim.tick(); compare_all(sim);
    dut->rd_req_valid_i = 0;
    // Drain
    dut->rd_rsp_ready_i = 1;
    tick_compare(sim, 10);

    // M2: Invalidate while FIFO has unpopped responses
    printf("  M2: invalidate with FIFO data pending\n");
    full_reset(sim, dut);
    do_install(sim, dut, make_addr(0xE100), 0xE1000000);
    // Push 2 responses into FIFO
    dut->rd_rsp_ready_i = 0;
    for (int i = 0; i < 2; i++) {
        dut->rd_req_valid_i   = 1;
        dut->rd_req_rbuf_id_i = 0;
        sim.tick(); compare_all(sim);
        dut->rd_req_valid_i = 0;
        sim.tick(); compare_all(sim);
    }
    // Invalidate while FIFO has data
    dut->invalidate_i = 1;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    // FIFO should still have data to pop (invalidate clears rbuf, not FIFO)
    dut->rd_rsp_ready_i = 1;
    tick_compare(sim, 5);

    // ════════════════════════════════════════════════════════
    // PART N: Multiple pending installs + selective clear
    // ════════════════════════════════════════════════════════
    printf("=== PART N: multiple pending + selective clear ===\n");
    dut->esr_ram_delay_i = 4; // longer pipeline for more overlap
    full_reset(sim, dut);

    // N1: 3 back-to-back installs, clear only the 2nd pending
    printf("  N1: 3 installs, clear middle pending\n");
    for (int i = 0; i < 3; i++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0xF000 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
    }
    // Wait 1 tick then clear only entry 1's pending (LRU assigns 0,1,2 sequentially)
    sim.tick(); compare_all(sim);
    dut->clear_pend_hot_i = 0x02; // only entry 1
    sim.tick(); compare_all(sim);
    idle(dut);
    // Provide data for remaining installs
    dut->data_valid_i   = 1;
    dut->data_address_i = make_addr(0xF000);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xF0000000;
    tick_compare(sim, 12);
    idle(dut);
    tick_compare(sim, 3);

    // N2: 4 back-to-back installs, clear entries 0+2, keep 1+3
    printf("  N2: 4 installs, clear alternating\n");
    full_reset(sim, dut);
    for (int i = 0; i < 4; i++) {
        dut->install_valid_i   = 1;
        dut->install_address_i = make_addr(0xF100 + i);
        sim.tick(); compare_all(sim);
        idle(dut);
    }
    sim.tick(); compare_all(sim);
    dut->clear_pend_hot_i = 0x05; // entries 0 and 2
    sim.tick(); compare_all(sim);
    idle(dut);
    dut->data_valid_i   = 1;
    dut->data_address_i = make_addr(0xF100);
    memset(dut->data_data_i.data(), 0, 64);
    dut->data_data_i[0] = 0xF1000000;
    tick_compare(sim, 12);
    idle(dut);
    tick_compare(sim, 3);

    // ════════════════════════════════════════════════════════
    // PART O: ram_delay 3,4 data content verification
    // ════════════════════════════════════════════════════════
    printf("=== PART O: delay 3,4 data content ===\n");

    for (int delay = 3; delay <= 4; delay++) {
        printf("  O%d: delay=%d install+read+overflow+read\n", delay-2, delay);
        dut->esr_ram_delay_i       = delay;
        dut->esr_scp_rbuf_enable_i = 0;
        full_reset(sim, dut);
        // Install 8 with distinct data
        for (int i = 0; i < 8; i++)
            do_install(sim, dut, make_addr(0xF200 + delay*0x100 + i), 0xF2000000 + delay*0x1000 + i);
        // Read all
        for (int i = 0; i < 8; i++) {
            dut->rd_rsp_ready_i   = 0;
            dut->rd_req_valid_i   = 1;
            dut->rd_req_rbuf_id_i = i;
            sim.tick(); compare_all(sim);
            dut->rd_req_valid_i = 0;
            sim.tick(); compare_all(sim);
            dut->rd_rsp_ready_i = 1;
            sim.tick(); compare_all(sim);
        }
        // Overflow 4
        for (int i = 0; i < 4; i++)
            do_install(sim, dut, make_addr(0xF200 + delay*0x100 + 0x80 + i), 0xF2800000 + delay*0x1000 + i);
        // Read all again
        for (int i = 0; i < 8; i++) {
            dut->rd_rsp_ready_i   = 0;
            dut->rd_req_valid_i   = 1;
            dut->rd_req_rbuf_id_i = i;
            sim.tick(); compare_all(sim);
            dut->rd_req_valid_i = 0;
            sim.tick(); compare_all(sim);
            dut->rd_rsp_ready_i = 1;
            sim.tick(); compare_all(sim);
        }
    }

    // ════════════════════════════════════════════════════════
    // PART P: No-op and boundary conditions
    // ════════════════════════════════════════════════════════
    printf("=== PART P: no-ops and boundaries ===\n");
    dut->esr_ram_delay_i       = 2;
    dut->esr_scp_rbuf_enable_i = 0;
    full_reset(sim, dut);

    // P1: Evict non-matching address — should be no-op
    printf("  P1: evict non-matching address\n");
    do_install(sim, dut, make_addr(0xFF00), 0xFF000000);
    dut->evict_valid_i   = 1;
    dut->evict_address_i = make_addr(0xFF99); // different address
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    // Entry should still be valid
    tick_compare(sim, 2);

    // P2: Read when rbuf_enable=0 — should not push to FIFO
    printf("  P2: read when disabled\n");
    full_reset(sim, dut);
    do_install(sim, dut, make_addr(0xFF10), 0xFF100000);
    dut->esr_l2_rbuf_enable_i = 0;
    dut->rd_req_valid_i   = 1;
    dut->rd_req_rbuf_id_i = 0;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    dut->esr_l2_rbuf_enable_i = 1;

    // P3: Install when rbuf_enable=0 — should be ignored
    printf("  P3: install when disabled\n");
    full_reset(sim, dut);
    dut->esr_l2_rbuf_enable_i = 0;
    dut->install_valid_i   = 1;
    dut->install_address_i = make_addr(0xFF20);
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 12);
    dut->esr_l2_rbuf_enable_i = 1;

    // P4: Evict when rbuf_enable=0
    printf("  P4: evict when disabled\n");
    full_reset(sim, dut);
    dut->esr_l2_rbuf_enable_i = 1;
    do_install(sim, dut, make_addr(0xFF30), 0xFF300000);
    dut->esr_l2_rbuf_enable_i = 0;
    dut->evict_valid_i   = 1;
    dut->evict_address_i = make_addr(0xFF30);
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 3);
    dut->esr_l2_rbuf_enable_i = 1;

    // P5: clear_mem_hot on empty buffer — no-op
    printf("  P5: clear on empty buffer\n");
    full_reset(sim, dut);
    dut->clear_mem_hot_i = 0xFF;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 2);

    // P6: clear_pend_hot with no pending — no-op
    printf("  P6: clear_pend with nothing pending\n");
    dut->clear_pend_hot_i = 0xFF;
    sim.tick(); compare_all(sim);
    idle(dut);
    tick_compare(sim, 2);

    // P7: Double invalidate
    printf("  P7: double invalidate\n");
    do_install(sim, dut, make_addr(0xFF40), 0xFF400000);
    dut->invalidate_i = 1;
    sim.tick(); compare_all(sim);
    sim.tick(); compare_all(sim); // second invalidate on already-empty
    idle(dut);
    tick_compare(sim, 2);

    return sim.finish();
}

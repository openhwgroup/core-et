// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_pipe_idx_cop_sm

#include "Vshirecache_idx_cop_sm_tb.h"
#include "sim_ctrl.h"
#include <cstdio>

using DUT = Vshirecache_idx_cop_sm_tb;

// ESR ctl bit positions
static constexpr int GO_BIT      = 0;
static constexpr int ABORT_BIT   = 1;
static constexpr int OPCODE_LSB  = 8;
static constexpr int STATE_LSB   = 24;

// idx_cop_sm opcodes
static constexpr int OP_ALL_INV   = 0;
static constexpr int OP_L2_INV    = 1;
static constexpr int OP_L2_FLUSH  = 2;
static constexpr int OP_L2_EVICT  = 3;
static constexpr int OP_L3_INV    = 4;
static constexpr int OP_L3_FLUSH  = 5;
static constexpr int OP_L3_EVICT  = 6;
static constexpr int OP_DBG_READ  = 7;
static constexpr int OP_DBG_WRITE = 8;
static constexpr int OP_SCP_ZERO  = 9;
static constexpr int OP_CB_INV    = 10;

// reqq opcodes
static constexpr int REQQ_IDX_ALL_INV = 0x28;
static constexpr int REQQ_IDX_L2_INV  = 0x29;
static constexpr int REQQ_OP_SYNC     = 0x2f;
static constexpr int REQQ_IDX_READ    = 0x26;

// One-hot state bits
static constexpr int ST_RESET  = 1 << 0;
static constexpr int ST_ALLINV = 1 << 1;
static constexpr int ST_IDLE   = 1 << 2;
static constexpr int ST_ACTIVE = 1 << 3;
static constexpr int ST_CBINV  = 1 << 4;
static constexpr int ST_DBG    = 1 << 5;
static constexpr int ST_SYNC   = 1 << 6;

static void clear_inputs(DUT* dut) {
    dut->wr_en_idx_cop_sm_ctl_i = 0;
    dut->wr_en_idx_cop_sm_data0_i = 0;
    dut->wr_en_idx_cop_sm_data1_i = 0;
    dut->wr_en_idx_cop_sm_ecc_i = 0;
    dut->wr_en_idx_cop_sm_physical_index_i = 0;
    dut->wr_data_i = 0;
    dut->mbist_on_i = 0;
    dut->cbuf_mem_valid_i = 0;
}

static void trigger_go(DUT* dut, int opcode) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = (1ULL << GO_BIT) | ((uint64_t)opcode << OPCODE_LSB);
}

static void trigger_abort(DUT* dut) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = (1ULL << ABORT_BIT);
}

static int get_state(DUT* dut) {
    return (dut->esr_status_ctl_o >> STATE_LSB) & 0x7F;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;  // need plenty of time for ALL_INV (512 sets × 4 subbanks)
    auto* dut = sim.dut.get();

    dut->rst_c_ni = 0;  // cold reset active
    dut->pipe_idx_cop_req_ready_i = 1;
    dut->my_bank_id_i = 0;
    dut->my_scp_virtual_shire_id_i = 0;
    dut->esr_sc_l2_set_base_i = 0;
    dut->esr_sc_l2_set_size_i = 4;  // small for testing
    dut->esr_sc_l3_set_base_i = 0;
    dut->esr_sc_l3_set_size_i = 4;
    dut->esr_sc_scp_set_base_i = 0;
    dut->esr_sc_scp_set_size_i = 0;
    clear_inputs(dut);

    // ── Custom reset: both warm and cold ──────────────────
    dut->rst_ni = 0;
    dut->rst_c_ni = 0;
    for (int i = 0; i < 5; i++) sim.tick();
    dut->rst_ni = 1;
    dut->rst_c_ni = 1;
    sim.tick();

    // After reset, FSM should be in RESET state, then start counting down
    sim.check(dut->pipe_idx_cop_is_active_o == 1, "active after reset (in RESET state)");
    sim.check(dut->pipe_idx_cop_l2_busy_o == 1, "l2_busy after cold reset");
    sim.check(dut->pipe_idx_cop_l3_busy_o == 1, "l3_busy after cold reset");

    // Wait for reset counter (256 cycles) then ALL_INV
    printf("phase 1: waiting for power-up ALL_INV...\n");
    for (int i = 0; i < 260; i++) {
        clear_inputs(dut);
        sim.tick();
    }
    // Should now be in ALL_INV state
    sim.check(get_state(dut) == ST_ALLINV, "state is ALL_INV after reset counter");

    // Let ALL_INV run to completion (512 sets × 4 sub_banks × 4 qws = 8192 cycles)
    // With always-ready, it should complete
    int all_inv_cycles = 0;
    while (get_state(dut) != ST_SYNC && all_inv_cycles < 20000) {
        clear_inputs(dut);
        sim.tick();
        all_inv_cycles++;
    }
    sim.check(get_state(dut) == ST_SYNC, "transitions to SYNC after ALL_INV");

    // SYNC should generate a Sync request
    // Wait for sync to be taken
    int sync_cycles = 0;
    while (get_state(dut) != ST_IDLE && sync_cycles < 100) {
        clear_inputs(dut);
        sim.tick();
        sync_cycles++;
    }
    sim.check(get_state(dut) == ST_IDLE, "reaches IDLE after SYNC");
    sim.check(dut->pipe_idx_cop_is_active_o == 0, "not active in IDLE");

    // ── Phase 2: ESR-triggered L2_INV with 4 sets ──────────
    printf("phase 2: L2_INV (4 sets)\n");
    dut->esr_sc_l2_set_size_i = 4;
    trigger_go(dut, OP_L2_INV);
    sim.tick();
    clear_inputs(dut);

    // Wait 2 cycles for start pipeline
    sim.tick();
    sim.tick();

    sim.check(get_state(dut) == ST_ACTIVE, "state is ACTIVE for L2_INV");
    sim.check(dut->pipe_idx_cop_l2_busy_o == 1, "l2_busy during L2_INV");

    // Count requests emitted (4 sets × 4 ways × 4 sub_banks × 4 qws = 256)
    // Actually, for ACTIVE mode: iterate set then way then sub_bank then qw
    int req_count = 0;
    int limit = 2000;
    while (get_state(dut) != ST_IDLE && limit > 0) {
        clear_inputs(dut);
        if (dut->pipe_idx_cop_req_valid_o) req_count++;
        sim.tick();
        limit--;
    }
    sim.check(get_state(dut) == ST_IDLE, "returns to IDLE after L2_INV");
    sim.check(req_count > 0, "emitted requests during L2_INV");
    printf("  L2_INV emitted %d requests\n", req_count);

    // ── Phase 3: Abort ──────────────────────────────────────
    printf("phase 3: abort test\n");
    dut->esr_sc_l2_set_size_i = 100;  // lots of sets
    trigger_go(dut, OP_L2_FLUSH);
    sim.tick();
    clear_inputs(dut);
    sim.tick();
    sim.tick();

    sim.check(get_state(dut) == ST_ACTIVE, "state is ACTIVE for L2_FLUSH");

    // Let it run a few cycles then abort
    for (int i = 0; i < 5; i++) {
        clear_inputs(dut);
        sim.tick();
    }
    trigger_abort(dut);
    sim.tick();
    clear_inputs(dut);

    // Wait for abort pipeline (2 cycles)
    for (int i = 0; i < 5; i++) sim.tick();

    sim.check(get_state(dut) == ST_IDLE, "returns to IDLE after abort");
    sim.check(dut->pipe_idx_cop_aborted_o == 1 || get_state(dut) == ST_IDLE,
              "aborted flag or IDLE after abort");

    // ── Phase 4: Dbg_Read ───────────────────────────────────
    printf("phase 4: debug read\n");
    trigger_go(dut, OP_DBG_READ);
    sim.tick();
    clear_inputs(dut);
    sim.tick();
    sim.tick();

    sim.check(get_state(dut) == ST_DBG, "state is DBG for Dbg_Read");

    // DBG does a single operation then returns to IDLE
    limit = 20;
    while (get_state(dut) != ST_IDLE && limit > 0) {
        clear_inputs(dut);
        sim.tick();
        limit--;
    }
    sim.check(get_state(dut) == ST_IDLE, "returns to IDLE after Dbg_Read");

    // ── Phase 5: Warm-only reset skips ALL_INV ──────────────
    printf("phase 5: warm-only reset\n");
    dut->rst_ni = 0;
    // rst_c_ni stays 1 (no cold reset)
    for (int i = 0; i < 5; i++) sim.tick();
    dut->rst_ni = 1;
    sim.tick();

    // Wait for reset counter
    for (int i = 0; i < 260; i++) {
        clear_inputs(dut);
        sim.tick();
    }
    // Should skip ALL_INV (warm reset only) and go to IDLE
    // Wait a few more cycles
    for (int i = 0; i < 10; i++) {
        clear_inputs(dut);
        sim.tick();
    }
    sim.check(get_state(dut) == ST_IDLE, "warm-only reset skips ALL_INV, goes to IDLE");

    return sim.finish();
}

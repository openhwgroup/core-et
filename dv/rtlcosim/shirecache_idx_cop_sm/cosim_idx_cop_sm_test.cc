// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_pipe_idx_cop_sm vs shirecache_pipe_idx_cop_sm

#include "Vcosim_idx_cop_sm_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>

using DUT = Vcosim_idx_cop_sm_tb;

// ESR ctl bit positions
static constexpr int GO_BIT          = 0;
static constexpr int ABORT_BIT       = 1;
static constexpr int OPCODE_LSB      = 8;
static constexpr int ECC_WR_EN_LSB   = 13;
static constexpr int LOGICAL_RAM_LSB = 14;

// ESR physical_index field positions
static constexpr int QW_LSB       = 0;
static constexpr int SUB_BANK_LSB = 8;
static constexpr int WAY_LSB      = 16;
static constexpr int SET_LSB      = 32;

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

// Logical RAM targets
static constexpr int LRAM_TAG_STATE = 0;
static constexpr int LRAM_TAG       = 1;
static constexpr int LRAM_DATA      = 2;

static void clear_inputs(DUT* dut) {
    dut->wr_en_idx_cop_sm_ctl_i = 0;
    dut->wr_en_idx_cop_sm_data0_i = 0;
    dut->wr_en_idx_cop_sm_data1_i = 0;
    dut->wr_en_idx_cop_sm_ecc_i = 0;
    dut->wr_en_idx_cop_sm_physical_index_i = 0;
    dut->wr_data_i = 0;
    dut->mbist_on_i = 0;
}

static void trigger_go(DUT* dut, int opcode) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = (1ULL << GO_BIT) | ((uint64_t)opcode << OPCODE_LSB);
}

static void trigger_go_with_opts(DUT* dut, int opcode, int logical_ram, bool ecc_wr_en) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = (1ULL << GO_BIT) |
                     ((uint64_t)opcode << OPCODE_LSB) |
                     ((uint64_t)ecc_wr_en << ECC_WR_EN_LSB) |
                     ((uint64_t)logical_ram << LOGICAL_RAM_LSB);
}

static void trigger_abort(DUT* dut) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = (1ULL << ABORT_BIT);
}

static void write_physical_index(DUT* dut, int set, int way, int sbb, int qw) {
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = ((uint64_t)qw << QW_LSB) |
                     ((uint64_t)sbb << SUB_BANK_LSB) |
                     ((uint64_t)way << WAY_LSB) |
                     ((uint64_t)set << SET_LSB);
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* dut = sim.dut.get();
    // Core outputs
    sim.compare("req_valid",      dut->orig_req_valid_o,      dut->new_req_valid_o);
    if (dut->orig_req_valid_o || dut->new_req_valid_o) {
        sim.compare("req_opcode",     dut->orig_req_opcode_o,     dut->new_req_opcode_o);
        sim.compare("req_address",    dut->orig_req_address_o,    dut->new_req_address_o);
        sim.compare("req_sync_flush", dut->orig_req_sync_flush_o, dut->new_req_sync_flush_o);
    }
    sim.compare("l2_busy",    dut->orig_l2_busy_o,    dut->new_l2_busy_o);
    sim.compare("l3_busy",    dut->orig_l3_busy_o,    dut->new_l3_busy_o);
    sim.compare("aborted",    dut->orig_aborted_o,    dut->new_aborted_o);
    sim.compare("is_active",  dut->orig_is_active_o,  dut->new_is_active_o);

    // ESR status read-back
    sim.compare("esr_ctl",   dut->orig_esr_ctl_o,   dut->new_esr_ctl_o);
    sim.compare("esr_pidx",  dut->orig_esr_pidx_o,  dut->new_esr_pidx_o);
    sim.compare("esr_data0", dut->orig_esr_data0_o, dut->new_esr_data0_o);
    sim.compare("esr_data1", dut->orig_esr_data1_o, dut->new_esr_data1_o);
    sim.compare("esr_ecc",   dut->orig_esr_ecc_o,   dut->new_esr_ecc_o);

    // Debug write control signals
    sim.compare("dbg_wr_ts_en",   dut->orig_dbg_wr_tag_state_en_o, dut->new_dbg_wr_tag_state_en_o);
    sim.compare("dbg_wr_tg_en",   dut->orig_dbg_wr_tags_en_o,      dut->new_dbg_wr_tags_en_o);
    sim.compare("dbg_wr_dt_en",   dut->orig_dbg_wr_data_en_o,      dut->new_dbg_wr_data_en_o);
    sim.compare("dbg_wr_ecc_en",  dut->orig_dbg_wr_ecc_wr_en_o,    dut->new_dbg_wr_ecc_wr_en_o);
    sim.compare("dbg_wr_qwen",    dut->orig_dbg_wr_data_qwen_o,    dut->new_dbg_wr_data_qwen_o);
}

// Run until both modules leave active, or limit reached
static int run_until_idle(CosimCtrl<DUT>& sim, int limit) {
    auto* dut = sim.dut.get();
    int cycles = 0;
    while (limit > 0) {
        clear_inputs(dut);
        sim.tick();
        compare_all(sim);
        if (!dut->orig_is_active_o && !dut->new_is_active_o) break;
        limit--;
        cycles++;
    }
    return cycles;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 6000000;
    auto* dut = sim.dut.get();

    dut->pipe_idx_cop_req_ready_i = 1;
    dut->my_bank_id_i = 1;
    dut->my_scp_virtual_shire_id_i = 0x42;
    dut->esr_sc_l2_set_base_i = 0;
    dut->esr_sc_l2_set_size_i = 0;
    dut->esr_sc_l3_set_base_i = 0;
    dut->esr_sc_l3_set_size_i = 0;
    dut->esr_sc_scp_set_base_i = 0;
    dut->esr_sc_scp_set_size_i = 0;
    dut->cbuf_mem_valid_i = 0;
    clear_inputs(dut);

    sim.reset();

    // ── Phase 1: Power-up ALL_INV ───────────────────────────
    printf("cosim phase 1: power-up ALL_INV (cold reset)\n");
    for (int i = 0; i < 260; i++) {
        clear_inputs(dut);
        sim.tick();
        compare_all(sim);
    }
    int c = run_until_idle(sim, 20000);
    printf("  ALL_INV completed in %d cycles\n", c);
    for (int i = 0; i < 5; i++) { clear_inputs(dut); sim.tick(); compare_all(sim); }

    // ── Phase 2: L2_INV with 8 sets ────────────────────────
    printf("cosim phase 2: L2_INV (8 sets)\n");
    dut->esr_sc_l2_set_size_i = 8;
    trigger_go(dut, OP_L2_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  L2_INV done\n");

    // ── Phase 3: L3_FLUSH with 4 sets, non-zero base ───────
    printf("cosim phase 3: L3_FLUSH (4 sets, base=10)\n");
    dut->esr_sc_l3_set_base_i = 10;
    dut->esr_sc_l3_set_size_i = 4;
    trigger_go(dut, OP_L3_FLUSH);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    dut->esr_sc_l3_set_base_i = 0;
    printf("  L3_FLUSH done\n");

    // ── Phase 4: L3_EVICT ──────────────────────────────────
    printf("cosim phase 4: L3_EVICT (4 sets)\n");
    dut->esr_sc_l3_set_size_i = 4;
    trigger_go(dut, OP_L3_EVICT);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  L3_EVICT done\n");

    // ── Phase 5: DBG_READ with specific set/way/sbb/qw ─────
    printf("cosim phase 5: DBG_READ (set=5, way=2, sbb=1, qw=3)\n");
    // First write the physical_index ESR so the module knows the target
    write_physical_index(dut, 5, 2, 1, 3);
    sim.tick(); compare_all(sim);
    // Now trigger DBG_READ
    trigger_go(dut, OP_DBG_READ);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 50);
    printf("  DBG_READ done\n");

    // ── Phase 6: DBG_WRITE targeting tag_state ──────────────
    printf("cosim phase 6: DBG_WRITE tag_state (set=3, way=1, sbb=0)\n");
    // Write physical index
    write_physical_index(dut, 3, 1, 0, 0);
    sim.tick(); compare_all(sim);
    // Write data0 (tag_state data)
    dut->wr_en_idx_cop_sm_data0_i = 1;
    dut->wr_data_i = 0xDEADBEEF12345678ULL;
    sim.tick(); compare_all(sim);
    clear_inputs(dut);
    // Write ecc
    dut->wr_en_idx_cop_sm_ecc_i = 1;
    dut->wr_data_i = 0x0000000000000055ULL;
    sim.tick(); compare_all(sim);
    clear_inputs(dut);
    // Trigger DBG_WRITE targeting tag_state with ecc_wr_en
    trigger_go_with_opts(dut, OP_DBG_WRITE, LRAM_TAG_STATE, true);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 50);
    printf("  DBG_WRITE tag_state done\n");

    // ── Phase 7: DBG_WRITE targeting tags ───────────────────
    printf("cosim phase 7: DBG_WRITE tags\n");
    write_physical_index(dut, 7, 0, 2, 0);
    sim.tick(); compare_all(sim);
    dut->wr_en_idx_cop_sm_data0_i = 1;
    dut->wr_data_i = 0x00AABBCC00112233ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    dut->wr_en_idx_cop_sm_data1_i = 1;
    dut->wr_data_i = 0x0044556600778899ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    trigger_go_with_opts(dut, OP_DBG_WRITE, LRAM_TAG, false);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 50);
    printf("  DBG_WRITE tags done\n");

    // ── Phase 8: DBG_WRITE targeting data ───────────────────
    printf("cosim phase 8: DBG_WRITE data (qw=2)\n");
    write_physical_index(dut, 1, 3, 3, 2);
    sim.tick(); compare_all(sim);
    dut->wr_en_idx_cop_sm_data0_i = 1;
    dut->wr_data_i = 0xCAFEBABE01020304ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    dut->wr_en_idx_cop_sm_data1_i = 1;
    dut->wr_data_i = 0x0A0B0C0D0E0F1011ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    trigger_go_with_opts(dut, OP_DBG_WRITE, LRAM_DATA, true);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 50);
    printf("  DBG_WRITE data done\n");

    // ── Phase 9: Abort during L2_EVICT ─────────────────────
    printf("cosim phase 9: abort during L2_EVICT\n");
    dut->esr_sc_l2_set_size_i = 100;
    trigger_go(dut, OP_L2_EVICT);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    trigger_abort(dut);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    printf("  abort done\n");

    // ── Phase 10: Squash (L2 size=0 with L2 op) ────────────
    printf("cosim phase 10: squash (L2 size=0)\n");
    dut->esr_sc_l2_set_size_i = 0;
    trigger_go(dut, OP_L2_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    printf("  squash done\n");

    // ── Phase 11: Squash L3 (size=0) ────────────────────────
    printf("cosim phase 11: squash L3 (size=0)\n");
    dut->esr_sc_l3_set_size_i = 0;
    trigger_go(dut, OP_L3_EVICT);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    printf("  squash L3 done\n");

    // ── Phase 12: Squash SCP (size=0) ───────────────────────
    printf("cosim phase 12: squash SCP (size=0)\n");
    dut->esr_sc_scp_set_size_i = 0;
    trigger_go(dut, OP_SCP_ZERO);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    printf("  squash SCP done\n");

    // ── Phase 13: ALL_INV from IDLE ─────────────────────────
    printf("cosim phase 13: ALL_INV from IDLE\n");
    trigger_go(dut, OP_ALL_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    c = run_until_idle(sim, 20000);
    printf("  ALL_INV from IDLE done (%d cycles)\n", c);

    // ── Phase 14: L2_EVICT with back-pressure ───────────────
    printf("cosim phase 14: L2_EVICT with back-pressure\n");
    dut->esr_sc_l2_set_size_i = 4;
    trigger_go(dut, OP_L2_EVICT);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    int bp_cycle = 0;
    int limit = 5000;
    while (limit > 0) {
        dut->pipe_idx_cop_req_ready_i = (bp_cycle % 2 == 0) ? 1 : 0;
        sim.tick(); compare_all(sim); clear_inputs(dut);
        if (!dut->orig_is_active_o && !dut->new_is_active_o) break;
        limit--; bp_cycle++;
    }
    dut->pipe_idx_cop_req_ready_i = 1;
    printf("  back-pressure done\n");

    // ── Phase 15: L2_EVICT with heavy back-pressure ─────────
    printf("cosim phase 15: L2_EVICT with heavy back-pressure (1 in 5)\n");
    dut->esr_sc_l2_set_size_i = 4;
    trigger_go(dut, OP_L2_EVICT);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    bp_cycle = 0;
    limit = 10000;
    while (limit > 0) {
        dut->pipe_idx_cop_req_ready_i = (bp_cycle % 5 == 0) ? 1 : 0;
        sim.tick(); compare_all(sim); clear_inputs(dut);
        if (!dut->orig_is_active_o && !dut->new_is_active_o) break;
        limit--; bp_cycle++;
    }
    dut->pipe_idx_cop_req_ready_i = 1;
    printf("  heavy back-pressure done\n");

    // ── Phase 16: SCP_ZERO ─────────────────────────────────
    printf("cosim phase 16: SCP_ZERO (4 sets)\n");
    dut->esr_sc_scp_set_size_i = 4;
    trigger_go(dut, OP_SCP_ZERO);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  SCP_ZERO done\n");

    // ── Phase 17: SCP_ZERO with non-zero base, different shire_id ──
    printf("cosim phase 17: SCP_ZERO (base=8, size=2, shire_id=0xAB)\n");
    dut->my_scp_virtual_shire_id_i = 0xAB;
    dut->esr_sc_scp_set_base_i = 8;
    dut->esr_sc_scp_set_size_i = 2;
    trigger_go(dut, OP_SCP_ZERO);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    dut->my_scp_virtual_shire_id_i = 0x42;
    dut->esr_sc_scp_set_base_i = 0;
    printf("  SCP_ZERO non-zero done\n");

    // ── Phase 18: CB_INV with populated cbuf state ──────────
    printf("cosim phase 18: CB_INV (8 valid entries)\n");
    dut->cbuf_mem_valid_i = 0x000000FF;  // entries 0-7 valid
    trigger_go(dut, OP_CB_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  CB_INV (8 valid) done\n");

    // ── Phase 19: CB_INV with scattered valid entries ───────
    printf("cosim phase 19: CB_INV (scattered valid: 0xA5A5A5A5)\n");
    dut->cbuf_mem_valid_i = 0xA5A5A5A5;  // alternating pattern
    trigger_go(dut, OP_CB_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  CB_INV (scattered) done\n");

    // ── Phase 20: CB_INV with all valid ─────────────────────
    printf("cosim phase 20: CB_INV (all 32 valid)\n");
    dut->cbuf_mem_valid_i = 0xFFFFFFFF;
    trigger_go(dut, OP_CB_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  CB_INV (all valid) done\n");

    // ── Phase 21: CB_INV with none valid ────────────────────
    printf("cosim phase 21: CB_INV (none valid)\n");
    dut->cbuf_mem_valid_i = 0;
    trigger_go(dut, OP_CB_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    printf("  CB_INV (none valid) done\n");

    // ── Phase 22: CB_INV with back-pressure ─────────────────
    printf("cosim phase 22: CB_INV (16 valid, back-pressure)\n");
    dut->cbuf_mem_valid_i = 0x0000FFFF;
    trigger_go(dut, OP_CB_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    bp_cycle = 0; limit = 5000;
    while (limit > 0) {
        dut->pipe_idx_cop_req_ready_i = (bp_cycle % 3 == 0) ? 1 : 0;
        sim.tick(); compare_all(sim); clear_inputs(dut);
        if (!dut->orig_is_active_o && !dut->new_is_active_o) break;
        limit--; bp_cycle++;
    }
    dut->pipe_idx_cop_req_ready_i = 1;
    dut->cbuf_mem_valid_i = 0;
    printf("  CB_INV back-pressure done\n");

    // ── Phase 23: MBIST abort during power-up ───────────────
    printf("cosim phase 23: MBIST abort during RESET state\n");
    // Reset both modules
    dut->rst_ni = 0;
    for (int i = 0; i < 5; i++) sim.tick();
    dut->rst_ni = 1;
    sim.tick(); compare_all(sim);
    // Assert mbist_on during reset counter
    dut->mbist_on_i = 1;
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    dut->mbist_on_i = 0;
    // Should skip directly to IDLE
    for (int i = 0; i < 260; i++) { clear_inputs(dut); sim.tick(); compare_all(sim); }
    printf("  MBIST abort done\n");

    // ── Phase 24: L2_FLUSH with non-zero base ───────────────
    printf("cosim phase 24: L2_FLUSH (base=100, size=4)\n");
    dut->esr_sc_l2_set_base_i = 100;
    dut->esr_sc_l2_set_size_i = 4;
    trigger_go(dut, OP_L2_FLUSH);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    dut->esr_sc_l2_set_base_i = 0;
    printf("  L2_FLUSH non-zero base done\n");

    // ── Phase 25: L2_INV with different bank_id ─────────────
    printf("cosim phase 25: L2_INV (bank_id=3, 4 sets)\n");
    dut->my_bank_id_i = 3;
    dut->esr_sc_l2_set_size_i = 4;
    trigger_go(dut, OP_L2_INV);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    run_until_idle(sim, 5000);
    dut->my_bank_id_i = 1;
    printf("  L2_INV bank_id=3 done\n");

    // ── Phase 26: ESR register write/read ───────────────────
    printf("cosim phase 26: ESR register writes and readback\n");
    // Write ctl register
    dut->wr_en_idx_cop_sm_ctl_i = 1;
    dut->wr_data_i = ((uint64_t)OP_L3_FLUSH << OPCODE_LSB) |
                     (1ULL << ECC_WR_EN_LSB) |
                     ((uint64_t)LRAM_TAG << LOGICAL_RAM_LSB);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    sim.tick(); compare_all(sim);

    // Write physical_index
    write_physical_index(dut, 42, 3, 2, 1);
    sim.tick(); compare_all(sim); clear_inputs(dut);
    sim.tick(); compare_all(sim);

    // Write data0
    dut->wr_en_idx_cop_sm_data0_i = 1;
    dut->wr_data_i = 0x123456789ABCDEF0ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    sim.tick(); compare_all(sim);

    // Write data1
    dut->wr_en_idx_cop_sm_data1_i = 1;
    dut->wr_data_i = 0xFEDCBA9876543210ULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    sim.tick(); compare_all(sim);

    // Write ecc
    dut->wr_en_idx_cop_sm_ecc_i = 1;
    dut->wr_data_i = 0x0000000000FFFFFFULL;
    sim.tick(); compare_all(sim); clear_inputs(dut);
    sim.tick(); compare_all(sim);
    printf("  ESR register readback done\n");

    // Final idle cycles
    for (int i = 0; i < 10; i++) { clear_inputs(dut); sim.tick(); compare_all(sim); }

    return sim.finish();
}

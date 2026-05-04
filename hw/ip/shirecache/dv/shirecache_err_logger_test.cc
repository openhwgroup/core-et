// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_err_logger.

#include "Vshirecache_err_logger_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vshirecache_err_logger_tb;

// ── Constants ────────────────────────────────────────────────
static constexpr int NUM_ENTRIES = 64;

// err_rsp_e values
static constexpr int ErrNone            = 0x00;
static constexpr int PipeErrL3ShireDec  = 0x01;
static constexpr int PipeErrScpShireDec = 0x02;
static constexpr int PipeErrL2OpNonEn   = 0x04;
static constexpr int ReqqErrMeshResp    = 0x10;

// err_log_code_e values
static constexpr int ErrLogEccSbe     = 0;
static constexpr int ErrLogEccDbe     = 1;
static constexpr int ErrLogEccSat     = 2;
static constexpr int ErrLogDecode     = 3;
static constexpr int ErrLogPerfmonSat = 4;

// reqq_orig_opcode_e values
static constexpr int OrigRead  = 0x00;
static constexpr int OrigWrite = 0x01;

// reqq_source_e values
static constexpr int ReqqSrcL2 = 0;
static constexpr int ReqqSrcL3 = 1;

// ── Helpers ──────────────────────────────────────────────────

static void clear_inputs(DUT* d) {
    d->alloc_l2_valid_i         = 0;
    d->alloc_l2_reqq_id_i       = 0;
    d->alloc_l2_err_valid_i     = 0;
    d->alloc_l2_err_type_i      = ErrNone;
    d->alloc_l2_err_imprecise_i = 0;

    d->alloc_l3_valid_i         = 0;
    d->alloc_l3_reqq_id_i       = 0;
    d->alloc_l3_err_valid_i     = 0;
    d->alloc_l3_err_type_i      = ErrNone;
    d->alloc_l3_err_imprecise_i = 0;

    d->pipe_tag_rsp_valid_i              = 0;
    d->pipe_tag_rsp_reqq_id_i            = 0;
    d->pipe_tag_rsp_err_rsp_i            = ErrNone;
    d->pipe_tag_rsp_tag_index_i          = 0;
    d->pipe_tag_rsp_orig_address_i       = 0;
    d->pipe_tag_rsp_tag_state_ram_sbe_i  = 0;
    d->pipe_tag_rsp_tag_state_ram_dbe_i  = 0;
    d->pipe_tag_rsp_tag_ram_sbe_i        = 0;
    d->pipe_tag_rsp_tag_ram_dbe_i        = 0;

    d->pipe_data_rsp_valid_i        = 0;
    d->pipe_data_rsp_reqq_id_i      = 0;
    d->pipe_data_rsp_data_index_i   = 0;
    d->pipe_data_rsp_data_ram_sbe_i = 0;
    d->pipe_data_rsp_data_ram_dbe_i = 0;

    d->to_l3_mesh_rsp_valid_i    = 0;
    d->to_l3_mesh_rsp_reqq_id_i  = 0;
    d->to_l3_mesh_rsp_err_i      = 0;
    d->to_sys_mesh_rsp_valid_i   = 0;
    d->to_sys_mesh_rsp_reqq_id_i = 0;
    d->to_sys_mesh_rsp_err_i     = 0;

    d->tag_state_sbe_overflow_i = 0;
    d->tag_state_dbe_overflow_i = 0;
    d->tag_sbe_overflow_i       = 0;
    d->tag_dbe_overflow_i       = 0;
    d->data_sbe_overflow_i      = 0;
    d->data_dbe_overflow_i      = 0;

    d->cyc_cntr_overflow_i = 0;
    d->p0_cntr_overflow_i  = 0;
    d->p1_cntr_overflow_i  = 0;

    d->rs_idx_i           = 0;
    d->rs_valid_i         = 0;
    d->rs_err_imprecise_i = 0;
    d->rs_address_i       = 0;
    d->rs_orig_opcode_i   = OrigRead;
    d->rs_reqq_source_i   = ReqqSrcL2;
    d->rs_tag_id_i        = 0;
    d->rs_source_i        = 0;
    d->rs_port_id_i       = 0;
    d->rs_traced_i        = 0;

    d->bank_req_valid_i      = 0;
    d->bank_req_ready_i      = 0;
    d->bank_rsp_valid_i      = 0;
    d->bank_rsp_ready_i      = 0;
    d->to_l3_mm_req_valid_i  = 0;
    d->to_l3_mm_req_ready_i  = 0;
    d->to_l3_mm_rsp_valid_i  = 0;
    d->to_l3_mm_rsp_ready_i  = 0;
    d->to_sys_mm_req_valid_i = 0;
    d->to_sys_mm_req_ready_i = 0;
    d->to_sys_mm_rsp_valid_i = 0;
    d->to_sys_mm_rsp_ready_i = 0;
    d->l3_ms_req_valid_i     = 0;
    d->l3_ms_req_ready_i     = 0;
    d->l3_ms_rsp_valid_i     = 0;
    d->l3_ms_rsp_ready_i     = 0;

    d->esr_err_rsp_enable_i        = 0;
    d->esr_err_int_enable_i        = 0;
    d->esr_trace_type_hot_enable_i = 0;
    d->esr_wr_en_err_log_info_i    = 0;
    d->esr_wr_en_reqq_debug_ctl_i  = 0;
    d->esr_wr_data_i               = 0;
}

static bool esr_valid(uint64_t info)      { return (info >> 0) & 1; }
static bool esr_multi(uint64_t info)      { return (info >> 1) & 1; }
static bool esr_enabled(uint64_t info)    { return (info >> 2) & 1; }
static bool esr_imprecise(uint64_t info)  { return (info >> 3) & 1; }
static int  esr_code(uint64_t info)       { return (info >> 4) & 7; }

// Pipeline: error input visible for 1 tick (tick 0).
// Keep reqq_state stable. After 2 ticks total, err_log_q updated.
//
// Tick 0 posedge: valid_r1_q captures 1.
// Tick 1 posedge: err_log_q captures. err_logged/err_detected update.
// After tick 1: read outputs.
//
// For clear: wr_en_err_log_info + matching code/valid.
// clear_err is combinational. err_log_d updated same cycle.
// Tick with clear: err_log_q captures clear'd value.
// After tick: outputs visible.

// Helper: inject error for 1 tick, then keep reqq_state for 1 more tick, then check.
// Returns esr_info after pipeline settles.
static uint64_t inject_and_wait(SimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    // Tick 0: error source is present. At posedge, valid_r1_q captures 1.
    sim.tick();
    // Now remove error source but keep reqq_state and esr config.
    // These fields must remain stable for r1 lookup.
    d->alloc_l2_valid_i         = 0;
    d->alloc_l2_err_valid_i     = 0;
    d->alloc_l3_valid_i         = 0;
    d->alloc_l3_err_valid_i     = 0;
    d->pipe_tag_rsp_valid_i     = 0;
    d->pipe_data_rsp_valid_i    = 0;
    d->to_l3_mesh_rsp_valid_i   = 0;
    d->to_l3_mesh_rsp_err_i     = 0;
    d->to_sys_mesh_rsp_valid_i  = 0;
    d->to_sys_mesh_rsp_err_i    = 0;
    // Tick 1: err_log_q captures. err_logged/err_detected update.
    sim.tick();
    return d->esr_info_o;
}

// Helper: clear error via ESR write-back. Returns after outputs settle.
static void clear_error(SimCtrl<DUT>& sim, int code) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->esr_wr_en_err_log_info_i = 1;
    d->esr_wr_data_i = (code << 4) | 1;
    sim.tick(); // clear_err fires combinationally, err_log_q captures clear'd value at posedge
    clear_inputs(d);
    // After this tick, err_log_q.valid=0, err_logged=0, err_detected=0
    sim.tick(); // extra settle
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 600000;
    clear_inputs(d);
    sim.reset();

    // ── Test 1: No errors after reset ───────────────────────
    printf("-- Test 1: No errors after reset\n");
    sim.tick();
    sim.check(d->err_detected_o == 0, "no err_detected after reset");
    sim.check(d->err_logged_o == 0, "no err_logged after reset");
    sim.check(d->set_reqq_err_detected_hot_o == 0, "no reqq err_detected after reset");
    sim.check(d->set_reqq_err_zero_data_hot_o == 0, "no reqq err_zero_data after reset");
    sim.check(esr_valid(d->esr_info_o) == false, "esr info valid=0 after reset");

    // ── Test 2: L2 allocation decode error ──────────────────
    printf("-- Test 2: L2 allocation decode error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 5;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i        = 5;
    d->rs_valid_i      = 1;
    d->rs_address_i    = 0x1000;
    d->rs_port_id_i    = 2;
    d->rs_tag_id_i     = 0xAB;
    d->rs_source_i     = 0x3;
    d->rs_orig_opcode_i = OrigRead;
    d->rs_reqq_source_i = ReqqSrcL2;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "L2 decode: esr info valid=1");
        sim.check(esr_code(info) == ErrLogDecode, "L2 decode: esr code=Decode");
        sim.check(esr_enabled(info), "L2 decode: esr enabled=1");
        sim.check(d->err_logged_o == 1, "L2 decode: err_logged=1");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 3: L3 allocation decode error ──────────────────
    printf("-- Test 3: L3 allocation decode error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l3_valid_i     = 1;
    d->alloc_l3_reqq_id_i   = 10;
    d->alloc_l3_err_valid_i = 1;
    d->alloc_l3_err_type_i  = PipeErrL3ShireDec;
    d->rs_idx_i    = 10;
    d->rs_valid_i  = 1;
    d->rs_address_i = 0x2000;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "L3 decode: esr valid");
        sim.check(esr_code(info) == ErrLogDecode, "L3 decode: code=Decode");
        sim.check(d->err_logged_o == 1, "L3 decode: err_logged");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 4: Tag decode error ────────────────────────────
    printf("-- Test 4: Tag decode error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_reqq_id_i = 3;
    d->pipe_tag_rsp_err_rsp_i = PipeErrScpShireDec;
    d->pipe_tag_rsp_orig_address_i = 0xABCD0;
    d->rs_idx_i   = 3;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Tag decode: esr valid");
        sim.check(esr_code(info) == ErrLogDecode, "Tag decode: code=Decode");
        sim.check(d->err_logged_o == 1, "Tag decode: err_logged");
        sim.check(d->esr_address_o != 0, "Tag decode: address captured");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 5: Tag ECC SBE ─────────────────────────────────
    printf("-- Test 5: Tag ECC SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_tag_rsp_valid_i       = 1;
    d->pipe_tag_rsp_reqq_id_i     = 7;
    d->pipe_tag_rsp_tag_ram_sbe_i = 0x2;
    d->pipe_tag_rsp_tag_index_i   = 0x55;
    d->pipe_tag_rsp_orig_address_i = 0xF0000;
    d->rs_idx_i   = 7;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Tag SBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccSbe, "Tag SBE: code=EccSbe");
        sim.check(d->err_logged_o == 1, "Tag SBE: err_logged");
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Test 6: Tag ECC DBE ─────────────────────────────────
    printf("-- Test 6: Tag ECC DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_tag_rsp_valid_i       = 1;
    d->pipe_tag_rsp_reqq_id_i     = 8;
    d->pipe_tag_rsp_tag_ram_dbe_i = 0x1;
    d->pipe_tag_rsp_tag_index_i   = 0x22;
    d->pipe_tag_rsp_orig_address_i = 0xD0000;
    d->rs_idx_i   = 8;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Tag DBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccDbe, "Tag DBE: code=EccDbe");
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Test 7: Tag State ECC SBE ───────────────────────────
    printf("-- Test 7: Tag State ECC SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_tag_rsp_valid_i              = 1;
    d->pipe_tag_rsp_reqq_id_i            = 2;
    d->pipe_tag_rsp_tag_state_ram_sbe_i  = 1;
    d->pipe_tag_rsp_tag_index_i          = 0x10;
    d->pipe_tag_rsp_orig_address_i       = 0xE0000;
    d->rs_idx_i   = 2;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Tag State SBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccSbe, "Tag State SBE: code=EccSbe");
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Test 8: Tag State ECC DBE ───────────────────────────
    printf("-- Test 8: Tag State ECC DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_tag_rsp_valid_i              = 1;
    d->pipe_tag_rsp_reqq_id_i            = 9;
    d->pipe_tag_rsp_tag_state_ram_dbe_i  = 1;
    d->pipe_tag_rsp_tag_index_i          = 0x40;
    d->pipe_tag_rsp_orig_address_i       = 0xB0000;
    d->rs_idx_i   = 9;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Tag State DBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccDbe, "Tag State DBE: code=EccDbe");
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Test 9: Data ECC SBE ────────────────────────────────
    printf("-- Test 9: Data ECC SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_data_rsp_valid_i        = 1;
    d->pipe_data_rsp_reqq_id_i      = 12;
    d->pipe_data_rsp_data_ram_sbe_i = 0x01;
    d->pipe_data_rsp_data_index_i   = 0x33;
    d->rs_idx_i   = 12;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Data SBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccSbe, "Data SBE: code=EccSbe");
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Test 10: Data ECC DBE ───────────────────────────────
    printf("-- Test 10: Data ECC DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_data_rsp_valid_i        = 1;
    d->pipe_data_rsp_reqq_id_i      = 13;
    d->pipe_data_rsp_data_ram_dbe_i = 0x80;
    d->pipe_data_rsp_data_index_i   = 0x44;
    d->rs_idx_i   = 13;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Data DBE: esr valid");
        sim.check(esr_code(info) == ErrLogEccDbe, "Data DBE: code=EccDbe");
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Test 11: L3 mesh response error ─────────────────────
    printf("-- Test 11: L3 mesh response error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->to_l3_mesh_rsp_valid_i   = 1;
    d->to_l3_mesh_rsp_reqq_id_i = 20;
    d->to_l3_mesh_rsp_err_i     = 1;
    d->rs_idx_i     = 20;
    d->rs_valid_i   = 1;
    d->rs_address_i = 0x5000;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "L3 mesh: esr valid");
        sim.check(esr_code(info) == ErrLogDecode, "L3 mesh: code=Decode");
        sim.check(d->err_logged_o == 1, "L3 mesh: err_logged");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 12: Sys mesh response error ────────────────────
    printf("-- Test 12: Sys mesh response error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->to_sys_mesh_rsp_valid_i   = 1;
    d->to_sys_mesh_rsp_reqq_id_i = 30;
    d->to_sys_mesh_rsp_err_i     = 1;
    d->rs_idx_i     = 30;
    d->rs_valid_i   = 1;
    d->rs_address_i = 0x6000;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Sys mesh: esr valid");
        sim.check(esr_code(info) == ErrLogDecode, "Sys mesh: code=Decode");
        sim.check(d->err_logged_o == 1, "Sys mesh: err_logged");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 13: ECC overflow rising-edge detection ─────────
    printf("-- Test 13: ECC overflow rising-edge\n");
    // Rising edge pipeline:
    //   Tick A: input=1. At posedge A: r1_q samples input (gets 1 if gating allows).
    //     Gating: if (|{input, r1_q}) r1_q <= input. Initially r1_q=0, input=1 => |{1,0}=1 => r1_q<=1.
    //   Tick B: At posedge B: r2_q samples r1_q. r1_q=1 from tick A. |{r1,r2}=|{1,0}=1 => r2_q<=1.
    //     Before posedge B: r1_q=1, r2_q=0 => RE fires. This means the err_info is valid at tick B (combinational).
    //     At posedge B: valid_r1_q captures from r0 (which is the RE-driven error valid).
    //   Tick C: valid_r1_q=1. err_log_q captures at posedge C.
    //   After tick C: outputs readable.
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    d->tag_state_sbe_overflow_i = 1;
    sim.tick(); // Tick A: r1_q <= 1
    sim.tick(); // Tick B: RE fires (r1=1,r2=0). valid_r1_q captures at posedge.
    d->tag_state_sbe_overflow_i = 0;
    sim.tick(); // Tick C: err_log_q captures at posedge.
    {
        uint64_t info = d->esr_info_o;
        sim.check(esr_valid(info), "ECC overflow: esr valid");
        sim.check(esr_code(info) == ErrLogEccSat, "ECC overflow: code=EccSat");
    }
    clear_error(sim, ErrLogEccSat);

    // ── Test 14: Perfmon overflow rising-edge ───────────────
    printf("-- Test 14: Perfmon overflow rising-edge\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    d->p0_cntr_overflow_i = 1;
    sim.tick(); // r1_q <= 1
    sim.tick(); // RE fires, valid_r1_q captured
    d->p0_cntr_overflow_i = 0;
    sim.tick(); // err_log_q captured
    {
        uint64_t info = d->esr_info_o;
        sim.check(esr_valid(info), "Perfmon overflow: esr valid");
        sim.check(esr_code(info) == ErrLogPerfmonSat, "Perfmon overflow: code=PerfmonSat");
    }
    clear_error(sim, ErrLogPerfmonSat);

    // ── Test 15: ESR write-back clear ───────────────────────
    printf("-- Test 15: ESR write-back clear\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 1;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i   = 1;
    d->rs_valid_i = 1;
    inject_and_wait(sim);
    sim.check(d->err_logged_o == 1, "ESR clear: error initially logged");

    clear_inputs(d);
    d->esr_wr_en_err_log_info_i = 1;
    d->esr_wr_data_i = (ErrLogDecode << 4) | 1;
    sim.tick();
    sim.check(!esr_valid(d->esr_info_o), "ESR clear: valid=0 after clear");
    sim.check(d->err_logged_o == 0, "ESR clear: err_logged=0");
    sim.check(d->err_detected_o == 0, "ESR clear: err_detected=0");
    clear_inputs(d);
    sim.tick(); // settle

    // ── Test 16: REQQ debug query (entry) ───────────────────
    printf("-- Test 16: REQQ debug query (entry)\n");
    clear_inputs(d);
    d->rs_idx_i         = 42;
    d->rs_valid_i       = 1;
    d->rs_address_i     = 0xDEAD;
    d->rs_orig_opcode_i = OrigWrite;
    d->rs_port_id_i     = 3;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 42;
    sim.tick();
    d->esr_wr_en_reqq_debug_ctl_i = 0;
    sim.tick();
    {
        uint64_t dbg0 = d->esr_reqq_debug0_o;
        uint64_t dbg1 = d->esr_reqq_debug1_o;
        sim.check((dbg0 | dbg1) != 0, "REQQ debug entry: non-zero output");
    }

    // ── Test 17: REQQ debug valid mask ──────────────────────
    printf("-- Test 17: REQQ debug valid mask\n");
    clear_inputs(d);
    d->rs_idx_i   = 0;
    d->rs_valid_i = 1;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 0x100;
    sim.tick();
    d->esr_wr_en_reqq_debug_ctl_i = 0;
    sim.tick();
    sim.check((d->esr_reqq_debug0_o & 1) == 1, "REQQ debug valid: entry 0 valid");

    // ── Test 18: IO valid/ready debug query ──────────────────
    printf("-- Test 18: IO valid/ready debug\n");
    clear_inputs(d);
    d->bank_req_valid_i     = 1;
    d->bank_req_ready_i     = 1;
    d->to_l3_mm_req_valid_i = 1;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 0x101;
    sim.tick();
    d->esr_wr_en_reqq_debug_ctl_i = 0;
    sim.tick();
    sim.check((d->esr_reqq_debug0_o | d->esr_reqq_debug1_o | d->esr_reqq_debug2_o) != 0,
              "IO debug: non-zero output");

    // ── Test 19: Error priority (enabled > masked) ──────────
    printf("-- Test 19: Error priority\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i         = 1;
    d->alloc_l2_reqq_id_i       = 1;
    d->alloc_l2_err_valid_i     = 1;
    d->alloc_l2_err_type_i      = PipeErrL2OpNonEn;
    d->pipe_data_rsp_valid_i        = 1;
    d->pipe_data_rsp_reqq_id_i      = 2;
    d->pipe_data_rsp_data_ram_sbe_i = 0x01;
    d->rs_idx_i   = 1;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Priority: esr valid");
        sim.check(esr_code(info) == ErrLogDecode, "Priority: decode wins over masked SBE");
        sim.check(esr_enabled(info), "Priority: enabled=1");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 20: Multi bit ──────────────────────────────────
    printf("-- Test 20: Multi bit\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 1;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i   = 1;
    d->rs_valid_i = 1;
    inject_and_wait(sim);
    sim.check(d->err_logged_o == 1, "Multi: first error logged");
    sim.check(!esr_multi(d->esr_info_o), "Multi: multi=0 after first");

    // Second enabled error
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l3_valid_i     = 1;
    d->alloc_l3_reqq_id_i   = 15;
    d->alloc_l3_err_valid_i = 1;
    d->alloc_l3_err_type_i  = PipeErrL3ShireDec;
    d->rs_idx_i   = 15;
    d->rs_valid_i = 1;
    inject_and_wait(sim);
    sim.check(esr_multi(d->esr_info_o), "Multi: multi=1 after second");

    clear_error(sim, ErrLogDecode);

    // ── Test 21: Masked error loads into empty log ──────────
    printf("-- Test 21: Masked error loads into empty log\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    // No int enable => masked
    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_reqq_id_i = 4;
    d->pipe_tag_rsp_err_rsp_i = PipeErrScpShireDec;
    d->rs_idx_i   = 4;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_valid(info), "Masked: valid=1 (logged into empty)");
        sim.check(!esr_enabled(info), "Masked: enabled=0");
        sim.check(d->err_logged_o == 0, "Masked: err_logged=0");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 22: err_detected with precise vs imprecise ─────
    printf("-- Test 22: Precise vs imprecise\n");
    // Precise (rsp_enable=1, err_imprecise=0)
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i         = 1;
    d->alloc_l2_reqq_id_i       = 6;
    d->alloc_l2_err_valid_i     = 1;
    d->alloc_l2_err_type_i      = PipeErrL2OpNonEn;
    d->alloc_l2_err_imprecise_i = 0;
    d->rs_idx_i   = 6;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(!esr_imprecise(info), "Precise: imprecise=0");
        sim.check(d->err_detected_o == 0, "Precise: err_detected=0");
        sim.check(d->err_logged_o == 1, "Precise: err_logged=1");
    }
    clear_error(sim, ErrLogDecode);

    // Imprecise (rsp_enable=0)
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 0;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i         = 1;
    d->alloc_l2_reqq_id_i       = 6;
    d->alloc_l2_err_valid_i     = 1;
    d->alloc_l2_err_type_i      = PipeErrL2OpNonEn;
    d->rs_idx_i   = 6;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_imprecise(info), "Imprecise: imprecise=1");
        sim.check(d->err_detected_o == 1, "Imprecise: err_detected=1");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 23: Idle stability ─────────────────────────────
    printf("-- Test 23: Idle stability\n");
    clear_inputs(d);
    for (int i = 0; i < 10; i++) sim.tick();
    sim.check(d->err_detected_o == 0, "Idle: err_detected=0");
    sim.check(d->err_logged_o == 0, "Idle: err_logged=0");

    // ── Test 24: Enabled overwrites masked ──────────────────
    printf("-- Test 24: Enabled overwrites masked\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_reqq_id_i = 4;
    d->pipe_tag_rsp_err_rsp_i = PipeErrScpShireDec;
    d->rs_idx_i   = 4;
    d->rs_valid_i = 1;
    inject_and_wait(sim);
    sim.check(!esr_enabled(d->esr_info_o), "Overwrite: masked logged");

    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 1;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i   = 1;
    d->rs_valid_i = 1;
    {
        uint64_t info = inject_and_wait(sim);
        sim.check(esr_enabled(info), "Overwrite: enabled wins");
    }
    clear_error(sim, ErrLogDecode);

    // ── Test 25: Multiple perfmon overflow bits ─────────────
    printf("-- Test 25: Multiple perfmon overflow\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    d->p0_cntr_overflow_i  = 1;
    d->p1_cntr_overflow_i  = 1;
    d->cyc_cntr_overflow_i = 1;
    sim.tick(); // r1_q captures
    sim.tick(); // RE fires, valid_r1_q captured
    d->p0_cntr_overflow_i  = 0;
    d->p1_cntr_overflow_i  = 0;
    d->cyc_cntr_overflow_i = 0;
    sim.tick(); // err_log_q captured
    {
        uint64_t info = d->esr_info_o;
        sim.check(esr_valid(info), "Multi perfmon: esr valid");
        sim.check(esr_code(info) == ErrLogPerfmonSat, "Multi perfmon: code");
        bool cyc = (info >> 8) & 1;
        bool p0  = (info >> 9) & 1;
        bool p1  = (info >> 10) & 1;
        sim.check(cyc && p0 && p1, "Multi perfmon: all 3 bits set");
    }
    clear_error(sim, ErrLogPerfmonSat);

    // ── Test 26: Reqq err_detected_hot combinational ────────
    printf("-- Test 26: Reqq hot signals combinational\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 5;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i   = 5;
    d->rs_valid_i = 1;
    // Don't tick yet - check combinational output after eval
    d->clk_i = 0;
    d->eval();
    sim.check((d->set_reqq_err_detected_hot_o >> 5) & 1,
              "Hot: err_detected[5] combinational");
    sim.check((d->set_reqq_err_zero_data_hot_o >> 5) & 1,
              "Hot: err_zero_data[5] combinational");
    // Now finish the inject
    inject_and_wait(sim);
    clear_error(sim, ErrLogDecode);

    return sim.finish();
}

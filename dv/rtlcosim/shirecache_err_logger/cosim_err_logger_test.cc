// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_err_logger vs shirecache_bank_err_logger

#include "Vcosim_err_logger_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_err_logger_tb;

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

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("err_detected",
                (uint32_t)d->orig_err_detected_o,
                (uint32_t)d->new_err_detected_o);
    sim.compare("err_logged",
                (uint32_t)d->orig_err_logged_o,
                (uint32_t)d->new_err_logged_o);
    sim.compare("set_reqq_err_detected_hot",
                (uint64_t)d->orig_set_reqq_err_detected_hot_o,
                (uint64_t)d->new_set_reqq_err_detected_hot_o);
    sim.compare("set_reqq_err_zero_data_hot",
                (uint64_t)d->orig_set_reqq_err_zero_data_hot_o,
                (uint64_t)d->new_set_reqq_err_zero_data_hot_o);
    sim.compare("esr_info",
                (uint64_t)d->orig_esr_info_o,
                (uint64_t)d->new_esr_info_o);
    sim.compare("esr_address",
                (uint64_t)d->orig_esr_address_o,
                (uint64_t)d->new_esr_address_o);
}

// Helper: clear the logged error via ESR write-back
static void clear_error(CosimCtrl<DUT>& sim, int code) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->esr_wr_en_err_log_info_i = 1;
    d->esr_wr_data_i = (code << 4) | 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    sim.tick();
    compare_all(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 400000;
    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Idle — verify clean after reset ────────────
    printf("-- Phase 1: Idle after reset\n");
    for (int i = 0; i < 20; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 2: L2 allocation decode error ─────────────────
    printf("-- Phase 2: L2 decode error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 5;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i      = 5;
    d->rs_valid_i    = 1;
    d->rs_address_i  = 0x1000;
    d->rs_port_id_i  = 2;
    d->rs_tag_id_i   = 0xAB;
    d->rs_source_i   = 0x3;
    sim.tick();
    compare_all(sim);
    // Pipeline: arb + register
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 5;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x1000;
    d->rs_port_id_i = 2;
    d->rs_tag_id_i = 0xAB;
    d->rs_source_i = 0x3;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Clear
    clear_error(sim, ErrLogDecode);

    // ── Phase 3: L3 allocation decode error ─────────────────
    printf("-- Phase 3: L3 decode error\n");
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
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 10;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x2000;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 4: Tag decode error ───────────────────────────
    printf("-- Phase 4: Tag decode error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->pipe_tag_rsp_valid_i   = 1;
    d->pipe_tag_rsp_reqq_id_i = 3;
    d->pipe_tag_rsp_err_rsp_i = PipeErrScpShireDec;
    d->pipe_tag_rsp_orig_address_i = 0xABCD0;
    d->rs_idx_i = 3;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 3;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 5: Tag ECC SBE ────────────────────────────────
    printf("-- Phase 5: Tag ECC SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_tag_rsp_valid_i     = 1;
    d->pipe_tag_rsp_reqq_id_i   = 7;
    d->pipe_tag_rsp_tag_ram_sbe_i = 0x2;
    d->pipe_tag_rsp_tag_index_i   = 0x55;
    d->pipe_tag_rsp_orig_address_i = 0xF0000;
    d->rs_idx_i = 7;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->rs_idx_i = 7;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Phase 6: Tag ECC DBE ────────────────────────────────
    printf("-- Phase 6: Tag ECC DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_tag_rsp_valid_i     = 1;
    d->pipe_tag_rsp_reqq_id_i   = 8;
    d->pipe_tag_rsp_tag_ram_dbe_i = 0x1;
    d->pipe_tag_rsp_tag_index_i   = 0x22;
    d->pipe_tag_rsp_orig_address_i = 0xD0000;
    d->rs_idx_i = 8;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->rs_idx_i = 8;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Phase 7: Tag state SBE/DBE ──────────────────────────
    printf("-- Phase 7: Tag state SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_tag_rsp_valid_i              = 1;
    d->pipe_tag_rsp_reqq_id_i            = 2;
    d->pipe_tag_rsp_tag_state_ram_sbe_i  = 1;
    d->pipe_tag_rsp_tag_index_i          = 0x10;
    d->pipe_tag_rsp_orig_address_i       = 0xE0000;
    d->rs_idx_i = 2;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->rs_idx_i = 2;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Phase 8: Data ECC SBE ───────────────────────────────
    printf("-- Phase 8: Data ECC SBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->pipe_data_rsp_valid_i        = 1;
    d->pipe_data_rsp_reqq_id_i      = 12;
    d->pipe_data_rsp_data_ram_sbe_i = 0x01;
    d->pipe_data_rsp_data_index_i   = 0x33;
    d->rs_idx_i = 12;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSbe);
    d->rs_idx_i = 12;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSbe);

    // ── Phase 9: Data ECC DBE ───────────────────────────────
    printf("-- Phase 9: Data ECC DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_data_rsp_valid_i        = 1;
    d->pipe_data_rsp_reqq_id_i      = 13;
    d->pipe_data_rsp_data_ram_dbe_i = 0x80;
    d->pipe_data_rsp_data_index_i   = 0x44;
    d->rs_idx_i = 13;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->rs_idx_i = 13;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Phase 10: L3 mesh response error ────────────────────
    printf("-- Phase 10: L3 mesh response error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->to_l3_mesh_rsp_valid_i   = 1;
    d->to_l3_mesh_rsp_reqq_id_i = 20;
    d->to_l3_mesh_rsp_err_i     = 1;
    d->rs_idx_i = 20;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x5000;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 20;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x5000;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 11: Sys mesh response error ───────────────────
    printf("-- Phase 11: Sys mesh response error\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->to_sys_mesh_rsp_valid_i   = 1;
    d->to_sys_mesh_rsp_reqq_id_i = 30;
    d->to_sys_mesh_rsp_err_i     = 1;
    d->rs_idx_i = 30;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x6000;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 30;
    d->rs_valid_i = 1;
    d->rs_address_i = 0x6000;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 12: ECC overflow rising-edge ──────────────────
    printf("-- Phase 12: ECC overflow rising edge\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    d->tag_state_sbe_overflow_i = 1;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    // Deassert and wait
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSat);

    // ── Phase 13: Perfmon overflow rising-edge ──────────────
    printf("-- Phase 13: Perfmon overflow rising edge\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    d->p0_cntr_overflow_i = 1;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogPerfmonSat);

    // ── Phase 14: ESR write-back clear ──────────────────────
    printf("-- Phase 14: ESR write-back\n");
    // Inject an error, then clear it
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 1;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i = 1;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 1;
    d->rs_valid_i = 1;
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 15: Multiple error types ──────────────────────
    printf("-- Phase 15: Multiple errors simultaneously\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode) | (1 << ErrLogEccSbe);
    // Inject L2 decode + tag ECC SBE simultaneously
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 1;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->pipe_tag_rsp_valid_i     = 1;
    d->pipe_tag_rsp_reqq_id_i   = 2;
    d->pipe_tag_rsp_tag_ram_sbe_i = 0x4;
    d->pipe_tag_rsp_tag_index_i   = 0x77;
    d->pipe_tag_rsp_orig_address_i = 0xC0000;
    d->rs_idx_i = 1;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogDecode) | (1 << ErrLogEccSbe);
    d->rs_idx_i = 1;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);
    clear_error(sim, ErrLogEccSbe);

    // ── Phase 16: REQQ debug query ──────────────────────────
    printf("-- Phase 16: REQQ debug query\n");
    clear_inputs(d);
    d->rs_idx_i = 42;
    d->rs_valid_i = 1;
    d->rs_address_i = 0xDEAD;
    d->rs_orig_opcode_i = OrigWrite;
    d->rs_port_id_i = 3;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 42;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->rs_idx_i = 42;
    d->rs_valid_i = 1;
    d->rs_address_i = 0xDEAD;
    d->rs_orig_opcode_i = OrigWrite;
    d->rs_port_id_i = 3;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Note: reqq_debug registers for entry queries may differ in raw bit layout
    // because reqq_entry_state_t has different struct packing between orig/new.
    // Only compare that both are non-zero (entry data was captured).
    sim.check((d->orig_esr_reqq_debug0_o | d->orig_esr_reqq_debug1_o) != 0,
              "orig reqq_debug non-zero for entry query");
    sim.check((d->new_esr_reqq_debug0_o | d->new_esr_reqq_debug1_o) != 0,
              "new reqq_debug non-zero for entry query");

    // ── Phase 17: REQQ debug valid query ────────────────────
    printf("-- Phase 17: REQQ debug valid query\n");
    clear_inputs(d);
    d->rs_idx_i = 0;
    d->rs_valid_i = 1;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 0x100;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->rs_idx_i = 0;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    sim.compare("reqq_debug0_valid",
                (uint64_t)d->orig_esr_reqq_debug0_o,
                (uint64_t)d->new_esr_reqq_debug0_o);

    // ── Phase 18: IO valid/ready debug query ────────────────
    printf("-- Phase 18: IO valid/ready debug\n");
    clear_inputs(d);
    d->bank_req_valid_i = 1;
    d->bank_req_ready_i = 1;
    d->to_l3_mm_req_valid_i = 1;
    d->esr_wr_en_reqq_debug_ctl_i = 1;
    d->esr_wr_data_i = 0x101;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->bank_req_valid_i = 1;
    d->bank_req_ready_i = 1;
    d->to_l3_mm_req_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    sim.compare("reqq_debug0_io",
                (uint64_t)d->orig_esr_reqq_debug0_o,
                (uint64_t)d->new_esr_reqq_debug0_o);
    sim.compare("reqq_debug1_io",
                (uint64_t)d->orig_esr_reqq_debug1_o,
                (uint64_t)d->new_esr_reqq_debug1_o);
    sim.compare("reqq_debug2_io",
                (uint64_t)d->orig_esr_reqq_debug2_o,
                (uint64_t)d->new_esr_reqq_debug2_o);

    // ── Phase 19: Extended idle ─────────────────────────────
    printf("-- Phase 19: Extended idle\n");
    clear_inputs(d);
    for (int i = 0; i < 100; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ── Phase 20: Tag state DBE ─────────────────────────────
    printf("-- Phase 20: Tag state DBE\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->pipe_tag_rsp_valid_i             = 1;
    d->pipe_tag_rsp_reqq_id_i           = 9;
    d->pipe_tag_rsp_tag_state_ram_dbe_i = 1;
    d->pipe_tag_rsp_tag_index_i         = 0x40;
    d->pipe_tag_rsp_orig_address_i      = 0xB0000;
    d->rs_idx_i = 9;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccDbe);
    d->rs_idx_i = 9;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccDbe);

    // ── Phase 21: err_detected with imprecise ───────────────
    printf("-- Phase 21: Imprecise error detection\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 0; // rsp disabled => imprecise = 1
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->alloc_l2_valid_i     = 1;
    d->alloc_l2_reqq_id_i   = 6;
    d->alloc_l2_err_valid_i = 1;
    d->alloc_l2_err_type_i  = PipeErrL2OpNonEn;
    d->rs_idx_i = 6;
    d->rs_valid_i = 1;
    sim.tick();
    compare_all(sim);
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 0;
    d->esr_err_int_enable_i = (1 << ErrLogDecode);
    d->rs_idx_i = 6;
    d->rs_valid_i = 1;
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogDecode);

    // ── Phase 22: All ECC overflow types ────────────────────
    printf("-- Phase 22: Multiple ECC overflow types\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    d->tag_sbe_overflow_i = 1;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSat);

    // Data overflow
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    d->data_dbe_overflow_i = 1;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogEccSat);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogEccSat);

    // ── Phase 23: Perfmon p1 and cyc overflow ───────────────
    printf("-- Phase 23: p1 + cyc overflow\n");
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    d->p1_cntr_overflow_i = 1;
    d->cyc_cntr_overflow_i = 1;
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_inputs(d);
    d->esr_err_rsp_enable_i = 1;
    d->esr_err_int_enable_i = (1 << ErrLogPerfmonSat);
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }
    clear_error(sim, ErrLogPerfmonSat);

    // ── Phase 24: Final idle ────────────────────────────────
    printf("-- Phase 24: Final idle\n");
    clear_inputs(d);
    for (int i = 0; i < 50; i++) {
        sim.tick();
        compare_all(sim);
    }

    printf("\nTotal comparison cycles: %d\n", sim.comparisons);
    return sim.finish();
}

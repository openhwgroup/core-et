// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_trace vs shirecache_bank_trace
//
// Drives identical stimulus to both, compares trace_valid and
// trace_data (3 segments) cycle-by-cycle.

#include "Vcosim_trace_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vcosim_trace_tb;

// Trace enable bit positions
static constexpr int EN_ALLOC_L2   = 0;
static constexpr int EN_ALLOC_L3   = 1;
static constexpr int EN_TC         = 2;
static constexpr int EN_TC_REQQ_ID = 3;
static constexpr int EN_TC_VICTIM  = 4;
static constexpr int EN_RBUF       = 5;
static constexpr int EN_MESH       = 6;
static constexpr int EN_REQQ       = 7;
static constexpr int EN_FILL       = 11;

// reqq_opcode_e values
static constexpr uint8_t L2_READ = 0x00;
static constexpr uint8_t L2_FILL = 0x0d;

// reqq_orig_opcode_e values
static constexpr uint8_t ORIG_L2_READ      = 0x00;
static constexpr uint8_t ORIG_COP_FLUSH    = 0x13;
static constexpr uint8_t ORIG_COP_EVICT    = 0x14;
static constexpr uint8_t ORIG_COP_PREFETCH = 0x17;

// cache_level_e values
static constexpr uint8_t CACHE_L2  = 0;
static constexpr uint8_t CACHE_L3  = 2;
static constexpr uint8_t CACHE_MEM = 3;

static void clear_inputs(DUT* d) {
    d->alloc_l2_valid_i = 0;
    d->alloc_l2_reqq_id_i = 0;
    d->alloc_l2_port_id_i = 0;
    d->alloc_l2_orig_opcode_i = 0;
    d->alloc_l2_address_i = 0;
    d->alloc_l2_qwen_i = 0;
    d->alloc_l2_traced_i = 0;
    d->alloc_l2_cache_level_start_i = 0;
    d->alloc_l2_cache_level_dest_i = 0;

    d->alloc_l3_valid_i = 0;
    d->alloc_l3_reqq_id_i = 0;
    d->alloc_l3_port_id_i = 0;
    d->alloc_l3_orig_opcode_i = 0;
    d->alloc_l3_address_i = 0;
    d->alloc_l3_qwen_i = 0;
    d->alloc_l3_traced_i = 0;

    d->pipe_tag_rsp_valid_i = 0;
    d->pipe_tag_rsp_reqq_id_i = 0;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_way_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_orig_address_i = 0;
    d->pipe_tag_rsp_victim_address_i = 0;
    d->pipe_tag_rsp_victim_qwen_i = 0;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0;
    d->pipe_tag_rsp_opcode_i = 0;

    d->pipe_rbuf_rd_req_ready_i = 0;
    d->pipe_rbuf_rd_req_valid_i = 0;
    d->pipe_rbuf_rd_req_reqq_id_i = 0;

    d->to_l3_mm_req_valid_i = 0;
    d->to_l3_mm_req_id_i = 0;
    d->to_l3_mm_req_ready_i = 0;
    d->to_l3_mm_rsp_valid_i = 0;
    d->to_l3_mm_rsp_id_i = 0;
    d->to_l3_mm_rsp_ready_i = 0;
    d->to_sys_mm_req_valid_i = 0;
    d->to_sys_mm_req_id_i = 0;
    d->to_sys_mm_req_ready_i = 0;
    d->to_sys_mm_rsp_valid_i = 0;
    d->to_sys_mm_rsp_id_i = 0;
    d->to_sys_mm_rsp_ready_i = 0;

    d->rspmux_l3_sent_valid_i = 0;
    d->rspmux_l3_sent_id_i = 0;

    d->rspmux_l2_sent_valid_i = 0;
    d->rspmux_l2_sent_reqq_id_i = 0;
    d->rspmux_l2_sent_traced_i = 0;

    d->trs_valid_i = 0;
    d->trs_reqq_id_i = 0;
    d->trs_opcode_i = 0;
    d->trs_rspmux_l2_eligible_i = 0;
    d->trs_rspmux_l3_eligible_i = 0;
    d->trs_rspmux_sent_i = 0;
    d->trs_pipe_req_eligible_i = 0;
    d->trs_to_l3_mesh_req_eligible_i = 0;
    d->trs_to_sys_mesh_req_eligible_i = 0;
    d->trs_rbuf_req_eligible_i = 0;
    d->trs_inflight_i = 0;
    d->trs_data_ready_i = 0;
    d->trs_wait_for_dataq_i = 0;
    d->trs_dep_valid_i = 0;
    d->trs_dep_tail_i = 0;
    d->trs_dep_meshll_tail_i = 0;
    d->trs_dep_non_meshll_head_i = 0;
    d->trs_dont_depend_on_me_i = 0;
    d->trs_rbuf_valid_i = 0;
    d->trs_rbuf_pending_valid_i = 0;
    d->trs_err_detected_i = 0;
    d->trs_traced_i = 0;

    d->rs0_idx_i = 0;
    d->rs0_valid_i = 0;
    d->rs0_traced_i = 0;
    d->rs0_address_i = 0;
    d->rs1_idx_i = 1;
    d->rs1_valid_i = 0;
    d->rs1_traced_i = 0;
    d->rs1_address_i = 0;

    d->esr_trace_type_hot_enable_i = 0;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("trace_valid", d->orig_trace_valid_o, d->new_trace_valid_o);
    sim.compare("trace_seg0",  d->orig_trace_seg0_o,  d->new_trace_seg0_o);
    sim.compare("trace_seg1",  d->orig_trace_seg1_o,  d->new_trace_seg1_o);
    sim.compare("trace_seg2",  d->orig_trace_seg2_o,  d->new_trace_seg2_o);
}

static void tick_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_all(sim);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 400000;
    clear_inputs(d);
    sim.reset();

    // ── Phase 1: Idle after reset ───────────────────────────
    printf("-- Phase 1: Idle after reset\n");
    compare_all(sim);
    for (int i = 0; i < 5; i++) tick_compare(sim);

    // ── Phase 2: L2 alloc trace ─────────────────────────────
    printf("-- Phase 2: L2 alloc trace\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 5;
    d->alloc_l2_port_id_i = 2;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0x1000;
    d->alloc_l2_qwen_i = 0xF;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 3: L3 alloc trace ─────────────────────────────
    printf("-- Phase 3: L3 alloc trace\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L3);
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 10;
    d->alloc_l3_port_id_i = 1;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0x2000;
    d->alloc_l3_qwen_i = 0xA;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L3);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 4: TC trace (non-fill) ────────────────────────
    printf("-- Phase 4: TC trace\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC);
    d->rs0_idx_i = 3;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 3;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_way_i = 2;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_orig_address_i = 0x3000;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0x5;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC);
    d->rs0_idx_i = 3;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 5: TC with fill blocked ───────────────────────
    printf("-- Phase 5: TC fill without EN_FILL\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC);
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 7;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_opcode_i = L2_FILL;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC);
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 5b: TC with fill + EN_FILL ────────────────────
    printf("-- Phase 5b: TC fill with EN_FILL\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_FILL);
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 7;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_opcode_i = L2_FILL;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_FILL);
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 6: RBUF trace ────────────────────────────────
    printf("-- Phase 6: RBUF trace\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 4;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x4000;
    d->pipe_rbuf_rd_req_valid_i = 1;
    d->pipe_rbuf_rd_req_ready_i = 1;
    d->pipe_rbuf_rd_req_reqq_id_i = 4;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 4;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x4000;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 4;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x4000;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 7: Mesh trace (to_l3 req) ─────────────────────
    printf("-- Phase 7: Mesh trace\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 2;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->to_l3_mm_req_valid_i = 1;
    d->to_l3_mm_req_ready_i = 1;
    d->to_l3_mm_req_id_i = 2;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 2;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 2;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 8: REQQ state change ──────────────────────────
    printf("-- Phase 8: REQQ state change\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 15;
    d->trs_opcode_i = L2_READ;
    d->trs_inflight_i = 1;
    d->trs_pipe_req_eligible_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 15;
    d->trs_opcode_i = L2_READ;
    d->trs_inflight_i = 1;
    d->trs_pipe_req_eligible_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 9: REQQ no change ─────────────────────────────
    printf("-- Phase 9: REQQ no change (suppressed)\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 15;
    d->trs_opcode_i = L2_READ;
    d->trs_inflight_i = 1;
    d->trs_pipe_req_eligible_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 10: Dual alloc L2 + L3 ────────────────────────
    printf("-- Phase 10: Dual L2+L3 alloc\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 1;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0x5000;
    d->alloc_l2_qwen_i = 0xC;
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 2;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0x6000;
    d->alloc_l3_qwen_i = 0x3;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 11: CopFlush remap ────────────────────────────
    printf("-- Phase 11: CopFlush remap\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 8;
    d->alloc_l2_orig_opcode_i = ORIG_COP_FLUSH;
    d->alloc_l2_cache_level_start_i = CACHE_L3;
    d->alloc_l2_address_i = 0x7000;
    d->alloc_l2_qwen_i = 0x1;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 12: CopEvict remap ────────────────────────────
    printf("-- Phase 12: CopEvict remap\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 12;
    d->alloc_l2_orig_opcode_i = ORIG_COP_EVICT;
    d->alloc_l2_cache_level_start_i = CACHE_MEM;
    d->alloc_l2_address_i = 0x8000;
    d->alloc_l2_qwen_i = 0x5;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 13: CopPrefetch remap ─────────────────────────
    printf("-- Phase 13: CopPrefetch remap\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 13;
    d->alloc_l2_orig_opcode_i = ORIG_COP_PREFETCH;
    d->alloc_l2_cache_level_dest_i = CACHE_L3;
    d->alloc_l2_address_i = 0x9000;
    d->alloc_l2_qwen_i = 0x2;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 14: TC with TcReqqId enable ───────────────────
    printf("-- Phase 14: TC with TcReqqId\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_TC_REQQ_ID);
    d->rs0_idx_i = 11;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 11;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_way_i = 3;
    d->pipe_tag_rsp_orig_address_i = 0xC000;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0xF;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_TC_REQQ_ID);
    d->rs0_idx_i = 11;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 15: TC with TcVictim enable ───────────────────
    printf("-- Phase 15: TC with TcVictim\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_TC_VICTIM);
    d->rs0_idx_i = 9;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 9;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_victim_i = 1;
    d->pipe_tag_rsp_way_i = 1;
    d->pipe_tag_rsp_orig_address_i = 0xA000;
    d->pipe_tag_rsp_victim_address_i = 0xB000;
    d->pipe_tag_rsp_victim_qwen_i = 0x6;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0x9;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_TC_VICTIM);
    d->rs0_idx_i = 9;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 16: Mesh multi-channel ────────────────────────
    printf("-- Phase 16: Mesh multi-channel\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 20;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs1_idx_i = 21;
    d->rs1_valid_i = 1;
    d->rs1_traced_i = 1;
    d->to_l3_mm_req_valid_i = 1;
    d->to_l3_mm_req_ready_i = 1;
    d->to_l3_mm_req_id_i = 20;
    d->to_sys_mm_rsp_valid_i = 1;
    d->to_sys_mm_rsp_ready_i = 1;
    d->to_sys_mm_rsp_id_i = 21;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 20;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs1_idx_i = 21;
    d->rs1_valid_i = 1;
    d->rs1_traced_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 17: L3 slave rsp ──────────────────────────────
    printf("-- Phase 17: L3 slave rsp\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 30;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rspmux_l3_sent_valid_i = 1;
    d->rspmux_l3_sent_id_i = 30;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 30;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 18: L2 neigh rsp ──────────────────────────────
    printf("-- Phase 18: L2 neigh rsp\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rspmux_l2_sent_valid_i = 1;
    d->rspmux_l2_sent_traced_i = 1;
    d->rspmux_l2_sent_reqq_id_i = 25;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 19: Non-traced alloc suppressed ───────────────
    printf("-- Phase 19: Non-traced alloc suppressed\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 0;  // NOT traced
    d->alloc_l2_reqq_id_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 20: All enables off ───────────────────────────
    printf("-- Phase 20: All enables off\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = 0;
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 1;
    tick_compare(sim);
    clear_inputs(d);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 21: REQQ state change then change again ───────
    printf("-- Phase 21: REQQ state changes\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 40;
    d->trs_opcode_i = L2_READ;
    d->trs_data_ready_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    // Change state
    d->trs_rbuf_valid_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    // Another change
    d->trs_inflight_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 22: Triple snippet: L2 + L3 + TC ─────────────
    printf("-- Phase 22: Triple snippet\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3) | (1 << EN_TC);
    d->rs0_idx_i = 50;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 50;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0xD000;
    d->alloc_l2_qwen_i = 0xF;
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 51;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0xE000;
    d->alloc_l3_qwen_i = 0xA;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 50;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    d->pipe_tag_rsp_orig_address_i = 0xD000;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0x5;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3) | (1 << EN_TC);
    d->rs0_idx_i = 50;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 23: Extended mixed stimulus ───────────────────
    printf("-- Phase 23: Extended mixed stimulus\n");
    for (int round = 0; round < 50; round++) {
        clear_inputs(d);
        uint16_t en = (1 << EN_ALLOC_L2) | (1 << EN_MESH) | (1 << EN_TC);
        d->esr_trace_type_hot_enable_i = en;

        // Vary stimuli per round
        if (round % 3 == 0) {
            d->alloc_l2_valid_i = 1;
            d->alloc_l2_traced_i = 1;
            d->alloc_l2_reqq_id_i = round & 0x3F;
            d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
            d->alloc_l2_address_i = (round * 0x100) & 0xFFFFFF;
            d->alloc_l2_qwen_i = round & 0xF;
        }
        if (round % 5 == 0) {
            d->rs0_idx_i = round & 0x3F;
            d->rs0_valid_i = 1;
            d->rs0_traced_i = 1;
            d->to_l3_mm_req_valid_i = 1;
            d->to_l3_mm_req_ready_i = 1;
            d->to_l3_mm_req_id_i = round & 0x3F;
        }
        if (round % 7 == 0) {
            d->rs0_idx_i = (round + 1) & 0x3F;
            d->rs0_valid_i = 1;
            d->rs0_traced_i = 1;
            d->pipe_tag_rsp_valid_i = 1;
            d->pipe_tag_rsp_reqq_id_i = (round + 1) & 0x3F;
            d->pipe_tag_rsp_hit_i = 1;
            d->pipe_tag_rsp_opcode_i = L2_READ;
            d->pipe_tag_rsp_orig_address_i = round * 0x200;
            d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0xF;
        }
        tick_compare(sim);
        clear_inputs(d);
        d->esr_trace_type_hot_enable_i = en;
        if (round % 5 == 0) {
            d->rs0_idx_i = round & 0x3F;
            d->rs0_valid_i = 1;
            d->rs0_traced_i = 1;
        }
        tick_compare(sim);
    }

    // ── Phase 24: RBUF without ready ────────────────────────
    printf("-- Phase 24: RBUF without ready\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 6;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x10000;
    d->pipe_rbuf_rd_req_valid_i = 1;
    d->pipe_rbuf_rd_req_ready_i = 0;
    d->pipe_rbuf_rd_req_reqq_id_i = 6;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 6;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x10000;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Phase 25: Extended idle drain ───────────────────────
    printf("-- Phase 25: Idle drain\n");
    clear_inputs(d);
    for (int i = 0; i < 10; i++) tick_compare(sim);

    // ── Phase 26: to_sys mesh channels ──────────────────────
    printf("-- Phase 26: to_sys mesh channels\n");
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 35;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs1_idx_i = 36;
    d->rs1_valid_i = 1;
    d->rs1_traced_i = 1;
    d->to_sys_mm_req_valid_i = 1;
    d->to_sys_mm_req_ready_i = 1;
    d->to_sys_mm_req_id_i = 35;
    d->to_l3_mm_rsp_valid_i = 1;
    d->to_l3_mm_rsp_ready_i = 1;
    d->to_l3_mm_rsp_id_i = 36;
    tick_compare(sim);
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 35;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs1_idx_i = 36;
    d->rs1_valid_i = 1;
    d->rs1_traced_i = 1;
    tick_compare(sim);
    tick_compare(sim);
    for (int i = 0; i < 3; i++) tick_compare(sim);

    // ── Final idle ──────────────────────────────────────────
    clear_inputs(d);
    for (int i = 0; i < 10; i++) tick_compare(sim);

    return sim.finish();
}

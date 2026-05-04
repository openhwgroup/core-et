// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_trace.
// Tests: alloc L2/L3, tag-check, rbuf, mesh, reqq snippets,
// enable/disable per type, multi-snippet packing, registered output.

#include "Vshirecache_trace_tb.h"
#include "sim_ctrl.h"
#include <cstdio>
#include <cstdint>

using DUT = Vshirecache_trace_tb;

// Trace enable bit positions (mirrors TraceEn* in shirecache_pkg.sv)
static constexpr int EN_ALLOC_L2   = 0;
static constexpr int EN_ALLOC_L3   = 1;
static constexpr int EN_TC         = 2;
static constexpr int EN_TC_REQQ_ID = 3;
static constexpr int EN_TC_VICTIM  = 4;
static constexpr int EN_RBUF       = 5;
static constexpr int EN_MESH       = 6;
static constexpr int EN_REQQ       = 7;
static constexpr int EN_FILL       = 11;

// Trace type enum values (3-bit, mirrors trace_type_e)
static constexpr uint8_t TRACE_NONE     = 0;
static constexpr uint8_t TRACE_ALLOC_L2 = 1;
static constexpr uint8_t TRACE_ALLOC_L3 = 2;
static constexpr uint8_t TRACE_TC       = 3;
static constexpr uint8_t TRACE_RBUF     = 4;
static constexpr uint8_t TRACE_MESH     = 5;
static constexpr uint8_t TRACE_REQQ     = 6;

// reqq_opcode_e values used in tests
static constexpr uint8_t L2_READ  = 0x00;
static constexpr uint8_t L2_FILL  = 0x0d;
static constexpr uint8_t L3_FILL  = 0x1b;

// reqq_orig_opcode_e values
static constexpr uint8_t ORIG_L2_READ        = 0x00;
static constexpr uint8_t ORIG_COP_FLUSH      = 0x13;
static constexpr uint8_t ORIG_COP_EVICT      = 0x14;
static constexpr uint8_t ORIG_COP_PREFETCH   = 0x17;
static constexpr uint8_t ORIG_FLUSH_NEXT     = 0x1d;
static constexpr uint8_t ORIG_EVICT_NEXT     = 0x1e;
static constexpr uint8_t ORIG_PREFETCH_NEXT  = 0x1f;

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

// Extract trace_type from a segment value (lowest 3 bits)
static uint8_t seg_trace_type(uint64_t seg) {
    return seg & 0x7;
}

// Helper: pre-arm enables one tick before stimulus so the clock gate latch
// has seen the enable on the prior negedge.  Then apply stimulus and tick.
static void prearm_enable(SimCtrl<DUT>& sim, uint16_t en) {
    sim.dut.get()->esr_trace_type_hot_enable_i = en;
    sim.tick();  // negedge latches the enable; posedge is idle
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 400000;
    clear_inputs(d);
    sim.reset();

    // ────────────────────────────────────────────────────────
    // Test 1: Reset state — no trace output
    // ────────────────────────────────────────────────────────
    printf("-- Test 1: Reset state\n");
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T1: valid=0 after reset");

    // ────────────────────────────────────────────────────────
    // Test 2: L2 allocation trace
    // Output appears after 1 tick (combinational -> registered).
    // Clock gate needs enable pre-armed on prior negedge.
    // ────────────────────────────────────────────────────────
    printf("-- Test 2: L2 alloc trace\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L2));
    // Now apply stimulus
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 5;
    d->alloc_l2_port_id_i = 2;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0x1000;
    d->alloc_l2_qwen_i = 0xF;
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T2: valid=1 after stimulus");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_ALLOC_L2,
              "T2: seg0 trace_type == ALLOC_L2");

    // ────────────────────────────────────────────────────────
    // Test 3: L3 allocation trace
    // ────────────────────────────────────────────────────────
    printf("-- Test 3: L3 alloc trace\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L3));
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L3);
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 10;
    d->alloc_l3_port_id_i = 1;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0x2000;
    d->alloc_l3_qwen_i = 0xA;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T3: valid=1");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_ALLOC_L3,
              "T3: seg0 trace_type == ALLOC_L3");

    // ────────────────────────────────────────────────────────
    // Test 4: Tag-check (TC) trace — not a fill opcode
    // ────────────────────────────────────────────────────────
    printf("-- Test 4: TC trace (non-fill)\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_TC));
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
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T4: valid=1");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_TC,
              "T4: seg0 trace_type == TC");

    // ────────────────────────────────────────────────────────
    // Test 5: TC trace with fill opcode — requires TraceEnFill
    // ────────────────────────────────────────────────────────
    printf("-- Test 5: TC fill without EN_FILL (should be blocked)\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_TC));
    d->esr_trace_type_hot_enable_i = (1 << EN_TC);  // no EN_FILL
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 7;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_opcode_i = L2_FILL;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T5: fill blocked without EN_FILL");

    printf("-- Test 5b: TC fill with EN_FILL (should pass)\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_TC) | (1 << EN_FILL));
    d->esr_trace_type_hot_enable_i = (1 << EN_TC) | (1 << EN_FILL);
    d->rs0_idx_i = 7;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 7;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_opcode_i = L2_FILL;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T5b: fill passes with EN_FILL");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_TC,
              "T5b: seg0 trace_type == TC");

    // ────────────────────────────────────────────────────────
    // Test 6: RBUF trace (internal FF + output FF = 2 cycle pipeline)
    // Cycle 1: drive rbuf valid+ready -> internal pipe_rbuf_rd_req_valid_q latches
    // Cycle 2: rbuf_valid goes high combinationally -> output FF latches
    // ────────────────────────────────────────────────────────
    printf("-- Test 6: RBUF trace\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_RBUF));
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 4;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x4000;
    d->pipe_rbuf_rd_req_valid_i = 1;
    d->pipe_rbuf_rd_req_ready_i = 1;
    d->pipe_rbuf_rd_req_reqq_id_i = 4;
    sim.tick();
    // After this tick: pipe_rbuf_rd_req_valid_q latched to 1, pipe_rbuf_rd_req_info_q latched.
    // rbuf_valid now depends on reqq_state[reqq_id].traced. Keep state.
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 4;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x4000;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T6: valid=1 for rbuf trace");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_RBUF,
              "T6: seg0 trace_type == RBUF");

    // ────────────────────────────────────────────────────────
    // Test 7: Mesh trace — to_l3 request (also 2-cycle: internal FF + output FF)
    // ────────────────────────────────────────────────────────
    printf("-- Test 7: Mesh trace (to_l3 req)\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_MESH));
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 2;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->to_l3_mm_req_valid_i = 1;
    d->to_l3_mm_req_ready_i = 1;
    d->to_l3_mm_req_id_i = 2;
    sim.tick();
    // After this: to_l3_req_valid_q = 1 (internal FF). mesh_rsp_valid is now high
    // combinationally from to_l3_req_valid_q. The output FF will capture next cycle.
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    d->rs0_idx_i = 2;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T7: valid=1 for mesh trace");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_MESH,
              "T7: seg0 trace_type == MESH");

    // ────────────────────────────────────────────────────────
    // Test 8: REQQ state change trace (1 cycle: combinational + output FF)
    // ────────────────────────────────────────────────────────
    printf("-- Test 8: REQQ state change trace\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_REQQ));
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 15;
    d->trs_opcode_i = L2_READ;
    d->trs_inflight_i = 1;
    d->trs_pipe_req_eligible_i = 1;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T8: valid=1 for reqq change");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_REQQ,
              "T8: seg0 trace_type == REQQ");

    // ────────────────────────────────────────────────────────
    // Test 9: REQQ no change — should not produce trace
    // (reqq_prev_q now equals reqq_next_if_changed)
    // ────────────────────────────────────────────────────────
    printf("-- Test 9: REQQ no change (suppressed)\n");
    // Keep same state from T8
    d->esr_trace_type_hot_enable_i = (1 << EN_REQQ);
    d->trs_valid_i = 1;
    d->trs_traced_i = 1;
    d->trs_reqq_id_i = 15;
    d->trs_opcode_i = L2_READ;
    d->trs_inflight_i = 1;
    d->trs_pipe_req_eligible_i = 1;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T9: valid=0 when reqq state unchanged");

    // ────────────────────────────────────────────────────────
    // Test 10: Enable/disable — L2 alloc with enable bit off
    // ────────────────────────────────────────────────────────
    printf("-- Test 10: L2 alloc disabled\n");
    clear_inputs(d);
    // pre-arm with the bit OFF; clock gate will block BUT let's also
    // keep some other bit on so the gate passes (otherwise can't sample output)
    // Actually, if all enables are 0, clock_en=0, clk_gated is off, and
    // the output flop does NOT update. This means valid stays at whatever it was.
    // We need to verify that the alloc combinational doesn't fire even before the FF.
    // Let's test by keeping enables 0 for two ticks to ensure clock was also off before.
    d->esr_trace_type_hot_enable_i = 0;
    sim.tick(); // clear pipeline with gate off
    sim.tick(); // another
    // Now drive L2 alloc with enable=0
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 1;
    d->esr_trace_type_hot_enable_i = 0;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T10: valid=0 when alloc_l2 disabled");

    // ────────────────────────────────────────────────────────
    // Test 11: Multiple snippets in same cycle — L2+L3 alloc
    // ────────────────────────────────────────────────────────
    printf("-- Test 11: Multi-snippet (L2+L3 alloc)\n");
    clear_inputs(d);
    uint16_t en11 = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3);
    prearm_enable(sim, en11);
    d->esr_trace_type_hot_enable_i = en11;
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 1;
    d->alloc_l2_port_id_i = 0;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0x5000;
    d->alloc_l2_qwen_i = 0xC;
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 2;
    d->alloc_l3_port_id_i = 1;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0x6000;
    d->alloc_l3_qwen_i = 0x3;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T11: valid=1 for multi-snippet");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_ALLOC_L2,
              "T11: seg0 == ALLOC_L2");
    sim.check(seg_trace_type(d->bank_trace_seg1_o) == TRACE_ALLOC_L3,
              "T11: seg1 == ALLOC_L3");
    sim.check(seg_trace_type(d->bank_trace_seg2_o) == TRACE_NONE,
              "T11: seg2 == NONE");

    // ────────────────────────────────────────────────────────
    // Test 12: L2 alloc with CopFlush remapped to FlushNext
    // ────────────────────────────────────────────────────────
    printf("-- Test 12: L2 CopFlush remap to FlushNext\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L2));
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 8;
    d->alloc_l2_orig_opcode_i = ORIG_COP_FLUSH;
    d->alloc_l2_cache_level_start_i = CACHE_L3;
    d->alloc_l2_address_i = 0x7000;
    d->alloc_l2_qwen_i = 0x1;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T12: valid=1");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_ALLOC_L2,
              "T12: trace_type == ALLOC_L2");
    // Extract orig_opcode: bits [13:9] of the segment
    uint8_t orig_op = (d->bank_trace_seg0_o >> 9) & 0x1F;
    sim.check(orig_op == ORIG_FLUSH_NEXT, "T12: orig_opcode remapped to FlushNext");

    // ────────────────────────────────────────────────────────
    // Test 13: TC with TraceEnTcVictim — victim address used
    // ────────────────────────────────────────────────────────
    printf("-- Test 13: TC with TcVictim enable\n");
    clear_inputs(d);
    uint16_t en13 = (1 << EN_TC) | (1 << EN_TC_VICTIM);
    prearm_enable(sim, en13);
    d->esr_trace_type_hot_enable_i = en13;
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
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T13: valid=1");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_TC,
              "T13: seg0 trace_type == TC");

    // ────────────────────────────────────────────────────────
    // Test 14: TC with TraceEnTcReqqId — reqq_id in tc snippet
    // ────────────────────────────────────────────────────────
    printf("-- Test 14: TC with TcReqqId enable\n");
    clear_inputs(d);
    uint16_t en14 = (1 << EN_TC) | (1 << EN_TC_REQQ_ID);
    prearm_enable(sim, en14);
    d->esr_trace_type_hot_enable_i = en14;
    d->rs0_idx_i = 11;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 11;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_way_i = 3;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_orig_address_i = 0xC000;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0xF;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T14: valid=1");
    // When TraceEnTcReqqId is set, reqq_id field has actual reqq_id
    // tc.reqq_id is at bits [8:3]
    uint8_t tc_reqq_id = (d->bank_trace_seg0_o >> 3) & 0x3F;
    sim.check(tc_reqq_id == 11, "T14: tc.reqq_id == 11");

    // ────────────────────────────────────────────────────────
    // Test 15: Mesh trace — multiple mesh channels in one cycle
    // ────────────────────────────────────────────────────────
    printf("-- Test 15: Mesh multi-channel\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_MESH));
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
    sim.tick();
    // Internal FFs latched. Now wait for combinational + output FF.
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_MESH);
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T15: valid=1 for multi-channel mesh");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_MESH,
              "T15: seg0 trace_type == MESH");

    // ────────────────────────────────────────────────────────
    // Test 16: Not-traced entry — should suppress
    // ────────────────────────────────────────────────────────
    printf("-- Test 16: Not-traced entry suppressed\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L2));
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 0;  // NOT traced
    d->alloc_l2_reqq_id_i = 1;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T16: valid=0 for non-traced entry");

    // ────────────────────────────────────────────────────────
    // Test 17: L2 CopEvict remap
    // ────────────────────────────────────────────────────────
    printf("-- Test 17: L2 CopEvict remap to EvictNext\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L2));
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 12;
    d->alloc_l2_orig_opcode_i = ORIG_COP_EVICT;
    d->alloc_l2_cache_level_start_i = CACHE_MEM;
    d->alloc_l2_address_i = 0x8000;
    d->alloc_l2_qwen_i = 0x5;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T17: valid=1");
    orig_op = (d->bank_trace_seg0_o >> 9) & 0x1F;
    sim.check(orig_op == ORIG_EVICT_NEXT, "T17: orig_opcode remapped to EvictNext");

    // ────────────────────────────────────────────────────────
    // Test 18: L2 CopPrefetch remap (uses cache_level_dest)
    // ────────────────────────────────────────────────────────
    printf("-- Test 18: L2 CopPrefetch remap to PrefetchNext\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_ALLOC_L2));
    d->esr_trace_type_hot_enable_i = (1 << EN_ALLOC_L2);
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 13;
    d->alloc_l2_orig_opcode_i = ORIG_COP_PREFETCH;
    d->alloc_l2_cache_level_dest_i = CACHE_L3;
    d->alloc_l2_address_i = 0x9000;
    d->alloc_l2_qwen_i = 0x2;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T18: valid=1");
    orig_op = (d->bank_trace_seg0_o >> 9) & 0x1F;
    sim.check(orig_op == ORIG_PREFETCH_NEXT, "T18: orig_opcode remapped to PrefetchNext");

    // ────────────────────────────────────────────────────────
    // Test 19: Three snippets packed into one packet
    // ────────────────────────────────────────────────────────
    printf("-- Test 19: Three snippets in one packet\n");
    clear_inputs(d);
    uint16_t en19 = (1 << EN_ALLOC_L2) | (1 << EN_ALLOC_L3) | (1 << EN_TC);
    prearm_enable(sim, en19);
    d->esr_trace_type_hot_enable_i = en19;
    d->rs0_idx_i = 30;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->alloc_l2_valid_i = 1;
    d->alloc_l2_traced_i = 1;
    d->alloc_l2_reqq_id_i = 30;
    d->alloc_l2_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l2_address_i = 0xD000;
    d->alloc_l2_qwen_i = 0xF;
    d->alloc_l3_valid_i = 1;
    d->alloc_l3_traced_i = 1;
    d->alloc_l3_reqq_id_i = 31;
    d->alloc_l3_orig_opcode_i = ORIG_L2_READ;
    d->alloc_l3_address_i = 0xE000;
    d->alloc_l3_qwen_i = 0xA;
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = 30;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_opcode_i = L2_READ;
    d->pipe_tag_rsp_orig_address_i = 0xD000;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0x5;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 1, "T19: valid=1 for 3-snippet packet");
    sim.check(seg_trace_type(d->bank_trace_seg0_o) == TRACE_ALLOC_L2,
              "T19: seg0 == ALLOC_L2");
    sim.check(seg_trace_type(d->bank_trace_seg1_o) == TRACE_ALLOC_L3,
              "T19: seg1 == ALLOC_L3");
    sim.check(seg_trace_type(d->bank_trace_seg2_o) == TRACE_TC,
              "T19: seg2 == TC");

    // ────────────────────────────────────────────────────────
    // Test 20: RBUF without ready — should not trigger
    // ────────────────────────────────────────────────────────
    printf("-- Test 20: RBUF without ready (suppressed)\n");
    clear_inputs(d);
    prearm_enable(sim, (1 << EN_RBUF));
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 6;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x10000;
    d->pipe_rbuf_rd_req_valid_i = 1;
    d->pipe_rbuf_rd_req_ready_i = 0;  // NOT ready
    d->pipe_rbuf_rd_req_reqq_id_i = 6;
    sim.tick();
    clear_inputs(d);
    d->esr_trace_type_hot_enable_i = (1 << EN_RBUF);
    d->rs0_idx_i = 6;
    d->rs0_valid_i = 1;
    d->rs0_traced_i = 1;
    d->rs0_address_i = 0x10000;
    sim.tick();
    sim.check(d->bank_trace_valid_o == 0, "T20: valid=0 when rbuf not ready");

    // ────────────────────────────────────────────────────────
    // Idle to drain pipeline
    // ────────────────────────────────────────────────────────
    clear_inputs(d);
    for (int i = 0; i < 5; i++) sim.tick();

    return sim.finish();
}

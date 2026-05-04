// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_reqq_entry — control flow behavior.

#include "Vshirecache_reqq_entry_tb.h"
#include "sim_ctrl.h"
#include <cstdio>

using DUT = Vshirecache_reqq_entry_tb;

// reqq_orig_opcode_e values
static constexpr int ORIG_READ  = 0x00;
static constexpr int ORIG_WRITE = 0x01;

// reqq_opcode_e values
static constexpr int OP_L2_READ     = 0x00;
static constexpr int OP_L2_WRITE    = 0x01;
static constexpr int OP_REQQ_INITIAL = 0x3d;
static constexpr int OP_REQQ_DONE   = 0x3f;

// address_space_e
static constexpr int ADDR_L2 = 0;

// reqq_source_e
static constexpr int SRC_L2 = 0;

// pipe_data_rsp_type_e
static constexpr int RSP_NORMAL = 0;

static void clear_inputs(DUT* dut) {
    dut->alloc_me_ag_i = 0;
    dut->alloc_me_ad_i = 0;
    dut->pipe_req_picked_me_i = 0;
    dut->pipe_req_picked_any_i = 0;
    dut->pipe_req_reqq_id_any_i = 0;
    dut->pipe_tag_rsp_valid_any_i = 0;
    dut->pipe_tag_rsp_valid_me_i = 0;
    dut->tag_rsp_hit_i = 0;
    dut->tag_rsp_victim_i = 0;
    dut->tag_rsp_victim_silent_i = 0;
    dut->tag_rsp_victim_address_i = 0;
    dut->tag_rsp_victim_address_space_i = 0;
    dut->tag_rsp_victim_qwen_i = 0;
    dut->tag_rsp_bubble_i = 0;
    dut->tag_rsp_opcode_i = 0;
    dut->tag_rsp_err_rsp_i = 0;
    dut->pipe_data_rsp_valid_me_i = 0;
    dut->data_rsp_type_i = 0;
    dut->pipe_cbuf_rsp_valid_any_i = 0;
    dut->dataq_write_status_any_i = 0;
    dut->dataq_write_status_me_i = 0;
    dut->dataq_write_status_id_i = 0;
    dut->dataq_read_status_me_i = 0;
    dut->to_l3_mesh_req_picked_i = 0;
    dut->to_l3_mesh_rsp_valid_me_i = 0;
    dut->to_l3_mesh_rsp_valid_any_i = 0;
    dut->to_sys_mesh_req_picked_i = 0;
    dut->to_sys_mesh_rsp_valid_me_i = 0;
    dut->to_sys_mesh_rsp_valid_any_i = 0;
    dut->mesh_bypassed_victim_i = 0;
    dut->rspmux_l2_picked_i = 0;
    dut->rspmux_l3_picked_i = 0;
    dut->rspmux_bypassed_data_rsp_i = 0;
    dut->rspmux_sent_valid_i = 0;
    dut->reqq_2nd_pass_need_priority_i = 0;
    dut->reqq_idx_cop_need_priority_i = 0;
    dut->pipe_idx_cop_aborted_i = 0;
    dut->rbuf_req_picked_me_i = 0;
    dut->clear_dep_hot_i = 0;
    dut->clear_dep_rbuf_1hot_pre_i = 0;
}

// Set alloc fields for a simple L2 request.
static void setup_alloc(DUT* dut, int orig_opcode, int opcode,
                         int address_space = ADDR_L2,
                         int reqq_source = SRC_L2) {
    dut->alloc_me_ag_i            = 1;
    dut->alloc_orig_opcode_i      = orig_opcode;
    dut->alloc_opcode_i           = opcode;
    dut->alloc_address_space_i    = address_space;
    dut->alloc_reqq_source_i      = reqq_source;
    dut->alloc_address_i          = 0x1000;
    dut->alloc_qwen_i             = 0xF;  // all qwords
    dut->alloc_size_i             = 6;    // SizeLine
    dut->alloc_trans_id_i         = 0;
    dut->alloc_zero_data_i        = 0;
    dut->alloc_dep_valid_i        = 0;
    dut->alloc_dep_id_i           = 0;
    dut->alloc_dep_non_meshll_head_i = 1;
    dut->alloc_err_valid_i        = 0;
    dut->alloc_err_type_i         = 0;
    dut->alloc_err_imprecise_i    = 0;
    dut->alloc_paired_id_i        = 0;
    dut->alloc_cache_level_start_i = 1;  // CacheL2
    dut->alloc_cache_level_dest_i  = 1;  // CacheL2
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 100000;
    auto* dut = sim.dut.get();

    dut->my_reqq_id_i = 1;
    clear_inputs(dut);
    sim.reset();

    // ── Test 1: Reset state ─────────────────────────────────
    printf("test 1: reset state\n");
    sim.check(dut->reqq_entry_valid_o == 0, "valid=0 after reset");
    sim.check(dut->pipe_req_eligible_o == 0, "pipe_req_eligible=0 after reset");
    sim.check(dut->rbuf_req_eligible_o == 0, "rbuf_req_eligible=0 after reset");
    sim.check(dut->to_l3_mesh_req_eligible_o == 0, "to_l3_mesh_req_eligible=0 after reset");
    sim.check(dut->to_sys_mesh_req_eligible_o == 0, "to_sys_mesh_req_eligible=0 after reset");
    sim.check(dut->rspmux_l2_eligible_o == 0, "rspmux_l2_eligible=0 after reset");
    sim.check(dut->rspmux_l3_eligible_o == 0, "rspmux_l3_eligible=0 after reset");

    // ── Test 2: Basic allocation (L2 Read) ──────────────────
    printf("test 2: basic allocation\n");
    setup_alloc(dut, ORIG_READ, OP_REQQ_INITIAL);
    sim.tick();
    clear_inputs(dut);

    sim.check(dut->reqq_entry_alloc_o == 1, "alloc fires on alloc cycle");
    sim.tick();
    sim.check(dut->reqq_entry_valid_o == 1, "valid=1 after alloc");
    // After ReqqInitial, the comb logic transitions opcode to L2Read in the
    // same cycle, and the flop captures it on the next edge.
    sim.tick();
    sim.check(dut->opcode_o == OP_L2_READ, "opcode transitions to L2Read");

    // After one more cycle, pipe_req_eligible should be high (no deps, head of list)
    sim.tick();
    sim.check(dut->pipe_req_eligible_o == 1, "pipe_req_eligible=1 for L2Read head");

    // ── Test 3: L2 Read hit → Done ─────────────────────────
    printf("test 3: L2 read hit -> done\n");

    // Pipe picks the request
    dut->pipe_req_picked_me_i = 1;
    dut->pipe_req_picked_any_i = 1;
    dut->pipe_req_reqq_id_any_i = 1;
    sim.tick();
    clear_inputs(dut);

    // Tag response: hit, no victim
    dut->pipe_tag_rsp_valid_any_i = 1;
    dut->pipe_tag_rsp_valid_me_i = 1;
    dut->tag_rsp_hit_i = 1;
    dut->tag_rsp_victim_i = 0;
    dut->tag_rsp_opcode_i = OP_L2_READ;
    sim.tick();
    clear_inputs(dut);

    // Data response: NormalRsp (confirms hit)
    dut->pipe_data_rsp_valid_me_i = 1;
    dut->data_rsp_type_i = RSP_NORMAL;
    sim.tick();
    clear_inputs(dut);

    // Let opcode settle
    sim.tick();
    sim.check(dut->opcode_o == OP_REQQ_DONE, "opcode transitions to ReqqDone after read hit");

    // rspmux_l2_eligible should go high (source is L2)
    sim.tick();
    sim.check(dut->rspmux_l2_eligible_o == 1, "rspmux_l2_eligible=1 after read hit done");

    // ── Test 4: Deallocation ────────────────────────────────
    printf("test 4: deallocation\n");

    // Rspmux picks the response
    dut->rspmux_l2_picked_i = 1;
    sim.tick();
    clear_inputs(dut);

    // rspmux_sent_valid tells the entry the response was sent
    dut->rspmux_sent_valid_i = 1;
    sim.tick();
    clear_inputs(dut);

    // Wait for dealloc to fire
    int limit = 10;
    while (dut->reqq_entry_valid_o && limit-- > 0) sim.tick();

    sim.check(dut->reqq_entry_valid_o == 0, "valid=0 after deallocation");

    // ── Test 5: L2 Write no victim → Done ───────────────────
    printf("test 5: L2 write no victim\n");
    clear_inputs(dut);
    setup_alloc(dut, ORIG_WRITE, OP_REQQ_INITIAL);
    sim.tick();
    clear_inputs(dut);

    // Let opcode transition: Initial -> L2Write
    sim.tick();
    sim.tick();
    sim.check(dut->opcode_o == OP_L2_WRITE, "opcode transitions to L2Write");

    // L2Write needs data_ready before eligible — signal dataq write complete
    dut->dataq_write_status_me_i = 1;
    sim.tick();
    clear_inputs(dut);

    // Now eligible should go high (data_ready + no deps)
    sim.tick();
    sim.check(dut->pipe_req_eligible_o == 1, "pipe_req_eligible=1 for L2Write after data_ready");

    // Pipe picks write
    dut->pipe_req_picked_me_i = 1;
    dut->pipe_req_picked_any_i = 1;
    dut->pipe_req_reqq_id_any_i = 1;
    sim.tick();
    clear_inputs(dut);

    // Tag response: no victim
    dut->pipe_tag_rsp_valid_any_i = 1;
    dut->pipe_tag_rsp_valid_me_i = 1;
    dut->tag_rsp_hit_i = 1;
    dut->tag_rsp_victim_i = 0;
    dut->tag_rsp_opcode_i = OP_L2_WRITE;
    sim.tick();
    clear_inputs(dut);

    // Let state settle
    sim.tick();
    sim.check(dut->opcode_o == OP_REQQ_DONE, "L2Write opcode transitions to ReqqDone");
    sim.check(dut->rspmux_l2_eligible_o == 1, "rspmux_l2_eligible after write done");

    // Complete the dealloc cycle: rspmux picks and sends
    dut->rspmux_l2_picked_i = 1;
    sim.tick();
    clear_inputs(dut);
    dut->rspmux_sent_valid_i = 1;
    sim.tick();
    clear_inputs(dut);

    // Dataq read status (pipe_req_picked set wait_for_dataq again, clear it)
    dut->dataq_read_status_me_i = 1;
    sim.tick();
    clear_inputs(dut);

    limit = 10;
    while (dut->reqq_entry_valid_o && limit-- > 0) sim.tick();
    sim.check(dut->reqq_entry_valid_o == 0, "valid=0 after write deallocation");

    // ── Test 6: Dependency suppression ──────────────────────
    printf("test 6: dependency suppression\n");
    clear_inputs(dut);
    setup_alloc(dut, ORIG_READ, OP_REQQ_INITIAL);
    dut->alloc_dep_valid_i = 1;
    dut->alloc_dep_id_i = 5;  // depends on entry 5
    sim.tick();
    clear_inputs(dut);

    // Let opcode transition to L2Read
    sim.tick();
    sim.tick();
    sim.check(dut->opcode_o == OP_L2_READ, "opcode is L2Read with dep");

    // pipe_req_eligible should stay 0 because dep_valid=1
    sim.tick();
    sim.check(dut->pipe_req_eligible_o == 0, "pipe_req_eligible=0 when dep_valid");

    // Clear the dependency via clear_dep_hot
    dut->clear_dep_hot_i = 1ULL << 5;
    sim.tick();
    clear_inputs(dut);

    // After dep clears, eligible should go high
    sim.tick();
    sim.check(dut->pipe_req_eligible_o == 1, "pipe_req_eligible=1 after dep cleared");

    return sim.finish();
}

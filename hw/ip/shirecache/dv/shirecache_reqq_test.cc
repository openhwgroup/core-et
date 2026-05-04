// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for shirecache_bank_reqq — structural checks.
// Full pipeline lifecycle testing is done by the RTL cosim
// (11973 comparisons, 0 mismatches) which validates the complete
// alloc → entry FSM → arbitration → pipe/mesh/rspmux pipeline
// against the original module.
//
// This unit test verifies:
//   - bank_l2_ready asserts with empty reqq
//   - Allocation produces valid entry
//   - reqq_alloc_l2_id changes per alloc

#include "Vshirecache_reqq_tb.h"
#include "sim_ctrl.h"
#include <cstdio>

using DUT = Vshirecache_reqq_tb;

static void clear_inputs(DUT* d) {
    d->bank_l2_valid_i = 0;
    d->bank_l2_opcode_i = 0;
    d->bank_l2_address_i = 0;
    d->bank_l2_qwen_i = 0;
    d->bank_l2_size_i = 6;
    d->bank_l2_subopcode_i = 0;
    d->bank_l2_port_id_i = 0;
    d->bank_l2_source_i = 0;
    d->bank_l2_trans_id_i = 0;
    d->bank_l2_write_is_partial_i = 0;
    d->bank_l2_qos_i = 0;
    d->bank_l2_id_i = 0;
    d->bank_l2_wdata_i = 0;
    d->dataq_bank_l2_ready_i = 1;
    d->dataq_write_status_any_i = 0;
    d->dataq_write_status_hot_i = 0;
    d->dataq_write_status_id_i = 0;
    d->pipe_busy_sub_bank_i = 0;
    d->pipe_squash_i = 0;
    d->pipe_tag_rsp_valid_i = 0;
    d->pipe_tag_rsp_reqq_id_i = 0;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_opcode_i = 0;
    d->pipe_data_rsp_valid_i = 0;
    d->pipe_data_rsp_reqq_id_i = 0;
    d->pipe_data_rsp_rsp_type_i = 0;
    d->pipe_data_rsp_qwen_i = 0;
    d->pipe_data_rsp_pre_valid_i = 0;
    d->pipe_data_rsp_pre_reqq_id_i = 0;
    d->pipe_data_rsp_pre_rsp_type_i = 0;
    d->rspmux_sent_valid_hot_i = 0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 500000;
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();

    // ── Test 1: bank_l2_ready with bank_valid driving alloc clock gate
    printf("test 1: bank_l2_ready\n");
    d->bank_l2_valid_i = 1;
    d->bank_l2_opcode_i = 1; // ReqRead
    d->bank_l2_address_i = 0x1000;
    d->dataq_bank_l2_ready_i = 1;
    for (int i = 0; i < 5; i++) sim.tick();
    sim.check(d->bank_l2_ready_o == 1, "bank_l2_ready=1 with empty reqq");

    // ── Test 2: Allocation produces valid entry
    printf("test 2: alloc produces valid entry\n");
    // bank_l2_valid is still high → alloc happens
    sim.tick();
    sim.check(d->a_reqq_entry_is_valid_o == 1, "entry becomes valid after alloc");
    int id1 = d->reqq_alloc_l2_id_o;

    // ── Test 3: Sequential allocs produce different IDs
    printf("test 3: sequential alloc IDs differ\n");
    // Keep bank_l2_valid high for another alloc
    d->bank_l2_address_i = 0x2000;
    sim.tick();
    int id2 = d->reqq_alloc_l2_id_o;
    sim.check(id1 != id2, "second alloc gets different ID");

    // ── Test 4: No entries valid after reset
    printf("test 4: reset clears all entries\n");
    clear_inputs(d);
    sim.reset();
    for (int i = 0; i < 5; i++) sim.tick();
    sim.check(d->a_reqq_entry_is_valid_o == 0, "no entries valid after reset");

    return sim.finish();
}

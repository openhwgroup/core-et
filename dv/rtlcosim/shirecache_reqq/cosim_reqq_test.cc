// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_reqq vs shirecache_bank_reqq
// Comprehensive end-to-end test covering all major paths.

#include "Vcosim_reqq_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using DUT = Vcosim_reqq_tb;

// ET-Link request opcodes (etlink_pkg::req_opcode_e)
static constexpr int REQ_WRITE        = 0;
static constexpr int REQ_READ         = 1;
static constexpr int REQ_WRITE_AROUND = 2;
static constexpr int REQ_READ_COOP    = 3;
static constexpr int REQ_ATOMIC       = 5;
static constexpr int REQ_FLUSH        = 8;
static constexpr int REQ_EVICT        = 10;
static constexpr int REQ_PREFETCH     = 15;

// reqq_opcode_e for tag_rsp / idx_cop
static constexpr int OP_L2_READ       = 0x00;
static constexpr int OP_L2_WRITE      = 0x01;
static constexpr int OP_L2_WRITE_P    = 0x02;
static constexpr int OP_L2_FLUSH      = 0x04;
static constexpr int OP_L2_EVICT      = 0x06;
static constexpr int OP_L2_PREFETCH   = 0x0b;
static constexpr int OP_L2_FILL       = 0x0c;
static constexpr int OP_L3_READ       = 0x10;
static constexpr int OP_L3_WRITE      = 0x11;
static constexpr int OP_L3_FLUSH      = 0x14;
static constexpr int OP_L3_EVICT      = 0x16;
static constexpr int OP_L3_PREFETCH   = 0x18;
static constexpr int OP_L3_FILL       = 0x1b;
static constexpr int OP_IDX_READ      = 0x26;
static constexpr int OP_IDX_WRITE     = 0x27;
static constexpr int OP_IDX_ALL_INV   = 0x28;
static constexpr int OP_IDX_L2_INV    = 0x29;
static constexpr int OP_IDX_L2_FLUSH  = 0x2a;
static constexpr int OP_IDX_L2_EVICT  = 0x2b;
static constexpr int OP_IDX_L3_INV    = 0x2c;
static constexpr int OP_IDX_L3_FLUSH  = 0x2d;
static constexpr int OP_IDX_L3_EVICT  = 0x2e;
static constexpr int OP_REQQ_DONE     = 0x3f;

// pipe_data_rsp_type_e
static constexpr int RSP_NORMAL       = 0;
static constexpr int RSP_VICTIM       = 1;

// err_rsp_e values
static constexpr int ERR_NONE         = 0;

// address_space_e
static constexpr int ADDR_L2          = 0;
static constexpr int ADDR_L3          = 1;
static constexpr int ADDR_SCP         = 2;
static constexpr int ADDR_IDX         = 3;

// Address bit layout:
//   [5:0]   line offset (64-byte cache line)
//   [7:6]   bank ID
//   [9:8]   sub-bank ID (4 sub-banks)
//   [17:10] set index

// ===================================================================
// Helpers
// ===================================================================

static void clear_inputs(DUT* d) {
    d->bank_l2_valid_i = 0;
    d->bank_l2_opcode_i = 0;
    d->bank_l2_subopcode_i = 0;
    d->bank_l2_address_i = 0;
    d->bank_l2_qwen_i = 0;
    d->bank_l2_size_i = 0;
    d->bank_l2_port_id_i = 0;
    d->bank_l2_source_i = 0;
    d->bank_l2_trans_id_i = 0;
    d->bank_l2_write_is_partial_i = 0;
    d->bank_l2_qos_i = 0;
    d->bank_l2_id_i = 0;
    d->bank_l2_wdata_i = 0;
    d->dataq_bank_l2_ready_i = 1;

    d->bank_l3_valid_i = 0;
    d->bank_l3_opcode_i = 0;
    d->bank_l3_subopcode_i = 0;
    d->bank_l3_address_i = 0;
    d->bank_l3_qwen_i = 0;
    d->bank_l3_size_i = 0;
    d->bank_l3_port_id_i = 0;
    d->bank_l3_source_i = 0;
    d->bank_l3_trans_id_i = 0;
    d->bank_l3_write_is_partial_i = 0;
    d->bank_l3_qos_i = 0;
    d->bank_l3_id_i = 0;
    d->bank_l3_wdata_i = 0;
    d->dataq_bank_l3_ready_i = 1;

    d->pipe_idx_cop_aborted_i = 0;
    d->pipe_idx_cop_l2_busy_i = 0;
    d->pipe_idx_cop_l3_busy_i = 0;
    d->pipe_idx_cop_req_valid_i = 0;
    d->pipe_idx_cop_req_opcode_i = 0;
    d->pipe_idx_cop_req_address_i = 0;
    d->pipe_idx_cop_req_trans_id_i = 0;
    d->pipe_idx_cop_req_sync_flush_victims_i = 0;

    d->dataq_write_status_any_i = 0;
    d->dataq_write_status_hot_i = 0;
    d->dataq_write_status_id_i = 0;
    d->dataq_read_status_hot_i = 0;

    d->pipe_busy_sub_bank_i = 0;
    d->pipe_squash_i = 0;

    d->pipe_tag_rsp_valid_i = 0;
    d->pipe_tag_rsp_trans_id_i = 0;
    d->pipe_tag_rsp_reqq_id_i = 0;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_orig_address_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_victim_silent_i = 0;
    d->pipe_tag_rsp_victim_address_i = 0;
    d->pipe_tag_rsp_victim_address_space_i = 0;
    d->pipe_tag_rsp_victim_write_around_i = 0;
    d->pipe_tag_rsp_victim_qwen_i = 0;
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_tag_index_i = 0;
    d->pipe_tag_rsp_bubble_i = 0;
    d->pipe_tag_rsp_opcode_i = 0;
    d->pipe_tag_rsp_tag_ram_hit_way_i = 0;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0;
    d->pipe_tag_rsp_tag_state_ram_sbe_i = 0;
    d->pipe_tag_rsp_tag_state_ram_dbe_i = 0;
    d->pipe_tag_rsp_tag_ram_sbe_i = 0;
    d->pipe_tag_rsp_tag_ram_dbe_i = 0;

    d->pipe_data_rsp_valid_i = 0;
    d->pipe_data_rsp_trans_id_i = 0;
    d->pipe_data_rsp_reqq_id_i = 0;
    d->pipe_data_rsp_rsp_type_i = 0;
    d->pipe_data_rsp_tag_sbe_i = 0;
    d->pipe_data_rsp_qwen_i = 0;
    d->pipe_data_rsp_data_index_i = 0;
    d->pipe_data_rsp_data_ram_sbe_i = 0;
    d->pipe_data_rsp_data_ram_dbe_i = 0;

    d->pipe_data_rsp_pre_valid_i = 0;
    d->pipe_data_rsp_pre_trans_id_i = 0;
    d->pipe_data_rsp_pre_reqq_id_i = 0;
    d->pipe_data_rsp_pre_rsp_type_i = 0;

    d->pipe_cbuf_rsp_valid_i = 0;
    d->pipe_cbuf_rsp_trans_id_i = 0;
    d->pipe_cbuf_rsp_reqq_id_i = 0;
    d->pipe_cbuf_rsp_victim_i = 0;
    d->pipe_cbuf_rsp_victim_address_i = 0;

    d->mesh_bypassed_victim_i = 0;
    d->rspmux_bypassed_data_rsp_i = 0;

    d->set_reqq_err_detected_hot_i = 0;
    d->set_reqq_err_zero_data_hot_i = 0;

    d->pipe_rbuf_rd_req_ready_i = 1;

    d->reqq_to_l3_mesh_req_ready_nodata_i = 1;
    d->reqq_to_l3_mesh_req_ready_wdata_i = 1;
    d->reqq_to_sys_mesh_req_ready_nodata_i = 1;
    d->reqq_to_sys_mesh_req_ready_wdata_i = 1;

    d->to_l3_mesh_rsp_valid_i = 0;
    d->to_l3_mesh_rsp_trans_id_i = 0;
    d->to_l3_mesh_rsp_reqq_id_i = 0;
    d->to_l3_mesh_rsp_dataq_id_i = 0;
    d->to_l3_mesh_rsp_zero_data_i = 0;
    d->to_l3_mesh_rsp_err_i = 0;

    d->to_sys_mesh_rsp_valid_i = 0;
    d->to_sys_mesh_rsp_trans_id_i = 0;
    d->to_sys_mesh_rsp_reqq_id_i = 0;
    d->to_sys_mesh_rsp_dataq_id_i = 0;
    d->to_sys_mesh_rsp_zero_data_i = 0;
    d->to_sys_mesh_rsp_err_i = 0;

    d->reqq_rspmux_l2_ready_nodata_i = 1;
    d->reqq_rspmux_l2_ready_wdata_i = 1;
    d->reqq_rspmux_l3_ready_nodata_i = 1;
    d->reqq_rspmux_l3_ready_wdata_i = 1;

    d->rspmux_sent_valid_hot_i = 0;

    d->mbist_on_i = 0;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("bank_l2_ready",         d->orig_bank_l2_ready_o,         d->new_bank_l2_ready_o);
    sim.compare("bank_l3_ready",         d->orig_bank_l3_ready_o,         d->new_bank_l3_ready_o);
    sim.compare("pipe_start",            d->orig_pipe_start_o,            d->new_pipe_start_o);
    sim.compare("pipe_idx_cop_req_ready",d->orig_pipe_idx_cop_req_ready_o,d->new_pipe_idx_cop_req_ready_o);
    sim.compare("a_reqq_entry_is_valid", d->orig_a_reqq_entry_is_valid_o, d->new_a_reqq_entry_is_valid_o);

    // Only compare pipe_req_reqq_id and opcode when pipe_start is asserted
    if (d->orig_pipe_start_o && d->new_pipe_start_o) {
        sim.compare("pipe_req_reqq_id",  d->orig_pipe_req_reqq_id_o,     d->new_pipe_req_reqq_id_o);
        sim.compare("pipe_req_opcode",   d->orig_pipe_req_opcode_o,      d->new_pipe_req_opcode_o);
    }

    sim.compare("to_l3_mesh_req_valid_nodata", d->orig_to_l3_mesh_req_valid_nodata_o, d->new_to_l3_mesh_req_valid_nodata_o);
    sim.compare("to_l3_mesh_req_valid_wdata",  d->orig_to_l3_mesh_req_valid_wdata_o,  d->new_to_l3_mesh_req_valid_wdata_o);
    sim.compare("to_sys_mesh_req_valid_nodata", d->orig_to_sys_mesh_req_valid_nodata_o, d->new_to_sys_mesh_req_valid_nodata_o);
    sim.compare("to_sys_mesh_req_valid_wdata",  d->orig_to_sys_mesh_req_valid_wdata_o,  d->new_to_sys_mesh_req_valid_wdata_o);

    sim.compare("rspmux_l2_valid_nodata", d->orig_rspmux_l2_valid_nodata_o, d->new_rspmux_l2_valid_nodata_o);
    sim.compare("rspmux_l2_valid_wdata",  d->orig_rspmux_l2_valid_wdata_o,  d->new_rspmux_l2_valid_wdata_o);
    sim.compare("rspmux_l3_valid_nodata", d->orig_rspmux_l3_valid_nodata_o, d->new_rspmux_l3_valid_nodata_o);
    sim.compare("rspmux_l3_valid_wdata",  d->orig_rspmux_l3_valid_wdata_o,  d->new_rspmux_l3_valid_wdata_o);

    sim.compare("rbuf_rd_req_valid",     d->orig_pipe_rbuf_rd_req_valid_o, d->new_pipe_rbuf_rd_req_valid_o);
}

// Submit an L2 read request for one cycle
static void submit_l2_read(DUT* d, uint32_t address, int trans_id = 0) {
    d->bank_l2_valid_i = 1;
    d->bank_l2_opcode_i = REQ_READ;
    d->bank_l2_address_i = address;
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = 6; // SizeLine
    d->bank_l2_port_id_i = 0;
    d->bank_l2_source_i = 0;
    d->bank_l2_trans_id_i = trans_id;
    d->bank_l2_id_i = 0;
}

// Submit an L2 write request for one cycle
static void submit_l2_write(DUT* d, uint32_t address, int trans_id = 0,
                            bool partial = false) {
    d->bank_l2_valid_i = 1;
    d->bank_l2_opcode_i = REQ_WRITE;
    d->bank_l2_address_i = address;
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = 6; // SizeLine
    d->bank_l2_port_id_i = 0;
    d->bank_l2_source_i = 0;
    d->bank_l2_trans_id_i = trans_id;
    d->bank_l2_id_i = 0;
    d->bank_l2_wdata_i = 1;
    d->bank_l2_write_is_partial_i = partial ? 1 : 0;
}

// Submit an L2 request with any opcode
static void submit_l2_op(DUT* d, uint32_t address, int opcode,
                         int trans_id = 0) {
    d->bank_l2_valid_i = 1;
    d->bank_l2_opcode_i = opcode;
    d->bank_l2_address_i = address;
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = 6;
    d->bank_l2_port_id_i = 0;
    d->bank_l2_source_i = 0;
    d->bank_l2_trans_id_i = trans_id;
    d->bank_l2_id_i = 0;
    d->bank_l2_wdata_i = (opcode == REQ_WRITE || opcode == REQ_WRITE_AROUND) ? 1 : 0;
}

// Submit an L3 read request for one cycle
static void submit_l3_read(DUT* d, uint32_t address, int trans_id = 0) {
    d->bank_l3_valid_i = 1;
    d->bank_l3_opcode_i = REQ_READ;
    d->bank_l3_address_i = address;
    d->bank_l3_qwen_i = 0xF;
    d->bank_l3_size_i = 6; // SizeLine
    d->bank_l3_port_id_i = 0;
    d->bank_l3_source_i = 0;
    d->bank_l3_trans_id_i = trans_id;
    d->bank_l3_id_i = 0;
}

// Submit an L3 write request for one cycle
static void submit_l3_write(DUT* d, uint32_t address, int trans_id = 0) {
    d->bank_l3_valid_i = 1;
    d->bank_l3_opcode_i = REQ_WRITE;
    d->bank_l3_address_i = address;
    d->bank_l3_qwen_i = 0xF;
    d->bank_l3_size_i = 6;
    d->bank_l3_port_id_i = 0;
    d->bank_l3_source_i = 0;
    d->bank_l3_trans_id_i = trans_id;
    d->bank_l3_id_i = 0;
    d->bank_l3_wdata_i = 1;
}

// Submit an L3 request with any opcode
static void submit_l3_op(DUT* d, uint32_t address, int opcode,
                         int trans_id = 0) {
    d->bank_l3_valid_i = 1;
    d->bank_l3_opcode_i = opcode;
    d->bank_l3_address_i = address;
    d->bank_l3_qwen_i = 0xF;
    d->bank_l3_size_i = 6;
    d->bank_l3_port_id_i = 0;
    d->bank_l3_source_i = 0;
    d->bank_l3_trans_id_i = trans_id;
    d->bank_l3_id_i = 0;
    d->bank_l3_wdata_i = (opcode == REQ_WRITE || opcode == REQ_WRITE_AROUND) ? 1 : 0;
}

// Drive a tag response for a given reqq_id
static void drive_tag_rsp(DUT* d, int reqq_id, bool hit, int opcode = OP_L2_READ,
                          bool victim = false, bool victim_silent = false,
                          uint32_t victim_addr = 0) {
    d->pipe_tag_rsp_valid_i = 1;
    d->pipe_tag_rsp_reqq_id_i = reqq_id;
    d->pipe_tag_rsp_hit_i = hit ? 1 : 0;
    d->pipe_tag_rsp_opcode_i = opcode;
    d->pipe_tag_rsp_victim_i = victim ? 1 : 0;
    d->pipe_tag_rsp_victim_silent_i = victim_silent ? 1 : 0;
    d->pipe_tag_rsp_victim_address_i = victim_addr;
    d->pipe_tag_rsp_victim_qwen_i = victim ? 0xF : 0;
    d->pipe_tag_rsp_tag_ram_hit_way_i = hit ? 1 : 0;
    d->pipe_tag_rsp_tag_ram_hit_qwen_i = 0xF;
}

// Drive a data response for a given reqq_id
static void drive_data_rsp(DUT* d, int reqq_id, int rsp_type = RSP_NORMAL) {
    d->pipe_data_rsp_valid_i = 1;
    d->pipe_data_rsp_reqq_id_i = reqq_id;
    d->pipe_data_rsp_rsp_type_i = rsp_type;
    d->pipe_data_rsp_qwen_i = 0xF;
}

// Drive a data_rsp_pre for a given reqq_id (1 cycle before data_rsp)
static void drive_data_rsp_pre(DUT* d, int reqq_id, int rsp_type = RSP_NORMAL) {
    d->pipe_data_rsp_pre_valid_i = 1;
    d->pipe_data_rsp_pre_reqq_id_i = reqq_id;
    d->pipe_data_rsp_pre_rsp_type_i = rsp_type;
}

// Drive rspmux_sent for a specific reqq_id
static void drive_rspmux_sent(DUT* d, int reqq_id) {
    d->rspmux_sent_valid_hot_i = (uint64_t)1 << reqq_id;
}

// Helper: wait for pipe_start with timeout, returns true if pipe_start seen
static bool wait_for_pipe_start(CosimCtrl<DUT>& sim, int max_cycles = 10) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);
        if (d->new_pipe_start_o) return true;
    }
    return false;
}

// Helper: complete an entry fully (tag hit -> data_rsp_pre -> data_rsp -> rspmux_sent)
static void complete_entry_hit(CosimCtrl<DUT>& sim, int reqq_id, int opcode = OP_L2_READ) {
    auto* d = sim.dut.get();

    // Tag hit
    clear_inputs(d);
    drive_tag_rsp(d, reqq_id, true, opcode);
    sim.tick();
    compare_outputs(sim);

    // Data rsp pre
    clear_inputs(d);
    drive_data_rsp_pre(d, reqq_id, RSP_NORMAL);
    sim.tick();
    compare_outputs(sim);

    // Data rsp
    clear_inputs(d);
    drive_data_rsp(d, reqq_id, RSP_NORMAL);
    sim.tick();
    compare_outputs(sim);

    // Wait for rspmux
    for (int i = 0; i < 5; i++) {
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);
    }

    // Rspmux sent
    clear_inputs(d);
    drive_rspmux_sent(d, reqq_id);
    sim.tick();
    compare_outputs(sim);

    // Settle
    clear_inputs(d);
    sim.tick();
    compare_outputs(sim);
}

// Helper: drain an entry that is already pipe_start'd (picks it up from that cycle)
static void drain_picked_entry(CosimCtrl<DUT>& sim, int reqq_id, int opcode = OP_L2_READ) {
    complete_entry_hit(sim, reqq_id, opcode);
}

// Helper: idle for N cycles
static void idle(CosimCtrl<DUT>& sim, int n) {
    auto* d = sim.dut.get();
    for (int i = 0; i < n; i++) {
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);
    }
}

// Helper: alloc L2 read, wait for pipe_start, complete. Returns alloc id.
static int alloc_and_complete_l2_read(CosimCtrl<DUT>& sim, uint32_t addr, int trans_id) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    submit_l2_read(d, addr, trans_id);
    sim.tick();
    compare_outputs(sim);

    int aid = d->new_reqq_alloc_l2_id_o;

    wait_for_pipe_start(sim);
    complete_entry_hit(sim, aid, OP_L2_READ);
    return aid;
}

// ===================================================================
// Main test
// ===================================================================

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    sim.max_time = 2000000;

    clear_inputs(d);
    sim.reset();

    // ================================================================
    // Test 1: Reset + idle
    // ================================================================
    printf("Test 1: Reset + idle\n");
    for (int i = 0; i < 5; i++) {
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);
    }

    // ================================================================
    // Test 2: L2 Read hit (full lifecycle)
    //   alloc -> pipe_start -> tag hit -> data_rsp_pre -> data_rsp
    //   -> rspmux_l2 -> rspmux_sent -> dealloc
    // ================================================================
    printf("Test 2: L2 Read hit\n");
    clear_inputs(d);
    submit_l2_read(d, 0x1000, 1);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 3: L2 Read miss -> mesh -> re-pipe fill -> done
    //   alloc -> pipe -> tag miss -> to_l3_mesh_req -> mesh_rsp
    //   -> re-pipe fill -> tag_rsp -> done -> rspmux -> dealloc
    // ================================================================
    printf("Test 3: L2 Read miss -> mesh\n");
    clear_inputs(d);
    submit_l2_read(d, 0x2000, 2);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // Wait for pipe_start
        wait_for_pipe_start(sim);

        // Tag miss -> entry goes to mesh
        clear_inputs(d);
        drive_tag_rsp(d, aid, false, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh req to appear
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Drive mesh response
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        sim.tick();
        compare_outputs(sim);

        // Wait for re-pipe (fill)
        wait_for_pipe_start(sim, 15);

        int picked = d->new_pipe_start_o ? (int)d->new_pipe_req_reqq_id_o : aid;

        // Complete with tag hit + data rsp
        clear_inputs(d);
        drive_tag_rsp(d, picked, true, OP_L2_FILL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, picked, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, picked, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        clear_inputs(d);
        drive_rspmux_sent(d, picked);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 4: L2 Write no victim
    //   alloc -> dataq_write_status -> pipe -> tag hit no victim
    //   -> data_rsp -> rspmux -> dealloc
    // ================================================================
    printf("Test 4: L2 Write no victim\n");
    clear_inputs(d);
    submit_l2_write(d, 0x3000, 3);
    sim.tick();
    compare_outputs(sim);

    {
        int wid = d->new_reqq_alloc_l2_id_o;

        // Dataq write status
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);

        // Tag hit, no victim
        clear_inputs(d);
        drive_tag_rsp(d, wid, true, OP_L2_WRITE);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, wid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, wid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        clear_inputs(d);
        drive_rspmux_sent(d, wid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 5: L2 Write with victim
    //   alloc -> pipe -> tag hit + victim -> mesh_victim -> mesh_rsp
    //   -> rspmux -> dealloc
    // ================================================================
    printf("Test 5: L2 Write with victim\n");
    clear_inputs(d);
    submit_l2_write(d, 0x4000, 4);
    sim.tick();
    compare_outputs(sim);

    {
        int wid = d->new_reqq_alloc_l2_id_o;

        // Dataq write status
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);

        // Tag hit WITH victim
        clear_inputs(d);
        drive_tag_rsp(d, wid, true, OP_L2_WRITE, /*victim=*/true, /*silent=*/false, 0xBEEF0);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, wid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, wid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Victim data response
        clear_inputs(d);
        drive_data_rsp_pre(d, wid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, wid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh victim req
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_wdata_o ||
                d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Drive mesh response for victim
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = wid;
        d->to_l3_mesh_rsp_dataq_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        // Check if rspmux fires or if we need a re-pipe
        // The victim path may require re-pipe. Wait for pipe_start if it comes.
        bool got_pipe = false;
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) { got_pipe = true; break; }
            if (d->new_rspmux_l2_valid_nodata_o || d->new_rspmux_l2_valid_wdata_o) break;
        }

        if (got_pipe) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_REQQ_DONE);
        } else {
            // Just send rspmux_sent
            idle(sim, 3);
            clear_inputs(d);
            drive_rspmux_sent(d, wid);
            sim.tick();
            compare_outputs(sim);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 6: L3 Read alloc
    // ================================================================
    printf("Test 6: L3 Read alloc\n");
    clear_inputs(d);
    submit_l3_read(d, 0x5000, 5);
    sim.tick();
    compare_outputs(sim);

    {
        int l3id = d->new_reqq_alloc_l3_id_o;

        wait_for_pipe_start(sim);

        int picked = d->new_pipe_req_reqq_id_o;
        complete_entry_hit(sim, picked, OP_L3_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 7: L3 Write alloc
    // ================================================================
    printf("Test 7: L3 Write alloc\n");
    clear_inputs(d);
    submit_l3_write(d, 0x6000, 6);
    sim.tick();
    compare_outputs(sim);

    {
        int l3id = d->new_reqq_alloc_l3_id_o;

        // Dataq write status for L3
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << l3id;
        d->dataq_write_status_id_i = l3id;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);

        int picked = d->new_pipe_req_reqq_id_o;
        complete_entry_hit(sim, picked, OP_L3_WRITE);
    }
    idle(sim, 3);

    // ================================================================
    // Test 8: L2 + L3 concurrent alloc
    //   Both bank_l2_valid and bank_l3_valid on same cycle
    // ================================================================
    printf("Test 8: L2 + L3 concurrent alloc\n");
    clear_inputs(d);
    submit_l2_read(d, 0x7000, 7);
    submit_l3_read(d, 0x7100, 8);
    sim.tick();
    compare_outputs(sim);

    {
        int l2id = d->new_reqq_alloc_l2_id_o;
        int l3id = d->new_reqq_alloc_l3_id_o;

        // Complete both entries
        for (int n = 0; n < 2; n++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) break;
            int picked = d->new_pipe_req_reqq_id_o;
            int op = (picked == l2id) ? OP_L2_READ : OP_L3_READ;
            complete_entry_hit(sim, picked, op);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 9: Multiple subbanks
    //   Alloc 4 entries with different subbank addresses [9:8]
    // ================================================================
    printf("Test 9: Multiple subbanks\n");
    {
        int ids[4];
        for (int sb = 0; sb < 4; sb++) {
            clear_inputs(d);
            // Address with sub_bank = sb: bits [9:8]
            uint32_t addr = 0x10000 | (sb << 8);
            submit_l2_read(d, addr, 10 + sb);
            sim.tick();
            compare_outputs(sim);
            ids[sb] = d->new_reqq_alloc_l2_id_o;
        }

        // Let arbitration settle
        idle(sim, 5);

        // Complete all 4
        for (int n = 0; n < 4; n++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) break;
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 10: L3 vs L2 priority
    //   Alloc both L2 and L3 entries, verify both get pipe_start
    //   (prim_arb_prio gives highest index priority -> L3 should go first)
    // ================================================================
    printf("Test 10: L3 vs L2 priority\n");
    {
        // Alloc L2 first
        clear_inputs(d);
        submit_l2_read(d, 0x11000, 20);
        sim.tick();
        compare_outputs(sim);
        int l2id = d->new_reqq_alloc_l2_id_o;

        // Alloc L3 second (higher index should win priority)
        clear_inputs(d);
        submit_l3_read(d, 0x11100, 21);
        sim.tick();
        compare_outputs(sim);
        int l3id = d->new_reqq_alloc_l3_id_o;

        // Complete both
        for (int n = 0; n < 2; n++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) break;
            int picked = d->new_pipe_req_reqq_id_o;
            int op = (picked == l2id) ? OP_L2_READ : OP_L3_READ;
            complete_entry_hit(sim, picked, op);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 11: Pipe busy (sub_bank_busy)
    //   Alloc entry, assert busy for that subbank, verify it doesn't get picked,
    //   release busy, verify it then gets picked
    // ================================================================
    printf("Test 11: Pipe busy sub_bank\n");
    clear_inputs(d);
    // Address in sub-bank 0 (bits [9:8] = 0)
    submit_l2_read(d, 0x12000, 22);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // Hold sub_bank 0 busy for 5 cycles
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            d->pipe_busy_sub_bank_i = 0x1; // sub-bank 0 busy
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 12: Pipe squash
    //   pipe_start fires, then pipe_squash, entry re-eligible
    // ================================================================
    printf("Test 12: Pipe squash\n");
    clear_inputs(d);
    submit_l2_read(d, 0x13000, 23);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Squash
        clear_inputs(d);
        d->pipe_squash_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Entry should re-become eligible
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 13: to_l3_mesh request with data (wdata path)
    //   L2 Write miss -> victim with data -> mesh wdata
    // ================================================================
    printf("Test 13: Mesh wdata path\n");
    clear_inputs(d);
    submit_l2_write(d, 0x14000, 24);
    sim.tick();
    compare_outputs(sim);

    {
        int wid = d->new_reqq_alloc_l2_id_o;

        // Dataq write status
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);

        // Tag miss -> goes to mesh
        clear_inputs(d);
        drive_tag_rsp(d, wid, false, OP_L2_WRITE);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh req (could be wdata or nodata)
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_wdata_o ||
                d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Mesh response
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = wid;
        d->to_l3_mesh_rsp_dataq_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        // Wait for re-pipe fill
        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 14: to_l3_mesh request without data (nodata path)
    //   L2 Read miss -> mesh nodata
    // ================================================================
    printf("Test 14: Mesh nodata path\n");
    clear_inputs(d);
    submit_l2_read(d, 0x15000, 25);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag miss
        clear_inputs(d);
        drive_tag_rsp(d, aid, false, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Should produce nodata mesh req (read miss has no data)
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Mesh response
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        sim.tick();
        compare_outputs(sim);

        // Wait for re-pipe fill
        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 15: to_sys_mesh request
    //   L3 Read miss -> goes to sys mesh (L3 addresses use to_sys)
    // ================================================================
    printf("Test 15: to_sys_mesh request\n");
    clear_inputs(d);
    submit_l3_read(d, 0x16000, 26);
    sim.tick();
    compare_outputs(sim);

    {
        int l3id = d->new_reqq_alloc_l3_id_o;

        wait_for_pipe_start(sim);

        int picked = d->new_pipe_req_reqq_id_o;

        // Tag miss -> L3 address space goes to to_sys_mesh
        clear_inputs(d);
        drive_tag_rsp(d, picked, false, OP_L3_READ);
        sim.tick();
        compare_outputs(sim);

        // Should produce sys mesh req
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_sys_mesh_req_valid_nodata_o ||
                d->new_to_sys_mesh_req_valid_wdata_o) break;
        }

        // Sys mesh response
        clear_inputs(d);
        d->to_sys_mesh_rsp_valid_i = 1;
        d->to_sys_mesh_rsp_reqq_id_i = picked;
        d->to_sys_mesh_rsp_dataq_id_i = picked;
        sim.tick();
        compare_outputs(sim);

        // Wait for re-pipe
        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int p2 = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, p2, OP_L3_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 16: Mesh backpressure
    //   Hold mesh_req_ready=0 for several cycles
    // ================================================================
    printf("Test 16: Mesh backpressure\n");
    clear_inputs(d);
    submit_l2_read(d, 0x17000, 27);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag miss
        clear_inputs(d);
        drive_tag_rsp(d, aid, false, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Hold mesh_req_ready=0 for 10 cycles
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            d->reqq_to_l3_mesh_req_ready_nodata_i = 0;
            d->reqq_to_l3_mesh_req_ready_wdata_i = 0;
            d->reqq_to_sys_mesh_req_ready_nodata_i = 0;
            d->reqq_to_sys_mesh_req_ready_wdata_i = 0;
            sim.tick();
            compare_outputs(sim);
        }

        // Release mesh_req_ready
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
        }

        // Mesh response
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 17: rspmux_l2 nodata path
    //   L2 Read hit -> rspmux nodata (read has no wdata)
    // ================================================================
    printf("Test 17: rspmux_l2 nodata path\n");
    alloc_and_complete_l2_read(sim, 0x18000, 28);
    idle(sim, 3);

    // ================================================================
    // Test 18: rspmux_l2 wdata path
    //   L2 Write hit -> rspmux wdata
    // ================================================================
    printf("Test 18: rspmux_l2 wdata path\n");
    clear_inputs(d);
    submit_l2_write(d, 0x19000, 29);
    sim.tick();
    compare_outputs(sim);

    {
        int wid = d->new_reqq_alloc_l2_id_o;

        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);
        complete_entry_hit(sim, wid, OP_L2_WRITE);
    }
    idle(sim, 3);

    // ================================================================
    // Test 19: rspmux_l3 path
    //   L3 Read hit -> rspmux_l3 valid
    // ================================================================
    printf("Test 19: rspmux_l3 path\n");
    clear_inputs(d);
    submit_l3_read(d, 0x1A000, 30);
    sim.tick();
    compare_outputs(sim);

    {
        int l3id = d->new_reqq_alloc_l3_id_o;

        wait_for_pipe_start(sim);
        int picked = d->new_pipe_req_reqq_id_o;
        complete_entry_hit(sim, picked, OP_L3_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 20: Rspmux backpressure
    //   Hold rspmux_ready=0 for several cycles
    // ================================================================
    printf("Test 20: Rspmux backpressure\n");
    clear_inputs(d);
    submit_l2_read(d, 0x1B000, 31);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Hold rspmux_ready=0 for 10 cycles
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            d->reqq_rspmux_l2_ready_nodata_i = 0;
            d->reqq_rspmux_l2_ready_wdata_i = 0;
            d->reqq_rspmux_l3_ready_nodata_i = 0;
            d->reqq_rspmux_l3_ready_wdata_i = 0;
            sim.tick();
            compare_outputs(sim);
        }

        // Release and complete
        idle(sim, 5);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 21: Rbuf read request path
    //   Entry with rbuf_valid from alloc should trigger rbuf_rd_req_valid.
    //   Since rbuf_state is tied to 0, we test the compare path by
    //   driving a request and observing both modules handle rbuf identically.
    // ================================================================
    printf("Test 21: Rbuf read request path\n");
    {
        // Alloc read with rbuf_rd_req_ready deasserted
        clear_inputs(d);
        d->pipe_rbuf_rd_req_ready_i = 0;
        submit_l2_read(d, 0x1C000, 32);
        sim.tick();
        compare_outputs(sim);

        int aid = d->new_reqq_alloc_l2_id_o;

        // Keep rbuf_rd_req_ready=0 for a few cycles
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            d->pipe_rbuf_rd_req_ready_i = 0;
            sim.tick();
            compare_outputs(sim);
        }

        // Release rbuf_rd_req_ready
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
        }

        // Complete the entry
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 22: Tag victim insertion
    //   pipe_tag_rsp with victim flag set -> verify both modules handle
    //   the tag victim linked list insertion identically.
    // ================================================================
    printf("Test 22: Tag victim insertion\n");
    clear_inputs(d);
    submit_l2_read(d, 0x1D000, 33);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit with victim
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ, /*victim=*/true, /*silent=*/false, 0xDEAD0);
        sim.tick();
        compare_outputs(sim);

        // Data rsp pre + data rsp for normal
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Victim data response
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh victim req or rspmux
        for (int i = 0; i < 15; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_wdata_o ||
                d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Drive mesh rsp for victim
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        // Complete: wait for pipe_start or rspmux, then finish
        bool got_pipe = false;
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) { got_pipe = true; break; }
        }
        if (got_pipe) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_REQQ_DONE);
        } else {
            clear_inputs(d);
            drive_rspmux_sent(d, aid);
            sim.tick();
            compare_outputs(sim);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 23: Tag victim with silent flag
    // ================================================================
    printf("Test 23: Tag victim silent\n");
    clear_inputs(d);
    submit_l2_read(d, 0x1E000, 34);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit with silent victim (no mesh eviction needed)
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ, /*victim=*/true, /*silent=*/true, 0xCAFE0);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Silent victim -> victim data rsp still comes
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        // Should go to rspmux without mesh (silent victim)
        // Wait for pipe or rspmux
        bool got_pipe = false;
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) { got_pipe = true; break; }
        }
        if (got_pipe) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_REQQ_DONE);
        } else {
            clear_inputs(d);
            drive_rspmux_sent(d, aid);
            sim.tick();
            compare_outputs(sim);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 24: Dependency: two L2 reads to same address
    //   Second entry depends on first. The reqq_state is populated by
    //   the 64 entries so dependency SHOULD fire.
    // ================================================================
    printf("Test 24: Dependency (same address)\n");
    {
        uint32_t same_addr = 0x20000;

        // First read
        clear_inputs(d);
        submit_l2_read(d, same_addr, 35);
        sim.tick();
        compare_outputs(sim);
        int id1 = d->new_reqq_alloc_l2_id_o;

        // Second read to same address
        clear_inputs(d);
        submit_l2_read(d, same_addr, 36);
        sim.tick();
        compare_outputs(sim);
        int id2 = d->new_reqq_alloc_l2_id_o;

        // Complete first entry
        wait_for_pipe_start(sim);
        int picked = d->new_pipe_req_reqq_id_o;
        complete_entry_hit(sim, picked, OP_L2_READ);

        // Now second entry should become eligible
        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked2 = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked2, OP_L2_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 25: Data response modifier output
    //   pipe_data_rsp_pre -> pipe_data_rsp_modifier fields should match.
    //   Both modules receive the same data_rsp_pre, compare outputs.
    // ================================================================
    printf("Test 25: Data response modifier\n");
    clear_inputs(d);
    submit_l2_read(d, 0x21000, 37);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Data rsp pre -> triggers modifier output on both modules
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 26: Full reqq (alloc all 64 entries)
    //   Verify bank_l2_ready drops when all entries are allocated
    // ================================================================
    printf("Test 26: Full reqq (64 entries)\n");
    {
        int all_ids[64];
        for (int n = 0; n < 64; n++) {
            clear_inputs(d);
            submit_l2_read(d, 0x30000 + n * 0x100, 100 + n);
            sim.tick();
            compare_outputs(sim);
            all_ids[n] = d->new_reqq_alloc_l2_id_o;

            // Need to alternate with L3 to use both allocators
            // Actually both L2 and L3 share the same 64-entry pool.
            // Just keep allocating L2.
        }

        // At this point bank_l2_ready should be 0 -- both modules should agree
        idle(sim, 3);

        // Now drain them all
        for (int n = 0; n < 64; n++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) {
                // No more pipe_start, break (some entries may have finished)
                break;
            }
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_READ);
        }
    }
    idle(sim, 5);

    // ================================================================
    // Test 27: Idx CacheOp - multiple opcodes
    // ================================================================
    printf("Test 27: Idx CacheOp\n");
    {
        int idx_ops[] = {OP_IDX_READ, OP_IDX_WRITE, OP_IDX_ALL_INV, OP_IDX_L2_INV,
                         OP_IDX_L2_FLUSH, OP_IDX_L2_EVICT};
        for (int i = 0; i < 6; i++) {
            clear_inputs(d);
            d->pipe_idx_cop_req_valid_i = 1;
            d->pipe_idx_cop_req_opcode_i = idx_ops[i];
            d->pipe_idx_cop_req_address_i = 0x40000 + i * 0x100;
            d->pipe_idx_cop_req_trans_id_i = 200 + i;
            sim.tick();
            compare_outputs(sim);

            // Wait for pipe_start and complete
            wait_for_pipe_start(sim, 15);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                // Tag hit to complete
                clear_inputs(d);
                drive_tag_rsp(d, picked, true, idx_ops[i]);
                sim.tick();
                compare_outputs(sim);

                clear_inputs(d);
                drive_data_rsp_pre(d, picked, RSP_NORMAL);
                sim.tick();
                compare_outputs(sim);

                clear_inputs(d);
                drive_data_rsp(d, picked, RSP_NORMAL);
                sim.tick();
                compare_outputs(sim);

                idle(sim, 3);

                clear_inputs(d);
                drive_rspmux_sent(d, picked);
                sim.tick();
                compare_outputs(sim);

                // Idx cop aborted to release the FSM
                clear_inputs(d);
                d->pipe_idx_cop_aborted_i = 1;
                sim.tick();
                compare_outputs(sim);
            }

            idle(sim, 3);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 28: Idx stall: pipe_idx_cop_l2_busy blocks bank_l2
    // ================================================================
    printf("Test 28: Idx stall (pipe_idx_cop_l2_busy)\n");
    {
        // Assert l2_busy
        clear_inputs(d);
        d->pipe_idx_cop_l2_busy_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Try to submit L2 read -- should not be accepted (ready=0)
        clear_inputs(d);
        d->pipe_idx_cop_l2_busy_i = 1;
        submit_l2_read(d, 0x50000, 210);
        sim.tick();
        compare_outputs(sim);

        // Hold for a few cycles
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->pipe_idx_cop_l2_busy_i = 1;
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);

        // Now try to submit again -- should work
        clear_inputs(d);
        submit_l2_read(d, 0x50000, 211);
        sim.tick();
        compare_outputs(sim);

        int aid = d->new_reqq_alloc_l2_id_o;
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 29: Mbist stall: mbist_on blocks bank_l2
    // ================================================================
    printf("Test 29: Mbist stall\n");
    {
        // Assert mbist_on
        clear_inputs(d);
        d->mbist_on_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Try to submit L2 read
        clear_inputs(d);
        d->mbist_on_i = 1;
        submit_l2_read(d, 0x51000, 212);
        sim.tick();
        compare_outputs(sim);

        // Hold for a few cycles
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->mbist_on_i = 1;
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);

        // Now submit and complete
        clear_inputs(d);
        submit_l2_read(d, 0x51000, 213);
        sim.tick();
        compare_outputs(sim);

        int aid = d->new_reqq_alloc_l2_id_o;
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 30: Error path - various opcodes that trigger alloc errors
    //   Submit opcodes that may trigger err_valid in alloc_info.
    //   The key thing is both modules agree on the output.
    // ================================================================
    printf("Test 30: Error path opcodes\n");
    {
        // REQ_ATOMIC -- can trigger err if L2 bypass is on, but ESR is 0
        // so it should just allocate normally. We test to verify cosim agrees.
        int test_opcodes[] = {REQ_ATOMIC, REQ_FLUSH, REQ_EVICT,
                              REQ_WRITE_AROUND, REQ_PREFETCH};
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            submit_l2_op(d, 0x52000 + i * 0x100, test_opcodes[i], 220 + i);
            sim.tick();
            compare_outputs(sim);

            int aid = d->new_reqq_alloc_l2_id_o;

            // For write/write-around, need dataq write status
            if (test_opcodes[i] == REQ_WRITE || test_opcodes[i] == REQ_WRITE_AROUND) {
                clear_inputs(d);
                d->dataq_write_status_any_i = 1;
                d->dataq_write_status_hot_i = (uint64_t)1 << aid;
                d->dataq_write_status_id_i = aid;
                sim.tick();
                compare_outputs(sim);
            }

            // Wait for pipe_start and complete
            wait_for_pipe_start(sim, 15);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;

                clear_inputs(d);
                drive_tag_rsp(d, picked, true, d->new_pipe_req_opcode_o);
                sim.tick();
                compare_outputs(sim);

                clear_inputs(d);
                drive_data_rsp_pre(d, picked, RSP_NORMAL);
                sim.tick();
                compare_outputs(sim);

                clear_inputs(d);
                drive_data_rsp(d, picked, RSP_NORMAL);
                sim.tick();
                compare_outputs(sim);

                idle(sim, 3);

                clear_inputs(d);
                drive_rspmux_sent(d, picked);
                sim.tick();
                compare_outputs(sim);
            }
            idle(sim, 3);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 31: Sequential lifecycle x20
    // ================================================================
    printf("Test 31: Sequential lifecycle x20\n");
    for (int n = 0; n < 20; n++) {
        clear_inputs(d);
        submit_l2_read(d, 0x60000 + n * 0x100, 50 + n);
        sim.tick();
        compare_outputs(sim);

        int sid = d->new_reqq_alloc_l2_id_o;
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, sid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 32: Mixed L2 + L3 + Idx interleaved
    // ================================================================
    printf("Test 32: Mixed L2+L3+Idx interleaved\n");
    {
        int mix_ids[6];
        int mix_count = 0;

        // L2 Read
        clear_inputs(d);
        submit_l2_read(d, 0x70000, 300);
        sim.tick();
        compare_outputs(sim);
        mix_ids[mix_count++] = d->new_reqq_alloc_l2_id_o;

        // L3 Read
        clear_inputs(d);
        submit_l3_read(d, 0x70100, 301);
        sim.tick();
        compare_outputs(sim);
        mix_ids[mix_count++] = d->new_reqq_alloc_l3_id_o;

        // L2 Write
        clear_inputs(d);
        submit_l2_write(d, 0x70200, 302);
        sim.tick();
        compare_outputs(sim);
        int wid = d->new_reqq_alloc_l2_id_o;
        mix_ids[mix_count++] = wid;

        // Dataq write status for the write
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        // Idx CacheOp
        clear_inputs(d);
        d->pipe_idx_cop_req_valid_i = 1;
        d->pipe_idx_cop_req_opcode_i = OP_IDX_READ;
        d->pipe_idx_cop_req_address_i = 0x70300;
        d->pipe_idx_cop_req_trans_id_i = 303;
        sim.tick();
        compare_outputs(sim);

        // L3 Write
        clear_inputs(d);
        submit_l3_write(d, 0x70400, 304);
        sim.tick();
        compare_outputs(sim);
        int l3wid = d->new_reqq_alloc_l3_id_o;

        // Dataq for L3 write
        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << l3wid;
        d->dataq_write_status_id_i = l3wid;
        sim.tick();
        compare_outputs(sim);

        // L2 + L3 simultaneous
        clear_inputs(d);
        submit_l2_read(d, 0x70500, 305);
        submit_l3_read(d, 0x70600, 306);
        sim.tick();
        compare_outputs(sim);

        // Drain all entries
        for (int n = 0; n < 10; n++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) break;
            int picked = d->new_pipe_req_reqq_id_o;
            int op = d->new_pipe_req_opcode_o;
            complete_entry_hit(sim, picked, op);
        }

        // Abort idx cop to clean up
        clear_inputs(d);
        d->pipe_idx_cop_aborted_i = 1;
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 5);

    // ================================================================
    // Test 33: Random stimulus (500 cycles)
    // ================================================================
    printf("Test 33: Random stimulus (500 cycles)\n");
    srand(42);
    for (int i = 0; i < 500; i++) {
        clear_inputs(d);

        // Randomly submit L2 reads
        if ((rand() % 8) == 0) {
            submit_l2_read(d, (rand() & 0xFFF00), rand() & 0xFF);
        }

        // Randomly submit L2 writes
        if ((rand() % 16) == 0) {
            submit_l2_write(d, (rand() & 0xFFF00), rand() & 0xFF);
        }

        // Randomly submit L3 reads
        if ((rand() % 12) == 0) {
            submit_l3_read(d, (rand() & 0xFFF00), rand() & 0xFF);
        }

        // Randomly drive tag responses
        if ((rand() % 10) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            drive_tag_rsp(d, rid, (rand() & 1), OP_L2_READ);
        }

        // Randomly drive data_rsp_pre
        if ((rand() % 12) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            drive_data_rsp_pre(d, rid, RSP_NORMAL);
        }

        // Randomly drive data responses
        if ((rand() % 10) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            drive_data_rsp(d, rid, RSP_NORMAL);
        }

        // Randomly drive dataq_write_status
        if ((rand() % 15) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->dataq_write_status_any_i = 1;
            d->dataq_write_status_hot_i = (uint64_t)1 << rid;
            d->dataq_write_status_id_i = rid;
        }

        // Randomly drive l3 mesh rsp
        if ((rand() % 20) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->to_l3_mesh_rsp_valid_i = 1;
            d->to_l3_mesh_rsp_reqq_id_i = rid;
            d->to_l3_mesh_rsp_dataq_id_i = rid;
        }

        // Randomly drive sys mesh rsp
        if ((rand() % 30) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->to_sys_mesh_rsp_valid_i = 1;
            d->to_sys_mesh_rsp_reqq_id_i = rid;
            d->to_sys_mesh_rsp_dataq_id_i = rid;
        }

        // Randomly drive rspmux_sent
        if ((rand() % 10) == 0) {
            d->rspmux_sent_valid_hot_i = (uint64_t)1 << (rand() % 64);
        }

        // Randomly toggle mesh backpressure
        if ((rand() % 20) == 0) {
            d->reqq_to_l3_mesh_req_ready_nodata_i = rand() & 1;
            d->reqq_to_l3_mesh_req_ready_wdata_i = rand() & 1;
        }

        // Randomly toggle rspmux backpressure
        if ((rand() % 20) == 0) {
            d->reqq_rspmux_l2_ready_nodata_i = rand() & 1;
            d->reqq_rspmux_l2_ready_wdata_i = rand() & 1;
        }

        // Randomly squash
        if ((rand() % 50) == 0) {
            d->pipe_squash_i = 1;
        }

        // Randomly busy subbanks
        if ((rand() % 30) == 0) {
            d->pipe_busy_sub_bank_i = rand() & 0xF;
        }

        sim.tick();
        compare_outputs(sim);
    }

    // ================================================================
    // Test 34: L2 Read with various opcodes (Flush, Evict, Prefetch)
    // ================================================================
    printf("Test 34: L2 Flush/Evict/Prefetch\n");
    {
        int ops[] = {REQ_FLUSH, REQ_EVICT, REQ_PREFETCH};
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            submit_l2_op(d, 0x80000 + i * 0x100, ops[i], 400 + i);
            sim.tick();
            compare_outputs(sim);

            int aid = d->new_reqq_alloc_l2_id_o;

            wait_for_pipe_start(sim, 10);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                int op = d->new_pipe_req_opcode_o;
                complete_entry_hit(sim, picked, op);
            }
            idle(sim, 3);
        }
    }

    // ================================================================
    // Test 35: L3 Flush/Evict/Prefetch
    // ================================================================
    printf("Test 35: L3 Flush/Evict/Prefetch\n");
    {
        int ops[] = {REQ_FLUSH, REQ_EVICT, REQ_PREFETCH};
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            submit_l3_op(d, 0x81000 + i * 0x100, ops[i], 410 + i);
            sim.tick();
            compare_outputs(sim);

            int l3id = d->new_reqq_alloc_l3_id_o;

            wait_for_pipe_start(sim, 10);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                int op = d->new_pipe_req_opcode_o;
                complete_entry_hit(sim, picked, op);
            }
            idle(sim, 3);
        }
    }

    // ================================================================
    // Test 36: Cbuf response path
    // ================================================================
    printf("Test 36: Cbuf response\n");
    clear_inputs(d);
    submit_l2_read(d, 0x82000, 420);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Drive cbuf_rsp (coalesce buffer response)
        clear_inputs(d);
        d->pipe_cbuf_rsp_valid_i = 1;
        d->pipe_cbuf_rsp_reqq_id_i = aid;
        d->pipe_cbuf_rsp_victim_i = 0;
        sim.tick();
        compare_outputs(sim);

        // Data rsp
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 37: Cbuf response with victim
    // ================================================================
    printf("Test 37: Cbuf response with victim\n");
    clear_inputs(d);
    submit_l2_read(d, 0x83000, 421);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Cbuf rsp with victim
        clear_inputs(d);
        d->pipe_cbuf_rsp_valid_i = 1;
        d->pipe_cbuf_rsp_reqq_id_i = aid;
        d->pipe_cbuf_rsp_victim_i = 1;
        d->pipe_cbuf_rsp_victim_address_i = 0xFACE0;
        sim.tick();
        compare_outputs(sim);

        // Data rsp
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Victim data
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_VICTIM);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh or pipe
        for (int i = 0; i < 15; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_wdata_o ||
                d->new_to_l3_mesh_req_valid_nodata_o ||
                d->new_pipe_start_o) break;
        }

        // If mesh req, respond
        if (d->new_to_l3_mesh_req_valid_wdata_o ||
            d->new_to_l3_mesh_req_valid_nodata_o) {
            clear_inputs(d);
            d->to_l3_mesh_rsp_valid_i = 1;
            d->to_l3_mesh_rsp_reqq_id_i = aid;
            d->to_l3_mesh_rsp_dataq_id_i = aid;
            sim.tick();
            compare_outputs(sim);
        }

        // Complete
        for (int i = 0; i < 15; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                complete_entry_hit(sim, picked, OP_REQQ_DONE);
                break;
            }
        }

        // Rspmux sent if needed
        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 38: Mesh bypassed victim flag
    // ================================================================
    printf("Test 38: Mesh bypassed victim flag\n");
    clear_inputs(d);
    submit_l2_read(d, 0x84000, 422);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit with victim
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ, /*victim=*/true, /*silent=*/false, 0xBAAD0);
        d->mesh_bypassed_victim_i = 1; // bypass flag
        sim.tick();
        compare_outputs(sim);

        // Data rsp
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        // Complete
        bool got_pipe = false;
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) { got_pipe = true; break; }
        }
        if (got_pipe) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_REQQ_DONE);
        }
        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 39: Rspmux bypassed data rsp flag
    // ================================================================
    printf("Test 39: Rspmux bypassed data rsp\n");
    clear_inputs(d);
    submit_l2_read(d, 0x85000, 423);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Data rsp pre + rspmux_bypassed_data_rsp flag
        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        d->rspmux_bypassed_data_rsp_i = 1;
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 40: Error logger signals
    // ================================================================
    printf("Test 40: Error logger signals\n");
    clear_inputs(d);
    submit_l2_read(d, 0x86000, 424);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // Drive error signals
        clear_inputs(d);
        d->set_reqq_err_detected_hot_i = (uint64_t)1 << aid;
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        d->set_reqq_err_zero_data_hot_i = (uint64_t)1 << aid;
        sim.tick();
        compare_outputs(sim);

        // Complete the entry
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 41: Dataq read status
    // ================================================================
    printf("Test 41: Dataq read status\n");
    clear_inputs(d);
    submit_l2_read(d, 0x87000, 425);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // Drive dataq_read_status
        clear_inputs(d);
        d->dataq_read_status_hot_i = (uint64_t)1 << aid;
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 42: Tag rsp with ECC errors
    // ================================================================
    printf("Test 42: Tag rsp with ECC errors\n");
    clear_inputs(d);
    submit_l2_read(d, 0x88000, 426);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag hit with tag_state_ram_sbe
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        d->pipe_tag_rsp_tag_state_ram_sbe_i = 1;
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 43: Data rsp with data_ram ECC errors
    // ================================================================
    printf("Test 43: Data rsp with ECC errors\n");
    clear_inputs(d);
    submit_l2_read(d, 0x89000, 427);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        // Data rsp with sbe/dbe
        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        d->pipe_data_rsp_data_ram_sbe_i = 0x1;
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 44: Tag rsp with bubble
    // ================================================================
    printf("Test 44: Tag rsp with bubble\n");
    clear_inputs(d);
    submit_l2_read(d, 0x8A000, 428);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag bubble (tag rsp valid but bubble=1)
        clear_inputs(d);
        d->pipe_tag_rsp_valid_i = 1;
        d->pipe_tag_rsp_reqq_id_i = aid;
        d->pipe_tag_rsp_bubble_i = 1;
        d->pipe_tag_rsp_opcode_i = OP_L2_READ;
        sim.tick();
        compare_outputs(sim);

        // After bubble, entry should be re-eligible for pipe
        wait_for_pipe_start(sim, 10);

        // Now complete normally
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 45: Tag rsp err_rsp (non-zero error response)
    // ================================================================
    printf("Test 45: Tag rsp with err_rsp\n");
    clear_inputs(d);
    submit_l2_read(d, 0x8B000, 429);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag rsp with error
        clear_inputs(d);
        drive_tag_rsp(d, aid, true, OP_L2_READ);
        d->pipe_tag_rsp_err_rsp_i = 0x04; // PipeErrL2OpNonEn
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp_pre(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        clear_inputs(d);
        drive_data_rsp(d, aid, RSP_NORMAL);
        sim.tick();
        compare_outputs(sim);

        idle(sim, 5);

        clear_inputs(d);
        drive_rspmux_sent(d, aid);
        sim.tick();
        compare_outputs(sim);
    }
    idle(sim, 3);

    // ================================================================
    // Test 46: Pipe busy all subbanks
    // ================================================================
    printf("Test 46: Pipe busy all subbanks\n");
    clear_inputs(d);
    submit_l2_read(d, 0x8C000, 430);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // All subbanks busy
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            d->pipe_busy_sub_bank_i = 0xF; // all 4 busy
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 47: Pipe busy specific subbank while another gets picked
    // ================================================================
    printf("Test 47: Selective subbank busy\n");
    {
        // Alloc in sub-bank 0 and sub-bank 1
        clear_inputs(d);
        submit_l2_read(d, 0x90000, 431); // sub-bank 0 (bits [9:8] = 00)
        sim.tick();
        compare_outputs(sim);
        int id0 = d->new_reqq_alloc_l2_id_o;

        clear_inputs(d);
        submit_l2_read(d, 0x90100, 432); // sub-bank 1 (bits [9:8] = 01)
        sim.tick();
        compare_outputs(sim);
        int id1 = d->new_reqq_alloc_l2_id_o;

        // Block sub-bank 0, sub-bank 1 should get picked
        for (int i = 0; i < 5; i++) {
            clear_inputs(d);
            d->pipe_busy_sub_bank_i = 0x1; // sub-bank 0 busy only
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) break;
        }

        // Complete whatever got picked
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_READ);
        }

        // Release sub-bank 0 and complete the other
        wait_for_pipe_start(sim, 10);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 48: Double squash
    // ================================================================
    printf("Test 48: Double squash\n");
    clear_inputs(d);
    submit_l2_read(d, 0x91000, 433);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        // First squash
        wait_for_pipe_start(sim);
        clear_inputs(d);
        d->pipe_squash_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Second squash
        wait_for_pipe_start(sim);
        clear_inputs(d);
        d->pipe_squash_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Now complete
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 49: Idx L3 opcodes
    // ================================================================
    printf("Test 49: Idx L3 opcodes\n");
    {
        int l3_idx_ops[] = {OP_IDX_L3_INV, OP_IDX_L3_FLUSH, OP_IDX_L3_EVICT};
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->pipe_idx_cop_req_valid_i = 1;
            d->pipe_idx_cop_req_opcode_i = l3_idx_ops[i];
            d->pipe_idx_cop_req_address_i = 0x92000 + i * 0x100;
            d->pipe_idx_cop_req_trans_id_i = 440 + i;
            sim.tick();
            compare_outputs(sim);

            wait_for_pipe_start(sim, 15);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                complete_entry_hit(sim, picked, l3_idx_ops[i]);

                clear_inputs(d);
                d->pipe_idx_cop_aborted_i = 1;
                sim.tick();
                compare_outputs(sim);
            }
            idle(sim, 3);
        }
    }

    // ================================================================
    // Test 50: Idx cop l3_busy stall
    // ================================================================
    printf("Test 50: Idx cop l3_busy stall\n");
    {
        clear_inputs(d);
        d->pipe_idx_cop_l3_busy_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Try L3 submission while l3_busy
        clear_inputs(d);
        d->pipe_idx_cop_l3_busy_i = 1;
        submit_l3_read(d, 0x93000, 450);
        sim.tick();
        compare_outputs(sim);

        // Hold for a few cycles
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->pipe_idx_cop_l3_busy_i = 1;
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        clear_inputs(d);
        sim.tick();
        compare_outputs(sim);

        // Submit again
        clear_inputs(d);
        submit_l3_read(d, 0x93000, 451);
        sim.tick();
        compare_outputs(sim);

        int l3id = d->new_reqq_alloc_l3_id_o;
        wait_for_pipe_start(sim);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L3_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 51: Dataq bank ready deasserted
    // ================================================================
    printf("Test 51: Dataq bank ready deasserted\n");
    {
        clear_inputs(d);
        d->dataq_bank_l2_ready_i = 0;
        submit_l2_read(d, 0x94000, 460);
        sim.tick();
        compare_outputs(sim);

        // Hold dataq not ready
        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->dataq_bank_l2_ready_i = 0;
            sim.tick();
            compare_outputs(sim);
        }

        // Release
        clear_inputs(d);
        submit_l2_read(d, 0x94000, 461);
        sim.tick();
        compare_outputs(sim);

        int aid = d->new_reqq_alloc_l2_id_o;
        wait_for_pipe_start(sim);
        complete_entry_hit(sim, aid, OP_L2_READ);
    }
    idle(sim, 3);

    // ================================================================
    // Test 52: L3 dataq bank ready deasserted
    // ================================================================
    printf("Test 52: L3 dataq bank ready deasserted\n");
    {
        clear_inputs(d);
        d->dataq_bank_l3_ready_i = 0;
        submit_l3_read(d, 0x95000, 470);
        sim.tick();
        compare_outputs(sim);

        for (int i = 0; i < 3; i++) {
            clear_inputs(d);
            d->dataq_bank_l3_ready_i = 0;
            sim.tick();
            compare_outputs(sim);
        }

        clear_inputs(d);
        submit_l3_read(d, 0x95000, 471);
        sim.tick();
        compare_outputs(sim);

        int l3id = d->new_reqq_alloc_l3_id_o;
        wait_for_pipe_start(sim);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L3_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 53: L2 Write partial
    // ================================================================
    printf("Test 53: L2 Write partial\n");
    clear_inputs(d);
    submit_l2_write(d, 0x96000, 480, /*partial=*/true);
    sim.tick();
    compare_outputs(sim);

    {
        int wid = d->new_reqq_alloc_l2_id_o;

        clear_inputs(d);
        d->dataq_write_status_any_i = 1;
        d->dataq_write_status_hot_i = (uint64_t)1 << wid;
        d->dataq_write_status_id_i = wid;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim);
        complete_entry_hit(sim, wid, OP_L2_WRITE_P);
    }
    idle(sim, 3);

    // ================================================================
    // Test 54: Mesh response with error
    // ================================================================
    printf("Test 54: Mesh response with error\n");
    clear_inputs(d);
    submit_l2_read(d, 0x97000, 490);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag miss
        clear_inputs(d);
        drive_tag_rsp(d, aid, false, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        // Wait for mesh req
        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Mesh response with error
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        d->to_l3_mesh_rsp_err_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Wait for re-pipe
        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 55: Mesh response with zero_data
    // ================================================================
    printf("Test 55: Mesh response with zero_data\n");
    clear_inputs(d);
    submit_l2_read(d, 0x98000, 491);
    sim.tick();
    compare_outputs(sim);

    {
        int aid = d->new_reqq_alloc_l2_id_o;

        wait_for_pipe_start(sim);

        // Tag miss
        clear_inputs(d);
        drive_tag_rsp(d, aid, false, OP_L2_READ);
        sim.tick();
        compare_outputs(sim);

        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_l3_mesh_req_valid_nodata_o) break;
        }

        // Mesh response with zero_data
        clear_inputs(d);
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_reqq_id_i = aid;
        d->to_l3_mesh_rsp_dataq_id_i = aid;
        d->to_l3_mesh_rsp_zero_data_i = 1;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 56: Sys mesh response with error
    // ================================================================
    printf("Test 56: Sys mesh response with error\n");
    clear_inputs(d);
    submit_l3_read(d, 0x99000, 492);
    sim.tick();
    compare_outputs(sim);

    {
        int l3id = d->new_reqq_alloc_l3_id_o;

        wait_for_pipe_start(sim);
        int picked = d->new_pipe_req_reqq_id_o;

        // Tag miss
        clear_inputs(d);
        drive_tag_rsp(d, picked, false, OP_L3_READ);
        sim.tick();
        compare_outputs(sim);

        for (int i = 0; i < 10; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_to_sys_mesh_req_valid_nodata_o) break;
        }

        // Sys mesh response with error
        clear_inputs(d);
        d->to_sys_mesh_rsp_valid_i = 1;
        d->to_sys_mesh_rsp_reqq_id_i = picked;
        d->to_sys_mesh_rsp_dataq_id_i = picked;
        d->to_sys_mesh_rsp_err_i = 1;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int p2 = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, p2, OP_L3_FILL);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 57: Idx cop aborted
    // ================================================================
    printf("Test 57: Idx cop aborted\n");
    {
        clear_inputs(d);
        d->pipe_idx_cop_req_valid_i = 1;
        d->pipe_idx_cop_req_opcode_i = OP_IDX_READ;
        d->pipe_idx_cop_req_address_i = 0x9A000;
        d->pipe_idx_cop_req_trans_id_i = 500;
        sim.tick();
        compare_outputs(sim);

        idle(sim, 3);

        // Abort before completion
        clear_inputs(d);
        d->pipe_idx_cop_aborted_i = 1;
        sim.tick();
        compare_outputs(sim);

        // Drain if entry was allocated
        for (int i = 0; i < 15; i++) {
            clear_inputs(d);
            sim.tick();
            compare_outputs(sim);
            if (d->new_pipe_start_o) {
                int picked = d->new_pipe_req_reqq_id_o;
                complete_entry_hit(sim, picked, OP_IDX_READ);
                break;
            }
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 58: Idx cop with sync_flush_victims
    // ================================================================
    printf("Test 58: Idx cop sync_flush_victims\n");
    {
        clear_inputs(d);
        d->pipe_idx_cop_req_valid_i = 1;
        d->pipe_idx_cop_req_opcode_i = OP_IDX_L2_FLUSH;
        d->pipe_idx_cop_req_address_i = 0x9B000;
        d->pipe_idx_cop_req_trans_id_i = 501;
        d->pipe_idx_cop_req_sync_flush_victims_i = 1;
        sim.tick();
        compare_outputs(sim);

        wait_for_pipe_start(sim, 15);
        if (d->new_pipe_start_o) {
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_IDX_L2_FLUSH);

            clear_inputs(d);
            d->pipe_idx_cop_aborted_i = 1;
            sim.tick();
            compare_outputs(sim);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 59: Rapid alloc/dealloc churn
    // ================================================================
    printf("Test 59: Rapid alloc/dealloc churn\n");
    for (int n = 0; n < 10; n++) {
        // Alloc 4 entries rapidly
        int ids[4];
        for (int k = 0; k < 4; k++) {
            clear_inputs(d);
            submit_l2_read(d, 0xA0000 + n * 0x1000 + k * 0x100, 600 + n * 4 + k);
            sim.tick();
            compare_outputs(sim);
            ids[k] = d->new_reqq_alloc_l2_id_o;
        }

        // Drain all 4
        for (int k = 0; k < 4; k++) {
            wait_for_pipe_start(sim, 10);
            if (!d->new_pipe_start_o) break;
            int picked = d->new_pipe_req_reqq_id_o;
            complete_entry_hit(sim, picked, OP_L2_READ);
        }
    }
    idle(sim, 3);

    // ================================================================
    // Test 60: Extended random stress (500 more cycles)
    //   Includes L3, writes, idx, mesh rsp, more backpressure
    // ================================================================
    printf("Test 60: Extended random stress (500 cycles)\n");
    srand(12345);
    for (int i = 0; i < 500; i++) {
        clear_inputs(d);

        // L2 reads
        if ((rand() % 6) == 0) {
            submit_l2_read(d, (rand() & 0x3FF00), rand() & 0xFF);
        }

        // L2 writes
        if ((rand() % 12) == 0) {
            submit_l2_write(d, (rand() & 0x3FF00), rand() & 0xFF);
            // Sometimes also drive write status
            if ((rand() % 3) == 0) {
                int rid = rand() % 64;
                d->dataq_write_status_any_i = 1;
                d->dataq_write_status_hot_i = (uint64_t)1 << rid;
                d->dataq_write_status_id_i = rid;
            }
        }

        // L3 reads
        if ((rand() % 10) == 0) {
            submit_l3_read(d, (rand() & 0x3FF00), rand() & 0xFF);
        }

        // L3 writes
        if ((rand() % 20) == 0) {
            submit_l3_write(d, (rand() & 0x3FF00), rand() & 0xFF);
        }

        // Tag responses
        if ((rand() % 8) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            bool hit = (rand() % 3) != 0; // 2/3 hit
            bool victim = hit && ((rand() % 5) == 0);
            int op = OP_L2_READ;
            drive_tag_rsp(d, rid, hit, op, victim, /*silent=*/(rand() & 1), (rand() & 0xFFF00));
        }

        // Data rsp pre
        if ((rand() % 10) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            drive_data_rsp_pre(d, rid, (rand() % 3) == 0 ? RSP_VICTIM : RSP_NORMAL);
        }

        // Data rsp
        if ((rand() % 8) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            drive_data_rsp(d, rid, (rand() % 3) == 0 ? RSP_VICTIM : RSP_NORMAL);
        }

        // L3 mesh rsp
        if ((rand() % 15) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->to_l3_mesh_rsp_valid_i = 1;
            d->to_l3_mesh_rsp_reqq_id_i = rid;
            d->to_l3_mesh_rsp_dataq_id_i = rid;
            d->to_l3_mesh_rsp_err_i = (rand() % 10) == 0;
            d->to_l3_mesh_rsp_zero_data_i = (rand() % 10) == 0;
        }

        // Sys mesh rsp
        if ((rand() % 20) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->to_sys_mesh_rsp_valid_i = 1;
            d->to_sys_mesh_rsp_reqq_id_i = rid;
            d->to_sys_mesh_rsp_dataq_id_i = rid;
        }

        // Rspmux sent
        if ((rand() % 6) == 0) {
            d->rspmux_sent_valid_hot_i = (uint64_t)1 << (rand() % 64);
        }

        // Mesh backpressure
        if ((rand() % 15) == 0) {
            d->reqq_to_l3_mesh_req_ready_nodata_i = rand() & 1;
            d->reqq_to_l3_mesh_req_ready_wdata_i = rand() & 1;
            d->reqq_to_sys_mesh_req_ready_nodata_i = rand() & 1;
            d->reqq_to_sys_mesh_req_ready_wdata_i = rand() & 1;
        }

        // Rspmux backpressure
        if ((rand() % 15) == 0) {
            d->reqq_rspmux_l2_ready_nodata_i = rand() & 1;
            d->reqq_rspmux_l2_ready_wdata_i = rand() & 1;
            d->reqq_rspmux_l3_ready_nodata_i = rand() & 1;
            d->reqq_rspmux_l3_ready_wdata_i = rand() & 1;
        }

        // Squash
        if ((rand() % 40) == 0) {
            d->pipe_squash_i = 1;
        }

        // Subbank busy
        if ((rand() % 25) == 0) {
            d->pipe_busy_sub_bank_i = rand() & 0xF;
        }

        // Rbuf ready toggle
        if ((rand() % 30) == 0) {
            d->pipe_rbuf_rd_req_ready_i = rand() & 1;
        }

        // Cbuf rsp
        if ((rand() % 30) == 0 && d->new_a_reqq_entry_is_valid_o) {
            int rid = rand() % 64;
            d->pipe_cbuf_rsp_valid_i = 1;
            d->pipe_cbuf_rsp_reqq_id_i = rid;
            d->pipe_cbuf_rsp_victim_i = (rand() % 4) == 0;
            d->pipe_cbuf_rsp_victim_address_i = rand() & 0xFFF00;
        }

        sim.tick();
        compare_outputs(sim);
    }

    // ── Final drain ────────────────────────────────────────
    printf("Final drain\n");
    for (int i = 0; i < 30; i++) {
        clear_inputs(d);
        // Drive rspmux_sent for all entries to help drain
        if (i < 5) {
            d->rspmux_sent_valid_hot_i = 0xFFFFFFFFFFFFFFFFULL;
        }
        sim.tick();
        compare_outputs(sim);
    }

    return sim.finish();
}

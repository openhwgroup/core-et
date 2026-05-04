// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_reqq_entry vs shirecache_bank_reqq_entry
// Comprehensive test covering all ~68 FSM paths.

#include "Vcosim_reqq_entry_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_reqq_entry_tb;

// ═══════════════════════════════════════════════════════
// Opcode constants (reqq_opcode_e)
// ═══════════════════════════════════════════════════════
static constexpr uint8_t OP_L2_READ            = 0x00;
static constexpr uint8_t OP_L2_WRITE           = 0x01;
static constexpr uint8_t OP_L2_WRITE_PARTIAL   = 0x02;
static constexpr uint8_t OP_L2_WRITE_AROUND    = 0x04;
static constexpr uint8_t OP_L2_LOCK            = 0x05;
static constexpr uint8_t OP_L2_UNLOCK          = 0x06;
static constexpr uint8_t OP_L2_UNLOCK_INV      = 0x07;
static constexpr uint8_t OP_L2_FLUSH           = 0x08;
static constexpr uint8_t OP_L2_EVICT           = 0x09;
static constexpr uint8_t OP_L2_PREFETCH        = 0x0a;
static constexpr uint8_t OP_L2_ATOMIC          = 0x0b;
static constexpr uint8_t OP_L2_FILL            = 0x0d;
static constexpr uint8_t OP_L2_MSG_SEND_DATA   = 0x0f;
static constexpr uint8_t OP_L3_READ            = 0x10;
static constexpr uint8_t OP_L3_WRITE           = 0x11;
static constexpr uint8_t OP_L3_WRITE_PARTIAL   = 0x12;
static constexpr uint8_t OP_L3_FLUSH           = 0x14;
static constexpr uint8_t OP_L3_FLUSH_WDATA     = 0x15;
static constexpr uint8_t OP_L3_EVICT           = 0x16;
static constexpr uint8_t OP_L3_EVICT_WDATA     = 0x17;
static constexpr uint8_t OP_L3_PREFETCH        = 0x18;
static constexpr uint8_t OP_L3_ATOMIC          = 0x19;
static constexpr uint8_t OP_L3_FILL            = 0x1b;
static constexpr uint8_t OP_SCP_READ           = 0x1d;
static constexpr uint8_t OP_SCP_WRITE          = 0x1e;
static constexpr uint8_t OP_SCP_WRITE_PARTIAL  = 0x1f;
static constexpr uint8_t OP_SCP_ATOMIC         = 0x20;
static constexpr uint8_t OP_SCP_FILL           = 0x21;
static constexpr uint8_t OP_SCP_ZERO           = 0x23;
static constexpr uint8_t OP_IDX_ABORT          = 0x25;
static constexpr uint8_t OP_IDX_L2_FLUSH       = 0x2a;
static constexpr uint8_t OP_IDX_L2_EVICT       = 0x2b;
static constexpr uint8_t OP_MESH_READ          = 0x30;
static constexpr uint8_t OP_MESH_WRITE         = 0x31;
static constexpr uint8_t OP_MESH_VICTIM        = 0x37;
static constexpr uint8_t OP_MESH_ATOMIC_RSP    = 0x38;
static constexpr uint8_t OP_REQQ_WAIT          = 0x3a;
static constexpr uint8_t OP_REQQ_ALLOC_ERR     = 0x3c;
static constexpr uint8_t OP_REQQ_INITIAL       = 0x3d;
static constexpr uint8_t OP_REQQ_INITIAL2      = 0x3e;
static constexpr uint8_t OP_REQQ_DONE          = 0x3f;

// ═══════════════════════════════════════════════════════
// Original opcodes (reqq_orig_opcode_e)
// ═══════════════════════════════════════════════════════
static constexpr uint8_t ORIG_READ             = 0x00;
static constexpr uint8_t ORIG_WRITE            = 0x01;
static constexpr uint8_t ORIG_WRITE_AROUND     = 0x02;
static constexpr uint8_t ORIG_WRITE_PARTIAL    = 0x03;
static constexpr uint8_t ORIG_READ_COOP        = 0x04;
static constexpr uint8_t ORIG_MSG_SEND_DATA    = 0x05;
static constexpr uint8_t ORIG_ATOMIC           = 0x06;
static constexpr uint8_t ORIG_SCP_READ         = 0x07;
static constexpr uint8_t ORIG_SCP_WRITE        = 0x08;
static constexpr uint8_t ORIG_SCP_WRITE_PARTIAL= 0x09;
static constexpr uint8_t ORIG_SCP_READ_COOP    = 0x0a;
static constexpr uint8_t ORIG_IDX              = 0x0b;
static constexpr uint8_t ORIG_SYNC             = 0x0c;
static constexpr uint8_t ORIG_READ_FORWARD     = 0x0d;
static constexpr uint8_t ORIG_WRITE_FORWARD    = 0x0e;
static constexpr uint8_t ORIG_WRITE_FORWARD_PART = 0x0f;
static constexpr uint8_t ORIG_COP_LOCK         = 0x10;
static constexpr uint8_t ORIG_COP_UNLOCK       = 0x11;
static constexpr uint8_t ORIG_COP_UNLOCK_INV   = 0x12;
static constexpr uint8_t ORIG_COP_FLUSH        = 0x13;
static constexpr uint8_t ORIG_COP_EVICT        = 0x14;
static constexpr uint8_t ORIG_COP_FLUSH_WDATA  = 0x15;
static constexpr uint8_t ORIG_COP_EVICT_WDATA  = 0x16;
static constexpr uint8_t ORIG_COP_PREFETCH     = 0x17;
static constexpr uint8_t ORIG_COP_SCP_FILL     = 0x18;
static constexpr uint8_t ORIG_ATOMIC2          = 0x19;
static constexpr uint8_t ORIG_WRITE_AROUND2    = 0x1a;
static constexpr uint8_t ORIG_WRITE_PARTIAL2   = 0x1b;
static constexpr uint8_t ORIG_ERR              = 0x1c;

// Address spaces
static constexpr uint8_t ADDR_L2   = 0;
static constexpr uint8_t ADDR_L3   = 1;
static constexpr uint8_t ADDR_SCP  = 2;
static constexpr uint8_t ADDR_IDX  = 3;
static constexpr uint8_t ADDR_MSG  = 4;

// Reqq sources
static constexpr uint8_t SRC_L2    = 0;
static constexpr uint8_t SRC_L3    = 1;
static constexpr uint8_t SRC_FSM   = 2;

// Cache levels
static constexpr uint8_t CACHE_L1  = 0;
static constexpr uint8_t CACHE_L2  = 1;
static constexpr uint8_t CACHE_L3  = 2;
static constexpr uint8_t CACHE_MEM = 3;

// Data rsp types
static constexpr uint8_t NORMAL_RSP = 0;
static constexpr uint8_t VICTIM_RSP = 1;

// ═══════════════════════════════════════════════════════
// Helpers
// ═══════════════════════════════════════════════════════

static void clear_inputs(DUT* d) {
    d->my_reqq_id_i = 0;

    d->alloc_me_ag_i = 0;
    d->alloc_any_ag_i = 0;
    d->coalesce_victim_match_alloc_ag_i = 0;

    d->alloc_reqq_id_i = 0;
    d->alloc_trans_id_i = 0;
    d->alloc_tag_id_i = 0;
    d->alloc_source_i = 0;
    d->alloc_port_id_i = 0;
    d->alloc_qos_i = 0;
    d->alloc_address_space_i = 0;
    d->alloc_reqq_source_i = 0;
    d->alloc_cache_level_start_i = 0;
    d->alloc_cache_level_dest_i = 0;
    d->alloc_scp_is_remote_i = 0;
    d->alloc_readcoop_dest_i = 0;
    d->alloc_address_i = 0;
    d->alloc_qwen_i = 0;
    d->alloc_size_i = 0;
    d->alloc_orig_opcode_i = 0;
    d->alloc_opcode_i = 0;
    d->alloc_zero_data_i = 0;
    d->alloc_scp_fill_source_is_scp_i = 0;
    d->alloc_dep_valid_i = 0;
    d->alloc_dep_non_meshll_head_i = 0;
    d->alloc_dep_id_i = 0;
    d->alloc_paired_id_i = 0;
    d->alloc_rbuf_valid_i = 0;
    d->alloc_rbuf_pending_valid_i = 0;
    d->alloc_rbuf_id_i = 0;
    d->alloc_err_valid_i = 0;
    d->alloc_err_type_i = 0;
    d->alloc_err_imprecise_i = 0;
    d->alloc_traced_i = 0;

    d->alloc_me_ad_i = 0;

    d->clear_dep_hot_i = 0;
    d->clear_dep_rbuf_1hot_pre_i = 0;

    d->tag_victim_dep_valid_i = 0;
    d->tag_victim_dep_id_i = 0;
    d->tag_victim_dep_tail_i = 0;
    d->tag_victim_insert_behind_me_i = 0;
    d->tag_victim_insert_ahead_of_me_i = 0;
    d->tag_victim_upgrade_behind_me_i = 0;
    d->tag_victim_address_match_i = 0;
    d->upgrade_me_to_meshll_i = 0;
    d->upgrade_ahead_of_me_to_meshll_i = 0;

    d->coalesce_victim_me_i = 0;
    d->coalesce_victim_dep_valid_i = 0;
    d->coalesce_victim_dep_id_i = 0;
    d->coalesce_victim_dep_non_meshll_head_i = 0;
    d->coalesce_victim_insert_behind_me_i = 0;

    d->dataq_write_status_any_i = 0;
    d->dataq_write_status_me_i = 0;
    d->dataq_write_status_id_i = 0;
    d->dataq_read_status_me_i = 0;

    d->pipe_req_picked_me_i = 0;
    d->pipe_req_reqq_id_any_i = 0;
    d->pipe_req_picked_any_i = 0;

    d->pipe_tag_rsp_valid_any_i = 0;
    d->pipe_tag_rsp_valid_me_i = 0;
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

    d->pipe_data_rsp_valid_me_i = 0;
    d->pipe_data_rsp_trans_id_i = 0;
    d->pipe_data_rsp_reqq_id_i = 0;
    d->pipe_data_rsp_rsp_type_i = 0;
    d->pipe_data_rsp_tag_sbe_i = 0;
    for (int i = 0; i < 16; i++) d->pipe_data_rsp_data_i[i] = 0;
    d->pipe_data_rsp_qwen_i = 0;
    d->pipe_data_rsp_data_index_i = 0;
    d->pipe_data_rsp_data_ram_sbe_i = 0;
    d->pipe_data_rsp_data_ram_dbe_i = 0;

    d->pipe_cbuf_rsp_valid_any_i = 0;
    d->pipe_cbuf_rsp_trans_id_i = 0;
    d->pipe_cbuf_rsp_reqq_id_i = 0;
    d->pipe_cbuf_rsp_victim_i = 0;
    d->pipe_cbuf_rsp_victim_address_i = 0;

    d->rbuf_req_picked_me_i = 0;

    d->to_l3_mesh_req_picked_i = 0;
    d->to_l3_mesh_rsp_valid_me_i = 0;
    d->to_l3_mesh_rsp_valid_any_i = 0;
    d->to_l3_mesh_rsp_trans_id_i = 0;
    d->to_l3_mesh_rsp_reqq_id_i = 0;
    d->to_l3_mesh_rsp_dataq_id_i = 0;
    for (int i = 0; i < 16; i++) d->to_l3_mesh_rsp_data_i[i] = 0;
    d->to_l3_mesh_rsp_zero_data_i = 0;
    d->to_l3_mesh_rsp_err_i = 0;

    d->to_sys_mesh_req_picked_i = 0;
    d->to_sys_mesh_rsp_valid_me_i = 0;
    d->to_sys_mesh_rsp_valid_any_i = 0;
    d->to_sys_mesh_rsp_trans_id_i = 0;
    d->to_sys_mesh_rsp_reqq_id_i = 0;
    d->to_sys_mesh_rsp_dataq_id_i = 0;
    for (int i = 0; i < 16; i++) d->to_sys_mesh_rsp_data_i[i] = 0;
    d->to_sys_mesh_rsp_zero_data_i = 0;
    d->to_sys_mesh_rsp_err_i = 0;

    d->mesh_bypassed_victim_i = 0;

    d->rspmux_l2_picked_i = 0;
    d->rspmux_l3_picked_i = 0;
    d->rspmux_bypassed_data_rsp_i = 0;
    d->rspmux_sent_valid_i = 0;

    d->reqq_2nd_pass_need_priority_i = 0;
    d->reqq_idx_cop_need_priority_i = 0;
    d->pipe_idx_cop_aborted_i = 0;

    d->set_reqq_err_detected_me_i = 0;
    d->set_reqq_err_zero_data_me_i = 0;

    d->esr_l2_bypass_i = 0;
    d->esr_l3_bypass_i = 0;
    d->esr_cbuf_enable_i = 0;
    d->esr_remote_l3_enable_i = 0;
}

// ═══════════════════════════════════════════════════════
// Comparison (all 11 output pairs)
// ═══════════════════════════════════════════════════════

static void compare(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    sim.compare("valid",          d->orig_reqq_entry_valid_o,          d->new_reqq_entry_valid_o);
    sim.compare("pipe_eligible",  d->orig_pipe_req_eligible_o,         d->new_pipe_req_eligible_o);
    sim.compare("rbuf_eligible",  d->orig_rbuf_req_eligible_o,         d->new_rbuf_req_eligible_o);
    sim.compare("l3_mesh_eligible", d->orig_to_l3_mesh_req_eligible_o, d->new_to_l3_mesh_req_eligible_o);
    sim.compare("sys_mesh_eligible", d->orig_to_sys_mesh_req_eligible_o, d->new_to_sys_mesh_req_eligible_o);
    sim.compare("rspmux_l2",     d->orig_rspmux_l2_eligible_o,        d->new_rspmux_l2_eligible_o);
    sim.compare("rspmux_l3",     d->orig_rspmux_l3_eligible_o,        d->new_rspmux_l3_eligible_o);
    sim.compare("clear_dep",     d->orig_clear_dep_behind_me_o,       d->new_clear_dep_behind_me_o);
    sim.compare("opcode",        d->orig_opcode_o,                    d->new_opcode_o);
    sim.compare("alloc",         d->orig_reqq_entry_alloc_o,          d->new_reqq_entry_alloc_o);
    sim.compare("dealloc",       d->orig_reqq_entry_dealloc_o,        d->new_reqq_entry_dealloc_o);
}

static void idle_tick(CosimCtrl<DUT>& sim) {
    clear_inputs(sim.dut.get());
    sim.tick();
    compare(sim);
}

// ═══════════════════════════════════════════════════════
// Allocation helper (full)
// ═══════════════════════════════════════════════════════

struct AllocCfg {
    uint8_t opcode;
    uint8_t orig_opcode;
    uint8_t addr_space;
    uint64_t address;
    uint8_t reqq_source;
    uint8_t cache_level_start;
    uint8_t cache_level_dest;
    uint8_t scp_is_remote;
    uint8_t paired_id;
    uint8_t err_valid;
    uint8_t err_type;
    uint8_t dep_valid;
    uint8_t dep_id;
    uint8_t qwen;
    uint8_t size;
    uint8_t zero_data;
};

static AllocCfg default_alloc(uint8_t opcode, uint8_t orig_opcode,
                               uint8_t addr_space, uint64_t address) {
    AllocCfg c{};
    c.opcode = opcode;
    c.orig_opcode = orig_opcode;
    c.addr_space = addr_space;
    c.address = address;
    c.reqq_source = SRC_L2;
    c.cache_level_start = CACHE_L2;
    c.cache_level_dest = CACHE_L2;
    c.scp_is_remote = 0;
    c.paired_id = 0;
    c.err_valid = 0;
    c.err_type = 0;
    c.dep_valid = 0;
    c.dep_id = 0;
    c.qwen = 0xF;
    c.size = 6; // line
    c.zero_data = 0;
    return c;
}

static void do_alloc(DUT* d, const AllocCfg& c) {
    d->alloc_me_ag_i = 1;
    d->alloc_any_ag_i = 1;
    d->alloc_reqq_id_i = d->my_reqq_id_i;
    d->alloc_trans_id_i = 0x100;
    d->alloc_tag_id_i = 0x10;
    d->alloc_source_i = 1;
    d->alloc_port_id_i = 0;
    d->alloc_qos_i = 0;
    d->alloc_address_space_i = c.addr_space;
    d->alloc_reqq_source_i = c.reqq_source;
    d->alloc_cache_level_start_i = c.cache_level_start;
    d->alloc_cache_level_dest_i = c.cache_level_dest;
    d->alloc_scp_is_remote_i = c.scp_is_remote;
    d->alloc_readcoop_dest_i = 0;
    d->alloc_address_i = c.address;
    d->alloc_qwen_i = c.qwen;
    d->alloc_size_i = c.size;
    d->alloc_orig_opcode_i = c.orig_opcode;
    d->alloc_opcode_i = c.opcode;
    d->alloc_zero_data_i = c.zero_data;
    d->alloc_scp_fill_source_is_scp_i = 0;
    d->alloc_dep_valid_i = c.dep_valid;
    d->alloc_dep_non_meshll_head_i = 0;
    d->alloc_dep_id_i = c.dep_id;
    d->alloc_paired_id_i = c.paired_id;
    d->alloc_rbuf_valid_i = 0;
    d->alloc_rbuf_pending_valid_i = 0;
    d->alloc_rbuf_id_i = 0;
    d->alloc_err_valid_i = c.err_valid;
    d->alloc_err_type_i = c.err_type;
    d->alloc_err_imprecise_i = 0;
    d->alloc_traced_i = 0;
}

static void do_alloc_ad(DUT* d) {
    d->alloc_me_ad_i = 1;
}

// ═══════════════════════════════════════════════════════
// Tag/Data/Mesh response helpers
// ═══════════════════════════════════════════════════════

static void do_tag_rsp_hit(DUT* d, uint8_t rsp_opcode = OP_L2_READ) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 1;
    d->pipe_tag_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_victim_silent_i = 0;
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_tag_rsp_miss(DUT* d, uint8_t rsp_opcode = OP_L2_READ) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 1;
    d->pipe_tag_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_victim_silent_i = 0;
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_tag_rsp_hit_victim(DUT* d, uint8_t rsp_opcode = OP_L2_WRITE,
                                   uint8_t victim_addr_space = ADDR_L2) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 1;
    d->pipe_tag_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 1;
    d->pipe_tag_rsp_victim_i = 1;
    d->pipe_tag_rsp_victim_silent_i = 0;
    d->pipe_tag_rsp_victim_address_i = 0x2000;
    d->pipe_tag_rsp_victim_address_space_i = victim_addr_space;
    d->pipe_tag_rsp_victim_write_around_i = 0;
    d->pipe_tag_rsp_victim_qwen_i = 0xF;
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_tag_rsp_miss_victim(DUT* d, uint8_t rsp_opcode = OP_L2_FILL) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 1;
    d->pipe_tag_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_victim_i = 1;
    d->pipe_tag_rsp_victim_silent_i = 0;
    d->pipe_tag_rsp_victim_address_i = 0x3000;
    d->pipe_tag_rsp_victim_address_space_i = ADDR_L2;
    d->pipe_tag_rsp_victim_write_around_i = 0;
    d->pipe_tag_rsp_victim_qwen_i = 0xF;
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_tag_rsp_decode_err(DUT* d, uint8_t rsp_opcode = OP_L2_READ) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 1;
    d->pipe_tag_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_err_rsp_i = 1; // non-zero = decode error
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

// Tag rsp for paired entry (uses valid_any, reqq_id matches paired)
static void do_tag_rsp_any(DUT* d, uint8_t paired_id, bool hit, bool victim,
                            uint8_t rsp_opcode = OP_L2_WRITE_PARTIAL) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 0;  // not "me" - it's the paired entry
    d->pipe_tag_rsp_reqq_id_i = paired_id;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = hit ? 1 : 0;
    d->pipe_tag_rsp_victim_i = victim ? 1 : 0;
    d->pipe_tag_rsp_victim_silent_i = 0;
    if (victim) {
        d->pipe_tag_rsp_victim_address_i = 0x4000;
        d->pipe_tag_rsp_victim_qwen_i = 0xF;
    }
    d->pipe_tag_rsp_err_rsp_i = 0;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_tag_rsp_any_decode_err(DUT* d, uint8_t paired_id,
                                       uint8_t rsp_opcode = OP_L2_WRITE_PARTIAL) {
    d->pipe_tag_rsp_valid_any_i = 1;
    d->pipe_tag_rsp_valid_me_i = 0;
    d->pipe_tag_rsp_reqq_id_i = paired_id;
    d->pipe_tag_rsp_trans_id_i = 0x100;
    d->pipe_tag_rsp_hit_i = 0;
    d->pipe_tag_rsp_victim_i = 0;
    d->pipe_tag_rsp_err_rsp_i = 1;
    d->pipe_tag_rsp_opcode_i = rsp_opcode;
}

static void do_data_rsp(DUT* d, uint8_t rsp_type = NORMAL_RSP) {
    d->pipe_data_rsp_valid_me_i = 1;
    d->pipe_data_rsp_reqq_id_i = d->my_reqq_id_i;
    d->pipe_data_rsp_trans_id_i = 0x100;
    d->pipe_data_rsp_rsp_type_i = rsp_type;
    d->pipe_data_rsp_qwen_i = 0xF;
}

static void do_pipe_pick(DUT* d) {
    d->pipe_req_picked_me_i = 1;
    d->pipe_req_picked_any_i = 1;
    d->pipe_req_reqq_id_any_i = d->my_reqq_id_i;
}

static void do_pipe_pick_any(DUT* d, uint8_t reqq_id) {
    d->pipe_req_picked_any_i = 1;
    d->pipe_req_reqq_id_any_i = reqq_id;
}

static void do_rspmux_l2_pick(DUT* d) {
    d->rspmux_l2_picked_i = 1;
}

static void do_rspmux_l3_pick(DUT* d) {
    d->rspmux_l3_picked_i = 1;
}

static void do_rspmux_sent(DUT* d) {
    d->rspmux_sent_valid_i = 1;
}

// L3 mesh rsp (for L2 address space fills: to_l3_mesh)
static void do_l3_mesh_rsp(DUT* d) {
    d->to_l3_mesh_rsp_valid_me_i = 1;
    d->to_l3_mesh_rsp_valid_any_i = 1;
    d->to_l3_mesh_rsp_reqq_id_i = d->my_reqq_id_i;
    d->to_l3_mesh_rsp_dataq_id_i = d->my_reqq_id_i;
    d->to_l3_mesh_rsp_trans_id_i = 0x100;
    d->to_l3_mesh_rsp_zero_data_i = 0;
    d->to_l3_mesh_rsp_err_i = 0;
}

// L3 mesh rsp with paired reqq_id (for WritePartial2 / Atomic2 flow)
static void do_l3_mesh_rsp_any(DUT* d, uint8_t reqq_id) {
    d->to_l3_mesh_rsp_valid_me_i = 0;
    d->to_l3_mesh_rsp_valid_any_i = 1;
    d->to_l3_mesh_rsp_reqq_id_i = reqq_id;
    d->to_l3_mesh_rsp_dataq_id_i = reqq_id;
    d->to_l3_mesh_rsp_trans_id_i = 0x100;
    d->to_l3_mesh_rsp_zero_data_i = 0;
    d->to_l3_mesh_rsp_err_i = 0;
}

// Sys mesh rsp (for L3 address space fills: to_sys_mesh)
static void do_sys_mesh_rsp(DUT* d) {
    d->to_sys_mesh_rsp_valid_me_i = 1;
    d->to_sys_mesh_rsp_valid_any_i = 1;
    d->to_sys_mesh_rsp_reqq_id_i = d->my_reqq_id_i;
    d->to_sys_mesh_rsp_dataq_id_i = d->my_reqq_id_i;
    d->to_sys_mesh_rsp_trans_id_i = 0x100;
    d->to_sys_mesh_rsp_zero_data_i = 0;
    d->to_sys_mesh_rsp_err_i = 0;
}

// Sys mesh rsp with paired reqq_id
static void do_sys_mesh_rsp_any(DUT* d, uint8_t reqq_id) {
    d->to_sys_mesh_rsp_valid_me_i = 0;
    d->to_sys_mesh_rsp_valid_any_i = 1;
    d->to_sys_mesh_rsp_reqq_id_i = reqq_id;
    d->to_sys_mesh_rsp_dataq_id_i = reqq_id;
    d->to_sys_mesh_rsp_trans_id_i = 0x100;
    d->to_sys_mesh_rsp_zero_data_i = 0;
    d->to_sys_mesh_rsp_err_i = 0;
}

static void do_dataq_write(DUT* d) {
    d->dataq_write_status_any_i = 1;
    d->dataq_write_status_me_i = 1;
    d->dataq_write_status_id_i = 0;
}

static void do_dataq_read(DUT* d) {
    d->dataq_read_status_me_i = 1;
}

// ═══════════════════════════════════════════════════════
// Common flow sequences
// ═══════════════════════════════════════════════════════

// Standard alloc (ag + ad + settle)
static void alloc_entry(CosimCtrl<DUT>& sim, const AllocCfg& c) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    do_alloc(d, c);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_alloc_ad(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 2; i++) idle_tick(sim);
}

// Standard alloc with dataq write (for Write/Fill ops)
static void alloc_entry_dataq(CosimCtrl<DUT>& sim, const AllocCfg& c) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    do_alloc(d, c);
    do_dataq_write(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_alloc_ad(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 2; i++) idle_tick(sim);
}

// Pipe pick + tag hit + data normal rsp -> rspmux L2 -> sent -> dealloc
static void finish_l2_read_hit(CosimCtrl<DUT>& sim, uint8_t rsp_opcode = OP_L2_READ) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    do_pipe_pick(d);
    sim.tick(); compare(sim);

    clear_inputs(d);
    do_tag_rsp_hit(d, rsp_opcode);
    do_data_rsp(d, NORMAL_RSP);
    sim.tick(); compare(sim);

    for (int i = 0; i < 2; i++) idle_tick(sim);

    clear_inputs(d);
    do_rspmux_l2_pick(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_rspmux_sent(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 3; i++) idle_tick(sim);
}

// Finish with L2 rspmux (no data rsp needed, just pick + sent)
static void finish_l2_rspmux(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < 2; i++) idle_tick(sim);
    clear_inputs(d);
    do_rspmux_l2_pick(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_rspmux_sent(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 3; i++) idle_tick(sim);
}

static void finish_l3_rspmux(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < 2; i++) idle_tick(sim);
    clear_inputs(d);
    do_rspmux_l3_pick(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_rspmux_sent(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 3; i++) idle_tick(sim);
}

// Finish L2 rspmux + dataq read (for writes/fills that set wait_for_dataq)
static void finish_l2_rspmux_dataq(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < 2; i++) idle_tick(sim);
    clear_inputs(d);
    do_rspmux_l2_pick(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_rspmux_sent(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_dataq_read(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 3; i++) idle_tick(sim);
}

static void finish_l3_rspmux_dataq(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    for (int i = 0; i < 2; i++) idle_tick(sim);
    clear_inputs(d);
    do_rspmux_l3_pick(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_rspmux_sent(d);
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_dataq_read(d);
    sim.tick(); compare(sim);
    for (int i = 0; i < 3; i++) idle_tick(sim);
}

// Wait for dealloc only (for entries where rspmux_sent is set immediately)
static void wait_dealloc(CosimCtrl<DUT>& sim) {
    for (int i = 0; i < 5; i++) idle_tick(sim);
}

// L3 mesh pick + L3 mesh rsp
static void mesh_l3_roundtrip(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->to_l3_mesh_req_picked_i = 1;
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_l3_mesh_rsp(d);
    sim.tick(); compare(sim);
}

// Sys mesh pick + sys mesh rsp
static void mesh_sys_roundtrip(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    d->to_sys_mesh_req_picked_i = 1;
    sim.tick(); compare(sim);
    clear_inputs(d);
    do_sys_mesh_rsp(d);
    sim.tick(); compare(sim);
}

// ═══════════════════════════════════════════════════════
// Main
// ═══════════════════════════════════════════════════════

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 80000000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    for (int i = 0; i < 5; i++) idle_tick(sim);

    // ── Phase 0: Reset + idle ────────────────────────────
    printf("phase 0: reset + idle verify\n");
    for (int i = 0; i < 5; i++) idle_tick(sim);
    sim.check(d->new_reqq_entry_valid_o == 0, "valid=0 after reset");
    sim.check(d->orig_reqq_entry_valid_o == 0, "orig valid=0 after reset");

    // ══════════════════════════════════════════════════════
    // L2 Address Space (normal, no bypass)
    // ══════════════════════════════════════════════════════

    // ── Phase 1: L2 Read hit ─────────────────────────────
    printf("phase 1: L2 Read hit\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x1000);
        alloc_entry(sim, c);
        finish_l2_read_hit(sim, OP_L2_READ);
    }

    // ── Phase 2: L2 Read miss -> MeshRead -> L2Fill -> Done ──
    printf("phase 2: L2 Read miss -> mesh fill -> L2Fill\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x2000);
        alloc_entry(sim, c);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag miss
        clear_inputs(d); do_tag_rsp_miss(d, OP_L2_READ);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // mesh roundtrip
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // fill pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // fill tag rsp: no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_FILL);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 3: L2 Read miss -> MeshRead -> L2Fill + victim -> MeshVictim -> Done
    printf("phase 3: L2 Read miss -> fill with victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x3000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_miss(d, OP_L2_READ);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // fill pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // fill tag rsp: victim
        clear_inputs(d); do_tag_rsp_miss_victim(d, OP_L2_FILL);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshVictim roundtrip
        mesh_l3_roundtrip(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 4: L2 Read hit WriteAround -> MeshWrite -> MeshRead -> L2Fill -> Done
    printf("phase 4: L2 Read hit WriteAround\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x4000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit + victim (WriteAround eviction)
        clear_inputs(d); do_tag_rsp_hit_victim(d, OP_L2_READ);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshWrite for victim
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshRead for refetch
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L2Fill
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_FILL);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 5: L2 Read decode error
    printf("phase 5: L2 Read decode error\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x5000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_decode_err(d, OP_L2_READ);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 6: L2 ReadForward -> MeshRead -> mesh rsp -> Done
    printf("phase 6: L2 ReadForward\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ_FORWARD, ADDR_L2, 0x6000);
        alloc_entry(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        // ReadForward: no rspmux, just dealloc (rspmux_sent set at alloc_ad)
        wait_dealloc(sim);
    }

    // ── Phase 7: L2 ReadCoop (same as Read)
    printf("phase 7: L2 ReadCoop hit\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ_COOP, ADDR_L2, 0x7000);
        alloc_entry(sim, c);
        finish_l2_read_hit(sim, OP_L2_READ);
    }

    // ── Phase 8: L2 Write no victim -> Done
    printf("phase 8: L2 Write no victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x8000);
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_WRITE);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 9: L2 Write with victim -> MeshVictim -> Done
    printf("phase 9: L2 Write with victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x9000);
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit_victim(d, OP_L2_WRITE);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 10: L2 WritePartial paired (hit path)
    printf("phase 10: L2 WritePartial hit\n");
    {
        uint8_t paired = 5;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_PARTIAL, ADDR_L2, 0xA000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Drive alloc_any_ag_i to signal 2nd entry allocated
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit, no victim -> Done
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 11: L2 WritePartial miss (-> MeshRead -> Wait -> 2nd pass)
    printf("phase 11: L2 WritePartial miss\n");
    {
        uint8_t paired = 7;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_PARTIAL, ADDR_L2, 0xB000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag miss
        clear_inputs(d); do_tag_rsp_miss(d, OP_L2_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshRead
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Wait state: paired entry picked for fill
        clear_inputs(d); do_pipe_pick_any(d, paired);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Now in L2WritePartial2nd -> pipe pick me -> back to L2WritePartial
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 12: L2 WritePartial2 (2nd half pair)
    printf("phase 12: L2 WritePartial2\n");
    {
        uint8_t paired = 3;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_PARTIAL2, ADDR_L2, 0xC000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Paired entry gets tag hit (no victim) -> WritePartial2 goes to Done
        clear_inputs(d);
        do_tag_rsp_any(d, paired, true, false, OP_L2_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ── Phase 13: L2 WriteForward -> MeshWrite -> Done
    printf("phase 13: L2 WriteForward\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_FORWARD, ADDR_L2, 0xD000);
        alloc_entry_dataq(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 14: L2 WriteForwardPartial -> MeshWrite -> Done
    printf("phase 14: L2 WriteForwardPartial\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_FORWARD_PART, ADDR_L2, 0xE000);
        alloc_entry_dataq(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 15: L2 WriteAround (cbuf disabled) -> MeshWrite -> Done
    printf("phase 15: L2 WriteAround (no cbuf)\n");
    {
        uint8_t paired = 8;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_AROUND, ADDR_L2, 0xF000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // cbuf disabled -> goes to MeshWrite
        mesh_l3_roundtrip(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 16: L2 WriteAround2 (no cbuf -> immediate Done)
    printf("phase 16: L2 WriteAround2\n");
    {
        uint8_t paired = 9;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_AROUND2, ADDR_L2, 0x10000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // cbuf disabled -> immediate ReqqDone
        wait_dealloc(sim);
    }

    // ── Phase 17: L2 Atomic paired (hit path)
    printf("phase 17: L2 Atomic hit\n");
    {
        uint8_t paired = 10;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC, ADDR_L2, 0x11000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // data_rsp NormalRsp -> Done
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_L2_ATOMIC);
        do_data_rsp(d, NORMAL_RSP);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 18: L2 Atomic miss (-> MeshRead -> Wait -> 2nd pass)
    printf("phase 18: L2 Atomic miss\n");
    {
        uint8_t paired = 11;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC, ADDR_L2, 0x12000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag miss
        clear_inputs(d); do_tag_rsp_miss(d, OP_L2_ATOMIC);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshRead
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Wait: paired picked for fill
        clear_inputs(d); do_pipe_pick_any(d, paired);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L2Atomic2nd -> pipe pick me -> back to L2Atomic
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit + data_rsp NormalRsp
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_L2_ATOMIC);
        do_data_rsp(d, NORMAL_RSP);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 19: L2 Atomic2 (2nd half pair)
    printf("phase 19: L2 Atomic2\n");
    {
        uint8_t paired = 12;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC2, ADDR_L2, 0x13000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Paired entry gets tag hit (no victim) -> Atomic2 goes to Done
        clear_inputs(d);
        do_tag_rsp_any(d, paired, true, false, OP_L2_ATOMIC);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ── Phase 20: L2 Lock -> pipe -> tag rsp -> Done
    printf("phase 20: L2 Lock\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_LOCK, ADDR_L2, 0x14000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit, no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_LOCK);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 21: L2 Unlock -> pipe -> Done
    printf("phase 21: L2 Unlock\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_UNLOCK, ADDR_L2, 0x15000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_UNLOCK);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 22: L2 UnlockInv -> pipe -> Done
    printf("phase 22: L2 UnlockInv\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_UNLOCK_INV, ADDR_L2, 0x16000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_UNLOCK_INV);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 23: L2 Flush -> pipe -> tag hit no victim -> Done
    printf("phase 23: L2 Flush (no victim)\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_FLUSH, ADDR_L2, 0x17000);
        c.cache_level_start = CACHE_L2;
        c.cache_level_dest = CACHE_L2;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_FLUSH);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 24: L2 Evict -> pipe -> tag rsp no victim -> Done
    printf("phase 24: L2 Evict (no victim)\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_EVICT, ADDR_L2, 0x18000);
        c.cache_level_start = CACHE_L2;
        c.cache_level_dest = CACHE_L2;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit, no victim -> Done
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_L2_EVICT);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 25: L2 Prefetch hit -> Done
    printf("phase 25: L2 Prefetch hit\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_PREFETCH, ADDR_L2, 0x19000);
        c.cache_level_start = CACHE_L2;
        c.cache_level_dest = CACHE_L2;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // hit, no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_PREFETCH);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 26: L2 Prefetch miss -> MeshRead -> fill -> Done
    printf("phase 26: L2 Prefetch miss\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_PREFETCH, ADDR_L2, 0x1A000);
        c.cache_level_start = CACHE_L2;
        c.cache_level_dest = CACHE_L2;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // miss
        clear_inputs(d); do_tag_rsp_miss(d, OP_L2_PREFETCH);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshRead
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L2Fill
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_FILL);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 27: L2 MsgSendData -> data_ready -> rspmux -> Done
    printf("phase 27: L2 MsgSendData\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_MSG_SEND_DATA, ADDR_MSG, 0x1B000);
        alloc_entry_dataq(sim, c);
        for (int i = 0; i < 3; i++) idle_tick(sim);
        // rspmux l2 pick (data_ready set by dataq_write_status)
        clear_inputs(d); do_rspmux_l2_pick(d);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // rspmux sent -> Done -> dealloc
        clear_inputs(d); do_rspmux_sent(d);
        sim.tick(); compare(sim);
        for (int i = 0; i < 3; i++) idle_tick(sim);
    }

    // ══════════════════════════════════════════════════════
    // L3 Address Space
    // ══════════════════════════════════════════════════════

    // ── Phase 28: L3 Read hit -> Done
    printf("phase 28: L3 Read hit\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L3, 0x20000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_L3_READ);
        do_data_rsp(d, NORMAL_RSP);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 29: L3 Read miss -> MeshRead -> L3Fill -> Done
    printf("phase 29: L3 Read miss -> mesh fill\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L3, 0x21000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_miss(d, OP_L3_READ);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L3 goes to sys mesh
        mesh_sys_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L3Fill
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_FILL);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 30: L3 Read hit WriteAround -> MeshWrite -> MeshRead -> L3Fill -> Done
    printf("phase 30: L3 Read hit WriteAround\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L3, 0x22000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit_victim(d, OP_L3_READ, ADDR_L3);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim); // MeshWrite
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim); // MeshRead
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L3Fill
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_FILL);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 31: L3 Write no victim -> Done
    printf("phase 31: L3 Write no victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L3, 0x23000);
        c.reqq_source = SRC_L3;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_WRITE);
        sim.tick(); compare(sim);
        finish_l3_rspmux_dataq(sim);
    }

    // ── Phase 32: L3 Write with victim -> MeshVictim -> Done
    printf("phase 32: L3 Write with victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L3, 0x24000);
        c.reqq_source = SRC_L3;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit_victim(d, OP_L3_WRITE, ADDR_L3);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim);
        finish_l3_rspmux_dataq(sim);
    }

    // ── Phase 33: L3 WritePartial paired (hit)
    printf("phase 33: L3 WritePartial hit\n");
    {
        uint8_t paired = 14;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_PARTIAL, ADDR_L3, 0x25000);
        c.reqq_source = SRC_L3;
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // hit, no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        finish_l3_rspmux_dataq(sim);
    }

    // ── Phase 34: L3 WritePartial2
    printf("phase 34: L3 WritePartial2\n");
    {
        uint8_t paired = 15;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE_PARTIAL2, ADDR_L3, 0x26000);
        c.reqq_source = SRC_L3;
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Paired entry gets tag hit (no victim)
        clear_inputs(d);
        do_tag_rsp_any(d, paired, true, false, OP_L3_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ── Phase 35: L3 Flush -> pipe -> Done
    printf("phase 35: L3 Flush (no victim)\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_FLUSH, ADDR_L3, 0x27000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_FLUSH);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 36: L3 FlushWData miss -> MeshWrite -> Done
    printf("phase 36: L3 FlushWData miss\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_FLUSH_WDATA, ADDR_L3, 0x28000);
        c.reqq_source = SRC_L3;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // miss -> MeshWrite with set_data_ready
        clear_inputs(d); do_tag_rsp_miss(d, OP_L3_FLUSH_WDATA);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim);
        finish_l3_rspmux_dataq(sim);
    }

    // ── Phase 37: L3 Evict -> pipe -> Done
    printf("phase 37: L3 Evict (no victim)\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_EVICT, ADDR_L3, 0x29000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_EVICT);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 38: L3 EvictWData miss -> MeshWrite -> Done
    printf("phase 38: L3 EvictWData miss\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_EVICT_WDATA, ADDR_L3, 0x2A000);
        c.reqq_source = SRC_L3;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // miss -> MeshWrite with set_data_ready
        clear_inputs(d); do_tag_rsp_miss(d, OP_L3_EVICT_WDATA);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim);
        finish_l3_rspmux_dataq(sim);
    }

    // ── Phase 39: L3 Prefetch hit -> Done
    printf("phase 39: L3 Prefetch hit\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_PREFETCH, ADDR_L3, 0x2B000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_PREFETCH);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 40: L3 Prefetch miss -> MeshRead -> L3Fill -> Done
    printf("phase 40: L3 Prefetch miss\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_PREFETCH, ADDR_L3, 0x2C000);
        c.reqq_source = SRC_L3;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_miss(d, OP_L3_PREFETCH);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_sys_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // L3Fill
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_FILL);
        sim.tick(); compare(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 41: L3 Atomic paired (hit -> MeshAtomicRsp -> Done)
    printf("phase 41: L3 Atomic hit\n");
    {
        uint8_t paired = 16;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC, ADDR_L3, 0x2D000);
        c.reqq_source = SRC_L3;
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit, no victim -> MeshAtomicRsp
        clear_inputs(d); do_tag_rsp_hit(d, OP_L3_ATOMIC);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshAtomicRsp -> mesh rsp -> Done
        mesh_sys_roundtrip(sim);
        finish_l3_rspmux(sim);
    }

    // ── Phase 42: L3 Atomic2
    printf("phase 42: L3 Atomic2\n");
    {
        uint8_t paired = 17;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC2, ADDR_L3, 0x2E000);
        c.reqq_source = SRC_L3;
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Paired entry gets tag hit (no victim)
        clear_inputs(d);
        do_tag_rsp_any(d, paired, true, false, OP_L3_ATOMIC);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ══════════════════════════════════════════════════════
    // SCP Address Space
    // ══════════════════════════════════════════════════════

    // ── Phase 43: SCP ScpFill -> MeshRead -> pipe -> Done
    printf("phase 43: SCP ScpFill\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_COP_SCP_FILL, ADDR_SCP, 0x30000);
        alloc_entry(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshRead (to get fill data)
        mesh_l3_roundtrip(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // ScpFill pipe
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_SCP_FILL);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 44: SCP ScpWrite (local) -> pipe -> Done
    printf("phase 44: SCP ScpWrite local\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_SCP_WRITE, ADDR_SCP, 0x31000);
        c.scp_is_remote = 0;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_SCP_WRITE);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 45: SCP ScpWritePartial (local) -> pipe -> Done
    printf("phase 45: SCP ScpWritePartial local\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_SCP_WRITE_PARTIAL, ADDR_SCP, 0x32000);
        c.scp_is_remote = 0;
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_SCP_WRITE_PARTIAL);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 46: SCP ScpRead (local) -> pipe -> Done
    printf("phase 46: SCP ScpRead local\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_SCP_READ, ADDR_SCP, 0x33000);
        c.scp_is_remote = 0;
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // data_rsp -> Done
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_SCP_READ);
        do_data_rsp(d);
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 47: SCP ScpRead (remote) -> MeshRead -> Done
    printf("phase 47: SCP ScpRead remote\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_SCP_READ, ADDR_SCP, 0x34000);
        c.scp_is_remote = 1;
        alloc_entry(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        // ReadForward-like: no rspmux needed, just dealloc
        wait_dealloc(sim);
    }

    // ── Phase 48: SCP Atomic (paired flow -> ScpAtomic -> MeshAtomicRsp -> Done)
    printf("phase 48: SCP Atomic\n");
    {
        uint8_t paired = 18;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC, ADDR_SCP, 0x35000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Signal 2nd entry
        clear_inputs(d);
        d->alloc_any_ag_i = 1;
        d->alloc_reqq_id_i = paired;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // pipe pick
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag rsp -> MeshAtomicRsp
        clear_inputs(d); do_tag_rsp_hit(d, OP_SCP_ATOMIC);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // MeshAtomicRsp
        mesh_sys_roundtrip(sim);
        // rspmux was set at paired alloc time
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 49: SCP Atomic2
    printf("phase 49: SCP Atomic2\n");
    {
        uint8_t paired = 19;
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC2, ADDR_SCP, 0x36000);
        c.paired_id = paired;
        alloc_entry_dataq(sim, c);
        // Immediate Done
        wait_dealloc(sim);
    }

    // ══════════════════════════════════════════════════════
    // Idx / Msg / Sync
    // ══════════════════════════════════════════════════════

    // ── Phase 50: Idx CacheOp -> pipe -> tag rsp no victim -> Done
    printf("phase 50: Idx CacheOp no victim\n");
    {
        auto c = default_alloc(OP_IDX_L2_FLUSH, ORIG_IDX, ADDR_IDX, 0x40000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // no victim
        clear_inputs(d); do_tag_rsp_hit(d, OP_IDX_L2_FLUSH);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ── Phase 51: Idx CacheOp with victim -> MeshWrite -> Done
    printf("phase 51: Idx CacheOp with victim\n");
    {
        auto c = default_alloc(OP_IDX_L2_EVICT, ORIG_IDX, ADDR_IDX, 0x41000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // victim
        clear_inputs(d); do_tag_rsp_hit_victim(d, OP_IDX_L2_EVICT);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        mesh_l3_roundtrip(sim);
        wait_dealloc(sim);
    }

    // ── Phase 52: Idx Abort
    printf("phase 52: Idx Abort\n");
    {
        auto c = default_alloc(OP_IDX_L2_FLUSH, ORIG_IDX, ADDR_IDX, 0x42000);
        alloc_entry(sim, c);
        // Drive abort while in pipe-eligible state, not picked and not inflight
        clear_inputs(d);
        d->pipe_idx_cop_aborted_i = 1;
        sim.tick(); compare(sim);
        // IdxAbort state: check if picked (it won't be)
        idle_tick(sim);
        // Not picked -> Done
        wait_dealloc(sim);
    }

    // ── Phase 53: Sync -> immediate Done
    printf("phase 53: Sync\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_SYNC, ADDR_IDX, 0x43000);
        // Sync transitions to Done at alloc_me_ad
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        do_alloc(d2, c);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        wait_dealloc(sim);
    }

    // ── Phase 54: MsgSendData (already tested in phase 27, skip duplicate)
    printf("phase 54: (MsgSendData - covered in phase 27)\n");

    // ══════════════════════════════════════════════════════
    // Bypass modes
    // ══════════════════════════════════════════════════════

    // ── Phase 55: L2 bypass Read -> MeshRead -> Done
    printf("phase 55: L2 bypass Read\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x50000);
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc(d2, c);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        // MeshRead -> mesh rsp -> Done
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        d2->to_l3_mesh_req_picked_i = 1;
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_l3_mesh_rsp(d2);
        sim.tick(); compare(sim);
        // dealloc (no rspmux needed)
        for (int i = 0; i < 5; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
    }

    // ── Phase 56: L2 bypass Write -> MeshWrite -> Done
    printf("phase 56: L2 bypass Write\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x51000);
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc(d2, c);
        do_dataq_write(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        // MeshWrite
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        d2->to_l3_mesh_req_picked_i = 1;
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_l3_mesh_rsp(d2);
        sim.tick(); compare(sim);
        // rspmux + dataq
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_rspmux_l2_pick(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_rspmux_sent(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_dataq_read(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 3; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
    }

    // ── Phase 57: L2 bypass Atomic -> immediate Done (no-op)
    printf("phase 57: L2 bypass Atomic (no-op)\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ATOMIC, ADDR_L2, 0x52000);
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc(d2, c);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        // rspmux_eligible set at alloc_ad
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_rspmux_l2_pick(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l2_bypass_i = 1;
        do_rspmux_sent(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 3; i++) {
            clear_inputs(d2);
            d2->esr_l2_bypass_i = 1;
            sim.tick(); compare(sim);
        }
    }

    // ── Phase 58: L3 bypass Read -> MeshRead -> Done
    printf("phase 58: L3 bypass Read\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L3, 0x53000);
        c.reqq_source = SRC_L3;
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_alloc(d2, c);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l3_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        // MeshRead via sys mesh
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        d2->to_sys_mesh_req_picked_i = 1;
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_sys_mesh_rsp(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 5; i++) {
            clear_inputs(d2);
            d2->esr_l3_bypass_i = 1;
            sim.tick(); compare(sim);
        }
    }

    // ── Phase 59: L3 bypass Write -> MeshWrite -> Done
    printf("phase 59: L3 bypass Write\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L3, 0x54000);
        c.reqq_source = SRC_L3;
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_alloc(d2, c);
        do_dataq_write(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l3_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        // MeshWrite via sys mesh
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        d2->to_sys_mesh_req_picked_i = 1;
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_sys_mesh_rsp(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) {
            clear_inputs(d2);
            d2->esr_l3_bypass_i = 1;
            sim.tick(); compare(sim);
        }
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_rspmux_l3_pick(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_rspmux_sent(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        d2->esr_l3_bypass_i = 1;
        do_dataq_read(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 3; i++) {
            clear_inputs(d2);
            d2->esr_l3_bypass_i = 1;
            sim.tick(); compare(sim);
        }
    }

    // ══════════════════════════════════════════════════════
    // Control paths
    // ══════════════════════════════════════════════════════

    // ── Phase 60: AllocErr -> Done
    printf("phase 60: AllocErr\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x60000);
        c.err_valid = 1;
        c.err_type = 1;  // some error
        alloc_entry(sim, c);
        // Should go to ReqqAllocErr -> ReqqDone
        for (int i = 0; i < 2; i++) idle_tick(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 61: OrigErr -> Done
    printf("phase 61: OrigErr\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_ERR, ADDR_L2, 0x61000);
        alloc_entry(sim, c);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 62: Dependency stall (dep_valid -> clear_dep_hot -> eligible)
    printf("phase 62: Dependency stall\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x62000);
        c.dep_valid = 1;
        c.dep_id = 5;
        alloc_entry(sim, c);
        // Should NOT be pipe eligible
        for (int i = 0; i < 3; i++) idle_tick(sim);
        // Clear dep
        clear_inputs(d);
        d->clear_dep_hot_i = (uint64_t)1 << 5;
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Now complete
        finish_l2_read_hit(sim, OP_L2_READ);
    }

    // ── Phase 63: Pipeline bypass victim (mesh_bypassed_victim)
    printf("phase 63: Pipeline bypass victim\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x63000);
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit with victim + mesh_bypassed_victim
        clear_inputs(d);
        do_tag_rsp_hit_victim(d, OP_L2_WRITE);
        d->mesh_bypassed_victim_i = 1;
        sim.tick(); compare(sim);
        // Victim was bypassed, so mesh rsp comes immediately
        for (int i = 0; i < 2; i++) idle_tick(sim);
        clear_inputs(d);
        do_l3_mesh_rsp(d);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 64: Rspmux bypass (rspmux_bypassed_data_rsp)
    printf("phase 64: Rspmux bypass\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x64000);
        alloc_entry(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        // tag hit + data rsp + rspmux bypass
        clear_inputs(d);
        do_tag_rsp_hit(d, OP_L2_READ);
        do_data_rsp(d, NORMAL_RSP);
        d->rspmux_bypassed_data_rsp_i = 1;
        sim.tick(); compare(sim);
        finish_l2_rspmux(sim);
    }

    // ── Phase 65: Dataq write status -> data_ready
    printf("phase 65: Dataq write status\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x65000);
        // Alloc without immediate dataq write
        auto* d2 = sim.dut.get();
        clear_inputs(d2);
        do_alloc(d2, c);
        sim.tick(); compare(sim);
        clear_inputs(d2);
        do_alloc_ad(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Drive dataq write status later
        clear_inputs(d2);
        do_dataq_write(d2);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // Complete the write path
        clear_inputs(d2); do_pipe_pick(d2);
        sim.tick(); compare(sim);
        clear_inputs(d2); do_tag_rsp_hit(d2, OP_L2_WRITE);
        sim.tick(); compare(sim);
        finish_l2_rspmux_dataq(sim);
    }

    // ── Phase 66: Wait_for_dataq -> dataq_read_status -> dealloc
    printf("phase 66: Wait_for_dataq\n");
    {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_WRITE, ADDR_L2, 0x66000);
        alloc_entry_dataq(sim, c);
        clear_inputs(d); do_pipe_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_tag_rsp_hit(d, OP_L2_WRITE);
        sim.tick(); compare(sim);
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // rspmux
        clear_inputs(d); do_rspmux_l2_pick(d);
        sim.tick(); compare(sim);
        clear_inputs(d); do_rspmux_sent(d);
        sim.tick(); compare(sim);
        // Should not dealloc yet (wait_for_dataq)
        for (int i = 0; i < 2; i++) idle_tick(sim);
        // dataq_read clears wait
        clear_inputs(d); do_dataq_read(d);
        sim.tick(); compare(sim);
        for (int i = 0; i < 3; i++) idle_tick(sim);
    }

    // ══════════════════════════════════════════════════════
    // Stress tests
    // ══════════════════════════════════════════════════════

    // ── Phase 67: Rapid alloc/dealloc sequence (5 entries back to back)
    printf("phase 67: Rapid alloc/dealloc (5 entries)\n");
    for (int seq = 0; seq < 5; seq++) {
        auto c = default_alloc(OP_REQQ_INITIAL, ORIG_READ, ADDR_L2, 0x70000 + seq * 0x1000);
        alloc_entry(sim, c);
        finish_l2_read_hit(sim, OP_L2_READ);
    }

    // ── Phase 68: Random stimulus (500 cycles)
    printf("phase 68: Random stimulus (500 cycles)\n");
    srand(42);
    for (int cyc = 0; cyc < 500; cyc++) {
        clear_inputs(d);
        // Low-probability random pulses
        if ((rand() & 0xF) == 0) d->alloc_me_ag_i = 1;
        if (d->alloc_me_ag_i) {
            d->alloc_any_ag_i = 1;
            d->alloc_opcode_i = OP_L2_READ;
            d->alloc_orig_opcode_i = ORIG_READ;
            d->alloc_address_i = rand() & 0xFFFFF;
            d->alloc_qwen_i = 0xF;
            d->alloc_size_i = 6;
            d->alloc_reqq_id_i = 0;
            d->alloc_trans_id_i = rand() & 0xFF;
        }
        if ((rand() & 0x7) == 0) {
            d->pipe_req_picked_me_i = 1;
            d->pipe_req_picked_any_i = 1;
            d->pipe_req_reqq_id_any_i = 0;
        }
        if ((rand() & 0x7) == 0) {
            d->pipe_tag_rsp_valid_any_i = 1;
            d->pipe_tag_rsp_valid_me_i = 1;
            d->pipe_tag_rsp_hit_i = rand() & 1;
            d->pipe_tag_rsp_err_rsp_i = 0;
            d->pipe_tag_rsp_reqq_id_i = 0;
            d->pipe_tag_rsp_opcode_i = OP_L2_READ;
        }
        if ((rand() & 0x7) == 0) {
            d->pipe_data_rsp_valid_me_i = 1;
            d->pipe_data_rsp_reqq_id_i = 0;
            d->pipe_data_rsp_qwen_i = 0xF;
        }
        if ((rand() & 0xF) == 0) {
            d->rspmux_l2_picked_i = 1;
        }
        if ((rand() & 0xF) == 0) {
            d->rspmux_sent_valid_i = 1;
        }
        if ((rand() & 0xF) == 0) {
            d->alloc_me_ad_i = 1;
        }
        if ((rand() & 0xF) == 0) {
            d->to_l3_mesh_rsp_valid_me_i = 1;
            d->to_l3_mesh_rsp_valid_any_i = 1;
            d->to_l3_mesh_rsp_reqq_id_i = 0;
        }
        if ((rand() & 0xF) == 0) {
            d->to_sys_mesh_rsp_valid_me_i = 1;
            d->to_sys_mesh_rsp_valid_any_i = 1;
            d->to_sys_mesh_rsp_reqq_id_i = 0;
        }
        if ((rand() & 0xF) == 0) {
            d->to_l3_mesh_req_picked_i = 1;
        }
        if ((rand() & 0xF) == 0) {
            d->to_sys_mesh_req_picked_i = 1;
        }
        if ((rand() & 0x1F) == 0) {
            d->dataq_write_status_any_i = 1;
            d->dataq_write_status_me_i = 1;
        }
        if ((rand() & 0x1F) == 0) {
            d->dataq_read_status_me_i = 1;
        }
        sim.tick();
        compare(sim);
    }
    printf("  phase 68 done\n");

    return sim.finish();
}

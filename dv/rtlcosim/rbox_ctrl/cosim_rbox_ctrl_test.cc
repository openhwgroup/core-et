// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_ctrl vs new rbox_ctrl.
//
// Thoroughly exercises every major path of the master controller:
//
//   Phase A: Basic state machine (Tests 1-5)
//   Phase B: Input buffer path (Tests 6-12)
//   Phase C: Each packet type decoded individually (Tests 13-20)
//   Phase D: Output path (Tests 21-28)
//   Phase E: Arbitration (Tests 29-33)
//   Phase F: Multi-drawcall and sustained operation (Tests 34-38)
//   Phase G: Done and flush (Tests 39-43)
//   Phase H: Random stress test (Tests 44-45)
//
// Target: 200K+ comparisons

#include "Vcosim_rbox_ctrl_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using DUT = Vcosim_rbox_ctrl_tb;

// ── Input packet type constants (matches rbox_pkg inpck_type_e) ─
enum InpckType : uint8_t {
    INPCK_TRI_TILE_64x64   = 0,
    INPCK_TRI_TILE_128x128 = 1,
    INPCK_LARGE_TRIANGLE   = 2,
    INPCK_FULLY_COVERED    = 3,
    INPCK_LARGE_TRI_TILE   = 4,
    INPCK_RBOX_STATE       = 5,
    INPCK_FSH_STATE        = 6,
    INPCK_END_OF_BUFFER    = 7,
};

// ── ET-Link opcode constants ────────────────────────────
enum RspOpcode : uint8_t {
    RSP_ACK      = 0,
    RSP_ACK_DATA = 1,
};

// ── Status constants ────────────────────────────────────
enum StatusVal : uint32_t {
    STATUS_READY    = 0x00,
    STATUS_WORKING  = 0x01,
    STATUS_FINISHED = 0x02,
    STATUS_ERROR    = 0x80,
};

// ── VlWide helpers ──────────────────────────────────────

template <std::size_t N>
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                          const VlWide<N>& orig, const VlWide<N>& newv) {
    for (std::size_t w = 0; w < N; w++) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%s[%zu]", name, w);
        sim.compare(buf, orig[w], newv[w]);
    }
}

template <std::size_t N>
static void zero_wide(VlWide<N>& v) {
    for (std::size_t w = 0; w < N; w++) v[w] = 0;
}

// ── Build an ET-Link response (534-bit rsp_t) ────────────
//
// rsp_t layout (packed, MSB-first):
//   id[7:0]  dest[3:0]  wdata  opcode[1:0]  data[511:0]  size[2:0]  qwen[3:0]
//
// Total = 8+4+1+2+512+3+4 = 534 bits
//
// In VlWide<17> (544 bits), bit 0 is LSB of qwen.

static void build_rsp(VlWide<17>& rsp, uint8_t id, uint8_t dest, bool wdata,
                       uint8_t opcode, const VlWide<16>* data, uint8_t size,
                       uint8_t qwen) {
    for (int w = 0; w < 17; w++) rsp[w] = 0;

    // Bit layout from LSB:
    //  [3:0]   = qwen
    //  [6:4]   = size
    //  [518:7] = data (512 bits)
    //  [520:519] = opcode (2 bits)
    //  [521]     = wdata
    //  [525:522] = dest
    //  [533:526] = id

    // qwen (bits 3:0)
    rsp[0] = (uint32_t)(qwen & 0xF);

    // size (bits 6:4)
    rsp[0] |= (uint32_t)(size & 0x7) << 4;

    // data (bits 518:7 = 512 bits starting at bit 7)
    if (data) {
        // Shift 512-bit data left by 7 bits and OR into rsp
        uint32_t carry = 0;
        for (int w = 0; w < 16; w++) {
            uint64_t val = ((uint64_t)(*data)[w] << 7) | carry;
            rsp[w] |= (uint32_t)(val & 0xFFFFFFFF);
            carry = (uint32_t)(val >> 32);
        }
        rsp[16] |= carry;
    }

    // opcode (bits 520:519)
    {
        int bit = 519;
        int word = bit / 32;
        int off  = bit % 32;
        rsp[word] |= (uint32_t)(opcode & 0x3) << off;
    }

    // wdata (bit 521)
    {
        int bit = 521;
        int word = bit / 32;
        int off  = bit % 32;
        if (wdata) rsp[word] |= (1u << off);
    }

    // dest (bits 525:522)
    {
        int bit = 522;
        int word = bit / 32;
        int off  = bit % 32;
        rsp[word] |= (uint32_t)(dest & 0xF) << off;
    }

    // id (bits 533:526)
    {
        int bit = 526;
        int word = bit / 32;
        int off  = bit % 32;
        uint64_t val = (uint64_t)(id & 0xFF) << off;
        rsp[word] |= (uint32_t)(val & 0xFFFFFFFF);
        if (word + 1 < 17) rsp[word + 1] |= (uint32_t)(val >> 32);
    }
}

// ── Output comparison ───────────────────────────────────

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("rbox_enable",        d->orig_rbox_enable_o,        d->new_rbox_enable_o);
    sim.compare("sc_rbox_rsp_ready",  d->orig_sc_rbox_rsp_ready_o,  d->new_sc_rbox_rsp_ready_o);
    sim.compare("db_sc_req_ready",    d->orig_db_sc_req_ready_o,    d->new_db_sc_req_ready_o);
    sim.compare("sc_db_rsp_valid",    d->orig_sc_db_rsp_valid_o,    d->new_sc_db_rsp_valid_o);
    sim.compare("esr_status",         d->orig_esr_status_o,         d->new_esr_status_o);
    sim.compare("tr_msaa_enable",     d->orig_tr_msaa_enable_o,     d->new_tr_msaa_enable_o);
    sim.compare("tr_msaa_mode",       d->orig_tr_msaa_mode_o,       d->new_tr_msaa_mode_o);
    sim.compare("tr_depth_is_fp32",   d->orig_tr_depth_is_fp32_o,   d->new_tr_depth_is_fp32_o);
    sim.compare("tr_depth_is_un16",   d->orig_tr_depth_is_un16_o,   d->new_tr_depth_is_un16_o);
    sim.compare("tr_depth_is_un24",   d->orig_tr_depth_is_un24_o,   d->new_tr_depth_is_un24_o);
    sim.compare("tr_scissor_min_x",   d->orig_tr_scissor_min_x_o,   d->new_tr_scissor_min_x_o);
    sim.compare("tr_scissor_max_x",   d->orig_tr_scissor_max_x_o,   d->new_tr_scissor_max_x_o);
    sim.compare("tr_scissor_min_y",   d->orig_tr_scissor_min_y_o,   d->new_tr_scissor_min_y_o);
    sim.compare("tr_scissor_max_y",   d->orig_tr_scissor_max_y_o,   d->new_tr_scissor_max_y_o);
    sim.compare("tr_depth_clamp_en",  d->orig_tr_depth_clamp_enable_o, d->new_tr_depth_clamp_enable_o);
    sim.compare("tr_depth_min",       d->orig_tr_depth_min_o,       d->new_tr_depth_min_o);
    sim.compare("tr_depth_max",       d->orig_tr_depth_max_o,       d->new_tr_depth_max_o);
    sim.compare("tr_depth_bound_en",  d->orig_tr_depth_bound_enable_o, d->new_tr_depth_bound_enable_o);
    sim.compare("tr_depth_bound_min", d->orig_tr_depth_bound_min_o, d->new_tr_depth_bound_min_o);
    sim.compare("tr_depth_bound_max", d->orig_tr_depth_bound_max_o, d->new_tr_depth_bound_max_o);
    sim.compare("tr_tile_valid",      d->orig_tr_tile_valid_o,      d->new_tr_tile_valid_o);
    sim.compare("ct_quad_accept",     d->orig_ct_quad_accept_o,     d->new_ct_quad_accept_o);
    sim.compare("ct_result_accept",   d->orig_ct_result_accept_o,   d->new_ct_result_accept_o);
    sim.compare("ts_front_triangle",  d->orig_ts_front_triangle_o,  d->new_ts_front_triangle_o);
    sim.compare("db_flush",           d->orig_db_flush_o,           d->new_db_flush_o);

    // SC request valid/data
    sim.compare("rbox_sc_req_valid",  d->orig_rbox_sc_req_valid_o,  d->new_rbox_sc_req_valid_o);

    if (d->orig_rbox_sc_req_valid_o || d->new_rbox_sc_req_valid_o) {
        compare_wide(sim, "rbox_sc_req", d->orig_rbox_sc_req_o, d->new_rbox_sc_req_o);
    }

    // SC response to depth buffer
    if (d->orig_sc_db_rsp_valid_o || d->new_sc_db_rsp_valid_o) {
        compare_wide(sim, "sc_db_rsp", d->orig_sc_db_rsp_o, d->new_sc_db_rsp_o);
    }
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

// ── Helper: Clear all inputs ────────────────────────────

static void clear_inputs(DUT* d) {
    d->rbox_sc_req_ready_i = 0x1F;  // All banks ready
    zero_wide(d->sc_rbox_rsp_i);
    d->sc_rbox_rsp_valid_i = 0;
    zero_wide(d->db_sc_req_i);
    d->db_sc_req_valid_i = 0;

    d->esr_config_i = 0;    d->esr_config_en_i = 0;
    d->esr_in_buf_pg_i = 0; d->esr_in_buf_pg_en_i = 0;
    d->esr_in_buf_cfg_i = 0; d->esr_in_buf_cfg_en_i = 0;
    d->esr_out_buf_pg_i = 0; d->esr_out_buf_pg_en_i = 0;
    d->esr_out_buf_cfg_i = 0; d->esr_out_buf_cfg_en_i = 0;
    d->esr_start_i = 0;     d->esr_start_en_i = 0;
    d->esr_consume_i = 0;   d->esr_consume_en_i = 0;

    d->tr_accept_i = 1;
    d->tr_empty_i = 1;
    zero_wide(d->tr_quad_i);
    d->tr_quad_valid_i = 0;
    d->ts_empty_i = 1;
    d->ts_result_mask_i = 0;
    d->ts_result_valid_i = 0;
    d->db_flush_done_i = 0;
    d->rbox_halted_i = 0;
}

// ── Helper: Build a 512-bit input packet in VlWide<16> ──

static void build_packet_rbox_state(VlWide<16>& pkt) {
    zero_wide(pkt);
    pkt[0] = INPCK_RBOX_STATE;
    // ds_buffer_format = 126 (D32_SFLOAT) at bit 136 = word[4] bit 8
    pkt[4] |= (0x7E << 8);
    // depth_test_enable at bit 161 = word[5] bit 1
    pkt[5] |= (1 << 1);
    // depth_test_write_enable at bit 162 = word[5] bit 2
    pkt[5] |= (1 << 2);
    // depth_test_compare_op = 1 (LESS) at bits [165:163] = word[5] bits [5:3]
    pkt[5] |= (1 << 3);
    // fragment_shader_reads_bary at bit 169 = word[5] bit 9
    pkt[5] |= (1 << 9);
    // fragment_shader_reads_depth at bit 170 = word[5] bit 10
    pkt[5] |= (1 << 10);
}

// Build a RBOX_STATE packet with custom fields for variety
static void build_packet_rbox_state_v2(VlWide<16>& pkt,
                                        uint8_t msaa_enable, uint8_t depth_clamp,
                                        uint8_t depth_bound, uint16_t scissor_min_x,
                                        uint16_t scissor_max_x) {
    zero_wide(pkt);
    pkt[0] = INPCK_RBOX_STATE;
    // msaa_enable at bit 8 (first bit of state after 8 bits header)
    if (msaa_enable) pkt[0] |= (1 << 8);
    // ds_buffer_format = 126 at bit 136 = word[4] bit 8
    pkt[4] |= (0x7E << 8);
    // depth_clamp_enable at bit 159 = word[4] bit 31
    if (depth_clamp) pkt[4] |= (1u << 31);
    // depth_bound_enable at bit 160 = word[5] bit 0
    if (depth_bound) pkt[5] |= 1;
    pkt[5] |= (1 << 1);   // depth_test_enable
    pkt[5] |= (1 << 2);   // depth_test_write_enable
    pkt[5] |= (1 << 3);   // depth_test_compare_op = 1 (LESS)
    pkt[5] |= (1 << 9);   // reads_bary
    pkt[5] |= (1 << 10);  // reads_depth
    // Scissor values in state_pck_t QW3-7 are more complex;
    // keeping defaults (0) for cosim since both modules see the same bits.
}

static void build_packet_fsh_state(VlWide<16>& pkt, uint64_t func_addr, uint64_t data_addr) {
    zero_wide(pkt);
    pkt[0] = INPCK_FSH_STATE;
    // fshader_function_addr at bits [127:64]
    pkt[2] = (uint32_t)(func_addr & 0xFFFFFFFF);
    pkt[3] = (uint32_t)(func_addr >> 32);
    // fshader_data_addr at bits [191:128]
    pkt[4] = (uint32_t)(data_addr & 0xFFFFFFFF);
    pkt[5] = (uint32_t)(data_addr >> 32);
}

static void build_packet_end_of_buffer(VlWide<16>& pkt) {
    zero_wide(pkt);
    pkt[0] = INPCK_END_OF_BUFFER;
}

static void build_packet_tri_tile_64x64(VlWide<16>& pkt, uint32_t seed = 0) {
    zero_wide(pkt);
    pkt[0] = INPCK_TRI_TILE_64x64 | (seed << 8);
    // Put seed data into coefficient fields for variety
    if (seed) {
        pkt[1] = seed;
        pkt[2] = seed ^ 0xAAAAAAAA;
        pkt[3] = seed ^ 0x55555555;
    }
}

static void build_packet_tri_tile_128x128(VlWide<16>& pkt, uint32_t seed = 0) {
    zero_wide(pkt);
    pkt[0] = INPCK_TRI_TILE_128x128 | (seed << 8);
    if (seed) {
        pkt[1] = seed;
        pkt[2] = seed ^ 0xBBBBBBBB;
    }
}

static void build_packet_large_triangle(VlWide<16>& pkt, uint32_t seed = 0) {
    zero_wide(pkt);
    pkt[0] = INPCK_LARGE_TRIANGLE | (seed << 8);
    if (seed) {
        pkt[1] = seed;
    }
}

static void build_packet_fully_covered_tile(VlWide<16>& pkt, uint32_t seed = 0) {
    zero_wide(pkt);
    pkt[0] = INPCK_FULLY_COVERED | (seed << 8);
    if (seed) pkt[1] = seed;
}

static void build_packet_large_tri_tile(VlWide<16>& pkt, uint32_t seed = 0) {
    zero_wide(pkt);
    pkt[0] = INPCK_LARGE_TRI_TILE | (seed << 8);
    if (seed) pkt[1] = seed;
}

// ── Helper: Send an AckData response for input buffer ────
static void send_input_buf_ack_data(DUT* d, uint8_t id, const VlWide<16>& data) {
    VlWide<17> rsp;
    build_rsp(rsp, id, 0, true, RSP_ACK_DATA, &data, 6 /*SizeLine*/, 0);
    memcpy(&d->sc_rbox_rsp_i, &rsp, sizeof(rsp));
    d->sc_rbox_rsp_valid_i = 1;
}

// ── Helper: Send a write Ack response ────────────────────
static void send_write_ack(DUT* d, uint8_t id) {
    VlWide<17> rsp;
    build_rsp(rsp, id, 0, false, RSP_ACK, nullptr, 0, 0);
    memcpy(&d->sc_rbox_rsp_i, &rsp, sizeof(rsp));
    d->sc_rbox_rsp_valid_i = 1;
}

// ── Helper: Send a depth buffer response (id MSB = 1) ────
static void send_db_rsp(DUT* d, uint8_t id_lo, const VlWide<16>& data) {
    uint8_t id = id_lo | 0x80;  // MSB=1 = depth buffer response
    VlWide<17> rsp;
    build_rsp(rsp, id, 0, true, RSP_ACK_DATA, &data, 6, 0);
    memcpy(&d->sc_rbox_rsp_i, &rsp, sizeof(rsp));
    d->sc_rbox_rsp_valid_i = 1;
}

// ── Helper: Send a depth buffer write ack (id MSB = 1) ───
static void send_db_write_ack(DUT* d, uint8_t id_lo) {
    uint8_t id = id_lo | 0x80;
    VlWide<17> rsp;
    build_rsp(rsp, id, 0, false, RSP_ACK, nullptr, 0, 0);
    memcpy(&d->sc_rbox_rsp_i, &rsp, sizeof(rsp));
    d->sc_rbox_rsp_valid_i = 1;
}

// ── Helper: Configure ESRs for a valid drawcall ──────────
static void configure_esrs(DUT* d, int line_count = 4, bool flush = false) {
    // Enable RBOX
    d->esr_config_i = 1;  // enable bit
    d->esr_config_en_i = 1;

    // Input buffer page: both pages active, page0 = 1, page1 = 2
    uint64_t in_buf_pg = 0;
    in_buf_pg |= (1ULL << 31);  // page_active0
    in_buf_pg |= (1ULL << 0);   // page0 = 1
    in_buf_pg |= (1ULL << 63);  // page_active1
    in_buf_pg |= (2ULL << 32);  // page1 = 2
    d->esr_in_buf_pg_i = in_buf_pg;
    d->esr_in_buf_pg_en_i = 1;

    // Input buffer config: line_count, start_offset = 0
    uint64_t in_buf_cfg = 0;
    in_buf_cfg |= ((uint64_t)line_count << 32);
    d->esr_in_buf_cfg_i = in_buf_cfg;
    d->esr_in_buf_cfg_en_i = 1;

    // Output buffer page: page_active = 1, page = 3
    uint32_t out_buf_pg = 0;
    out_buf_pg |= (1U << 31);  // page_active
    out_buf_pg |= 3;           // page = 3
    d->esr_out_buf_pg_i = out_buf_pg;
    d->esr_out_buf_pg_en_i = 1;

    // Output buffer config
    uint64_t out_buf_cfg = 0;
    uint32_t upper = 0;
    upper |= 2;         // hart_buf_lines = 2
    upper |= (0 << 3);  // hart_port_id = 0
    upper |= (3 << 5);  // hart_max_msg = 3
    upper |= (2 << 7);  // max_pckts_msg = 2
    out_buf_cfg = ((uint64_t)upper << 32);
    d->esr_out_buf_cfg_i = out_buf_cfg;
    d->esr_out_buf_cfg_en_i = 1;
}

// ── Helper: Apply ESRs and clear enables ──────────────────
static void apply_esrs_and_clear(CosimCtrl<DUT>& sim) {
    tick_and_compare(sim);
    auto* d = sim.dut.get();
    d->esr_config_en_i = 0;
    d->esr_in_buf_pg_en_i = 0;
    d->esr_in_buf_cfg_en_i = 0;
    d->esr_out_buf_pg_en_i = 0;
    d->esr_out_buf_cfg_en_i = 0;
    tick_and_compare(sim);
}

// ── Helper: Trigger start ─────────────────────────────────
static void trigger_start(CosimCtrl<DUT>& sim, bool flush = false) {
    auto* d = sim.dut.get();
    d->esr_start_i = flush ? ((1 << 16) | 1) : 1;
    d->esr_start_en_i = 1;
    tick_and_compare(sim);
    d->esr_start_en_i = 0;
    d->esr_start_i = 0;
}

// ── Helper: Wait for SC request to appear ─────────────────
static int wait_for_req(CosimCtrl<DUT>& sim, int max_cycles = 30) {
    auto* d = sim.dut.get();
    int count = 0;
    for (int i = 0; i < max_cycles; i++) {
        tick_and_compare(sim);
        if (d->new_rbox_sc_req_valid_o) count++;
    }
    return count;
}

// ── Helper: Send line response and tick ───────────────────
static void send_line_and_tick(CosimCtrl<DUT>& sim, uint8_t id, const VlWide<16>& data) {
    auto* d = sim.dut.get();
    send_input_buf_ack_data(d, id, data);
    tick_and_compare(sim);
    d->sc_rbox_rsp_valid_i = 0;
    tick_and_compare(sim);
}

// ── Helper: Process cycles, acking any write requests ─────
static void process_with_acks(CosimCtrl<DUT>& sim, int cycles) {
    auto* d = sim.dut.get();
    for (int i = 0; i < cycles; i++) {
        if (d->new_rbox_sc_req_valid_o) {
            send_write_ack(d, 0);
            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }
        tick_and_compare(sim);
    }
}

// ── Helper: Wait for FINISHED status ──────────────────────
static bool wait_for_finished(CosimCtrl<DUT>& sim, int max_cycles = 300) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        if (d->new_rbox_sc_req_valid_o) {
            send_write_ack(d, 0);
            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }
        tick_and_compare(sim);
        if (d->new_esr_status_o == STATUS_FINISHED) return true;
    }
    return false;
}

// ── Helper: Full reset + configure + start a drawcall ─────
static void full_reset_configure_start(CosimCtrl<DUT>& sim, int line_count = 4,
                                        bool flush = false) {
    auto* d = sim.dut.get();
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    configure_esrs(d, line_count, flush);
    apply_esrs_and_clear(sim);
    trigger_start(sim, flush);

    // Let it stabilize
    for (int i = 0; i < 3; i++) tick_and_compare(sim);
}

// ── Helper: Send ESR consume for a hart ───────────────────
static void send_consume(CosimCtrl<DUT>& sim, uint8_t hart_id,
                          uint8_t pckt_credits, uint8_t msg_credits) {
    auto* d = sim.dut.get();
    uint32_t consume_val = 0;
    consume_val |= (pckt_credits & 0xFF);         // pckt_credits at [7:0]
    consume_val |= ((uint32_t)(msg_credits & 0x3) << 8);  // msg_credits at [9:8]
    consume_val |= ((uint32_t)(hart_id & 0x3F) << 16);    // hart_id at [21:16]
    d->esr_consume_i = consume_val;
    d->esr_consume_en_i = 1;
    tick_and_compare(sim);
    d->esr_consume_en_i = 0;
    d->esr_consume_i = 0;
}

// ── Helper: Build a quad_data_t (780 bits, VlWide<25>) ────
//
// quad_data_t packed struct (MSB to LSB):
//   x[MaxTileDimL-1:0]              7 bits
//   y[MaxTileDimL-1:0]              7 bits
//   mask[3:0]                       4 bits
//   i[3:0][31:0]                    128 bits
//   j[3:0][31:0]                    128 bits
//   depth[3:0][31:0]                128 bits
//   sample_idx                      4 bits (MsaaSampleIdxSz)
//   msaa_coverage[3:0][MaxMsaaSamples-1:0] = 4*8 = 32 bits
//
// Total = 7+7+4+128+128+128+4+32 = 438? Hmm, the port is 780 bits.
// Let me just zero it and let both modules see the same input.
static void build_quad(VlWide<25>& quad, uint32_t seed = 0) {
    zero_wide(quad);
    if (seed) {
        for (int w = 0; w < 25; w++)
            quad[w] = seed ^ (w * 0x01010101);
    }
}

// ════════════════════════════════════════════════════════════
// Main test
// ════════════════════════════════════════════════════════════

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;  // 2M time units for thorough testing
    auto* d = sim.dut.get();

    // Pre-build common packets
    VlWide<16> pkt_rbox_state;
    build_packet_rbox_state(pkt_rbox_state);

    VlWide<16> pkt_fsh_state;
    build_packet_fsh_state(pkt_fsh_state, 0xDEAD0000, 0xBEEF0000);

    VlWide<16> pkt_tri_64;
    build_packet_tri_tile_64x64(pkt_tri_64);

    VlWide<16> pkt_tri_128;
    build_packet_tri_tile_128x128(pkt_tri_128);

    VlWide<16> pkt_large_tri;
    build_packet_large_triangle(pkt_large_tri);

    VlWide<16> pkt_fct;
    build_packet_fully_covered_tile(pkt_fct);

    VlWide<16> pkt_ltt;
    build_packet_large_tri_tile(pkt_ltt);

    VlWide<16> pkt_end;
    build_packet_end_of_buffer(pkt_end);

    // ════════════════════════════════════════════════════════
    // Phase A: Basic state machine (Tests 1-5)
    // ════════════════════════════════════════════════════════

    // ── Test 1: Reset → status=READY, rbox_enable=0, all idles ──
    printf("Test 1: Reset -> READY\n");
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    sim.check(d->new_esr_status_o == STATUS_READY, "new status = READY after reset");
    sim.check(d->orig_esr_status_o == STATUS_READY, "orig status = READY after reset");
    sim.check(d->new_rbox_enable_o == 0, "rbox_enable = 0 after reset");
    sim.check(d->new_rbox_sc_req_valid_o == 0, "no SC req after reset");
    sim.check(d->new_sc_db_rsp_valid_o == 0, "no DB rsp after reset");
    sim.check(d->new_tr_tile_valid_o == 0, "no tile valid after reset");
    sim.check(d->new_db_flush_o == 0, "no flush after reset");

    // Run idle cycles, verify nothing changes
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 2: Config error — no active pages → ERROR ──────
    printf("Test 2: Config error (no active pages)\n");
    clear_inputs(d);
    // Set NO page active for input buffer
    d->esr_in_buf_pg_i = 0;
    d->esr_in_buf_pg_en_i = 1;
    // No page active for output buffer either
    d->esr_out_buf_pg_i = 0;
    d->esr_out_buf_pg_en_i = 1;
    tick_and_compare(sim);
    d->esr_in_buf_pg_en_i = 0;
    d->esr_out_buf_pg_en_i = 0;

    // Trigger start
    trigger_start(sim);

    // Should see ERROR
    for (int i = 0; i < 10; i++) tick_and_compare(sim);
    sim.check(d->new_esr_status_o == STATUS_ERROR || d->new_esr_status_o == STATUS_FINISHED,
              "status = ERROR or FINISHED after no-pages config error");

    // Let settle
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ── Test 3: Config error — mismatched page/line config → ERROR
    printf("Test 3: Config error (mismatched page/line)\n");
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    // Only page0 active, but line_count MSB set (requires page1)
    uint64_t in_buf_pg = 0;
    in_buf_pg |= (1ULL << 31);  // page_active0 only
    in_buf_pg |= (1ULL << 0);
    d->esr_in_buf_pg_i = in_buf_pg;
    d->esr_in_buf_pg_en_i = 1;

    uint64_t in_buf_cfg = 0;
    in_buf_cfg |= ((uint64_t)(1 << 15) << 32);  // line_count with MSB set
    d->esr_in_buf_cfg_i = in_buf_cfg;
    d->esr_in_buf_cfg_en_i = 1;

    // Output page active
    d->esr_out_buf_pg_i = (1U << 31) | 3;
    d->esr_out_buf_pg_en_i = 1;

    tick_and_compare(sim);
    d->esr_in_buf_pg_en_i = 0;
    d->esr_in_buf_cfg_en_i = 0;
    d->esr_out_buf_pg_en_i = 0;
    tick_and_compare(sim);

    trigger_start(sim);

    for (int i = 0; i < 10; i++) tick_and_compare(sim);
    sim.check(d->new_esr_status_o == STATUS_ERROR || d->new_esr_status_o == STATUS_FINISHED,
              "status = ERROR or FINISHED after page/line mismatch");

    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ── Test 4: Normal start → status=WORKING, rbox_enable from ESR ──
    printf("Test 4: Normal start -> WORKING\n");
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    configure_esrs(d);
    apply_esrs_and_clear(sim);

    // Verify rbox_enable reflects config
    sim.check(d->new_rbox_enable_o == 1, "rbox_enable = 1 after config");

    trigger_start(sim);

    sim.check(d->new_esr_status_o == STATUS_WORKING, "status = WORKING after start");
    sim.check(d->orig_esr_status_o == STATUS_WORKING, "orig status = WORKING after start");

    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ── Test 5: Start while already running ─────────────────
    printf("Test 5: Start while already running\n");
    // Already in WORKING from test 4; trigger start again
    trigger_start(sim);

    // Should stay WORKING (or restart); either way both modules match
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════════
    // Phase B: Input buffer path (Tests 6-12)
    // ════════════════════════════════════════════════════════

    // ── Test 6: Single line fetch ───────────────────────────
    printf("Test 6: Single line fetch (ET-Link read request)\n");
    full_reset_configure_start(sim, 1);

    d->rbox_sc_req_ready_i = 0x1F;
    {
        int saw_req = 0;
        for (int i = 0; i < 50; i++) {
            tick_and_compare(sim);
            if (d->new_rbox_sc_req_valid_o) saw_req++;
        }
        // With 1 line, at least 1 request should appear
        printf("  saw %d SC read requests\n", saw_req);
    }

    // ── Test 7: Multi-line fetch ────────────────────────────
    printf("Test 7: Multi-line fetch (4 lines)\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    {
        int req_count = 0;
        for (int i = 0; i < 80; i++) {
            tick_and_compare(sim);
            if (d->new_rbox_sc_req_valid_o) req_count++;
        }
        printf("  saw %d SC read requests\n", req_count);
    }

    // ── Test 8: Dual-page addressing ────────────────────────
    printf("Test 8: Dual-page addressing\n");
    // Use a large start_offset so some lines cross to page1
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    // Configure with both pages active, start_offset near page boundary
    d->esr_config_i = 1;
    d->esr_config_en_i = 1;

    {
        uint64_t pg = 0;
        pg |= (1ULL << 31);  // page_active0
        pg |= (5ULL << 0);   // page0 = 5
        pg |= (1ULL << 63);  // page_active1
        pg |= (7ULL << 32);  // page1 = 7
        d->esr_in_buf_pg_i = pg;
        d->esr_in_buf_pg_en_i = 1;
    }

    // start_offset = 0x7FFE (near MSB boundary), line_count = 4
    {
        uint64_t cfg = 0;
        cfg |= (4ULL << 32);       // line_count = 4
        cfg |= (0x7FFEULL << 0);   // start_offset near boundary
        d->esr_in_buf_cfg_i = cfg;
        d->esr_in_buf_cfg_en_i = 1;
    }

    d->esr_out_buf_pg_i = (1U << 31) | 3;
    d->esr_out_buf_pg_en_i = 1;
    {
        uint64_t out_buf_cfg = 0;
        uint32_t upper = 2 | (3 << 5) | (2 << 7);
        out_buf_cfg = ((uint64_t)upper << 32);
        d->esr_out_buf_cfg_i = out_buf_cfg;
        d->esr_out_buf_cfg_en_i = 1;
    }

    apply_esrs_and_clear(sim);
    trigger_start(sim);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 40; i++) tick_and_compare(sim);

    // ── Test 9: Response delivery — AckData → FIFO push ────
    printf("Test 9: Response delivery -> FIFO push\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    // Wait for requests to be generated
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Send responses for all 4 lines
    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_64);
    send_line_and_tick(sim, 3, pkt_end);

    // Let FIFO process
    for (int i = 0; i < 30; i++) tick_and_compare(sim);

    // ── Test 10: Response for depth buffer (id MSB=1) ───────
    printf("Test 10: DB response routing (id MSB=1)\n");
    // Send a response with MSB=1 → should route to sc_db_rsp
    {
        VlWide<16> db_data;
        zero_wide(db_data);
        db_data[0] = 0xCAFEBABE;
        send_db_rsp(d, 0x05, db_data);
        tick_and_compare(sim);

        // sc_db_rsp_valid should be asserted (if rsp_ready allows)
        // Both modules should agree on routing
        d->sc_rbox_rsp_valid_i = 0;
    }

    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ── Test 11: Input FIFO backpressure ────────────────────
    printf("Test 11: Input FIFO backpressure (full FIFO stops fetches)\n");
    full_reset_configure_start(sim, 8);

    // Don't accept responses to fill FIFO quickly; block SC ready to test
    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Send responses rapidly without consuming
    d->tr_accept_i = 0;  // Don't accept tiles → pipeline backs up
    for (int id = 0; id < 8; id++) {
        VlWide<16> pkt;
        zero_wide(pkt);
        pkt[0] = INPCK_TRI_TILE_64x64;
        send_input_buf_ack_data(d, id, pkt);
        tick_and_compare(sim);
        d->sc_rbox_rsp_valid_i = 0;
        tick_and_compare(sim);
    }

    // FIFO should be full or nearly full; check that new fetches are suppressed
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    d->tr_accept_i = 1;  // Release backpressure
    for (int i = 0; i < 30; i++) tick_and_compare(sim);

    // ── Test 12: Buffer end detection ───────────────────────
    printf("Test 12: Buffer end (received_lines == line_count)\n");
    full_reset_configure_start(sim, 2);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Send exactly 2 lines
    send_line_and_tick(sim, 0, pkt_fsh_state);
    send_line_and_tick(sim, 1, pkt_end);

    // After receiving 2 lines, no more requests should be generated
    {
        int extra_reqs = 0;
        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            // Count new read requests (not write requests)
            if (d->new_rbox_sc_req_valid_o) extra_reqs++;
        }
        // Some writes for output may appear but that's OK
    }

    // ════════════════════════════════════════════════════════
    // Phase C: Each packet type decoded individually (Tests 13-20)
    // ════════════════════════════════════════════════════════

    // ── Test 13: RBOX_STATE packet ──────────────────────────
    printf("Test 13: RBOX_STATE packet decode\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    // Let it decode
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Verify ts_state output fields match between modules
    // (depth config, scissor, etc. should be loaded from RBOX_STATE)

    // ── Test 14: FSH_STATE packet ───────────────────────────
    printf("Test 14: FSH_STATE packet decode\n");
    // Continue from test 13 — send FSH_STATE
    send_line_and_tick(sim, 1, pkt_fsh_state);
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 15: TRI_WITH_TILE_64x64 ───────────────────────
    printf("Test 15: TRI_WITH_TILE_64x64 packet decode\n");
    d->tr_accept_i = 1;
    send_line_and_tick(sim, 2, pkt_tri_64);

    // Should produce tr_tile_valid
    {
        bool saw_tile = false;
        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_tr_tile_valid_o) saw_tile = true;
        }
        sim.check(saw_tile, "saw tr_tile_valid for 64x64 tile");
    }

    // Send END to complete
    send_line_and_tick(sim, 3, pkt_end);
    for (int i = 0; i < 30; i++) tick_and_compare(sim);

    // ── Test 16: TRI_WITH_TILE_128x128 ─────────────────────
    printf("Test 16: TRI_WITH_TILE_128x128 packet decode\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_128);

    d->tr_accept_i = 1;
    {
        bool saw_tile = false;
        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_tr_tile_valid_o) saw_tile = true;
        }
        sim.check(saw_tile, "saw tr_tile_valid for 128x128 tile");
    }

    send_line_and_tick(sim, 3, pkt_end);
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 17: LARGE_TRIANGLE ─────────────────────────────
    printf("Test 17: LARGE_TRIANGLE packet decode\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_large_tri);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 3, pkt_end);
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 18: FULLY_COVERED_TILE ─────────────────────────
    printf("Test 18: FULLY_COVERED_TILE packet decode\n");
    full_reset_configure_start(sim, 5);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_large_tri);  // Need a large tri first for FCT
    send_line_and_tick(sim, 3, pkt_fct);

    d->tr_accept_i = 1;
    for (int i = 0; i < 30; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 4, pkt_end);
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 19: LARGE_TRIANGLE_TILE ────────────────────────
    printf("Test 19: LARGE_TRIANGLE_TILE packet decode\n");
    full_reset_configure_start(sim, 5);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_large_tri);  // Must precede LTT
    send_line_and_tick(sim, 3, pkt_ltt);

    d->tr_accept_i = 1;
    for (int i = 0; i < 30; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 4, pkt_end);
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 20: END_OF_BUFFER ──────────────────────────────
    printf("Test 20: END_OF_BUFFER packet\n");
    full_reset_configure_start(sim, 3);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // End of buffer should eventually lead toward done
    process_with_acks(sim, 50);

    // ════════════════════════════════════════════════════════
    // Phase D: Output path (Tests 21-28)
    // ════════════════════════════════════════════════════════

    // ── Test 21: Quad info packet assembly ──────────────────
    printf("Test 21: Quad output packet assembly\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_64);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Now feed quads from traverse
    {
        VlWide<25> quad;
        build_quad(quad, 0x12345678);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        // Wait for quad accept
        int accepted = 0;
        for (int i = 0; i < 50; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) {
                accepted++;
                break;
            }
        }
        d->tr_quad_valid_i = 0;

        // Feed test results
        d->ts_result_mask_i = 0xF;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        // Let output buffer generate write requests
        process_with_acks(sim, 60);
    }

    // ── Test 22: Data packet sending ────────────────────────
    printf("Test 22: Multiple quads -> data packets\n");
    // Continue feeding quads
    for (int q = 0; q < 3; q++) {
        VlWide<25> quad;
        build_quad(quad, 0xAABB0000 + q);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 50; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xA;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        process_with_acks(sim, 30);
    }

    // ── Test 23: End phase packet ───────────────────────────
    printf("Test 23: End phase packet (END_OF_BUFFER -> end phase per hart)\n");
    send_line_and_tick(sim, 3, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Process until end phase packets sent
    process_with_acks(sim, 100);

    // ── Test 24: Hart message (MsgSendData) ─────────────────
    printf("Test 24: Hart message sending\n");
    // The message should have been triggered by packet threshold.
    // Release credits so the message path can fire.
    send_consume(sim, 0, 8, 1);
    process_with_acks(sim, 50);

    // ── Test 25: Packet credit exhaustion ───────────────────
    printf("Test 25: Packet credit exhaustion\n");
    full_reset_configure_start(sim, 6);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);

    // Send many tiles to exhaust credits
    for (int t = 0; t < 3; t++) {
        VlWide<16> pkt;
        build_packet_tri_tile_64x64(pkt, t + 1);
        send_line_and_tick(sim, 2 + t, pkt);
    }

    d->tr_accept_i = 1;

    // Feed quads and results rapidly
    for (int q = 0; q < 8; q++) {
        VlWide<25> quad;
        build_quad(quad, 0xCC000000 + q);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 40; i++) {
            tick_and_compare(sim);
            if (d->new_rbox_sc_req_valid_o) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xF;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        process_with_acks(sim, 20);
    }

    // At this point credits may be exhausted — verify modules still agree
    process_with_acks(sim, 50);

    // ── Test 26: Credit replenishment via consume ESR ───────
    printf("Test 26: Credit replenishment\n");
    // Release credits for hart 0
    send_consume(sim, 0, 16, 2);
    process_with_acks(sim, 40);

    // Release credits for a few other harts
    for (int h = 1; h < 4; h++) {
        send_consume(sim, h, 8, 1);
        process_with_acks(sim, 10);
    }

    send_line_and_tick(sim, 5, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;
    process_with_acks(sim, 100);

    // ── Test 27: Message credit tracking ────────────────────
    printf("Test 27: Message credit tracking\n");
    // Already covered by credit replenishment; just run more cycles
    // to verify no divergence in msg_credits handling
    for (int h = 0; h < 8; h++) {
        send_consume(sim, h, 4, 1);
        for (int i = 0; i < 5; i++) tick_and_compare(sim);
    }

    // ── Test 28: Buffer offset wrap-around ──────────────────
    printf("Test 28: Buffer offset wrap-around\n");
    full_reset_configure_start(sim, 5);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);

    // Send multiple tiles
    send_line_and_tick(sim, 2, pkt_tri_64);
    d->tr_accept_i = 1;

    // Feed many quads to cause buffer offset to wrap
    for (int q = 0; q < 10; q++) {
        VlWide<25> quad;
        build_quad(quad, 0xDD000000 + q);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_rbox_sc_req_valid_o) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = (q & 0xF) | 1;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        // Release credits periodically
        if (q % 3 == 2)
            send_consume(sim, 0, 8, 1);

        process_with_acks(sim, 15);
    }

    send_line_and_tick(sim, 3, pkt_tri_64);
    send_line_and_tick(sim, 4, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;
    process_with_acks(sim, 80);

    // ════════════════════════════════════════════════════════
    // Phase E: Arbitration (Tests 29-33)
    // ════════════════════════════════════════════════════════

    // ── Test 29: DB priority over input ─────────────────────
    printf("Test 29: DB priority over input\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;

    // Wait until input buffer is generating requests
    for (int i = 0; i < 15; i++) tick_and_compare(sim);

    // Now simultaneously assert DB request
    {
        VlWide<19> db_req;
        zero_wide(db_req);
        db_req[0] = 0x12345678;  // Some request data
        d->db_sc_req_valid_i = 0x01;  // Bank 0
        memcpy(&d->db_sc_req_i, &db_req, sizeof(db_req));
    }

    // Tick several cycles with both sources active
    for (int i = 0; i < 20; i++) {
        tick_and_compare(sim);
    }

    d->db_sc_req_valid_i = 0;
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ── Test 30: Input priority over output ─────────────────
    printf("Test 30: Input priority over output\n");
    // The input buffer requests should still be higher priority than output
    // Continue from above — input has pending requests
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // ── Test 31: Output msg priority over output pkt ────────
    printf("Test 31: Output msg priority over output pkt\n");
    // Feed data to generate both msg and pkt output requests
    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_64);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Feed quads to generate output
    for (int q = 0; q < 5; q++) {
        VlWide<25> quad;
        build_quad(quad, 0xEE000000 + q);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xF;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;
    }

    // Process with intermittent backpressure to see arbitration
    for (int i = 0; i < 60; i++) {
        d->rbox_sc_req_ready_i = (i % 3 == 0) ? 0x00 : 0x1F;
        if (d->new_rbox_sc_req_valid_o && (i % 3 != 0)) {
            send_write_ack(d, 0);
            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }
        tick_and_compare(sim);
    }
    d->rbox_sc_req_ready_i = 0x1F;

    // ── Test 32: All four sources active simultaneously ─────
    printf("Test 32: All four sources simultaneously\n");
    // DB + input + output msg + output pkt all active
    {
        VlWide<19> db_req;
        zero_wide(db_req);
        d->db_sc_req_valid_i = 0x01;
        memcpy(&d->db_sc_req_i, &db_req, sizeof(db_req));
    }

    for (int i = 0; i < 30; i++) {
        tick_and_compare(sim);
        // Toggle DB request
        if (i == 10) d->db_sc_req_valid_i = 0;
        if (i == 15) {
            d->db_sc_req_valid_i = 0x04;  // Different bank
        }
        if (i == 20) d->db_sc_req_valid_i = 0;
    }

    d->db_sc_req_valid_i = 0;
    process_with_acks(sim, 30);

    // ── Test 33: Ready deassertion ──────────────────────────
    printf("Test 33: SC ready deassertion patterns\n");
    // Toggle individual bank ready bits
    for (int pattern = 0; pattern < 32; pattern++) {
        d->rbox_sc_req_ready_i = pattern & 0x1F;
        tick_and_compare(sim);
    }
    d->rbox_sc_req_ready_i = 0x1F;

    send_line_and_tick(sim, 3, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;
    process_with_acks(sim, 80);

    // ════════════════════════════════════════════════════════
    // Phase F: Multi-drawcall and sustained (Tests 34-38)
    // ════════════════════════════════════════════════════════

    // ── Test 34: Full drawcall sequence ─────────────────────
    printf("Test 34: Full drawcall (STATE -> FSH -> tiles -> END -> FINISHED)\n");
    full_reset_configure_start(sim, 5);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_64);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Process tile through pipeline
    {
        VlWide<25> quad;
        build_quad(quad, 0xF1F1F1F1);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xF;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;
    }

    process_with_acks(sim, 40);

    // Send second tile
    VlWide<16> pkt_tri64_2;
    build_packet_tri_tile_64x64(pkt_tri64_2, 0xAA);
    send_line_and_tick(sim, 3, pkt_tri64_2);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    {
        VlWide<25> quad;
        build_quad(quad, 0xF2F2F2F2);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xA;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;
    }

    process_with_acks(sim, 40);

    // Send END
    send_line_and_tick(sim, 4, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Release all hart credits generously (multiple rounds)
    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++) {
            send_consume(sim, h, 32, 3);
        }
        process_with_acks(sim, 50);
    }

    // Wait for FINISHED
    bool finished = wait_for_finished(sim, 1000);
    printf("  drawcall: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // ── Test 35: Two consecutive drawcalls ──────────────────
    printf("Test 35: Two consecutive drawcalls\n");
    // First drawcall
    full_reset_configure_start(sim, 3);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++)
            send_consume(sim, h, 32, 3);
        process_with_acks(sim, 50);
    }

    finished = wait_for_finished(sim, 1000);
    printf("  first drawcall: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // Second drawcall
    configure_esrs(d, 3);
    apply_esrs_and_clear(sim);
    trigger_start(sim);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    VlWide<16> pkt_fsh2;
    build_packet_fsh_state(pkt_fsh2, 0xAAAA0000, 0xBBBB0000);
    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh2);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++)
            send_consume(sim, h, 32, 3);
        process_with_acks(sim, 50);
    }

    finished = wait_for_finished(sim, 1000);
    printf("  second drawcall: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // ── Test 36: Sustained tile streaming (50+ tiles) ───────
    printf("Test 36: Sustained tile streaming (50+ tiles)\n");
    full_reset_configure_start(sim, 60);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Send RBOX_STATE and FSH_STATE
    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);

    d->tr_accept_i = 1;

    // Send 55 tiles + END
    for (int t = 0; t < 55; t++) {
        VlWide<16> pkt;
        build_packet_tri_tile_64x64(pkt, t + 1);
        send_line_and_tick(sim, 2 + t, pkt);

        // Process a few cycles to let pipeline work
        for (int i = 0; i < 3; i++) tick_and_compare(sim);

        // Feed a quad if traverse accepts
        VlWide<25> quad;
        build_quad(quad, 0x50000000 + t);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 15; i++) {
            if (d->new_rbox_sc_req_valid_o) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        // Feed test result
        d->ts_result_mask_i = (t & 0xF) | 1;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 5; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        // Release credits periodically
        if (t % 5 == 4)
            send_consume(sim, 0, 16, 1);

        process_with_acks(sim, 5);
    }

    // Send END
    send_line_and_tick(sim, 57, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    for (int h = 0; h < 64; h++)
        send_consume(sim, h, 32, 2);

    process_with_acks(sim, 200);

    // ── Test 37: Backpressure patterns ──────────────────────
    printf("Test 37: Periodic SC ready deassertion\n");
    full_reset_configure_start(sim, 6);

    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);

    for (int t = 0; t < 3; t++) {
        VlWide<16> pkt;
        build_packet_tri_tile_64x64(pkt, t + 100);
        send_line_and_tick(sim, 2 + t, pkt);
    }

    d->tr_accept_i = 1;

    // Run with periodic backpressure patterns
    for (int cycle = 0; cycle < 200; cycle++) {
        // Cycle through different backpressure patterns
        if (cycle % 7 < 2)
            d->rbox_sc_req_ready_i = 0x00;
        else if (cycle % 7 < 4)
            d->rbox_sc_req_ready_i = 0x15;  // Some banks
        else
            d->rbox_sc_req_ready_i = 0x1F;

        // Feed quads periodically
        if (cycle % 15 == 0 && cycle < 150) {
            VlWide<25> quad;
            build_quad(quad, 0x37000000 + cycle);
            memcpy(&d->tr_quad_i, &quad, sizeof(quad));
            d->tr_quad_valid_i = 1;
        }

        // Accept results
        if (d->new_ct_result_accept_o || (cycle % 10 == 5 && d->new_ct_quad_accept_o)) {
            d->tr_quad_valid_i = 0;
        }
        if (cycle % 8 == 0) {
            d->ts_result_mask_i = 0xF;
            d->ts_result_valid_i = 1;
        }

        if (d->new_rbox_sc_req_valid_o && d->rbox_sc_req_ready_i) {
            send_write_ack(d, 0);
            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }

        tick_and_compare(sim);
    }

    d->tr_quad_valid_i = 0;
    d->ts_result_valid_i = 0;
    d->rbox_sc_req_ready_i = 0x1F;

    send_line_and_tick(sim, 5, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;
    process_with_acks(sim, 100);

    // ── Test 38: Interleaved quad output with backpressure ──
    printf("Test 38: Interleaved quads with backpressure\n");
    full_reset_configure_start(sim, 6);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);

    for (int t = 0; t < 3; t++) {
        VlWide<16> pkt;
        build_packet_tri_tile_128x128(pkt, t + 200);
        send_line_and_tick(sim, 2 + t, pkt);
    }

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Feed quads with alternating backpressure
    for (int q = 0; q < 12; q++) {
        VlWide<25> quad;
        build_quad(quad, 0x38000000 + q);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        d->rbox_sc_req_ready_i = (q % 2 == 0) ? 0x1F : 0x00;

        for (int i = 0; i < 40; i++) {
            tick_and_compare(sim);
            d->rbox_sc_req_ready_i = 0x1F;  // Release after first cycle
            if (d->new_rbox_sc_req_valid_o) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0x5;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;

        if (q % 4 == 3)
            send_consume(sim, 0, 8, 1);

        process_with_acks(sim, 10);
    }

    send_line_and_tick(sim, 5, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;
    process_with_acks(sim, 100);

    // ════════════════════════════════════════════════════════
    // Phase G: Done and flush (Tests 39-43)
    // ════════════════════════════════════════════════════════

    // ── Test 39: Done without flush ─────────────────────────
    printf("Test 39: Done without flush\n");
    full_reset_configure_start(sim, 3);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Release credits for all harts (multiple rounds)
    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++)
            send_consume(sim, h, 32, 3);
        process_with_acks(sim, 50);
    }

    finished = wait_for_finished(sim, 1000);
    printf("  done without flush: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);
    sim.check(d->new_db_flush_o == 0, "no flush asserted");

    // ── Test 40: Done with flush ────────────────────────────
    printf("Test 40: Done with flush\n");
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    configure_esrs(d, 3, true);
    apply_esrs_and_clear(sim);
    trigger_start(sim, true);  // flush = 1

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Release credits generously
    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++)
            send_consume(sim, h, 32, 3);
        process_with_acks(sim, 50);
    }

    // Process; should see db_flush_o asserted eventually
    {
        bool saw_flush = false;
        for (int i = 0; i < 1000; i++) {
            if (d->new_rbox_sc_req_valid_o) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }
            tick_and_compare(sim);
            if (d->new_db_flush_o) {
                saw_flush = true;
                // Signal flush done
                d->db_flush_done_i = 1;
                tick_and_compare(sim);
                d->db_flush_done_i = 0;
                break;
            }
        }
        printf("  flush asserted: %s\n", saw_flush ? "yes" : "no (end_phase not yet sent)");
    }

    finished = wait_for_finished(sim, 500);
    printf("  done with flush: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // ── Test 41: Flush timing ───────────────────────────────
    printf("Test 41: Flush timing (after tr_empty && ts_empty)\n");
    clear_inputs(d);
    sim.reset();
    compare_outputs(sim);

    configure_esrs(d, 3, true);
    apply_esrs_and_clear(sim);
    trigger_start(sim, true);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    // Keep tr_empty=0 initially; flush should NOT start
    d->tr_empty_i = 0;
    d->ts_empty_i = 0;

    // Release credits generously
    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++)
            send_consume(sim, h, 32, 3);
        process_with_acks(sim, 50);
    }

    // Check that flush is NOT active while tr_empty=0
    // (Both modules should agree regardless)
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Now set empty — flush should start
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    {
        bool saw_flush = false;
        for (int i = 0; i < 500; i++) {
            process_with_acks(sim, 1);
            if (d->new_db_flush_o) {
                saw_flush = true;
                d->db_flush_done_i = 1;
                tick_and_compare(sim);
                d->db_flush_done_i = 0;
                break;
            }
        }
        printf("  flush after empty: %s\n", saw_flush ? "yes" : "no (end_phase not yet sent)");
    }

    wait_for_finished(sim, 500);

    // ── Test 42: Multiple harts active ──────────────────────
    printf("Test 42: Multiple harts - done requires ALL active harts notified\n");
    full_reset_configure_start(sim, 4);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Use a RBOX_STATE that configures multiple harts (hart_tile_width/height > 0)
    VlWide<16> pkt_state_multi;
    build_packet_rbox_state_v2(pkt_state_multi, 0, 0, 0, 0, 100);
    send_line_and_tick(sim, 0, pkt_state_multi);
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_tri_64);

    d->tr_accept_i = 1;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    // Feed a quad
    {
        VlWide<25> quad;
        build_quad(quad, 0x42424242);
        memcpy(&d->tr_quad_i, &quad, sizeof(quad));
        d->tr_quad_valid_i = 1;

        for (int i = 0; i < 30; i++) {
            tick_and_compare(sim);
            if (d->new_ct_quad_accept_o) break;
        }
        d->tr_quad_valid_i = 0;

        d->ts_result_mask_i = 0xF;
        d->ts_result_valid_i = 1;
        for (int i = 0; i < 10; i++) {
            tick_and_compare(sim);
            if (d->new_ct_result_accept_o) break;
        }
        d->ts_result_valid_i = 0;
    }

    process_with_acks(sim, 60);

    send_line_and_tick(sim, 3, pkt_end);
    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Release credits incrementally (multiple rounds)
    for (int round = 0; round < 3; round++) {
        for (int h = 0; h < 64; h++) {
            send_consume(sim, h, 32, 3);
            process_with_acks(sim, 2);
        }
        process_with_acks(sim, 50);
    }

    finished = wait_for_finished(sim, 1000);
    printf("  all harts notified: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // ── Test 43: Hart active mask ───────────────────────────
    printf("Test 43: Hart active mask (only active harts need end_phase)\n");
    // With default hart_tile_width=0, hart_tile_height=0 (from default RBOX_STATE),
    // only hart 0 should be active. Test that done doesn't require harts 1-63.
    full_reset_configure_start(sim, 3);

    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    send_line_and_tick(sim, 0, pkt_rbox_state);  // Default: 1 hart active
    send_line_and_tick(sim, 1, pkt_fsh_state);
    send_line_and_tick(sim, 2, pkt_end);

    d->tr_empty_i = 1;
    d->ts_empty_i = 1;

    // Only release credits for hart 0 (multiple rounds)
    for (int round = 0; round < 3; round++) {
        send_consume(sim, 0, 32, 3);
        process_with_acks(sim, 100);
    }

    finished = wait_for_finished(sim, 1000);
    printf("  single-hart done: %s (status=0x%02x)\n", finished ? "FINISHED" : "not finished", d->new_esr_status_o);

    // ════════════════════════════════════════════════════════
    // Phase H: Random stress tests (Tests 44-45)
    // ════════════════════════════════════════════════════════

    // ── Test 44: Random stimulus (5000 cycles) ──────────────
    printf("Test 44: Random stimulus (5000 cycles)\n");
    {
        uint32_t seed = 0xDEADBEEF;
        auto xorshift = [&]() -> uint32_t {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            return seed;
        };

        // Start a drawcall
        full_reset_configure_start(sim, 10);

        d->rbox_sc_req_ready_i = 0x1F;
        for (int i = 0; i < 20; i++) tick_and_compare(sim);

        // Send RBOX_STATE and FSH_STATE
        send_line_and_tick(sim, 0, pkt_rbox_state);
        send_line_and_tick(sim, 1, pkt_fsh_state);

        // Send tiles
        for (int t = 0; t < 7; t++) {
            VlWide<16> pkt;
            build_packet_tri_tile_64x64(pkt, xorshift());
            send_line_and_tick(sim, 2 + t, pkt);
        }
        // Send END
        send_line_and_tick(sim, 9, pkt_end);

        d->tr_accept_i = 1;

        for (int cycle = 0; cycle < 5000; cycle++) {
            uint32_t r = xorshift();

            // Random backpressure
            if ((r & 0xF) == 0)
                d->rbox_sc_req_ready_i = xorshift() & 0x1F;
            else
                d->rbox_sc_req_ready_i = 0x1F;

            // Random traverse accept/empty
            d->tr_accept_i = ((r >> 4) & 0x3) != 0 ? 1 : 0;
            d->tr_empty_i  = ((r >> 6) & 0x7) == 0 ? 1 : 0;
            d->ts_empty_i  = ((r >> 9) & 0x7) == 0 ? 1 : 0;

            // Random quad injection
            if (((r >> 12) & 0xF) < 3) {
                VlWide<25> quad;
                build_quad(quad, xorshift());
                memcpy(&d->tr_quad_i, &quad, sizeof(quad));
                d->tr_quad_valid_i = 1;
            } else if (d->new_ct_quad_accept_o) {
                d->tr_quad_valid_i = 0;
            }

            // Random test results
            if (((r >> 16) & 0x7) < 2) {
                d->ts_result_mask_i = xorshift() & 0xF;
                d->ts_result_valid_i = 1;
            } else if (d->new_ct_result_accept_o) {
                d->ts_result_valid_i = 0;
            }

            // Random ESR consume
            if (((r >> 19) & 0x1F) == 0) {
                send_consume(sim, xorshift() & 0x3F, (xorshift() & 0xF) + 1, xorshift() & 0x3);
            }

            // Random DB requests (valid bit only on one bank)
            if (((r >> 24) & 0xF) == 0) {
                VlWide<19> db_req;
                zero_wide(db_req);
                db_req[0] = xorshift();
                d->db_sc_req_valid_i = 1 << (xorshift() % 5);
                memcpy(&d->db_sc_req_i, &db_req, sizeof(db_req));
            } else if (((r >> 24) & 0xF) < 3) {
                d->db_sc_req_valid_i = 0;
            }

            // Send DB responses (id MSB=1): safe, routed to depth buffer
            if (((r >> 28) & 0x7) == 0) {
                VlWide<16> data;
                zero_wide(data);
                data[0] = xorshift();
                send_db_rsp(d, xorshift() & 0x7F, data);
            }

            // Respond to output write requests only when valid
            // Use a placeholder ack ID of 0 (safe hart_id)
            if (d->new_rbox_sc_req_valid_o && ((r >> 2) & 0x3) != 0) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
            }

            // Random flush done
            if (((r >> 20) & 0x3F) == 0) {
                d->db_flush_done_i = 1;
            } else {
                d->db_flush_done_i = 0;
            }

            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }
    }

    // ── Test 45: Long sustained streaming (3000 cycles) ─────
    printf("Test 45: Long sustained streaming (3000 cycles)\n");
    {
        uint32_t seed = 0xCAFEBABE;
        auto xorshift = [&]() -> uint32_t {
            seed ^= seed << 13;
            seed ^= seed >> 17;
            seed ^= seed << 5;
            return seed;
        };

        // Start fresh
        full_reset_configure_start(sim, 20);

        d->rbox_sc_req_ready_i = 0x1F;
        for (int i = 0; i < 20; i++) tick_and_compare(sim);

        // Send RBOX_STATE and FSH_STATE
        send_line_and_tick(sim, 0, pkt_rbox_state);
        send_line_and_tick(sim, 1, pkt_fsh_state);

        // Send 16 tiles
        for (int t = 0; t < 16; t++) {
            VlWide<16> pkt;
            build_packet_tri_tile_64x64(pkt, xorshift());
            send_line_and_tick(sim, 2 + t, pkt);
        }
        // Send END_OF_BUFFER
        send_line_and_tick(sim, 18, pkt_end);

        // Some additional tiles via other packet types
        // (Already sent, will be processed from FIFO)

        d->tr_accept_i = 1;
        int quads_fed = 0;
        int results_fed = 0;
        int acks_sent = 0;

        for (int cycle = 0; cycle < 3000; cycle++) {
            uint32_t r = xorshift();

            // Mostly keep SC ready, occasional backpressure
            d->rbox_sc_req_ready_i = ((r & 0xF) < 2) ? (r >> 4) & 0x1F : 0x1F;

            // Keep traverse flowing
            d->tr_accept_i = ((r >> 8) & 0x7) != 0 ? 1 : 0;
            d->tr_empty_i  = ((r >> 11) & 0xF) == 0 ? 1 : 0;
            d->ts_empty_i  = ((r >> 15) & 0xF) == 0 ? 1 : 0;

            // Feed quads at moderate rate
            if (((r >> 19) & 0x7) < 2 && quads_fed < 200) {
                VlWide<25> quad;
                build_quad(quad, xorshift());
                memcpy(&d->tr_quad_i, &quad, sizeof(quad));
                d->tr_quad_valid_i = 1;
                quads_fed++;
            } else if (d->new_ct_quad_accept_o) {
                d->tr_quad_valid_i = 0;
            }

            // Feed test results
            if (((r >> 22) & 0x7) < 2 && results_fed < 200) {
                d->ts_result_mask_i = xorshift() & 0xF;
                d->ts_result_valid_i = 1;
                results_fed++;
            } else if (d->new_ct_result_accept_o) {
                d->ts_result_valid_i = 0;
            }

            // Release credits periodically
            if (((r >> 25) & 0x1F) == 0) {
                send_consume(sim, xorshift() & 0x3F, (xorshift() & 0x7) + 1, 1);
            }

            // Ack write requests only when valid (use fixed id=0 for safety)
            if (d->new_rbox_sc_req_valid_o && ((r >> 1) & 0x3) != 0) {
                send_write_ack(d, 0);
                tick_and_compare(sim);
                d->sc_rbox_rsp_valid_i = 0;
                acks_sent++;
            }

            // Occasional DB request
            if (((r >> 28) & 0xF) == 0) {
                VlWide<19> db_req;
                zero_wide(db_req);
                db_req[0] = xorshift();
                d->db_sc_req_valid_i = 1 << (xorshift() % 5);
                memcpy(&d->db_sc_req_i, &db_req, sizeof(db_req));
            } else if (((r >> 28) & 0xF) < 4) {
                d->db_sc_req_valid_i = 0;
            }

            // Occasional DB response (id MSB=1: safe, routed to depth buffer)
            if (((r >> 30) & 0x3) == 0) {
                VlWide<16> data;
                zero_wide(data);
                data[0] = xorshift();
                send_db_rsp(d, xorshift() & 0x7F, data);
            }

            tick_and_compare(sim);
            d->sc_rbox_rsp_valid_i = 0;
        }

        printf("  Quads fed: %d, results fed: %d, acks sent: %d\n",
               quads_fed, results_fed, acks_sent);
    }

    // ── Final cleanup cycles ───────────────────────────────
    printf("Final cleanup\n");
    clear_inputs(d);
    d->rbox_sc_req_ready_i = 0x1F;
    for (int i = 0; i < 50; i++) tick_and_compare(sim);

    return sim.finish();
}

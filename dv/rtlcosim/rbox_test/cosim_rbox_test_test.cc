// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original rbox_test vs new rbox_test.
//
// Exercises every major path of the depth/stencil test unit:
//   - All three DS formats (D32_SFLOAT, D24_UNORM_S8_UINT, D16_UNORM)
//   - All 8 comparison ops for depth
//   - Stencil test + all 8 stencil ops
//   - Depth write enable/disable
//   - Stencil enable/disable
//   - Front vs back triangle (stencil state selection)
//   - MSAA 2x mode
//   - Flush
//   - Cache hit/miss/writeback paths
//   - Random stimulus

#include "Vcosim_rbox_test_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstring>
#include <cstdio>

using DUT = Vcosim_rbox_test_tb;

// ── DS Format / CompOp / StencilOp constants ────────────

enum DsFormat : uint16_t {
    DS_FMT_D16_UNORM         = 124,
    DS_FMT_D32_SFLOAT        = 126,
    DS_FMT_D24_UNORM_S8_UINT = 129,
};

enum CompOp : uint8_t {
    CMP_NEVER           = 0,
    CMP_LESS            = 1,
    CMP_EQUAL           = 2,
    CMP_LESS_OR_EQUAL   = 3,
    CMP_GREATER         = 4,
    CMP_NOT_EQUAL       = 5,
    CMP_GREATER_OR_EQUAL= 6,
    CMP_ALWAYS          = 7,
};

enum StencilOp : uint8_t {
    STENCIL_KEEP      = 0,
    STENCIL_ZERO      = 1,
    STENCIL_REPLACE   = 2,
    STENCIL_INC_CLAMP = 3,
    STENCIL_DEC_CLAMP = 4,
    STENCIL_INVERT    = 5,
    STENCIL_INC_WRAP  = 6,
    STENCIL_DEC_WRAP  = 7,
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

// ── Output comparison ───────────────────────────────────

static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    sim.compare("ts_quad_accept",  d->orig_ts_quad_accept_o,  d->new_ts_quad_accept_o);
    sim.compare("ts_result_valid", d->orig_ts_result_valid_o, d->new_ts_result_valid_o);
    sim.compare("ts_empty",        d->orig_ts_empty_o,        d->new_ts_empty_o);
    sim.compare("db_flush_done",   d->orig_db_flush_done_o,   d->new_db_flush_done_o);

    // Compare result mask when either is valid
    if (d->orig_ts_result_valid_o || d->new_ts_result_valid_o) {
        sim.compare("ts_result_mask", d->orig_ts_result_mask_o, d->new_ts_result_mask_o);
    }

    // Compare ET-Link request valid
    sim.compare("db_sc_req_valid", d->orig_db_sc_req_valid_o, d->new_db_sc_req_valid_o);

    // Compare ET-Link request data when valid
    if (d->orig_db_sc_req_valid_o || d->new_db_sc_req_valid_o) {
        compare_wide(sim, "db_sc_req", d->orig_db_sc_req_o, d->new_db_sc_req_o);
    }

    // Compare debug outputs
    compare_wide(sim, "dbg_db_read_data", d->orig_dbg_db_read_data_o, d->new_dbg_db_read_data_o);

    // Compare debug metadata (lower 38 bits only — original is 38 bits zero-extended to 43)
    sim.compare("dbg_db_meta_lo",
                (uint32_t)(d->orig_dbg_db_meta_read_data_o & 0xFFFFFFFF),
                (uint32_t)(d->new_dbg_db_meta_read_data_o  & 0xFFFFFFFF));
    sim.compare("dbg_db_meta_hi",
                (uint32_t)((d->orig_dbg_db_meta_read_data_o >> 32) & 0x3F),
                (uint32_t)((d->new_dbg_db_meta_read_data_o  >> 32) & 0x3F));
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
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

    // qwen (bits 3:0)
    rsp[0] = (uint32_t)(qwen & 0xF);

    // size (bits 6:4)
    rsp[0] |= (uint32_t)(size & 0x7) << 4;

    // data (bits 518:7) -- 512 bits starting at bit 7
    if (data) {
        uint32_t carry = 0;
        for (int w = 0; w < 16; w++) {
            uint64_t val = ((uint64_t)(*data)[w] << 7) | carry;
            rsp[w] |= (uint32_t)(val & 0xFFFFFFFF);
            carry = (uint32_t)(val >> 32);
        }
        rsp[16] |= carry;
    }

    // opcode (bits 520:519)
    rsp[16] |= (uint32_t)(opcode & 0x3) << (519 - 512);

    // wdata (bit 521)
    if (wdata) rsp[16] |= (1u << (521 - 512));

    // dest (bits 525:522)
    rsp[16] |= (uint32_t)(dest & 0xF) << (522 - 512);

    // id (bits 533:526)
    rsp[16] &= ~((uint32_t)0xFF << 14);
    rsp[16] |= (uint32_t)(id & 0xFF) << 14;
}

// Build an AckData response (fill): opcode=1 (RspAckData), wdata=1
static void build_fill_rsp(VlWide<17>& rsp, uint8_t entry_id,
                            const VlWide<16>& data) {
    build_rsp(rsp, entry_id | 0x80, 0, true, 1 /*RspAckData*/, &data, 6 /*SizeLine*/, 0xF);
}

// Build an Ack response (spill ack): opcode=0 (RspAck), wdata=0
static void build_spill_ack_rsp(VlWide<17>& rsp, uint8_t entry_id) {
    build_rsp(rsp, entry_id | 0x80, 0, false, 0 /*RspAck*/, nullptr, 6 /*SizeLine*/, 0);
}

// ── State configuration helpers ─────────────────────────

struct TestState {
    uint8_t  msaa_enable = 0;
    uint8_t  msaa_samples = 0;
    uint64_t ds_buffer_addr = 0;
    uint16_t ds_buffer_format = DS_FMT_D32_SFLOAT;
    uint8_t  ds_buffer_tiled = 0;
    uint16_t ds_buffer_row_pitch = 0;
    uint8_t  depth_clamp_enable = 0;
    uint8_t  depth_bound_enable = 0;
    uint8_t  depth_test_enable = 1;
    uint8_t  depth_test_write_enable = 1;
    uint8_t  depth_test_compare_op = CMP_LESS;
    uint8_t  stencil_test_enable = 0;
    // Stencil front
    uint8_t  sf_ref_value = 0;
    uint8_t  sf_write_mask = 0xFF;
    uint8_t  sf_compare_mask = 0xFF;
    uint8_t  sf_compare_op = CMP_ALWAYS;
    uint8_t  sf_depth_fail_op = STENCIL_KEEP;
    uint8_t  sf_pass_op = STENCIL_KEEP;
    uint8_t  sf_fail_op = STENCIL_KEEP;
    // Stencil back
    uint8_t  sb_ref_value = 0;
    uint8_t  sb_write_mask = 0xFF;
    uint8_t  sb_compare_mask = 0xFF;
    uint8_t  sb_compare_op = CMP_ALWAYS;
    uint8_t  sb_depth_fail_op = STENCIL_KEEP;
    uint8_t  sb_pass_op = STENCIL_KEEP;
    uint8_t  sb_fail_op = STENCIL_KEEP;
};

static void apply_state(DUT* d, const TestState& s) {
    d->st_msaa_enable_i = s.msaa_enable;
    d->st_msaa_samples_i = s.msaa_samples;
    d->st_ds_buffer_addr_i = s.ds_buffer_addr;
    d->st_ds_buffer_format_i = s.ds_buffer_format;
    d->st_ds_buffer_tiled_i = s.ds_buffer_tiled;
    d->st_ds_buffer_row_pitch_i = s.ds_buffer_row_pitch;
    d->st_depth_clamp_enable_i = s.depth_clamp_enable;
    d->st_depth_bound_enable_i = s.depth_bound_enable;
    d->st_depth_test_enable_i = s.depth_test_enable;
    d->st_depth_test_write_enable_i = s.depth_test_write_enable;
    d->st_depth_test_compare_op_i = s.depth_test_compare_op;
    d->st_stencil_test_enable_i = s.stencil_test_enable;
    d->st_sf_ref_value_i = s.sf_ref_value;
    d->st_sf_write_mask_i = s.sf_write_mask;
    d->st_sf_compare_mask_i = s.sf_compare_mask;
    d->st_sf_compare_op_i = s.sf_compare_op;
    d->st_sf_depth_fail_op_i = s.sf_depth_fail_op;
    d->st_sf_pass_op_i = s.sf_pass_op;
    d->st_sf_fail_op_i = s.sf_fail_op;
    d->st_sb_ref_value_i = s.sb_ref_value;
    d->st_sb_write_mask_i = s.sb_write_mask;
    d->st_sb_compare_mask_i = s.sb_compare_mask;
    d->st_sb_compare_op_i = s.sb_compare_op;
    d->st_sb_depth_fail_op_i = s.sb_depth_fail_op;
    d->st_sb_pass_op_i = s.sb_pass_op;
    d->st_sb_fail_op_i = s.sb_fail_op;
}

static void clear_stimulus(DUT* d) {
    d->db_sc_req_ready_i = 0;
    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
    d->sc_db_rsp_valid_i = 0;
    d->ts_quad_valid_i = 0;
    d->ct_result_accept_i = 0;
    d->ts_front_triangle_i = 0;
    d->db_flush_i = 0;
    d->dbg_db_read_valid_i = 0;
    for (int w = 0; w < 16; w++) d->dbg_db_write_data_i[w] = 0;
    d->dbg_db_write_valid_i = 0;
    d->dbg_db_meta_write_data_i = 0;
    d->dbg_db_meta_write_valid_i = 0;
}

static void init_inputs(DUT* d) {
    clear_stimulus(d);
    d->qt_x_i = 0;
    d->qt_y_i = 0;
    d->qt_sample_idx_i = 0;
    d->qt_mask_i = 0;
    d->qt_depth0_i = 0;
    d->qt_depth1_i = 0;
    d->qt_depth2_i = 0;
    d->qt_depth3_i = 0;
    d->dbg_ram_ptr_i = 0;

    TestState s;
    s.depth_test_enable = 0;
    s.depth_test_write_enable = 0;
    apply_state(d, s);
}

// ── Submit a quad and wait for it to be accepted ────────
// Returns true if accepted within timeout cycles.
static bool submit_quad_wait(CosimCtrl<DUT>& sim, int timeout = 100) {
    auto* d = sim.dut.get();
    for (int i = 0; i < timeout; i++) {
        tick_and_compare(sim);
        if (d->new_ts_quad_accept_o) {
            d->ts_quad_valid_i = 0;
            return true;
        }
    }
    printf("  WARNING: quad not accepted in %d cycles\n", timeout);
    d->ts_quad_valid_i = 0;
    return false;
}

// ── Wait for result valid, accept it, and return the mask ──
static uint8_t wait_result(CosimCtrl<DUT>& sim, int timeout = 500) {
    auto* d = sim.dut.get();
    for (int i = 0; i < timeout; i++) {
        tick_and_compare(sim);
        if (d->new_ts_result_valid_o) {
            uint8_t mask = d->new_ts_result_mask_o;
            d->ct_result_accept_i = 1;
            tick_and_compare(sim);
            d->ct_result_accept_i = 0;
            return mask;
        }
    }
    printf("  WARNING: result not valid in %d cycles\n", timeout);
    return 0xFF;  // sentinel
}

// ── Respond to cache requests (fill/spill model) ────────
// Processes up to max_cycles, responding to any pending requests.
// fill_data_base is used as seed for fill data words.
static void service_cache(CosimCtrl<DUT>& sim, int max_cycles,
                          uint32_t fill_data_base = 0) {
    auto* d = sim.dut.get();
    for (int i = 0; i < max_cycles; i++) {
        d->db_sc_req_ready_i = 1;

        // Check if there's a pending request from the DUT
        if (d->new_db_sc_req_valid_o) {
            // Extract request info from the 584-bit req:
            // req_t layout: id[7:0] at bits [583:576]
            // In VlWide<19>: word 18 bits [7:0]
            uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;

            // opcode at bits [520:519] => word 16 bits [8:7]
            uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;

            VlWide<17> rsp;
            if (opcode == 0) {
                // Read request -> send fill data
                VlWide<16> fill_data;
                for (int w = 0; w < 16; w++)
                    fill_data[w] = fill_data_base + (static_cast<uint32_t>(w) * 0x11111111u);
                build_fill_rsp(rsp, req_id & 0x7F, fill_data);
            } else {
                // Write request -> send ack
                build_spill_ack_rsp(rsp, req_id & 0x7F);
            }
            // Respond next cycle
            tick_and_compare(sim);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
            tick_and_compare(sim);
            d->sc_db_rsp_valid_i = 0;
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        } else {
            tick_and_compare(sim);
        }
    }
}

// ── Full quad submission + cache service + result drain ──
// Submits a quad, services the cache, and drains the result.
// Returns the result mask.
static uint8_t full_quad_cycle(CosimCtrl<DUT>& sim, uint32_t fill_data_base = 0) {
    auto* d = sim.dut.get();

    // Submit the quad
    d->db_sc_req_ready_i = 1;
    d->ts_quad_valid_i = 1;

    // Wait for accept (servicing cache along the way)
    int timeout = 500;
    bool accepted = false;
    for (int i = 0; i < timeout && !accepted; i++) {
        tick_and_compare(sim);
        if (d->new_ts_quad_accept_o) {
            accepted = true;
            d->ts_quad_valid_i = 0;
        }

        // Service cache requests
        if (d->new_db_sc_req_valid_o) {
            uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
            uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;

            VlWide<17> rsp;
            if (opcode == 0) {
                VlWide<16> fill_data;
                for (int w = 0; w < 16; w++)
                    fill_data[w] = fill_data_base + (static_cast<uint32_t>(w) * 0x11111111u);
                build_fill_rsp(rsp, req_id & 0x7F, fill_data);
            } else {
                build_spill_ack_rsp(rsp, req_id & 0x7F);
            }
            tick_and_compare(sim);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
            tick_and_compare(sim);
            d->sc_db_rsp_valid_i = 0;
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        }
    }
    if (!accepted) {
        printf("  WARNING: quad not accepted\n");
        d->ts_quad_valid_i = 0;
    }

    // Now service cache and wait for result
    timeout = 800;
    for (int i = 0; i < timeout; i++) {
        tick_and_compare(sim);

        // Service any cache requests
        if (d->new_db_sc_req_valid_o) {
            uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
            uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;

            VlWide<17> rsp;
            if (opcode == 0) {
                VlWide<16> fill_data;
                for (int w = 0; w < 16; w++)
                    fill_data[w] = fill_data_base + (static_cast<uint32_t>(w) * 0x11111111u);
                build_fill_rsp(rsp, req_id & 0x7F, fill_data);
            } else {
                build_spill_ack_rsp(rsp, req_id & 0x7F);
            }
            tick_and_compare(sim);
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
            d->sc_db_rsp_valid_i = 1;
            tick_and_compare(sim);
            d->sc_db_rsp_valid_i = 0;
            for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        }

        if (d->new_ts_result_valid_o) {
            uint8_t mask = d->new_ts_result_mask_o;
            d->ct_result_accept_i = 1;
            tick_and_compare(sim);
            d->ct_result_accept_i = 0;
            return mask;
        }
    }
    printf("  WARNING: result not obtained\n");
    return 0xFF;
}

// Set quad input data
static void set_quad(DUT* d, uint16_t x, uint16_t y, uint8_t mask,
                     uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
                     uint8_t sample_idx = 0) {
    d->qt_x_i = x & 0x7FFF;
    d->qt_y_i = y & 0x7FFF;
    d->qt_sample_idx_i = sample_idx & 0xF;
    d->qt_mask_i = mask & 0xF;
    d->qt_depth0_i = d0;
    d->qt_depth1_i = d1;
    d->qt_depth2_i = d2;
    d->qt_depth3_i = d3;
}

// ── Main test ───────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000000;  // 2M cycles for thorough testing

    init_inputs(sim.dut.get());
    sim.reset(10);
    compare_outputs(sim);

    auto* d = sim.dut.get();

    // ════════════════════════════════════════════════════
    // Test 1: Reset -- verify idle
    // ════════════════════════════════════════════════════
    printf("test 1: reset idle\n");
    d->db_sc_req_ready_i = 1;
    tick_and_compare(sim);
    tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 2: D32_SFLOAT, depth test LESS -- basic pass
    // ════════════════════════════════════════════════════
    printf("test 2: D32_SFLOAT, LESS -- basic pass\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x10000;
        s.ds_buffer_row_pitch = 1;  // minimal
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_LESS;
        apply_state(d, s);

        // Fragment depths all less than the fill data (0x00000000 < fill_base)
        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0x3F800000);  // fill = 1.0f
        // Depth 0x00100000 < 0x3F800000 (as unsigned comparison in f32)
        // Actually depth_f32_compare uses unsigned comparison so this should pass.
        printf("  result mask: 0x%x\n", mask);
    }
    // Drain a few cycles
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 3: D32_SFLOAT, depth test ALWAYS
    // ════════════════════════════════════════════════════
    printf("test 3: D32_SFLOAT, ALWAYS\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x20000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        set_quad(d, 0, 0, 0xF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  result mask: 0x%x (expect 0xF)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 4: D32_SFLOAT, depth test NEVER
    // ════════════════════════════════════════════════════
    printf("test 4: D32_SFLOAT, NEVER\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x30000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_NEVER;
        apply_state(d, s);

        set_quad(d, 0, 0, 0xF, 0x00000001, 0x00000001, 0x00000001, 0x00000001);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  result mask: 0x%x (expect 0x0)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 5: D24_UNORM_S8_UINT with stencil test
    // ════════════════════════════════════════════════════
    printf("test 5: D24_UNORM_S8_UINT, stencil comparison\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x40000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        // Front stencil: ref=0x80, compare_mask=0xFF, op=LESS
        // stencil_test = (ref & mask) < (in_value & mask)
        // = 0x80 < buf_stencil[31:24]
        // fill data: buf_stencil = fill[31:24] = some byte
        s.sf_ref_value = 0x80;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_LESS;
        s.sf_pass_op = STENCIL_REPLACE;
        s.sf_fail_op = STENCIL_KEEP;
        s.sf_depth_fail_op = STENCIL_KEEP;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        // Fragment depth = 0x00100000 (depth[23:0] = 0x100000)
        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        // Fill data: stencil in buf = fill_base[31:24] of the 32-bit entry at quad offset
        // fill_data_base = 0xFF000000 -> stencil byte = 0xFF
        // Test: 0x80 < 0xFF => true => stencil passes
        uint8_t mask = full_quad_cycle(sim, 0xFF000000);
        printf("  result mask: 0x%x (stencil 0x80 < 0xFF pass)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 6: All 8 stencil ops
    // ════════════════════════════════════════════════════
    printf("test 6: stencil ops (all 8)\n");
    for (int op = 0; op < 8; op++) {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x50000 + op * 0x10000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        s.sf_ref_value = 0x42;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_ALWAYS;  // stencil always passes
        s.sf_pass_op = op;  // test this stencil op
        s.sf_fail_op = STENCIL_KEEP;
        s.sf_depth_fail_op = STENCIL_KEEP;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        set_quad(d, 0, 0, 0xF, 0x00000001, 0x00000001, 0x00000001, 0x00000001);
        uint8_t mask = full_quad_cycle(sim, 0x10000000);
        printf("  stencil op %d: mask=0x%x\n", op, mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 7: D16_UNORM format
    // ════════════════════════════════════════════════════
    printf("test 7: D16_UNORM\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D16_UNORM;
        s.ds_buffer_addr = 0xD0000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_LESS;
        apply_state(d, s);

        // Fragment depth[15:0] = 0x0100, fill data will have some value
        set_quad(d, 0, 0, 0xF, 0x0100, 0x0100, 0x0100, 0x0100);
        uint8_t mask = full_quad_cycle(sim, 0x80008000);
        printf("  D16 result mask: 0x%x\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 8: All 8 comparison ops (D32_SFLOAT)
    // ════════════════════════════════════════════════════
    printf("test 8: all 8 comparison ops\n");
    for (int op = 0; op < 8; op++) {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0xE0000 + op * 0x10000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = op;
        apply_state(d, s);

        // Fragment depth = 0x3F000000 (0.5f), fill ~ 0x3F800000 (1.0f)
        set_quad(d, 0, 0, 0xF, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000);
        uint8_t mask = full_quad_cycle(sim, 0x3F800000);
        printf("  comp_op %d: mask=0x%x\n", op, mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 9: Depth write disable
    // ════════════════════════════════════════════════════
    printf("test 9: depth write disable\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x160000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 0;  // writes disabled
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        set_quad(d, 0, 0, 0xF, 0x3F800000, 0x3F800000, 0x3F800000, 0x3F800000);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  write_disable mask: 0x%x (expect 0xF)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 10: Stencil disable
    // ════════════════════════════════════════════════════
    printf("test 10: stencil disable\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x170000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 0;  // stencil disabled
        apply_state(d, s);

        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  stencil_disable mask: 0x%x (expect 0xF)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 11: MSAA mode 2x
    // ════════════════════════════════════════════════════
    printf("test 11: MSAA 2x\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x180000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_LESS;
        s.msaa_enable = 1;
        s.msaa_samples = 1;  // 2x (mode encoding: 4'd1)
        apply_state(d, s);

        // Sample 0
        set_quad(d, 4, 4, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000, 0);
        uint8_t mask0 = full_quad_cycle(sim, 0x3F800000);
        printf("  MSAA 2x sample 0: mask=0x%x\n", mask0);

        // Sample 1
        set_quad(d, 4, 4, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000, 1);
        uint8_t mask1 = full_quad_cycle(sim, 0x3F800000);
        printf("  MSAA 2x sample 1: mask=0x%x\n", mask1);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 12: Front vs back triangle (stencil state selection)
    // ════════════════════════════════════════════════════
    printf("test 12: front vs back triangle stencil\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x190000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        // Front: ref=0x10, ALWAYS
        s.sf_ref_value = 0x10;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_ALWAYS;
        s.sf_pass_op = STENCIL_REPLACE;
        s.sf_fail_op = STENCIL_KEEP;
        s.sf_depth_fail_op = STENCIL_KEEP;
        // Back: ref=0xF0, NEVER (stencil test fails)
        s.sb_ref_value = 0xF0;
        s.sb_compare_mask = 0xFF;
        s.sb_write_mask = 0xFF;
        s.sb_compare_op = CMP_NEVER;
        s.sb_pass_op = STENCIL_KEEP;
        s.sb_fail_op = STENCIL_ZERO;
        s.sb_depth_fail_op = STENCIL_KEEP;
        apply_state(d, s);

        // Front triangle (should pass stencil)
        d->ts_front_triangle_i = 1;
        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask_front = full_quad_cycle(sim, 0);
        printf("  front triangle: mask=0x%x (expect 0xF)\n", mask_front);

        // Back triangle (stencil fails with NEVER)
        d->ts_front_triangle_i = 0;
        s.ds_buffer_addr = 0x1A0000;  // different address to avoid cache
        apply_state(d, s);
        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask_back = full_quad_cycle(sim, 0);
        printf("  back triangle: mask=0x%x (expect 0x0)\n", mask_back);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    d->ts_front_triangle_i = 0;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 13: Flush
    // ════════════════════════════════════════════════════
    printf("test 13: flush\n");
    {
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;

        int flush_cycles = 0;
        int acks_sent = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            d->db_sc_req_ready_i = 1;

            // Service write-back requests
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
                acks_sent++;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        d->db_flush_i = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);
        printf("  flush completed in %d cycles, %d acks\n", flush_cycles, acks_sent);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 14: Multiple quads to same depth line (cache hit)
    // ════════════════════════════════════════════════════
    printf("test 14: cache hit -- same line\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x200000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        // First quad fetches the line
        set_quad(d, 0, 0, 0xF, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000);
        uint8_t mask0 = full_quad_cycle(sim, 0);
        printf("  first quad: mask=0x%x\n", mask0);

        // Second quad hits the same line (same x/y rounded to line)
        set_quad(d, 0, 0, 0xF, 0x3F400000, 0x3F400000, 0x3F400000, 0x3F400000);
        uint8_t mask1 = full_quad_cycle(sim, 0);
        printf("  second quad (hit): mask=0x%x\n", mask1);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 15: Quads to different depth lines (cache miss -> fetch)
    // ════════════════════════════════════════════════════
    printf("test 15: cache miss -- different lines\n");
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x210000;
        s.ds_buffer_row_pitch = 2;  // larger pitch for different lines
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        // First quad at position (0,0)
        set_quad(d, 0, 0, 0xF, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000);
        uint8_t mask0 = full_quad_cycle(sim, 0);
        printf("  line0 quad: mask=0x%x\n", mask0);

        // Second quad at different Y position -> different cache line
        set_quad(d, 0, 512, 0xF, 0x3F400000, 0x3F400000, 0x3F400000, 0x3F400000);
        uint8_t mask1 = full_quad_cycle(sim, 0x10101010);
        printf("  line1 quad: mask=0x%x\n", mask1);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 16: Depth writeback (dirty line eviction)
    // ════════════════════════════════════════════════════
    printf("test 16: dirty line writeback\n");
    {
        // Flush first to clean slate
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            d->db_sc_req_ready_i = 1;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        d->db_flush_i = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);

        // Now write to many different lines to fill cache and force evictions
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x300000;
        s.ds_buffer_row_pitch = 4;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        // Write quads to unique addresses to populate cache
        for (int i = 0; i < 10; i++) {
            // Each with unique address based on y
            set_quad(d, 0, (uint16_t)(i * 256), 0xF,
                     0x3F000000 + i, 0x3F000000 + i,
                     0x3F000000 + i, 0x3F000000 + i);
            full_quad_cycle(sim, 0);
        }
        printf("  wrote 10 quads to different lines\n");
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 17: Random stimulus (500+ cycles)
    // ════════════════════════════════════════════════════
    printf("test 17: random stimulus (600 cycles)\n");
    {
        uint32_t seed = 0xCAFEBABE;

        // Start with a basic state
        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x400000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_LESS;
        apply_state(d, s);

        for (int i = 0; i < 600; i++) {
            seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5;

            clear_stimulus(d);
            d->db_sc_req_ready_i = (seed & 0x3) != 0;  // 75% ready

            uint8_t action = (seed >> 4) & 0xF;

            if (action < 4) {
                // Submit a quad
                uint16_t qx = (seed >> 8) & 0x3F;     // small coords to hit same lines
                uint16_t qy = (seed >> 14) & 0x3F;
                uint8_t mask = ((seed >> 20) & 0xF) | 1;  // at least 1 bit set
                d->qt_x_i = qx;
                d->qt_y_i = qy;
                d->qt_mask_i = mask;
                d->qt_sample_idx_i = 0;
                d->qt_depth0_i = seed;
                d->qt_depth1_i = seed ^ 0x11111111;
                d->qt_depth2_i = seed ^ 0x22222222;
                d->qt_depth3_i = seed ^ 0x33333333;
                d->ts_quad_valid_i = 1;
            } else if (action < 6) {
                // Accept result
                d->ct_result_accept_i = d->new_ts_result_valid_o ? 1 : 0;
            } else if (action < 8) {
                // Change compare op
                s.depth_test_compare_op = (seed >> 8) & 0x7;
                apply_state(d, s);
            } else if (action < 10) {
                // Service a cache response
                if (d->new_db_sc_req_valid_o) {
                    uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                    uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;

                    VlWide<17> rsp;
                    if (opcode == 0) {
                        VlWide<16> fill_data;
                        for (int w = 0; w < 16; w++)
                            fill_data[w] = seed + (static_cast<uint32_t>(w) * 0x11111111u);
                        build_fill_rsp(rsp, req_id & 0x7F, fill_data);
                    } else {
                        build_spill_ack_rsp(rsp, req_id & 0x7F);
                    }
                    for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                    d->sc_db_rsp_valid_i = 1;
                }
            } else if (action < 12) {
                // Change to D24 + stencil
                s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
                s.stencil_test_enable = 1;
                s.sf_ref_value = (seed >> 8) & 0xFF;
                s.sf_compare_op = (seed >> 16) & 0x7;
                s.sf_pass_op = (seed >> 19) & 0x7;
                d->ts_front_triangle_i = (seed >> 22) & 1;
                apply_state(d, s);
            } else if (action < 14) {
                // Change to D16
                s.ds_buffer_format = DS_FMT_D16_UNORM;
                s.stencil_test_enable = 0;
                apply_state(d, s);
            } else {
                // Idle
            }

            tick_and_compare(sim);
        }

        // Drain: accept all pending results and service cache
        clear_stimulus(d);
        d->db_sc_req_ready_i = 1;
        for (int drain = 0; drain < 200; drain++) {
            if (d->new_ts_result_valid_o)
                d->ct_result_accept_i = 1;
            else
                d->ct_result_accept_i = 0;

            // Service cache
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;
                VlWide<17> rsp;
                if (opcode == 0) {
                    VlWide<16> fill_data;
                    for (int w = 0; w < 16; w++) fill_data[w] = 0xDEAD0000 + w;
                    build_fill_rsp(rsp, req_id & 0x7F, fill_data);
                } else {
                    build_spill_ack_rsp(rsp, req_id & 0x7F);
                }
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
                tick_and_compare(sim);
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }

            tick_and_compare(sim);
        }
    }

    // ── Full drain: accept all pending results and service cache ──
    printf("  draining pipeline after random test...\n");
    {
        // First drain all pending results and service cache
        clear_stimulus(d);
        d->db_sc_req_ready_i = 1;
        for (int drain = 0; drain < 2000; drain++) {
            d->ct_result_accept_i = d->new_ts_result_valid_o ? 1 : 0;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                uint8_t opcode = (d->new_db_sc_req_o[16] >> 7) & 0x3;
                VlWide<17> rsp;
                if (opcode == 0) {
                    VlWide<16> fill_data;
                    for (int w = 0; w < 16; w++) fill_data[w] = 0xABCD0000 + w;
                    build_fill_rsp(rsp, req_id & 0x7F, fill_data);
                } else {
                    build_spill_ack_rsp(rsp, req_id & 0x7F);
                }
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
                tick_and_compare(sim);
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
            tick_and_compare(sim);
        }
        d->ct_result_accept_i = 0;

        // Now flush
        clear_stimulus(d);
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 10; i++) tick_and_compare(sim);
        printf("  drain + flush complete\n");
    }

    // ════════════════════════════════════════════════════
    // Test 18: D24 with all 8 comparison ops
    // ════════════════════════════════════════════════════
    printf("test 18: D24 all comparison ops\n");

    // Flush first
    d->db_flush_i = 1;
    d->db_sc_req_ready_i = 1;
    {
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);
    }

    for (int op = 0; op < 8; op++) {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x500000 + op * 0x10000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = op;
        s.stencil_test_enable = 0;
        apply_state(d, s);

        // frag depth[23:0] = 0x400000, buf depth from fill
        set_quad(d, 0, 0, 0xF, 0x00400000, 0x00400000, 0x00400000, 0x00400000);
        uint8_t mask = full_quad_cycle(sim, 0x00800000);
        printf("  D24 comp_op %d: mask=0x%x\n", op, mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 19: D16 with all 8 comparison ops
    // ════════════════════════════════════════════════════
    printf("test 19: D16 all comparison ops\n");

    // Flush
    d->db_flush_i = 1;
    d->db_sc_req_ready_i = 1;
    {
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);
    }

    for (int op = 0; op < 8; op++) {
        TestState s;
        s.ds_buffer_format = DS_FMT_D16_UNORM;
        s.ds_buffer_addr = 0x600000 + op * 0x10000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = op;
        apply_state(d, s);

        // frag depth[15:0] = 0x4000, fill_base -> buf values
        set_quad(d, 0, 0, 0xF, 0x4000, 0x4000, 0x4000, 0x4000);
        uint8_t mask = full_quad_cycle(sim, 0x80008000);
        printf("  D16 comp_op %d: mask=0x%x\n", op, mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 20: Stencil fail/depth_fail/pass op selection
    // ════════════════════════════════════════════════════
    printf("test 20: stencil op selection paths\n");

    // Flush
    d->db_flush_i = 1;
    d->db_sc_req_ready_i = 1;
    {
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);
    }

    // Path A: stencil pass + depth pass -> pass_op
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x700000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        s.sf_ref_value = 0x42;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_ALWAYS;
        s.sf_pass_op = STENCIL_INC_CLAMP;
        s.sf_fail_op = STENCIL_ZERO;
        s.sf_depth_fail_op = STENCIL_DEC_CLAMP;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0x10000000);
        printf("  pass+pass: mask=0x%x (expect 0xF)\n", mask);
    }

    // Path B: stencil pass + depth fail -> depth_fail_op
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x710000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_NEVER;  // depth always fails
        s.stencil_test_enable = 1;
        s.sf_ref_value = 0x42;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_ALWAYS;
        s.sf_pass_op = STENCIL_INC_CLAMP;
        s.sf_fail_op = STENCIL_ZERO;
        s.sf_depth_fail_op = STENCIL_INVERT;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0x10000000);
        printf("  pass+depth_fail: mask=0x%x (expect 0x0)\n", mask);
    }

    // Path C: stencil fail -> fail_op
    {
        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0x720000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        s.sf_ref_value = 0x42;
        s.sf_compare_mask = 0xFF;
        s.sf_write_mask = 0xFF;
        s.sf_compare_op = CMP_NEVER;  // stencil always fails
        s.sf_pass_op = STENCIL_INC_CLAMP;
        s.sf_fail_op = STENCIL_DEC_WRAP;
        s.sf_depth_fail_op = STENCIL_INVERT;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0x10000000);
        printf("  stencil_fail: mask=0x%x (expect 0x0)\n", mask);
    }

    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    d->ts_front_triangle_i = 0;
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 21: Depth test disabled (all pass)
    // ════════════════════════════════════════════════════
    printf("test 21: depth test disabled\n");
    {
        // Flush
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);

        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x800000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 0;  // disabled
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_NEVER;  // would fail if enabled
        apply_state(d, s);

        set_quad(d, 0, 0, 0xF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  depth_disabled: mask=0x%x (expect 0xF)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 22: Partial mask (some fragments masked out)
    // ════════════════════════════════════════════════════
    printf("test 22: partial mask\n");
    {
        // Flush
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);

        TestState s;
        s.ds_buffer_format = DS_FMT_D32_SFLOAT;
        s.ds_buffer_addr = 0x900000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        apply_state(d, s);

        // Mask = 0x5 (only fragments 0 and 2)
        set_quad(d, 0, 0, 0x5, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000);
        uint8_t mask = full_quad_cycle(sim, 0);
        printf("  partial_mask: mask=0x%x (expect 0x5)\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    for (int i = 0; i < 5; i++) tick_and_compare(sim);

    // ════════════════════════════════════════════════════
    // Test 23: Stencil compare_mask and write_mask
    // ════════════════════════════════════════════════════
    printf("test 23: stencil masks\n");
    {
        d->db_flush_i = 1;
        d->db_sc_req_ready_i = 1;
        int flush_cycles = 0;
        while (!d->new_db_flush_done_o && flush_cycles < 3000) {
            tick_and_compare(sim);
            flush_cycles++;
            if (d->new_db_sc_req_valid_o) {
                uint8_t req_id = d->new_db_sc_req_o[18] & 0xFF;
                VlWide<17> rsp;
                build_spill_ack_rsp(rsp, req_id);
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = rsp[w];
                d->sc_db_rsp_valid_i = 1;
            } else {
                d->sc_db_rsp_valid_i = 0;
                for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
            }
        }
        d->db_flush_i = 0;
        d->sc_db_rsp_valid_i = 0;
        for (int w = 0; w < 17; w++) d->sc_db_rsp_i[w] = 0;
        for (int i = 0; i < 5; i++) tick_and_compare(sim);

        TestState s;
        s.ds_buffer_format = DS_FMT_D24_UNORM_S8_UINT;
        s.ds_buffer_addr = 0xA00000;
        s.ds_buffer_row_pitch = 1;
        s.depth_test_enable = 1;
        s.depth_test_write_enable = 1;
        s.depth_test_compare_op = CMP_ALWAYS;
        s.stencil_test_enable = 1;
        // Only compare lower nibble
        s.sf_ref_value = 0x05;
        s.sf_compare_mask = 0x0F;  // only lower nibble matters
        s.sf_write_mask = 0xF0;    // only write upper nibble
        s.sf_compare_op = CMP_EQUAL;
        s.sf_pass_op = STENCIL_REPLACE;
        s.sf_fail_op = STENCIL_KEEP;
        s.sf_depth_fail_op = STENCIL_KEEP;
        apply_state(d, s);
        d->ts_front_triangle_i = 1;

        // Fill has stencil byte = 0x15 (lower nibble = 5), so (ref & mask) = 5 == (buf & mask) = 5 -> pass
        set_quad(d, 0, 0, 0xF, 0x00100000, 0x00100000, 0x00100000, 0x00100000);
        uint8_t mask = full_quad_cycle(sim, 0x15000000);
        printf("  stencil_masks: mask=0x%x\n", mask);
    }
    clear_stimulus(d);
    d->db_sc_req_ready_i = 1;
    d->ts_front_triangle_i = 0;
    for (int i = 0; i < 10; i++) tick_and_compare(sim);

    // Final drain
    for (int i = 0; i < 20; i++) tick_and_compare(sim);

    return sim.finish();
}

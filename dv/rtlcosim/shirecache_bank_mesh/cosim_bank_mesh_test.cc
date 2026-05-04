// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_mesh vs shirecache_bank_mesh
// Both ToL3 and ToSys configs.

#include "Vcosim_bank_mesh_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using DUT = Vcosim_bank_mesh_tb;

// Compare WData (uint32_t) arrays for wide signals (>64 bit).
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                          const uint32_t* orig, const uint32_t* newv, int words) {
    bool match = memcmp(orig, newv, words * sizeof(uint32_t)) == 0;
    sim.comparisons++;
    if (!match) {
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] %s:", (unsigned long)sim.sim_time, name);
        for (int i = words-1; i >= 0; i--)
            printf(" [%d]=%08x/%08x", i, orig[i], newv[i]);
        printf("\n");
    }
}

// Fill a 512-bit WData[16] array with deterministic non-zero data.
static void fill_wide(uint32_t* arr, uint32_t seed) {
    for (int i = 0; i < 16; i++)
        arr[i] = seed * 2654435761u + i;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();
    // ToL3 handshake + control
    sim.compare("L3:reqq_rdy",       d->l3_orig_reqq_ready_o,       d->l3_new_reqq_ready_o);
    sim.compare("L3:dataq_rdy",      d->l3_orig_dataq_ready_o,      d->l3_new_dataq_ready_o);
    sim.compare("L3:bypass_vic",     d->l3_orig_bypassed_victim_o,  d->l3_new_bypassed_victim_o);
    sim.compare("L3:req_valid",      d->l3_orig_req_valid_o,        d->l3_new_req_valid_o);
    if (d->l3_orig_req_valid_o && d->l3_new_req_valid_o) {
        sim.compare("L3:req_op",     d->l3_orig_req_opcode_o,  d->l3_new_req_opcode_o);
        sim.compare("L3:req_id",     d->l3_orig_req_id_o,      d->l3_new_req_id_o);
        sim.compare("L3:req_src",    d->l3_orig_req_source_o,  d->l3_new_req_source_o);
        sim.compare("L3:req_qos",    d->l3_orig_req_qos_o,     d->l3_new_req_qos_o);
        sim.compare("L3:req_wdata",  d->l3_orig_req_wdata_o,   d->l3_new_req_wdata_o);
        sim.compare("L3:req_qwen",   d->l3_orig_req_qwen_o,    d->l3_new_req_qwen_o);
        sim.compare("L3:req_ben_en", d->l3_orig_req_ben_en_o,  d->l3_new_req_ben_en_o);
        sim.compare("L3:req_size",   d->l3_orig_req_size_o,    d->l3_new_req_size_o);
        sim.compare("L3:req_addr",   d->l3_orig_req_address_o, d->l3_new_req_address_o);
        // Data only valid when wdata=1 AND qwen!=0 (original en_ff_gated_struct
        // stores data per-quadword gated by wdata & qwen[i]).
        // Ben only valid when ben_en=1 (gated by rd_ben).
        if (d->l3_orig_req_wdata_o && d->l3_orig_req_qwen_o)
            compare_wide(sim, "L3:req_data", d->l3_orig_req_data_o, d->l3_new_req_data_o, 16);
        if (d->l3_orig_req_ben_en_o)
            sim.compare("L3:req_ben", d->l3_orig_req_ben_o, d->l3_new_req_ben_o);
    }
    sim.compare("L3:rsp_rdy",        d->l3_orig_rsp_ready_o,        d->l3_new_rsp_ready_o);
    sim.compare("L3:rsp_v_reqq",     d->l3_orig_rsp_valid_reqq_o,   d->l3_new_rsp_valid_reqq_o);
    sim.compare("L3:rsp_v_dataq",    d->l3_orig_rsp_valid_dataq_o,  d->l3_new_rsp_valid_dataq_o);
    sim.compare("L3:rsp_v_rspmux",   d->l3_orig_rsp_valid_rspmux_l2_o, d->l3_new_rsp_valid_rspmux_l2_o);
    sim.compare("L3:rsp_v_l3slv",    d->l3_orig_rsp_valid_l3_slave_o,  d->l3_new_rsp_valid_l3_slave_o);
    if (d->l3_orig_rsp_valid_reqq_o && d->l3_new_rsp_valid_reqq_o) {
        sim.compare("L3:rsp_id",     d->l3_orig_rsp_reqq_id_o, d->l3_new_rsp_reqq_id_o);
        sim.compare("L3:rsp_err",    d->l3_orig_rsp_err_o,     d->l3_new_rsp_err_o);
        // rsp data always valid since test drives mesh_rsp_wdata_i=1
        compare_wide(sim, "L3:rsp_data", d->l3_orig_rsp_data_o, d->l3_new_rsp_data_o, 16);
    }
    // ToSys handshake
    sim.compare("SYS:reqq_rdy",      d->sys_orig_reqq_ready_o,      d->sys_new_reqq_ready_o);
    sim.compare("SYS:dataq_rdy",     d->sys_orig_dataq_ready_o,     d->sys_new_dataq_ready_o);
    sim.compare("SYS:bypass_vic",    d->sys_orig_bypassed_victim_o, d->sys_new_bypassed_victim_o);
    sim.compare("SYS:req_valid",     d->sys_orig_req_valid_o,       d->sys_new_req_valid_o);
    sim.compare("SYS:rsp_rdy",       d->sys_orig_rsp_ready_o,       d->sys_new_rsp_ready_o);
    sim.compare("SYS:rsp_v_reqq",    d->sys_orig_rsp_valid_reqq_o,  d->sys_new_rsp_valid_reqq_o);
    sim.compare("SYS:rsp_v_dataq",   d->sys_orig_rsp_valid_dataq_o, d->sys_new_rsp_valid_dataq_o);
    sim.compare("SYS:rsp_v_rspmux",  d->sys_orig_rsp_valid_rspmux_l2_o, d->sys_new_rsp_valid_rspmux_l2_o);
    sim.compare("SYS:rsp_v_l3slv",   d->sys_orig_rsp_valid_l3_slave_o,  d->sys_new_rsp_valid_l3_slave_o);
}

static void tick_cmp(CosimCtrl<DUT>& sim) { sim.tick(); compare_all(sim); }

static void clear(DUT* d) {
    d->mesh_master_bank_clk_en_i = 1;
    d->my_bank_id_i = 0;
    d->reqq_valid_i = 0; d->dataq_valid_i = 0; d->pipe_valid_i = 0;
    d->mesh_req_ready_i = 1; d->mesh_rsp_valid_i = 0;
    d->rsp_ready_reqq_i = 1; d->rsp_ready_dataq_i = 1;
    d->rsp_ready_rspmux_l2_i = 1; d->rsp_ready_l3_slave_i = 1;
    d->reqq_trans_id_i = 0; d->reqq_reqq_id_i = 0; d->reqq_dataq_id_i = 0;
    memset(d->reqq_data_i, 0, sizeof(d->reqq_data_i));
    d->reqq_with_data_i = 0; d->reqq_rd_ben_i = 0; d->reqq_qwen_i = 0;
    d->reqq_ben_i = 0;
    d->dataq_trans_id_i = 0; d->dataq_reqq_id_i = 0; d->dataq_dataq_id_i = 0;
    memset(d->dataq_data_i, 0, sizeof(d->dataq_data_i));
    d->dataq_with_data_i = 0; d->dataq_rd_ben_i = 0; d->dataq_qwen_i = 0;
    d->dataq_ben_i = 0;
    d->pipe_trans_id_i = 0; d->pipe_reqq_id_i = 0;
    d->pipe_rsp_type_i = 1; memset(d->pipe_data_i, 0, sizeof(d->pipe_data_i));
    d->pipe_mod_dataq_id_i = 0; d->pipe_mod_which_mesh_i = 0;
    d->pipe_mod_disable_bypass_i = 0; d->pipe_mod_redirect_i = 0;
    d->mesh_rsp_trans_id_i = 0; d->mesh_rsp_id_i = 0; d->mesh_rsp_err_i = 0;
    d->mesh_rsp_wdata_i = 0; memset(d->mesh_rsp_data_i, 0, sizeof(d->mesh_rsp_data_i));
    d->esr_l2_bypass_i = 0; d->esr_l3_bypass_i = 0; d->esr_remote_l3_enable_i = 0;
    d->rs_idx0_i = 0; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
    d->rs_reqq_source0_i = 0; d->rs_address_space0_i = 0; d->rs_size0_i = 6;
    d->rs_qos0_i = 0; d->rs_qwen0_i = 0xF;
    d->rs_address0_i = 0;
    d->rs_dataq_id0_i = 0; d->rs_trans_id0_i = 0;
    d->rs_idx1_i = 1; d->rs_opcode1_i = 0x30; d->rs_orig_opcode1_i = 0;
    d->rs_reqq_source1_i = 0; d->rs_address_space1_i = 0; d->rs_size1_i = 6;
    d->rs_qos1_i = 0; d->rs_qwen1_i = 0xF;
    d->rs_address1_i = 0;
    d->rs_dataq_id1_i = 0; d->rs_trans_id1_i = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    auto* d = sim.dut.get();
    clear(d);
    sim.reset(5);
    for (int i = 0; i < 3; i++) tick_cmp(sim);

    printf("Phase 1: idle\n");
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 2: reqq request\n");
    clear(d);
    d->rs_idx0_i = 5; d->rs_opcode0_i = 0x30;
    d->reqq_valid_i = 1; d->reqq_reqq_id_i = 5;
    tick_cmp(sim); d->reqq_valid_i = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 3: dataq request\n");
    clear(d);
    d->rs_idx0_i = 2; d->rs_opcode0_i = 0x31;
    d->dataq_valid_i = 1; d->dataq_reqq_id_i = 2; d->dataq_with_data_i = 1;
    tick_cmp(sim); d->dataq_valid_i = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 4: victim bypass\n");
    clear(d);
    d->pipe_valid_i = 1; d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    tick_cmp(sim);
    d->pipe_mod_which_mesh_i = 1;
    tick_cmp(sim);
    d->pipe_valid_i = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 5: priority\n");
    clear(d);
    d->rs_idx0_i = 0; d->rs_opcode0_i = 0x30;
    d->reqq_valid_i = 1; d->dataq_valid_i = 1; d->dataq_with_data_i = 1;
    d->pipe_valid_i = 1; d->pipe_rsp_type_i = 1; d->pipe_mod_which_mesh_i = 0;
    tick_cmp(sim); d->reqq_valid_i = 0;
    tick_cmp(sim); d->dataq_valid_i = 0;
    for (int i = 0; i < 6; i++) tick_cmp(sim);
    d->pipe_valid_i = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 6: response\n");
    clear(d);
    d->rs_idx0_i = 3; d->rs_opcode0_i = 0x31;
    d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 3;
    tick_cmp(sim); d->mesh_rsp_valid_i = 0;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 7: response reqq+dataq\n");
    clear(d);
    d->rsp_ready_dataq_i = 0;
    d->rs_idx0_i = 7; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
    d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 7;
    tick_cmp(sim); d->mesh_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);
    d->rsp_ready_dataq_i = 1;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 8: backpressure\n");
    clear(d);
    d->mesh_req_ready_i = 0;
    d->rs_idx0_i = 0; d->rs_opcode0_i = 0x30;
    for (int i = 0; i < 4; i++) {
        d->reqq_valid_i = 1; d->reqq_reqq_id_i = i;
        tick_cmp(sim);
    }
    d->reqq_valid_i = 0; d->mesh_req_ready_i = 1;
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    printf("Phase 9: error delay\n");
    clear(d);
    d->rsp_ready_reqq_i = 0;
    d->rs_idx0_i = 12; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
    d->rs_reqq_source0_i = 0;
    d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 12; d->mesh_rsp_err_i = 1;
    tick_cmp(sim); d->mesh_rsp_valid_i = 0;
    for (int i = 0; i < 3; i++) tick_cmp(sim);
    d->rsp_ready_reqq_i = 1;
    for (int i = 0; i < 5; i++) tick_cmp(sim);

    printf("Phase 10: opcode sweep\n");
    uint8_t ops[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};
    for (auto op : ops) {
        clear(d);
        d->rs_idx0_i = 0; d->rs_opcode0_i = op;
        d->reqq_valid_i = 1; d->reqq_reqq_id_i = 0;
        tick_cmp(sim); d->reqq_valid_i = 0;
        for (int i = 0; i < 5; i++) tick_cmp(sim);
    }

    printf("Phase 11: random (500 cycles)\n");
    uint32_t seed = 0xBEEF;
    for (int c = 0; c < 500; c++) {
        seed = seed * 1103515245u + 12345u;
        d->reqq_valid_i = (seed >> 0) & 1;
        d->dataq_valid_i = (seed >> 1) & 1;
        d->pipe_valid_i = (seed >> 2) & 1;
        d->mesh_req_ready_i = ((seed >> 3) & 3) != 0;
        d->mesh_rsp_valid_i = (seed >> 5) & 1;
        d->rsp_ready_reqq_i = ((seed >> 6) & 3) != 0;
        d->rsp_ready_dataq_i = ((seed >> 8) & 3) != 0;
        d->rsp_ready_rspmux_l2_i = ((seed >> 10) & 3) != 0;
        d->rsp_ready_l3_slave_i = ((seed >> 12) & 3) != 0;
        d->my_bank_id_i = (seed >> 14) & 3;
        // Keep reqq IDs in {0,1} so lookups always hit a configured reqq_state entry.
        // The original en_ff_gated_struct only stores data when reqq_state[dataq_id].qwen!=0,
        // and reqq_state is rebuilt combinationally each cycle from rs_idx0/idx1.
        d->rs_idx0_i = 0;
        d->rs_idx1_i = 1;
        d->reqq_reqq_id_i = (seed >> 16) & 1;
        d->dataq_reqq_id_i = (seed >> 22) & 1;
        d->reqq_dataq_id_i = (seed >> 17) & 1;
        d->dataq_dataq_id_i = (seed >> 23) & 1;
        d->reqq_with_data_i = 1;
        d->dataq_with_data_i = 1;
        d->reqq_rd_ben_i = 1;
        d->dataq_rd_ben_i = 1;
        d->pipe_rsp_type_i = (seed >> 30) & 1;
        d->pipe_mod_which_mesh_i = (seed >> 31) & 1;
        d->mesh_rsp_id_i = (seed >> 24) & 1;  // must also be 0 or 1
        d->mesh_rsp_err_i = (seed >> 23) & 1;
        d->mesh_rsp_wdata_i = 1;
        d->rs_opcode0_i = 0x30 + ((seed >> 24) % 9);
        d->rs_opcode1_i = 0x30 + ((seed >> 27) % 9);
        d->rs_orig_opcode0_i = (seed >> 7) & 0x1F;
        d->rs_reqq_source0_i = (seed >> 13) & 3;
        d->rs_address_space0_i = (seed >> 15) & 7;
        d->esr_l2_bypass_i = (seed >> 20) & 1;
        d->esr_l3_bypass_i = (seed >> 21) & 1;
        // Drive non-zero wide/address data
        fill_wide(d->reqq_data_i, seed);
        fill_wide(d->dataq_data_i, seed ^ 0xA5A5A5A5u);
        fill_wide(d->pipe_data_i, seed ^ 0x5A5A5A5Au);
        fill_wide(d->mesh_rsp_data_i, seed ^ 0xDEADBEEFu);
        d->reqq_ben_i = (uint64_t)seed * 0x9E3779B97F4A7C15ULL;
        d->dataq_ben_i = (uint64_t)(seed ^ 0x12345678u) * 0x9E3779B97F4A7C15ULL;
        d->rs_address0_i = (uint64_t)(seed & 0xFFFFFFFFFFULL);
        d->rs_address1_i = (uint64_t)((seed ^ 0x87654321u) & 0xFFFFFFFFFFULL);
        tick_cmp(sim);
    }

    clear(d);
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    printf("Phase 12: address computation with non-zero addresses\n");
    {
        // 12a: Normal reqq request — address should be line-aligned (bits [39:6] ++ 6'b0)
        clear(d);
        uint64_t test_addr = 0x1234567890ULL;
        uint64_t expected_addr = test_addr & ~0x3FULL;  // line-aligned: clear lower 6 bits
        d->rs_address0_i = test_addr;
        d->rs_idx0_i = 5; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
        d->reqq_valid_i = 1; d->reqq_reqq_id_i = 5;
        d->reqq_with_data_i = 1;  // wdata=1 so data is valid
        d->reqq_rd_ben_i = 1;     // ben_en=1 so ben is valid
        // Fill data with a known pattern
        for (int i = 0; i < 16; i++) d->reqq_data_i[i] = 0xCAFE0000u + i;
        d->reqq_ben_i = 0xFFFFFFFFFFFFFFFFULL;
        tick_cmp(sim); d->reqq_valid_i = 0;
        for (int i = 0; i < 5; i++) tick_cmp(sim);

        // 12b: ScpFill — address comes from data[39:6] ++ 6'b0
        clear(d);
        uint64_t scp_fill_data_addr = 0xABCDEF0100ULL;
        uint64_t expected_scp_addr = scp_fill_data_addr & ~0x3FULL;
        d->rs_idx0_i = 3; d->rs_opcode0_i = 0x30;
        d->rs_orig_opcode0_i = 0x18;  // OrigCopScpFill
        d->reqq_valid_i = 1; d->reqq_reqq_id_i = 3;
        // Put the ScpFill address into the low 40 bits of data
        d->reqq_data_i[0] = (uint32_t)(scp_fill_data_addr & 0xFFFFFFFFu);
        d->reqq_data_i[1] = (uint32_t)((scp_fill_data_addr >> 32) & 0xFFu);
        for (int i = 2; i < 16; i++) d->reqq_data_i[i] = 0xDEAD0000u + i;
        d->reqq_ben_i = 0x0123456789ABCDEFULL;
        d->rs_address0_i = 0xFFFFFFFFFFULL;  // should be overridden by ScpFill logic
        tick_cmp(sim); d->reqq_valid_i = 0;
        for (int i = 0; i < 5; i++) tick_cmp(sim);

        // 12c: Second random address, different bank ID
        clear(d);
        d->my_bank_id_i = 2;
        d->rs_address0_i = 0x9876543210ULL;
        d->rs_idx0_i = 1; d->rs_opcode0_i = 0x31;
        d->dataq_valid_i = 1; d->dataq_reqq_id_i = 1; d->dataq_with_data_i = 1;
        fill_wide(d->dataq_data_i, 0xFACEFEED);
        d->dataq_ben_i = 0xAAAAAAAAAAAAAAAAULL;
        tick_cmp(sim); d->dataq_valid_i = 0;
        for (int i = 0; i < 5; i++) tick_cmp(sim);
    }

    printf("Phase 13: response data passthrough with non-zero data\n");
    {
        // 13a: Push a mesh response with non-zero data, hold rsp_ready_reqq_i low
        clear(d);
        d->rsp_ready_reqq_i = 0;
        d->rs_idx0_i = 9; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
        d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 9;
        d->mesh_rsp_wdata_i = 1;  // wdata=1 so data is valid
        // Fill response data with a known pattern
        for (int i = 0; i < 16; i++) d->mesh_rsp_data_i[i] = 0xBEEF0000u + i;
        tick_cmp(sim);
        // Response should stay visible while rsp_ready_reqq_i is low
        d->mesh_rsp_valid_i = 0;
        for (int i = 0; i < 3; i++) tick_cmp(sim);
        // Now release the ready and let it drain
        d->rsp_ready_reqq_i = 1;
        for (int i = 0; i < 5; i++) tick_cmp(sim);

        // 13b: Another response with different data pattern, check dataq path
        clear(d);
        d->rsp_ready_dataq_i = 0;
        d->rs_idx0_i = 4; d->rs_opcode0_i = 0x31; d->rs_orig_opcode0_i = 0;
        d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 4; d->mesh_rsp_wdata_i = 1;
        for (int i = 0; i < 16; i++) d->mesh_rsp_data_i[i] = 0x12340000u + (i * 0x1111u);
        tick_cmp(sim);
        d->mesh_rsp_valid_i = 0;
        for (int i = 0; i < 3; i++) tick_cmp(sim);
        d->rsp_ready_dataq_i = 1;
        for (int i = 0; i < 5; i++) tick_cmp(sim);

        // 13c: Response with error and non-zero data
        clear(d);
        d->rsp_ready_reqq_i = 0;
        d->rs_idx0_i = 15; d->rs_opcode0_i = 0x30; d->rs_orig_opcode0_i = 0;
        d->mesh_rsp_valid_i = 1; d->mesh_rsp_id_i = 15; d->mesh_rsp_err_i = 1;
        d->mesh_rsp_wdata_i = 1;
        for (int i = 0; i < 16; i++) d->mesh_rsp_data_i[i] = 0xFF000000u | (i << 16) | i;
        tick_cmp(sim);
        d->mesh_rsp_valid_i = 0;
        for (int i = 0; i < 3; i++) tick_cmp(sim);
        d->rsp_ready_reqq_i = 1;
        for (int i = 0; i < 5; i++) tick_cmp(sim);
    }

    clear(d);
    for (int i = 0; i < 10; i++) tick_cmp(sim);

    sim.finish();
    return 0;
}

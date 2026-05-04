// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Cosim: shire_cache_bank_dataq vs shirecache_bank_dataq

#include "Vcosim_dataq_tb.h"
#include "cosim_ctrl.h"
#include <cstdio>
#include <cstring>

using DUT = Vcosim_dataq_tb;

// ET-Link request opcodes
static constexpr int REQ_WRITE        = 0;
static constexpr int REQ_WRITE_AROUND = 2;
static constexpr int REQ_MSG_SEND     = 4;
static constexpr int REQ_ATOMIC       = 5;
static constexpr int REQ_FLUSH_TO_MEM = 9;
static constexpr int REQ_EVICT_TO_MEM = 11;
static constexpr int REQ_SCP_FILL     = 14;

// ET-Link size
static constexpr int SIZE_LINE = 0x6;  // 3'b110

// ── Helpers ────────────────────────────────────────────────────

static void clear_inputs(DUT* dut) {
    // Pipeline write
    dut->pipe_data_rsp_valid_i = 0;
    dut->mesh_bypassed_victim_i = 0;
    dut->rspmux_bypassed_data_rsp_i = 0;

    // Bank L2
    dut->bank_l2_valid_i = 0;
    dut->bank_l2_ready_i = 0;

    // to_l3 mesh
    dut->to_l3_mesh_rsp_valid_i = 0;

    // Bank L3
    dut->bank_l3_valid_i = 0;
    dut->bank_l3_ready_i = 0;

    // to_sys mesh
    dut->to_sys_mesh_rsp_valid_i = 0;

    // Pipeline read
    dut->pipe_dataq_rd_req_valid_i = 0;

    // Read sink inputs
    dut->reqq_to_l3_mesh_req_valid_wdata_i = 0;
    dut->reqq_rspmux_l2_valid_wdata_i = 0;
    dut->reqq_to_sys_mesh_req_valid_wdata_i = 0;
    dut->reqq_rspmux_l3_valid_wdata_i = 0;

    // Downstream ready
    dut->dataq_to_l3_mesh_req_ready_i = 1;
    dut->dataq_rspmux_l2_ready_i = 1;
    dut->dataq_to_sys_mesh_req_ready_i = 1;
    dut->dataq_rspmux_l3_ready_i = 1;
}

// Fill a VlWide<16> (512 bits) with a pattern seeded by `seed`.
static void fill_data(VlWide<16>& w, uint32_t seed) {
    for (int i = 0; i < 16; i++) {
        w[i] = seed * 0x01010101u + i * 0x13579BDFu;
    }
}

// Compare two VlWide<16> arrays
static bool data_eq(const VlWide<16>& a, const VlWide<16>& b) {
    return memcmp(&a, &b, sizeof(VlWide<16>)) == 0;
}

// Compare wide data with name logging
static void compare_data(CosimCtrl<DUT>& sim, const char* name,
                         const VlWide<16>& orig, const VlWide<16>& newv) {
    sim.comparisons++;
    if (!data_eq(orig, newv)) {
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] %s\n    orig=", (unsigned long)sim.sim_time, name);
        for (int i = 15; i >= 0; i--) printf("%08x", orig[i]);
        printf("\n    new =");
        for (int i = 15; i >= 0; i--) printf("%08x", newv[i]);
        printf("\n");
    }
}

// ── Pipe read tracking ────────────────────────────────────────
// The pipe_dataq_rd_rsp outputs are driven by retained register values
// when no read is in flight. The data comes from RAM which may differ
// between latch-RAM (original) and FF-RAM (new) when uninitialized.
// We track when a pipe read was issued and compare data 3 cycles later.
//
// Shift register: bit 0 = current cycle read issued, shifts left each cycle.
// When bit 3 is set, the response data is valid.
static uint32_t pipe_rd_sr = 0;

// Call this after driving inputs (before tick) to mark a pipe read.
static void mark_pipe_read() { pipe_rd_sr |= 1; }

// ── Comparison ─────────────────────────────────────────────────

static void compare_ctrl(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    // Ready signals
    sim.compare("bank_l2_ready",       d->orig_dataq_bank_l2_ready_o,       d->new_dataq_bank_l2_ready_o);
    sim.compare("bank_l3_ready",       d->orig_dataq_bank_l3_ready_o,       d->new_dataq_bank_l3_ready_o);
    sim.compare("l3_mesh_rsp_ready",   d->orig_to_l3_mesh_rsp_ready_o,      d->new_to_l3_mesh_rsp_ready_o);
    sim.compare("sys_mesh_rsp_ready",  d->orig_to_sys_mesh_rsp_ready_o,     d->new_to_sys_mesh_rsp_ready_o);

    // Write status
    sim.compare("wr_status_any",       d->orig_dataq_write_status_any_o,    d->new_dataq_write_status_any_o);
    sim.compare("wr_status_id",        d->orig_dataq_write_status_id_o,     d->new_dataq_write_status_id_o);
    sim.compare("wr_status_hot",       d->orig_dataq_write_status_hot_o,    d->new_dataq_write_status_hot_o);
    sim.compare("rd_status_hot",       d->orig_dataq_read_status_hot_o,     d->new_dataq_read_status_hot_o);

    // Read sink ready signals
    sim.compare("l3_mesh_rd_ready",    d->orig_reqq_to_l3_mesh_req_ready_wdata_o,  d->new_reqq_to_l3_mesh_req_ready_wdata_o);
    sim.compare("rspmux_l2_rd_ready",  d->orig_reqq_rspmux_l2_ready_wdata_o,       d->new_reqq_rspmux_l2_ready_wdata_o);
    sim.compare("sys_mesh_rd_ready",   d->orig_reqq_to_sys_mesh_req_ready_wdata_o,  d->new_reqq_to_sys_mesh_req_ready_wdata_o);
    sim.compare("rspmux_l3_rd_ready",  d->orig_reqq_rspmux_l3_ready_wdata_o,       d->new_reqq_rspmux_l3_ready_wdata_o);

    // Pipeline read response: compare data/ben only when response is expected
    // (3 cycles after the pipe read request — bit 3 of the shift register).
    if (pipe_rd_sr & 0x8) {
        sim.compare("pipe_rsp_trans_id", d->orig_pipe_rd_rsp_trans_id_o, d->new_pipe_rd_rsp_trans_id_o);
        sim.compare("pipe_rsp_reqq_id",  d->orig_pipe_rd_rsp_reqq_id_o,  d->new_pipe_rd_rsp_reqq_id_o);
        sim.compare("pipe_rsp_err",      d->orig_pipe_rd_rsp_err_o,      d->new_pipe_rd_rsp_err_o);
        compare_data(sim, "pipe_rsp_data", d->orig_pipe_rd_rsp_data_o,   d->new_pipe_rd_rsp_data_o);
        sim.compare("pipe_rsp_ben",      d->orig_pipe_rd_rsp_ben_o,      d->new_pipe_rd_rsp_ben_o);
    }
    // Advance the shift register
    pipe_rd_sr <<= 1;

    // to_l3_mesh read output
    sim.compare("l3_mesh_req_valid",   d->orig_dataq_to_l3_mesh_req_valid_o,      d->new_dataq_to_l3_mesh_req_valid_o);
    if (d->orig_dataq_to_l3_mesh_req_valid_o || d->new_dataq_to_l3_mesh_req_valid_o) {
        sim.compare("l3_mesh_req_tid",    d->orig_dataq_to_l3_mesh_req_trans_id_o,  d->new_dataq_to_l3_mesh_req_trans_id_o);
        sim.compare("l3_mesh_req_rid",    d->orig_dataq_to_l3_mesh_req_reqq_id_o,   d->new_dataq_to_l3_mesh_req_reqq_id_o);
        compare_data(sim, "l3_mesh_req_data", d->orig_dataq_to_l3_mesh_req_data_o,  d->new_dataq_to_l3_mesh_req_data_o);
        sim.compare("l3_mesh_req_ben",    d->orig_dataq_to_l3_mesh_req_ben_o,       d->new_dataq_to_l3_mesh_req_ben_o);
    }

    // rspmux_l2
    sim.compare("rspmux_l2_valid",     d->orig_dataq_rspmux_l2_valid_o,      d->new_dataq_rspmux_l2_valid_o);
    if (d->orig_dataq_rspmux_l2_valid_o || d->new_dataq_rspmux_l2_valid_o) {
        sim.compare("rspmux_l2_tid",   d->orig_dataq_rspmux_l2_trans_id_o,   d->new_dataq_rspmux_l2_trans_id_o);
        sim.compare("rspmux_l2_rid",   d->orig_dataq_rspmux_l2_reqq_id_o,    d->new_dataq_rspmux_l2_reqq_id_o);
        compare_data(sim, "rspmux_l2_data", d->orig_dataq_rspmux_l2_data_o,  d->new_dataq_rspmux_l2_data_o);
    }

    // to_sys_mesh
    sim.compare("sys_mesh_req_valid",  d->orig_dataq_to_sys_mesh_req_valid_o,      d->new_dataq_to_sys_mesh_req_valid_o);
    if (d->orig_dataq_to_sys_mesh_req_valid_o || d->new_dataq_to_sys_mesh_req_valid_o) {
        sim.compare("sys_mesh_req_tid", d->orig_dataq_to_sys_mesh_req_trans_id_o,   d->new_dataq_to_sys_mesh_req_trans_id_o);
        sim.compare("sys_mesh_req_rid", d->orig_dataq_to_sys_mesh_req_reqq_id_o,    d->new_dataq_to_sys_mesh_req_reqq_id_o);
        compare_data(sim, "sys_mesh_req_data", d->orig_dataq_to_sys_mesh_req_data_o, d->new_dataq_to_sys_mesh_req_data_o);
        sim.compare("sys_mesh_req_ben", d->orig_dataq_to_sys_mesh_req_ben_o,        d->new_dataq_to_sys_mesh_req_ben_o);
    }

    // rspmux_l3
    sim.compare("rspmux_l3_valid",     d->orig_dataq_rspmux_l3_valid_o,      d->new_dataq_rspmux_l3_valid_o);
    if (d->orig_dataq_rspmux_l3_valid_o || d->new_dataq_rspmux_l3_valid_o) {
        sim.compare("rspmux_l3_tid",   d->orig_dataq_rspmux_l3_trans_id_o,   d->new_dataq_rspmux_l3_trans_id_o);
        sim.compare("rspmux_l3_rid",   d->orig_dataq_rspmux_l3_reqq_id_o,    d->new_dataq_rspmux_l3_reqq_id_o);
        compare_data(sim, "rspmux_l3_data", d->orig_dataq_rspmux_l3_data_o,  d->new_dataq_rspmux_l3_data_o);
    }
}

// ── Stimulus helpers ───────────────────────────────────────────

// Write via pipeline path: one-cycle pulse
static void pipe_write(DUT* d, int reqq_id, int dataq_id, uint32_t seed) {
    d->pipe_data_rsp_valid_i = 1;
    d->pipe_data_rsp_reqq_id_i = reqq_id;
    d->pipe_data_rsp_trans_id_i = seed;
    d->pipe_data_rsp_qwen_i = 0xF;  // all QW
    fill_data(d->pipe_data_rsp_data_i, seed);
    d->pipe_data_rsp_mod_dataq_id_i = dataq_id;
    d->mesh_bypassed_victim_i = 0;
    d->rspmux_bypassed_data_rsp_i = 0;
}

// Write via to_l3_mesh path
static void l3_mesh_write(DUT* d, int reqq_id, int dataq_id, uint32_t seed) {
    d->to_l3_mesh_rsp_valid_i = 1;
    d->to_l3_mesh_rsp_trans_id_i = seed;
    d->to_l3_mesh_rsp_reqq_id_i = reqq_id;
    d->to_l3_mesh_rsp_dataq_id_i = dataq_id;
    fill_data(d->to_l3_mesh_rsp_data_i, seed);
}

// Write via to_sys_mesh path
static void sys_mesh_write(DUT* d, int reqq_id, int dataq_id, uint32_t seed) {
    d->to_sys_mesh_rsp_valid_i = 1;
    d->to_sys_mesh_rsp_trans_id_i = seed;
    d->to_sys_mesh_rsp_reqq_id_i = reqq_id;
    d->to_sys_mesh_rsp_dataq_id_i = dataq_id;
    fill_data(d->to_sys_mesh_rsp_data_i, seed);
}

// Write via bank_l2 path (ReqWrite with wdata)
static void bank_l2_write(DUT* d, int alloc_id, uint32_t seed) {
    d->bank_l2_valid_i = 1;
    d->bank_l2_ready_i = 1;
    d->reqq_alloc_l2_id_ag_i = alloc_id;
    d->bank_l2_trans_id_i = seed;
    d->bank_l2_opcode_i = REQ_WRITE;
    d->bank_l2_subopcode_i = 0;
    d->bank_l2_address_i = 0;
    d->bank_l2_wdata_i = 1;
    fill_data(d->bank_l2_data_i, seed);
    d->bank_l2_ben_i = UINT64_C(0xFFFFFFFFFFFFFFFF);
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = SIZE_LINE;
    d->bank_l2_write_is_partial_i = 0;
}

// Write via bank_l3 path (ReqWrite with wdata)
static void bank_l3_write(DUT* d, int alloc_id, uint32_t seed) {
    d->bank_l3_valid_i = 1;
    d->bank_l3_ready_i = 1;
    d->reqq_alloc_l3_id_ag_i = alloc_id;
    d->bank_l3_trans_id_i = seed;
    d->bank_l3_opcode_i = REQ_WRITE;
    fill_data(d->bank_l3_data_i, seed);
    d->bank_l3_ben_i = UINT64_C(0xFFFFFFFFFFFFFFFF);
    d->bank_l3_qwen_i = 0xF;
    d->bank_l3_size_i = SIZE_LINE;
    d->bank_l3_wdata_i = 1;
    d->bank_l3_write_is_partial_i = 0;
}

// Pipeline read
static void pipe_read(DUT* d, int reqq_id, uint32_t trans_id) {
    d->pipe_dataq_rd_req_valid_i = 1;
    d->pipe_dataq_rd_req_trans_id_i = trans_id;
    d->pipe_dataq_rd_req_reqq_id_i = reqq_id;
    d->pipe_dataq_rd_req_partial_i = 0;
    d->pipe_dataq_rd_req_qwen_i = 0xF;
    mark_pipe_read();
}

// Read via to_l3_mesh
static void l3_mesh_read(DUT* d, int reqq_id, int dataq_id, uint32_t tid) {
    d->reqq_to_l3_mesh_req_valid_wdata_i = 1;
    d->reqq_to_l3_mesh_req_trans_id_i = tid;
    d->reqq_to_l3_mesh_req_reqq_id_i = reqq_id;
    d->reqq_to_l3_mesh_req_dataq_id_i = dataq_id;
    d->reqq_to_l3_mesh_req_qwen_i = 0xF;
    d->reqq_to_l3_mesh_req_rd_ben_i = 0;
}

// Read via rspmux_l2
static void rspmux_l2_read(DUT* d, int reqq_id, uint32_t tid) {
    d->reqq_rspmux_l2_valid_wdata_i = 1;
    d->reqq_rspmux_l2_trans_id_i = tid;
    d->reqq_rspmux_l2_reqq_id_i = reqq_id;
    d->reqq_rspmux_l2_qwen_i = 0xF;
}

// Read via to_sys_mesh
static void sys_mesh_read(DUT* d, int reqq_id, int dataq_id, uint32_t tid) {
    d->reqq_to_sys_mesh_req_valid_wdata_i = 1;
    d->reqq_to_sys_mesh_req_trans_id_i = tid;
    d->reqq_to_sys_mesh_req_reqq_id_i = reqq_id;
    d->reqq_to_sys_mesh_req_dataq_id_i = dataq_id;
    d->reqq_to_sys_mesh_req_qwen_i = 0xF;
    d->reqq_to_sys_mesh_req_rd_ben_i = 0;
}

// Read via rspmux_l3
static void rspmux_l3_read(DUT* d, int reqq_id, uint32_t tid) {
    d->reqq_rspmux_l3_valid_wdata_i = 1;
    d->reqq_rspmux_l3_trans_id_i = tid;
    d->reqq_rspmux_l3_reqq_id_i = reqq_id;
    d->reqq_rspmux_l3_qwen_i = 0xF;
}

// Idle one cycle with comparison
static void idle_tick(CosimCtrl<DUT>& sim) {
    clear_inputs(sim.dut.get());
    sim.tick();
    compare_ctrl(sim);
}

// ── Main ───────────────────────────────────────────────────────

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000000;
    auto* d = sim.dut.get();

    clear_inputs(d);
    sim.reset();

    // Let the reset propagate
    for (int i = 0; i < 5; i++) idle_tick(sim);

    // ── Phase 1: Pipeline write then pipeline read ───────────
    printf("cosim phase 1: pipe write entry 0, then pipe read\n");
    pipe_write(d, 0, 0, 0xAA);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);

    // Wait for write to propagate (registered)
    for (int i = 0; i < 3; i++) idle_tick(sim);

    // Pipeline read entry 0
    pipe_read(d, 0, 0x100);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);

    // Read response is 3 cycles after request
    for (int i = 0; i < 5; i++) idle_tick(sim);
    printf("  phase 1 done\n");

    // ── Phase 2: Pipeline write multiple entries ─────────────
    printf("cosim phase 2: pipe write entries 1-3\n");
    for (int ent = 1; ent <= 3; ent++) {
        pipe_write(d, ent, ent, 0xBB + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    // Read them back
    for (int ent = 1; ent <= 3; ent++) {
        idle_tick(sim);
        pipe_read(d, ent, 0x200 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 2 done\n");

    // ── Phase 3: to_l3_mesh write then l3_mesh read ──────────
    printf("cosim phase 3: to_l3_mesh write entry 4, l3_mesh read back\n");
    l3_mesh_write(d, 4, 4, 0xCC);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    // Read via to_l3_mesh read port
    l3_mesh_read(d, 4, 4, 0x300);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    // Wait for data to appear at output
    for (int i = 0; i < 8; i++) idle_tick(sim);
    printf("  phase 3 done\n");

    // ── Phase 4: to_sys_mesh write then sys_mesh read ────────
    printf("cosim phase 4: to_sys_mesh write entry 5, sys_mesh read back\n");
    sys_mesh_write(d, 5, 5, 0xDD);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    sys_mesh_read(d, 5, 5, 0x400);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) idle_tick(sim);
    printf("  phase 4 done\n");

    // ── Phase 5: bank_l2 write then rspmux_l2 read ──────────
    printf("cosim phase 5: bank_l2 write entry 6, rspmux_l2 read back\n");
    bank_l2_write(d, 6, 0xEE);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    rspmux_l2_read(d, 6, 0x500);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) idle_tick(sim);
    printf("  phase 5 done\n");

    // ── Phase 6: bank_l3 write then rspmux_l3 read ──────────
    printf("cosim phase 6: bank_l3 write entry 7, rspmux_l3 read back\n");
    bank_l3_write(d, 7, 0xFF);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    rspmux_l3_read(d, 7, 0x600);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) idle_tick(sim);
    printf("  phase 6 done\n");

    // ── Phase 7: Pipeline write with bypass flags ────────────
    printf("cosim phase 7: pipe write with rspmux bypass (should not write)\n");
    pipe_write(d, 8, 8, 0x11);
    d->rspmux_bypassed_data_rsp_i = 1;  // bypass -> no write
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) idle_tick(sim);
    printf("  phase 7 done\n");

    printf("cosim phase 7b: pipe write with mesh bypass (should not write)\n");
    pipe_write(d, 9, 9, 0x22);
    d->mesh_bypassed_victim_i = 1;  // bypass -> no write
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) idle_tick(sim);
    printf("  phase 7b done\n");

    // ── Phase 8: Concurrent writes on multiple paths ─────────
    printf("cosim phase 8: simultaneous pipe + l3_mesh + bank_l2 writes\n");
    pipe_write(d, 10, 10, 0x33);
    l3_mesh_write(d, 11, 11, 0x44);
    bank_l2_write(d, 12, 0x55);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    // Let them drain
    for (int i = 0; i < 10; i++) idle_tick(sim);

    // Read all back via pipeline
    for (int ent = 10; ent <= 12; ent++) {
        pipe_read(d, ent, 0x700 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 8 done\n");

    // ── Phase 9: Fill write FIFOs to test backpressure ───────
    printf("cosim phase 9: fill l3_mesh write FIFO (backpressure test)\n");
    // FIFO depth is 2, so 3rd write should see full
    l3_mesh_write(d, 13, 13, 0x66);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);

    l3_mesh_write(d, 14, 14, 0x77);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);

    // Third should see ready=0 (FIFO full)
    l3_mesh_write(d, 15, 15, 0x88);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);

    // Drain
    for (int i = 0; i < 15; i++) idle_tick(sim);
    printf("  phase 9 done\n");

    // ── Phase 10: Concurrent reads from multiple sinks ───────
    printf("cosim phase 10: write 4 entries, read from all 4 sink paths\n");
    // Write entries 16-19 via pipeline
    for (int ent = 16; ent <= 19; ent++) {
        pipe_write(d, ent, ent, 0x90 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    for (int i = 0; i < 3; i++) idle_tick(sim);

    // Simultaneously read from all 4 non-pipe read ports
    l3_mesh_read(d, 16, 16, 0xA10);
    rspmux_l2_read(d, 17, 0xA11);
    sys_mesh_read(d, 18, 18, 0xA12);
    rspmux_l3_read(d, 19, 0xA13);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    // Only one can win arbitration per cycle; let them drain
    for (int i = 0; i < 20; i++) idle_tick(sim);
    printf("  phase 10 done\n");

    // ── Phase 11: Read with downstream backpressure ──────────
    printf("cosim phase 11: read with downstream backpressure\n");
    // Write entry 20 via pipeline
    pipe_write(d, 20, 20, 0xB0);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 3; i++) idle_tick(sim);

    // Read via to_l3_mesh but hold ready=0
    d->dataq_to_l3_mesh_req_ready_i = 0;
    l3_mesh_read(d, 20, 20, 0xC00);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    d->dataq_to_l3_mesh_req_ready_i = 0;
    for (int i = 0; i < 5; i++) {
        d->dataq_to_l3_mesh_req_ready_i = 0;
        idle_tick(sim);
    }
    // Now release
    d->dataq_to_l3_mesh_req_ready_i = 1;
    for (int i = 0; i < 5; i++) idle_tick(sim);
    printf("  phase 11 done\n");

    // ── Phase 12: Bank L2 partial write (with BEN) ───────────
    printf("cosim phase 12: bank_l2 partial write with BEN\n");
    d->bank_l2_valid_i = 1;
    d->bank_l2_ready_i = 1;
    d->reqq_alloc_l2_id_ag_i = 21;
    d->bank_l2_trans_id_i = 0xD00;
    d->bank_l2_opcode_i = REQ_WRITE;
    d->bank_l2_subopcode_i = 0;
    d->bank_l2_address_i = 0;
    d->bank_l2_wdata_i = 1;
    fill_data(d->bank_l2_data_i, 0xD1);
    // Partial: only some bytes enabled
    d->bank_l2_ben_i = UINT64_C(0x0F0F0F0F0F0F0F0F);
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = SIZE_LINE;
    d->bank_l2_write_is_partial_i = 1;
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    // Read back via pipeline to compare data+ben
    pipe_read(d, 21, 0xD02);
    d->pipe_dataq_rd_req_partial_i = 1;
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);
    printf("  phase 12 done\n");

    // ── Phase 13: Rapid fire pipeline writes+reads ───────────
    printf("cosim phase 13: rapid-fire pipe writes then reads\n");
    for (int ent = 22; ent <= 31; ent++) {
        pipe_write(d, ent, ent, 0xE00 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    for (int i = 0; i < 5; i++) idle_tick(sim);
    for (int ent = 22; ent <= 31; ent++) {
        pipe_read(d, ent, 0xF00 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 13 done\n");

    // ── Phase 14: Write via all 4 non-pipe paths in sequence ─
    printf("cosim phase 14: write via all 4 non-pipe paths, read via pipe\n");
    // bank_l2 -> entry 32
    bank_l2_write(d, 32, 0x1000);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) idle_tick(sim);

    // to_l3_mesh -> entry 33
    l3_mesh_write(d, 33, 33, 0x1100);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) idle_tick(sim);

    // bank_l3 -> entry 34
    bank_l3_write(d, 34, 0x1200);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) idle_tick(sim);

    // to_sys_mesh -> entry 35
    sys_mesh_write(d, 35, 35, 0x1300);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 4; i++) idle_tick(sim);

    // Read all back via pipe
    for (int ent = 32; ent <= 35; ent++) {
        pipe_read(d, ent, 0x1400 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 14 done\n");

    // ── Phase 15: Pipeline write priority over non-pipe ──────
    printf("cosim phase 15: pipe write has priority over non-pipe write\n");
    // Simultaneously issue pipe write and l3_mesh write
    pipe_write(d, 36, 36, 0x1500);
    l3_mesh_write(d, 37, 37, 0x1600);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    // Read both back
    pipe_read(d, 36, 0x1700);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    pipe_read(d, 37, 0x1800);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);
    printf("  phase 15 done\n");

    // ── Phase 16: Read via l3_mesh with rd_ben ───────────────
    printf("cosim phase 16: l3_mesh read with rd_ben=1\n");
    // Write entry 38 with BEN via bank_l2 partial
    d->bank_l2_valid_i = 1;
    d->bank_l2_ready_i = 1;
    d->reqq_alloc_l2_id_ag_i = 38;
    d->bank_l2_trans_id_i = 0x1900;
    d->bank_l2_opcode_i = REQ_WRITE;
    d->bank_l2_subopcode_i = 0;
    d->bank_l2_address_i = 0;
    d->bank_l2_wdata_i = 1;
    fill_data(d->bank_l2_data_i, 0x1A);
    d->bank_l2_ben_i = UINT64_C(0x55555555AAAAAAAA);
    d->bank_l2_qwen_i = 0xF;
    d->bank_l2_size_i = SIZE_LINE;
    d->bank_l2_write_is_partial_i = 1;
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);

    // Read with rd_ben=1 (should include ben in output)
    d->reqq_to_l3_mesh_req_valid_wdata_i = 1;
    d->reqq_to_l3_mesh_req_trans_id_i = 0x1B00;
    d->reqq_to_l3_mesh_req_reqq_id_i = 38;
    d->reqq_to_l3_mesh_req_dataq_id_i = 38;
    d->reqq_to_l3_mesh_req_qwen_i = 0xF;
    d->reqq_to_l3_mesh_req_rd_ben_i = 1;
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 10; i++) idle_tick(sim);
    printf("  phase 16 done\n");

    // ── Phase 17: Bank L3 write path ───────────────────────
    printf("cosim phase 17: bank_l3 write + rspmux_l3 read\n");
    bank_l3_write(d, 39, 0x2000);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);
    rspmux_l3_read(d, 39, 0x2001);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 8; i++) idle_tick(sim);
    printf("  phase 17 done\n");

    // ── Phase 18: Zero-data writes ──────────────────────────
    printf("cosim phase 18: zero-data pipe write\n");
    {
        // Write with zero_data flag via l3_mesh (it has the zero_data field)
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_trans_id_i = 0x2100;
        d->to_l3_mesh_rsp_reqq_id_i = 40;
        d->to_l3_mesh_rsp_dataq_id_i = 40;
        // zero_data not exposed as scalar port — defaults to 0 in struct
        // Data should be all zeros
        for (int i = 0; i < 16; i++) d->to_l3_mesh_rsp_data_i[i] = 0;
        // err defaults to 0
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read back via pipe
        pipe_read(d, 40, 0x2102);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    // Also zero_data via sys_mesh
    {
        d->to_sys_mesh_rsp_valid_i = 1;
        d->to_sys_mesh_rsp_trans_id_i = 0x2200;
        d->to_sys_mesh_rsp_reqq_id_i = 41;
        d->to_sys_mesh_rsp_dataq_id_i = 41;
        // zero_data not exposed as scalar port
        for (int i = 0; i < 16; i++) d->to_sys_mesh_rsp_data_i[i] = 0;
        // err defaults to 0
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        pipe_read(d, 41, 0x2202);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 18 done\n");

    // ── Phase 19: Deep address range (entries 50-63) ────────
    printf("cosim phase 19: deep address range (entries 50-63)\n");
    for (int ent = 50; ent < 64; ent++) {
        pipe_write(d, ent, ent, 0x3000 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    for (int i = 0; i < 5; i++) idle_tick(sim);
    for (int ent = 50; ent < 64; ent++) {
        pipe_read(d, ent, 0x4000 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 19 done\n");

    // ── Phase 20: FIFO depth saturation (sustained writes) ──
    printf("cosim phase 20: FIFO saturation\n");
    {
        // Rapid bank_l2 + l3_mesh + sys_mesh + bank_l3 writes without draining
        for (int burst = 0; burst < 4; burst++) {
            int ent = burst;
            // Overwrite entries 0-3 rapidly
            pipe_write(d, ent, ent, 0x5000 + burst);
            sim.tick(); compare_ctrl(sim);
            // Don't clear — issue another immediately
            l3_mesh_write(d, ent + 4, ent + 4, 0x5100 + burst);
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
        }
        for (int i = 0; i < 10; i++) idle_tick(sim);
    }
    printf("  phase 20 done\n");

    // ── Phase 21: Write then read with safe spacing ────────
    printf("cosim phase 21: write then read with safe timing\n");
    {
        // Write entry 5, wait for write to propagate, then read
        pipe_write(d, 5, 5, 0x6000);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        // Must wait 3+ cycles for write to fully propagate through RF
        for (int i = 0; i < 4; i++) idle_tick(sim);
        pipe_read(d, 5, 0x6001);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Write entry 6, read different entry 5 immediately (no hazard)
        pipe_write(d, 6, 6, 0x6100);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        pipe_read(d, 5, 0x6101); // different entry — safe
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read entry 6 after safe delay
        for (int i = 0; i < 3; i++) idle_tick(sim);
        pipe_read(d, 6, 0x6102);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 21 done\n");

    // ── Phase 22: Multiple simultaneous write status ────────
    printf("cosim phase 22: write status with multiple writes\n");
    {
        // Issue pipe write and bank_l2 write simultaneously to different entries
        pipe_write(d, 7, 7, 0x7000);
        bank_l2_write(d, 8, 0x7100);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Issue pipe + l3_mesh + sys_mesh simultaneously
        pipe_write(d, 9, 9, 0x7200);
        l3_mesh_write(d, 10, 10, 0x7300);
        d->to_sys_mesh_rsp_valid_i = 1;
        d->to_sys_mesh_rsp_trans_id_i = 0x7400;
        d->to_sys_mesh_rsp_reqq_id_i = 11;
        d->to_sys_mesh_rsp_dataq_id_i = 11;
        fill_data(d->to_sys_mesh_rsp_data_i, 0x74);
        // zero_data defaults to 0
        // err defaults to 0
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 8; i++) idle_tick(sim);
    }
    printf("  phase 22 done\n");

    // ── Phase 23: Read status with multiple reads ───────────
    printf("cosim phase 23: read status with multiple reads\n");
    {
        // Read entries 7-11 via different sinks
        pipe_read(d, 7, 0x8000);
        l3_mesh_read(d, 8, 8, 0x8001);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        rspmux_l2_read(d, 9, 0x8002);
        sys_mesh_read(d, 10, 10, 0x8003);
        rspmux_l3_read(d, 11, 0x8004);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 10; i++) idle_tick(sim);
    }
    printf("  phase 23 done\n");

    // ── Phase 24: Partial QW writes (qwen != 0xF) ──────────
    printf("cosim phase 24: partial QW writes\n");
    {
        // Write only QW0+QW2 to entry 42
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = 42;
        d->pipe_data_rsp_trans_id_i = 0x9000;
        d->pipe_data_rsp_qwen_i = 0x5; // QW0+QW2 only
        fill_data(d->pipe_data_rsp_data_i, 0x90);
        d->pipe_data_rsp_mod_dataq_id_i = 42;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Write QW1+QW3 to same entry
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = 42;
        d->pipe_data_rsp_trans_id_i = 0x9100;
        d->pipe_data_rsp_qwen_i = 0xA; // QW1+QW3
        fill_data(d->pipe_data_rsp_data_i, 0x91);
        d->pipe_data_rsp_mod_dataq_id_i = 42;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read back — should have merged data
        pipe_read(d, 42, 0x9200);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 24 done\n");

    // ── Phase 25: Overwrite same entry rapidly ──────────────
    printf("cosim phase 25: rapid overwrite same entry\n");
    for (int round = 0; round < 4; round++) {
        pipe_write(d, 43, 43, 0xA000 + round);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    for (int i = 0; i < 5; i++) idle_tick(sim);
    pipe_read(d, 43, 0xA100);
    sim.tick(); compare_ctrl(sim);
    clear_inputs(d);
    for (int i = 0; i < 5; i++) idle_tick(sim);
    printf("  phase 25 done\n");

    // ── Phase 26: Error flag on mesh response ───────────────
    printf("cosim phase 26: mesh response with error flag\n");
    {
        d->to_l3_mesh_rsp_valid_i = 1;
        d->to_l3_mesh_rsp_trans_id_i = 0xB000;
        d->to_l3_mesh_rsp_reqq_id_i = 44;
        d->to_l3_mesh_rsp_dataq_id_i = 44;
        fill_data(d->to_l3_mesh_rsp_data_i, 0xB0);
        // zero_data defaults to 0
        // err not exposed as scalar — defaults to 0 (no error injection in this test)
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        pipe_read(d, 44, 0xB100);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 26 done\n");

    // ── Phase 27: Atomic data packing (conf/offset in upper bits) ─
    printf("cosim phase 27: atomic data packing\n");
    {
        // Atomic writes pack conf+offset into upper data bits.
        // Write entry 45 with data that has non-zero upper bits (simulating atomic operand).
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = 45;
        d->pipe_data_rsp_trans_id_i = 0xC000;
        d->pipe_data_rsp_qwen_i = 0xF;
        // Fill with pattern that exercises upper 256 bits (atomic region)
        for (int i = 0; i < 16; i++) d->pipe_data_rsp_data_i[i] = 0xFEDCBA00 + i;
        d->pipe_data_rsp_mod_dataq_id_i = 45;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read back — all 512 bits including upper atomic region must match
        pipe_read(d, 45, 0xC001);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 27 done\n");

    // ── Phase 28: All-ones and all-zeros data patterns ──────
    printf("cosim phase 28: all-ones and all-zeros patterns\n");
    {
        // All ones
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = 46;
        d->pipe_data_rsp_trans_id_i = 0xD000;
        d->pipe_data_rsp_qwen_i = 0xF;
        for (int i = 0; i < 16; i++) d->pipe_data_rsp_data_i[i] = 0xFFFFFFFF;
        d->pipe_data_rsp_mod_dataq_id_i = 46;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        pipe_read(d, 46, 0xD001);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // All zeros
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = 47;
        d->pipe_data_rsp_trans_id_i = 0xD100;
        d->pipe_data_rsp_qwen_i = 0xF;
        for (int i = 0; i < 16; i++) d->pipe_data_rsp_data_i[i] = 0x00000000;
        d->pipe_data_rsp_mod_dataq_id_i = 47;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        pipe_read(d, 47, 0xD101);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 28 done\n");

    // ── Phase 29: Single-QW write patterns (each QW isolated) ─
    printf("cosim phase 29: single-QW writes\n");
    for (int qw = 0; qw < 4; qw++) {
        int ent = 48 + qw;
        d->pipe_data_rsp_valid_i = 1;
        d->pipe_data_rsp_reqq_id_i = ent;
        d->pipe_data_rsp_trans_id_i = 0xE000 + qw;
        d->pipe_data_rsp_qwen_i = (1 << qw); // single QW
        fill_data(d->pipe_data_rsp_data_i, 0xE0 + qw);
        d->pipe_data_rsp_mod_dataq_id_i = ent;
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        pipe_read(d, ent, 0xE100 + qw);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 29 done\n");

    // ── Phase 30: All 4 write sources to same entry (round-robin stress) ─
    printf("cosim phase 30: all write sources to entry 52 in sequence\n");
    {
        pipe_write(d, 52, 52, 0xF000);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);

        l3_mesh_write(d, 52, 52, 0xF100);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);

        bank_l2_write(d, 52, 0xF200);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);

        // sys_mesh write
        d->to_sys_mesh_rsp_valid_i = 1;
        d->to_sys_mesh_rsp_trans_id_i = 0xF300;
        d->to_sys_mesh_rsp_reqq_id_i = 52;
        d->to_sys_mesh_rsp_dataq_id_i = 52;
        fill_data(d->to_sys_mesh_rsp_data_i, 0xF3);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);

        // Read final value — should be last writer wins (sys_mesh)
        pipe_read(d, 52, 0xF400);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 30 done\n");

    // ── Phase 31: Read from all 5 sinks for same entry ──────
    printf("cosim phase 31: read same entry from all 5 sinks\n");
    {
        // Write entry 53
        pipe_write(d, 53, 53, 0x10000);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read via pipe
        pipe_read(d, 53, 0x10001);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read via l3_mesh
        l3_mesh_read(d, 53, 53, 0x10002);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read via rspmux_l2
        rspmux_l2_read(d, 53, 0x10003);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read via sys_mesh
        sys_mesh_read(d, 53, 53, 0x10004);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);

        // Read via rspmux_l3
        rspmux_l3_read(d, 53, 0x10005);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 5; i++) idle_tick(sim);
    }
    printf("  phase 31 done\n");

    // ── Phase 32: Bank L2 with different opcodes ────────────
    printf("cosim phase 32: bank_l2 with various opcodes\n");
    {
        uint8_t opcodes[] = {REQ_WRITE, 0x02 /*WriteAround*/, 0x06 /*Atomic*/, 0x05 /*MsgSendData*/};
        for (int t = 0; t < 4; t++) {
            int ent = 54 + t;
            d->bank_l2_valid_i = 1;
            d->bank_l2_ready_i = 1;
            d->reqq_alloc_l2_id_ag_i = ent;
            d->bank_l2_trans_id_i = 0x11000 + t;
            d->bank_l2_opcode_i = opcodes[t];
            d->bank_l2_subopcode_i = 0;
            d->bank_l2_address_i = 0x100 * t;
            d->bank_l2_wdata_i = 1;
            fill_data(d->bank_l2_data_i, 0x110 + t);
            d->bank_l2_ben_i = UINT64_C(0xFFFFFFFFFFFFFFFF);
            d->bank_l2_qwen_i = 0xF;
            d->bank_l2_size_i = SIZE_LINE;
            d->bank_l2_write_is_partial_i = 0;
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            for (int i = 0; i < 5; i++) idle_tick(sim);
        }
        // Read them all back via pipe
        for (int t = 0; t < 4; t++) {
            pipe_read(d, 54 + t, 0x12000 + t);
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            for (int i = 0; i < 5; i++) idle_tick(sim);
        }
    }
    printf("  phase 32 done\n");

    // ── Phase 33: Sustained read arbitration (4 simultaneous readers) ─
    printf("cosim phase 33: sustained read arbitration\n");
    {
        // Pre-write entries 58-61
        for (int e = 58; e <= 61; e++) {
            pipe_write(d, e, e, 0x13000 + e);
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            for (int i = 0; i < 4; i++) idle_tick(sim);
        }

        // Issue all 4 non-pipe reads simultaneously, repeatedly
        for (int round = 0; round < 3; round++) {
            l3_mesh_read(d, 58, 58, 0x14000 + round);
            rspmux_l2_read(d, 59, 0x14100 + round);
            sys_mesh_read(d, 60, 60, 0x14200 + round);
            rspmux_l3_read(d, 61, 0x14300 + round);
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            // Let arbitration drain — only one wins per cycle
            for (int i = 0; i < 8; i++) idle_tick(sim);
        }
    }
    printf("  phase 33 done\n");

    // ── Phase 34: BEN with alternating byte patterns ────────
    printf("cosim phase 34: alternating BEN patterns\n");
    {
        uint64_t ben_patterns[] = {
            UINT64_C(0xAAAAAAAAAAAAAAAA),
            UINT64_C(0x5555555555555555),
            UINT64_C(0xFF00FF00FF00FF00),
            UINT64_C(0x00FF00FF00FF00FF)
        };
        for (int p = 0; p < 4; p++) {
            int ent = 58 + p; // reuse entries
            d->bank_l2_valid_i = 1;
            d->bank_l2_ready_i = 1;
            d->reqq_alloc_l2_id_ag_i = ent;
            d->bank_l2_trans_id_i = 0x15000 + p;
            d->bank_l2_opcode_i = REQ_WRITE;
            d->bank_l2_wdata_i = 1;
            fill_data(d->bank_l2_data_i, 0x150 + p);
            d->bank_l2_ben_i = ben_patterns[p];
            d->bank_l2_qwen_i = 0xF;
            d->bank_l2_size_i = SIZE_LINE;
            d->bank_l2_write_is_partial_i = 1;
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            for (int i = 0; i < 5; i++) idle_tick(sim);

            // Read back with rd_ben via l3_mesh
            d->reqq_to_l3_mesh_req_valid_wdata_i = 1;
            d->reqq_to_l3_mesh_req_trans_id_i = 0x16000 + p;
            d->reqq_to_l3_mesh_req_reqq_id_i = ent;
            d->reqq_to_l3_mesh_req_dataq_id_i = ent;
            d->reqq_to_l3_mesh_req_qwen_i = 0xF;
            d->reqq_to_l3_mesh_req_rd_ben_i = 1;
            sim.tick(); compare_ctrl(sim);
            clear_inputs(d);
            for (int i = 0; i < 8; i++) idle_tick(sim);
        }
    }
    printf("  phase 34 done\n");

    // ── Phase 35: Write all 64 entries, read all back ───────
    printf("cosim phase 35: full 64-entry sweep\n");
    for (int ent = 0; ent < 64; ent++) {
        pipe_write(d, ent, ent, 0x20000 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        idle_tick(sim);
    }
    for (int i = 0; i < 5; i++) idle_tick(sim);
    for (int ent = 0; ent < 64; ent++) {
        pipe_read(d, ent, 0x30000 + ent);
        sim.tick(); compare_ctrl(sim);
        clear_inputs(d);
        for (int i = 0; i < 4; i++) idle_tick(sim);
    }
    printf("  phase 35 done\n");

    // ── Phase 36: Final idle drain ──────────────────────────
    printf("cosim phase 36: final idle drain\n");
    for (int i = 0; i < 20; i++) idle_tick(sim);
    printf("  phase 36 done\n");

    return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: minion_frontend vs frontend_top.
//
// Drives identical stimulus through both modules and compares all
// outputs cycle-by-cycle: ICache requests, instruction output,
// intpipe decode, VPU decode.

#include "Vcosim_frontend_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vcosim_frontend_top_tb;

struct Tb0Hist {
    uint64_t t;
    uint8_t  orig_clock_enable, new_clock_enable;
    uint8_t  orig_rd_ptr, new_rd_ptr;
    uint8_t  orig_ptr_next, new_ptr_next;
    uint8_t  orig_buf_id, new_buf_id;
    uint8_t  orig_buf_id_next, new_buf_id_next;
    uint32_t orig_inst_data, new_inst_data;
    uint8_t  orig_inst_rvc, new_inst_rvc;
    uint8_t  orig_f6_valid, new_f6_valid;
    uint8_t  orig_f6_stall, new_f6_stall;
    uint8_t  orig_dbg_clear, new_dbg_clear;
    uint8_t  orig_ffb_update, new_ffb_update;
    uint8_t  orig_clock_gated, new_clock_gated;
    uint8_t  orig_f5_req_valid, new_f5_req_valid;
    uint8_t  orig_f6_req_valid, new_f6_req_valid;
    uint8_t  orig_f6_wr, new_f6_wr;
    uint8_t  orig_wr_en, new_wr_en;
    uint8_t  orig_wr1p_next, new_wr1p_next;
    uint8_t  orig_wr1p, new_wr1p;
    uint8_t  orig_req_buf, new_req_buf;
    uint8_t  orig_buf_empty, new_buf_empty;
};

static constexpr int kTb0HistLen = 12;
static Tb0Hist tb0_hist[kTb0HistLen];
static int tb0_hist_pos = 0;

static void record_tb0_history(DUT* d, uint64_t sim_time) {
    Tb0Hist& h = tb0_hist[tb0_hist_pos % kTb0HistLen];
    h.t = sim_time;
    h.orig_clock_enable = d->orig_tb0_clock_enable_o;
    h.new_clock_enable = d->new_tb0_clock_enable_o;
    h.orig_rd_ptr = d->orig_tb0_rd_ptr_o;
    h.new_rd_ptr = d->new_tb0_rd_ptr_o;
    h.orig_ptr_next = d->orig_tb0_f6_buffer_ptr_next_o;
    h.new_ptr_next = d->new_tb0_f6_buffer_ptr_next_o;
    h.orig_buf_id = d->orig_tb0_f6_buffer_id_o;
    h.new_buf_id = d->new_tb0_f6_buffer_id_o;
    h.orig_buf_id_next = d->orig_tb0_f6_buffer_id_next_o;
    h.new_buf_id_next = d->new_tb0_f6_buffer_id_next_o;
    h.orig_inst_data = d->orig_tb0_f6_buffer_inst_data_o;
    h.new_inst_data = d->new_tb0_f6_buffer_inst_data_o;
    h.orig_inst_rvc = d->orig_tb0_f6_inst_rvc_o;
    h.new_inst_rvc = d->new_tb0_f6_inst_rvc_o;
    h.orig_f6_valid = d->orig_tb0_f6_valid_o;
    h.new_f6_valid = d->new_tb0_f6_valid_o;
    h.orig_f6_stall = d->orig_tb0_f6_stall_o;
    h.new_f6_stall = d->new_tb0_f6_stall_o;
    h.orig_dbg_clear = d->orig_tb0_dbg_clear_f7_o;
    h.new_dbg_clear = d->new_tb0_dbg_clear_f7_o;
    h.orig_ffb_update = d->orig_tb0_ffb_update_read_pointer_o;
    h.new_ffb_update = d->new_tb0_ffb_update_read_pointer_o;
    h.orig_clock_gated = d->orig_tb0_clock_gated_o;
    h.new_clock_gated = d->new_tb0_clock_gated_o;
    h.orig_f5_req_valid = d->orig_tb0_f5_req_valid_o;
    h.new_f5_req_valid = d->new_tb0_f5_req_valid_o;
    h.orig_f6_req_valid = d->orig_tb0_f6_req_valid_o;
    h.new_f6_req_valid = d->new_tb0_f6_req_valid_o;
    h.orig_f6_wr = d->orig_tb0_f6_wr_o;
    h.new_f6_wr = d->new_tb0_f6_wr_o;
    h.orig_wr_en = d->orig_tb0_wr_en_o;
    h.new_wr_en = d->new_tb0_wr_en_o;
    h.orig_wr1p_next = d->orig_tb0_f6_buffer_wr_1p_next_o;
    h.new_wr1p_next = d->new_tb0_f6_buffer_wr_1p_next_o;
    h.orig_wr1p = d->orig_tb0_f6_buffer_wr_1p_o;
    h.new_wr1p = d->new_tb0_f6_buffer_wr_1p_o;
    h.orig_req_buf = d->orig_tb0_f6_req_buffer_o;
    h.new_req_buf = d->new_tb0_f6_req_buffer_o;
    h.orig_buf_empty = d->orig_tb0_buf_empty_o;
    h.new_buf_empty = d->new_tb0_buf_empty_o;
    tb0_hist_pos++;
}

static void dump_tb0_history(void) {
    printf("=== TB0 history before first mismatch ===\n");
    int count = tb0_hist_pos < kTb0HistLen ? tb0_hist_pos : kTb0HistLen;
    int start = tb0_hist_pos - count;
    for (int i = 0; i < count; ++i) {
        const Tb0Hist& h = tb0_hist[(start + i) % kTb0HistLen];
        printf("  [t=%lu] cken o=%d n=%d | rd_ptr o=0x%02x n=0x%02x | ptr_next o=0x%02x n=0x%02x | id o=%d n=%d | idn o=%d n=%d"
               " | inst o=0x%08x n=0x%08x | rvc o=%d n=%d | f6v o=%d n=%d | stall o=%d n=%d"
               " | dbgclr o=%d n=%d | ffbupd o=%d n=%d | gclk o=%d n=%d"
               " | f5v o=%d n=%d | f6vreq o=%d n=%d | wr o=%d n=%d | wren o=0x%02x n=0x%02x"
               " | wr1pn o=0x%02x n=0x%02x | wr1p o=0x%02x n=0x%02x | reqbuf o=%d n=%d | empty o=0x%x n=0x%x\n",
               (unsigned long)h.t,
               h.orig_clock_enable, h.new_clock_enable,
               h.orig_rd_ptr, h.new_rd_ptr,
               h.orig_ptr_next, h.new_ptr_next,
               h.orig_buf_id, h.new_buf_id,
               h.orig_buf_id_next, h.new_buf_id_next,
               h.orig_inst_data, h.new_inst_data,
               h.orig_inst_rvc, h.new_inst_rvc,
               h.orig_f6_valid, h.new_f6_valid,
               h.orig_f6_stall, h.new_f6_stall,
               h.orig_dbg_clear, h.new_dbg_clear,
               h.orig_ffb_update, h.new_ffb_update,
               h.orig_clock_gated, h.new_clock_gated,
               h.orig_f5_req_valid, h.new_f5_req_valid,
               h.orig_f6_req_valid, h.new_f6_req_valid,
               h.orig_f6_wr, h.new_f6_wr,
               h.orig_wr_en, h.new_wr_en,
               h.orig_wr1p_next, h.new_wr1p_next,
               h.orig_wr1p, h.new_wr1p,
               h.orig_req_buf, h.new_req_buf,
               h.orig_buf_empty, h.new_buf_empty);
    }
}

static void dump_tb0_point(const char* tag, DUT* d, uint64_t sim_time) {
    printf("  [%s t=%lu] cken o=%d n=%d | gclk o=%d n=%d | rd_ptr o=0x%02x n=0x%02x"
           " | ptr_next o=0x%02x n=0x%02x | id o=%d n=%d | idn o=%d n=%d"
           " | inst o=0x%08x n=0x%08x | rvc o=%d n=%d | f5v o=%d n=%d | f6vreq o=%d n=%d"
           " | f6v o=%d n=%d | stall o=%d n=%d | wr o=%d n=%d | wren o=0x%02x n=0x%02x"
           " | wr1pn o=0x%02x n=0x%02x | wr1p o=0x%02x n=0x%02x | reqbuf o=%d n=%d"
           " | dbgclr o=%d n=%d | ffbupd o=%d n=%d | empty o=0x%x n=0x%x\n",
           tag, (unsigned long)sim_time,
           d->orig_tb0_clock_enable_o, d->new_tb0_clock_enable_o,
           d->orig_tb0_clock_gated_o, d->new_tb0_clock_gated_o,
           d->orig_tb0_rd_ptr_o, d->new_tb0_rd_ptr_o,
           d->orig_tb0_f6_buffer_ptr_next_o, d->new_tb0_f6_buffer_ptr_next_o,
           d->orig_tb0_f6_buffer_id_o, d->new_tb0_f6_buffer_id_o,
           d->orig_tb0_f6_buffer_id_next_o, d->new_tb0_f6_buffer_id_next_o,
           d->orig_tb0_f6_buffer_inst_data_o, d->new_tb0_f6_buffer_inst_data_o,
           d->orig_tb0_f6_inst_rvc_o, d->new_tb0_f6_inst_rvc_o,
           d->orig_tb0_f5_req_valid_o, d->new_tb0_f5_req_valid_o,
           d->orig_tb0_f6_req_valid_o, d->new_tb0_f6_req_valid_o,
           d->orig_tb0_f6_valid_o, d->new_tb0_f6_valid_o,
           d->orig_tb0_f6_stall_o, d->new_tb0_f6_stall_o,
           d->orig_tb0_f6_wr_o, d->new_tb0_f6_wr_o,
           d->orig_tb0_wr_en_o, d->new_tb0_wr_en_o,
           d->orig_tb0_f6_buffer_wr_1p_next_o, d->new_tb0_f6_buffer_wr_1p_next_o,
           d->orig_tb0_f6_buffer_wr_1p_o, d->new_tb0_f6_buffer_wr_1p_o,
           d->orig_tb0_f6_req_buffer_o, d->new_tb0_f6_req_buffer_o,
           d->orig_tb0_dbg_clear_f7_o, d->new_tb0_dbg_clear_f7_o,
           d->orig_tb0_ffb_update_read_pointer_o, d->new_tb0_ffb_update_read_pointer_o,
           d->orig_tb0_buf_empty_o, d->new_tb0_buf_empty_o);
}

// ─── xorshift32 PRNG ─────────────────────────────────────────────
static uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

// ─── Wide signal comparison helpers ──────────────────────────────
static bool wide_eq(const uint32_t* a, const uint32_t* b, int words) {
    for (int i = 0; i < words; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

static void print_wide(const char* label, const uint32_t* v, int words) {
    printf("  %s = 0x", label);
    for (int i = words - 1; i >= 0; i--) printf("%08x", v[i]);
}

static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                         const uint32_t* orig, const uint32_t* newv,
                         int words) {
    sim.comparisons++;
    if (!wide_eq(orig, newv, words)) {
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] %s:\n", (unsigned long)sim.sim_time, name);
        print_wide("orig", orig, words);
        printf("\n");
        print_wide(" new", newv, words);
        printf("\n");
    }
}

// ─── Compare all outputs ─────────────────────────────────────────
static void compare_outputs(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    // ICache request valid
    sim.compare("f1_icache_req_valid",
                (uint64_t)d->orig_f1_icache_req_valid_o,
                (uint64_t)d->new_f1_icache_req_valid_o);

    // ICache request data (compare only when both valid)
    if (d->orig_f1_icache_req_valid_o && d->new_f1_icache_req_valid_o) {
        sim.compare("f1_icache_req",
                    d->orig_f1_icache_req_o,
                    d->new_f1_icache_req_o);
    }

    // Instruction valid
    sim.compare("id_inst_valid",
                (uint64_t)d->orig_id_inst_valid_o,
                (uint64_t)d->new_id_inst_valid_o);

    // Instruction data (compare only when both valid)
    if (d->orig_id_inst_valid_o && d->new_id_inst_valid_o) {
        sim.compare("id_inst_thread_id",
                    (uint64_t)d->orig_id_inst_thread_id_o,
                    (uint64_t)d->new_id_inst_thread_id_o);

        compare_wide(sim, "id_inst_data",
                     d->orig_id_inst_data_o,
                     d->new_id_inst_data_o, 3);

        sim.compare("id_intpipe_ctrl",
                    d->orig_id_intpipe_ctrl_o,
                    d->new_id_intpipe_ctrl_o);

        sim.compare("id_vpu_sigs",
                    d->orig_id_vpu_sigs_o,
                    d->new_id_vpu_sigs_o);
    }

    // Exposed thread-buffer probes must also remain equivalent. These are
    // intentionally exported by the wrapper to catch internal divergence
    // earlier than the top-level frontend outputs.
    sim.compare("tb_inst_valid",
                (uint64_t)d->orig_tb_inst_valid_o,
                (uint64_t)d->new_tb_inst_valid_o);
    if ((d->orig_tb_inst_valid_o & d->new_tb_inst_valid_o) & 0x1) {
        sim.compare("tb_inst_bits_0",
                    (uint64_t)d->orig_tb_inst_bits_0_o,
                    (uint64_t)d->new_tb_inst_bits_0_o);
    }
    if ((d->orig_tb_inst_valid_o & d->new_tb_inst_valid_o) & 0x2) {
        sim.compare("tb_inst_bits_1",
                    (uint64_t)d->orig_tb_inst_bits_1_o,
                    (uint64_t)d->new_tb_inst_bits_1_o);
    }
    sim.compare("tb0_clock_enable", (uint64_t)d->orig_tb0_clock_enable_o, (uint64_t)d->new_tb0_clock_enable_o);
    sim.compare("tb0_f4_req_buffer", (uint64_t)d->orig_tb0_f4_req_buffer_o, (uint64_t)d->new_tb0_f4_req_buffer_o);
    sim.compare("tb0_f5_req_buffer", (uint64_t)d->orig_tb0_f5_req_buffer_o, (uint64_t)d->new_tb0_f5_req_buffer_o);
    sim.compare("tb0_f6_req_buffer", (uint64_t)d->orig_tb0_f6_req_buffer_o, (uint64_t)d->new_tb0_f6_req_buffer_o);
    sim.compare("tb0_f5_req_valid", (uint64_t)d->orig_tb0_f5_req_valid_o, (uint64_t)d->new_tb0_f5_req_valid_o);
    sim.compare("tb0_f6_req_valid", (uint64_t)d->orig_tb0_f6_req_valid_o, (uint64_t)d->new_tb0_f6_req_valid_o);
    sim.compare("tb0_f6_inst_rvc", (uint64_t)d->orig_tb0_f6_inst_rvc_o, (uint64_t)d->new_tb0_f6_inst_rvc_o);
    sim.compare("tb0_f6_buffer_ptr_next", (uint64_t)d->orig_tb0_f6_buffer_ptr_next_o, (uint64_t)d->new_tb0_f6_buffer_ptr_next_o);
    sim.compare("tb0_f6_buffer_id", (uint64_t)d->orig_tb0_f6_buffer_id_o, (uint64_t)d->new_tb0_f6_buffer_id_o);
    sim.compare("tb0_f6_buffer_id_next", (uint64_t)d->orig_tb0_f6_buffer_id_next_o, (uint64_t)d->new_tb0_f6_buffer_id_next_o);
    sim.compare("tb0_f6_last_from_buffer", (uint64_t)d->orig_tb0_f6_last_from_buffer_o, (uint64_t)d->new_tb0_f6_last_from_buffer_o);
    sim.compare("tb0_f6_split", (uint64_t)d->orig_tb0_f6_split_o, (uint64_t)d->new_tb0_f6_split_o);
    sim.compare("tb0_clock_gated", (uint64_t)d->orig_tb0_clock_gated_o, (uint64_t)d->new_tb0_clock_gated_o);
    sim.compare("tb0_f6_buffer_wr_1p_next", (uint64_t)d->orig_tb0_f6_buffer_wr_1p_next_o, (uint64_t)d->new_tb0_f6_buffer_wr_1p_next_o);
    sim.compare("tb0_f6_buffer_wr_1p", (uint64_t)d->orig_tb0_f6_buffer_wr_1p_o, (uint64_t)d->new_tb0_f6_buffer_wr_1p_o);
    sim.compare("tb0_f6_buffer_inst_data", (uint64_t)d->orig_tb0_f6_buffer_inst_data_o, (uint64_t)d->new_tb0_f6_buffer_inst_data_o);
    sim.compare("tb0_f6_stall", (uint64_t)d->orig_tb0_f6_stall_o, (uint64_t)d->new_tb0_f6_stall_o);
    sim.compare("tb0_dbg_clear_f7", (uint64_t)d->orig_tb0_dbg_clear_f7_o, (uint64_t)d->new_tb0_dbg_clear_f7_o);
    sim.compare("tb0_ffb_update_read_pointer", (uint64_t)d->orig_tb0_ffb_update_read_pointer_o, (uint64_t)d->new_tb0_ffb_update_read_pointer_o);
    sim.compare("tb0_rd_ptr", (uint64_t)d->orig_tb0_rd_ptr_o, (uint64_t)d->new_tb0_rd_ptr_o);
    sim.compare("tb0_buf_empty", (uint64_t)d->orig_tb0_buf_empty_o, (uint64_t)d->new_tb0_buf_empty_o);
    sim.compare("tb0_f6_valid", (uint64_t)d->orig_tb0_f6_valid_o, (uint64_t)d->new_tb0_f6_valid_o);
    sim.compare("tb0_f6_wr", (uint64_t)d->orig_tb0_f6_wr_o, (uint64_t)d->new_tb0_f6_wr_o);
    sim.compare("tb0_wr_en", (uint64_t)d->orig_tb0_wr_en_o, (uint64_t)d->new_tb0_wr_en_o);
    sim.compare("tb0_wr_buf", (uint64_t)d->orig_tb0_wr_buf_o, (uint64_t)d->new_tb0_wr_buf_o);
    sim.compare("tb1_rd_ptr", (uint64_t)d->orig_tb1_rd_ptr_o, (uint64_t)d->new_tb1_rd_ptr_o);
    sim.compare("tb1_buf_empty", (uint64_t)d->orig_tb1_buf_empty_o, (uint64_t)d->new_tb1_buf_empty_o);
    sim.compare("tb1_using_pfb", (uint64_t)d->orig_tb1_using_pfb_o, (uint64_t)d->new_tb1_using_pfb_o);
    sim.compare("tb1_dbg_start_ff", (uint64_t)d->orig_tb1_dbg_start_ff_o, (uint64_t)d->new_tb1_dbg_start_ff_o);
}

// ─── Step: eval + compare + tick ─────────────────────────────────
static void step(CosimCtrl<DUT>& sim) {
    sim.dut->eval();
    record_tb0_history(sim.dut.get(), sim.sim_time);
    int before = sim.mismatches;
    compare_outputs(sim);
    // On first mismatch, dump full state
    if (before == 0 && sim.mismatches > 0) {
        auto* d = sim.dut.get();
        dump_tb0_history();
        printf("=== FIRST MISMATCH at t=%lu ===\n", (unsigned long)sim.sim_time);
        printf("  orig: valid=%d tid=%d\n", d->orig_id_inst_valid_o, d->orig_id_inst_thread_id_o);
        printf("  new:  valid=%d tid=%d\n", d->new_id_inst_valid_o, d->new_id_inst_thread_id_o);
        printf("  orig_f1_req_valid=%d new_f1_req_valid=%d\n",
               d->orig_f1_icache_req_valid_o, d->new_f1_icache_req_valid_o);
        printf("  f5_resp_valid=%d f5_resp_miss=%d f6_fill_done=%d\n",
               d->f5_icache_resp_valid_i, d->f5_icache_resp_miss_i, d->f6_icache_fill_done_i);
        printf("  f0_core_req_valid=%d id_core_stall=%d id_inst_ready=%d\n",
               d->f0_core_req_valid_i, d->id_core_stall_i, d->id_inst_ready_i);
        printf("  TB valid: orig=%d%d new=%d%d\n",
               (d->orig_tb_inst_valid_o>>1)&1, d->orig_tb_inst_valid_o&1,
               (d->new_tb_inst_valid_o>>1)&1, d->new_tb_inst_valid_o&1);
        printf("  TB t0 inst_bits: orig=0x%08x new=0x%08x %s\n",
               d->orig_tb_inst_bits_0_o, d->new_tb_inst_bits_0_o,
               d->orig_tb_inst_bits_0_o == d->new_tb_inst_bits_0_o ? "MATCH" : "DIFFER");
        printf("  TB t1 inst_bits: orig=0x%08x new=0x%08x %s\n",
               d->orig_tb_inst_bits_1_o, d->new_tb_inst_bits_1_o,
               d->orig_tb_inst_bits_1_o == d->new_tb_inst_bits_1_o ? "MATCH" : "DIFFER");
    }
    sim.tick();
}

static void step_half(CosimCtrl<DUT>& sim, int clk_level) {
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

// ─── Idle all stimulus inputs ────────────────────────────────────
static void idle_all(DUT* d) {
    d->f0_thread_enabled_i   = 0;
    d->f0_reset_vector_i     = 0;
    d->vm_status_i           = 0;
    d->f1_icache_req_ready_i = 0;
    d->f5_icache_resp_valid_i = 0;
    d->f5_icache_resp_miss_i  = 0;
    std::memset(&d->f5_icache_resp_i, 0, sizeof(d->f5_icache_resp_i));
    d->f6_icache_fill_done_i = 0;
    d->f0_core_req_valid_i   = 0;
    std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));
    d->id_core_stall_i       = 0;
    d->id_inst_ready_i       = 0;
    d->debug_ffb_wdata_i     = 0;
    d->debug_ffb_en_i        = 0;
    d->debug_ffb_thread_sel_i = 0;
    d->debug_ffb_exec_i      = 0;
    d->halt_i                = 0;
    d->halted_i              = 0;
    d->reset_debug_i         = 0;
}

// ─── Pack icache response ────────────────────────────────────────
// icache_fe_resp_t = {data[255:0], page_fault, access_fault, cacheable, bus_err, ecc_err}
// Total 261 bits, packed MSB-first.
static void pack_icache_resp(DUT* d, bool pf, bool af, bool cacheable,
                             bool bus_err, bool ecc_err,
                             const uint32_t data[8]) {
    std::memset(&d->f5_icache_resp_i, 0, sizeof(d->f5_icache_resp_i));
    uint32_t flags = (ecc_err ? 1U : 0) | (bus_err ? 2U : 0)
                   | (cacheable ? 4U : 0) | (af ? 8U : 0) | (pf ? 16U : 0);
    uint64_t carry = flags;
    for (int w = 0; w < 8; w++) {
        carry |= ((uint64_t)data[w]) << 5;
        d->f5_icache_resp_i[w] = (uint32_t)(carry & 0xFFFFFFFF);
        carry >>= 32;
    }
    d->f5_icache_resp_i[8] = (uint32_t)(carry & 0x1F);
}

// ─── Pack core redirect requests ─────────────────────────────────
// fe_req_t = {pc[48:0], speculative, debug_info{halt, resume}}
// Total 52 bits per thread. Two threads packed as [1:0] = 104 bits.
static uint64_t make_fe_req(uint64_t pc, bool speculative,
                            bool halt, bool resume) {
    return ((pc & 0x1FFFFFFFFFFFFULL) << 3)
         | ((speculative ? 1ULL : 0) << 2)
         | ((halt ? 1ULL : 0) << 1)
         | (resume ? 1ULL : 0);
}

static void pack_core_reqs(DUT* d, uint64_t req0, uint64_t req1) {
    // 104 bits = req1[51:0] ## req0[51:0]
    d->f0_core_req_i[0] = (uint32_t)(req0 & 0xFFFFFFFF);
    uint32_t req0_hi = (uint32_t)((req0 >> 32) & 0xFFFFF);
    uint32_t req1_lo = (uint32_t)(req1 & 0xFFF);
    d->f0_core_req_i[1] = req0_hi | (req1_lo << 20);
    d->f0_core_req_i[2] = (uint32_t)((req1 >> 12) & 0xFFFFFFFF);
    d->f0_core_req_i[3] = (uint32_t)((req1 >> 44) & 0xFF);
}

static void drive_half_random(DUT* d, uint32_t& seed, bool compressed) {
    uint32_t r = xorshift32(seed);

    d->f1_icache_req_ready_i = (r & 0x3) != 0;
    d->id_inst_ready_i       = ((r >> 2) & 0x3) != 0;
    d->f5_icache_resp_valid_i = ((r >> 4) & 0x7) == 0;
    d->f5_icache_resp_miss_i  = ((r >> 7) & 0xF) == 0;
    d->f6_icache_fill_done_i  = ((r >> 11) & 0xF) == 0;
    d->id_core_stall_i        = ((r >> 15) & 0xF) == 0 ? ((r >> 19) & 0x3) : 0;

    if (((r >> 21) & 0x3F) == 0) {
        uint32_t r2 = xorshift32(seed);
        int which = r2 & 0x3;
        d->f0_core_req_valid_i = which & 0x3;
        uint64_t pc0 = ((uint64_t)(r2 >> 2) << 1) & 0xFFFFFFFFFFFFULL;
        uint64_t pc1 = ((uint64_t)xorshift32(seed) << 1) & 0xFFFFFFFFFFFFULL;
        pack_core_reqs(d, make_fe_req(pc0, false, false, false),
                          make_fe_req(pc1, false, false, false));
    } else {
        d->f0_core_req_valid_i = 0;
        std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));
    }

    if (d->f5_icache_resp_valid_i) {
        uint32_t data_r[8];
        for (int w = 0; w < 8; w++) {
            data_r[w] = xorshift32(seed);
            if (!compressed) data_r[w] |= 3;
        }
        bool pf = ((r >> 24) & 0x1F) == 0;
        bool af = ((r >> 29) & 0x7) == 0 && !pf;
        bool be = ((seed >> 8) & 0x3F) == 0 && !pf && !af;
        bool ee = ((seed >> 14) & 0x3F) == 0 && !pf && !af && !be;
        pack_icache_resp(d, pf, af, true, be, ee, data_r);
    } else {
        std::memset(&d->f5_icache_resp_i, 0, sizeof(d->f5_icache_resp_i));
    }

    if (((r >> 10) & 0x1FF) == 0) {
        uint32_t rd = xorshift32(seed);
        d->debug_ffb_wdata_i = ((uint64_t)xorshift32(seed) << 32) | xorshift32(seed);
        d->debug_ffb_en_i = rd & 0xF;
        d->debug_ffb_thread_sel_i = (rd >> 4) & 1;
    } else {
        d->debug_ffb_en_i = 0;
    }

    if (((r >> 6) & 0xFFF) == 0) {
        uint32_t re = xorshift32(seed);
        d->debug_ffb_exec_i = re & 0x3;
    } else {
        d->debug_ffb_exec_i = 0;
    }

    d->reset_debug_i = (((r >> 18) & 0xFFF) == 0) ? 1 : 0;
}

// Phase summary helper
static void phase_summary(CosimCtrl<DUT>& sim, const char* name,
                          int start_comp, int start_mm) {
    printf("  %s: %d comparisons, %d mismatches\n",
           name, sim.comparisons - start_comp, sim.mismatches - start_mm);
}

// ═════════════════════════════════════════════════════════
// Main test
// ═════════════════════════════════════════════════════════

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000000;

    idle_all(sim.dut.get());
    sim.reset();

    auto* d = sim.dut.get();
    int c0, m0;

    // ─── Phase 1: Single thread basic operation ──────────
    printf("=== Phase 1: Single-thread basic operation ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    d->f0_thread_enabled_i   = 0x1;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);
    for (int i = 0; i < 10; i++) step(sim);

    // Deliver ICache response: 8 NOP instructions
    uint32_t data_nops[8] = {
        0x00000013, 0x00000013, 0x00000013, 0x00000013,
        0x00000013, 0x00000013, 0x00000013, 0x00000013
    };
    d->f5_icache_resp_valid_i = 1;
    pack_icache_resp(d, false, false, true, false, false, data_nops);
    step(sim);
    d->f5_icache_resp_valid_i = 0;

    for (int i = 0; i < 30; i++) step(sim);

    // Deliver varied instructions: ADD, SUB, LUI, ADDI, AND, OR, SLL, ECALL
    uint32_t data_varied[8] = {
        0x002081B3, 0x40208233, 0x000012B7, 0x00028313,
        0x0020F3B3, 0x0020E433, 0x002094B3, 0x00000073
    };
    d->f5_icache_resp_valid_i = 1;
    pack_icache_resp(d, false, false, true, false, false, data_varied);
    step(sim);
    d->f5_icache_resp_valid_i = 0;

    for (int i = 0; i < 30; i++) step(sim);
    phase_summary(sim, "Phase 1", c0, m0);

    // ─── Phase 2: Dual-thread operation ──────────────────
    printf("=== Phase 2: Dual-thread operation ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    idle_all(d);
    sim.reset();

    d->f0_thread_enabled_i   = 0x3;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);
    for (int i = 0; i < 15; i++) step(sim);

    // Deliver interleaved ICache responses
    for (int blk = 0; blk < 6; blk++) {
        uint32_t data_blk[8];
        for (int w = 0; w < 8; w++)
            data_blk[w] = (0x00000013 | ((blk * 8 + w) << 20));
        d->f5_icache_resp_valid_i = 1;
        pack_icache_resp(d, false, false, true, false, false, data_blk);
        step(sim);
        d->f5_icache_resp_valid_i = 0;
        for (int i = 0; i < 10; i++) step(sim);
    }

    for (int i = 0; i < 20; i++) step(sim);
    phase_summary(sim, "Phase 2", c0, m0);

    // ─── Phase 3: Mispredict / redirect ──────────────────
    printf("=== Phase 3: Mispredict / redirect ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    idle_all(d);
    sim.reset();

    d->f0_thread_enabled_i   = 0x3;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);
    for (int i = 0; i < 15; i++) step(sim);

    // Deliver some data
    uint32_t data_pre[8] = {
        0x00100093, 0x00200113, 0x00300193, 0x00400213,
        0x00500293, 0x00600313, 0x00700393, 0x00800413
    };
    d->f5_icache_resp_valid_i = 1;
    pack_icache_resp(d, false, false, true, false, false, data_pre);
    step(sim);
    d->f5_icache_resp_valid_i = 0;

    for (int i = 0; i < 10; i++) step(sim);

    // Mispredict thread 0
    d->f0_core_req_valid_i = 0x1;
    pack_core_reqs(d, make_fe_req(0x80001000ULL, false, false, false), 0);
    step(sim);
    d->f0_core_req_valid_i = 0;
    std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));

    for (int i = 0; i < 20; i++) step(sim);

    // Mispredict thread 1
    d->f0_core_req_valid_i = 0x2;
    pack_core_reqs(d, 0, make_fe_req(0x80002000ULL, false, false, false));
    step(sim);
    d->f0_core_req_valid_i = 0;
    std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));

    for (int i = 0; i < 20; i++) step(sim);

    // Simultaneous mispredict both threads
    d->f0_core_req_valid_i = 0x3;
    pack_core_reqs(d, make_fe_req(0x80003000ULL, false, false, false),
                      make_fe_req(0x80004000ULL, false, false, false));
    step(sim);
    d->f0_core_req_valid_i = 0;
    std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));

    for (int i = 0; i < 30; i++) step(sim);
    phase_summary(sim, "Phase 3", c0, m0);

    // ─── Phase 4: Stall / backpressure ───────────────────
    printf("=== Phase 4: Stall / backpressure ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    idle_all(d);
    sim.reset();

    d->f0_thread_enabled_i   = 0x1;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);
    for (int i = 0; i < 15; i++) step(sim);

    uint32_t data_stall[8] = {
        0x00A00093, 0x00B00113, 0x00C00193, 0x00D00213,
        0x00E00293, 0x00F00313, 0x01000393, 0x01100413
    };
    d->f5_icache_resp_valid_i = 1;
    pack_icache_resp(d, false, false, true, false, false, data_stall);
    step(sim);
    d->f5_icache_resp_valid_i = 0;

    // Stall consumer
    d->id_inst_ready_i = 0;
    for (int i = 0; i < 15; i++) step(sim);

    // Release stall
    d->id_inst_ready_i = 1;
    for (int i = 0; i < 30; i++) step(sim);

    // ICache backpressure
    d->f1_icache_req_ready_i = 0;
    for (int i = 0; i < 10; i++) step(sim);
    d->f1_icache_req_ready_i = 1;
    for (int i = 0; i < 20; i++) step(sim);
    phase_summary(sim, "Phase 4", c0, m0);

    // ─── Phase 4b: Half-cycle preview / response stress ─────────
    printf("=== Phase 4b: Half-cycle preview stress ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    idle_all(d);
    sim.reset();

    d->f0_thread_enabled_i   = 0x1;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);
    for (int i = 0; i < 15; i++) step(sim);

    uint32_t half_data_a[8] = {
        0x01000013, 0x02000013, 0x03000013, 0x04000013,
        0x05000013, 0x06000013, 0x07000013, 0x08000013
    };
    uint32_t half_data_b[8] = {
        0x11000013, 0x12000013, 0x13000013, 0x14000013,
        0x15000013, 0x16000013, 0x17000013, 0x18000013
    };

    d->f5_icache_resp_valid_i = 1;
    d->f5_icache_resp_miss_i  = 0;
    pack_icache_resp(d, false, false, true, false, false, half_data_a);
    step_half(sim, 0);
    pack_icache_resp(d, false, false, true, false, false, half_data_b);
    step_half(sim, 1);
    d->f5_icache_resp_valid_i = 0;
    for (int i = 0; i < 24; i++) step(sim);

    uint32_t half_seed = 0x31415926u;
    for (int i = 0; i < 4000; i++) {
        drive_half_random(d, half_seed, false);
        step_half(sim, 0);
        drive_half_random(d, half_seed, true);
        step_half(sim, 1);
    }
    phase_summary(sim, "Phase 4b", c0, m0);

    // ─── Phase 5: Heavy random stimulus ──────────────────
    printf("=== Phase 5: Heavy random stimulus (25000 cycles) ===\n");
    c0 = sim.comparisons; m0 = sim.mismatches;

    idle_all(d);
    sim.reset();

    d->f0_thread_enabled_i   = 0x3;
    d->f0_reset_vector_i     = 0x80000000ULL;
    d->f1_icache_req_ready_i = 1;
    d->id_inst_ready_i       = 1;

    for (int i = 0; i < 5; i++) step(sim);

    uint32_t seed = 0xCAFEBABE;

    for (int cycle = 0; cycle < 25000; cycle++) {
        uint32_t r = xorshift32(seed);

        // Keep reset coverage in the explicit phase resets above. Do not
        // inject a live midstream reset pulse here: the original model uses
        // synchronous active-high reset while the translated RTL is allowed to
        // use async-assert/sync-deassert active-low reset, so random in-flight
        // resets with active stimulus create non-equivalent probe timing.
        d->rst_ni = 1;

        // Random ICache req ready (biased toward ready)
        d->f1_icache_req_ready_i = (r & 0x3) != 0;  // 75% ready

        // Random inst ready (biased toward ready)
        d->id_inst_ready_i = ((r >> 2) & 0x3) != 0;  // 75% ready

        // Random ICache response
        d->f5_icache_resp_valid_i = ((r >> 4) & 0x7) == 0;  // ~12.5%
        d->f5_icache_resp_miss_i  = ((r >> 7) & 0xF) == 0;  // ~6.25%

        // Random fill done
        d->f6_icache_fill_done_i = ((r >> 11) & 0xF) == 0;  // ~6.25%

        // Random core stall
        d->id_core_stall_i = ((r >> 15) & 0xF) == 0 ? (r >> 19) & 0x3 : 0;

        // Random mispredict (rare)
        if (((r >> 21) & 0x3F) == 0) {
            uint32_t r2 = xorshift32(seed);
            int which = r2 & 0x3;
            d->f0_core_req_valid_i = which & 0x3;
            uint64_t pc0 = ((uint64_t)(r2 >> 2) << 1) & 0xFFFFFFFFFFFFULL;
            uint64_t pc1 = ((uint64_t)xorshift32(seed) << 1) & 0xFFFFFFFFFFFFULL;
            pack_core_reqs(d, make_fe_req(pc0, false, false, false),
                              make_fe_req(pc1, false, false, false));
        } else {
            d->f0_core_req_valid_i = 0;
        }

        // Random ICache response data
        if (d->f5_icache_resp_valid_i) {
            uint32_t data_r[8];
            for (int w = 0; w < 8; w++) {
                data_r[w] = xorshift32(seed);
                // Mix of 32-bit and 16-bit instructions
                if ((r >> (w & 7)) & 1)
                    data_r[w] |= 3;  // force bits[1:0]=11 for 32-bit
            }
            bool pf  = ((r >> 24) & 0x1F) == 0;
            bool af  = ((r >> 29) & 0x7) == 0 && !pf;
            bool be  = ((seed >> 8) & 0x3F) == 0 && !pf && !af;
            bool ee  = ((seed >> 14) & 0x3F) == 0 && !pf && !af && !be;
            pack_icache_resp(d, pf, af, true, be, ee, data_r);
        }

        // Random thread enable changes (very rare)
        if (((r >> 14) & 0x1FF) == 0) {
            d->f0_thread_enabled_i = xorshift32(seed) & 0x3;
            if (d->f0_thread_enabled_i == 0)
                d->f0_thread_enabled_i = 0x1;  // keep at least one
        }

        // Random halt/halted (rare)
        if (((r >> 8) & 0xFF) == 0) {
            uint32_t rh = xorshift32(seed);
            d->halt_i   = rh & 0x3;
            d->halted_i = (rh >> 2) & 0x3;
        }

        // Random VM status (rare)
        if (((r >> 16) & 0xFF) == 0) {
            d->vm_status_i = (uint16_t)(xorshift32(seed) & 0xFFFF);
        }

        // Random debug FFB writes (rare)
        if (((r >> 10) & 0x1FF) == 0) {
            uint32_t rd = xorshift32(seed);
            d->debug_ffb_wdata_i = ((uint64_t)xorshift32(seed) << 32) | xorshift32(seed);
            d->debug_ffb_en_i = rd & 0xF;
            d->debug_ffb_thread_sel_i = (rd >> 4) & 1;
        } else {
            d->debug_ffb_en_i = 0;
        }

        // Random debug exec (very rare)
        if (((r >> 6) & 0xFFF) == 0) {
            uint32_t re = xorshift32(seed);
            d->debug_ffb_exec_i = re & 0x3;
        } else {
            d->debug_ffb_exec_i = 0;
        }

        // Random reset_debug (very rare)
        d->reset_debug_i = (((r >> 18) & 0xFFF) == 0) ? 1 : 0;

        step(sim);

        // Clear one-shot signals
        if (d->f0_core_req_valid_i) {
            d->f0_core_req_valid_i = 0;
            std::memset(&d->f0_core_req_i, 0, sizeof(d->f0_core_req_i));
        }
    }

    phase_summary(sim, "Phase 5", c0, m0);

    return sim.finish();
}

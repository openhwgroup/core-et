// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_thread_buffer_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstring>

using DUT = Vcosim_thread_buffer_tb;
constexpr uint64_t kFeReqDebugHalt = 0x2ULL;

struct TbHist {
    uint64_t t;
    uint8_t  orig_clock_enable, new_clock_enable;
    uint8_t  orig_clock_gated, new_clock_gated;
    uint8_t  orig_f4_req_buffer, new_f4_req_buffer;
    uint8_t  orig_f5_req_buffer, new_f5_req_buffer;
    uint8_t  orig_f5_req_valid, new_f5_req_valid;
    uint8_t  orig_f6_req_valid, new_f6_req_valid;
    uint8_t  orig_f6_req_buffer, new_f6_req_buffer;
    uint8_t  orig_f6_write_ad, new_f6_write_ad;
    uint8_t  orig_f6_wr, new_f6_wr;
    uint8_t  orig_f6_wren, new_f6_wren;
    uint8_t  orig_f6_wr1p_next, new_f6_wr1p_next;
    uint8_t  orig_f6_wr1p, new_f6_wr1p;
    uint8_t  orig_ptr, new_ptr;
    uint8_t  orig_ptr_next, new_ptr_next;
    uint8_t  orig_buf_empty, new_buf_empty;
    uint8_t  orig_f6_valid, new_f6_valid;
};

static constexpr int kHistLen = 12;
static TbHist hist[kHistLen];
static int hist_pos = 0;

static void record_history(DUT* d, uint64_t sim_time) {
    TbHist& h = hist[hist_pos % kHistLen];
    h.t = sim_time;
    h.orig_clock_enable = d->orig_clock_enable_o;
    h.new_clock_enable = d->new_clock_enable_o;
    h.orig_clock_gated = d->orig_clock_gated_o;
    h.new_clock_gated = d->new_clock_gated_o;
    h.orig_f4_req_buffer = d->orig_f4_req_buffer_o;
    h.new_f4_req_buffer = d->new_f4_req_buffer_o;
    h.orig_f5_req_buffer = d->orig_f5_req_buffer_o;
    h.new_f5_req_buffer = d->new_f5_req_buffer_o;
    h.orig_f5_req_valid = d->orig_f5_req_valid_o;
    h.new_f5_req_valid = d->new_f5_req_valid_o;
    h.orig_f6_req_valid = d->orig_f6_req_valid_o;
    h.new_f6_req_valid = d->new_f6_req_valid_o;
    h.orig_f6_req_buffer = d->orig_f6_req_buffer_o;
    h.new_f6_req_buffer = d->new_f6_req_buffer_o;
    h.orig_f6_write_ad = d->orig_f6_write_ad_o;
    h.new_f6_write_ad = d->new_f6_write_ad_o;
    h.orig_f6_wr = d->orig_f6_buffer_wr_o;
    h.new_f6_wr = d->new_f6_buffer_wr_o;
    h.orig_f6_wren = d->orig_f6_buffer_wren_o;
    h.new_f6_wren = d->new_f6_buffer_wren_o;
    h.orig_f6_wr1p_next = d->orig_f6_buffer_wr_1p_next_o;
    h.new_f6_wr1p_next = d->new_f6_buffer_wr_1p_next_o;
    h.orig_f6_wr1p = d->orig_f6_buffer_wr_1p_o;
    h.new_f6_wr1p = d->new_f6_buffer_wr_1p_o;
    h.orig_ptr = d->orig_f6_buffer_ptr_o;
    h.new_ptr = d->new_f6_buffer_ptr_o;
    h.orig_ptr_next = d->orig_f6_buffer_ptr_next_o;
    h.new_ptr_next = d->new_f6_buffer_ptr_next_o;
    h.orig_buf_empty = d->orig_buffer_empty_o;
    h.new_buf_empty = d->new_buffer_empty_o;
    h.orig_f6_valid = d->orig_f6_valid_o;
    h.new_f6_valid = d->new_f6_valid_o;
    hist_pos++;
}

static void dump_history(void) {
    printf("=== thread_buffer history before first mismatch ===\n");
    int count = hist_pos < kHistLen ? hist_pos : kHistLen;
    int start = hist_pos - count;
    for (int i = 0; i < count; ++i) {
        const TbHist& h = hist[(start + i) % kHistLen];
        printf("  [t=%lu] cken o=%d n=%d | gclk o=%d n=%d | f4 o=%d n=%d | f5 o=%d n=%d"
               " | f5v o=%d n=%d | f6vreq o=%d n=%d | f6buf o=%d n=%d | wrad o=%d n=%d"
               " | wr o=%d n=%d | wren o=0x%x n=0x%x | wr1pn o=0x%x n=0x%x | wr1p o=0x%x n=0x%x"
               " | ptr o=0x%02x n=0x%02x | ptrn o=0x%02x n=0x%02x | empty o=0x%x n=0x%x | f6v o=%d n=%d\n",
               (unsigned long)h.t,
               h.orig_clock_enable, h.new_clock_enable,
               h.orig_clock_gated, h.new_clock_gated,
               h.orig_f4_req_buffer, h.new_f4_req_buffer,
               h.orig_f5_req_buffer, h.new_f5_req_buffer,
               h.orig_f5_req_valid, h.new_f5_req_valid,
               h.orig_f6_req_valid, h.new_f6_req_valid,
               h.orig_f6_req_buffer, h.new_f6_req_buffer,
               h.orig_f6_write_ad, h.new_f6_write_ad,
               h.orig_f6_wr, h.new_f6_wr,
               h.orig_f6_wren, h.new_f6_wren,
               h.orig_f6_wr1p_next, h.new_f6_wr1p_next,
               h.orig_f6_wr1p, h.new_f6_wr1p,
               h.orig_ptr, h.new_ptr,
               h.orig_ptr_next, h.new_ptr_next,
               h.orig_buf_empty, h.new_buf_empty,
               h.orig_f6_valid, h.new_f6_valid);
    }
}

static void dump_point(const char* tag, DUT* d, uint64_t sim_time) {
    printf("  [%s t=%lu] cken o=%d n=%d | gclk o=%d n=%d | f4 o=%d n=%d | f5 o=%d n=%d"
           " | f5v o=%d n=%d | f6vreq o=%d n=%d | f6buf o=%d n=%d | wrad o=%d n=%d"
           " | wr o=%d n=%d | wren o=0x%x n=0x%x | wr1pn o=0x%x n=0x%x | wr1p o=0x%x n=0x%x"
           " | ptr o=0x%02x n=0x%02x | ptrn o=0x%02x n=0x%02x | empty o=0x%x n=0x%x | f6v o=%d n=%d\n",
           tag, (unsigned long)sim_time,
           d->orig_clock_enable_o, d->new_clock_enable_o,
           d->orig_clock_gated_o, d->new_clock_gated_o,
           d->orig_f4_req_buffer_o, d->new_f4_req_buffer_o,
           d->orig_f5_req_buffer_o, d->new_f5_req_buffer_o,
           d->orig_f5_req_valid_o, d->new_f5_req_valid_o,
           d->orig_f6_req_valid_o, d->new_f6_req_valid_o,
           d->orig_f6_req_buffer_o, d->new_f6_req_buffer_o,
           d->orig_f6_write_ad_o, d->new_f6_write_ad_o,
           d->orig_f6_buffer_wr_o, d->new_f6_buffer_wr_o,
           d->orig_f6_buffer_wren_o, d->new_f6_buffer_wren_o,
           d->orig_f6_buffer_wr_1p_next_o, d->new_f6_buffer_wr_1p_next_o,
           d->orig_f6_buffer_wr_1p_o, d->new_f6_buffer_wr_1p_o,
           d->orig_f6_buffer_ptr_o, d->new_f6_buffer_ptr_o,
           d->orig_f6_buffer_ptr_next_o, d->new_f6_buffer_ptr_next_o,
           d->orig_buffer_empty_o, d->new_buffer_empty_o,
           d->orig_f6_valid_o, d->new_f6_valid_o);
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
    uint64_t o_req = sim.dut->orig_f0_icache_req_valid_o;
    uint64_t n_req = sim.dut->new_f0_icache_req_valid_o;
    uint64_t o_val = sim.dut->orig_f7_inst_valid_o;
    uint64_t n_val = sim.dut->new_f7_inst_valid_o;
    sim.compare("f0_icache_req_valid", o_req, n_req);
    sim.compare("f7_inst_valid", o_val, n_val);
    sim.compare("f6_buffer_ptr",
                (uint64_t)sim.dut->orig_f6_buffer_ptr_o,
                (uint64_t)sim.dut->new_f6_buffer_ptr_o);
    sim.compare("buffer_empty",
                (uint64_t)sim.dut->orig_buffer_empty_o,
                (uint64_t)sim.dut->new_buffer_empty_o);
    sim.compare("f6_valid",
                (uint64_t)sim.dut->orig_f6_valid_o,
                (uint64_t)sim.dut->new_f6_valid_o);
    sim.compare("f6_buffer_wr",
                (uint64_t)sim.dut->orig_f6_buffer_wr_o,
                (uint64_t)sim.dut->new_f6_buffer_wr_o);
    sim.compare("f6_buffer_wren",
                (uint64_t)sim.dut->orig_f6_buffer_wren_o,
                (uint64_t)sim.dut->new_f6_buffer_wren_o);
    sim.compare("f0_req_buffer",
                (uint64_t)sim.dut->orig_f0_req_buffer_o,
                (uint64_t)sim.dut->new_f0_req_buffer_o);
    sim.compare("f5_req_buffer",
                (uint64_t)sim.dut->orig_f5_req_buffer_o,
                (uint64_t)sim.dut->new_f5_req_buffer_o);
    sim.compare("f6_req_buffer",
                (uint64_t)sim.dut->orig_f6_req_buffer_o,
                (uint64_t)sim.dut->new_f6_req_buffer_o);
    sim.compare("using_pfb",
                (uint64_t)sim.dut->orig_using_pfb_o,
                (uint64_t)sim.dut->new_using_pfb_o);
    sim.compare("clock_enable",
                (uint64_t)sim.dut->orig_clock_enable_o,
                (uint64_t)sim.dut->new_clock_enable_o);
    sim.compare("f4_req_buffer",
                (uint64_t)sim.dut->orig_f4_req_buffer_o,
                (uint64_t)sim.dut->new_f4_req_buffer_o);
    sim.compare("f5_req_valid",
                (uint64_t)sim.dut->orig_f5_req_valid_o,
                (uint64_t)sim.dut->new_f5_req_valid_o);
    sim.compare("f6_req_valid",
                (uint64_t)sim.dut->orig_f6_req_valid_o,
                (uint64_t)sim.dut->new_f6_req_valid_o);
    sim.compare("f6_inst_rvc",
                (uint64_t)sim.dut->orig_f6_inst_rvc_o,
                (uint64_t)sim.dut->new_f6_inst_rvc_o);
    sim.compare("f6_buffer_ptr_next",
                (uint64_t)sim.dut->orig_f6_buffer_ptr_next_o,
                (uint64_t)sim.dut->new_f6_buffer_ptr_next_o);
    sim.compare("f6_buffer_id",
                (uint64_t)sim.dut->orig_f6_buffer_id_o,
                (uint64_t)sim.dut->new_f6_buffer_id_o);
    sim.compare("f6_buffer_id_next",
                (uint64_t)sim.dut->orig_f6_buffer_id_next_o,
                (uint64_t)sim.dut->new_f6_buffer_id_next_o);
    sim.compare("f6_last_from_buffer",
                (uint64_t)sim.dut->orig_f6_last_from_buffer_o,
                (uint64_t)sim.dut->new_f6_last_from_buffer_o);
    sim.compare("f6_split",
                (uint64_t)sim.dut->orig_f6_split_o,
                (uint64_t)sim.dut->new_f6_split_o);
    sim.compare("clock_gated",
                (uint64_t)sim.dut->orig_clock_gated_o,
                (uint64_t)sim.dut->new_clock_gated_o);
    sim.compare("f6_buffer_wr_1p_next",
                (uint64_t)sim.dut->orig_f6_buffer_wr_1p_next_o,
                (uint64_t)sim.dut->new_f6_buffer_wr_1p_next_o);
    sim.compare("f6_buffer_wr_1p",
                (uint64_t)sim.dut->orig_f6_buffer_wr_1p_o,
                (uint64_t)sim.dut->new_f6_buffer_wr_1p_o);
    sim.compare("f6_buffer_inst_data",
                (uint64_t)sim.dut->orig_f6_buffer_inst_data_o,
                (uint64_t)sim.dut->new_f6_buffer_inst_data_o);
    sim.compare("f6_stall",
                (uint64_t)sim.dut->orig_f6_stall_o,
                (uint64_t)sim.dut->new_f6_stall_o);
    sim.compare("dbg_clear_f7",
                (uint64_t)sim.dut->orig_dbg_clear_f7_o,
                (uint64_t)sim.dut->new_dbg_clear_f7_o);
    sim.compare("ffb_update_read_pointer",
                (uint64_t)sim.dut->orig_ffb_update_read_pointer_o,
                (uint64_t)sim.dut->new_ffb_update_read_pointer_o);
    sim.comparisons++;
    if (sim.dut->orig_f6_write_ad_o != sim.dut->new_f6_write_ad_o) {
        if (sim.mismatches == 1) {
            dump_history();
            dump_point("first-mismatch", sim.dut.get(), sim.sim_time);
        }
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] f6_write_ad: orig=0x%llx new=0x%llx"
               " | reqbuf f0 o=%d n=%d f5 o=%d n=%d f6 o=%d n=%d"
               " | ptr_next o=0x%02x n=0x%02x id o=%d n=%d idn o=%d n=%d"
               " | rvc o=%d n=%d last o=%d n=%d split o=%d n=%d"
               " | wr1p_next o=0x%02x n=0x%02x wr1p o=0x%02x n=0x%02x"
               " | inst_data o=0x%08x n=0x%08x"
               " | clk_gated o=%d n=%d stall o=%d n=%d dbgclr o=%d n=%d ffbptr o=%d n=%d"
               " | using_pfb o=%d n=%d"
               " | wren o=0x%02x n=0x%02x | wr o=%d n=%d | f6_valid o=%d n=%d"
               " | empty o=0x%x n=0x%x\n",
               (unsigned long)sim.sim_time,
               (unsigned long long)sim.dut->orig_f6_write_ad_o,
               (unsigned long long)sim.dut->new_f6_write_ad_o,
               sim.dut->orig_f0_req_buffer_o, sim.dut->new_f0_req_buffer_o,
               sim.dut->orig_f5_req_buffer_o, sim.dut->new_f5_req_buffer_o,
               sim.dut->orig_f6_req_buffer_o, sim.dut->new_f6_req_buffer_o,
               sim.dut->orig_f6_buffer_ptr_next_o, sim.dut->new_f6_buffer_ptr_next_o,
               sim.dut->orig_f6_buffer_id_o, sim.dut->new_f6_buffer_id_o,
               sim.dut->orig_f6_buffer_id_next_o, sim.dut->new_f6_buffer_id_next_o,
               sim.dut->orig_f6_inst_rvc_o, sim.dut->new_f6_inst_rvc_o,
               sim.dut->orig_f6_last_from_buffer_o, sim.dut->new_f6_last_from_buffer_o,
               sim.dut->orig_f6_split_o, sim.dut->new_f6_split_o,
               sim.dut->orig_f6_buffer_wr_1p_next_o, sim.dut->new_f6_buffer_wr_1p_next_o,
               sim.dut->orig_f6_buffer_wr_1p_o, sim.dut->new_f6_buffer_wr_1p_o,
               sim.dut->orig_f6_buffer_inst_data_o, sim.dut->new_f6_buffer_inst_data_o,
               sim.dut->orig_clock_gated_o, sim.dut->new_clock_gated_o,
               sim.dut->orig_f6_stall_o, sim.dut->new_f6_stall_o,
               sim.dut->orig_dbg_clear_f7_o, sim.dut->new_dbg_clear_f7_o,
               sim.dut->orig_ffb_update_read_pointer_o, sim.dut->new_ffb_update_read_pointer_o,
               sim.dut->orig_using_pfb_o, sim.dut->new_using_pfb_o,
               sim.dut->orig_f6_buffer_wren_o, sim.dut->new_f6_buffer_wren_o,
               sim.dut->orig_f6_buffer_wr_o, sim.dut->new_f6_buffer_wr_o,
               sim.dut->orig_f6_valid_o, sim.dut->new_f6_valid_o,
               sim.dut->orig_buffer_empty_o, sim.dut->new_buffer_empty_o);
    }
    if (o_val && n_val) {
        sim.compare("f7_inst_rvc",
                    (uint64_t)sim.dut->orig_f7_inst_rvc_o,
                    (uint64_t)sim.dut->new_f7_inst_rvc_o);
        bool match = (sim.dut->orig_f7_inst_data_o[0] == sim.dut->new_f7_inst_data_o[0])
                  && (sim.dut->orig_f7_inst_data_o[1] == sim.dut->new_f7_inst_data_o[1])
                  && ((sim.dut->orig_f7_inst_data_o[2] & 0x00FFFFFF) ==
                      (sim.dut->new_f7_inst_data_o[2] & 0x00FFFFFF));
        sim.comparisons++;
        if (!match) {
            sim.mismatches++;
            printf("  MISMATCH [t=%lu] f7_inst_data: w0 o=%08x n=%08x w1 o=%08x n=%08x w2 o=%08x n=%08x\n",
                   (unsigned long)sim.sim_time,
                   sim.dut->orig_f7_inst_data_o[0], sim.dut->new_f7_inst_data_o[0],
                   sim.dut->orig_f7_inst_data_o[1], sim.dut->new_f7_inst_data_o[1],
                   sim.dut->orig_f7_inst_data_o[2], sim.dut->new_f7_inst_data_o[2]);
        }
    }
}

static void idle_all(DUT* dut) {
    dut->f0_thread_enabled_i = 0; dut->f0_reset_vector_i = 0;
    dut->f0_disable_thread_i = 0; dut->f0_enable_thread_i = 0;
    dut->in_debug_mode_i = 0; dut->vm_status_i = 0;
    dut->f0_icache_req_ready_i = 0; dut->f1_icache_req_ready_i = 0;
    dut->f5_icache_resp_miss_i = 0;
    dut->f6_icache_resp_valid_i = 0; dut->f6_icache_resp_miss_i = 0;
    std::memset(&dut->f6_icache_resp_i, 0, sizeof(dut->f6_icache_resp_i));
    dut->f7_icache_fill_done_i = 0;
    dut->f0_core_req_valid_i = 0;
    std::memset(&dut->f0_core_req_i, 0, sizeof(dut->f0_core_req_i));
    dut->f7_inst_ready_i = 0;
    dut->debug_ffb_wdata_i = 0; dut->debug_ffb_en_i = 0;
    dut->debug_ffb_exec_i = 0; dut->io_halt_i = 0;
}

static void step(CosimCtrl<DUT>& sim) {
    record_history(sim.dut.get(), sim.sim_time);
    sim.dut->eval();
    compare_outputs(sim);
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

static void pack_resp(DUT* dut, bool pf, bool af, bool cacheable,
                      bool bus_err, bool ecc_err, const uint32_t data[8]) {
    std::memset(&dut->f6_icache_resp_i, 0, sizeof(dut->f6_icache_resp_i));
    uint32_t flags = (ecc_err ? 1U : 0) | (bus_err ? 2U : 0) | (cacheable ? 4U : 0)
                   | (af ? 8U : 0) | (pf ? 16U : 0);
    uint64_t carry = flags;
    for (int w = 0; w < 8; w++) {
        carry |= ((uint64_t)data[w]) << 5;
        dut->f6_icache_resp_i[w] = (uint32_t)(carry & 0xFFFFFFFF);
        carry >>= 32;
    }
    dut->f6_icache_resp_i[8] = (uint32_t)(carry & 0x1F);
}

static void drive_half_random(DUT* dut, uint32_t& seed, int& prev_f5_miss, bool compressed) {
    seed = seed * 1103515245u + 12345u;
    dut->f0_icache_req_ready_i = (seed >> 0) & 1;
    dut->f1_icache_req_ready_i = (seed >> 1) & 1;
    dut->f7_inst_ready_i = (seed >> 2) & 1;
    int this_miss = (seed >> 4) & 1;
    dut->f5_icache_resp_miss_i = this_miss;
    dut->f6_icache_resp_valid_i = (seed >> 3) & 1;
    dut->f6_icache_resp_miss_i = prev_f5_miss;
    prev_f5_miss = this_miss;
    dut->f0_core_req_valid_i = ((seed >> 5) & 0x1F) == 0;
    dut->f7_icache_fill_done_i = ((seed >> 10) & 0xF) == 0;
    dut->in_debug_mode_i = ((seed >> 14) & 0x3F) == 0;
    dut->debug_ffb_exec_i = dut->in_debug_mode_i && (((seed >> 20) & 0x1F) == 0);
    dut->debug_ffb_en_i = dut->in_debug_mode_i ? ((seed >> 21) & 0xF) : 0;
    dut->debug_ffb_wdata_i = (uint64_t(seed) << 32) | (uint64_t(seed) ^ 0xA5A5A5A5u);
    dut->io_halt_i = ((seed >> 27) & 0x7F) == 0;
    uint32_t data[8];
    for (int w = 0; w < 8; w++) {
        data[w] = compressed ? (seed * (w + 1)) : ((seed * (w + 1)) | 3u);
    }
    pack_resp(dut,
              ((seed >> 20) & 0xF) == 0,
              ((seed >> 24) & 0xF) == 0,
              true,
              ((seed >> 28) & 0x1F) == 0,
              false,
              data);
}

// Wait for f6_valid_q in the pipeline (wait for request to reach F6)
static void wait_for_f6(CosimCtrl<DUT>& sim, int max_cycles) {
    for (int i = 0; i < max_cycles; i++) {
        step(sim);
        // f6 is reached when neither module produces req_valid anymore
        // (pipeline occupied) — crude but works
    }
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 4000000;
    idle_all(sim.dut.get());
    sim.reset();

    // Enable thread
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80000000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    // Wait until request reaches F6 (7 cycles for F0-F6 pipeline)
    for (int i = 0; i < 8; i++) step(sim);

    // Deliver hit with distinct data per word (all 32-bit instructions: bits[1:0]=11)
    printf("=== test 1: deliver 32-bit instructions ===\n");
    uint32_t data32[8] = {0xAABBCC13, 0x11223313, 0x44556613, 0x77889913,
                          0xDDEEFF13, 0xABCDEF13, 0x13579B13, 0x2468AC13};
    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), false, false, true, false, false, data32);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    // Read instructions
    for (int i = 0; i < 30; i++) step(sim);

    // Flush and restart with compressed instructions
    printf("=== test 2: deliver compressed instructions ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    std::memset(&sim.dut->f0_core_req_i, 0, sizeof(sim.dut->f0_core_req_i));
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    // Compressed: bits[1:0] != 11. Each 16-bit halfword is an instruction.
    uint32_t data_rvc[8] = {0x45014501, 0x45014501, 0x45014501, 0x45014501,
                            0x45014501, 0x45014501, 0x45014501, 0x45014501};
    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), false, false, true, false, false, data_rvc);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    for (int i = 0; i < 30; i++) step(sim);

    // Flush and restart with mixed data
    printf("=== test 3: deliver mixed 32/16-bit ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    // Mix: first word has 32-bit (0x13 suffix), second has compressed (0x01 suffix)
    uint32_t data_mix[8] = {0xDEAD0013, 0x45010001, 0xBEEF0013, 0xA0010001,
                            0xCAFE0013, 0x60010001, 0xF00D0013, 0x85020001};
    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), false, false, true, false, false, data_mix);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    for (int i = 0; i < 30; i++) step(sim);

    // Test with page fault
    printf("=== test 4: page fault response ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    uint32_t data_pf[8] = {0};
    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), true, false, true, false, false, data_pf);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    for (int i = 0; i < 15; i++) step(sim);

    // Test with access fault
    printf("=== test 5: access fault ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), false, true, true, false, false, data_pf);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    for (int i = 0; i < 15; i++) step(sim);

    // Test with bus error
    printf("=== test 6: bus error ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    sim.dut->f6_icache_resp_valid_i = 1;
    pack_resp(sim.dut.get(), false, false, true, true, false, data_pf);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;

    for (int i = 0; i < 15; i++) step(sim);

    // Test with ICache miss then fill
    printf("=== test 7: miss then fill ===\n");
    sim.dut->f0_core_req_valid_i = 1;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    for (int i = 0; i < 8; i++) step(sim);

    // Deliver a miss response
    sim.dut->f5_icache_resp_miss_i = 1;  // predict miss at F5
    step(sim);
    sim.dut->f5_icache_resp_miss_i = 0;
    sim.dut->f6_icache_resp_valid_i = 1;
    sim.dut->f6_icache_resp_miss_i = 1;  // miss at F6
    pack_resp(sim.dut.get(), false, false, true, false, false, data32);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;
    sim.dut->f6_icache_resp_miss_i = 0;

    // Wait, then fill done
    for (int i = 0; i < 5; i++) step(sim);
    sim.dut->f7_icache_fill_done_i = 1;
    step(sim);
    sim.dut->f7_icache_fill_done_i = 0;

    // Wait for new request cycle after fill, then deliver hit
    for (int i = 0; i < 10; i++) step(sim);
    sim.dut->f5_icache_resp_miss_i = 0;
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 1;
    sim.dut->f6_icache_resp_miss_i = 0;
    uint32_t data_fill[8] = {0x00100013, 0x00200013, 0x00300013, 0x00400013,
                             0x00500013, 0x00600013, 0x00700013, 0x00800013};
    pack_resp(sim.dut.get(), false, false, true, false, false, data_fill);
    step(sim);
    sim.dut->f6_icache_resp_valid_i = 0;
    for (int i = 0; i < 20; i++) step(sim);

    // Multiple miss/fill cycles
    printf("=== test 7b: repeated miss/fill ===\n");
    for (int rep = 0; rep < 5; rep++) {
        sim.dut->f0_core_req_valid_i = 1;
        step(sim);
        sim.dut->f0_core_req_valid_i = 0;
        for (int i = 0; i < 8; i++) step(sim);

        // Miss
        sim.dut->f5_icache_resp_miss_i = 1;
        step(sim);
        sim.dut->f5_icache_resp_miss_i = 0;
        sim.dut->f6_icache_resp_valid_i = 1;
        sim.dut->f6_icache_resp_miss_i = 1;
        pack_resp(sim.dut.get(), false, false, true, false, false, data32);
        step(sim);
        sim.dut->f6_icache_resp_valid_i = 0;
        sim.dut->f6_icache_resp_miss_i = 0;

        // Fill
        for (int i = 0; i < 3; i++) step(sim);
        sim.dut->f7_icache_fill_done_i = 1;
        step(sim);
        sim.dut->f7_icache_fill_done_i = 0;

        // Hit
        for (int i = 0; i < 10; i++) step(sim);
        sim.dut->f6_icache_resp_valid_i = 1;
        sim.dut->f6_icache_resp_miss_i = 0;
        for (int w = 0; w < 8; w++) data_fill[w] = 0x00000013 + (rep * 8 + w) * 0x100000;
        pack_resp(sim.dut.get(), false, false, true, false, false, data_fill);
        step(sim);
        sim.dut->f6_icache_resp_valid_i = 0;
        for (int i = 0; i < 15; i++) step(sim);
    }

    // Debug/program buffer path.
    // This exercises debug_mode_start/reset_prog_buffer/debug_ffb_exec, which
    // the original latch-based path handles through the same "1 phase early"
    // RF write protocol as the normal fetch buffer fills.
    printf("=== test 7c: debug program buffer ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80000000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    sim.dut->f0_core_req_valid_i = 1;
    sim.dut->f0_core_req_i = kFeReqDebugHalt;
    step(sim);
    sim.dut->f0_core_req_valid_i = 0;
    sim.dut->f0_core_req_i = 0;
    sim.dut->in_debug_mode_i = 1;

    for (int i = 0; i < 6; i++) step(sim);

    sim.dut->debug_ffb_exec_i = 1;
    step(sim);
    sim.dut->debug_ffb_exec_i = 0;

    bool saw_debug_inst = false;
    for (int i = 0; i < 24; i++) {
        if (sim.dut->orig_f7_inst_valid_o && sim.dut->new_f7_inst_valid_o) saw_debug_inst = true;
        step(sim);
    }
    sim.check(saw_debug_inst, "debug program buffer produced at least one instruction");
    sim.dut->in_debug_mode_i = 0;

    printf("=== test 7d: half-cycle response pulse ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80000000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;
    for (int i = 0; i < 8; i++) step(sim);
    uint32_t half_data_a[8] = {0x01000013, 0x02000013, 0x03000013, 0x04000013,
                               0x05000013, 0x06000013, 0x07000013, 0x08000013};
    uint32_t half_data_b[8] = {0x11000013, 0x12000013, 0x13000013, 0x14000013,
                               0x15000013, 0x16000013, 0x17000013, 0x18000013};
    sim.dut->f6_icache_resp_valid_i = 1;
    sim.dut->f6_icache_resp_miss_i = 0;
    pack_resp(sim.dut.get(), false, false, true, false, false, half_data_a);
    step_half(sim, 0);
    pack_resp(sim.dut.get(), false, false, true, false, false, half_data_b);
    step_half(sim, 1);
    sim.dut->f6_icache_resp_valid_i = 0;
    for (int i = 0; i < 24; i++) step(sim);

    printf("=== test 7e: half-cycle random (4000 iterations) ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80000000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    uint32_t half_seed = 0x31415926u;
    int half_prev_f5_miss = 0;
    for (int i = 0; i < 4000; i++) {
        drive_half_random(sim.dut.get(), half_seed, half_prev_f5_miss, false);
        step_half(sim, 0);
        drive_half_random(sim.dut.get(), half_seed, half_prev_f5_miss, true);
        step_half(sim, 1);
    }

    // Heavy random with real data — drives f5_icache_resp_miss_i properly
    printf("=== test 8: random stimulus (20000 cycles) ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80000000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    uint32_t seed = 0xFACEFEED;
    int prev_f5_miss = 0;
    for (int i = 0; i < 20000; i++) {
        seed = seed * 1103515245 + 12345;
        sim.dut->f0_icache_req_ready_i  = (seed >> 0) & 1;
        sim.dut->f1_icache_req_ready_i  = (seed >> 1) & 1;
        sim.dut->f7_inst_ready_i        = (seed >> 2) & 1;
        // Pipeline the miss signal: f5 miss this cycle becomes f6 miss next cycle
        int this_miss = (seed >> 4) & 1;
        sim.dut->f5_icache_resp_miss_i  = this_miss;
        sim.dut->f6_icache_resp_valid_i = (seed >> 3) & 1;
        sim.dut->f6_icache_resp_miss_i  = prev_f5_miss;
        prev_f5_miss = this_miss;
        sim.dut->f0_core_req_valid_i    = ((seed >> 5) & 0x1F) == 0;
        sim.dut->f7_icache_fill_done_i  = ((seed >> 10) & 0xF) == 0;
        if (sim.dut->f6_icache_resp_valid_i) {
            uint32_t d[8];
            for (int w = 0; w < 8; w++) d[w] = (seed * (w+1)) | 3; // bits[1:0]=11
            pack_resp(sim.dut.get(), false, false, true, false, false, d);
        }
        step(sim);
    }

    // Second seed with compressed data
    printf("=== test 9: random with compressed (10000 cycles) ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0xFFFF8000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    seed = 0xDEADBEEF;
    prev_f5_miss = 0;
    for (int i = 0; i < 10000; i++) {
        seed = seed * 1103515245 + 12345;
        sim.dut->f0_icache_req_ready_i  = (seed >> 0) & 1;
        sim.dut->f1_icache_req_ready_i  = (seed >> 1) & 1;
        sim.dut->f7_inst_ready_i        = (seed >> 2) & 1;
        int this_miss = (seed >> 4) & 1;
        sim.dut->f5_icache_resp_miss_i  = this_miss;
        sim.dut->f6_icache_resp_valid_i = (seed >> 3) & 1;
        sim.dut->f6_icache_resp_miss_i  = prev_f5_miss;
        prev_f5_miss = this_miss;
        sim.dut->f0_core_req_valid_i    = ((seed >> 5) & 0x1F) == 0;
        sim.dut->f7_icache_fill_done_i  = ((seed >> 10) & 0xF) == 0;
        if (sim.dut->f6_icache_resp_valid_i) {
            uint32_t d[8];
            for (int w = 0; w < 8; w++) d[w] = seed * (w+1); // mixed 32/16-bit
            pack_resp(sim.dut.get(),
                      ((seed >> 20) & 0xF) == 0,  // occasional pf
                      ((seed >> 24) & 0xF) == 0,  // occasional af
                      true,
                      ((seed >> 28) & 0x1F) == 0,  // occasional bus_err
                      false, d);
        }
        step(sim);
    }

    // Third seed with thread disable/enable
    printf("=== test 10: random with disable (10000 cycles) ===\n");
    idle_all(sim.dut.get());
    sim.reset();
    idle_all(sim.dut.get());
    sim.dut->f0_thread_enabled_i = 1;
    sim.dut->f0_enable_thread_i = 1;
    sim.dut->f0_reset_vector_i = 0x80004000ULL;
    sim.dut->f0_icache_req_ready_i = 1;
    sim.dut->f1_icache_req_ready_i = 1;
    sim.dut->f7_inst_ready_i = 1;
    step(sim);
    sim.dut->f0_enable_thread_i = 0;

    seed = 0x12345678;
    prev_f5_miss = 0;
    for (int i = 0; i < 10000; i++) {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        sim.dut->f0_icache_req_ready_i  = (seed >> 0) & 1;
        sim.dut->f1_icache_req_ready_i  = (seed >> 1) & 1;
        sim.dut->f7_inst_ready_i        = (seed >> 2) & 1;
        int this_miss = (seed >> 4) & 1;
        sim.dut->f5_icache_resp_miss_i  = this_miss;
        sim.dut->f6_icache_resp_valid_i = (seed >> 3) & 1;
        sim.dut->f6_icache_resp_miss_i  = prev_f5_miss;
        prev_f5_miss = this_miss;
        sim.dut->f0_core_req_valid_i    = ((seed >> 5) & 0x1F) == 0;
        sim.dut->f7_icache_fill_done_i  = ((seed >> 10) & 0xF) == 0;
        sim.dut->io_halt_i             = ((seed >> 18) & 0x7F) == 0;
        if (((seed >> 15) & 0x3F) == 0) {
            sim.dut->f0_disable_thread_i = 1;
            sim.dut->f0_thread_enabled_i = 0;
        } else {
            sim.dut->f0_disable_thread_i = 0;
            sim.dut->f0_thread_enabled_i = 1;
        }
        if (sim.dut->f6_icache_resp_valid_i) {
            uint32_t d[8];
            for (int w = 0; w < 8; w++) d[w] = seed ^ (w * 0xAAAAAAAA);
            pack_resp(sim.dut.get(), false, false, true, false, false, d);
        }
        step(sim);
    }
    return sim.finish();
}

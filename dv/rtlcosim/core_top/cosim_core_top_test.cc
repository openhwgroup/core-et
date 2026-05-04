// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original core_top vs new core_top.

#include "Vcosim_core_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

using DUT = Vcosim_core_top_tb;

namespace {

constexpr int kRandomCycles = 12000;

uint32_t rng_state = 0x51C0A7E5u;

uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

uint64_t xorshift64() {
  return (static_cast<uint64_t>(xorshift32()) << 32) | xorshift32();
}

void fill_random_bytes(void* ptr, size_t size) {
  auto* bytes = static_cast<unsigned char*>(ptr);
  for (size_t i = 0; i < size; ++i) {
    bytes[i] = static_cast<unsigned char>(xorshift32());
  }
}

template <typename T>
void zero_obj(T& obj) {
  std::memset(&obj, 0, sizeof(obj));
}

template <typename T>
void fill_random_obj(T& obj) {
  fill_random_bytes(&obj, sizeof(obj));
}

uint32_t pack_vpu_dcache_ctl(bool read_en,
                             uint32_t way,
                             uint32_t addr,
                             bool size64,
                             bool size256,
                             bool bid,
                             bool tenb_credit,
                             uint32_t tenb_credit_entry,
                             bool tfma_enabled,
                             bool reduce_wait,
                             bool tfma_rf_write) {
  uint32_t raw = 0;
  raw |= (read_en ? 1u : 0u) << 21;
  raw |= (way & 0x3u) << 19;
  raw |= (addr & 0x3ffu) << 9;
  raw |= (size64 ? 1u : 0u) << 8;
  raw |= (size256 ? 1u : 0u) << 7;
  raw |= (bid ? 1u : 0u) << 6;
  raw |= (tenb_credit ? 1u : 0u) << 5;
  raw |= (tenb_credit_entry & 0x3u) << 3;
  raw |= (tfma_enabled ? 1u : 0u) << 2;
  raw |= (reduce_wait ? 1u : 0u) << 1;
  raw |= tfma_rf_write ? 1u : 0u;
  return raw;
}

uint32_t make_valid_vpu_dcache_ctl() {
  const uint32_t mode = xorshift32() & 0x7u;
  switch (mode) {
    case 0:
    case 1: {
      const uint32_t size_mode = xorshift32() % 3u;  // 0=32b, 1=64b, 2=256b
      return pack_vpu_dcache_ctl(
          true,
          xorshift32() & 0x3u,
          xorshift32() & 0x3ffu,
          size_mode == 1u,
          size_mode == 2u,
          xorshift32() & 1u,
          false,
          0,
          false,
          false,
          false);
    }
    case 2:
      return pack_vpu_dcache_ctl(
          false, 0, 0, false, false, false, true, xorshift32() & 0x3u, false, false, false);
    case 3:
      return pack_vpu_dcache_ctl(
          false, 0, 0, false, false, false, false, 0, true, xorshift32() & 1u, xorshift32() & 1u);
    default:
      return 0;
  }
}

template <typename T>
void print_bits(const T& value) {
  if constexpr (sizeof(T) <= sizeof(uint64_t)) {
    uint64_t raw = 0;
    std::memcpy(&raw, &value, sizeof(T));
    std::printf("0x%llx", static_cast<unsigned long long>(raw));
  } else {
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    std::printf("0x");
    for (int i = static_cast<int>(sizeof(T)) - 1; i >= 0; --i) {
      std::printf("%02x", bytes[i]);
    }
  }
}

template <typename T>
void compare_bits(CosimCtrl<DUT>& sim, const char* name, const T& orig, const T& newv) {
  sim.comparisons++;
  if (std::memcmp(&orig, &newv, sizeof(T)) != 0) {
    sim.mismatches++;
    std::printf("  MISMATCH [t=%lu] %s: orig=",
                static_cast<unsigned long>(sim.sim_time), name);
    print_bits(orig);
    std::printf(" new=");
    print_bits(newv);
    std::printf("\n");
  }
}

#define CMP(sig) compare_bits(sim, #sig, d->orig_##sig, d->new_##sig)

void compare_all(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();

  CMP(l2_dcache_evict_req_valid);
  CMP(l2_dcache_evict_req);
  CMP(l2_dcache_miss_req_valid);
  CMP(l2_dcache_miss_req);
  CMP(l2_dcache_resp_ready);
  CMP(fe_req_valid_int);
  CMP(fe_req_int);
  CMP(fe_stall_int);
  CMP(fe_resp_ready_int);
  CMP(fe_resp_valid_int);
  CMP(fe_resp_thread_id_int);
  CMP(fe_resp_int);
  CMP(fe_thread_enabled_f_int);
  CMP(fe_thread_enabled_ff_int);
  CMP(fe_tb_req_valid_int);
  CMP(fe_tb_req_int);
  CMP(fe_tb_req_ready_int);
  CMP(fe_req_ready_arb_int);
  CMP(fe_tb_inst_valid_int);
  CMP(fe_tb_inst_bits_int);
  CMP(fe_tb_data_int);
  CMP(fe_fifo_head_data_int);
  CMP(fe_fifo_pending_data_int);
  CMP(fe_fifo_rd_ptr_int);
  CMP(fe_fifo_wr_ptr_int);
  CMP(fe_inst_fifo_empty_int);
  CMP(fe_inst_fifo_full_int);
  CMP(fe_exp_thread_id_int);
  CMP(vm_status_int);
  CMP(id_intpipe_ctrl_int);
  CMP(id_frontend_vpu_ctrl_int);
  CMP(debug_ffb_wdata_int);
  CMP(debug_ffb_en_int);
  CMP(debug_ffb_thread_sel_int);
  CMP(debug_ex_program_buffer_int);
  CMP(debug_halt_int);
  CMP(pending_halt_int);
  CMP(dcache_core_ctrl_int);
  CMP(dcache_core_ctrl_resp_int);
  CMP(dcache_req_int);
  CMP(dcache_req_valid_int);
  CMP(dcache_req_gsc_int);
  CMP(dcache_resp_int);
  CMP(dcache_resp_valid_int);
  CMP(dcache_scoreboard_int);
  CMP(dcache_debug_sigs_int);
  CMP(dcache_col2_state_int);
  CMP(dcache_col2_req_int);
  CMP(dcache_col2_req_addr_int);
  CMP(dcache_col2_req_count_int);
  CMP(dcache_col2_id_running_int);
  CMP(dcache_col2_l2_scp_dest_int);
  CMP(dcache_col2_err_mem_int);
  CMP(dcache_col2_cache_op_req_is_l2_int);
  CMP(dcache_col2_cache_op_req_is_nop_int);
  CMP(dcache_col2_l2_cache_op_cnt_int);
  CMP(dcache_col2_reprate_count_int);
  CMP(dcache_col2_reprate_grant_int);
  CMP(dcache_col2_l2_req_valid_started_int);
  CMP(dcache_col2_ready_counters_int);
  CMP(dcache_col2_l2_resp_valid_int);
  CMP(dcache_col2_l2_resp_id_int);
  CMP(dcache_col2_l2_resp_dest_int);
  CMP(dcache_col2_l2_resp_wdata_int);
  CMP(dcache_col2_l2_resp_opcode_int);
  CMP(dcache_col2_l2_resp_data_int);
  CMP(dcache_col2_l2_resp_size_int);
  CMP(dcache_col2_l2_resp_qwen_int);
  CMP(dcache_col2_l2_resp_data_hi_int);
  CMP(dcache_l2_resp_in_data_hi_int);
  CMP(dcache_col2_l2_resp_is_prefetch_int);
  CMP(dcache_col2_pf_l2_evict_ready_int);
  CMP(dcache_s3_lrsc_valid_int);
  CMP(icache_req_valid);
  CMP(icache_req);
  CMP(icache_flush_data);
  CMP(satp_info);
  CMP(matp_info);
  CMP(tlb_invalidate);
  CMP(dc_ptw_req_data);
  CMP(dc_ptw_req_valid);
  CMP(id_vpu_req);
  CMP(id_vpu_decoder_sigs);
  CMP(ex_vpu_req);
  CMP(tag_vpu_kill);
  CMP(mem_vpu_kill);
  CMP(wb_vpu_kill);
  CMP(f0_vpu_ctrl);
  CMP(wb_dcache_vpu_resp_valid);
  CMP(wb_dcache_vpu_resp);
  CMP(dcache_vpu_scp_res);
  CMP(dcache_vpu_scp_data);
  CMP(dcache_vpu_tenb_data);
  CMP(vpu_reduce_ctrl);
  CMP(flb_neigh_req_valid);
  CMP(flb_neigh_req_data);
  CMP(trace_encoder);
  CMP(apb_pready);
  CMP(apb_prdata);
  CMP(apb_pslverr);
  CMP(debug_out);
  CMP(pmu_count_up);
  CMP(pmu_read_sel);
  CMP(pmu_write_en);
  CMP(pmu_write_data);
  CMP(pmu_neigh_event_sel);
  CMP(sm_match_debug_signals_tl0);
  CMP(sm_filter_debug_signals_tl0);
  CMP(sm_data_debug_signals_tl0);
  CMP(sm_match_debug_signals_tl1);
  CMP(sm_filter_debug_signals_tl1);
  CMP(sm_data_debug_signals_tl1);
  CMP(sm_match_debug_signals_ts);
  CMP(sm_filter_debug_signals_ts);
  CMP(sm_data_debug_signals_ts);
  CMP(sm_match_debug_signals_co);
  CMP(sm_filter_debug_signals_co);
  CMP(sm_data_debug_signals_co);
  CMP(sm_match_debug_signals_col2);
  CMP(sm_filter_debug_signals_col2);
  CMP(sm_data_debug_signals_col2);
  CMP(sm_match_debug_signals_mh);
  CMP(sm_filter_debug_signals_mh);
  CMP(sm_data_debug_signals_mh);
  CMP(sm_match_debug_signals_dc);
  CMP(sm_filter_debug_signals_dc);
  CMP(sm_data_debug_signals_dc);
  CMP(intpipe_debug_monitor);
}

#undef CMP

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_all(sim);
}

void clear_inputs(DUT* d) {
  d->stim_dft = 0;
  d->stim_ioshire = 0;
  d->stim_chicken_bit_dcache = 0;
  d->stim_chicken_bit_intpipe = 0;
  d->stim_chicken_bit_frontend = 0;
  d->stim_l2_dcache_evict_req_ready = 0xffu;
  d->stim_l2_dcache_miss_req_ready = 0xffu;
  d->stim_l2_dcache_resp_valid = 0;
  zero_obj(d->stim_l2_dcache_resp);
  d->stim_icache_req_ready = 1;
  d->stim_icache_resp_valid = 0;
  d->stim_icache_resp_miss = 0;
  zero_obj(d->stim_icache_resp);
  d->stim_icache_fill_done = 0;
  d->stim_dc_ptw_req_ready = 1;
  zero_obj(d->stim_ptw_dc_resp_data);
  d->stim_ptw_dc_resp_valid = 0;
  zero_obj(d->stim_interrupts);
  d->stim_shire_id = 0;
  d->stim_shire_min_id = 0;
  d->stim_enabled = 1;
  d->stim_reset_vector = 0x1000;
  zero_obj(d->stim_mprot);
  d->stim_vmspagesize = 0;
  zero_obj(d->stim_vpu_dcache_ctl);
  zero_obj(d->stim_id_vpu_ctrl);
  zero_obj(d->stim_ex_vpu_ctrl);
  zero_obj(d->stim_f2_vpu_ctrl);
  zero_obj(d->stim_f3_vpu_ctrl);
  zero_obj(d->stim_wb_vpu_ctrl);
  d->stim_flb_neigh_resp_valid = 0;
  d->stim_flb_neigh_resp_data = 0;
  d->stim_te_thread_sel = 0;
  d->stim_te_enable = 0;
  d->stim_apb_paddr = 0;
  d->stim_apb_pwrite = 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = 0;
  zero_obj(d->stim_debug_in);
  zero_obj(d->stim_esr_features);
  d->stim_esr_bypass_dcache = 0;
  d->stim_esr_shire_coop_mode = 0;
  d->stim_mem_ctrl_override = 0;
  zero_obj(d->stim_pmu_read_data);
  d->stim_io_events_vpu = 0;
}

struct ApbState {
  int phase = 0;
  uint16_t addr = 0;
  bool write = false;
  uint64_t data = 0;
  int wait_cycles = 0;
};

void drive_apb(DUT* d, ApbState& apb, bool ready_seen, bool allow_new_txn) {
  d->stim_apb_paddr = apb.addr;
  d->stim_apb_pwrite = apb.write ? 1 : 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = apb.data;

  if (!allow_new_txn) {
    apb.phase = 0;
    apb.wait_cycles = 0;
    return;
  }

  if (apb.phase == 0) {
    if ((xorshift32() & 0x1f) == 0) {
      apb.addr = static_cast<uint16_t>(xorshift32());
      apb.write = (xorshift32() & 1) != 0;
      apb.data = xorshift64();
      apb.phase = 1;
      apb.wait_cycles = 0;
      d->stim_apb_paddr = apb.addr;
      d->stim_apb_pwrite = apb.write ? 1 : 0;
      d->stim_apb_psel = 1;
      d->stim_apb_pwdata = apb.data;
    }
    return;
  }

  if (apb.phase == 1) {
    d->stim_apb_psel = 1;
    apb.phase = 2;
    return;
  }

  d->stim_apb_psel = 1;
  d->stim_apb_penable = 1;
  apb.wait_cycles++;
  if (ready_seen || apb.wait_cycles > 8) {
    apb.phase = 0;
    apb.wait_cycles = 0;
  }
}

void randomize_functional_inputs(DUT* d) {
  d->stim_ioshire = xorshift32() & 1;
  d->stim_chicken_bit_dcache = xorshift32() & 1;
  d->stim_chicken_bit_intpipe = xorshift32() & 1;
  d->stim_chicken_bit_frontend = xorshift32() & 1;
  d->stim_l2_dcache_evict_req_ready = xorshift32();
  d->stim_l2_dcache_miss_req_ready = xorshift32();
  d->stim_icache_req_ready = (xorshift32() & 0x3) != 0;
  d->stim_dc_ptw_req_ready = (xorshift32() & 0x3) != 0;
  fill_random_obj(d->stim_interrupts);
  d->stim_shire_id = xorshift32();
  d->stim_shire_min_id = xorshift32();
  d->stim_enabled = xorshift32() & 0x3;
  if (d->stim_enabled == 0) d->stim_enabled = 1;
  d->stim_reset_vector = 0x1000ull + ((static_cast<uint64_t>(xorshift32()) & 0x1ffull) << 2);
  fill_random_obj(d->stim_mprot);
  d->stim_vmspagesize = xorshift32();
  d->stim_vpu_dcache_ctl = make_valid_vpu_dcache_ctl();
  fill_random_obj(d->stim_id_vpu_ctrl);
  fill_random_obj(d->stim_ex_vpu_ctrl);
  fill_random_obj(d->stim_f2_vpu_ctrl);
  fill_random_obj(d->stim_f3_vpu_ctrl);
  fill_random_obj(d->stim_wb_vpu_ctrl);
  d->stim_flb_neigh_resp_valid = xorshift32() & 1;
  d->stim_flb_neigh_resp_data = xorshift32() & 1;
  d->stim_te_thread_sel = xorshift32() & 1;
  d->stim_te_enable = xorshift32() & 1;
  fill_random_obj(d->stim_debug_in);
  fill_random_obj(d->stim_esr_features);
  d->stim_esr_bypass_dcache = xorshift32() & 1;
  d->stim_esr_shire_coop_mode = xorshift32() & 1;
  d->stim_mem_ctrl_override = xorshift32();
  fill_random_obj(d->stim_pmu_read_data);
  d->stim_io_events_vpu = xorshift32();
}

void maybe_drive_responses(DUT* d) {
  d->stim_l2_dcache_resp_valid = 0;
  zero_obj(d->stim_l2_dcache_resp);
  d->stim_icache_resp_valid = 0;
  d->stim_icache_resp_miss = 0;
  zero_obj(d->stim_icache_resp);
  d->stim_icache_fill_done = 0;
  zero_obj(d->stim_ptw_dc_resp_data);
  d->stim_ptw_dc_resp_valid = 0;

  if (((d->orig_icache_req_valid && d->new_icache_req_valid) && (xorshift32() & 1) == 0) ||
      ((xorshift32() & 0x1f) == 0)) {
    d->stim_icache_resp_valid = 1;
    d->stim_icache_resp_miss = xorshift32() & 1;
    d->stim_icache_fill_done = xorshift32() & 1;
    fill_random_obj(d->stim_icache_resp);
  }

  if (((d->orig_l2_dcache_resp_ready && d->new_l2_dcache_resp_ready) && (xorshift32() & 0x3) == 0) ||
      ((xorshift32() & 0x3f) == 0)) {
    d->stim_l2_dcache_resp_valid = 1;
    fill_random_obj(d->stim_l2_dcache_resp);
  }

  if (((d->orig_dc_ptw_req_valid && d->new_dc_ptw_req_valid) && d->stim_dc_ptw_req_ready &&
       (xorshift32() & 0x3) == 0) || ((xorshift32() & 0x3f) == 0)) {
    d->stim_ptw_dc_resp_valid = 1;
    fill_random_obj(d->stim_ptw_dc_resp_data);
  }
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 50000000;

  auto* d = sim.dut.get();
  clear_inputs(d);

  d->rst_c_ni = 0;
  d->rst_w_ni = 0;
  d->rst_d_ni = 0;
  for (int i = 0; i < 8; ++i) {
    tick_and_compare(sim);
  }

  d->rst_c_ni = 1;
  d->rst_w_ni = 1;
  d->rst_d_ni = 1;
  for (int i = 0; i < 4; ++i) {
    tick_and_compare(sim);
  }

  for (int i = 0; i < 8; ++i) {
    clear_inputs(d);
    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    tick_and_compare(sim);
  }

  clear_inputs(d);
  d->rst_c_ni = 1;
  d->rst_w_ni = 1;
  d->rst_d_ni = 1;
  d->stim_dft = 0x07;
  tick_and_compare(sim);
  d->stim_dft = 0x10;
  tick_and_compare(sim);
  d->stim_dft = 0x18;
  tick_and_compare(sim);
  d->stim_dft = 0x00;
  tick_and_compare(sim);

  ApbState apb;

  for (int cycle = 0; cycle < kRandomCycles; ++cycle) {
    clear_inputs(d);

    d->rst_c_ni = 1;
    d->rst_w_ni = 1;
    d->rst_d_ni = 1;
    if (cycle >= 64 && cycle < 66) d->rst_d_ni = 0;
    if (cycle >= 128 && cycle < 130) d->rst_w_ni = 0;
    if (cycle >= 192 && cycle < 194) d->rst_c_ni = 0;

    const bool any_reset = !(d->rst_c_ni && d->rst_w_ni && d->rst_d_ni);
    if (!any_reset) {
      randomize_functional_inputs(d);
      maybe_drive_responses(d);
    } else {
      apb.phase = 0;
      apb.wait_cycles = 0;
    }

    const bool apb_ready_seen = d->orig_apb_pready && d->new_apb_pready;
    drive_apb(d, apb, apb_ready_seen, !any_reset);

    tick_and_compare(sim);
  }

  return sim.finish();
}

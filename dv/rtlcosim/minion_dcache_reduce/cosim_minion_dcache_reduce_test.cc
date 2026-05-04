// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_reduce vs minion_dcache_reduce.

#include "Vcosim_minion_dcache_reduce_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_reduce_tb;

namespace {

constexpr int kRandomCycles = 12000;
constexpr int kTensorstoreScpBit = 48;

struct BoolCoverage {
  bool seen0 = false;
  bool seen1 = false;
  void sample(bool v) {
    if (v) seen1 = true;
    else seen0 = true;
  }
  bool full() const { return seen0 && seen1; }
};

struct NonZeroCoverage {
  bool seen_zero = false;
  bool seen_nonzero = false;
  void sample(uint64_t v) {
    if (v == 0) seen_zero = true;
    else seen_nonzero = true;
  }
  bool full() const { return seen_zero && seen_nonzero; }
};

struct Coverage {
  BoolCoverage chicken_bit_dcache, reduce_start, tensorstore_start, thread_id;
  BoolCoverage l2_resp_valid, re_l2_req_ready, ts_l2_req_ready;
  BoolCoverage s0_available, s1_available, s1_tlb_wait, s1_tlb_fail;
  BoolCoverage ba_full, s2_valid, s1_valid, ba_read_used, scp_ctrl_on;
  BoolCoverage vpu_reduce_wait, mh_going_to_uc_store, tensorstore_scp;
  NonZeroCoverage global_min_id, tensorstore_stride, ctrl_bits;
  NonZeroCoverage l2_resp_opcode, l2_resp_id, l2_resp_data0, l2_resp_data1, l2_resp_data2, l2_resp_data3;
  NonZeroCoverage s2_addr, s2_ba_written_entry, s1_ba_data;
};

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

uint64_t make_reduce_ctrl(uint32_t seed) {
  const uint64_t opcode = uint64_t(seed & 0x7u);
  const uint64_t count = uint64_t((seed >> 3) & 0xfu);
  const uint64_t rows = uint64_t((seed >> 7) & 0xfu);
  const uint64_t cols = uint64_t((seed >> 11) & 0x3u);
  const uint64_t pair = uint64_t((seed >> 13) & 0xffu);
  return opcode | (count << 16) | (rows << 51) | (cols << 55) | (pair << 32);
}

uint64_t set_tensorstore_scp(uint64_t ctrl_bits, bool scp) {
  ctrl_bits &= ~(uint64_t{1} << kTensorstoreScpBit);
  ctrl_bits |= uint64_t{scp} << kTensorstoreScpBit;
  return ctrl_bits;
}

void clear_inputs(DUT* dut) {
  dut->global_min_id_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->reduce_start_i = 0;
  dut->tensorstore_start_i = 0;
  dut->tensorstore_stride_i = 0;
  dut->ctrl_bits_i = 0;
  dut->thread_id_i = 0;
  dut->l2_resp_valid_i = 0;
  dut->l2_resp_opcode_i = 0;
  dut->l2_resp_id_i = 0;
  for (int i = 0; i < 4; ++i) dut->l2_resp_data_i[i] = 0;
  dut->re_l2_req_ready_i = 0;
  dut->ts_l2_req_ready_i = 0;
  dut->s0_available_i = 0;
  dut->s1_available_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->s2_addr_i = 0;
  dut->ba_full_i = 0;
  dut->s2_valid_i = 0;
  dut->s2_ba_written_entry_i = 0;
  dut->s1_valid_i = 0;
  dut->ba_read_used_i = 0;
  dut->s1_ba_data_i = 0;
  dut->scp_ctrl_on_i = 0;
  dut->vpu_reduce_wait_i = 0;
  dut->mh_going_to_uc_store_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.chicken_bit_dcache.sample(dut->chicken_bit_dcache_i);
  cov.reduce_start.sample(dut->reduce_start_i);
  cov.tensorstore_start.sample(dut->tensorstore_start_i);
  cov.thread_id.sample(dut->thread_id_i);
  cov.l2_resp_valid.sample(dut->l2_resp_valid_i);
  cov.re_l2_req_ready.sample(dut->re_l2_req_ready_i);
  cov.ts_l2_req_ready.sample(dut->ts_l2_req_ready_i);
  cov.s0_available.sample(dut->s0_available_i);
  cov.s1_available.sample(dut->s1_available_i);
  cov.s1_tlb_wait.sample(dut->s1_tlb_wait_i);
  cov.s1_tlb_fail.sample(dut->s1_tlb_fail_i);
  cov.ba_full.sample(dut->ba_full_i);
  cov.s2_valid.sample(dut->s2_valid_i);
  cov.s1_valid.sample(dut->s1_valid_i);
  cov.ba_read_used.sample(dut->ba_read_used_i);
  cov.scp_ctrl_on.sample(dut->scp_ctrl_on_i);
  cov.vpu_reduce_wait.sample(dut->vpu_reduce_wait_i);
  cov.mh_going_to_uc_store.sample(dut->mh_going_to_uc_store_i);
  cov.tensorstore_scp.sample(((dut->ctrl_bits_i >> kTensorstoreScpBit) & 1u) != 0u);
  cov.global_min_id.sample(dut->global_min_id_i);
  cov.tensorstore_stride.sample(dut->tensorstore_stride_i);
  cov.ctrl_bits.sample(dut->ctrl_bits_i);
  cov.l2_resp_opcode.sample(dut->l2_resp_opcode_i);
  cov.l2_resp_id.sample(dut->l2_resp_id_i);
  cov.l2_resp_data0.sample(dut->l2_resp_data_i[0]);
  cov.l2_resp_data1.sample(dut->l2_resp_data_i[1]);
  cov.l2_resp_data2.sample(dut->l2_resp_data_i[2]);
  cov.l2_resp_data3.sample(dut->l2_resp_data_i[3]);
  cov.s2_addr.sample(dut->s2_addr_i);
  cov.s2_ba_written_entry.sample(dut->s2_ba_written_entry_i);
  cov.s1_ba_data.sample(dut->s1_ba_data_i);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.chicken_bit_dcache.full(), "coverage: chicken_bit_dcache exercised 0/1");
  sim.check(cov.reduce_start.full(), "coverage: reduce_start exercised 0/1");
  sim.check(cov.tensorstore_start.full(), "coverage: tensorstore_start exercised 0/1");
  sim.check(cov.thread_id.full(), "coverage: thread_id exercised 0/1");
  sim.check(cov.l2_resp_valid.full(), "coverage: l2_resp_valid exercised 0/1");
  sim.check(cov.re_l2_req_ready.full(), "coverage: re_l2_req_ready exercised 0/1");
  sim.check(cov.ts_l2_req_ready.full(), "coverage: ts_l2_req_ready exercised 0/1");
  sim.check(cov.s0_available.full(), "coverage: s0_available exercised 0/1");
  sim.check(cov.s1_available.full(), "coverage: s1_available exercised 0/1");
  sim.check(cov.s1_tlb_wait.full(), "coverage: s1_tlb_wait exercised 0/1");
  sim.check(cov.s1_tlb_fail.full(), "coverage: s1_tlb_fail exercised 0/1");
  sim.check(cov.ba_full.full(), "coverage: ba_full exercised 0/1");
  sim.check(cov.s2_valid.full(), "coverage: s2_valid exercised 0/1");
  sim.check(cov.s1_valid.full(), "coverage: s1_valid exercised 0/1");
  sim.check(cov.ba_read_used.full(), "coverage: ba_read_used exercised 0/1");
  sim.check(cov.scp_ctrl_on.full(), "coverage: scp_ctrl_on exercised 0/1");
  sim.check(cov.vpu_reduce_wait.full(), "coverage: vpu_reduce_wait exercised 0/1");
  sim.check(cov.mh_going_to_uc_store.full(), "coverage: mh_going_to_uc_store exercised 0/1");
  sim.check(cov.tensorstore_scp.full(), "coverage: tensorstore_scp.scp exercised 0/1");
  sim.check(cov.global_min_id.full(), "coverage: global_min_id exercised zero/nonzero");
  sim.check(cov.tensorstore_stride.full(), "coverage: tensorstore_stride exercised zero/nonzero");
  sim.check(cov.ctrl_bits.full(), "coverage: ctrl_bits exercised zero/nonzero");
  sim.check(cov.l2_resp_opcode.full(), "coverage: l2_resp_opcode exercised zero/nonzero");
  sim.check(cov.l2_resp_id.full(), "coverage: l2_resp_id exercised zero/nonzero");
  sim.check(cov.l2_resp_data0.full(), "coverage: l2_resp_data[0] exercised zero/nonzero");
  sim.check(cov.l2_resp_data1.full(), "coverage: l2_resp_data[1] exercised zero/nonzero");
  sim.check(cov.l2_resp_data2.full(), "coverage: l2_resp_data[2] exercised zero/nonzero");
  sim.check(cov.l2_resp_data3.full(), "coverage: l2_resp_data[3] exercised zero/nonzero");
  sim.check(cov.s2_addr.full(), "coverage: s2_addr exercised zero/nonzero");
  sim.check(cov.s2_ba_written_entry.full(), "coverage: s2_ba_written_entry exercised zero/nonzero");
  sim.check(cov.s1_ba_data.full(), "coverage: s1_ba_data exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("reduce_ready", sim.dut->orig_reduce_ready_o, sim.dut->new_reduce_ready_o);
  sim.compare("tensorstore_busy", sim.dut->orig_tensorstore_busy_o, sim.dut->new_tensorstore_busy_o);
  sim.compare("buffer_avail", sim.dut->orig_buffer_avail_o, sim.dut->new_buffer_avail_o);
  sim.compare("re_l2_req_valid", sim.dut->orig_re_l2_req_valid_o, sim.dut->new_re_l2_req_valid_o);
  sim.compare("ts_l2_req_valid", sim.dut->orig_ts_l2_req_valid_o, sim.dut->new_ts_l2_req_valid_o);
  sim.compare("re_l2_req_id", sim.dut->orig_re_l2_req_id_o, sim.dut->new_re_l2_req_id_o);
  sim.compare("ts_l2_req_id", sim.dut->orig_ts_l2_req_id_o, sim.dut->new_ts_l2_req_id_o);
  sim.compare("re_l2_req_opcode", sim.dut->orig_re_l2_req_opcode_o, sim.dut->new_re_l2_req_opcode_o);
  sim.compare("ts_l2_req_opcode", sim.dut->orig_ts_l2_req_opcode_o, sim.dut->new_ts_l2_req_opcode_o);
  sim.compare("re_l2_req_subopcode", sim.dut->orig_re_l2_req_subopcode_o, sim.dut->new_re_l2_req_subopcode_o);
  sim.compare("ts_l2_req_subopcode", sim.dut->orig_ts_l2_req_subopcode_o, sim.dut->new_ts_l2_req_subopcode_o);
  sim.compare("re_l2_req_address", sim.dut->orig_re_l2_req_address_o, sim.dut->new_re_l2_req_address_o);
  sim.compare("ts_l2_req_address", sim.dut->orig_ts_l2_req_address_o, sim.dut->new_ts_l2_req_address_o);
  for (int i = 0; i < 8; ++i) {
    sim.compare("re_l2_req_data[" + std::to_string(i) + "]",
                sim.dut->orig_re_l2_req_data_o[i], sim.dut->new_re_l2_req_data_o[i]);
    sim.compare("ts_l2_req_data[" + std::to_string(i) + "]",
                sim.dut->orig_ts_l2_req_data_o[i], sim.dut->new_ts_l2_req_data_o[i]);
  }
  sim.compare("re_l2_req_size", sim.dut->orig_re_l2_req_size_o, sim.dut->new_re_l2_req_size_o);
  sim.compare("ts_l2_req_size", sim.dut->orig_ts_l2_req_size_o, sim.dut->new_ts_l2_req_size_o);
  sim.compare("re_l2_req_qwen", sim.dut->orig_re_l2_req_qwen_o, sim.dut->new_re_l2_req_qwen_o);
  sim.compare("ts_l2_req_qwen", sim.dut->orig_ts_l2_req_qwen_o, sim.dut->new_ts_l2_req_qwen_o);
  sim.compare("re_ts_l2_req_inflight", sim.dut->orig_re_ts_l2_req_inflight_o,
              sim.dut->new_re_ts_l2_req_inflight_o);
  sim.compare("pipe_used", sim.dut->orig_pipe_used_o, sim.dut->new_pipe_used_o);
  sim.compare("s1_inst_send_val", sim.dut->orig_s1_inst_send_val_o, sim.dut->new_s1_inst_send_val_o);
  sim.compare("s2_inst_send_val", sim.dut->orig_s2_inst_send_val_o, sim.dut->new_s2_inst_send_val_o);
  sim.compare("s3_inst_send_val", sim.dut->orig_s3_inst_send_val_o, sim.dut->new_s3_inst_send_val_o);
  sim.compare("s1_inst_op_val", sim.dut->orig_s1_inst_op_val_o, sim.dut->new_s1_inst_op_val_o);
  sim.compare("s2_inst_op_val", sim.dut->orig_s2_inst_op_val_o, sim.dut->new_s2_inst_op_val_o);
  sim.compare("s3_inst_op_val", sim.dut->orig_s3_inst_op_val_o, sim.dut->new_s3_inst_op_val_o);
  sim.compare("ts_scp_read", sim.dut->orig_ts_scp_read_o, sim.dut->new_ts_scp_read_o);
  sim.compare("ts_scp_read_way", sim.dut->orig_ts_scp_read_way_o, sim.dut->new_ts_scp_read_way_o);
  sim.compare("ts_scp_read_addr", sim.dut->orig_ts_scp_read_addr_o, sim.dut->new_ts_scp_read_addr_o);
  sim.compare("ts_col_size_is_128b", sim.dut->orig_ts_col_size_is_128b_o,
              sim.dut->new_ts_col_size_is_128b_o);
  sim.compare("s0_addr_load", sim.dut->orig_s0_addr_load_o, sim.dut->new_s0_addr_load_o);
  sim.compare("s0_addr", sim.dut->orig_s0_addr_o, sim.dut->new_s0_addr_o);
  sim.compare("s0_addr_msb_err", sim.dut->orig_s0_addr_msb_err_o, sim.dut->new_s0_addr_msb_err_o);
  sim.compare("s0_thread_id", sim.dut->orig_s0_thread_id_o, sim.dut->new_s0_thread_id_o);
  sim.compare("s1_addr_load", sim.dut->orig_s1_addr_load_o, sim.dut->new_s1_addr_load_o);
  sim.compare("ba_alloc", sim.dut->orig_ba_alloc_o, sim.dut->new_ba_alloc_o);
  sim.compare("ba_write_needed", sim.dut->orig_ba_write_needed_o, sim.dut->new_ba_write_needed_o);
  sim.compare("s3_ba_write_addr", sim.dut->orig_s3_ba_write_addr_o, sim.dut->new_s3_ba_write_addr_o);
  sim.compare("wait_l2_to_ba_resp", sim.dut->orig_wait_l2_to_ba_resp_o,
              sim.dut->new_wait_l2_to_ba_resp_o);
  sim.compare("ba_entries_required", sim.dut->orig_ba_entries_required_o,
              sim.dut->new_ba_entries_required_o);
  sim.compare("ba_read_en", sim.dut->orig_ba_read_en_o, sim.dut->new_ba_read_en_o);
  sim.compare("ba_read_en_spec", sim.dut->orig_ba_read_en_spec_o, sim.dut->new_ba_read_en_spec_o);
  sim.compare("ba_read_addr", sim.dut->orig_ba_read_addr_o, sim.dut->new_ba_read_addr_o);
  sim.compare("vpu_send_reg", sim.dut->orig_vpu_send_reg_o, sim.dut->new_vpu_send_reg_o);
  sim.compare("vpu_exec_op", sim.dut->orig_vpu_exec_op_o, sim.dut->new_vpu_exec_op_o);
  sim.compare("vpu_nothing", sim.dut->orig_vpu_nothing_o, sim.dut->new_vpu_nothing_o);
  sim.compare("vm_status_ts", sim.dut->orig_vm_status_ts_o, sim.dut->new_vm_status_ts_o);
  sim.compare("ts_start", sim.dut->orig_ts_start_o, sim.dut->new_ts_start_o);
  sim.compare("ts_op", sim.dut->orig_ts_op_o, sim.dut->new_ts_op_o);
  sim.compare("err_flags", sim.dut->orig_err_flags_o, sim.dut->new_err_flags_o);
  sim.compare("bus_err", sim.dut->orig_bus_err_o, sim.dut->new_bus_err_o);
  sim.compare("csr_debug_signals", sim.dut->orig_csr_debug_signals_o, sim.dut->new_csr_debug_signals_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

void run_directed_reduce(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->global_min_id_i = 0x12;
  sim.dut->reduce_start_i = 1;
  sim.dut->ctrl_bits_i = make_reduce_ctrl(0x12345678u);
  sim.dut->thread_id_i = 1;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->re_l2_req_ready_i = 1;
  sim.dut->ts_l2_req_ready_i = 1;
  tick_and_compare(sim, &cov);

  sim.dut->reduce_start_i = 0;
  for (int cycle = 0; cycle < 12; ++cycle) {
    sim.dut->l2_resp_valid_i = (cycle == 4);
    sim.dut->l2_resp_opcode_i = cycle & 0x3;
    sim.dut->l2_resp_id_i = 0x10 + cycle;
    sim.dut->l2_resp_data_i[0] = 0x1111000000000000ULL ^ cycle;
    sim.dut->l2_resp_data_i[1] = 0x2222000000000000ULL ^ cycle;
    sim.dut->l2_resp_data_i[2] = 0x3333000000000000ULL ^ cycle;
    sim.dut->l2_resp_data_i[3] = 0x4444000000000000ULL ^ cycle;
    sim.dut->vpu_reduce_wait_i = (cycle & 1);
    sim.dut->mh_going_to_uc_store_i = (cycle == 6);
    tick_and_compare(sim, &cov);
  }
}

void run_directed_tensorstore(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->global_min_id_i = 0x03;
  sim.dut->tensorstore_start_i = 1;
  sim.dut->tensorstore_stride_i = 0x80;
  sim.dut->ctrl_bits_i = set_tensorstore_scp(make_reduce_ctrl(0x89ABCDEFu), true);
  sim.dut->thread_id_i = 0;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->ts_l2_req_ready_i = 1;
  sim.dut->re_l2_req_ready_i = 1;
  sim.dut->scp_ctrl_on_i = 1;
  tick_and_compare(sim, &cov);

  sim.dut->tensorstore_start_i = 0;
  for (int cycle = 0; cycle < 16; ++cycle) {
    sim.dut->s2_valid_i = (cycle >= 2) && (cycle <= 8);
    sim.dut->s2_addr_i = 0x1000 + (cycle << 6);
    sim.dut->s2_ba_written_entry_i = cycle & 0x7;
    sim.dut->s1_valid_i = (cycle >= 3);
    sim.dut->ba_read_used_i = (cycle & 1);
    sim.dut->s1_ba_data_i = 0x0101010101010101ULL ^ cycle;
    sim.dut->ba_full_i = (cycle == 5);
    sim.dut->s1_tlb_wait_i = (cycle == 1);
    sim.dut->s1_tlb_fail_i = (cycle == 10);
    tick_and_compare(sim, &cov);
  }
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  Coverage coverage;

  clear_inputs(sim.dut.get());
  sample_coverage(sim.dut.get(), coverage);
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);

  run_directed_reduce(sim, coverage);
  run_directed_tensorstore(sim, coverage);

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu, 0x2468ACE1u};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < (kRandomCycles / 4); ++cycle) {
      sim.dut->global_min_id_i = xorshift32(seed) & 0x3fu;
      sim.dut->chicken_bit_dcache_i = xorshift32(seed) & 1u;
      sim.dut->reduce_start_i = (cycle % 37 == 0) ? 1u : (xorshift32(seed) & 1u);
      sim.dut->tensorstore_start_i = (cycle % 41 == 0) ? 1u : (xorshift32(seed) & 1u);
      sim.dut->tensorstore_stride_i = (uint64_t(xorshift32(seed)) << 12) | (xorshift32(seed) & 0xff0u);
      sim.dut->ctrl_bits_i = set_tensorstore_scp(make_reduce_ctrl(xorshift32(seed)),
                                                 (xorshift32(seed) & 1u) != 0u);
      sim.dut->thread_id_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_valid_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_opcode_i = xorshift32(seed) & 0x3u;
      sim.dut->l2_resp_id_i = xorshift32(seed) & 0xffu;
      for (int i = 0; i < 4; ++i) {
        sim.dut->l2_resp_data_i[i] = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      }
      sim.dut->re_l2_req_ready_i = xorshift32(seed) & 1u;
      sim.dut->ts_l2_req_ready_i = xorshift32(seed) & 1u;
      sim.dut->s0_available_i = xorshift32(seed) & 1u;
      sim.dut->s1_available_i = xorshift32(seed) & 1u;
      sim.dut->s1_tlb_wait_i = xorshift32(seed) & 1u;
      sim.dut->s1_tlb_fail_i = xorshift32(seed) & 1u;
      sim.dut->s2_addr_i = ((uint64_t(xorshift32(seed)) << 12) | (xorshift32(seed) & 0xfffu)) & ((1ULL << 40) - 1);
      sim.dut->ba_full_i = xorshift32(seed) & 1u;
      sim.dut->s2_valid_i = xorshift32(seed) & 1u;
      sim.dut->s2_ba_written_entry_i = xorshift32(seed) & 0x7u;
      sim.dut->s1_valid_i = xorshift32(seed) & 1u;
      sim.dut->ba_read_used_i = xorshift32(seed) & 1u;
      sim.dut->s1_ba_data_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      sim.dut->scp_ctrl_on_i = xorshift32(seed) & 1u;
      sim.dut->vpu_reduce_wait_i = xorshift32(seed) & 1u;
      sim.dut->mh_going_to_uc_store_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  sim.finish();
}

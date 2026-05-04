// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_cache_op_unit_l2 vs minion_dcache_cache_op_unit_l2.

#include "Vcosim_minion_dcache_cache_op_unit_l2_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_cache_op_unit_l2_tb;

namespace {

struct BoolCoverage {
  bool seen0 = false;
  bool seen1 = false;
  void sample(bool v) { if (v) seen1 = true; else seen0 = true; }
  bool full() const { return seen0 && seen1; }
};

struct NonZeroCoverage {
  bool seen_zero = false;
  bool seen_nonzero = false;
  void sample(uint64_t v) { if (v == 0) seen_zero = true; else seen_nonzero = true; }
  bool full() const { return seen_zero && seen_nonzero; }
};

struct Coverage {
  BoolCoverage chicken_bit_dcache, l2_resp_valid, cache_op_valid, cache_op_thread_id;
  BoolCoverage cache_op_use_tmask, cache_op_tmask_ready, cache_op_l2_scp_fill;
  BoolCoverage l2_req_ready, cache_op_valid_byp, cache_op_byp_thread_id, cache_op_byp_use_tmask;
  BoolCoverage cache_op_byp_l2_scp_fill, s0_available, s1_available, s1_tlb_wait, s1_tlb_fail;
  BoolCoverage vm_status_co_mprv, vm_status_co_sum, vm_status_co_mxr, vm_status_co_debug;
  NonZeroCoverage shire_id, cacheop_max, cacheop_reprate, l2_resp_id, l2_resp_opcode;
  NonZeroCoverage cache_op_cmd, cache_op_start_level, cache_op_dest_level, cache_op_count;
  NonZeroCoverage cache_op_addr, cache_op_stride, cache_op_tmask_bits, cache_op_l2_scp_dest;
  NonZeroCoverage cache_op_byp_cmd, cache_op_byp_start_level, cache_op_byp_dest_level;
  NonZeroCoverage cache_op_byp_count, cache_op_byp_addr, cache_op_byp_stride;
  NonZeroCoverage cache_op_byp_tmask_bits, cache_op_byp_l2_scp_dest, s1_tlb_addr;
  NonZeroCoverage vm_status_debug, vm_status_sum, vm_status_mxr, vm_status_prv, vm_status_mpp, vm_status_mprv;
  NonZeroCoverage vm_status_co_prv, vm_status_co_mpp;
};

void clear_inputs(DUT* dut) {
  dut->shire_id_i = 0;
  dut->cacheop_max_i = 0;
  dut->cacheop_reprate_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->l2_resp_valid_i = 0;
  dut->l2_resp_id_i = 0;
  dut->l2_resp_opcode_i = 0;
  dut->cache_op_valid_i = 0;
  dut->cache_op_cmd_i = 0;
  dut->cache_op_start_level_i = 0;
  dut->cache_op_dest_level_i = 0;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0;
  dut->cache_op_stride_i = 0;
  dut->cache_op_thread_id_i = 0;
  dut->cache_op_use_tmask_i = 0;
  dut->cache_op_tmask_ready_i = 0;
  dut->cache_op_tmask_bits_i = 0;
  dut->cache_op_l2_scp_dest_i = 0;
  dut->cache_op_l2_scp_fill_i = 0;
  dut->l2_req_ready_i = 0;
  dut->cache_op_valid_byp_i = 0;
  dut->cache_op_byp_cmd_i = 0;
  dut->cache_op_byp_start_level_i = 0;
  dut->cache_op_byp_dest_level_i = 0;
  dut->cache_op_byp_count_i = 0;
  dut->cache_op_byp_addr_i = 0;
  dut->cache_op_byp_stride_i = 0;
  dut->cache_op_byp_thread_id_i = 0;
  dut->cache_op_byp_use_tmask_i = 0;
  dut->cache_op_byp_tmask_bits_i = 0;
  dut->cache_op_byp_l2_scp_dest_i = 0;
  dut->cache_op_byp_l2_scp_fill_i = 0;
  dut->s0_available_i = 0;
  dut->s1_available_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->s1_tlb_addr_i = 0;
  dut->vm_status_debug_i = 0;
  dut->vm_status_sum_i = 0;
  dut->vm_status_mxr_i = 0;
  dut->vm_status_prv_i = 0;
  dut->vm_status_mpp_i = 0;
  dut->vm_status_mprv_i = 0;
  dut->vm_status_co_prv_i = 0;
  dut->vm_status_co_mpp_i = 0;
  dut->vm_status_co_mprv_i = 0;
  dut->vm_status_co_sum_i = 0;
  dut->vm_status_co_mxr_i = 0;
  dut->vm_status_co_debug_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.chicken_bit_dcache.sample(dut->chicken_bit_dcache_i);
  cov.l2_resp_valid.sample(dut->l2_resp_valid_i);
  cov.cache_op_valid.sample(dut->cache_op_valid_i);
  cov.cache_op_thread_id.sample(dut->cache_op_thread_id_i);
  cov.cache_op_use_tmask.sample(dut->cache_op_use_tmask_i);
  cov.cache_op_tmask_ready.sample(dut->cache_op_tmask_ready_i);
  cov.cache_op_l2_scp_fill.sample(dut->cache_op_l2_scp_fill_i);
  cov.l2_req_ready.sample(dut->l2_req_ready_i);
  cov.cache_op_valid_byp.sample(dut->cache_op_valid_byp_i);
  cov.cache_op_byp_thread_id.sample(dut->cache_op_byp_thread_id_i);
  cov.cache_op_byp_use_tmask.sample(dut->cache_op_byp_use_tmask_i);
  cov.cache_op_byp_l2_scp_fill.sample(dut->cache_op_byp_l2_scp_fill_i);
  cov.s0_available.sample(dut->s0_available_i);
  cov.s1_available.sample(dut->s1_available_i);
  cov.s1_tlb_wait.sample(dut->s1_tlb_wait_i);
  cov.s1_tlb_fail.sample(dut->s1_tlb_fail_i);
  cov.vm_status_co_mprv.sample(dut->vm_status_co_mprv_i);
  cov.vm_status_co_sum.sample(dut->vm_status_co_sum_i);
  cov.vm_status_co_mxr.sample(dut->vm_status_co_mxr_i);
  cov.vm_status_co_debug.sample(dut->vm_status_co_debug_i);
  cov.shire_id.sample(dut->shire_id_i);
  cov.cacheop_max.sample(dut->cacheop_max_i);
  cov.cacheop_reprate.sample(dut->cacheop_reprate_i);
  cov.l2_resp_id.sample(dut->l2_resp_id_i);
  cov.l2_resp_opcode.sample(dut->l2_resp_opcode_i);
  cov.cache_op_cmd.sample(dut->cache_op_cmd_i);
  cov.cache_op_start_level.sample(dut->cache_op_start_level_i);
  cov.cache_op_dest_level.sample(dut->cache_op_dest_level_i);
  cov.cache_op_count.sample(dut->cache_op_count_i);
  cov.cache_op_addr.sample(dut->cache_op_addr_i);
  cov.cache_op_stride.sample(dut->cache_op_stride_i);
  cov.cache_op_tmask_bits.sample(dut->cache_op_tmask_bits_i);
  cov.cache_op_l2_scp_dest.sample(dut->cache_op_l2_scp_dest_i);
  cov.cache_op_byp_cmd.sample(dut->cache_op_byp_cmd_i);
  cov.cache_op_byp_start_level.sample(dut->cache_op_byp_start_level_i);
  cov.cache_op_byp_dest_level.sample(dut->cache_op_byp_dest_level_i);
  cov.cache_op_byp_count.sample(dut->cache_op_byp_count_i);
  cov.cache_op_byp_addr.sample(dut->cache_op_byp_addr_i);
  cov.cache_op_byp_stride.sample(dut->cache_op_byp_stride_i);
  cov.cache_op_byp_tmask_bits.sample(dut->cache_op_byp_tmask_bits_i);
  cov.cache_op_byp_l2_scp_dest.sample(dut->cache_op_byp_l2_scp_dest_i);
  cov.s1_tlb_addr.sample(dut->s1_tlb_addr_i);
  cov.vm_status_debug.sample(dut->vm_status_debug_i);
  cov.vm_status_sum.sample(dut->vm_status_sum_i);
  cov.vm_status_mxr.sample(dut->vm_status_mxr_i);
  cov.vm_status_prv.sample(dut->vm_status_prv_i);
  cov.vm_status_mpp.sample(dut->vm_status_mpp_i);
  cov.vm_status_mprv.sample(dut->vm_status_mprv_i);
  cov.vm_status_co_prv.sample(dut->vm_status_co_prv_i);
  cov.vm_status_co_mpp.sample(dut->vm_status_co_mpp_i);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.chicken_bit_dcache.full(), "coverage: chicken_bit_dcache exercised 0/1");
  sim.check(cov.l2_resp_valid.full(), "coverage: l2_resp_valid exercised 0/1");
  sim.check(cov.cache_op_valid.full(), "coverage: cache_op_valid exercised 0/1");
  sim.check(cov.cache_op_thread_id.full(), "coverage: cache_op_thread_id exercised 0/1");
  sim.check(cov.cache_op_use_tmask.full(), "coverage: cache_op_use_tmask exercised 0/1");
  sim.check(cov.cache_op_tmask_ready.full(), "coverage: cache_op_tmask_ready exercised 0/1");
  sim.check(cov.cache_op_l2_scp_fill.full(), "coverage: cache_op_l2_scp_fill exercised 0/1");
  sim.check(cov.l2_req_ready.full(), "coverage: l2_req_ready exercised 0/1");
  sim.check(cov.cache_op_valid_byp.full(), "coverage: cache_op_valid_byp exercised 0/1");
  sim.check(cov.cache_op_byp_thread_id.full(), "coverage: cache_op_byp_thread_id exercised 0/1");
  sim.check(cov.cache_op_byp_use_tmask.full(), "coverage: cache_op_byp_use_tmask exercised 0/1");
  sim.check(cov.cache_op_byp_l2_scp_fill.full(), "coverage: cache_op_byp_l2_scp_fill exercised 0/1");
  sim.check(cov.s0_available.full(), "coverage: s0_available exercised 0/1");
  sim.check(cov.s1_available.full(), "coverage: s1_available exercised 0/1");
  sim.check(cov.s1_tlb_wait.full(), "coverage: s1_tlb_wait exercised 0/1");
  sim.check(cov.s1_tlb_fail.full(), "coverage: s1_tlb_fail exercised 0/1");
  sim.check(cov.shire_id.full(), "coverage: shire_id exercised zero/nonzero");
  sim.check(cov.cacheop_max.full(), "coverage: cacheop_max exercised zero/nonzero");
  sim.check(cov.cacheop_reprate.full(), "coverage: cacheop_reprate exercised zero/nonzero");
  sim.check(cov.l2_resp_id.full(), "coverage: l2_resp_id exercised zero/nonzero");
  sim.check(cov.l2_resp_opcode.full(), "coverage: l2_resp_opcode exercised zero/nonzero");
  sim.check(cov.cache_op_cmd.full(), "coverage: cache_op_cmd exercised zero/nonzero");
  sim.check(cov.cache_op_start_level.full(), "coverage: cache_op_start_level exercised zero/nonzero");
  sim.check(cov.cache_op_dest_level.full(), "coverage: cache_op_dest_level exercised zero/nonzero");
  sim.check(cov.cache_op_count.full(), "coverage: cache_op_count exercised zero/nonzero");
  sim.check(cov.cache_op_addr.full(), "coverage: cache_op_addr exercised zero/nonzero");
  sim.check(cov.cache_op_stride.full(), "coverage: cache_op_stride exercised zero/nonzero");
  sim.check(cov.cache_op_tmask_bits.full(), "coverage: cache_op_tmask_bits exercised zero/nonzero");
  sim.check(cov.cache_op_l2_scp_dest.full(), "coverage: cache_op_l2_scp_dest exercised zero/nonzero");
  sim.check(cov.cache_op_byp_cmd.full(), "coverage: cache_op_byp_cmd exercised zero/nonzero");
  sim.check(cov.cache_op_byp_start_level.full(), "coverage: cache_op_byp_start_level exercised zero/nonzero");
  sim.check(cov.cache_op_byp_dest_level.full(), "coverage: cache_op_byp_dest_level exercised zero/nonzero");
  sim.check(cov.cache_op_byp_count.full(), "coverage: cache_op_byp_count exercised zero/nonzero");
  sim.check(cov.cache_op_byp_addr.full(), "coverage: cache_op_byp_addr exercised zero/nonzero");
  sim.check(cov.cache_op_byp_stride.full(), "coverage: cache_op_byp_stride exercised zero/nonzero");
  sim.check(cov.cache_op_byp_tmask_bits.full(), "coverage: cache_op_byp_tmask_bits exercised zero/nonzero");
  sim.check(cov.cache_op_byp_l2_scp_dest.full(), "coverage: cache_op_byp_l2_scp_dest exercised zero/nonzero");
  sim.check(cov.s1_tlb_addr.full(), "coverage: s1_tlb_addr exercised zero/nonzero");
  sim.check(cov.vm_status_debug.full(), "coverage: vm_status_debug exercised zero/nonzero");
  sim.check(cov.vm_status_sum.full(), "coverage: vm_status_sum exercised zero/nonzero");
  sim.check(cov.vm_status_mxr.full(), "coverage: vm_status_mxr exercised zero/nonzero");
  sim.check(cov.vm_status_prv.full(), "coverage: vm_status_prv exercised zero/nonzero");
  sim.check(cov.vm_status_mpp.full(), "coverage: vm_status_mpp exercised zero/nonzero");
  sim.check(cov.vm_status_mprv.full(), "coverage: vm_status_mprv exercised zero/nonzero");
  sim.check(cov.vm_status_co_prv.full(), "coverage: vm_status_co_prv exercised zero/nonzero");
  sim.check(cov.vm_status_co_mpp.full(), "coverage: vm_status_co_mpp exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("busy", sim.dut->orig_busy_o, sim.dut->new_busy_o);
  sim.compare("th_id_complete", sim.dut->orig_th_id_complete_o, sim.dut->new_th_id_complete_o);
  sim.compare("cache_op_ready", sim.dut->orig_cache_op_ready_o, sim.dut->new_cache_op_ready_o);
  sim.compare("l2_req_valid", sim.dut->orig_l2_req_valid_o, sim.dut->new_l2_req_valid_o);
  sim.compare("l2_req_id", sim.dut->orig_l2_req_id_o, sim.dut->new_l2_req_id_o);
  sim.compare("l2_req_opcode", sim.dut->orig_l2_req_opcode_o, sim.dut->new_l2_req_opcode_o);
  sim.compare("l2_req_subopcode", sim.dut->orig_l2_req_subopcode_o, sim.dut->new_l2_req_subopcode_o);
  sim.compare("l2_req_address", sim.dut->orig_l2_req_address_o, sim.dut->new_l2_req_address_o);
  sim.compare("l2_req_data0", sim.dut->orig_l2_req_data0_o, sim.dut->new_l2_req_data0_o);
  sim.compare("l2_req_data1", sim.dut->orig_l2_req_data1_o, sim.dut->new_l2_req_data1_o);
  sim.compare("l2_req_data2", sim.dut->orig_l2_req_data2_o, sim.dut->new_l2_req_data2_o);
  sim.compare("l2_req_data3", sim.dut->orig_l2_req_data3_o, sim.dut->new_l2_req_data3_o);
  sim.compare("l2_req_size", sim.dut->orig_l2_req_size_o, sim.dut->new_l2_req_size_o);
  sim.compare("l2_req_qwen", sim.dut->orig_l2_req_qwen_o, sim.dut->new_l2_req_qwen_o);
  sim.compare("cache_op_ready_byp", sim.dut->orig_cache_op_ready_byp_o, sim.dut->new_cache_op_ready_byp_o);
  sim.compare("s0_addr_load", sim.dut->orig_s0_addr_load_o, sim.dut->new_s0_addr_load_o);
  sim.compare("s0_addr", sim.dut->orig_s0_addr_o, sim.dut->new_s0_addr_o);
  sim.compare("s0_addr_msb_err", sim.dut->orig_s0_addr_msb_err_o, sim.dut->new_s0_addr_msb_err_o);
  sim.compare("s0_thread_id", sim.dut->orig_s0_thread_id_o, sim.dut->new_s0_thread_id_o);
  sim.compare("s1_addr_load", sim.dut->orig_s1_addr_load_o, sim.dut->new_s1_addr_load_o);
  sim.compare("vm_status_l2_prv", sim.dut->orig_vm_status_l2_prv_o, sim.dut->new_vm_status_l2_prv_o);
  sim.compare("vm_status_l2_mpp", sim.dut->orig_vm_status_l2_mpp_o, sim.dut->new_vm_status_l2_mpp_o);
  sim.compare("vm_status_l2_mprv", sim.dut->orig_vm_status_l2_mprv_o, sim.dut->new_vm_status_l2_mprv_o);
  sim.compare("vm_status_l2_sum", sim.dut->orig_vm_status_l2_sum_o, sim.dut->new_vm_status_l2_sum_o);
  sim.compare("vm_status_l2_mxr", sim.dut->orig_vm_status_l2_mxr_o, sim.dut->new_vm_status_l2_mxr_o);
  sim.compare("vm_status_l2_debug", sim.dut->orig_vm_status_l2_debug_o, sim.dut->new_vm_status_l2_debug_o);
  sim.compare("err_flags", sim.dut->orig_err_flags_o, sim.dut->new_err_flags_o);
  sim.compare("bus_err", sim.dut->orig_bus_err_o, sim.dut->new_bus_err_o);
  sim.compare("csr_debug_signals", sim.dut->orig_csr_debug_signals_o, sim.dut->new_csr_debug_signals_o);
  sim.compare("sm_match_chunk0", sim.dut->orig_sm_match_chunk0_o, sim.dut->new_sm_match_chunk0_o);
  sim.compare("sm_filter_chunk0", sim.dut->orig_sm_filter_chunk0_o, sim.dut->new_sm_filter_chunk0_o);
  sim.compare("sm_filter_chunk1", sim.dut->orig_sm_filter_chunk1_o, sim.dut->new_sm_filter_chunk1_o);
  sim.compare("sm_filter_chunk2", sim.dut->orig_sm_filter_chunk2_o, sim.dut->new_sm_filter_chunk2_o);
  sim.compare("sm_filter_chunk3", sim.dut->orig_sm_filter_chunk3_o, sim.dut->new_sm_filter_chunk3_o);
  sim.compare("sm_data_chunk0", sim.dut->orig_sm_data_chunk0_o, sim.dut->new_sm_data_chunk0_o);
  sim.compare("sm_data_chunk1", sim.dut->orig_sm_data_chunk1_o, sim.dut->new_sm_data_chunk1_o);
  sim.compare("sm_data_chunk2", sim.dut->orig_sm_data_chunk2_o, sim.dut->new_sm_data_chunk2_o);
  sim.compare("sm_data_chunk3", sim.dut->orig_sm_data_chunk3_o, sim.dut->new_sm_data_chunk3_o);
  sim.compare("sm_data_chunk4", sim.dut->orig_sm_data_chunk4_o, sim.dut->new_sm_data_chunk4_o);
  sim.compare("sm_data_chunk5", sim.dut->orig_sm_data_chunk5_o, sim.dut->new_sm_data_chunk5_o);
  sim.compare("sm_data_chunk6", sim.dut->orig_sm_data_chunk6_o, sim.dut->new_sm_data_chunk6_o);
  sim.compare("sm_data_chunk7", sim.dut->orig_sm_data_chunk7_o, sim.dut->new_sm_data_chunk7_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void drive_prefetch_req(DUT* dut) {
  dut->cache_op_cmd_i = 4;
  dut->cache_op_start_level_i = 0;
  dut->cache_op_dest_level_i = 1;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0x12340;
  dut->cache_op_stride_i = 0x40;
  dut->cache_op_thread_id_i = 1;
}

void run_directed(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  drive_prefetch_req(sim.dut.get());
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 0;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->s1_tlb_addr_i = 0x15555;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->l2_req_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->l2_req_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 14;
  sim.dut->l2_resp_opcode_i = 3;
  tick_and_compare(sim, &cov);
  sim.dut->l2_resp_valid_i = 0;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  sim.dut->cache_op_valid_byp_i = 1;
  sim.dut->cache_op_byp_cmd_i = 4;
  sim.dut->cache_op_byp_start_level_i = 1;
  sim.dut->cache_op_byp_dest_level_i = 2;
  sim.dut->cache_op_byp_addr_i = 0x8000;
  sim.dut->cache_op_byp_stride_i = 1;
  sim.dut->cache_op_byp_thread_id_i = 0;
  sim.dut->vm_status_co_prv_i = 3;
  sim.dut->vm_status_co_mpp_i = 1;
  sim.dut->vm_status_co_mprv_i = 1;
  sim.dut->vm_status_co_sum_i = 1;
  sim.dut->vm_status_co_mxr_i = 1;
  sim.dut->vm_status_co_debug_i = 1;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  sim.dut->cache_op_valid_i = 1;
  sim.dut->cache_op_cmd_i = 4;
  sim.dut->cache_op_start_level_i = 0;
  sim.dut->cache_op_dest_level_i = 3;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
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

  run_directed(sim, coverage);

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu, 0x2468ACE1u};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < 1024; ++cycle) {
      sim.dut->shire_id_i = xorshift32(seed) & 0xFFu;
      sim.dut->cacheop_max_i = xorshift32(seed) & 0xFu;
      sim.dut->cacheop_reprate_i = xorshift32(seed) & 0xFu;
      sim.dut->chicken_bit_dcache_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_valid_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_id_i = xorshift32(seed) & 0xFFu;
      sim.dut->l2_resp_opcode_i = xorshift32(seed) & 0x3u;
      sim.dut->cache_op_valid_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_cmd_i = xorshift32(seed) & 0xFu;
      sim.dut->cache_op_start_level_i = xorshift32(seed) & 0x3u;
      sim.dut->cache_op_dest_level_i = xorshift32(seed) & 0x3u;
      sim.dut->cache_op_count_i = xorshift32(seed) & 0xFu;
      sim.dut->cache_op_addr_i = ((uint64_t(xorshift32(seed)) << 8) ^ xorshift32(seed)) & ((1ULL << 48) - 1);
      sim.dut->cache_op_stride_i = ((uint64_t(xorshift32(seed)) << 8) ^ xorshift32(seed)) & ((1ULL << 48) - 1);
      sim.dut->cache_op_thread_id_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_use_tmask_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_tmask_ready_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_tmask_bits_i = xorshift32(seed) & 0xFFFFu;
      sim.dut->cache_op_l2_scp_dest_i = xorshift32(seed) & ((1u << 17) - 1);
      sim.dut->cache_op_l2_scp_fill_i = xorshift32(seed) & 1u;
      sim.dut->l2_req_ready_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_valid_byp_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_byp_cmd_i = xorshift32(seed) & 0xFu;
      sim.dut->cache_op_byp_start_level_i = xorshift32(seed) & 0x3u;
      sim.dut->cache_op_byp_dest_level_i = xorshift32(seed) & 0x3u;
      sim.dut->cache_op_byp_count_i = xorshift32(seed) & 0xFu;
      sim.dut->cache_op_byp_addr_i = ((uint64_t(xorshift32(seed)) << 8) ^ xorshift32(seed)) & ((1ULL << 48) - 1);
      sim.dut->cache_op_byp_stride_i = ((uint64_t(xorshift32(seed)) << 8) ^ xorshift32(seed)) & ((1ULL << 48) - 1);
      sim.dut->cache_op_byp_thread_id_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_byp_use_tmask_i = xorshift32(seed) & 1u;
      sim.dut->cache_op_byp_tmask_bits_i = xorshift32(seed) & 0xFFFFu;
      sim.dut->cache_op_byp_l2_scp_dest_i = xorshift32(seed) & ((1u << 17) - 1);
      sim.dut->cache_op_byp_l2_scp_fill_i = xorshift32(seed) & 1u;
      sim.dut->s0_available_i = xorshift32(seed) & 1u;
      sim.dut->s1_available_i = xorshift32(seed) & 1u;
      sim.dut->s1_tlb_wait_i = xorshift32(seed) & 1u;
      sim.dut->s1_tlb_fail_i = xorshift32(seed) & 1u;
      sim.dut->s1_tlb_addr_i = xorshift32(seed) & ((1u << 28) - 1);
      sim.dut->vm_status_debug_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_sum_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_mxr_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_prv_i = xorshift32(seed) & 0xFu;
      sim.dut->vm_status_mpp_i = xorshift32(seed) & 0xFu;
      sim.dut->vm_status_mprv_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_co_prv_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_co_mpp_i = xorshift32(seed) & 0x3u;
      sim.dut->vm_status_co_mprv_i = xorshift32(seed) & 1u;
      sim.dut->vm_status_co_sum_i = xorshift32(seed) & 1u;
      sim.dut->vm_status_co_mxr_i = xorshift32(seed) & 1u;
      sim.dut->vm_status_co_debug_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

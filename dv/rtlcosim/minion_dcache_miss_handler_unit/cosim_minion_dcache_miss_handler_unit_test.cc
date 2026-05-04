// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_miss_handler_unit vs minion_dcache_miss_handler_unit.

#include "Vcosim_minion_dcache_miss_handler_unit_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_miss_handler_unit_tb;

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
  BoolCoverage req_valid, req_tag_match, req_cacheable, req_thread_id, req_mem_g;
  BoolCoverage req_is_amo, req_is_msg, req_is_bypass, inflight_reduce;
  BoolCoverage l2_evict_ready, l2_miss_ready, l2_resp_valid, l2_resp_slot_avail;
  BoolCoverage meta_write_ready, writeback_ready, writeback_done;
  NonZeroCoverage neigh_min_id, req_addr, req_set, req_cmd, req_typ, req_buf_id, req_way_en;
  NonZeroCoverage req_old_meta_state, req_old_meta_tag, l2_resp_id, l2_resp_opcode;
};

void clear_inputs(DUT* dut) {
  dut->neigh_min_id_i = 0;
  dut->req_valid_i = 0;
  dut->req_addr_i = 0;
  dut->req_set_i = 0;
  dut->req_cmd_i = 0;
  dut->req_typ_i = 0;
  dut->req_buf_id_i = 0;
  dut->req_way_en_i = 0;
  dut->req_tag_match_i = 0;
  dut->req_old_meta_state_i = 0;
  dut->req_old_meta_tag_i = 0;
  dut->req_cacheable_i = 0;
  dut->req_thread_id_i = 0;
  dut->req_mem_g_i = 0;
  dut->req_is_amo_i = 0;
  dut->req_is_msg_i = 0;
  dut->req_is_bypass_i = 0;
  dut->inflight_reduce_i = 0;
  dut->l2_evict_ready_i = 0;
  dut->l2_miss_ready_i = 0;
  dut->l2_resp_valid_i = 0;
  dut->l2_resp_id_i = 0;
  dut->l2_resp_opcode_i = 0;
  dut->l2_resp_slot_avail_i = 0;
  dut->meta_write_ready_i = 0;
  dut->writeback_ready_i = 0;
  dut->writeback_done_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.req_valid.sample(dut->req_valid_i);
  cov.req_tag_match.sample(dut->req_tag_match_i);
  cov.req_cacheable.sample(dut->req_cacheable_i);
  cov.req_thread_id.sample(dut->req_thread_id_i);
  cov.req_mem_g.sample(dut->req_mem_g_i);
  cov.req_is_amo.sample(dut->req_is_amo_i);
  cov.req_is_msg.sample(dut->req_is_msg_i);
  cov.req_is_bypass.sample(dut->req_is_bypass_i);
  cov.inflight_reduce.sample(dut->inflight_reduce_i);
  cov.l2_evict_ready.sample(dut->l2_evict_ready_i);
  cov.l2_miss_ready.sample(dut->l2_miss_ready_i);
  cov.l2_resp_valid.sample(dut->l2_resp_valid_i);
  cov.l2_resp_slot_avail.sample(dut->l2_resp_slot_avail_i);
  cov.meta_write_ready.sample(dut->meta_write_ready_i);
  cov.writeback_ready.sample(dut->writeback_ready_i);
  cov.writeback_done.sample(dut->writeback_done_i);
  cov.neigh_min_id.sample(dut->neigh_min_id_i);
  cov.req_addr.sample(dut->req_addr_i);
  cov.req_set.sample(dut->req_set_i);
  cov.req_cmd.sample(dut->req_cmd_i);
  cov.req_typ.sample(dut->req_typ_i);
  cov.req_buf_id.sample(dut->req_buf_id_i);
  cov.req_way_en.sample(dut->req_way_en_i);
  cov.req_old_meta_state.sample(dut->req_old_meta_state_i);
  cov.req_old_meta_tag.sample(dut->req_old_meta_tag_i);
  cov.l2_resp_id.sample(dut->l2_resp_id_i);
  cov.l2_resp_opcode.sample(dut->l2_resp_opcode_i);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.req_valid.full(), "coverage: req_valid exercised 0/1");
  sim.check(cov.req_tag_match.full(), "coverage: req_tag_match exercised 0/1");
  sim.check(cov.req_cacheable.full(), "coverage: req_cacheable exercised 0/1");
  sim.check(cov.req_thread_id.full(), "coverage: req_thread_id exercised 0/1");
  sim.check(cov.req_mem_g.full(), "coverage: req_mem_g exercised 0/1");
  sim.check(cov.req_is_amo.full(), "coverage: req_is_amo exercised 0/1");
  sim.check(cov.req_is_msg.full(), "coverage: req_is_msg exercised 0/1");
  sim.check(cov.req_is_bypass.full(), "coverage: req_is_bypass exercised 0/1");
  sim.check(cov.inflight_reduce.full(), "coverage: inflight_reduce exercised 0/1");
  sim.check(cov.l2_evict_ready.full(), "coverage: l2_evict_ready exercised 0/1");
  sim.check(cov.l2_miss_ready.full(), "coverage: l2_miss_ready exercised 0/1");
  sim.check(cov.l2_resp_valid.full(), "coverage: l2_resp_valid exercised 0/1");
  sim.check(cov.l2_resp_slot_avail.full(), "coverage: l2_resp_slot_avail exercised 0/1");
  sim.check(cov.meta_write_ready.full(), "coverage: meta_write_ready exercised 0/1");
  sim.check(cov.writeback_ready.full(), "coverage: writeback_ready exercised 0/1");
  sim.check(cov.writeback_done.full(), "coverage: writeback_done exercised 0/1");
  sim.check(cov.neigh_min_id.full(), "coverage: neigh_min_id exercised zero/nonzero");
  sim.check(cov.req_addr.full(), "coverage: req_addr exercised zero/nonzero");
  sim.check(cov.req_set.full(), "coverage: req_set exercised zero/nonzero");
  sim.check(cov.req_cmd.full(), "coverage: req_cmd exercised zero/nonzero");
  sim.check(cov.req_typ.full(), "coverage: req_typ exercised zero/nonzero");
  sim.check(cov.req_buf_id.full(), "coverage: req_buf_id exercised zero/nonzero");
  sim.check(cov.req_way_en.full(), "coverage: req_way_en exercised zero/nonzero");
  sim.check(cov.req_old_meta_state.full(), "coverage: req_old_meta_state exercised zero/nonzero");
  sim.check(cov.req_old_meta_tag.full(), "coverage: req_old_meta_tag exercised zero/nonzero");
  sim.check(cov.l2_resp_id.full(), "coverage: l2_resp_id exercised zero/nonzero");
  sim.check(cov.l2_resp_opcode.full(), "coverage: l2_resp_opcode exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("req_ready", sim.dut->orig_req_ready_o, sim.dut->new_req_ready_o);
  sim.compare("req_assigned", sim.dut->orig_req_assigned_o, sim.dut->new_req_assigned_o);
  sim.compare("req_accepted", sim.dut->orig_req_accepted_o, sim.dut->new_req_accepted_o);
  sim.compare("can_accept_uc", sim.dut->orig_can_accept_uc_o, sim.dut->new_can_accept_uc_o);
  sim.compare("wait_l2_to_ba_resp", sim.dut->orig_wait_l2_to_ba_resp_o, sim.dut->new_wait_l2_to_ba_resp_o);
  sim.compare("going_to_uc_store", sim.dut->orig_going_to_uc_store_o, sim.dut->new_going_to_uc_store_o);
  sim.compare("l2_evict_valid", sim.dut->orig_l2_evict_valid_o, sim.dut->new_l2_evict_valid_o);
  sim.compare("l2_evict_id", sim.dut->orig_l2_evict_id_o, sim.dut->new_l2_evict_id_o);
  sim.compare("l2_evict_opcode", sim.dut->orig_l2_evict_opcode_o, sim.dut->new_l2_evict_opcode_o);
  sim.compare("l2_evict_subopcode", sim.dut->orig_l2_evict_subopcode_o, sim.dut->new_l2_evict_subopcode_o);
  sim.compare("l2_evict_addr", sim.dut->orig_l2_evict_addr_o, sim.dut->new_l2_evict_addr_o);
  sim.compare("l2_evict_data0", sim.dut->orig_l2_evict_data0_o, sim.dut->new_l2_evict_data0_o);
  sim.compare("l2_evict_data1", sim.dut->orig_l2_evict_data1_o, sim.dut->new_l2_evict_data1_o);
  sim.compare("l2_evict_data2", sim.dut->orig_l2_evict_data2_o, sim.dut->new_l2_evict_data2_o);
  sim.compare("l2_evict_data3", sim.dut->orig_l2_evict_data3_o, sim.dut->new_l2_evict_data3_o);
  sim.compare("l2_evict_size", sim.dut->orig_l2_evict_size_o, sim.dut->new_l2_evict_size_o);
  sim.compare("l2_evict_qwen", sim.dut->orig_l2_evict_qwen_o, sim.dut->new_l2_evict_qwen_o);
  sim.compare("l2_miss_valid", sim.dut->orig_l2_miss_valid_o, sim.dut->new_l2_miss_valid_o);
  sim.compare("l2_miss_id", sim.dut->orig_l2_miss_id_o, sim.dut->new_l2_miss_id_o);
  sim.compare("l2_miss_opcode", sim.dut->orig_l2_miss_opcode_o, sim.dut->new_l2_miss_opcode_o);
  sim.compare("l2_miss_subopcode", sim.dut->orig_l2_miss_subopcode_o, sim.dut->new_l2_miss_subopcode_o);
  sim.compare("l2_miss_addr", sim.dut->orig_l2_miss_addr_o, sim.dut->new_l2_miss_addr_o);
  sim.compare("l2_miss_data0", sim.dut->orig_l2_miss_data0_o, sim.dut->new_l2_miss_data0_o);
  sim.compare("l2_miss_data1", sim.dut->orig_l2_miss_data1_o, sim.dut->new_l2_miss_data1_o);
  sim.compare("l2_miss_data2", sim.dut->orig_l2_miss_data2_o, sim.dut->new_l2_miss_data2_o);
  sim.compare("l2_miss_data3", sim.dut->orig_l2_miss_data3_o, sim.dut->new_l2_miss_data3_o);
  sim.compare("l2_miss_size", sim.dut->orig_l2_miss_size_o, sim.dut->new_l2_miss_size_o);
  sim.compare("l2_miss_qwen", sim.dut->orig_l2_miss_qwen_o, sim.dut->new_l2_miss_qwen_o);
  sim.compare("l2_resp_stall", sim.dut->orig_l2_resp_stall_o, sim.dut->new_l2_resp_stall_o);
  sim.compare("refill_way", sim.dut->orig_refill_way_o, sim.dut->new_refill_way_o);
  sim.compare("refill_addr", sim.dut->orig_refill_addr_o, sim.dut->new_refill_addr_o);
  sim.compare("meta_write_valid_next", sim.dut->orig_meta_write_valid_next_o, sim.dut->new_meta_write_valid_next_o);
  sim.compare("meta_write_valid", sim.dut->orig_meta_write_valid_o, sim.dut->new_meta_write_valid_o);
  sim.compare("meta_write_clear_slock", sim.dut->orig_meta_write_clear_slock_o, sim.dut->new_meta_write_clear_slock_o);
  sim.compare("meta_write_set", sim.dut->orig_meta_write_set_o, sim.dut->new_meta_write_set_o);
  sim.compare("meta_write_way_en", sim.dut->orig_meta_write_way_en_o, sim.dut->new_meta_write_way_en_o);
  sim.compare("meta_write_state", sim.dut->orig_meta_write_state_o, sim.dut->new_meta_write_state_o);
  sim.compare("meta_write_tag", sim.dut->orig_meta_write_tag_o, sim.dut->new_meta_write_tag_o);
  sim.compare("writeback_valid", sim.dut->orig_writeback_valid_o, sim.dut->new_writeback_valid_o);
  sim.compare("writeback_tag", sim.dut->orig_writeback_tag_o, sim.dut->new_writeback_tag_o);
  sim.compare("writeback_set", sim.dut->orig_writeback_set_o, sim.dut->new_writeback_set_o);
  sim.compare("writeback_way_en", sim.dut->orig_writeback_way_en_o, sim.dut->new_writeback_way_en_o);
  sim.compare("writeback_is_tbox", sim.dut->orig_writeback_is_tbox_o, sim.dut->new_writeback_is_tbox_o);
  sim.compare("writeback_mem_idx", sim.dut->orig_writeback_mem_idx_o, sim.dut->new_writeback_mem_idx_o);
  sim.compare("writeback_addr_low", sim.dut->orig_writeback_addr_low_o, sim.dut->new_writeback_addr_low_o);
  sim.compare("force_ba_rptr", sim.dut->orig_force_ba_rptr_o, sim.dut->new_force_ba_rptr_o);
  sim.compare("force_ba_rptr_qual", sim.dut->orig_force_ba_rptr_qual_o, sim.dut->new_force_ba_rptr_qual_o);
  sim.compare("ba_rptr", sim.dut->orig_ba_rptr_o, sim.dut->new_ba_rptr_o);
  sim.compare("ba_is_amo", sim.dut->orig_ba_is_amo_o, sim.dut->new_ba_is_amo_o);
  sim.compare("ba_wptr", sim.dut->orig_ba_wptr_o, sim.dut->new_ba_wptr_o);
  sim.compare("idle", sim.dut->orig_idle_o, sim.dut->new_idle_o);
  sim.compare("probe_rdy", sim.dut->orig_probe_rdy_o, sim.dut->new_probe_rdy_o);
  sim.compare("metaw_rdy", sim.dut->orig_metaw_rdy_o, sim.dut->new_metaw_rdy_o);
  sim.compare("fence_rdy", sim.dut->orig_fence_rdy_o, sim.dut->new_fence_rdy_o);
  sim.compare("handler_ready", sim.dut->orig_handler_ready_o, sim.dut->new_handler_ready_o);
  sim.compare("handler_fill_bus_error", sim.dut->orig_handler_fill_bus_error_o, sim.dut->new_handler_fill_bus_error_o);
  sim.compare("handler_uc_load_bus_error", sim.dut->orig_handler_uc_load_bus_error_o, sim.dut->new_handler_uc_load_bus_error_o);
  sim.compare("match_miss_line", sim.dut->orig_match_miss_line_o, sim.dut->new_match_miss_line_o);
  sim.compare("bus_err", sim.dut->orig_bus_err_o, sim.dut->new_bus_err_o);
  sim.compare("bus_err_addr", sim.dut->orig_bus_err_addr_o, sim.dut->new_bus_err_addr_o);
  sim.compare("csr_debug_signals", sim.dut->orig_csr_debug_signals_o, sim.dut->new_csr_debug_signals_o);
  sim.compare("sm_match_h0", sim.dut->orig_sm_match_h0_o, sim.dut->new_sm_match_h0_o);
  sim.compare("sm_match_h1", sim.dut->orig_sm_match_h1_o, sim.dut->new_sm_match_h1_o);
  sim.compare("sm_filter_h0_0", sim.dut->orig_sm_filter_h0_chunk0_o, sim.dut->new_sm_filter_h0_chunk0_o);
  sim.compare("sm_filter_h0_1", sim.dut->orig_sm_filter_h0_chunk1_o, sim.dut->new_sm_filter_h0_chunk1_o);
  sim.compare("sm_filter_h0_2", sim.dut->orig_sm_filter_h0_chunk2_o, sim.dut->new_sm_filter_h0_chunk2_o);
  sim.compare("sm_filter_h0_3", sim.dut->orig_sm_filter_h0_chunk3_o, sim.dut->new_sm_filter_h0_chunk3_o);
  sim.compare("sm_filter_h1_0", sim.dut->orig_sm_filter_h1_chunk0_o, sim.dut->new_sm_filter_h1_chunk0_o);
  sim.compare("sm_filter_h1_1", sim.dut->orig_sm_filter_h1_chunk1_o, sim.dut->new_sm_filter_h1_chunk1_o);
  sim.compare("sm_filter_h1_2", sim.dut->orig_sm_filter_h1_chunk2_o, sim.dut->new_sm_filter_h1_chunk2_o);
  sim.compare("sm_filter_h1_3", sim.dut->orig_sm_filter_h1_chunk3_o, sim.dut->new_sm_filter_h1_chunk3_o);
  sim.compare("sm_data_h0_0", sim.dut->orig_sm_data_h0_chunk0_o, sim.dut->new_sm_data_h0_chunk0_o);
  sim.compare("sm_data_h0_1", sim.dut->orig_sm_data_h0_chunk1_o, sim.dut->new_sm_data_h0_chunk1_o);
  sim.compare("sm_data_h0_2", sim.dut->orig_sm_data_h0_chunk2_o, sim.dut->new_sm_data_h0_chunk2_o);
  sim.compare("sm_data_h0_3", sim.dut->orig_sm_data_h0_chunk3_o, sim.dut->new_sm_data_h0_chunk3_o);
  sim.compare("sm_data_h0_4", sim.dut->orig_sm_data_h0_chunk4_o, sim.dut->new_sm_data_h0_chunk4_o);
  sim.compare("sm_data_h0_5", sim.dut->orig_sm_data_h0_chunk5_o, sim.dut->new_sm_data_h0_chunk5_o);
  sim.compare("sm_data_h0_6", sim.dut->orig_sm_data_h0_chunk6_o, sim.dut->new_sm_data_h0_chunk6_o);
  sim.compare("sm_data_h0_7", sim.dut->orig_sm_data_h0_chunk7_o, sim.dut->new_sm_data_h0_chunk7_o);
  sim.compare("sm_data_h1_0", sim.dut->orig_sm_data_h1_chunk0_o, sim.dut->new_sm_data_h1_chunk0_o);
  sim.compare("sm_data_h1_1", sim.dut->orig_sm_data_h1_chunk1_o, sim.dut->new_sm_data_h1_chunk1_o);
  sim.compare("sm_data_h1_2", sim.dut->orig_sm_data_h1_chunk2_o, sim.dut->new_sm_data_h1_chunk2_o);
  sim.compare("sm_data_h1_3", sim.dut->orig_sm_data_h1_chunk3_o, sim.dut->new_sm_data_h1_chunk3_o);
  sim.compare("sm_data_h1_4", sim.dut->orig_sm_data_h1_chunk4_o, sim.dut->new_sm_data_h1_chunk4_o);
  sim.compare("sm_data_h1_5", sim.dut->orig_sm_data_h1_chunk5_o, sim.dut->new_sm_data_h1_chunk5_o);
  sim.compare("sm_data_h1_6", sim.dut->orig_sm_data_h1_chunk6_o, sim.dut->new_sm_data_h1_chunk6_o);
  sim.compare("sm_data_h1_7", sim.dut->orig_sm_data_h1_chunk7_o, sim.dut->new_sm_data_h1_chunk7_o);
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

void drive_base_req(DUT* dut) {
  dut->req_addr_i = 0x800;
  dut->req_set_i = 4;
  dut->req_cmd_i = 0;
  dut->req_typ_i = 2;
  dut->req_buf_id_i = 3;
  dut->req_way_en_i = 0x2;
  dut->req_old_meta_tag_i = 0x12;
}

void run_directed(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cacheable_i = 1;
  sim.dut->req_old_meta_state_i = 1;
  tick_and_compare(sim, &cov);

  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_addr_i = 0x900;
  sim.dut->req_set_i = 5;
  sim.dut->req_way_en_i = 0x1;
  sim.dut->req_buf_id_i = 1;
  sim.dut->req_thread_id_i = 1;
  tick_and_compare(sim, &cov);

  sim.dut->req_valid_i = 0;
  sim.dut->l2_miss_ready_i = 1;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);

  sim.dut->l2_miss_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 2;
  sim.dut->l2_resp_opcode_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->l2_resp_id_i = 3;
  sim.dut->l2_resp_slot_avail_i = 1;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cmd_i = 1;
  sim.dut->req_cacheable_i = 0;
  sim.dut->l2_evict_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->req_valid_i = 0;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->l2_evict_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 10;
  sim.dut->l2_resp_opcode_i = 0;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cacheable_i = 0;
  sim.dut->req_thread_id_i = 1;
  sim.dut->req_buf_id_i = 5;
  sim.dut->l2_miss_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->req_valid_i = 0;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->l2_miss_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 9;
  sim.dut->l2_resp_opcode_i = 3;
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
      sim.dut->neigh_min_id_i = xorshift32(seed) & 0x7u;
      sim.dut->req_valid_i = xorshift32(seed) & 1u;
      sim.dut->req_addr_i =
          ((uint64_t(xorshift32(seed)) << 8) ^ xorshift32(seed)) & ((1ULL << 40) - 1);
      sim.dut->req_set_i = xorshift32(seed) & 0xFu;
      sim.dut->req_cmd_i = xorshift32(seed) & 0x1Fu;
      sim.dut->req_typ_i = xorshift32(seed) & 0xFu;
      sim.dut->req_buf_id_i = xorshift32(seed) & 0x7u;
      sim.dut->req_way_en_i = 1u << (xorshift32(seed) & 0x3u);
      sim.dut->req_tag_match_i = xorshift32(seed) & 1u;
      sim.dut->req_old_meta_state_i = xorshift32(seed) & 0x3u;
      sim.dut->req_old_meta_tag_i =
          ((uint64_t(xorshift32(seed)) << 1) ^ xorshift32(seed)) & ((1ULL << 33) - 1);
      sim.dut->req_cacheable_i = xorshift32(seed) & 1u;
      sim.dut->req_thread_id_i = xorshift32(seed) & 1u;
      sim.dut->req_mem_g_i = xorshift32(seed) & 1u;
      sim.dut->req_is_amo_i = xorshift32(seed) & 1u;
      sim.dut->req_is_msg_i = xorshift32(seed) & 1u;
      sim.dut->req_is_bypass_i = xorshift32(seed) & 1u;
      sim.dut->inflight_reduce_i = xorshift32(seed) & 1u;
      sim.dut->l2_evict_ready_i = xorshift32(seed) & 1u;
      sim.dut->l2_miss_ready_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_valid_i = xorshift32(seed) & 1u;
      sim.dut->l2_resp_id_i = xorshift32(seed) & 0xFFu;
      sim.dut->l2_resp_opcode_i = xorshift32(seed) & 0x3u;
      sim.dut->l2_resp_slot_avail_i = xorshift32(seed) & 1u;
      sim.dut->meta_write_ready_i = xorshift32(seed) & 1u;
      sim.dut->writeback_ready_i = xorshift32(seed) & 1u;
      sim.dut->writeback_done_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

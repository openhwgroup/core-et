// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_replay_queue vs minion_dcache_replay_queue.

#include "Vcosim_minion_dcache_replay_queue_tb.h"
#include "cosim_ctrl.h"

#include <cstdlib>
#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_replay_queue_tb;

namespace {

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
  BoolCoverage alloc_req_pre, alloc_req_val, alloc_gsc, push_req, push_req_prev;
  BoolCoverage req_phys, req_misaligned, req_misaligned_2nd, req_dest_fp;
  BoolCoverage req_dest_thread, req_replay, req_cache_op, req_buf_alloc;
  BoolCoverage req_cacheable, req_uc_load_2nd, req_uc_load_ok, req_miss_fill_ok;
  BoolCoverage cacheable, req_conf_vpu_rf_write, req_conf_miss_alloc;
  BoolCoverage misaligned_1st, uc_load_1st, uc_load_ok, miss_fill_ok;
  BoolCoverage rearm, rearm_prev, dealloc, uc_load_fill, uc_load_fill_ok;
  BoolCoverage vpu_rf_in_use, replay_grant;
  NonZeroCoverage req_addr, req_dest_addr, req_cmd, req_typ, req_gsc_cnt, req_ps_mask;
  NonZeroCoverage req_replay_entry, req_replay_age, req_buf_id, req_gsc32_idx, req_vm_sel;
  NonZeroCoverage req_conf_miss_accepted, req_conf_miss_assigned;
  NonZeroCoverage rearm_entry, dealloc_entry, miss_available, miss_fill_error, uc_load_fill_id;
};

void clear_inputs(DUT* dut) {
  dut->alloc_req_pre_i = 0;
  dut->alloc_req_val_i = 0;
  dut->alloc_gsc_i = 0;
  dut->push_req_i = 0;
  dut->push_req_prev_i = 0;
  dut->req_addr_i = 0;
  dut->req_phys_i = 0;
  dut->req_misaligned_i = 0;
  dut->req_misaligned_2nd_i = 0;
  dut->req_dest_fp_i = 0;
  dut->req_dest_addr_i = 0;
  dut->req_dest_thread_i = 0;
  dut->req_cmd_i = 0;
  dut->req_typ_i = 0;
  dut->req_gsc_cnt_i = 0;
  dut->req_ps_mask_i = 0;
  dut->req_replay_i = 0;
  dut->req_replay_entry_i = 0;
  dut->req_replay_age_i = 0;
  dut->req_cache_op_i = 0;
  dut->req_buf_alloc_i = 0;
  dut->req_buf_id_i = 0;
  dut->req_cacheable_i = 0;
  dut->req_uc_load_2nd_i = 0;
  dut->req_uc_load_ok_i = 0;
  dut->req_miss_fill_ok_i = 0;
  dut->req_gsc32_idx_i = 0;
  dut->req_vm_sel_i = 0;
  dut->req_mem_global_i = 0;
  dut->cacheable_i = 0;
  dut->req_conf_vpu_rf_write_i = 0;
  dut->req_conf_miss_alloc_i = 0;
  dut->req_conf_miss_accepted_i = 0;
  dut->req_conf_miss_assigned_i = 0;
  dut->misaligned_1st_i = 0;
  dut->uc_load_1st_i = 0;
  dut->uc_load_ok_i = 0;
  dut->miss_fill_ok_i = 0;
  dut->rearm_i = 0;
  dut->rearm_prev_i = 0;
  dut->rearm_entry_i = 0;
  dut->dealloc_i = 0;
  dut->dealloc_entry_i = 0;
  dut->miss_available_i = 0;
  dut->miss_fill_error_i = 0;
  dut->uc_load_fill_i = 0;
  dut->uc_load_fill_id_i = 0;
  dut->uc_load_fill_ok_i = 0;
  dut->vpu_rf_in_use_i = 0;
  dut->replay_grant_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.alloc_req_pre.sample(dut->alloc_req_pre_i);
  cov.alloc_req_val.sample(dut->alloc_req_val_i);
  cov.alloc_gsc.sample(dut->alloc_gsc_i);
  cov.push_req.sample(dut->push_req_i);
  cov.push_req_prev.sample(dut->push_req_prev_i);
  cov.req_phys.sample(dut->req_phys_i);
  cov.req_misaligned.sample(dut->req_misaligned_i);
  cov.req_misaligned_2nd.sample(dut->req_misaligned_2nd_i);
  cov.req_dest_fp.sample(dut->req_dest_fp_i);
  cov.req_dest_thread.sample(dut->req_dest_thread_i);
  cov.req_replay.sample(dut->req_replay_i);
  cov.req_cache_op.sample(dut->req_cache_op_i);
  cov.req_buf_alloc.sample(dut->req_buf_alloc_i);
  cov.req_cacheable.sample(dut->req_cacheable_i);
  cov.req_uc_load_2nd.sample(dut->req_uc_load_2nd_i);
  cov.req_uc_load_ok.sample(dut->req_uc_load_ok_i);
  cov.req_miss_fill_ok.sample(dut->req_miss_fill_ok_i);
  cov.cacheable.sample(dut->cacheable_i);
  cov.req_conf_vpu_rf_write.sample(dut->req_conf_vpu_rf_write_i);
  cov.req_conf_miss_alloc.sample(dut->req_conf_miss_alloc_i);
  cov.misaligned_1st.sample(dut->misaligned_1st_i);
  cov.uc_load_1st.sample(dut->uc_load_1st_i);
  cov.uc_load_ok.sample(dut->uc_load_ok_i);
  cov.miss_fill_ok.sample(dut->miss_fill_ok_i);
  cov.rearm.sample(dut->rearm_i);
  cov.rearm_prev.sample(dut->rearm_prev_i);
  cov.dealloc.sample(dut->dealloc_i);
  cov.uc_load_fill.sample(dut->uc_load_fill_i);
  cov.uc_load_fill_ok.sample(dut->uc_load_fill_ok_i);
  cov.vpu_rf_in_use.sample(dut->vpu_rf_in_use_i);
  cov.replay_grant.sample(dut->replay_grant_i);
  cov.req_addr.sample(dut->req_addr_i);
  cov.req_dest_addr.sample(dut->req_dest_addr_i);
  cov.req_cmd.sample(dut->req_cmd_i);
  cov.req_typ.sample(dut->req_typ_i);
  cov.req_gsc_cnt.sample(dut->req_gsc_cnt_i);
  cov.req_ps_mask.sample(dut->req_ps_mask_i);
  cov.req_replay_entry.sample(dut->req_replay_entry_i);
  cov.req_replay_age.sample(dut->req_replay_age_i);
  cov.req_buf_id.sample(dut->req_buf_id_i);
  cov.req_gsc32_idx.sample(dut->req_gsc32_idx_i);
  cov.req_vm_sel.sample(dut->req_vm_sel_i);
  cov.req_conf_miss_accepted.sample(dut->req_conf_miss_accepted_i);
  cov.req_conf_miss_assigned.sample(dut->req_conf_miss_assigned_i);
  cov.rearm_entry.sample(dut->rearm_entry_i);
  cov.dealloc_entry.sample(dut->dealloc_entry_i);
  cov.miss_available.sample(dut->miss_available_i);
  cov.miss_fill_error.sample(dut->miss_fill_error_i);
  cov.uc_load_fill_id.sample(dut->uc_load_fill_id_i);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.alloc_req_pre.full(), "coverage: alloc_req_pre exercised 0/1");
  sim.check(cov.alloc_req_val.full(), "coverage: alloc_req_val exercised 0/1");
  sim.check(cov.alloc_gsc.full(), "coverage: alloc_gsc exercised 0/1");
  sim.check(cov.push_req.full(), "coverage: push_req exercised 0/1");
  sim.check(cov.push_req_prev.full(), "coverage: push_req_prev exercised 0/1");
  sim.check(cov.req_phys.full(), "coverage: req_phys exercised 0/1");
  sim.check(cov.req_misaligned.full(), "coverage: req_misaligned exercised 0/1");
  sim.check(cov.req_misaligned_2nd.full(), "coverage: req_misaligned_2nd exercised 0/1");
  sim.check(cov.req_dest_fp.full(), "coverage: req_dest_fp exercised 0/1");
  sim.check(cov.req_dest_thread.full(), "coverage: req_dest_thread exercised 0/1");
  sim.check(cov.req_replay.full(), "coverage: req_replay exercised 0/1");
  sim.check(cov.req_cache_op.full(), "coverage: req_cache_op exercised 0/1");
  sim.check(cov.req_buf_alloc.full(), "coverage: req_buf_alloc exercised 0/1");
  sim.check(cov.req_cacheable.full(), "coverage: req_cacheable exercised 0/1");
  sim.check(cov.req_uc_load_2nd.full(), "coverage: req_uc_load_2nd exercised 0/1");
  sim.check(cov.req_uc_load_ok.full(), "coverage: req_uc_load_ok exercised 0/1");
  sim.check(cov.req_miss_fill_ok.full(), "coverage: req_miss_fill_ok exercised 0/1");
  sim.check(cov.cacheable.full(), "coverage: cacheable exercised 0/1");
  sim.check(cov.req_conf_vpu_rf_write.full(), "coverage: req_conf_vpu_rf_write exercised 0/1");
  sim.check(cov.req_conf_miss_alloc.full(), "coverage: req_conf_miss_alloc exercised 0/1");
  sim.check(cov.misaligned_1st.full(), "coverage: misaligned_1st exercised 0/1");
  sim.check(cov.uc_load_1st.full(), "coverage: uc_load_1st exercised 0/1");
  sim.check(cov.uc_load_ok.full(), "coverage: uc_load_ok exercised 0/1");
  sim.check(cov.miss_fill_ok.full(), "coverage: miss_fill_ok exercised 0/1");
  sim.check(cov.rearm.full(), "coverage: rearm exercised 0/1");
  sim.check(cov.rearm_prev.full(), "coverage: rearm_prev exercised 0/1");
  sim.check(cov.dealloc.full(), "coverage: dealloc exercised 0/1");
  sim.check(cov.uc_load_fill.full(), "coverage: uc_load_fill exercised 0/1");
  sim.check(cov.uc_load_fill_ok.full(), "coverage: uc_load_fill_ok exercised 0/1");
  sim.check(cov.vpu_rf_in_use.full(), "coverage: vpu_rf_in_use exercised 0/1");
  sim.check(cov.replay_grant.full(), "coverage: replay_grant exercised 0/1");
  sim.check(cov.req_addr.full(), "coverage: req_addr exercised zero/nonzero");
  sim.check(cov.req_dest_addr.full(), "coverage: req_dest_addr exercised zero/nonzero");
  sim.check(cov.req_cmd.full(), "coverage: req_cmd exercised zero/nonzero");
  sim.check(cov.req_typ.full(), "coverage: req_typ exercised zero/nonzero");
  sim.check(cov.req_gsc_cnt.full(), "coverage: req_gsc_cnt exercised zero/nonzero");
  sim.check(cov.req_ps_mask.full(), "coverage: req_ps_mask exercised zero/nonzero");
  sim.check(cov.req_replay_entry.full(), "coverage: req_replay_entry exercised zero/nonzero");
  sim.check(cov.req_replay_age.full(), "coverage: req_replay_age exercised zero/nonzero");
  sim.check(cov.req_buf_id.full(), "coverage: req_buf_id exercised zero/nonzero");
  sim.check(cov.req_gsc32_idx.full(), "coverage: req_gsc32_idx exercised zero/nonzero");
  sim.check(cov.req_vm_sel.full(), "coverage: req_vm_sel exercised zero/nonzero");
  sim.check(cov.req_conf_miss_accepted.full(), "coverage: req_conf_miss_accepted exercised zero/nonzero");
  sim.check(cov.req_conf_miss_assigned.full(), "coverage: req_conf_miss_assigned exercised zero/nonzero");
  sim.check(cov.rearm_entry.full(), "coverage: rearm_entry exercised zero/nonzero");
  sim.check(cov.dealloc_entry.full(), "coverage: dealloc_entry exercised zero/nonzero");
  sim.check(cov.miss_available.full(), "coverage: miss_available exercised zero/nonzero");
  sim.check(cov.miss_fill_error.full(), "coverage: miss_fill_error exercised zero/nonzero");
  sim.check(cov.uc_load_fill_id.full(), "coverage: uc_load_fill_id exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("full", sim.dut->orig_full_o, sim.dut->new_full_o);
  sim.compare("half_full", sim.dut->orig_half_full_o, sim.dut->new_half_full_o);
  sim.compare("empty", sim.dut->orig_empty_o, sim.dut->new_empty_o);
  sim.compare("rq_conflict", sim.dut->orig_rq_conflict_o, sim.dut->new_rq_conflict_o);
  sim.compare("rq_conflict_to_co", sim.dut->orig_rq_conflict_to_co_o, sim.dut->new_rq_conflict_to_co_o);
  sim.compare("replay_bid", sim.dut->orig_replay_bid_o, sim.dut->new_replay_bid_o);
  sim.compare("replay_valid", sim.dut->orig_replay_valid_o, sim.dut->new_replay_valid_o);
  sim.compare("replay_entry", sim.dut->orig_replay_entry_o, sim.dut->new_replay_entry_o);
  sim.compare("replay_age", sim.dut->orig_replay_age_o, sim.dut->new_replay_age_o);
  sim.compare("replay_uc_ok", sim.dut->orig_replay_uc_ok_o, sim.dut->new_replay_uc_ok_o);
  sim.compare("replay_miss_ok", sim.dut->orig_replay_miss_ok_o, sim.dut->new_replay_miss_ok_o);
  for (int i = 0; i < 5; ++i) {
    sim.compare("replay_req[" + std::to_string(i) + "]",
                sim.dut->orig_replay_req_o[i], sim.dut->new_replay_req_o[i]);
  }
  sim.compare("scoreboard", sim.dut->orig_scoreboard_o, sim.dut->new_scoreboard_o);
  sim.compare("fp_sb_dealloc", sim.dut->orig_fp_sb_dealloc_o, sim.dut->new_fp_sb_dealloc_o);
  sim.compare("int_sb_dealloc", sim.dut->orig_int_sb_dealloc_o, sim.dut->new_int_sb_dealloc_o);
}

void step_half(CosimCtrl<DUT>& sim, int clk_level, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->clk_i = clk_level;
  sim.dut->eval();
  compare_outputs(sim);
  sim.sim_time++;
  if (sim.sim_time >= sim.max_time) {
    printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
    std::exit(2);
  }
}

uint32_t xorshift32(uint32_t& s);

void mutate_push_payload_midcycle(DUT* dut) {
  dut->req_dest_fp_i ^= 1;
  dut->req_dest_addr_i ^= 0x1Fu;
  dut->req_dest_thread_i ^= 1;
  dut->req_cmd_i ^= 0x1Bu;
  dut->req_typ_i ^= 0xFu;
  dut->req_gsc_cnt_i ^= 0x7u;
  dut->req_ps_mask_i ^= 0xA5u;
  dut->req_buf_alloc_i ^= 1;
  dut->req_buf_id_i ^= 0x7u;
  dut->req_cache_op_i ^= 1;
  dut->req_gsc32_idx_i ^= 0xA55A5AA5u;
  dut->req_vm_sel_i ^= 0x7u;
  dut->req_mem_global_i ^= 1;
}

void mutate_rearm_payload_midcycle(DUT* dut) {
  dut->req_addr_i ^= 0x3FFu;
  dut->req_phys_i ^= 1;
  dut->req_misaligned_i ^= 1;
  dut->req_misaligned_2nd_i ^= 1;
  dut->req_uc_load_2nd_i ^= 1;
  dut->cacheable_i ^= 1;
}

void mutate_req_payload_midcycle(DUT* dut, uint32_t& seed) {
  dut->req_addr_i ^= xorshift32(seed) & ((1ULL << 20) - 1);
  dut->req_phys_i ^= xorshift32(seed) & 1u;
  dut->req_misaligned_i ^= xorshift32(seed) & 1u;
  dut->req_misaligned_2nd_i ^= xorshift32(seed) & 1u;
  dut->req_dest_fp_i ^= xorshift32(seed) & 1u;
  dut->req_dest_addr_i ^= xorshift32(seed) & 0x1Fu;
  dut->req_dest_thread_i ^= xorshift32(seed) & 1u;
  dut->req_cmd_i ^= xorshift32(seed) & 0x1Fu;
  dut->req_typ_i ^= xorshift32(seed) & 0xFu;
  dut->req_gsc_cnt_i ^= xorshift32(seed) & 0x7u;
  dut->req_ps_mask_i ^= xorshift32(seed) & 0xFFu;
  dut->req_cache_op_i ^= xorshift32(seed) & 1u;
  dut->req_buf_alloc_i ^= xorshift32(seed) & 1u;
  dut->req_buf_id_i ^= xorshift32(seed) & 0x7u;
  dut->req_cacheable_i ^= xorshift32(seed) & 1u;
  dut->req_uc_load_2nd_i ^= xorshift32(seed) & 1u;
  dut->req_gsc32_idx_i ^= xorshift32(seed);
  dut->req_vm_sel_i ^= xorshift32(seed) & 0x7u;
  dut->req_mem_global_i ^= xorshift32(seed) & 1u;
  dut->cacheable_i ^= xorshift32(seed) & 1u;
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  step_half(sim, 0, cov);
  step_half(sim, 1, cov);
}

void tick_and_compare_with_midcycle_mutation(CosimCtrl<DUT>& sim, Coverage* cov, uint32_t& seed) {
  step_half(sim, 0, cov);
  mutate_req_payload_midcycle(sim.dut.get(), seed);
  sim.dut->eval();
  compare_outputs(sim);
  step_half(sim, 1, cov);
}

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void stage_push(CosimCtrl<DUT>& sim, Coverage& cov) {
  sim.dut->push_req_prev_i = 1;
  step_half(sim, 0, &cov);
  sim.dut->push_req_prev_i = 0;
  sim.dut->push_req_i = 1;
  mutate_push_payload_midcycle(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);
  step_half(sim, 1, &cov);
  sim.dut->push_req_i = 0;
}

void stage_rearm(CosimCtrl<DUT>& sim, Coverage& cov) {
  sim.dut->rearm_prev_i = 1;
  step_half(sim, 0, &cov);
  sim.dut->rearm_prev_i = 0;
  sim.dut->rearm_i = 1;
  mutate_rearm_payload_midcycle(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);
  step_half(sim, 1, &cov);
  sim.dut->rearm_i = 0;
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

  sim.dut->req_addr_i = 0x120;
  sim.dut->req_dest_addr_i = 5;
  sim.dut->req_cmd_i = 1;
  sim.dut->req_typ_i = 2;
  sim.dut->req_gsc_cnt_i = 3;
  sim.dut->req_ps_mask_i = 0x5A;
  sim.dut->req_cacheable_i = 1;
  sim.dut->cacheable_i = 1;
  sim.dut->req_uc_load_ok_i = 1;
  sim.dut->req_miss_fill_ok_i = 1;
  sim.dut->uc_load_ok_i = 1;
  sim.dut->miss_fill_ok_i = 1;
  stage_push(sim, coverage);
  tick_and_compare(sim, &coverage);
  sim.dut->replay_grant_i = 1;
  tick_and_compare(sim, &coverage);
  sim.dut->replay_grant_i = 0;
  tick_and_compare(sim, &coverage);

  sim.dut->req_replay_i = 1;
  sim.dut->req_replay_entry_i = 7;
  sim.dut->req_replay_age_i = sim.dut->new_replay_age_o;
  sim.dut->rearm_entry_i = 7;
  sim.dut->req_conf_vpu_rf_write_i = 1;
  sim.dut->vpu_rf_in_use_i = 1;
  stage_rearm(sim, coverage);
  sim.dut->vpu_rf_in_use_i = 0;
  tick_and_compare(sim, &coverage);

  sim.dut->dealloc_i = 1;
  sim.dut->dealloc_entry_i = 7;
  tick_and_compare(sim, &coverage);
  sim.dut->dealloc_i = 0;

  clear_inputs(sim.dut.get());
  sim.dut->req_addr_i = 0x220;
  sim.dut->req_dest_fp_i = 1;
  sim.dut->req_dest_thread_i = 1;
  sim.dut->req_dest_addr_i = 9;
  sim.dut->req_cmd_i = 1;
  sim.dut->req_typ_i = 1;
  sim.dut->req_cacheable_i = 1;
  sim.dut->cacheable_i = 1;
  sim.dut->req_conf_miss_alloc_i = 1;
  sim.dut->req_conf_miss_assigned_i = 1;
  stage_push(sim, coverage);
  tick_and_compare(sim, &coverage);
  sim.dut->miss_available_i = 1;
  tick_and_compare(sim, &coverage);
  sim.dut->miss_available_i = 0;
  sim.dut->dealloc_i = 1;
  sim.dut->dealloc_entry_i = 7;
  tick_and_compare(sim, &coverage);
  sim.dut->dealloc_i = 0;

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < 1024; ++cycle) {
      sim.dut->alloc_req_pre_i = xorshift32(seed) & 1u;
      sim.dut->alloc_req_val_i = xorshift32(seed) & 1u;
      sim.dut->alloc_gsc_i = xorshift32(seed) & 1u;
      sim.dut->push_req_prev_i = xorshift32(seed) & 1u;
      sim.dut->push_req_i = xorshift32(seed) & 1u;
      sim.dut->req_addr_i = xorshift32(seed) & ((1ULL << 20) - 1);
      sim.dut->req_phys_i = xorshift32(seed) & 1u;
      sim.dut->req_misaligned_i = xorshift32(seed) & 1u;
      sim.dut->req_misaligned_2nd_i = xorshift32(seed) & 1u;
      sim.dut->req_dest_fp_i = xorshift32(seed) & 1u;
      sim.dut->req_dest_addr_i = xorshift32(seed) & 0x1Fu;
      sim.dut->req_dest_thread_i = xorshift32(seed) & 1u;
      sim.dut->req_cmd_i = xorshift32(seed) & 0x1Fu;
      sim.dut->req_typ_i = xorshift32(seed) & 0xFu;
      sim.dut->req_gsc_cnt_i = xorshift32(seed) & 0x7u;
      sim.dut->req_ps_mask_i = xorshift32(seed) & 0xFFu;
      sim.dut->req_replay_i = xorshift32(seed) & 1u;
      sim.dut->req_replay_entry_i = xorshift32(seed) & 0x7u;
      sim.dut->req_replay_age_i = xorshift32(seed) & 0x1Fu;
      sim.dut->req_cache_op_i = xorshift32(seed) & 1u;
      sim.dut->req_buf_alloc_i = xorshift32(seed) & 1u;
      sim.dut->req_buf_id_i = xorshift32(seed) & 0x7u;
      sim.dut->req_cacheable_i = xorshift32(seed) & 1u;
      sim.dut->req_uc_load_2nd_i = xorshift32(seed) & 1u;
      sim.dut->req_uc_load_ok_i = xorshift32(seed) & 1u;
      sim.dut->req_miss_fill_ok_i = xorshift32(seed) & 1u;
      sim.dut->req_gsc32_idx_i = xorshift32(seed);
      sim.dut->req_vm_sel_i = xorshift32(seed) & 0x7u;
      sim.dut->req_mem_global_i = xorshift32(seed) & 1u;
      sim.dut->cacheable_i = xorshift32(seed) & 1u;
      sim.dut->req_conf_vpu_rf_write_i = xorshift32(seed) & 1u;
      sim.dut->req_conf_miss_alloc_i = xorshift32(seed) & 1u;
      sim.dut->req_conf_miss_accepted_i = xorshift32(seed) & 0x3u;
      sim.dut->req_conf_miss_assigned_i = xorshift32(seed) & 0x3u;
      sim.dut->misaligned_1st_i = xorshift32(seed) & 1u;
      sim.dut->uc_load_1st_i = xorshift32(seed) & 1u;
      sim.dut->uc_load_ok_i = xorshift32(seed) & 1u;
      sim.dut->miss_fill_ok_i = xorshift32(seed) & 1u;
      sim.dut->rearm_i = xorshift32(seed) & 1u;
      sim.dut->rearm_prev_i = xorshift32(seed) & 1u;
      sim.dut->rearm_entry_i = xorshift32(seed) & 0x7u;
      sim.dut->dealloc_i = xorshift32(seed) & 1u;
      sim.dut->dealloc_entry_i = xorshift32(seed) & 0x7u;
      sim.dut->miss_available_i = xorshift32(seed) & 0x3u;
      sim.dut->miss_fill_error_i = xorshift32(seed) & 0x3u;
      sim.dut->uc_load_fill_i = xorshift32(seed) & 1u;
      sim.dut->uc_load_fill_id_i = xorshift32(seed) & 0x7u;
      sim.dut->uc_load_fill_ok_i = xorshift32(seed) & 1u;
      sim.dut->vpu_rf_in_use_i = xorshift32(seed) & 1u;
      sim.dut->replay_grant_i = xorshift32(seed) & 1u;
      tick_and_compare_with_midcycle_mutation(sim, &coverage, seed);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

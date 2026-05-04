// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_cache_op_unit vs minion_dcache_cache_op_unit.

#include "Vcosim_minion_dcache_cache_op_unit_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_cache_op_unit_tb;

namespace {

constexpr int kRandomCycles = 12000;
constexpr uint64_t kPaTagMask = (1ULL << 33) - 1ULL;
constexpr uint32_t kSetMask = (1u << 4) - 1u;

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
  BoolCoverage chicken_bit_dcache, cache_op_valid, cache_op_thread_id;
  BoolCoverage cache_op_use_tmask, cache_op_tmask_ready, cache_op_l2_scp_fill;
  BoolCoverage meta_read_grant, s1_tlb_wait, s1_tlb_fail, meta_write_ready;
  BoolCoverage writeback_ready, writeback_done, id_rq_full, id_co_pf_grant;
  BoolCoverage cache_op_ready_byp, mh_clear_slock_valid, da_clear_rdy, rq_conflict;
  BoolCoverage cfg_clear_all, cfg_clear_low, cfg_alloc_scp;
  BoolCoverage vm_status_debug, vm_status_sum, vm_status_mxr, vm_status_mprv;
  NonZeroCoverage cache_op_cmd, cache_op_start_level, cache_op_dest_level, cache_op_count;
  NonZeroCoverage cache_op_addr, cache_op_way, cache_op_stride, cache_op_tmask_bits;
  NonZeroCoverage cache_op_l2_scp_dest, meta_read_resp_way_en;
  NonZeroCoverage meta_read_resp_tag0, meta_read_resp_tag1, meta_read_resp_tag2, meta_read_resp_tag3;
  NonZeroCoverage meta_read_resp_state0, meta_read_resp_state1, meta_read_resp_state2, meta_read_resp_state3;
  NonZeroCoverage s1_phys_addr_tag, mh_clear_slock_set, mh_clear_slock_way;
  NonZeroCoverage mh_probe_rdy, mh_metaw_rdy, cfg_cache_mode;
  NonZeroCoverage vm_status_prv, vm_status_mpp;
};

void clear_inputs(DUT* dut) {
  dut->chicken_bit_dcache_i = 0;
  dut->cache_op_valid_i = 0;
  dut->cache_op_cmd_i = 0;
  dut->cache_op_start_level_i = 0;
  dut->cache_op_dest_level_i = 0;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0;
  dut->cache_op_way_i = 0;
  dut->cache_op_stride_i = 0;
  dut->cache_op_thread_id_i = 0;
  dut->cache_op_use_tmask_i = 0;
  dut->cache_op_tmask_ready_i = 0;
  dut->cache_op_tmask_bits_i = 0;
  dut->cache_op_l2_scp_dest_i = 0;
  dut->cache_op_l2_scp_fill_i = 0;
  dut->meta_read_grant_i = 0;
  dut->meta_read_resp_way_en_i = 0;
  dut->meta_read_resp_tag0_i = 0;
  dut->meta_read_resp_tag1_i = 0;
  dut->meta_read_resp_tag2_i = 0;
  dut->meta_read_resp_tag3_i = 0;
  dut->meta_read_resp_state0_i = 0;
  dut->meta_read_resp_state1_i = 0;
  dut->meta_read_resp_state2_i = 0;
  dut->meta_read_resp_state3_i = 0;
  dut->s1_phys_addr_tag_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->meta_write_ready_i = 0;
  dut->writeback_ready_i = 0;
  dut->writeback_done_i = 0;
  dut->id_rq_full_i = 0;
  dut->id_co_pf_grant_i = 0;
  dut->cache_op_ready_byp_i = 0;
  dut->mh_clear_slock_valid_i = 0;
  dut->mh_clear_slock_set_i = 0;
  dut->mh_clear_slock_way_i = 0;
  dut->da_clear_rdy_i = 0;
  dut->mh_probe_rdy_i = 0x3;
  dut->mh_metaw_rdy_i = 0x3;
  dut->rq_conflict_i = 0;
  dut->cfg_clear_all_i = 0;
  dut->cfg_clear_low_i = 0;
  dut->cfg_alloc_scp_i = 0;
  dut->cfg_cache_mode_i = 0;
  dut->vm_status_debug_i = 0;
  dut->vm_status_sum_i = 0;
  dut->vm_status_mxr_i = 0;
  dut->vm_status_prv_i = 0;
  dut->vm_status_mpp_i = 0;
  dut->vm_status_mprv_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.chicken_bit_dcache.sample(dut->chicken_bit_dcache_i);
  cov.cache_op_valid.sample(dut->cache_op_valid_i);
  cov.cache_op_thread_id.sample(dut->cache_op_thread_id_i);
  cov.cache_op_use_tmask.sample(dut->cache_op_use_tmask_i);
  cov.cache_op_tmask_ready.sample(dut->cache_op_tmask_ready_i);
  cov.cache_op_l2_scp_fill.sample(dut->cache_op_l2_scp_fill_i);
  cov.meta_read_grant.sample(dut->meta_read_grant_i);
  cov.s1_tlb_wait.sample(dut->s1_tlb_wait_i);
  cov.s1_tlb_fail.sample(dut->s1_tlb_fail_i);
  cov.meta_write_ready.sample(dut->meta_write_ready_i);
  cov.writeback_ready.sample(dut->writeback_ready_i);
  cov.writeback_done.sample(dut->writeback_done_i);
  cov.id_rq_full.sample(dut->id_rq_full_i);
  cov.id_co_pf_grant.sample(dut->id_co_pf_grant_i);
  cov.cache_op_ready_byp.sample(dut->cache_op_ready_byp_i);
  cov.mh_clear_slock_valid.sample(dut->mh_clear_slock_valid_i);
  cov.da_clear_rdy.sample(dut->da_clear_rdy_i);
  cov.rq_conflict.sample(dut->rq_conflict_i);
  cov.cfg_clear_all.sample(dut->cfg_clear_all_i);
  cov.cfg_clear_low.sample(dut->cfg_clear_low_i);
  cov.cfg_alloc_scp.sample(dut->cfg_alloc_scp_i);
  cov.vm_status_debug.sample(dut->vm_status_debug_i);
  cov.vm_status_sum.sample(dut->vm_status_sum_i);
  cov.vm_status_mxr.sample(dut->vm_status_mxr_i);
  cov.vm_status_mprv.sample(dut->vm_status_mprv_i);
  cov.cache_op_cmd.sample(dut->cache_op_cmd_i);
  cov.cache_op_start_level.sample(dut->cache_op_start_level_i);
  cov.cache_op_dest_level.sample(dut->cache_op_dest_level_i);
  cov.cache_op_count.sample(dut->cache_op_count_i);
  cov.cache_op_addr.sample(dut->cache_op_addr_i);
  cov.cache_op_way.sample(dut->cache_op_way_i);
  cov.cache_op_stride.sample(dut->cache_op_stride_i);
  cov.cache_op_tmask_bits.sample(dut->cache_op_tmask_bits_i);
  cov.cache_op_l2_scp_dest.sample(dut->cache_op_l2_scp_dest_i);
  cov.meta_read_resp_way_en.sample(dut->meta_read_resp_way_en_i);
  cov.meta_read_resp_tag0.sample(dut->meta_read_resp_tag0_i);
  cov.meta_read_resp_tag1.sample(dut->meta_read_resp_tag1_i);
  cov.meta_read_resp_tag2.sample(dut->meta_read_resp_tag2_i);
  cov.meta_read_resp_tag3.sample(dut->meta_read_resp_tag3_i);
  cov.meta_read_resp_state0.sample(dut->meta_read_resp_state0_i);
  cov.meta_read_resp_state1.sample(dut->meta_read_resp_state1_i);
  cov.meta_read_resp_state2.sample(dut->meta_read_resp_state2_i);
  cov.meta_read_resp_state3.sample(dut->meta_read_resp_state3_i);
  cov.s1_phys_addr_tag.sample(dut->s1_phys_addr_tag_i);
  cov.mh_clear_slock_set.sample(dut->mh_clear_slock_set_i);
  cov.mh_clear_slock_way.sample(dut->mh_clear_slock_way_i);
  cov.mh_probe_rdy.sample(dut->mh_probe_rdy_i);
  cov.mh_metaw_rdy.sample(dut->mh_metaw_rdy_i);
  cov.cfg_cache_mode.sample(dut->cfg_cache_mode_i);
  cov.vm_status_prv.sample(dut->vm_status_prv_i);
  cov.vm_status_mpp.sample(dut->vm_status_mpp_i);
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("busy", sim.dut->orig_busy_o, sim.dut->new_busy_o);
  sim.compare("cache_op_ready", sim.dut->orig_cache_op_ready_o, sim.dut->new_cache_op_ready_o);
  sim.compare("th_id_complete", sim.dut->orig_th_id_complete_o, sim.dut->new_th_id_complete_o);
  sim.compare("meta_read_bid", sim.dut->orig_meta_read_bid_o, sim.dut->new_meta_read_bid_o);
  sim.compare("meta_read_valid", sim.dut->orig_meta_read_valid_o, sim.dut->new_meta_read_valid_o);
  sim.compare("meta_read_phys", sim.dut->orig_meta_read_phys_o, sim.dut->new_meta_read_phys_o);
  sim.compare("meta_read_force_set", sim.dut->orig_meta_read_force_set_o, sim.dut->new_meta_read_force_set_o);
  sim.compare("meta_read_thread_id", sim.dut->orig_meta_read_thread_id_o, sim.dut->new_meta_read_thread_id_o);
  sim.compare("meta_read_skip_pma", sim.dut->orig_meta_read_skip_pma_o, sim.dut->new_meta_read_skip_pma_o);
  sim.compare("meta_read_addr", sim.dut->orig_meta_read_addr_o, sim.dut->new_meta_read_addr_o);
  sim.compare("meta_read_addr_msb_err", sim.dut->orig_meta_read_addr_msb_err_o, sim.dut->new_meta_read_addr_msb_err_o);
  sim.compare("meta_write_valid_next", sim.dut->orig_meta_write_valid_next_o, sim.dut->new_meta_write_valid_next_o);
  sim.compare("meta_write_valid", sim.dut->orig_meta_write_valid_o, sim.dut->new_meta_write_valid_o);
  sim.compare("meta_write_set", sim.dut->orig_meta_write_set_o, sim.dut->new_meta_write_set_o);
  sim.compare("meta_write_way_en", sim.dut->orig_meta_write_way_en_o, sim.dut->new_meta_write_way_en_o);
  sim.compare("meta_write_tag", sim.dut->orig_meta_write_tag_o, sim.dut->new_meta_write_tag_o);
  sim.compare("meta_write_coh_state", sim.dut->orig_meta_write_coh_state_o, sim.dut->new_meta_write_coh_state_o);
  sim.compare("nack_set_valid", sim.dut->orig_nack_set_valid_o, sim.dut->new_nack_set_valid_o);
  sim.compare("nack_set_value", sim.dut->orig_nack_set_value_o, sim.dut->new_nack_set_value_o);
  sim.compare("writeback_valid", sim.dut->orig_writeback_valid_o, sim.dut->new_writeback_valid_o);
  sim.compare("writeback_tag", sim.dut->orig_writeback_tag_o, sim.dut->new_writeback_tag_o);
  sim.compare("writeback_set", sim.dut->orig_writeback_set_o, sim.dut->new_writeback_set_o);
  sim.compare("writeback_way_en", sim.dut->orig_writeback_way_en_o, sim.dut->new_writeback_way_en_o);
  sim.compare("id_co_pf_bid", sim.dut->orig_id_co_pf_bid_o, sim.dut->new_id_co_pf_bid_o);
  sim.compare("s0_co_pf_valid", sim.dut->orig_s0_co_pf_valid_o, sim.dut->new_s0_co_pf_valid_o);
  sim.compare("s0_co_pf_addr", sim.dut->orig_s0_co_pf_addr_o, sim.dut->new_s0_co_pf_addr_o);
  sim.compare("s0_co_pf_addr_msb_err", sim.dut->orig_s0_co_pf_addr_msb_err_o, sim.dut->new_s0_co_pf_addr_msb_err_o);
  sim.compare("s0_co_pf_cmd", sim.dut->orig_s0_co_pf_cmd_o, sim.dut->new_s0_co_pf_cmd_o);
  sim.compare("s0_co_pf_typ", sim.dut->orig_s0_co_pf_typ_o, sim.dut->new_s0_co_pf_typ_o);
  sim.compare("s0_co_pf_cacheable", sim.dut->orig_s0_co_pf_cacheable_o, sim.dut->new_s0_co_pf_cacheable_o);
  sim.compare("s0_co_pf_thread_id", sim.dut->orig_s0_co_pf_thread_id_o, sim.dut->new_s0_co_pf_thread_id_o);
  sim.compare("s0_co_pf_cache_op", sim.dut->orig_s0_co_pf_cache_op_o, sim.dut->new_s0_co_pf_cache_op_o);
  sim.compare("s0_co_pf_vm_sel", sim.dut->orig_s0_co_pf_vm_sel_o, sim.dut->new_s0_co_pf_vm_sel_o);
  sim.compare("cache_op_valid_byp", sim.dut->orig_cache_op_valid_byp_o, sim.dut->new_cache_op_valid_byp_o);
  sim.compare("cache_op_byp_cmd", sim.dut->orig_cache_op_byp_cmd_o, sim.dut->new_cache_op_byp_cmd_o);
  sim.compare("cache_op_byp_start_level", sim.dut->orig_cache_op_byp_start_level_o, sim.dut->new_cache_op_byp_start_level_o);
  sim.compare("cache_op_byp_dest_level", sim.dut->orig_cache_op_byp_dest_level_o, sim.dut->new_cache_op_byp_dest_level_o);
  sim.compare("cache_op_byp_count", sim.dut->orig_cache_op_byp_count_o, sim.dut->new_cache_op_byp_count_o);
  sim.compare("cache_op_byp_addr", sim.dut->orig_cache_op_byp_addr_o, sim.dut->new_cache_op_byp_addr_o);
  sim.compare("cache_op_byp_way", sim.dut->orig_cache_op_byp_way_o, sim.dut->new_cache_op_byp_way_o);
  sim.compare("cache_op_byp_stride", sim.dut->orig_cache_op_byp_stride_o, sim.dut->new_cache_op_byp_stride_o);
  sim.compare("cache_op_byp_thread_id", sim.dut->orig_cache_op_byp_thread_id_o, sim.dut->new_cache_op_byp_thread_id_o);
  sim.compare("cache_op_byp_use_tmask", sim.dut->orig_cache_op_byp_use_tmask_o, sim.dut->new_cache_op_byp_use_tmask_o);
  sim.compare("cache_op_byp_tmask_ready", sim.dut->orig_cache_op_byp_tmask_ready_o, sim.dut->new_cache_op_byp_tmask_ready_o);
  sim.compare("cache_op_byp_tmask_bits", sim.dut->orig_cache_op_byp_tmask_bits_o, sim.dut->new_cache_op_byp_tmask_bits_o);
  sim.compare("cache_op_byp_l2_scp_dest", sim.dut->orig_cache_op_byp_l2_scp_dest_o, sim.dut->new_cache_op_byp_l2_scp_dest_o);
  sim.compare("cache_op_byp_l2_scp_fill", sim.dut->orig_cache_op_byp_l2_scp_fill_o, sim.dut->new_cache_op_byp_l2_scp_fill_o);
  sim.compare("da_clear_en", sim.dut->orig_da_clear_en_o, sim.dut->new_da_clear_en_o);
  sim.compare("da_clear_val", sim.dut->orig_da_clear_val_o, sim.dut->new_da_clear_val_o);
  sim.compare("da_clear_set", sim.dut->orig_da_clear_set_o, sim.dut->new_da_clear_set_o);
  sim.compare("da_clear_way", sim.dut->orig_da_clear_way_o, sim.dut->new_da_clear_way_o);
  sim.compare("da_clear_idx", sim.dut->orig_da_clear_idx_o, sim.dut->new_da_clear_idx_o);
  sim.compare("vm_status_co_prv", sim.dut->orig_vm_status_co_prv_o, sim.dut->new_vm_status_co_prv_o);
  sim.compare("vm_status_co_mpp", sim.dut->orig_vm_status_co_mpp_o, sim.dut->new_vm_status_co_mpp_o);
  sim.compare("vm_status_co_mprv", sim.dut->orig_vm_status_co_mprv_o, sim.dut->new_vm_status_co_mprv_o);
  sim.compare("vm_status_co_sum", sim.dut->orig_vm_status_co_sum_o, sim.dut->new_vm_status_co_sum_o);
  sim.compare("vm_status_co_mxr", sim.dut->orig_vm_status_co_mxr_o, sim.dut->new_vm_status_co_mxr_o);
  sim.compare("vm_status_co_debug", sim.dut->orig_vm_status_co_debug_o, sim.dut->new_vm_status_co_debug_o);
  sim.compare("err_flags", sim.dut->orig_err_flags_o, sim.dut->new_err_flags_o);
  sim.compare("csr_debug_signals", sim.dut->orig_csr_debug_signals_o, sim.dut->new_csr_debug_signals_o);
  sim.compare("sm_match_debug_signals", sim.dut->orig_sm_match_debug_signals_o, sim.dut->new_sm_match_debug_signals_o);
  for (int i = 0; i < 8; ++i) {
    sim.compare("hlock_chunk", sim.dut->orig_hlock_chunks_o[i], sim.dut->new_hlock_chunks_o[i]);
    sim.compare("slock_chunk", sim.dut->orig_slock_chunks_o[i], sim.dut->new_slock_chunks_o[i]);
  }
  for (int i = 0; i < 4; ++i) {
    sim.compare("sm_filter_chunk", sim.dut->orig_sm_filter_chunks_o[i], sim.dut->new_sm_filter_chunks_o[i]);
  }
  for (int i = 0; i < 8; ++i) {
    sim.compare("sm_data_chunk", sim.dut->orig_sm_data_chunks_o[i], sim.dut->new_sm_data_chunks_o[i]);
  }
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  int mismatches_before = sim.mismatches;
  compare_outputs(sim);
  if ((mismatches_before == 0) && (sim.mismatches != 0)) {
    printf("FIRST MISMATCH [t=%lu] valid=%u cmd=%u start=%u dest=%u count=%u addr=0x%llx way=%u stride=0x%llx thread=%u\n",
           (unsigned long)sim.sim_time, sim.dut->cache_op_valid_i, sim.dut->cache_op_cmd_i,
           sim.dut->cache_op_start_level_i, sim.dut->cache_op_dest_level_i, sim.dut->cache_op_count_i,
           (unsigned long long)sim.dut->cache_op_addr_i, sim.dut->cache_op_way_i,
           (unsigned long long)sim.dut->cache_op_stride_i, sim.dut->cache_op_thread_id_i);
    printf("  meta: grant=%u way_en=0x%x states=%u/%u/%u/%u tags=%llx/%llx/%llx/%llx phys=0x%llx tlb_wait=%u tlb_fail=%u mw_ready=%u wb_ready=%u wb_done=%u\n",
           sim.dut->meta_read_grant_i, sim.dut->meta_read_resp_way_en_i,
           sim.dut->meta_read_resp_state0_i, sim.dut->meta_read_resp_state1_i,
           sim.dut->meta_read_resp_state2_i, sim.dut->meta_read_resp_state3_i,
           (unsigned long long)sim.dut->meta_read_resp_tag0_i,
           (unsigned long long)sim.dut->meta_read_resp_tag1_i,
           (unsigned long long)sim.dut->meta_read_resp_tag2_i,
           (unsigned long long)sim.dut->meta_read_resp_tag3_i,
           (unsigned long long)sim.dut->s1_phys_addr_tag_i, sim.dut->s1_tlb_wait_i,
           sim.dut->s1_tlb_fail_i, sim.dut->meta_write_ready_i, sim.dut->writeback_ready_i,
           sim.dut->writeback_done_i);
    printf("  orig: mw_valid=%u mw_next=%u mw_set=%u mw_way=0x%x mw_tag=0x%llx mw_coh=%u wb_valid=%u wb_set=%u wb_way=0x%x wb_tag=0x%llx byp=0x%llx busy=%u ready=%u\n",
           sim.dut->orig_meta_write_valid_o, sim.dut->orig_meta_write_valid_next_o,
           sim.dut->orig_meta_write_set_o, sim.dut->orig_meta_write_way_en_o,
           (unsigned long long)sim.dut->orig_meta_write_tag_o, sim.dut->orig_meta_write_coh_state_o,
           sim.dut->orig_writeback_valid_o, sim.dut->orig_writeback_set_o,
           sim.dut->orig_writeback_way_en_o, (unsigned long long)sim.dut->orig_writeback_tag_o,
           (unsigned long long)sim.dut->orig_cache_op_byp_addr_o, sim.dut->orig_busy_o,
           sim.dut->orig_cache_op_ready_o);
    printf("  new : mw_valid=%u mw_next=%u mw_set=%u mw_way=0x%x mw_tag=0x%llx mw_coh=%u wb_valid=%u wb_set=%u wb_way=0x%x wb_tag=0x%llx byp=0x%llx busy=%u ready=%u\n",
           sim.dut->new_meta_write_valid_o, sim.dut->new_meta_write_valid_next_o,
           sim.dut->new_meta_write_set_o, sim.dut->new_meta_write_way_en_o,
           (unsigned long long)sim.dut->new_meta_write_tag_o, sim.dut->new_meta_write_coh_state_o,
           sim.dut->new_writeback_valid_o, sim.dut->new_writeback_set_o,
           sim.dut->new_writeback_way_en_o, (unsigned long long)sim.dut->new_writeback_tag_o,
           (unsigned long long)sim.dut->new_cache_op_byp_addr_o, sim.dut->new_busy_o,
           sim.dut->new_cache_op_ready_o);
  }
}

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void run_directed(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->cache_op_cmd_i = 4;
  sim.dut->cache_op_start_level_i = 0;
  sim.dut->cache_op_dest_level_i = 0;
  sim.dut->cache_op_addr_i = 0x12340;
  sim.dut->cache_op_stride_i = 0x40;
  sim.dut->cache_op_thread_id_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 0;
  sim.dut->id_co_pf_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->id_co_pf_grant_i = 0;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  sim.dut->cache_op_cmd_i = 3;
  sim.dut->cache_op_start_level_i = 0;
  sim.dut->cache_op_dest_level_i = 1;
  sim.dut->cache_op_addr_i = 0x2c0;
  sim.dut->cache_op_way_i = 2;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 0;
  sim.dut->meta_read_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->meta_read_grant_i = 0;
  sim.dut->meta_read_resp_state2_i = 0;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->meta_write_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->meta_write_ready_i = 0;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  sim.dut->cache_op_cmd_i = 1;
  sim.dut->cache_op_start_level_i = 0;
  sim.dut->cache_op_dest_level_i = 0;
  sim.dut->cache_op_addr_i = 0x4000;
  sim.dut->cache_op_use_tmask_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 0;
  sim.dut->cache_op_tmask_ready_i = 1;
  sim.dut->cache_op_tmask_bits_i = 1;
  sim.dut->meta_read_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_tmask_ready_i = 0;
  sim.dut->meta_read_grant_i = 0;
  sim.dut->s1_tlb_wait_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->s1_tlb_wait_i = 0;
  tick_and_compare(sim, &cov);

  clear_inputs(sim.dut.get());
  sim.dut->cache_op_cmd_i = 3;
  sim.dut->cache_op_start_level_i = 0;
  sim.dut->cache_op_dest_level_i = 1;
  sim.dut->cache_op_addr_i = 0x6c0;
  sim.dut->cache_op_way_i = 2;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->cache_op_valid_i = 0;
  sim.dut->meta_read_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->meta_read_grant_i = 0;
  sim.dut->meta_read_resp_state2_i = 3;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  sim.dut->writeback_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->writeback_ready_i = 0;
  sim.dut->writeback_done_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->writeback_done_i = 0;
  sim.dut->meta_write_ready_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->meta_write_ready_i = 0;
  tick_and_compare(sim, &cov);
}

void run_random(CosimCtrl<DUT>& sim, Coverage& cov) {
  uint32_t rng = 0x13579bdfu;
  static constexpr uint8_t kCmds[] = {0, 1, 2, 3, 4, 6, 7, 8, 9};
  uint8_t req_cmd = 0;
  uint8_t req_start_level = 0;
  uint8_t req_dest_level = 0;
  uint8_t req_count = 0;
  uint64_t req_addr = 0;
  uint8_t req_way = 0;
  uint64_t req_stride = 0;
  uint8_t req_thread_id = 0;
  bool req_use_tmask = false;
  bool req_tmask_ready = false;
  uint32_t req_tmask_bits = 1;
  uint8_t req_l2_scp_dest = 0;
  bool req_l2_scp_fill = false;
  bool req_active = false;
  bool pending_issue = false;
  for (int cycle = 0; cycle < kRandomCycles; ++cycle) {
    sim.dut->eval();
    uint32_t r0 = xorshift32(rng);
    uint32_t r1 = xorshift32(rng);
    uint32_t r2 = xorshift32(rng);
    uint32_t r3 = xorshift32(rng);
    uint8_t cmd = kCmds[r0 % (sizeof(kCmds) / sizeof(kCmds[0]))];
    uint8_t hit_way = (r1 >> 0) & 0x3;
    uint8_t hit_state = (r1 >> 4) & 0x3;
    uint8_t way_en = ((r1 >> 8) & 1) ? static_cast<uint8_t>(1u << hit_way) : 0;
    bool meta_bid = sim.dut->orig_meta_read_bid_o || sim.dut->new_meta_read_bid_o;
    bool meta_valid = sim.dut->orig_meta_read_valid_o || sim.dut->new_meta_read_valid_o;
    bool wb_valid = sim.dut->orig_writeback_valid_o || sim.dut->new_writeback_valid_o;
    bool pf_bid = sim.dut->orig_id_co_pf_bid_o || sim.dut->new_id_co_pf_bid_o;
    bool byp_valid = sim.dut->orig_cache_op_valid_byp_o || sim.dut->new_cache_op_valid_byp_o;
    bool both_idle = !sim.dut->orig_busy_o && !sim.dut->new_busy_o;
    bool both_ready = sim.dut->orig_cache_op_ready_o && sim.dut->new_cache_op_ready_o;
    bool choose_issue = both_idle && both_ready && !pending_issue && (((r0 >> 1) & 0x3) == 0);
    bool issue_req = pending_issue;

    if (choose_issue) {
      req_cmd = cmd;
      if (req_cmd == 4) {
        req_start_level = 0;
        req_dest_level = 0;
      } else if ((req_cmd == 2) || (req_cmd == 3)) {
        req_start_level = 0;
        req_dest_level = 1 + ((r0 >> 20) & 0x1);
      } else if ((req_cmd == 6) || (req_cmd == 7)) {
        req_start_level = 0;
        req_dest_level = 1 + ((r0 >> 21) & 0x1);
      } else {
        req_start_level = ((r0 >> 22) & 1) ? 1 : 0;
        req_dest_level = 0;
      }
      req_count = (r0 >> 12) & 0x7;
      req_addr = (((uint64_t)r1 << 16) ^ ((uint64_t)r2 << 4)) & ((1ULL << 40) - 64);
      if ((cycle & 0x3f) == 0) req_addr = 0;
      req_way = (r0 >> 8) & 0x3;
      req_stride = ((uint64_t)(r2 & 0x3ff) << 6);
      req_thread_id = (r1 >> 2) & 1;
      req_use_tmask = ((r1 >> 3) & 1) != 0;
      req_tmask_ready = !req_use_tmask;
      req_tmask_bits = r2 | 1u;
      req_l2_scp_dest = (r1 >> 5) & 0x3;
      req_l2_scp_fill = ((r1 >> 7) & 1) != 0;
      pending_issue = true;
    } else if (issue_req) {
      pending_issue = false;
    }

    if (req_active && req_use_tmask && !req_tmask_ready && (((r2 >> 9) & 0x3) == 0)) {
      req_tmask_ready = true;
    }

    sim.dut->chicken_bit_dcache_i = (r0 >> 0) & 1;
    sim.dut->cache_op_valid_i = issue_req;
    sim.dut->cache_op_cmd_i = req_cmd;
    sim.dut->cache_op_start_level_i = req_start_level;
    sim.dut->cache_op_dest_level_i = req_dest_level;
    sim.dut->cache_op_count_i = req_count;
    sim.dut->cache_op_addr_i = req_addr;
    sim.dut->cache_op_way_i = req_way;
    sim.dut->cache_op_stride_i = req_stride;
    sim.dut->cache_op_thread_id_i = req_thread_id;
    sim.dut->cache_op_use_tmask_i = req_use_tmask;
    sim.dut->cache_op_tmask_ready_i = req_tmask_ready;
    sim.dut->cache_op_tmask_bits_i = req_tmask_bits;
    sim.dut->cache_op_l2_scp_dest_i = req_l2_scp_dest;
    sim.dut->cache_op_l2_scp_fill_i = req_l2_scp_fill;

    sim.dut->meta_read_grant_i = meta_bid && (((r2 >> 0) & 0x3) != 0);
    sim.dut->meta_read_resp_way_en_i = meta_valid ? way_en : 0;
    sim.dut->meta_read_resp_tag0_i = r0 & kPaTagMask;
    sim.dut->meta_read_resp_tag1_i = r1 & kPaTagMask;
    sim.dut->meta_read_resp_tag2_i = r2 & kPaTagMask;
    sim.dut->meta_read_resp_tag3_i = r3 & kPaTagMask;
    sim.dut->meta_read_resp_state0_i = (hit_way == 0) ? hit_state : ((r0 >> 24) & 0x3);
    sim.dut->meta_read_resp_state1_i = (hit_way == 1) ? hit_state : ((r1 >> 24) & 0x3);
    sim.dut->meta_read_resp_state2_i = (hit_way == 2) ? hit_state : ((r2 >> 24) & 0x3);
    sim.dut->meta_read_resp_state3_i = (hit_way == 3) ? hit_state : ((r3 >> 24) & 0x3);
    sim.dut->s1_phys_addr_tag_i = r3 & kPaTagMask;
    sim.dut->s1_tlb_wait_i = meta_valid && ((r2 >> 1) & 1);
    sim.dut->s1_tlb_fail_i = meta_valid && !sim.dut->s1_tlb_wait_i && ((r2 >> 2) & 1);
    sim.dut->meta_write_ready_i = ((r2 >> 3) & 1) | !(sim.dut->orig_meta_write_valid_o || sim.dut->new_meta_write_valid_o);
    sim.dut->writeback_ready_i = wb_valid && ((r2 >> 4) & 1);
    sim.dut->writeback_done_i = wb_valid && sim.dut->writeback_ready_i && ((r2 >> 5) & 1);
    sim.dut->id_rq_full_i = (r2 >> 6) & 1;
    sim.dut->id_co_pf_grant_i = pf_bid && !sim.dut->id_rq_full_i && ((r2 >> 7) & 1);
    sim.dut->cache_op_ready_byp_i = byp_valid ? ((r2 >> 8) & 1) : ((r2 >> 8) & 1);
    sim.dut->mh_clear_slock_valid_i = (r3 >> 0) & 1;
    sim.dut->mh_clear_slock_set_i = (r3 >> 1) & kSetMask;
    sim.dut->mh_clear_slock_way_i = (r3 >> 5) & 0x3;
    sim.dut->da_clear_rdy_i = (r3 >> 7) & 1;
    sim.dut->mh_probe_rdy_i = ((r3 >> 8) & 1) ? 0x3 : (r3 & 0x3);
    sim.dut->mh_metaw_rdy_i = ((r3 >> 10) & 1) ? 0x3 : ((r3 >> 2) & 0x3);
    sim.dut->rq_conflict_i = (r3 >> 12) & 1;
    sim.dut->cfg_clear_all_i = (r3 >> 13) & 1;
    sim.dut->cfg_clear_low_i = (r3 >> 14) & 1;
    sim.dut->cfg_alloc_scp_i = (r3 >> 15) & 1;
    sim.dut->cfg_cache_mode_i = (r3 >> 16) & 0x3;
    sim.dut->vm_status_debug_i = r0 & 0x3;
    sim.dut->vm_status_sum_i = r1 & 0x3;
    sim.dut->vm_status_mxr_i = r2 & 0x3;
    sim.dut->vm_status_prv_i = r3 & 0xF;
    sim.dut->vm_status_mpp_i = (r3 >> 4) & 0xF;
    sim.dut->vm_status_mprv_i = (r3 >> 8) & 0x3;
    tick_and_compare(sim, &cov);
    if (sim.mismatches != 0) {
      return;
    }
    req_active = sim.dut->orig_busy_o || sim.dut->new_busy_o;
  }
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.chicken_bit_dcache.full(), "coverage: chicken_bit_dcache exercised 0/1");
  sim.check(cov.cache_op_valid.full(), "coverage: cache_op_valid exercised 0/1");
  sim.check(cov.cache_op_thread_id.full(), "coverage: cache_op_thread_id exercised 0/1");
  sim.check(cov.cache_op_use_tmask.full(), "coverage: cache_op_use_tmask exercised 0/1");
  sim.check(cov.cache_op_tmask_ready.full(), "coverage: cache_op_tmask_ready exercised 0/1");
  sim.check(cov.cache_op_l2_scp_fill.full(), "coverage: cache_op_l2_scp_fill exercised 0/1");
  sim.check(cov.meta_read_grant.full(), "coverage: meta_read_grant exercised 0/1");
  sim.check(cov.s1_tlb_wait.full(), "coverage: s1_tlb_wait exercised 0/1");
  sim.check(cov.s1_tlb_fail.full(), "coverage: s1_tlb_fail exercised 0/1");
  sim.check(cov.meta_write_ready.full(), "coverage: meta_write_ready exercised 0/1");
  sim.check(cov.writeback_ready.full(), "coverage: writeback_ready exercised 0/1");
  sim.check(cov.writeback_done.full(), "coverage: writeback_done exercised 0/1");
  sim.check(cov.id_rq_full.full(), "coverage: id_rq_full exercised 0/1");
  sim.check(cov.id_co_pf_grant.full(), "coverage: id_co_pf_grant exercised 0/1");
  sim.check(cov.cache_op_ready_byp.full(), "coverage: cache_op_ready_byp exercised 0/1");
  sim.check(cov.mh_clear_slock_valid.full(), "coverage: mh_clear_slock_valid exercised 0/1");
  sim.check(cov.da_clear_rdy.full(), "coverage: da_clear_rdy exercised 0/1");
  sim.check(cov.rq_conflict.full(), "coverage: rq_conflict exercised 0/1");
  sim.check(cov.cfg_clear_all.full(), "coverage: cfg_clear_all exercised 0/1");
  sim.check(cov.cfg_clear_low.full(), "coverage: cfg_clear_low exercised 0/1");
  sim.check(cov.cfg_alloc_scp.full(), "coverage: cfg_alloc_scp exercised 0/1");
  sim.check(cov.vm_status_debug.full(), "coverage: vm_status_debug exercised 0/1");
  sim.check(cov.vm_status_sum.full(), "coverage: vm_status_sum exercised 0/1");
  sim.check(cov.vm_status_mxr.full(), "coverage: vm_status_mxr exercised 0/1");
  sim.check(cov.vm_status_mprv.full(), "coverage: vm_status_mprv exercised 0/1");
  sim.check(cov.cache_op_cmd.full(), "coverage: cache_op_cmd exercised zero/nonzero");
  sim.check(cov.cache_op_start_level.full(), "coverage: cache_op_start_level exercised zero/nonzero");
  sim.check(cov.cache_op_dest_level.full(), "coverage: cache_op_dest_level exercised zero/nonzero");
  sim.check(cov.cache_op_count.full(), "coverage: cache_op_count exercised zero/nonzero");
  sim.check(cov.cache_op_addr.full(), "coverage: cache_op_addr exercised zero/nonzero");
  sim.check(cov.cache_op_way.full(), "coverage: cache_op_way exercised zero/nonzero");
  sim.check(cov.cache_op_stride.full(), "coverage: cache_op_stride exercised zero/nonzero");
  sim.check(cov.cache_op_tmask_bits.full(), "coverage: cache_op_tmask_bits exercised zero/nonzero");
  sim.check(cov.cache_op_l2_scp_dest.full(), "coverage: cache_op_l2_scp_dest exercised zero/nonzero");
  sim.check(cov.meta_read_resp_way_en.full(), "coverage: meta_read_resp_way_en exercised zero/nonzero");
  sim.check(cov.meta_read_resp_tag0.full(), "coverage: meta_read_resp_tag0 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_tag1.full(), "coverage: meta_read_resp_tag1 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_tag2.full(), "coverage: meta_read_resp_tag2 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_tag3.full(), "coverage: meta_read_resp_tag3 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_state0.full(), "coverage: meta_read_resp_state0 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_state1.full(), "coverage: meta_read_resp_state1 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_state2.full(), "coverage: meta_read_resp_state2 exercised zero/nonzero");
  sim.check(cov.meta_read_resp_state3.full(), "coverage: meta_read_resp_state3 exercised zero/nonzero");
  sim.check(cov.s1_phys_addr_tag.full(), "coverage: s1_phys_addr_tag exercised zero/nonzero");
  sim.check(cov.mh_clear_slock_set.full(), "coverage: mh_clear_slock_set exercised zero/nonzero");
  sim.check(cov.mh_clear_slock_way.full(), "coverage: mh_clear_slock_way exercised zero/nonzero");
  sim.check(cov.mh_probe_rdy.full(), "coverage: mh_probe_rdy exercised zero/nonzero");
  sim.check(cov.mh_metaw_rdy.full(), "coverage: mh_metaw_rdy exercised zero/nonzero");
  sim.check(cov.cfg_cache_mode.full(), "coverage: cfg_cache_mode exercised zero/nonzero");
  sim.check(cov.vm_status_prv.full(), "coverage: vm_status_prv exercised zero/nonzero");
  sim.check(cov.vm_status_mpp.full(), "coverage: vm_status_mpp exercised zero/nonzero");
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  Coverage cov;

  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);

  run_directed(sim, cov);
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);
  run_random(sim, cov);
  check_coverage(sim, cov);
  return sim.finish();
}

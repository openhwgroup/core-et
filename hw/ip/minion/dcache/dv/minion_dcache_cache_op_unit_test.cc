// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_cache_op_unit.

#include "Vminion_dcache_cache_op_unit_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_cache_op_unit_tb;

namespace {

constexpr uint8_t kCacheOpPref = 4;
constexpr uint8_t kCacheOpEvictSw = 3;
constexpr uint8_t kLevelL1 = 0;
constexpr uint8_t kLevelL2 = 1;
constexpr uint8_t kStateInvalid = 0;
constexpr uint8_t kDcCmdPfr = 2;
constexpr uint8_t kDcTypeD = 3;

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
  dut->probe_set_i = 0;
  dut->probe_way_i = 0;
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

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void tick_n(SimCtrl<DUT>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    tick_and_eval(sim);
  }
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void drive_prefetch_req(DUT* dut) {
  dut->cache_op_valid_i = 1;
  dut->cache_op_cmd_i = kCacheOpPref;
  dut->cache_op_start_level_i = kLevelL1;
  dut->cache_op_dest_level_i = kLevelL1;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0x12340;
  dut->cache_op_stride_i = 0x40;
  dut->cache_op_thread_id_i = 1;
}

void drive_evict_sw_req(DUT* dut) {
  dut->cache_op_valid_i = 1;
  dut->cache_op_cmd_i = kCacheOpEvictSw;
  dut->cache_op_start_level_i = kLevelL1;
  dut->cache_op_dest_level_i = kLevelL2;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0x2c0;  // set 11
  dut->cache_op_way_i = 2;
  dut->cache_op_stride_i = 0;
  dut->cache_op_thread_id_i = 0;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->busy_o == 0, "reset leaves cache-op unit idle");
  sim.check(sim.dut->cache_op_ready_o == 1, "idle unit is immediately ready after reset");
  sim.check(sim.dut->th_id_complete_o == 0xF, "all thread/id slots complete after reset");
  sim.check(sim.dut->hlock_probe_o == 0, "reset clears hard locks");
  sim.check(sim.dut->slock_probe_o == 0, "reset clears soft locks");

  drive_prefetch_req(sim.dut.get());
  sim.dut->eval();
  sim.check(sim.dut->cache_op_ready_o == 1, "idle unit accepts L1 prefetch");
  tick_and_eval(sim);

  sim.check(sim.dut->busy_o == 1, "accepted prefetch leaves idle state");
  sim.check(sim.dut->id_co_pf_bid_o == 1, "prefetch state bids for pipeline injection");
  sim.check(sim.dut->meta_read_bid_o == 0, "prefetch path bypasses metadata read");

  sim.dut->cache_op_valid_i = 0;
  sim.dut->id_co_pf_grant_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->s0_co_pf_valid_o == 1, "pipeline grant produces a prefetch request");
  sim.check(sim.dut->s0_co_pf_addr_o == 0x12340, "prefetch request carries the aligned address");
  sim.check(sim.dut->s0_co_pf_cmd_o == kDcCmdPfr, "prefetch request uses dcache_cmd_PFR");
  sim.check(sim.dut->s0_co_pf_typ_o == kDcTypeD, "prefetch request uses dword type");
  sim.check(sim.dut->s0_co_pf_thread_id_o == 1, "prefetch request keeps the thread id");
  sim.check(sim.dut->s0_co_pf_cache_op_o == 1, "prefetch request is tagged as cache-op work");
  sim.check(sim.dut->s0_co_pf_vm_sel_o == 1, "prefetch request uses the cache-op VM selector");

  sim.dut->id_co_pf_grant_i = 0;
  tick_n(sim, 2);
  sim.check(sim.dut->busy_o == 0, "single prefetch request retires cleanly");
  sim.check(((sim.dut->th_id_complete_o >> 2) & 0x1) == 1, "completion returns once prefetch retires");

  reset_and_clear(sim);
  drive_evict_sw_req(sim.dut.get());
  sim.dut->eval();
  sim.check(sim.dut->cache_op_ready_o == 1, "idle unit accepts Evict_SW");
  tick_and_eval(sim);

  sim.check(sim.dut->busy_o == 1, "Evict_SW request starts work");
  sim.check(sim.dut->meta_read_bid_o == 1, "Evict_SW starts with a metadata read bid");
  sim.check(sim.dut->meta_read_force_set_o == 1, "Evict_SW uses forced set/way lookup");
  sim.check(sim.dut->meta_read_phys_o == 0, "Evict_SW is not a physical metadata read");
  sim.check(sim.dut->meta_read_thread_id_o == 0, "Evict_SW preserves thread id on metadata read");

  sim.dut->cache_op_valid_i = 0;
  sim.dut->meta_read_grant_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->meta_read_valid_o == 1, "metadata grant is captured one cycle later");

  sim.dut->meta_read_grant_i = 0;
  sim.dut->meta_read_resp_state2_i = kStateInvalid;
  tick_n(sim, 4);

  sim.check(sim.dut->meta_write_valid_o == 1, "invalid Evict_SW line falls through to metadata write");
  sim.check(sim.dut->meta_write_way_en_o == 0x4, "Evict_SW writes back the selected way");
  sim.check(sim.dut->meta_write_coh_state_o == kStateInvalid, "Evict_SW keeps invalid line state");
  sim.check(sim.dut->writeback_valid_o == 0, "invalid line does not trigger writeback");
  sim.check(sim.dut->nack_set_valid_o == 1, "metadata update keeps the set nacked");
  sim.check(sim.dut->nack_set_value_o == 11, "nacked set matches the request set");

  sim.dut->meta_write_ready_i = 1;
  tick_n(sim, 2);
  sim.check(sim.dut->busy_o == 0, "Evict_SW invalid-line path retires after metadata write");
  sim.check(sim.dut->cache_op_valid_byp_o == 0, "Evict_SW to L2 does not forward when the line was invalid");
  sim.check(sim.dut->err_flags_o == 0, "simple Evict_SW path reports no errors");

  return sim.finish();
}

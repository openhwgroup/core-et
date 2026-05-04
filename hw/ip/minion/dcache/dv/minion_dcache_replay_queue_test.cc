// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_replay_queue.

#include "Vminion_dcache_replay_queue_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_dcache_replay_queue_tb;

namespace {

constexpr uint8_t kCmdXrd = 0;
constexpr uint8_t kCmdXwr = 1;

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
  dut->req_cmd_i = kCmdXrd;
  dut->req_typ_i = 0;
  dut->req_gsc_cnt_i = 0;
  dut->req_ps_mask_i = 0;
  dut->req_replay_i = 0;
  dut->req_replay_entry_i = 0;
  dut->req_replay_age_i = 0;
  dut->req_cache_op_i = 0;
  dut->req_buf_alloc_i = 0;
  dut->req_buf_id_i = 0;
  dut->req_cacheable_i = 1;
  dut->req_uc_load_2nd_i = 0;
  dut->req_uc_load_ok_i = 1;
  dut->req_miss_fill_ok_i = 1;
  dut->req_gsc32_idx_i = 0;
  dut->req_vm_sel_i = 0;
  dut->req_mem_global_i = 0;
  dut->cacheable_i = 1;
  dut->req_conf_vpu_rf_write_i = 0;
  dut->req_conf_miss_alloc_i = 0;
  dut->req_conf_miss_accepted_i = 0;
  dut->req_conf_miss_assigned_i = 0;
  dut->misaligned_1st_i = 0;
  dut->uc_load_1st_i = 0;
  dut->uc_load_ok_i = 1;
  dut->miss_fill_ok_i = 1;
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
  dut->vpu_rf_in_use_i = 1;
  dut->replay_grant_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void prep_push_read(DUT* dut, uint64_t addr, uint8_t thread, bool fp, uint8_t reg_addr) {
  dut->req_addr_i = addr;
  dut->req_dest_thread_i = thread;
  dut->req_dest_fp_i = fp;
  dut->req_dest_addr_i = reg_addr;
  dut->req_cmd_i = kCmdXrd;
  dut->req_cacheable_i = 1;
  dut->cacheable_i = 1;
}

void stage_push(SimCtrl<DUT>& sim) {
  sim.dut->push_req_prev_i = 1;
  tick_and_eval(sim);
  sim.dut->push_req_prev_i = 0;
  sim.dut->push_req_i = 1;
  tick_and_eval(sim);
  sim.dut->push_req_i = 0;
}

void stage_rearm(SimCtrl<DUT>& sim, uint8_t entry, uint8_t age) {
  sim.dut->req_replay_i = 1;
  sim.dut->req_replay_entry_i = entry;
  sim.dut->req_replay_age_i = age;
  sim.dut->rearm_entry_i = entry;
  sim.dut->rearm_prev_i = 1;
  tick_and_eval(sim);
  sim.dut->rearm_prev_i = 0;
  sim.dut->rearm_i = 1;
  tick_and_eval(sim);
  sim.dut->rearm_i = 0;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();

  sim.check(sim.dut->full_o == 0, "reset leaves replay queue not full");
  sim.check(sim.dut->half_full_o == 0, "reset leaves replay queue above half available");
  sim.check(sim.dut->empty_o == 0x3, "reset marks both threads empty");
  sim.check(sim.dut->replay_bid_o == 0, "reset has no replay candidate");
  sim.check(sim.dut->scoreboard_valid_o == 0, "reset clears scoreboard");

  prep_push_read(sim.dut.get(), 0x120, 0, false, 5);
  stage_push(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->empty_o == 0x2, "first thread becomes non-empty after push");
  sim.check(sim.dut->replay_bid_o == 1, "pushed read becomes replay candidate");
  sim.check(sim.dut->replay_entry_o == 7, "first push uses highest free replay entry");
  sim.check(sim.dut->replay_req_cmd_o == kCmdXrd, "replay request preserves command");
  sim.check(sim.dut->replay_req_dest_thread_o == 0, "replay request preserves thread");
  sim.check(sim.dut->replay_req_dest_addr_o == 5, "replay request preserves destination register");
  sim.check(sim.dut->replay_req_addr_o == 0x120, "replay request preserves address");
  sim.check(sim.dut->scoreboard_valid_o == 0x80, "scoreboard tracks the pending read entry");

  sim.dut->replay_grant_i = 1;
  tick_and_eval(sim);
  sim.dut->replay_grant_i = 0;
  sim.check(sim.dut->replay_valid_o == 1, "grant marks a replay launch");
  tick_and_eval(sim);
  sim.check(sim.dut->replay_bid_o == 0, "granted entry is no longer pending");
  sim.check(sim.dut->scoreboard_valid_o == 0x80, "scoreboard still tracks allocated entry until dealloc");

  sim.dut->req_conf_vpu_rf_write_i = 1;
  stage_rearm(sim, 7, sim.dut->replay_age_o);
  sim.check(sim.dut->replay_bid_o == 0, "rearmed entry waits while VPU RF is busy");
  sim.dut->req_replay_i = 0;
  sim.dut->vpu_rf_in_use_i = 0;
  tick_and_eval(sim);
  sim.check(sim.dut->replay_bid_o == 1, "VPU RF wakeup releases rearmed entry");

  clear_inputs(sim.dut.get());
  sim.dut->dealloc_i = 1;
  sim.dut->dealloc_entry_i = 7;
  tick_and_eval(sim);
  sim.dut->dealloc_i = 0;
  sim.check(sim.dut->int_sb_dealloc_o == 0x1, "dealloc pulses integer scoreboard release for thread 0");

  clear_inputs(sim.dut.get());
  prep_push_read(sim.dut.get(), 0x220, 1, true, 9);
  sim.dut->req_conf_miss_alloc_i = 1;
  sim.dut->req_conf_miss_assigned_i = 1;
  stage_push(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->replay_bid_o == 0, "miss-assigned entry is blocked before wakeup");
  sim.dut->miss_available_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->replay_bid_o == 1, "miss-available wakeup releases blocked entry");
  sim.check(sim.dut->scoreboard_valid_o == 0x80, "scoreboard tracks the remaining allocated read entry");

  clear_inputs(sim.dut.get());
  sim.dut->dealloc_i = 1;
  sim.dut->dealloc_entry_i = 7;
  tick_and_eval(sim);
  sim.dut->dealloc_i = 0;
  sim.check(sim.dut->fp_sb_dealloc_o == 0x2, "dealloc pulses FP scoreboard release for thread 1");
  tick_and_eval(sim);
  sim.check(sim.dut->empty_o == 0x3, "deallocating both entries empties the queue");

  return sim.finish();
}

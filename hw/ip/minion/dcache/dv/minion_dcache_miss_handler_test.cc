// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_miss_handler.

#include "Vminion_dcache_miss_handler_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_miss_handler_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->neigh_min_id_i = 0;
  dut->req_pri_val_i = 0;
  dut->req_sec_val_i = 0;
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
  dut->inflight0_valid_i = 0;
  dut->inflight0_way_en_i = 0;
  dut->inflight0_set_i = 0;
  dut->inflight1_valid_i = 0;
  dut->inflight1_way_en_i = 0;
  dut->inflight1_set_i = 0;
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

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void drive_base_req(DUT* dut) {
  dut->req_addr_i = 0x800;
  dut->req_set_i = 4;
  dut->req_cmd_i = 0;   // Xrd
  dut->req_typ_i = 2;   // W
  dut->req_buf_id_i = 3;
  dut->req_way_en_i = 0x2;
  dut->req_old_meta_tag_i = 0x12;
  dut->req_thread_id_i = 0;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->idle_o == 1, "reset leaves miss handler idle");
  sim.check(sim.dut->req_pri_rdy_o == 1, "reset leaves primary request ready");
  sim.check(sim.dut->ready_o == 1, "reset reports handler ready for replay");

  drive_base_req(sim.dut.get());
  sim.dut->req_pri_val_i = 1;
  sim.dut->req_cacheable_i = 1;
  sim.dut->req_old_meta_state_i = 1;  // shared, clean victim
  sim.dut->l2_miss_ready_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->req_pri_rdy_o == 1, "clean cacheable miss can be accepted");
  tick_and_eval(sim);
  sim.check(sim.dut->idle_o == 0, "accepted miss leaves idle state");
  sim.check(sim.dut->l2_miss_valid_o == 1, "accepted clean miss issues L2 read");
  sim.check(sim.dut->l2_miss_opcode_o == 1, "miss path uses read opcode");
  sim.check(sim.dut->l2_miss_size_o == 6, "cacheable miss requests a full line");
  sim.dut->req_pri_val_i = 0;
  tick_and_eval(sim);

  sim.dut->l2_miss_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 2;       // miss handler 0
  sim.dut->l2_resp_opcode_i = 1;   // AckData
  sim.dut->l2_resp_slot_avail_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->ready_o == 0, "first fill chunk is not replay-ready yet");
  tick_and_eval(sim);
  sim.dut->l2_resp_slot_avail_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->ready_o == 1, "last fill chunk with available slot is replay-ready");
  tick_and_eval(sim);
  sim.check(sim.dut->meta_write_valid_o == 1, "last fill chunk triggers tag write");
  sim.check(sim.dut->meta_write_state_o == 2, "read miss refills to exclusive state");
  sim.dut->l2_resp_valid_i = 0;
  sim.dut->meta_write_ready_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->meta_write_valid_o == 0, "metadata write handshake clears valid");
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->idle_o == 1, "clean miss retires back to idle");

  reset_and_clear(sim);
  drive_base_req(sim.dut.get());
  sim.dut->req_pri_val_i = 1;
  sim.dut->req_cacheable_i = 1;
  sim.dut->req_old_meta_state_i = 3;  // modified, dirty victim
  sim.dut->writeback_ready_i = 1;
  sim.dut->eval();
  tick_and_eval(sim);
  sim.check(sim.dut->writeback_valid_o == 1, "dirty miss first acquires writeback");
  sim.check(sim.dut->writeback_set_o == 4, "writeback carries victim set");
  sim.check(sim.dut->writeback_way_en_o == 0x2, "writeback carries victim way");

  reset_and_clear(sim);
  drive_base_req(sim.dut.get());
  sim.dut->req_pri_val_i = 1;
  sim.dut->req_cmd_i = 1;  // Xwr
  sim.dut->req_cacheable_i = 0;
  sim.dut->req_old_meta_state_i = 0;
  sim.dut->l2_evict_ready_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->can_accept_uc_o == 1, "uncacheable store is accepted as UC");
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->going_to_uc_store_o == 1, "UC store enters store path");
  tick_and_eval(sim);
  sim.check(sim.dut->l2_evict_valid_o == 1, "UC store issues evict/write request");
  sim.check(sim.dut->l2_evict_opcode_o == 0, "UC store uses write opcode");
  sim.check(sim.dut->force_ba_rptr_qual_o == 1, "UC store qualifies buffer pointer when accepted");
  sim.dut->req_pri_val_i = 0;
  tick_and_eval(sim);
  sim.dut->l2_evict_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 10;      // UC store handler 0
  sim.dut->l2_resp_opcode_i = 0;   // Ack
  tick_and_eval(sim);
  sim.check(sim.dut->idle_o == 1, "UC store acks retire the handler");

  reset_and_clear(sim);
  drive_base_req(sim.dut.get());
  sim.dut->req_pri_val_i = 1;
  sim.dut->req_cacheable_i = 0;
  sim.dut->req_old_meta_state_i = 0;
  sim.dut->l2_miss_ready_i = 1;
  sim.dut->eval();
  tick_and_eval(sim);
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->wait_l2_to_ba_resp_o == 1, "UC load waits for fill-to-buffer response");
  sim.dut->req_pri_val_i = 0;
  sim.dut->l2_miss_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 8;       // UC load handler 0
  sim.dut->l2_resp_opcode_i = 3;   // Err
  sim.dut->eval();
  sim.check(sim.dut->uc_load_bus_error_o == 1, "UC load error raises UC load bus error");
  sim.check(sim.dut->bus_err_o == 1, "UC load error reports thread-local bus error");
  sim.check(sim.dut->bus_err_addr_o == 0x800, "bus error reports request address");
  tick_and_eval(sim);

  return sim.finish();
}

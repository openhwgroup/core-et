// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_miss_handler_unit.

#include "Vminion_dcache_miss_handler_unit_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_miss_handler_unit_tb;

namespace {

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
  dut->req_cmd_i = 0;
  dut->req_typ_i = 2;
  dut->req_buf_id_i = 3;
  dut->req_way_en_i = 0x2;
  dut->req_old_meta_tag_i = 0x12;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->idle_o == 1, "reset leaves miss-handler unit idle");
  sim.check(sim.dut->req_ready_o == 1, "reset exposes a ready handler");
  sim.check(sim.dut->req_assigned_o == 0x1, "reset keeps handler 0 reserved for thread-0 UC requests");

  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cacheable_i = 1;
  sim.dut->req_old_meta_state_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->req_ready_o == 1, "clean miss can allocate a handler");
  sim.check(sim.dut->req_accepted_o == 0x2, "primary-miss allocation prefers handler 1");
  sim.check(sim.dut->req_assigned_o == 0x2, "primary miss is assigned to handler 1");
  tick_and_eval(sim);

  sim.dut->req_valid_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->l2_miss_valid_o == 1, "first miss issues an outer-memory read");
  sim.check(sim.dut->l2_miss_id_o == 3, "handler 1 uses miss-handler-1 request id");

  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_addr_i = 0x900;
  sim.dut->req_set_i = 5;
  sim.dut->req_way_en_i = 0x1;
  sim.dut->req_buf_id_i = 1;
  sim.dut->req_thread_id_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->req_accepted_o == 0x1, "second primary miss allocates handler 0");
  sim.check(sim.dut->req_assigned_o == 0x1, "second primary miss is assigned to handler 0");
  tick_and_eval(sim);

  sim.dut->req_valid_i = 0;
  sim.dut->l2_miss_ready_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->l2_miss_valid_o == 1, "two active misses still drive a miss request");
  sim.check(sim.dut->l2_miss_id_o == 2, "miss arbiter first grants handler 0");
  sim.tick();
  sim.dut->eval();
  sim.check(sim.dut->l2_miss_id_o == 3, "miss arbiter rotates to handler 1 on the next grant");

  reset_and_clear(sim);
  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cmd_i = 1;
  sim.dut->req_cacheable_i = 0;
  sim.dut->req_thread_id_i = 0;
  sim.dut->l2_evict_ready_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->can_accept_uc_o == 1, "thread-0 UC store is accepted");
  sim.check(sim.dut->req_accepted_o == 0x1, "thread-0 UC store is routed to handler 0");
  tick_and_eval(sim);
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->going_to_uc_store_o == 1, "UC store drives the shared store indication");
  sim.check(sim.dut->force_ba_rptr_o == 1, "UC store latches the buffer-array read override");
  sim.check(sim.dut->ba_rptr_o == 3, "UC store reports the selected buffer entry");
  sim.check(sim.dut->l2_evict_valid_o == 1, "UC store arbitrates onto the evict bus");
  sim.check(sim.dut->l2_evict_id_o == 10, "handler 0 UC store uses the UC-store request id");
  tick_and_eval(sim);

  sim.dut->req_valid_i = 0;
  sim.dut->l2_evict_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 10;
  sim.dut->l2_resp_opcode_i = 0;
  tick_and_eval(sim);
  sim.dut->l2_resp_valid_i = 0;
  tick_and_eval(sim);
  sim.check(sim.dut->idle_o == 1, "UC store ack retires the unit");

  reset_and_clear(sim);
  drive_base_req(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_cacheable_i = 0;
  sim.dut->req_thread_id_i = 1;
  sim.dut->req_buf_id_i = 5;
  sim.dut->l2_miss_ready_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->req_accepted_o == 0x2, "thread-1 UC load is routed to handler 1");
  tick_and_eval(sim);
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->wait_l2_to_ba_resp_o == 1, "UC load advertises pending buffer-array fill");
  sim.dut->req_valid_i = 0;
  sim.dut->l2_miss_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 9;
  sim.dut->l2_resp_opcode_i = 3;
  sim.dut->eval();
  sim.check(sim.dut->ba_wptr_o == 5, "UC load response selects the matching buffer-array write pointer");
  sim.check(sim.dut->handler_uc_load_bus_error_o == 0x2, "UC load error is reported by handler 1");
  sim.check(sim.dut->bus_err_o == 0x2, "UC load error merges into the thread-1 bus error bit");
  sim.check(sim.dut->bus_err_addr_o == 0x800, "merged bus error reports the failing request address");

  return sim.finish();
}

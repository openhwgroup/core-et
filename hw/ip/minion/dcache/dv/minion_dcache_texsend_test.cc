// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_texsend.

#include "Vminion_dcache_texsend_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_dcache_texsend_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->neigh_min_id_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->tex_send_valid_i = 0;
  dut->tex_send_ctrl_i = 0;
  dut->l2_resp_valid_i = 0;
  for (int i = 0; i < 9; ++i) dut->l2_resp_i[i] = 0;
  dut->l2_req_ready_i = 0;
  dut->writeback_ready_i = 0;
  dut->writeback_done_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();

  sim.check(sim.dut->idle_o == 1, "texsend stub is always idle");
  sim.check(sim.dut->tex_send_ready_o == 1, "texsend stub is always ready");
  sim.check(sim.dut->l2_req_valid_o == 0, "texsend stub never emits an L2 request");
  sim.check(sim.dut->writeback_valid_o == 0, "texsend stub never emits a writeback request");
  sim.check(sim.dut->csr_debug_signals_o == 0, "texsend stub clears debug signals");

  sim.dut->neigh_min_id_i = 7;
  sim.dut->chicken_bit_dcache_i = 1;
  sim.dut->tex_send_valid_i = 1;
  sim.dut->tex_send_ctrl_i = 0xFFFF;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_i[0] = 0xFFFFFFFFu;
  sim.dut->l2_req_ready_i = 1;
  sim.dut->writeback_ready_i = 1;
  sim.dut->writeback_done_i = 1;
  tick_and_eval(sim);

  sim.check(sim.dut->idle_o == 1, "stub ignores active stimulus and stays idle");
  sim.check(sim.dut->tex_send_ready_o == 1, "stub ignores active stimulus and stays ready");
  sim.check(sim.dut->l2_req_valid_o == 0, "stub suppresses all L2 requests");
  sim.check(sim.dut->writeback_valid_o == 0, "stub suppresses all writeback requests");
  sim.check(sim.dut->l2_req_o[0] == 0, "stub drives zero L2 request bundle");
  sim.check(sim.dut->writeback_req_o == 0, "stub drives zero writeback bundle");

  return sim.finish();
}

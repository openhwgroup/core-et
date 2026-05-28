// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_int_scoreboard_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vintpipe_int_scoreboard_tb;

namespace {

void clear_inputs(DUT* d) {
  d->rd_thread_id_i = 0;
  d->rd_addr0_i = 0;
  d->rd_addr1_i = 0;
  d->rd_addr2_i = 0;
  d->dcache_valid_i = 0;
  d->dcache_fp_i = 0;
  d->dcache_thread_i = 0;
  d->dcache_addr0_i = 0;
  d->dcache_addr1_i = 0;
  d->dcache_addr2_i = 0;
  d->dcache_addr3_i = 0;
  d->dcache_addr4_i = 0;
  d->dcache_addr5_i = 0;
  d->dcache_addr6_i = 0;
  d->dcache_addr7_i = 0;
  d->int_div_valid_i = 0;
  d->int_div_fp_i = 0;
  d->int_div_addr_i = 0;
  d->int_div_thread_i = 0;
  d->int_flb_valid_i = 0;
  d->int_flb_fp_i = 0;
  d->int_flb_addr_i = 0;
  d->int_flb_thread_i = 0;
  d->vpu_toint_valid_i = 0;
  d->vpu_toint_fp_i = 0;
  d->vpu_toint_thread_i = 0;
  d->vpu_toint_addr0_i = 0;
  d->vpu_toint_addr1_i = 0;
  d->vpu_toint_addr2_i = 0;
  d->vpu_toint_addr3_i = 0;
}

void check_outputs(SimCtrl<DUT>& sim, const char* name,
                   uint8_t exp_rd_data, uint8_t exp_dcache_div, bool exp_x31) {
  sim.dut->eval();
  char msg[192];
  std::snprintf(msg, sizeof(msg), "%s rd_data got=0x%x exp=0x%x",
                name, sim.dut->rd_data_o, exp_rd_data);
  sim.check(sim.dut->rd_data_o == exp_rd_data, msg);
  std::snprintf(msg, sizeof(msg), "%s rd_data_dcache_div got=0x%x exp=0x%x",
                name, sim.dut->rd_data_dcache_div_o, exp_dcache_div);
  sim.check(sim.dut->rd_data_dcache_div_o == exp_dcache_div, msg);
  std::snprintf(msg, sizeof(msg), "%s rd_x31 got=%u exp=%u",
                name, sim.dut->rd_x31_o, exp_x31);
  sim.check(sim.dut->rd_x31_o == exp_x31, msg);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset(2);

  clear_inputs(sim.dut.get());
  sim.dut->rd_addr0_i = 5;
  sim.dut->rd_addr1_i = 10;
  sim.dut->rd_addr2_i = 15;
  check_outputs(sim, "reset_no_hazard", 0x0, 0x0, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 5;
  sim.dut->rd_addr1_i = 10;
  sim.dut->rd_addr2_i = 15;
  sim.dut->dcache_valid_i = 1u << 2;
  sim.dut->dcache_addr2_i = 5;
  sim.dut->dcache_thread_i = 0;
  sim.dut->dcache_fp_i = 0;
  check_outputs(sim, "dcache_replay_hit_port0", 0x1, 0x1, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 5;
  sim.dut->rd_addr1_i = 10;
  sim.dut->rd_addr2_i = 15;
  sim.dut->int_div_valid_i = 1;
  sim.dut->int_div_addr_i = 10;
  sim.dut->int_div_thread_i = 1;
  check_outputs(sim, "divider_hit_port1", 0x2, 0x2, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 5;
  sim.dut->rd_addr1_i = 10;
  sim.dut->rd_addr2_i = 15;
  sim.dut->int_flb_valid_i = 1;
  sim.dut->int_flb_addr_i = 15;
  sim.dut->int_flb_thread_i = 1;
  check_outputs(sim, "fast_local_barrier_hit_port2", 0x4, 0x0, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 3;
  sim.dut->rd_addr1_i = 4;
  sim.dut->rd_addr2_i = 5;
  sim.dut->vpu_toint_valid_i = 1u << 1;
  sim.dut->vpu_toint_addr1_i = 3;
  sim.dut->vpu_toint_thread_i = 0;
  check_outputs(sim, "vpu_toint_hit_port0", 0x1, 0x0, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 5;
  sim.dut->int_div_valid_i = 1;
  sim.dut->int_div_addr_i = 5;
  sim.dut->int_div_thread_i = 0;
  check_outputs(sim, "thread_mismatch_misses", 0x0, 0x0, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 5;
  sim.dut->dcache_valid_i = 1u << 0;
  sim.dut->dcache_addr0_i = 5;
  sim.dut->dcache_fp_i = 1u << 0;
  check_outputs(sim, "fp_mismatch_misses", 0x0, 0x0, false);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 1;
  sim.dut->rd_addr1_i = 2;
  sim.dut->rd_addr2_i = 3;
  sim.dut->int_div_valid_i = 1;
  sim.dut->int_div_addr_i = 31;
  sim.dut->int_div_thread_i = 0;
  check_outputs(sim, "rd_x31_divider", 0x0, 0x0, true);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 6;
  sim.dut->rd_addr1_i = 7;
  sim.dut->rd_addr2_i = 8;
  sim.dut->dcache_valid_i = 1u << 4;
  sim.dut->dcache_addr4_i = 6;
  sim.dut->dcache_thread_i = 1u << 4;
  sim.dut->int_div_valid_i = 1;
  sim.dut->int_div_addr_i = 7;
  sim.dut->int_div_thread_i = 1;
  sim.dut->int_flb_valid_i = 1;
  sim.dut->int_flb_addr_i = 8;
  sim.dut->int_flb_thread_i = 1;
  sim.dut->vpu_toint_valid_i = 1u << 2;
  sim.dut->vpu_toint_addr2_i = 8;
  sim.dut->vpu_toint_thread_i = 1u << 2;
  check_outputs(sim, "multiple_sources_all_ports", 0x7, 0x3, false);

  return sim.finish();
}

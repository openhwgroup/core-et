// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_fp_scoreboard_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vintpipe_fp_scoreboard_tb;

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
  d->vpu_fp_valid_i = 0;
  d->vpu_fp_fp_i = 0;
  d->vpu_fp_thread_i = 0;
  d->vpu_fp_addr0_i = 0;
  d->vpu_fp_addr1_i = 0;
  d->vpu_fp_addr2_i = 0;
  d->vpu_fp_addr3_i = 0;
  d->vpu_fp_addr4_i = 0;
  d->vpu_fp_addr5_i = 0;
  d->vpu_fp_addr6_i = 0;
  d->vpu_fp_addr7_i = 0;
  d->vpu_fp_addr8_i = 0;
  d->vpu_fp_addr9_i = 0;
  d->vpu_fp_addr10_i = 0;
  d->vpu_fp_addr11_i = 0;
  d->vpu_fp_addr12_i = 0;
  d->vpu_fp_addr13_i = 0;
}

void check_outputs(SimCtrl<DUT>& sim, const char* name,
                   uint8_t exp_rd_data, uint8_t exp_dcache) {
  sim.dut->eval();
  char msg[192];
  std::snprintf(msg, sizeof(msg), "%s rd_data got=0x%x exp=0x%x",
                name, sim.dut->rd_data_o, exp_rd_data);
  sim.check(sim.dut->rd_data_o == exp_rd_data, msg);
  std::snprintf(msg, sizeof(msg), "%s rd_data_dcache got=0x%x exp=0x%x",
                name, sim.dut->rd_data_dcache_o, exp_dcache);
  sim.check(sim.dut->rd_data_dcache_o == exp_dcache, msg);
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
  check_outputs(sim, "reset_no_hazard", 0x0, 0x0);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 5;
  sim.dut->rd_addr1_i = 10;
  sim.dut->rd_addr2_i = 15;
  sim.dut->dcache_valid_i = 1u << 1;
  sim.dut->dcache_fp_i = 1u << 1;
  sim.dut->dcache_addr1_i = 5;
  sim.dut->dcache_thread_i = 0;
  check_outputs(sim, "dcache_fp_hit_port0", 0x1, 0x1);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 9;
  sim.dut->rd_addr1_i = 11;
  sim.dut->rd_addr2_i = 13;
  sim.dut->vpu_fp_valid_i = 1u << 10;
  sim.dut->vpu_fp_fp_i = 1u << 10;
  sim.dut->vpu_fp_thread_i = 1u << 10;
  sim.dut->vpu_fp_addr10_i = 11;
  check_outputs(sim, "vpu_fp_hit_port1", 0x2, 0x0);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 5;
  sim.dut->dcache_valid_i = 1u << 0;
  sim.dut->dcache_addr0_i = 5;
  sim.dut->dcache_fp_i = 0;
  check_outputs(sim, "dcache_int_dest_misses_fp_reads", 0x0, 0x0);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 5;
  sim.dut->vpu_fp_valid_i = 1u << 3;
  sim.dut->vpu_fp_fp_i = 1u << 3;
  sim.dut->vpu_fp_addr3_i = 5;
  sim.dut->vpu_fp_thread_i = 0;
  check_outputs(sim, "vpu_thread_mismatch", 0x0, 0x0);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr0_i = 5;
  sim.dut->dcache_valid_i = 1u << 7;
  sim.dut->dcache_fp_i = 1u << 7;
  sim.dut->dcache_addr7_i = 6;
  sim.dut->dcache_thread_i = 0;
  check_outputs(sim, "dcache_address_mismatch", 0x0, 0x0);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr0_i = 6;
  sim.dut->rd_addr1_i = 7;
  sim.dut->rd_addr2_i = 8;
  sim.dut->dcache_valid_i = (1u << 4) | (1u << 5);
  sim.dut->dcache_fp_i = (1u << 4) | (1u << 5);
  sim.dut->dcache_thread_i = (1u << 4) | (1u << 5);
  sim.dut->dcache_addr4_i = 6;
  sim.dut->dcache_addr5_i = 8;
  sim.dut->vpu_fp_valid_i = 1u << 13;
  sim.dut->vpu_fp_fp_i = 1u << 13;
  sim.dut->vpu_fp_thread_i = 1u << 13;
  sim.dut->vpu_fp_addr13_i = 7;
  check_outputs(sim, "multiple_fp_sources", 0x7, 0x5);

  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_mask_scoreboard_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vintpipe_mask_scoreboard_tb;

namespace {

void clear_inputs(DUT* d) {
  d->rd_thread_id_i = 0;
  d->rd_addr_i = 0;
  d->wd_addr_i = 0;
  d->vpu_mask_valid_i = 0;
  d->vpu_mask_thread_i = 0;
  d->vpu_mask_addr0_i = 0;
  d->vpu_mask_addr1_i = 0;
  d->vpu_mask_addr2_i = 0;
  d->vpu_mask_addr3_i = 0;
}

void check_outputs(SimCtrl<DUT>& sim, const char* name,
                   uint8_t exp_rd, uint8_t exp_wd) {
  sim.dut->eval();
  char msg[160];
  std::snprintf(msg, sizeof(msg), "%s rd_data got=0x%02x exp=0x%02x",
                name, sim.dut->rd_data_o, exp_rd);
  sim.check(sim.dut->rd_data_o == exp_rd, msg);
  std::snprintf(msg, sizeof(msg), "%s wd_data got=0x%02x exp=0x%02x",
                name, sim.dut->wd_data_o, exp_wd);
  sim.check(sim.dut->wd_data_o == exp_wd, msg);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset(2);

  clear_inputs(sim.dut.get());
  sim.dut->rd_addr_i = 0xff;
  sim.dut->wd_addr_i = 0xff;
  check_outputs(sim, "reset_no_hazard", 0x00, 0x00);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr_i = 1u << 3;
  sim.dut->vpu_mask_valid_i = 1u << 0;
  sim.dut->vpu_mask_addr0_i = 3;
  sim.dut->vpu_mask_thread_i = 0;
  check_outputs(sim, "read_mask_hazard", 0x08, 0x00);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->wd_addr_i = 1u << 5;
  sim.dut->vpu_mask_valid_i = 1u << 1;
  sim.dut->vpu_mask_addr1_i = 5;
  sim.dut->vpu_mask_thread_i = 1u << 1;
  check_outputs(sim, "write_mask_hazard", 0x00, 0x20);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr_i = (1u << 1) | (1u << 6);
  sim.dut->wd_addr_i = (1u << 6);
  sim.dut->vpu_mask_valid_i = (1u << 2) | (1u << 3);
  sim.dut->vpu_mask_addr2_i = 1;
  sim.dut->vpu_mask_addr3_i = 6;
  sim.dut->vpu_mask_thread_i = (1u << 2) | (1u << 3);
  check_outputs(sim, "multiple_mask_destinations", 0x42, 0x40);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 1;
  sim.dut->rd_addr_i = 1u << 2;
  sim.dut->wd_addr_i = 1u << 2;
  sim.dut->vpu_mask_valid_i = 1u << 0;
  sim.dut->vpu_mask_addr0_i = 2;
  sim.dut->vpu_mask_thread_i = 0;
  check_outputs(sim, "thread_mismatch", 0x00, 0x00);

  clear_inputs(sim.dut.get());
  sim.dut->rd_thread_id_i = 0;
  sim.dut->rd_addr_i = 1u << 7;
  sim.dut->wd_addr_i = 1u << 7;
  sim.dut->vpu_mask_valid_i = 0;
  sim.dut->vpu_mask_addr0_i = 7;
  check_outputs(sim, "invalid_entry_ignored", 0x00, 0x00);

  return sim.finish();
}

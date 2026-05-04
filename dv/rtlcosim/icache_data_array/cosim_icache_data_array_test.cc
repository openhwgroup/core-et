// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_data_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_icache_data_array_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->f1_read_req_valid_i = 0;
  dut->f1_read_req_way_i = 0;
  dut->f1_read_req_set_i = 0;
  dut->f1_write_req_valid_i = 0;
  dut->f1_write_req_way_i = 0;
  dut->f1_write_req_set_i = 0;
  dut->f2_mem_req_ready_i = 0;
  dut->f0_mem_resp_valid_i = 0;
  dut->f0_mem_resp_mem0_lo_i = 0;
  dut->f0_mem_resp_mem0_hi_i = 0;
  dut->f0_mem_resp_mem0_ecc_i = 0;
  dut->f0_mem_resp_mem1_lo_i = 0;
  dut->f0_mem_resp_mem1_hi_i = 0;
  dut->f0_mem_resp_mem1_ecc_i = 0;
  dut->f0_mem_resp_mem2_lo_i = 0;
  dut->f0_mem_resp_mem2_hi_i = 0;
  dut->f0_mem_resp_mem2_ecc_i = 0;
  dut->f0_mem_resp_mem3_lo_i = 0;
  dut->f0_mem_resp_mem3_hi_i = 0;
  dut->f0_mem_resp_mem3_ecc_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("f0_read_resp_early_valid", dut->orig_f0_read_resp_early_valid_o,
              dut->new_f0_read_resp_early_valid_o);
  sim.compare("f0_read_resp_valid", dut->orig_f0_read_resp_valid_o,
              dut->new_f0_read_resp_valid_o);
  sim.compare("f0_read_resp_qw0", dut->orig_f0_read_resp_qw0_o, dut->new_f0_read_resp_qw0_o);
  sim.compare("f0_read_resp_qw1", dut->orig_f0_read_resp_qw1_o, dut->new_f0_read_resp_qw1_o);
  sim.compare("f0_read_resp_qw2", dut->orig_f0_read_resp_qw2_o, dut->new_f0_read_resp_qw2_o);
  sim.compare("f0_read_resp_qw3", dut->orig_f0_read_resp_qw3_o, dut->new_f0_read_resp_qw3_o);
  sim.compare("f0_read_resp_qw4", dut->orig_f0_read_resp_qw4_o, dut->new_f0_read_resp_qw4_o);
  sim.compare("f0_read_resp_qw5", dut->orig_f0_read_resp_qw5_o, dut->new_f0_read_resp_qw5_o);
  sim.compare("f0_read_resp_qw6", dut->orig_f0_read_resp_qw6_o, dut->new_f0_read_resp_qw6_o);
  sim.compare("f0_read_resp_qw7", dut->orig_f0_read_resp_qw7_o, dut->new_f0_read_resp_qw7_o);
  sim.compare("f0_read_resp_sbe_per_block", dut->orig_f0_read_resp_sbe_per_block_o,
              dut->new_f0_read_resp_sbe_per_block_o);
  sim.compare("f0_read_resp_dbe_per_block", dut->orig_f0_read_resp_dbe_per_block_o,
              dut->new_f0_read_resp_dbe_per_block_o);
  sim.compare("f2_mem_req_write", dut->orig_f2_mem_req_write_o, dut->new_f2_mem_req_write_o);
  sim.compare("f2_mem_req_addr", dut->orig_f2_mem_req_addr_o, dut->new_f2_mem_req_addr_o);
  sim.compare("f2_mem_req_valid", dut->orig_f2_mem_req_valid_o, dut->new_f2_mem_req_valid_o);
  sim.compare("f0_mem_resp_ready", dut->orig_f0_mem_resp_ready_o, dut->new_f0_mem_resp_ready_o);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();
  std::mt19937_64 rng(0x1ca11ca5ULL);

  clear_inputs(dut);
  sim.reset(2);

  for (int cycle = 0; cycle < 32; ++cycle) {
    dut->f1_read_req_valid_i = (cycle & 1) != 0;
    dut->f1_read_req_way_i = cycle & 0x3;
    dut->f1_read_req_set_i = (cycle * 9) & 0x7f;
    dut->f1_write_req_valid_i = (cycle & 2) != 0;
    dut->f1_write_req_way_i = (cycle + 1) & 0x3;
    dut->f1_write_req_set_i = (cycle * 13) & 0x7f;
    dut->f2_mem_req_ready_i = (cycle & 4) != 0;
    dut->f0_mem_resp_valid_i = (cycle & 8) != 0;
    dut->f0_mem_resp_mem0_lo_i = cycle;
    dut->f0_mem_resp_mem0_hi_i = ~static_cast<uint64_t>(cycle);
    dut->f0_mem_resp_mem0_ecc_i = cycle;
    dut->f0_mem_resp_mem1_lo_i = cycle + 1;
    dut->f0_mem_resp_mem1_hi_i = ~static_cast<uint64_t>(cycle + 1);
    dut->f0_mem_resp_mem1_ecc_i = cycle + 1;
    dut->f0_mem_resp_mem2_lo_i = cycle + 2;
    dut->f0_mem_resp_mem2_hi_i = ~static_cast<uint64_t>(cycle + 2);
    dut->f0_mem_resp_mem2_ecc_i = cycle + 2;
    dut->f0_mem_resp_mem3_lo_i = cycle + 3;
    dut->f0_mem_resp_mem3_hi_i = ~static_cast<uint64_t>(cycle + 3);
    dut->f0_mem_resp_mem3_ecc_i = cycle + 3;
    sim.tick();
    compare_outputs(sim);
  }

  for (int cycle = 0; cycle < 5000; ++cycle) {
    dut->f1_read_req_valid_i = rng() & 1U;
    dut->f1_read_req_way_i = rng() & 0x3U;
    dut->f1_read_req_set_i = rng() & 0x7fU;
    dut->f1_write_req_valid_i = rng() & 1U;
    dut->f1_write_req_way_i = rng() & 0x3U;
    dut->f1_write_req_set_i = rng() & 0x7fU;
    dut->f2_mem_req_ready_i = rng() & 1U;
    dut->f0_mem_resp_valid_i = rng() & 1U;
    dut->f0_mem_resp_mem0_lo_i = rng();
    dut->f0_mem_resp_mem0_hi_i = rng();
    dut->f0_mem_resp_mem0_ecc_i = rng() & 0xffffU;
    dut->f0_mem_resp_mem1_lo_i = rng();
    dut->f0_mem_resp_mem1_hi_i = rng();
    dut->f0_mem_resp_mem1_ecc_i = rng() & 0xffffU;
    dut->f0_mem_resp_mem2_lo_i = rng();
    dut->f0_mem_resp_mem2_hi_i = rng();
    dut->f0_mem_resp_mem2_ecc_i = rng() & 0xffffU;
    dut->f0_mem_resp_mem3_lo_i = rng();
    dut->f0_mem_resp_mem3_hi_i = rng();
    dut->f0_mem_resp_mem3_ecc_i = rng() & 0xffffU;
    sim.tick();
    compare_outputs(sim);
  }

  return sim.finish();
}

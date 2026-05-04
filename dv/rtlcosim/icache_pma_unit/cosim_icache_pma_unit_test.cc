// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_pma_unit_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_icache_pma_unit_tb;

static uint32_t xorshift32(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.dut->clk_i = 0;
  sim.dut->rst_ni = 1;
  sim.dut->ioshire_i = 0;
  sim.dut->paddr_i = 0;
  sim.dut->mprot_i = 0;
  sim.dut->ptw_prv_i = 0;
  sim.tick();
  sim.compare("access_fault", sim.dut->orig_access_fault_o, sim.dut->new_access_fault_o);
  sim.compare("cacheable", sim.dut->orig_cacheable_o, sim.dut->new_cacheable_o);

  uint32_t seed = 0x1234cafeu;
  for (int i = 0; i < 6000; i++) {
    sim.dut->ioshire_i = xorshift32(seed) & 1u;
    sim.dut->paddr_i = ((uint64_t(xorshift32(seed)) << 20) ^ uint64_t(xorshift32(seed))) & ((1ULL << 40) - 1);
    sim.dut->mprot_i = xorshift32(seed) & 0x7fu;
    sim.dut->ptw_prv_i = xorshift32(seed) & 0x3u;
    sim.tick();
    sim.compare("access_fault", sim.dut->orig_access_fault_o, sim.dut->new_access_fault_o);
    sim.compare("cacheable", sim.dut->orig_cacheable_o, sim.dut->new_cacheable_o);
  }

  return sim.finish();
}

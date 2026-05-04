// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_texsend vs minion_dcache_texsend.

#include "Vcosim_minion_dcache_texsend_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_texsend_tb;

namespace {

struct BoolCoverage {
  bool seen0 = false;
  bool seen1 = false;
  void sample(bool v) {
    if (v) seen1 = true;
    else seen0 = true;
  }
  bool full() const { return seen0 && seen1; }
};

struct Coverage {
  BoolCoverage chicken_bit_dcache, tex_send_valid, l2_resp_valid;
  BoolCoverage l2_req_ready, writeback_ready, writeback_done;
  bool neigh_min_zero = false, neigh_min_nonzero = false;
  bool tex_send_ctrl_zero = false, tex_send_ctrl_nonzero = false;
  bool l2_resp_zero = false, l2_resp_nonzero = false;
};

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

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.chicken_bit_dcache.sample(dut->chicken_bit_dcache_i);
  cov.tex_send_valid.sample(dut->tex_send_valid_i);
  cov.l2_resp_valid.sample(dut->l2_resp_valid_i);
  cov.l2_req_ready.sample(dut->l2_req_ready_i);
  cov.writeback_ready.sample(dut->writeback_ready_i);
  cov.writeback_done.sample(dut->writeback_done_i);
  if (dut->neigh_min_id_i == 0) cov.neigh_min_zero = true;
  else cov.neigh_min_nonzero = true;
  if (dut->tex_send_ctrl_i == 0) cov.tex_send_ctrl_zero = true;
  else cov.tex_send_ctrl_nonzero = true;
  bool l2_resp_zero = true;
  for (int i = 0; i < 9; ++i) l2_resp_zero &= (dut->l2_resp_i[i] == 0);
  if (l2_resp_zero) cov.l2_resp_zero = true;
  else cov.l2_resp_nonzero = true;
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.chicken_bit_dcache.full(), "coverage: chicken_bit_dcache exercised 0/1");
  sim.check(cov.tex_send_valid.full(), "coverage: tex_send_valid exercised 0/1");
  sim.check(cov.l2_resp_valid.full(), "coverage: l2_resp_valid exercised 0/1");
  sim.check(cov.l2_req_ready.full(), "coverage: l2_req_ready exercised 0/1");
  sim.check(cov.writeback_ready.full(), "coverage: writeback_ready exercised 0/1");
  sim.check(cov.writeback_done.full(), "coverage: writeback_done exercised 0/1");
  sim.check(cov.neigh_min_zero && cov.neigh_min_nonzero, "coverage: neigh_min_id exercised zero/nonzero");
  sim.check(cov.tex_send_ctrl_zero && cov.tex_send_ctrl_nonzero,
            "coverage: tex_send_ctrl exercised zero/nonzero");
  sim.check(cov.l2_resp_zero && cov.l2_resp_nonzero, "coverage: l2_resp exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("idle", sim.dut->orig_idle_o, sim.dut->new_idle_o);
  sim.compare("tex_send_ready", sim.dut->orig_tex_send_ready_o, sim.dut->new_tex_send_ready_o);
  sim.compare("l2_req_valid", sim.dut->orig_l2_req_valid_o, sim.dut->new_l2_req_valid_o);
  for (int i = 0; i < 10; ++i) {
    sim.compare("l2_req[" + std::to_string(i) + "]",
                sim.dut->orig_l2_req_o[i], sim.dut->new_l2_req_o[i]);
  }
  sim.compare("writeback_valid", sim.dut->orig_writeback_valid_o, sim.dut->new_writeback_valid_o);
  sim.compare("writeback_req", sim.dut->orig_writeback_req_o, sim.dut->new_writeback_req_o);
  sim.compare("csr_debug_signals", sim.dut->orig_csr_debug_signals_o, sim.dut->new_csr_debug_signals_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
  if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
  compare_outputs(sim);
}

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  Coverage coverage;

  clear_inputs(sim.dut.get());
  sample_coverage(sim.dut.get(), coverage);
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_outputs(sim);

  sim.dut->neigh_min_id_i = 7;
  sim.dut->chicken_bit_dcache_i = 1;
  sim.dut->tex_send_valid_i = 1;
  sim.dut->tex_send_ctrl_i = 0xFFFFu;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_i[0] = 0xFFFFFFFFu;
  sim.dut->l2_req_ready_i = 1;
  sim.dut->writeback_ready_i = 1;
  sim.dut->writeback_done_i = 1;
  tick_and_compare(sim, &coverage);

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < 512; ++cycle) {
      sim.dut->neigh_min_id_i = xorshift32(seed) & 0x7u;
      sim.dut->chicken_bit_dcache_i = xorshift32(seed) & 1u;
      sim.dut->tex_send_valid_i = xorshift32(seed) & 1u;
      sim.dut->tex_send_ctrl_i = xorshift32(seed);
      sim.dut->l2_resp_valid_i = xorshift32(seed) & 1u;
      for (int i = 0; i < 9; ++i) sim.dut->l2_resp_i[i] = xorshift32(seed);
      sim.dut->l2_req_ready_i = xorshift32(seed) & 1u;
      sim.dut->writeback_ready_i = xorshift32(seed) & 1u;
      sim.dut->writeback_done_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

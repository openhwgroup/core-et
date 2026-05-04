// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_writeback_unit vs minion_dcache_writeback_unit.

#include "Vcosim_minion_dcache_writeback_unit_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_writeback_unit_tb;
using U256 = std::array<uint64_t, 4>;

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

struct NonZeroCoverage {
  bool seen_zero = false;
  bool seen_nonzero = false;
  void sample(uint64_t v) {
    if (v == 0) seen_zero = true;
    else seen_nonzero = true;
  }
  bool full() const { return seen_zero && seen_nonzero; }
};

struct Coverage {
  BoolCoverage req_valid, req_is_tbox, req_mem_idx, data_req_grant, data_req_nack, l2_req_ready;
  NonZeroCoverage req_tag, req_set, req_way_en, req_addr_low;
  NonZeroCoverage data_resp_bank0, data_resp_bank1, data_resp_bank2, data_resp_bank3;
};

void clear_inputs(DUT* dut) {
  dut->req_valid_i = 0;
  dut->req_tag_i = 0;
  dut->req_set_i = 0;
  dut->req_way_en_i = 0;
  dut->req_is_tbox_i = 0;
  dut->req_mem_idx_i = 0;
  dut->req_addr_low_i = 0;
  dut->data_req_grant_i = 0;
  dut->data_req_nack_i = 0;
  dut->data_resp_bank0_i = 0;
  dut->data_resp_bank1_i = 0;
  dut->data_resp_bank2_i = 0;
  dut->data_resp_bank3_i = 0;
  dut->l2_req_ready_i = 0;
}

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.req_valid.sample(dut->req_valid_i);
  cov.req_is_tbox.sample(dut->req_is_tbox_i);
  cov.req_mem_idx.sample(dut->req_mem_idx_i);
  cov.data_req_grant.sample(dut->data_req_grant_i);
  cov.data_req_nack.sample(dut->data_req_nack_i);
  cov.l2_req_ready.sample(dut->l2_req_ready_i);
  cov.req_tag.sample(dut->req_tag_i);
  cov.req_set.sample(dut->req_set_i);
  cov.req_way_en.sample(dut->req_way_en_i);
  cov.req_addr_low.sample(dut->req_addr_low_i);
  cov.data_resp_bank0.sample(dut->data_resp_bank0_i);
  cov.data_resp_bank1.sample(dut->data_resp_bank1_i);
  cov.data_resp_bank2.sample(dut->data_resp_bank2_i);
  cov.data_resp_bank3.sample(dut->data_resp_bank3_i);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.req_valid.full(), "coverage: req_valid exercised 0/1");
  sim.check(cov.req_is_tbox.full(), "coverage: req_is_tbox exercised 0/1");
  sim.check(cov.req_mem_idx.full(), "coverage: req_mem_idx exercised 0/1");
  sim.check(cov.data_req_grant.full(), "coverage: data_req_grant exercised 0/1");
  sim.check(cov.data_req_nack.full(), "coverage: data_req_nack exercised 0/1");
  sim.check(cov.l2_req_ready.full(), "coverage: l2_req_ready exercised 0/1");
  sim.check(cov.req_tag.full(), "coverage: req_tag exercised zero/nonzero");
  sim.check(cov.req_set.full(), "coverage: req_set exercised zero/nonzero");
  sim.check(cov.req_way_en.full(), "coverage: req_way_en exercised zero/nonzero");
  sim.check(cov.req_addr_low.full(), "coverage: req_addr_low exercised zero/nonzero");
  sim.check(cov.data_resp_bank0.full(), "coverage: data_resp_bank0 exercised zero/nonzero");
  sim.check(cov.data_resp_bank1.full(), "coverage: data_resp_bank1 exercised zero/nonzero");
  sim.check(cov.data_resp_bank2.full(), "coverage: data_resp_bank2 exercised zero/nonzero");
  sim.check(cov.data_resp_bank3.full(), "coverage: data_resp_bank3 exercised zero/nonzero");
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("req_ready", sim.dut->orig_req_ready_o, sim.dut->new_req_ready_o);
  sim.compare("data_req_bid", sim.dut->orig_data_req_bid_o, sim.dut->new_data_req_bid_o);
  sim.compare("data_req_valid", sim.dut->orig_data_req_valid_o, sim.dut->new_data_req_valid_o);
  sim.compare("data_req_valid_l", sim.dut->orig_data_req_valid_l_o, sim.dut->new_data_req_valid_l_o);
  sim.compare("data_req_valid_h", sim.dut->orig_data_req_valid_h_o, sim.dut->new_data_req_valid_h_o);
  sim.compare("data_req_way", sim.dut->orig_data_req_way_o, sim.dut->new_data_req_way_o);
  sim.compare("data_req_addr", sim.dut->orig_data_req_addr_o, sim.dut->new_data_req_addr_o);
  sim.compare("l2_req_valid", sim.dut->orig_l2_req_valid_o, sim.dut->new_l2_req_valid_o);
  sim.compare("l2_req_id", sim.dut->orig_l2_req_id_o, sim.dut->new_l2_req_id_o);
  sim.compare("l2_req_source", sim.dut->orig_l2_req_source_o, sim.dut->new_l2_req_source_o);
  sim.compare("l2_req_wdata", sim.dut->orig_l2_req_wdata_o, sim.dut->new_l2_req_wdata_o);
  sim.compare("l2_req_opcode", sim.dut->orig_l2_req_opcode_o, sim.dut->new_l2_req_opcode_o);
  sim.compare("l2_req_address", sim.dut->orig_l2_req_address_o, sim.dut->new_l2_req_address_o);
  for (int i = 0; i < 8; ++i) {
    sim.compare("l2_req_data[" + std::to_string(i) + "]",
                sim.dut->orig_l2_req_data_o[i],
                sim.dut->new_l2_req_data_o[i]);
  }
  sim.compare("l2_req_size", sim.dut->orig_l2_req_size_o, sim.dut->new_l2_req_size_o);
  sim.compare("l2_req_qwen", sim.dut->orig_l2_req_qwen_o, sim.dut->new_l2_req_qwen_o);
  sim.compare("l2_req_subopcode", sim.dut->orig_l2_req_subopcode_o, sim.dut->new_l2_req_subopcode_o);
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

void load_banks(DUT* dut, const U256& value) {
  dut->data_resp_bank0_i = value[0];
  dut->data_resp_bank1_i = value[1];
  dut->data_resp_bank2_i = value[2];
  dut->data_resp_bank3_i = value[3];
}

void run_directed_normal(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x123456789ULL;
  sim.dut->req_set_i = 6;
  sim.dut->req_way_en_i = 4;
  tick_and_compare(sim, &cov);

  load_banks(sim.dut.get(), {0x1111222233334444ULL, 0x5555666677778888ULL,
                             0x9999AAAABBBBCCCCULL, 0xDDDDEEEEFFFF0001ULL});
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);

  load_banks(sim.dut.get(), {0x0123456789ABCDEFULL, 0xFEDCBA9876543210ULL,
                             0x0F0E0D0C0B0A0908ULL, 0x8070605040302010ULL});
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);

  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 1;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
}

void run_directed_tbox(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x111111111ULL;
  sim.dut->req_set_i = 3;
  sim.dut->req_way_en_i = 2;
  sim.dut->req_is_tbox_i = 1;
  sim.dut->req_mem_idx_i = 1;
  sim.dut->req_addr_low_i = 0x22;
  tick_and_compare(sim, &cov);

  load_banks(sim.dut.get(), {0xABCDABCDABCDABCDULL, 0x1111222233334444ULL,
                             0x2222333344445555ULL, 0x3333444455556666ULL});
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);

  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 1;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
}

void run_directed_nack(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x200000001ULL;
  sim.dut->req_set_i = 1;
  sim.dut->req_way_en_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_grant_i = 0;
  sim.dut->data_req_nack_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_nack_i = 0;
  sim.dut->l2_req_ready_i = 1;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
}

void run_directed_backpressure(CosimCtrl<DUT>& sim, Coverage& cov) {
  clear_inputs(sim.dut.get());
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x300000001ULL;
  sim.dut->req_set_i = 2;
  sim.dut->req_way_en_i = 8;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_grant_i = 1;
  tick_and_compare(sim, &cov);
  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 0;
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
  tick_and_compare(sim, &cov);
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

  run_directed_normal(sim, coverage);
  run_directed_tbox(sim, coverage);
  run_directed_nack(sim, coverage);
  run_directed_backpressure(sim, coverage);

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu, 0x2468ACE1u};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    clear_inputs(sim.dut.get());
    for (int cycle = 0; cycle < 512; ++cycle) {
      sim.dut->req_valid_i = xorshift32(seed) & 1u;
      sim.dut->req_tag_i =
          ((uint64_t(xorshift32(seed)) << 1) ^ xorshift32(seed)) & ((1ULL << 33) - 1);
      sim.dut->req_set_i = xorshift32(seed) & 0x7Fu;
      sim.dut->req_way_en_i = 1u << (xorshift32(seed) & 0x3u);
      sim.dut->req_is_tbox_i = xorshift32(seed) & 1u;
      sim.dut->req_mem_idx_i = xorshift32(seed) & 1u;
      sim.dut->req_addr_low_i = xorshift32(seed) & 0x7Fu;
      sim.dut->data_req_grant_i = xorshift32(seed) & 1u;
      sim.dut->data_req_nack_i = xorshift32(seed) & 1u;
      sim.dut->data_resp_bank0_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      sim.dut->data_resp_bank1_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      sim.dut->data_resp_bank2_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      sim.dut->data_resp_bank3_i = (uint64_t(xorshift32(seed)) << 32) | xorshift32(seed);
      sim.dut->l2_req_ready_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_coop_tload_ports_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_neigh_coop_tload_ports_tb;

namespace {
uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("slv_rdy_out_data", d->orig_slv_rdy_out_data_bits_o, d->new_slv_rdy_out_data_bits_o);
  sim.compare("slv_rdy_out_valid", d->orig_slv_rdy_out_valid_o, d->new_slv_rdy_out_valid_o);
  sim.compare("slv_rdy_in_data_lv", d->orig_slv_rdy_in_data_lv_bits_o, d->new_slv_rdy_in_data_lv_bits_o);
  sim.compare("slv_rdy_in_valid_lv", d->orig_slv_rdy_in_valid_lv_o, d->new_slv_rdy_in_valid_lv_o);
  sim.compare("mst_done_out_coop_id", d->orig_mst_done_out_coop_id_o, d->new_mst_done_out_coop_id_o);
  sim.compare("mst_done_out_valid", d->orig_mst_done_out_valid_o, d->new_mst_done_out_valid_o);
  sim.compare("mst_done_in_coop_id_lv", d->orig_mst_done_in_coop_id_lv_bits_o,
              d->new_mst_done_in_coop_id_lv_bits_o);
  sim.compare("mst_done_in_valid_lv", d->orig_mst_done_in_valid_lv_o, d->new_mst_done_in_valid_lv_o);
}

void tick_compare(CosimCtrl<DUT>& sim, uint32_t& ones, uint32_t& zeros) {
  auto* d = sim.dut.get();
  uint32_t packed = 0;
  packed |= d->slv_rdy_out_data_lv_bits_i & 0x7fu;
  packed |= (d->slv_rdy_out_valid_lv_i & 1u) << 7;
  packed |= (d->slv_rdy_in_data_bits_i & 0x1fffffu) << 8;
  packed ^= (d->slv_rdy_in_valid_i & 0x7u) << 1;  // sampled separately below too
  ones |= packed;
  zeros |= ~packed;
  ones |= (d->slv_rdy_in_valid_i & 0x7u) << 29;
  zeros |= ((~d->slv_rdy_in_valid_i) & 0x7u) << 29;

  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 100000;
  auto* d = sim.dut.get();
  d->slv_rdy_out_data_lv_bits_i = 0;
  d->slv_rdy_out_valid_lv_i = 0;
  d->slv_rdy_in_data_bits_i = 0;
  d->slv_rdy_in_valid_i = 0;
  d->mst_done_out_coop_id_lv_i = 0;
  d->mst_done_out_valid_lv_i = 0;
  d->mst_done_in_coop_id_bits_i = 0;
  d->mst_done_in_valid_i = 0;
  sim.reset();
  compare_outputs(sim);

  uint32_t seed = 0x504f5254u;
  uint32_t ones = 0, zeros = 0;
  uint8_t done_id_ones = 0, done_id_zeros = 0;
  uint8_t done_valid_ones = 0, done_valid_zeros = 0;
  uint16_t done_in_id_ones = 0, done_in_id_zeros = 0;
  uint8_t done_in_valid_ones = 0, done_in_valid_zeros = 0;

  for (int cycle = 0; cycle < 4096; ++cycle) {
    if ((cycle & 31) == 0) {
      d->slv_rdy_out_data_lv_bits_i = 0x7f;
      d->slv_rdy_out_valid_lv_i = 1;
      d->slv_rdy_in_data_bits_i = 0x1fffff;
      d->slv_rdy_in_valid_i = 0x7;
      d->mst_done_out_coop_id_lv_i = 0x1f;
      d->mst_done_out_valid_lv_i = 0x7;
      d->mst_done_in_coop_id_bits_i = 0x7fff;
      d->mst_done_in_valid_i = 0x7;
    } else if ((cycle & 31) == 1) {
      d->slv_rdy_out_data_lv_bits_i = 0;
      d->slv_rdy_out_valid_lv_i = 0;
      d->slv_rdy_in_data_bits_i = 0;
      d->slv_rdy_in_valid_i = 0;
      d->mst_done_out_coop_id_lv_i = 0;
      d->mst_done_out_valid_lv_i = 0;
      d->mst_done_in_coop_id_bits_i = 0;
      d->mst_done_in_valid_i = 0;
    } else {
      d->slv_rdy_out_data_lv_bits_i = xorshift32(seed) & 0x7fu;
      d->slv_rdy_out_valid_lv_i = xorshift32(seed) & 1u;
      d->slv_rdy_in_data_bits_i = xorshift32(seed) & 0x1fffffu;
      d->slv_rdy_in_valid_i = xorshift32(seed) & 0x7u;
      d->mst_done_out_coop_id_lv_i = xorshift32(seed) & 0x1fu;
      d->mst_done_out_valid_lv_i = xorshift32(seed) & 0x7u;
      d->mst_done_in_coop_id_bits_i = xorshift32(seed) & 0x7fffu;
      d->mst_done_in_valid_i = xorshift32(seed) & 0x7u;
    }

    done_id_ones |= d->mst_done_out_coop_id_lv_i;
    done_id_zeros |= static_cast<uint8_t>(~d->mst_done_out_coop_id_lv_i) & 0x1fu;
    done_valid_ones |= d->mst_done_out_valid_lv_i;
    done_valid_zeros |= static_cast<uint8_t>(~d->mst_done_out_valid_lv_i) & 0x7u;
    done_in_id_ones |= d->mst_done_in_coop_id_bits_i;
    done_in_id_zeros |= static_cast<uint16_t>(~d->mst_done_in_coop_id_bits_i) & 0x7fffu;
    done_in_valid_ones |= d->mst_done_in_valid_i;
    done_in_valid_zeros |= static_cast<uint8_t>(~d->mst_done_in_valid_i) & 0x7u;
    tick_compare(sim, ones, zeros);
  }

  sim.check((ones & 0x1fffffffu) == 0x1fffffffu && (zeros & 0x1fffffffu) == 0x1fffffffu,
            "coverage: slave ready data/valid inputs toggled 0/1");
  sim.check(done_id_ones == 0x1f && done_id_zeros == 0x1f,
            "coverage: mst_done_out_coop_id_lv toggled 0/1");
  sim.check(done_valid_ones == 0x7 && done_valid_zeros == 0x7,
            "coverage: mst_done_out_valid_lv toggled 0/1");
  sim.check(done_in_id_ones == 0x7fff && done_in_id_zeros == 0x7fff,
            "coverage: mst_done_in_coop_id toggled 0/1");
  sim.check(done_in_valid_ones == 0x7 && done_in_valid_zeros == 0x7,
            "coverage: mst_done_in_valid toggled 0/1");
  sim.check(sim.comparisons >= 30000, "comparison count >= 30000");
  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_miss_ff_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_neigh_miss_ff_tb;

static uint32_t rng_state = 0x31415926U;

static uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("l2_dcache_req_ready", dut->orig_l2_dcache_req_ready_o, dut->new_l2_dcache_req_ready_o);
  sim.compare("miss_valid", dut->orig_miss_valid_o, dut->new_miss_valid_o);
  sim.compare("miss_req_id", dut->orig_miss_req_id_o, dut->new_miss_req_id_o);
  sim.compare("miss_req_source", dut->orig_miss_req_source_o, dut->new_miss_req_source_o);
  sim.compare("miss_req_wdata", dut->orig_miss_req_wdata_o, dut->new_miss_req_wdata_o);
  sim.compare("miss_req_opcode", dut->orig_miss_req_opcode_o, dut->new_miss_req_opcode_o);
  sim.compare("miss_req_address", dut->orig_miss_req_address_o, dut->new_miss_req_address_o);
  sim.compare("miss_req_data", dut->orig_miss_req_data_o, dut->new_miss_req_data_o);
  sim.compare("miss_req_size", dut->orig_miss_req_size_o, dut->new_miss_req_size_o);
  sim.compare("miss_req_qwen", dut->orig_miss_req_qwen_o, dut->new_miss_req_qwen_o);
  sim.compare("miss_req_subopcode", dut->orig_miss_req_subopcode_o, dut->new_miss_req_subopcode_o);
  sim.compare("coop_miss_valid", dut->orig_coop_miss_valid_o, dut->new_coop_miss_valid_o);
  sim.compare("coop0_req_id", dut->orig_coop0_req_id_o, dut->new_coop0_req_id_o);
  sim.compare("coop0_req_opcode", dut->orig_coop0_req_opcode_o, dut->new_coop0_req_opcode_o);
  sim.compare("coop0_req_address", dut->orig_coop0_req_address_o, dut->new_coop0_req_address_o);
  sim.compare("coop0_req_data", dut->orig_coop0_req_data_o, dut->new_coop0_req_data_o);
  sim.compare("coop0_req_size", dut->orig_coop0_req_size_o, dut->new_coop0_req_size_o);
  sim.compare("coop0_req_qwen", dut->orig_coop0_req_qwen_o, dut->new_coop0_req_qwen_o);
  sim.compare("coop0_req_subopcode", dut->orig_coop0_req_subopcode_o, dut->new_coop0_req_subopcode_o);
  sim.compare("coop1_req_id", dut->orig_coop1_req_id_o, dut->new_coop1_req_id_o);
  sim.compare("coop1_req_opcode", dut->orig_coop1_req_opcode_o, dut->new_coop1_req_opcode_o);
  sim.compare("coop1_req_address", dut->orig_coop1_req_address_o, dut->new_coop1_req_address_o);
  sim.compare("coop1_req_data", dut->orig_coop1_req_data_o, dut->new_coop1_req_data_o);
  sim.compare("coop1_req_size", dut->orig_coop1_req_size_o, dut->new_coop1_req_size_o);
  sim.compare("coop1_req_qwen", dut->orig_coop1_req_qwen_o, dut->new_coop1_req_qwen_o);
  sim.compare("coop1_req_subopcode", dut->orig_coop1_req_subopcode_o, dut->new_coop1_req_subopcode_o);
  sim.compare("dbg_sm_signals", dut->orig_dbg_sm_signals_o, dut->new_dbg_sm_signals_o);
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  sim.reset(2);

  printf("phase 1: directed regular/cooperative flows\n");
  const uint8_t valid_patterns[] = {0x0, 0x4, 0x1, 0x2, 0x7};
  const uint8_t miss_ready_patterns[] = {0, 1, 0, 0, 1};
  const uint8_t coop_ready_patterns[] = {0, 0, 1, 2, 3};

  for (int i = 0; i < 5; ++i) {
    dut->l2_req_id_i = 3 + i;
    dut->l2_req_wdata_i = i & 1;
    dut->l2_req_opcode_i = (i * 3) & 0x1f;
    dut->l2_req_address_i = 0x1000 + (i * 0x80);
    dut->l2_req_data_i = 0x100 + i;
    dut->l2_req_size_i = i & 0x7;
    dut->l2_req_qwen_i = (i * 5) & 0xf;
    dut->l2_req_subopcode_i = (i * 11) & 0x7f;
    dut->l2_dcache_req_valid_i = valid_patterns[i];
    dut->miss_ready_i = miss_ready_patterns[i];
    dut->coop_miss_ready_i = coop_ready_patterns[i];
    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  printf("phase 2: random stimulus (3000 cycles)\n");
  for (int i = 0; i < 3000; ++i) {
    dut->l2_req_id_i = xorshift32() & 0x1f;
    dut->l2_req_wdata_i = xorshift32() & 0x1;
    dut->l2_req_opcode_i = xorshift32() & 0x1f;
    dut->l2_req_address_i = ((uint64_t)(xorshift32() & 0xff) << 32) | xorshift32();
    dut->l2_req_data_i = xorshift32() & 0x3ffff;
    dut->l2_req_size_i = xorshift32() & 0x7;
    dut->l2_req_qwen_i = xorshift32() & 0xf;
    dut->l2_req_subopcode_i = xorshift32() & 0x7f;
    dut->l2_dcache_req_valid_i = xorshift32() & 0x7;
    dut->miss_ready_i = xorshift32() & 0x1;
    dut->coop_miss_ready_i = xorshift32() & 0x3;
    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  return sim.finish();
}

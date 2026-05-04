// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_evict_ff_unit_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_neigh_evict_ff_unit_tb;

static uint32_t rng_state = 0x4e696768U;

static uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

static uint64_t build_local_msg_addr(uint8_t shire_id, uint8_t neigh_id, uint8_t local_dest, uint8_t msg_id) {
  uint64_t addr = 0;
  addr |= uint64_t(msg_id & 0xff) << 3;
  addr |= uint64_t(((neigh_id & 0x3) << 3) | (local_dest & 0x7)) << 13;
  addr |= uint64_t(0) << 20;
  addr |= uint64_t(shire_id) << 22;
  addr |= uint64_t(1) << 32;
  return addr;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("l2_dcache_req_ready", dut->orig_l2_dcache_req_ready_o, dut->new_l2_dcache_req_ready_o);
  sim.compare("local_message_valid", dut->orig_local_message_valid_o, dut->new_local_message_valid_o);
  sim.compare("local_message_id", dut->orig_local_message_id_o, dut->new_local_message_id_o);
  sim.compare("local_message_dest", dut->orig_local_message_dest_o, dut->new_local_message_dest_o);
  sim.compare("local_message_wdata", dut->orig_local_message_wdata_o, dut->new_local_message_wdata_o);
  sim.compare("local_message_opcode", dut->orig_local_message_opcode_o, dut->new_local_message_opcode_o);
  sim.compare("local_message_data0", dut->orig_local_message_data0_o, dut->new_local_message_data0_o);
  sim.compare("local_message_data1", dut->orig_local_message_data1_o, dut->new_local_message_data1_o);
  sim.compare("local_message_data2", dut->orig_local_message_data2_o, dut->new_local_message_data2_o);
  sim.compare("local_message_data3", dut->orig_local_message_data3_o, dut->new_local_message_data3_o);
  sim.compare("local_message_size", dut->orig_local_message_size_o, dut->new_local_message_size_o);
  sim.compare("local_message_qwen", dut->orig_local_message_qwen_o, dut->new_local_message_qwen_o);
  sim.compare("evict_valid", dut->orig_evict_valid_o, dut->new_evict_valid_o);
  sim.compare("evict_req_id", dut->orig_evict_req_id_o, dut->new_evict_req_id_o);
  sim.compare("evict_req_source", dut->orig_evict_req_source_o, dut->new_evict_req_source_o);
  sim.compare("evict_req_wdata", dut->orig_evict_req_wdata_o, dut->new_evict_req_wdata_o);
  sim.compare("evict_req_opcode", dut->orig_evict_req_opcode_o, dut->new_evict_req_opcode_o);
  sim.compare("evict_req_address", dut->orig_evict_req_address_o, dut->new_evict_req_address_o);
  sim.compare("evict_req_data0", dut->orig_evict_req_data0_o, dut->new_evict_req_data0_o);
  sim.compare("evict_req_data1", dut->orig_evict_req_data1_o, dut->new_evict_req_data1_o);
  sim.compare("evict_req_data2", dut->orig_evict_req_data2_o, dut->new_evict_req_data2_o);
  sim.compare("evict_req_data3", dut->orig_evict_req_data3_o, dut->new_evict_req_data3_o);
  sim.compare("evict_req_size", dut->orig_evict_req_size_o, dut->new_evict_req_size_o);
  sim.compare("evict_req_qwen", dut->orig_evict_req_qwen_o, dut->new_evict_req_qwen_o);
  sim.compare("evict_req_subopcode", dut->orig_evict_req_subopcode_o, dut->new_evict_req_subopcode_o);
  sim.compare("dbg_sm_signals", dut->orig_dbg_sm_signals_o, dut->new_dbg_sm_signals_o);
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  sim.reset(2);

  printf("phase 1: directed local/regular mixes\n");
  for (int i = 0; i < 12; ++i) {
    dut->shire_id_i = 0x20 + (i & 0x1f);
    dut->neigh_id_i = i & 0x3;
    dut->local_channel_mask_i = (i & 1) ? 0x2 : 0x1;
    dut->local_dest0_i = 1;
    dut->local_dest1_i = 5;
    dut->local_dest2_i = 6;
    dut->l2_req_id_i = i & 0x1f;
    dut->l2_req_source_i = i & 1;
    dut->l2_req_wdata_i = 1;
    dut->l2_req_opcode_i = (i & 1) ? 4 : 10;
    dut->l2_req_address_i = (i & 1)
        ? build_local_msg_addr(dut->shire_id_i, dut->neigh_id_i, dut->local_dest1_i, 0x40 + i)
        : (0x1000 + (i * 0x40));
    dut->l2_req_data0_i = 0x100 + i;
    dut->l2_req_data1_i = 0x200 + i;
    dut->l2_req_data2_i = 0x300 + i;
    dut->l2_req_data3_i = 0x400 + i;
    dut->l2_req_size_i = (i & 1) ? 5 : 6;
    dut->l2_req_qwen_i = i & 0xf;
    dut->l2_req_subopcode_i = (i * 7) & 0x7f;
    dut->l2_dcache_req_valid_i = 1;
    dut->local_message_ready_i = (i >> 1) & 1;
    dut->evict_ready_i = (i >> 2) & 1;
    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  printf("phase 2: random stimulus (3000 cycles)\n");
  for (int i = 0; i < 3000; ++i) {
    dut->shire_id_i = xorshift32() & 0xff;
    dut->neigh_id_i = xorshift32() & 0x3;
    dut->local_channel_mask_i = xorshift32() & 0x7;
    dut->local_dest0_i = xorshift32() & 0x7;
    dut->local_dest1_i = xorshift32() & 0x7;
    dut->local_dest2_i = xorshift32() & 0x7;
    dut->l2_req_id_i = xorshift32() & 0x1f;
    dut->l2_req_source_i = xorshift32() & 0x1;
    dut->l2_req_wdata_i = 1;
    dut->l2_req_opcode_i = ((i & 3) == 0) ? 4 : (xorshift32() & 0x1f);
    if ((i & 3) == 0) {
      dut->l2_req_address_i =
          build_local_msg_addr(dut->shire_id_i, dut->neigh_id_i, dut->local_dest0_i, xorshift32() & 0xff);
      dut->l2_req_size_i = xorshift32() & 0x5;
    } else {
      dut->l2_req_address_i = ((uint64_t)(xorshift32() & 0xff) << 32) | xorshift32();
      dut->l2_req_size_i = xorshift32() & 0x7;
    }
    dut->l2_req_data0_i = xorshift32();
    dut->l2_req_data1_i = xorshift32();
    dut->l2_req_data2_i = xorshift32();
    dut->l2_req_data3_i = xorshift32();
    dut->l2_req_qwen_i = xorshift32() & 0xf;
    dut->l2_req_subopcode_i = xorshift32() & 0x7f;
    dut->l2_dcache_req_valid_i = xorshift32() & 0x1;
    dut->local_message_ready_i = xorshift32() & 0x1;
    dut->evict_ready_i = xorshift32() & 0x1;
    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  return sim.finish();
}

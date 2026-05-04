// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_shared_ptw_tb.h"
#include "cosim_ctrl.h"

#include <string>

using DUT = Vcosim_neigh_shared_ptw_tb;

namespace {

uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 0;
  for (int i = 0; i < 5; ++i) {
    dut->req_satp_mode_i[i] = 0;
    dut->req_satp_ppn_i[i] = 0;
    dut->req_prv_i[i] = 0;
    dut->req_addr_i[i] = 0;
  }
  dut->req_valid_i = 0;
  dut->invalidate_i = 0;
  dut->mprot_enable_secure_memory_i = 0;
  dut->mprot_dram_size_i = 0;
  dut->mprot_disable_osbox_access_i = 0;
  dut->mprot_disable_pcie_access_i = 0;
  dut->mprot_io_access_mode_i = 0;
  dut->vmspagesize_i = 0;
  dut->mem_miss_req_disable_next_i = 0;
  dut->mem_miss_req_ready_i = 0;
  dut->mem_fill_req_id_i = 0;
  dut->mem_fill_req_dest_i = 0;
  dut->mem_fill_req_wdata_i = 0;
  dut->mem_fill_req_opcode_i = 0;
  dut->mem_fill_req_data_0_i = 0;
  dut->mem_fill_req_data_1_i = 0;
  dut->mem_fill_req_data_2_i = 0;
  dut->mem_fill_req_data_3_i = 0;
  dut->mem_fill_req_data_4_i = 0;
  dut->mem_fill_req_data_5_i = 0;
  dut->mem_fill_req_data_6_i = 0;
  dut->mem_fill_req_data_7_i = 0;
  dut->mem_fill_req_size_i = 0;
  dut->mem_fill_req_qwen_i = 0;
  dut->mem_fill_req_valid_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("req_ready", sim.dut->orig_req_ready_o, sim.dut->new_req_ready_o);
  sim.compare("resp_data", sim.dut->orig_resp_data_o, sim.dut->new_resp_data_o);
  sim.compare("resp_valid", sim.dut->orig_resp_valid_o, sim.dut->new_resp_valid_o);
  sim.compare("mem_miss_req_id", sim.dut->orig_mem_miss_req_id_o, sim.dut->new_mem_miss_req_id_o);
  sim.compare("mem_miss_req_source", sim.dut->orig_mem_miss_req_source_o, sim.dut->new_mem_miss_req_source_o);
  sim.compare("mem_miss_req_wdata", sim.dut->orig_mem_miss_req_wdata_o, sim.dut->new_mem_miss_req_wdata_o);
  sim.compare("mem_miss_req_opcode", sim.dut->orig_mem_miss_req_opcode_o, sim.dut->new_mem_miss_req_opcode_o);
  sim.compare("mem_miss_req_address", sim.dut->orig_mem_miss_req_address_o, sim.dut->new_mem_miss_req_address_o);
  sim.compare("mem_miss_req_data_0", sim.dut->orig_mem_miss_req_data_0_o, sim.dut->new_mem_miss_req_data_0_o);
  sim.compare("mem_miss_req_data_1", sim.dut->orig_mem_miss_req_data_1_o, sim.dut->new_mem_miss_req_data_1_o);
  sim.compare("mem_miss_req_data_2", sim.dut->orig_mem_miss_req_data_2_o, sim.dut->new_mem_miss_req_data_2_o);
  sim.compare("mem_miss_req_data_3", sim.dut->orig_mem_miss_req_data_3_o, sim.dut->new_mem_miss_req_data_3_o);
  sim.compare("mem_miss_req_size", sim.dut->orig_mem_miss_req_size_o, sim.dut->new_mem_miss_req_size_o);
  sim.compare("mem_miss_req_qwen", sim.dut->orig_mem_miss_req_qwen_o, sim.dut->new_mem_miss_req_qwen_o);
  sim.compare("mem_miss_req_subopcode", sim.dut->orig_mem_miss_req_subopcode_o, sim.dut->new_mem_miss_req_subopcode_o);
  sim.compare("mem_miss_req_valid", sim.dut->orig_mem_miss_req_valid_o, sim.dut->new_mem_miss_req_valid_o);
  sim.compare("mem_fill_req_ready", sim.dut->orig_mem_fill_req_ready_o, sim.dut->new_mem_fill_req_ready_o);
  sim.compare("dbg_sm_signals", sim.dut->orig_dbg_sm_signals_o, sim.dut->new_dbg_sm_signals_o);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  clear_inputs(dut);
  sim.reset(10);
  compare_outputs(sim);

  dut->ioshire_i = 1;
  dut->vmspagesize_i = 3;
  dut->req_satp_mode_i[1] = 8;
  dut->req_satp_ppn_i[1] = 0x40480;
  dut->req_prv_i[1] = 1;
  dut->req_addr_i[1] = 0x12;
  dut->req_satp_mode_i[4] = 8;
  dut->req_satp_ppn_i[4] = 0x40490;
  dut->req_prv_i[4] = 1;
  dut->req_addr_i[4] = 0x34;
  dut->req_valid_i = 0x12;
  sim.tick();
  compare_outputs(sim);

  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  compare_outputs(sim);

  dut->mem_fill_req_opcode_i = 1;
  dut->mem_fill_req_size_i = 6;
  dut->mem_fill_req_data_0_i = 0x10000003;
  dut->mem_fill_req_valid_i = 1;
  sim.tick();
  compare_outputs(sim);

  dut->mem_fill_req_valid_i = 0;
  sim.tick();
  compare_outputs(sim);

  uint32_t seed = 0x2e67d19bu;
  for (int cycle = 0; cycle < 2500; ++cycle) {
    for (int i = 0; i < 5; ++i) {
      seed = next_rand(seed);
      dut->req_satp_mode_i[i] = ((seed >> 0) & 1u) ? 8u : 9u;
      dut->req_satp_ppn_i[i] = seed & ((1ULL << 44) - 1ULL);
      dut->req_prv_i[i] = (seed >> 4) & 0x3u;
      dut->req_addr_i[i] = (seed >> 6) & ((1u << 27) - 1u);
    }
    seed = next_rand(seed);
    dut->ioshire_i = (seed >> 0) & 1u;
    dut->req_valid_i = (seed >> 1) & 0x1fu;
    dut->invalidate_i = (seed >> 6) & 0x1fu;
    dut->mprot_enable_secure_memory_i = (seed >> 11) & 1u;
    dut->mprot_dram_size_i = (seed >> 12) & 0x3u;
    dut->mprot_disable_osbox_access_i = (seed >> 14) & 1u;
    dut->mprot_disable_pcie_access_i = (seed >> 15) & 1u;
    dut->mprot_io_access_mode_i = (seed >> 16) & 0x3u;
    dut->vmspagesize_i = (seed >> 18) & 0x3u;
    dut->mem_miss_req_disable_next_i = (seed >> 20) & 1u;
    dut->mem_miss_req_ready_i = (seed >> 21) & 1u;
    dut->mem_fill_req_id_i = (seed >> 22) & 0xffu;
    dut->mem_fill_req_dest_i = (seed >> 0) & 0x1fu;
    dut->mem_fill_req_wdata_i = (seed >> 30) & 1u;
    dut->mem_fill_req_opcode_i = (seed >> 24) & 0x3u;
    dut->mem_fill_req_data_0_i = 0x1111111100000000ull ^ seed;
    dut->mem_fill_req_data_1_i = 0x2222222200000000ull ^ (static_cast<uint64_t>(seed) << 1);
    dut->mem_fill_req_data_2_i = 0x3333333300000000ull ^ (static_cast<uint64_t>(seed) << 2);
    dut->mem_fill_req_data_3_i = 0x4444444400000000ull ^ (static_cast<uint64_t>(seed) << 3);
    dut->mem_fill_req_data_4_i = 0x5555555500000000ull ^ (static_cast<uint64_t>(seed) << 4);
    dut->mem_fill_req_data_5_i = 0x6666666600000000ull ^ (static_cast<uint64_t>(seed) << 5);
    dut->mem_fill_req_data_6_i = 0x7777777700000000ull ^ (static_cast<uint64_t>(seed) << 6);
    dut->mem_fill_req_data_7_i = 0x8888888800000000ull ^ (static_cast<uint64_t>(seed) << 7);
    dut->mem_fill_req_size_i = (seed >> 26) & 0x7u;
    dut->mem_fill_req_qwen_i = (seed >> 10) & 0xfu;
    dut->mem_fill_req_valid_i = (seed >> 29) & 1u;
    sim.tick();
    compare_outputs(sim);
  }

  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_ptw_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_ptw_top_tb;

namespace {

constexpr uint8_t kSatpModeSv39 = 8;
constexpr uint8_t kVmPage4K = 0;
constexpr uint8_t kVmPage1G = 3;
constexpr uint8_t kRspAckData = 1;

uint64_t make_pte(uint64_t ppn, bool r, bool w, bool x, bool a = false, bool d = false) {
  return (ppn << 10)
       | (static_cast<uint64_t>(d) << 7)
       | (static_cast<uint64_t>(a) << 6)
       | (static_cast<uint64_t>(x) << 3)
       | (static_cast<uint64_t>(w) << 2)
       | (static_cast<uint64_t>(r) << 1)
       | 1ULL;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("req_ready", dut->orig_req_ready_o, dut->new_req_ready_o);
  sim.compare("resp_data", dut->orig_resp_data_o, dut->new_resp_data_o);
  sim.compare("resp_valid", dut->orig_resp_valid_o, dut->new_resp_valid_o);
  sim.compare("mem_miss_req_id", dut->orig_mem_miss_req_id_o, dut->new_mem_miss_req_id_o);
  sim.compare("mem_miss_req_source", dut->orig_mem_miss_req_source_o, dut->new_mem_miss_req_source_o);
  sim.compare("mem_miss_req_wdata", dut->orig_mem_miss_req_wdata_o, dut->new_mem_miss_req_wdata_o);
  sim.compare("mem_miss_req_opcode", dut->orig_mem_miss_req_opcode_o, dut->new_mem_miss_req_opcode_o);
  sim.compare("mem_miss_req_address", dut->orig_mem_miss_req_address_o, dut->new_mem_miss_req_address_o);
  sim.compare("mem_miss_req_data_0", dut->orig_mem_miss_req_data_0_o, dut->new_mem_miss_req_data_0_o);
  sim.compare("mem_miss_req_data_1", dut->orig_mem_miss_req_data_1_o, dut->new_mem_miss_req_data_1_o);
  sim.compare("mem_miss_req_data_2", dut->orig_mem_miss_req_data_2_o, dut->new_mem_miss_req_data_2_o);
  sim.compare("mem_miss_req_data_3", dut->orig_mem_miss_req_data_3_o, dut->new_mem_miss_req_data_3_o);
  sim.compare("mem_miss_req_size", dut->orig_mem_miss_req_size_o, dut->new_mem_miss_req_size_o);
  sim.compare("mem_miss_req_qwen", dut->orig_mem_miss_req_qwen_o, dut->new_mem_miss_req_qwen_o);
  sim.compare("mem_miss_req_subopcode", dut->orig_mem_miss_req_subopcode_o, dut->new_mem_miss_req_subopcode_o);
  sim.compare("mem_miss_req_valid", dut->orig_mem_miss_req_valid_o, dut->new_mem_miss_req_valid_o);
  sim.compare("mem_fill_req_ready", dut->orig_mem_fill_req_ready_o, dut->new_mem_fill_req_ready_o);
  sim.compare("dbg_sm_signals", dut->orig_dbg_sm_signals_o, dut->new_dbg_sm_signals_o);
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 0;
  dut->req_satp_mode_i = 0;
  dut->req_satp_ppn_i = 0;
  dut->req_prv_i = 0;
  dut->req_addr_i = 0;
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

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();
  std::mt19937_64 rng(0x5eed5eedULL);

  clear_inputs(dut);
  sim.reset(2);

  for (int cycle = 0; cycle < 16; ++cycle) {
    clear_inputs(dut);
    if (cycle == 2) {
      dut->req_valid_i = 1;
      dut->ioshire_i = 0;
      dut->req_satp_mode_i = kSatpModeSv39;
      dut->req_satp_ppn_i = 0x40480u;
      dut->req_prv_i = 1;
      dut->vmspagesize_i = kVmPage1G;
    } else if (cycle == 6) {
      dut->req_valid_i = 1;
      dut->ioshire_i = 1;
      dut->req_satp_mode_i = kSatpModeSv39;
      dut->req_satp_ppn_i = 0x40480u;
      dut->req_prv_i = 1;
      dut->vmspagesize_i = kVmPage4K;
    } else if (cycle == 7 || cycle == 10) {
      dut->mem_miss_req_ready_i = 1;
    } else if (cycle == 8) {
      dut->mem_fill_req_valid_i = 1;
      dut->mem_fill_req_opcode_i = kRspAckData;
      dut->mem_fill_req_size_i = 6;
      dut->mem_fill_req_data_0_i = make_pte(0x40481u, false, false, false);
    } else if (cycle == 11) {
      dut->mem_fill_req_valid_i = 1;
      dut->mem_fill_req_opcode_i = kRspAckData;
      dut->mem_fill_req_size_i = 6;
      dut->mem_fill_req_data_0_i = make_pte(0x30000u, true, false, false);
    }
    sim.tick();
    compare_outputs(sim);
  }

  for (int cycle = 0; cycle < 4000; ++cycle) {
    dut->ioshire_i = rng() & 1U;
    dut->req_satp_mode_i = kSatpModeSv39;
    dut->req_satp_ppn_i = rng() & ((1u << 28) - 1u);
    dut->req_prv_i = rng() & 0x3U;
    dut->req_addr_i = rng() & ((1ULL << 36) - 1ULL);
    dut->req_valid_i = rng() & 1U;
    dut->invalidate_i = rng() & 1U;
    dut->mprot_enable_secure_memory_i = rng() & 1U;
    dut->mprot_dram_size_i = rng() & 0x3U;
    dut->mprot_disable_osbox_access_i = rng() & 1U;
    dut->mprot_disable_pcie_access_i = rng() & 1U;
    dut->mprot_io_access_mode_i = rng() & 0x3U;
    dut->vmspagesize_i = (rng() & 1U) ? kVmPage4K : kVmPage1G;
    dut->mem_miss_req_disable_next_i = rng() & 1U;
    dut->mem_miss_req_ready_i = rng() & 1U;
    dut->mem_fill_req_id_i = rng() & 0xffU;
    dut->mem_fill_req_dest_i = rng() & 0xfU;
    dut->mem_fill_req_wdata_i = rng() & 1U;
    dut->mem_fill_req_opcode_i = rng() & 0x3U;
    dut->mem_fill_req_data_0_i = rng();
    dut->mem_fill_req_data_1_i = rng();
    dut->mem_fill_req_data_2_i = rng();
    dut->mem_fill_req_data_3_i = rng();
    dut->mem_fill_req_data_4_i = rng();
    dut->mem_fill_req_data_5_i = rng();
    dut->mem_fill_req_data_6_i = rng();
    dut->mem_fill_req_data_7_i = rng();
    dut->mem_fill_req_size_i = rng() & 0x7U;
    dut->mem_fill_req_qwen_i = rng() & 0xfU;
    dut->mem_fill_req_valid_i = rng() & 1U;

    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  return sim.finish();
}

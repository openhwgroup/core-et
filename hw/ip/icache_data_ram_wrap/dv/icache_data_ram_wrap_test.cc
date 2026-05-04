// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_data_ram_wrap_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vicache_data_ram_wrap_tb;

namespace {

constexpr uint32_t kMemAddrWidth = 9;
constexpr uint32_t kMemIdxWidth = 2;
constexpr uint32_t kNumNeighWidth = 2;
constexpr uint32_t kIcacheDbgChunkWidth = 1;
constexpr uint32_t kIcacheDbgBlockChunkWidth = 4;
constexpr uint8_t kRspAckData = 1;
constexpr uint8_t kRspMsgRcvData = 2;
constexpr uint8_t kSizeLine = 6;
constexpr uint8_t kSrcIcache = 8;

void clear_inputs(DUT* dut) {
  dut->dft_sram_clk_i = 0;
  dut->dft_scanmode_i = 0;
  dut->dft_scan_reset_n_i = 0;
  dut->dft_sram_clk_override_i = 0;
  dut->dft_ram_rei_i = 0;
  dut->dft_ram_wei_i = 0;
  dut->ram_cfg_test_en_i = 0;
  dut->ram_cfg_rm_i = 0;
  dut->ram_cfg_rme_i = 0;
  dut->ram_cfg_ra_i = 0;
  dut->ram_cfg_wa_i = 0;
  dut->ram_cfg_wpulse_i = 0;
  dut->ram_cfg_deep_sleep_i = 0;
  dut->ram_cfg_shut_down_i = 0;
  dut->icache_req_write_i = 0;
  dut->icache_req_addr_i = 0;
  dut->icache_req_valid_i = 0;
  dut->icache_resp_ready_i = 1;
  dut->neigh_sc_rsp_id_i = 0;
  dut->neigh_sc_rsp_dest_i = 0;
  dut->neigh_sc_rsp_wdata_i = 0;
  dut->neigh_sc_rsp_opcode_i = 0;
  dut->neigh_sc_rsp_data_0_i = 0;
  dut->neigh_sc_rsp_data_1_i = 0;
  dut->neigh_sc_rsp_data_2_i = 0;
  dut->neigh_sc_rsp_data_3_i = 0;
  dut->neigh_sc_rsp_data_4_i = 0;
  dut->neigh_sc_rsp_data_5_i = 0;
  dut->neigh_sc_rsp_data_6_i = 0;
  dut->neigh_sc_rsp_data_7_i = 0;
  dut->neigh_sc_rsp_size_i = 0;
  dut->neigh_sc_rsp_qwen_i = 0;
  dut->neigh_sc_rsp_valid_i = 0;
  dut->neigh_sc_rsp_ready_i = 0;
  dut->bist_req_mbist_on_i = 0;
  dut->bist_req_mbi_sel_i = 0;
  dut->bist_req_mbi_rd_en_i = 0;
  dut->bist_req_mbi_wr_en_i = 0;
  dut->bist_req_mbi_addr_i = 0;
  dut->bist_req_mbi_wdata_0_i = 0;
  dut->bist_req_mbi_wdata_1_i = 0;
  dut->bist_req_mbi_wdata_2_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

uint32_t make_bist_addr(uint32_t neigh_idx, uint32_t mem_idx, uint32_t word_addr) {
  return word_addr | (mem_idx << kMemAddrWidth) | (neigh_idx << (kMemAddrWidth + kMemIdxWidth));
}

uint32_t make_dbg_addr(uint32_t word_addr, uint32_t col, uint32_t chunk, bool ecc = false) {
  return (word_addr << kIcacheDbgBlockChunkWidth)
       | (static_cast<uint32_t>(ecc) << (kIcacheDbgBlockChunkWidth - 1))
       | (col << kIcacheDbgChunkWidth)
       | chunk;
}

void idle_ticks(SimCtrl<DUT>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) sim.tick();
}

bool wait_for_apb_ready(SimCtrl<DUT>& sim, int max_cycles = 24) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->apb_pready_o) return true;
    sim.tick();
  }
  return sim.dut->apb_pready_o;
}

bool wait_for_icache_resp(SimCtrl<DUT>& sim, int max_cycles = 24) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->icache_resp_valid_o) return true;
    sim.tick();
  }
  return sim.dut->icache_resp_valid_o;
}

bool wait_for_bist_rsp(SimCtrl<DUT>& sim, int max_cycles = 24) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->bist_rsp_mbi_rdata_0_o || sim.dut->bist_rsp_mbi_rdata_1_o ||
        (sim.dut->bist_rsp_mbi_rdata_2_o & 0xffffULL)) {
      return true;
    }
    sim.tick();
  }
  return sim.dut->bist_rsp_mbi_rdata_0_o || sim.dut->bist_rsp_mbi_rdata_1_o ||
         (sim.dut->bist_rsp_mbi_rdata_2_o & 0xffffULL);
}

std::array<uint64_t, 9> get_icache_line(DUT* dut) {
  return {
    dut->icache_resp_qw0_o, dut->icache_resp_qw1_o, dut->icache_resp_qw2_o,
    dut->icache_resp_qw3_o, dut->icache_resp_qw4_o, dut->icache_resp_qw5_o,
    dut->icache_resp_qw6_o, dut->icache_resp_qw7_o, dut->icache_resp_qw8_o
  };
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  clear_inputs(dut);
  sim.reset(2);
  clear_inputs(dut);
  dut->eval();

  sim.check(dut->icache_req_ready_o == 1, "reset leaves mission port ready");
  sim.check(dut->icache_resp_valid_o == 0, "reset clears icache response valid");
  sim.check(dut->apb_pready_o == 0, "reset clears APB pready");
  sim.check(dut->apb_pslverr_o == 0, "APB slverr stays low");

  const uint32_t dbg_word_addr = 7;
  const uint32_t dbg_col = 2;
  const uint32_t dbg_chunk = 1;
  const uint64_t dbg_pattern = 0x1122334455667788ULL;

  dut->apb_paddr_i = make_dbg_addr(dbg_word_addr, dbg_col, dbg_chunk, false);
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  dut->apb_pwdata_i = dbg_pattern;
  sim.check(wait_for_apb_ready(sim), "APB debug write completes");
  sim.check(dut->apb_pready_o == 1, "APB debug write raises pready");
  clear_inputs(dut);
  sim.tick();

  dut->apb_paddr_i = make_dbg_addr(dbg_word_addr, dbg_col, dbg_chunk, false);
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  sim.check(wait_for_apb_ready(sim), "APB debug read completes");
  sim.check(dut->apb_prdata_o == dbg_pattern, "APB debug read returns the written chunk");
  sim.check(dut->apb_pslverr_o == 0, "APB debug read keeps slverr low");
  clear_inputs(dut);
  sim.tick();

  const uint32_t bist_word_addr = 5;
  const uint32_t bist_mem_idx = 1;
  const uint64_t bist_wdata0 = 0x0123456789abcdefULL;
  const uint64_t bist_wdata1 = 0xfedcba9876543210ULL;
  const uint16_t bist_wdata2 = 0x5aa5U;

  dut->bist_req_mbi_sel_i = 1;
  dut->bist_req_mbi_wr_en_i = 1;
  dut->bist_req_mbi_addr_i = make_bist_addr(0, bist_mem_idx, bist_word_addr);
  dut->bist_req_mbi_wdata_0_i = bist_wdata0;
  dut->bist_req_mbi_wdata_1_i = bist_wdata1;
  dut->bist_req_mbi_wdata_2_i = bist_wdata2;
  sim.tick();
  clear_inputs(dut);
  idle_ticks(sim, 8);

  dut->bist_req_mbi_sel_i = 1;
  dut->bist_req_mbi_rd_en_i = 1;
  dut->bist_req_mbi_addr_i = make_bist_addr(0, bist_mem_idx, bist_word_addr);
  sim.tick();
  clear_inputs(dut);
  sim.check(wait_for_bist_rsp(sim), "BIST read produces a transient readback");
  sim.check(dut->bist_rsp_mbi_rdata_0_o == bist_wdata0, "BIST read returns low 64 bits");
  sim.check(dut->bist_rsp_mbi_rdata_1_o == bist_wdata1, "BIST read returns middle 64 bits");
  sim.check((dut->bist_rsp_mbi_rdata_2_o & 0xffffULL) == bist_wdata2, "BIST read returns top 16 bits");

  const uint32_t fill_addr = 9;
  dut->icache_req_write_i = 1;
  dut->icache_req_addr_i = fill_addr;
  dut->icache_req_valid_i = 1;
  dut->icache_resp_ready_i = 1;
  sim.check(dut->icache_req_ready_o == 1, "mission write sees ready before issue");
  sim.tick();
  clear_inputs(dut);
  dut->icache_resp_ready_i = 1;

  dut->neigh_sc_rsp_id_i = 0x3c;
  dut->neigh_sc_rsp_dest_i = kSrcIcache;
  dut->neigh_sc_rsp_wdata_i = 1;
  dut->neigh_sc_rsp_opcode_i = kRspAckData;
  dut->neigh_sc_rsp_data_0_i = 0x0102030405060708ULL;
  dut->neigh_sc_rsp_data_1_i = 0x1112131415161718ULL;
  dut->neigh_sc_rsp_data_2_i = 0x2122232425262728ULL;
  dut->neigh_sc_rsp_data_3_i = 0x3132333435363738ULL;
  dut->neigh_sc_rsp_data_4_i = 0x4142434445464748ULL;
  dut->neigh_sc_rsp_data_5_i = 0x5152535455565758ULL;
  dut->neigh_sc_rsp_data_6_i = 0x6162636465666768ULL;
  dut->neigh_sc_rsp_data_7_i = 0x7172737475767778ULL;
  dut->neigh_sc_rsp_size_i = kSizeLine;
  dut->neigh_sc_rsp_qwen_i = 0xf;
  dut->neigh_sc_rsp_valid_i = 1;
  dut->neigh_sc_rsp_ready_i = 1;
  sim.tick();
  clear_inputs(dut);
  dut->icache_resp_ready_i = 1;

  sim.check(wait_for_icache_resp(sim), "mission write produces an icache response");
  auto write_line = get_icache_line(dut);
  sim.check(dut->icache_resp_valid_o == 1, "mission write response is marked valid");
  sim.tick();

  dut->icache_req_write_i = 0;
  dut->icache_req_addr_i = fill_addr;
  dut->icache_req_valid_i = 1;
  dut->icache_resp_ready_i = 1;
  sim.tick();
  clear_inputs(dut);
  dut->icache_resp_ready_i = 1;
  sim.check(wait_for_icache_resp(sim), "mission read returns data after fill");
  auto read_line = get_icache_line(dut);
  for (int i = 0; i < 9; ++i) {
    sim.check(read_line[i] == write_line[i], "mission readback matches the earlier fill response");
  }

  return sim.finish();
}

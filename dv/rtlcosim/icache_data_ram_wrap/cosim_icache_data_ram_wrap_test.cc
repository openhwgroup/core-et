// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_data_ram_wrap_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_icache_data_ram_wrap_tb;

namespace {

constexpr uint32_t kMemAddrWidth = 9;
constexpr uint32_t kMemIdxWidth = 2;
constexpr uint32_t kIcacheDbgChunkWidth = 1;
constexpr uint32_t kIcacheDbgBlockChunkWidth = 4;
constexpr uint8_t kRspAckData = 1;
constexpr uint8_t kSizeLine = 6;
constexpr uint8_t kSrcIcache = 8;

uint32_t make_bist_addr(uint32_t neigh_idx, uint32_t mem_idx, uint32_t word_addr) {
  return word_addr | (mem_idx << kMemAddrWidth) | (neigh_idx << (kMemAddrWidth + kMemIdxWidth));
}

uint32_t make_dbg_addr(uint32_t word_addr, uint32_t col, uint32_t chunk, bool ecc = false) {
  return (word_addr << kIcacheDbgBlockChunkWidth)
       | (static_cast<uint32_t>(ecc) << (kIcacheDbgBlockChunkWidth - 1))
       | (col << kIcacheDbgChunkWidth)
       | chunk;
}

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

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("icache_req_ready", dut->orig_icache_req_ready_o, dut->new_icache_req_ready_o);
  sim.compare("icache_resp_valid", dut->orig_icache_resp_valid_o, dut->new_icache_resp_valid_o);
  sim.compare("icache_resp_qw0", dut->orig_icache_resp_qw0_o, dut->new_icache_resp_qw0_o);
  sim.compare("icache_resp_qw1", dut->orig_icache_resp_qw1_o, dut->new_icache_resp_qw1_o);
  sim.compare("icache_resp_qw2", dut->orig_icache_resp_qw2_o, dut->new_icache_resp_qw2_o);
  sim.compare("icache_resp_qw3", dut->orig_icache_resp_qw3_o, dut->new_icache_resp_qw3_o);
  sim.compare("icache_resp_qw4", dut->orig_icache_resp_qw4_o, dut->new_icache_resp_qw4_o);
  sim.compare("icache_resp_qw5", dut->orig_icache_resp_qw5_o, dut->new_icache_resp_qw5_o);
  sim.compare("icache_resp_qw6", dut->orig_icache_resp_qw6_o, dut->new_icache_resp_qw6_o);
  sim.compare("icache_resp_qw7", dut->orig_icache_resp_qw7_o, dut->new_icache_resp_qw7_o);
  sim.compare("icache_resp_qw8", dut->orig_icache_resp_qw8_o, dut->new_icache_resp_qw8_o);
  sim.compare("bist_rsp_qw0", dut->orig_bist_rsp_mbi_rdata_0_o, dut->new_bist_rsp_mbi_rdata_0_o);
  sim.compare("bist_rsp_qw1", dut->orig_bist_rsp_mbi_rdata_1_o, dut->new_bist_rsp_mbi_rdata_1_o);
  sim.compare("bist_rsp_qw2", dut->orig_bist_rsp_mbi_rdata_2_o, dut->new_bist_rsp_mbi_rdata_2_o);
  sim.compare("apb_pready", dut->orig_apb_pready_o, dut->new_apb_pready_o);
  sim.compare("apb_prdata", dut->orig_apb_prdata_o, dut->new_apb_prdata_o);
  sim.compare("apb_pslverr", dut->orig_apb_pslverr_o, dut->new_apb_pslverr_o);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();
  std::mt19937_64 rng(0x1ca11ca5eedULL);

  clear_inputs(dut);
  sim.reset(2);

  for (int i = 0; i < 8; ++i) {
    clear_inputs(dut);
    dut->dft_sram_clk_i = i & 1;
    dut->dft_scanmode_i = (i >> 1) & 1;
    dut->dft_scan_reset_n_i = (i >> 2) & 1;
    dut->dft_sram_clk_override_i = (i == 3);
    dut->dft_ram_rei_i = (i == 4);
    dut->dft_ram_wei_i = (i == 5);
    dut->ram_cfg_test_en_i = i & 1;
    dut->ram_cfg_rm_i = i;
    dut->ram_cfg_rme_i = (i >> 1) & 1;
    dut->ram_cfg_ra_i = i & 0x3;
    dut->ram_cfg_wa_i = (i + 1) & 0x3;
    dut->ram_cfg_wpulse_i = i & 0x7;
    dut->ram_cfg_deep_sleep_i = (i == 6);
    dut->ram_cfg_shut_down_i = (i == 7);
    sim.tick();
    compare_outputs(sim);
  }

  clear_inputs(dut);
  dut->apb_paddr_i = make_dbg_addr(7, 2, 1, false);
  dut->apb_pwrite_i = 1;
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  dut->apb_pwdata_i = 0x1122334455667788ULL;
  for (int i = 0; i < 12; ++i) {
    sim.tick();
    compare_outputs(sim);
    if (dut->orig_apb_pready_o && dut->new_apb_pready_o) break;
  }

  clear_inputs(dut);
  dut->apb_paddr_i = make_dbg_addr(7, 2, 1, false);
  dut->apb_psel_i = 1;
  dut->apb_penable_i = 1;
  for (int i = 0; i < 12; ++i) {
    sim.tick();
    compare_outputs(sim);
    if (dut->orig_apb_pready_o && dut->new_apb_pready_o) break;
  }

  clear_inputs(dut);
  dut->bist_req_mbist_on_i = 1;
  dut->bist_req_mbi_sel_i = 1;
  dut->bist_req_mbi_wr_en_i = 1;
  dut->bist_req_mbi_addr_i = make_bist_addr(0, 1, 5);
  dut->bist_req_mbi_wdata_0_i = 0x0123456789abcdefULL;
  dut->bist_req_mbi_wdata_1_i = 0xfedcba9876543210ULL;
  dut->bist_req_mbi_wdata_2_i = 0x5aa5ULL;
  sim.tick();
  compare_outputs(sim);

  clear_inputs(dut);
  dut->bist_req_mbist_on_i = 1;
  dut->bist_req_mbi_sel_i = 1;
  dut->bist_req_mbi_rd_en_i = 1;
  dut->bist_req_mbi_addr_i = make_bist_addr(0, 1, 5);
  sim.tick();
  compare_outputs(sim);
  for (int i = 0; i < 12; ++i) {
    clear_inputs(dut);
    sim.tick();
    compare_outputs(sim);
  }

  clear_inputs(dut);
  dut->icache_req_write_i = 1;
  dut->icache_req_addr_i = 9;
  dut->icache_req_valid_i = 1;
  dut->icache_resp_ready_i = 1;
  sim.tick();
  compare_outputs(sim);

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
  compare_outputs(sim);

  for (int i = 0; i < 12; ++i) {
    clear_inputs(dut);
    dut->icache_resp_ready_i = 1;
    sim.tick();
    compare_outputs(sim);
  }

  for (int cycle = 0; cycle < 5000; ++cycle) {
    dut->dft_sram_clk_i = cycle & 1;
    dut->dft_scanmode_i = rng() & 1U;
    dut->dft_scan_reset_n_i = rng() & 1U;
    dut->dft_sram_clk_override_i = rng() & 1U;
    dut->dft_ram_rei_i = rng() & 1U;
    dut->dft_ram_wei_i = rng() & 1U;
    dut->ram_cfg_test_en_i = rng() & 1U;
    dut->ram_cfg_rm_i = rng() & 0xfU;
    dut->ram_cfg_rme_i = rng() & 1U;
    dut->ram_cfg_ra_i = rng() & 0x3U;
    dut->ram_cfg_wa_i = rng() & 0x3U;
    dut->ram_cfg_wpulse_i = rng() & 0x7U;
    dut->ram_cfg_deep_sleep_i = rng() & 1U;
    dut->ram_cfg_shut_down_i = rng() & 1U;
    dut->icache_req_write_i = rng() & 1U;
    dut->icache_req_addr_i = rng() & 0x1ffU;
    dut->icache_req_valid_i = rng() & 1U;
    dut->icache_resp_ready_i = rng() & 1U;
    dut->neigh_sc_rsp_id_i = rng() & 0xffU;
    dut->neigh_sc_rsp_dest_i = rng() & 0xfU;
    dut->neigh_sc_rsp_wdata_i = rng() & 1U;
    dut->neigh_sc_rsp_opcode_i = rng() & 0x3U;
    dut->neigh_sc_rsp_data_0_i = rng();
    dut->neigh_sc_rsp_data_1_i = rng();
    dut->neigh_sc_rsp_data_2_i = rng();
    dut->neigh_sc_rsp_data_3_i = rng();
    dut->neigh_sc_rsp_data_4_i = rng();
    dut->neigh_sc_rsp_data_5_i = rng();
    dut->neigh_sc_rsp_data_6_i = rng();
    dut->neigh_sc_rsp_data_7_i = rng();
    dut->neigh_sc_rsp_size_i = rng() & 0x7U;
    dut->neigh_sc_rsp_qwen_i = rng() & 0xfU;
    dut->neigh_sc_rsp_valid_i = rng() & 1U;
    dut->neigh_sc_rsp_ready_i = rng() & 1U;
    dut->bist_req_mbist_on_i = rng() & 1U;
    dut->bist_req_mbi_sel_i = rng() & 1U;
    dut->bist_req_mbi_rd_en_i = rng() & 1U;
    dut->bist_req_mbi_wr_en_i = rng() & 1U;
    dut->bist_req_mbi_addr_i = rng() & 0x1fffU;
    dut->bist_req_mbi_wdata_0_i = rng();
    dut->bist_req_mbi_wdata_1_i = rng();
    dut->bist_req_mbi_wdata_2_i = rng();
    dut->apb_paddr_i = rng() & 0x1fffU;
    dut->apb_pwrite_i = rng() & 1U;
    dut->apb_psel_i = rng() & 1U;
    dut->apb_penable_i = rng() & 1U;
    dut->apb_pwdata_i = rng();

    sim.tick();
    compare_outputs(sim);
    if (sim.mismatches > 20) break;
  }

  return sim.finish();
}

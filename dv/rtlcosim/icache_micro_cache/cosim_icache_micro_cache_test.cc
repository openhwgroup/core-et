// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_micro_cache_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

using DUT = Vcosim_icache_micro_cache_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;

using BlockWords = std::array<uint64_t, 8>;

void clear_l1_resp(DUT* dut) {
  dut->l1_miss_resp_early_valid_i = 0;
  dut->l1_miss_resp_valid_i = 0;
  dut->l1_miss_resp_data_0_i = 0;
  dut->l1_miss_resp_data_1_i = 0;
  dut->l1_miss_resp_data_2_i = 0;
  dut->l1_miss_resp_data_3_i = 0;
  dut->l1_miss_resp_data_4_i = 0;
  dut->l1_miss_resp_data_5_i = 0;
  dut->l1_miss_resp_data_6_i = 0;
  dut->l1_miss_resp_data_7_i = 0;
  dut->l1_miss_resp_ecc_err_i = 0;
  dut->l1_miss_resp_l2_err_i = 0;
}

void clear_ptw_resp(DUT* dut) {
  dut->ptw_resp_canceled_req_i = 0;
  dut->ptw_resp_access_fault_i = 0;
  dut->ptw_resp_rsvd_for_hw_i = 0;
  dut->ptw_resp_ppn_i = 0;
  dut->ptw_resp_rsvd_for_sw_i = 0;
  dut->ptw_resp_d_i = 0;
  dut->ptw_resp_a_i = 0;
  dut->ptw_resp_g_i = 0;
  dut->ptw_resp_u_i = 0;
  dut->ptw_resp_x_i = 0;
  dut->ptw_resp_w_i = 0;
  dut->ptw_resp_r_i = 0;
  dut->ptw_resp_v_i = 0;
  dut->ptw_resp_valid_i = 0;
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 1;
  dut->prefetch_prv_i = kPrvS;
  dut->prefetch_start_addr_i = 0;
  dut->prefetch_num_lines_i = 0;
  dut->prefetch_start_i = 0;
  dut->mprot_i = 0;
  dut->vmspagesize_i = 0;
  dut->bypass_icache_i = 0;
  dut->coop_mode_i = 0;
  dut->req_valid_i = 0;
  dut->req_min_id_i = 0;
  dut->req_addr_i = 0;
  dut->req_prv_i = kPrvS;
  dut->req_mprv_i = 0;
  dut->req_mpp_i = kPrvM;
  dut->req_sum_i = 0;
  dut->req_mxr_i = 0;
  dut->req_debug_i = 0;
  dut->flush_data_i = 0;
  dut->l1_miss_req_ready_i = 0;
  clear_l1_resp(dut);
  dut->satp_mode0_i = kModeBare;
  dut->satp_ppn0_i = 0;
  dut->satp_mode1_i = kModeBare;
  dut->satp_ppn1_i = 0;
  dut->matp_mode0_i = kModeBare;
  dut->matp_ppn0_i = 0;
  dut->matp_mode1_i = kModeBare;
  dut->matp_ppn1_i = 0;
  dut->tlb_invalidate_i = 0;
  dut->ptw_req_ready_i = 0;
  clear_ptw_resp(dut);
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("prefetch_done", sim.dut->orig_prefetch_done_o, sim.dut->new_prefetch_done_o);
  sim.compare("req_ready", sim.dut->orig_req_ready_o, sim.dut->new_req_ready_o);
  sim.compare("resp_valid", sim.dut->orig_resp_valid_o, sim.dut->new_resp_valid_o);
  sim.compare("resp_miss", sim.dut->orig_resp_miss_o, sim.dut->new_resp_miss_o);
  sim.compare("resp_data_0", sim.dut->orig_resp_data_0_o, sim.dut->new_resp_data_0_o);
  sim.compare("resp_data_1", sim.dut->orig_resp_data_1_o, sim.dut->new_resp_data_1_o);
  sim.compare("resp_data_2", sim.dut->orig_resp_data_2_o, sim.dut->new_resp_data_2_o);
  sim.compare("resp_data_3", sim.dut->orig_resp_data_3_o, sim.dut->new_resp_data_3_o);
  sim.compare("resp_page_fault", sim.dut->orig_resp_page_fault_o, sim.dut->new_resp_page_fault_o);
  sim.compare("resp_access_fault", sim.dut->orig_resp_access_fault_o, sim.dut->new_resp_access_fault_o);
  sim.compare("resp_cacheable", sim.dut->orig_resp_cacheable_o, sim.dut->new_resp_cacheable_o);
  sim.compare("resp_bus_err", sim.dut->orig_resp_bus_err_o, sim.dut->new_resp_bus_err_o);
  sim.compare("resp_ecc_err", sim.dut->orig_resp_ecc_err_o, sim.dut->new_resp_ecc_err_o);
  sim.compare("resp_fill_done", sim.dut->orig_resp_fill_done_o, sim.dut->new_resp_fill_done_o);
  sim.compare("l1_miss_req_valid", sim.dut->orig_l1_miss_req_valid_o, sim.dut->new_l1_miss_req_valid_o);
  sim.compare("l1_miss_req_addr", sim.dut->orig_l1_miss_req_addr_o, sim.dut->new_l1_miss_req_addr_o);
  sim.compare("ptw_req_satp_mode", sim.dut->orig_ptw_req_satp_mode_o, sim.dut->new_ptw_req_satp_mode_o);
  sim.compare("ptw_req_satp_ppn", sim.dut->orig_ptw_req_satp_ppn_o, sim.dut->new_ptw_req_satp_ppn_o);
  sim.compare("ptw_req_prv", sim.dut->orig_ptw_req_prv_o, sim.dut->new_ptw_req_prv_o);
  sim.compare("ptw_req_addr", sim.dut->orig_ptw_req_addr_o, sim.dut->new_ptw_req_addr_o);
  sim.compare("ptw_req_valid", sim.dut->orig_ptw_req_valid_o, sim.dut->new_ptw_req_valid_o);
  sim.compare("ptw_invalidate", sim.dut->orig_ptw_invalidate_o, sim.dut->new_ptw_invalidate_o);
  sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
  sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
  sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);
  sim.compare("dbg_chunk0", sim.dut->orig_dbg_chunk0_o, sim.dut->new_dbg_chunk0_o);
  sim.compare("dbg_chunk1", sim.dut->orig_dbg_chunk1_o, sim.dut->new_dbg_chunk1_o);
  sim.compare("dbg_chunk2", sim.dut->orig_dbg_chunk2_o, sim.dut->new_dbg_chunk2_o);
  sim.compare("dbg_chunk3", sim.dut->orig_dbg_chunk3_o, sim.dut->new_dbg_chunk3_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_outputs(sim);
}

void apply_reset(CosimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(1);
  clear_inputs(sim.dut.get());
  compare_outputs(sim);
}

void set_block_words(DUT* dut, const BlockWords& words, bool ecc_err, bool l2_err) {
  dut->l1_miss_resp_early_valid_i = 1;
  dut->l1_miss_resp_valid_i = 1;
  dut->l1_miss_resp_data_0_i = words[0];
  dut->l1_miss_resp_data_1_i = words[1];
  dut->l1_miss_resp_data_2_i = words[2];
  dut->l1_miss_resp_data_3_i = words[3];
  dut->l1_miss_resp_data_4_i = words[4];
  dut->l1_miss_resp_data_5_i = words[5];
  dut->l1_miss_resp_data_6_i = words[6];
  dut->l1_miss_resp_data_7_i = words[7];
  dut->l1_miss_resp_ecc_err_i = ecc_err;
  dut->l1_miss_resp_l2_err_i = l2_err;
}

uint64_t rand_addr(std::mt19937_64& rng) {
  switch (rng() & 0x3) {
    case 0: return 0x0040000000ULL + ((rng() & 0xffULL) << 6);
    case 1: return 0x8000001000ULL + ((rng() & 0x3ffULL) << 6);
    case 2: return 0x00000123456000ULL + ((rng() & 0x3fULL) << 12);
    default: return rng() & ((1ULL << 49) - 1);
  }
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 1000000;
  apply_reset(sim);

  BlockWords block_a = {
      0x1111111111111111ULL, 0x2222222222222222ULL, 0x3333333333333333ULL, 0x4444444444444444ULL,
      0x5555555555555555ULL, 0x6666666666666666ULL, 0x7777777777777777ULL, 0x8888888888888888ULL};
  BlockWords block_b = {
      0xABABABABABABABABULL, 0xCDCDCDCDCDCDCDCDULL, 0xEFEFEFEFEFEFEFEFULL, 0x0123456789ABCDEFULL,
      0x13579BDF2468ACE0ULL, 0x0F0E0D0C0B0A0908ULL, 0x1122334455667788ULL, 0x99AABBCCDDEEFF00ULL};

  sim.dut->req_addr_i = 0x0040000000ULL;
  sim.dut->req_prv_i = kPrvS;
  sim.dut->req_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req_valid_i = 0;
  for (int i = 0; i < 6; ++i) tick_and_compare(sim);
  sim.dut->l1_miss_req_ready_i = 1;
  tick_and_compare(sim);
  sim.dut->l1_miss_req_ready_i = 0;
  set_block_words(sim.dut.get(), block_a, false, false);
  tick_and_compare(sim);
  clear_l1_resp(sim.dut.get());
  sim.dut->req_addr_i = 0x0040000000ULL;
  sim.dut->req_prv_i = kPrvS;
  sim.dut->req_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req_valid_i = 0;
  for (int i = 0; i < 4; ++i) tick_and_compare(sim);

  sim.dut->ioshire_i = 0;
  sim.dut->bypass_icache_i = 1;
  sim.dut->req_addr_i = 0x8000001000ULL;
  sim.dut->req_prv_i = kPrvM;
  sim.dut->req_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req_valid_i = 0;
  for (int i = 0; i < 6; ++i) tick_and_compare(sim);
  sim.dut->l1_miss_req_ready_i = 1;
  tick_and_compare(sim);
  sim.dut->l1_miss_req_ready_i = 0;
  set_block_words(sim.dut.get(), block_b, true, true);
  tick_and_compare(sim);
  clear_l1_resp(sim.dut.get());
  sim.dut->req_addr_i = 0x8000001000ULL;
  sim.dut->req_prv_i = kPrvM;
  sim.dut->req_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req_valid_i = 0;
  for (int i = 0; i < 4; ++i) tick_and_compare(sim);

  std::mt19937_64 rng(0x1CA7ECACULL);
  bool pending_l1_resp = false;
  bool pending_ptw_resp = false;
  BlockWords queued_block = {};

  for (int cycle = 0; cycle < 20000; ++cycle) {
    sim.dut->ioshire_i = rng() & 0x1;
    sim.dut->prefetch_prv_i = rng() & 0x3;
    sim.dut->prefetch_start_addr_i = rand_addr(rng) >> 6;
    sim.dut->prefetch_num_lines_i = rng() & 0x3;
    sim.dut->prefetch_start_i = ((rng() & 0x3ff) == 0);
    sim.dut->mprot_i = rng() & 0x7f;
    sim.dut->vmspagesize_i = rng() & 0x3;
    sim.dut->bypass_icache_i = rng() & 0x1;
    sim.dut->coop_mode_i = rng() & 0x1;
    sim.dut->req_valid_i = rng() & 0x1;
    sim.dut->req_min_id_i = rng() & 0x1;
    sim.dut->req_addr_i = rand_addr(rng);
    sim.dut->req_prv_i = rng() & 0x3;
    sim.dut->req_mprv_i = rng() & 0x1;
    sim.dut->req_mpp_i = rng() & 0x3;
    sim.dut->req_sum_i = rng() & 0x1;
    sim.dut->req_mxr_i = rng() & 0x1;
    sim.dut->req_debug_i = rng() & 0x1;
    sim.dut->flush_data_i = ((rng() & 0x3ff) == 0);
    sim.dut->satp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->satp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->tlb_invalidate_i = rng() & 0x3;
    sim.dut->apb_paddr_i = rng() & ((1ULL << 8) - 1);
    sim.dut->apb_pwrite_i = rng() & 0x1;
    sim.dut->apb_psel_i = rng() & 0x1;
    sim.dut->apb_penable_i = rng() & 0x1;
    sim.dut->apb_pwdata_i = rng();

    sim.dut->l1_miss_req_ready_i = rng() & 0x1;
    sim.dut->ptw_req_ready_i = rng() & 0x1;

    clear_l1_resp(sim.dut.get());
    if (pending_l1_resp) {
      set_block_words(sim.dut.get(), queued_block, rng() & 0x1, rng() & 0x1);
      pending_l1_resp = false;
    }

    clear_ptw_resp(sim.dut.get());
    if (pending_ptw_resp) {
      sim.dut->ptw_resp_canceled_req_i = rng() & 0x1;
      sim.dut->ptw_resp_access_fault_i = rng() & 0x1;
      sim.dut->ptw_resp_rsvd_for_hw_i = rng() & 0xff;
      sim.dut->ptw_resp_ppn_i = rng() & ((1ULL << 44) - 1);
      sim.dut->ptw_resp_rsvd_for_sw_i = rng() & 0x3;
      sim.dut->ptw_resp_d_i = rng() & 0x1;
      sim.dut->ptw_resp_a_i = rng() & 0x1;
      sim.dut->ptw_resp_g_i = rng() & 0x1;
      sim.dut->ptw_resp_u_i = rng() & 0x1;
      sim.dut->ptw_resp_x_i = rng() & 0x1;
      sim.dut->ptw_resp_w_i = rng() & 0x1;
      sim.dut->ptw_resp_r_i = rng() & 0x1;
      sim.dut->ptw_resp_v_i = rng() & 0x1;
      sim.dut->ptw_resp_valid_i = 1;
      pending_ptw_resp = false;
    }

    tick_and_compare(sim);

    if (sim.dut->new_l1_miss_req_valid_o && sim.dut->l1_miss_req_ready_i) {
      for (int i = 0; i < 8; ++i) queued_block[i] = rng();
      pending_l1_resp = true;
    }
    if (sim.dut->new_ptw_req_valid_o && sim.dut->ptw_req_ready_i) {
      pending_ptw_resp = true;
    }
  }

  return sim.finish();
}

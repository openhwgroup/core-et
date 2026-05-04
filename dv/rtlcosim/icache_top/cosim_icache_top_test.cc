// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_top_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <random>

using DUT = Vcosim_icache_top_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kRspAckData = 1;

using BlockWords = std::array<uint64_t, 8>;

void clear_l2_resp(DUT* dut) {
  dut->l2_miss_resp_valid_i = 0;
  dut->l2_miss_resp_opcode_i = 0;
}

void clear_ptw0_resp(DUT* dut) {
  dut->ptw0_resp_canceled_req_i = 0;
  dut->ptw0_resp_access_fault_i = 0;
  dut->ptw0_resp_rsvd_for_hw_i = 0;
  dut->ptw0_resp_ppn_i = 0;
  dut->ptw0_resp_rsvd_for_sw_i = 0;
  dut->ptw0_resp_d_i = 0;
  dut->ptw0_resp_a_i = 0;
  dut->ptw0_resp_g_i = 0;
  dut->ptw0_resp_u_i = 0;
  dut->ptw0_resp_x_i = 0;
  dut->ptw0_resp_w_i = 0;
  dut->ptw0_resp_r_i = 0;
  dut->ptw0_resp_v_i = 0;
  dut->ptw0_resp_valid_i = 0;
}

void clear_ptw1_resp(DUT* dut) {
  dut->ptw1_resp_canceled_req_i = 0;
  dut->ptw1_resp_access_fault_i = 0;
  dut->ptw1_resp_rsvd_for_hw_i = 0;
  dut->ptw1_resp_ppn_i = 0;
  dut->ptw1_resp_rsvd_for_sw_i = 0;
  dut->ptw1_resp_d_i = 0;
  dut->ptw1_resp_a_i = 0;
  dut->ptw1_resp_g_i = 0;
  dut->ptw1_resp_u_i = 0;
  dut->ptw1_resp_x_i = 0;
  dut->ptw1_resp_w_i = 0;
  dut->ptw1_resp_r_i = 0;
  dut->ptw1_resp_v_i = 0;
  dut->ptw1_resp_valid_i = 0;
}

void clear_sram_resp(DUT* dut) {
  dut->sram_resp_valid_i = 0;
  dut->sram_resp_qw0_i = 0;
  dut->sram_resp_qw1_i = 0;
  dut->sram_resp_qw2_i = 0;
  dut->sram_resp_qw3_i = 0;
  dut->sram_resp_qw4_i = 0;
  dut->sram_resp_qw5_i = 0;
  dut->sram_resp_qw6_i = 0;
  dut->sram_resp_qw7_i = 0;
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 1;
  dut->prefetch_prv_i = kPrvS;
  dut->prefetch_start_addr_i = 0;
  dut->prefetch_num_lines_i = 0;
  dut->prefetch_start_i = 0;
  dut->err_rsp_enable_i = 0;
  dut->err_interrupt_enable_i = 0;
  dut->mprot_i = 0;
  dut->vmspagesize_i = 0;
  dut->bypass_icache_i = 0;
  dut->coop_mode_i = 0;
  dut->req0_valid_i = 0;
  dut->req0_min_id_i = 0;
  dut->req0_addr_i = 0;
  dut->req0_prv_i = kPrvS;
  dut->req0_mprv_i = 0;
  dut->req0_mpp_i = kPrvM;
  dut->req0_sum_i = 0;
  dut->req0_mxr_i = 0;
  dut->req0_debug_i = 0;
  dut->req1_valid_i = 0;
  dut->req1_min_id_i = 0;
  dut->req1_addr_i = 0;
  dut->req1_prv_i = kPrvS;
  dut->req1_mprv_i = 0;
  dut->req1_mpp_i = kPrvM;
  dut->req1_sum_i = 0;
  dut->req1_mxr_i = 0;
  dut->req1_debug_i = 0;
  dut->flush_data_i = 0;
  dut->l2_miss_req_disable_next_i = 0;
  dut->l2_miss_req_ready_i = 0;
  clear_l2_resp(dut);
  dut->satp_mode0_i = kModeBare;
  dut->satp_ppn0_i = 0;
  dut->satp_mode1_i = kModeBare;
  dut->satp_ppn1_i = 0;
  dut->matp_mode0_i = kModeBare;
  dut->matp_ppn0_i = 0;
  dut->matp_mode1_i = kModeBare;
  dut->matp_ppn1_i = 0;
  dut->tlb_invalidate_i = 0;
  dut->ptw0_req_ready_i = 0;
  dut->ptw1_req_ready_i = 0;
  clear_ptw0_resp(dut);
  clear_ptw1_resp(dut);
  dut->sram_req_ready_i = 1;
  clear_sram_resp(dut);
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("prefetch_done", sim.dut->orig_prefetch_done_o, sim.dut->new_prefetch_done_o);
  sim.compare("err_log_sbe", sim.dut->orig_err_log_sbe_o, sim.dut->new_err_log_sbe_o);
  sim.compare("err_log_dbe", sim.dut->orig_err_log_dbe_o, sim.dut->new_err_log_dbe_o);
  sim.compare("err_log_way", sim.dut->orig_err_log_way_o, sim.dut->new_err_log_way_o);
  sim.compare("err_log_set", sim.dut->orig_err_log_set_o, sim.dut->new_err_log_set_o);
  sim.compare("err_log_sbe_bits", sim.dut->orig_err_log_sbe_bits_o, sim.dut->new_err_log_sbe_bits_o);
  sim.compare("err_log_dbe_bits", sim.dut->orig_err_log_dbe_bits_o, sim.dut->new_err_log_dbe_bits_o);
  sim.compare("err_log_addr", sim.dut->orig_err_log_addr_o, sim.dut->new_err_log_addr_o);
  sim.compare("req0_ready", sim.dut->orig_req0_ready_o, sim.dut->new_req0_ready_o);
  sim.compare("req1_ready", sim.dut->orig_req1_ready_o, sim.dut->new_req1_ready_o);
  sim.compare("resp0_valid", sim.dut->orig_resp0_valid_o, sim.dut->new_resp0_valid_o);
  sim.compare("resp0_miss", sim.dut->orig_resp0_miss_o, sim.dut->new_resp0_miss_o);
  sim.compare("resp0_data_0", sim.dut->orig_resp0_data_0_o, sim.dut->new_resp0_data_0_o);
  sim.compare("resp0_data_1", sim.dut->orig_resp0_data_1_o, sim.dut->new_resp0_data_1_o);
  sim.compare("resp0_data_2", sim.dut->orig_resp0_data_2_o, sim.dut->new_resp0_data_2_o);
  sim.compare("resp0_data_3", sim.dut->orig_resp0_data_3_o, sim.dut->new_resp0_data_3_o);
  sim.compare("resp0_page_fault", sim.dut->orig_resp0_page_fault_o, sim.dut->new_resp0_page_fault_o);
  sim.compare("resp0_access_fault", sim.dut->orig_resp0_access_fault_o, sim.dut->new_resp0_access_fault_o);
  sim.compare("resp0_cacheable", sim.dut->orig_resp0_cacheable_o, sim.dut->new_resp0_cacheable_o);
  sim.compare("resp0_bus_err", sim.dut->orig_resp0_bus_err_o, sim.dut->new_resp0_bus_err_o);
  sim.compare("resp0_ecc_err", sim.dut->orig_resp0_ecc_err_o, sim.dut->new_resp0_ecc_err_o);
  sim.compare("resp0_fill_done", sim.dut->orig_resp0_fill_done_o, sim.dut->new_resp0_fill_done_o);
  sim.compare("resp1_valid", sim.dut->orig_resp1_valid_o, sim.dut->new_resp1_valid_o);
  sim.compare("resp1_miss", sim.dut->orig_resp1_miss_o, sim.dut->new_resp1_miss_o);
  sim.compare("resp1_data_0", sim.dut->orig_resp1_data_0_o, sim.dut->new_resp1_data_0_o);
  sim.compare("resp1_data_1", sim.dut->orig_resp1_data_1_o, sim.dut->new_resp1_data_1_o);
  sim.compare("resp1_data_2", sim.dut->orig_resp1_data_2_o, sim.dut->new_resp1_data_2_o);
  sim.compare("resp1_data_3", sim.dut->orig_resp1_data_3_o, sim.dut->new_resp1_data_3_o);
  sim.compare("resp1_page_fault", sim.dut->orig_resp1_page_fault_o, sim.dut->new_resp1_page_fault_o);
  sim.compare("resp1_access_fault", sim.dut->orig_resp1_access_fault_o, sim.dut->new_resp1_access_fault_o);
  sim.compare("resp1_cacheable", sim.dut->orig_resp1_cacheable_o, sim.dut->new_resp1_cacheable_o);
  sim.compare("resp1_bus_err", sim.dut->orig_resp1_bus_err_o, sim.dut->new_resp1_bus_err_o);
  sim.compare("resp1_ecc_err", sim.dut->orig_resp1_ecc_err_o, sim.dut->new_resp1_ecc_err_o);
  sim.compare("resp1_fill_done", sim.dut->orig_resp1_fill_done_o, sim.dut->new_resp1_fill_done_o);
  sim.compare("l2_req_valid", sim.dut->orig_l2_miss_req_valid_o, sim.dut->new_l2_miss_req_valid_o);
  sim.compare("l2_req_addr", sim.dut->orig_l2_miss_req_addr_o, sim.dut->new_l2_miss_req_addr_o);
  sim.compare("l2_req_opcode", sim.dut->orig_l2_miss_req_opcode_o, sim.dut->new_l2_miss_req_opcode_o);
  sim.compare("l2_req_size", sim.dut->orig_l2_miss_req_size_o, sim.dut->new_l2_miss_req_size_o);
  sim.compare("l2_req_subopcode", sim.dut->orig_l2_miss_req_subopcode_o, sim.dut->new_l2_miss_req_subopcode_o);
  sim.compare("l2_resp_ready", sim.dut->orig_l2_miss_resp_ready_o, sim.dut->new_l2_miss_resp_ready_o);
  sim.compare("ptw0_mode", sim.dut->orig_ptw0_req_satp_mode_o, sim.dut->new_ptw0_req_satp_mode_o);
  sim.compare("ptw0_ppn", sim.dut->orig_ptw0_req_satp_ppn_o, sim.dut->new_ptw0_req_satp_ppn_o);
  sim.compare("ptw0_prv", sim.dut->orig_ptw0_req_prv_o, sim.dut->new_ptw0_req_prv_o);
  sim.compare("ptw0_addr", sim.dut->orig_ptw0_req_addr_o, sim.dut->new_ptw0_req_addr_o);
  sim.compare("ptw0_valid", sim.dut->orig_ptw0_req_valid_o, sim.dut->new_ptw0_req_valid_o);
  sim.compare("ptw0_inv", sim.dut->orig_ptw0_invalidate_o, sim.dut->new_ptw0_invalidate_o);
  sim.compare("ptw1_mode", sim.dut->orig_ptw1_req_satp_mode_o, sim.dut->new_ptw1_req_satp_mode_o);
  sim.compare("ptw1_ppn", sim.dut->orig_ptw1_req_satp_ppn_o, sim.dut->new_ptw1_req_satp_ppn_o);
  sim.compare("ptw1_prv", sim.dut->orig_ptw1_req_prv_o, sim.dut->new_ptw1_req_prv_o);
  sim.compare("ptw1_addr", sim.dut->orig_ptw1_req_addr_o, sim.dut->new_ptw1_req_addr_o);
  sim.compare("ptw1_valid", sim.dut->orig_ptw1_req_valid_o, sim.dut->new_ptw1_req_valid_o);
  sim.compare("ptw1_inv", sim.dut->orig_ptw1_invalidate_o, sim.dut->new_ptw1_invalidate_o);
  sim.compare("sram_req_write", sim.dut->orig_sram_req_write_o, sim.dut->new_sram_req_write_o);
  sim.compare("sram_req_addr", sim.dut->orig_sram_req_addr_o, sim.dut->new_sram_req_addr_o);
  sim.compare("sram_req_valid", sim.dut->orig_sram_req_valid_o, sim.dut->new_sram_req_valid_o);
  sim.compare("sram_resp_ready", sim.dut->orig_sram_resp_ready_o, sim.dut->new_sram_resp_ready_o);
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

void set_block_words(DUT* dut, const BlockWords& words) {
  dut->sram_resp_valid_i = 1;
  dut->sram_resp_qw0_i = words[0];
  dut->sram_resp_qw1_i = words[1];
  dut->sram_resp_qw2_i = words[2];
  dut->sram_resp_qw3_i = words[3];
  dut->sram_resp_qw4_i = words[4];
  dut->sram_resp_qw5_i = words[5];
  dut->sram_resp_qw6_i = words[6];
  dut->sram_resp_qw7_i = words[7];
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

  BlockWords zeros = {0, 0, 0, 0, 0, 0, 0, 0};

  sim.dut->req0_addr_i = 0x0040000000ULL;
  sim.dut->req0_prv_i = kPrvS;
  sim.dut->req0_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req0_valid_i = 0;
  for (int i = 0; i < 6; ++i) tick_and_compare(sim);
  sim.dut->l2_miss_req_ready_i = 1;
  tick_and_compare(sim);
  sim.dut->l2_miss_req_ready_i = 0;
  sim.dut->l2_miss_resp_opcode_i = kRspAckData;
  sim.dut->l2_miss_resp_valid_i = 1;
  tick_and_compare(sim);
  clear_l2_resp(sim.dut.get());
  set_block_words(sim.dut.get(), zeros);
  tick_and_compare(sim);
  clear_sram_resp(sim.dut.get());
  sim.dut->req0_addr_i = 0x0040000000ULL;
  sim.dut->req0_prv_i = kPrvS;
  sim.dut->req0_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req0_valid_i = 0;
  for (int i = 0; i < 8; ++i) tick_and_compare(sim);

  sim.dut->ioshire_i = 0;
  sim.dut->satp_mode1_i = kModeSv39;
  sim.dut->satp_ppn1_i = 0x0010000;
  sim.dut->req1_addr_i = 0x00000123456000ULL;
  sim.dut->req1_prv_i = kPrvS;
  sim.dut->req1_valid_i = 1;
  tick_and_compare(sim);
  sim.dut->req1_valid_i = 0;
  for (int i = 0; i < 8; ++i) tick_and_compare(sim);

  std::mt19937_64 rng(0x1CA7E70ULL);
  for (int cycle = 0; cycle < 20000; ++cycle) {
    sim.dut->ioshire_i = rng() & 0x1;
    sim.dut->prefetch_prv_i = rng() & 0x3;
    sim.dut->prefetch_start_addr_i = rand_addr(rng) >> 6;
    sim.dut->prefetch_num_lines_i = rng() & 0x3f;
    sim.dut->prefetch_start_i = ((rng() & 0x3ff) == 0);
    sim.dut->err_rsp_enable_i = rng() & 0x1;
    sim.dut->err_interrupt_enable_i = rng() & 0x7;
    sim.dut->mprot_i = rng() & 0x7f;
    sim.dut->vmspagesize_i = rng() & 0x3;
    sim.dut->bypass_icache_i = rng() & 0x1;
    sim.dut->coop_mode_i = rng() & 0x1;
    sim.dut->req0_valid_i = rng() & 0x1;
    sim.dut->req0_min_id_i = 0;
    sim.dut->req0_addr_i = rand_addr(rng);
    sim.dut->req0_prv_i = rng() & 0x3;
    sim.dut->req0_mprv_i = rng() & 0x1;
    sim.dut->req0_mpp_i = rng() & 0x3;
    sim.dut->req0_sum_i = rng() & 0x1;
    sim.dut->req0_mxr_i = rng() & 0x1;
    sim.dut->req0_debug_i = rng() & 0x1;
    sim.dut->req1_valid_i = rng() & 0x1;
    sim.dut->req1_min_id_i = 0;
    sim.dut->req1_addr_i = rand_addr(rng);
    sim.dut->req1_prv_i = rng() & 0x3;
    sim.dut->req1_mprv_i = rng() & 0x1;
    sim.dut->req1_mpp_i = rng() & 0x3;
    sim.dut->req1_sum_i = rng() & 0x1;
    sim.dut->req1_mxr_i = rng() & 0x1;
    sim.dut->req1_debug_i = rng() & 0x1;
    sim.dut->flush_data_i = ((rng() & 0x7ff) == 0);
    sim.dut->l2_miss_req_disable_next_i = ((rng() & 0x7f) == 0);
    sim.dut->l2_miss_req_ready_i = rng() & 0x1;
    sim.dut->l2_miss_resp_valid_i = rng() & 0x1;
    sim.dut->l2_miss_resp_opcode_i = rng() & 0x3;
    sim.dut->satp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->satp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->tlb_invalidate_i = rng() & 0x3;
    sim.dut->ptw0_req_ready_i = rng() & 0x1;
    sim.dut->ptw1_req_ready_i = rng() & 0x1;
    sim.dut->ptw0_resp_canceled_req_i = rng() & 0x1;
    sim.dut->ptw0_resp_access_fault_i = rng() & 0x1;
    sim.dut->ptw0_resp_rsvd_for_hw_i = rng();
    sim.dut->ptw0_resp_ppn_i = rng() & ((1ULL << 28) - 1);
    sim.dut->ptw0_resp_rsvd_for_sw_i = rng() & 0x3;
    sim.dut->ptw0_resp_d_i = rng() & 0x1;
    sim.dut->ptw0_resp_a_i = rng() & 0x1;
    sim.dut->ptw0_resp_g_i = rng() & 0x1;
    sim.dut->ptw0_resp_u_i = rng() & 0x1;
    sim.dut->ptw0_resp_x_i = rng() & 0x1;
    sim.dut->ptw0_resp_w_i = rng() & 0x1;
    sim.dut->ptw0_resp_r_i = rng() & 0x1;
    sim.dut->ptw0_resp_v_i = rng() & 0x1;
    sim.dut->ptw0_resp_valid_i = rng() & 0x1;
    sim.dut->ptw1_resp_canceled_req_i = rng() & 0x1;
    sim.dut->ptw1_resp_access_fault_i = rng() & 0x1;
    sim.dut->ptw1_resp_rsvd_for_hw_i = rng();
    sim.dut->ptw1_resp_ppn_i = rng() & ((1ULL << 28) - 1);
    sim.dut->ptw1_resp_rsvd_for_sw_i = rng() & 0x3;
    sim.dut->ptw1_resp_d_i = rng() & 0x1;
    sim.dut->ptw1_resp_a_i = rng() & 0x1;
    sim.dut->ptw1_resp_g_i = rng() & 0x1;
    sim.dut->ptw1_resp_u_i = rng() & 0x1;
    sim.dut->ptw1_resp_x_i = rng() & 0x1;
    sim.dut->ptw1_resp_w_i = rng() & 0x1;
    sim.dut->ptw1_resp_r_i = rng() & 0x1;
    sim.dut->ptw1_resp_v_i = rng() & 0x1;
    sim.dut->ptw1_resp_valid_i = rng() & 0x1;
    sim.dut->sram_req_ready_i = rng() & 0x1;
    sim.dut->sram_resp_valid_i = rng() & 0x1;
    sim.dut->sram_resp_qw0_i = rng();
    sim.dut->sram_resp_qw1_i = rng();
    sim.dut->sram_resp_qw2_i = rng();
    sim.dut->sram_resp_qw3_i = rng();
    sim.dut->sram_resp_qw4_i = rng();
    sim.dut->sram_resp_qw5_i = rng();
    sim.dut->sram_resp_qw6_i = rng();
    sim.dut->sram_resp_qw7_i = rng();
    sim.dut->apb_paddr_i = rng() & ((1U << 10) - 1);
    sim.dut->apb_pwrite_i = rng() & 0x1;
    sim.dut->apb_psel_i = rng() & 0x1;
    sim.dut->apb_penable_i = rng() & 0x1;
    sim.dut->apb_pwdata_i = rng();

    tick_and_compare(sim);
  }

  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_tlb_array vs minion_dcache_tlb_array.

#include "Vcosim_minion_dcache_tlb_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_tlb_array_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;

struct BoolCoverage {
  bool seen0 = false;
  bool seen1 = false;
  void sample(bool v) { if (v) seen1 = true; else seen0 = true; }
  bool full() const { return seen0 && seen1; }
};

struct Coverage {
  BoolCoverage req_mprv, req_sum, req_mxr, req_debug, req_passthrough, req_store;
  BoolCoverage req_msb_err, req_valid, satp_info_en, matp_info_en, tlb_invalidate;
  BoolCoverage ptw_req_ready, ptw_resp_canceled_req, ptw_resp_access_fault, ptw_resp_d;
  BoolCoverage ptw_resp_a, ptw_resp_u, ptw_resp_x, ptw_resp_w, ptw_resp_r, ptw_resp_v, ptw_resp_valid;
  bool seen_satp_bare = false, seen_satp_sv39 = false;
  bool seen_matp_bare = false, seen_matp_sv39 = false;
  bool seen_prv_u = false, seen_prv_s = false, seen_prv_m = false;
};

void sample_coverage(DUT* dut, Coverage& cov) {
  cov.req_mprv.sample(dut->req_mprv_i);
  cov.req_sum.sample(dut->req_sum_i);
  cov.req_mxr.sample(dut->req_mxr_i);
  cov.req_debug.sample(dut->req_debug_i);
  cov.req_passthrough.sample(dut->req_passthrough_i);
  cov.req_store.sample(dut->req_store_i);
  cov.req_msb_err.sample(dut->req_msb_err_i);
  cov.req_valid.sample(dut->req_valid_i);
  cov.satp_info_en.sample(dut->satp_info_en_i);
  cov.matp_info_en.sample(dut->matp_info_en_i);
  cov.tlb_invalidate.sample(dut->tlb_invalidate_i);
  cov.ptw_req_ready.sample(dut->ptw_req_ready_i);
  cov.ptw_resp_canceled_req.sample(dut->ptw_resp_canceled_req_i);
  cov.ptw_resp_access_fault.sample(dut->ptw_resp_access_fault_i);
  cov.ptw_resp_d.sample(dut->ptw_resp_d_i);
  cov.ptw_resp_a.sample(dut->ptw_resp_a_i);
  cov.ptw_resp_u.sample(dut->ptw_resp_u_i);
  cov.ptw_resp_x.sample(dut->ptw_resp_x_i);
  cov.ptw_resp_w.sample(dut->ptw_resp_w_i);
  cov.ptw_resp_r.sample(dut->ptw_resp_r_i);
  cov.ptw_resp_v.sample(dut->ptw_resp_v_i);
  cov.ptw_resp_valid.sample(dut->ptw_resp_valid_i);
  if (dut->satp_mode_i == kModeBare) cov.seen_satp_bare = true;
  if (dut->satp_mode_i == kModeSv39) cov.seen_satp_sv39 = true;
  if (dut->matp_mode_i == kModeBare) cov.seen_matp_bare = true;
  if (dut->matp_mode_i == kModeSv39) cov.seen_matp_sv39 = true;
  if (dut->req_prv_i == kPrvU) cov.seen_prv_u = true;
  if (dut->req_prv_i == kPrvS) cov.seen_prv_s = true;
  if (dut->req_prv_i == kPrvM) cov.seen_prv_m = true;
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
  sim.check(cov.req_mprv.full(), "coverage: req_mprv exercised 0/1");
  sim.check(cov.req_sum.full(), "coverage: req_sum exercised 0/1");
  sim.check(cov.req_mxr.full(), "coverage: req_mxr exercised 0/1");
  sim.check(cov.req_debug.full(), "coverage: req_debug exercised 0/1");
  sim.check(cov.req_passthrough.full(), "coverage: req_passthrough exercised 0/1");
  sim.check(cov.req_store.full(), "coverage: req_store exercised 0/1");
  sim.check(cov.req_msb_err.full(), "coverage: req_msb_err exercised 0/1");
  sim.check(cov.req_valid.full(), "coverage: req_valid exercised 0/1");
  sim.check(cov.satp_info_en.full(), "coverage: satp_info_en exercised 0/1");
  sim.check(cov.matp_info_en.full(), "coverage: matp_info_en exercised 0/1");
  sim.check(cov.tlb_invalidate.full(), "coverage: tlb_invalidate exercised 0/1");
  sim.check(cov.ptw_req_ready.full(), "coverage: ptw_req_ready exercised 0/1");
  sim.check(cov.ptw_resp_canceled_req.full(), "coverage: ptw_resp_canceled_req exercised 0/1");
  sim.check(cov.ptw_resp_access_fault.full(), "coverage: ptw_resp_access_fault exercised 0/1");
  sim.check(cov.ptw_resp_d.full(), "coverage: ptw_resp_d exercised 0/1");
  sim.check(cov.ptw_resp_a.full(), "coverage: ptw_resp_a exercised 0/1");
  sim.check(cov.ptw_resp_u.full(), "coverage: ptw_resp_u exercised 0/1");
  sim.check(cov.ptw_resp_x.full(), "coverage: ptw_resp_x exercised 0/1");
  sim.check(cov.ptw_resp_w.full(), "coverage: ptw_resp_w exercised 0/1");
  sim.check(cov.ptw_resp_r.full(), "coverage: ptw_resp_r exercised 0/1");
  sim.check(cov.ptw_resp_v.full(), "coverage: ptw_resp_v exercised 0/1");
  sim.check(cov.ptw_resp_valid.full(), "coverage: ptw_resp_valid exercised 0/1");
  sim.check(cov.seen_satp_bare && cov.seen_satp_sv39, "coverage: SATP bare/sv39 exercised");
  sim.check(cov.seen_matp_bare && cov.seen_matp_sv39, "coverage: MATP bare/sv39 exercised");
  sim.check(cov.seen_prv_u && cov.seen_prv_s && cov.seen_prv_m, "coverage: req_prv exercised U/S/M");
}

void clear_inputs(DUT* dut) {
  dut->dft_scanmode_i = 0;
  dut->esr_vmspagesize_i = 0;
  dut->req_prv_i = kPrvS;
  dut->req_mprv_i = 0;
  dut->req_mpp_i = kPrvM;
  dut->req_sum_i = 0;
  dut->req_mxr_i = 0;
  dut->req_debug_i = 0;
  dut->req_vpn_i = 0;
  dut->req_passthrough_i = 0;
  dut->req_store_i = 0;
  dut->req_msb_err_i = 0;
  dut->req_valid_i = 0;
  dut->satp_mode_i = kModeBare;
  dut->satp_ppn_i = 0;
  dut->matp_mode_i = kModeBare;
  dut->matp_ppn_i = 0;
  dut->satp_info_en_i = 0;
  dut->matp_info_en_i = 0;
  dut->tlb_invalidate_i = 0;
  dut->ptw_req_ready_i = 0;
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

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("resp_fill_pending", sim.dut->orig_resp_fill_pending_o, sim.dut->new_resp_fill_pending_o);
  sim.compare("resp_miss", sim.dut->orig_resp_miss_o, sim.dut->new_resp_miss_o);
  sim.compare("resp_ppn", sim.dut->orig_resp_ppn_o, sim.dut->new_resp_ppn_o);
  sim.compare("resp_xcpt_ld", sim.dut->orig_resp_xcpt_ld_o, sim.dut->new_resp_xcpt_ld_o);
  sim.compare("resp_xcpt_st", sim.dut->orig_resp_xcpt_st_o, sim.dut->new_resp_xcpt_st_o);
  sim.compare("resp_access_fault", sim.dut->orig_resp_access_fault_o, sim.dut->new_resp_access_fault_o);
  sim.compare("ptw_req_satp_mode", sim.dut->orig_ptw_req_satp_mode_o, sim.dut->new_ptw_req_satp_mode_o);
  sim.compare("ptw_req_satp_ppn", sim.dut->orig_ptw_req_satp_ppn_o, sim.dut->new_ptw_req_satp_ppn_o);
  sim.compare("ptw_req_prv", sim.dut->orig_ptw_req_prv_o, sim.dut->new_ptw_req_prv_o);
  sim.compare("ptw_req_addr", sim.dut->orig_ptw_req_addr_o, sim.dut->new_ptw_req_addr_o);
  sim.compare("ptw_req_valid", sim.dut->orig_ptw_req_valid_o, sim.dut->new_ptw_req_valid_o);
  sim.compare("vm_enabled", sim.dut->orig_vm_enabled_o, sim.dut->new_vm_enabled_o);
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

  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x123456789ULL;
  tick_and_compare(sim, &coverage);

  clear_inputs(sim.dut.get());
  sim.dut->satp_info_en_i = 1;
  tick_and_compare(sim, &coverage);
  clear_inputs(sim.dut.get());
  sim.dut->satp_mode_i = kModeSv39;
  sim.dut->satp_ppn_i = 0x0011111;
  tick_and_compare(sim, &coverage);

  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x000ABCDEULL;
  tick_and_compare(sim, &coverage);
  sim.dut->req_valid_i = 0;
  sim.dut->ptw_req_ready_i = 1;
  tick_and_compare(sim, &coverage);
  sim.dut->ptw_req_ready_i = 0;
  sim.dut->ptw_resp_ppn_i = 0x000ABCDE;
  sim.dut->ptw_resp_d_i = 1;
  sim.dut->ptw_resp_a_i = 1;
  sim.dut->ptw_resp_r_i = 1;
  sim.dut->ptw_resp_w_i = 1;
  sim.dut->ptw_resp_v_i = 1;
  sim.dut->ptw_resp_valid_i = 1;
  tick_and_compare(sim, &coverage);
  clear_inputs(sim.dut.get());
  tick_and_compare(sim, &coverage);

  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x000ABCDEULL;
  tick_and_compare(sim, &coverage);

  clear_inputs(sim.dut.get());
  sim.dut->matp_info_en_i = 1;
  tick_and_compare(sim, &coverage);
  clear_inputs(sim.dut.get());
  sim.dut->matp_mode_i = kModeSv39;
  sim.dut->matp_ppn_i = 0x0022222;
  tick_and_compare(sim, &coverage);

  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x000FEDCBULL;
  sim.dut->req_mprv_i = 1;
  sim.dut->req_mpp_i = kPrvM;
  sim.dut->req_debug_i = 1;
  tick_and_compare(sim, &coverage);

  constexpr uint32_t kSeeds[] = {0x13579BDFu, 0xC001CAFEu, 0x5F3759DFu};
  for (uint32_t seed_init : kSeeds) {
    uint32_t seed = seed_init;
    for (int cycle = 0; cycle < 2048; ++cycle) {
      sim.dut->dft_scanmode_i = xorshift32(seed) & 1u;
      sim.dut->esr_vmspagesize_i = xorshift32(seed) & 0x3u;
      sim.dut->req_prv_i = xorshift32(seed) & 0x3u;
      sim.dut->req_mprv_i = xorshift32(seed) & 1u;
      sim.dut->req_mpp_i = xorshift32(seed) & 0x3u;
      sim.dut->req_sum_i = xorshift32(seed) & 1u;
      sim.dut->req_mxr_i = xorshift32(seed) & 1u;
      sim.dut->req_debug_i = xorshift32(seed) & 1u;
      sim.dut->req_vpn_i = ((uint64_t(xorshift32(seed)) << 4) ^ xorshift32(seed)) & ((1ULL << 36) - 1);
      sim.dut->req_passthrough_i = xorshift32(seed) & 1u;
      sim.dut->req_store_i = xorshift32(seed) & 1u;
      sim.dut->req_msb_err_i = xorshift32(seed) & 1u;
      sim.dut->req_valid_i = xorshift32(seed) & 1u;
      sim.dut->satp_mode_i = (xorshift32(seed) & 1u) ? kModeSv39 : kModeBare;
      sim.dut->satp_ppn_i = xorshift32(seed) & ((1u << 28) - 1);
      sim.dut->matp_mode_i = (xorshift32(seed) & 1u) ? kModeSv39 : kModeBare;
      sim.dut->matp_ppn_i = xorshift32(seed) & ((1u << 28) - 1);
      sim.dut->satp_info_en_i = xorshift32(seed) & 1u;
      sim.dut->matp_info_en_i = xorshift32(seed) & 1u;
      sim.dut->tlb_invalidate_i = xorshift32(seed) & 1u;
      sim.dut->ptw_req_ready_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_canceled_req_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_access_fault_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_rsvd_for_hw_i = xorshift32(seed) & 0xFFu;
      sim.dut->ptw_resp_ppn_i =
          ((uint64_t(xorshift32(seed)) << 12) ^ xorshift32(seed)) & ((1ULL << 44) - 1);
      sim.dut->ptw_resp_rsvd_for_sw_i = xorshift32(seed) & 0x3u;
      sim.dut->ptw_resp_d_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_a_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_g_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_u_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_x_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_w_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_r_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_v_i = xorshift32(seed) & 1u;
      sim.dut->ptw_resp_valid_i = xorshift32(seed) & 1u;
      tick_and_compare(sim, &coverage);
    }
  }

  check_coverage(sim, coverage);
  return sim.finish();
}

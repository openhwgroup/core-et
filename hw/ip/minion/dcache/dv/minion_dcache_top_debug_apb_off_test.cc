// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// DebugApbEn=0 APB error-path test for minion_dcache_top.

#include "Vminion_dcache_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vminion_dcache_top_tb;

namespace {

constexpr uint8_t kPrvS = 1;
constexpr uint16_t kMetadataDebugAddr = 0x0000;
constexpr uint16_t kDataDebugAddr = 0x0200;

void clear_inputs(DUT* dut) {
  dut->id_core_alloc_rq_pre_i = 0;
  dut->s0_core_alloc_rq_val_i = 0;
  dut->id_core_gsc_i = 0;
  dut->s0_core_req_valid_i = 0;
  dut->req_addr_i = 0;
  dut->req_addr_msb_err_i = 0;
  dut->req_dest_fp_i = 0;
  dut->req_dest_addr_i = 0;
  dut->req_thread_id_i = 0;
  dut->req_cmd_i = 0;
  dut->req_typ_i = 0;
  dut->store_data_lo_i = 0;
  dut->req_gsc_cnt_i = 0;
  dut->req_ps_mask_i = 0xff;
  dut->req_phys_i = 0;
  dut->req_mem_global_i = 0;
  dut->req_gsc32_idx_i = 0;
  dut->vmspagesize_i = 0;
  dut->vm_prv_t0_i = kPrvS;
  dut->vm_prv_t1_i = kPrvS;
  dut->satp_mode_i = 0;
  dut->satp_ppn_i = 0;
  dut->matp_mode_i = 0;
  dut->matp_ppn_i = 0;
  dut->satp_info_en_i = 0;
  dut->matp_info_en_i = 0;
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
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
  dut->l2_resp_valid_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

bool wait_for_id_ready(SimCtrl<DUT>& sim, int max_cycles = 256) {
  for (int i = 0; i < max_cycles; ++i) {
    if (sim.dut->id_core_ready_o == 1) return true;
    tick_and_eval(sim);
  }
  return sim.dut->id_core_ready_o == 1;
}

void apb_clear(DUT* dut) {
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwdata_i = 0;
}

void check_apb_error(SimCtrl<DUT>& sim, uint16_t addr, bool write, const char* what) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = write ? 1 : 0;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = 0xA55AA55A11223344ull;
  tick_and_eval(sim);

  sim.dut->apb_penable_i = 1;
  bool saw_ready = false;
  bool saw_error = false;
  uint64_t read_data = ~uint64_t{0};
  for (int i = 0; i < 4; ++i) {
    tick_and_eval(sim);
    if (sim.dut->apb_pready_o == 1) {
      saw_ready = true;
      saw_error = (sim.dut->apb_pslverr_o == 1);
      read_data = sim.dut->apb_prdata_o;
      break;
    }
  }

  sim.check(saw_ready, std::string(what) + " completes");
  sim.check(saw_error, std::string(what) + " returns pslverr");
  sim.check(read_data == 0, std::string(what) + " returns zero read data");

  apb_clear(sim.dut.get());
  tick_and_eval(sim);
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(wait_for_id_ready(sim), "DebugApbEn=0 reset reaches ID-ready idle state");
  sim.check(sim.dut->apb_pslverr_o == 0, "DebugApbEn=0 leaves idle APB error low");

  check_apb_error(sim, kMetadataDebugAddr, false, "DebugApbEn=0 metadata-window APB read");
  check_apb_error(sim, kMetadataDebugAddr, true, "DebugApbEn=0 metadata-window APB write");
  check_apb_error(sim, kDataDebugAddr, false, "DebugApbEn=0 data-window APB read");
  check_apb_error(sim, kDataDebugAddr, true, "DebugApbEn=0 data-window APB write");

  return sim.finish();
}

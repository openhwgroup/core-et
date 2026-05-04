// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// DebugApbEn=0 APB error-path test for minion core_top.

#include "Vcore_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcore_top_tb;

namespace {

constexpr uint16_t kDcacheDebugAddr = 0x0000;
constexpr uint16_t kDebugModuleAddr = 0x0804;

void clear_inputs(DUT* dut) {
  dut->enabled_i = 0x1;
  dut->reset_vector_i = 0x1000;
  dut->icache_req_ready_i = 1;
  dut->icache_resp_valid_i = 0;
  for (int i = 0; i < 8; ++i) dut->icache_resp_data_i[i] = 0;
  dut->debug_halt_i = 0;
  dut->debug_resume_i = 0;
  dut->debug_resethalt_i = 0;
  dut->debug_ackhavereset_i = 0;
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void reset_all(SimCtrl<DUT>& sim) {
  sim.dut->rst_c_ni = 0;
  sim.dut->rst_w_ni = 0;
  sim.dut->rst_d_ni = 0;
  for (int i = 0; i < 5; ++i) sim.tick();
  sim.dut->rst_c_ni = 1;
  sim.dut->rst_w_ni = 1;
  sim.dut->rst_d_ni = 1;
  sim.tick();
}

void apb_clear(DUT* dut) {
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void check_apb_error(SimCtrl<DUT>& sim, uint16_t addr, bool write, const char* what) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = write ? 1 : 0;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = 0x1122334455667788ull;
  sim.tick();

  sim.dut->apb_penable_i = 1;
  bool saw_ready = false;
  bool saw_error = false;
  uint64_t read_data = ~uint64_t{0};
  for (int i = 0; i < 4; ++i) {
    sim.tick();
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
  sim.tick();
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 200000;

  clear_inputs(sim.dut.get());
  sim.dut->rst_c_ni = 1;
  sim.dut->rst_w_ni = 1;
  sim.dut->rst_d_ni = 1;
  reset_all(sim);

  check_apb_error(sim, kDcacheDebugAddr, false, "DebugApbEn=0 dcache-window APB read");
  check_apb_error(sim, kDcacheDebugAddr, true, "DebugApbEn=0 dcache-window APB write");
  check_apb_error(sim, kDebugModuleAddr, false, "DebugApbEn=0 debug-module APB read");
  check_apb_error(sim, kDebugModuleAddr, true, "DebugApbEn=0 debug-module APB write");

  return sim.finish();
}

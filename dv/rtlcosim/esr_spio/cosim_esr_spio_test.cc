// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_esr_spio_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_esr_spio_tb;

namespace {

constexpr uint8_t kAddrSpdmctrl   = 0xA;
constexpr uint8_t kAddrSphastatus = 0xB;

void clear_inputs(DUT* dut) {
  dut->rst_c_ni = 1;
  dut->rst_w_ni = 1;
  dut->rst_d_ni = 1;
  dut->test_en_i = 0;

  dut->dbg_halted_i = 0;
  dut->dbg_running_i = 0;
  dut->dbg_have_reset_i = 0;
  dut->dbg_busy_i = 0;
  dut->dbg_exception_i = 0;
  dut->dbg_error_i = 0;
  dut->spio_rc_resumeack_i = 0;

  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void apb_idle(DUT* dut) {
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
  sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
  sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);
  sim.compare("esr_spdmctrl", sim.dut->orig_esr_spdmctrl_o, sim.dut->new_esr_spdmctrl_o);
  sim.compare("esr_sphastatus", sim.dut->orig_esr_sphastatus_o, sim.dut->new_esr_sphastatus_o);
}

void apply_reset(CosimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.dut->rst_c_ni = 0;
  sim.dut->rst_w_ni = 0;
  sim.dut->rst_d_ni = 0;
  for (int i = 0; i < 5; ++i) {
    sim.tick();
    compare_outputs(sim);
  }
  sim.dut->rst_c_ni = 1;
  sim.dut->rst_w_ni = 1;
  sim.dut->rst_d_ni = 1;
  sim.tick();
  compare_outputs(sim);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_outputs(sim);
}

void apb_read(CosimCtrl<DUT>& sim, uint8_t addr) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = 0;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  tick_and_compare(sim);

  sim.dut->apb_penable_i = 1;
  tick_and_compare(sim);

  apb_idle(sim.dut.get());
  tick_and_compare(sim);
}

void apb_write(CosimCtrl<DUT>& sim, uint8_t addr, uint64_t data) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = 1;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = data;
  tick_and_compare(sim);

  sim.dut->apb_penable_i = 1;
  tick_and_compare(sim);

  apb_idle(sim.dut.get());
  tick_and_compare(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  apply_reset(sim);

  sim.dut->dbg_busy_i = 1;
  sim.dut->dbg_running_i = 1;
  sim.dut->dbg_have_reset_i = 1;
  sim.dut->spio_rc_resumeack_i = 1;
  tick_and_compare(sim);

  sim.dut->dbg_error_i = 1;
  sim.dut->dbg_exception_i = 1;
  tick_and_compare(sim);
  sim.dut->dbg_error_i = 0;
  sim.dut->dbg_exception_i = 0;
  apb_write(sim, kAddrSphastatus, 0);

  apb_write(sim, kAddrSpdmctrl, 0xf000000eull);
  apb_read(sim, kAddrSpdmctrl);
  apb_read(sim, kAddrSphastatus);
  apb_read(sim, 0x0);

  std::mt19937_64 rng(0x4553525350494fULL);
  for (int cycle = 0; cycle < 4000; ++cycle) {
    sim.dut->dbg_halted_i = rng() & 0x3;
    sim.dut->dbg_running_i = rng() & 0x3;
    sim.dut->dbg_have_reset_i = rng() & 0x3;
    sim.dut->dbg_busy_i = rng() & 0x3;
    sim.dut->dbg_exception_i = rng() & 0x3;
    sim.dut->dbg_error_i = rng() & 0x3;
    sim.dut->spio_rc_resumeack_i = rng() & 0x1;

    if ((rng() & 0x3) == 0) {
      sim.dut->apb_paddr_i = rng() & 0xF;
      sim.dut->apb_pwrite_i = rng() & 0x1;
      sim.dut->apb_psel_i = 1;
      sim.dut->apb_penable_i = 0;
      sim.dut->apb_pwdata_i = rng();
      tick_and_compare(sim);

      sim.dut->apb_psel_i = 1;
      sim.dut->apb_penable_i = 1;
      tick_and_compare(sim);

      apb_idle(sim.dut.get());
      tick_and_compare(sim);
    } else {
      apb_idle(sim.dut.get());
      tick_and_compare(sim);
    }
  }

  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vesr_spio_tb.h"
#include "sim_ctrl.h"

using DUT = Vesr_spio_tb;

namespace {

constexpr uint8_t kAddrSpdmctrl   = 0xA;
constexpr uint8_t kAddrSphastatus = 0xB;

constexpr uint64_t kSpdmNdmreset        = 1ull << 1;
constexpr uint64_t kSpdmClrResetHaltReq = 1ull << 2;
constexpr uint64_t kSpdmSetResetHaltReq = 1ull << 3;
constexpr uint64_t kSpdmAckHaveReset    = 1ull << 28;
constexpr uint64_t kSpdmHartReset       = 1ull << 29;
constexpr uint64_t kSpdmResumeReq       = 1ull << 30;
constexpr uint64_t kSpdmHaltReq         = 1ull << 31;

constexpr uint64_t kSpHaStatusHalted    = 1ull << 0;
constexpr uint64_t kSpHaStatusRunning   = 1ull << 1;
constexpr uint64_t kSpHaStatusResumeAck = 1ull << 2;
constexpr uint64_t kSpHaStatusHaveReset = 1ull << 3;
constexpr uint64_t kSpHaStatusBusy      = 1ull << 4;
constexpr uint64_t kSpHaStatusException = 1ull << 5;
constexpr uint64_t kSpHaStatusError     = 1ull << 6;

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

void apply_reset(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.dut->rst_c_ni = 0;
  sim.dut->rst_w_ni = 0;
  sim.dut->rst_d_ni = 0;
  for (int i = 0; i < 5; ++i) sim.tick();
  sim.dut->rst_c_ni = 1;
  sim.dut->rst_w_ni = 1;
  sim.dut->rst_d_ni = 1;
  sim.tick();
}

uint64_t apb_read(SimCtrl<DUT>& sim, uint8_t addr) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = 0;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.tick();

  sim.dut->apb_penable_i = 1;
  sim.tick();
  uint64_t data = sim.dut->apb_prdata_o;
  apb_idle(sim.dut.get());
  return data;
}

void apb_write(SimCtrl<DUT>& sim, uint8_t addr, uint64_t data, bool idle_after = true) {
  sim.dut->apb_paddr_i = addr;
  sim.dut->apb_pwrite_i = 1;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = data;
  sim.tick();

  sim.dut->apb_penable_i = 1;
  sim.tick();

  if (idle_after) {
    apb_idle(sim.dut.get());
  }
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  apply_reset(sim);

  sim.check(sim.dut->esr_spdmctrl_o == 0, "spdmctrl resets to zero");
  sim.check(sim.dut->esr_sphastatus_o == 0, "sphastatus resets to zero");
  sim.check(sim.dut->apb_pready_o == 1, "APB pready is hard-wired high");

  sim.check(apb_read(sim, kAddrSpdmctrl) == 0, "spdmctrl APB read resets to zero");
  sim.check(apb_read(sim, kAddrSphastatus) == 0, "sphastatus APB read resets to zero");

  sim.dut->dbg_busy_i = 1;
  sim.dut->dbg_running_i = 1;
  sim.dut->dbg_have_reset_i = 1;
  sim.dut->spio_rc_resumeack_i = 1;
  sim.tick();
  sim.check((sim.dut->esr_sphastatus_o & (kSpHaStatusBusy | kSpHaStatusRunning |
                                          kSpHaStatusHaveReset | kSpHaStatusResumeAck))
              == (kSpHaStatusBusy | kSpHaStatusRunning |
                  kSpHaStatusHaveReset | kSpHaStatusResumeAck),
            "live SPIO status bits reflect debug inputs");

  sim.dut->dbg_error_i = 1;
  sim.dut->dbg_exception_i = 1;
  sim.tick();
  sim.check((sim.dut->esr_sphastatus_o & (kSpHaStatusError | kSpHaStatusException))
              == (kSpHaStatusError | kSpHaStatusException),
            "error/exception status latch high from debug input");

  sim.dut->dbg_error_i = 0;
  sim.dut->dbg_exception_i = 0;
  apb_write(sim, kAddrSphastatus, 0, true);
  sim.check((sim.dut->esr_sphastatus_o & (kSpHaStatusError | kSpHaStatusException)) == 0,
            "error/exception clear via APB write with low bits");

  const uint64_t ctrl_write = kSpdmNdmreset | kSpdmClrResetHaltReq |
                              kSpdmSetResetHaltReq | kSpdmAckHaveReset |
                              kSpdmHartReset | kSpdmResumeReq | kSpdmHaltReq;
  apb_write(sim, kAddrSpdmctrl, ctrl_write, false);
  sim.check((sim.dut->esr_spdmctrl_o & ctrl_write) == ctrl_write,
            "spdmctrl output exposes persistent and pulse fields after write");

  apb_idle(sim.dut.get());
  sim.tick();
  sim.check((sim.dut->esr_spdmctrl_o &
             (kSpdmClrResetHaltReq | kSpdmSetResetHaltReq |
              kSpdmAckHaveReset | kSpdmResumeReq)) == 0,
            "pulse fields self-clear after one cycle");
  sim.check((sim.dut->esr_spdmctrl_o & (kSpdmNdmreset | kSpdmHartReset | kSpdmHaltReq))
              == (kSpdmNdmreset | kSpdmHartReset | kSpdmHaltReq),
            "persistent control bits stay asserted");

  sim.check(apb_read(sim, kAddrSpdmctrl) == (kSpdmNdmreset | kSpdmHartReset),
            "APB read masks out haltreq and pulse-only fields");

  sim.dut->apb_paddr_i = 0x0;
  sim.dut->apb_pwrite_i = 0;
  sim.dut->apb_psel_i = 1;
  sim.dut->apb_penable_i = 0;
  sim.tick();
  sim.dut->apb_penable_i = 1;
  sim.tick();
  sim.check(sim.dut->apb_pslverr_o == 1, "invalid address raises APB slave error");
  apb_idle(sim.dut.get());

  return sim.finish();
}

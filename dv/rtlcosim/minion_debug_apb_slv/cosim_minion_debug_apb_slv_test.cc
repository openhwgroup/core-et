// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: original minion_debug_apb_slv vs new translation.

#include "Vcosim_minion_debug_apb_slv_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

using DUT = Vcosim_minion_debug_apb_slv_tb;

namespace {

constexpr uint16_t kPpMsg = 0x800;
constexpr uint16_t kThread1 = 0x200;
constexpr uint16_t kNxData0 = 0x0;
constexpr uint16_t kNxData1 = 0x1;
constexpr uint16_t kAxData0 = 0x2;
constexpr uint16_t kAxProgBuf0 = 0x4;
constexpr uint16_t kAxProgBuf1 = 0x5;
constexpr uint16_t kNxProgBuf0 = 0x6;
constexpr uint16_t kAbsCmd = 0x8;
constexpr uint16_t kInvalid = 0x1f;

uint32_t rng = 0x6d2b79f5u;
uint32_t next_rand() {
  rng ^= rng << 13;
  rng ^= rng >> 17;
  rng ^= rng << 5;
  return rng;
}
uint64_t next_rand64() {
  return (static_cast<uint64_t>(next_rand()) << 32) | next_rand();
}

void idle(DUT* d) {
  d->stim_apb_paddr = 0;
  d->stim_apb_pwrite = 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = 0;
  d->stim_csr_wdata = 0;
  d->stim_update_ddata0 = 0;
  d->stim_in_debug_mode = 0;
}

void compare_all(CosimCtrl<DUT>& sim) {
  auto* d = sim.dut.get();
  sim.compare("apb_pready", d->orig_apb_pready, d->new_apb_pready);
  sim.compare("apb_prdata", d->orig_apb_prdata, d->new_apb_prdata);
  sim.compare("apb_pslverr", d->orig_apb_pslverr, d->new_apb_pslverr);
  sim.compare("read_ddata0_t0", d->orig_read_ddata0_t0, d->new_read_ddata0_t0);
  sim.compare("read_ddata0_t1", d->orig_read_ddata0_t1, d->new_read_ddata0_t1);
  sim.compare("ffb_wdata", d->orig_ffb_wdata, d->new_ffb_wdata);
  sim.compare("ffb_en", d->orig_ffb_en, d->new_ffb_en);
  sim.compare("ffb_thread_sel", d->orig_ffb_thread_sel, d->new_ffb_thread_sel);
  sim.compare("ex_program_buffer", d->orig_ex_program_buffer, d->new_ex_program_buffer);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  sim.dut->eval();
  compare_all(sim);
}

void setup_apb(DUT* d, uint16_t addr, bool write, uint64_t data) {
  d->stim_apb_paddr = addr;
  d->stim_apb_pwrite = write ? 1 : 0;
  d->stim_apb_psel = 1;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = data;
}

void clear_apb(DUT* d) {
  d->stim_apb_paddr = 0;
  d->stim_apb_pwrite = 0;
  d->stim_apb_psel = 0;
  d->stim_apb_penable = 0;
  d->stim_apb_pwdata = 0;
}

bool apb_access(CosimCtrl<DUT>& sim, uint16_t addr, bool write, uint64_t data,
                bool* pslverr, uint64_t* prdata, uint8_t* ffb_en,
                uint8_t* ex_program_buffer) {
  *ffb_en = 0;
  *ex_program_buffer = 0;
  setup_apb(sim.dut.get(), addr, write, data);
  tick_and_compare(sim);
  *ffb_en |= sim.dut->orig_ffb_en;
  *ex_program_buffer |= sim.dut->orig_ex_program_buffer;
  sim.dut->stim_apb_penable = 1;

  *pslverr = false;
  *prdata = 0;
  for (int i = 0; i < 8; ++i) {
    sim.dut->eval();
    compare_all(sim);
    *ffb_en |= sim.dut->orig_ffb_en;
    *ex_program_buffer |= sim.dut->orig_ex_program_buffer;
    if (sim.dut->orig_apb_pready && sim.dut->new_apb_pready) {
      *pslverr = sim.dut->orig_apb_pslverr;
      *prdata = sim.dut->orig_apb_prdata;
      sim.tick();
      sim.dut->eval();
      compare_all(sim);
      clear_apb(sim.dut.get());
      tick_and_compare(sim);
      return true;
    }
    tick_and_compare(sim);
    *ffb_en |= sim.dut->orig_ffb_en;
    *ex_program_buffer |= sim.dut->orig_ex_program_buffer;
  }

  clear_apb(sim.dut.get());
  tick_and_compare(sim);
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 2000000;
  auto* d = sim.dut.get();

  idle(d);
  d->rst_ni = 0;
  for (int i = 0; i < 4; ++i) tick_and_compare(sim);
  d->rst_ni = 1;
  tick_and_compare(sim);

  d->stim_in_debug_mode = 0x3;
  d->stim_csr_wdata = 0x1122334455667788ull;
  d->stim_update_ddata0 = 0x1;
  tick_and_compare(sim);
  d->stim_update_ddata0 = 0;
  sim.check(d->orig_read_ddata0_t0 == 0x1122334455667788ull &&
            d->new_read_ddata0_t0 == 0x1122334455667788ull,
            "CSR update reaches ddata0 thread0");

  bool err = false;
  uint64_t rdata = 0;
  uint8_t ffb_en = 0;
  uint8_t ex_pb = 0;
  bool ready = apb_access(sim, kPpMsg | kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && rdata == 0x55667788ull,
            "valid read returns ddata0 low word");

  // Regression for the EN_FF macro translation: s1_rdata is not reset by the
  // original reset input, while the adjacent RST_EN_FF control registers are.
  d->rst_ni = 0;
  tick_and_compare(sim);
  sim.check(d->orig_apb_prdata == 0x55667788ull && d->new_apb_prdata == 0x55667788ull,
            "read-data stage retains value while reset is asserted");
  d->rst_ni = 1;
  tick_and_compare(sim);

  d->stim_in_debug_mode = 0x3;
  ready = apb_access(sim, kPpMsg | kNxData1, true, 0xaabbccddull, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && d->orig_read_ddata0_t0 == 0xaabbccdd55667788ull,
            "write high ddata0 word completes");

  ready = apb_access(sim, kPpMsg | kThread1 | kNxData0, true, 0x01020304ull,
                     &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && d->orig_read_ddata0_t1 == 0x01020304ull,
            "thread select steers ddata0 write");

  ready = apb_access(sim, kPpMsg | kAxProgBuf0, true, 0xfeedfacecafebeefull,
                     &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && (ffb_en & 0x4) && (ex_pb & 0x1),
            "AX progbuf0 write forwards data and executes");

  ready = apb_access(sim, kPpMsg | kAxProgBuf1, true, 0x0123456789abcdefull,
                     &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && (ffb_en & 0x8) && (ex_pb & 0x1),
            "AX progbuf1 write enables high program-buffer word and executes");

  ready = apb_access(sim, kPpMsg | kNxProgBuf0, true, 0xf00dull,
                     &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && (ffb_en & 0x4) && !(ex_pb & 0x1),
            "NX progbuf write enables word without execute");

  ready = apb_access(sim, kPpMsg | kAbsCmd, true, 0x1234ull, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && !err && ((ffb_en & 0x3) == 0x3) && (ex_pb & 0x1),
            "abscmd write enables low program-buffer words and executes");

  d->stim_in_debug_mode = 0x0;
  ready = apb_access(sim, kPpMsg | kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && err, "access is rejected when selected thread is not halted");

  d->stim_in_debug_mode = 0x3;
  ready = apb_access(sim, kPpMsg | kInvalid, false, 0, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && err, "invalid register address is rejected");

  ready = apb_access(sim, kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb);
  sim.check(ready && err, "invalid protection prefix is rejected");

  // Random APB/control traffic. It intentionally includes protocol-odd cycles
  // and reset pulses to exercise every input bit and the reset-vs-enable order.
  for (int cycle = 0; cycle < 6000; ++cycle) {
    uint32_t r = next_rand();
    d->rst_ni = ((cycle % 997) == 0) ? 0 : 1;
    d->stim_apb_paddr = static_cast<uint16_t>(next_rand() & 0xfff);
    d->stim_apb_pwrite = (r >> 0) & 1;
    d->stim_apb_psel = (r >> 1) & 1;
    d->stim_apb_penable = (r >> 2) & 1;
    d->stim_apb_pwdata = next_rand64();
    d->stim_csr_wdata = next_rand64();
    d->stim_update_ddata0 = (r >> 3) & 0x3;
    d->stim_in_debug_mode = (r >> 5) & 0x3;
    tick_and_compare(sim);
  }

  d->rst_ni = 1;
  idle(d);
  tick_and_compare(sim);

  return sim.finish();
}

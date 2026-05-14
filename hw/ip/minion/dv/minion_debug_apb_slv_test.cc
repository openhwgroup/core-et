// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Targeted unit test for minion_debug_apb_slv.

#include "Vminion_debug_apb_slv_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vminion_debug_apb_slv_tb;

namespace {

constexpr uint16_t kPpMsg = 0x800;
constexpr uint16_t kThread1 = 0x200;
constexpr uint16_t kNxData0 = 0x0;
constexpr uint16_t kNxData1 = 0x1;
constexpr uint16_t kAxData0 = 0x2;
constexpr uint16_t kAxProgBuf0 = 0x4;
constexpr uint16_t kAbsCmd = 0x8;
constexpr uint16_t kInvalid = 0x1f;

void idle(DUT* d) {
  d->apb_paddr_i = 0;
  d->apb_pwrite_i = 0;
  d->apb_psel_i = 0;
  d->apb_penable_i = 0;
  d->apb_pwdata_i = 0;
  d->csr_wdata_i = 0;
  d->update_ddata0_i = 0;
  d->in_debug_mode_i = 0;
}

void setup_apb(DUT* d, uint16_t addr, bool write, uint64_t data) {
  d->apb_paddr_i = addr;
  d->apb_pwrite_i = write ? 1 : 0;
  d->apb_psel_i = 1;
  d->apb_penable_i = 0;
  d->apb_pwdata_i = data;
}

void clear_apb(DUT* d) {
  d->apb_paddr_i = 0;
  d->apb_pwrite_i = 0;
  d->apb_psel_i = 0;
  d->apb_penable_i = 0;
  d->apb_pwdata_i = 0;
}

bool apb_access(SimCtrl<DUT>& sim, uint16_t addr, bool write, uint64_t data,
                bool* pslverr, uint64_t* prdata, uint8_t* ffb_en,
                uint8_t* ex_program_buffer, uint64_t* ffb_wdata) {
  setup_apb(sim.dut.get(), addr, write, data);
  sim.tick();
  if (sim.dut->ffb_en_o) *ffb_en |= sim.dut->ffb_en_o;
  if (sim.dut->ex_program_buffer_o) *ex_program_buffer |= sim.dut->ex_program_buffer_o;
  if (sim.dut->ffb_wdata_o) *ffb_wdata = sim.dut->ffb_wdata_o;
  sim.dut->apb_penable_i = 1;

  *pslverr = false;
  *prdata = 0;
  for (int i = 0; i < 8; ++i) {
    sim.dut->eval();
    if (sim.dut->ffb_en_o) *ffb_en |= sim.dut->ffb_en_o;
    if (sim.dut->ex_program_buffer_o) *ex_program_buffer |= sim.dut->ex_program_buffer_o;
    if (sim.dut->ffb_wdata_o) *ffb_wdata = sim.dut->ffb_wdata_o;
    if (sim.dut->apb_pready_o) {
      *pslverr = sim.dut->apb_pslverr_o;
      *prdata = sim.dut->apb_prdata_o;
      sim.tick();
      clear_apb(sim.dut.get());
      sim.tick();
      return true;
    }
    sim.tick();
    if (sim.dut->ffb_en_o) *ffb_en |= sim.dut->ffb_en_o;
    if (sim.dut->ex_program_buffer_o) *ex_program_buffer |= sim.dut->ex_program_buffer_o;
    if (sim.dut->ffb_wdata_o) *ffb_wdata = sim.dut->ffb_wdata_o;
  }

  clear_apb(sim.dut.get());
  sim.tick();
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 200000;

  idle(sim.dut.get());
  sim.reset();
  idle(sim.dut.get());

  sim.dut->in_debug_mode_i = 0x3;
  sim.dut->csr_wdata_i = 0x1122334455667788ull;
  sim.dut->update_ddata0_i = 0x1;
  sim.tick();
  sim.dut->update_ddata0_i = 0;
  sim.check(sim.dut->read_ddata0_t0_o == 0x1122334455667788ull,
            "CSR update writes thread0 ddata0");

  bool err = false;
  uint64_t rdata = 0;
  uint8_t ffb_en = 0;
  uint8_t ex_pb = 0;
  uint64_t ffb_wdata = 0;
  bool ready = apb_access(sim, kPpMsg | kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err && rdata == 0x55667788ull,
            "valid debug APB read returns ddata0 low word");

  // The original s1_rdata EN_FF has no reset. Keep that exact behavior even
  // though the control RST_EN_FF stages reset in the debug reset domain.
  sim.dut->rst_ni = 0;
  sim.tick();
  sim.check(sim.dut->apb_prdata_o == 0x55667788ull,
            "APB read-data staging register is not reset by rst_ni");
  sim.dut->rst_ni = 1;
  sim.tick();

  sim.dut->in_debug_mode_i = 0x3;
  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kNxData1, true, 0xaabbccddull, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err, "valid debug APB write to ddata0 high word completes");
  sim.check(sim.dut->read_ddata0_t0_o == 0xaabbccdd55667788ull,
            "APB write updates ddata0 high word");

  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kThread1 | kNxData0, true, 0x01020304ull,
                     &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err, "thread1 APB write completes when thread1 is in debug mode");
  sim.check(sim.dut->read_ddata0_t1_o == 0x01020304ull,
            "thread select steers APB ddata0 write");

  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kAxProgBuf0, true, 0xfeedfacecafebeefull,
                     &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err, "execute program-buffer write completes after execute delay");
  sim.check((ffb_en & 0x4) != 0, "AX progbuf0 write pulses frontend program-buffer word enable");
  sim.check((ex_pb & 0x1) != 0, "AX progbuf0 write pulses execute for selected thread");
  sim.check(ffb_wdata == 0xfeedfacecafebeefull,
            "program-buffer write data is forwarded");

  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kAbsCmd, true, 0x1234ull, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err, "abscmd write completes");
  sim.check((ffb_en & 0x3) == 0x3, "abscmd write enables low two program-buffer words");

  sim.dut->in_debug_mode_i = 0x0;
  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && err, "debug APB rejects accesses when selected thread is not halted");

  sim.dut->in_debug_mode_i = 0x3;
  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kInvalid, false, 0, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && err, "debug APB rejects invalid register addresses");

  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kNxData0, false, 0, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && err, "debug APB rejects invalid protection prefix");

  ffb_en = 0; ex_pb = 0; ffb_wdata = 0;
  ready = apb_access(sim, kPpMsg | kAxData0, true, 0xdeadbeefull, &err, &rdata, &ffb_en, &ex_pb, &ffb_wdata);
  sim.check(ready && !err && (ex_pb & 0x1), "AX data write updates ddata0 and executes program buffer");
  sim.check((sim.dut->read_ddata0_t0_o & 0xffffffffull) == 0xdeadbeefull,
            "AX data write updates ddata0 low word");

  return sim.finish();
}

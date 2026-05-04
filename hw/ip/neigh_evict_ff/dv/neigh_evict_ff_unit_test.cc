// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_evict_ff_unit_tb.h"
#include "sim_ctrl.h"

using DUT = Vneigh_evict_ff_unit_tb;

namespace {

constexpr uint8_t kOpcodeMsgSendData = 4;
constexpr uint8_t kOpcodeEvict = 10;
constexpr uint8_t kSizeHLine = 5;
constexpr uint8_t kSizeLine = 6;

uint64_t build_local_msg_addr(uint8_t shire_id, uint8_t neigh_id, uint8_t local_dest, uint8_t msg_id) {
  uint64_t addr = 0;
  addr |= uint64_t(msg_id & 0xff) << 3;
  addr |= uint64_t(((neigh_id & 0x3) << 3) | (local_dest & 0x7)) << 13;
  addr |= uint64_t(0) << 20;
  addr |= uint64_t(shire_id) << 22;
  addr |= uint64_t(1) << 32;
  return addr;
}

void clear_inputs(DUT* dut) {
  dut->shire_id_i = 0;
  dut->neigh_id_i = 0;
  dut->l2_req_id_i = 0;
  dut->l2_req_source_i = 0;
  dut->l2_req_wdata_i = 0;
  dut->l2_req_opcode_i = 0;
  dut->l2_req_address_i = 0;
  dut->l2_req_data0_i = 0;
  dut->l2_req_data1_i = 0;
  dut->l2_req_data2_i = 0;
  dut->l2_req_data3_i = 0;
  dut->l2_req_size_i = 0;
  dut->l2_req_qwen_i = 0;
  dut->l2_req_subopcode_i = 0;
  dut->l2_dcache_req_valid_i = 0;
  dut->local_channel_mask_i = 0;
  dut->local_dest0_i = 0;
  dut->local_dest1_i = 0;
  dut->local_dest2_i = 0;
  dut->local_message_ready_i = 0;
  dut->evict_ready_i = 0;
}

void drive_req(DUT* dut,
               uint8_t id,
               uint8_t opcode,
               uint64_t address,
               uint64_t data0,
               uint64_t data1,
               uint64_t data2,
               uint64_t data3,
               uint8_t size,
               uint8_t qwen,
               uint8_t subopcode) {
  dut->l2_req_id_i = id;
  dut->l2_req_wdata_i = 1;
  dut->l2_req_opcode_i = opcode;
  dut->l2_req_address_i = address;
  dut->l2_req_data0_i = data0;
  dut->l2_req_data1_i = data1;
  dut->l2_req_data2_i = data2;
  dut->l2_req_data3_i = data3;
  dut->l2_req_size_i = size;
  dut->l2_req_qwen_i = qwen;
  dut->l2_req_subopcode_i = subopcode;
}

void reset_and_idle(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.tick();
  sim.dut->eval();
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  reset_and_idle(sim);

  sim.check(dut->l2_dcache_req_ready_o == 1, "reset leaves unit ready");
  sim.check(dut->dbg_sm_signals_o == 1, "debug availability bit high after reset");
  sim.check(dut->evict_valid_o == 0, "no evict valid after reset");
  sim.check(dut->local_message_valid_o == 0, "no local message valid after reset");

  dut->shire_id_i = 0x21;
  dut->neigh_id_i = 0x2;
  drive_req(dut, 0x07, kOpcodeEvict, 0x123450, 0x1111, 0x2222, 0x3333, 0x4444, kSizeLine, 0x9, 0x33);
  dut->l2_dcache_req_valid_i = 1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 1, "line evict keeps unit ready after first 256b pass");
  sim.check(dut->evict_valid_o == 0, "line evict not valid until full line captured");
  sim.check(dut->dbg_sm_signals_o == 1, "debug bit stays high while collecting first pass");

  drive_req(dut, 0x07, kOpcodeEvict, 0x123450, 0xaaaa, 0xbbbb, 0xcccc, 0xdddd, kSizeLine, 0x9, 0x33);
  dut->l2_dcache_req_valid_i = 1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0, "line evict fills unit after second pass");
  sim.check(dut->dbg_sm_signals_o == 0, "debug bit tracks occupied unit");
  sim.check(dut->evict_valid_o == 1, "line evict becomes valid after full capture");
  sim.check(dut->evict_req_id_o == 0x07, "evict id matches captured request");
  sim.check(dut->evict_req_opcode_o == kOpcodeEvict, "evict opcode matches captured request");
  sim.check(dut->evict_req_data0_o == 0x1111, "first output pass replays first stored chunk");
  sim.check(dut->evict_req_data3_o == 0x4444, "upper qword of first output pass matches first chunk");

  clear_inputs(dut);
  dut->evict_ready_i = 1;
  tick_and_eval(sim);
  sim.check(dut->evict_valid_o == 1, "second output pass remains valid while unit drains");
  sim.check(dut->evict_req_data0_o == 0xaaaa, "second output pass replays second stored chunk");
  sim.check(dut->evict_req_data3_o == 0xdddd, "upper qword of second output pass matches second chunk");

  clear_inputs(dut);
  dut->evict_ready_i = 1;
  tick_and_eval(sim);
  sim.check(dut->l2_dcache_req_ready_o == 1, "unit becomes available after final output pass");
  sim.check(dut->evict_valid_o == 0, "evict valid clears after drain");

  reset_and_idle(sim);
  dut->shire_id_i = 0x21;
  dut->neigh_id_i = 0x2;
  dut->local_channel_mask_i = 0x2;
  dut->local_dest1_i = 0x5;
  drive_req(dut, 0x12, kOpcodeMsgSendData, build_local_msg_addr(0x21, 0x2, 0x5, 0x4d),
            0xdead, 0xbeef, 0xcafe, 0xfeed, kSizeHLine, 0xf, 0x41);
  dut->l2_dcache_req_valid_i = 1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0, "local half-line message consumes unit in one pass");
  sim.check(dut->evict_valid_o == 0, "local message does not drive normal evict output");
  sim.check(dut->local_message_valid_o == 0x2, "matching local channel asserts valid");
  sim.check(dut->local_message_id_o == 0x4d, "local message id comes from ESR address field");
  sim.check(dut->local_message_dest_o == (0x5 << 1),
            "local message dest carries thread plus local-minion hart field");
  sim.check(dut->local_message_opcode_o == 2, "local message opcode is MsgRcvData");
  sim.check(dut->local_message_data0_o == 0xdead, "local message carries stored payload");
  sim.check(dut->local_message_data3_o == 0xfeed, "local message carries upper payload qword");

  bool local_message_cleared = false;
  bool local_unit_freed = false;
  for (int i = 0; i < 4; ++i) {
    clear_inputs(dut);
    dut->shire_id_i = 0x21;
    dut->neigh_id_i = 0x2;
    dut->local_channel_mask_i = 0x2;
    dut->local_dest1_i = 0x5;
    dut->local_message_ready_i = 1;
    tick_and_eval(sim);
    local_message_cleared |= (dut->local_message_valid_o == 0x0);
    local_unit_freed |= (dut->l2_dcache_req_ready_o == 1) && (dut->dbg_sm_signals_o == 1);
  }

  sim.check(local_message_cleared, "local message valid clears after consume");
  sim.check(local_unit_freed, "local message ready eventually frees unit");
  sim.check(dut->dbg_sm_signals_o == 1, "debug bit returns high when unit is free");

  return sim.finish();
}

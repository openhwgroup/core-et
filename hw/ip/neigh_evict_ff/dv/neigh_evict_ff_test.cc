// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_evict_ff_tb.h"
#include "sim_ctrl.h"

using DUT = Vneigh_evict_ff_tb;

namespace {

constexpr uint8_t kOpcodeMsgSendData = 4;
constexpr uint8_t kOpcodeEvict = 10;
constexpr uint8_t kSizeHLine = 5;

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
  dut->coop_evict_ready_i = 0;
}

void drive_req(DUT* dut, uint8_t id, uint8_t opcode, uint64_t address,
               uint64_t data0, uint64_t data1, uint64_t data2, uint64_t data3) {
  dut->l2_req_id_i = id;
  dut->l2_req_wdata_i = 1;
  dut->l2_req_opcode_i = opcode;
  dut->l2_req_address_i = address;
  dut->l2_req_data0_i = data0;
  dut->l2_req_data1_i = data1;
  dut->l2_req_data2_i = data2;
  dut->l2_req_data3_i = data3;
  dut->l2_req_size_i = kSizeHLine;
  dut->l2_req_qwen_i = 0xf;
  dut->l2_req_subopcode_i = 0x22;
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

  sim.check(dut->l2_dcache_req_ready_o == 0x3, "wrapper reset exposes both evict slots as ready");
  sim.check(dut->dbg_sm_signals_o == 0x3, "wrapper debug bits track both free slots");
  sim.check(dut->evict_valid_o == 0, "regular path idle after reset");
  sim.check(dut->coop_evict_valid_o == 0, "coop path idle after reset");

  dut->shire_id_i = 0x2a;
  dut->neigh_id_i = 0x1;
  dut->local_channel_mask_i = 0x1;
  dut->local_dest0_i = 0x3;
  drive_req(dut, 0x15, kOpcodeMsgSendData, build_local_msg_addr(0x2a, 0x1, 0x3, 0x66),
            0x1001, 0x1002, 0x1003, 0x1004);
  dut->l2_dcache_req_valid_i = 0x2;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x1, "common path capture consumes only regular slot");
  sim.check(dut->local_message_valid_o == 0x1, "regular path can still redirect to local message network");
  sim.check(dut->evict_valid_o == 0, "regular local message does not drive external evict");
  sim.check(dut->coop_evict_valid_o == 0, "coop path remains idle during regular capture");
  sim.check(dut->local_message_id_o == 0x66, "wrapper forwards local message metadata");

  clear_inputs(dut);
  dut->shire_id_i = 0x2a;
  dut->neigh_id_i = 0x1;
  dut->local_channel_mask_i = 0x1;
  dut->local_dest0_i = 0x3;
  dut->local_message_ready_i = 1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x3, "regular local message drains and frees slot");
  sim.check(dut->local_message_valid_o == 0x0, "local message valid clears after regular drain");

  reset_and_idle(sim);
  dut->shire_id_i = 0x2a;
  dut->neigh_id_i = 0x1;
  dut->local_channel_mask_i = 0x1;
  dut->local_dest0_i = 0x3;
  drive_req(dut, 0x0b, kOpcodeMsgSendData, build_local_msg_addr(0x2a, 0x1, 0x3, 0x21),
            0x2001, 0x2002, 0x2003, 0x2004);
  dut->l2_dcache_req_valid_i = 0x1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x2, "coop capture consumes only cooperative slot");
  sim.check(dut->local_message_valid_o == 0x0, "coop path disables local-message routing");
  sim.check(dut->evict_valid_o == 0, "regular path remains idle during coop capture");
  sim.check(dut->coop_evict_valid_o == 1, "coop path forwards captured request externally");
  sim.check(dut->coop_evict_req_id_o == 0x0b, "coop request id matches captured request");
  sim.check(dut->coop_evict_req_opcode_o == kOpcodeMsgSendData, "coop request opcode matches captured request");
  sim.check(dut->coop_evict_req_data0_o == 0x2001, "coop request data matches captured payload");
  sim.check(dut->coop_evict_req_data3_o == 0x2004, "coop request upper payload matches capture");

  clear_inputs(dut);
  dut->coop_evict_ready_i = 1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x3, "coop path drain frees wrapper slot");
  sim.check(dut->coop_evict_valid_o == 0, "coop valid clears after drain");

  reset_and_idle(sim);
  dut->shire_id_i = 0x2a;
  dut->neigh_id_i = 0x1;
  drive_req(dut, 0x1e, kOpcodeEvict, 0x345600, 0x3001, 0x3002, 0x3003, 0x3004);
  dut->l2_dcache_req_valid_i = 0x3;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x0, "simultaneous valid bits fill both wrapper units");
  sim.check(dut->evict_valid_o == 1, "regular wrapper output valid rises");
  sim.check(dut->coop_evict_valid_o == 1, "coop wrapper output valid rises");
  sim.check(dut->evict_req_id_o == 0x1e, "regular wrapper request id matches shared capture");
  sim.check(dut->coop_evict_req_id_o == 0x1e, "coop wrapper request id matches shared capture");
  sim.check(dut->dbg_sm_signals_o == 0x0, "debug bits drop when both wrapper units are full");

  return sim.finish();
}

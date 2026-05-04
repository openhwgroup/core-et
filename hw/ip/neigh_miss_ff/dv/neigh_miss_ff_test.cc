// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_miss_ff_tb.h"
#include "sim_ctrl.h"

using DUT = Vneigh_miss_ff_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->l2_req_id_i = 0;
  dut->l2_req_wdata_i = 0;
  dut->l2_req_opcode_i = 0;
  dut->l2_req_address_i = 0;
  dut->l2_req_data_i = 0;
  dut->l2_req_size_i = 0;
  dut->l2_req_qwen_i = 0;
  dut->l2_req_subopcode_i = 0;
  dut->l2_dcache_req_valid_i = 0;
  dut->miss_ready_i = 0;
  dut->coop_miss_ready_i = 0;
}

void drive_req(DUT* dut,
               uint8_t id,
               bool wdata,
               uint8_t opcode,
               uint64_t address,
               uint32_t data,
               uint8_t size,
               uint8_t qwen,
               uint8_t subopcode) {
  dut->l2_req_id_i = id;
  dut->l2_req_wdata_i = wdata;
  dut->l2_req_opcode_i = opcode;
  dut->l2_req_address_i = address;
  dut->l2_req_data_i = data;
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

  sim.check(dut->l2_dcache_req_ready_o == 0x7, "reset leaves all miss slots available");
  sim.check(dut->dbg_sm_signals_o == 0x7, "debug availability matches ready after reset");
  sim.check(dut->miss_valid_o == 0, "no regular miss valid after reset");
  sim.check(dut->coop_miss_valid_o == 0, "no cooperative miss valid after reset");

  drive_req(dut, 0x12, 1, 0x4, 0x12345, 0x2aa55, 0x6, 0x9, 0x53);
  dut->l2_dcache_req_valid_i = 0x4;
  dut->eval();
  sim.check(dut->l2_dcache_req_ready_o == 0x7, "regular miss sees slot available before capture");
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x3, "regular miss capture consumes only common slot");
  sim.check(dut->dbg_sm_signals_o == 0x3, "debug bits track regular slot occupancy");
  sim.check(dut->miss_valid_o == 1, "regular miss valid rises after capture");
  sim.check(dut->miss_req_id_o == 0x12, "regular miss id matches captured request");
  sim.check(dut->miss_req_source_o == 0, "regular miss source remains zero");
  sim.check(dut->miss_req_wdata_o == 1, "regular miss wdata matches captured request");
  sim.check(dut->miss_req_opcode_o == 0x4, "regular miss opcode matches captured request");
  sim.check(dut->miss_req_address_o == 0x12345, "regular miss address matches captured request");
  sim.check(dut->miss_req_data_o == 0x2aa55, "regular miss data matches captured request");
  sim.check(dut->miss_req_size_o == 0x6, "regular miss size matches captured request");
  sim.check(dut->miss_req_qwen_o == 0x9, "regular miss qwen matches captured request");
  sim.check(dut->miss_req_subopcode_o == 0x53, "regular miss subopcode matches captured request");
  sim.check(dut->coop_miss_valid_o == 0, "regular miss does not drive cooperative valids");

  clear_inputs(dut);
  dut->miss_ready_i = 1;
  dut->eval();
  tick_and_eval(sim);
  sim.check(dut->l2_dcache_req_ready_o == 0x7, "regular miss ready frees common slot");
  sim.check(dut->miss_valid_o == 0, "regular miss valid clears after consume");

  reset_and_idle(sim);
  drive_req(dut, 0x03, 0, 0x1, 0x45678, 0x15555, 0x2, 0x3, 0x11);
  dut->l2_dcache_req_valid_i = 0x1;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x6, "TenB cooperative capture consumes slot 0");
  sim.check(dut->coop_miss_valid_o == 0x1, "TenB cooperative valid rises");
  sim.check(dut->coop0_req_id_o == 0x03, "TenB cooperative id matches captured request");
  sim.check(dut->coop0_req_opcode_o == 0x1, "TenB cooperative opcode matches captured request");
  sim.check(dut->coop0_req_address_o == 0x45678, "TenB cooperative address matches captured request");
  sim.check(dut->coop0_req_data_o == 0x15555, "TenB cooperative data matches captured request");

  clear_inputs(dut);
  dut->coop_miss_ready_i = 0x1;
  tick_and_eval(sim);
  sim.check(dut->l2_dcache_req_ready_o == 0x7, "TenB cooperative ready frees slot 0");
  sim.check(dut->coop_miss_valid_o == 0x0, "TenB cooperative valid clears after consume");

  reset_and_idle(sim);
  drive_req(dut, 0x1b, 1, 0x5, 0x98765, 0x03333, 0x1, 0x4, 0x22);
  dut->l2_dcache_req_valid_i = 0x2;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x5, "Scp cooperative capture consumes slot 1");
  sim.check(dut->coop_miss_valid_o == 0x2, "Scp cooperative valid rises");
  sim.check(dut->coop1_req_id_o == 0x1b, "Scp cooperative id matches captured request");
  sim.check(dut->coop1_req_opcode_o == 0x5, "Scp cooperative opcode matches captured request");
  sim.check(dut->coop1_req_address_o == 0x98765, "Scp cooperative address matches captured request");
  sim.check(dut->coop1_req_data_o == 0x03333, "Scp cooperative data matches captured request");

  clear_inputs(dut);
  dut->coop_miss_ready_i = 0x2;
  tick_and_eval(sim);
  sim.check(dut->l2_dcache_req_ready_o == 0x7, "Scp cooperative ready frees slot 1");
  sim.check(dut->coop_miss_valid_o == 0x0, "Scp cooperative valid clears after consume");

  reset_and_idle(sim);
  drive_req(dut, 0x0f, 1, 0x8, 0xabcde, 0x3c3c3, 0x6, 0xf, 0x7f);
  dut->l2_dcache_req_valid_i = 0x7;
  tick_and_eval(sim);

  sim.check(dut->l2_dcache_req_ready_o == 0x0, "simultaneous capture can fill all three slots");
  sim.check(dut->miss_valid_o == 1, "simultaneous capture fills regular slot too");
  sim.check(dut->coop_miss_valid_o == 0x3, "simultaneous capture fills both cooperative slots");
  sim.check(dut->miss_req_id_o == 0x0f, "simultaneous capture replicates payload into regular slot");
  sim.check(dut->coop0_req_id_o == 0x0f, "simultaneous capture replicates payload into coop0");
  sim.check(dut->coop1_req_id_o == 0x0f, "simultaneous capture replicates payload into coop1");

  clear_inputs(dut);
  dut->miss_ready_i = 1;
  dut->coop_miss_ready_i = 0x3;
  tick_and_eval(sim);
  sim.check(dut->l2_dcache_req_ready_o == 0x7, "consuming all outputs frees all slots");
  sim.check(dut->miss_valid_o == 0, "regular slot clears after simultaneous consume");
  sim.check(dut->coop_miss_valid_o == 0x0, "cooperative slots clear after simultaneous consume");

  return sim.finish();
}


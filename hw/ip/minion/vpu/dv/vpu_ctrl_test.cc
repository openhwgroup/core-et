// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_ctrl_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_ctrl_tb;

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 2000;

  sim.dut->id_core_valid_i = 0;
  sim.dut->id_core_thread_id_i = 0;
  sim.dut->ren_mask_i = 0;
  sim.dut->swap12_i = 0;
  sim.dut->swap13_i = 0;
  sim.dut->swap23_i = 0;
  sim.dut->txfma_i = 0;
  sim.dut->rom_i = 0;
  sim.dut->fromint_i = 0;
  sim.dut->maskop_i = 0;
  sim.dut->wen_i = 0;
  sim.dut->wb_dmem_resp_val_i = 0;
  sim.dut->ra1_i = 0;
  sim.dut->ra2_i = 0;
  sim.dut->ra3_i = 0;
  sim.reset();

  sim.check(sim.dut->id_regfile_ren_o == 0, "reset/idle leaves ID RF reads disabled");
  sim.check(sim.dut->id_regfile_thread_id_o == 0, "reset/idle leaves ID RF thread IDs clear");
  sim.check(sim.dut->ex_sh_sw_valid_o == 0, "reset/idle leaves EX short-path valid clear");
  sim.check(sim.dut->ex_txfma_clock_valid_o == 0, "reset/idle leaves TXFMA clock request clear");
  sim.check(sim.dut->ex_rom_valid_o == 0, "reset/idle leaves ROM/trans valid clear");
  sim.check(sim.dut->io_events_o == 0, "reset/idle leaves VPU event vector clear");

  sim.dut->id_core_valid_i = 1;
  sim.dut->id_core_thread_id_i = 1;
  sim.dut->ren_mask_i = 0x7;
  sim.dut->swap12_i = 0;
  sim.dut->swap13_i = 0;
  sim.dut->swap23_i = 0;
  sim.dut->txfma_i = 0;
  sim.dut->rom_i = 0;
  sim.dut->fromint_i = 0;
  sim.dut->maskop_i = 0;
  sim.dut->wen_i = 1;
  sim.dut->ra1_i = 3;
  sim.dut->ra2_i = 11;
  sim.dut->ra3_i = 19;
  sim.dut->eval();
  sim.check(sim.dut->id_regfile_ren_o == 0x7, "decoder ren1/ren2/ren3 enable all ID RF read ports");
  sim.check(sim.dut->id_regfile_thread_id_o == 0x7, "core thread ID fans out to all ID RF read ports");
  sim.check(sim.dut->id_regfile_raddr1_o == 3, "ID read port 1 selects encoded RA1");
  sim.check(sim.dut->id_regfile_raddr2_o == 11, "ID read port 2 selects encoded RA2");
  sim.check(sim.dut->id_regfile_raddr3_o == 19, "ID read port 3 selects encoded RA3");

  sim.tick();
  sim.check(sim.dut->ex_sh_sw_valid_o == 1, "valid sh/sw decode reaches EX short-path valid");
  sim.check(sim.dut->ex_sh_sw_clock_valid_o == 1, "valid sh/sw decode requests the short-path clock");
  for (int i = 0; i < 2; ++i) sim.tick();

  sim.dut->id_core_valid_i = 1;
  sim.dut->id_core_thread_id_i = 0;
  sim.dut->ren_mask_i = 0x1;
  sim.dut->swap12_i = 1;
  sim.dut->swap13_i = 0;
  sim.dut->swap23_i = 0;
  sim.dut->txfma_i = 0;
  sim.dut->rom_i = 0;
  sim.dut->fromint_i = 0;
  sim.dut->maskop_i = 0;
  sim.dut->wen_i = 0;
  sim.dut->ra1_i = 5;
  sim.dut->ra2_i = 17;
  sim.dut->ra3_i = 23;
  sim.dut->eval();
  sim.check(sim.dut->id_regfile_ren_o == 0x2, "swap12 maps a ren1-only instruction onto read port 2");
  sim.check(sim.dut->id_regfile_raddr2_o == 5, "swap12 forwards encoded RA1 to read port 2");
  sim.check(sim.dut->id_regfile_thread_id_o == 0, "thread 0 fans out as clear ID RF thread IDs");

  sim.dut->ren_mask_i = 0x1;
  sim.dut->swap12_i = 0;
  sim.dut->swap13_i = 1;
  sim.dut->swap23_i = 0;
  sim.dut->ra1_i = 7;
  sim.dut->ra2_i = 18;
  sim.dut->ra3_i = 24;
  sim.dut->eval();
  sim.check(sim.dut->id_regfile_ren_o == 0x4, "swap13 maps a ren1-only instruction onto read port 3");
  sim.check(sim.dut->id_regfile_raddr3_o == 7, "swap13 forwards encoded RA1 to read port 3");

  sim.dut->ren_mask_i = 0x2;
  sim.dut->swap13_i = 0;
  sim.dut->swap23_i = 1;
  sim.dut->ra1_i = 6;
  sim.dut->ra2_i = 21;
  sim.dut->ra3_i = 29;
  sim.dut->eval();
  sim.check(sim.dut->id_regfile_ren_o == 0x4, "swap23 maps a ren2-only instruction onto read port 3");
  sim.check(sim.dut->id_regfile_raddr3_o == 21, "swap23 forwards encoded RA2 to read port 3");

  sim.dut->swap23_i = 0;
  sim.dut->ren_mask_i = 0x1;
  sim.dut->fromint_i = 1;
  sim.dut->txfma_i = 1;
  sim.dut->rom_i = 1;
  sim.dut->wen_i = 1;
  sim.dut->wb_dmem_resp_val_i = 0;
  sim.tick();
  sim.check(sim.dut->ex_req_fromint_o == 1, "from-int decode reaches the EX request bundle");
  sim.check(sim.dut->ex_txfma_clock_valid_o == 1, "TXFMA decode requests the TXFMA clock");
  sim.check(sim.dut->ex_rom_valid_o == 1, "ROM/trans decode reaches EX ROM valid");
  sim.check(sim.dut->ex_rom_clock_valid_o == 1, "ROM/trans decode requests the ROM clock");

  sim.dut->wb_dmem_resp_val_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->f3_regfile_wen_l_o == 1,
            "dcache writeback response raises the F3 load-write enable");

  return sim.finish();
}

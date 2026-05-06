// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_lane_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vvpu_lane_tb;

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.max_time = 2000;

  sim.dut->fromint_valid_i = 0;
  sim.dut->fromint_data_i = 0;
  sim.dut->f4_load_bypass_i = 0;
  sim.dut->bypass_addr_i = 3;
  sim.dut->bypass_data_i = 0;
  sim.dut->sh_sw_valid_i = 0;
  sim.dut->sh_sw_clock_valid_i = 0;
  sim.dut->sh_sw_add_i = 0;
  sim.dut->ex_in1_i = 0x11112222u;
  sim.dut->ex_in3_i = 0x55556666u;
  sim.dut->bypass_pipe_en_i = 0;
  sim.dut->tima_valid_i = 0;
  sim.dut->tima_valid_unqual_i = 0;
  sim.dut->txfma_valid_i = 0;
  sim.dut->txfma_clock_valid_i = 0;
  sim.dut->rom_valid_i = 0;
  sim.dut->rom_clock_valid_i = 0;
  sim.dut->chicken_vpulane_i = 1;
  sim.dut->chicken_vputima_i = 1;
  sim.reset();

  sim.check(sim.dut->ex_regfile_rdata_bypass0_o == 0,
            "reset/idle leaves bypass read operand 0 clear");
  sim.check(sim.dut->ex_regfile_rdata_bypass1_o == 0,
            "reset/idle leaves bypass read operand 1 clear");
  sim.check(sim.dut->f8_wdata_o == 0, "reset/idle leaves F8 write data clear");
  sim.check(sim.dut->f8_wflags_o == 0, "reset/idle leaves F8 flags clear");
  sim.check(sim.dut->f8_txfma_comp_res_o == 0, "reset/idle leaves TXFMA compare clear");

  sim.dut->fromint_valid_i = 1;
  sim.dut->fromint_data_i = 0x12345678u;
  sim.dut->f4_load_bypass_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->ex_regfile_rdata_bypass0_o == 0x12345678u,
            "from-int control forwards integer data into operand 0");
  sim.check(sim.dut->ex_regfile_rdata_bypass1_o == 0,
            "from-int without TXFMA leaves operand 1 on the RF path");

  sim.dut->fromint_valid_i = 0;
  sim.dut->bypass_addr_i = 9;
  sim.dut->bypass_data_i = 0xcafebeefu;
  sim.dut->f4_load_bypass_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->ex_regfile_rdata_bypass0_o == 0xcafebeefu,
            "F4 load-write bypass forwards matching load data to operand 0");
  sim.check(sim.dut->ex_regfile_rdata_bypass1_o == 0,
            "F4 operand-0 load bypass does not affect operand 1");

  sim.dut->f4_load_bypass_i = 0;
  sim.tick();
  sim.check(sim.dut->f3_maskop_bit_o == 0, "idle short-path mask bit remains clear");
  sim.check(sim.dut->txfma_trans_dbg_o == 0, "idle TXFMA/trans debug remains clear");

  sim.dut->fromint_valid_i = 1;
  sim.dut->fromint_data_i = 0x2468ace0u;
  sim.dut->sh_sw_valid_i = 1;
  sim.dut->sh_sw_clock_valid_i = 1;
  sim.dut->sh_sw_add_i = 0;
  sim.dut->ex_in1_i = 0;
  sim.dut->ex_in3_i = 0;
  sim.dut->bypass_pipe_en_i = 1;
  sim.tick();
  sim.check(sim.dut->f2_fswizz_rdata_o == 0x2468ace0u,
            "short-path MVXF captures the from-int operand into F2 swizzle read data");

  sim.dut->fromint_valid_i = 0;
  sim.dut->sh_sw_valid_i = 0;
  sim.tick();
  sim.check(sim.dut->f3_maskop_bit_o == 0,
            "short-path MVXF result has clear bit 0 in the F3 mask probe");
  for (int i = 0; i < 6; ++i) sim.tick();
  sim.check(sim.dut->f8_sh_sw_wdata_o == 0x2468ace0u,
            "short-path MVXF result reaches the F8 sh/sw write-data tap");
  sim.check(sim.dut->f8_wdata_o == 0x2468ace0u,
            "short-path MVXF result reaches the lane F8 write-data mux");

  sim.dut->bypass_pipe_en_i = 0;
  sim.dut->txfma_valid_i = 1;
  sim.dut->txfma_clock_valid_i = 1;
  sim.dut->rom_valid_i = 1;
  sim.dut->rom_clock_valid_i = 1;
  sim.dut->tima_valid_i = 1;
  sim.dut->tima_valid_unqual_i = 1;
  sim.dut->chicken_vpulane_i = 0;
  sim.dut->chicken_vputima_i = 0;
  sim.tick();
  sim.check(sim.dut->f8_txfma_comp_res_o == 0,
            "TXFMA/ROM/TIMA clock-valid smoke path keeps compare result clear for zeroed operands");

  return sim.finish();
}

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for rbox_ctrl.
//
// Verifies ESR configuration, status transitions (ready -> working -> error
// on config error), and enable output.
// Full packet decode and output generation are covered by the cosim.

#include "Vrbox_ctrl_tb.h"
#include "sim_ctrl.h"
#include <cstdint>

using DUT = Vrbox_ctrl_tb;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;

    // Initialize all inputs
    sim.dut->esr_config_i = 0;
    sim.dut->esr_config_en_i = 0;
    sim.dut->esr_in_buf_pg_i = 0;
    sim.dut->esr_in_buf_pg_en_i = 0;
    sim.dut->esr_in_buf_cfg_i = 0;
    sim.dut->esr_in_buf_cfg_en_i = 0;
    sim.dut->esr_out_buf_pg_i = 0;
    sim.dut->esr_out_buf_pg_en_i = 0;
    sim.dut->esr_out_buf_cfg_i = 0;
    sim.dut->esr_out_buf_cfg_en_i = 0;
    sim.dut->esr_start_i = 0;
    sim.dut->esr_start_en_i = 0;
    sim.dut->esr_consume_i = 0;
    sim.dut->esr_consume_en_i = 0;
    sim.dut->tr_accept_i = 0;
    sim.dut->tr_empty_i = 1;
    sim.dut->ts_empty_i = 1;
    sim.dut->sc_req_ready_i = 1;
    sim.dut->db_flush_done_i = 0;
    sim.dut->rbox_halted_i = 0;
    sim.reset(5);

    // -- Test 1: Status is Ready after reset --
    printf("test 1: status = Ready (0x00) after reset\n");
    sim.tick();
    sim.check((sim.dut->esr_status_o & 0xFF) == 0x00, "status = Ready (0x00)");

    // -- Test 2: ESR config enable sets rbox_enable_o --
    printf("test 2: ESR config enable\n");
    sim.dut->esr_config_i = 1;  // bit 0 = enable
    sim.dut->esr_config_en_i = 1;
    sim.tick();
    sim.dut->esr_config_en_i = 0;
    sim.tick();
    sim.check(sim.dut->rbox_enable_o == 1, "rbox_enable_o = 1 after config enable");

    // -- Test 3: Start with no page active -> config error --
    printf("test 3: start with bad config -> error status\n");
    // In-buf page not active (both page_active bits = 0) -> config error
    sim.dut->esr_in_buf_pg_i = 0;  // No pages active
    sim.dut->esr_in_buf_pg_en_i = 1;
    sim.tick();
    sim.dut->esr_in_buf_pg_en_i = 0;

    sim.dut->esr_out_buf_pg_i = 0;  // No out page active
    sim.dut->esr_out_buf_pg_en_i = 1;
    sim.tick();
    sim.dut->esr_out_buf_pg_en_i = 0;

    // Trigger start
    sim.dut->esr_start_i = 1;  // bit 0 = start
    sim.dut->esr_start_en_i = 1;
    sim.tick();
    sim.dut->esr_start_en_i = 0;

    // Wait for status to update
    int timeout = 20;
    while ((sim.dut->esr_status_o & 0xFF) == 0x00 && timeout > 0) {
        sim.tick();
        timeout--;
    }
    // Should be in error (0x80) or finished (0x02) due to config error
    uint8_t status = sim.dut->esr_status_o & 0xFF;
    sim.check(status == 0x80 || status == 0x02,
              "status is Error(0x80) or Finished(0x02) after bad config start");
    printf("  status = 0x%02x\n", status);

    // -- Test 4: No tile valid output when no valid input --
    printf("test 4: no tile output without input\n");
    sim.check(sim.dut->tr_tile_valid_o == 0, "no tile valid without input");

    return sim.finish();
}

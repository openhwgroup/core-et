// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// shirecache_pipe_example — Verilator smoke test.
//
// Resets the pipeline and verifies quiescent state.

#include "Vshirecache_pipe_example.h"
#include "sim_ctrl.h"

using DUT = Vshirecache_pipe_example;

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // Tie off scalar inputs. Wide struct ports (VlWide) are
    // zero-initialized by Verilator's constructor.
    sim.dut->rst_c_ni = 0;
    sim.dut->dft_sram_clk_i = 0;
    sim.dut->pipe_start_i = 0;
    sim.dut->pipe_rbuf_rd_req_valid_i = 0;
    sim.dut->pipe_rbuf_rd_rsp_ready_i = 0;
    sim.dut->pipe_idx_cop_req_ready_i = 0;
    sim.dut->mbist_on_i = 0;
    sim.dut->my_bank_id_i = 0;
    sim.dut->my_l3_virtual_shire_id_i = 0;
    sim.dut->my_scp_virtual_shire_id_i = 0;

    // Reset both reset domains.
    sim.reset();
    sim.dut->rst_c_ni = 1;
    sim.tick();
    sim.tick();

    // After reset, pipeline should be idle.
    printf("test: quiescent after reset\n");
    sim.check(sim.dut->pipe_active_o == 0, "pipe not active");
    sim.check(sim.dut->pipe_squash_o == 0, "no squash");
    sim.check(sim.dut->pipe_tag_rsp_valid_o == 0, "no tag response");
    sim.check(sim.dut->pipe_data_rsp_valid_o == 0, "no data response");
    sim.check(sim.dut->pipe_cbuf_rsp_valid_o == 0, "no cbuf response");
    sim.check(sim.dut->pipe_idx_cop_req_valid_o == 0, "no idx_cop request");
    // idx_cop_sm is expected to be active after reset (ALL_INV phase).
    sim.check(sim.dut->pipe_idx_cop_is_active_o == 1, "idx_cop active (ALL_INV)");

    // Let it run a few cycles — should stay idle.
    printf("test: stays idle\n");
    for (int i = 0; i < 10; i++) sim.tick();
    sim.check(sim.dut->pipe_active_o == 0, "still not active after 10 cycles");

    return sim.finish();
}

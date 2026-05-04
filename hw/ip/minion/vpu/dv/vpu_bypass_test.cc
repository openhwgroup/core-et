// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vvpu_bypass_tb.h"
#include "sim_ctrl.h"

using DUT = Vvpu_bypass_tb;

namespace {

constexpr uint64_t kAllSigs = ~0ULL;

void set_defaults(DUT* dut) {
    dut->lane_id_i = 0;
    dut->ex_rf_raddr0_i = 1;
    dut->ex_rf_raddr1_i = 2;
    dut->ex_rf_raddr2_i = 3;
    dut->ex_regfile_thread_id0_i = 0;
    dut->ex_regfile_thread_id1_i = 0;
    dut->ex_regfile_thread_id2_i = 0;
    dut->ex_rf_rdata0_i = 0x11111111U;
    dut->ex_rf_rdata1_i = 0x22222222U;
    dut->ex_rf_rdata2_i = 0x33333333U;
    dut->ex_req_use_prev_sigs_i = 0;
    dut->ex_req_sigs_i = 0;
    dut->ex_req_fromint_i = 0;
    dut->ex_req_txfma_i = 0;
    dut->ex_req_imm_i = 0x13579U;
    dut->ex_req_rm_i = 0x5;
    dut->ex_req_typ_i = 0x2;
    dut->ex_fromint_data_i = 0x89abcdefU;
    dut->ex_tena_rf_rdata_i = 0xa1a2a3a4U;
    dut->ex_tenb_rf_rdata_i = 0xb1b2b3b4U;
    dut->ex_tena_regfile_bypass_en_i = 0;
    dut->ex_tenb_regfile_bypass_en_i = 0;
    dut->ex_bypass_force_txfma_in0_i = 0;
    dut->ex_bypass_force_txfma_in1_i = 0;
    dut->ex_bypass_force_txfma_in2_i = 0;
    dut->ex_bypass_force_shsw_in1_i = 0;
    dut->ex_bypass_force_shsw_in2_i = 0;
    dut->f3_sh_sw_data_i = 0;
    dut->f3_sh_sw_exc_i = 0;
    dut->f3_regfile_waddr_i = 0;
    dut->f3_regfile_wmask_i = 0;
    dut->f3_thread_id_i = 0;
    dut->f3_bypass_clk_en_i = 0;
    dut->f3_data_vector_i = 1;
    dut->f4_regfile_waddr_i = 0;
    dut->f4_regfile_wmask_i = 0;
    dut->f4_thread_id_i = 0;
    dut->f4_bypass_clk_en_i = 0;
    dut->f4_regfile_waddr_l_i = 0;
    dut->f4_regfile_wmask_l_i = 0;
    dut->f4_regfile_thread_id_l_i = 0;
    dut->f4_regfile_wdata_l_i = 0;
    dut->f5_regfile_waddr_i = 0;
    dut->f5_regfile_wmask_i = 0;
    dut->f5_thread_id_i = 0;
    dut->f5_bypass_clk_en_i = 0;
    dut->f6_regfile_waddr_i = 0;
    dut->f6_regfile_wmask_i = 0;
    dut->f6_thread_id_i = 0;
    dut->f6_bypass_clk_en_i = 0;
    dut->f7_regfile_waddr_i = 0;
    dut->f7_regfile_wmask_i = 0;
    dut->f7_thread_id_i = 0;
    dut->f7_bypass_clk_en_i = 0;
    dut->f8_regfile_waddr_i = 0;
    dut->f8_regfile_wmask_i = 0;
    dut->f8_thread_id_i = 0;
    dut->f8_txfma_data_i = 0;
    dut->f8_txfma_exc_i = 0;
    dut->f8_data_vector_i = 1;
    dut->f8_txfma_op_i = 0;
}

void flush_pipeline(SimCtrl<DUT>& sim) {
    sim.dut->f3_bypass_clk_en_i = 1;
    sim.dut->f4_bypass_clk_en_i = 1;
    sim.dut->f5_bypass_clk_en_i = 1;
    sim.dut->f6_bypass_clk_en_i = 1;
    sim.dut->f7_bypass_clk_en_i = 1;
    sim.dut->f3_sh_sw_data_i = 0;
    sim.dut->f3_sh_sw_exc_i = 0;
    sim.dut->f8_txfma_data_i = 0;
    sim.dut->f8_txfma_exc_i = 0;
    sim.dut->f8_txfma_op_i = 0;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->f3_bypass_clk_en_i = 0;
    sim.dut->f4_bypass_clk_en_i = 0;
    sim.dut->f5_bypass_clk_en_i = 0;
    sim.dut->f6_bypass_clk_en_i = 0;
    sim.dut->f7_bypass_clk_en_i = 0;
}

void advance_pipeline(SimCtrl<DUT>& sim, uint32_t data, uint32_t exc) {
    sim.dut->f3_sh_sw_data_i = data;
    sim.dut->f3_sh_sw_exc_i = exc;
    sim.dut->f3_bypass_clk_en_i = 1;
    sim.dut->f4_bypass_clk_en_i = 1;
    sim.dut->f5_bypass_clk_en_i = 1;
    sim.dut->f6_bypass_clk_en_i = 1;
    sim.dut->f7_bypass_clk_en_i = 1;
    for (int i = 0; i < 5; ++i) sim.tick();
    sim.dut->f3_bypass_clk_en_i = 0;
    sim.dut->f4_bypass_clk_en_i = 0;
    sim.dut->f5_bypass_clk_en_i = 0;
    sim.dut->f6_bypass_clk_en_i = 0;
    sim.dut->f7_bypass_clk_en_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 2000;

    set_defaults(sim.dut.get());
    sim.reset();
    flush_pipeline(sim);

    sim.tick();
    sim.check(sim.dut->ex_req_in1_o == 0x11111111U, "src0 defaults to RF data");
    sim.check(sim.dut->ex_req_in2_o == 0x22222222U, "src1 defaults to RF data");
    sim.check(sim.dut->ex_req_in3_o == 0x33333333U, "src2 defaults to RF data");
    sim.check(sim.dut->ex_req_use_prev_sigs_o == 0, "use_prev_sigs passes through");
    sim.check(sim.dut->ex_req_sigs_o == 0, "control bundle passes through");
    sim.check(sim.dut->ex_req_imm_o == 0x13579U, "immediate passes through");
    sim.check(sim.dut->ex_req_rm_o == 0x5, "rounding mode passes through");
    sim.check(sim.dut->ex_req_typ_o == 0x2, "type passes through");

    sim.dut->ex_tena_regfile_bypass_en_i = 1;
    sim.tick();
    sim.check(sim.dut->ex_req_in1_o == 0xa1a2a3a4U, "src0 takes tena bypass first");

    sim.dut->ex_tena_regfile_bypass_en_i = 0;
    sim.dut->ex_bypass_force_txfma_in0_i = 1;
    sim.dut->f8_txfma_data_i = 0xdeadbeefU;
    sim.tick();
    sim.check(sim.dut->ex_req_in1_o == 0xdeadbeefU, "src0 can be forced from TXFMA");

    sim.dut->ex_bypass_force_txfma_in0_i = 0;
    sim.dut->ex_tenb_regfile_bypass_en_i = 1;
    sim.tick();
    sim.check(sim.dut->ex_req_in2_o == 0xb1b2b3b4U, "src1 takes tenb bypass first");

    sim.dut->ex_tenb_regfile_bypass_en_i = 0;
    sim.dut->ex_bypass_force_shsw_in1_i = 1;
    advance_pipeline(sim, 0x0badc0deU, 0x09U);
    sim.tick();
    sim.check(sim.dut->ex_req_in2_o == 0x0badc0deU, "src1 can be forced from short-path data");

    sim.dut->ex_bypass_force_shsw_in1_i = 0;
    sim.dut->ex_req_fromint_i = 1;
    sim.dut->ex_req_txfma_i = 1;
    sim.tick();
    sim.check(sim.dut->ex_req_in3_o == 0x89abcdefU, "src2 takes fromint data only for TXFMA fromint ops");

    sim.dut->ex_req_fromint_i = 0;
    sim.dut->ex_req_txfma_i = 0;
    sim.dut->ex_req_use_prev_sigs_i = 1;
    sim.dut->ex_req_sigs_i = kAllSigs;
    sim.dut->ex_req_fromint_i = 1;
    sim.dut->ex_req_txfma_i = 1;
    sim.tick();
    sim.check(sim.dut->ex_req_use_prev_sigs_o == 1, "non-data request metadata still passes through");
    sim.check(sim.dut->ex_req_sigs_o != 0, "non-zero control bundle stays non-zero through the bypass path");

    sim.dut->ex_req_sigs_i = 0;
    sim.dut->lane_id_i = 1;
    sim.dut->f3_sh_sw_data_i = 1;
    sim.dut->f3_sh_sw_exc_i = 0x15U;
    sim.dut->f3_data_vector_i = 0;
    sim.tick();
    sim.check(sim.dut->f3_maskop_bit_o == 0, "scalar short-path data is zeroed on non-zero lanes");

    sim.dut->lane_id_i = 0;
    sim.tick();
    sim.check(sim.dut->f3_maskop_bit_o == 1, "lane 0 keeps scalar short-path data");

    advance_pipeline(sim, 0x12345678U, 0x12U);
    sim.tick();
    sim.check(sim.dut->f8_sh_sw_wdata_o == 0x12345678U, "short-path pipeline reaches F8");
    sim.check(sim.dut->f8_wdata_o == 0x12345678U, "F8 defaults to short-path writeback data");
    sim.check(sim.dut->f8_wflags_o == 0x12U, "F8 defaults to short-path flags");

    sim.dut->f8_txfma_data_i = 0xfacecafeU;
    sim.dut->f8_txfma_exc_i = 0x2aU;
    sim.dut->f8_txfma_op_i = 1;
    sim.tick();
    sim.check(sim.dut->f8_sh_sw_wdata_o == 0x12345678U, "raw short-path data stays available even when TXFMA wins");
    sim.check(sim.dut->f8_wdata_o == 0xfacecafeU, "TXFMA result overrides F8 writeback data");
    sim.check(sim.dut->f8_wflags_o == 0x2aU, "TXFMA result overrides F8 flags");

    sim.dut->lane_id_i = 1;
    sim.dut->f8_data_vector_i = 0;
    sim.tick();
    sim.check(sim.dut->f8_wdata_o == 0, "scalar F8 data is zeroed on non-zero lanes");
    sim.check(sim.dut->f8_wflags_o == 0x2aU, "scalar F8 lane gating preserves exception flags");

    return sim.finish();
}

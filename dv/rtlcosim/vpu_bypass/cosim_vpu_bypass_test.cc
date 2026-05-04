// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_vpu_bypass_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_vpu_bypass_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void set_defaults(DUT* dut) {
    dut->lane_id_i = 0;
    dut->ex_rf_raddr0_i = 0;
    dut->ex_rf_raddr1_i = 0;
    dut->ex_rf_raddr2_i = 0;
    dut->ex_regfile_thread_id0_i = 0;
    dut->ex_regfile_thread_id1_i = 0;
    dut->ex_regfile_thread_id2_i = 0;
    dut->ex_rf_rdata0_i = 0;
    dut->ex_rf_rdata1_i = 0;
    dut->ex_rf_rdata2_i = 0;
    dut->ex_req_use_prev_sigs_i = 0;
    dut->ex_req_sigs_i = 0;
    dut->ex_req_fromint_i = 0;
    dut->ex_req_txfma_i = 0;
    dut->ex_req_imm_i = 0;
    dut->ex_req_rm_i = 0;
    dut->ex_req_typ_i = 0;
    dut->ex_fromint_data_i = 0;
    dut->ex_tena_rf_rdata_i = 0;
    dut->ex_tenb_rf_rdata_i = 0;
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
    dut->f3_data_vector_i = 0;
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
    dut->f8_data_vector_i = 0;
    dut->f8_txfma_op_i = 0;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("ex_req_use_prev_sigs", sim.dut->orig_ex_req_use_prev_sigs_o, sim.dut->new_ex_req_use_prev_sigs_o);
    sim.compare("ex_req_sigs", sim.dut->orig_ex_req_sigs_o, sim.dut->new_ex_req_sigs_o);
    sim.compare("ex_req_imm", sim.dut->orig_ex_req_imm_o, sim.dut->new_ex_req_imm_o);
    sim.compare("ex_req_rm", sim.dut->orig_ex_req_rm_o, sim.dut->new_ex_req_rm_o);
    sim.compare("ex_req_typ", sim.dut->orig_ex_req_typ_o, sim.dut->new_ex_req_typ_o);
    sim.compare("ex_req_in1", sim.dut->orig_ex_req_in1_o, sim.dut->new_ex_req_in1_o);
    sim.compare("ex_req_in2", sim.dut->orig_ex_req_in2_o, sim.dut->new_ex_req_in2_o);
    sim.compare("ex_req_in3", sim.dut->orig_ex_req_in3_o, sim.dut->new_ex_req_in3_o);
    sim.compare("f3_maskop_bit", sim.dut->orig_f3_maskop_bit_o, sim.dut->new_f3_maskop_bit_o);
    sim.compare("f8_wflags", sim.dut->orig_f8_wflags_o, sim.dut->new_f8_wflags_o);
    sim.compare("f8_wdata", sim.dut->orig_f8_wdata_o, sim.dut->new_f8_wdata_o);
    sim.compare("f8_sh_sw_wdata", sim.dut->orig_f8_sh_sw_wdata_o, sim.dut->new_f8_sh_sw_wdata_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    set_defaults(sim.dut.get());
    sim.reset(4);

    sim.dut->f3_bypass_clk_en_i = 1;
    sim.dut->f4_bypass_clk_en_i = 1;
    sim.dut->f5_bypass_clk_en_i = 1;
    sim.dut->f6_bypass_clk_en_i = 1;
    sim.dut->f7_bypass_clk_en_i = 1;
    for (int i = 0; i < 6; ++i) tick_and_compare(sim);
    sim.dut->f3_bypass_clk_en_i = 0;
    sim.dut->f4_bypass_clk_en_i = 0;
    sim.dut->f5_bypass_clk_en_i = 0;
    sim.dut->f6_bypass_clk_en_i = 0;
    sim.dut->f7_bypass_clk_en_i = 0;

    sim.dut->lane_id_i = 3;
    sim.dut->ex_rf_raddr0_i = 5;
    sim.dut->ex_rf_raddr1_i = 9;
    sim.dut->ex_rf_raddr2_i = 11;
    sim.dut->ex_regfile_thread_id0_i = 1;
    sim.dut->ex_regfile_thread_id1_i = 1;
    sim.dut->ex_regfile_thread_id2_i = 1;
    sim.dut->ex_req_use_prev_sigs_i = 1;
    sim.dut->ex_req_sigs_i = 0x123456789ull;
    sim.dut->ex_req_fromint_i = 1;
    sim.dut->ex_req_txfma_i = 1;
    sim.dut->ex_req_imm_i = 0x55aa5u;
    sim.dut->ex_req_rm_i = 5;
    sim.dut->ex_req_typ_i = 2;
    sim.dut->ex_fromint_data_i = 0xdeadbeefu;
    sim.dut->ex_tena_rf_rdata_i = 0x89abcdefu;
    sim.dut->ex_tenb_rf_rdata_i = 0x76543210u;
    sim.dut->ex_tena_regfile_bypass_en_i = 1;
    sim.dut->ex_tenb_regfile_bypass_en_i = 1;
    sim.dut->ex_bypass_force_txfma_in0_i = 1;
    sim.dut->ex_bypass_force_txfma_in1_i = 1;
    sim.dut->ex_bypass_force_txfma_in2_i = 1;
    sim.dut->ex_bypass_force_shsw_in1_i = 1;
    sim.dut->ex_bypass_force_shsw_in2_i = 1;
    sim.dut->f3_sh_sw_data_i = 0x11111111u;
    sim.dut->f3_sh_sw_exc_i = 0x15u;
    sim.dut->f3_regfile_waddr_i = 5;
    sim.dut->f3_regfile_wmask_i = 1;
    sim.dut->f3_thread_id_i = 1;
    sim.dut->f3_bypass_clk_en_i = 1;
    sim.dut->f3_data_vector_i = 1;
    tick_and_compare(sim);

    sim.dut->f4_regfile_waddr_i = 9;
    sim.dut->f4_regfile_wmask_i = 1;
    sim.dut->f4_thread_id_i = 1;
    sim.dut->f4_bypass_clk_en_i = 1;
    sim.dut->f4_regfile_waddr_l_i = 11;
    sim.dut->f4_regfile_wmask_l_i = 3;
    sim.dut->f4_regfile_thread_id_l_i = 1;
    sim.dut->f4_regfile_wdata_l_i = 0x22222222u;
    tick_and_compare(sim);

    sim.dut->f5_regfile_waddr_i = 5;
    sim.dut->f5_regfile_wmask_i = 1;
    sim.dut->f5_thread_id_i = 1;
    sim.dut->f5_bypass_clk_en_i = 1;
    sim.dut->f6_regfile_waddr_i = 9;
    sim.dut->f6_regfile_wmask_i = 1;
    sim.dut->f6_thread_id_i = 1;
    sim.dut->f6_bypass_clk_en_i = 1;
    sim.dut->f7_regfile_waddr_i = 11;
    sim.dut->f7_regfile_wmask_i = 1;
    sim.dut->f7_thread_id_i = 1;
    sim.dut->f7_bypass_clk_en_i = 1;
    sim.dut->f8_regfile_waddr_i = 5;
    sim.dut->f8_regfile_wmask_i = 1;
    sim.dut->f8_thread_id_i = 1;
    sim.dut->f8_txfma_data_i = 0x33333333u;
    sim.dut->f8_txfma_exc_i = 0x2au;
    sim.dut->f8_data_vector_i = 1;
    sim.dut->f8_txfma_op_i = 1;
    tick_and_compare(sim);

    uint32_t seed = 0x5eeda123U;
    for (int i = 0; i < 8192; ++i) {
        sim.dut->lane_id_i = xorshift32(seed) & 7U;
        sim.dut->ex_rf_raddr0_i = xorshift32(seed) & 31U;
        sim.dut->ex_rf_raddr1_i = xorshift32(seed) & 31U;
        sim.dut->ex_rf_raddr2_i = xorshift32(seed) & 31U;
        sim.dut->ex_regfile_thread_id0_i = xorshift32(seed) & 1U;
        sim.dut->ex_regfile_thread_id1_i = xorshift32(seed) & 1U;
        sim.dut->ex_regfile_thread_id2_i = xorshift32(seed) & 1U;
        sim.dut->ex_rf_rdata0_i = xorshift32(seed);
        sim.dut->ex_rf_rdata1_i = xorshift32(seed);
        sim.dut->ex_rf_rdata2_i = xorshift32(seed);
        sim.dut->ex_req_use_prev_sigs_i = xorshift32(seed) & 1U;
        sim.dut->ex_req_sigs_i = (uint64_t(xorshift32(seed)) << 16) ^ xorshift32(seed);
        sim.dut->ex_req_fromint_i = xorshift32(seed) & 1U;
        sim.dut->ex_req_txfma_i = xorshift32(seed) & 1U;
        sim.dut->ex_req_imm_i = xorshift32(seed) & 0xFFFFFU;
        sim.dut->ex_req_rm_i = xorshift32(seed) & 7U;
        sim.dut->ex_req_typ_i = xorshift32(seed) & 3U;
        sim.dut->ex_fromint_data_i = xorshift32(seed);
        sim.dut->ex_tena_rf_rdata_i = xorshift32(seed);
        sim.dut->ex_tenb_rf_rdata_i = xorshift32(seed);
        sim.dut->ex_tena_regfile_bypass_en_i = xorshift32(seed) & 1U;
        sim.dut->ex_tenb_regfile_bypass_en_i = xorshift32(seed) & 1U;
        sim.dut->ex_bypass_force_txfma_in0_i = xorshift32(seed) & 1U;
        sim.dut->ex_bypass_force_txfma_in1_i = xorshift32(seed) & 1U;
        sim.dut->ex_bypass_force_txfma_in2_i = xorshift32(seed) & 1U;
        sim.dut->ex_bypass_force_shsw_in1_i = xorshift32(seed) & 1U;
        sim.dut->ex_bypass_force_shsw_in2_i = xorshift32(seed) & 1U;
        sim.dut->f3_sh_sw_data_i = xorshift32(seed);
        sim.dut->f3_sh_sw_exc_i = xorshift32(seed) & 0x3fU;
        sim.dut->f3_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f3_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f3_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f3_bypass_clk_en_i = xorshift32(seed) & 1U;
        sim.dut->f3_data_vector_i = xorshift32(seed) & 1U;
        sim.dut->f4_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f4_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f4_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f4_bypass_clk_en_i = xorshift32(seed) & 1U;
        sim.dut->f4_regfile_waddr_l_i = xorshift32(seed) & 31U;
        sim.dut->f4_regfile_wmask_l_i = xorshift32(seed) & 3U;
        sim.dut->f4_regfile_thread_id_l_i = xorshift32(seed) & 1U;
        sim.dut->f4_regfile_wdata_l_i = xorshift32(seed);
        sim.dut->f5_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f5_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f5_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f5_bypass_clk_en_i = xorshift32(seed) & 1U;
        sim.dut->f6_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f6_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f6_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f6_bypass_clk_en_i = xorshift32(seed) & 1U;
        sim.dut->f7_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f7_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f7_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f7_bypass_clk_en_i = xorshift32(seed) & 1U;
        sim.dut->f8_regfile_waddr_i = xorshift32(seed) & 31U;
        sim.dut->f8_regfile_wmask_i = xorshift32(seed) & 1U;
        sim.dut->f8_thread_id_i = xorshift32(seed) & 1U;
        sim.dut->f8_txfma_data_i = xorshift32(seed);
        sim.dut->f8_txfma_exc_i = xorshift32(seed) & 0x3fU;
        sim.dut->f8_data_vector_i = xorshift32(seed) & 1U;
        sim.dut->f8_txfma_op_i = xorshift32(seed) & 1U;
        tick_and_compare(sim);
    }

    sim.check(sim.comparisons >= 90000, "vpu_bypass cosim should exceed 90k comparisons");
    return sim.finish();
}

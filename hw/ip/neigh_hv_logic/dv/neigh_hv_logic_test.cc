// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_hv_logic_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vneigh_hv_logic_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->apb_pready_neigh_i = 0;
  dut->apb_req_psel_i = 0;
  dut->apb_req_penable_i = 0;
  dut->apb_rsp_valid_i = 0;
  dut->apb_rsp_pop_prdata_i = 0;
  dut->apb_rsp_pop_pready_i = 0;
  dut->apb_rsp_pop_pslverr_i = 0;
  dut->bpam_run_control_i = 0;
  dut->dmctrl_write_enable_i = 0;
  dut->bpam_ack_halt_i = 0;
  dut->bpam_ack_resume_i = 0;
  dut->clk_shire_fb_i = 0;
  dut->clk_neigh_fb_i = 0;
  dut->icache_err_datain_i = 0;
  dut->ipi_trigger_i = 0;
  dut->dft_tdr_ctrl_i = 0;
  dut->pwr_ctrl_glb_isolate_i = 0;
  dut->uc_to_neigh_fcc_i = 0;
  dut->l2_bank_first_half_i = 0;
  dut->l2_bank_id_i = 0;
  dut->l2_bank_source_i = 0;
  dut->l2_bank_wdata_i = 0;
  dut->l2_bank_opcode_i = 0;
  dut->l2_bank_subopcode_i = 0;
  dut->l2_bank_address_i = 0;
  dut->l2_bank_size_i = 0;
  dut->l2_bank_qwen_i = 0;
  dut->l2_bank_data0_i = 0;
  dut->l2_bank_data1_i = 0;
  dut->l2_bank_data2_i = 0;
  dut->l2_bank_data3_i = 0;
  dut->l2_bank_valid_i = 0;
  dut->l2_bank_ready_i = 0;
  dut->sc_bank_valid_i = 0;
  dut->sc_req_ready_i = 0;
  dut->sc_bank0_id_i = 0;
  dut->sc_bank1_id_i = 0;
  dut->sc_bank2_id_i = 0;
  dut->sc_bank3_id_i = 0;
  dut->sc_bank0_data0_i = 0;
  dut->sc_bank1_data0_i = 0;
  dut->sc_bank2_data0_i = 0;
  dut->sc_bank3_data0_i = 0;
  dut->sc_uc_valid_i = 0;
  dut->sc_uc_id_i = 0;
  dut->sc_uc_wdata_i = 0;
  dut->sc_uc_data0_i = 0;
}

void eval(DUT* dut) { dut->eval(); }

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->apb_req_push_o == 0, "apb req push reset");
  sim.check(sim.dut->bpam_push_o == 0, "bpam push reset");
  sim.check(sim.dut->bpam_ack_halt_o == 0, "bpam halt ack reset");
  sim.check(sim.dut->bpam_ack_resume_o == 0, "bpam resume ack reset");
  sim.check(sim.dut->icache_err_dataout_o == 0, "icache error flop reset");
  sim.check(sim.dut->ct_one_o == 1, "constant one");

  // APB request push and wait-for-ready behavior.
  sim.dut->apb_req_psel_i = 1;
  eval(sim.dut.get());
  sim.check(sim.dut->apb_req_push_o == 1, "apb psel pushes when not waiting");
  sim.dut->apb_req_penable_i = 1;
  sim.tick();
  sim.check(sim.dut->apb_req_push_o == 0, "apb penable enters ready wait");
  sim.dut->apb_pready_neigh_i = 1;
  sim.tick();
  sim.check(sim.dut->apb_req_push_o == 1, "apb ready clears wait");
  sim.dut->apb_req_psel_i = 0;
  sim.dut->apb_req_penable_i = 0;
  sim.dut->apb_pready_neigh_i = 0;

  // APB response synchronizer passes only valid payloads.
  sim.dut->apb_rsp_pop_prdata_i = 0x1122334455667788ull;
  sim.dut->apb_rsp_pop_pready_i = 1;
  sim.dut->apb_rsp_pop_pslverr_i = 1;
  sim.dut->apb_rsp_valid_i = 0;
  eval(sim.dut.get());
  sim.check(sim.dut->apb_rsp_sync_prdata_o == 0, "apb rsp zero when invalid");
  sim.check(sim.dut->apb_rsp_sync_pready_o == 0, "apb rsp pready zero when invalid");
  sim.dut->apb_rsp_valid_i = 1;
  eval(sim.dut.get());
  sim.check(sim.dut->apb_rsp_sync_prdata_o == 0x1122334455667788ull,
            "apb rsp prdata passes when valid");
  sim.check(sim.dut->apb_rsp_sync_pready_o == 1, "apb rsp pready passes");
  sim.check(sim.dut->apb_rsp_sync_pslverr_o == 1, "apb rsp pslverr passes");

  // BPAM push asserts on data changes or explicit dmctrl writes.
  sim.dut->bpam_run_control_i = 0x5;
  eval(sim.dut.get());
  sim.check(sim.dut->bpam_push_o == 1, "bpam changed run-control pushes");
  sim.tick();
  sim.check(sim.dut->bpam_push_o == 0, "bpam unchanged run-control does not push");
  sim.dut->dmctrl_write_enable_i = 1;
  eval(sim.dut.get());
  sim.check(sim.dut->bpam_push_o == 1, "bpam dmctrl write forces push");
  sim.dut->dmctrl_write_enable_i = 0;

  // BPAM TBOX acknowledge register.
  sim.dut->bpam_ack_halt_i = 1;
  sim.dut->bpam_ack_resume_i = 1;
  sim.tick();
  sim.check(sim.dut->bpam_ack_halt_o == 1, "bpam halt ack registered");
  sim.check(sim.dut->bpam_ack_resume_o == 1, "bpam resume ack registered");
  sim.dut->bpam_ack_halt_i = 0;
  sim.dut->bpam_ack_resume_i = 0;
  sim.tick();
  sim.check(sim.dut->bpam_ack_halt_o == 0, "bpam halt ack clears with input");
  sim.check(sim.dut->bpam_ack_resume_o == 0, "bpam resume ack clears with input");

  // Clock feedback and simple combinational support leaves.
  sim.dut->clk_shire_fb_i = 0;
  sim.dut->clk_neigh_fb_i = 0;
  eval(sim.dut.get());
  sim.check(sim.dut->dll_feedback_shire_o == 1, "shire feedback inverts low clock");
  sim.check(sim.dut->dll_feedback_neigh_o == 0, "neigh feedback buffers low clock");
  sim.dut->clk_shire_fb_i = 1;
  sim.dut->clk_neigh_fb_i = 1;
  eval(sim.dut.get());
  sim.check(sim.dut->dll_feedback_shire_o == 0, "shire feedback inverts high clock");
  sim.check(sim.dut->dll_feedback_neigh_o == 1, "neigh feedback buffers high clock");

  sim.dut->ipi_trigger_i = 0;
  sim.dut->uc_to_neigh_fcc_i = 0;
  sim.dut->pwr_ctrl_glb_isolate_i = 1;
  sim.dut->dft_tdr_ctrl_i = 0;
  eval(sim.dut.get());
  sim.check(sim.dut->ipi_trigger_push_o == 0, "ipi no trigger");
  sim.check(sim.dut->neigh_uc_fcc_push_o == 0, "uc fcc no trigger");
  sim.check(sim.dut->pwr_ctrl_glb_isolate_o == 1, "isolation passes when TDR inactive");
  sim.dut->ipi_trigger_i = 0x8001;
  sim.dut->uc_to_neigh_fcc_i = 0x80;
  sim.dut->dft_tdr_ctrl_i = 1;
  eval(sim.dut.get());
  sim.check(sim.dut->ipi_trigger_push_o == 1, "ipi OR-reduces triggers");
  sim.check(sim.dut->neigh_uc_fcc_push_o == 1, "uc fcc OR-reduces FCC bits");
  sim.check(sim.dut->pwr_ctrl_glb_isolate_o == 0, "TDR deasserts isolation output");

  // ICache error toggle-capture path.
  sim.dut->icache_err_datain_i = 1;
  sim.tick();
  sim.check(sim.dut->icache_err_dataout_o == 1, "icache err captures asserted input");
  sim.tick();
  sim.check(sim.dut->icache_err_dataout_o == 1, "icache err holds unchanged input");
  sim.dut->icache_err_datain_i = 0;
  sim.tick();
  sim.check(sim.dut->icache_err_dataout_o == 0, "icache err captures deasserted input");

  // L2 bank one-half request: data is replicated into both halves.
  sim.dut->l2_bank_id_i = 0x44;
  sim.dut->l2_bank_source_i = 0x3;
  sim.dut->l2_bank_wdata_i = 1;
  sim.dut->l2_bank_opcode_i = 4;
  sim.dut->l2_bank_subopcode_i = 0x12;
  sim.dut->l2_bank_address_i = 0xabcde;
  sim.dut->l2_bank_size_i = 5;
  sim.dut->l2_bank_qwen_i = 0xf;
  sim.dut->l2_bank_data0_i = 0x0101010101010101ull;
  sim.dut->l2_bank_data1_i = 0x0202020202020202ull;
  sim.dut->l2_bank_data2_i = 0x0303030303030303ull;
  sim.dut->l2_bank_data3_i = 0x0404040404040404ull;
  sim.dut->l2_bank_valid_i = 1;
  sim.dut->l2_bank_ready_i = 1;
  sim.dut->l2_bank_first_half_i = 0;
  eval(sim.dut.get());
  sim.check(sim.dut->l2_bank_info_pop_o == 1, "l2 bank pops one-half request");
  sim.check(sim.dut->l2_bank_data_pop_o == 1, "l2 bank pops write data");
  sim.check(sim.dut->l2_miss_valid_o == 1, "l2 bank one-half request valid");
  sim.check(sim.dut->l2_miss_id_o == 0x44, "l2 bank id forwarded");
  sim.check(sim.dut->l2_miss_data0_o == 0x0101010101010101ull,
            "l2 bank low half word0 replicated");
  sim.check(sim.dut->l2_miss_data4_o == 0x0101010101010101ull,
            "l2 bank high half word0 replicated");

  // L2 bank two-half evict request: first half is stored, second half is joined.
  sim.dut->l2_bank_first_half_i = 1;
  sim.dut->l2_bank_ready_i = 0;
  sim.dut->l2_bank_data0_i = 0x1111111111111111ull;
  sim.dut->l2_bank_data1_i = 0x2222222222222222ull;
  sim.dut->l2_bank_data2_i = 0x3333333333333333ull;
  sim.dut->l2_bank_data3_i = 0x4444444444444444ull;
  eval(sim.dut.get());
  sim.check(sim.dut->l2_bank_info_pop_o == 1, "l2 bank pops first half without L2 ready");
  sim.check(sim.dut->l2_miss_valid_o == 0, "l2 bank first half is not output valid");
  sim.tick();
  sim.dut->l2_bank_first_half_i = 0;
  sim.dut->l2_bank_ready_i = 1;
  sim.dut->l2_bank_data0_i = 0xaaaaaaaaaaaaaaaaull;
  sim.dut->l2_bank_data1_i = 0xbbbbbbbbbbbbbbbbull;
  sim.dut->l2_bank_data2_i = 0xccccccccccccccccull;
  sim.dut->l2_bank_data3_i = 0xddddddddddddddddull;
  eval(sim.dut.get());
  sim.check(sim.dut->l2_miss_valid_o == 1, "l2 bank second half output valid");
  sim.check(sim.dut->l2_miss_data0_o == 0x1111111111111111ull,
            "l2 bank joined first-half word0 low");
  sim.check(sim.dut->l2_miss_data4_o == 0xaaaaaaaaaaaaaaaaull,
            "l2 bank joined second-half word0 high");
  sim.tick();
  sim.dut->l2_bank_valid_i = 0;

  // Shire-cache request mux/arbiter: bank path.
  sim.dut->sc_bank0_id_i = 0x10;
  sim.dut->sc_bank1_id_i = 0x11;
  sim.dut->sc_bank2_id_i = 0x12;
  sim.dut->sc_bank3_id_i = 0x13;
  sim.dut->sc_bank0_data0_i = 0x1000;
  sim.dut->sc_bank1_data0_i = 0x2000;
  sim.dut->sc_bank2_data0_i = 0x3000;
  sim.dut->sc_bank3_data0_i = 0x4000;
  sim.dut->sc_bank_valid_i = 0x2;
  sim.dut->sc_req_ready_i = 0x2;
  eval(sim.dut.get());
  sim.check(sim.dut->sc_req_valid_o == 0x2, "neigh_sc grants ready bank1");
  sim.check(sim.dut->sc_bank_ready_o == 0x2, "neigh_sc bank ready one-hot");
  sim.check(sim.dut->sc_req_id_o == 0x11, "neigh_sc bank id selected");
  sim.check(sim.dut->sc_req_data0_o == 0x2000, "neigh_sc bank data selected");
  sim.tick();

  // Shire-cache request mux/arbiter: UC path with replicated 256-bit data.
  sim.dut->sc_bank_valid_i = 0;
  sim.dut->sc_uc_valid_i = 1;
  sim.dut->sc_uc_wdata_i = 1;
  sim.dut->sc_uc_id_i = 0x55;
  sim.dut->sc_uc_data0_i = 0xabcdef0123456789ull;
  sim.dut->sc_req_ready_i = 0x10;
  eval(sim.dut.get());
  sim.check(sim.dut->sc_req_valid_o == 0x10, "neigh_sc grants UC port");
  sim.check(sim.dut->sc_uc_info_pop_o == 1, "neigh_sc pops UC info");
  sim.check(sim.dut->sc_uc_data_pop_o == 1, "neigh_sc pops UC data when wdata");
  sim.check(sim.dut->sc_req_id_o == 0x55, "neigh_sc UC id selected");
  sim.check(sim.dut->sc_req_data0_o == 0xabcdef0123456789ull,
            "neigh_sc UC low data selected");
  sim.check(sim.dut->sc_req_data4_o == 0xabcdef0123456789ull,
            "neigh_sc UC high data replicated");

  sim.dut->sc_req_ready_i = 0;
  eval(sim.dut.get());
  sim.check(sim.dut->sc_req_valid_o == 0, "neigh_sc no grant when downstream not ready");
  sim.check(sim.dut->sc_uc_info_pop_o == 0, "neigh_sc does not pop UC when not granted");

  return sim.finish();
}

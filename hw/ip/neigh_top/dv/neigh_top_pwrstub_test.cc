// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_top_pwrstub_tb.h"
#include "sim_ctrl.h"

int main(int argc, char** argv) {
    SimCtrl<Vneigh_top_pwrstub_tb> sim(argc, argv);
    auto* dut = sim.dut.get();

    dut->isolate_i = 0;
    dut->coop_slv_valid_i = 1;
    dut->coop_mst_valid_i = 0x5;
    dut->apb_paddr_i = 0x15555;
    dut->apb_penable_i = 1;
    dut->apb_psel_i = 1;
    dut->apb_pwdata_i = 0x1122334455667788ull;
    dut->apb_pwrite_i = 1;
    dut->apb_prdata_i = 0x8877665544332211ull;
    dut->apb_pready_i = 0;
    dut->apb_pslverr_i = 0;
    dut->req_ready_i = 0x7;
    dut->req_valid_i = 0x3;
    dut->rsp_ready_i = 0;
    dut->rsp_valid_i = 1;
    dut->prefetch_done_i = 0;
    dut->icache_err_detected_i = 1;
    dut->icache_err_logged_i = 1;
    dut->icache_resp_valid_i = 1;
    dut->icache_resp_ready_i = 0;
    dut->esr_tree_i = 0x2aa;
    dut->flb_req_valid_i = 1;
    dut->flb_resp_valid_i = 1;
    dut->icache_req_write_i = 1;
    dut->icache_req_valid_i = 1;
    dut->icache_req_ready_i = 1;
    dut->bpam_halt_ack_i = 0;
    dut->bpam_resume_ack_i = 0;
    dut->reset_w_icache_i = 1;
    dut->eco_i = 0x155;
    dut->pwr_glb_nsleepout_i = 1;
    dut->pwr_min_nsleepout_i = 0xa5;
    dut->eval();

    sim.check(dut->coop_slv_valid_o == 1, "pwrstub pass-through coop slave valid");
    sim.check(dut->coop_mst_valid_o == 0x5, "pwrstub pass-through coop master valid");
    sim.check(dut->apb_paddr_o == 0x15555, "pwrstub always passes APB address");
    sim.check(dut->apb_penable_o == 1, "pwrstub pass-through APB penable");
    sim.check(dut->apb_psel_o == 1, "pwrstub pass-through APB psel");
    sim.check(dut->apb_pwdata_o == 0x1122334455667788ull, "pwrstub always passes APB write data");
    sim.check(dut->apb_pwrite_o == 1, "pwrstub always passes APB write flag");
    sim.check(dut->apb_prdata_o == 0x8877665544332211ull, "pwrstub always passes APB read data");
    sim.check(dut->apb_pready_o == 0, "pwrstub pass-through APB ready");
    sim.check(dut->apb_pslverr_o == 0, "pwrstub pass-through APB error");
    sim.check(dut->req_ready_o == 0x7, "pwrstub pass-through SC request ready");
    sim.check(dut->req_valid_o == 0x3, "pwrstub pass-through SC request valid");
    sim.check(dut->rsp_ready_o == 0, "pwrstub pass-through SC response ready");
    sim.check(dut->rsp_valid_o == 1, "pwrstub pass-through SC response valid");
    sim.check(dut->prefetch_done_o == 0, "pwrstub pass-through prefetch done");
    sim.check(dut->icache_err_detected_o == 1, "pwrstub pass-through icache detected error");
    sim.check(dut->icache_err_logged_o == 1, "pwrstub pass-through icache logged error");
    sim.check(dut->icache_resp_valid_o == 1, "pwrstub pass-through icache response valid");
    sim.check(dut->icache_resp_ready_o == 0, "pwrstub pass-through icache response ready");
    sim.check(dut->esr_tree_o == 0x2aa, "pwrstub pass-through ESR and/or tree");
    sim.check(dut->flb_req_valid_o == 1, "pwrstub pass-through FLB request valid");
    sim.check(dut->flb_resp_valid_o == 1, "pwrstub pass-through FLB response valid");
    sim.check(dut->icache_req_write_o == 1, "pwrstub pass-through icache request write");
    sim.check(dut->icache_req_valid_o == 1, "pwrstub pass-through icache request valid");
    sim.check(dut->icache_req_ready_o == 1, "pwrstub pass-through icache request ready");
    sim.check(dut->bpam_halt_ack_o == 0, "pwrstub pass-through BPAM halt ack");
    sim.check(dut->bpam_resume_ack_o == 0, "pwrstub pass-through BPAM resume ack");
    sim.check(dut->reset_w_icache_o == 1, "pwrstub pass-through icache reset payload");
    sim.check(dut->eco_o == 0x155, "pwrstub pass-through ECO output");
    sim.check(dut->pwr_glb_nsleepout_o == 1, "pwrstub pass-through global nsleepout");
    sim.check(dut->pwr_min_nsleepout_o == 0xa5, "pwrstub pass-through minion nsleepout");

    dut->isolate_i = 1;
    dut->eval();
    sim.check(dut->coop_slv_valid_o == 0, "isolation clears coop slave valid");
    sim.check(dut->coop_mst_valid_o == 0, "isolation clears coop master valid");
    sim.check(dut->apb_paddr_o == 0x15555, "isolation still passes APB address");
    sim.check(dut->apb_penable_o == 0, "isolation clears APB penable");
    sim.check(dut->apb_psel_o == 0, "isolation clears APB psel");
    sim.check(dut->apb_pwdata_o == 0x1122334455667788ull, "isolation still passes APB write data");
    sim.check(dut->apb_pwrite_o == 1, "isolation still passes APB write flag");
    sim.check(dut->apb_prdata_o == 0x8877665544332211ull, "isolation still passes APB read data");
    sim.check(dut->apb_pready_o == 1, "isolation auto-completes APB");
    sim.check(dut->apb_pslverr_o == 1, "isolation raises APB error");
    sim.check(dut->req_ready_o == 0, "isolation clears SC request ready");
    sim.check(dut->req_valid_o == 0, "isolation clears SC request valid");
    sim.check(dut->rsp_ready_o == 1, "isolation auto-accepts SC responses");
    sim.check(dut->rsp_valid_o == 0, "isolation drops SC response valid");
    sim.check(dut->prefetch_done_o == 1, "isolation returns prefetch done reset value");
    sim.check(dut->icache_err_detected_o == 0, "isolation clears icache detected error");
    sim.check(dut->icache_err_logged_o == 0, "isolation clears icache logged error");
    sim.check(dut->icache_resp_valid_o == 0, "isolation drops icache response valid");
    sim.check(dut->icache_resp_ready_o == 1, "isolation auto-accepts icache response");
    sim.check(dut->esr_tree_o == 0, "isolation clears ESR and/or tree");
    sim.check(dut->flb_req_valid_o == 0, "isolation clears FLB request valid");
    sim.check(dut->flb_resp_valid_o == 0, "isolation clears FLB response valid");
    sim.check(dut->icache_req_write_o == 0, "isolation clears icache request write");
    sim.check(dut->icache_req_valid_o == 0, "isolation clears icache request valid");
    sim.check(dut->icache_req_ready_o == 0, "isolation clears icache request ready");
    sim.check(dut->bpam_halt_ack_o == 1, "isolation acknowledges BPAM halt");
    sim.check(dut->bpam_resume_ack_o == 1, "isolation acknowledges BPAM resume");
    sim.check(dut->reset_w_icache_o == 0, "isolation clears icache reset payload");
    sim.check(dut->eco_o == 0, "isolation clears ECO output");
    sim.check(dut->pwr_glb_nsleepout_o == 0, "isolation clears global nsleepout");
    sim.check(dut->pwr_min_nsleepout_o == 0, "isolation clears minion nsleepout");

    return sim.finish();
}

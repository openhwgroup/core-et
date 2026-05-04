// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_cache_op_unit_l2.

#include "Vminion_dcache_cache_op_unit_l2_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_cache_op_unit_l2_tb;

namespace {

void clear_inputs(DUT* dut) {
  dut->shire_id_i = 0;
  dut->cacheop_max_i = 0;
  dut->cacheop_reprate_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->l2_resp_valid_i = 0;
  dut->l2_resp_id_i = 0;
  dut->l2_resp_opcode_i = 0;
  dut->cache_op_valid_i = 0;
  dut->cache_op_cmd_i = 0;
  dut->cache_op_start_level_i = 0;
  dut->cache_op_dest_level_i = 0;
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0;
  dut->cache_op_stride_i = 0;
  dut->cache_op_thread_id_i = 0;
  dut->cache_op_use_tmask_i = 0;
  dut->cache_op_tmask_ready_i = 0;
  dut->cache_op_tmask_bits_i = 0;
  dut->cache_op_l2_scp_dest_i = 0;
  dut->cache_op_l2_scp_fill_i = 0;
  dut->l2_req_ready_i = 0;
  dut->cache_op_valid_byp_i = 0;
  dut->cache_op_byp_cmd_i = 0;
  dut->cache_op_byp_start_level_i = 0;
  dut->cache_op_byp_dest_level_i = 0;
  dut->cache_op_byp_count_i = 0;
  dut->cache_op_byp_addr_i = 0;
  dut->cache_op_byp_stride_i = 0;
  dut->cache_op_byp_thread_id_i = 0;
  dut->cache_op_byp_use_tmask_i = 0;
  dut->cache_op_byp_tmask_bits_i = 0;
  dut->cache_op_byp_l2_scp_dest_i = 0;
  dut->cache_op_byp_l2_scp_fill_i = 0;
  dut->s0_available_i = 0;
  dut->s1_available_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->s1_tlb_addr_i = 0;
  dut->vm_status_debug_i = 0;
  dut->vm_status_sum_i = 0;
  dut->vm_status_mxr_i = 0;
  dut->vm_status_prv_i = 0;
  dut->vm_status_mpp_i = 0;
  dut->vm_status_mprv_i = 0;
  dut->vm_status_co_prv_i = 0;
  dut->vm_status_co_mpp_i = 0;
  dut->vm_status_co_mprv_i = 0;
  dut->vm_status_co_sum_i = 0;
  dut->vm_status_co_mxr_i = 0;
  dut->vm_status_co_debug_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void drive_prefetch_req(DUT* dut) {
  dut->cache_op_cmd_i = 4;          // cache_op_Pref
  dut->cache_op_start_level_i = 0;  // L1
  dut->cache_op_dest_level_i = 1;   // L2
  dut->cache_op_count_i = 0;
  dut->cache_op_addr_i = 0x12340;
  dut->cache_op_stride_i = 0x40;
  dut->cache_op_thread_id_i = 1;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->busy_o == 0, "reset leaves cache-op-l2 idle");
  sim.check(sim.dut->cache_op_ready_o == 0, "reset does not report a valid L2 op until request decode is driven");
  sim.check(sim.dut->cache_op_ready_byp_o == 1, "reset accepts bypass work immediately");
  sim.check(sim.dut->th_id_complete_o == 0xF, "reset reports all thread/id slots complete");

  drive_prefetch_req(sim.dut.get());
  sim.dut->cache_op_valid_i = 1;
  tick_and_eval(sim);
  sim.dut->eval();
  sim.check(sim.dut->cache_op_ready_o == 1, "prefetch to L2 is accepted from idle");
  tick_and_eval(sim);

  sim.check(sim.dut->busy_o == 1, "accepted prefetch leaves idle state");
  sim.check(sim.dut->vm_status_l2_prv_o == 0, "non-bypass request captures thread-1 VM status by default");
  sim.check(sim.dut->s0_addr_o == 0x12340, "translate state forwards the aligned virtual address");
  sim.check(sim.dut->s0_thread_id_o == 1, "translate state forwards the request thread");

  sim.dut->cache_op_valid_i = 0;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->s1_tlb_addr_i = 0x15555;
  tick_and_eval(sim);
  sim.check(sim.dut->s1_addr_load_o == 1, "available TLB pipeline advances into stage 1");

  tick_and_eval(sim);
  sim.dut->eval();
  sim.check(sim.dut->l2_req_valid_o == 1, "translated request issues to L2");
  sim.check(sim.dut->l2_req_id_o == 14, "thread-1/id-0 request maps to L2 prefetch slot 2");
  sim.check(sim.dut->l2_req_opcode_o == 15, "prefetch emits ET_LINK_REQ_Prefetch");
  sim.check(sim.dut->l2_req_subopcode_o == 0, "prefetch uses zero subopcode");
  sim.check(sim.dut->l2_req_address_o == ((0x15555ULL << 12) | 0x340ULL),
            "prefetch request combines translated and untranslated address bits");
  sim.check(sim.dut->l2_req_data_low_o == 0x20, "prefetch request encodes dest/start level metadata");
  sim.check(sim.dut->l2_req_size_o == 0, "prefetch uses byte size");
  sim.check(sim.dut->l2_req_qwen_o == 0x1, "prefetch uses single-lane qwen");
  sim.dut->l2_req_ready_i = 1;
  tick_and_eval(sim);

  sim.check(sim.dut->busy_o == 0, "single prefetch request retires after successful L2 issue");
  sim.check(((sim.dut->th_id_complete_o >> 2) & 0x1) == 0,
            "the thread-1/id-0 completion bit stays low while a response is outstanding");

  sim.dut->l2_req_ready_i = 0;
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_id_i = 14;
  sim.dut->l2_resp_opcode_i = 3;
  sim.dut->eval();
  sim.check(sim.dut->bus_err_o == 0x2, "error response raises the thread-1 bus error bit");
  tick_and_eval(sim);
  sim.dut->l2_resp_valid_i = 0;
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->th_id_complete_o == 0xF, "response completion clears the outstanding slot");

  reset_and_clear(sim);
  sim.dut->cache_op_valid_byp_i = 1;
  sim.dut->cache_op_byp_cmd_i = 4;          // cache_op_Pref
  sim.dut->cache_op_byp_start_level_i = 1;  // L2
  sim.dut->cache_op_byp_dest_level_i = 2;   // L3
  sim.dut->cache_op_byp_addr_i = 0x8000;
  sim.dut->cache_op_byp_stride_i = 1;
  sim.dut->cache_op_byp_thread_id_i = 0;
  sim.dut->vm_status_co_prv_i = 3;
  sim.dut->vm_status_co_mpp_i = 1;
  sim.dut->vm_status_co_mprv_i = 1;
  sim.dut->vm_status_co_sum_i = 1;
  sim.dut->vm_status_co_mxr_i = 1;
  sim.dut->vm_status_co_debug_i = 1;
  sim.dut->eval();
  sim.check(sim.dut->cache_op_ready_byp_o == 1, "idle unit accepts bypass request");
  tick_and_eval(sim);
  sim.check(sim.dut->vm_status_l2_prv_o == 3, "bypass request captures vm_status_co");
  sim.check(sim.dut->vm_status_l2_mpp_o == 1, "bypass VM mpp is preserved");
  sim.check(sim.dut->vm_status_l2_mprv_o == 1, "bypass VM mprv is preserved");
  sim.check(sim.dut->vm_status_l2_sum_o == 1, "bypass VM sum is preserved");
  sim.check(sim.dut->vm_status_l2_mxr_o == 1, "bypass VM mxr is preserved");
  sim.check(sim.dut->vm_status_l2_debug_o == 1, "bypass VM debug is preserved");

  reset_and_clear(sim);
  sim.dut->cache_op_valid_i = 1;
  sim.dut->cache_op_cmd_i = 4;          // cache_op_Pref
  sim.dut->cache_op_start_level_i = 0;  // L1
  sim.dut->cache_op_dest_level_i = 3;   // MEM
  tick_and_eval(sim);
  sim.dut->eval();
  sim.check(sim.dut->cache_op_ready_o == 1, "NOP-style prefetch-to-MEM is still acknowledged");
  tick_and_eval(sim);
  sim.check(sim.dut->busy_o == 0, "NOP request does not leave the unit busy");

  return sim.finish();
}

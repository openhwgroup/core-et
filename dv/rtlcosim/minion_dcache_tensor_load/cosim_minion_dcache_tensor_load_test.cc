// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: dcache_tensor_load vs minion_dcache_tensor_load.

#include "Vcosim_minion_dcache_tensor_load_tb.h"
#include "cosim_ctrl.h"

#include <cstdlib>
#include <cstdint>
#include <string>

using DUT = Vcosim_minion_dcache_tensor_load_tb;

namespace {

constexpr int kRandomCycles = 12000;

uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

void clear_inputs(DUT* dut) {
  dut->shire_min_id_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->tl_use_tmask_i = 0;
  dut->tl_use_coop_i = 0;
  dut->tl_transform_i = 0;
  dut->tl_scp_dest_i = 0;
  dut->tl_sel_i = 0;
  dut->tl_addr_i = 0;
  dut->tl_offset_i = 0;
  dut->tl_num_lines_i = 0;
  dut->tl_stride_i = 0;
  dut->tl_start_i = 0;
  dut->tl_restart_b_i = 0;
  dut->tl_tmask_ready_i = 0;
  dut->tl_tmask_bits_i = 0;
  dut->tl_neigh_mask_i = 0;
  dut->tl_minion_mask_i = 0;
  dut->tl_coop_id_i = 0;
  dut->thread_id_i = 0;
  dut->s0_available_i = 0;
  dut->s1_available_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->s2_addr_i = 0;
  dut->l2_req_ready_coop_i = 0;
  dut->l2_req_ready_no_coop_i = 0;
  dut->l2_resp_valid_i = 0;
  dut->l2_resp_id_i = 0;
  dut->l2_resp_opcode_i = 0;
  for (int i = 0; i < 8; ++i) dut->l2_resp_data_i[i] = 0;
  dut->load_ready_i = 0;
  dut->tfma_enabled_i = 0;
  dut->tenb_tstore_busy_i = 0;
  dut->tenb_credit_i = 0;
  dut->tenb_credit_entry_i = 0;
  dut->tenb_written_i = 0;
}

void compare_one(CosimCtrl<DUT>& sim, int idx) {
  if (idx == 0) {
    sim.compare("busy0", sim.dut->orig_busy0_o, sim.dut->new_busy0_o);
    sim.compare("id_complete0", sim.dut->orig_id_complete0_o, sim.dut->new_id_complete0_o);
    sim.compare("s0_addr_load0", sim.dut->orig_s0_addr_load0_o, sim.dut->new_s0_addr_load0_o);
    sim.compare("s0_addr0", sim.dut->orig_s0_addr0_o, sim.dut->new_s0_addr0_o);
    sim.compare("s0_addr_msb_err0", sim.dut->orig_s0_addr_msb_err0_o, sim.dut->new_s0_addr_msb_err0_o);
    sim.compare("s0_thread_id0", sim.dut->orig_s0_thread_id0_o, sim.dut->new_s0_thread_id0_o);
    sim.compare("s1_addr_load0", sim.dut->orig_s1_addr_load0_o, sim.dut->new_s1_addr_load0_o);
    sim.compare("l2_req_valid_coop0", sim.dut->orig_l2_req_valid_coop0_o, sim.dut->new_l2_req_valid_coop0_o);
    sim.compare("l2_req_valid_no_coop0", sim.dut->orig_l2_req_valid_no_coop0_o, sim.dut->new_l2_req_valid_no_coop0_o);
    sim.compare("l2_req_id0", sim.dut->orig_l2_req_id0_o, sim.dut->new_l2_req_id0_o);
    sim.compare("l2_req_opcode0", sim.dut->orig_l2_req_opcode0_o, sim.dut->new_l2_req_opcode0_o);
    sim.compare("l2_req_addr0", sim.dut->orig_l2_req_addr0_o, sim.dut->new_l2_req_addr0_o);
    sim.compare("l2_req_data0", sim.dut->orig_l2_req_data0_o, sim.dut->new_l2_req_data0_o);
    sim.compare("l2_req_qwen0", sim.dut->orig_l2_req_qwen0_o, sim.dut->new_l2_req_qwen0_o);
    sim.compare("load_way0", sim.dut->orig_load_way0_o, sim.dut->new_load_way0_o);
    sim.compare("load_addr0", sim.dut->orig_load_addr0_o, sim.dut->new_load_addr0_o);
    for (int i = 0; i < 4; ++i) {
      sim.compare("load_data0[" + std::to_string(i) + "]",
                  sim.dut->orig_load_data0_o[i], sim.dut->new_load_data0_o[i]);
      sim.compare("sm_filter0[" + std::to_string(i) + "]",
                  sim.dut->orig_sm_filter0_o[i], sim.dut->new_sm_filter0_o[i]);
    }
    for (int i = 0; i < 8; ++i) {
      sim.compare("sm_data0[" + std::to_string(i) + "]",
                  sim.dut->orig_sm_data0_o[i], sim.dut->new_sm_data0_o[i]);
    }
    sim.compare("load_valid0", sim.dut->orig_load_valid0_o, sim.dut->new_load_valid0_o);
    sim.compare("load_valid_pre0", sim.dut->orig_load_valid_pre0_o, sim.dut->new_load_valid_pre0_o);
    sim.compare("load_capture0", sim.dut->orig_load_capture0_o, sim.dut->new_load_capture0_o);
    sim.compare("tenb_ready0", sim.dut->orig_tenb_ready0_o, sim.dut->new_tenb_ready0_o);
    sim.compare("tenb_flush0", sim.dut->orig_tenb_flush0_o, sim.dut->new_tenb_flush0_o);
    sim.compare("vm_status_tl0", sim.dut->orig_vm_status_tl0_o, sim.dut->new_vm_status_tl0_o);
    sim.compare("err_flags0", sim.dut->orig_err_flags0_o, sim.dut->new_err_flags0_o);
    sim.compare("tl_start0", sim.dut->orig_tl_start0_o, sim.dut->new_tl_start0_o);
    sim.compare("tl_op0", sim.dut->orig_tl_op0_o, sim.dut->new_tl_op0_o);
    sim.compare("bus_err0", sim.dut->orig_bus_err0_o, sim.dut->new_bus_err0_o);
    sim.compare("csr_debug0", sim.dut->orig_csr_debug0_o, sim.dut->new_csr_debug0_o);
    sim.compare("sm_match0", sim.dut->orig_sm_match0_o, sim.dut->new_sm_match0_o);
    sim.compare("ctrl_state0_dbg", sim.dut->orig_ctrl_state0_dbg_o, sim.dut->new_ctrl_state0_dbg_o);
    sim.compare("enabled0_dbg", sim.dut->orig_enabled0_dbg_o, sim.dut->new_enabled0_dbg_o);
    sim.compare("count0_dbg", sim.dut->orig_count0_dbg_o, sim.dut->new_count0_dbg_o);
    sim.compare("shared_count0_dbg", sim.dut->orig_shared_count0_dbg_o, sim.dut->new_shared_count0_dbg_o);
    sim.compare("l2_busy0_dbg", sim.dut->orig_l2_busy0_dbg_o, sim.dut->new_l2_busy0_dbg_o);
    sim.compare("shared_load_idx0_dbg", sim.dut->orig_shared_load_idx0_dbg_o,
                sim.dut->new_shared_load_idx0_dbg_o);
    sim.compare("trans_split0_dbg", sim.dut->orig_trans_split0_dbg_o, sim.dut->new_trans_split0_dbg_o);
    sim.compare("shared_l2_counter0_dbg", sim.dut->orig_shared_l2_counter0_dbg_o,
                sim.dut->new_shared_l2_counter0_dbg_o);
    sim.compare("shared_l2_store0_dbg", sim.dut->orig_shared_l2_store0_dbg_o,
                sim.dut->new_shared_l2_store0_dbg_o);
    sim.compare("transform0_dbg", sim.dut->orig_transform0_dbg_o, sim.dut->new_transform0_dbg_o);
    sim.compare("trans_flags0_dbg", sim.dut->orig_trans_flags0_dbg_o, sim.dut->new_trans_flags0_dbg_o);
    sim.compare("tmp_data00_dbg", sim.dut->orig_tmp_data00_dbg_o, sim.dut->new_tmp_data00_dbg_o);
    sim.compare("l2_resp_for_me0_dbg", sim.dut->orig_l2_resp_for_me0_dbg_o,
                sim.dut->new_l2_resp_for_me0_dbg_o);
  } else {
    sim.compare("busy1", sim.dut->orig_busy1_o, sim.dut->new_busy1_o);
    sim.compare("id_complete1", sim.dut->orig_id_complete1_o, sim.dut->new_id_complete1_o);
    sim.compare("s0_addr_load1", sim.dut->orig_s0_addr_load1_o, sim.dut->new_s0_addr_load1_o);
    sim.compare("s0_addr1", sim.dut->orig_s0_addr1_o, sim.dut->new_s0_addr1_o);
    sim.compare("s0_addr_msb_err1", sim.dut->orig_s0_addr_msb_err1_o, sim.dut->new_s0_addr_msb_err1_o);
    sim.compare("s0_thread_id1", sim.dut->orig_s0_thread_id1_o, sim.dut->new_s0_thread_id1_o);
    sim.compare("s1_addr_load1", sim.dut->orig_s1_addr_load1_o, sim.dut->new_s1_addr_load1_o);
    sim.compare("l2_req_valid_coop1", sim.dut->orig_l2_req_valid_coop1_o, sim.dut->new_l2_req_valid_coop1_o);
    sim.compare("l2_req_valid_no_coop1", sim.dut->orig_l2_req_valid_no_coop1_o, sim.dut->new_l2_req_valid_no_coop1_o);
    sim.compare("l2_req_id1", sim.dut->orig_l2_req_id1_o, sim.dut->new_l2_req_id1_o);
    sim.compare("l2_req_opcode1", sim.dut->orig_l2_req_opcode1_o, sim.dut->new_l2_req_opcode1_o);
    sim.compare("l2_req_addr1", sim.dut->orig_l2_req_addr1_o, sim.dut->new_l2_req_addr1_o);
    sim.compare("l2_req_data1", sim.dut->orig_l2_req_data1_o, sim.dut->new_l2_req_data1_o);
    sim.compare("l2_req_qwen1", sim.dut->orig_l2_req_qwen1_o, sim.dut->new_l2_req_qwen1_o);
    sim.compare("load_way1", sim.dut->orig_load_way1_o, sim.dut->new_load_way1_o);
    sim.compare("load_addr1", sim.dut->orig_load_addr1_o, sim.dut->new_load_addr1_o);
    for (int i = 0; i < 4; ++i) {
      sim.compare("load_data1[" + std::to_string(i) + "]",
                  sim.dut->orig_load_data1_o[i], sim.dut->new_load_data1_o[i]);
      sim.compare("sm_filter1[" + std::to_string(i) + "]",
                  sim.dut->orig_sm_filter1_o[i], sim.dut->new_sm_filter1_o[i]);
    }
    for (int i = 0; i < 8; ++i) {
      sim.compare("sm_data1[" + std::to_string(i) + "]",
                  sim.dut->orig_sm_data1_o[i], sim.dut->new_sm_data1_o[i]);
    }
    sim.compare("load_valid1", sim.dut->orig_load_valid1_o, sim.dut->new_load_valid1_o);
    sim.compare("load_valid_pre1", sim.dut->orig_load_valid_pre1_o, sim.dut->new_load_valid_pre1_o);
    sim.compare("load_capture1", sim.dut->orig_load_capture1_o, sim.dut->new_load_capture1_o);
    sim.compare("tenb_ready1", sim.dut->orig_tenb_ready1_o, sim.dut->new_tenb_ready1_o);
    sim.compare("tenb_flush1", sim.dut->orig_tenb_flush1_o, sim.dut->new_tenb_flush1_o);
    sim.compare("vm_status_tl1", sim.dut->orig_vm_status_tl1_o, sim.dut->new_vm_status_tl1_o);
    sim.compare("err_flags1", sim.dut->orig_err_flags1_o, sim.dut->new_err_flags1_o);
    sim.compare("tl_start1", sim.dut->orig_tl_start1_o, sim.dut->new_tl_start1_o);
    sim.compare("tl_op1", sim.dut->orig_tl_op1_o, sim.dut->new_tl_op1_o);
    sim.compare("bus_err1", sim.dut->orig_bus_err1_o, sim.dut->new_bus_err1_o);
    sim.compare("csr_debug1", sim.dut->orig_csr_debug1_o, sim.dut->new_csr_debug1_o);
    sim.compare("sm_match1", sim.dut->orig_sm_match1_o, sim.dut->new_sm_match1_o);
    sim.compare("ctrl_state1_dbg", sim.dut->orig_ctrl_state1_dbg_o, sim.dut->new_ctrl_state1_dbg_o);
    sim.compare("enabled1_dbg", sim.dut->orig_enabled1_dbg_o, sim.dut->new_enabled1_dbg_o);
    sim.compare("count1_dbg", sim.dut->orig_count1_dbg_o, sim.dut->new_count1_dbg_o);
    sim.compare("load_way_sel1_dbg", sim.dut->orig_load_way_sel1_dbg_o, sim.dut->new_load_way_sel1_dbg_o);
    sim.compare("tenb_avail1_dbg", sim.dut->orig_tenb_avail1_dbg_o, sim.dut->new_tenb_avail1_dbg_o);
    sim.compare("tenb_rcv_ok1_dbg", sim.dut->orig_tenb_rcv_ok1_dbg_o, sim.dut->new_tenb_rcv_ok1_dbg_o);
    sim.compare("tenb_credits_ok1_dbg", sim.dut->orig_tenb_credits_ok1_dbg_o,
                sim.dut->new_tenb_credits_ok1_dbg_o);
    sim.compare("tenb_credits1_dbg", sim.dut->orig_tenb_credits1_dbg_o, sim.dut->new_tenb_credits1_dbg_o);
    sim.compare("cond31_dbg", sim.dut->orig_cond31_dbg_o, sim.dut->new_cond31_dbg_o);
    sim.compare("tenb_count1_dbg", sim.dut->orig_tenb_count1_dbg_o, sim.dut->new_tenb_count1_dbg_o);
    sim.compare("s2_req_valid1_dbg", sim.dut->orig_s2_req_valid1_dbg_o, sim.dut->new_s2_req_valid1_dbg_o);
    sim.compare("s2_req_ready1_dbg", sim.dut->orig_s2_req_ready1_dbg_o, sim.dut->new_s2_req_ready1_dbg_o);
    sim.compare("sel_t1_dbg", sim.dut->orig_sel_t1_dbg_o, sim.dut->new_sel_t1_dbg_o);
    sim.compare("sel_set1_dbg", sim.dut->orig_sel_set1_dbg_o, sim.dut->new_sel_set1_dbg_o);
    sim.compare("sel_idx1_dbg", sim.dut->orig_sel_idx1_dbg_o, sim.dut->new_sel_idx1_dbg_o);
    sim.compare("tfma_started1_dbg", sim.dut->orig_tfma_started1_dbg_o, sim.dut->new_tfma_started1_dbg_o);
    sim.compare("tfma_active_start1_dbg", sim.dut->orig_tfma_active_start1_dbg_o,
                sim.dut->new_tfma_active_start1_dbg_o);
  }
}

void step_half(CosimCtrl<DUT>& sim, int clk_level) {
  sim.dut->clk_i = clk_level;
  sim.dut->eval();
  compare_one(sim, 0);
  compare_one(sim, 1);
  sim.sim_time++;
  if (sim.sim_time >= sim.max_time) {
    printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
    std::exit(2);
  }
}

void step_half_no_compare(CosimCtrl<DUT>& sim, int clk_level) {
  sim.dut->clk_i = clk_level;
  sim.dut->eval();
  sim.sim_time++;
  if (sim.sim_time >= sim.max_time) {
    printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
    std::exit(2);
  }
}

void mutate_l2_resp_midcycle(DUT* dut, uint32_t& seed) {
  for (int i = 0; i < 8; ++i) {
    dut->l2_resp_data_i[i] ^= xorshift32(seed) ^ (0x9E3779B9u * static_cast<uint32_t>(i + 1));
  }
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  step_half(sim, 0);
  step_half(sim, 1);
}

void tick_and_compare_with_midcycle_mutation(CosimCtrl<DUT>& sim, uint32_t& seed) {
  step_half(sim, 0);
  mutate_l2_resp_midcycle(sim.dut.get(), seed);
  sim.dut->eval();
  compare_one(sim, 0);
  compare_one(sim, 1);
  step_half(sim, 1);
}

void pulse_reset_and_compare_at_posedge(CosimCtrl<DUT>& sim) {
  // The original tensor-load resets synchronously while the translation uses
  // the project-standard async reset. Skip the low half-cycle transition and
  // resume comparisons at the reset clock edge where the architected state
  // should align.
  sim.dut->rst_ni = 0;
  step_half_no_compare(sim, 0);
  step_half(sim, 1);
  sim.dut->rst_ni = 1;
}

void run_directed_tfma_reset_corner(CosimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.dut->tfma_enabled_i = 1;
  tick_and_compare(sim);
  sim.check(sim.dut->orig_tfma_started1_dbg_o == 1, "directed TFMA pulse drives orig tfma_started");
  sim.check(sim.dut->new_tfma_started1_dbg_o == 1, "directed TFMA pulse drives new tfma_started");
  sim.check(sim.dut->orig_tfma_active_start1_dbg_o == 0,
            "directed TFMA pulse leaves orig tfma_active_start low");
  sim.check(sim.dut->new_tfma_active_start1_dbg_o == 0,
            "directed TFMA pulse leaves new tfma_active_start low");

  clear_inputs(sim.dut.get());
  sim.dut->tfma_enabled_i = 1;
  pulse_reset_and_compare_at_posedge(sim);
  sim.check(sim.dut->orig_tfma_started1_dbg_o == 1,
            "reset pulse preserves orig non-reset tfma_started");
  sim.check(sim.dut->new_tfma_started1_dbg_o == 1,
            "reset pulse preserves new non-reset tfma_started");

  clear_inputs(sim.dut.get());
  tick_and_compare(sim);
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
  compare_one(sim, 0);
  compare_one(sim, 1);

  run_directed_tfma_reset_corner(sim);

  uint32_t seed = 0x12345678u;
  for (int cycle = 0; cycle < kRandomCycles; ++cycle) {
    const uint32_t r0 = xorshift32(seed);
    const uint32_t r1 = xorshift32(seed);
    const uint32_t r2 = xorshift32(seed);
    clear_inputs(sim.dut.get());
    sim.dut->shire_min_id_i = r0 & 0x7;
    sim.dut->chicken_bit_dcache_i = (r0 >> 3) & 1;
    sim.dut->tl_use_tmask_i = (r0 >> 4) & 1;
    sim.dut->tl_use_coop_i = (r0 >> 5) & 1;
    sim.dut->tl_transform_i = (r0 >> 6) & 0x7;
    sim.dut->tl_scp_dest_i = (r0 >> 9) & 0x3f;
    sim.dut->tl_sel_i = (r0 >> 15) & 1;
    sim.dut->tl_addr_i = (r1 & 0x1fffffff);
    sim.dut->tl_offset_i = (r1 >> 29) & 0x3;
    sim.dut->tl_num_lines_i = r2 & 0xf;
    sim.dut->tl_stride_i = (static_cast<uint64_t>(r1) << 12) ^ r2;
    sim.dut->tl_start_i = ((cycle % 17) == 0);
    sim.dut->tl_restart_b_i = ((cycle % 53) == 7);
    sim.dut->tl_tmask_ready_i = (r1 >> 3) & 1;
    sim.dut->tl_tmask_bits_i = r2 & 0xff;
    sim.dut->tl_neigh_mask_i = (r1 >> 8) & 0xf;
    sim.dut->tl_minion_mask_i = (r1 >> 12) & 0xff;
    sim.dut->tl_coop_id_i = (r1 >> 20) & 0x1f;
    sim.dut->thread_id_i = (r1 >> 25) & 1;
    sim.dut->s0_available_i = (r0 >> 16) & 1;
    sim.dut->s1_available_i = (r0 >> 17) & 1;
    sim.dut->s1_tlb_wait_i = (r0 >> 18) & 1;
    sim.dut->s1_tlb_fail_i = (r0 >> 19) & 1;
    sim.dut->s2_addr_i = (static_cast<uint64_t>(r0) << 10) ^ (static_cast<uint64_t>(r2) << 1);
    sim.dut->l2_req_ready_coop_i = (r0 >> 20) & 1;
    sim.dut->l2_req_ready_no_coop_i = (r0 >> 21) & 1;
    sim.dut->l2_resp_valid_i = (r0 >> 22) & 1;
    sim.dut->l2_resp_id_i = r1 & 0xff;
    sim.dut->l2_resp_opcode_i = r1 & 0x3;
    for (int i = 0; i < 8; ++i) sim.dut->l2_resp_data_i[i] = xorshift32(seed);
    sim.dut->load_ready_i = (r0 >> 23) & 1;
    sim.dut->tfma_enabled_i = (r0 >> 24) & 1;
    sim.dut->tenb_tstore_busy_i = (r0 >> 25) & 1;
    sim.dut->tenb_credit_i = (r0 >> 26) & 1;
    sim.dut->tenb_credit_entry_i = (r0 >> 27) & 0x3;
    sim.dut->tenb_written_i = (r0 >> 29) & 1;
    tick_and_compare_with_midcycle_mutation(sim, seed);
  }

  return sim.finish();
}

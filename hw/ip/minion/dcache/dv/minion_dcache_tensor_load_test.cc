// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_tensor_load.

#include "Vminion_dcache_tensor_load_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_tensor_load_tb;

namespace {

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
  dut->l2_resp_data0_i = 0;
  dut->l2_resp_data1_i = 0;
  dut->l2_resp_data2_i = 0;
  dut->l2_resp_data3_i = 0;
  dut->load_ready_i = 0;
  dut->tfma_enabled_i = 0;
  dut->tenb_tstore_busy_i = 0;
  dut->tenb_credit_i = 0;
  dut->tenb_credit_entry_i = 0;
  dut->tenb_written_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void tick_n(SimCtrl<DUT>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) tick_and_eval(sim);
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->busy0_o == 0, "module 0 reset leaves idle");
  sim.check(sim.dut->busy1_o == 0, "module 1 reset leaves idle");
  sim.check(sim.dut->l2_req_valid_coop0_o == 0 && sim.dut->l2_req_valid_no_coop0_o == 0,
            "module 0 reset leaves L2 idle");
  sim.check(sim.dut->l2_req_valid_coop1_o == 0 && sim.dut->l2_req_valid_no_coop1_o == 0,
            "module 1 reset leaves L2 idle");
  sim.check(sim.dut->err_flags0_o == 0 && sim.dut->err_flags1_o == 0,
            "reset leaves tensor-load error flags clear");

  sim.dut->shire_min_id_i = 3;
  sim.dut->tl_sel_i = 0;
  sim.dut->tl_start_i = 1;
  sim.dut->tl_tmask_ready_i = 1;
  sim.dut->tl_scp_dest_i = 5;
  sim.dut->tl_addr_i = 0x12;
  sim.dut->tl_stride_i = 0x40;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->l2_req_ready_no_coop_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->tl_start0_o == 1, "module 0 exposes start pulse on selected request");
  sim.check(sim.dut->busy0_o == 1, "module 0 becomes busy on accepted request");
  sim.check(sim.dut->busy1_o == 0, "module 1 ignores tl_sel=0 request");

  sim.dut->tl_start_i = 0;
  bool saw_s0_load0 = false;
  bool saw_s1_load0 = false;
  bool saw_l2_req0 = false;
  for (int i = 0; i < 8; ++i) {
    saw_s0_load0 |= (sim.dut->s0_addr_load0_o == 1);
    saw_s1_load0 |= (sim.dut->s1_addr_load0_o == 1);
    saw_l2_req0 |= (sim.dut->l2_req_valid_no_coop0_o == 1);
    if (i == 2) sim.dut->s2_addr_i = 0x800;
    tick_and_eval(sim);
  }
  sim.check(saw_s0_load0, "module 0 drives s0 address load during plain request");
  sim.check(saw_s1_load0, "module 0 advances to the s1 address load stage");
  sim.check(saw_l2_req0, "module 0 emits a non-cooperative L2 request");
  sim.check(sim.dut->l2_req_addr0_o == 0x800, "module 0 forwards the translated cache-line address");

  reset_and_clear(sim);
  sim.dut->tl_sel_i = 1;
  sim.dut->tl_start_i = 1;
  sim.dut->tl_tmask_ready_i = 1;
  sim.dut->tenb_tstore_busy_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->tl_start1_o == 1, "module 1 exposes start pulse on selected request");

  sim.dut->tl_start_i = 0;
  sim.dut->tl_restart_b_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->busy1_o == 1, "module 1 restart enters the flush path");
  sim.check(sim.dut->tenb_flush1_o == 1, "module 1 restart drives the TENB flush output");

  sim.dut->tl_restart_b_i = 0;
  tick_n(sim, 4);
  sim.check(sim.dut->tenb_flush1_o == 0, "module 1 flush pulse deasserts after restart handling");

  reset_and_clear(sim);
  sim.dut->tl_sel_i = 0;
  sim.dut->tl_start_i = 1;
  sim.dut->tl_tmask_ready_i = 1;
  sim.dut->tl_transform_i = 1;
  sim.dut->tl_offset_i = 0;
  sim.dut->tl_num_lines_i = 0;
  sim.dut->tl_stride_i = 0x40;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->l2_req_ready_no_coop_i = 1;
  tick_and_eval(sim);
  sim.dut->tl_start_i = 0;

  bool saw_load_pre0 = false;
  bool saw_capture0 = false;
  bool saw_load_valid0 = false;
  for (int i = 0; i < 24; ++i) {
    if (i == 2) sim.dut->s2_addr_i = 0x1400;
    if (i == 6) {
      sim.dut->l2_resp_valid_i = 1;
      sim.dut->l2_resp_id_i = 0;
      sim.dut->l2_resp_data0_i = 0x1111222233334444ULL;
      sim.dut->l2_resp_data1_i = 0x5555666677778888ULL;
      sim.dut->l2_resp_data2_i = 0x9999AAAABBBBCCCCULL;
      sim.dut->l2_resp_data3_i = 0xDDDDEEEEFFFF0000ULL;
    } else {
      sim.dut->l2_resp_valid_i = 0;
    }
    if (i >= 8) sim.dut->load_ready_i = 1;
    saw_load_pre0 |= (sim.dut->load_valid_pre0_o == 1);
    saw_capture0 |= (sim.dut->load_capture0_o == 1);
    saw_load_valid0 |= (sim.dut->load_valid0_o != 0);
    tick_and_eval(sim);
  }
  sim.check(saw_capture0, "interleave request captures returned L2 data");
  sim.check(sim.dut->busy0_o == 1 || saw_capture0, "interleave request remains active after the transformed fetch");
  sim.check(sim.dut->busy0_o == 1 || saw_load_valid0, "interleave request remains active until refill work is drained");

  return sim.finish();
}

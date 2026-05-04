// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_reduce.

#include "Vminion_dcache_reduce_tb.h"
#include "sim_ctrl.h"

using DUT = Vminion_dcache_reduce_tb;

namespace {

constexpr uint64_t kReduceDoNothingCtrl =
    (uint64_t{1} << 3) |  // partner
    (uint64_t{1} << 16) | // num_regs
    uint64_t{2};          // action = do nothing

uint64_t make_tensorstore_ctrl(uint64_t addr, uint8_t rows, uint8_t cols) {
  return ((static_cast<uint64_t>(cols & 0x3u)) << 55)
      | ((static_cast<uint64_t>(rows & 0x0fu)) << 51)
      | ((addr >> 4) << 4);
}

void clear_inputs(DUT* dut) {
  dut->global_min_id_i = 0;
  dut->chicken_bit_dcache_i = 0;
  dut->reduce_start_i = 0;
  dut->tensorstore_start_i = 0;
  dut->tensorstore_stride_i = 0;
  dut->ctrl_bits_i = 0;
  dut->thread_id_i = 0;
  dut->re_l2_req_ready_i = 0;
  dut->ts_l2_req_ready_i = 0;
  dut->s0_available_i = 0;
  dut->s1_available_i = 0;
  dut->s1_tlb_wait_i = 0;
  dut->s1_tlb_fail_i = 0;
  dut->s2_addr_i = 0;
  dut->ba_full_i = 0;
  dut->s2_valid_i = 0;
  dut->s1_valid_i = 0;
  dut->ba_read_used_i = 0;
  dut->scp_ctrl_on_i = 0;
  dut->vpu_reduce_wait_i = 0;
  dut->mh_going_to_uc_store_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void tick_n(SimCtrl<DUT>& sim, int cycles) {
  for (int i = 0; i < cycles; ++i) {
    tick_and_eval(sim);
  }
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
  sim.check(sim.dut->reduce_ready_o == 1, "reset leaves reduce idle");
  sim.check(sim.dut->tensorstore_busy_o == 0, "reset leaves tensorstore idle");
  sim.check(sim.dut->re_l2_req_valid_o == 0, "reset leaves reduce L2 port idle");
  sim.check(sim.dut->ts_l2_req_valid_o == 0, "reset leaves tensorstore L2 port idle");

  sim.dut->reduce_start_i = 1;
  sim.dut->ctrl_bits_i = kReduceDoNothingCtrl;
  sim.dut->eval();
  sim.check(sim.dut->reduce_ready_o == 1, "reduce no-op request is accepted while idle");
  tick_and_eval(sim);

  sim.dut->reduce_start_i = 0;
  sim.check(sim.dut->reduce_ready_o == 0, "accepted reduce request leaves idle state");
  for (int i = 0; i < 8; ++i) {
    tick_and_eval(sim);
  }
  sim.check(sim.dut->err_mem_fault_t0_o == 0 && sim.dut->err_mem_fault_t1_o == 0,
            "do-nothing reduce does not raise memory-fault flags");

  reset_and_clear(sim);
  sim.dut->tensorstore_start_i = 1;
  sim.dut->ctrl_bits_i = make_tensorstore_ctrl(0x4000, 0, 0);
  sim.dut->tensorstore_stride_i = 0x10;
  sim.dut->thread_id_i = 1;
  sim.dut->s0_available_i = 1;
  sim.dut->s1_available_i = 1;
  sim.dut->s2_addr_i = 0x8000;
  sim.dut->eval();
  sim.check(sim.dut->ts_start_o == 1, "tensorstore start pulse is exposed for counters");
  tick_and_eval(sim);

  sim.dut->tensorstore_start_i = 0;
  bool saw_ba_alloc = false;
  bool saw_vpu_send_reg = false;
  bool saw_send_pipe = false;
  for (int i = 0; i < 12; ++i) {
    tick_and_eval(sim);
    saw_ba_alloc |= (sim.dut->ba_alloc_o == 1);
    saw_vpu_send_reg |= (sim.dut->vpu_send_reg_o == 1);
    saw_send_pipe |= (sim.dut->s1_inst_send_val_o == 1);
  }
  sim.check(sim.dut->tensorstore_busy_o == 1, "tensorstore request enters busy state");
  sim.check(saw_ba_alloc, "translated tensorstore row allocates a buffer entry");
  sim.check(saw_vpu_send_reg, "tensorstore row requests VPU register data");
  sim.check(saw_send_pipe, "tensorstore drives the sender pipe");
  sim.check(sim.dut->ts_op_o == 0, "tensorstore does not count an L2 op before request acceptance");

  return sim.finish();
}

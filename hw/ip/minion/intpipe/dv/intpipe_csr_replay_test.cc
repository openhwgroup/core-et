// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_csr_replay_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vintpipe_csr_replay_tb;

namespace {

void idle_inputs(DUT* d) {
  d->wb_wdata_i = 0;
  d->wb_thread_id_i = 0;
  d->io_interrupt_i = 0;
  d->io_redirect_i = 0;
  d->reg_co_evict_va_en_i = 0;
  d->reg_co_flush_va_en_i = 0;
  d->reg_co_lock_va_en_qual_i = 0;
  d->reg_co_unlock_va_en_qual_i = 0;
  d->reg_co_lock_sw_en_i = 0;
  d->reg_co_unlock_sw_en_i = 0;
  d->reg_co_prefetch_va_en_i = 0;
  d->reg_co_evict_sw_en_i = 0;
  d->reg_co_flush_sw_en_i = 0;
  d->reg_tensor_load_l2scp_en_i = 0;
  d->reg_flb_en_i = 0;
  d->reg_tensor_load_en_i = 0;
  d->reg_tensor_fma_en_i = 0;
  d->reg_tensor_quant_en_i = 0;
  d->reg_tensor_reduce_en_i = 0;
  d->reg_tensor_store_en_i = 0;
  d->reg_tensor_conv_ctrl_en_i = 0;
  d->reg_tensor_conv_size_en_i = 0;
  d->reg_fcc_en_i = 0;
  d->reg_tensor_wait_en_i = 0;
  d->reg_tex_send_en_i = 0;
  d->reg_tensor_mask_read_invalid_i = 0;
  d->pmu_read_en_wb_i = 0;
  d->pmu_read_mem_ready_i = 0x3;

  d->dcache_cache_op_ready_i = 1;
  d->dcache_cache_op_busy_changed_i = 0;
  d->dcache_cache_op_complete_l1_i = 0xf;
  d->dcache_cache_op_complete_l2_i = 0xf;
  d->dcache_tl_ready_0_i = 1;
  d->dcache_tl_ready_1_i = 1;
  d->dcache_ts_ready_i = 1;
  d->dcache_tl_complete_0_i = 0x3;
  d->dcache_reduce_ready_i = 1;
  d->dcache_tex_send_ready_i = 1;
  d->fl_barrier_ready_i = 1;
  d->tensor_fma_replay_condition_i = 0;
  d->tensor_quant_replay_condition_i = 0;
  d->vpu_treduce_enabled_i = 0;
  d->tenb_tl_pend_tfma_en_i = 0;
  d->tenb_restart_process_i = 0;
  d->tload_is_for_tenb_i = 0;
  d->conv_can_accept_new_write_i = 0x3;
  d->fcc_ready_i = 0xf;
  d->wb_fcc_addr_i = 0;
  d->msg_port_pget_block_i = 0;
  d->msg_port_stall_i = 0;
  d->reg_tensor_mask_ready_i = 0x3;
}

void tick_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void check_replay(SimCtrl<DUT>& sim, const char* name, bool exp_replay,
                  uint8_t exp_stall) {
  sim.dut->eval();
  char msg[192];
  std::snprintf(msg, sizeof(msg), "%s io_replay got=%u exp=%u",
                name, sim.dut->io_replay_o, exp_replay);
  sim.check(sim.dut->io_replay_o == exp_replay, msg);
  std::snprintf(msg, sizeof(msg), "%s io_replay_stall got=0x%x exp=0x%x",
                name, sim.dut->io_replay_stall_o, exp_stall);
  sim.check(sim.dut->io_replay_stall_o == exp_stall, msg);
}

template <typename Fn>
void directed_replay_case(SimCtrl<DUT>& sim, const char* name, bool exp_replay, Fn set_fn) {
  idle_inputs(sim.dut.get());
  tick_eval(sim);
  set_fn(sim.dut.get());
  check_replay(sim, name, exp_replay, 0x0);
  tick_eval(sim);
  check_replay(sim, name, exp_replay, exp_replay ? (1u << sim.dut->wb_thread_id_i) : 0x0);
  idle_inputs(sim.dut.get());
  sim.dut->io_redirect_i = 0x3;
  tick_eval(sim);
  check_replay(sim, name, false, 0x0);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  idle_inputs(sim.dut.get());
  sim.reset(2);
  idle_inputs(sim.dut.get());
  sim.dut->eval();

  const bool vpu_en = sim.dut->vpu_en_o;
  check_replay(sim, "reset_idle", false, 0x0);

  directed_replay_case(sim, "cache_op_not_ready", true, [](DUT* d) {
    d->reg_co_evict_va_en_i = 1;
    d->dcache_cache_op_ready_i = 0;
  });

  directed_replay_case(sim, "flb_not_ready", true, [](DUT* d) {
    d->reg_flb_en_i = 1;
    d->fl_barrier_ready_i = 0;
  });

  directed_replay_case(sim, "tensor_load_l2scp_cache_op", vpu_en, [](DUT* d) {
    d->reg_tensor_load_l2scp_en_i = 1;
    d->dcache_cache_op_ready_i = 0;
  });

  directed_replay_case(sim, "tensor_load_tl0", vpu_en, [](DUT* d) {
    d->reg_tensor_load_en_i = 1;
    d->dcache_tl_ready_0_i = 0;
    d->tload_is_for_tenb_i = 0;
  });

  directed_replay_case(sim, "tensor_load_tl1_tenb_pending", vpu_en, [](DUT* d) {
    d->reg_tensor_load_en_i = 1;
    d->dcache_tl_ready_1_i = 1;
    d->tenb_tl_pend_tfma_en_i = 1;
    d->tload_is_for_tenb_i = 1;
  });

  directed_replay_case(sim, "tensor_fma_condition", true, [](DUT* d) {
    d->reg_tensor_fma_en_i = 1;
    d->tensor_fma_replay_condition_i = 1;
  });

  directed_replay_case(sim, "tensor_quant_condition", true, [](DUT* d) {
    d->reg_tensor_quant_en_i = 1;
    d->tensor_quant_replay_condition_i = 1;
  });

  directed_replay_case(sim, "tensor_reduce_not_ready", vpu_en, [](DUT* d) {
    d->reg_tensor_reduce_en_i = 1;
    d->dcache_reduce_ready_i = 0;
  });

  directed_replay_case(sim, "tensor_store_not_ready", vpu_en, [](DUT* d) {
    d->reg_tensor_store_en_i = 1;
    d->dcache_ts_ready_i = 0;
  });

  directed_replay_case(sim, "conv_busy_thread1", true, [](DUT* d) {
    d->wb_thread_id_i = 1;
    d->reg_tensor_conv_ctrl_en_i = 0x2;
    d->conv_can_accept_new_write_i = 0x1;
  });

  directed_replay_case(sim, "fcc0_no_credit", true, [](DUT* d) {
    d->reg_fcc_en_i = 0x1;
    d->fcc_ready_i = 0x0;
    d->wb_fcc_addr_i = 0;
    d->wb_wdata_i = 0;
  });

  directed_replay_case(sim, "fcc1_no_credit", true, [](DUT* d) {
    d->reg_fcc_en_i = 0x1;
    d->fcc_ready_i = 0x0;
    d->wb_fcc_addr_i = 0;
    d->wb_wdata_i = 1;
  });

  directed_replay_case(sim, "tensor_mask_invalid", true, [](DUT* d) {
    d->reg_tensor_mask_read_invalid_i = 1;
    d->reg_tensor_mask_ready_i = 0;
  });

  directed_replay_case(sim, "pmu_read_not_ready", true, [](DUT* d) {
    d->pmu_read_en_wb_i = 1;
    d->pmu_read_mem_ready_i = 0x0;
  });

  directed_replay_case(sim, "tex_send_not_ready", vpu_en, [](DUT* d) {
    d->reg_tex_send_en_i = 1;
    d->dcache_tex_send_ready_i = 0;
  });

  directed_replay_case(sim, "pget_block", true, [](DUT* d) {
    d->msg_port_pget_block_i = 1;
    d->msg_port_stall_i = 0x1;
  });

  directed_replay_case(sim, "tensor_wait_prefetch0", vpu_en, [](DUT* d) {
    d->reg_tensor_wait_en_i = 0x1;
    d->wb_wdata_i = 2;
    d->dcache_cache_op_complete_l2_i &= ~0x1;
  });

  directed_replay_case(sim, "tensor_wait_fma_condition", true, [](DUT* d) {
    d->reg_tensor_wait_en_i = 0x1;
    d->wb_wdata_i = 7;
    d->tensor_fma_replay_condition_i = 1;
  });

  // Interrupt and redirect clear already-latched stalls even when the blocking
  // condition remains asserted.
  idle_inputs(sim.dut.get());
  sim.dut->reg_tensor_fma_en_i = 1;
  sim.dut->tensor_fma_replay_condition_i = 1;
  tick_eval(sim);
  check_replay(sim, "interrupt_setup", true, 0x1);
  idle_inputs(sim.dut.get());
  sim.dut->tensor_fma_replay_condition_i = 1;
  tick_eval(sim);
  check_replay(sim, "interrupt_before_clear", false, 0x1);
  sim.dut->io_interrupt_i = 0x1;
  tick_eval(sim);
  check_replay(sim, "interrupt_clears_stall", false, 0x0);

  idle_inputs(sim.dut.get());
  sim.dut->reg_tensor_quant_en_i = 1;
  sim.dut->tensor_quant_replay_condition_i = 1;
  tick_eval(sim);
  check_replay(sim, "redirect_setup", true, 0x1);
  idle_inputs(sim.dut.get());
  sim.dut->tensor_quant_replay_condition_i = 1;
  tick_eval(sim);
  check_replay(sim, "redirect_before_clear", false, 0x1);
  sim.dut->io_redirect_i = 0x1;
  tick_eval(sim);
  check_replay(sim, "redirect_clears_stall", false, 0x0);

  return sim.finish();
}

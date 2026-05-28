// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module intpipe_csr_replay_tb
  import minion_pkg::*;
#(
  parameter bit VpuEn = 1'b1
) (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [63:0] wb_wdata_i,
  input  logic        wb_thread_id_i,
  input  logic [1:0]  io_interrupt_i,
  input  logic [1:0]  io_redirect_i,
  output logic        io_replay_o,
  output logic [1:0]  io_replay_stall_o,
  output logic        vpu_en_o,

  input  logic        reg_co_evict_va_en_i,
  input  logic        reg_co_flush_va_en_i,
  input  logic        reg_co_lock_va_en_qual_i,
  input  logic        reg_co_unlock_va_en_qual_i,
  input  logic        reg_co_lock_sw_en_i,
  input  logic        reg_co_unlock_sw_en_i,
  input  logic        reg_co_prefetch_va_en_i,
  input  logic        reg_co_evict_sw_en_i,
  input  logic        reg_co_flush_sw_en_i,
  input  logic        reg_tensor_load_l2scp_en_i,
  input  logic        reg_flb_en_i,
  input  logic        reg_tensor_load_en_i,
  input  logic        reg_tensor_fma_en_i,
  input  logic        reg_tensor_quant_en_i,
  input  logic        reg_tensor_reduce_en_i,
  input  logic        reg_tensor_store_en_i,
  input  logic [1:0]  reg_tensor_conv_ctrl_en_i,
  input  logic [1:0]  reg_tensor_conv_size_en_i,
  input  logic [1:0]  reg_fcc_en_i,
  input  logic [1:0]  reg_tensor_wait_en_i,
  input  logic        reg_tex_send_en_i,
  input  logic        reg_tensor_mask_read_invalid_i,
  input  logic        pmu_read_en_wb_i,
  input  logic [1:0]  pmu_read_mem_ready_i,

  input  logic        dcache_cache_op_ready_i,
  input  logic [1:0]  dcache_cache_op_busy_changed_i,
  input  logic [3:0]  dcache_cache_op_complete_l1_i,
  input  logic [3:0]  dcache_cache_op_complete_l2_i,
  input  logic        dcache_tl_ready_0_i,
  input  logic        dcache_tl_ready_1_i,
  input  logic        dcache_ts_ready_i,
  input  logic [1:0]  dcache_tl_complete_0_i,
  input  logic        dcache_reduce_ready_i,
  input  logic        dcache_tex_send_ready_i,
  input  logic        fl_barrier_ready_i,
  input  logic        tensor_fma_replay_condition_i,
  input  logic        tensor_quant_replay_condition_i,
  input  logic        vpu_treduce_enabled_i,
  input  logic        tenb_tl_pend_tfma_en_i,
  input  logic        tenb_restart_process_i,
  input  logic        tload_is_for_tenb_i,
  input  logic [1:0]  conv_can_accept_new_write_i,
  input  logic [3:0]  fcc_ready_i,
  input  logic [1:0]  wb_fcc_addr_i,
  input  logic        msg_port_pget_block_i,
  input  logic [7:0]  msg_port_stall_i,
  input  logic [1:0]  reg_tensor_mask_ready_i
);

  dcache_core_ctrl_t            dcache_ctrl_resp;
  logic [NrThreads-1:0][NrMsgPorts-1:0] msg_port_stall;

  always_comb begin
    dcache_ctrl_resp = '0;
    dcache_ctrl_resp.cache_op_ready = dcache_cache_op_ready_i;
    dcache_ctrl_resp.cache_op_busy_changed = dcache_cache_op_busy_changed_i;
    dcache_ctrl_resp.cache_op_complete_l1[0] = dcache_cache_op_complete_l1_i[1:0];
    dcache_ctrl_resp.cache_op_complete_l1[1] = dcache_cache_op_complete_l1_i[3:2];
    dcache_ctrl_resp.cache_op_complete_l2[0] = dcache_cache_op_complete_l2_i[1:0];
    dcache_ctrl_resp.cache_op_complete_l2[1] = dcache_cache_op_complete_l2_i[3:2];
    dcache_ctrl_resp.tl_ready_0 = dcache_tl_ready_0_i;
    dcache_ctrl_resp.tl_ready_1 = dcache_tl_ready_1_i;
    dcache_ctrl_resp.ts_ready = dcache_ts_ready_i;
    dcache_ctrl_resp.tl_complete_0 = dcache_tl_complete_0_i;
    dcache_ctrl_resp.reduce_ready = dcache_reduce_ready_i;
    dcache_ctrl_resp.tex_send_ready = dcache_tex_send_ready_i;

    msg_port_stall[0] = msg_port_stall_i[3:0];
    msg_port_stall[1] = msg_port_stall_i[7:4];
  end

  assign vpu_en_o = VpuEn;

  intpipe_csr_replay #(.VpuEn(VpuEn)) u_dut (
    .clk_i,
    .rst_ni,
    .wb_wdata                       (wb_wdata_i),
    .wb_thread_id                   (wb_thread_id_i),
    .io_interrupt                   (io_interrupt_i),
    .io_redirect                    (io_redirect_i),
    .io_replay                      (io_replay_o),
    .io_replay_stall                (io_replay_stall_o),
    .reg_co_evict_va_en             (reg_co_evict_va_en_i),
    .reg_co_flush_va_en             (reg_co_flush_va_en_i),
    .reg_co_lock_va_en_qual         (reg_co_lock_va_en_qual_i),
    .reg_co_unlock_va_en_qual       (reg_co_unlock_va_en_qual_i),
    .reg_co_lock_sw_en              (reg_co_lock_sw_en_i),
    .reg_co_unlock_sw_en            (reg_co_unlock_sw_en_i),
    .reg_co_prefetch_va_en          (reg_co_prefetch_va_en_i),
    .reg_co_evict_sw_en             (reg_co_evict_sw_en_i),
    .reg_co_flush_sw_en             (reg_co_flush_sw_en_i),
    .reg_tensor_load_l2scp_en       (reg_tensor_load_l2scp_en_i),
    .reg_flb_en                     (reg_flb_en_i),
    .reg_tensor_load_en             (reg_tensor_load_en_i),
    .reg_tensor_fma_en              (reg_tensor_fma_en_i),
    .reg_tensor_quant_en            (reg_tensor_quant_en_i),
    .reg_tensor_reduce_en           (reg_tensor_reduce_en_i),
    .reg_tensor_store_en            (reg_tensor_store_en_i),
    .reg_tensor_conv_ctrl_en        (reg_tensor_conv_ctrl_en_i),
    .reg_tensor_conv_size_en        (reg_tensor_conv_size_en_i),
    .reg_fcc_en                     (reg_fcc_en_i),
    .reg_tensor_wait_en             (reg_tensor_wait_en_i),
    .reg_tex_send_en                (reg_tex_send_en_i),
    .reg_tensor_mask_read_invalid   (reg_tensor_mask_read_invalid_i),
    .pmu_read_en_wb                 (pmu_read_en_wb_i),
    .pmu_read_mem_ready             (pmu_read_mem_ready_i),
    .dcache_ctrl_resp               (dcache_ctrl_resp),
    .fl_barrier_ready               (fl_barrier_ready_i),
    .tensor_fma_replay_condition    (tensor_fma_replay_condition_i),
    .tensor_quant_replay_condition  (tensor_quant_replay_condition_i),
    .vpu_treduce_enabled            (vpu_treduce_enabled_i),
    .tenb_tl_pend_tfma_en           (tenb_tl_pend_tfma_en_i),
    .tenb_restart_process           (tenb_restart_process_i),
    .tload_is_for_tenb              (tload_is_for_tenb_i),
    .conv_can_accept_new_write      (conv_can_accept_new_write_i),
    .fcc_ready                      (fcc_ready_i),
    .wb_fcc_addr                    (wb_fcc_addr_i),
    .msg_port_pget_block            (msg_port_pget_block_i),
    .msg_port_stall                 (msg_port_stall),
    .reg_tensor_mask_ready          (reg_tensor_mask_ready_i)
  );

endmodule

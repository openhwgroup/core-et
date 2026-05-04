// Co-simulation testbench: intpipe_csr_replay (new) vs original.

`include "soc.vh"

module cosim_replay_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus — control signals
  input  logic [63:0] stim_wb_wdata,
  input  logic        stim_wb_thread_id,
  input  logic [1:0]  stim_io_interrupt,
  input  logic [1:0]  stim_io_redirect,

  // Stimulus — CSR enable pulses
  input  logic        stim_reg_co_evict_va_en,
  input  logic        stim_reg_co_flush_va_en,
  input  logic        stim_reg_co_lock_va_en_qual,
  input  logic        stim_reg_co_unlock_va_en_qual,
  input  logic        stim_reg_co_lock_sw_en,
  input  logic        stim_reg_co_unlock_sw_en,
  input  logic        stim_reg_co_prefetch_va_en,
  input  logic        stim_reg_co_evict_sw_en,
  input  logic        stim_reg_co_flush_sw_en,
  input  logic        stim_reg_tensor_load_l2scp_en,
  input  logic        stim_reg_flb_en,
  input  logic        stim_reg_tensor_load_en,
  input  logic        stim_reg_tensor_fma_en,
  input  logic        stim_reg_tensor_quant_en,
  input  logic        stim_reg_tensor_reduce_en,
  input  logic        stim_reg_tensor_store_en,
  input  logic [1:0]  stim_reg_tensor_conv_ctrl_en,
  input  logic [1:0]  stim_reg_tensor_conv_size_en,
  input  logic [1:0]  stim_reg_fcc_en,
  input  logic [1:0]  stim_reg_tensor_wait_en,
  input  logic        stim_reg_tex_send_en,
  input  logic        stim_reg_tensor_mask_read_invalid,
  input  logic        stim_pmu_read_en_wb,
  input  logic [1:0]  stim_pmu_read_mem_ready,

  // Stimulus — condition check signals
  input  logic [22:0] stim_dcache_ctrl_flat,  // packed dcache_core_ctrl / dcache_core_ctrl_t
  input  logic        stim_fl_barrier_ready,
  input  logic        stim_tensor_fma_replay_condition,
  input  logic        stim_tensor_quant_replay_condition,
  input  logic        stim_vpu_treduce_enabled,
  input  logic        stim_tenb_tl_pend_tfma_en,
  input  logic        stim_tenb_restart_process,
  input  logic        stim_tload_is_for_tenb,
  input  logic [1:0]  stim_conv_can_accept_new_write,
  input  logic [3:0]  stim_fcc_ready,
  input  logic [1:0]  stim_wb_fcc_addr,
  input  logic        stim_msg_port_pget_block,
  input  logic [7:0]  stim_msg_port_stall_flat,  // [1:0][3:0] flattened
  input  logic [1:0]  stim_reg_tensor_mask_ready,

  // New outputs
  output logic        new_io_replay,
  output logic [1:0]  new_io_replay_stall,

  // Original outputs
  output logic        orig_io_replay,
  output logic [1:0]  orig_io_replay_stall
);

  import minion_pkg::*;

  // Cast flat bit vector to structs
  dcache_core_ctrl_t dcache_ctrl_new;
  dcache_core_ctrl   dcache_ctrl_orig;
  assign dcache_ctrl_new  = stim_dcache_ctrl_flat;
  assign dcache_ctrl_orig = stim_dcache_ctrl_flat;

  // Unpack msg_port_stall from flat to [1:0][3:0]
  logic [1:0][3:0] msg_port_stall_unpacked;
  assign msg_port_stall_unpacked[0] = stim_msg_port_stall_flat[3:0];
  assign msg_port_stall_unpacked[1] = stim_msg_port_stall_flat[7:4];

  // -- New module --
  intpipe_csr_replay u_new (
    .clk_i                          (clk_i),
    .rst_ni                         (rst_ni),
    .wb_wdata                       (stim_wb_wdata),
    .wb_thread_id                   (stim_wb_thread_id),
    .io_interrupt                   (stim_io_interrupt),
    .io_redirect                    (stim_io_redirect),
    .io_replay                      (new_io_replay),
    .io_replay_stall                (new_io_replay_stall),
    .reg_co_evict_va_en             (stim_reg_co_evict_va_en),
    .reg_co_flush_va_en             (stim_reg_co_flush_va_en),
    .reg_co_lock_va_en_qual         (stim_reg_co_lock_va_en_qual),
    .reg_co_unlock_va_en_qual       (stim_reg_co_unlock_va_en_qual),
    .reg_co_lock_sw_en              (stim_reg_co_lock_sw_en),
    .reg_co_unlock_sw_en            (stim_reg_co_unlock_sw_en),
    .reg_co_prefetch_va_en          (stim_reg_co_prefetch_va_en),
    .reg_co_evict_sw_en             (stim_reg_co_evict_sw_en),
    .reg_co_flush_sw_en             (stim_reg_co_flush_sw_en),
    .reg_tensor_load_l2scp_en       (stim_reg_tensor_load_l2scp_en),
    .reg_flb_en                     (stim_reg_flb_en),
    .reg_tensor_load_en             (stim_reg_tensor_load_en),
    .reg_tensor_fma_en              (stim_reg_tensor_fma_en),
    .reg_tensor_quant_en            (stim_reg_tensor_quant_en),
    .reg_tensor_reduce_en           (stim_reg_tensor_reduce_en),
    .reg_tensor_store_en            (stim_reg_tensor_store_en),
    .reg_tensor_conv_ctrl_en        (stim_reg_tensor_conv_ctrl_en),
    .reg_tensor_conv_size_en        (stim_reg_tensor_conv_size_en),
    .reg_fcc_en                     (stim_reg_fcc_en),
    .reg_tensor_wait_en             (stim_reg_tensor_wait_en),
    .reg_tex_send_en                (stim_reg_tex_send_en),
    .reg_tensor_mask_read_invalid   (stim_reg_tensor_mask_read_invalid),
    .pmu_read_en_wb                 (stim_pmu_read_en_wb),
    .pmu_read_mem_ready             (stim_pmu_read_mem_ready),
    .dcache_ctrl_resp               (dcache_ctrl_new),
    .fl_barrier_ready               (stim_fl_barrier_ready),
    .tensor_fma_replay_condition    (stim_tensor_fma_replay_condition),
    .tensor_quant_replay_condition  (stim_tensor_quant_replay_condition),
    .vpu_treduce_enabled            (stim_vpu_treduce_enabled),
    .tenb_tl_pend_tfma_en           (stim_tenb_tl_pend_tfma_en),
    .tenb_restart_process           (stim_tenb_restart_process),
    .tload_is_for_tenb              (stim_tload_is_for_tenb),
    .conv_can_accept_new_write      (stim_conv_can_accept_new_write),
    .fcc_ready                      (stim_fcc_ready),
    .wb_fcc_addr                    (stim_wb_fcc_addr),
    .msg_port_pget_block            (stim_msg_port_pget_block),
    .msg_port_stall                 (msg_port_stall_unpacked),
    .reg_tensor_mask_ready          (stim_reg_tensor_mask_ready)
  );

  // -- Original module --
  intpipe_csr_replay_orig u_orig (
    .clock                          (clk_i),
    .reset                          (~rst_ni),
    .wb_wdata                       (stim_wb_wdata),
    .wb_thread_id                   (stim_wb_thread_id),
    .io_interrupt                   (stim_io_interrupt),
    .io_redirect                    (stim_io_redirect),
    .io_replay                      (orig_io_replay),
    .io_replay_stall                (orig_io_replay_stall),
    .reg_co_evict_va_en             (stim_reg_co_evict_va_en),
    .reg_co_flush_va_en             (stim_reg_co_flush_va_en),
    .reg_co_lock_va_en_qual         (stim_reg_co_lock_va_en_qual),
    .reg_co_unlock_va_en_qual       (stim_reg_co_unlock_va_en_qual),
    .reg_co_lock_sw_en              (stim_reg_co_lock_sw_en),
    .reg_co_unlock_sw_en            (stim_reg_co_unlock_sw_en),
    .reg_co_prefetch_va_en          (stim_reg_co_prefetch_va_en),
    .reg_co_evict_sw_en             (stim_reg_co_evict_sw_en),
    .reg_co_flush_sw_en             (stim_reg_co_flush_sw_en),
    .reg_tensor_load_l2scp_en       (stim_reg_tensor_load_l2scp_en),
    .reg_flb_en                     (stim_reg_flb_en),
    .reg_tensor_load_en             (stim_reg_tensor_load_en),
    .reg_tensor_fma_en              (stim_reg_tensor_fma_en),
    .reg_tensor_quant_en            (stim_reg_tensor_quant_en),
    .reg_tensor_reduce_en           (stim_reg_tensor_reduce_en),
    .reg_tensor_store_en            (stim_reg_tensor_store_en),
    .reg_tensor_conv_ctrl_en        (stim_reg_tensor_conv_ctrl_en),
    .reg_tensor_conv_size_en        (stim_reg_tensor_conv_size_en),
    .reg_fcc_en                     (stim_reg_fcc_en),
    .reg_tensor_wait_en             (stim_reg_tensor_wait_en),
    .reg_tex_send_en                (stim_reg_tex_send_en),
    .reg_tensor_mask_read_invalid   (stim_reg_tensor_mask_read_invalid),
    .pmu_read_en_wb                 (stim_pmu_read_en_wb),
    .pmu_read_mem_ready             (stim_pmu_read_mem_ready),
    .dcache_ctrl_resp               (dcache_ctrl_orig),
    .fl_barrier_ready               (stim_fl_barrier_ready),
    .tensor_fma_replay_condition    (stim_tensor_fma_replay_condition),
    .tensor_quant_replay_condition  (stim_tensor_quant_replay_condition),
    .vpu_treduce_enabled            (stim_vpu_treduce_enabled),
    .tenb_tl_pend_tfma_en           (stim_tenb_tl_pend_tfma_en),
    .tenb_restart_process           (stim_tenb_restart_process),
    .tload_is_for_tenb              (stim_tload_is_for_tenb),
    .conv_can_accept_new_write      (stim_conv_can_accept_new_write),
    .fcc_ready                      (stim_fcc_ready),
    .wb_fcc_addr                    (stim_wb_fcc_addr),
    .msg_port_pget_block            (stim_msg_port_pget_block),
    .msg_port_stall                 (msg_port_stall_unpacked),
    .reg_tensor_mask_ready          (stim_reg_tensor_mask_ready)
  );

endmodule

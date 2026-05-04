// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_ml (
  // System signals
  input  logic                                         clock,
  input  logic                                         reset,
  // Control port from core to start new Tensor FMA
  input  core_vpu_ctrl                                 f0_core_ctrl,
  output logic                                         tfma_enabled,
  output logic                                         tfma_wrrf_enabled,
  output logic                                         tquant_enabled,
  output logic                                         tstore_reduce_enabled,
  // Requests to read data to dcache
  input  dcache_vpu_scp_resp                           dcache_scp_resp,
  input  logic [`DCACHE_DATA_RANGE]                    dcache_scp_data,
  output vpu_dcache_ctrl                               dcache_ctrl,
  // Dcache reduce control
  input  dcache_vpu_reduce_ctrl                        dcache_reduce_ctrl,
  // Signals going to VPU/SCP control
  output vpu_ml_load_ctrl                              reduce_load_ctrl,
  output vpu_ml_load_ctrl                              tensorfma_load_ctrl,
  output logic                                         load_ctrl_pre_tena_wen,
  output logic                                         load_ctrl_pre_tenb_wen,
  output logic                                         id_inst_en_next,
  output logic [`INST_RANGE]                           id_inst_next,
  output logic [`VPU_LANE_NUM-1:0]                     id_mask_next,
  output logic                                         id_inst_en,
  input  logic [`INST_RANGE]                           id_inst,
  output logic                                         id_inst_fma_en,
  output logic                                         id_inst_tena_quant_en,
  // TensorTMP control
  output logic                                         f2_tentmp_wen_early,
  output logic                                         f3_tentmp_wen,
  output logic [`VPU_TENTMP_ADDR]                      f3_tentmp_waddr,
  output logic                                         ex_tentmp_ren,
  output logic [`VPU_TENTMP_ADDR]                      ex_tentmp_raddr,
  // TensorQuant RF control
  output logic                                         ex_tquant_ren,
  output logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0] ex_tquant_rdata,
  // Control straight to TIMA (bypasses VPU CTRL)
  output logic                                         ex_tima_valid,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]  ex_tima_mask,
  output logic [`VPU_TENA_ADDR]                        ex_tima_tena_raddr,
  output logic [`VPU_TENB_ADDR]                        ex_tima_tenb_raddr,
  output logic                                         ex_tima_last_pass,
  output logic                                         ex_tima_ren3,
  output logic                                         ex_tima_sa,
  output logic                                         ex_tima_sb,
  output logic                                         ex_tena_rdata_mask,
  output logic                                         f2_tima_ren3,
  output logic [`VPU_TENC_ADDR]                        f2_tima_tenc_raddr,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]  f3_tima_tenc_wen,
  output logic [`VPU_TENC_ADDR]                        f3_tima_tenc_waddr,
  output logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]  f3_tima_rf_wen,
  output logic                                         io_events_wait_tenb,
  // debug
  output logic [10:0]                                  vpu_tensorquant_debug,
  output logic [24:0]                                  vpu_tensorreduce_debug,
  output logic [24:0]                                  vpu_tensorfma_debug
);

////////////////////////////////////////////////////////////////////////////////
// Module that takes control over FPU and executes the TensorFMA instruction
////////////////////////////////////////////////////////////////////////////////

vpu_dcache_ctrl           tensorfma_dcache_ctrl;       // Control signals going to dcache from tensor FMA
logic [`INST_RANGE]       tensorfma_inst_next;         // TensorFMAs generated instruction for next cycle
logic [`VPU_LANE_NUM-1:0] tensorfma_mask_next;         // Mask for the fma instruction
logic                     tensorfma_inst_en_next;      // TensorFMA wants to generate an instruction next cycle
logic                     tfma_wrtenc_wrrf_enabled;    // Sync with TensorQuant/TensorReduce


vpu_tensorfma tensorfma (
  // System signals
  .clock                      (clock),
  .reset                      (reset),
  // Control port from core to start new TensorFMA
  .f0_core_ctrl               (f0_core_ctrl),
  .enabled                    (tfma_enabled),
  // Dependencies with other units
  .tensorquant_pend           (tquant_enabled),
  .tensorstore_pend           (tstore_reduce_enabled),
  // Requests to read data to dcache
  .dcache_scp_resp            (dcache_scp_resp),
  .dcache_ctrl                (tensorfma_dcache_ctrl),
  // Signals going to VPU/Load control
  .load_ctrl                  (tensorfma_load_ctrl),
  .load_ctrl_pre_tena_wen     (load_ctrl_pre_tena_wen),
  .load_ctrl_pre_tenb_wen     (load_ctrl_pre_tenb_wen),
  .fma_en                     (id_inst_fma_en),
  .fma_en_next                (tensorfma_inst_en_next),
  .fma_inst_next              (tensorfma_inst_next),
  .fma_mask_next              (tensorfma_mask_next),
  .fma_inst                   (id_inst),
  .ex_tena_rdata_mask         (ex_tena_rdata_mask),
  // Sync with Core
  .pending_vpu_rf_write       (tfma_wrrf_enabled),
  // Sync with TensorQuant/TensorReduce
  .pending_vpu_tenc_vrf_write (tfma_wrtenc_wrrf_enabled),
  // TensorTMP control
  .f2_tentmp_wen_early        (f2_tentmp_wen_early),
  .f3_tentmp_wen              (f3_tentmp_wen),
  .f3_tentmp_waddr            (f3_tentmp_waddr),
  .ex_tentmp_ren              (ex_tentmp_ren),
  .ex_tentmp_raddr            (ex_tentmp_raddr),
  // Control straight to TIMA (bypasses VPU CTRL)
  .ex_tima_valid              (ex_tima_valid),
  .ex_tima_mask               (ex_tima_mask),
  .ex_tima_tena_raddr         (ex_tima_tena_raddr),
  .ex_tima_tenb_raddr         (ex_tima_tenb_raddr),
  .ex_tima_last_pass          (ex_tima_last_pass),
  .ex_tima_ren3               (ex_tima_ren3),
  .ex_tima_sa                 (ex_tima_sa),
  .ex_tima_sb                 (ex_tima_sb),
  .f2_tima_ren3               (f2_tima_ren3),
  .f2_tima_tenc_raddr         (f2_tima_tenc_raddr),
  .f3_tima_tenc_wen           (f3_tima_tenc_wen),
  .f3_tima_tenc_waddr         (f3_tima_tenc_waddr),
  .f3_tima_rf_wen             (f3_tima_rf_wen),
  .io_events_wait_tenb        (io_events_wait_tenb),
  .vpu_tensorfma_debug        (vpu_tensorfma_debug)
);

////////////////////////////////////////////////////////////////////////////////
// Module that executes the reduce operation between two minions
////////////////////////////////////////////////////////////////////////////////

logic [`INST_RANGE] reduce_inst_next;    // Reduce generated instruction for next cycle
logic               reduce_inst_en_next; // Reduce is enabled for next cycle
logic               reduce_wait;         // Can't start a reduce/tstore due dependencies with other tensorops

vpu_tensorreduce tensorreduce (
  // System signals
  .clock                  (clock),
  .reset                  (reset),
  // Request port for new reduce operation
  .reduce_start           (f0_core_ctrl.reduce_start),
  .tensorstore_start      (f0_core_ctrl.tensorstore_start),
  .tensorfma_start        (f0_core_ctrl.tensorfma_start),
  .tensorquant_start      (f0_core_ctrl.tensorquant_start),
  .reduce_ctrl            (f0_core_ctrl.reduce_ctrl),
  .enabled                (tstore_reduce_enabled),
  // Dependencies with other units
  .scoreboard_pend        (f0_core_ctrl.pending_scoreboard_t0),
  .tensorfma_pend         (tfma_wrtenc_wrrf_enabled),
  .tensorquant_pend       (tquant_enabled),
  .reduce_wait            (reduce_wait),
  // Control coming from Dcache
  .dcache_reduce_ctrl     (dcache_reduce_ctrl),
  // Instructions stuffed in the VPU pipeline
  .load_ctrl              (reduce_load_ctrl),
  .reduce_inst_en_next    (reduce_inst_en_next),
  .reduce_inst_next       (reduce_inst_next),
  // debug
  .vpu_tensorreduce_debug (vpu_tensorreduce_debug)
);

////////////////////////////////////////////////////////////////////////////////
// Module that executes the tensorquant operation
////////////////////////////////////////////////////////////////////////////////

vpu_dcache_scp_req        tensorquant_scp_req;      // Scratchpad request to dcache
logic [`INST_RANGE]       tensorquant_inst_next;    // Instruction to be injected in ID next cycle
logic [`VPU_LANE_NUM-1:0] tensorquant_mask_next;    // Mask for the quant instruction
logic                     tensorquant_inst_en_next; // Injecting to ID an instruction in next cycle
logic                     enabled;                  // ML block has something active

vpu_tensorquant tensorquant (
  // System signals
  .clock                    (clock),
  .reset                    (reset),
  // Control port from core to start new TensorQuant
  .f0_core_ctrl             (f0_core_ctrl),
  .enabled                  (tquant_enabled),
  // Dependencies with other units
  .tensorfma_pend           (tfma_wrtenc_wrrf_enabled),
  .tensorstore_pend         (tstore_reduce_enabled),
  // Scratchpad port
  .dcache_scp_available     (!tensorfma_dcache_ctrl.scp_req.read_en),
  .dcache_scp_req           (tensorquant_scp_req),
  .dcache_scp_data          (dcache_scp_data),
  // Signals going to VPU pipeline
  .quant_en_next            (tensorquant_inst_en_next),
  .quant_inst_next          (tensorquant_inst_next),
  .quant_mask_next          (tensorquant_mask_next),
  .quant_tena_en            (id_inst_tena_quant_en),
  // TensorQuant RF control
  .ex_ren                   (ex_tquant_ren),
  .ex_rdata                 (ex_tquant_rdata),
  // debug
  .vpu_tensorquant_debug    (vpu_tensorquant_debug)
);

//         CLK    RST    EN       DOUT        DIN              DEF
`RST_EN_FF(clock, reset, enabled, id_inst_en, id_inst_en_next, 1'b0)

always_comb begin
  // Stall floating operations if either of the units is enabled
  enabled = tfma_enabled || tstore_reduce_enabled || tquant_enabled;

  // Selects which instruction to generate
  if (reduce_inst_en_next) begin
    id_inst_next = reduce_inst_next;
    id_mask_next = {`VPU_LANE_NUM{1'b1}};
  end else if (tensorquant_inst_en_next) begin
    id_inst_next = tensorquant_inst_next;
    id_mask_next = tensorquant_mask_next;
  end else begin
    id_inst_next = tensorfma_inst_next;
    id_mask_next = tensorfma_mask_next;
  end

  // Selects who is controlling the Scratchpad read
  dcache_ctrl = tensorfma_dcache_ctrl;
  // TensorFMA has priority over TensorQuant
  if (!tensorfma_dcache_ctrl.scp_req.read_en && tensorquant_scp_req.read_en) begin
    dcache_ctrl.scp_req = tensorquant_scp_req;
  end
  // OR the bidding
  dcache_ctrl.scp_req.bid = tensorfma_dcache_ctrl.scp_req.bid || tensorquant_scp_req.bid;
  dcache_ctrl.reduce_wait = reduce_wait;

  id_inst_en_next = tensorfma_inst_en_next || reduce_inst_en_next || tensorquant_inst_en_next;
end


endmodule


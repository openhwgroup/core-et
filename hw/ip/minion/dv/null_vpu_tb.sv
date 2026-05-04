// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// null_vpu_tb -- focused smoke wrapper for the integer-only VPU stub.

module null_vpu_tb
  import minion_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [1:0] pattern_sel_i,
  output logic input_pattern_nonzero_o,
  output logic id_core_ctrl_zero_o,
  output logic ex_core_ctrl_fail_closed_o,
  output logic f2_core_ctrl_zero_o,
  output logic f3_core_ctrl_zero_o,
  output logic wb_core_ctrl_zero_o,
  output logic io_events_zero_o,
  output logic vpu_dbg_match_zero_o,
  output logic vpu_dbg_filter_zero_o,
  output logic vpu_dbg_data_zero_o,
  output logic dcache_ctrl_zero_o
);

  minion_vpu_id_req       id_core_req;
  vpu_ctrl_sigs_t         id_vpu_decoder_sigs;
  minion_vpu_ex_req       ex_core_req;
  logic                   f2_core_kill;
  logic                   f3_core_kill;
  logic                   f4_core_kill;
  core_vpu_ctrl           f0_core_ctrl;
  logic                   chicken_bit_vpulane;
  logic                   chicken_bit_vputima;
  logic                   chicken_bit_vputrans;
  logic [1:0]             mem_ctrl_override;
  logic                   wb_dcache_resp_valid;
  dcache_vpu_resp         wb_dcache_resp;
  dcache_vpu_scp_resp     dcache_scp_resp;
  logic [VpuDataSz-1:0]   dcache_scp_data;
  logic [VpuDataSz-1:0]   dcache_tenb_data;
  dcache_vpu_reduce_ctrl  dcache_reduce_ctrl;

  vpu_id_ctrl_t           id_core_ctrl;
  vpu_minion_ex_ctrl      ex_core_ctrl;
  vpu_minion_tag_ctrl     f2_core_ctrl;
  vpu_minion_mem_ctrl     f3_core_ctrl;
  vpu_minion_wb_ctrl      wb_core_ctrl;
  logic [CsrNrEventsVpu-1:0] io_events;
  logic [63:0]            vpu_dbg_match;
  logic [199:0]           vpu_dbg_filter;
  logic [4:0][127:0]      vpu_dbg_data;
  vpu_dcache_ctrl         dcache_ctrl;

  vpu_id_ctrl_t           expected_id_core_ctrl;
  vpu_minion_ex_ctrl      expected_ex_core_ctrl;
  vpu_minion_tag_ctrl     expected_f2_core_ctrl;
  vpu_minion_mem_ctrl     expected_f3_core_ctrl;
  vpu_minion_wb_ctrl      expected_wb_core_ctrl;
  vpu_dcache_ctrl         expected_dcache_ctrl;

  always_comb begin
    id_core_req = '0;
    id_vpu_decoder_sigs = '0;
    ex_core_req = '0;
    f2_core_kill = 1'b0;
    f3_core_kill = 1'b0;
    f4_core_kill = 1'b0;
    f0_core_ctrl = '0;
    chicken_bit_vpulane = 1'b0;
    chicken_bit_vputima = 1'b0;
    chicken_bit_vputrans = 1'b0;
    mem_ctrl_override = '0;
    wb_dcache_resp_valid = 1'b0;
    wb_dcache_resp = '0;
    dcache_scp_resp = '0;
    dcache_scp_data = '0;
    dcache_tenb_data = '0;
    dcache_reduce_ctrl = '0;

    unique case (pattern_sel_i)
      2'd0: begin
      end
      2'd1: begin
        id_core_req = '{valid: 1'b1, thread_id: 1'b1, fcsr_rm: 3'b101, inst_bits: 32'hf00d_cafe};
        id_vpu_decoder_sigs = '1;
        ex_core_req = '{gscing: 1'b1, gsc_src: '1, kill: 1'b1, fromint_data: 64'h0123_4567_89ab_cdef, thread_id: 1'b1};
        f2_core_kill = 1'b1;
        f4_core_kill = 1'b1;
        f0_core_ctrl = '1;
        chicken_bit_vpulane = 1'b1;
        chicken_bit_vputrans = 1'b1;
        mem_ctrl_override = 2'b10;
        wb_dcache_resp_valid = 1'b1;
        wb_dcache_resp = '1;
        dcache_scp_resp = '1;
        dcache_scp_data = {{(VpuDataSz-64){1'b0}}, 64'hfeed_face_dead_beef};
        dcache_tenb_data = {{(VpuDataSz-64){1'b0}}, 64'h55aa_f00d_cafe_1234};
        dcache_reduce_ctrl = '{send_reg: 1'b1, exec_op: 1'b0, nothing: 1'b1};
      end
      2'd2: begin
        id_core_req = '1;
        id_vpu_decoder_sigs = '1;
        ex_core_req = '1;
        f2_core_kill = 1'b1;
        f3_core_kill = 1'b1;
        f4_core_kill = 1'b1;
        f0_core_ctrl = '1;
        chicken_bit_vpulane = 1'b1;
        chicken_bit_vputima = 1'b1;
        chicken_bit_vputrans = 1'b1;
        mem_ctrl_override = '1;
        wb_dcache_resp_valid = 1'b1;
        wb_dcache_resp = '1;
        dcache_scp_resp = '1;
        dcache_scp_data = '1;
        dcache_tenb_data = '1;
        dcache_reduce_ctrl = '1;
      end
      default: begin
        id_core_req = '{valid: 1'b1, thread_id: 1'b0, fcsr_rm: 3'b010, inst_bits: 32'h1357_9bdf};
        id_vpu_decoder_sigs = '1;
        id_vpu_decoder_sigs.cmd = VpuCmdX;
        id_vpu_decoder_sigs.dtype = VpuDtypeF32;
        ex_core_req = '{gscing: 1'b0, gsc_src: '0, kill: 1'b1, fromint_data: 64'h89ab_cdef_0123_4567, thread_id: 1'b0};
        f3_core_kill = 1'b1;
        f0_core_ctrl = '{tensorfma_start: 1'b1, reduce_start: 1'b1, tensorstore_start: 1'b1,
                         tensorquant_start: 1'b1, pending_scoreboard_t0: 1'b1, default: '1};
        chicken_bit_vputima = 1'b1;
        mem_ctrl_override = 2'b01;
        wb_dcache_resp_valid = 1'b1;
        wb_dcache_resp = '{thread_id: 1'b1, gdst: '1, typ: DcTypePs, addr: 5'h12, data: '1, ps_mask: '1};
        dcache_scp_resp = '{fill_is_tenb_early: 1'b1, fill_is_tenb: 1'b0, tenb_flush: 1'b1, tenb_line: 4'ha};
        dcache_scp_data = {{(VpuDataSz-64){1'b0}}, 64'ha5a5_a5a5_5a5a_5a5a};
        dcache_tenb_data = {{(VpuDataSz-64){1'b0}}, 64'h3c3c_c3c3_9696_6969};
        dcache_reduce_ctrl = '{send_reg: 1'b0, exec_op: 1'b1, nothing: 1'b0};
      end
    endcase
  end

  always_comb begin
    expected_id_core_ctrl = '0;
    expected_ex_core_ctrl = '0;
    expected_ex_core_ctrl.illegal_rm = 1'b1;
    expected_f2_core_ctrl = '0;
    expected_f3_core_ctrl = '0;
    expected_wb_core_ctrl = '0;
    expected_dcache_ctrl = '0;

    input_pattern_nonzero_o = |{
      id_core_req,
      id_vpu_decoder_sigs,
      ex_core_req,
      f2_core_kill,
      f3_core_kill,
      f4_core_kill,
      f0_core_ctrl,
      chicken_bit_vpulane,
      chicken_bit_vputima,
      chicken_bit_vputrans,
      mem_ctrl_override,
      wb_dcache_resp_valid,
      wb_dcache_resp,
      dcache_scp_resp,
      dcache_scp_data,
      dcache_tenb_data,
      dcache_reduce_ctrl
    };

    id_core_ctrl_zero_o = (id_core_ctrl == expected_id_core_ctrl);
    ex_core_ctrl_fail_closed_o = (ex_core_ctrl == expected_ex_core_ctrl);
    f2_core_ctrl_zero_o = (f2_core_ctrl == expected_f2_core_ctrl);
    f3_core_ctrl_zero_o = (f3_core_ctrl == expected_f3_core_ctrl);
    wb_core_ctrl_zero_o = (wb_core_ctrl == expected_wb_core_ctrl);
    io_events_zero_o = (io_events == '0);
    vpu_dbg_match_zero_o = (vpu_dbg_match == '0);
    vpu_dbg_filter_zero_o = (vpu_dbg_filter == '0);
    vpu_dbg_data_zero_o = (vpu_dbg_data == '0);
    dcache_ctrl_zero_o = (dcache_ctrl == expected_dcache_ctrl);
  end

  null_vpu u_null_vpu (
    .clk_i                 (clk_i),
    .rst_ni                (rst_ni),
    .id_core_req_i         (id_core_req),
    .id_vpu_decoder_sigs_i (id_vpu_decoder_sigs),
    .ex_core_req_i         (ex_core_req),
    .f2_core_kill_i        (f2_core_kill),
    .f3_core_kill_i        (f3_core_kill),
    .f4_core_kill_i        (f4_core_kill),
    .f0_core_ctrl_i        (f0_core_ctrl),
    .chicken_bit_vpulane_i (chicken_bit_vpulane),
    .chicken_bit_vputima_i (chicken_bit_vputima),
    .chicken_bit_vputrans_i(chicken_bit_vputrans),
    .mem_ctrl_override_i   (mem_ctrl_override),
    .wb_dcache_resp_valid_i(wb_dcache_resp_valid),
    .wb_dcache_resp_i      (wb_dcache_resp),
    .dcache_scp_resp_i     (dcache_scp_resp),
    .dcache_scp_data_i     (dcache_scp_data),
    .dcache_tenb_data_i    (dcache_tenb_data),
    .dcache_reduce_ctrl_i  (dcache_reduce_ctrl),
    .id_core_ctrl_o        (id_core_ctrl),
    .ex_core_ctrl_o        (ex_core_ctrl),
    .f2_core_ctrl_o        (f2_core_ctrl),
    .f3_core_ctrl_o        (f3_core_ctrl),
    .wb_core_ctrl_o        (wb_core_ctrl),
    .io_events_o           (io_events),
    .vpu_dbg_match_o       (vpu_dbg_match),
    .vpu_dbg_filter_o      (vpu_dbg_filter),
    .vpu_dbg_data_o        (vpu_dbg_data),
    .dcache_ctrl_o         (dcache_ctrl)
  );

endmodule

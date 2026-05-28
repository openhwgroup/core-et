// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_xll_control #(
  parameter int unsigned ClockDivideBits = 4,
  parameter int unsigned ClockDivideSelBits = 2,
  parameter int unsigned RegisterPtrBits = 6,
  parameter int unsigned RegisterNumBits = 4,
  parameter int unsigned RegisterAddrBits = 6,
  parameter int unsigned RegisterDataBits = 16,
  parameter int unsigned RegisterDataBytes = 2,
  parameter int unsigned RegisterWdataBits = 256
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  logic                                      esr_enable_i,
  input  logic                                      esr_reset_i,
  input  logic                                      esr_run_i,
  input  logic                                      esr_write_i,
  input  logic [RegisterPtrBits-1:0]                esr_reg_first_i,
  input  logic [RegisterNumBits-1:0]                esr_reg_num_i,
  input  logic [ClockDivideSelBits-1:0]             esr_pclk_sel_i,
  input  logic [RegisterWdataBits-1:0]              esr_wdata_i,
  output logic                                      esr_busy_o,
  output logic                                      esr_rrdy_o,
  output logic [RegisterDataBits-1:0]               esr_rdata_o,

  output logic                                      pclk_o,
  output logic                                      preset_no,
  output logic [RegisterAddrBits-1:0]               paddr_o,
  output logic                                      psel_o,
  output logic                                      penable_o,
  output logic                                      pwrite_o,
  output logic [RegisterDataBits-1:0]               pwdata_o,
  output logic [RegisterDataBytes-1:0]              pstrb_o,
  input  logic [RegisterDataBits-1:0]               prdata_i,
  input  logic                                      pready_i
);

  logic ctrl_enable;
  logic clock_en;
  logic [ClockDivideBits-1:0] clock_count_q;
  logic [ClockDivideBits-1:0] clock_count_d;
  logic clock_count_en;
  logic pclk_gen_q;
  logic pclk_gen_d;
  logic eval_rdy;
  logic rdy_value;
  logic clr_rdy;
  logic rdy_q;

  logic auto_run_pulse;
  logic auto_run_q;
  logic trigger_op;
  logic trigger_op_pending_q;
  logic trigger_op_pending_d;

  logic program_state_q;
  logic program_state_d;
  logic program_ctail_q;
  logic program_ctail_d;

  logic [RegisterNumBits-1:0] reg_idx_q;
  logic [RegisterNumBits-1:0] reg_idx_d;
  logic [RegisterPtrBits-1:0] reg_ptr_q;
  logic [RegisterPtrBits-1:0] reg_ptr_d;
  logic [RegisterPtrBits-1:0] reg_ptr_first;
  logic [RegisterPtrBits-1:0] reg_ptr_last;
  logic [RegisterPtrBits-1:0] esr_reg_num_ext;
  logic reg_ptr_up;
  logic reg_transfer_end;
  logic reg_ctail_up;

  logic [1:0] count_access_q;
  logic [1:0] count_access_d;
  logic count_access_up;

  logic [RegisterDataBits-1:0] reg_esr_user_value;
  logic [RegisterDataBits-1:0] prdata_q;
  logic prdata_up;

  assign ctrl_enable = esr_enable_i | program_state_q | program_ctail_q;
  assign esr_busy_o = trigger_op_pending_q | program_state_q | program_ctail_q;

  assign clock_count_d = clock_count_q - 1'b1;
  assign clock_count_en = ctrl_enable | (clock_count_q != {ClockDivideBits{1'b1}});
  assign pclk_gen_d = ~clock_count_q[esr_pclk_sel_i];
  assign pclk_o = pclk_gen_q;
  assign preset_no = rst_ni & ~esr_reset_i;

  assign clock_en = (pclk_gen_q & ~pclk_gen_d) & ctrl_enable;
  assign eval_rdy = (~pclk_gen_q & pclk_gen_d) & ctrl_enable;
  assign clr_rdy = clock_en;
  assign rdy_value = clr_rdy ? 1'b0 : (eval_rdy & pready_i);

  always_comb begin
    trigger_op_pending_d = trigger_op_pending_q;
    if (auto_run_pulse) begin
      trigger_op_pending_d = 1'b1;
    end
    if (trigger_op) begin
      trigger_op_pending_d = 1'b0;
    end
  end

  assign auto_run_pulse = esr_run_i && !auto_run_q;
  assign trigger_op = trigger_op_pending_q && clock_en;

  always_comb begin
    reg_ptr_d = reg_ptr_q + 1'b1;
    reg_idx_d = reg_idx_q + 1'b1;
    program_state_d = program_state_q;
    program_ctail_d = program_ctail_q;

    if ((reg_ptr_q == reg_ptr_last) && reg_transfer_end) begin
      program_state_d = 1'b0;
      program_ctail_d = 1'b1;
    end

    if (trigger_op) begin
      reg_ptr_d = reg_ptr_first;
      reg_idx_d = '0;
      program_state_d = 1'b1;
    end

    if (program_ctail_q && (count_access_q == 2'd3)) begin
      program_ctail_d = 1'b0;
    end
  end

  assign reg_ptr_up = trigger_op || reg_transfer_end;
  assign reg_ctail_up = program_state_q || program_ctail_q;
  assign reg_transfer_end = clock_en && (count_access_q == 2'd3) && program_state_q;
  assign reg_ptr_first = esr_reg_first_i;
  assign esr_reg_num_ext = {{(RegisterPtrBits-RegisterNumBits){1'b0}}, esr_reg_num_i};
  assign reg_ptr_last = esr_write_i ? (esr_reg_first_i + esr_reg_num_ext) : esr_reg_first_i;

  always_comb begin
    count_access_d = count_access_q + 2'd1;
    if (trigger_op) begin
      count_access_d = 2'b00;
    end
  end

  assign count_access_up = trigger_op ||
      (program_state_q && clock_en && !(psel_o && penable_o && !rdy_q)) ||
      (program_ctail_q && clock_en);

  assign reg_esr_user_value = esr_wdata_i[reg_idx_q*RegisterDataBits +: RegisterDataBits];

  assign psel_o = program_state_q && (count_access_q != 2'd0) && (count_access_q != 2'd3);
  assign penable_o = program_state_q && (count_access_q == 2'd2);
  assign paddr_o = reg_ptr_q[RegisterAddrBits-1:0];
  assign pwrite_o = esr_write_i;
  assign pwdata_o = reg_esr_user_value;
  assign pstrb_o = {RegisterDataBytes{1'b1}};

  assign prdata_up = program_state_q && pready_i && clock_en && !esr_write_i;
  assign esr_rdata_o = prdata_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      clock_count_q        <= {ClockDivideBits{1'b1}};
      pclk_gen_q           <= 1'b0;
      rdy_q                <= 1'b0;
      auto_run_q           <= 1'b0;
      trigger_op_pending_q <= 1'b0;
      program_state_q      <= 1'b0;
      reg_ptr_q            <= '0;
      reg_idx_q            <= '0;
      program_ctail_q      <= 1'b0;
      count_access_q       <= 2'b00;
      esr_rrdy_o           <= 1'b0;
    end else begin
      if (clock_count_en) begin
        clock_count_q <= clock_count_d;
      end
      pclk_gen_q <= pclk_gen_d;
      if (ctrl_enable && (eval_rdy || clr_rdy)) begin
        rdy_q <= rdy_value;
      end
      if (ctrl_enable) begin
        auto_run_q <= esr_run_i;
        trigger_op_pending_q <= trigger_op_pending_d;
      end
      if (reg_ptr_up) begin
        program_state_q <= program_state_d;
        reg_ptr_q <= reg_ptr_d;
        reg_idx_q <= reg_idx_d;
      end
      if (reg_ctail_up) begin
        program_ctail_q <= program_ctail_d;
      end
      if (count_access_up) begin
        count_access_q <= count_access_d;
      end
      if (clock_en) begin
        esr_rrdy_o <= prdata_up;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (prdata_up) begin
      prdata_q <= prdata_i;
    end
  end

endmodule

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_trans
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit EnableExtraTrans = EnableExtraTransDefault
  )
(
  input  logic                                             clk_aon_i,
  input  logic                                             rst_ni,
  input  logic                                             id_valid_i,
  input  logic                                             id_core_thread_id_i,
  input  logic [InstSize-1:0]                              id_instruction_i,
  input  logic                                             ex_valid_i,
  input  logic                                             ex_core_thread_id_i,
  input  logic [InstSize-1:0]                              ex_instruction_i,
  input  logic [VpuLaneNum-1:0]                            ex_mask_rf0_i,
  input  logic                                             chicken_bit_vputrans_i,
  input  logic                                             f6_killed_i,
  input  logic                                             f7_killed_i,
  input  logic                                             f8_killed_i,
  output logic                                             id_insert_en_next_o,
  output logic [InstSize-1:0]                              id_insert_inst_next_o,
  output logic                                             id_insert_en_o,
  output logic                                             id_trans_thread_id_o,
  output logic                                             ex_trans_thread_id_o,
  output logic                                             f6_trans_ctrl_en_o,
  output trans_scoreboard_slot                                 id_trans_scoreboard_o [VpuTransScoreboardSize/2-1:0],
  output logic [VpuLaneNum-1:0]                            f6_trans_wmask_o,
  output logic                                             f6_trans_thread_id_o,
  output logic                                             f8_trans_ctrl_en_o,
  output logic                                             f8_trans_fcsr_en_o,
  output logic                                             id_trans_busy_o,
  output trans_state_t                                     state_o
);

  localparam int unsigned TransPipeStages      = 7;
  localparam int unsigned TransScoreboardSlots = VpuTransScoreboardSize / 2;
  localparam int unsigned TransPipeIdxW        = $clog2(TransPipeStages);
  localparam logic [TransPipeIdxW-1:0] TransLastSlot     = 3'd6;
  localparam logic [TransPipeIdxW-1:0] TransWrapMinusTwo = 3'd5;
  localparam logic [TransPipeIdxW-1:0] TransPlusTwo      = 3'd2;

  localparam logic [31:0] TransNop = 32'h0000_0013;

  localparam logic [6:0] TrFma1RcpF7    = 7'b0000000;
  localparam logic [6:0] TrFma2RcpF7    = 7'b0000001;
  localparam logic [6:0] TrFma1RsqrtF7  = 7'b0000010;
  localparam logic [6:0] TrFma2RsqrtF7  = 7'b0000011;
  localparam logic [6:0] TrNrF7         = 7'b0000100;
  localparam logic [6:0] TrFma1LogF7    = 7'b0000111;
  localparam logic [6:0] TrFma2LogF7    = 7'b0001000;
  localparam logic [6:0] TrMulLogF7     = 7'b0001001;
  localparam logic [6:0] TrTrSinF7      = 7'b0001010;
  localparam logic [6:0] TrRrSinF7      = 7'b0001011;
  localparam logic [6:0] TrP1SinF7      = 7'b0001100;
  localparam logic [6:0] TrP2SinF7      = 7'b0001101;
  localparam logic [6:0] TrP3SinF7      = 7'b0001110;
  localparam logic [6:0] TrRrExpF7      = 7'b0001111;
  localparam logic [6:0] TrFma1ExpF7    = 7'b0010000;
  localparam logic [6:0] TrFma2ExpF7    = 7'b0010001;
  localparam logic [6:0] RTypeOpcode    = 7'b0110011;
  localparam logic [2:0] RoundNearestEven = 3'b000;

  logic                      clk_gated;
  logic                      enable_slot;
  logic [TransPipeIdxW-1:0]  slot_current_q;
  logic [TransPipeIdxW-1:0]  slot_next;
  logic [TransPipeIdxW-1:0]  slot_next_next;
  logic [TransPipeIdxW-1:0]  slot_previous;

  trans_state_t              states_q [TransPipeStages-1:0];
  trans_state_t              states_d [TransPipeStages-1:0];
  trans_state_t              id_state;
  trans_state_t              ex_state_q;
  trans_state_t              f5_state;
  trans_state_t              f6_state_q;
  trans_state_t              push_ex_state_d;

  logic                      next_id_trans_thread_id_d;
  trans_write_ctrl_t         f5_write_ctrl_d;
  /* verilator lint_off UNUSEDSIGNAL */  // The original split write-control pipe carries mask/thread bits earlier than they are consumed; some fields are intentionally unused in one stage and picked up in the next.
  trans_write_ctrl_t         f6_write_ctrl_q;
  trans_write_ctrl_t         f7_write_ctrl_d;
  trans_write_ctrl_t         f8_write_ctrl_q;
  /* verilator lint_on UNUSEDSIGNAL */
  trans_scoreboard_slot      trans_scoreboard_next_d [TransScoreboardSlots-1:0];

  function automatic trans_state_t trans_null_state();
    trans_state_t s;
    begin
      s = '0;
      s.id = TransNullState;
      return s;
    end
  endfunction

  /* verilator lint_off UNUSEDSIGNAL */  // These helpers intentionally inspect only the specific fields used by the original control logic; the remaining packed fields stay untouched.
  function automatic logic state_is_null(trans_state_t s);
    begin
      return s.id == TransNullState;
    end
  endfunction

  function automatic logic state_update_en(trans_state_t proposed, trans_state_t current);
    begin
      return ((proposed.id != TransNullState) || (current.id != TransNullState)) && (proposed != current);
    end
  endfunction

  function automatic trans_state_t trans_cstate(trans_state_id_e state_id, trans_state_t old_state);
    trans_state_t s;
    begin
      s = old_state;
      s.id = state_id;
      return s;
    end
  endfunction

  function automatic trans_state_t trans_cistate(
    trans_state_id_e       state_id,
    logic                  thread_id,
    logic [InstSize-1:0]   instruction,
    logic [VpuLaneNum-1:0] mask
  );
    trans_state_t s;
    begin
      s = '0;
      s.id = state_id;
      s.thread_id = thread_id;
      s.rs = instruction[19:15];
      s.rd = instruction[11:7];
      s.mask_valid = 1'b0;
      s.mask = mask;
      return s;
    end
  endfunction

  function automatic logic [InstSize-1:0] trans_instr(
    logic [6:0]                funct7,
    logic [FregAddrSize-1:0]   rd,
    logic [FregAddrSize-1:0]   rs1,
    logic [FregAddrSize-1:0]   rs2
  );
    begin
      return {funct7, rs2, rs1, RoundNearestEven, rd, RTypeOpcode};
    end
  endfunction

  function automatic logic is_used(trans_state_t s);
    begin
      unique case (s.id)
        TransNullState,
        TransDirtyState: is_used = 1'b0;
        default:         is_used = 1'b1;
      endcase
    end
  endfunction

  function automatic trans_write_ctrl_t make_write_ctrl(trans_state_t s);
    trans_write_ctrl_t ctrl;
    begin
      ctrl = '0;
      unique case (s.id)
        TransExpRrState: begin
          ctrl.en = 1'b1;
          ctrl.mask = s.mask;
          ctrl.thread_id = s.thread_id;
        end
        TransSinRrState: begin
          if (EnableExtraTrans) begin
            ctrl.en = 1'b1;
            ctrl.mask = s.mask;
            ctrl.thread_id = s.thread_id;
          end
        end
        TransDirtyState: begin
          ctrl.en = 1'b1;
          ctrl.mask = s.mask;
          ctrl.thread_id = s.thread_id;
          ctrl.fcsr_en = |s.mask;
        end
        TransNullState: begin
          ctrl = '0;
        end
        default: begin
          ctrl.en = 1'b1;
          ctrl.mask = '0;
          ctrl.thread_id = s.thread_id;
        end
      endcase
      return ctrl;
    end
  endfunction

  function automatic minion_reg_dest_t make_reg_dest(
    logic [FregAddrSize-1:0] addr,
    logic                    thread_id
  );
    minion_reg_dest_t dest;
    begin
      dest = '0;
      dest.fp = 1'b1;
      dest.addr = addr;
      dest.thread_id = thread_id;
      return dest;
    end
  endfunction

  function automatic trans_scoreboard_slot make_slot_from_state(trans_state_t s);
    trans_scoreboard_slot slot;
    begin
      slot = '0;
      unique case (s.id)
        TransRcpFma1State,
        TransRcpFma2State,
        TransRsqrtFma1State,
        TransRsqrtFma2State,
        TransLogFma1State,
        TransLogFma2State,
        TransLogMulState,
        TransNrState: begin
          if ((s.id != TransRsqrtFma1State && s.id != TransRsqrtFma2State) || EnableExtraTrans) begin
            slot.rs_valid = 1'b1;
            slot.rs = make_reg_dest(s.rs, s.thread_id);
            slot.rd_valid = 1'b1;
            slot.rd = make_reg_dest(s.rd, s.thread_id);
          end
        end
        TransExpRrState,
        TransExpFma1State,
        TransExpFma2State,
        TransSinTrState,
        TransSinRrState,
        TransSinP1State,
        TransSinP2State,
        TransSinP3State,
        TransDirtyState: begin
          if ((s.id != TransSinTrState && s.id != TransSinRrState && s.id != TransSinP1State &&
               s.id != TransSinP2State && s.id != TransSinP3State) || EnableExtraTrans) begin
            slot.rd_valid = 1'b1;
            slot.rd = make_reg_dest(s.rd, s.thread_id);
          end
        end
        default: begin
        end
      endcase
      return slot;
    end
  endfunction

  function automatic trans_scoreboard_slot make_ex_slot(
    trans_state_t           s,
    logic                   valid,
    logic                   thread_id,
    logic [InstSize-1:0]    instruction,
    logic                   killed
  );
    trans_scoreboard_slot slot;
    begin
      slot = '0;
      if (state_is_null(s) || (s.id == TransDirtyState) || killed) begin
        if (valid) begin
          casez (instruction)
            32'b010110000111?????000?????1111011: begin // FRCP_PS
              slot.rs_valid = 1'b1;
              slot.rs = make_reg_dest(instruction[19:15], thread_id);
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(instruction[11:7], thread_id);
            end
            32'b010110001000?????000?????1111011: begin // FRSQ_PS
              if (EnableExtraTrans) begin
                slot.rs_valid = 1'b1;
                slot.rs = make_reg_dest(instruction[19:15], thread_id);
                slot.rd_valid = 1'b1;
                slot.rd = make_reg_dest(instruction[11:7], thread_id);
              end
            end
            32'b010110000011?????000?????1111011: begin // FLOG_PS
              slot.rs_valid = 1'b1;
              slot.rs = make_reg_dest(instruction[19:15], thread_id);
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(instruction[11:7], thread_id);
            end
            32'b010110000110?????000?????1111011: begin // FSIN_PS
              if (EnableExtraTrans) begin
                slot.rs_valid = 1'b1;
                slot.rs = make_reg_dest(instruction[19:15], thread_id);
                slot.rd_valid = 1'b1;
                slot.rd = make_reg_dest(instruction[11:7], thread_id);
              end
            end
            32'b010110000100?????000?????1111011: begin // FEXP_PS
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(instruction[11:7], thread_id);
            end
            32'b0011000??????????000?????1111011: begin // FRCP_FIX_RAST
              slot.rs_valid = 1'b1;
              slot.rs = make_reg_dest(instruction[19:15], thread_id);
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(instruction[11:7], thread_id);
            end
            default: begin
            end
          endcase
        end
      end else begin
        unique case (s.id)
          TransRcpFma1State,
          TransLogFma1State,
          TransLogFma2State: begin
            slot.rs_valid = 1'b1;
            slot.rs = make_reg_dest(s.rs, s.thread_id);
            slot.rd_valid = 1'b1;
            slot.rd = make_reg_dest(s.rd, s.thread_id);
          end
          TransRcpFma2State,
          TransLogMulState,
          TransNrState,
          TransExpRrState,
          TransExpFma1State,
          TransExpFma2State: begin
            slot.rd_valid = 1'b1;
            slot.rd = make_reg_dest(s.rd, s.thread_id);
          end
          TransRsqrtFma1State: begin
            if (EnableExtraTrans) begin
              slot.rs_valid = 1'b1;
              slot.rs = make_reg_dest(s.rs, s.thread_id);
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(s.rd, s.thread_id);
            end
          end
          TransRsqrtFma2State: begin
            if (EnableExtraTrans) begin
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(s.rd, s.thread_id);
            end
          end
          TransSinTrState,
          TransSinRrState,
          TransSinP1State,
          TransSinP2State,
          TransSinP3State: begin
            if (EnableExtraTrans) begin
              slot.rd_valid = 1'b1;
              slot.rd = make_reg_dest(s.rd, s.thread_id);
            end
          end
          default: begin
          end
        endcase
      end
      return slot;
    end
  endfunction
  /* verilator lint_on UNUSEDSIGNAL */

  prim_clk_gate u_clk_gate (
    .clk_i (clk_aon_i),
    .en_i  (enable_slot || chicken_bit_vputrans_i || !rst_ni),
    .dft_i ('0),
    .clk_o (clk_gated)
  );

  always_comb begin
    slot_next = (slot_current_q == TransLastSlot) ? '0 : (slot_current_q + 1'b1);
    slot_next_next = (slot_current_q >= TransWrapMinusTwo) ? (slot_current_q - TransWrapMinusTwo)
                                                            : (slot_current_q + TransPlusTwo);
    slot_previous = (slot_current_q == '0) ? TransLastSlot : (slot_current_q - 1'b1);

    id_state = states_q[slot_current_q];
    f5_state = states_q[slot_next_next];

    for (int i = 0; i < TransPipeStages; ++i) begin
      states_d[i] = states_q[i];
    end
    states_d[slot_previous] = push_ex_state_d;
  end

  assign state_o = id_state;

  always_comb begin
    enable_slot = (push_ex_state_d.id != TransNullState);
    for (int i = 0; i < TransPipeStages; ++i) begin
      enable_slot |= (states_q[i].id != TransNullState);
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) ex_state_q <= '0;
    else if (state_update_en(id_state, ex_state_q)) ex_state_q <= id_state;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) f6_state_q <= '0;
    else if (state_update_en(f5_state, f6_state_q)) f6_state_q <= f5_state;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) slot_current_q <= '0;
    else if (enable_slot) slot_current_q <= slot_next;
  end

  for (genvar i = 0; i < TransPipeStages; ++i) begin : gen_state_ff
    always_ff @(posedge clk_gated or negedge rst_ni) begin
      if (!rst_ni) states_q[i] <= '0;
      else if (state_update_en(states_d[i], states_q[i])) states_q[i] <= states_d[i];
    end
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) id_insert_en_o <= 1'b0;
    else         id_insert_en_o <= id_insert_en_next_o;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) id_trans_thread_id_o <= 1'b0;
    else if (id_insert_en_next_o) id_trans_thread_id_o <= next_id_trans_thread_id_d;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) f6_write_ctrl_q <= '0;
    else if (f5_write_ctrl_d.en || f6_write_ctrl_q.en) f6_write_ctrl_q <= f5_write_ctrl_d;
  end

  always_ff @(posedge clk_gated or negedge rst_ni) begin
    if (!rst_ni) f8_write_ctrl_q <= '0;
    else if (f7_write_ctrl_d.en || f8_write_ctrl_q.en) f8_write_ctrl_q <= f7_write_ctrl_d;
  end

  always_ff @(posedge clk_aon_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (int i = 0; i < TransScoreboardSlots; ++i) begin
        id_trans_scoreboard_o[i] <= '0;
      end
    end else if (id_valid_i || enable_slot) begin
      for (int i = 0; i < TransScoreboardSlots; ++i) begin
        id_trans_scoreboard_o[i] <= trans_scoreboard_next_d[i];
      end
    end
  end

  always_comb begin
    id_trans_busy_o = 1'b0;
    for (int i = 0; i < TransPipeStages; ++i) begin
      id_trans_busy_o |= is_used(states_q[i]);
    end

    push_ex_state_d = trans_null_state();
    if (state_is_null(ex_state_q) || (ex_state_q.id == TransDirtyState) || f8_killed_i) begin
      if (ex_valid_i) begin
        casez (ex_instruction_i)
          32'b010110000111?????000?????1111011: begin // FRCP_PS
            push_ex_state_d = trans_cistate(TransRcpFma1State, ex_core_thread_id_i, ex_instruction_i, ex_mask_rf0_i);
          end
          32'b010110001000?????000?????1111011: begin // FRSQ_PS
            if (EnableExtraTrans) begin
              push_ex_state_d = trans_cistate(TransRsqrtFma1State, ex_core_thread_id_i, ex_instruction_i, ex_mask_rf0_i);
            end
          end
          32'b010110000011?????000?????1111011: begin // FLOG_PS
            push_ex_state_d = trans_cistate(TransLogFma1State, ex_core_thread_id_i, ex_instruction_i, ex_mask_rf0_i);
          end
          32'b010110000110?????000?????1111011: begin // FSIN_PS
            if (EnableExtraTrans) begin
              push_ex_state_d = trans_cistate(TransSinTrState, ex_core_thread_id_i, ex_instruction_i, ex_mask_rf0_i);
            end
          end
          32'b010110000100?????000?????1111011: begin // FEXP_PS
            push_ex_state_d = trans_cistate(TransExpRrState, ex_core_thread_id_i, ex_instruction_i, ex_mask_rf0_i);
          end
          32'b0011000??????????000?????1111011: begin // FRCP_FIX_RAST
            push_ex_state_d = '{
              id:         TransNrState,
              thread_id:  ex_core_thread_id_i,
              rs:         ex_instruction_i[24:20],
              rd:         ex_instruction_i[11:7],
              mask_valid: 1'b0,
              mask:       ex_mask_rf0_i
            };
          end
          default: begin
          end
        endcase
        id_trans_busy_o |= (push_ex_state_d.id != TransNullState);
      end
    end else begin
      unique case (ex_state_q.id)
        TransRcpFma1State:   push_ex_state_d = trans_cstate(TransRcpFma2State, ex_state_q);
        TransRcpFma2State:   push_ex_state_d = trans_cstate(TransDirtyState, ex_state_q);
        TransRsqrtFma1State: push_ex_state_d = EnableExtraTrans ? trans_cstate(TransRsqrtFma2State, ex_state_q) : trans_null_state();
        TransRsqrtFma2State: push_ex_state_d = EnableExtraTrans ? trans_cstate(TransDirtyState, ex_state_q) : trans_null_state();
        TransLogFma1State:   push_ex_state_d = trans_cstate(TransLogFma2State, ex_state_q);
        TransLogFma2State:   push_ex_state_d = trans_cstate(TransLogMulState, ex_state_q);
        TransLogMulState:    push_ex_state_d = trans_cstate(TransDirtyState, ex_state_q);
        TransSinTrState:     push_ex_state_d = EnableExtraTrans ? trans_cstate(TransSinRrState, ex_state_q) : trans_null_state();
        TransSinRrState:     push_ex_state_d = EnableExtraTrans ? trans_cstate(TransSinP1State, ex_state_q) : trans_null_state();
        TransSinP1State:     push_ex_state_d = EnableExtraTrans ? trans_cstate(TransSinP2State, ex_state_q) : trans_null_state();
        TransSinP2State:     push_ex_state_d = EnableExtraTrans ? trans_cstate(TransSinP3State, ex_state_q) : trans_null_state();
        TransSinP3State:     push_ex_state_d = EnableExtraTrans ? trans_cstate(TransDirtyState, ex_state_q) : trans_null_state();
        TransExpRrState:     push_ex_state_d = trans_cstate(TransExpFma1State, ex_state_q);
        TransExpFma1State:   push_ex_state_d = trans_cstate(TransExpFma2State, ex_state_q);
        TransExpFma2State:   push_ex_state_d = trans_cstate(TransDirtyState, ex_state_q);
        TransNrState:        push_ex_state_d = trans_cstate(TransDirtyState, ex_state_q);
        default: begin
        end
      endcase
    end
  end

  always_comb begin
    id_insert_en_next_o = 1'b0;
    id_insert_inst_next_o = TransNop;
    next_id_trans_thread_id_d = 1'b0;
    ex_trans_thread_id_o = ex_state_q.thread_id;

    if ((f6_state_q.id != TransNullState) && (f6_state_q.id != TransDirtyState) && !f6_killed_i) begin
      id_insert_en_next_o = 1'b1;
      next_id_trans_thread_id_d = f6_state_q.thread_id;

      unique case (f6_state_q.id)
        TransRcpFma1State: id_insert_inst_next_o = trans_instr(TrFma1RcpF7,   f6_state_q.rd, '0,            f6_state_q.rs);
        TransRcpFma2State: id_insert_inst_next_o = trans_instr(TrFma2RcpF7,   f6_state_q.rd, '0,            f6_state_q.rs);
        TransRsqrtFma1State: if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrFma1RsqrtF7, f6_state_q.rd, '0,            f6_state_q.rs);
        TransRsqrtFma2State: if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrFma2RsqrtF7, f6_state_q.rd, '0,            f6_state_q.rs);
        TransLogFma1State: id_insert_inst_next_o = trans_instr(TrFma1LogF7,   f6_state_q.rd, '0,            f6_state_q.rs);
        TransLogFma2State: id_insert_inst_next_o = trans_instr(TrFma2LogF7,   f6_state_q.rd, '0,            f6_state_q.rs);
        TransLogMulState:  id_insert_inst_next_o = trans_instr(TrMulLogF7,    f6_state_q.rd, '0,            f6_state_q.rs);
        TransSinTrState:   if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrTrSinF7,     f6_state_q.rd, '0,            '0);
        TransSinRrState:   if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrRrSinF7,     f6_state_q.rd, '0,            f6_state_q.rd);
        TransSinP1State:   if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrP1SinF7,     f6_state_q.rd, f6_state_q.rd, f6_state_q.rd);
        TransSinP2State:   if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrP2SinF7,     f6_state_q.rd, '0,            f6_state_q.rd);
        TransSinP3State:   if (EnableExtraTrans)
          id_insert_inst_next_o = trans_instr(TrP3SinF7,     f6_state_q.rd, '0,            f6_state_q.rd);
        TransExpRrState:   id_insert_inst_next_o = trans_instr(TrRrExpF7,     f6_state_q.rd, '0,            f6_state_q.rd);
        TransExpFma1State: id_insert_inst_next_o = trans_instr(TrFma1ExpF7,   f6_state_q.rd, '0,            f6_state_q.rd);
        TransExpFma2State: id_insert_inst_next_o = trans_instr(TrFma2ExpF7,   f6_state_q.rd, '0,            f6_state_q.rd);
        TransNrState:      id_insert_inst_next_o = trans_instr(TrNrF7,        f6_state_q.rd, '0,            f6_state_q.rs);
        default: begin
        end
      endcase
    end
  end

  always_comb begin
    f5_write_ctrl_d = make_write_ctrl(f5_state);
    f7_write_ctrl_d = make_write_ctrl(id_state);
  end

  always_comb begin
    f6_trans_ctrl_en_o = 1'b0;
    f6_trans_wmask_o = '0;
    f6_trans_thread_id_o = 1'b0;
    f8_trans_ctrl_en_o = 1'b0;
    f8_trans_fcsr_en_o = 1'b0;

    if (f6_write_ctrl_q.en && !f6_killed_i) begin
      f6_trans_ctrl_en_o = 1'b1;
      f6_trans_wmask_o = f6_write_ctrl_q.mask;
      f6_trans_thread_id_o = f6_write_ctrl_q.thread_id;
    end

    if (f8_write_ctrl_q.en && !f8_killed_i) begin
      f8_trans_ctrl_en_o = 1'b1;
      f8_trans_fcsr_en_o = f8_write_ctrl_q.fcsr_en;
    end
  end

  always_comb begin
    trans_scoreboard_next_d[0] = (slot_next == 0) ? make_ex_slot(states_d[6], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                 (slot_next == 1) ? make_ex_slot(states_d[0], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[0]);
    trans_scoreboard_next_d[1] = (slot_next == 1) ? make_slot_from_state(states_d[6]) :
                                 (slot_next == 2) ? make_ex_slot(states_d[1], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[1]);
    trans_scoreboard_next_d[2] = (slot_next == 2) ? make_slot_from_state(states_d[6]) :
                                 (slot_next == 3) ? make_ex_slot(states_d[2], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[2]);
    trans_scoreboard_next_d[3] = (slot_next == 3) ? make_slot_from_state(states_d[6]) :
                                 (slot_next == 4) ? make_ex_slot(states_d[3], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[3]);
    trans_scoreboard_next_d[4] = (slot_next == 4) ? make_slot_from_state(states_d[6]) :
                                 (slot_next == 5) ? make_ex_slot(states_d[4], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[4]);
    trans_scoreboard_next_d[5] = (slot_next == 5) ? make_slot_from_state(states_d[6]) :
                                 (slot_next == 6) ? make_ex_slot(states_d[5], id_valid_i, id_core_thread_id_i, id_instruction_i, f7_killed_i) :
                                                    make_slot_from_state(states_d[5]);
  end

endmodule

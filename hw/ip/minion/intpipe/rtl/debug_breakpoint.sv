// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// debug_breakpoint — Address breakpoint comparator.
//
// Pure combinational. Compares input addresses (PC or load/store addresses)
// with configured CSR trigger data. Supports address masking.
//
// TRIGGER_TYPE: 0 = program counter (fetch), 1 = data (load/store)
//
// Line-by-line translation of: debug_breakpoint (etcore-soc)

module debug_breakpoint
  import minion_pkg::*;
#(
  parameter int unsigned TriggerType = 0
) (
  // Breakpoint configuration
  input  minion_bp_ctrl_t [NrThreadsDefault-1:0]              control_ip,
  input  logic [NrThreadsDefault-1:0]                         in_debug_ip,
  input  logic [NrThreadsDefault-1:0][1:0]                    prv_ip,
  input  logic [NrThreadsDefault-1:0][VaSize-1:0]             tdata2_ip,

  // Signals from core that can trigger the breakpoint
  input  logic [VaSizeExt-1:0]                                address_ip,
  /* verilator lint_off UNUSEDSIGNAL */  // cmd_ip unused when TriggerType==0 (insn breakpoint)
  input  debug_bp_e                                           cmd_ip,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                                valid_ip,
  input  logic                                                thread_id_ip,

  // Outputs
  output logic                                                timing_op,
  output logic                                                xcpt_op,
  output logic                                                enter_debug_op,
  output logic                                                update_op
);

  localparam int unsigned MaskMax = 6;
  localparam int unsigned Insn = 0;
  localparam int unsigned Data = 1;

  logic prv_enable;
  logic match;
  logic enabled;
  logic control_match;

  always_comb begin
    prv_enable = (prv_ip[thread_id_ip] == PrvM && control_ip[thread_id_ip].m)
              || (prv_ip[thread_id_ip] == PrvS && control_ip[thread_id_ip].s)
              || (prv_ip[thread_id_ip] == PrvU && control_ip[thread_id_ip].u);
    control_match = control_ip[thread_id_ip].match;
  end

  // Enable logic depends on trigger type
  if (TriggerType == Data) begin : gen_enab_data
    assign enabled = !in_debug_ip[thread_id_ip] && valid_ip
                   && (control_ip[thread_id_ip].load && cmd_is_load(cmd_ip)
                    || control_ip[thread_id_ip].store && cmd_is_store(cmd_ip))
                   && prv_enable;
  end else begin : gen_enab_insn
    assign enabled = !in_debug_ip[thread_id_ip] && control_ip[thread_id_ip].execute
                   && valid_ip && prv_enable;
  end

  logic [MaskMax-1:0]    mask;
  logic [VaSizeExt-1:0]  masked_tdata2;
  logic [VaSizeExt-1:0]  masked_op;

  // MASK and compare
  always_comb begin
    masked_op = address_ip;
    masked_tdata2 = {tdata2_ip[thread_id_ip][VaSize-1], tdata2_ip[thread_id_ip]};

    // compute mask (used when match==1)
    casez (masked_tdata2[MaskMax-1:0])
      6'b?????0: mask = 6'b000001;
      6'b????01: mask = 6'b000011;
      6'b???011: mask = 6'b000111;
      6'b??0111: mask = 6'b001111;
      6'b?01111: mask = 6'b011111;
      default:   mask = 6'b111111;
    endcase

    // apply mask on operands
    if (control_match) begin
      masked_tdata2[MaskMax-1:0] |= mask;
      masked_op[MaskMax-1:0]     |= mask;
    end

    // compare
    match = masked_op == masked_tdata2;

    // generate outputs
    xcpt_op        = match && enabled && !control_ip[thread_id_ip].action;
    enter_debug_op = match && enabled &&  control_ip[thread_id_ip].action;
    update_op      = match && enabled;
  end

  // Timing output depends on trigger type
  if (TriggerType == Insn) begin : gen_timing_insn
    assign timing_op = 1'b0;
  end else begin : gen_timing_data
    assign timing_op = get_timing_before_or_after(cmd_ip);
  end

  // Helper functions

  function automatic logic cmd_is_load;
    input debug_bp_e cmd;
    case (cmd)
      DbgBpLoad:        cmd_is_load = 1'b1;
      DbgBpStore:       cmd_is_load = 1'b0;
      DbgBpTensorLoad:  cmd_is_load = 1'b1;
      DbgBpTensorStore: cmd_is_load = 1'b0;
      DbgBpAmo:         cmd_is_load = 1'b1;
      DbgBpCo:          cmd_is_load = 1'b1;
      default:           cmd_is_load = 1'b0;
    endcase
  endfunction

  function automatic logic cmd_is_store;
    input debug_bp_e cmd;
    case (cmd)
      DbgBpLoad:        cmd_is_store = 1'b0;
      DbgBpStore:       cmd_is_store = 1'b1;
      DbgBpTensorLoad:  cmd_is_store = 1'b0;
      DbgBpTensorStore: cmd_is_store = 1'b1;
      DbgBpAmo:         cmd_is_store = 1'b1;
      DbgBpCo:          cmd_is_store = 1'b0;
      default:           cmd_is_store = 1'b0;
    endcase
  endfunction

  function automatic logic get_timing_before_or_after;
    input debug_bp_e cmd;
    case (cmd)
      DbgBpLoad:        get_timing_before_or_after = 1'b0;
      DbgBpStore:       get_timing_before_or_after = 1'b0;
      DbgBpTensorLoad:  get_timing_before_or_after = 1'b1;
      DbgBpTensorStore: get_timing_before_or_after = 1'b1;
      DbgBpAmo:         get_timing_before_or_after = 1'b0;
      DbgBpCo:          get_timing_before_or_after = 1'b1;
      default:           get_timing_before_or_after = 1'b0;
    endcase
  endfunction

endmodule

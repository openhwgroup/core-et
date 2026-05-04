// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"


// Description: block comparing input addresses (both PC and load/store addresses)
//              with the configured CSR trigger.
//              It is purely combinational, so its outputs need to be registered
//
//              Outputs are:
//                          timing_op        => value to put in CSR tdata2
//                          xcpt_op          => generate and exception
//                          enter_debug_op   => action is to enter debug mode
//                          update_op         => OR of the previous 2, pulse saying timing_op can be updated and gsc progress
//
//              PARAMETERS:
//                          TRIGGER_TYPE: 0 corresponds to program counter, index 1 to ld/st

module debug_breakpoint #(
  parameter TRIGGER_TYPE = 0
) (
  // breakpoint configuration
  input                                      minion_bp_ctrl [`CORE_NR_THREADS-1:0] control_ip, // fields from csr tdata1
  input [`CORE_NR_THREADS-1:0]               in_debug_ip, // is thread in debug mode
  input [`CORE_NR_THREADS-1:0][1:0]          prv_ip, // current thread priviledge mode
  input [`CORE_NR_THREADS-1:0][`VA_SIZE-1:0] tdata2_ip, // data to compare to (from csr tdata2)

  // signals from core.. that can trigger the breakpoint
  input [`VA_SIZE_EXT-1:0]                   address_ip, // memory address
  input debug_bp_t                           cmd_ip, // memory command
  input                                      valid_ip, // address_ip is valid
  input                                      thread_id_ip, // thread id corresponding to the PC

  // outputs
  output logic                               timing_op, // timing bit in CSR tdata1
  output logic                               xcpt_op, // trigger generates an exception
  output logic                               enter_debug_op,  // trigger enters debug mode
  output logic                               update_op
);

  localparam MASKMAX = 6;
  localparam INSN = 0;
  localparam DATA = 1;

  logic                                       prv_enable;

  logic                                       match;
  logic                                       enabled;
  logic                                       control_match;

  always_comb begin
    prv_enable = ( prv_ip[thread_id_ip] == `CSR_PRV_M && control_ip[thread_id_ip].m ||
                   prv_ip[thread_id_ip] == `CSR_PRV_S && control_ip[thread_id_ip].s ||
                   prv_ip[thread_id_ip] == `CSR_PRV_U && control_ip[thread_id_ip].u);
    control_match = control_ip[thread_id_ip].match;
  end

  generate
    if (TRIGGER_TYPE == DATA) begin: ENAB
      assign enabled = !in_debug_ip[thread_id_ip] && valid_ip &&
                       ( control_ip[thread_id_ip].load &&  cmd_is_load(cmd_ip) ||
                         control_ip[thread_id_ip].store && cmd_is_store(cmd_ip) ) && prv_enable;
    end
    else begin: ENAB
      assign enabled = !in_debug_ip[thread_id_ip] && control_ip[thread_id_ip].execute && valid_ip &&  prv_enable;
    end
  endgenerate

  logic [MASKMAX-1:0]                         mask;
  logic [`VA_SIZE_EXT-1:0]                    masked_tdata2;
  logic [`VA_SIZE_EXT-1:0]                    masked_op;

  // MASK
  always_comb begin
    masked_op = address_ip;
    masked_tdata2 = { tdata2_ip[thread_id_ip][`VA_SIZE-1], tdata2_ip[thread_id_ip]};
    // compute mask (used when match==1)
    casez (masked_tdata2[MASKMAX-1:0])
      6'b?????0: mask = 6'b000001;
      6'b????01: mask = 6'b000011;
      6'b???011: mask = 6'b000111;
      6'b??0111: mask = 6'b001111;
      6'b?01111: mask = 6'b011111;
      default:   mask = 6'b111111;
    endcase // casez ( tdata2_ip[MASKMAX-1:0] )

    // apply mask on operands
    if (control_match) begin
      masked_tdata2[MASKMAX-1:0] |= mask;
      masked_op[MASKMAX-1:0] |= mask;
    end

    // compare
    match = masked_op == masked_tdata2;

    // and generate outputs
    xcpt_op = match && enabled && !control_ip[thread_id_ip].action;
    enter_debug_op = match && enabled && control_ip[thread_id_ip].action;
    update_op = match && enabled;
  end // always_comb

  generate
    if (TRIGGER_TYPE == INSN) begin: timing
      assign timing_op = 1'b0;
    end
    else begin: timing
      assign timing_op = get_timing_before_or_after(cmd_ip);
    end
  endgenerate

  function automatic logic cmd_is_load;
    input debug_bp_t cmd;
    begin
      case (cmd)
        debug_bp_load:         cmd_is_load = 1'b1;
        debug_bp_store:        cmd_is_load = 1'b0;
        debug_bp_tensor_load:  cmd_is_load = 1'b1;
        debug_bp_tensor_store: cmd_is_load = 1'b0;
        debug_bp_amo:          cmd_is_load = 1'b1;
        debug_bp_co:           cmd_is_load = 1'b1;
        default:               cmd_is_load = 1'b0;
      endcase
    end
  endfunction

  function automatic logic cmd_is_store;
    input debug_bp_t cmd;
    begin
      case (cmd)
        debug_bp_load:         cmd_is_store = 1'b0;
        debug_bp_store:        cmd_is_store = 1'b1;
        debug_bp_tensor_load:  cmd_is_store = 1'b0;
        debug_bp_tensor_store: cmd_is_store = 1'b1;
        debug_bp_amo:          cmd_is_store = 1'b1;
        debug_bp_co:           cmd_is_store = 1'b0;
        default:               cmd_is_store = 1'b0;
      endcase
    end
  endfunction

  function automatic logic get_timing_before_or_after;
    input debug_bp_t cmd;
    begin
      case (cmd)
        debug_bp_load:         get_timing_before_or_after = 1'b0;
        debug_bp_store:        get_timing_before_or_after = 1'b0;
        debug_bp_tensor_load:  get_timing_before_or_after = 1'b1;
        debug_bp_tensor_store: get_timing_before_or_after = 1'b1;
        debug_bp_amo:          get_timing_before_or_after = 1'b0;
        debug_bp_co:           get_timing_before_or_after = 1'b1;
        default:               get_timing_before_or_after = 1'b0;
      endcase // case (cmd)
    end
  endfunction

endmodule

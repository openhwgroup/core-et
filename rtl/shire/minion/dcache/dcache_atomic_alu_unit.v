// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_atomic_alu_unit (
  // System signals
  input  logic               clock,
  input  logic               reset,
  // Request input
  input  logic [2:0]         addr,
  input  dcache_cmd          cmd,
  input  logic [1:0]         typ,
  input  logic [`XREG_RANGE] lhs,
  input  logic [`XREG_RANGE] rhs,
  // Operation result
  output logic [`XREG_RANGE] out
);

////////////////////////////////////////////////////////////////////////////////
// Decoding based on operation
////////////////////////////////////////////////////////////////////////////////

  logic sgned;
  logic max;
  logic min;

  always_comb begin
    sgned = (cmd == dcache_cmd_XA_MIN) || (cmd == dcache_cmd_XA_MAX);
    max   = (cmd == dcache_cmd_XA_MAX) || (cmd == dcache_cmd_XA_MAXU);
    min   = (cmd == dcache_cmd_XA_MIN) || (cmd == dcache_cmd_XA_MINU);
  end

////////////////////////////////////////////////////////////////////////////////
// Operations
////////////////////////////////////////////////////////////////////////////////

  logic [`XREG_RANGE] adder_out;
  logic [`XREG_RANGE] adder_mask;
  logic [`XREG_RANGE] rhs_int;
  logic               word;
  logic               cmp_lhs;
  logic               cmp_rhs;
  logic               lt_lo;
  logic               lt_hi;
  logic               eq_hi;
  logic               lt;
  logic               less;

  always_comb begin
    // Extracts memory (rhs_int) operand based on size
    // This is done for stores, we replicate the data that we are about to
    // write in all the possible positions
    if (typ == 2'b00) begin
      rhs_int = {8{rhs[7:0]}};         // Replicate 8 LSB in case of 8b op
    end else if (typ == 2'b01) begin
      rhs_int = {4{rhs[15:0]}};        // Replicate 16 LSB in case of 16b op
    end else if (typ == 2'b10) begin
      rhs_int = {2{rhs[`WREG_RANGE]}}; // Replicate 32 LSB in case of 32b op
    end else begin
      rhs_int = rhs;
    end

    // Add operation
    adder_mask = {`XREG_SIZE{1'b1}} ^ (`ZX(`XREG_SIZE, addr[2]) << 31);
    adder_out = (lhs & adder_mask) + (rhs_int & adder_mask);

    // Compares
    word    = !typ[0];
    cmp_lhs = (word && !addr[2]) ? lhs[`WREG_MSB]     : lhs[`XREG_MSB];
    cmp_rhs = (word && !addr[2]) ? rhs_int[`WREG_MSB] : rhs_int[`XREG_MSB];
    lt_lo   = lhs[`WREG_RANGE]  <  rhs_int[`WREG_RANGE];
    lt_hi   = lhs[63:32] <  rhs_int[63:32];
    eq_hi   = lhs[63:32] == rhs_int[63:32];
    lt      = word ? (addr[2] ? lt_hi : lt_lo) : lt_hi || eq_hi && lt_lo;
    less    = (cmp_lhs == cmp_rhs) ? lt : sgned ? cmp_lhs : cmp_rhs;
  end

////////////////////////////////////////////////////////////////////////////////
// Selects the operation and generates masked output
////////////////////////////////////////////////////////////////////////////////

  logic [`XREG_RANGE] out_mux;    // Mux of the different unit outputs
  logic [7:0]         wmask_byte; // The write mask for stores at byte level

  always_comb begin
    // Selects which result is actual output. Stores go through else
    if (cmd == dcache_cmd_XA_ADD) begin
      out_mux = adder_out;
    end else if (cmd == dcache_cmd_XA_AND) begin
      out_mux = lhs & rhs_int;
    end else if (cmd == dcache_cmd_XA_OR) begin
      out_mux = lhs | rhs_int;
    end else if (cmd == dcache_cmd_XA_XOR) begin
      out_mux = lhs ^ rhs_int;
    end else begin
      out_mux = (less ? min : max) ? lhs : rhs_int;
    end

    // For stores, we need to keep the original bits for the data that we are
    // not writing (original data is in lhs) and we need to write the new bits
    // in the other data

    // First generate how many bytes are written
    if (typ == 2'b00) begin
      wmask_byte = 8'b0000_0001; // Byte
    end else if (typ == 2'b01) begin
      wmask_byte = 8'b0000_0011; // Half
    end else if (typ == 2'b10) begin
      wmask_byte = 8'b0000_1111; // Word
    end else begin
      wmask_byte = 8'b1111_1111; // DWord
    end

    // Shifts the mask based on address. As accesses are aligned, there's no
    // need to rotate and we can do shift
    wmask_byte = (wmask_byte << addr);
    // For all the bytes
    for (integer i = 0; i < 8; i++) begin
      // Select between original data and new data based on wmask
      out[i*8+:8] = (out_mux[i*8+:8] & {8{wmask_byte[i]}}) | (lhs[i*8+:8] & {8{~wmask_byte[i]}});
    end
  end

endmodule


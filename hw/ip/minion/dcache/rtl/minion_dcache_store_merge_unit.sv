// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache store merge unit.
//
// Reimplements `dcache_store_merge_unit` from the original CORE-ET minion.

module minion_dcache_store_merge_unit
  import minion_dcache_pkg::*;
(
  input  logic [4:0]            addr_i,
  input  dcache_type_e          typ_i,
  input  logic [DcachePsMaskSize-1:0] ps_mask_i,
  input  logic [DcacheDataSize-1:0]   orig_data_i,
  input  logic [DcacheDataSize-1:0]   store_data_i,
  input  logic                  enable_lo_i,
  input  logic                  enable_hi_i,
  output logic [DcacheDataSize-1:0]   merge_out_o
);

  logic [DcacheDataSize-1:0]  store_data_aligned;
  /* verilator lint_off UNUSED */  // Original lower-half rotate intermediates are preserved verbatim
  logic [DcacheDataSize-1:0]  store_data_aligned_lo;
  logic [DcacheDataBytes-1:0] wmask_byte_lo;
  logic [DcacheDataBytes-1:0] wmask_byte;
  logic [DcacheDataBytes-1:0] wmask_byte_orig;

  always_comb begin
    if (typ_i == DcTypePs) begin
      wmask_byte_orig = {{4{ps_mask_i[7]}}, {4{ps_mask_i[6]}}, {4{ps_mask_i[5]}}, {4{ps_mask_i[4]}},
                         {4{ps_mask_i[3]}}, {4{ps_mask_i[2]}}, {4{ps_mask_i[1]}}, {4{ps_mask_i[0]}}};
    end else if (dcache_type_is_byte(typ_i)) begin
      wmask_byte_orig = 32'h0000_0001;
    end else if (dcache_type_is_hword(typ_i)) begin
      wmask_byte_orig = 32'h0000_0003;
    end else if (dcache_type_is_word(typ_i)) begin
      wmask_byte_orig = 32'h0000_000f;
    end else begin
      wmask_byte_orig = 32'h0000_00ff;
    end

    {wmask_byte, wmask_byte_lo} = {wmask_byte_orig & {DcacheDataBytes{enable_hi_i}},
                                   wmask_byte_orig & {DcacheDataBytes{enable_lo_i}}} << addr_i;

    {store_data_aligned, store_data_aligned_lo} = {store_data_i, store_data_i} << {addr_i, 3'b000};

    merge_out_o = orig_data_i;
    for (int unsigned i = 0; i < DcacheDataBytes; i++) begin
      merge_out_o[i*8 +: 8] = ({8{ wmask_byte[i]}} & store_data_aligned[i*8 +: 8])
                            | ({8{~wmask_byte[i]}} & orig_data_i       [i*8 +: 8]);
    end
  end

endmodule

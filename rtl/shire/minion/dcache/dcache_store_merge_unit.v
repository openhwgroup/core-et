// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_store_merge_unit (
  // Request input
  input  logic [4:0]                      addr,
  input  dcache_type                      typ,
  input  logic [`DMEM_REQ_PS_MASK_SZ-1:0] ps_mask,
  input  logic [`DCACHE_DATA_RANGE]       orig_data,
  input  logic [`DCACHE_DATA_RANGE]       store_data,
  input  logic                            enable_lo,
  input  logic                            enable_hi,
  // Operation result
  output logic [`DCACHE_DATA_RANGE]       merge_out
);

////////////////////////////////////////////////////////////////////////////////
// Selects the operation and generates masked output
////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_DATA_RANGE]            store_data_aligned;    // Store data aligned to final position within data block
  logic [`DCACHE_DATA_RANGE]            store_data_aligned_lo; // Store data aligned to final position within data block
  logic [`DCACHE_DATA_NUM_BYTES_RANGE]  wmask_byte;            // The write mask for stores at byte level after alignment
  logic [`DCACHE_DATA_NUM_BYTES_RANGE]  wmask_byte_orig;       // The write mask for stores at byte level before alignment
  logic [`DCACHE_DATA_NUM_BYTES_RANGE]  wmask_byte_lo;         // The write mask for stores at byte level

  always_comb begin
    // For stores, we need to keep the original bits for the data that we are
    // not writing (original data is in lhs) and we need to write the new bits
    // in the other data

    // First generate how many bytes are written
    if (typ == dcache_type_PS) begin
      wmask_byte_orig = { {4{ps_mask[7]}}, {4{ps_mask[6]}}, {4{ps_mask[5]}}, {4{ps_mask[4]}},
                          {4{ps_mask[3]}}, {4{ps_mask[2]}}, {4{ps_mask[1]}}, {4{ps_mask[0]}} }; // PS
    end else if (dcache_type_is_byte(typ)) begin
      wmask_byte_orig = 32'b0000_0000_0000_0001; // Byte
    end else if (dcache_type_is_hword(typ)) begin
      wmask_byte_orig = 32'b0000_0000_0000_0011; // Half
    end else if (dcache_type_is_word(typ)) begin
      wmask_byte_orig = 32'b0000_0000_0000_1111; // Word
    end else begin
      wmask_byte_orig = 32'b0000_0000_1111_1111; // DWord
    end

    // Shifts the mask based on address.
    // In regular case enable_hi is not set and enable_lo is not set, so
    // a shift is performed
    // In case of misaligned access case 3 (different block, same cacheline),
    // we do a rotate to write bits in MSB and LSB
    // In case of second pass of a misaligned access case 1/2 (two blocks done
    // in two passes), then MSBs are cleared as we just write the LSBs
    { wmask_byte, wmask_byte_lo } = { wmask_byte_orig & {32{enable_hi}}, wmask_byte_orig & {32{enable_lo}} } << addr;

    // Aligns the store data according to the address
    { store_data_aligned, store_data_aligned_lo } = { store_data, store_data } << { addr, 3'b0 };

    // For all the bytes
    for (integer i = 0; i < `DCACHE_DATA_NUM_BYTES; i++) begin
      // Select between original data and new data based on wmask
      merge_out[i*8 +: 8] = ({8{ wmask_byte[i]}} & store_data_aligned[i*8 +: 8])
                          | ({8{~wmask_byte[i]}} & orig_data         [i*8 +: 8]);
    end
  end

endmodule


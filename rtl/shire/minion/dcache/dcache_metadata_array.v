// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// This module stores the meta data of the DCache
// Meta data is both the tag and coherency state of all the cachelines
module dcache_metadata_array (
  // System signals
  input  logic                               clock,
  input  logic                               reset,
  // Read request
  input  logic [`DCACHE_SET_RANGE]           s1_read_set,
  // Write request
  input  logic                               s1_write_valid,
  input  logic                               s1_write_valid_prev,
  input  dcache_meta_write_req               s1_write_req,
  // Read output data
  output logic [`DCACHE_WAYS-1:0]            s1_read_resp_valid,
  output dcache_meta_data [`DCACHE_WAYS-1:0] s1_read_resp,
  // Clear all the information
  input  logic                               cfg_clear_all,
  input  logic                               cfg_clear_low
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S1 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// We have valid bit for each entry, so we can easily invalidate an entry
////////////////////////////////////////////////////////////////////////////////

  logic                                     up_valid;      // Update valid flags
  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] s1_valid;      // Valid entries in the dcache
  logic [`DCACHE_SETS-1:0][`DCACHE_WAYS-1:0] s1_valid_next; // Valid entries in the dcache

  logic                                     s1_write_valid_prev_p2; // Latched phase 2 early write enable


  //         CLK    RST    EN        DOUT      DIN            DEF
  `RST_EN_FF(clock, reset, up_valid, s1_valid, s1_valid_next, {`DCACHE_SETS*`DCACHE_WAYS{1'b0}})

  // Create advanced valid signal for latch capture
  `LATCH_P2(clock, s1_write_valid_prev_p2, s1_write_valid_prev)

  genvar way;
  generate
    for (way = 0; way < `DCACHE_WAYS; way++) begin : tag_way
      logic                      s1_write_en;
      logic [`DCACHE_SET_RANGE]  s1_write_set;
      dcache_meta_data           s1_write_data;

      rf_latch_1r_1w #(
        .WIDTH           ( $bits(dcache_meta_data) ),
        .ENTRIES         ( `DCACHE_SETS            ),
        .LEVEL2_CLK_GATE ( 1                       )
      ) mem_array (
        // System signals
        .clk             ( clock                   ),
        .test_en         ( 1'b0                    ),
        // Read port
        .rd_addr_a       ( s1_read_set             ),
        .rd_data_a       ( s1_read_resp[way]       ),
        // Write port
        .wr_en_a         ( s1_write_en             ),
        .wr_addr_a       ( s1_write_set            ),
        .wr_data_a_en_1p ( s1_write_valid_prev_p2  ),
        .wr_data_a       ( s1_write_data           )
      );

      always_comb begin
        s1_write_en   =  (s1_write_valid && s1_write_req.way_en[way]);
        s1_write_set  = s1_write_req.set;
        s1_write_data = s1_write_req.data;
      end

    end
  endgenerate

////////////////////////////////////////////////////////////////////////////////
// Valid update
////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    s1_valid_next = s1_valid;

    // Sets valid
    if (cfg_clear_all) begin
      s1_valid_next = {`DCACHE_SETS*`DCACHE_WAYS{1'b0}};
    end else if (cfg_clear_low) begin
      for (integer i=0; i<`DCACHE_SETS_SCP_SPLIT; i++) begin
        s1_valid_next[i] = {(`DCACHE_WAYS){1'b0}};
      end
    end else if (s1_write_valid) begin
      for (integer w = 0; w < `DCACHE_WAYS; w++) begin
        if (s1_write_req.way_en[w]) begin
          s1_valid_next[s1_write_req.set][w] = (s1_write_req.data.coh_state != State_Invalid);
        end
      end
    end

    up_valid = cfg_clear_all | cfg_clear_low | s1_write_valid;
  end

////////////////////////////////////////////////////////////////////////////////
// Valid read port
////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    for (integer i = 0; i < `DCACHE_WAYS; i++) begin
      s1_read_resp_valid[i] = s1_valid[s1_read_set][i];
    end
  end

endmodule


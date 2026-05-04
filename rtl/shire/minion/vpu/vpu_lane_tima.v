// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_lane_tima (
  // System signals
  input         TE,
  input         clk,
  input         tima_clk_en,
  // F1 inputs
  input         f1_tima_mul_en,
  input         tima_in1_signed,
  input         tima_in2_signed,
  // F2 inputs
  input         f2_tima_en,
  input         f2_tima_in3_en,
  input         f2_tima_zap_opdc,
  // TEN A inputs
  input  [31:0] tena_rd_data,
  // TEN B inputs
  input         tenb_wr_en,
  input         tenb_wr_en_early,
  input  [1:0]  tenb_rd_addr,
  input  [1:0]  tenb_wr_addr,
  input  [31:0] tenb_wr_data,
  // TEN C inputs
  input         tenc_wr_en,
  input  [3:0]  tenc_rd_addr,
  input  [3:0]  tenc_wr_addr,
  // F3 outputs
  output [31:0] tenb_out,
  output [31:0] tima_out
);

logic [31:0] tenc_rd_data;
logic        tima_clock;


////////////////////////////////////////////////////////////////////////////////
// Tensor B register file
////////////////////////////////////////////////////////////////////////////////

vpu_tensorb_rf tenb_rf (
  // System signal
  .clock       ( clk              ),
  // Read ports
  .rd_addr     ( tenb_rd_addr     ),
  .rd_data     ( tenb_out         ),
  // Write ports
  .wr_en_early ( tenb_wr_en_early ),
  .wr_en       ( tenb_wr_en       ),
  .wr_addr     ( tenb_wr_addr     ),
  .wr_data     ( tenb_wr_data     )
);


///////////////////////////////////////////////////////////////////////////////
// TensorC Register File
///////////////////////////////////////////////////////////////////////////////

vpu_tensorc_rf tenc_rf (
  // System signal
  .clock       ( clk              ),
  // Read ports
  .rd_addr     ( tenc_rd_addr     ),
  .rd_data     ( tenc_rd_data     ),
  .rd_en       ( f2_tima_in3_en   ),
  // Write ports
  .wr_en       ( tenc_wr_en       ),
  .wr_addr     ( tenc_wr_addr     ),
  .wr_data     ( tima_out         )
);


///////////////////////////////////////////////////////////////////////////////
// tima clock gate
///////////////////////////////////////////////////////////////////////////////
et_clk_gate cgate_tima (
  .enclk ( tima_clock  ),
  .en    ( tima_clk_en ),
  .clk   ( clk         ),
  .te    ( 1'b0        )
);


///////////////////////////////////////////////////////////////////////////////
// tima top
///////////////////////////////////////////////////////////////////////////////

tima_top tima (
  // System signals
  .clock         ( tima_clock        ),
  // F1 inputs
  .f1_mul_en     ( f1_tima_mul_en    ),
  .f1_in1        ( tena_rd_data      ),
  .f1_in2        ( tenb_out          ),
  .f1_in1_signed ( tima_in1_signed   ),
  .f1_in2_signed ( tima_in2_signed   ),
  // F2 inputs
  .f3_in3        ( tenc_rd_data      ),
  .f2_zap_opd3   ( !f2_tima_zap_opdc ),
  .f2_tima_en    ( f2_tima_en        ),
  // F3 outputs
  .f3_out        ( tima_out          )
);


endmodule

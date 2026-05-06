// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off UNUSEDSIGNAL */  // Imported original rounding helper keeps the full adder sum bus even when the LSB is not observed.
module txfma_rnd_adder
#(
  parameter left_bit     = 32,
  parameter right_bit    = 0,
  parameter rnd_ctl_sz   = 5,
  parameter rnd_ps       = 1
)
(
  input  [left_bit:right_bit]          in,
  input  [rnd_ctl_sz-1:0]              rnd_ctl,
  input                                cin,
  output logic [left_bit-1:right_bit]  out
);

logic [left_bit:right_bit]     round_vector;
logic [left_bit:right_bit]     rnd_adder_sum;

  //round vector
generate
  if (rnd_ps == 1)
  begin : ROUND_VECTOR_PS_PH_INT
    always_comb begin
      round_vector = '0;

      round_vector[22:21] = rnd_ctl[4:3]; //ph hi
      round_vector[9:8]   = rnd_ctl[2:1]; //ps
      round_vector[1]     = rnd_ctl[0];   //int
    end
  end
endgenerate


///////////////////////////////////////////////////////////////////////////////
// round adder
///////////////////////////////////////////////////////////////////////////////
localparam rnd_adder_width = left_bit-right_bit+1;
txfma_adder
#(
  .Width(rnd_adder_width)
)
rnd_adder
(
  .a_i  (in),
  .b_i  (round_vector),
  .cin_i(cin),
  .sum_o(rnd_adder_sum)
);

assign out = rnd_adder_sum[32:1];

endmodule
/* verilator lint_on UNUSEDSIGNAL */

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// synopsys translate_off 

`include "soc.vh"

module txfma_top_fake
(
    // System signals
    input  logic        clock,
    input  logic        reset,
    input  logic        sleep,
    // Inputs
    input  logic        in_valid,
    input  vpu_input_t  in_data,
    input trans_fma_coefficients  trans_coefficients,    
    // Outputs
    output vpu_output_t out_data
);
  
import "DPI-C" pure function int dpi_TXFMA_EMU(input bit [31:0] src1, input bit [31:0] src2, input bit [31:0] src3, input bit [6:0] cmd, input bit [3:0] dtype, input bit [2:0] rm, output bit [5:0] out_flags);

//
// F2
//

vpu_input_t f2_bits;
logic       f2_valid;

`RST_FF(clock, reset, f2_valid, in_valid, 1'b0)
`FF    (clock,        f2_bits,  in_data)

//
// F3
//

vpu_input_t f3_bits;
logic       f3_valid;

`RST_FF(clock, reset, f3_valid, f2_valid, 1'b0)
`FF    (clock,        f3_bits,  f2_bits)

//
// F4
//

vpu_input_t f4_bits;
logic       f4_valid;

`RST_FF(clock, reset, f4_valid, f3_valid, 1'b0)
`FF    (clock,        f4_bits,  f3_bits)

//
// F5
//

vpu_input_t f5_bits;
logic       f5_valid;

`RST_FF(clock, reset, f5_valid, f4_valid, 1'b0)
`FF    (clock,        f5_bits,  f4_bits)


//
// F6
//

vpu_input_t f6_bits;
logic       f6_valid;

`RST_FF(clock, reset, f6_valid, f5_valid, 1'b0)
`FF    (clock,        f6_bits,  f5_bits)

//
// F7
//

vpu_input_t f7_bits;
logic       f7_valid;

`RST_FF(clock, reset, f7_valid, f6_valid, 1'b0)
`FF    (clock,        f7_bits,  f6_bits)

bit[5:0] out_flags;
always_ff @ (posedge clock)
begin
    if(f7_valid)
    begin
        out_data.data[31:0] <= dpi_TXFMA_EMU(f7_bits.in1[31:0], f7_bits.in2[31:0], f7_bits.in3[31:0], f7_bits.sigs.cmd, f7_bits.sigs.dtype, f7_bits.rm, out_flags);
        out_data.exc <= out_flags;
    end
end


endmodule

// synopsys translate_on 


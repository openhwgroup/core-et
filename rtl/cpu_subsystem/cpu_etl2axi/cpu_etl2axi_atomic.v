// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

//===================================================================
// This module implements the ETL2AXI bridge atomic functionality
//   Take in the read data (d0_rd_data) and operands (d0_operands)
//   and create updated data to write back into memory (d1_wr_data)
//===================================================================
module cpu_etl2axi_atomic
(
  // System signals
  input  logic                            clock,
  // Input atomic request
  input  etl2axi_amo_conf_t               d0_conf,
  input  logic [`CPU_AXI_ADDR_OFFSET-1:0] d0_addr_offset,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0]  d0_rd_data,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0]  d0_operands,
  input  logic                            d0_valid,
  // Output atomic result
  output logic [`CPU_AXI_DATA_WIDTH-1:0]  d1_wr_data
);

//===================================================================
// LOCAL PARAMETERS
//===================================================================
localparam cNAN_sw = 32'h7fc00000;

//===================================================================
// INTERNAL DECLARATIONS
//===================================================================
// D0 stage
logic [`SC_ATOMIC_SW_SEL_ADDR_RANGE] d0_addr_offset_sw;
logic [`SC_ATOMIC_DW_SEL_ADDR_RANGE] d0_addr_offset_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d0_rd_data_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d0_operand_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d0_expected_dw;
// D1 stage
etl2axi_amo_conf_t                   d1_conf;
logic [`SC_ATOMIC_SW_SEL_ADDR_RANGE] d1_addr_offset_sw;
logic [`SC_ATOMIC_DW_SEL_ADDR_RANGE] d1_addr_offset_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_rd_data_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_operand_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_expected_dw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_rd_data_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_operand_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_expected_sw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_add;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_xor;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_and;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_or;
logic                                d1_atomic_rd_data_gt_operand_usw;
logic                                d1_atomic_rd_data_gt_operand_ssw;
logic                                d1_atomic_rd_data_gt_operand_fsw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_imin_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_imax_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_umin_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_umax_sw;
logic                                d1_atomic_rd_data_is_qnan_sw;
logic                                d1_atomic_operand_is_qnan_sw;
logic                                d1_atomic_rd_data_is_snan_sw;
logic                                d1_atomic_operand_is_snan_sw;
logic                                d1_atomic_any_nan_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_any_nan_result_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_fmin_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_fmax_sw;
logic                                d1_atomic_rd_data_gt_operand_udw;
logic                                d1_atomic_rd_data_gt_operand_sdw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_imin_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_imax_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_umin_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_umax_dw;
logic                                d1_atomic_comp_sw;
logic [`SC_SW_DATA_SIZE-1:0]         d1_atomic_comp_swap_sw;
logic                                d1_atomic_comp_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_comp_swap_dw;
logic [`SC_DW_DATA_SIZE-1:0]         d1_atomic_alu;

//===================================================================
// D0 stage
//===================================================================

// Extract address offset aligned to a single-word
assign d0_addr_offset_sw = d0_addr_offset[`SC_ATOMIC_SW_SEL_ADDR_RANGE];
// Extract address offset aligned to a double-word
assign d0_addr_offset_dw = d0_addr_offset[`SC_ATOMIC_DW_SEL_ADDR_RANGE];

// Select read data from appropriate offset (address-aligned)
// NOTE: 32 LSBs are selected assuming single-word alignment
assign d0_rd_data_dw[`SC_SW_DATA_SIZE-1 : 0]                = d0_rd_data[`SC_SW_DATA_SIZE*d0_addr_offset_sw                    +: `SC_SW_DATA_SIZE];
// NOTE: 32 MSBs are selected assuming double-word alignment
assign d0_rd_data_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE] = d0_rd_data[`SC_DW_DATA_SIZE*d0_addr_offset_dw + `SC_SW_DATA_SIZE +: `SC_DW_DATA_SIZE-`SC_SW_DATA_SIZE];

// Extract operands from appropriate offset (ls-aligned)
assign d0_operand_dw  = d0_operands[0                +: `SC_DW_DATA_SIZE]; // First operand
assign d0_expected_dw = d0_operands[`SC_DW_DATA_SIZE +: `SC_DW_DATA_SIZE]; // Compare-swap "expected" operand

//===================================================================
// D1 stage
// - Perform atomic operations
//===================================================================
//     CLK    EN        DOUT               DIN
`EN_FF(clock, d0_valid, d1_conf,           d0_conf)
`EN_FF(clock, d0_valid, d1_addr_offset_sw, d0_addr_offset_sw)
`EN_FF(clock, d0_valid, d1_rd_data_dw,     d0_rd_data_dw)
`EN_FF(clock, d0_valid, d1_operand_dw,     d0_operand_dw)
`EN_FF(clock, d0_valid, d1_expected_dw,    d0_expected_dw)

// Extract double-word address offset
assign d1_addr_offset_dw = d1_addr_offset_sw[`SC_ATOMIC_DW_SEL_ADDR_RANGE];

// Extract single-word operands
assign d1_rd_data_sw  = d1_rd_data_dw[`SC_SW_DATA_SIZE-1:0];
assign d1_operand_sw  = d1_operand_dw[`SC_SW_DATA_SIZE-1:0];
assign d1_expected_sw = d1_expected_dw[`SC_SW_DATA_SIZE-1:0];

//-------------------------------------------------------------------
// Atomic operations
//-------------------------------------------------------------------
// Adder
// A single adder can be used for both single-word and double-word operations
assign d1_atomic_add = d1_rd_data_dw + d1_operand_dw;

// Logical operations
// A single logical operator can be used for both single-word and double-word operations
assign d1_atomic_xor = d1_rd_data_dw ^ d1_operand_dw;
assign d1_atomic_and = d1_rd_data_dw & d1_operand_dw;
assign d1_atomic_or  = d1_rd_data_dw | d1_operand_dw;

// min/max comparisons
// - NOTE: Ordering on each mode follows the following scheme:
//
//   usw  ssw fsw
//   111
//   110
//   101
//   100
//   011 011  011
//   010 010  010
//   001 001  001
//   000 000  000
//       111  100
//       110  101
//       101  110
//       100  111
//
// A single unsigned integer check is used and them adapted to each mode

// Single-word unsigned integer comparison
assign d1_atomic_rd_data_gt_operand_usw = d1_rd_data_sw > d1_operand_sw;
// Adapt for single-word signed integer comparison
// (invert result if signs are different)
assign d1_atomic_rd_data_gt_operand_ssw = d1_atomic_rd_data_gt_operand_usw ^ (d1_rd_data_sw[`SC_SW_DATA_SIZE-1] ^ d1_operand_sw[`SC_SW_DATA_SIZE-1]);
// Adapt for single-word floating point comparison
// (invert result if either of the operands are negative)
assign d1_atomic_rd_data_gt_operand_fsw = d1_atomic_rd_data_gt_operand_usw ^ (d1_rd_data_sw[`SC_SW_DATA_SIZE-1] | d1_operand_sw[`SC_SW_DATA_SIZE-1]);

// Single-word integer min/max functions
assign d1_atomic_imin_sw = !d1_atomic_rd_data_gt_operand_ssw ? d1_rd_data_sw : d1_operand_sw;
assign d1_atomic_imax_sw =  d1_atomic_rd_data_gt_operand_ssw ? d1_rd_data_sw : d1_operand_sw;
assign d1_atomic_umin_sw = !d1_atomic_rd_data_gt_operand_usw ? d1_rd_data_sw : d1_operand_sw;
assign d1_atomic_umax_sw =  d1_atomic_rd_data_gt_operand_usw ? d1_rd_data_sw : d1_operand_sw;

// Single-word float NaN calculation
//
// - This is done according to the RISC-V spec v2.2, “F” Standard Extension v2.0 (section 8.3),
//   which is compliant with the IEEE 754-2008 arithmetic standard
//
// - If at least one input is a signaling NaN, or if both inputs are quiet NaNs, the result is the canonical NaN. 
//   If one operand is a quiet NaN and the other is not a NaN, the result is the non-NaN operand
//   So in brief:
//   * if any of the inputs is a sNaN the result is the canonical NaN
//   * if both inputs are qNaN the result is the canonical NaN
//   * if one of the inputs is a qNaN and the other is not a NaN the result is the non-NaN number
//
// - NOTE: NaN format ({sign[31], exponent[30:23], significand[22:0]}):
//   * quiet     NaN (qNaN) => (value[30:23] == 8'hff) && (value[22] == 1'b1)
//   * signaling NaN (sNaN) => (value[30:23] == 8'hff) && (value[22] == 1'b0) && (value[21:0] != 0)
//   * canonical NaN (cNan) => 32'h7fc0_0000
//
assign d1_atomic_rd_data_is_qnan_sw = (d1_rd_data_sw[`CPU_FP32_EXP_RANGE] == '1) &&  d1_rd_data_sw[`CPU_FP32_MANT_MSB];
assign d1_atomic_operand_is_qnan_sw = (d1_operand_sw[`CPU_FP32_EXP_RANGE] == '1) &&  d1_operand_sw[`CPU_FP32_MANT_MSB];
assign d1_atomic_rd_data_is_snan_sw = (d1_rd_data_sw[`CPU_FP32_EXP_RANGE] == '1) && !d1_rd_data_sw[`CPU_FP32_MANT_MSB] && |d1_rd_data_sw[`CPU_FP32_MANT_MSB-1:`CPU_FP32_MANT_LSB];
assign d1_atomic_operand_is_snan_sw = (d1_operand_sw[`CPU_FP32_EXP_RANGE] == '1) && !d1_operand_sw[`CPU_FP32_MANT_MSB] && |d1_operand_sw[`CPU_FP32_MANT_MSB-1:`CPU_FP32_MANT_LSB];
assign d1_atomic_any_nan_sw         = |{d1_atomic_rd_data_is_qnan_sw, d1_atomic_operand_is_qnan_sw, d1_atomic_rd_data_is_snan_sw, d1_atomic_operand_is_snan_sw};
assign d1_atomic_any_nan_result_sw  = (d1_atomic_rd_data_is_snan_sw || d1_atomic_operand_is_snan_sw ||         
                                       (d1_atomic_rd_data_is_qnan_sw && d1_atomic_operand_is_qnan_sw)) ? cNAN_sw
                                    : d1_atomic_rd_data_is_qnan_sw                                     ? d1_operand_sw
                                    :                                                                    d1_rd_data_sw; // d1_atomic_operand_is_qnan_sw

// Single-word float min/max functions
assign d1_atomic_fmin_sw = d1_atomic_any_nan_sw              ? d1_atomic_any_nan_result_sw
                         : !d1_atomic_rd_data_gt_operand_fsw ? d1_rd_data_sw : d1_operand_sw;
assign d1_atomic_fmax_sw = d1_atomic_any_nan_sw              ? d1_atomic_any_nan_result_sw
                         :  d1_atomic_rd_data_gt_operand_fsw ? d1_rd_data_sw : d1_operand_sw; 

// For double-word unsigned integer comparison, compare only upper half and reuse single-word comparison
assign d1_atomic_rd_data_gt_operand_udw = (d1_rd_data_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE] > d1_operand_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE])
                                       || ((d1_rd_data_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE] == d1_operand_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE]) &&
                                           d1_atomic_rd_data_gt_operand_usw);
// Adpat for double-word signed integer comparison
// (invert result if signs are different)
assign d1_atomic_rd_data_gt_operand_sdw = d1_atomic_rd_data_gt_operand_udw ^ (d1_rd_data_dw[`SC_DW_DATA_SIZE-1] ^ d1_operand_dw[`SC_DW_DATA_SIZE-1]);

// Double-word min/max functions
assign d1_atomic_imin_dw = !d1_atomic_rd_data_gt_operand_sdw ? d1_rd_data_dw : d1_operand_dw;
assign d1_atomic_imax_dw =  d1_atomic_rd_data_gt_operand_sdw ? d1_rd_data_dw : d1_operand_dw;
assign d1_atomic_umin_dw = !d1_atomic_rd_data_gt_operand_udw ? d1_rd_data_dw : d1_operand_dw;
assign d1_atomic_umax_dw =  d1_atomic_rd_data_gt_operand_udw ? d1_rd_data_dw : d1_operand_dw;

// Compare-swap
assign d1_atomic_comp_sw      = d1_rd_data_sw == d1_expected_sw;
assign d1_atomic_comp_swap_sw = d1_atomic_comp_sw ? d1_operand_sw : d1_rd_data_sw;
// For double-word comparison, compare only upper half and reuse singled-word comparison
assign d1_atomic_comp_dw      = (d1_rd_data_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE] == d1_expected_dw[`SC_DW_DATA_SIZE-1 : `SC_SW_DATA_SIZE]) && d1_atomic_comp_sw;
assign d1_atomic_comp_swap_dw = d1_atomic_comp_dw ? d1_operand_dw : d1_rd_data_dw;

//-------------------------------------------------------------------
// Operation selection and assignment to output write data (address aligned)
//-------------------------------------------------------------------
always_comb begin
  case (d1_conf.op)
    // Swap read data with the operand
    ET_LINK_AMO_Swap : d1_atomic_alu = d1_operand_dw;

    // A single adder is used for both single-word and double-word operations
    ET_LINK_AMO_Add  : d1_atomic_alu = d1_atomic_add;

    // A single logical operator is used for both single-word and double-word operations
    ET_LINK_AMO_Xor  : d1_atomic_alu = d1_atomic_xor;
    ET_LINK_AMO_And  : d1_atomic_alu = d1_atomic_and;
    ET_LINK_AMO_Or   : d1_atomic_alu = d1_atomic_or;

    // Comparisons: single-word result is muxed into the lower half of the double-word result to reduce muxing
    ET_LINK_AMO_Min:
    begin
      d1_atomic_alu = d1_atomic_imin_dw;
      
      if (d1_conf.size == ETL2AXI_TT_AMOSIZE_SWord)
        d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_imin_sw;
    end

    ET_LINK_AMO_Max:
    begin
      d1_atomic_alu = d1_atomic_imax_dw;
      
      if (d1_conf.size == ETL2AXI_TT_AMOSIZE_SWord)
        d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_imax_sw;
    end

    ET_LINK_AMO_MinU:
    begin
      d1_atomic_alu = d1_atomic_umin_dw;
      
      if (d1_conf.size == ETL2AXI_TT_AMOSIZE_SWord)
        d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_umin_sw;
    end

    ET_LINK_AMO_MaxU:
    begin
      d1_atomic_alu = d1_atomic_umax_dw;
      
      if (d1_conf.size == ETL2AXI_TT_AMOSIZE_SWord)
        d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_umax_sw;
    end

    // Float comparisons are always single-word
    ET_LINK_AMO_Min_PS:
    begin
      d1_atomic_alu = d1_rd_data_dw; // Fill in upper half with previous value (just to reduce muxing)
      d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_fmin_sw; 
    end

    ET_LINK_AMO_Max_PS:
    begin
      d1_atomic_alu = d1_rd_data_dw; // Fill in upper half with previous value (just to reduce muxing)
      d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_fmax_sw; 
    end

    ET_LINK_AMO_Cmp_Swap:
    begin
      d1_atomic_alu = d1_atomic_comp_swap_dw;
      
      if (d1_conf.size == ETL2AXI_TT_AMOSIZE_SWord)
        d1_atomic_alu[`SC_SW_DATA_SIZE-1:0] = d1_atomic_comp_swap_sw;
    end

    default : d1_atomic_alu = d1_rd_data_dw; // Maintain previous value
  endcase
end

// Align result according to address offset
always_comb begin
  d1_wr_data = '0;
  if (d1_conf.size == ETL2AXI_TT_AMOSIZE_DWord)
    // Double-word result is aligned assuming double-word alignment
    d1_wr_data[`SC_DW_DATA_SIZE*d1_addr_offset_dw +: `SC_DW_DATA_SIZE] = d1_atomic_alu;
  else
    // Single-word result is aligned assuming single-word alignment
    d1_wr_data[`SC_SW_DATA_SIZE*d1_addr_offset_sw +: `SC_SW_DATA_SIZE] = d1_atomic_alu[`SC_SW_DATA_SIZE-1:0];
end

endmodule

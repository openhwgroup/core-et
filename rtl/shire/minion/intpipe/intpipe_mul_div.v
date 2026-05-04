// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module intpipe_mul_div (
  // System signals
  input  logic               clock,
  input  logic               reset,
  // Request port
  output logic               req_ready,
  input  logic               req_valid,
  input  core_alu_func       req_fn,
  input  logic               req_dw,
  input  logic [`XREG_RANGE] req_in1,
  input  logic [`XREG_RANGE] req_in2,
  input  minion_reg_dest     req_dest,
  input  logic               kill,
  // Response port
  input  logic               resp_ready,
  output logic               resp_valid,
  output logic               resp_valid_early,
  output logic [`XREG_RANGE] resp_data,
  output minion_reg_dest     resp_dest
);

  ////////////////////////////////////////////////////////////////////////////////
  // Unit FSM state and input sampling.
  ////////////////////////////////////////////////////////////////////////////////

  typedef enum logic [2:0] {
    MD_Ready,         // Mult Div unit is available
    MD_Neg_Input,
    MD_Busy,
    MD_Move_Rem,
    MD_Neg_Output,
    MD_Done
  } MD_state;

  typedef struct packed {
    logic           dw;      // 32b (0) or 64b (1) operation
    minion_reg_dest dest;    // Register to write result
    logic           is_mul;  // Doing a multiply
    logic           is_hi;   // High result
  } MD_req;

  MD_state    state, state_next;     // State of the execution unit
  MD_req      req, req_next;         // Request unit is working on
  logic [6:0] count, count_next;     // Counts how many steps we have done
  logic       new_req;               // Sample new request
  logic       neg_out, neg_out_next; // Stores if the divide results have to be negated
  logic       lhs_sign;              // Left hand operand has sign set
  logic       rhs_sign;              // Right hand operand has sign set
  logic       mul_early_out;         // Mul can do an early out (only for operations accessing 64LSB
  logic       clock_enable;          // Enable the clocks in the unit

  //         CLK    RST    EN            DOUT   DIN         DEF
  `RST_EN_FF(clock, reset, clock_enable, state, state_next, MD_Ready)
  `EN_FF    (clock,        new_req,      req,   req_next)

  // FSM transition
  // FSM transition
  always_comb begin
    state_next = state;
    // Ready
    if((state == MD_Ready) && req_valid)
      state_next = (lhs_sign || rhs_sign && !req_next.is_mul) ? MD_Neg_Input : MD_Busy;
    // Neg Input
    else if(state == MD_Neg_Input)
      state_next = MD_Busy;
    // Busy
    else if(state == MD_Busy) begin
      // Doing Mul and early out or done
      if(req.is_mul && (mul_early_out || (count == 7)))
        state_next = req.is_hi ? MD_Move_Rem : MD_Done;
      // Doing divide and all iterations done
      else if(!req.is_mul && (count == `XREG_SIZE))
        state_next = req.is_hi ? MD_Move_Rem
            : neg_out   ? MD_Neg_Output
            :             MD_Done;
    end else if(state == MD_Move_Rem)
      // Move Remainder
      state_next = neg_out ? MD_Neg_Output : MD_Done;
    // Negate output
    else if(state == MD_Neg_Output)
      state_next = MD_Done;
    // Done
    else if((state == MD_Done) && resp_ready)
      state_next = MD_Ready;

    // Kill
    if(kill)
      state_next = MD_Ready;

    clock_enable = (state != MD_Ready) || req_valid;
  end

  // FSM outputs
  logic [`XREG_SIZE*2+1:0] remainder, remainder_next; // Remainder
  logic [`XREG_SIZE:0]     divisor, divisor_next;     // Divisor
  logic                    remainder_get_negated;     // Remainder should get its negated value
  logic                    remainder_get_msb;         // Remainder should get its MSB
  logic                    remainder_get_mul;         // Remainder should get the output of the mul datapath
  logic                    remainder_get_div;         // Remainder should get the output of the div datapath
  logic                    divisor_get_sub;           // Divisor should get subtractor value

  always_comb begin
    remainder_get_negated = 1'b0;
    remainder_get_msb     = 1'b0;
    remainder_get_mul     = 1'b0;
    remainder_get_div     = 1'b0;
    divisor_get_sub       = 1'b0;

    // Ready
    req_ready = (state == MD_Ready);
    // Neg input
    if(state == MD_Neg_Input) begin
      if(remainder[`XREG_MSB] || req.is_mul)
        remainder_get_negated = 1'b1;
      if(divisor[`XREG_MSB] || req.is_mul)
        divisor_get_sub = 1'b1;
    end else if(state == MD_Busy) begin
      // Busy
      remainder_get_mul =  req.is_mul;
      remainder_get_div = !req.is_mul;
    end else if(state == MD_Move_Rem)
      // Move Remainder
      remainder_get_msb = 1'b1;
    // Negate output
    else if(state == MD_Neg_Output)
      remainder_get_negated = 1'b1;

    // Done
    resp_valid       = (state == MD_Done);
    resp_valid_early = (state_next == MD_Done);
    resp_dest        = req.dest;
    resp_data        = req.dw ? remainder[`XREG_RANGE] : { {`WREG_SIZE{remainder[`WREG_MSB]}}, remainder[`WREG_RANGE] }; // Sends 32b sign extend or 64b depending on size
  end

  // Gets new request
  logic [`XREG_RANGE] lhs_in;        // Left hand source input data after extension for sign
  logic [`XREG_RANGE] rhs_in;        // Right hand source input data after extension for sign
  logic               lhs_is_signed; // Left hand operand is signed
  logic               rhs_is_signed; // Right hand operand is signed

  always_comb begin
    // Get some info of the input bits
    // Left/Right hand source is signed
    lhs_is_signed =  (req_fn == core_alu_func_DIV)
        || (req_fn == core_alu_func_REM)
        || (req_fn == core_alu_func_MULH)
        || (req_fn == core_alu_func_MULHSU);
    rhs_is_signed =  (req_fn == core_alu_func_DIV)
        || (req_fn == core_alu_func_REM)
        || (req_fn == core_alu_func_MULH);

    // Sign of the left/right hand source
    lhs_sign = lhs_is_signed && (req_dw ? req_in1[`XREG_MSB] : req_in1[`WREG_MSB]);
    rhs_sign = rhs_is_signed && (req_dw ? req_in2[`XREG_MSB] : req_in2[`WREG_MSB]);
    // Bits of the left/right hand source
    lhs_in = req_dw ? req_in1                                           // 64b for 64b operation
        :          { {`WREG_SIZE{lhs_sign}}, req_in1[`WREG_RANGE] }; // Sign extend for 32b operation
    rhs_in = req_dw ? req_in2                                           // 64b for 64b operation
        :          { {`WREG_SIZE{rhs_sign}}, req_in2[`WREG_RANGE] }; // Sign extend for 32b operation

    new_req = req_ready && req_valid;
    // Packs the request from core
    req_next.dw      = req_dw;
    req_next.dest    = req_dest;
    req_next.is_mul  =  (req_fn == core_alu_func_MUL)
        || (req_fn == core_alu_func_MULH)
        || (req_fn == core_alu_func_MULHU)
        || (req_fn == core_alu_func_MULHSU);
    req_next.is_hi   =  (req_fn == core_alu_func_REM)
        || (req_fn == core_alu_func_REMU)
        || (req_fn == core_alu_func_MULH)
        || (req_fn == core_alu_func_MULHU)
        || (req_fn == core_alu_func_MULHSU);
  end

////////////////////////////////////////////////////////////////////////////////
// This is the Mul and Divide datapath. We are doing 8bits per cycle for the
// mul operations and 1bit per cycle (radix-2) for the divides.
////////////////////////////////////////////////////////////////////////////////

logic [`XREG_SIZE:0] subtractor;          // Subtractor for the divide
logic [`XREG_RANGE]  negated_remainder;   // Negated remainder
logic [5:0]          div_early_out_shift; // Shift amount for early out
logic                div_sub_is_neg;      // Divide subtract is negative
logic                div_early_out;       // Early out for the divisor

//     CLK    EN            DOUT       DIN
`EN_FF(clock, clock_enable, count,     count_next)
`EN_FF(clock, clock_enable, remainder, remainder_next)
`EN_FF(clock, clock_enable, divisor,   divisor_next)
`EN_FF(clock, clock_enable, neg_out,   neg_out_next)

  always_comb begin
    // Negates the remainder
    negated_remainder = -remainder[`XREG_RANGE];

    // Count
    if     (div_early_out)    count_next = div_early_out_shift; // Update to new count for divisor early out
    else if(state == MD_Busy) count_next = count + 7'b1;        // Increment count while busy
    else                      count_next = 7'b0;                // Set to 0 otherwise (needed for new req)

    // Divisor
    if     (new_req)         divisor_next = { rhs_sign, rhs_in };
    else if(divisor_get_sub) divisor_next = subtractor;
    else                     divisor_next = divisor;

    // Recomputes the divide negation of result
    if     (new_req)                                                             neg_out_next = !req_next.is_mul && (req.is_hi ? lhs_sign : (lhs_sign != rhs_sign));
    else if((state == MD_Busy) && (count == 0) && !div_sub_is_neg && !req.is_hi) neg_out_next = 1'b0;
    else                                                                         neg_out_next = neg_out;
  end

// Multiplier datapath. The multiplier goes as follows. The multiplicand is
// of 64b and is stored in the divisor FF. The multiplier is stored in the LSB
// of the remainder, and only 8b are used on each pass. The accumulated
// results are stored in the MSB of the remainder FF.
// The mul does a 64bx8b multiply and accumulates the result to the previous
// passes. The results are stored on the upper 72b of the remainder and the
// multiplicand is shift right 8b on the remainder as well. This way, on every
// pass the accumulated result moves 8b to the right on the remainder. This is
// a nice approach in the sense that we do the multiply and add always from
// the same positions and we don't need to select from different remainder
// bits where to accumulate and multiply
logic [129:0] mul_remainder_next; // Remainder value for mul operations
logic [128:0] mul_req_next;       // Next accumulate + multiplier
logic [128:0] mul_reg;            // Accumulate + multiplier
logic [72:0]  mul_accum;          // Accumulate value for multiplier
logic [72:0]  mul_prod;           // Partial Product
logic [64:0]  mul_mpcand;         // Multiplicand
logic [63:0]  mul_mplier;         // Multiplier
logic [63:0]  mul_early_out_res;  // Result for the early out
logic [7:0]   mul_early_check;    // Which bytes we need to check

  always_comb begin
    // Multiply + add of the partial product
    mul_reg      = { remainder[129:65], remainder[63:0] };
    mul_mplier   = mul_reg[63:0];
    mul_accum    = { {8{mul_reg[128]}}, mul_reg[128:64] };
    mul_mpcand   = divisor;
    mul_prod     = mul_mplier[7:0] * mul_mpcand + mul_accum;
    mul_req_next = { mul_prod, mul_mplier[63:8] };

    // Gets which bytes to check for early out
    mul_early_check = 8'b1111_1111 >> count;
    mul_early_out = (count != 7) && (count != 0) && !req.is_hi;
    for(integer unsigned i = 0; i < 7; i++)
      mul_early_out &= (mul_mplier[8*i+:8] == 8'b0);
    // As results are not in the LSB bits for early out, we need to shift them
    // for the missing passes
    mul_early_out_res = mul_reg >> (64 - (count * 8));
    mul_remainder_next[129:64] = { mul_req_next[128:64], 1'b0 };
    mul_remainder_next[63:0]   = mul_early_out ? mul_early_out_res : mul_req_next[63:0];
  end

// Divide datapath
logic [129:0] div_remainder_next; // Remainder value for div operations
logic [5:0]   div_early_out_pos;  // Position to start computation for early out
logic [5:0]   div_divisor_msb;    // First bit set of divisor
logic [5:0]   div_dividend_msb;   // First bit set of dividend
logic         div_early_out_zero; // Division result is 0

  always_comb begin
    // Subtracts the divisor to the reminder
    subtractor = remainder[128:64] - divisor[64:0];

    // Checks if the subtract is negative
    div_sub_is_neg = subtractor[64];

    div_remainder_next[129:65] = div_sub_is_neg ? remainder[127:64] : subtractor[63:0];
    div_remainder_next[64:0]   = { remainder[63:0], !div_sub_is_neg };

    // Computes the early out
    // Gets the first bit set for the divisor
    div_divisor_msb = 0;
    for(integer unsigned i = 1; i < 64; i++)
      if(divisor[i]) div_divisor_msb = i;
    // Gets the first bit set for the dividend
    div_dividend_msb = 0;
    for(integer unsigned i = 1; i < 64; i++)
      if(remainder[i]) div_dividend_msb = i;
    // Computes early out cases (result is zero and simply skip useless
    // computation)
    div_early_out_pos = 6'd63 + div_divisor_msb - div_dividend_msb;
    div_early_out_zero = div_divisor_msb > div_dividend_msb;
    // Checks early out condition (div_sub_is_neg is for not div by 0)
    // Check on Busy state to guarantee that negatives have been converted to
    // positives so we correctly early out
    div_early_out = (state == MD_Busy) && !req.is_mul && (count == 0) && div_sub_is_neg && (div_early_out_pos > 0 || div_early_out_zero);
    div_early_out_shift = div_early_out_zero ? 63 : div_early_out_pos;
    if(div_early_out)
      div_remainder_next = (remainder[63:0] << div_early_out_shift);
  end

// This is the datapath of remainder next, which is controlled by FSM, div
// pipeline and mul pipeline
  always_comb begin
    if     (remainder_get_negated) remainder_next = `ZX($bits(remainder_next), negated_remainder);
    else if(remainder_get_msb)     remainder_next = remainder[128:65];
    else if(new_req)               remainder_next = lhs_in;
    else if(remainder_get_mul)     remainder_next = mul_remainder_next;
    else if(remainder_get_div)     remainder_next = div_remainder_next;
    else                           remainder_next = remainder;
  end

endmodule


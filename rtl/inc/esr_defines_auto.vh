// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _ESR_DEFINES_AUTO_VH_
`define _ESR_DEFINES_AUTO_VH_


/***********************************************************************
 * macros for signal declarations
 ***********************************************************************/
`define ESR_DECLARATION(NAME, WIDTH) \
    logic [WIDTH-1:0] reg_``NAME, reg_``NAME``_next; \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define ESR_DECLARATION_DEC_ONLY(NAME) \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define ESR_DECLARATION_WITH_TYPE(NAME, TYPE) \
    TYPE reg_``NAME, reg_``NAME``_pre, reg_``NAME``_next; \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define ESR_DECLARATION_LATCH(NAME, WIDTH) \
 `ESR_DECLARATION(NAME, WIDTH) \
  logic  reg_``NAME``_latch_clk_en; \
  logic  reg_``NAME``_latch_clk;

`define ESR_DECLARATION_LATCH_WITH_TYPE(NAME, TYPE) \
  `ESR_DECLARATION_WITH_TYPE(NAME, TYPE) \
  logic  reg_``NAME``_latch_clk_en; \
  logic  reg_``NAME``_latch_clk;

`define ESR_LATCH_RESET_DECLARATION(NAME)                      \
logic NAME``_reg;                                              \
rst_repeat rst_repeat_``NAME (.reset_out(NAME``_reg),          \
                              .reset_in(NAME),                 \
                              .clock(clock),                   \
                              .dft__reset_byp(dft__reset_byp), \
                              .dft__reset(dft__reset) );

`define ESR_LATCH_P1_WDATA_DECLARATION                                                                \
   /*latched write data*/                                                                             \
   logic                                       s0_wen_p2;                                             \
   logic                                       clk_wdata_n;                                           \
   logic  [`ESR_APB_D_WIDTH-1:0]               apb_pwdata_1p;                                         \
    /*prepare to latch write data*/                                                                   \
   `LATCH_P2(clock, s0_wen_p2, s0_en && apb_pwrite)                                                   \
   et_clk_gate_n cbuf_wr_data_a(.enclk_n(clk_wdata_n), .en_1p(s0_wen_p2), .clk(clock), .te(test_en)); \
   always @(clk_wdata_n or apb_pwdata)                                                                \
     if (clk_wdata_n == 1'b0) apb_pwdata_1p <= apb_pwdata;


/***********************************************************************
 * macros for decoding and reading
 ***********************************************************************/

`define ESR_DECODE_DEFAULT(NAME) \
        reg_``NAME``_en_next = 1'b0;

`define ESR_DECODE_CASE( AD, NAME, READ_V) \
         AD: begin \
            s0_addr_valid = 1'b1; \
            reg_``NAME``_en_next = apb_pwrite; \
            s0_rdata = READ_V; \
         end 

`define ESR_DECODE_CASE_RD_CONSTANT(AD, NAME, READ_V) `ESR_DECODE_CASE( AD, NAME, READ_V)


`define ESR_DECODE_CASE_SHADOW( AD, NAME, READ_V, READ_F) \
         AD: begin \
            s0_addr_valid = 1'b1; \
            reg_``NAME``_en_next = apb_pwrite; \
            s0_rdata = READ_F(READ_V[mem_thread_id]); \
         end 

`define ESR_DECODE_CASE_RO(AD, READ_V) \
         AD: begin \
            s0_addr_valid = 1'b1; \
            s0_rdata = READ_V; \
         end

`define ESR_DECODE_SEQ(NAME)  \
      `RST_EN_FF(clock, reset_c, s0_en || s1_en, reg_``NAME``_en, reg_``NAME``_en_next, 1'b0)
   

/***********************************************************************
 * macros for core writes
 ***********************************************************************/
`define ESR_WRITE_COMB_DEFAULT(NAME) reg_``NAME``_next = reg_``NAME;

`define ESR_WRITE_LATCH_CLK_EN_ZERO(NAME) reg_``NAME``_latch_clk_en = '0;

`define ESR_WRITE_COMB_DEFAULT_CNT(NAME, MSB, LSB, UP) \
      reg_``NAME``_next[LSB] = UP ? reg_``NAME``[LSB] + 1'b1 : reg_``NAME[LSB]; \
      reg_``NAME``_next[MSB] = UP && &reg_``NAME``[LSB]? reg_``NAME[MSB] + 1'b1 : reg_``NAME[MSB];


`define ESR_WRITE_COMB(NAME, WDATA) if ( reg_``NAME``_en) reg_``NAME``_next = WDATA;

`define ESR_WRITE_COMB_SHADOW(NAME, WDATA, DST, WFUNC) \
         if ( reg_``NAME``_en) reg_``DST``_next = WFUNC(reg_``DST, WDATA);


`define ESR_WRITE_COMB_LATCH(NAME, WDATA) \
  reg_``NAME``_next = WDATA; \
  reg_``NAME``_latch_clk_en |= reg_``NAME``_en;

`define ESR_WRITE_COMB_LATCH_SHADOW(NAME, WDATA, DST, WFUNC) \
   reg_``DST``_next = WFUNC(reg_``DST, WDATA); \
   reg_``DST``_latch_clk_en = reg_``NAME``_en;


`define ESR_WRITE_SEQ(CLOCK, NAME, NAME_NEXT) `FF(CLOCK, reg_``NAME, reg_``NAME_NEXT)

`define ESR_WRITE_SEQ_RST(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_c, reg_``NAME, reg_``NAME_NEXT, RESETVAL)
`define ESR_WRITE_SEQ_RST_ND(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_w, reg_``NAME, reg_``NAME_NEXT, RESETVAL)
`define ESR_WRITE_SEQ_RST_D(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_d, reg_``NAME, reg_``NAME_NEXT, RESETVAL)

`define ESR_WRITE_LATCH(CLOCK, NAME, NAME_DST, NAME_NEXT) \
   esr_clk_gate #(.EDGE_(1)) cbuf_rf_wr_``NAME(.clk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .eclk(CLOCK), .testmode(1'b0 /*disable test_en to avoid current spike*/));   \
always @(reg_``NAME``_latch_clk or reg_``NAME_NEXT)  \
  if (reg_``NAME``_latch_clk) begin \
      reg_``NAME_DST <= reg_``NAME_NEXT; \
  end

`define ESR_WRITE_LATCH_RST(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   esr_clk_gate #(.EDGE_(1)) cbuf_rf_wr_``NAME(.clk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .eclk(CLOCK), .testmode(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_c_reg" */ \
  always @(reset_c_reg or reg_``NAME``_latch_clk or reg_``NAME_NEXT) \
    if (reset_c_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end


`define ESR_WRITE_LATCH_RST_ND(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   esr_clk_gate #(.EDGE_(1)) cbuf_rf_wr_``NAME(.clk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .eclk(CLOCK), .testmode(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_w_reg" */ \
  always @(reset_w_reg or reg_``NAME``_latch_clk or reg_``NAME_NEXT) \
    if (reset_w_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end

`define ESR_WRITE_LATCH_RST_D(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   esr_clk_gate #(.EDGE_(1)) cbuf_rf_wr_``NAME(.clk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .eclk(CLOCK), .testmode(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_d_reg" */ \
  always @(reset_d_reg or reg_``NAME``_latch_clk or reg_``NAME_NEXT)  \
    if (reset_d_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end


   
`define ESR_WRITE_TYPE_WITH_RESERVED(NAME, FUNC) \
   always_comb reg_``NAME = FUNC(reg_``NAME``_pre);
   
   

           

`endif


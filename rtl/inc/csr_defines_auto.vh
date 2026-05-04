// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _CSR_DEFINES_AUTO_
 `define _CSR_DEFINES_AUTO_

/***********************************************************************
 * macros for signal declarations
 ***********************************************************************/
`define CSR_DECLARATION_SHARED(NAME, WIDTH) \
    logic [WIDTH-1:0] reg_``NAME, reg_``NAME``_next; \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_NON_SHARED(NAME, WIDTH) \
    logic [`CORE_NR_THREADS-1:0][WIDTH-1:0] reg_``NAME, reg_``NAME``_next; \
    logic [`CORE_NR_THREADS-1:0] reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_SHARED_DEC_ONLY(NAME) \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_NON_SHARED_DEC_ONLY(NAME) \
    logic [`CORE_NR_THREADS-1:0] reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_SHARED_WITH_TYPE(NAME, TYPE) \
    TYPE reg_``NAME, reg_``NAME``_pre, reg_``NAME``_next; \
    logic  reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_NON_SHARED_WITH_TYPE(NAME, TYPE) \
    TYPE [`CORE_NR_THREADS-1:0] reg_``NAME, reg_``NAME``_pre, reg_``NAME``_next; \
    logic [`CORE_NR_THREADS-1:0] reg_``NAME``_en, reg_``NAME``_en_next;

`define CSR_DECLARATION_SHARED_LATCH(NAME, WIDTH) \
 `CSR_DECLARATION_SHARED(NAME, WIDTH) \
  logic  reg_``NAME``_latch_clk_en; \
  logic  reg_``NAME``_latch_clk;

`define CSR_DECLARATION_NON_SHARED_LATCH(NAME, WIDTH) \
  `CSR_DECLARATION_NON_SHARED(NAME, WIDTH) \
   logic [`CORE_NR_THREADS-1:0] reg_``NAME``_latch_clk_en; \
   logic [`CORE_NR_THREADS-1:0] reg_``NAME``_latch_clk;

`define CSR_DECLARATION_SHARED_LATCH_WITH_TYPE(NAME, TYPE) \
  `CSR_DECLARATION_SHARED_WITH_TYPE(NAME, TYPE) \
  logic  reg_``NAME``_latch_clk_en; \
  logic  reg_``NAME``_latch_clk;

`define CSR_DECLARATION_NON_SHARED_LATCH_WITH_TYPE(NAME, TYPE) \
 `CSR_DECLARATION_NON_SHARED_WITH_TYPE(NAME, TYPE) \
   logic [`CORE_NR_THREADS-1:0] reg_``NAME``_latch_clk_en; \
   logic [`CORE_NR_THREADS-1:0] reg_``NAME``_latch_clk;

`define CSR_LATCH_RESET_DECLARATION(NAME)                      \
logic NAME``_reg;                                              \
rst_repeat rst_repeat_``NAME (.reset_out(NAME``_reg),          \
                              .reset_in(NAME),                 \
                              .clock(clock),                   \
                              .dft__reset_byp(dft__reset_byp), \
                              .dft__reset(dft__reset) );


/***********************************************************************
 * macros for decoding and reading
 ***********************************************************************/

`define CSR_DECODE_DEFAULT_SHARED(NAME) \
        reg_``NAME``_en_next = 1'b0;

`define CSR_DECODE_DEFAULT_NON_SHARED(NAME) \
        reg_``NAME``_en_next[0] = 1'b0; \
        reg_``NAME``_en_next[1] = 1'b0;

`define CSR_DECODE_CASE_SHARED( AD, NAME, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = mem_wen; \
            io_rw_wb_rdata_next = 64'(READ_V); \
         end 

`define CSR_DECODE_CASE_SHARED_SX( AD, NAME, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = mem_wen; \
            io_rw_wb_rdata_next = `SX(64, READ_V); \
         end 


`define CSR_DECODE_CASE_SHARED_WITH_TRAP( AD, NAME, READ_V, TRAP) \
         AD: begin \
            mem_addr_valid = !TRAP; \
            reg_``NAME``_en_next = mem_wen && !TRAP; \
            io_rw_wb_rdata_next = 64'(READ_V); \
            mem_feature_xcpt = TRAP; \
         end 

`define CSR_DECODE_CASE_NON_SHARED( AD, NAME, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id}; \
            io_rw_wb_rdata_next = 64'(READ_V[mem_thread_id]); \
         end 

`define CSR_DECODE_CASE_NON_SHARED_SX( AD, NAME, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id}; \
            io_rw_wb_rdata_next = `SX(64, READ_V[mem_thread_id]); \
         end 


`define CSR_DECODE_CASE_NON_SHARED_WITH_TRAP( AD, NAME, READ_V, TRAP) \
         AD: begin \
            mem_addr_valid = !TRAP; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id} & { (`CORE_NR_THREADS) {!TRAP}}; \
            io_rw_wb_rdata_next = 64'(READ_V[mem_thread_id]); \
            mem_feature_xcpt = TRAP; \
         end


`define CSR_DECODE_CASE_SHARED_RD_CONSTANT(AD, NAME, READ_V) `CSR_DECODE_CASE_SHARED( AD, NAME, READ_V)
`define CSR_DECODE_CASE_SHARED_RD_CONSTANT_WITH_TRAP(AD, NAME, READ_V, TRAP) `CSR_DECODE_CASE_SHARED_WITH_TRAP( AD, NAME, READ_V, TRAP)

`define CSR_DECODE_CASE_NON_SHARED_RD_CONSTANT( AD, NAME, READ_V) \
          AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id}; \
            io_rw_wb_rdata_next = READ_V; \
         end 
`define CSR_DECODE_CASE_NON_SHARED_RD_CONSTANT_WITH_TRAP( AD, NAME, READ_V, TRAP) \
          AD: begin \
            mem_addr_valid = !TRAP; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id}  & { (`CORE_NR_THREADS) {!TRAP}}; \
            io_rw_wb_rdata_next = READ_V; \
            mem_feature_xcpt = TRAP; \
         end 


`define CSR_DECODE_CASE_SHARED_SHADOW( AD, NAME, READ_V, READ_F) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = mem_wen; \
            io_rw_wb_rdata_next = READ_F(READ_V[mem_thread_id]); \
         end 

`define CSR_DECODE_CASE_NON_SHARED_SHADOW( AD, NAME, READ_V, READ_F) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            reg_``NAME``_en_next = { mem_wen && mem_thread_id, mem_wen && !mem_thread_id}; \
            io_rw_wb_rdata_next = 64'(READ_F(READ_V[mem_thread_id])); \
         end 

`define CSR_DECODE_CASE_RO_SHARED(AD, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            io_rw_wb_rdata_next = READ_V; \
         end

`define CSR_DECODE_CASE_RO_NON_SHARED(AD, READ_V) \
         AD: begin \
            mem_addr_valid = 1'b1; \
            io_rw_wb_rdata_next = READ_V[mem_thread_id]; \
         end

`define CSR_DECODE_CASE_RO_NON_SHARED_WITH_TRAP(AD, READ_V, TRAP) \
         AD: begin \
            mem_addr_valid = !TRAP; \
            io_rw_wb_rdata_next = READ_V[mem_thread_id]; \
            mem_feature_xcpt = TRAP; \
         end



`define CSR_DECODE_SEQ_SHARED(NAME) \
                  `RST_EN_FF(clock_dec, reset_c, \
                             reg_``NAME``_en ^ reg_``NAME``_en_next,  /* en = self XOR */ \
                             reg_``NAME``_en, \
                             reg_``NAME``_en_next, \
                             '0)

`define CSR_DECODE_SEQ_NON_SHARED(NAME) \
                  `RST_EN_FF(clock_dec, reset_c, \
                             reg_``NAME``_en != reg_``NAME``_en_next,  /* en = self XOR */ \
                             reg_``NAME``_en, \
                             reg_``NAME``_en_next, \
                             '0)
  

/***********************************************************************
 * macros for core writes
 ***********************************************************************/
`define CSR_WRITE_COMB_DEFAULT(NAME) reg_``NAME``_next = reg_``NAME;

`define CSR_WRITE_LATCH_CLK_EN_ZERO(NAME) reg_``NAME``_latch_clk_en = '0;

`define CSR_WRITE_COMB_DEFAULT_SHARED_CNT(NAME, MSB, LSB, UP) \
reg_``NAME``_next[LSB] = UP ? reg_``NAME``[LSB] + 1'b1 : reg_``NAME[LSB]; \
      reg_``NAME``_next[MSB] = UP && &reg_``NAME``[LSB]? reg_``NAME[MSB] + 1'b1 : reg_``NAME[MSB];

`define CSR_WRITE_COMB_DEFAULT_NON_SHARED_CNT(NAME, MSB, LSB, UP) \
      for ( int i = 0; i< `CORE_NR_THREADS; i++ ) begin \
         reg_``NAME``_next[i][LSB] = UP[i] ? reg_``NAME``[i][LSB] + 1'b1 : reg_``NAME[i][LSB]; \
         reg_``NAME``_next[i][MSB] = UP[i] && &reg_``NAME``[i][LSB]? reg_``NAME[i][MSB] + 1'b1 : reg_``NAME[i][MSB]; \
      end

`define CSR_WRITE_COMB_SHARED(NAME, WDATA) if ( reg_``NAME``_en) reg_``NAME``_next = WDATA;

`define CSR_WRITE_COMB_NON_SHARED(NAME, WDATA) \
     if ( reg_``NAME``_en[0]) reg_``NAME``_next[0] = WDATA; \
     if ( reg_``NAME``_en[1]) reg_``NAME``_next[1] = WDATA;

`define CSR_WRITE_COMB_SHARED_SHADOW(NAME, WDATA, DST, WFUNC) \
         if ( reg_``NAME``_en) reg_``DST``_next = WFUNC(reg_``DST``_next, WDATA);

`define CSR_WRITE_COMB_NON_SHARED_SHADOW(NAME, WDATA, DST, WFUNC) \
     if ( reg_``NAME``_en[0]) reg_``DST``_next[0] = WFUNC(reg_``DST``_next[0], WDATA); \
     if ( reg_``NAME``_en[1]) reg_``DST``_next[1] = WFUNC(reg_``DST``_next[1], WDATA);


`define CSR_WRITE_COMB_LATCH_SHARED(NAME, WDATA) \
  reg_``NAME``_next = WDATA; \
  reg_``NAME``_latch_clk_en |= reg_``NAME``_en;

`define CSR_WRITE_COMB_LATCH_NON_SHARED(NAME, WDATA) \
   reg_``NAME``_next[0] = WDATA; \
   reg_``NAME``_next[1] = WDATA; \
   reg_``NAME``_latch_clk_en |= reg_``NAME``_en;

`define CSR_WRITE_COMB_LATCH_SHARED_SHADOW(NAME, WDATA, DST, WFUNC) \
   if (reg_``NAME``_en) reg_``DST``_next = WFUNC(reg_``DST``_next, WDATA); \
   reg_``DST``_latch_clk_en = reg_``NAME``_en;

`define CSR_WRITE_COMB_LATCH_NON_SHARED_SHADOW(NAME, WDATA, DST, WFUNC) \
   if (reg_``NAME``_en[0]) reg_``DST``_next[0] = WFUNC(reg_``DST``_next[0], WDATA); \
   if (reg_``NAME``_en[1]) reg_``DST``_next[1] = WFUNC(reg_``DST``_next[1], WDATA); \
   reg_``DST``_latch_clk_en |= reg_``NAME``_en; 

   
`define CSR_WRITE_COMB_SHARED_UNTYPED_SHADOW(NAME, WDATA, DST) \
         if ( reg_``NAME``_en) reg_``DST``_next = WDATA;

`define CSR_WRITE_COMB_NON_SHARED_UNTYPED_SHADOW(NAME, WDATA, DST) \
     if ( reg_``NAME``_en[0]) reg_``DST``_next[0] = WDATA; \
     if ( reg_``NAME``_en[1]) reg_``DST``_next[1] = WDATA;

`define CSR_WRITE_COMB_LATCH_SHARED_UNTYPED_SHADOW(NAME, WDATA, DST) \
   if (reg_``NAME``_en) reg_``DST``_next = WDATA; \
   reg_``DST``_latch_clk_en = reg_``NAME``_en;

`define CSR_WRITE_COMB_LATCH_NON_SHARED_UNTYPED_SHADOW(NAME, WDATA, DST) \
   if (reg_``NAME``_en[0]) reg_``DST``_next[0] = WDATA; \
   if (reg_``NAME``_en[1]) reg_``DST``_next[1] = WDATA; \
  reg_``DST``_latch_clk_en |= reg_``NAME``_en; 


   
`define CSR_WRITE_SEQ_SHARED(CLOCK, NAME, NAME_NEXT) `FF(CLOCK, reg_``NAME, reg_``NAME_NEXT)

`define CSR_WRITE_SEQ_SHARED_RST_ND(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_w, reg_``NAME, reg_``NAME_NEXT, RESETVAL)

`define CSR_WRITE_SEQ_SHARED_RST_D(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_d, reg_``NAME, reg_``NAME_NEXT, RESETVAL)

`define CSR_WRITE_SEQ_SHARED_RST(CLOCK, NAME, NAME_NEXT,  RESETVAL)`RST_FF(CLOCK, reset_c, reg_``NAME, reg_``NAME_NEXT, RESETVAL)

`define CSR_WRITE_SEQ_NON_SHARED(CLOCK, NAME, NAME_NEXT) \
  `CSR_WRITE_SEQ_SHARED(CLOCK, NAME[0], NAME_NEXT[0]) \
  `CSR_WRITE_SEQ_SHARED(CLOCK, NAME[1], NAME_NEXT[1])

`define CSR_WRITE_SEQ_NON_SHARED_RST(CLOCK, NAME, NAME_NEXT,  RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST(CLOCK, NAME[0], NAME_NEXT[0], RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST(CLOCK, NAME[1], NAME_NEXT[1], RESETVAL)

`define CSR_WRITE_SEQ_NON_SHARED_RST_D(CLOCK, NAME, NAME_NEXT,  RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST_D(CLOCK, NAME[0], NAME_NEXT[0], RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST_D(CLOCK, NAME[1], NAME_NEXT[1], RESETVAL)

`define CSR_WRITE_SEQ_NON_SHARED_RST_ND(CLOCK, NAME, NAME_NEXT,  RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST_ND(CLOCK, NAME[0], NAME_NEXT[0], RESETVAL) \
  `CSR_WRITE_SEQ_SHARED_RST_ND(CLOCK, NAME[1], NAME_NEXT[1], RESETVAL)


`define CSR_WRITE_LATCH_SHARED(CLOCK, NAME, NAME_DST, NAME_NEXT) \
et_clk_gate cbuf_rf_wr_``NAME(.enclk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
always @(reg_``NAME``_latch_clk or reg_``NAME_NEXT) \
  if (reg_``NAME``_latch_clk) begin \
      reg_``NAME_DST <= reg_``NAME_NEXT; \
  end

`define CSR_WRITE_LATCH_SHARED_RST(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   et_clk_gate cbuf_rf_wr_``NAME(.enclk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_c_reg" */ \
  always @(reset_c_reg or reg_``NAME``_latch_clk or `reg_``NAME_NEXT) begin \
    if (reset_c_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end \
  end

`define CSR_WRITE_LATCH_SHARED_RST_D(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   et_clk_gate cbuf_rf_wr_``NAME(.enclk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_d_reg" */ \
  always @(reset_d_reg or reg_``NAME``_latch_clk or reg_``NAME_NEXT) begin \
    if (reset_d_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end \
  end


`define CSR_WRITE_LATCH_SHARED_RST_ND(CLOCK, NAME, NAME_DST,  NAME_NEXT, RESETVAL) \
   et_clk_gate cbuf_rf_wr_``NAME(.enclk(reg_``NAME``_latch_clk), .en(reg_``NAME``_latch_clk_en), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_w_reg" */ \
  always @(reset_w_reg or reg_``NAME``_latch_clk or reg_``NAME_NEXT) begin \
    if (reset_w_reg) reg_``NAME_DST <= RESETVAL; \
    else if (reg_``NAME``_latch_clk) begin \
        reg_``NAME_DST <= reg_``NAME_NEXT; \
    end \
  end

`define CSR_WRITE_LATCH_NON_SHARED(CLOCK, NAME, NAME_DST, NAME_NEXT) \
  et_clk_gate cbuf_rf_wr_``NAME``0(.enclk(reg_``NAME``_latch_clk[0]), .en(reg_``NAME``_latch_clk_en[0]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  et_clk_gate cbuf_rf_wr_``NAME``1(.enclk(reg_``NAME``_latch_clk[1]), .en(reg_``NAME``_latch_clk_en[1]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  always @(reg_``NAME``_latch_clk[0] or reg_``NAME_NEXT[0]) \
    if (reg_``NAME``_latch_clk[0]) begin \
        reg_``NAME_DST[0] <= reg_``NAME_NEXT[0]; \
    end \
  always @(reg_``NAME``_latch_clk[1] or reg_``NAME_NEXT[1]) \
    if (reg_``NAME``_latch_clk[1]) begin \
        reg_``NAME_DST[1] <= reg_``NAME_NEXT[1]; \
      end
       

`define CSR_WRITE_LATCH_NON_SHARED_RST_ND(CLOCK, NAME, NAME_DST, NAME_NEXT, RESETVAL) \
  et_clk_gate cbuf_rf_wr_``NAME``0(.enclk(reg_``NAME``_latch_clk[0]), .en(reg_``NAME``_latch_clk_en[0]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  et_clk_gate cbuf_rf_wr_``NAME``1(.enclk(reg_``NAME``_latch_clk[1]), .en(reg_``NAME``_latch_clk_en[1]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_w_reg" */ \
  always @(reset_w_reg or reg_``NAME``_latch_clk[0] or reg_``NAME_NEXT[0]) begin \
    if (reset_w_reg) reg_``NAME_DST[0] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[0]) begin \
        reg_``NAME_DST[0] <= reg_``NAME_NEXT[0]; \
    end \
    end \
  /* synopsys async_set_reset "reset_w_reg" */ \
  always @(reset_w_reg or reg_``NAME``_latch_clk[1] or reg_``NAME_NEXT[1]) begin \
    if (reset_w_reg) reg_``NAME_DST[1] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[1]) begin \
        reg_``NAME_DST[1] <= reg_``NAME_NEXT[1]; \
    end \
   end 


`define CSR_WRITE_LATCH_NON_SHARED_RST_D(CLOCK, NAME, NAME_DST, NAME_NEXT, RESETVAL) \
  et_clk_gate cbuf_rf_wr_``NAME``0(.enclk(reg_``NAME``_latch_clk[0]), .en(reg_``NAME``_latch_clk_en[0]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  et_clk_gate cbuf_rf_wr_``NAME``1(.enclk(reg_``NAME``_latch_clk[1]), .en(reg_``NAME``_latch_clk_en[1]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_d_reg" */ \
  always @(reset_d_reg or reg_``NAME``_latch_clk[0] or reg_``NAME_NEXT[0]) begin \
    if (reset_d_reg) reg_``NAME_DST[0] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[0]) begin \
        reg_``NAME_DST[0] <= reg_``NAME_NEXT[0]; \
    end \
    end \
  /* synopsys async_set_reset "reset_d_reg" */ \
  always @(reset_d_reg or reg_``NAME``_latch_clk[1] or reg_``NAME_NEXT[1]) begin \
    if (reset_d_reg) reg_``NAME_DST[1] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[1]) begin \
        reg_``NAME_DST[1] <= reg_``NAME_NEXT[1]; \
    end \
   end 



`define CSR_WRITE_LATCH_NON_SHARED_RST(CLOCK, NAME, NAME_DST, NAME_NEXT, RESETVAL) \
  et_clk_gate cbuf_rf_wr_``NAME``0(.enclk(reg_``NAME``_latch_clk[0]), .en(reg_``NAME``_latch_clk_en[0]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  et_clk_gate cbuf_rf_wr_``NAME``1(.enclk(reg_``NAME``_latch_clk[1]), .en(reg_``NAME``_latch_clk_en[1]), .clk(CLOCK), .te(1'b0 /*disable test_en to avoid current spike*/));   \
  /* synopsys async_set_reset "reset_c_reg" */ \
  always @(reset_c_reg or reg_``NAME``_latch_clk[0] or reg_``NAME_NEXT[0]) begin \
    if (reset_c_reg) reg_``NAME_DST[0] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[0]) begin \
        reg_``NAME_DST[0] <= reg_``NAME_NEXT[0]; \
    end \
    end \
  /* synopsys async_set_reset "reset_c_reg" */ \
  always @(reset_c_reg or reg_``NAME``_latch_clk[1] or reg_``NAME_NEXT[1]) begin \
    if (reset_c_reg) reg_``NAME_DST[1] <= RESETVAL; \
    else if (reg_``NAME``_latch_clk[1]) begin \
        reg_``NAME_DST[1] <= reg_``NAME_NEXT[1]; \
    end \
   end 

`define CSR_WRITE_TYPE_WITH_RESERVED_SHARED(NAME, FUNC) \
   always_comb reg_``NAME = FUNC(reg_``NAME``_pre);
   
`define CSR_WRITE_TYPE_WITH_RESERVED_NON_SHARED(NAME, FUNC) \
   always_comb reg_``NAME[0] = FUNC(reg_``NAME``_pre[0]); \
   always_comb reg_``NAME[1] = FUNC(reg_``NAME``_pre[1]);
   
     



`endif

// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Some usefule macros
`ifndef _ET_MACROS_
`define _ET_MACROS_

    // type replacement
    `ifndef EVL_SIMULATION
    `define toVector(TYPE) \
        logic [$bits(TYPE)-1:0]
    `else
    `define toVector(TYPE) TYPE
    `endif

    `ifndef EVL_SIMULATION
    `define toVectorExt(TYPE,EXT_DIMS) \
        logic EXT_DIMS [$bits(TYPE)-1:0]
    `else
    `define toVectorExt(TYPE,EXT_DIMS) TYPE EXT_DIMS
    `endif

    // Extensions
    `define ZX(SIZE, SIGNAL) \
        (((SIZE-$bits(SIGNAL)) == 0) ? SIGNAL : { {(SIZE-$bits(SIGNAL)){1'b0}}, SIGNAL })

    `define SX(SIZE, SIGNAL) \
        (((SIZE-$bits(SIGNAL)) == 0) ? SIGNAL : { {(SIZE-$bits(SIGNAL)){SIGNAL[$bits(SIGNAL)-1]}}, SIGNAL })

    // Flip flops
    `define FF(CLK, DATAOUT, DATAIN) \
        always @ (posedge CLK) \
            DATAOUT <= DATAIN;

    `define RST_FF(CLK, RST, DATAOUT, DATAIN, DEF) \
        always @ (posedge CLK) \
            if(RST) DATAOUT <= DEF; \
            else    DATAOUT <= DATAIN;

    `define EN_FF(CLK, EN, DATAOUT, DATAIN) \
        always @ (posedge CLK) \
            if(EN) DATAOUT <= DATAIN;

    `define RST_EN_FF(CLK, RST, EN, DATAOUT, DATAIN, DEF) \
        always @ (posedge CLK) \
            if(RST)     DATAOUT <= DEF; \
            else if(EN) DATAOUT <= DATAIN;

    // Flip flop variants with asynchronous low reset.
    `define RST_FF_RASL(CLK, RSTN, DATAOUT, DATAIN, DEF) \
        always @ (posedge CLK or negedge RSTN) \
            if(!RSTN) DATAOUT <= DEF; \
            else      DATAOUT <= DATAIN;

    `define RST_EN_FF_RASL(CLK, RSTN, EN, DATAOUT, DATAIN, DEF) \
        always @ (posedge CLK or negedge RSTN) \
            if(!RSTN)     DATAOUT <= DEF; \
            else if(EN)   DATAOUT <= DATAIN;


    // Latches
    `define LATCH(CLK, DOUT, DIN) \
          if (1) begin \
              localparam type T = type(DIN); \
              wire T _din = DIN; \
              always @(CLK or _din) \
                  if(CLK) DOUT <= _din; \
          end

    `define LATCH_P2(CLK, DOUT, DIN) \
        if (1) begin \
            localparam type T = type(DIN); \
            wire T _din = DIN; \
            always @(CLK or _din) \
                if(~CLK) DOUT <= _din; \
        end

    // valid is 1 in the same cycle as setCond, and it is cleared the next cycle after clearCond
    `define VALID( CLK, RST, NAME, setCond, clearCond) \
        `RST_FF(CLK, RST, NAME``_pending, (clearCond) ? 1'b0 : (setCond) ? 1'b1 : NAME``_pending, 1'b0) \
         assign NAME = NAME``_pending || (setCond);

    // valid is 1 in the cycle after setCond, and it is cleared the next cycle after clearCond
    `define VALID_NEXT( CLK, RST, NAME, setCond, clearCond) \
        `RST_FF(CLK, RST, NAME, (setCond)? 1'b1 : (clearCond) ? 1'b0 : NAME, 1'b0)

    // Simple compares
    `define MIN(a,b) ((a)>(b) ? (b) : (a) )
    `define MAX(a,b) ((a)>(b) ? (a) : (b) )

    /** Returns the n-cycle delayed version of the input signal, using EN as enable.
     *
     * IN input to delay
     * OUT output signal
     * INITVAL input to delay
     * STAGES number of cycles to delay
     * EN enable the shift
     */
    `define RST_EN_SHIFT_REG(CLK, RST, STAGES, EN, OUT, IN, INITVAL) \
        generate  \
            if (STAGES==0) \
            begin: RST_EN_SHIFT_REG0_``OUT \
                assign OUT = IN; \
            end \
            else if (STAGES==1) \
            begin: RST_EN_SHIFT_REG1_``OUT \
                `RST_EN_FF(CLK, RST, EN, OUT, IN, INITVAL) \
            end \
            else \
            begin: RST_EN_SHIFT_REG_``OUT \
                logic [STAGES-1:0][$bits(IN)-1:0] delayed; \
                always_ff@(posedge CLK) begin \
                    if (RST) begin \
                        for (int i = 0 ; i < STAGES; i++) delayed[i] <= INITVAL; \
                    end else if (EN) begin \
                        for (int i = 1 ; i < STAGES; i++) delayed[i] <= delayed[i-1]; \
                        delayed[0] <= IN; \
                    end \
                end \
                assign OUT = delayed[ STAGES - 1]; \
            end // else: !if(STAGES==1)  \
        endgenerate

    /** Returns the n-cycle delayed version of the input signal, using EN[i] as enable per stage.
     *
     * IN input to delay
     * OUT output signal
     * INITVAL input to delay
     * STAGES number of cycles to delay
     * EN enable the shift
     */
    `define RST_EN_PIPE_SHIFT_REG(CLK, RST, STAGES, EN_OUT, EN_IN, OUT, IN, INITVAL_EN, INITVAL) \
        generate  \
            if (STAGES==0) \
            begin: RST_EN_PIPE_SHIFT_REG0_``OUT \
                assign OUT = IN; \
            end \
            else if (STAGES==1) \
            begin: RST_EN_PIPE_SHIFT_REG1_``OUT \
                `RST_FF   (CLK, RST, EN_OUT, EN_IN, INITVAL_EN) \
                `RST_EN_FF(CLK, RST, EN_OUT, OUT, IN, INITVAL) \
            end \
            else \
            begin: RST_EN_PIPE_SHIFT_REG_``OUT \
                logic [STAGES-1:0] delayed_en; \
                always_ff@(posedge CLK) begin \
                    if (RST) begin \
                        for (int i = 0 ; i < STAGES; i++) delayed_en[i] <= INITVAL_EN; \
                    end else begin \
                        for (int i = 1 ; i < STAGES; i++) delayed_en[i] <= delayed_en[i-1]; \
                        delayed[0] <= EN_IN; \
                    end \
                end \
                assign EN_OUT = delayed_en[ STAGES - 1]; \
                logic [STAGES-1:0][$bits(IN)-1:0] delayed; \
                always_ff@(posedge CLK) begin \
                    if (RST) begin \
                        for (int i = 0 ; i < STAGES; i++) delayed[i] <= INITVAL; \
                    end else if (delayed_en[i]) begin \
                        for (int i = 1 ; i < STAGES; i++) delayed[i] <= delayed[i-1]; \
                        delayed[0] <= IN; \
                    end \
                end \
                assign OUT = delayed[ STAGES - 1]; \
            end // else: !if(STAGES==1)  \
        endgenerate

    // same as above, but without reset
    `define EN_SHIFT_REG(CLK, STAGES, EN, OUT, IN) \
        generate  \
            if (STAGES==0) \
            begin: EN_SHIFT_REG0_``OUT \
               assign OUT = IN; \
            end \
            else if (STAGES==1) \
            begin: EN_SHIFT_REG1_``OUT \
                `EN_FF(CLK, EN, OUT, IN) \
            end \
            else \
            begin: EN_SHIFT_REG_``OUT \
                logic [STAGES-1:0][$bits(IN)-1:0] delayed; \
                always_ff@(posedge CLK) begin \
                    if (EN) begin \
                        for (int i = 1 ; i < STAGES; i++) delayed[i] <= delayed[i-1]; \
                        delayed[0] <= IN; \
                    end \
                end \
                assign OUT = delayed[ STAGES - 1]; \
            end // else: !if(STAGES==1)  \
        endgenerate


    // same as above, but without enable
    `define RST_SHIFT_REG(CLK, RST, STAGES, OUT, IN, INITVAL) \
        generate  \
            if (STAGES==0) \
            begin: RST_SHIFT_REG0_``OUT \
                assign OUT = IN; \
            end \
            else if (STAGES==1) \
            begin: RST_SHIFT_REG1_``OUT \
                `RST_FF(CLK, RST, OUT, IN, INITVAL) \
            end \
            else \
            begin: RST_SHIFT_REG_``OUT \
                logic [STAGES-1:0][$bits(IN)-1:0] delayed; \
                always_ff@(posedge CLK) begin \
                    if (RST) begin \
                        for (int i = 0 ; i < STAGES; i++) delayed[i] <= INITVAL; \
                    end else begin \
                        for (int i = 1 ; i < STAGES; i++) delayed[i] <= delayed[i-1]; \
                        delayed[0] <= IN; \
                    end \
                end \
                assign OUT = delayed[ STAGES - 1]; \
            end // else: !if(STAGES==1)  \
        endgenerate


    `define PIPE_STAGE(CLK, RST, IN_DATA, IN_VALID, IN_READY, OUT_DATA, OUT_VALID, OUT_READY) \
         `RST_EN_FF(CLK, RST, IN_READY, OUT_VALID, IN_VALID, 1'b0) \
         `EN_FF(CLK, IN_READY && IN_VALID, OUT_DATA, IN_DATA) \
          assign IN_READY = !(OUT_VALID && !(OUT_READY));


    `define RC_PIPE_STAGE(CLK, RST, RUN, IN_DATA, IN_VALID, IN_READY, OUT_DATA, OUT_VALID, OUT_READY) \
         `RST_EN_FF(CLK, RST, RUN && IN_READY, OUT_VALID, IN_VALID, 1'b0) \
         `EN_FF(CLK, RUN && IN_READY && IN_VALID, OUT_DATA, IN_DATA) \
          assign IN_READY = !(OUT_VALID && !(OUT_READY));

    typedef enum logic [7:0] {
       gating_type_et_link_req         = 8'd1,
       gating_type_et_link_rsp         = 8'd2,
       gating_type_sc_xbar_req         = 8'd3,
       gating_type_sc_xbar_rsp         = 8'd4,
       gating_type_sc_dataq_wr         = 8'd5,
       gating_type_sc_mesh_master_req  = 8'd6,
       gating_type_sc_mesh_slave_req   = 8'd7,
       gating_type_sc_mesh_master_rsp  = 8'd8,
       gating_type_sc_mesh_slave_rsp   = 8'd9,
       gating_type_sc_reqq_mesh_req    = 8'd10,
       gating_type_sc_rspmux           = 8'd11,
       gating_type_sc_rspmux_long      = 8'd12,
       gating_type_default             = 8'd0
    } gating_type_t;

   // Flip flops with XOR self gating
    `define XFF(CLK, DATAOUT, DATAIN) \
         generate \
         if (1) begin : XFF_``DATAOUT\
             logic      __xen, __xclk; \
             assign __xen =  |((DATAOUT) ^ (DATAIN)); \
             et_clk_gate cgate ( .enclk ( __xclk ), .en ( __xen ), .clk ( CLK ), .te (1'b0)); \
             `FF(__xclk, DATAOUT, DATAIN) \
          end \
          endgenerate

    `define RST_XFF(CLK, RST, DATAOUT, DATAIN, DEF) \
        generate \
         if (1) begin : RST_XFF_``DATAOUT \
             logic      __xen, __xclk; \
             assign __xen =  (|((DATAOUT) ^ (DATAIN))) | (RST); \
             et_clk_gate cgate ( .enclk ( __xclk ), .en ( __xen ), .clk ( CLK ), .te (1'b0)); \
             `RST_FF(__xclk, RST, DATAOUT, DATAIN, DEF) \
          end \
          endgenerate


    `define EN_XFF(CLK, EN, DATAOUT, DATAIN) \
         generate \
         if (1) begin : EN_XFF_``DATAOUT \
             logic      __xen, __xclk; \
             assign __xen =  (|((DATAOUT) ^ (DATAIN))) & (EN); \
             et_clk_gate cgate ( .enclk ( __xclk ), .en ( __xen ), .clk ( CLK ), .te (1'b0)); \
             `FF(__xclk, DATAOUT, DATAIN) \
          end \
          endgenerate


    `define RST_EN_XFF(CLK, RST, EN, DATAOUT, DATAIN, DEF) \
         generate \
         if (1) begin : RST_EN_XFF_``DATAOUT \
             logic      __xen, __xclk; \
             assign __xen =  ((|((DATAOUT) ^ (DATAIN))) & (EN)) | (RST); \
             et_clk_gate cgate ( .enclk ( __xclk ), .en ( __xen ), .clk ( CLK ), .te (1'b0)); \
             `RST_FF(__xclk, RST, DATAOUT, DATAIN, DEF) \
          end \
          endgenerate


   // FF with clock enable being the OR of D and Q ( intented for very low duty cycle signals)
    `define RST_OFF(CLK, RST, DATAOUT, DATAIN, DEF) \
        generate \
         if (1) begin : RST_OFF_``DATAOUT \
             logic      __xen, __xclk; \
             assign __xen = (|(DATAOUT)) || (|(DATAIN)) || (RST); \
             et_clk_gate cgate ( .enclk ( __xclk ), .en ( __xen ), .clk ( CLK ), .te (1'b0)); \
             `RST_FF(__xclk, RST, DATAOUT, DATAIN, DEF) \
          end \
          endgenerate



`endif // _ET_MACROS_

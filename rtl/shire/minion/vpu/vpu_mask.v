// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_mask (
  // System signals
  input                                                   clock,
  input                                                   reset,
  // EX/F1 data and control inputs
  input                                                   ex_in_valid,
  input                                                   ex_inst_valid,
  input        [$clog2(`VPU_REGMASK_NUM)-1:0]             ex_regfile_raddr1,
  input        [$clog2(`VPU_REGMASK_NUM)-1:0]             ex_regfile_raddr2,
  input        [`VPU_FCMD_SZ-1:0]                         ex_cmd,
  input        [`VPU_LANE_NUM-1:0]                        ex_imm,
  input                                                   ex_fromint,
  input        [`XREG_RANGE]                              ex_fromint_data,
  input                                                   ex_thread_id,
  input                                                   ex_ignore_mask,
  // F2 inputs
  input                                                   f2_thread_id,
  input                                                   f2_wen,
  input                                                   f2_core_kill,
  input        [$clog2(`VPU_REGMASK_NUM)-1:0]             f2_waddr,
  input                                                   f2_maskop,
  input                                                   f2_tointm,
  // F3 inputs
  input                                                   f3_thread_id,
  input                                                   f3_regmask_from_txfma,
  input                                                   f3_regmask_from_short,
  input                                                   f3_core_kill,
  input        [`VPU_LANE_NUM-1:0]                        f3_regmask_wdata_lane,
  // F4 inputs
  input                                                   f4_regmask_from_txfma,
  input                                                   f4_core_kill,
  // F5 inputs
  input                                                   f5_thread_id,
  // F6 inputs
  input                                                   f6_thread_id,
  // F7 inputs
  input                                                   f7_thread_id,
  // F8 inputs
  input                                                   f8_thread_id,
  input        [`VPU_LANE_NUM-1:0]                        f8_regmask_comp_res,
  // EX/F1 outputs
  output logic [`VPU_LANE_NUM-1:0]                        ex_mask_rf0,
  output logic [`VPU_LANE_NUM-1:0]                        ex_mask_in1,
  // F2 outputs
  output logic [`VPU_LANE_NUM-1:0]                        f2_mask_rf0,
  // F8 outputs
  output logic [`XREG_RANGE]                              f8_toint_data,
  output logic [`VPU_LANE_NUM-1:0]                        f8_regmask_store,
  // WB outputs
  output logic [`VPU_MASK_SCOREBOARD_SIZE-1:0]            wb_mask_valid,
  output minion_mreg_dest [`VPU_MASK_SCOREBOARD_SIZE-1:0] wb_mask_dest
);


  // Signal Declarations
  logic [1:0][`VPU_REGMASK_NUM-1:0][`VPU_LANE_NUM-1:0] ex_mask_rf;                // Contents of the Mask RF
  logic [1:0][`VPU_REGMASK_NUM-1:0][`VPU_LANE_NUM-1:0] f8_mask_rf_wdata;          // F6 mask RF write data
  logic [1:0][`VPU_LANE_NUM-1:0]                       ex_mask_rf_bypass;         // Bypass of the Mask RF
  logic [`VPU_LANE_NUM-1:0]                            mask_rf0_bypass;           // Bypass of the Mask RF0
  logic                                                f8_mask_rf_clk_en;
  logic [`VPU_REGMASK_ALL_DATA_SZ-1:0]                 f2_regmask_wdata;
  logic                                                f2_valid;
  logic                                                f3_valid;
  logic                                                f4_valid;
  logic                                                f5_valid;
  logic                                                f6_valid;
  logic                                                f7_valid;
  logic [`VPU_REGMASK_ALL_DATA_SZ-1:0]                 f3_regmask_wdata;
  logic [4:0]                                          f2_toint_data;
  logic [4:0]                                          f3_toint_data;
  logic [4:0]                                          f4_toint_data;
  logic [4:0]                                          f5_toint_data;
  logic [4:0]                                          f6_toint_data;
  logic [4:0]                                          f7_toint_data;
  logic [4:0]                                          f8_toint_data_pre;
  logic                                                f2_mova_x_m;
  logic                                                f3_mova_x_m;
  logic                                                f4_mova_x_m;
  logic                                                f5_mova_x_m;
  logic                                                f6_mova_x_m;
  logic                                                f7_mova_x_m;
  logic                                                f8_mova_x_m;
  logic [`VPU_LANE_NUM-1:0]                            f2_regmask_store_to_mask;
  logic [`VPU_LANE_NUM-1:0]                            f2_regmask_store;
  logic [`VPU_LANE_NUM-1:0]                            f2_regmask_store_bypass;
  logic                                                f2_regmask_store_all;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f2_regmask_waddr;
  logic [`VPU_LANE_NUM-1:0]                            f3_regmask_store;
  logic [`VPU_LANE_NUM-1:0]                            f3_regmask_store_bypass;
  logic [`VPU_LANE_NUM-1:0]                            f3_regmask_store_to_f4;
  logic [`VPU_LANE_NUM-1:0]                            f4_regmask_store_to_f5;
  logic                                                f3_regmask_store_all;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f3_regmask_waddr;
  logic [`VPU_LANE_NUM-1:0]                            f4_regmask_store;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f4_regmask_waddr;
  logic [`VPU_LANE_NUM-1:0]                            f5_regmask_store;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f5_regmask_waddr;
  logic [`VPU_LANE_NUM-1:0]                            f6_regmask_store;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f6_regmask_waddr;
  logic [`VPU_LANE_NUM-1:0]                            f7_regmask_store;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f7_regmask_waddr;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 f8_regmask_waddr;
  logic [$clog2(`VPU_REGMASK_NUM)-1:0]                 ex_regmask_raddr;
  logic [`VPU_LANE_NUM-1:0]                            ex_regmask_data;
  logic [15:0]                                         mpopc_mask;
  logic [15:0]                                         mpopc_rast;
  logic [`VPU_DATA_S_SZ-1:0]                           ex_in1;
  logic [`VPU_DATA_S_SZ-1:0]                           ex_in2;
  logic [`VPU_FCMD_SZ-1:0]                             f2_cmd;
  logic [`VPU_DATA_S_SZ-1:0]                           f2_in1;
  logic [`VPU_DATA_S_SZ-1:0]                           f2_in2;
  logic [`VPU_LANE_NUM-1:0]                            f2_imm;
  logic [`VPU_REGMASK_ALL_DATA_SZ-1:0]                 f3_regmask_wdata_sel;
  logic                                                f3_regmask_store_all_qual;
  logic                                                f3_regmask_store_qual;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// EX/F1 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Register Mask
////////////////////////////////////////////////////////////////////////////////

  //     CLK    EN                 DOUT        DIN
  `EN_FF(clock, f8_mask_rf_clk_en, ex_mask_rf, f8_mask_rf_wdata)


  ////////////////////////////////////////////////////////////////////////////////
  // bypass rf src[1:0] for mask instructions
  ////////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // 2 operands
    for (integer j = 0; j < 2; j++) begin

      // src addr
      ex_regmask_raddr = (j == 0) ? ex_regfile_raddr1 : ex_regfile_raddr2;
      // ex mask data
      ex_regmask_data  = ex_mask_rf[ex_thread_id][ex_regmask_raddr];

      for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
        f2_regmask_store_bypass[i] = f2_regmask_store_to_mask[i];
        f3_regmask_store_bypass[i] = f3_regmask_store[i];

        ex_mask_rf_bypass[j][i] = (ex_regmask_raddr == f2_regmask_waddr) && (ex_thread_id == f2_thread_id) && f2_regmask_store_bypass[i] ? f2_regmask_wdata[i]
                                : (ex_regmask_raddr == f3_regmask_waddr) && (ex_thread_id == f3_thread_id) && f3_regmask_store_bypass[i] ? f3_regmask_wdata_sel[i]
                                : (ex_regmask_raddr == f8_regmask_waddr) && (ex_thread_id == f8_thread_id) && f8_regmask_store[i]        ? f8_regmask_comp_res[i]
                                :                                                                                                          ex_regmask_data[i];
      end
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // bypass rf0 for vector instructions
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
      mask_rf0_bypass[i] = (f2_regmask_waddr == 0) && (ex_thread_id == f2_thread_id) && f2_regmask_store_bypass[i] ? f2_regmask_wdata[i]
                         : (f3_regmask_waddr == 0) && (ex_thread_id == f3_thread_id) && f3_regmask_store_bypass[i] ? f3_regmask_wdata_sel[i]
                         : (f8_regmask_waddr == 0) && (ex_thread_id == f8_thread_id) && f8_regmask_store[i]        ? f8_regmask_comp_res[i]
                         :                                                                                           ex_mask_rf[ex_thread_id][0][i];
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Sends m0 out
  ////////////////////////////////////////////////////////////////////////////////
  always_comb begin
    if (ex_ignore_mask) // mask not used in flq/fsq or fcmovm
      ex_mask_rf0 = {(`VPU_LANE_NUM) {1'b1}};
    else
      ex_mask_rf0 = mask_rf0_bypass;

    ex_mask_in1 = mask_rf0_bypass; // in fcmovm.ps mask is used as selector, not as mask
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Selct input operands
  ////////////////////////////////////////////////////////////////////////////////
  assign ex_in1 = ex_fromint ? ex_fromint_data[31:0]
                :              {24'b0, ex_mask_rf_bypass[0]};
  assign ex_in2 = ex_fromint ? ex_fromint_data[63:32]
                :              {24'b0, ex_mask_rf_bypass[1]};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F2 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN               DOUT             DIN          DEF
  `RST_FF(clock, reset,                  f2_valid,        ex_in_valid, 1'b0)
  `FF    (clock,                         f2_cmd,          ex_cmd)
  `EN_FF (clock,        ex_in_valid,     f2_in1,          ex_in1)
  `EN_FF (clock,        ex_in_valid,     f2_in2,          ex_in2)
  `EN_FF (clock,        ex_in_valid,     f2_imm,          ex_imm)
  `EN_FF (clock,        ex_inst_valid,   f2_mask_rf0,     ex_mask_rf0)


  ////////////////////////////////////////////////////////////////////////////////
  // mask operations
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin

    //
    // Mask Op results
    //

    // mova m.x: default
    f2_regmask_wdata = {f2_in2, f2_in1};

    // mov m.x
    if (f2_cmd == `VPU_FCMD_MOV_M_X) begin
      // int data | imm
      f2_regmask_wdata = {{`VPU_REGMASK_ALL_DATA_SZ - `VPU_LANE_NUM{1'b0}},
                          f2_in1[`VPU_LANE_NUM-1:0] | f2_imm[`VPU_LANE_NUM-1:0]};
    end
    // mand
    if (f2_cmd == `VPU_FCMD_MAND) begin
      f2_regmask_wdata = {32'b0, (f2_in1 & f2_in2)};
    end
    // mor
    if (f2_cmd == `VPU_FCMD_MOR) begin
      f2_regmask_wdata = {32'b0, (f2_in1 | f2_in2)};
    end
    // mxor
    if (f2_cmd == `VPU_FCMD_MXOR) begin
      f2_regmask_wdata = {32'b0, (f2_in1 ^ f2_in2)};
    end
    // mnot
    if (f2_cmd == `VPU_FCMD_MNOT) begin
      f2_regmask_wdata = {32'b0, ~f2_in1};
    end

    //
    // To Int results
    //

    // mov.x.m decoding
    f2_mova_x_m = (f2_cmd == `VPU_FCMD_MOVA_X_M);

    // mpopc: default
    f2_toint_data = 4'b0;
    for (integer n = 0; n < `VPU_LANE_NUM; n++)
      f2_toint_data += (f2_in1[n]) ? 4'b1 : 4'b0;

    // mpopcz
    if (f2_cmd == `VPU_FCMD_MPOPCZ) begin
      f2_toint_data = 4'b0;
      for (integer n = 0; n < `VPU_LANE_NUM; n++)
        f2_toint_data += (!f2_in1[n]) ? 4'b1 : 4'b0;
    end

    // mpopc_rast
    if (f2_cmd == `VPU_FCMD_MPOPC_RAST) begin
      case (f2_imm[3:0])
        4'd0:    mpopc_mask = 16'h0f0f;
        4'd1:    mpopc_mask = 16'h3c3c;
        4'd2:    mpopc_mask = 16'hf0f0;
        default: mpopc_mask = 16'hffff;
      endcase

      mpopc_rast = ({f2_in2[7:0], f2_in1[7:0]}) & mpopc_mask;

      f2_toint_data = 4'b0;
      for (integer n = 0; n < 16; n++)
        f2_toint_data += (mpopc_rast[n]) ? 4'b1 : 4'b0;
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Signals to control writing to regmask registers
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f2_regmask_store_to_mask = `VPU_LANE_NUM'b0; // No dependency with core kill to improve timing

    if (f2_wen && (f2_cmd != `VPU_FCMD_MOVA_M_X)) begin
      if (f2_maskop)
        f2_regmask_store_to_mask = {`VPU_LANE_NUM{1'b1}};
      if (f2_tointm)
        f2_regmask_store_to_mask = f2_mask_rf0;
    end

    f2_regmask_store = f2_regmask_store_to_mask & {`VPU_LANE_NUM{!f2_core_kill}};

    f2_regmask_store_all = f2_wen && f2_maskop && (f2_cmd == `VPU_FCMD_MOVA_M_X) && !f2_core_kill;
    f2_regmask_waddr     = f2_waddr & {{($clog2(`VPU_REGMASK_NUM)) {|f2_regmask_store_to_mask}}};
  end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //       CLK    RST    EN                                                  DOUT                  DIN                   DEF
  `RST_FF (clock, reset,                                                     f3_valid,             f2_valid,             1'b0)
  `RST_OFF(clock, reset,                                                     f3_regmask_store,     f2_regmask_store,     `VPU_LANE_NUM'b0)
  `RST_FF (clock, reset,                                                     f3_regmask_store_all, f2_regmask_store_all, 1'b0)
  `EN_FF  (clock,        (|f2_regmask_store_to_mask | f2_regmask_store_all), f3_regmask_wdata,     f2_regmask_wdata)
  `EN_FF  (clock,        f2_valid,                                           f3_toint_data,        f2_toint_data)
  `EN_FF  (clock,        |f2_regmask_store_to_mask,                          f3_regmask_waddr,     f2_regmask_waddr)
  `EN_FF  (clock,        f2_valid,                                           f3_mova_x_m,          f2_mova_x_m)

  ////////////////////////////////////////////////////////////////////////////////
  //  select data from short compare or internal pipeline data
  ////////////////////////////////////////////////////////////////////////////////

  assign f3_regmask_wdata_sel = f3_regmask_from_short ? {56'b0, f3_regmask_wdata_lane} // short lane
                              :                          f3_regmask_wdata;             // internal pipeline


  ////////////////////////////////////////////////////////////////////////////////
  // Computes regmask going to F4 after kill and only for TXFMA operations
  ////////////////////////////////////////////////////////////////////////////////

  assign f3_regmask_store_to_f4 = f3_regmask_store & {`VPU_LANE_NUM{~f3_core_kill & f3_regmask_from_txfma || f3_regmask_from_short}};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F4 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////////
  //  select data from short compare or internal pipeline data
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN                 DOUT                   DIN                     DEF
  `RST_FF (clock, reset,                    f4_valid,              f3_valid,               1'b0)
  `RST_OFF(clock, reset,                    f4_regmask_store,      f3_regmask_store_to_f4, `VPU_LANE_NUM'b0)
  `EN_FF  (clock,        f3_valid,          f4_toint_data,         f3_toint_data)
  `EN_FF  (clock,        |f3_regmask_store, f4_regmask_waddr,      f3_regmask_waddr)
  `EN_FF  (clock,        f3_valid,          f4_mova_x_m,           f3_mova_x_m)

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F5 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  assign f4_regmask_store_to_f5 = f4_regmask_store & {`VPU_LANE_NUM{~f4_core_kill & f4_regmask_from_txfma}};


  //      CLK    RST    EN                 DOUT              DIN                     DEF
  `RST_FF (clock, reset,                    f5_valid,         f4_valid,               1'b0)
  `RST_OFF(clock, reset,                    f5_regmask_store, f4_regmask_store_to_f5, `VPU_LANE_NUM'b0)
  `EN_FF  (clock,        f4_valid,          f5_toint_data,    f4_toint_data)
  `EN_FF  (clock,        |f4_regmask_store, f5_regmask_waddr, f4_regmask_waddr)
  `EN_FF  (clock,        f4_valid,          f5_mova_x_m,      f4_mova_x_m)


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F6 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN                 DOUT               DIN               DEF
  `RST_FF (clock, reset,                    f6_valid,          f5_valid,         1'b0)
  `RST_OFF(clock, reset,                    f6_regmask_store,  f5_regmask_store, '0)
  `EN_FF  (clock,        f5_valid,          f6_toint_data,     f5_toint_data)
  `EN_FF  (clock,        |f5_regmask_store, f6_regmask_waddr,  f5_regmask_waddr)
  `EN_FF  (clock,        f5_valid,          f6_mova_x_m,       f5_mova_x_m)


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F7 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN                 DOUT               DIN               DEF
  `RST_FF (clock, reset,                    f7_valid,          f6_valid,         1'b0)
  `RST_OFF(clock, reset,                    f7_regmask_store,  f6_regmask_store, '0)
  `EN_FF  (clock,        f6_valid,          f7_toint_data,     f6_toint_data)
  `EN_FF  (clock,        |f6_regmask_store, f7_regmask_waddr,  f6_regmask_waddr)
  `EN_FF  (clock,        f6_valid,          f7_mova_x_m,       f6_mova_x_m)


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F8 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN                    DOUT                      DIN                   DEF
  `RST_OFF(clock, reset,                       f8_regmask_store,         f7_regmask_store,     '0)
  `EN_FF  (clock,        f7_valid,             f8_toint_data_pre,        f7_toint_data)
  `EN_FF  (clock,        |f7_regmask_store,    f8_regmask_waddr,         f7_regmask_waddr)
  `EN_FF  (clock,        f7_valid,             f8_mova_x_m,              f7_mova_x_m)


  ////////////////////////////////////////////////////////////////////////////////
  // mask rf write data
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    // default
    f8_mask_rf_wdata = ex_mask_rf;

    f3_regmask_store_all_qual = f3_regmask_store_all && !f3_core_kill;
    f3_regmask_store_qual     = (|f3_regmask_store) && !f3_regmask_from_txfma && !f3_core_kill;

    // Write from F3 move all instruction (move all from int)
    if (f3_regmask_store_all_qual) begin
      f8_mask_rf_wdata[f3_thread_id] = f3_regmask_wdata_sel;
      // Write from F3 or F8 stage one mask entry
    end

    // Write from mask or sh/sw comp op
    if (f3_regmask_store_qual) begin
      for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
        if (f3_regmask_store[i])
          f8_mask_rf_wdata[f3_thread_id][f3_regmask_waddr][i] = f3_regmask_wdata_sel[i];
      end
    end

    // Write from TXFMA comp op
    if (|f8_regmask_store) begin
      for (integer i = 0; i < `VPU_LANE_NUM; i++) begin
        if (f8_regmask_store[i]) begin
          f8_mask_rf_wdata[f8_thread_id][f8_regmask_waddr][i] = f8_regmask_comp_res[i];
        end
      end
    end


    // clock enable
    f8_mask_rf_clk_en = (|f3_regmask_store) && !f3_core_kill
                     || f3_regmask_store_all && !f3_core_kill
                     || (|f8_regmask_store);
  end


  ////////////////////////////////////////////////////////////////////////////////
  // To int data muxing
  ////////////////////////////////////////////////////////////////////////////////

  always_comb begin
    f8_toint_data = {59'b0, f8_toint_data_pre};

    if (f8_mova_x_m)
      f8_toint_data = ex_mask_rf[f8_thread_id];
  end


  ////////////////////////////////////////////////////////////////////////////////
  // mask scoreboard going to intpipe
  ////////////////////////////////////////////////////////////////////////////////

  // mask scoreboard
  always_comb begin
    // f5
    wb_mask_valid[0]          = |f5_regmask_store;
    wb_mask_dest[0].addr      = f5_regmask_waddr;
    wb_mask_dest[0].thread_id = f5_thread_id;

    // f6
    wb_mask_valid[1]          = |f6_regmask_store;
    wb_mask_dest[1].addr      = f6_regmask_waddr;
    wb_mask_dest[1].thread_id = f6_thread_id;

    // f7
    wb_mask_valid[2]          = |f7_regmask_store;
    wb_mask_dest[2].addr      = f7_regmask_waddr;
    wb_mask_dest[2].thread_id = f7_thread_id;

    // f8
    wb_mask_valid[3]          = |f8_regmask_store;
    wb_mask_dest[3].addr      = f8_regmask_waddr;
    wb_mask_dest[3].thread_id = f8_thread_id;

  end

endmodule


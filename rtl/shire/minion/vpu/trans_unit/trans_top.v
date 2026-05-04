// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module trans_top (
  // System signals
  input  logic                     clock,
  input  logic                     reset,
  // EX request
  input  logic                     ex_valid,
  input  vpu_input_t               ex_in_bits,
  input  logic [`VPU_FLAGS_SZ-1:0] ex_txfma_exc,
  // F6 results
  output trans_fma_coefficients    f8_rom_response
);

  logic f2_trans_valid;
  logic f3_trans_valid;
  logic f4_trans_valid;
  logic f5_trans_valid;
  logic f6_trans_valid;
  logic f7_trans_valid;

  logic [`VPU_DATA_S_SZ-1:0] f2_rom_raddr;
  logic [`VPU_FCMD_SZ-1:0]   f2_rom_cmd;

`ifdef ENABLE_EXTRA_TRANS
  logic [`TRANS_SIN_ADDR_SIZE-1:0] f2_sin_addr;
  logic                            f2_sin_taylor;
  logic                            f3_sin_taylor;
  logic                            f4_sin_taylor;
  logic                            f5_sin_taylor;
`endif 

  trans_rom_ctrl f2_rom_ctrl;
  trans_rom_ctrl f3_rom_ctrl;
  trans_rom_ctrl f4_rom_ctrl;
  trans_rom_ctrl f5_rom_ctrl;

  trans_rom_index f5_rom_rcp1_index;
  trans_rom_index f5_rom_rcp2_index;
`ifdef ENABLE_EXTRA_TRANS
  trans_rom_index f5_rom_rsqrt1_index;
  trans_rom_index f5_rom_rsqrt2_index;
`endif
  trans_rom_index f5_rom_log1_index;
  trans_rom_index f5_rom_log2_index;
  trans_rom_index f5_rom_exp1_index;
  trans_rom_index f5_rom_exp2_index;
`ifdef ENABLE_EXTRA_TRANS
  trans_rom_index f5_rom_sin1_index;
  trans_rom_index f5_rom_sin2_index;
`endif

  trans_fma_coefficients f5_rom_rcp1_coefs;
  trans_fma_coefficients f5_rom_rcp2_coefs;
`ifdef ENABLE_EXTRA_TRANS
  trans_fma_coefficients f5_rom_rsqrt1_coefs;
  trans_fma_coefficients f5_rom_rsqrt2_coefs;
`endif
  trans_fma_coefficients f5_rom_log1_coefs;
  trans_fma_coefficients f5_rom_log2_coefs;
  trans_fma_coefficients f5_rom_exp1_coefs;
  trans_fma_coefficients f5_rom_exp2_coefs;
`ifdef ENABLE_EXTRA_TRANS
  trans_fma_coefficients f5_rom_sin1_coefs;
  trans_fma_coefficients f5_rom_sin2_coefs;
`endif
  trans_fma_coefficients f5_rom_response;
  trans_fma_coefficients f6_rom_response;
  trans_fma_coefficients f7_rom_response;

  logic ex_exc_int, f2_exc, f3_exc, f4_exc, f5_exc, f6_exc, f7_exc, f8_exc;

  always_comb begin
    ex_exc_int = (ex_in_bits.sigs.cmd == `VPU_TRANS_EXP_RR) ? (|ex_txfma_exc) : (f8_exc);
  end

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////


  //      CLK    RST    EN        DOUT            DIN                  DEF
  `RST_FF(clock, reset,           f2_trans_valid, ex_valid,            1'b0)
  `EN_FF (clock,        ex_valid, f2_rom_raddr,   ex_in_bits.in2)
  `EN_FF (clock,        ex_valid, f2_rom_cmd,     ex_in_bits.sigs.cmd)
  `EN_FF (clock,        ex_valid, f2_exc,         ex_exc_int)

`ifdef ENABLE_EXTRA_TRANS
  always_comb begin
    case (f2_rom_raddr[30:23])
      8'd124:  f2_sin_addr = {1'b1, f2_rom_raddr[22:18]};
      8'd123:  f2_sin_addr = {1'b0, 1'b1, f2_rom_raddr[22:19]};
      8'd122:  f2_sin_addr = {2'b0, 1'b1, f2_rom_raddr[22:20]};
      8'd121:  f2_sin_addr = {4'b0, 1'b1, f2_rom_raddr[22:22]};
      default: f2_sin_addr = 6'b0;
    endcase

    f2_sin_taylor = f2_rom_raddr[30:23] < 8'd120;
  end
`endif

  always_comb begin
    case (f2_rom_cmd)
      `VPU_TRANS_RCP_RR:      f2_rom_ctrl.cmd = TRANS_ROM_RCP1_ID;
      `VPU_TRANS_RCP_FMA1:    f2_rom_ctrl.cmd = TRANS_ROM_RCP2_ID;
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_RSQRT_RR:    f2_rom_ctrl.cmd = TRANS_ROM_RSQRT1_ID;
      `VPU_TRANS_RSQRT_FMA1:  f2_rom_ctrl.cmd = TRANS_ROM_RSQRT2_ID;
`endif
      `VPU_TRANS_LOG_RR:      f2_rom_ctrl.cmd = TRANS_ROM_LOG1_ID;
      `VPU_TRANS_LOG_FMA1:    f2_rom_ctrl.cmd = TRANS_ROM_LOG2_ID;
      `VPU_TRANS_EXP_RR:      f2_rom_ctrl.cmd = TRANS_ROM_EXP1_ID;
      `VPU_TRANS_EXP_FMA1:    f2_rom_ctrl.cmd = TRANS_ROM_EXP2_ID;
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_SIN_RR:      f2_rom_ctrl.cmd = TRANS_ROM_SIN1_ID;
      `VPU_TRANS_SIN_P1:      f2_rom_ctrl.cmd = TRANS_ROM_SIN2_ID;
      `VPU_TRANS_SIN_P2:      f2_rom_ctrl.cmd = TRANS_ROM_SIN3_ID;
`endif
      default:                f2_rom_ctrl.cmd = TRANS_ROM_NONE_ID;
    endcase

    case (f2_rom_cmd)
      `VPU_TRANS_RCP_RR:      f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_RCP_ADDR_RANGE, f2_rom_raddr);
      `VPU_TRANS_RCP_FMA1:    f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_RCP_ADDR_RANGE, f2_rom_raddr);
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_RSQRT_RR:    f2_rom_ctrl.index = f2_rom_raddr[`TRANS_RSQRT_ADDR_RANGE];
      `VPU_TRANS_RSQRT_FMA1:  f2_rom_ctrl.index = f2_rom_raddr[`TRANS_RSQRT_ADDR_RANGE];
`endif
      `VPU_TRANS_LOG_RR:      f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_LOG_ADDR_RANGE, f2_rom_raddr);
      `VPU_TRANS_LOG_FMA1:    f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_LOG_ADDR_RANGE, f2_rom_raddr);
      `VPU_TRANS_EXP_RR:      f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_EXP_ADDR_RANGE, f2_rom_raddr);
      `VPU_TRANS_EXP_FMA1:    f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_EXP_ADDR_RANGE, f2_rom_raddr);
`ifdef ENABLE_EXTRA_TRANS
      `VPU_TRANS_SIN_RR:      f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_SIN_ADDR_RANGE, f2_sin_addr);
      `VPU_TRANS_SIN_P1:      f2_rom_ctrl.index = `TRANS_ROM_SET_INDEX(`TRANS_SIN_ADDR_RANGE, f2_sin_addr);
`endif
      default:                f2_rom_ctrl.index = 'b0;
    endcase
  end

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F3 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN              DOUT            DIN             DEF
  `RST_FF(clock, reset,                 f3_trans_valid, f2_trans_valid, 1'b0)
  `EN_FF (clock,        f2_trans_valid, f3_rom_ctrl,    f2_rom_ctrl)
  `EN_FF (clock,        f2_trans_valid, f3_exc,         f2_exc)
`ifdef ENABLE_EXTRA_TRANS
  `EN_FF (clock,        f2_trans_valid, f3_sin_taylor,  f2_sin_taylor)
`endif

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F4 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN              DOUT            DIN             DEF
  `RST_FF(clock, reset,                 f4_trans_valid, f3_trans_valid, 1'b0)
  `EN_FF (clock,        f3_trans_valid, f4_rom_ctrl,    f3_rom_ctrl)
  `EN_FF (clock,        f3_trans_valid, f4_exc,         f3_exc)
`ifdef ENABLE_EXTRA_TRANS
  `EN_FF (clock,        f3_trans_valid, f4_sin_taylor,  f3_sin_taylor)
`endif

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F5 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN              DOUT            DIN             DEF
  `RST_FF(clock, reset,                 f5_trans_valid, f4_trans_valid, 1'b0)
  `EN_FF (clock,        f4_trans_valid, f5_rom_ctrl,    f4_rom_ctrl)
  `EN_FF (clock,        f4_trans_valid, f5_exc,         f4_exc)
`ifdef ENABLE_EXTRA_TRANS
  `EN_FF (clock,        f4_trans_valid, f5_sin_taylor,  f4_sin_taylor)
`endif

  always_comb begin
    f5_rom_rcp1_index = 'b0;
    f5_rom_rcp2_index = 'b0;
`ifdef ENABLE_EXTRA_TRANS
    f5_rom_rsqrt1_index = 'b0;
    f5_rom_rsqrt2_index = 'b0;
`endif
    f5_rom_log1_index = 'b0;
    f5_rom_log2_index = 'b0;
    f5_rom_exp1_index = 'b0;
    f5_rom_exp2_index = 'b0;
`ifdef ENABLE_EXTRA_TRANS
    f5_rom_sin1_index = 'b0;
    f5_rom_sin2_index = 'b0;
`endif

    case (f5_rom_ctrl.cmd)
      TRANS_ROM_RCP1_ID:    f5_rom_rcp1_index = f5_rom_ctrl.index;
      TRANS_ROM_RCP2_ID:    f5_rom_rcp2_index = f5_rom_ctrl.index;
`ifdef ENABLE_EXTRA_TRANS
      TRANS_ROM_RSQRT1_ID:  f5_rom_rsqrt1_index = f5_rom_ctrl.index;
      TRANS_ROM_RSQRT2_ID:  f5_rom_rsqrt2_index = f5_rom_ctrl.index;
`endif
      TRANS_ROM_LOG1_ID:    f5_rom_log1_index = f5_rom_ctrl.index;
      TRANS_ROM_LOG2_ID:    f5_rom_log2_index = f5_rom_ctrl.index;
      TRANS_ROM_EXP1_ID:    f5_rom_exp1_index = f5_rom_ctrl.index;
      TRANS_ROM_EXP2_ID:    f5_rom_exp2_index = f5_rom_ctrl.index;
`ifdef ENABLE_EXTRA_TRANS
      TRANS_ROM_SIN1_ID:    f5_rom_sin1_index = f5_rom_ctrl.index;
      TRANS_ROM_SIN2_ID:    f5_rom_sin2_index = f5_rom_ctrl.index;
`endif
      default:              f5_rom_rcp1_index = '0;
    endcase
  end

  //RCP ROMs
  trans_rcp_rom_case_c1c2 trans_rcp_rom_c1c2 (
    .index (f5_rom_rcp1_index),
    .data  (f5_rom_rcp1_coefs)
  );

  trans_rcp_rom_case_c0 trans_rcp_rom_c0 (
    .index (f5_rom_rcp2_index),
    .data  (f5_rom_rcp2_coefs)
  );

`ifdef ENABLE_EXTRA_TRANS
  //RSQ ROMs
  trans_rsqrt_rom_case_c1c2 trans_rsqrt_rom_c1c2 (
    .index (f5_rom_rsqrt1_index),
    .data  (f5_rom_rsqrt1_coefs)
  );

  trans_rsqrt_rom_case_c0 trans_rsqrt_rom_c0 (
    .index (f5_rom_rsqrt2_index),
    .data  (f5_rom_rsqrt2_coefs)
  );
`endif

  //LOG ROMs
  trans_log_rom_case_c1c2 trans_log_rom_c1c2 (
    .index (f5_rom_log1_index),
    .data  (f5_rom_log1_coefs)
  );

  trans_log_rom_case_c0 trans_log_rom_c0 (
    .index (f5_rom_log2_index),
    .data  (f5_rom_log2_coefs)
  );

  //EXP ROMs
  trans_exp_rom_case_c1c2 trans_exp_rom_c1c2 (
    .index (f5_rom_exp1_index),
    .data  (f5_rom_exp1_coefs)
  );

  trans_exp_rom_case_c0 trans_exp_rom_c0 (
    .index (f5_rom_exp2_index),
    .data  (f5_rom_exp2_coefs)
  );

`ifdef ENABLE_EXTRA_TRANS
  //SIN ROMs
  trans_sin_rom_case_c1c2 trans_sin_rom_c1c2 (
    .index (f5_rom_sin1_index),
    .data  (f5_rom_sin1_coefs)
  );

  trans_sin_rom_case_c0 trans_sin_rom_c0 (
    .index (f5_rom_sin2_index),
    .data  (f5_rom_sin2_coefs)
  );
`endif

  //rom data sel
  always_comb begin

    case (f5_rom_ctrl.cmd)
      TRANS_ROM_RCP1_ID:   f5_rom_response = f5_rom_rcp1_coefs;
      TRANS_ROM_RCP2_ID:   f5_rom_response = f5_rom_rcp2_coefs;
`ifdef ENABLE_EXTRA_TRANS
      TRANS_ROM_RSQRT1_ID: f5_rom_response = f5_rom_rsqrt1_coefs;
      TRANS_ROM_RSQRT2_ID: f5_rom_response = f5_rom_rsqrt2_coefs;
`endif
      TRANS_ROM_LOG1_ID:   f5_rom_response = f5_rom_log1_coefs;
      TRANS_ROM_LOG2_ID:   f5_rom_response = f5_rom_log2_coefs;
      TRANS_ROM_EXP1_ID:   f5_rom_response = f5_rom_exp1_coefs;
      TRANS_ROM_EXP2_ID: begin
        f5_rom_response     = f5_rom_exp2_coefs;
        f5_rom_response.exc = f5_exc;
      end
`ifdef ENABLE_EXTRA_TRANS
      TRANS_ROM_SIN1_ID: begin
        f5_rom_response        = f5_rom_sin1_coefs;
        f5_rom_response.taylor = f5_sin_taylor;
      end
      TRANS_ROM_SIN2_ID: begin
        f5_rom_response        = f5_sin_taylor ? 32'b11001001000011111101101100000000 : f5_rom_sin2_coefs;
        f5_rom_response.taylor = f5_sin_taylor;
      end
      TRANS_ROM_SIN3_ID: begin
        f5_rom_response.taylor = f5_sin_taylor;
      end
`endif
      default: f5_rom_response = 'b0;
    endcase
  end

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F6 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN              DOUT             DIN             DEF
  `RST_FF(clock, reset,                 f6_trans_valid,  f5_trans_valid, 1'b0)
  `EN_FF(clock,         f5_trans_valid, f6_rom_response, f5_rom_response)
  `EN_FF(clock,         f5_trans_valid, f6_exc,          f5_exc)


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F7 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //      CLK    RST    EN              DOUT             DIN             DEF
  `RST_FF(clock, reset,                 f7_trans_valid,  f6_trans_valid, 1'b0)
  `EN_FF(clock,         f6_trans_valid, f7_rom_response, f6_rom_response)
  `EN_FF(clock,         f6_trans_valid, f7_exc,          f6_exc)


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // F8 Stage
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  //     CLK    EN              DOUT             DIN
  `EN_FF(clock, f7_trans_valid, f8_rom_response, f7_rom_response)
  `EN_FF(clock, f7_trans_valid, f8_exc,          f7_exc)

endmodule

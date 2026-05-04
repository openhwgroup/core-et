// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Sub-bank memory interface.
//
// Instantiates the tag RAM, tag state RAM, and data RAM wrappers.
// Implements the _tap/_dap -> _ta/_da input pipeline register stage
// (1 cycle latency before RAM) and the response valid/data pipeline
// (variable 2/3/4 cycle delay based on esr_ram_delay).
//
// Behaviorally equivalent to shire_cache_pipe_sub_bank_mem.

// DataRamAddrSize includes way bits; per-QW RAM addr is narrower -- top bits unused.
/* verilator lint_off UNUSEDSIGNAL */  // DataRamAddrSize wider than per-QW RAM address; upper bits intentionally unused
/* verilator lint_off WIDTHTRUNC */  // data RAM address intentionally truncated to per-QW RAM width
module shirecache_pipe_sub_bank_mem
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
#(
  parameter int unsigned SetsPerSubBank = 256,
  parameter bit          TagStateDualPort = 1'b1
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // -- RAM delay configuration --
  input  logic [RamDelaySize-1:0]           esr_ram_delay_i,

  // -- Tag state RAM request --
  input  logic                              ts_rd_en_i,
  input  logic                              ts_wr_en_i,
  input  logic [$clog2(SetsPerSubBank)-1:0] ts_rd_addr_i,
  input  logic [$clog2(SetsPerSubBank)-1:0] ts_wr_addr_i,
  input  logic [TagStateRamDataSize-1:0]    ts_wr_data_i,
  input  logic [DvTransIdSize-1:0]          ts_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             ts_rd_reqq_id_i,

  // -- Tag state RAM response --
  output logic                              ts_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          ts_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             ts_rsp_reqq_id_o,
  output logic [TagStateRamDataSize-1:0]    ts_rsp_data_o,

  // -- Tag RAM request --
  input  logic                              t_rd_en_i,
  input  logic                              t_wr_en_i,
  input  logic [$clog2(SetsPerSubBank)-1:0] t_addr_i,
  input  logic [TagRamDataSize-1:0]         t_wr_data_i,
  input  logic [DvTransIdSize-1:0]          t_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             t_rd_reqq_id_i,

  // -- Tag RAM response --
  output logic                              t_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          t_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             t_rsp_reqq_id_o,
  output logic [TagRamDataSize-1:0]         t_rsp_data_o,

  // -- Data RAM request --
  input  logic                              d_rd_en_i,
  input  logic                              d_wr_en_i,
  input  logic [DataRamAddrSize-1:0]        d_addr_i,
  input  logic [DataRamDataSize-1:0]        d_wr_data_i,
  input  logic [LineQwSize-1:0]             d_wr_qwen_i,
  input  logic [DvTransIdSize-1:0]          d_rd_trans_id_i,
  input  logic [ReqqIdSize-1:0]             d_rd_reqq_id_i,

  // -- Data RAM response --
  output logic                              d_rsp_valid_o,
  output logic [DvTransIdSize-1:0]          d_rsp_trans_id_o,
  output logic [ReqqIdSize-1:0]             d_rsp_reqq_id_o,
  output logic [DataRamDataSize-1:0]        d_rsp_data_o,

  // -- Technology hooks --
  input  logic                              dft_sram_clk_i,
  input  ram_cfg_t                          ram_cfg_i,
  input  dft_t                              dft_i
);

  localparam int unsigned TsAddrSize = $clog2(SetsPerSubBank);
  localparam int unsigned TAddrSize  = $clog2(SetsPerSubBank);
  localparam int unsigned RamDly     = 4; // max RAM cycles of delay

  // ================================================================
  // Combinational _tap/_dap aliases (inputs renamed)
  // ================================================================
  // The caller already provides flattened, muxed fields.
  // These aliases match the original's _tap/_dap naming.

  // -- Tag state --
  logic                              ts_rd_en_tap;
  logic                              ts_wr_en_tap;
  logic                              ts_clock_en_tap;
  logic [DvTransIdSize-1:0]          ts_trans_id_tap;
  logic [ReqqIdSize-1:0]             ts_reqq_id_tap;
  logic [TsAddrSize-1:0]             ts_rd_addr_tap;
  logic [TsAddrSize-1:0]             ts_wr_addr_tap;
  logic [TagStateRamDataSize-1:0]    ts_wr_data_tap;

  assign ts_rd_en_tap    = ts_rd_en_i;
  assign ts_wr_en_tap    = ts_wr_en_i;
  assign ts_clock_en_tap = ts_rd_en_i | ts_wr_en_i;
  assign ts_trans_id_tap = ts_rd_trans_id_i;
  assign ts_reqq_id_tap  = ts_rd_reqq_id_i;
  assign ts_rd_addr_tap  = ts_rd_addr_i;
  assign ts_wr_addr_tap  = ts_wr_addr_i;
  assign ts_wr_data_tap  = ts_wr_data_i;

  // -- Tag --
  logic                              t_rd_en_tap;
  logic                              t_wr_en_tap;
  logic                              t_clock_en_tap;
  logic [DvTransIdSize-1:0]          t_trans_id_tap;
  logic [ReqqIdSize-1:0]             t_reqq_id_tap;
  logic [TAddrSize-1:0]              t_addr_tap;
  logic [TagRamDataSize-1:0]         t_wr_data_tap;

  assign t_rd_en_tap    = t_rd_en_i;
  assign t_wr_en_tap    = t_wr_en_i;
  assign t_clock_en_tap = t_rd_en_i | t_wr_en_i;
  assign t_trans_id_tap = t_rd_trans_id_i;
  assign t_reqq_id_tap  = t_rd_reqq_id_i;
  assign t_addr_tap     = t_addr_i;
  assign t_wr_data_tap  = t_wr_data_i;

  // -- Data --
  logic                              d_rd_en_tap;
  logic                              d_wr_en_tap;
  logic                              d_clock_en_tap;
  logic [DvTransIdSize-1:0]          d_trans_id_tap;
  logic [ReqqIdSize-1:0]             d_reqq_id_tap;
  logic [DataRamAddrSize-1:0]        d_addr_tap;
  logic [DataRamDataSize-1:0]        d_wr_data_tap;
  logic [LineQwSize-1:0]             d_wr_qwen_tap;

  assign d_rd_en_tap    = d_rd_en_i;
  assign d_wr_en_tap    = d_wr_en_i;
  assign d_clock_en_tap = d_rd_en_i | d_wr_en_i;
  assign d_trans_id_tap = d_rd_trans_id_i;
  assign d_reqq_id_tap  = d_rd_reqq_id_i;
  assign d_addr_tap     = d_addr_i;
  assign d_wr_data_tap  = d_wr_data_i;
  assign d_wr_qwen_tap  = d_wr_qwen_i;

  // ================================================================
  // Input pipeline registers: _tap/_dap -> _ta/_da  (1 cycle latency)
  // ================================================================
  // RST_EN_FF for clock_en, rd_en, wr_en (reset to 0).
  // EN_FF for trans_id, reqq_id, addr, wr_data (gated by clock_en_tap).

  // -- Tag state _ta registers --
  logic                              ts_clock_en_ta;
  logic                              ts_rd_en_ta;
  logic                              ts_wr_en_ta;
  logic [DvTransIdSize-1:0]          ts_trans_id_ta;
  logic [ReqqIdSize-1:0]             ts_reqq_id_ta;
  logic [TsAddrSize-1:0]             ts_rd_addr_ta;
  logic [TsAddrSize-1:0]             ts_wr_addr_ta;
  logic [TagStateRamDataSize-1:0]    ts_wr_data_ta;

  // RST_EN_FF: clock_en_ta (en = clock_en_ta || clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) ts_clock_en_ta <= 1'b0;
    else if (ts_clock_en_ta | ts_clock_en_tap) ts_clock_en_ta <= ts_clock_en_tap;
  end

  // RST_EN_FF: rd_en_ta, wr_en_ta (en = clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ts_rd_en_ta <= 1'b0;
      ts_wr_en_ta <= 1'b0;
    end else if (ts_clock_en_tap) begin
      ts_rd_en_ta <= ts_rd_en_tap;
      ts_wr_en_ta <= ts_wr_en_tap;
    end
  end

  // EN_FF: trans_id, reqq_id (en = clock_en_tap)
  always_ff @(posedge clk_i) begin
    if (ts_clock_en_tap) begin
      ts_trans_id_ta <= ts_trans_id_tap;
      ts_reqq_id_ta  <= ts_reqq_id_tap;
    end
  end

  // Dual-port: separate rd/wr addr with individual enables
  // Single-port: single muxed addr
  generate
    if (TagStateDualPort) begin : gen_ts_dp
      // EN_FF: rd_addr (en = clock_en_tap && rd_en_tap)
      always_ff @(posedge clk_i) begin
        if (ts_clock_en_tap & ts_rd_en_tap) ts_rd_addr_ta <= ts_rd_addr_tap;
      end
      // EN_FF: wr_addr (en = clock_en_tap && wr_en_tap)
      always_ff @(posedge clk_i) begin
        if (ts_clock_en_tap & ts_wr_en_tap) ts_wr_addr_ta <= ts_wr_addr_tap;
      end
    end else begin : gen_ts_sp
      // Single-port: addr is already muxed by caller, use rd_addr_ta as the single addr
      // EN_FF: addr (en = clock_en_tap)
      always_ff @(posedge clk_i) begin
        if (ts_clock_en_tap) ts_rd_addr_ta <= ts_rd_addr_tap;
      end
      assign ts_wr_addr_ta = ts_rd_addr_ta; // unused in SP mode, tie off
    end
  endgenerate

  // EN_FF: wr_data (en = clock_en_tap && wr_en_tap)
  always_ff @(posedge clk_i) begin
    if (ts_clock_en_tap & ts_wr_en_tap) ts_wr_data_ta <= ts_wr_data_tap;
  end

  // -- Tag _ta registers --
  logic                              t_clock_en_ta;
  logic                              t_rd_en_ta;
  logic                              t_wr_en_ta;
  logic [DvTransIdSize-1:0]          t_trans_id_ta;
  logic [ReqqIdSize-1:0]             t_reqq_id_ta;
  logic [TAddrSize-1:0]              t_addr_ta;
  logic [TagRamDataSize-1:0]         t_wr_data_ta;

  // RST_EN_FF: clock_en_ta (en = clock_en_ta || clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) t_clock_en_ta <= 1'b0;
    else if (t_clock_en_ta | t_clock_en_tap) t_clock_en_ta <= t_clock_en_tap;
  end

  // RST_EN_FF: rd_en_ta, wr_en_ta (en = clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      t_rd_en_ta <= 1'b0;
      t_wr_en_ta <= 1'b0;
    end else if (t_clock_en_tap) begin
      t_rd_en_ta <= t_rd_en_tap;
      t_wr_en_ta <= t_wr_en_tap;
    end
  end

  // EN_FF: trans_id, reqq_id, addr (en = clock_en_tap)
  always_ff @(posedge clk_i) begin
    if (t_clock_en_tap) begin
      t_trans_id_ta <= t_trans_id_tap;
      t_reqq_id_ta  <= t_reqq_id_tap;
      t_addr_ta     <= t_addr_tap;
    end
  end

  // EN_FF: wr_data (en = clock_en_tap && wr_en_tap)
  always_ff @(posedge clk_i) begin
    if (t_clock_en_tap & t_wr_en_tap) t_wr_data_ta <= t_wr_data_tap;
  end

  // -- Data _da registers --
  logic                              d_clock_en_da;
  logic                              d_rd_en_da;
  logic                              d_wr_en_da;
  logic [DvTransIdSize-1:0]          d_trans_id_da;
  logic [ReqqIdSize-1:0]             d_reqq_id_da;
  logic [DataRamAddrSize-1:0]        d_addr_da;
  logic [DataRamDataSize-1:0]        d_wr_data_da;
  logic [LineQwSize-1:0]             d_wr_qwen_da;

  // RST_EN_FF: clock_en_da (en = clock_en_da || clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) d_clock_en_da <= 1'b0;
    else if (d_clock_en_da | d_clock_en_tap) d_clock_en_da <= d_clock_en_tap;
  end

  // RST_EN_FF: rd_en_da, wr_en_da (en = clock_en_tap)
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      d_rd_en_da <= 1'b0;
      d_wr_en_da <= 1'b0;
    end else if (d_clock_en_tap) begin
      d_rd_en_da <= d_rd_en_tap;
      d_wr_en_da <= d_wr_en_tap;
    end
  end

  // EN_FF: trans_id, reqq_id, addr (en = clock_en_tap)
  always_ff @(posedge clk_i) begin
    if (d_clock_en_tap) begin
      d_trans_id_da <= d_trans_id_tap;
      d_reqq_id_da  <= d_reqq_id_tap;
      d_addr_da     <= d_addr_tap;
    end
  end

  // EN_FF: wr_data, wr_qwen (en = clock_en_tap && wr_en_tap)
  always_ff @(posedge clk_i) begin
    if (d_clock_en_tap & d_wr_en_tap) begin
      d_wr_data_da <= d_wr_data_tap;
      d_wr_qwen_da <= d_wr_qwen_tap;
    end
  end

  // ================================================================
  // RAM wrappers (connected to _ta/_da registered signals)
  // ================================================================

  logic [TagStateRamDataSize-1:0] ts_rd_data_ta_dn;

  shirecache_pipe_tag_state_ram_wrap #(
    .SetsPerSubBank (SetsPerSubBank),
    .DualPort       (TagStateDualPort)
  ) u_tag_state_ram (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .clock_en_i  (ts_clock_en_ta),
    .ram_delay_i (esr_ram_delay_i),
    .rd_en_i     (ts_rd_en_ta),
    .wr_en_i     (ts_wr_en_ta),
    .rd_addr_i   (ts_rd_addr_ta),
    .wr_addr_i   (ts_wr_addr_ta),
    .wr_data_i   (ts_wr_data_ta),
    .rd_data_o   (ts_rd_data_ta_dn),
    .ram_cfg_i,
    .dft_i
  );

  logic [TagRamDataSize-1:0] t_rd_data_ta_dn;

  shirecache_pipe_tag_ram_wrap #(
    .SetsPerSubBank (SetsPerSubBank)
  ) u_tag_ram (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .clock_en_i  (t_clock_en_ta),
    .ram_delay_i (esr_ram_delay_i),
    .rd_en_i     (t_rd_en_ta),
    .wr_en_i     (t_wr_en_ta),
    .addr_i      (t_addr_ta),
    .wr_data_i   (t_wr_data_ta),
    .rd_data_o   (t_rd_data_ta_dn),
    .ram_cfg_i,
    .dft_i
  );

  logic [DataRamDataSize-1:0] d_rd_data_da_dn;

  shirecache_pipe_data_ram_wrap #(
    .SetsPerSubBank (SetsPerSubBank)
  ) u_data_ram (
    .clk_i,
    .rst_ni,
    .dft_sram_clk_i,
    .clock_en_i  (d_clock_en_da),
    .ram_delay_i (esr_ram_delay_i),
    .rd_en_i     (d_rd_en_da),
    .wr_en_i     (d_wr_en_da),
    .addr_i      (d_addr_da),
    .wr_data_i   (d_wr_data_da),
    .wr_qwen_i   (d_wr_qwen_da),
    .rd_data_o   (d_rd_data_da_dn),
    .ram_cfg_i,
    .dft_i
  );

  // ================================================================
  // Response valid pipeline (shift register, variable delay)
  // ================================================================
  // d0 uses _ta/_da REGISTERED signals.
  // Shift register vec[3:0] with RST_EN_FF, then select output by delay.

  logic ram_delay_2, ram_delay_3, ram_delay_4;
  assign ram_delay_2 = (esr_ram_delay_i == RamDelaySize'(2));
  assign ram_delay_3 = (esr_ram_delay_i == RamDelaySize'(3));
  assign ram_delay_4 = (esr_ram_delay_i == RamDelaySize'(4));

  // ================================================================
  // Tag state response pipeline
  // ================================================================

  // d0 from registered signals
  logic ts_rd_valid_d0;
  assign ts_rd_valid_d0 = ts_clock_en_ta & ts_rd_en_ta;

  // Valid shift register: RST_EN_FF
  logic [RamDly-1:0] ts_valid_vec_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) ts_valid_vec_q <= '0;
    else if (|{ts_valid_vec_q, ts_rd_valid_d0})
      ts_valid_vec_q <= {ts_valid_vec_q[RamDly-2:0], ts_rd_valid_d0};
  end

  logic ts_valid_d1, ts_valid_d2, ts_valid_d3, ts_valid_d4;
  assign ts_valid_d1 = ts_valid_vec_q[0];
  assign ts_valid_d2 = ts_valid_vec_q[1];
  assign ts_valid_d3 = ts_valid_vec_q[2];
  assign ts_valid_d4 = ts_valid_vec_q[3];

  // Trans_id / reqq_id pipeline: EN_FF stages gated by valid
  logic [DvTransIdSize-1:0] ts_trans_id_d0;
  logic [ReqqIdSize-1:0]    ts_reqq_id_d0;
  assign ts_trans_id_d0 = ts_trans_id_ta;
  assign ts_reqq_id_d0  = ts_reqq_id_ta;

  logic [DvTransIdSize-1:0] ts_trans_id_d1, ts_trans_id_d2, ts_trans_id_d3, ts_trans_id_d4;
  logic [ReqqIdSize-1:0]    ts_reqq_id_d1,  ts_reqq_id_d2,  ts_reqq_id_d3,  ts_reqq_id_d4;

  always_ff @(posedge clk_i) begin
    if (ts_rd_valid_d0) begin
      ts_trans_id_d1 <= ts_trans_id_d0;
      ts_reqq_id_d1  <= ts_reqq_id_d0;
    end
    if (ts_valid_d1) begin
      ts_trans_id_d2 <= ts_trans_id_d1;
      ts_reqq_id_d2  <= ts_reqq_id_d1;
    end
    if (ts_valid_d2) begin
      ts_trans_id_d3 <= ts_trans_id_d2;
      ts_reqq_id_d3  <= ts_reqq_id_d2;
    end
    if (ts_valid_d3) begin
      ts_trans_id_d4 <= ts_trans_id_d3;
      ts_reqq_id_d4  <= ts_reqq_id_d3;
    end
  end

  // Select output based on DualPort mode and ram_delay
  logic                           ts_rsp_valid_sel;
  logic [DvTransIdSize-1:0]       ts_rsp_trans_id_sel;
  logic [ReqqIdSize-1:0]          ts_rsp_reqq_id_sel;

  generate
    if (TagStateDualPort) begin : gen_ts_rsp_dp
      // Variable delay: select d2/d3/d4 based on esr_ram_delay
      assign ts_rsp_valid_sel =
        (ram_delay_2 & ts_valid_d2) |
        (ram_delay_3 & ts_valid_d3) |
        (ram_delay_4 & ts_valid_d4);

      assign ts_rsp_trans_id_sel =
        ({DvTransIdSize{ram_delay_2}} & ts_trans_id_d2) |
        ({DvTransIdSize{ram_delay_3}} & ts_trans_id_d3) |
        ({DvTransIdSize{ram_delay_4}} & ts_trans_id_d4);

      assign ts_rsp_reqq_id_sel =
        ({ReqqIdSize{ram_delay_2}} & ts_reqq_id_d2) |
        ({ReqqIdSize{ram_delay_3}} & ts_reqq_id_d3) |
        ({ReqqIdSize{ram_delay_4}} & ts_reqq_id_d4);
    end else begin : gen_ts_rsp_sp
      // Single-port: constant 1-cycle delay
      assign ts_rsp_valid_sel    = ts_valid_d1;
      assign ts_rsp_trans_id_sel = ts_trans_id_d1;
      assign ts_rsp_reqq_id_sel  = ts_reqq_id_d1;
    end
  endgenerate

  // Response data gating: zero output when not valid
  assign ts_rsp_valid_o    = ts_rsp_valid_sel;
  assign ts_rsp_trans_id_o = {DvTransIdSize{ts_rsp_valid_sel}}       & ts_rsp_trans_id_sel;
  assign ts_rsp_reqq_id_o  = {ReqqIdSize{ts_rsp_valid_sel}}          & ts_rsp_reqq_id_sel;
  assign ts_rsp_data_o     = {TagStateRamDataSize{ts_rsp_valid_sel}} & ts_rd_data_ta_dn;

  // ================================================================
  // Tag response pipeline
  // ================================================================

  logic t_rd_valid_d0;
  assign t_rd_valid_d0 = t_clock_en_ta & t_rd_en_ta;

  logic [RamDly-1:0] t_valid_vec_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) t_valid_vec_q <= '0;
    else if (|{t_valid_vec_q, t_rd_valid_d0})
      t_valid_vec_q <= {t_valid_vec_q[RamDly-2:0], t_rd_valid_d0};
  end

  logic t_valid_d1, t_valid_d2, t_valid_d3, t_valid_d4;
  assign t_valid_d1 = t_valid_vec_q[0];
  assign t_valid_d2 = t_valid_vec_q[1];
  assign t_valid_d3 = t_valid_vec_q[2];
  assign t_valid_d4 = t_valid_vec_q[3];

  logic [DvTransIdSize-1:0] t_trans_id_d0;
  logic [ReqqIdSize-1:0]    t_reqq_id_d0;
  assign t_trans_id_d0 = t_trans_id_ta;
  assign t_reqq_id_d0  = t_reqq_id_ta;

  logic [DvTransIdSize-1:0] t_trans_id_d1, t_trans_id_d2, t_trans_id_d3, t_trans_id_d4;
  logic [ReqqIdSize-1:0]    t_reqq_id_d1,  t_reqq_id_d2,  t_reqq_id_d3,  t_reqq_id_d4;

  always_ff @(posedge clk_i) begin
    if (t_rd_valid_d0) begin
      t_trans_id_d1 <= t_trans_id_d0;
      t_reqq_id_d1  <= t_reqq_id_d0;
    end
    if (t_valid_d1) begin
      t_trans_id_d2 <= t_trans_id_d1;
      t_reqq_id_d2  <= t_reqq_id_d1;
    end
    if (t_valid_d2) begin
      t_trans_id_d3 <= t_trans_id_d2;
      t_reqq_id_d3  <= t_reqq_id_d2;
    end
    if (t_valid_d3) begin
      t_trans_id_d4 <= t_trans_id_d3;
      t_reqq_id_d4  <= t_reqq_id_d3;
    end
  end

  // Variable delay select
  logic                     t_rsp_valid_sel;
  logic [DvTransIdSize-1:0] t_rsp_trans_id_sel;
  logic [ReqqIdSize-1:0]    t_rsp_reqq_id_sel;

  assign t_rsp_valid_sel =
    (ram_delay_2 & t_valid_d2) |
    (ram_delay_3 & t_valid_d3) |
    (ram_delay_4 & t_valid_d4);

  assign t_rsp_trans_id_sel =
    ({DvTransIdSize{ram_delay_2}} & t_trans_id_d2) |
    ({DvTransIdSize{ram_delay_3}} & t_trans_id_d3) |
    ({DvTransIdSize{ram_delay_4}} & t_trans_id_d4);

  assign t_rsp_reqq_id_sel =
    ({ReqqIdSize{ram_delay_2}} & t_reqq_id_d2) |
    ({ReqqIdSize{ram_delay_3}} & t_reqq_id_d3) |
    ({ReqqIdSize{ram_delay_4}} & t_reqq_id_d4);

  // Response data gating
  assign t_rsp_valid_o    = t_rsp_valid_sel;
  assign t_rsp_trans_id_o = {DvTransIdSize{t_rsp_valid_sel}}  & t_rsp_trans_id_sel;
  assign t_rsp_reqq_id_o  = {ReqqIdSize{t_rsp_valid_sel}}     & t_rsp_reqq_id_sel;
  assign t_rsp_data_o     = {TagRamDataSize{t_rsp_valid_sel}} & t_rd_data_ta_dn;

  // ================================================================
  // Data response pipeline
  // ================================================================

  logic d_rd_valid_d0;
  assign d_rd_valid_d0 = d_clock_en_da & d_rd_en_da;

  logic [RamDly-1:0] d_valid_vec_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) d_valid_vec_q <= '0;
    else if (|{d_valid_vec_q, d_rd_valid_d0})
      d_valid_vec_q <= {d_valid_vec_q[RamDly-2:0], d_rd_valid_d0};
  end

  logic d_valid_d1, d_valid_d2, d_valid_d3, d_valid_d4;
  assign d_valid_d1 = d_valid_vec_q[0];
  assign d_valid_d2 = d_valid_vec_q[1];
  assign d_valid_d3 = d_valid_vec_q[2];
  assign d_valid_d4 = d_valid_vec_q[3];

  logic [DvTransIdSize-1:0] d_trans_id_d0;
  logic [ReqqIdSize-1:0]    d_reqq_id_d0;
  assign d_trans_id_d0 = d_trans_id_da;
  assign d_reqq_id_d0  = d_reqq_id_da;

  logic [DvTransIdSize-1:0] d_trans_id_d1, d_trans_id_d2, d_trans_id_d3, d_trans_id_d4;
  logic [ReqqIdSize-1:0]    d_reqq_id_d1,  d_reqq_id_d2,  d_reqq_id_d3,  d_reqq_id_d4;

  always_ff @(posedge clk_i) begin
    if (d_rd_valid_d0) begin
      d_trans_id_d1 <= d_trans_id_d0;
      d_reqq_id_d1  <= d_reqq_id_d0;
    end
    if (d_valid_d1) begin
      d_trans_id_d2 <= d_trans_id_d1;
      d_reqq_id_d2  <= d_reqq_id_d1;
    end
    if (d_valid_d2) begin
      d_trans_id_d3 <= d_trans_id_d2;
      d_reqq_id_d3  <= d_reqq_id_d2;
    end
    if (d_valid_d3) begin
      d_trans_id_d4 <= d_trans_id_d3;
      d_reqq_id_d4  <= d_reqq_id_d3;
    end
  end

  // Variable delay select
  logic                      d_rsp_valid_sel;
  logic [DvTransIdSize-1:0]  d_rsp_trans_id_sel;
  logic [ReqqIdSize-1:0]     d_rsp_reqq_id_sel;

  assign d_rsp_valid_sel =
    (ram_delay_2 & d_valid_d2) |
    (ram_delay_3 & d_valid_d3) |
    (ram_delay_4 & d_valid_d4);

  assign d_rsp_trans_id_sel =
    ({DvTransIdSize{ram_delay_2}} & d_trans_id_d2) |
    ({DvTransIdSize{ram_delay_3}} & d_trans_id_d3) |
    ({DvTransIdSize{ram_delay_4}} & d_trans_id_d4);

  assign d_rsp_reqq_id_sel =
    ({ReqqIdSize{ram_delay_2}} & d_reqq_id_d2) |
    ({ReqqIdSize{ram_delay_3}} & d_reqq_id_d3) |
    ({ReqqIdSize{ram_delay_4}} & d_reqq_id_d4);

  // Response data gating
  assign d_rsp_valid_o    = d_rsp_valid_sel;
  assign d_rsp_trans_id_o = {DvTransIdSize{d_rsp_valid_sel}}   & d_rsp_trans_id_sel;
  assign d_rsp_reqq_id_o  = {ReqqIdSize{d_rsp_valid_sel}}      & d_rsp_reqq_id_sel;
  assign d_rsp_data_o     = {DataRamDataSize{d_rsp_valid_sel}} & d_rd_data_da_dn;

/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNUSEDSIGNAL */

endmodule

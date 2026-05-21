// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module dll_dly_est_core
  import esr_pkg::*;
#(
  parameter int unsigned Width      = 8,
  parameter int unsigned SyncStages = 3,
  parameter int unsigned PolyInit   = 1
) (
  input  logic                 clk_i,
  input  logic                 rst_ni,

  input  esr_dll_dly_est_ctl_t dll_dly_est_ctl_i,
  output esr_dll_dly_est_sts_t dll_dly_est_sts_o,

  output logic                 tx_dly_est_write_o,
  output logic [7:0]           tx_dly_est_code_o,
  input  logic [7:0]           rx_dly_est_code_i
);

  typedef enum logic [1:0] {
    TxStateIdle = 2'b00,
    TxStateBusy = 2'b01,
    TxStateDone = 2'b10,
    TxStateInit = 2'b11
  } tx_lfsr_state_e;

  typedef enum logic [1:0] {
    RxStateIdle = 2'b00,
    RxStateBusy = 2'b01,
    RxStateDone = 2'b10,
    RxStateInit = 2'b11
  } rx_lfsr_state_e;

  localparam logic [7:0] PolyInitByte = PolyInit[7:0];
  localparam logic [Width-1:0] ErrCntZero = {Width{1'b0}};
  localparam logic [Width-1:0] ErrCntMax  = {Width{1'b1}};
  localparam logic [Width-1:0] ErrCntOne  = {{(Width - 1){1'b0}}, 1'b1};

  logic       ctl_enable;
  logic       ctl_init;
  logic       ctl_start;
  logic [7:0] ctl_txn;

  logic       enable_q;
  logic       start_q;
  logic       init_q;
  logic [7:0] txn_q;

  logic [23:0] tx_cnt_q;
  logic [23:0] tx_cnt_d;
  logic        tx_cnt_end;

  logic [Width-1:0] err_cnt_q;
  logic [Width-1:0] err_cnt_d;

  logic done_q;
  logic done_d;

  logic       tx_init;
  logic       rx_init;
  logic       tx_lfsr_en;
  logic       rx_lfsr_en;
  logic [7:0] tx_lfsr_q;
  logic [7:0] tx_lfsr_d;
  logic [7:0] rx_lfsr_q;
  logic [7:0] rx_lfsr_d;

  logic tx_end;
  logic rx_end;
  logic err_cnt_en;
  logic rx_dly_est_valid;

  tx_lfsr_state_e tx_state_q;
  tx_lfsr_state_e tx_state_d;
  rx_lfsr_state_e rx_state_q;
  rx_lfsr_state_e rx_state_d;

  logic [SyncStages-1:0] loopback_flops_q;
  logic [SyncStages-1:0] tx_end_loopback_flops_q;

  assign ctl_enable = dll_dly_est_ctl_i.ctl_enable;
  assign ctl_init   = dll_dly_est_ctl_i.ctl_init;
  assign ctl_start  = dll_dly_est_ctl_i.ctl_start;
  assign ctl_txn    = dll_dly_est_ctl_i.ctl_txn;

  assign dll_dly_est_sts_o.sts_done = done_q;
  generate
    if (Width == 8) begin : gen_sts_errn_exact
      assign dll_dly_est_sts_o.sts_errn = err_cnt_q;
    end else if (Width > 8) begin : gen_sts_errn_trunc
      assign dll_dly_est_sts_o.sts_errn = err_cnt_q[7:0];
    end else begin : gen_sts_errn_extend
      assign dll_dly_est_sts_o.sts_errn = {{(8 - Width){1'b0}}, err_cnt_q};
    end
  endgenerate

  assign tx_dly_est_code_o = tx_lfsr_q;

  always_comb begin
    tx_cnt_d = (tx_init || done_q) ? 24'h000000 : tx_cnt_q + 24'h000001;
  end

  always_comb begin
    tx_lfsr_d[0] = tx_init ? PolyInitByte[0] : tx_lfsr_q[7];
    tx_lfsr_d[1] = tx_init ? PolyInitByte[1] : tx_lfsr_q[0];
    tx_lfsr_d[2] = tx_init ? PolyInitByte[2] : tx_lfsr_q[1] ^ tx_lfsr_q[7];
    tx_lfsr_d[3] = tx_init ? PolyInitByte[3] : tx_lfsr_q[2] ^ tx_lfsr_q[7];
    tx_lfsr_d[4] = tx_init ? PolyInitByte[4] : tx_lfsr_q[3] ^ tx_lfsr_q[7];
    tx_lfsr_d[5] = tx_init ? PolyInitByte[5] : tx_lfsr_q[4];
    tx_lfsr_d[6] = tx_init ? PolyInitByte[6] : tx_lfsr_q[5];
    tx_lfsr_d[7] = tx_init ? PolyInitByte[7] : tx_lfsr_q[6];
  end

  always_comb begin
    rx_lfsr_d[0] = rx_init ? PolyInitByte[0] : rx_lfsr_q[7];
    rx_lfsr_d[1] = rx_init ? PolyInitByte[1] : rx_lfsr_q[0];
    rx_lfsr_d[2] = rx_init ? PolyInitByte[2] : rx_lfsr_q[1] ^ rx_lfsr_q[7];
    rx_lfsr_d[3] = rx_init ? PolyInitByte[3] : rx_lfsr_q[2] ^ rx_lfsr_q[7];
    rx_lfsr_d[4] = rx_init ? PolyInitByte[4] : rx_lfsr_q[3] ^ rx_lfsr_q[7];
    rx_lfsr_d[5] = rx_init ? PolyInitByte[5] : rx_lfsr_q[4];
    rx_lfsr_d[6] = rx_init ? PolyInitByte[6] : rx_lfsr_q[5];
    rx_lfsr_d[7] = rx_init ? PolyInitByte[7] : rx_lfsr_q[6];
  end

  always_comb begin
    err_cnt_d = err_cnt_q;
    if (rx_init) begin
      err_cnt_d = ErrCntZero;
    end else if ((rx_lfsr_q != rx_dly_est_code_i) && rx_dly_est_valid &&
                 (err_cnt_q != ErrCntMax)) begin
      err_cnt_d = err_cnt_q + ErrCntOne;
    end
  end

  assign tx_cnt_end = ((txn_q[1:0] == 2'b00) && tx_cnt_q[5])  ||
                      ((txn_q[1:0] == 2'b01) && tx_cnt_q[11]) ||
                      ((txn_q[1:0] == 2'b10) && tx_cnt_q[17]) ||
                      ((txn_q[1:0] == 2'b11) && tx_cnt_q[23]);

  assign tx_end = (txn_q[7:2] == 6'h00) && tx_cnt_end;
  assign rx_end = tx_end_loopback_flops_q[SyncStages-1];
  assign rx_dly_est_valid = loopback_flops_q[SyncStages-1];

  always_comb begin
    tx_state_d = tx_state_q;
    tx_lfsr_en = 1'b0;
    tx_dly_est_write_o = 1'b0;
    tx_init = 1'b0;

    unique case (tx_state_q)
      TxStateIdle: begin
        if (init_q) begin
          tx_state_d = TxStateInit;
        end else if (start_q) begin
          tx_state_d = TxStateBusy;
        end
      end

      TxStateBusy: begin
        if (init_q) begin
          tx_state_d = TxStateInit;
        end else if (tx_cnt_end) begin
          tx_state_d = TxStateDone;
          tx_dly_est_write_o = 1'b1;
        end else begin
          tx_lfsr_en = 1'b1;
          tx_dly_est_write_o = 1'b1;
        end
      end

      TxStateDone: begin
        if (init_q) begin
          tx_state_d = TxStateInit;
        end else if (done_q) begin
          tx_state_d = TxStateIdle;
        end
      end

      TxStateInit: begin
        tx_state_d = TxStateIdle;
        tx_init = 1'b1;
      end
    endcase
  end

  always_comb begin
    rx_state_d = rx_state_q;
    rx_lfsr_en = 1'b0;
    rx_init = 1'b0;
    done_d = done_q;
    err_cnt_en = 1'b0;

    unique case (rx_state_q)
      RxStateIdle: begin
        if (start_q) begin
          rx_state_d = RxStateBusy;
          err_cnt_en = 1'b1;
          done_d = 1'b0;
        end else if (init_q) begin
          rx_state_d = RxStateInit;
          done_d = 1'b0;
        end
      end

      RxStateBusy: begin
        if (init_q) begin
          rx_state_d = RxStateInit;
        end else if (rx_dly_est_valid && rx_end) begin
          rx_state_d = RxStateDone;
          done_d = 1'b1;
          err_cnt_en = 1'b1;
        end else begin
          rx_lfsr_en = rx_dly_est_valid;
          err_cnt_en = rx_dly_est_valid;
        end
      end

      RxStateDone: begin
        rx_state_d = RxStateIdle;
      end

      RxStateInit: begin
        rx_state_d = RxStateIdle;
        rx_init = 1'b1;
        err_cnt_en = 1'b1;
        done_d = 1'b0;
      end
    endcase
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      enable_q <= 1'b0;
    end else begin
      enable_q <= ctl_enable;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      txn_q <= 8'h00;
    end else if (ctl_enable && ctl_start) begin
      txn_q <= ctl_txn;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      start_q <= 1'b0;
      init_q  <= 1'b0;
    end else if (ctl_enable) begin
      start_q <= ctl_start;
      init_q  <= ctl_init;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tx_cnt_q <= 24'h000000;
    end else if (enable_q && (tx_lfsr_en || done_q || tx_init)) begin
      tx_cnt_q <= tx_cnt_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tx_lfsr_q <= PolyInitByte;
    end else if (enable_q && (tx_lfsr_en || tx_init)) begin
      tx_lfsr_q <= tx_lfsr_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rx_lfsr_q <= PolyInitByte;
    end else if (enable_q && (rx_lfsr_en || rx_init)) begin
      rx_lfsr_q <= rx_lfsr_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      err_cnt_q <= ErrCntZero;
    end else if (enable_q && err_cnt_en) begin
      err_cnt_q <= err_cnt_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      done_q <= 1'b0;
    end else if (enable_q && err_cnt_en) begin
      done_q <= done_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tx_state_q <= TxStateIdle;
    end else begin
      tx_state_q <= tx_state_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rx_state_q <= RxStateIdle;
    end else begin
      rx_state_q <= rx_state_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      loopback_flops_q <= '0;
    end else begin
      for (int unsigned ii = 0; ii < SyncStages; ii++) begin
        if (ii == 0) begin
          loopback_flops_q[0] <= tx_dly_est_write_o;
        end else begin
          loopback_flops_q[ii] <= loopback_flops_q[ii-1];
        end
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tx_end_loopback_flops_q <= '0;
    end else begin
      for (int unsigned ii = 0; ii < SyncStages; ii++) begin
        if (ii == 0) begin
          tx_end_loopback_flops_q[0] <= tx_end;
        end else begin
          tx_end_loopback_flops_q[ii] <= tx_end_loopback_flops_q[ii-1];
        end
      end
    end
  end

endmodule

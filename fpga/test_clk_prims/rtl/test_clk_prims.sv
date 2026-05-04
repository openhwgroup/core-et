// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Focused bring-up harness for clock technology primitives.

module test_clk_prims #(
  parameter int unsigned ClkHz    = 1_152_000,
  parameter int unsigned BaudRate = 115200
) (
  input  logic       clk_i,
  input  logic       rst_ni,
  output logic       uart_tx_o,
  output logic [7:0] led_o
);

  import dft_pkg::*;

  typedef enum logic [4:0] {
    StGateOff,
    StGateOn,
    StGateOffSettle,
    StGateOffHold,
    StGateScan,
    StGateNOff,
    StGateNOn,
    StGateNOffSettle,
    StGateNOffHold,
    StMux0Settle,
    StMux0Run,
    StMux1Settle,
    StMux1Run,
    StDone
  } test_state_e;

  localparam logic [7:0] ShortWindow  = 8'd8;
  localparam logic [7:0] LongWindow   = 8'd48;
  localparam logic [7:0] SettleWindow = 8'd8;

  test_state_e state_q;
  logic [7:0]  phase_ctr_q;

  logic        gate_en;
  logic        gate_scanmode;
  logic        gate_n_en;
  logic        mux_sel;

  dft_t        dft;
  logic        clk_gate;
  logic        clk_gate_n;
  logic        clk_div2_q;
  logic        clk_mux;

  logic [15:0] gate_count_q;
  logic [15:0] gate_n_count_q;
  logic [15:0] mux_count_q;

  logic [15:0] g0_count_q;
  logic [15:0] g1_count_q;
  logic [15:0] g2_settle_count_q;
  logic [15:0] g2_count_q;
  logic [15:0] gs_count_q;
  logic [15:0] n0_count_q;
  logic [15:0] n1_count_q;
  logic [15:0] n2_settle_count_q;
  logic [15:0] n2_count_q;
  logic [15:0] mux_start_count_q;
  logic [15:0] m0_delta_q;
  logic [15:0] m1_delta_q;

  logic        pass_q;
  logic        done_q;

  logic [3:0]  line_q;
  logic [4:0]  char_q;
  logic        msg_active_q;
  logic [7:0]  tx_data;
  logic        tx_valid;
  logic        tx_ready;
  logic        tx_busy;

  always_comb begin
    gate_en = 1'b0;
    gate_scanmode = 1'b0;
    gate_n_en = 1'b0;
    mux_sel = 1'b0;

    unique case (state_q)
      StGateOn: begin
        gate_en = 1'b1;
      end
      StGateScan: begin
        gate_scanmode = 1'b1;
      end
      StGateNOn: begin
        gate_n_en = 1'b1;
      end
      StMux1Settle, StMux1Run: begin
        mux_sel = 1'b1;
      end
      default: begin
      end
    endcase
  end

  always_comb begin
    dft = '0;
    dft.scanmode = gate_scanmode;
  end

  prim_clk_gate u_clk_gate (
    .clk_i,
    .en_i (gate_en),
    .dft_i(dft),
    .clk_o(clk_gate)
  );

  prim_clk_gate_n u_clk_gate_n (
    .clk_i,
    .en_i  (gate_n_en),
    .clk_no(clk_gate_n)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) clk_div2_q <= 1'b0;
    else         clk_div2_q <= ~clk_div2_q;
  end

  prim_clk_mux #(
    .UseTechClockMux(1'b1)
  ) u_clk_mux (
    .clk0_i(clk_i),
    .clk1_i(clk_div2_q),
    .sel_i (mux_sel),
    .clk_o (clk_mux)
  );

  always_ff @(posedge clk_gate or negedge rst_ni) begin
    if (!rst_ni) gate_count_q <= '0;
    else         gate_count_q <= gate_count_q + 16'd1;
  end

  always_ff @(posedge clk_gate_n or negedge rst_ni) begin
    if (!rst_ni) gate_n_count_q <= '0;
    else         gate_n_count_q <= gate_n_count_q + 16'd1;
  end

  always_ff @(posedge clk_mux or negedge rst_ni) begin
    if (!rst_ni) mux_count_q <= '0;
    else         mux_count_q <= mux_count_q + 16'd1;
  end

  function automatic logic phase_done(input logic [7:0] cycles);
    phase_done = (phase_ctr_q == (cycles - 1));
  endfunction

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q          <= StGateOff;
      phase_ctr_q      <= '0;
      g0_count_q       <= '0;
      g1_count_q       <= '0;
      g2_settle_count_q <= '0;
      g2_count_q       <= '0;
      gs_count_q       <= '0;
      n0_count_q       <= '0;
      n1_count_q       <= '0;
      n2_settle_count_q <= '0;
      n2_count_q       <= '0;
      mux_start_count_q <= '0;
      m0_delta_q       <= '0;
      m1_delta_q       <= '0;
      pass_q           <= 1'b1;
      done_q           <= 1'b0;
    end else begin
      phase_ctr_q <= phase_ctr_q + 8'd1;

      unique case (state_q)
        StGateOff: begin
          if (phase_done(ShortWindow)) begin
            g0_count_q <= gate_count_q;
            if (gate_count_q != 16'd0) pass_q <= 1'b0;
            state_q <= StGateOn;
            phase_ctr_q <= '0;
          end
        end

        StGateOn: begin
          if (phase_done(LongWindow)) begin
            g1_count_q <= gate_count_q;
            if (gate_count_q <= (g0_count_q + 16'd8)) pass_q <= 1'b0;
            state_q <= StGateOffSettle;
            phase_ctr_q <= '0;
          end
        end

        StGateOffSettle: begin
          if (phase_done(SettleWindow)) begin
            g2_settle_count_q <= gate_count_q;
            state_q <= StGateOffHold;
            phase_ctr_q <= '0;
          end
        end

        StGateOffHold: begin
          if (phase_done(LongWindow)) begin
            g2_count_q <= gate_count_q;
            if (gate_count_q != g2_settle_count_q) pass_q <= 1'b0;
            state_q <= StGateScan;
            phase_ctr_q <= '0;
          end
        end

        StGateScan: begin
          if (phase_done(LongWindow)) begin
            gs_count_q <= gate_count_q;
            if (gate_count_q <= (g2_count_q + 16'd8)) pass_q <= 1'b0;
            state_q <= StGateNOff;
            phase_ctr_q <= '0;
          end
        end

        StGateNOff: begin
          if (phase_done(ShortWindow)) begin
            n0_count_q <= gate_n_count_q;
            if (gate_n_count_q != 16'd0) pass_q <= 1'b0;
            state_q <= StGateNOn;
            phase_ctr_q <= '0;
          end
        end

        StGateNOn: begin
          if (phase_done(LongWindow)) begin
            n1_count_q <= gate_n_count_q;
            if (gate_n_count_q <= (n0_count_q + 16'd8)) pass_q <= 1'b0;
            state_q <= StGateNOffSettle;
            phase_ctr_q <= '0;
          end
        end

        StGateNOffSettle: begin
          if (phase_done(SettleWindow)) begin
            n2_settle_count_q <= gate_n_count_q;
            state_q <= StGateNOffHold;
            phase_ctr_q <= '0;
          end
        end

        StGateNOffHold: begin
          if (phase_done(LongWindow)) begin
            n2_count_q <= gate_n_count_q;
            if (gate_n_count_q != n2_settle_count_q) pass_q <= 1'b0;
            state_q <= StMux0Settle;
            phase_ctr_q <= '0;
          end
        end

        StMux0Settle: begin
          if (phase_done(SettleWindow)) begin
            mux_start_count_q <= mux_count_q;
            state_q <= StMux0Run;
            phase_ctr_q <= '0;
          end
        end

        StMux0Run: begin
          if (phase_done(LongWindow)) begin
            m0_delta_q <= mux_count_q - mux_start_count_q;
            if ((mux_count_q - mux_start_count_q) <= 16'd16) pass_q <= 1'b0;
            state_q <= StMux1Settle;
            phase_ctr_q <= '0;
          end
        end

        StMux1Settle: begin
          if (phase_done(SettleWindow)) begin
            mux_start_count_q <= mux_count_q;
            state_q <= StMux1Run;
            phase_ctr_q <= '0;
          end
        end

        StMux1Run: begin
          if (phase_done(LongWindow)) begin
            m1_delta_q <= mux_count_q - mux_start_count_q;
            if ((mux_count_q - mux_start_count_q) <= 16'd8) pass_q <= 1'b0;
            if ((mux_count_q - mux_start_count_q + 16'd8) >= m0_delta_q) pass_q <= 1'b0;
            state_q <= StDone;
            done_q <= 1'b1;
            phase_ctr_q <= '0;
          end
        end

        default: begin
          state_q <= StDone;
          done_q <= 1'b1;
        end
      endcase
    end
  end

  function automatic logic [7:0] hex_digit(input logic [3:0] nibble);
    unique case (nibble)
      4'h0:    hex_digit = 8'h30;
      4'h1:    hex_digit = 8'h31;
      4'h2:    hex_digit = 8'h32;
      4'h3:    hex_digit = 8'h33;
      4'h4:    hex_digit = 8'h34;
      4'h5:    hex_digit = 8'h35;
      4'h6:    hex_digit = 8'h36;
      4'h7:    hex_digit = 8'h37;
      4'h8:    hex_digit = 8'h38;
      4'h9:    hex_digit = 8'h39;
      4'hA:    hex_digit = 8'h41;
      4'hB:    hex_digit = 8'h42;
      4'hC:    hex_digit = 8'h43;
      4'hD:    hex_digit = 8'h44;
      4'hE:    hex_digit = 8'h45;
      default: hex_digit = 8'h46;
    endcase
  endfunction

  function automatic logic [7:0] hex16_digit(
    input logic [15:0] value,
    input logic [1:0]  digit_idx
  );
    unique case (digit_idx)
      2'd0:    hex16_digit = hex_digit(value[15:12]);
      2'd1:    hex16_digit = hex_digit(value[11:8]);
      2'd2:    hex16_digit = hex_digit(value[7:4]);
      default: hex16_digit = hex_digit(value[3:0]);
    endcase
  endfunction

  function automatic logic [7:0] boot_byte(input logic [4:0] idx);
    unique case (idx)
      0:  boot_byte = 8'h41;  // A
      1:  boot_byte = 8'h69;  // i
      2:  boot_byte = 8'h6E;  // n
      3:  boot_byte = 8'h65;  // e
      4:  boot_byte = 8'h6B;  // k
      5:  boot_byte = 8'h6B;  // k
      6:  boot_byte = 8'h6F;  // o
      7:  boot_byte = 8'h20;  // space
      8:  boot_byte = 8'h74;  // t
      9:  boot_byte = 8'h65;  // e
      10: boot_byte = 8'h73;  // s
      11: boot_byte = 8'h74;  // t
      12: boot_byte = 8'h5F;  // _
      13: boot_byte = 8'h63;  // c
      14: boot_byte = 8'h6C;  // l
      15: boot_byte = 8'h6B;  // k
      16: boot_byte = 8'h5F;  // _
      17: boot_byte = 8'h70;  // p
      18: boot_byte = 8'h72;  // r
      19: boot_byte = 8'h69;  // i
      20: boot_byte = 8'h6D;  // m
      21: boot_byte = 8'h73;  // s
      22: boot_byte = 8'h0D;  // \r
      default: boot_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] count_line_byte(
    input logic [7:0]  tag0,
    input logic [7:0]  tag1,
    input logic [7:0]  kind,
    input logic [15:0] value,
    input logic [4:0]  idx
  );
    unique case (idx)
      0:  count_line_byte = tag0;
      1:  count_line_byte = tag1;
      2:  count_line_byte = 8'h20;  // space
      3:  count_line_byte = kind;
      4:  count_line_byte = 8'h3D;  // =
      5:  count_line_byte = hex16_digit(value, 2'd0);
      6:  count_line_byte = hex16_digit(value, 2'd1);
      7:  count_line_byte = hex16_digit(value, 2'd2);
      8:  count_line_byte = hex16_digit(value, 2'd3);
      9:  count_line_byte = 8'h0D;  // \r
      default: count_line_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [7:0] status_byte(
    input logic pass_i,
    input logic [4:0] idx
  );
    unique case (idx)
      0:  status_byte = 8'h54;  // T
      1:  status_byte = 8'h45;  // E
      2:  status_byte = 8'h53;  // S
      3:  status_byte = 8'h54;  // T
      4:  status_byte = 8'h20;  // space
      5:  status_byte = 8'h43;  // C
      6:  status_byte = 8'h4C;  // L
      7:  status_byte = 8'h4B;  // K
      8:  status_byte = 8'h20;  // space
      9:  status_byte = 8'h50;  // P
      10: status_byte = 8'h52;  // R
      11: status_byte = 8'h49;  // I
      12: status_byte = 8'h4D;  // M
      13: status_byte = 8'h53;  // S
      14: status_byte = 8'h20;  // space
      15: status_byte = pass_i ? 8'h50 : 8'h46;  // P/F
      16: status_byte = pass_i ? 8'h41 : 8'h41;  // A/A
      17: status_byte = pass_i ? 8'h53 : 8'h49;  // S/I
      18: status_byte = pass_i ? 8'h53 : 8'h4C;  // S/L
      19: status_byte = pass_i ? 8'h45 : 8'h45;  // E/E
      20: status_byte = pass_i ? 8'h44 : 8'h44;  // D/D
      21: status_byte = 8'h0D;  // \r
      default: status_byte = 8'h0A;  // \n
    endcase
  endfunction

  function automatic logic [4:0] line_last_idx(input logic [3:0] line);
    unique case (line)
      4'd0:    line_last_idx = 5'd23;
      4'd10:   line_last_idx = 5'd22;
      default: line_last_idx = 5'd10;
    endcase
  endfunction

  always_comb begin
    unique case (line_q)
      4'd0: tx_data = boot_byte(char_q);
      4'd1: tx_data = count_line_byte(8'h47, 8'h30, 8'h43, g0_count_q, char_q);  // G0 C=
      4'd2: tx_data = count_line_byte(8'h47, 8'h31, 8'h43, g1_count_q, char_q);  // G1 C=
      4'd3: tx_data = count_line_byte(8'h47, 8'h32, 8'h43, g2_count_q, char_q);  // G2 C=
      4'd4: tx_data = count_line_byte(8'h47, 8'h53, 8'h43, gs_count_q, char_q);  // GS C=
      4'd5: tx_data = count_line_byte(8'h4E, 8'h30, 8'h43, n0_count_q, char_q);  // N0 C=
      4'd6: tx_data = count_line_byte(8'h4E, 8'h31, 8'h43, n1_count_q, char_q);  // N1 C=
      4'd7: tx_data = count_line_byte(8'h4E, 8'h32, 8'h43, n2_count_q, char_q);  // N2 C=
      4'd8: tx_data = count_line_byte(8'h4D, 8'h30, 8'h44, m0_delta_q, char_q);  // M0 D=
      4'd9: tx_data = count_line_byte(8'h4D, 8'h31, 8'h44, m1_delta_q, char_q);  // M1 D=
      default: tx_data = status_byte(pass_q, char_q);
    endcase
  end

  assign tx_valid = msg_active_q && tx_ready;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      msg_active_q <= 1'b0;
      line_q       <= '0;
      char_q       <= '0;
    end else begin
      if (done_q && !msg_active_q && line_q == '0 && char_q == '0) begin
        msg_active_q <= 1'b1;
      end else if (msg_active_q && tx_ready) begin
        if (char_q == line_last_idx(line_q)) begin
          char_q <= '0;
          if (line_q == 4'd10) begin
            msg_active_q <= 1'b0;
          end else begin
            line_q <= line_q + 4'd1;
          end
        end else begin
          char_q <= char_q + 5'd1;
        end
      end
    end
  end

  ulx3s_uart_tx #(
    .ClkHz(ClkHz),
    .BaudRate(BaudRate)
  ) u_tx (
    .clk_i,
    .rst_ni,
    .valid_i(tx_valid),
    .data_i (tx_data),
    .ready_o(tx_ready),
    .tx_o   (uart_tx_o),
    .busy_o (tx_busy)
  );

  assign led_o = {
    done_q,
    pass_q,
    !pass_q,
    gate_en,
    gate_n_en,
    mux_sel,
    tx_busy,
    msg_active_q
  };

endmodule

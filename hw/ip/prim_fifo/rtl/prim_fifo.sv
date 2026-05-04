// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Generic FIFO matching CORE-ET gen_fifo.
//
// This preserves the original contract, including:
// - registered `full_o` / `valid_o`
// - combinational `full_next_o` / `valid_next_o`
// - raw pointer/count updates on asserted push/pop (not handshake-gated)
// - optional latch-timed storage path via `push_early_i`

module prim_fifo #(
  parameter int unsigned Width      = 32,
  parameter int unsigned Depth      = 32,
  parameter bit          UseLatches = 1'b0,
  localparam int unsigned PtrW      = (Depth > 1) ? $clog2(Depth) : 1,
  localparam int unsigned CountW    = PtrW + 1
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  /* verilator lint_off UNUSEDSIGNAL */  // Only consumed when UseLatches=1; the non-latched translation preserves the original unused port.
  input  logic              push_early_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic              push_i,
  input  logic [Width-1:0]  push_data_i,
  output logic              full_o,
  output logic              full_next_o,

  input  logic              pop_i,
  output logic [Width-1:0]  pop_data_o,
  output logic              valid_o,
  output logic              valid_next_o
);

  /* verilator lint_off WIDTHTRUNC */  // Depth-derived constants are intentionally truncated to the local counter/pointer widths.
  localparam logic [CountW-1:0] DepthMinusOne = Depth - 1;
  localparam logic [CountW-1:0] CountOne      = {{(CountW-1){1'b0}}, 1'b1};
  localparam logic [PtrW-1:0]   PtrOne        = {{(PtrW-1){1'b0}}, 1'b1};
  localparam logic [PtrW-1:0]   LastPtr       = Depth - 1;
  /* verilator lint_on WIDTHTRUNC */

  logic [CountW-1:0] count_q, count_d;
  logic [PtrW-1:0]   read_ptr_q, write_ptr_q;

  always_comb begin
    count_d = count_q;
    if (pop_i && !push_i) count_d = count_q - CountOne;
    if (push_i && !pop_i) count_d = count_q + CountOne;
  end

  always_comb begin
    if (pop_i) begin
      full_next_o = push_i && full_o;
    end else begin
      full_next_o = full_o || ((count_q == DepthMinusOne) && push_i);
    end
    valid_next_o = (count_d != '0);
  end

  generate
    if ((1 << PtrW) == Depth) begin : gen_pow2_ptrs
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          read_ptr_q <= '0;
        end else if (pop_i) begin
          read_ptr_q <= read_ptr_q + PtrOne;
        end
      end

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          write_ptr_q <= '0;
        end else if (push_i) begin
          write_ptr_q <= write_ptr_q + PtrOne;
        end
      end
    end else begin : gen_wrap_ptrs
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          read_ptr_q <= '0;
        end else if (pop_i) begin
          if (read_ptr_q == LastPtr) begin
            read_ptr_q <= '0;
          end else begin
            read_ptr_q <= read_ptr_q + PtrOne;
          end
        end
      end

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          write_ptr_q <= '0;
        end else if (push_i) begin
          if (write_ptr_q == LastPtr) begin
            write_ptr_q <= '0;
          end else begin
            write_ptr_q <= write_ptr_q + PtrOne;
          end
        end
      end
    end
  endgenerate

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      count_q <= '0;
      full_o  <= 1'b0;
      valid_o <= 1'b0;
    end else begin
      count_q <= count_d;
      full_o  <= full_next_o;
      valid_o <= valid_next_o;
    end
  end

  generate
    if (UseLatches) begin : gen_latched_storage
      prim_rf_1r1w_preview #(
        .Width(Width),
        .Depth(Depth)
      ) u_mem (
        .preview_clk_i        (clk_i),
        .rf_clk_i             (clk_i),
        .wr_data_en_1p_next_i (push_early_i),
        .wr_en_i              (push_i),
        .wr_addr_i            (write_ptr_q),
        .wr_data_i            (push_data_i),
        .rd_addr_i            (read_ptr_q),
        .rd_data_o            (pop_data_o)
      );
    end else begin : gen_ff_storage
      logic [Depth-1:0][Width-1:0] data_array_q;

      always_ff @(posedge clk_i) begin
        if (push_i) begin
          data_array_q[write_ptr_q] <= push_data_i;
        end
      end

      assign pop_data_o = data_array_q[read_ptr_q];
    end
  endgenerate

  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert and used only as an assertion guard here.
  always_ff @(posedge clk_i) begin
    if (rst_ni) begin
      if (pop_i && !valid_o)
        $warning("prim_fifo: popping empty FIFO");
      if (push_i && full_o && !pop_i)
        $warning("prim_fifo: writing to FIFO when full");
    end
  end
  /* verilator lint_on SYNCASYNCNET */
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni)
    valid_o || !pop_i)
  else $warning("prim_fifo: popping empty FIFO");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    !full_o || !push_i || pop_i)
  else $warning("prim_fifo: writing to FIFO when full");
`endif
  // synthesis translate_on

endmodule

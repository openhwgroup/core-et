// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: semi-synchronous LV->HV FIFO.
// ECP5 implementation preserving the CORE-ET vcfifo_wr_lov_ss handshake.

module prim_fifo_semisync_lov
  import dft_pkg::*;
#(
  parameter int unsigned Width = 32,
  parameter int unsigned Depth = 3,
  parameter int unsigned PushRelay = 0,
  parameter int unsigned PopRelay = 0,
  localparam int unsigned EffectiveDepth = Depth + ((PushRelay != 0) ? 1 : 0) +
                                           ((PopRelay  != 0) ? 1 : 0),
  localparam int unsigned CountWidth = (EffectiveDepth <= 1) ? 1 : $clog2(EffectiveDepth + 1)
) (
  input  logic             clk_wr_i,
  input  logic             rst_wr_ni,
  input  logic             push_i,
  input  logic [Width-1:0] wdata_i,
  output logic             ready_o,
  input  logic             clk_rd_i,
  input  logic             rst_rd_ni,
  input  logic             pop_i,
  output logic [Width-1:0] rdata_o,
  output logic             valid_o,
  input  dft_t             dft_hv_i,
  input  dft_t             dft_lv_i
);

  localparam int unsigned ResetSyncStages = 2;
  localparam logic [CountWidth-1:0] CountOne = {{CountWidth-1{1'b0}}, 1'b1};
  localparam logic [CountWidth-1:0] EffectiveDepthCount = EffectiveDepth[CountWidth-1:0];

  // The original _ss FIFO resets each side when either functional reset is
  // asserted. In scan mode, sync_rst bypasses that functional OR and uses the
  // domain-local DFT reset directly.
  logic [ResetSyncStages-1:0] rst_wr_meta_q;
  logic [ResetSyncStages-1:0] rst_rd_meta_q;
  logic rst_wr_async_ni;
  logic rst_rd_async_ni;
  logic rst_wr_sync;
  logic rst_rd_sync;

  assign rst_wr_async_ni = dft_lv_i.scanmode ? dft_lv_i.scan_reset_n : (rst_wr_ni & rst_rd_ni);
  assign rst_rd_async_ni = dft_hv_i.scanmode ? dft_hv_i.scan_reset_n : (rst_wr_ni & rst_rd_ni);

  always_ff @(posedge clk_wr_i or negedge rst_wr_async_ni) begin
    if (!rst_wr_async_ni) begin
      rst_wr_meta_q <= '1;
    end else begin
      rst_wr_meta_q <= {rst_wr_meta_q[ResetSyncStages-2:0], 1'b0};
    end
  end

  always_ff @(posedge clk_rd_i or negedge rst_rd_async_ni) begin
    if (!rst_rd_async_ni) begin
      rst_rd_meta_q <= '1;
    end else begin
      rst_rd_meta_q <= {rst_rd_meta_q[ResetSyncStages-2:0], 1'b0};
    end
  end

  assign rst_wr_sync = dft_lv_i.scanmode ? ~dft_lv_i.scan_reset_n : rst_wr_meta_q[ResetSyncStages-1];
  assign rst_rd_sync = dft_hv_i.scanmode ? ~dft_hv_i.scan_reset_n : rst_rd_meta_q[ResetSyncStages-1];

  // Write-domain pulse and credit logic from vcfifo_wr_pd_ss.
  logic                  push_d1_q;
  logic [Width-1:0]      push_data_d1_q;
  logic                  pop_d1_q;
  logic                  pop_d2_q;
  logic [CountWidth-1:0] credits_q;
  logic                  reset_sync_d1_q;

  logic                  really_push;
  logic                  pop_in;
  logic [CountWidth-1:0] credits_next;
  logic                  full_next;
  logic                  reset_sync_fe;

  assign really_push  = push_i & ready_o;
  assign pop_in       = (PopRelay != 0) ? pop_d2_q : pop_d1_q;
  assign credits_next = pop_in ? (credits_q - CountOne) : (credits_q + CountOne);
  assign full_next    = (credits_next == EffectiveDepthCount);
  assign reset_sync_fe = !rst_wr_sync & reset_sync_d1_q;

  always_ff @(posedge clk_wr_i) begin
    if (rst_wr_sync) begin
      pop_d2_q        <= 1'b0;
      push_d1_q       <= 1'b0;
      credits_q       <= '0;
      reset_sync_d1_q <= 1'b1;
      ready_o         <= 1'b0;
    end else begin
      reset_sync_d1_q <= rst_wr_sync;
      if (pop_d2_q | pop_d1_q) begin
        pop_d2_q <= pop_d1_q;
      end
      if (push_d1_q | really_push) begin
        push_d1_q <= really_push;
      end
      if (really_push ^ pop_in) begin
        credits_q <= credits_next;
      end
      if (really_push ^ (pop_in | reset_sync_fe)) begin
        ready_o <= !full_next | reset_sync_fe;
      end
    end
  end

  always_ff @(posedge clk_wr_i) begin
    if (really_push) begin
      push_data_d1_q <= wdata_i;
    end
  end

  // Read-domain pulse relay and destination FIFO from vcfifo_rd_pd_ss.
  logic             push_d2_q;
  logic [Width-1:0] push_data_d2_q;
  logic             really_pop;
  logic             push_in;
  logic [Width-1:0] push_data_in;

  assign really_pop   = pop_i & valid_o;
  assign push_in      = (PushRelay != 0) ? push_d2_q      : push_d1_q;
  assign push_data_in = (PushRelay != 0) ? push_data_d2_q : push_data_d1_q;

  always_ff @(posedge clk_rd_i) begin
    if (rst_rd_sync) begin
      pop_d1_q       <= 1'b0;
      push_d2_q      <= 1'b0;
      push_data_d2_q <= '0;
    end else begin
      if (pop_d1_q | really_pop) begin
        pop_d1_q <= really_pop;
      end
      if (push_d2_q | push_d1_q) begin
        push_d2_q <= push_d1_q;
      end
      if (push_d1_q) begin
        push_data_d2_q <= push_data_d1_q;
      end
    end
  end

  generate
    if (EffectiveDepth == 1) begin : gen_single_entry_fifo
      logic valid_q;
      logic full;
      logic [Width-1:0] rdata_q;

      assign full    = valid_q & !pop_i;
      assign valid_o = valid_q;
      assign rdata_o = rdata_q;

      always_ff @(posedge clk_rd_i) begin
        if (rst_rd_sync) begin
          valid_q <= 1'b0;
        end else if (!full) begin
          valid_q <= push_in;
        end
      end

      always_ff @(posedge clk_rd_i) begin
        if (push_in & !full) begin
          rdata_q <= push_data_in;
        end
      end
    end else begin : gen_multi_entry_fifo
      localparam int unsigned MemDepth = EffectiveDepth - 1;
      localparam int unsigned PtrWidth = (MemDepth == 1) ? 1 : $clog2(MemDepth);
      localparam logic [EffectiveDepth:0] LevelEmpty = {{EffectiveDepth{1'b0}}, 1'b1};
      localparam logic [PtrWidth-1:0] PtrOne = {{PtrWidth-1{1'b0}}, 1'b1};
      localparam int unsigned LastPtrInt = MemDepth - 1;
      localparam logic [PtrWidth-1:0] LastPtr = LastPtrInt[PtrWidth-1:0];

      logic [EffectiveDepth:0] level_q;
      logic [EffectiveDepth:0] level_next;
      logic                    full;
      logic                    mem_valid;
      logic                    wdata_taken;
      logic                    rdata_taken;
      logic                    level_en;
      logic                    inc_wptr;
      logic                    inc_rptr;
      logic [PtrWidth-1:0]     wptr_q;
      logic [PtrWidth-1:0]     rptr_q;
      logic [PtrWidth-1:0]     wptr_next;
      logic [PtrWidth-1:0]     rptr_next;
      logic [Width-1:0]        mem_q [MemDepth];
      logic [Width-1:0]        rdata_q;
      logic [Width-1:0]        rdata_next;
      logic                    rdata_en;

      assign full        = level_q[EffectiveDepth];
      assign valid_o     = !level_q[0];
      assign rdata_o     = rdata_q;
      assign wdata_taken = push_in & !full;
      assign rdata_taken = pop_i & valid_o;
      assign level_en    = wdata_taken ^ rdata_taken;
      assign level_next  = wdata_taken ? {level_q[EffectiveDepth-1:0], 1'b0} :
                                         {1'b0, level_q[EffectiveDepth:1]};
      assign mem_valid   = !(|level_q[1:0]);
      assign inc_wptr    = push_in & (mem_valid | (valid_o & !pop_i)) & !full;
      assign inc_rptr    = pop_i & mem_valid;
      assign wptr_next   = (wptr_q == LastPtr) ? '0 : (wptr_q + PtrOne);
      assign rptr_next   = (rptr_q == LastPtr) ? '0 : (rptr_q + PtrOne);
      assign rdata_en    = (push_in & !valid_o) | (push_in & pop_i & !mem_valid) | inc_rptr;

      always_comb begin
        if ((push_in & !valid_o) | (push_in & pop_i & !mem_valid)) begin
          rdata_next = push_data_in;
        end else if (inc_rptr) begin
          rdata_next = mem_q[rptr_q];
        end else begin
          rdata_next = rdata_q;
        end
      end

      always_ff @(posedge clk_rd_i) begin
        if (rst_rd_sync) begin
          level_q <= LevelEmpty;
          wptr_q  <= '0;
          rptr_q  <= '0;
        end else begin
          if (level_en) begin
            level_q <= level_next;
          end
          if (inc_wptr) begin
            wptr_q <= wptr_next;
          end
          if (inc_rptr) begin
            rptr_q <= rptr_next;
          end
        end
      end

      always_ff @(posedge clk_rd_i) begin
        if (inc_wptr) begin
          mem_q[wptr_q] <= push_data_in;
        end
        if (rdata_en) begin
          rdata_q <= rdata_next;
        end
      end
    end
  endgenerate

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_dft;
  assign unused_dft = &{1'b0, dft_hv_i.sram_clk_override, dft_hv_i.ram_rei, dft_hv_i.ram_wei,
                               dft_lv_i.sram_clk_override, dft_lv_i.ram_rei, dft_lv_i.ram_wei};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule

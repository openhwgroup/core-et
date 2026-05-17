// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_dmctrl #(
  parameter int unsigned SyncStages = 2,
  parameter int unsigned DelayStages = 19
) (
  input  logic                 clk_i,
  input  logic                 rst_ni,
  input  esr_pkg::esr_ms_dmctrl_t dmctrl_i,
  output esr_pkg::esr_ms_dmctrl_t dmctrl_o
);

  logic [SyncStages-1:0] we_sync_q;
  logic [DelayStages:1]  delay_q;
  logic                  write_enable_prev_q;
  logic                  write_enable_edge;
  esr_pkg::esr_ms_dmctrl_t dmctrl_q;

  assign write_enable_edge = write_enable_prev_q ^ delay_q[DelayStages];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      we_sync_q           <= '0;
      delay_q             <= '0;
      write_enable_prev_q <= 1'b0;
      dmctrl_q            <= '0;
    end else begin
      we_sync_q[0] <= dmctrl_i.write_enable;
      for (int unsigned i = 1; i < SyncStages; i++) begin
        we_sync_q[i] <= we_sync_q[i-1];
      end

      delay_q[1] <= we_sync_q[SyncStages-1];
      for (int unsigned i = 1; i < DelayStages; i++) begin
        delay_q[i+1] <= delay_q[i];
      end

      write_enable_prev_q <= delay_q[DelayStages];

      if (write_enable_edge) begin
        dmctrl_q.dmactive        <= dmctrl_i.dmactive;
        dmctrl_q.ndmreset        <= dmctrl_i.ndmreset;
        dmctrl_q.clresethaltreq  <= dmctrl_i.clresethaltreq;
        dmctrl_q.setresethaltreq <= dmctrl_i.setresethaltreq;
        dmctrl_q.hasel           <= dmctrl_i.hasel;
        dmctrl_q.ackhavereset    <= dmctrl_i.ackhavereset;
        dmctrl_q.hartreset       <= dmctrl_i.hartreset;
        dmctrl_q.resumereq       <= dmctrl_i.resumereq;
        dmctrl_q.haltreq         <= dmctrl_i.haltreq;
      end
      dmctrl_q.write_enable <= write_enable_edge;
    end
  end

  assign dmctrl_o = dmctrl_q;

endmodule

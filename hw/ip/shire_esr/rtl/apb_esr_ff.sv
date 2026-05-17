// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module apb_esr_ff #(
  parameter int unsigned NumInterfaces = shire_esr_pkg::NumNeigh + shirecache_pkg::Banks + 2
) (
  input  logic                           clk_i,
  input  logic                           rst_ni,

  input  apb_pkg::req_t [NumInterfaces-1:0] apb_req_i,
  output apb_pkg::req_t [NumInterfaces-1:0] apb_req_o,
  input  apb_pkg::rsp_t [NumInterfaces-1:0] apb_rsp_i,
  output apb_pkg::rsp_t [NumInterfaces-1:0] apb_rsp_o
);
  typedef enum logic [2:0] {
    StIdle  = 3'b000,
    StSetup = 3'b001,
    StExec  = 3'b010,
    StReady = 3'b011,
    StDone  = 3'b100
  } state_e;

  state_e [NumInterfaces-1:0] state_q;
  state_e [NumInterfaces-1:0] state_d;
  logic   [NumInterfaces-1:0] state_en;
  logic   [NumInterfaces-1:0] setup_en;
  logic   [NumInterfaces-1:0] done_en;

  apb_pkg::req_t [NumInterfaces-1:0] req_q;
  apb_pkg::rsp_t [NumInterfaces-1:0] rsp_q;

  for (genvar i = 0; i < NumInterfaces; i++) begin : gen_apb_ff
    always_comb begin
      state_d[i] = state_q[i];
      state_en[i] = 1'b0;

      unique case (state_q[i])
        StIdle: begin
          if (apb_req_i[i].psel) begin
            state_d[i] = StSetup;
            state_en[i] = 1'b1;
          end
        end
        StSetup: begin
          if (apb_req_i[i].penable) begin
            state_d[i] = StExec;
            state_en[i] = 1'b1;
          end
        end
        StExec: begin
          if (apb_rsp_i[i].pready) begin
            state_d[i] = StReady;
            state_en[i] = 1'b1;
          end
        end
        StReady: begin
          state_d[i] = StDone;
          state_en[i] = 1'b1;
        end
        StDone: begin
          if (apb_req_i[i].psel) begin
            state_d[i] = StSetup;
          end else begin
            state_d[i] = StIdle;
          end
          state_en[i] = 1'b1;
        end
        default: begin
          state_d[i] = StIdle;
          state_en[i] = 1'b1;
        end
      endcase
    end

    assign setup_en[i] = ((state_q[i] == StIdle) && apb_req_i[i].psel) ||
                         ((state_q[i] == StDone) && apb_req_i[i].psel);
    assign done_en[i] = (state_q[i] == StExec) && apb_rsp_i[i].pready;

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        state_q[i] <= StIdle;
        req_q[i] <= '0;
        rsp_q[i] <= '0;
      end else begin
        if (state_en[i]) begin
          state_q[i] <= state_d[i];
        end
        if (setup_en[i]) begin
          req_q[i].paddr <= apb_req_i[i].paddr;
          req_q[i].pwrite <= apb_req_i[i].pwrite;
          req_q[i].pwdata <= apb_req_i[i].pwdata;
        end
        if (done_en[i]) begin
          rsp_q[i].pslverr <= apb_rsp_i[i].pslverr;
          rsp_q[i].prdata <= apb_rsp_i[i].prdata;
        end
      end
    end

    always_comb begin
      apb_req_o[i] = req_q[i];
      apb_req_o[i].psel = (state_q[i] == StSetup) || (state_q[i] == StExec);
      apb_req_o[i].penable = (state_q[i] == StExec);

      apb_rsp_o[i] = rsp_q[i];
      apb_rsp_o[i].pready = (state_q[i] == StReady);
    end
  end
endmodule

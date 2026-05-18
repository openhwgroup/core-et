// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module sbm_top #(
  parameter int unsigned SbmInterfaces      = shire_sbm_pkg::SbmInterfaces,
  parameter int unsigned ShireApbInterfaces = shire_sbm_pkg::ShireApbInterfaces
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,
  input  logic [shire_sbm_pkg::NumNeigh-1:0]        rst_neigh_ni,
  input  logic                                      rst_rbox_ni,
  input  logic                                      rst_shire_cache_ni,

  input  logic [shire_sbm_pkg::AxiIdWidth-1:0]      shire_virtual_id_i,
  input  logic                                      sys_axi_ar_valid_i,
  input  logic                                      sys_axi_aw_valid_i,
  output logic                                      sbm_write_credit_return_o,

  input  axi_pkg::sys_slave_ar_t                    sys_axi_ar_i,
  output logic                                      sys_axi_ar_ready_o,
  input  axi_pkg::sys_slave_aw_t                    sys_axi_aw_i,
  output logic                                      sys_axi_aw_ready_o,
  input  axi_pkg::sys_slave_w_t                     sys_axi_w_i,
  output logic                                      sys_axi_w_ready_o,
  output axi_pkg::sys_slave_b_t                     sys_axi_b_o,
  output logic                                      sys_axi_b_valid_o,
  input  logic                                      sys_axi_b_ready_i,
  output axi_pkg::sys_slave_r_t                     sys_axi_r_o,
  output logic                                      sys_axi_r_valid_o,
  input  logic                                      sys_axi_r_ready_i,

  output shire_sbm_pkg::shire_apb_req_t [ShireApbInterfaces-1:0] apb_req_o,
  input  shire_sbm_pkg::shire_apb_rsp_t [ShireApbInterfaces-1:0] apb_rsp_i
);

  shire_sbm_pkg::shire_apb_req_t [SbmInterfaces-1:0] sbm_apb_req;
  shire_sbm_pkg::shire_apb_rsp_t [SbmInterfaces-1:0] sbm_apb_rsp;
  shire_sbm_pkg::shire_apb_req_t [ShireApbInterfaces-1:0] apb_req_in;
  shire_sbm_pkg::shire_apb_rsp_t [ShireApbInterfaces-1:0] apb_rsp_out;
  logic unused_sbm_top_ok;
  assign unused_sbm_top_ok = ^apb_rsp_out[ShireApbInterfaces-1];

  shire_bus_master #(
    .SbmInterfaces (SbmInterfaces)
  ) u_sbm (
    .clk_i,
    .rst_ni,
    .rst_neigh_ni,
    .rst_shire_cache_ni,
    .rst_rbox_ni,
    .shire_virtual_id_i,
    .sys_axi_ar_valid_i,
    .sys_axi_aw_valid_i,
    .credit_return_write_o (sbm_write_credit_return_o),
    .sys_axi_ar_i,
    .sys_axi_ar_ready_o,
    .sys_axi_aw_i,
    .sys_axi_aw_ready_o,
    .sys_axi_w_i,
    .sys_axi_w_ready_o,
    .sys_axi_b_o,
    .sys_axi_b_valid_o,
    .sys_axi_b_ready_i,
    .sys_axi_r_o,
    .sys_axi_r_valid_o,
    .sys_axi_r_ready_i,
    .apb_req_o (sbm_apb_req),
    .apb_rsp_i (sbm_apb_rsp)
  );

  always_comb begin
    apb_req_in = '0;
    apb_req_in[SbmInterfaces-1:0] = sbm_apb_req;
    sbm_apb_rsp = apb_rsp_out[SbmInterfaces-1:0];
  end

  typedef enum logic [2:0] {
    StIdle  = 3'b000,
    StSetup = 3'b001,
    StExec  = 3'b010,
    StReady = 3'b011,
    StDone  = 3'b100
  } apb_ff_state_e;

  apb_ff_state_e [ShireApbInterfaces-1:0] state_q;
  apb_ff_state_e [ShireApbInterfaces-1:0] state_d;
  logic [ShireApbInterfaces-1:0] state_en;
  logic [ShireApbInterfaces-1:0] setup_en;
  logic [ShireApbInterfaces-1:0] done_en;

  shire_sbm_pkg::shire_apb_req_t [ShireApbInterfaces-1:0] req_q;
  shire_sbm_pkg::shire_apb_rsp_t [ShireApbInterfaces-1:0] rsp_q;

  for (genvar i = 0; i < ShireApbInterfaces; i++) begin : gen_apb_ff
    always_comb begin
      state_d[i] = state_q[i];
      state_en[i] = 1'b0;
      unique case (state_q[i])
        StIdle: begin
          if (apb_req_in[i].psel) begin
            state_d[i] = StSetup;
            state_en[i] = 1'b1;
          end
        end
        StSetup: begin
          if (apb_req_in[i].penable) begin
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
          state_d[i] = apb_req_in[i].psel ? StSetup : StIdle;
          state_en[i] = 1'b1;
        end
        default: begin
          state_d[i] = StIdle;
          state_en[i] = 1'b1;
        end
      endcase
    end

    assign setup_en[i] = ((state_q[i] == StIdle) && apb_req_in[i].psel) ||
                         ((state_q[i] == StDone) && apb_req_in[i].psel);
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
          req_q[i].paddr <= apb_req_in[i].paddr;
          req_q[i].pwrite <= apb_req_in[i].pwrite;
          req_q[i].pwdata <= apb_req_in[i].pwdata;
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
      apb_rsp_out[i] = rsp_q[i];
      apb_rsp_out[i].pready = (state_q[i] == StReady);
    end
  end

endmodule : sbm_top

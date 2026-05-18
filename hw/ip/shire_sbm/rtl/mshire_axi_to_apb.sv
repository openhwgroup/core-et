// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module mshire_axi_to_apb #(
  parameter int unsigned ApbAddrWidth = shire_sbm_pkg::SbmApbAddrWidth,
  parameter int unsigned ApbDataWidth = shire_sbm_pkg::ApbDataWidth,
  parameter bit          ShiftDataEnRd = 1'b1,
  parameter bit          ResponseControl = 1'b0
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  output logic                    ready_o,

  input  shire_sbm_pkg::sbm_axi_a_t axi_ar_i,
  input  logic                    axi_ar_valid_i,
  input  shire_sbm_pkg::sbm_axi_a_t axi_aw_i,
  input  logic                    axi_aw_valid_i,
  input  shire_sbm_pkg::sbm_axi_w_t axi_w_i,

  output axi_pkg::sys_slave_b_t   axi_b_o,
  output logic                    axi_b_valid_o,
  input  logic                    axi_b_ready_i,

  output axi_pkg::sys_slave_r_t   axi_r_o,
  output logic                    axi_r_valid_o,
  input  logic                    axi_r_ready_i,

  output logic [ApbAddrWidth-1:0] apb_paddr_o,
  output logic                    apb_penable_o,
  input  logic [ApbDataWidth-1:0] apb_prdata_i,
  input  logic                    apb_pready_i,
  output logic                    apb_psel_o,
  input  logic                    apb_pslverr_i,
  output logic [ApbDataWidth-1:0] apb_pwdata_o,
  output logic                    apb_pwrite_o
);

  typedef enum logic [2:0] {
    StIdle        = 3'b000,
    StStage1      = 3'b001,
    StStage2      = 3'b010,
    StStage3      = 3'b011,
    StReadPending = 3'b100
  } conversion_stage_e;

  conversion_stage_e state_q;

  logic read_request_waiting_q;
  logic operation_q;
  logic rsp_stored_q;

  shire_sbm_pkg::sbm_axi_a_t axi_ar_q;
  logic [shire_sbm_pkg::AxiIdWidth-1:0] axi_aw_id_q;
  axi_pkg::sys_slave_r_t     axi_r_q;
  axi_pkg::sys_slave_b_t     axi_b_q;

  logic [axi_pkg::SysSlaveDataSize-1:0] read_data_shifted;
  logic [axi_pkg::SysSlaveDataSize-1:0] read_data_zext;

  assign read_data_shifted = {axi_pkg::SysSlaveDataSize / ApbDataWidth{apb_prdata_i}};
  assign read_data_zext = {{(axi_pkg::SysSlaveDataSize - ApbDataWidth){1'b0}}, apb_prdata_i};

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ready_o <= 1'b1;
      axi_b_valid_o <= 1'b0;
      axi_r_valid_o <= 1'b0;
      apb_psel_o <= 1'b0;
      apb_penable_o <= 1'b0;
      read_request_waiting_q <= 1'b0;
      operation_q <= 1'b0;
      state_q <= StIdle;
    end else begin
      unique case (state_q)
        StIdle: begin
          axi_b_valid_o <= 1'b0;
          axi_r_valid_o <= 1'b0;
          ready_o <= (axi_aw_valid_i || axi_ar_valid_i) ? 1'b0 : 1'b1;
          read_request_waiting_q <= axi_aw_valid_i && axi_ar_valid_i;
          apb_penable_o <= 1'b0;
          apb_psel_o <= 1'b0;
          rsp_stored_q <= 1'b0;

          if (axi_aw_valid_i) begin
            operation_q <= 1'b1;
            axi_aw_id_q <= axi_aw_i.id;
            axi_ar_q <= axi_ar_i;
            apb_paddr_o <= axi_aw_i.addr[ApbAddrWidth-1:0];
            apb_pwrite_o <= 1'b1;
            apb_psel_o <= 1'b1;
            apb_pwdata_o <= axi_w_i.data;
            state_q <= StStage1;
          end else if (axi_ar_valid_i) begin
            operation_q <= 1'b0;
            axi_ar_q <= axi_ar_i;
            apb_paddr_o <= axi_ar_i.addr[ApbAddrWidth-1:0];
            apb_pwrite_o <= 1'b0;
            apb_psel_o <= 1'b1;
            state_q <= StStage1;
          end
        end

        StReadPending: begin
          operation_q <= 1'b0;
          rsp_stored_q <= 1'b0;
          axi_b_valid_o <= 1'b0;
          apb_paddr_o <= axi_ar_q.addr[ApbAddrWidth-1:0];
          apb_pwrite_o <= 1'b0;
          apb_psel_o <= 1'b1;
          state_q <= StStage1;
        end

        StStage1: begin
          apb_penable_o <= 1'b1;
          state_q <= StStage2;
        end

        StStage2: begin
          if (operation_q) begin
            if (apb_pready_i && !rsp_stored_q) begin
              rsp_stored_q <= 1'b1;
              axi_b_valid_o <= 1'b1;
              axi_b_o.resp <= apb_pslverr_i ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
              axi_b_o.id <= axi_aw_id_q;
              axi_b_q.resp <= apb_pslverr_i ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
              axi_b_q.id <= axi_aw_id_q;
              apb_penable_o <= 1'b0;
              apb_psel_o <= 1'b0;
              if (!ResponseControl) begin
                state_q <= axi_b_ready_i ? (read_request_waiting_q ? StReadPending : StIdle) : StStage2;
                ready_o <= axi_b_ready_i && !read_request_waiting_q;
              end else begin
                state_q <= axi_b_ready_i ? StStage3 : StStage2;
              end
            end else if (rsp_stored_q) begin
              axi_b_valid_o <= axi_b_ready_i ? 1'b0 : 1'b1;
              axi_b_o <= axi_b_q;
              state_q <= axi_b_ready_i ? (read_request_waiting_q ? StReadPending : StIdle) : StStage2;
              ready_o <= axi_b_ready_i && !read_request_waiting_q;
            end
          end else begin
            if (apb_pready_i && !rsp_stored_q) begin
              rsp_stored_q <= 1'b1;
              axi_r_q.id <= axi_ar_q.id;
              axi_r_q.resp <= apb_pslverr_i ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
              axi_r_q.last <= 1'b1;
              axi_r_q.data <= ShiftDataEnRd ? read_data_shifted : read_data_zext;
              axi_r_o.id <= axi_ar_q.id;
              axi_r_o.resp <= apb_pslverr_i ? axi_pkg::RespSlvErr : axi_pkg::RespOkay;
              axi_r_o.last <= 1'b1;
              axi_r_o.data <= ShiftDataEnRd ? read_data_shifted : read_data_zext;
              axi_r_valid_o <= 1'b1;
              apb_penable_o <= 1'b0;
              apb_psel_o <= 1'b0;
              if (!ResponseControl) begin
                state_q <= axi_r_ready_i ? StIdle : StStage2;
                ready_o <= axi_r_ready_i;
              end else begin
                state_q <= axi_r_ready_i ? StStage3 : StStage2;
              end
            end else if (rsp_stored_q) begin
              axi_r_o <= axi_r_q;
              if (!ResponseControl) begin
                axi_r_valid_o <= axi_r_ready_i ? 1'b0 : 1'b1;
                state_q <= axi_r_ready_i ? StIdle : StStage2;
                ready_o <= axi_r_ready_i;
              end else begin
                axi_r_valid_o <= 1'b1;
                state_q <= axi_r_ready_i ? StStage3 : StStage2;
              end
            end
          end
        end

        StStage3: begin
          if (operation_q) begin
            if (axi_b_ready_i) begin
              axi_b_valid_o <= 1'b0;
              state_q <= read_request_waiting_q ? StReadPending : StIdle;
              ready_o <= !read_request_waiting_q;
            end
          end else begin
            if (axi_r_ready_i) begin
              axi_r_valid_o <= 1'b0;
              state_q <= StIdle;
              ready_o <= 1'b1;
            end
          end
        end

        default: begin
          state_q <= StIdle;
          ready_o <= 1'b1;
          axi_b_valid_o <= 1'b0;
          axi_r_valid_o <= 1'b0;
          apb_psel_o <= 1'b0;
          apb_penable_o <= 1'b0;
        end
      endcase
    end
  end

endmodule : mshire_axi_to_apb

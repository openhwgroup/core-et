// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_ch_apb_mux
  import neigh_ch_apb_mux_pkg::*;
(
  input  logic                   clk_i,
  input  logic                   rst_ni,

  // APB connection with the shire / BPAM.
  input  apb_to_neigh_t          apb_req_from_shire_i,
  output apb_from_neigh_t        apb_rsp_to_shire_o,

  // APB connection with the minions.
  input  logic [MinPerN-1:0][DataWidth-1:0] minion_apb_prdata_i,
  input  logic [MinPerN-1:0]                minion_apb_pready_i,
  input  logic [MinPerN-1:0]                minion_apb_pslverr_i,
  output logic                              minion_apb_penable_o,
  output logic [MinPerN-1:0]                minion_apb_psel_o,
  output logic                              minion_apb_pwrite_o,
  output logic [MinDmApbAddrWidth-1:0]      minion_apb_paddr_o,
  output logic [DataWidth-1:0]              minion_apb_pwdata_o,

  // APB connection with neighborhood-channel resources.
  output apb_to_neigh_esrs_t    apb_req_struct_esr_o,
  output apb_to_neigh_icache_t  apb_req_struct_icache_o,
  output apb_to_neigh_tbox_t    apb_req_struct_tbox_o,
  input  apb_from_neigh_channel_t apb_rsp_struct_esr_i,
  input  apb_from_neigh_channel_t apb_rsp_struct_icache_i,
  input  apb_from_neigh_channel_t apb_rsp_struct_tbox_i
);

  apb_to_neigh_t       apb_req_to_min;
  apb_from_neigh_t     apb_rsp_from_min;

  apb_to_neigh_t       apb_req_from_shire_q;
  apb_from_neigh_t     apb_rsp_to_shire_d;
  apb_to_neigh_tbox_t  apb_req_struct_tbox_d;

  logic                apb_rsp_esr_valid_q;
  logic                apb_rsp_esr_valid_d;
  logic [1:0]          neigh_resource_class;
  logic [MinPerN-1:0][DataWidth-1:0] minion_apb_prdata_q;
  logic [MinPerN-1:0]                minion_apb_pready_q;
  logic [MinPerN-1:0]                minion_apb_pslverr_q;

  assign neigh_resource_class = apb_req_from_shire_q.apb_paddr[NeighDmApbAddrClassMsb:
                                                               NeighDmApbAddrClassLsb];

  always_comb begin
    apb_rsp_esr_valid_d = apb_req_from_shire_q.apb_psel && apb_req_from_shire_q.apb_penable;

    if ((neigh_resource_class == NeighResourceMinionEsr) |
        (neigh_resource_class == NeighResourceMinionDc)) begin
      apb_req_to_min.apb_paddr   = apb_req_from_shire_q.apb_paddr;
      apb_req_to_min.apb_pwrite  = apb_req_from_shire_q.apb_pwrite;
      apb_req_to_min.apb_psel    = apb_req_from_shire_q.apb_psel;
      apb_req_to_min.apb_penable = apb_req_from_shire_q.apb_penable;
      apb_req_to_min.apb_pwdata  = apb_req_from_shire_q.apb_pwdata;
      apb_rsp_to_shire_d         = apb_rsp_from_min;

      apb_req_struct_esr_o       = '0;
      apb_req_struct_icache_o    = '0;
      apb_req_struct_tbox_d      = '0;
    end else begin
      if (apb_req_from_shire_q.apb_paddr[NeighDmApbAddrDestMsb]) begin
        apb_req_struct_esr_o = '0;

        if (apb_req_from_shire_q.apb_paddr[NeighChTboxSelectBit]) begin
          apb_req_struct_icache_o = '0;
          apb_req_struct_tbox_d.apb_paddr =
              apb_req_from_shire_q.apb_paddr[TboxNeighApbAddrWidth-1:0];
          apb_req_struct_tbox_d.apb_pwrite  = apb_req_from_shire_q.apb_pwrite;
          apb_req_struct_tbox_d.apb_psel    = apb_req_from_shire_q.apb_psel;
          apb_req_struct_tbox_d.apb_penable = apb_req_from_shire_q.apb_penable;
          apb_req_struct_tbox_d.apb_pwdata  = apb_req_from_shire_q.apb_pwdata;
          apb_rsp_to_shire_d                = apb_rsp_struct_tbox_i;
        end else begin
          apb_req_struct_tbox_d = '0;
          apb_req_struct_icache_o.apb_paddr =
              apb_req_from_shire_q.apb_paddr[IcacheNeighApbAddrWidth-1:0];
          apb_req_struct_icache_o.apb_pwrite  = apb_req_from_shire_q.apb_pwrite;
          apb_req_struct_icache_o.apb_psel    = apb_req_from_shire_q.apb_psel;
          apb_req_struct_icache_o.apb_penable = apb_req_from_shire_q.apb_penable;
          apb_req_struct_icache_o.apb_pwdata  = apb_req_from_shire_q.apb_pwdata;
          apb_rsp_to_shire_d                  = apb_rsp_struct_icache_i;
        end
      end else begin
        apb_req_struct_icache_o = '0;
        apb_req_struct_tbox_d   = '0;
        apb_req_struct_esr_o.apb_paddr =
            apb_req_from_shire_q.apb_paddr[EsrNeighApbPpAddrWidth-1:0];
        apb_req_struct_esr_o.apb_pwrite  = apb_req_from_shire_q.apb_pwrite;
        apb_req_struct_esr_o.apb_psel    = apb_req_from_shire_q.apb_psel;
        apb_req_struct_esr_o.apb_penable = apb_req_from_shire_q.apb_penable;
        apb_req_struct_esr_o.apb_pwdata  = apb_req_from_shire_q.apb_pwdata;
        apb_rsp_to_shire_d = apb_rsp_esr_valid_q ? apb_rsp_struct_esr_i : '0;
      end

      apb_req_to_min = '0;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      apb_req_from_shire_q  <= '0;
      apb_rsp_to_shire_o    <= '0;
      apb_req_struct_tbox_o <= '0;
      apb_rsp_esr_valid_q   <= 1'b0;
      minion_apb_pready_q   <= '0;
      minion_apb_prdata_q   <= '0;
      minion_apb_pslverr_q  <= '0;
    end else begin
      apb_req_from_shire_q  <= apb_req_from_shire_i;
      apb_rsp_to_shire_o    <= apb_rsp_to_shire_d;
      apb_req_struct_tbox_o <= apb_req_struct_tbox_d;
      apb_rsp_esr_valid_q   <= apb_rsp_esr_valid_d;
      minion_apb_pready_q   <= minion_apb_pready_i;
      minion_apb_prdata_q   <= minion_apb_prdata_i;
      minion_apb_pslverr_q  <= minion_apb_pslverr_i;
    end
  end

  bpam2minions #(
    .AdInWidth  (NeighDmApbAddrWidth),
    .AdOutWidth (MinDmApbAddrWidth),
    .DataWidth  (DataWidth),
    .NrMinions  (MinPerN)
  ) u_bpam2minions (
    .clk_i,
    .rst_ni,
    .req_apb_paddr_i   (apb_req_to_min.apb_paddr),
    .req_apb_pwrite_i  (apb_req_to_min.apb_pwrite),
    .req_apb_psel_i    (apb_req_to_min.apb_psel),
    .req_apb_penable_i (apb_req_to_min.apb_penable),
    .req_apb_pwdata_i  (apb_req_to_min.apb_pwdata),
    .rsp_apb_pready_o  (apb_rsp_from_min.apb_pready),
    .rsp_apb_prdata_o  (apb_rsp_from_min.apb_prdata),
    .rsp_apb_pslverr_o (apb_rsp_from_min.apb_pslverr),
    .minion_apb_paddr_o   (minion_apb_paddr_o),
    .minion_apb_pwrite_o  (minion_apb_pwrite_o),
    .minion_apb_psel_o    (minion_apb_psel_o),
    .minion_apb_penable_o (minion_apb_penable_o),
    .minion_apb_pwdata_o  (minion_apb_pwdata_o),
    .minion_apb_pready_i  (minion_apb_pready_q),
    .minion_apb_prdata_i  (minion_apb_prdata_q),
    .minion_apb_pslverr_i (minion_apb_pslverr_q)
  );

endmodule : neigh_ch_apb_mux

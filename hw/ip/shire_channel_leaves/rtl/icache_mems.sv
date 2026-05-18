// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_mems #(
  parameter int unsigned NumNeigh = neigh_pkg::NumNeigh
) (
  input  logic                                           clk_i,
  input  logic [NumNeigh-1:0]                           rst_ni,
  input  logic                                           dft_sram_clk_i,
  input  dft_pkg::dft_t                                  dft_i,

  input  logic [NumNeigh-1:0]                            icache_req_write_i,
  input  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [NumNeigh-1:0]                            icache_req_valid_i,
  output logic [NumNeigh-1:0]                            icache_req_ready_o,
  output logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout_o,
  output logic [NumNeigh-1:0]                            icache_resp_valid_o,
  input  logic [NumNeigh-1:0]                            icache_resp_ready_i,

  input  etlink_pkg::rsp_t                               neigh_sc_rsp_info_i [NumNeigh-1:0],
  input  logic [NumNeigh-1:0]                            neigh_sc_rsp_valid_i,
  input  logic [NumNeigh-1:0]                            neigh_sc_rsp_ready_i,

  input  ram_cfg_pkg::ram_cfg_t                          ram_cfg_i,

  input  shirecache_pkg::icache_bist_req_t               bist_req_info_i,
  output shirecache_pkg::icache_bist_rsp_t               bist_rsp_info_o,

  input  logic [icache_pkg::IcacheMemsApbAddrWidth-1:0]  apb_paddr_i,
  input  logic                                           apb_pwrite_i,
  input  logic                                           apb_psel_i,
  input  logic                                           apb_penable_i,
  input  logic [minion_pkg::BpamShireApbDataWidth-1:0]   apb_pwdata_i,
  output logic                                           apb_pready_o,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0]   apb_prdata_o,
  output logic                                           apb_pslverr_o
);

  localparam int unsigned ApbNeighMsb = icache_pkg::IcacheMemsApbAddrWidth - 1;
  localparam int unsigned ApbNeighLsb =
      icache_pkg::IcacheMemsApbAddrWidth - icache_pkg::IcacheMemsApbNeighWidth;

  shirecache_pkg::icache_bist_rsp_t int_bist_rsp_info [NumNeigh-1:0];
  logic [NumNeigh-1:0][icache_pkg::IcacheDataApbAddrWidth-1:0] int_apb_paddr;
  logic [NumNeigh-1:0] int_apb_pwrite;
  logic [NumNeigh-1:0] int_apb_psel;
  logic [NumNeigh-1:0] int_apb_penable;
  logic [NumNeigh-1:0][minion_pkg::BpamShireApbDataWidth-1:0] int_apb_pwdata;
  logic [NumNeigh-1:0] int_apb_pready;
  logic [NumNeigh-1:0][minion_pkg::BpamShireApbDataWidth-1:0] int_apb_prdata;
  logic [NumNeigh-1:0] int_apb_pslverr;

  logic [icache_pkg::IcacheMemsApbNeighWidth-1:0] apb_neigh_sel;
  logic unused_apb_region_bits;

  assign apb_neigh_sel = apb_paddr_i[ApbNeighMsb:ApbNeighLsb];
  assign unused_apb_region_bits = ^apb_paddr_i[ApbNeighLsb-1:icache_pkg::IcacheDataApbAddrWidth];

  for (genvar n = 0; n < NumNeigh; n++) begin : gen_icache_sram
    localparam logic [icache_pkg::IcacheMemsApbNeighWidth-1:0] NeighSel = n;

    icache_data_ram_wrap #(
      .Id(n)
    ) u_data_ram_wrap (
      .clk_i                 (clk_i),
      .rst_ni                (rst_ni[n]),
      .dft_sram_clk_i        (dft_sram_clk_i),
      .dft_i                 (dft_i),
      .icache_req_write_i    (icache_req_write_i[n]),
      .icache_req_addr_i     (icache_req_addr_i[n]),
      .icache_req_valid_i    (icache_req_valid_i[n]),
      .icache_req_ready_o    (icache_req_ready_o[n]),
      .icache_resp_dout_o    (icache_resp_dout_o[n]),
      .icache_resp_valid_o   (icache_resp_valid_o[n]),
      .icache_resp_ready_i   (icache_resp_ready_i[n]),
      .neigh_sc_rsp_info_i   (neigh_sc_rsp_info_i[n]),
      .neigh_sc_rsp_valid_i  (neigh_sc_rsp_valid_i[n]),
      .neigh_sc_rsp_ready_i  (neigh_sc_rsp_ready_i[n]),
      .ram_cfg_i             (ram_cfg_i),
      .bist_req_info_i       (bist_req_info_i),
      .bist_rsp_info_o       (int_bist_rsp_info[n]),
      .apb_paddr_i           (int_apb_paddr[n]),
      .apb_pwrite_i          (int_apb_pwrite[n]),
      .apb_psel_i            (int_apb_psel[n]),
      .apb_penable_i         (int_apb_penable[n]),
      .apb_pwdata_i          (int_apb_pwdata[n]),
      .apb_pready_o          (int_apb_pready[n]),
      .apb_prdata_o          (int_apb_prdata[n]),
      .apb_pslverr_o         (int_apb_pslverr[n])
    );

    assign int_apb_paddr[n] = apb_paddr_i[icache_pkg::IcacheDataApbAddrWidth-1:0];
    assign int_apb_pwrite[n] = apb_pwrite_i;
    assign int_apb_psel[n] = apb_psel_i & (apb_neigh_sel == NeighSel);
    assign int_apb_penable[n] = apb_penable_i & (apb_neigh_sel == NeighSel);
    assign int_apb_pwdata[n] = apb_pwdata_i;
  end

  always_comb begin
    bist_rsp_info_o = '0;
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      bist_rsp_info_o |= int_bist_rsp_info[n];
    end
  end

  assign apb_pready_o = |int_apb_pready;

  always_comb begin
    apb_prdata_o = '0;
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      apb_prdata_o |= int_apb_prdata[n] &
          {minion_pkg::BpamShireApbDataWidth{int_apb_pready[n]}};
    end
  end

  assign apb_pslverr_o = |int_apb_pslverr | (unused_apb_region_bits & 1'b0);

endmodule

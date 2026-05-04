`include "rtl/inc/soc.vh"

`ifndef __EVL_PSHIRE_NOC_PKG
   `include "dv/soc/pshire/pshire_noc/evl_pshire_noc_pkg.sv"
`endif

module evl_pshire_noc_interface #(parameter int PSHIRE_ID         = 0,
                                  parameter bit PSHIRE_NOC_ONLY   = 1'b0,
                                  parameter bit PSHIRE_NO_ESR     = 1'b0,
                                  parameter bit ALLOW_ZEBU_DRIVER = 1'b0) (
      input  wire                             reset,
      input  wire                             noc_clk,
      input  wire                             ios_clk,
      input  wire                             apb_clk,
      input  wire                             pcie_axi_clk,
      output wire                             zebu_driver,

      inout  wire   [5:0]                     p0_spio_m_AWID,
      inout  wire  [39:0]                     p0_spio_m_AWADDR,
      inout  wire   [7:0]                     p0_spio_m_AWLEN,
      inout  wire   [2:0]                     p0_spio_m_AWSIZE,
      inout  wire   [1:0]                     p0_spio_m_AWBURST,
      inout  wire                             p0_spio_m_AWLOCK,
      inout  wire   [3:0]                     p0_spio_m_AWCACHE,
      inout  wire   [2:0]                     p0_spio_m_AWPROT,
      inout  wire   [3:0]                     p0_spio_m_AWQOS,
      inout  wire                             p0_spio_m_AWVALID,
      inout  wire                             p0_spio_m_AWREADY,
      inout  wire  [31:0]                     p0_spio_m_WDATA,
      inout  wire   [3:0]                     p0_spio_m_WSTRB,
      inout  wire                             p0_spio_m_WLAST,
      inout  wire                             p0_spio_m_WVALID,
      inout  wire                             p0_spio_m_WREADY,
      inout  wire                             p0_spio_m_BREADY,
      inout  wire   [5:0]                     p0_spio_m_BID,
      inout  wire   [1:0]                     p0_spio_m_BRESP,
      inout  wire                             p0_spio_m_BVALID,
      inout  wire   [5:0]                     p0_spio_m_ARID,
      inout  wire  [39:0]                     p0_spio_m_ARADDR,
      inout  wire   [7:0]                     p0_spio_m_ARLEN,
      inout  wire   [2:0]                     p0_spio_m_ARSIZE,
      inout  wire   [1:0]                     p0_spio_m_ARBURST,
      inout  wire                             p0_spio_m_ARLOCK,
      inout  wire   [3:0]                     p0_spio_m_ARCACHE,
      inout  wire   [2:0]                     p0_spio_m_ARPROT,
      inout  wire   [3:0]                     p0_spio_m_ARQOS,
      inout  wire                             p0_spio_m_ARVALID,
      inout  wire                             p0_spio_m_ARREADY,
      inout  wire                             p0_spio_m_RREADY,
      inout  wire   [5:0]                     p0_spio_m_RID,
      inout  wire  [31:0]                     p0_spio_m_RDATA,
      inout  wire   [1:0]                     p0_spio_m_RRESP,
      inout  wire                             p0_spio_m_RLAST,
      inout  wire                             p0_spio_m_RVALID,

      inout  wire  [31:0]                     rbm_m_awaddr,
      inout  wire   [2:0]                     rbm_m_awprot,
      inout  wire                             rbm_m_awvalid,
      inout  wire                             rbm_m_awready,
      inout  wire  [31:0]                     rbm_m_wdata,
      inout  wire   [3:0]                     rbm_m_wstrb,
      inout  wire                             rbm_m_wvalid,
      inout  wire                             rbm_m_wready,
      inout  wire                             rbm_m_bready,
      inout  wire   [1:0]                     rbm_m_bresp,
      inout  wire                             rbm_m_bvalid,
      inout  wire  [31:0]                     rbm_m_araddr,
      inout  wire   [2:0]                     rbm_m_arprot,
      inout  wire                             rbm_m_arvalid,
      inout  wire                             rbm_m_arready,
      inout  wire                             rbm_m_rready,
      inout  wire  [31:0]                     rbm_m_rdata,
      inout  wire   [1:0]                     rbm_m_rresp,
      inout  wire                             rbm_m_rvalid,

      inout  wire  [31:0]                     apbslv_paddr,
      inout  wire                             apbslv_penable,
      inout  wire                             apbslv_psel,
      inout  wire   [3:0]                     apbslv_pstrb,
      inout  wire  [31:0]                     apbslv_pwdata,
      inout  wire                             apbslv_pwrite,
      inout  wire  [31:0]                     apbslv_prdata,
      inout  wire                             apbslv_pready,
      inout  wire                             apbslv_pslverr,

      inout  wire  [31:0]                     pcie0_dbi_araddr,
      inout  wire   [1:0]                     pcie0_dbi_arburst,
      inout  wire   [3:0]                     pcie0_dbi_arcache,
      inout  wire   [3:0]                     pcie0_dbi_arid,
      inout  wire   [3:0]                     pcie0_dbi_arlen,
      inout  wire                             pcie0_dbi_arlock,
      inout  wire   [2:0]                     pcie0_dbi_arprot,
      inout  wire   [3:0]                     pcie0_dbi_arqos,
      inout  wire   [2:0]                     pcie0_dbi_arsize,
      inout  wire                             pcie0_dbi_arvalid,
      inout  wire  [31:0]                     pcie0_dbi_awaddr,
      inout  wire   [1:0]                     pcie0_dbi_awburst,
      inout  wire   [3:0]                     pcie0_dbi_awcache,
      inout  wire   [3:0]                     pcie0_dbi_awid,
      inout  wire   [3:0]                     pcie0_dbi_awlen,
      inout  wire                             pcie0_dbi_awlock,
      inout  wire   [2:0]                     pcie0_dbi_awprot,
      inout  wire   [3:0]                     pcie0_dbi_awqos,
      inout  wire   [2:0]                     pcie0_dbi_awsize,
      inout  wire                             pcie0_dbi_awvalid,
      inout  wire                             pcie0_dbi_bready,
      inout  wire                             pcie0_dbi_rready,
      inout  wire  [31:0]                     pcie0_dbi_wdata,
      inout  wire                             pcie0_dbi_wlast,
      inout  wire   [3:0]                     pcie0_dbi_wstrb,
      inout  wire                             pcie0_dbi_wvalid,
      inout  wire                             pcie0_dbi_arready,
      inout  wire                             pcie0_dbi_awready,
      inout  wire   [3:0]                     pcie0_dbi_bid,
      inout  wire   [1:0]                     pcie0_dbi_bresp,
      inout  wire                             pcie0_dbi_bvalid,
      inout  wire  [31:0]                     pcie0_dbi_rdata,
      inout  wire   [3:0]                     pcie0_dbi_rid,
      inout  wire                             pcie0_dbi_rlast,
      inout  wire   [1:0]                     pcie0_dbi_rresp,
      inout  wire                             pcie0_dbi_rvalid,
      inout  wire                             pcie0_dbi_wready,

      inout  wire                             pcie0_mstr_arready,
      inout  wire                             pcie0_mstr_awready,
      inout  wire   [4:0]                     pcie0_mstr_bid,
      inout  wire   [1:0]                     pcie0_mstr_bresp,
      inout  wire                             pcie0_mstr_bvalid,
      inout  wire [255:0]                     pcie0_mstr_rdata,
      inout  wire   [4:0]                     pcie0_mstr_rid,
      inout  wire                             pcie0_mstr_rlast,
      inout  wire   [1:0]                     pcie0_mstr_rresp,
      inout  wire                             pcie0_mstr_rvalid,
      inout  wire                             pcie0_mstr_wready,
      inout  wire  [63:0]                     pcie0_mstr_araddr,
      inout  wire   [1:0]                     pcie0_mstr_arburst,
      inout  wire   [3:0]                     pcie0_mstr_arcache,
      inout  wire   [4:0]                     pcie0_mstr_arid,
      inout  wire   [2:0]                     pcie0_mstr_arlen,
      inout  wire                             pcie0_mstr_arlock,
      inout  wire   [2:0]                     pcie0_mstr_arprot,
      inout  wire   [3:0]                     pcie0_mstr_arqos,
      inout  wire   [2:0]                     pcie0_mstr_arsize,
      inout  wire                             pcie0_mstr_arvalid,
      inout  wire  [63:0]                     pcie0_mstr_awaddr,
      inout  wire   [1:0]                     pcie0_mstr_awburst,
      inout  wire   [3:0]                     pcie0_mstr_awcache,
      inout  wire   [4:0]                     pcie0_mstr_awid,
      inout  wire   [2:0]                     pcie0_mstr_awlen,
      inout  wire                             pcie0_mstr_awlock,
      inout  wire   [2:0]                     pcie0_mstr_awprot,
      inout  wire   [3:0]                     pcie0_mstr_awqos,
      inout  wire   [2:0]                     pcie0_mstr_awsize,
      inout  wire                             pcie0_mstr_awvalid,
      inout  wire                             pcie0_mstr_bready,
      inout  wire                             pcie0_mstr_rready,
      inout  wire [255:0]                     pcie0_mstr_wdata,
      inout  wire                             pcie0_mstr_wlast,
      inout  wire  [31:0]                     pcie0_mstr_wstrb,
      inout  wire                             pcie0_mstr_wvalid,

      inout  wire  [63:0]                     pcie0_slv_araddr,
      inout  wire   [1:0]                     pcie0_slv_arburst,
      inout  wire   [3:0]                     pcie0_slv_arcache,
      inout  wire  [15:0]                     pcie0_slv_arid,
      inout  wire   [2:0]                     pcie0_slv_arlen,
      inout  wire                             pcie0_slv_arlock,
      inout  wire   [2:0]                     pcie0_slv_arprot,
      inout  wire   [3:0]                     pcie0_slv_arqos,
      inout  wire   [2:0]                     pcie0_slv_arsize,
      inout  wire                             pcie0_slv_arvalid,
      inout  wire  [63:0]                     pcie0_slv_awaddr,
      inout  wire   [1:0]                     pcie0_slv_awburst,
      inout  wire   [3:0]                     pcie0_slv_awcache,
      inout  wire  [15:0]                     pcie0_slv_awid,
      inout  wire   [2:0]                     pcie0_slv_awlen,
      inout  wire                             pcie0_slv_awlock,
      inout  wire   [2:0]                     pcie0_slv_awprot,
      inout  wire   [3:0]                     pcie0_slv_awqos,
      inout  wire   [2:0]                     pcie0_slv_awsize,
      inout  wire                             pcie0_slv_awvalid,
      inout  wire                             pcie0_slv_bready,
      inout  wire                             pcie0_slv_rready,
      inout  wire [255:0]                     pcie0_slv_wdata,
      inout  wire                             pcie0_slv_wlast,
      inout  wire  [31:0]                     pcie0_slv_wstrb,
      inout  wire                             pcie0_slv_wvalid,
      inout  wire                             pcie0_slv_arready,
      inout  wire                             pcie0_slv_awready,
      inout  wire  [15:0]                     pcie0_slv_bid,
      inout  wire   [1:0]                     pcie0_slv_bresp,
      inout  wire                             pcie0_slv_bvalid,
      inout  wire [255:0]                     pcie0_slv_rdata,
      inout  wire  [15:0]                     pcie0_slv_rid,
      inout  wire                             pcie0_slv_rlast,
      inout  wire   [1:0]                     pcie0_slv_rresp,
      inout  wire                             pcie0_slv_rvalid,
      inout  wire                             pcie0_slv_wready,

      inout  wire  [31:0]                     pcie1_dbi_araddr,
      inout  wire   [1:0]                     pcie1_dbi_arburst,
      inout  wire   [3:0]                     pcie1_dbi_arcache,
      inout  wire   [3:0]                     pcie1_dbi_arid,
      inout  wire   [3:0]                     pcie1_dbi_arlen,
      inout  wire                             pcie1_dbi_arlock,
      inout  wire   [2:0]                     pcie1_dbi_arprot,
      inout  wire   [3:0]                     pcie1_dbi_arqos,
      inout  wire   [2:0]                     pcie1_dbi_arsize,
      inout  wire                             pcie1_dbi_arvalid,
      inout  wire  [31:0]                     pcie1_dbi_awaddr,
      inout  wire   [1:0]                     pcie1_dbi_awburst,
      inout  wire   [3:0]                     pcie1_dbi_awcache,
      inout  wire   [3:0]                     pcie1_dbi_awid,
      inout  wire   [3:0]                     pcie1_dbi_awlen,
      inout  wire                             pcie1_dbi_awlock,
      inout  wire   [2:0]                     pcie1_dbi_awprot,
      inout  wire   [3:0]                     pcie1_dbi_awqos,
      inout  wire   [2:0]                     pcie1_dbi_awsize,
      inout  wire                             pcie1_dbi_awvalid,
      inout  wire                             pcie1_dbi_bready,
      inout  wire                             pcie1_dbi_rready,
      inout  wire  [31:0]                     pcie1_dbi_wdata,
      inout  wire                             pcie1_dbi_wlast,
      inout  wire   [3:0]                     pcie1_dbi_wstrb,
      inout  wire                             pcie1_dbi_wvalid,
      inout  wire                             pcie1_dbi_arready,
      inout  wire                             pcie1_dbi_awready,
      inout  wire   [3:0]                     pcie1_dbi_bid,
      inout  wire   [1:0]                     pcie1_dbi_bresp,
      inout  wire                             pcie1_dbi_bvalid,
      inout  wire  [31:0]                     pcie1_dbi_rdata,
      inout  wire   [3:0]                     pcie1_dbi_rid,
      inout  wire                             pcie1_dbi_rlast,
      inout  wire   [1:0]                     pcie1_dbi_rresp,
      inout  wire                             pcie1_dbi_rvalid,
      inout  wire                             pcie1_dbi_wready,

      inout  wire                             pcie1_mstr_arready,
      inout  wire                             pcie1_mstr_awready,
      inout  wire   [4:0]                     pcie1_mstr_bid,
      inout  wire   [1:0]                     pcie1_mstr_bresp,
      inout  wire                             pcie1_mstr_bvalid,
      inout  wire [127:0]                     pcie1_mstr_rdata,
      inout  wire   [4:0]                     pcie1_mstr_rid,
      inout  wire                             pcie1_mstr_rlast,
      inout  wire   [1:0]                     pcie1_mstr_rresp,
      inout  wire                             pcie1_mstr_rvalid,
      inout  wire                             pcie1_mstr_wready,
      inout  wire  [63:0]                     pcie1_mstr_araddr,
      inout  wire   [1:0]                     pcie1_mstr_arburst,
      inout  wire   [3:0]                     pcie1_mstr_arcache,
      inout  wire   [4:0]                     pcie1_mstr_arid,
      inout  wire   [2:0]                     pcie1_mstr_arlen,
      inout  wire                             pcie1_mstr_arlock,
      inout  wire   [2:0]                     pcie1_mstr_arprot,
      inout  wire   [3:0]                     pcie1_mstr_arqos,
      inout  wire   [2:0]                     pcie1_mstr_arsize,
      inout  wire                             pcie1_mstr_arvalid,
      inout  wire  [63:0]                     pcie1_mstr_awaddr,
      inout  wire   [1:0]                     pcie1_mstr_awburst,
      inout  wire   [3:0]                     pcie1_mstr_awcache,
      inout  wire   [4:0]                     pcie1_mstr_awid,
      inout  wire   [2:0]                     pcie1_mstr_awlen,
      inout  wire                             pcie1_mstr_awlock,
      inout  wire   [2:0]                     pcie1_mstr_awprot,
      inout  wire   [3:0]                     pcie1_mstr_awqos,
      inout  wire   [2:0]                     pcie1_mstr_awsize,
      inout  wire                             pcie1_mstr_awvalid,
      inout  wire                             pcie1_mstr_bready,
      inout  wire                             pcie1_mstr_rready,
      inout  wire [127:0]                     pcie1_mstr_wdata,
      inout  wire                             pcie1_mstr_wlast,
      inout  wire  [15:0]                     pcie1_mstr_wstrb,
      inout  wire                             pcie1_mstr_wvalid,

      inout  wire  [63:0]                     pcie1_slv_araddr,
      inout  wire   [1:0]                     pcie1_slv_arburst,
      inout  wire   [3:0]                     pcie1_slv_arcache,
      inout  wire  [15:0]                     pcie1_slv_arid,
      inout  wire   [2:0]                     pcie1_slv_arlen,
      inout  wire                             pcie1_slv_arlock,
      inout  wire   [2:0]                     pcie1_slv_arprot,
      inout  wire   [3:0]                     pcie1_slv_arqos,
      inout  wire   [2:0]                     pcie1_slv_arsize,
      inout  wire                             pcie1_slv_arvalid,
      inout  wire  [63:0]                     pcie1_slv_awaddr,
      inout  wire   [1:0]                     pcie1_slv_awburst,
      inout  wire   [3:0]                     pcie1_slv_awcache,
      inout  wire  [15:0]                     pcie1_slv_awid,
      inout  wire   [2:0]                     pcie1_slv_awlen,
      inout  wire                             pcie1_slv_awlock,
      inout  wire   [2:0]                     pcie1_slv_awprot,
      inout  wire   [3:0]                     pcie1_slv_awqos,
      inout  wire   [2:0]                     pcie1_slv_awsize,
      inout  wire                             pcie1_slv_awvalid,
      inout  wire                             pcie1_slv_bready,
      inout  wire                             pcie1_slv_rready,
      inout  wire [127:0]                     pcie1_slv_wdata,
      inout  wire                             pcie1_slv_wlast,
      inout  wire  [15:0]                     pcie1_slv_wstrb,
      inout  wire                             pcie1_slv_wvalid,
      inout  wire                             pcie1_slv_arready,
      inout  wire                             pcie1_slv_awready,
      inout  wire  [15:0]                     pcie1_slv_bid,
      inout  wire   [1:0]                     pcie1_slv_bresp,
      inout  wire                             pcie1_slv_bvalid,
      inout  wire [127:0]                     pcie1_slv_rdata,
      inout  wire  [15:0]                     pcie1_slv_rid,
      inout  wire                             pcie1_slv_rlast,
      inout  wire   [1:0]                     pcie1_slv_rresp,
      inout  wire                             pcie1_slv_rvalid,
      inout  wire                             pcie1_slv_wready,

      inout  wire [`IOS_APB_ADDR_WIDTH - 1:0] pshr_noc_pshr_pll0_slave_apb_paddr,
      inout  wire                             pshr_noc_pshr_pll0_slave_apb_penable,
      inout  wire [`IOS_APB_DATA_WIDTH - 1:0] pshr_noc_pshr_pll0_slave_apb_prdata,
      inout  wire                             pshr_noc_pshr_pll0_slave_apb_pready,
      inout  wire                             pshr_noc_pshr_pll0_slave_apb_psel,
      inout  wire                             pshr_noc_pshr_pll0_slave_apb_pslverr,
      inout  wire [`IOS_APB_DATA_WIDTH - 1:0] pshr_noc_pshr_pll0_slave_apb_pwdata,
      inout  wire                             pshr_noc_pshr_pll0_slave_apb_pwrite,

      inout  wire  [31:0]                     pshr_noc_pshr_esr_slave_apb_paddr,
      inout  wire                             pshr_noc_pshr_esr_slave_apb_penable,
      inout  wire  [31:0]                     pshr_noc_pshr_esr_slave_apb_prdata,
      inout  wire                             pshr_noc_pshr_esr_slave_apb_pready,
      inout  wire                             pshr_noc_pshr_esr_slave_apb_psel,
      inout  wire                             pshr_noc_pshr_esr_slave_apb_pslverr,
      inout  wire  [31:0]                     pshr_noc_pshr_esr_slave_apb_pwdata,
      inout  wire                             pshr_noc_pshr_esr_slave_apb_pwrite,

      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_awid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_awvalid,
      inout  wire  [39:0]                     pshr_esr_pshr_noc_master_axi_awaddr,
      inout  wire   [7:0]                     pshr_esr_pshr_noc_master_axi_awlen,
      inout  wire   [2:0]                     pshr_esr_pshr_noc_master_axi_awsize,
      inout  wire   [1:0]                     pshr_esr_pshr_noc_master_axi_awburst,
      inout  wire                             pshr_esr_pshr_noc_master_axi_awlock,
      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_awqos,
      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_awcache,
      inout  wire   [2:0]                     pshr_esr_pshr_noc_master_axi_awprot,
      inout  wire                             pshr_esr_pshr_noc_master_axi_awready,

      inout  wire                             pshr_esr_pshr_noc_master_axi_wvalid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_wlast,
      inout  wire  [63:0]                     pshr_esr_pshr_noc_master_axi_wdata,
      inout  wire   [7:0]                     pshr_esr_pshr_noc_master_axi_wstrb,
      inout  wire                             pshr_esr_pshr_noc_master_axi_wready,

      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_bid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_bvalid,
      inout  wire   [1:0]                     pshr_esr_pshr_noc_master_axi_bresp,
      inout  wire                             pshr_esr_pshr_noc_master_axi_bready,

      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_arid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_arvalid,
      inout  wire  [39:0]                     pshr_esr_pshr_noc_master_axi_araddr,
      inout  wire   [7:0]                     pshr_esr_pshr_noc_master_axi_arlen,
      inout  wire   [2:0]                     pshr_esr_pshr_noc_master_axi_arsize,
      inout  wire   [1:0]                     pshr_esr_pshr_noc_master_axi_arburst,
      inout  wire                             pshr_esr_pshr_noc_master_axi_arlock,
      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_arqos,
      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_arcache,
      inout  wire   [2:0]                     pshr_esr_pshr_noc_master_axi_arprot,
      inout  wire                             pshr_esr_pshr_noc_master_axi_arready,

      inout  wire   [3:0]                     pshr_esr_pshr_noc_master_axi_rid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_rvalid,
      inout  wire                             pshr_esr_pshr_noc_master_axi_rlast,
      inout  wire  [63:0]                     pshr_esr_pshr_noc_master_axi_rdata,
      inout  wire   [1:0]                     pshr_esr_pshr_noc_master_axi_rresp,
      inout  wire                             pshr_esr_pshr_noc_master_axi_rready,

      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_awid,
      inout  wire                             pshr_dbg_dma_am_master_axi_awvalid,
      inout  wire  [39:0]                     pshr_dbg_dma_am_master_axi_awaddr,
      inout  wire   [7:0]                     pshr_dbg_dma_am_master_axi_awlen,
      inout  wire   [2:0]                     pshr_dbg_dma_am_master_axi_awsize,
      inout  wire   [1:0]                     pshr_dbg_dma_am_master_axi_awburst,
      inout  wire                             pshr_dbg_dma_am_master_axi_awlock,
      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_awqos,
      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_awcache,
      inout  wire   [2:0]                     pshr_dbg_dma_am_master_axi_awprot,
      inout  wire                             pshr_dbg_dma_am_master_axi_awready,

      inout  wire                             pshr_dbg_dma_am_master_axi_wvalid,
      inout  wire                             pshr_dbg_dma_am_master_axi_wlast,
      inout  wire  [63:0]                     pshr_dbg_dma_am_master_axi_wdata,
      inout  wire   [7:0]                     pshr_dbg_dma_am_master_axi_wstrb,
      inout  wire                             pshr_dbg_dma_am_master_axi_wready,

      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_bid,
      inout  wire                             pshr_dbg_dma_am_master_axi_bvalid,
      inout  wire   [1:0]                     pshr_dbg_dma_am_master_axi_bresp,
      inout  wire                             pshr_dbg_dma_am_master_axi_bready,

      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_arid,
      inout  wire                             pshr_dbg_dma_am_master_axi_arvalid,
      inout  wire  [39:0]                     pshr_dbg_dma_am_master_axi_araddr,
      inout  wire   [7:0]                     pshr_dbg_dma_am_master_axi_arlen,
      inout  wire   [2:0]                     pshr_dbg_dma_am_master_axi_arsize,
      inout  wire   [1:0]                     pshr_dbg_dma_am_master_axi_arburst,
      inout  wire                             pshr_dbg_dma_am_master_axi_arlock,
      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_arqos,
      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_arcache,
      inout  wire   [2:0]                     pshr_dbg_dma_am_master_axi_arprot,
      inout  wire                             pshr_dbg_dma_am_master_axi_arready,

      inout  wire   [3:0]                     pshr_dbg_dma_am_master_axi_rid,
      inout  wire                             pshr_dbg_dma_am_master_axi_rvalid,
      inout  wire                             pshr_dbg_dma_am_master_axi_rlast,
      inout  wire  [63:0]                     pshr_dbg_dma_am_master_axi_rdata,
      inout  wire   [1:0]                     pshr_dbg_dma_am_master_axi_rresp,
      inout  wire                             pshr_dbg_dma_am_master_axi_rready
   );


   //-----------------------------------------------------------------------------------------------
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_pshire_noc_pkg::*;

   generate
      if (PSHIRE_NOC_ONLY == 1'b1) begin: g_pnoc_axi_ifcs
         tri0 [`EVL_BUS_REGION_MSB:0] p0_spio_m_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] p0_spio_m_AWREGION;
         tri0                         p0_spio_m_WUSER;
         tri0                         p0_spio_m_AWUSER;
         tri0                         p0_spio_m_BUSER;
         tri0                         p0_spio_m_RUSER;
         tri0                         p0_spio_m_ARUSER;

         // p0_spio_master (direct connection to IOShire spio NoC)
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (6),
                             .ADDR_WIDTH   (40),
                             .DATA_WIDTH   (32),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_p0_spio_master_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (ios_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (p0_spio_m_AWID),
            .aw_addr      (p0_spio_m_AWADDR),
            .aw_len       (p0_spio_m_AWLEN),
            .aw_size      (p0_spio_m_AWSIZE),
            .aw_burst     (p0_spio_m_AWBURST),
            .aw_lock      (p0_spio_m_AWLOCK),
            .aw_cache     (p0_spio_m_AWCACHE),
            .aw_prot      (p0_spio_m_AWPROT),
            .aw_qos       (p0_spio_m_AWQOS),
            .aw_region    (p0_spio_m_AWREGION),
            .aw_user      (p0_spio_m_AWUSER),
            .aw_valid     (p0_spio_m_AWVALID),
            .aw_ready     (p0_spio_m_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (p0_spio_m_WUSER),
            .w_data       (p0_spio_m_WDATA),
            .w_strb       (p0_spio_m_WSTRB),
            .w_last       (p0_spio_m_WLAST),
            .w_valid      (p0_spio_m_WVALID),
            .w_ready      (p0_spio_m_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (p0_spio_m_BUSER),
            .b_id         (p0_spio_m_BID),
            .b_resp       (p0_spio_m_BRESP),
            .b_valid      (p0_spio_m_BVALID),
            .b_ready      (p0_spio_m_BREADY),
            .b_trans_id   (),

            .ar_id        (p0_spio_m_ARID),
            .ar_addr      (p0_spio_m_ARADDR),
            .ar_len       (p0_spio_m_ARLEN),
            .ar_size      (p0_spio_m_ARSIZE),
            .ar_burst     (p0_spio_m_ARBURST),
            .ar_lock      (p0_spio_m_ARLOCK),
            .ar_cache     (p0_spio_m_ARCACHE),
            .ar_prot      (p0_spio_m_ARPROT),
            .ar_qos       (p0_spio_m_ARQOS),
            .ar_region    (p0_spio_m_ARREGION),
            .ar_user      (p0_spio_m_ARUSER),
            .ar_valid     (p0_spio_m_ARVALID),
            .ar_ready     (p0_spio_m_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (p0_spio_m_RLAST),
            .r_user       (p0_spio_m_RUSER),
            .r_id         (p0_spio_m_RID),
            .r_data       (p0_spio_m_RDATA),
            .r_resp       (p0_spio_m_RRESP),
            .r_valid      (p0_spio_m_RVALID),
            .r_ready      (p0_spio_m_RREADY),
            .r_trans_id   ()
         );

         // PCIE0
         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_mstr_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_mstr_awregion;
         tri0                         pcie0_mstr_awuser;
         tri0                         pcie0_mstr_wuser;
         tri0                         pcie0_mstr_buser;
         tri0                         pcie0_mstr_ruser;
         tri0                         pcie0_mstr_aruser;

         // pcie0_mstr
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (5),
                             .ADDR_WIDTH   (64),
                             .DATA_WIDTH   (256),
                             .AXLEN_WIDTH  (3),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie0_mstr_master_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie0_mstr_awid),
            .aw_addr      (pcie0_mstr_awaddr),
            .aw_len       (pcie0_mstr_awlen),
            .aw_size      (pcie0_mstr_awsize),
            .aw_burst     (pcie0_mstr_awburst),
            .aw_lock      (pcie0_mstr_awlock),
            .aw_cache     (pcie0_mstr_awcache),
            .aw_prot      (pcie0_mstr_awprot),
            .aw_qos       (pcie0_mstr_awqos),
            .aw_region    (pcie0_mstr_awregion),
            .aw_user      (pcie0_mstr_awuser),
            .aw_valid     (pcie0_mstr_awvalid),
            .aw_ready     (pcie0_mstr_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie0_mstr_wuser),
            .w_data       (pcie0_mstr_wdata),
            .w_strb       (pcie0_mstr_wstrb),
            .w_last       (pcie0_mstr_wlast),
            .w_valid      (pcie0_mstr_wvalid),
            .w_ready      (pcie0_mstr_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie0_mstr_buser),
            .b_id         (pcie0_mstr_bid),
            .b_resp       (pcie0_mstr_bresp),
            .b_valid      (pcie0_mstr_bvalid),
            .b_ready      (pcie0_mstr_bready),
            .b_trans_id   (),

            .ar_id        (pcie0_mstr_arid),
            .ar_addr      (pcie0_mstr_araddr),
            .ar_len       (pcie0_mstr_arlen),
            .ar_size      (pcie0_mstr_arsize),
            .ar_burst     (pcie0_mstr_arburst),
            .ar_lock      (pcie0_mstr_arlock),
            .ar_cache     (pcie0_mstr_arcache),
            .ar_prot      (pcie0_mstr_arprot),
            .ar_qos       (pcie0_mstr_arqos),
            .ar_region    (pcie0_mstr_arregion),
            .ar_user      (pcie0_mstr_aruser),
            .ar_valid     (pcie0_mstr_arvalid),
            .ar_ready     (pcie0_mstr_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie0_mstr_rlast),
            .r_user       (pcie0_mstr_ruser),
            .r_id         (pcie0_mstr_rid),
            .r_data       (pcie0_mstr_rdata),
            .r_resp       (pcie0_mstr_rresp),
            .r_valid      (pcie0_mstr_rvalid),
            .r_ready      (pcie0_mstr_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_dbi_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_dbi_awregion;
         tri0                         pcie0_dbi_awuser;
         tri0                         pcie0_dbi_wuser;
         tri0                         pcie0_dbi_buser;
         tri0                         pcie0_dbi_aruser;
         tri0                         pcie0_dbi_ruser;

         // pcie0_dbi
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (4),
                             .ADDR_WIDTH   (32),
                             .DATA_WIDTH   (32),
                             .AXLEN_WIDTH  (4),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie0_dbi_slave_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie0_dbi_awid),
            .aw_addr      (pcie0_dbi_awaddr),
            .aw_len       (pcie0_dbi_awlen),
            .aw_size      (pcie0_dbi_awsize),
            .aw_burst     (pcie0_dbi_awburst),
            .aw_lock      (pcie0_dbi_awlock),
            .aw_cache     (pcie0_dbi_awcache),
            .aw_prot      (pcie0_dbi_awprot),
            .aw_qos       (pcie0_dbi_awqos),
            .aw_region    (pcie0_dbi_awregion),
            .aw_user      (pcie0_dbi_awuser),
            .aw_valid     (pcie0_dbi_awvalid),
            .aw_ready     (pcie0_dbi_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie0_dbi_wuser),
            .w_data       (pcie0_dbi_wdata),
            .w_strb       (pcie0_dbi_wstrb),
            .w_last       (pcie0_dbi_wlast),
            .w_valid      (pcie0_dbi_wvalid),
            .w_ready      (pcie0_dbi_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie0_dbi_buser),
            .b_id         (pcie0_dbi_bid),
            .b_resp       (pcie0_dbi_bresp),
            .b_valid      (pcie0_dbi_bvalid),
            .b_ready      (pcie0_dbi_bready),
            .b_trans_id   (),

            .ar_id        (pcie0_dbi_arid),
            .ar_addr      (pcie0_dbi_araddr),
            .ar_len       (pcie0_dbi_arlen),
            .ar_size      (pcie0_dbi_arsize),
            .ar_burst     (pcie0_dbi_arburst),
            .ar_lock      (pcie0_dbi_arlock),
            .ar_cache     (pcie0_dbi_arcache),
            .ar_prot      (pcie0_dbi_arprot),
            .ar_qos       (pcie0_dbi_arqos),
            .ar_region    (pcie0_dbi_arregion),
            .ar_user      (pcie0_dbi_aruser),
            .ar_valid     (pcie0_dbi_arvalid),
            .ar_ready     (pcie0_dbi_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie0_dbi_rlast),
            .r_user       (pcie0_dbi_ruser),
            .r_id         (pcie0_dbi_rid),
            .r_data       (pcie0_dbi_rdata),
            .r_resp       (pcie0_dbi_rresp),
            .r_valid      (pcie0_dbi_rvalid),
            .r_ready      (pcie0_dbi_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_slv_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie0_slv_awregion;
         tri0                         pcie0_slv_awuser;
         tri0                         pcie0_slv_wuser;
         tri0                         pcie0_slv_buser;
         tri0                         pcie0_slv_aruser;
         tri0                         pcie0_slv_ruser;

         // pcie0_slv
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (16),
                             .ADDR_WIDTH   (64),
                             .DATA_WIDTH   (256),
                             .AXLEN_WIDTH  (3),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie0_slv_slave_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie0_slv_awid),
            .aw_addr      (pcie0_slv_awaddr),
            .aw_len       (pcie0_slv_awlen),
            .aw_size      (pcie0_slv_awsize),
            .aw_burst     (pcie0_slv_awburst),
            .aw_lock      (pcie0_slv_awlock),
            .aw_cache     (pcie0_slv_awcache),
            .aw_prot      (pcie0_slv_awprot),
            .aw_qos       (pcie0_slv_awqos),
            .aw_region    (pcie0_slv_awregion),
            .aw_user      (pcie0_slv_awuser),
            .aw_valid     (pcie0_slv_awvalid),
            .aw_ready     (pcie0_slv_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie0_slv_wuser),
            .w_data       (pcie0_slv_wdata),
            .w_strb       (pcie0_slv_wstrb),
            .w_last       (pcie0_slv_wlast),
            .w_valid      (pcie0_slv_wvalid),
            .w_ready      (pcie0_slv_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie0_slv_buser),
            .b_id         (pcie0_slv_bid),
            .b_resp       (pcie0_slv_bresp),
            .b_valid      (pcie0_slv_bvalid),
            .b_ready      (pcie0_slv_bready),
            .b_trans_id   (),

            .ar_id        (pcie0_slv_arid),
            .ar_addr      (pcie0_slv_araddr),
            .ar_len       (pcie0_slv_arlen),
            .ar_size      (pcie0_slv_arsize),
            .ar_burst     (pcie0_slv_arburst),
            .ar_lock      (pcie0_slv_arlock),
            .ar_cache     (pcie0_slv_arcache),
            .ar_prot      (pcie0_slv_arprot),
            .ar_qos       (pcie0_slv_arqos),
            .ar_region    (pcie0_slv_arregion),
            .ar_user      (pcie0_slv_aruser),
            .ar_valid     (pcie0_slv_arvalid),
            .ar_ready     (pcie0_slv_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie0_slv_rlast),
            .r_user       (pcie0_slv_ruser),
            .r_id         (pcie0_slv_rid),
            .r_data       (pcie0_slv_rdata),
            .r_resp       (pcie0_slv_rresp),
            .r_valid      (pcie0_slv_rvalid),
            .r_ready      (pcie0_slv_rready),
            .r_trans_id   ()
         );


         // PCIE1
         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_mstr_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_mstr_awregion;
         tri0                         pcie1_mstr_wuser;
         tri0                         pcie1_mstr_awuser;
         tri0                         pcie1_mstr_buser;
         tri0                         pcie1_mstr_aruser;
         tri0                         pcie1_mstr_ruser;

         // pcie1_mstr
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (5),
                             .ADDR_WIDTH   (64),
                             .DATA_WIDTH   (128),
                             .AXLEN_WIDTH  (3),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie1_mstr_master_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie1_mstr_awid),
            .aw_addr      (pcie1_mstr_awaddr),
            .aw_len       (pcie1_mstr_awlen),
            .aw_size      (pcie1_mstr_awsize),
            .aw_burst     (pcie1_mstr_awburst),
            .aw_lock      (pcie1_mstr_awlock),
            .aw_cache     (pcie1_mstr_awcache),
            .aw_prot      (pcie1_mstr_awprot),
            .aw_qos       (pcie1_mstr_awqos),
            .aw_region    (pcie1_mstr_awregion),
            .aw_user      (pcie1_mstr_awuser),
            .aw_valid     (pcie1_mstr_awvalid),
            .aw_ready     (pcie1_mstr_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie1_mstr_wuser),
            .w_data       (pcie1_mstr_wdata),
            .w_strb       (pcie1_mstr_wstrb),
            .w_last       (pcie1_mstr_wlast),
            .w_valid      (pcie1_mstr_wvalid),
            .w_ready      (pcie1_mstr_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie1_mstr_buser),
            .b_id         (pcie1_mstr_bid),
            .b_resp       (pcie1_mstr_bresp),
            .b_valid      (pcie1_mstr_bvalid),
            .b_ready      (pcie1_mstr_bready),
            .b_trans_id   (),

            .ar_id        (pcie1_mstr_arid),
            .ar_addr      (pcie1_mstr_araddr),
            .ar_len       (pcie1_mstr_arlen),
            .ar_size      (pcie1_mstr_arsize),
            .ar_burst     (pcie1_mstr_arburst),
            .ar_lock      (pcie1_mstr_arlock),
            .ar_cache     (pcie1_mstr_arcache),
            .ar_prot      (pcie1_mstr_arprot),
            .ar_qos       (pcie1_mstr_arqos),
            .ar_region    (pcie1_mstr_arregion),
            .ar_user      (pcie1_mstr_aruser),
            .ar_valid     (pcie1_mstr_arvalid),
            .ar_ready     (pcie1_mstr_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie1_mstr_rlast),
            .r_user       (pcie1_mstr_ruser),
            .r_id         (pcie1_mstr_rid),
            .r_data       (pcie1_mstr_rdata),
            .r_resp       (pcie1_mstr_rresp),
            .r_valid      (pcie1_mstr_rvalid),
            .r_ready      (pcie1_mstr_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_dbi_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_dbi_awregion;
         tri0                         pcie1_dbi_awuser;
         tri0                         pcie1_dbi_wuser;
         tri0                         pcie1_dbi_buser;
         tri0                         pcie1_dbi_aruser;
         tri0                         pcie1_dbi_ruser;

         // pcie1_dbi
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (4),
                             .ADDR_WIDTH   (32),
                             .DATA_WIDTH   (32),
                             .AXLEN_WIDTH  (4),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie1_dbi_slave_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie1_dbi_awid),
            .aw_addr      (pcie1_dbi_awaddr),
            .aw_len       (pcie1_dbi_awlen),
            .aw_size      (pcie1_dbi_awsize),
            .aw_burst     (pcie1_dbi_awburst),
            .aw_lock      (pcie1_dbi_awlock),
            .aw_cache     (pcie1_dbi_awcache),
            .aw_prot      (pcie1_dbi_awprot),
            .aw_qos       (pcie1_dbi_awqos),
            .aw_region    (pcie1_dbi_awregion),
            .aw_user      (pcie1_dbi_awuser),
            .aw_valid     (pcie1_dbi_awvalid),
            .aw_ready     (pcie1_dbi_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie1_dbi_wuser),
            .w_data       (pcie1_dbi_wdata),
            .w_strb       (pcie1_dbi_wstrb),
            .w_last       (pcie1_dbi_wlast),
            .w_valid      (pcie1_dbi_wvalid),
            .w_ready      (pcie1_dbi_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie1_dbi_buser),
            .b_id         (pcie1_dbi_bid),
            .b_resp       (pcie1_dbi_bresp),
            .b_valid      (pcie1_dbi_bvalid),
            .b_ready      (pcie1_dbi_bready),
            .b_trans_id   (),

            .ar_id        (pcie1_dbi_arid),
            .ar_addr      (pcie1_dbi_araddr),
            .ar_len       (pcie1_dbi_arlen),
            .ar_size      (pcie1_dbi_arsize),
            .ar_burst     (pcie1_dbi_arburst),
            .ar_lock      (pcie1_dbi_arlock),
            .ar_cache     (pcie1_dbi_arcache),
            .ar_prot      (pcie1_dbi_arprot),
            .ar_qos       (pcie1_dbi_arqos),
            .ar_region    (pcie1_dbi_arregion),
            .ar_user      (pcie1_dbi_aruser),
            .ar_valid     (pcie1_dbi_arvalid),
            .ar_ready     (pcie1_dbi_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie1_dbi_rlast),
            .r_user       (pcie1_dbi_ruser),
            .r_id         (pcie1_dbi_rid),
            .r_data       (pcie1_dbi_rdata),
            .r_resp       (pcie1_dbi_rresp),
            .r_valid      (pcie1_dbi_rvalid),
            .r_ready      (pcie1_dbi_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_slv_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pcie1_slv_awregion;
         tri0                         pcie1_slv_awuser;
         tri0                         pcie1_slv_wuser;
         tri0                         pcie1_slv_buser;
         tri0                         pcie1_slv_aruser;
         tri0                         pcie1_slv_ruser;

         // pcie1_slv
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (16),
                             .ADDR_WIDTH   (64),
                             .DATA_WIDTH   (128),
                             .AXLEN_WIDTH  (3),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pcie1_slv_slave_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pcie1_slv_awid),
            .aw_addr      (pcie1_slv_awaddr),
            .aw_len       (pcie1_slv_awlen),
            .aw_size      (pcie1_slv_awsize),
            .aw_burst     (pcie1_slv_awburst),
            .aw_lock      (pcie1_slv_awlock),
            .aw_cache     (pcie1_slv_awcache),
            .aw_prot      (pcie1_slv_awprot),
            .aw_qos       (pcie1_slv_awqos),
            .aw_region    (pcie1_slv_awregion),
            .aw_user      (pcie1_slv_awuser),
            .aw_valid     (pcie1_slv_awvalid),
            .aw_ready     (pcie1_slv_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pcie1_slv_wuser),
            .w_data       (pcie1_slv_wdata),
            .w_strb       (pcie1_slv_wstrb),
            .w_last       (pcie1_slv_wlast),
            .w_valid      (pcie1_slv_wvalid),
            .w_ready      (pcie1_slv_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pcie1_slv_buser),
            .b_id         (pcie1_slv_bid),
            .b_resp       (pcie1_slv_bresp),
            .b_valid      (pcie1_slv_bvalid),
            .b_ready      (pcie1_slv_bready),
            .b_trans_id   (),

            .ar_id        (pcie1_slv_arid),
            .ar_addr      (pcie1_slv_araddr),
            .ar_len       (pcie1_slv_arlen),
            .ar_size      (pcie1_slv_arsize),
            .ar_burst     (pcie1_slv_arburst),
            .ar_lock      (pcie1_slv_arlock),
            .ar_cache     (pcie1_slv_arcache),
            .ar_prot      (pcie1_slv_arprot),
            .ar_qos       (pcie1_slv_arqos),
            .ar_region    (pcie1_slv_arregion),
            .ar_user      (pcie1_slv_aruser),
            .ar_valid     (pcie1_slv_arvalid),
            .ar_ready     (pcie1_slv_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pcie1_slv_rlast),
            .r_user       (pcie1_slv_ruser),
            .r_id         (pcie1_slv_rid),
            .r_data       (pcie1_slv_rdata),
            .r_resp       (pcie1_slv_rresp),
            .r_valid      (pcie1_slv_rvalid),
            .r_ready      (pcie1_slv_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pshr_esr_pshr_noc_master_axi_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pshr_esr_pshr_noc_master_axi_awregion;
         tri0                         pshr_esr_pshr_noc_master_axi_wuser;
         tri0                         pshr_esr_pshr_noc_master_axi_awuser;
         tri0                         pshr_esr_pshr_noc_master_axi_buser;
         tri0                         pshr_esr_pshr_noc_master_axi_ruser;
         tri0                         pshr_esr_pshr_noc_master_axi_aruser;

         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (4),
                             .ADDR_WIDTH   (40),
                             .DATA_WIDTH   (64),
                             .AXLEN_WIDTH  (8),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pshr_esr_pshr_noc_master_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pshr_esr_pshr_noc_master_axi_awid),
            .aw_addr      (pshr_esr_pshr_noc_master_axi_awaddr),
            .aw_len       (pshr_esr_pshr_noc_master_axi_awlen),
            .aw_size      (pshr_esr_pshr_noc_master_axi_awsize),
            .aw_burst     (pshr_esr_pshr_noc_master_axi_awburst),
            .aw_lock      (pshr_esr_pshr_noc_master_axi_awlock),
            .aw_cache     (pshr_esr_pshr_noc_master_axi_awcache),
            .aw_prot      (pshr_esr_pshr_noc_master_axi_awprot),
            .aw_qos       (pshr_esr_pshr_noc_master_axi_awqos),
            .aw_region    (pshr_esr_pshr_noc_master_axi_awregion),
            .aw_user      (pshr_esr_pshr_noc_master_axi_awuser),
            .aw_valid     (pshr_esr_pshr_noc_master_axi_awvalid),
            .aw_ready     (pshr_esr_pshr_noc_master_axi_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pshr_esr_pshr_noc_master_axi_wuser),
            .w_data       (pshr_esr_pshr_noc_master_axi_wdata),
            .w_strb       (pshr_esr_pshr_noc_master_axi_wstrb),
            .w_last       (pshr_esr_pshr_noc_master_axi_wlast),
            .w_valid      (pshr_esr_pshr_noc_master_axi_wvalid),
            .w_ready      (pshr_esr_pshr_noc_master_axi_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pshr_esr_pshr_noc_master_axi_buser),
            .b_id         (pshr_esr_pshr_noc_master_axi_bid),
            .b_resp       (pshr_esr_pshr_noc_master_axi_bresp),
            .b_valid      (pshr_esr_pshr_noc_master_axi_bvalid),
            .b_ready      (pshr_esr_pshr_noc_master_axi_bready),
            .b_trans_id   (),

            .ar_id        (pshr_esr_pshr_noc_master_axi_arid),
            .ar_addr      (pshr_esr_pshr_noc_master_axi_araddr),
            .ar_len       (pshr_esr_pshr_noc_master_axi_arlen),
            .ar_size      (pshr_esr_pshr_noc_master_axi_arsize),
            .ar_burst     (pshr_esr_pshr_noc_master_axi_arburst),
            .ar_lock      (pshr_esr_pshr_noc_master_axi_arlock),
            .ar_cache     (pshr_esr_pshr_noc_master_axi_arcache),
            .ar_prot      (pshr_esr_pshr_noc_master_axi_arprot),
            .ar_qos       (pshr_esr_pshr_noc_master_axi_arqos),
            .ar_region    (pshr_esr_pshr_noc_master_axi_arregion),
            .ar_user      (pshr_esr_pshr_noc_master_axi_aruser),
            .ar_valid     (pshr_esr_pshr_noc_master_axi_arvalid),
            .ar_ready     (pshr_esr_pshr_noc_master_axi_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pshr_esr_pshr_noc_master_axi_rlast),
            .r_user       (pshr_esr_pshr_noc_master_axi_ruser),
            .r_id         (pshr_esr_pshr_noc_master_axi_rid),
            .r_data       (pshr_esr_pshr_noc_master_axi_rdata),
            .r_resp       (pshr_esr_pshr_noc_master_axi_rresp),
            .r_valid      (pshr_esr_pshr_noc_master_axi_rvalid),
            .r_ready      (pshr_esr_pshr_noc_master_axi_rready),
            .r_trans_id   ()
         );

         tri0 [`EVL_BUS_REGION_MSB:0] pshr_dbg_dma_am_master_axi_arregion;
         tri0 [`EVL_BUS_REGION_MSB:0] pshr_dbg_dma_am_master_axi_awregion;
         tri0                         pshr_dbg_dma_am_master_axi_wuser;
         tri0                         pshr_dbg_dma_am_master_axi_awuser;
         tri0                         pshr_dbg_dma_am_master_axi_buser;
         tri0                         pshr_dbg_dma_am_master_axi_ruser;
         tri0                         pshr_dbg_dma_am_master_axi_aruser;

         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (4),
                             .ADDR_WIDTH   (40),
                             .DATA_WIDTH   (64),
                             .AXLEN_WIDTH  (8),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (1),
                             .AWUSER_WIDTH (1)) m_pshr_dbg_dma_am_master_axi (
            .isolate      (1'b0),
            .reset        (reset),
            .clk          (pcie_axi_clk),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (pshr_dbg_dma_am_master_axi_awid),
            .aw_addr      (pshr_dbg_dma_am_master_axi_awaddr),
            .aw_len       (pshr_dbg_dma_am_master_axi_awlen),
            .aw_size      (pshr_dbg_dma_am_master_axi_awsize),
            .aw_burst     (pshr_dbg_dma_am_master_axi_awburst),
            .aw_lock      (pshr_dbg_dma_am_master_axi_awlock),
            .aw_cache     (pshr_dbg_dma_am_master_axi_awcache),
            .aw_prot      (pshr_dbg_dma_am_master_axi_awprot),
            .aw_qos       (pshr_dbg_dma_am_master_axi_awqos),
            .aw_region    (pshr_dbg_dma_am_master_axi_awregion),
            .aw_user      (pshr_dbg_dma_am_master_axi_awuser),
            .aw_valid     (pshr_dbg_dma_am_master_axi_awvalid),
            .aw_ready     (pshr_dbg_dma_am_master_axi_awready),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),

            .w_user       (pshr_dbg_dma_am_master_axi_wuser),
            .w_data       (pshr_dbg_dma_am_master_axi_wdata),
            .w_strb       (pshr_dbg_dma_am_master_axi_wstrb),
            .w_last       (pshr_dbg_dma_am_master_axi_wlast),
            .w_valid      (pshr_dbg_dma_am_master_axi_wvalid),
            .w_ready      (pshr_dbg_dma_am_master_axi_wready),
            .w_vc_valid   (),
            .w_trans_id   (),

            .b_user       (pshr_dbg_dma_am_master_axi_buser),
            .b_id         (pshr_dbg_dma_am_master_axi_bid),
            .b_resp       (pshr_dbg_dma_am_master_axi_bresp),
            .b_valid      (pshr_dbg_dma_am_master_axi_bvalid),
            .b_ready      (pshr_dbg_dma_am_master_axi_bready),
            .b_trans_id   (),

            .ar_id        (pshr_dbg_dma_am_master_axi_arid),
            .ar_addr      (pshr_dbg_dma_am_master_axi_araddr),
            .ar_len       (pshr_dbg_dma_am_master_axi_arlen),
            .ar_size      (pshr_dbg_dma_am_master_axi_arsize),
            .ar_burst     (pshr_dbg_dma_am_master_axi_arburst),
            .ar_lock      (pshr_dbg_dma_am_master_axi_arlock),
            .ar_cache     (pshr_dbg_dma_am_master_axi_arcache),
            .ar_prot      (pshr_dbg_dma_am_master_axi_arprot),
            .ar_qos       (pshr_dbg_dma_am_master_axi_arqos),
            .ar_region    (pshr_dbg_dma_am_master_axi_arregion),
            .ar_user      (pshr_dbg_dma_am_master_axi_aruser),
            .ar_valid     (pshr_dbg_dma_am_master_axi_arvalid),
            .ar_ready     (pshr_dbg_dma_am_master_axi_arready),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),

            .r_last       (pshr_dbg_dma_am_master_axi_rlast),
            .r_user       (pshr_dbg_dma_am_master_axi_ruser),
            .r_id         (pshr_dbg_dma_am_master_axi_rid),
            .r_data       (pshr_dbg_dma_am_master_axi_rdata),
            .r_resp       (pshr_dbg_dma_am_master_axi_rresp),
            .r_valid      (pshr_dbg_dma_am_master_axi_rvalid),
            .r_ready      (pshr_dbg_dma_am_master_axi_rready),
            .r_trans_id   ()
         );
      end
   endgenerate

   generate
      if ((PSHIRE_NO_ESR == 1'b1) && (PSHIRE_NOC_ONLY == 1'b1)) begin : g_pnoc_apb_ifcs
         //evl_apb_interface #(.AGENT_ID   (0),
         //                    .ADDR_WIDTH (32),
         //                    .DATA_WIDTH (32)) m_apbslv (
         //      .isolate      ( 1'b0           ),
         //      .pclk         ( apb_clk        ),
         //      .presetn      ( reset          ),
         //      .penable      ( apbslv_penable ),
         //      .psel         ( apbslv_psel    ),
         //      .pwrite       ( apbslv_pwrite  ),
         //      .paddr        ( apbslv_paddr   ),
         //      .pwdata       ( apbslv_pwdata  ),
         //      .pready       ( apbslv_pready  ),
         //      .prdata       ( apbslv_prdata  ),
         //      .pslverr      ( apbslv_pslverr )
         //   );

         //evl_apb_interface #(.ADDR_WIDTH (`IOS_APB_ADDR_WIDTH),
         //                    .DATA_WIDTH (`IOS_APB_ADDR_WIDTH)) m_pshr_pll0_slave_apb (
         //      .isolate      ( 1'b0           ),
         //      .pclk         ( apb_clk        ),
         //      .presetn      ( reset          ),
         //      .penable      ( pshr_noc_pshr_pll0_slave_apb_penable ),
         //      .psel         ( pshr_noc_pshr_pll0_slave_apb_psel    ),
         //      .pwrite       ( pshr_noc_pshr_pll0_slave_apb_pwrite  ),
         //      .paddr        ( pshr_noc_pshr_pll0_slave_apb_paddr   ),
         //      .pwdata       ( pshr_noc_pshr_pll0_slave_apb_pwdata  ),
         //      .pready       ( pshr_noc_pshr_pll0_slave_apb_pready  ),
         //      .prdata       ( pshr_noc_pshr_pll0_slave_apb_prdata  ),
         //      .pslverr      ( pshr_noc_pshr_pll0_slave_apb_pslverr )
         //   );

         //evl_apb_interface #(.ADDR_WIDTH (32),
         //                    .DATA_WIDTH (32)) m_pshr_esr_slave_apb (
         //      .isolate      ( 1'b0           ),
         //      .pclk         ( apb_clk        ),
         //      .presetn      ( reset          ),
         //      .penable      ( pshr_noc_pshr_esr_slave_apb_penable ),
         //      .psel         ( pshr_noc_pshr_esr_slave_apb_psel    ),
         //      .pwrite       ( pshr_noc_pshr_esr_slave_apb_pwrite  ),
         //      .paddr        ( pshr_noc_pshr_esr_slave_apb_paddr   ),
         //      .pwdata       ( pshr_noc_pshr_esr_slave_apb_pwdata  ),
         //      .pready       ( pshr_noc_pshr_esr_slave_apb_pready  ),
         //      .prdata       ( pshr_noc_pshr_esr_slave_apb_prdata  ),
         //      .pslverr      ( pshr_noc_pshr_esr_slave_apb_pslverr )
         //   );

         assign apbslv_pready = 1'b1;
         assign apbslv_prdata = 32'b0;
         assign apbslv_pslverr = 1'b0;
         assign pshr_noc_pshr_pll0_slave_apb_pready = 1'b1;
         assign pshr_noc_pshr_pll0_slave_apb_prdata = 32'b0;
         assign pshr_noc_pshr_pll0_slave_apb_pslverr = 1'b0;
         assign pshr_noc_pshr_esr_slave_apb_pready = 1'b1;
         assign pshr_noc_pshr_esr_slave_apb_prdata = 32'b0;
         assign pshr_noc_pshr_esr_slave_apb_pslverr = 1'b0;
      end
   endgenerate


   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   int m_pshire_id;
   int m_zebu_driver = 1'b0;
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                               PSHIRE_NOC_RTL_AGENT,
                                                               PSHIRE_ID,
                                                               "evl_pshire_noc_env", "", "", "", { 1 });

   assign zebu_driver =  m_zebu_driver;


   //-----------------------------------------------------------------------------------------------
   //
   // Create a Shire RTL port instance and drive it appropriately.
   //
   evl_pshire_noc_rtl_port m_rtl_port      = new($sformatf("%m"), PSHIRE_ID, "", { 1 });
   bit                     m_start_monitor = 1'b0;
   bit                     m_reset_on      = 1'b0;
   bit                     m_reset_off     = 1'b0;
   string                  m_plus_arg;

   initial begin
      m_zebu_driver = (`et_test_plusargs("use_zebu_pci_driver=1") == 0) ? 1'b0 : ALLOW_ZEBU_DRIVER;
      m_rtl_port.set_int_param_by_name("pk_pshire_noc_only", ((PSHIRE_NOC_ONLY == 0) ? 0 : 1));
      m_rtl_port.set_int_param_by_name("pk_pshire_no_esr", ((PSHIRE_NO_ESR == 0) ? 0 : 1));
      m_rtl_port.set_int_param_by_name("pk_zebu_driver", int'(m_zebu_driver));

      m_pshire_id = PSHIRE_ID;
      #1ns m_start_monitor = 1'b1;
      @(posedge pcie_axi_clk);
   end

   always @(posedge pcie_axi_clk) begin
      if (m_start_monitor === 1'b1) begin
         if (reset === 1'b1) begin
            if (m_reset_on === 1'b0) begin
               m_rtl_port.assert_reset();
            end
         end
         else if (reset === 1'b0) begin
            if (m_reset_off === 1'b0) begin
               m_rtl_port.deassert_reset();
            end
         end
         m_rtl_port.ping_clock();
      end
   end

   wire unused_ok = &{ 1'b0,
                       noc_clk,
                       ios_clk,
                       apb_clk,
                       1'b0
                     };

endmodule

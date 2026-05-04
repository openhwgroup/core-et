`include "rtl/inc/soc.vh"
`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"
`include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
`include "dv/common/tlink/tlink_vc/evl_tlink_pkg.sv"
`include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_pkg.sv"
`include "dv/maxion/maxshire_tb/evl_maxshire_tb_pkg.sv"
`include "dv/soc/shire/shire_channel/evl_shire_channel_pkg.sv"
`include "dv/shire_common/shire_cache/evl_shire_cache_pkg.sv"

`ifndef __EVL_IOSHIRE_PKG
   `include "dv/soc/ioshire/evl_ioshire_pkg.sv"
`endif

`ifdef IOSHIRE_WRAPPER_GATESIM
   `define EVL_IFC_PORT input
`endif

`ifndef EVL_MAXSHIRE_PATH
   `define EVL_MAXSHIRE_PATH ioshire_wrapper.i_ioshire_top.maxshire_top
`endif

module evl_ioshire_interface #(parameter int    IOSHIRE_ID        = 0,
                               parameter int    SP_IS_STUB        = 0,
                               parameter int    NOC_IS_STUB       = 0,
                               parameter int    NUM_L3_PORTS      = 2,
                               parameter bit    IOSHIRE_NOC_ONLY  = 1'b0,
                               parameter bit    IOSHIRE_NO_PU     = 1'b0,
                               parameter string ROM_FILE_NAME     = "spROMImage.hex",
                               parameter string RAM_FILE_NAME     = "spRAMImage.hex") (

      input wire                                    reset_ios_ckcore,
      input wire                                    clk__noc,
      input wire                                    clk_ios,
      input wire                                    clk_ios_ahb,
      input wire                                    clk_maxshire_uncore,
      input wire                                    clk_maxshire_core,

      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb0_paddr,
      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb0_pwdata,
      `EVL_IFC_PORT wire                            spio_p10_apb0_pwrite,
      `EVL_IFC_PORT wire                            spio_p10_apb0_psel,
      `EVL_IFC_PORT wire                            spio_p10_apb0_penable,
      `EVL_IFC_PORT wire                            spio_p10_apb0_pready,
      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb0_prdata,
      `EVL_IFC_PORT wire                            spio_p10_apb0_pslverr,

      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb1_paddr,
      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb1_pwdata,
      `EVL_IFC_PORT wire                            spio_p10_apb1_pwrite,
      `EVL_IFC_PORT wire                            spio_p10_apb1_psel,
      `EVL_IFC_PORT wire                            spio_p10_apb1_penable,
      `EVL_IFC_PORT wire                            spio_p10_apb1_pready,
      `EVL_IFC_PORT wire [ 31:0]                    spio_p10_apb1_prdata,
      `EVL_IFC_PORT wire                            spio_p10_apb1_pslverr,

      // Main NoC interfaces
      `EVL_IFC_PORT wire sc_slave_axi_AR_channel_t  ioshire_slave_axi_AR,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_ARVALID,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_ARREADY,
      `EVL_IFC_PORT wire sc_slave_axi_AW_channel_t  ioshire_slave_axi_AW,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_AWVALID,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_AWREADY,
      `EVL_IFC_PORT wire sc_slave_axi_W_channel_t   ioshire_slave_axi_W,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_WVALID,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_WREADY,
      `EVL_IFC_PORT wire sc_slave_axi_B_channel_t   ioshire_slave_axi_B,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_BVALID,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_BREADY,
      `EVL_IFC_PORT wire sc_slave_axi_R_channel_t   ioshire_slave_axi_R,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_RVALID,
      `EVL_IFC_PORT wire                            ioshire_slave_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t ioshire_esr_master_axi_AR,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_ARVALID,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t ioshire_esr_master_axi_AW,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_AWVALID,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  ioshire_esr_master_axi_W,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_WVALID,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  ioshire_esr_master_axi_B,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_BVALID,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  ioshire_esr_master_axi_R,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_RVALID,
      `EVL_IFC_PORT wire                            ioshire_esr_master_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t ioshire_tosys_master_axi_AR,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_ARVALID,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t ioshire_tosys_master_axi_AW,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_AWVALID,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  ioshire_tosys_master_axi_W,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_WVALID,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  ioshire_tosys_master_axi_B,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_BVALID,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  ioshire_tosys_master_axi_R,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_RVALID,
      `EVL_IFC_PORT wire                            ioshire_tosys_master_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t ioshire_tol3_master_axi_AR,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_ARVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t ioshire_tol3_master_axi_AW,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_AWVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  ioshire_tol3_master_axi_W,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_WVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  ioshire_tol3_master_axi_B,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_BVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  ioshire_tol3_master_axi_R,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_RVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3_master_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t ioshire_tol3b_master_axi_AR,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_ARVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t ioshire_tol3b_master_axi_AW,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_AWVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  ioshire_tol3b_master_axi_W,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_WVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  ioshire_tol3b_master_axi_B,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_BVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  ioshire_tol3b_master_axi_R,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_RVALID,
      `EVL_IFC_PORT wire                            ioshire_tol3b_master_axi_RREADY,

      // Maxion AXI interfaces
      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t maxion_uc_to_l3_axi_AR,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_ARVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t maxion_uc_to_l3_axi_AW,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_AWVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  maxion_uc_to_l3_axi_W,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_WVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  maxion_uc_to_l3_axi_B,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_BVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  maxion_uc_to_l3_axi_R,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_RVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_l3_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t maxion_to_sys_axi_AR,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_ARVALID,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t maxion_to_sys_axi_AW,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_AWVALID,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  maxion_to_sys_axi_W,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_WVALID,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  maxion_to_sys_axi_B,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_BVALID,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  maxion_to_sys_axi_R,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_RVALID,
      `EVL_IFC_PORT wire                            maxion_to_sys_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t maxion_uc_to_sys_axi_AR,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_ARVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t maxion_uc_to_sys_axi_AW,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_AWVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  maxion_uc_to_sys_axi_W,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_WVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  maxion_uc_to_sys_axi_B,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_BVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  maxion_uc_to_sys_axi_R,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_RVALID,
      `EVL_IFC_PORT wire                            maxion_uc_to_sys_axi_RREADY,

      `EVL_IFC_PORT wire sc_slave_axi_AR_channel_t  [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_AR,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_ARVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_ARREADY,
      `EVL_IFC_PORT wire sc_slave_axi_AW_channel_t  [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_AW,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_AWVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_AWREADY,
      `EVL_IFC_PORT wire sc_slave_axi_W_channel_t   [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_W,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_WVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_WREADY,
      `EVL_IFC_PORT wire sc_slave_axi_B_channel_t   [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_B,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_BVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_BREADY,
      `EVL_IFC_PORT wire sc_slave_axi_R_channel_t   [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_R,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_RVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]   maxion_l3_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_AR,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_ARVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_AW,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_AWVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_W,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_WVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_B,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_BVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_R,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_RVALID,
      `EVL_IFC_PORT wire                            [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]  maxion_to_l3_axi_RREADY,

      `EVL_IFC_PORT wire sys_slave_axi_AR_channel_t maxion_sys_axi_AR,
      `EVL_IFC_PORT wire                            maxion_sys_axi_ARVALID,
      `EVL_IFC_PORT wire                            maxion_sys_axi_ARREADY,
      `EVL_IFC_PORT wire sys_slave_axi_AW_channel_t maxion_sys_axi_AW,
      `EVL_IFC_PORT wire                            maxion_sys_axi_AWVALID,
      `EVL_IFC_PORT wire [1:0]                      maxion_sys_axi_AWvcvalid,
      `EVL_IFC_PORT wire [1:0]                      maxion_sys_axi_AWcredit,
      `EVL_IFC_PORT wire                            maxion_sys_axi_AWREADY,
      `EVL_IFC_PORT wire sys_slave_axi_W_channel_t  maxion_sys_axi_W,
      `EVL_IFC_PORT wire                            maxion_sys_axi_WVALID,
      `EVL_IFC_PORT wire [1:0]                      maxion_sys_axi_Wvcvalid,
      `EVL_IFC_PORT wire                            maxion_sys_axi_WREADY,
      `EVL_IFC_PORT wire sys_slave_axi_B_channel_t  maxion_sys_axi_B,
      `EVL_IFC_PORT wire                            maxion_sys_axi_BVALID,
      `EVL_IFC_PORT wire                            maxion_sys_axi_BREADY,
      `EVL_IFC_PORT wire sys_slave_axi_R_channel_t  maxion_sys_axi_R,
      `EVL_IFC_PORT wire                            maxion_sys_axi_RVALID,
      `EVL_IFC_PORT wire                            maxion_sys_axi_RREADY,

      // PU_NOC AXI Interfaces
      `EVL_IFC_PORT wire [ 39:0]       pu_noc_pu_scram_axi_ARADDR,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_ARID,
      `EVL_IFC_PORT wire [  7:0]       pu_noc_pu_scram_axi_ARLEN,
      `EVL_IFC_PORT wire [  1:0]       pu_noc_pu_scram_axi_ARBURST,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_ARCACHE,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_ARLOCK,
      `EVL_IFC_PORT wire [  2:0]       pu_noc_pu_scram_axi_ARPROT,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_ARQOS,
      `EVL_IFC_PORT wire [  2:0]       pu_noc_pu_scram_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_ARVALID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_ARREADY,
      `EVL_IFC_PORT wire [ 39:0]       pu_noc_pu_scram_axi_AWADDR,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_AWID,
      `EVL_IFC_PORT wire [  7:0]       pu_noc_pu_scram_axi_AWLEN,
      `EVL_IFC_PORT wire [  1:0]       pu_noc_pu_scram_axi_AWBURST,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_AWCACHE,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_AWLOCK,
      `EVL_IFC_PORT wire [  2:0]       pu_noc_pu_scram_axi_AWPROT,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_AWQOS,
      `EVL_IFC_PORT wire [  2:0]       pu_noc_pu_scram_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_AWREADY,
      `EVL_IFC_PORT wire [511:0]       pu_noc_pu_scram_axi_WDATA,
      `EVL_IFC_PORT wire [ 63:0]       pu_noc_pu_scram_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_WLAST,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_WVALID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_WREADY,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_BID,
      `EVL_IFC_PORT wire [  1:0]       pu_noc_pu_scram_axi_BRESP,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_BVALID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_BREADY,
      `EVL_IFC_PORT wire [511:0]       pu_noc_pu_scram_axi_RDATA,
      `EVL_IFC_PORT wire [  3:0]       pu_noc_pu_scram_axi_RID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_RLAST,
      `EVL_IFC_PORT wire [  1:0]       pu_noc_pu_scram_axi_RRESP,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_RVALID,
      `EVL_IFC_PORT wire               pu_noc_pu_scram_axi_RREADY,

      `EVL_IFC_PORT wire [39:0]        pu_dma_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_dma_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [2:0]         pu_dma_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        pu_dma_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_dma_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [2:0]         pu_dma_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        pu_dma_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_WID,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        pu_dma_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         pu_dma_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_dma_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_dma_pu_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        pu_dma_am_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_dma_am_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_dma_am_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [4:0]         pu_dma_am_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [7:0]         pu_dma_am_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_dma_am_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [3:0]         pu_dma_am_pu_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [2:0]         pu_dma_am_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        pu_dma_am_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_dma_am_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_dma_am_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [4:0]         pu_dma_am_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [7:0]         pu_dma_am_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_dma_am_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [3:0]         pu_dma_am_pu_noc_axi_AWQOS,
      `EVL_IFC_PORT wire [2:0]         pu_dma_am_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [63:0]        pu_dma_am_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [7:0]         pu_dma_am_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [4:0]         pu_dma_am_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_dma_am_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [63:0]        pu_dma_am_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [4:0]         pu_dma_am_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_dma_am_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_dma_am_pu_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        pu_emmc_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_emmc_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [2:0]         pu_emmc_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        pu_emmc_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_emmc_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [2:0]         pu_emmc_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [63:0]        pu_emmc_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_WID,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [7:0]         pu_emmc_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [63:0]        pu_emmc_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         pu_emmc_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_emmc_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_emmc_pu_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        pu_usb0_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_usb0_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [2:0]         pu_usb0_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        pu_usb0_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_usb0_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [2:0]         pu_usb0_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        pu_usb0_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_WID,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        pu_usb0_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         pu_usb0_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_usb0_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_usb0_pu_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        pu_usb1_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_usb1_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [2:0]         pu_usb1_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        pu_usb1_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_usb1_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [2:0]         pu_usb1_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        pu_usb1_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_WID,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        pu_usb1_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         pu_usb1_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_usb1_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_usb1_pu_noc_axi_WREADY,

      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         pu_static_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        pu_static_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         pu_static_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_WREADY,
      `EVL_IFC_PORT wire [31:0]        pu_static_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         pu_static_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [7:0]         pu_static_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_static_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [2:0]         pu_static_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [31:0]        pu_static_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         pu_static_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [7:0]         pu_static_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         pu_static_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_AWQOS,
      `EVL_IFC_PORT wire [2:0]         pu_static_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        pu_static_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         pu_static_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               pu_static_pu_noc_axi_WVALID,

      // SPIO_NoC -> PU_NOC Interface
      `EVL_IFC_PORT wire [ 39:0]       spio_noc_pu_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_ARID,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_pu_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_pu_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_pu_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_pu_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_pu_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_ARVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_ARREADY,
      `EVL_IFC_PORT wire [ 39:0]       spio_noc_pu_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_AWID,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_pu_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_pu_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_pu_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_pu_noc_axi_AWQOS,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_pu_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [  4:0]       spio_noc_pu_noc_axi_AWUSER,
      `EVL_IFC_PORT wire [511:0]       spio_noc_pu_noc_axi_WDATA,
      `EVL_IFC_PORT wire [ 63:0]       spio_noc_pu_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_WLAST,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_WVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_WREADY,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_BID,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_pu_noc_axi_BRESP,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_BVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_BREADY,
      `EVL_IFC_PORT wire [511:0]       spio_noc_pu_noc_axi_RDATA,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_pu_noc_axi_RID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_RLAST,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_pu_noc_axi_RRESP,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_RVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_axi_RREADY,

      // SPIO_NOC Interfaces
      `EVL_IFC_PORT wire [ 39:0]       spio_noc_spio_rom_axi_ARADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_ARID,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_ARQOS,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_spio_rom_axi_ARLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_rom_axi_ARBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_ARCACHE,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_ARLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_rom_axi_ARPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_rom_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_ARVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_ARREADY,
      `EVL_IFC_PORT wire [ 39:0]       spio_noc_spio_rom_axi_AWADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_AWID,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_AWQOS,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_spio_rom_axi_AWLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_rom_axi_AWBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_AWCACHE,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_AWLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_rom_axi_AWPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_rom_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_AWREADY,
      `EVL_IFC_PORT wire [511:0]       spio_noc_spio_rom_axi_WDATA,
      `EVL_IFC_PORT wire [ 63:0]       spio_noc_spio_rom_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_WLAST,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_WVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_WREADY,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_BID,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_rom_axi_BRESP,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_BVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_BREADY,
      `EVL_IFC_PORT wire [511:0]       spio_noc_spio_rom_axi_RDATA,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_rom_axi_RID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_RLAST,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_rom_axi_RRESP,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_RVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_rom_axi_RREADY,

      `EVL_IFC_PORT wire [ 39:0]       spio_noc_spio_scram_axi_ARADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_ARID,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_ARQOS,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_spio_scram_axi_ARLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_scram_axi_ARBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_ARCACHE,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_ARLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_scram_axi_ARPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_scram_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_ARVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_ARREADY,
      `EVL_IFC_PORT wire [ 39:0]       spio_noc_spio_scram_axi_AWADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_AWID,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_AWQOS,
      `EVL_IFC_PORT wire [  7:0]       spio_noc_spio_scram_axi_AWLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_scram_axi_AWBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_AWCACHE,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_AWLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_scram_axi_AWPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_noc_spio_scram_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_AWREADY,
      `EVL_IFC_PORT wire [511:0]       spio_noc_spio_scram_axi_WDATA,
      `EVL_IFC_PORT wire [ 63:0]       spio_noc_spio_scram_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_WLAST,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_WVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_WREADY,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_BID,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_scram_axi_BRESP,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_BVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_BREADY,
      `EVL_IFC_PORT wire [511:0]       spio_noc_spio_scram_axi_RDATA,
      `EVL_IFC_PORT wire [  3:0]       spio_noc_spio_scram_axi_RID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_RLAST,
      `EVL_IFC_PORT wire [  1:0]       spio_noc_spio_scram_axi_RRESP,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_RVALID,
      `EVL_IFC_PORT wire               spio_noc_spio_scram_axi_RREADY,

      `EVL_IFC_PORT wire [ 39:0]       spio_sp_spio_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_ARID,
      `EVL_IFC_PORT wire [  7:0]       spio_sp_spio_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_spio_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_spio_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_spio_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_ARVALID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_ARREADY,
      `EVL_IFC_PORT wire [ 39:0]       spio_sp_spio_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_AWID,
      `EVL_IFC_PORT wire [  7:0]       spio_sp_spio_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_spio_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_AWQOS,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_spio_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_spio_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [  4:0]       spio_sp_spio_noc_axi_AWUSER,
      `EVL_IFC_PORT wire [511:0]       spio_sp_spio_noc_axi_WDATA,
      `EVL_IFC_PORT wire [ 63:0]       spio_sp_spio_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_WLAST,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_WVALID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_WREADY,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_BID,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_spio_noc_axi_BRESP,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_BVALID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_BREADY,
      `EVL_IFC_PORT wire [511:0]       spio_sp_spio_noc_axi_RDATA,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_spio_noc_axi_RID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_RLAST,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_spio_noc_axi_RRESP,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_RVALID,
      `EVL_IFC_PORT wire               spio_sp_spio_noc_axi_RREADY,

      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_ARREADY,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_BID,
      `EVL_IFC_PORT wire [1:0]         spio_noc_dbg_comm_axi_BRESP,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        spio_noc_dbg_comm_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_RID,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         spio_noc_dbg_comm_axi_RRESP,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_RVALID,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_WREADY,
      `EVL_IFC_PORT wire [39:0]        spio_noc_dbg_comm_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         spio_noc_dbg_comm_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_ARID,
      `EVL_IFC_PORT wire [7:0]         spio_noc_dbg_comm_axi_ARLEN,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_noc_dbg_comm_axi_ARPROT,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_ARQOS,
      `EVL_IFC_PORT wire [2:0]         spio_noc_dbg_comm_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_noc_dbg_comm_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         spio_noc_dbg_comm_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_AWID,
      `EVL_IFC_PORT wire [7:0]         spio_noc_dbg_comm_axi_AWLEN,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_noc_dbg_comm_axi_AWPROT,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_AWQOS,
      `EVL_IFC_PORT wire [2:0]         spio_noc_dbg_comm_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_BREADY,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        spio_noc_dbg_comm_axi_WDATA,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         spio_noc_dbg_comm_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_noc_dbg_comm_axi_WVALID,

      `EVL_IFC_PORT wire [39:0]        spio_am_spio_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         spio_am_spio_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_ARID,
      `EVL_IFC_PORT wire [7:0]         spio_am_spio_noc_axi_ARLEN,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_am_spio_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [2:0]         spio_am_spio_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_am_spio_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         spio_am_spio_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_AWID,
      `EVL_IFC_PORT wire [7:0]         spio_am_spio_noc_axi_AWLEN,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_am_spio_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_AWQOS,
      `EVL_IFC_PORT wire [2:0]         spio_am_spio_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_BREADY,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_RREADY,
      `EVL_IFC_PORT wire [63:0]        spio_am_spio_noc_axi_WDATA,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_WLAST,
      `EVL_IFC_PORT wire [7:0]         spio_am_spio_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_WVALID,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         spio_am_spio_noc_axi_BRESP,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_BVALID,
      `EVL_IFC_PORT wire [63:0]        spio_am_spio_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         spio_am_spio_noc_axi_RID,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         spio_am_spio_noc_axi_RRESP,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_RVALID,
      `EVL_IFC_PORT wire               spio_am_spio_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        spio_dma_spio_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_ARID,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_ARLEN,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_dma_spio_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [2:0]         spio_dma_spio_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_dma_spio_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_AWID,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_AWLEN,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_dma_spio_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [2:0]         spio_dma_spio_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_BREADY,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        spio_dma_spio_noc_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_WID,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_WLAST,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_WVALID,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_BRESP,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        spio_dma_spio_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         spio_dma_spio_noc_axi_RID,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         spio_dma_spio_noc_axi_RRESP,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_RVALID,
      `EVL_IFC_PORT wire               spio_dma_spio_noc_axi_WREADY,

      `EVL_IFC_PORT wire [39:0]        spio_vault_spio_noc_axi_ARADDR,
      `EVL_IFC_PORT wire [1:0]         spio_vault_spio_noc_axi_ARBURST,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_ARCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_ARID,
      `EVL_IFC_PORT wire [7:0]         spio_vault_spio_noc_axi_ARLEN,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_vault_spio_noc_axi_ARPROT,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_ARQOS,
      `EVL_IFC_PORT wire [2:0]         spio_vault_spio_noc_axi_ARSIZE,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_vault_spio_noc_axi_AWADDR,
      `EVL_IFC_PORT wire [1:0]         spio_vault_spio_noc_axi_AWBURST,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_AWCACHE,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_AWID,
      `EVL_IFC_PORT wire [7:0]         spio_vault_spio_noc_axi_AWLEN,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_vault_spio_noc_axi_AWPROT,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_AWQOS,
      `EVL_IFC_PORT wire [2:0]         spio_vault_spio_noc_axi_AWSIZE,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_BREADY,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_RREADY,
      `EVL_IFC_PORT wire [63:0]        spio_vault_spio_noc_axi_WDATA,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_WLAST,
      `EVL_IFC_PORT wire [7:0]         spio_vault_spio_noc_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_WVALID,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_ARREADY,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_AWREADY,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_BID,
      `EVL_IFC_PORT wire [1:0]         spio_vault_spio_noc_axi_BRESP,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_BVALID,
      `EVL_IFC_PORT wire [63:0]        spio_vault_spio_noc_axi_RDATA,
      `EVL_IFC_PORT wire [3:0]         spio_vault_spio_noc_axi_RID,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_RLAST,
      `EVL_IFC_PORT wire [1:0]         spio_vault_spio_noc_axi_RRESP,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_RVALID,
      `EVL_IFC_PORT wire               spio_vault_spio_noc_axi_WREADY,

      `EVL_IFC_PORT wire               spio_p10_pcie_s_ARREADY,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_AWREADY,
      `EVL_IFC_PORT wire [5:0]         spio_p10_pcie_s_BID,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_s_BRESP,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_BVALID,
      `EVL_IFC_PORT wire [31:0]        spio_p10_pcie_s_RDATA,
      `EVL_IFC_PORT wire [5:0]         spio_p10_pcie_s_RID,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_RLAST,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_s_RRESP,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_RVALID,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_WREADY,
      `EVL_IFC_PORT wire [39:0]        spio_p10_pcie_s_ARADDR,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_s_ARBURST,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_s_ARCACHE,
      `EVL_IFC_PORT wire [5:0]         spio_p10_pcie_s_ARID,
      `EVL_IFC_PORT wire [7:0]         spio_p10_pcie_s_ARLEN,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_ARLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_s_ARPROT,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_s_ARQOS,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_s_ARSIZE,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_p10_pcie_s_AWADDR,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_s_AWBURST,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_s_AWCACHE,
      `EVL_IFC_PORT wire [5:0]         spio_p10_pcie_s_AWID,
      `EVL_IFC_PORT wire [7:0]         spio_p10_pcie_s_AWLEN,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_AWLOCK,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_s_AWPROT,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_s_AWQOS,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_s_AWSIZE,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_AWVALID,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_BREADY,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_RREADY,
      `EVL_IFC_PORT wire [31:0]        spio_p10_pcie_s_WDATA,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_WLAST,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_s_WSTRB,
      `EVL_IFC_PORT wire               spio_p10_pcie_s_WVALID,

      // Regbus Interfaces
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_AWVALID,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_AWREADY,
      `EVL_IFC_PORT wire [ 39:  0]     spio_sp_main0_main_noc_rbm_s_AWADDR,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_main0_main_noc_rbm_s_AWPROT,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_WVALID,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_WREADY,
      `EVL_IFC_PORT wire [ 31:0]       spio_sp_main0_main_noc_rbm_s_WDATA,
      `EVL_IFC_PORT wire [  3:0]       spio_sp_main0_main_noc_rbm_s_WSTRB,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_BVALID,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_BREADY,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_main0_main_noc_rbm_s_BRESP,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_ARVALID,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_ARREADY,
      `EVL_IFC_PORT wire [ 39:  0]     spio_sp_main0_main_noc_rbm_s_ARADDR,
      `EVL_IFC_PORT wire [  2:0]       spio_sp_main0_main_noc_rbm_s_ARPROT,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_RVALID,
      `EVL_IFC_PORT wire               spio_sp_main0_main_noc_rbm_s_RREADY,
      `EVL_IFC_PORT wire [ 31:0]       spio_sp_main0_main_noc_rbm_s_RDATA,
      `EVL_IFC_PORT wire [  1:0]       spio_sp_main0_main_noc_rbm_s_RRESP,

      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_ARREADY,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_AWREADY,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_rbm_s_BRESP,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_BVALID,
      `EVL_IFC_PORT wire [31:0]        spio_p10_pcie_rbm_s_RDATA,
      `EVL_IFC_PORT wire [1:0]         spio_p10_pcie_rbm_s_RRESP,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_RVALID,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_WREADY,
      `EVL_IFC_PORT wire [39:0]        spio_p10_pcie_rbm_s_ARADDR,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_rbm_s_ARPROT,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_p10_pcie_rbm_s_AWADDR,
      `EVL_IFC_PORT wire [2:0]         spio_p10_pcie_rbm_s_AWPROT,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_AWVALID,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_BREADY,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_RREADY,
      `EVL_IFC_PORT wire [31:0]        spio_p10_pcie_rbm_s_WDATA,
      `EVL_IFC_PORT wire [3:0]         spio_p10_pcie_rbm_s_WSTRB,
      `EVL_IFC_PORT wire               spio_p10_pcie_rbm_s_WVALID,

      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_ARREADY,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_AWREADY,
      `EVL_IFC_PORT wire [1:0]         spio_noc_pu_noc_regbus_axi_BRESP,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_BVALID,
      `EVL_IFC_PORT wire [31:0]        spio_noc_pu_noc_regbus_axi_RDATA,
      `EVL_IFC_PORT wire [1:0]         spio_noc_pu_noc_regbus_axi_RRESP,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_RVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_WREADY,
      `EVL_IFC_PORT wire [39:0]        spio_noc_pu_noc_regbus_axi_ARADDR,
      `EVL_IFC_PORT wire [2:0]         spio_noc_pu_noc_regbus_axi_ARPROT,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_ARVALID,
      `EVL_IFC_PORT wire [39:0]        spio_noc_pu_noc_regbus_axi_AWADDR,
      `EVL_IFC_PORT wire [2:0]         spio_noc_pu_noc_regbus_axi_AWPROT,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_AWVALID,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_BREADY,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_RREADY,
      `EVL_IFC_PORT wire [31:0]        spio_noc_pu_noc_regbus_axi_WDATA,
      `EVL_IFC_PORT wire [3:0]         spio_noc_pu_noc_regbus_axi_WSTRB,
      `EVL_IFC_PORT wire               spio_noc_pu_noc_regbus_axi_WVALID,

      `EVL_IFC_PORT wire [127:0]       host_beat_data_io0_lf_msg,
      `EVL_IFC_PORT wire               host_beat_eop_io0_lf_msg,
      `EVL_IFC_PORT wire               host_beat_sop_io0_lf_msg,
      `EVL_IFC_PORT wire               host_beat_valid_io0_lf_msg,
      `EVL_IFC_PORT wire               host_credit_inc_io0_lf_msg,
      `EVL_IFC_PORT wire   [7:0]       host_dest_hp_io0_lf_msg,
      `EVL_IFC_PORT wire   [1:0]       host_dest_int_io0_lf_msg,
      `EVL_IFC_PORT wire   [3:0]       host_xfer_qos_io0_lf_msg,
      `EVL_IFC_PORT wire [127:0]       noc_beat_data_io0_lf_msg,
      `EVL_IFC_PORT wire               noc_beat_eop_io0_lf_msg,
      `EVL_IFC_PORT wire               noc_beat_sop_io0_lf_msg,
      `EVL_IFC_PORT wire               noc_beat_valid_io0_lf_msg,
      `EVL_IFC_PORT wire               noc_credit_inc_io0_lf_msg,

      `EVL_IFC_PORT wire  [31:0]       host_beat_data_io0_lf_evt,
      `EVL_IFC_PORT wire               host_beat_eop_io0_lf_evt,
      `EVL_IFC_PORT wire               host_beat_sop_io0_lf_evt,
      `EVL_IFC_PORT wire               host_beat_valid_io0_lf_evt,
      `EVL_IFC_PORT wire               host_credit_inc_io0_lf_evt,
      `EVL_IFC_PORT wire   [7:0]       host_dest_hp_io0_lf_evt,
      `EVL_IFC_PORT wire   [1:0]       host_dest_int_io0_lf_evt,
      `EVL_IFC_PORT wire   [3:0]       host_xfer_qos_io0_lf_evt,
      `EVL_IFC_PORT wire  [31:0]       noc_beat_data_io0_lf_evt,
      `EVL_IFC_PORT wire               noc_beat_eop_io0_lf_evt,
      `EVL_IFC_PORT wire               noc_beat_sop_io0_lf_evt,
      `EVL_IFC_PORT wire               noc_beat_valid_io0_lf_evt,
      `EVL_IFC_PORT wire               noc_credit_inc_io0_lf_evt,

      `EVL_IFC_PORT wire [127:0]       host_beat_data_io0_rt_msg,
      `EVL_IFC_PORT wire               host_beat_eop_io0_rt_msg,
      `EVL_IFC_PORT wire               host_beat_sop_io0_rt_msg,
      `EVL_IFC_PORT wire               host_beat_valid_io0_rt_msg,
      `EVL_IFC_PORT wire               host_credit_inc_io0_rt_msg,
      `EVL_IFC_PORT wire   [7:0]       host_dest_hp_io0_rt_msg,
      `EVL_IFC_PORT wire   [1:0]       host_dest_int_io0_rt_msg,
      `EVL_IFC_PORT wire   [3:0]       host_xfer_qos_io0_rt_msg,
      `EVL_IFC_PORT wire [127:0]       noc_beat_data_io0_rt_msg,
      `EVL_IFC_PORT wire               noc_beat_eop_io0_rt_msg,
      `EVL_IFC_PORT wire               noc_beat_sop_io0_rt_msg,
      `EVL_IFC_PORT wire               noc_beat_valid_io0_rt_msg,
      `EVL_IFC_PORT wire               noc_credit_inc_io0_rt_msg,

      `EVL_IFC_PORT wire  [31:0]       host_beat_data_io0_rt_evt,
      `EVL_IFC_PORT wire               host_beat_eop_io0_rt_evt,
      `EVL_IFC_PORT wire               host_beat_sop_io0_rt_evt,
      `EVL_IFC_PORT wire               host_beat_valid_io0_rt_evt,
      `EVL_IFC_PORT wire               host_credit_inc_io0_rt_evt,
      `EVL_IFC_PORT wire   [7:0]       host_dest_hp_io0_rt_evt,
      `EVL_IFC_PORT wire   [1:0]       host_dest_int_io0_rt_evt,
      `EVL_IFC_PORT wire   [3:0]       host_xfer_qos_io0_rt_evt,
      `EVL_IFC_PORT wire  [31:0]       noc_beat_data_io0_rt_evt,
      `EVL_IFC_PORT wire               noc_beat_eop_io0_rt_evt,
      `EVL_IFC_PORT wire               noc_beat_sop_io0_rt_evt,
      `EVL_IFC_PORT wire               noc_beat_valid_io0_rt_evt,
      `EVL_IFC_PORT wire               noc_credit_inc_io0_rt_evt

   );


   //-----------------------------------------------------------------------------------------------
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_ioshire_pkg::*;

   wire [127:0]       host_beat_data_io0_lf_msg_int;
   wire               host_beat_eop_io0_lf_msg_int;
   wire               host_beat_sop_io0_lf_msg_int;
   wire               host_beat_valid_io0_lf_msg_int;
   wire               host_credit_inc_io0_lf_msg_int;
   wire   [7:0]       host_dest_hp_io0_lf_msg_int;
   wire   [1:0]       host_dest_int_io0_lf_msg_int;
   wire   [3:0]       host_xfer_qos_io0_lf_msg_int;
   wire [127:0]       noc_beat_data_io0_lf_msg_int;
   wire               noc_beat_eop_io0_lf_msg_int;
   wire               noc_beat_sop_io0_lf_msg_int;
   wire               noc_beat_valid_io0_lf_msg_int;
   wire               noc_credit_inc_io0_lf_msg_int;

   wire  [31:0]       host_beat_data_io0_lf_evt_int;
   wire               host_beat_eop_io0_lf_evt_int;
   wire               host_beat_sop_io0_lf_evt_int;
   wire               host_beat_valid_io0_lf_evt_int;
   wire               host_credit_inc_io0_lf_evt_int;
   wire   [7:0]       host_dest_hp_io0_lf_evt_int;
   wire   [1:0]       host_dest_int_io0_lf_evt_int;
   wire   [3:0]       host_xfer_qos_io0_lf_evt_int;
   wire  [31:0]       noc_beat_data_io0_lf_evt_int;
   wire               noc_beat_eop_io0_lf_evt_int;
   wire               noc_beat_sop_io0_lf_evt_int;
   wire               noc_beat_valid_io0_lf_evt_int;
   wire               noc_credit_inc_io0_lf_evt_int;

   wire [127:0]       host_beat_data_io0_rt_msg_int;
   wire               host_beat_eop_io0_rt_msg_int;
   wire               host_beat_sop_io0_rt_msg_int;
   wire               host_beat_valid_io0_rt_msg_int;
   wire               host_credit_inc_io0_rt_msg_int;
   wire   [7:0]       host_dest_hp_io0_rt_msg_int;
   wire   [1:0]       host_dest_int_io0_rt_msg_int;
   wire   [3:0]       host_xfer_qos_io0_rt_msg_int;
   wire [127:0]       noc_beat_data_io0_rt_msg_int;
   wire               noc_beat_eop_io0_rt_msg_int;
   wire               noc_beat_sop_io0_rt_msg_int;
   wire               noc_beat_valid_io0_rt_msg_int;
   wire               noc_credit_inc_io0_rt_msg_int;

   wire  [31:0]       host_beat_data_io0_rt_evt_int;
   wire               host_beat_eop_io0_rt_evt_int;
   wire               host_beat_sop_io0_rt_evt_int;
   wire               host_beat_valid_io0_rt_evt_int;
   wire               host_credit_inc_io0_rt_evt_int;
   wire   [7:0]       host_dest_hp_io0_rt_evt_int;
   wire   [1:0]       host_dest_int_io0_rt_evt_int;
   wire   [3:0]       host_xfer_qos_io0_rt_evt_int;
   wire  [31:0]       noc_beat_data_io0_rt_evt_int;
   wire               noc_beat_eop_io0_rt_evt_int;
   wire               noc_beat_sop_io0_rt_evt_int;
   wire               noc_beat_valid_io0_rt_evt_int;
   wire               noc_credit_inc_io0_rt_evt_int;


   `ifdef DV_IOSHIRE_DBGNOC_STIM
      assign host_dest_hp_io0_lf_msg         = host_dest_hp_io0_lf_msg_int;
      assign host_dest_int_io0_lf_msg        = host_dest_int_io0_lf_msg_int;
      assign host_xfer_qos_io0_lf_msg        = host_xfer_qos_io0_lf_msg_int;
      assign host_beat_valid_io0_lf_msg      = host_beat_valid_io0_lf_msg_int;
      assign host_beat_data_io0_lf_msg       = host_beat_data_io0_lf_msg_int;
      assign host_beat_sop_io0_lf_msg        = host_beat_sop_io0_lf_msg_int;
      assign host_beat_eop_io0_lf_msg        = host_beat_eop_io0_lf_msg_int;

      assign noc_beat_data_io0_lf_msg_int    = noc_beat_data_io0_lf_msg;
      assign noc_beat_valid_io0_lf_msg_int   = noc_beat_valid_io0_lf_msg;
      assign noc_beat_sop_io0_lf_msg_int     = noc_beat_sop_io0_lf_msg;
      assign noc_beat_eop_io0_lf_msg_int     = noc_beat_eop_io0_lf_msg;

      assign host_credit_inc_io0_lf_msg      = host_credit_inc_io0_lf_msg_int;
      assign noc_credit_inc_io0_lf_msg_int   = noc_credit_inc_io0_lf_msg;

      assign host_dest_hp_io0_lf_evt         = host_dest_hp_io0_lf_evt_int;
      assign host_dest_int_io0_lf_evt        = host_dest_int_io0_lf_evt_int;
      assign host_xfer_qos_io0_lf_evt        = host_xfer_qos_io0_lf_evt_int;
      assign host_beat_valid_io0_lf_evt      = host_beat_valid_io0_lf_evt_int;
      assign host_beat_data_io0_lf_evt       = host_beat_data_io0_lf_evt_int;
      assign host_beat_sop_io0_lf_evt        = host_beat_sop_io0_lf_evt_int;
      assign host_beat_eop_io0_lf_evt        = host_beat_eop_io0_lf_evt_int;

      assign noc_beat_data_io0_lf_evt_int    = noc_beat_data_io0_lf_evt;
      assign noc_beat_valid_io0_lf_evt_int   = noc_beat_valid_io0_lf_evt;
      assign noc_beat_sop_io0_lf_evt_int     = noc_beat_sop_io0_lf_evt;
      assign noc_beat_eop_io0_lf_evt_int     = noc_beat_eop_io0_lf_evt;

      assign host_credit_inc_io0_lf_evt      = host_credit_inc_io0_lf_evt_int;
      assign noc_credit_inc_io0_lf_evt_int   = noc_credit_inc_io0_lf_evt;

      assign host_dest_hp_io0_rt_msg         = host_dest_hp_io0_rt_msg_int;
      assign host_dest_int_io0_rt_msg        = host_dest_int_io0_rt_msg_int;
      assign host_xfer_qos_io0_rt_msg        = host_xfer_qos_io0_rt_msg_int;
      assign host_beat_valid_io0_rt_msg      = host_beat_valid_io0_rt_msg_int;
      assign host_beat_data_io0_rt_msg       = host_beat_data_io0_rt_msg_int;
      assign host_beat_sop_io0_rt_msg        = host_beat_sop_io0_rt_msg_int;
      assign host_beat_eop_io0_rt_msg        = host_beat_eop_io0_rt_msg_int;

      assign noc_beat_data_io0_rt_msg_int    = noc_beat_data_io0_rt_msg;
      assign noc_beat_valid_io0_rt_msg_int   = noc_beat_valid_io0_rt_msg;
      assign noc_beat_sop_io0_rt_msg_int     = noc_beat_sop_io0_rt_msg;
      assign noc_beat_eop_io0_rt_msg_int     = noc_beat_eop_io0_rt_msg;

      assign host_credit_inc_io0_rt_msg      = host_credit_inc_io0_rt_msg_int;
      assign noc_credit_inc_io0_rt_msg_int   = noc_credit_inc_io0_rt_msg;

      assign host_dest_hp_io0_rt_evt         = host_dest_hp_io0_rt_evt_int;
      assign host_dest_int_io0_rt_evt        = host_dest_int_io0_rt_evt_int;
      assign host_xfer_qos_io0_rt_evt        = host_xfer_qos_io0_rt_evt_int;
      assign host_beat_valid_io0_rt_evt      = host_beat_valid_io0_rt_evt_int;
      assign host_beat_data_io0_rt_evt       = host_beat_data_io0_rt_evt_int;
      assign host_beat_sop_io0_rt_evt        = host_beat_sop_io0_rt_evt_int;
      assign host_beat_eop_io0_rt_evt        = host_beat_eop_io0_rt_evt_int;

      assign noc_beat_data_io0_rt_evt_int    = noc_beat_data_io0_rt_evt;
      assign noc_beat_valid_io0_rt_evt_int   = noc_beat_valid_io0_rt_evt;
      assign noc_beat_sop_io0_rt_evt_int     = noc_beat_sop_io0_rt_evt;
      assign noc_beat_eop_io0_rt_evt_int     = noc_beat_eop_io0_rt_evt;

      assign host_credit_inc_io0_rt_evt      = host_credit_inc_io0_rt_evt_int;
      assign noc_credit_inc_io0_rt_evt_int   = noc_credit_inc_io0_rt_evt;

   `else
      assign host_dest_hp_io0_lf_msg_int     = host_dest_hp_io0_lf_msg;
      assign host_dest_int_io0_lf_msg_int    = host_dest_int_io0_lf_msg;
      assign host_xfer_qos_io0_lf_msg_int    = host_xfer_qos_io0_lf_msg;
      assign host_beat_valid_io0_lf_msg_int  = host_beat_valid_io0_lf_msg;
      assign host_beat_data_io0_lf_msg_int   = host_beat_data_io0_lf_msg;
      assign host_beat_sop_io0_lf_msg_int    = host_beat_sop_io0_lf_msg;
      assign host_beat_eop_io0_lf_msg_int    = host_beat_eop_io0_lf_msg;

      assign noc_beat_data_io0_lf_msg_int    = noc_beat_data_io0_lf_msg;
      assign noc_beat_valid_io0_lf_msg_int   = noc_beat_valid_io0_lf_msg;
      assign noc_beat_sop_io0_lf_msg_int     = noc_beat_sop_io0_lf_msg;
      assign noc_beat_eop_io0_lf_msg_int     = noc_beat_eop_io0_lf_msg;

      assign host_credit_inc_io0_lf_msg_int  = host_credit_inc_io0_lf_msg;
      assign noc_credit_inc_io0_lf_msg_int   = noc_credit_inc_io0_lf_msg;

      assign host_dest_hp_io0_lf_evt_int     = host_dest_hp_io0_lf_evt;
      assign host_dest_int_io0_lf_evt_int    = host_dest_int_io0_lf_evt;
      assign host_xfer_qos_io0_lf_evt_int    = host_xfer_qos_io0_lf_evt;
      assign host_beat_valid_io0_lf_evt_int  = host_beat_valid_io0_lf_evt;
      assign host_beat_data_io0_lf_evt_int   = host_beat_data_io0_lf_evt;
      assign host_beat_sop_io0_lf_evt_int    = host_beat_sop_io0_lf_evt;
      assign host_beat_eop_io0_lf_evt_int    = host_beat_eop_io0_lf_evt;

      assign noc_beat_data_io0_lf_evt_int    = noc_beat_data_io0_lf_evt;
      assign noc_beat_valid_io0_lf_evt_int   = noc_beat_valid_io0_lf_evt;
      assign noc_beat_sop_io0_lf_evt_int     = noc_beat_sop_io0_lf_evt;
      assign noc_beat_eop_io0_lf_evt_int     = noc_beat_eop_io0_lf_evt;

      assign host_credit_inc_io0_lf_evt_int  = host_credit_inc_io0_lf_evt;
      assign noc_credit_inc_io0_lf_evt_int   = noc_credit_inc_io0_lf_evt;

      assign host_dest_hp_io0_rt_msg_int     = host_dest_hp_io0_rt_msg;
      assign host_dest_int_io0_rt_msg_int    = host_dest_int_io0_rt_msg;
      assign host_xfer_qos_io0_rt_msg_int    = host_xfer_qos_io0_rt_msg;
      assign host_beat_valid_io0_rt_msg_int  = host_beat_valid_io0_rt_msg;
      assign host_beat_data_io0_rt_msg_int   = host_beat_data_io0_rt_msg;
      assign host_beat_sop_io0_rt_msg_int    = host_beat_sop_io0_rt_msg;
      assign host_beat_eop_io0_rt_msg_int    = host_beat_eop_io0_rt_msg;

      assign noc_beat_data_io0_rt_msg_int    = noc_beat_data_io0_rt_msg;
      assign noc_beat_valid_io0_rt_msg_int   = noc_beat_valid_io0_rt_msg;
      assign noc_beat_sop_io0_rt_msg_int     = noc_beat_sop_io0_rt_msg;
      assign noc_beat_eop_io0_rt_msg_int     = noc_beat_eop_io0_rt_msg;

      assign host_credit_inc_io0_rt_msg_int  = host_credit_inc_io0_rt_msg;
      assign noc_credit_inc_io0_rt_msg_int   = noc_credit_inc_io0_rt_msg;

      assign host_dest_hp_io0_rt_evt_int     = host_dest_hp_io0_rt_evt;
      assign host_dest_int_io0_rt_evt_int    = host_dest_int_io0_rt_evt;
      assign host_xfer_qos_io0_rt_evt_int    = host_xfer_qos_io0_rt_evt;
      assign host_beat_valid_io0_rt_evt_int  = host_beat_valid_io0_rt_evt;
      assign host_beat_data_io0_rt_evt_int   = host_beat_data_io0_rt_evt;
      assign host_beat_sop_io0_rt_evt_int    = host_beat_sop_io0_rt_evt;
      assign host_beat_eop_io0_rt_evt_int    = host_beat_eop_io0_rt_evt;

      assign noc_beat_data_io0_rt_evt_int    = noc_beat_data_io0_rt_evt;
      assign noc_beat_valid_io0_rt_evt_int   = noc_beat_valid_io0_rt_evt;
      assign noc_beat_sop_io0_rt_evt_int     = noc_beat_sop_io0_rt_evt;
      assign noc_beat_eop_io0_rt_evt_int     = noc_beat_eop_io0_rt_evt;

      assign host_credit_inc_io0_rt_evt_int  = host_credit_inc_io0_rt_evt;
      assign noc_credit_inc_io0_rt_evt_int   = noc_credit_inc_io0_rt_evt;

   `endif

   //`ifdef DV_IOSHIRE_DBGNOC_STIM
      //assign host_dest_int_io0_rt_msg = (host_beat_valid_io0_rt_msg == 1'b1) ? 2'b01 : 2'bx;
      //assign host_dest_int_io0_rt_evt = (host_beat_valid_io0_rt_evt == 1'b1) ? 2'b01 : 2'bx;
      //assign host_dest_int_io0_lf_msg = (host_beat_valid_io0_lf_msg == 1'b1) ? 2'b01 : 2'bx;
      //assign host_dest_int_io0_lf_evt = (host_beat_valid_io0_lf_evt == 1'b1) ? 2'b01 : 2'bx;

      //assign host_xfer_qos_io0_rt_msg = (host_beat_valid_io0_rt_msg == 1'b1) ? 4'b01 : 4'bx;
      //assign host_xfer_qos_io0_rt_evt = (host_beat_valid_io0_rt_evt == 1'b1) ? 4'b01 : 4'bx;
      //assign host_xfer_qos_io0_lf_msg = (host_beat_valid_io0_lf_msg == 1'b1) ? 4'b01 : 4'bx;
      //assign host_xfer_qos_io0_lf_evt = (host_beat_valid_io0_lf_evt == 1'b1) ? 4'b01 : 4'bx;

      //assign host_credit_inc_io0_rt_msg = 1'b0;
      //assign host_credit_inc_io0_rt_evt = 1'b0;
      //assign host_credit_inc_io0_lf_msg = 1'b0;
      //assign host_credit_inc_io0_lf_evt = 1'b0;

      //
      // Debug NoC RT_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (IOSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_rt_msg (
            .reset            (reset_ios_ckcore),
            .clk              (clk__noc),

            .tx_req_addr      (host_dest_hp_io0_rt_msg_int),
            .tx_req_dest_int  (host_dest_int_io0_rt_msg_int),
            .tx_req_qos       (host_xfer_qos_io0_rt_msg_int),
            .tx_req_valid     (host_beat_valid_io0_rt_msg_int),
            .tx_req_data      (host_beat_data_io0_rt_msg_int),
            .tx_req_sop       (host_beat_sop_io0_rt_msg_int),
            .tx_req_eop       (host_beat_eop_io0_rt_msg_int),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_rt_msg_int),
            .rx_req_valid     (noc_beat_valid_io0_rt_msg_int),
            .rx_req_sop       (noc_beat_sop_io0_rt_msg_int),
            .rx_req_eop       (noc_beat_eop_io0_rt_msg_int),

            .host_credit_inc  (host_credit_inc_io0_rt_msg_int),
            .noc_credit_inc   (noc_credit_inc_io0_rt_msg_int)
         );

      //
      // Debug NoC RT_EVT IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (IOSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (32),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_EVT_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_rt_evt (
            .reset            (reset_ios_ckcore),
            .clk              (clk__noc),

            .tx_req_addr      (host_dest_hp_io0_rt_evt_int),
            .tx_req_dest_int  (host_dest_int_io0_rt_evt_int),
            .tx_req_qos       (host_xfer_qos_io0_rt_evt_int),
            .tx_req_valid     (host_beat_valid_io0_rt_evt_int),
            .tx_req_data      (host_beat_data_io0_rt_evt_int),
            .tx_req_sop       (host_beat_sop_io0_rt_evt_int),
            .tx_req_eop       (host_beat_eop_io0_rt_evt_int),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_rt_evt_int),
            .rx_req_valid     (noc_beat_valid_io0_rt_evt_int),
            .rx_req_sop       (noc_beat_sop_io0_rt_evt_int),
            .rx_req_eop       (noc_beat_eop_io0_rt_evt_int),

            .host_credit_inc  (host_credit_inc_io0_rt_evt_int),
            .noc_credit_inc   (noc_credit_inc_io0_rt_evt_int)
         );

      //
      // Debug NoC LF_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (IOSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_lf_msg (
            .reset            (reset_ios_ckcore),
            .clk              (clk__noc),

            .tx_req_addr      (host_dest_hp_io0_lf_msg_int),
            .tx_req_dest_int  (host_dest_int_io0_lf_msg_int),
            .tx_req_qos       (host_xfer_qos_io0_lf_msg_int),
            .tx_req_valid     (host_beat_valid_io0_lf_msg_int),
            .tx_req_data      (host_beat_data_io0_lf_msg_int),
            .tx_req_sop       (host_beat_sop_io0_lf_msg_int),
            .tx_req_eop       (host_beat_eop_io0_lf_msg_int),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_lf_msg_int),
            .rx_req_valid     (noc_beat_valid_io0_lf_msg_int),
            .rx_req_sop       (noc_beat_sop_io0_lf_msg_int),
            .rx_req_eop       (noc_beat_eop_io0_lf_msg_int),

            .host_credit_inc  (host_credit_inc_io0_lf_msg_int),
            .noc_credit_inc   (noc_credit_inc_io0_lf_msg_int)
         );

      //
      // Debug NoC LF_EVT IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (IOSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (32),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_EVT_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_lf_evt (
            .reset            (reset_ios_ckcore),
            .clk              (clk__noc),

            .tx_req_addr      (host_dest_hp_io0_lf_evt_int),
            .tx_req_dest_int  (host_dest_int_io0_lf_evt_int),
            .tx_req_qos       (host_xfer_qos_io0_lf_evt_int),
            .tx_req_valid     (host_beat_valid_io0_lf_evt_int),
            .tx_req_data      (host_beat_data_io0_lf_evt_int),
            .tx_req_sop       (host_beat_sop_io0_lf_evt_int),
            .tx_req_eop       (host_beat_eop_io0_lf_evt_int),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_lf_evt_int),
            .rx_req_valid     (noc_beat_valid_io0_lf_evt_int),
            .rx_req_sop       (noc_beat_sop_io0_lf_evt_int),
            .rx_req_eop       (noc_beat_eop_io0_lf_evt_int),

            .host_credit_inc  (host_credit_inc_io0_lf_evt_int),
            .noc_credit_inc   (noc_credit_inc_io0_lf_evt_int)
         );

   //`endif

   `ifdef DV_IOSHIRE_NO_PU
      assign pu_dma_pu_noc_axi_WID  = 'b0;
      assign pu_emmc_pu_noc_axi_WID = 'b0;
      assign pu_usb0_pu_noc_axi_WID = 'b0;
      assign pu_usb1_pu_noc_axi_WID = 'b0;
   `endif
   `ifdef DV_IOSHIRE_NOC_ONLY
      assign spio_dma_spio_noc_axi_WID = 'b0;
   `endif

   //
   // Service Processor to SPROM AXI Interface
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_rom_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_rom_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_rom_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_rom_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_rom_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_spio_rom_axi_aw_region;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_spio_rom_axi_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_noc_spio_rom_axi_aw_user;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_noc_spio_rom_axi_ar_user;
   tri0                                       spio_noc_spio_rom_axi_w_user;
   tri0                                       spio_noc_spio_rom_axi_b_user;
   tri0                                       spio_noc_spio_rom_axi_r_user;
   tri0                                       spio_noc_spio_rom_axi_aw_qos;
   tri0                                       spio_noc_spio_rom_axi_ar_qos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH   (3),
                       .ARUSER_WIDTH   (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH   (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH (4),
                       .AXQOS_WIDTH    (4),
                       .AXLEN_WIDTH    (8),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_rom_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_noc_spio_rom_axi_AWID),
         .aw_addr      (spio_noc_spio_rom_axi_AWADDR),
         .aw_len       (spio_noc_spio_rom_axi_AWLEN),
         .aw_size      (spio_noc_spio_rom_axi_AWSIZE),
         .aw_burst     (spio_noc_spio_rom_axi_AWBURST),
         .aw_lock      (spio_noc_spio_rom_axi_AWLOCK),
         .aw_cache     (spio_noc_spio_rom_axi_AWCACHE),
         .aw_prot      (spio_noc_spio_rom_axi_AWPROT),
         .aw_qos       (spio_noc_spio_rom_axi_AWQOS),
         .aw_region    (spio_noc_spio_rom_axi_aw_region),
         .aw_user      (spio_noc_spio_rom_axi_aw_user),
         .aw_valid     (spio_noc_spio_rom_axi_AWVALID),
         .aw_ready     (spio_noc_spio_rom_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_noc_spio_rom_axi_aw_trans_id),

         .w_user       (spio_noc_spio_rom_axi_w_user),
         .w_data       (spio_noc_spio_rom_axi_WDATA),
         .w_strb       (spio_noc_spio_rom_axi_WSTRB),
         .w_last       (spio_noc_spio_rom_axi_WLAST),
         .w_valid      (spio_noc_spio_rom_axi_WVALID),
         .w_ready      (spio_noc_spio_rom_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_noc_spio_rom_axi_w_trans_id),

         .b_user       (spio_noc_spio_rom_axi_b_user),
         .b_id         (spio_noc_spio_rom_axi_BID),
         .b_resp       (spio_noc_spio_rom_axi_BRESP),
         .b_valid      (spio_noc_spio_rom_axi_BVALID),
         .b_ready      (spio_noc_spio_rom_axi_BREADY),
         .b_trans_id   (spio_noc_spio_rom_axi_b_trans_id),

         .ar_id        (spio_noc_spio_rom_axi_ARID),
         .ar_addr      (spio_noc_spio_rom_axi_ARADDR),
         .ar_len       (spio_noc_spio_rom_axi_ARLEN),
         .ar_size      (spio_noc_spio_rom_axi_ARSIZE),
         .ar_burst     (spio_noc_spio_rom_axi_ARBURST),
         .ar_lock      (spio_noc_spio_rom_axi_ARLOCK),
         .ar_cache     (spio_noc_spio_rom_axi_ARCACHE),
         .ar_prot      (spio_noc_spio_rom_axi_ARPROT),
         .ar_qos       (spio_noc_spio_rom_axi_ARQOS),
         .ar_region    (spio_noc_spio_rom_axi_ar_region),
         .ar_user      (spio_noc_spio_rom_axi_ar_user),
         .ar_valid     (spio_noc_spio_rom_axi_ARVALID),
         .ar_ready     (spio_noc_spio_rom_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_noc_spio_rom_axi_ar_trans_id),

         .r_last       (spio_noc_spio_rom_axi_RLAST),
         .r_user       (spio_noc_spio_rom_axi_r_user),
         .r_id         (spio_noc_spio_rom_axi_RID),
         .r_data       (spio_noc_spio_rom_axi_RDATA),
         .r_resp       (spio_noc_spio_rom_axi_RRESP),
         .r_valid      (spio_noc_spio_rom_axi_RVALID),
         .r_ready      (spio_noc_spio_rom_axi_RREADY),
         .r_trans_id   (spio_noc_spio_rom_axi_r_trans_id)
      );

   //
   // Service Processor to Scratchpad RAM AXI Interface
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_scram_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_scram_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_scram_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_scram_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_spio_scram_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_spio_scram_axi_aw_region;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_spio_scram_axi_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_noc_spio_scram_axi_aw_user;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_noc_spio_scram_axi_ar_user;
   tri0                                       spio_noc_spio_scram_axi_w_user;
   tri0                                       spio_noc_spio_scram_axi_b_user;
   tri0                                       spio_noc_spio_scram_axi_r_user;
   tri0                                       spio_noc_spio_scram_axi_aw_qos;
   tri0                                       spio_noc_spio_scram_axi_ar_qos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH   (3),
                       .ARUSER_WIDTH   (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH   (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH (4),
                       .AXQOS_WIDTH    (4),
                       .AXLEN_WIDTH    (8),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_scram_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_noc_spio_scram_axi_AWID),
         .aw_addr      (spio_noc_spio_scram_axi_AWADDR),
         .aw_len       (spio_noc_spio_scram_axi_AWLEN),
         .aw_size      (spio_noc_spio_scram_axi_AWSIZE),
         .aw_burst     (spio_noc_spio_scram_axi_AWBURST),
         .aw_lock      (spio_noc_spio_scram_axi_AWLOCK),
         .aw_cache     (spio_noc_spio_scram_axi_AWCACHE),
         .aw_prot      (spio_noc_spio_scram_axi_AWPROT),
         .aw_qos       (spio_noc_spio_scram_axi_AWQOS),
         .aw_region    (spio_noc_spio_scram_axi_aw_region),
         .aw_user      (spio_noc_spio_scram_axi_aw_user),
         .aw_valid     (spio_noc_spio_scram_axi_AWVALID),
         .aw_ready     (spio_noc_spio_scram_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_noc_spio_scram_axi_aw_trans_id),

         .w_user       (spio_noc_spio_scram_axi_w_user),
         .w_data       (spio_noc_spio_scram_axi_WDATA),
         .w_strb       (spio_noc_spio_scram_axi_WSTRB),
         .w_last       (spio_noc_spio_scram_axi_WLAST),
         .w_valid      (spio_noc_spio_scram_axi_WVALID),
         .w_ready      (spio_noc_spio_scram_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_noc_spio_scram_axi_w_trans_id),

         .b_user       (spio_noc_spio_scram_axi_b_user),
         .b_id         (spio_noc_spio_scram_axi_BID),
         .b_resp       (spio_noc_spio_scram_axi_BRESP),
         .b_valid      (spio_noc_spio_scram_axi_BVALID),
         .b_ready      (spio_noc_spio_scram_axi_BREADY),
         .b_trans_id   (spio_noc_spio_scram_axi_b_trans_id),

         .ar_id        (spio_noc_spio_scram_axi_ARID),
         .ar_addr      (spio_noc_spio_scram_axi_ARADDR),
         .ar_len       (spio_noc_spio_scram_axi_ARLEN),
         .ar_size      (spio_noc_spio_scram_axi_ARSIZE),
         .ar_burst     (spio_noc_spio_scram_axi_ARBURST),
         .ar_lock      (spio_noc_spio_scram_axi_ARLOCK),
         .ar_cache     (spio_noc_spio_scram_axi_ARCACHE),
         .ar_prot      (spio_noc_spio_scram_axi_ARPROT),
         .ar_qos       (spio_noc_spio_scram_axi_ARQOS),
         .ar_region    (spio_noc_spio_scram_axi_ar_region),
         .ar_user      (spio_noc_spio_scram_axi_ar_user),
         .ar_valid     (spio_noc_spio_scram_axi_ARVALID),
         .ar_ready     (spio_noc_spio_scram_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_noc_spio_scram_axi_ar_trans_id),

         .r_last       (spio_noc_spio_scram_axi_RLAST),
         .r_user       (spio_noc_spio_scram_axi_r_user),
         .r_id         (spio_noc_spio_scram_axi_RID),
         .r_data       (spio_noc_spio_scram_axi_RDATA),
         .r_resp       (spio_noc_spio_scram_axi_RRESP),
         .r_valid      (spio_noc_spio_scram_axi_RVALID),
         .r_ready      (spio_noc_spio_scram_axi_RREADY),
         .r_trans_id   (spio_noc_spio_scram_axi_r_trans_id)
      );

   //
   // SPIO_NOC to PU_NOC AXI Interface
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_pu_noc_axi_aw_region;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_pu_noc_axi_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_noc_pu_noc_axi_aw_user;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_noc_pu_noc_axi_ar_user;
   tri0                                       spio_noc_pu_noc_axi_w_user;
   tri0                                       spio_noc_pu_noc_axi_b_user;
   tri0                                       spio_noc_pu_noc_axi_r_user;
   tri0                                       spio_noc_pu_noc_axi_aw_qos;
   tri0                                       spio_noc_pu_noc_axi_ar_qos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (8),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH   (3),
                       .ARUSER_WIDTH   (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH   (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH (`EVL_BUS_REGION_WIDTH),
                       .AXQOS_WIDTH    (4),
                       .AXLEN_WIDTH    (8),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS(1)) m_spio_noc_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_noc_pu_noc_axi_AWID),
         .aw_addr      (spio_noc_pu_noc_axi_AWADDR),
         .aw_len       (spio_noc_pu_noc_axi_AWLEN),
         .aw_size      (spio_noc_pu_noc_axi_AWSIZE),
         .aw_burst     (spio_noc_pu_noc_axi_AWBURST),
         .aw_lock      (spio_noc_pu_noc_axi_AWLOCK),
         .aw_cache     (spio_noc_pu_noc_axi_AWCACHE),
         .aw_prot      (spio_noc_pu_noc_axi_AWPROT),
         .aw_qos       (spio_noc_pu_noc_axi_AWQOS),
         .aw_region    (spio_noc_pu_noc_axi_aw_region),
         .aw_user      (spio_noc_pu_noc_axi_AWUSER),
         .aw_valid     (spio_noc_pu_noc_axi_AWVALID),
         .aw_ready     (spio_noc_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_noc_pu_noc_axi_aw_trans_id),

         .w_user       (spio_noc_pu_noc_axi_w_user),
         .w_data       (spio_noc_pu_noc_axi_WDATA),
         .w_strb       (spio_noc_pu_noc_axi_WSTRB),
         .w_last       (spio_noc_pu_noc_axi_WLAST),
         .w_valid      (spio_noc_pu_noc_axi_WVALID),
         .w_ready      (spio_noc_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_noc_pu_noc_axi_w_trans_id),

         .b_user       (spio_noc_pu_noc_axi_b_user),
         .b_id         (spio_noc_pu_noc_axi_BID),
         .b_resp       (spio_noc_pu_noc_axi_BRESP),
         .b_valid      (spio_noc_pu_noc_axi_BVALID),
         .b_ready      (spio_noc_pu_noc_axi_BREADY),
         .b_trans_id   (spio_noc_pu_noc_axi_b_trans_id),

         .ar_id        (spio_noc_pu_noc_axi_ARID),
         .ar_addr      (spio_noc_pu_noc_axi_ARADDR),
         .ar_len       (spio_noc_pu_noc_axi_ARLEN),
         .ar_size      (spio_noc_pu_noc_axi_ARSIZE),
         .ar_burst     (spio_noc_pu_noc_axi_ARBURST),
         .ar_lock      (spio_noc_pu_noc_axi_ARLOCK),
         .ar_cache     (spio_noc_pu_noc_axi_ARCACHE),
         .ar_prot      (spio_noc_pu_noc_axi_ARPROT),
         .ar_qos       (spio_noc_pu_noc_axi_ARQOS),
         .ar_region    (spio_noc_pu_noc_axi_ar_region),
         .ar_user      (spio_noc_pu_noc_axi_ar_user),
         .ar_valid     (spio_noc_pu_noc_axi_ARVALID),
         .ar_ready     (spio_noc_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_noc_pu_noc_axi_ar_trans_id),

         .r_last       (spio_noc_pu_noc_axi_RLAST),
         .r_user       (spio_noc_pu_noc_axi_r_user),
         .r_id         (spio_noc_pu_noc_axi_RID),
         .r_data       (spio_noc_pu_noc_axi_RDATA),
         .r_resp       (spio_noc_pu_noc_axi_RRESP),
         .r_valid      (spio_noc_pu_noc_axi_RVALID),
         .r_ready      (spio_noc_pu_noc_axi_RREADY),
         .r_trans_id   (spio_noc_pu_noc_axi_r_trans_id)
      );


   `ifdef EVL_MAXSHIRE_CHECKS // ------------------------------------------------------------------{

      wire                            have_activity;
      wire [`NUM_MAXION_CORES-1:0]    maxion_busy;

      wire sys_slave_axi_AR_channel_t sbm_mesh_slave_axi_AR;
      wire                            sbm_mesh_slave_axi_ARVALID;
      wire                            sbm_mesh_slave_axi_ARREADY;
      wire sys_slave_axi_AW_channel_t sbm_mesh_slave_axi_AW;
      wire                            sbm_mesh_slave_axi_AWVALID;
      wire                            sbm_mesh_slave_axi_AWREADY;
      wire sys_slave_axi_W_channel_t  sbm_mesh_slave_axi_W;
      wire                            sbm_mesh_slave_axi_WVALID;
      wire                            sbm_mesh_slave_axi_WREADY;
      wire sys_slave_axi_B_channel_t  sbm_mesh_slave_axi_B;
      wire                            sbm_mesh_slave_axi_BVALID;
      wire                            sbm_mesh_slave_axi_BREADY;
      wire sys_slave_axi_R_channel_t  sbm_mesh_slave_axi_R;
      wire                            sbm_mesh_slave_axi_RVALID;
      wire                            sbm_mesh_slave_axi_RREADY;

      assign sbm_mesh_slave_axi_AR      = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_AR;
      assign sbm_mesh_slave_axi_ARVALID = `EVL_MAXSHIRE_PATH.sbm_enable_read;
      assign sbm_mesh_slave_axi_ARREADY = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_ARREADY;
      assign sbm_mesh_slave_axi_AW      = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_AW;
      assign sbm_mesh_slave_axi_AWVALID = `EVL_MAXSHIRE_PATH.sbm_enable_write;
      assign sbm_mesh_slave_axi_AWREADY = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_AWREADY;
      assign sbm_mesh_slave_axi_W       = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_W;
      assign sbm_mesh_slave_axi_WVALID  = `EVL_MAXSHIRE_PATH.sbm_enable_write;
      assign sbm_mesh_slave_axi_WREADY  = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_WREADY;
      assign sbm_mesh_slave_axi_B       = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_B;
      assign sbm_mesh_slave_axi_BVALID  = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_BVALID;
      assign sbm_mesh_slave_axi_BREADY  = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_BREADY;
      assign sbm_mesh_slave_axi_R       = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_R;
      assign sbm_mesh_slave_axi_RVALID  = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_RVALID;
      assign sbm_mesh_slave_axi_RREADY  = `EVL_MAXSHIRE_PATH.sbm_sys_mesh_slave_axi_RREADY;

      evl_maxshire_interface #(.IOSHIRE_ID       (IOSHIRE_ID),
                               .NUM_L3_PORTS     (NUM_L3_PORTS),
                               .NUM_NEIGH        (`MAXSHIRE_SC_NUM_NEIGH),
                               .NUM_ET_REQ_PORTS (`MAXSHIRE_SC_NUM_BANKS + `MAXSHIRE_SC_NUM_UC),
                               .SKIP_LEVEL       (2)) maxshire_top (
            .reset_ios_ckcore                  ( reset_ios_ckcore                                     ),
            .clk_core                          ( clk_maxshire_core                                    ),
            .clk_uncore                        ( clk_maxshire_uncore                                  ),
            .clk_ios                           ( clk_ios                                              ),
            .clk_ios_ahb                       ( clk_ios_ahb                                          ),

            .uc_to_l3_mesh_master_axi_AR       ( maxion_uc_to_l3_axi_AR                               ),
            .uc_to_l3_mesh_master_axi_ARVALID  ( maxion_uc_to_l3_axi_ARVALID                          ),
            .uc_to_l3_mesh_master_axi_ARREADY  ( maxion_uc_to_l3_axi_ARREADY                          ),
            .uc_to_l3_mesh_master_axi_AW       ( maxion_uc_to_l3_axi_AW                               ),
            .uc_to_l3_mesh_master_axi_AWVALID  ( maxion_uc_to_l3_axi_AWVALID                          ),
            .uc_to_l3_mesh_master_axi_AWREADY  ( maxion_uc_to_l3_axi_AWREADY                          ),
            .uc_to_l3_mesh_master_axi_W        ( maxion_uc_to_l3_axi_W                                ),
            .uc_to_l3_mesh_master_axi_WVALID   ( maxion_uc_to_l3_axi_WVALID                           ),
            .uc_to_l3_mesh_master_axi_WREADY   ( maxion_uc_to_l3_axi_WREADY                           ),
            .uc_to_l3_mesh_master_axi_B        ( maxion_uc_to_l3_axi_B                                ),
            .uc_to_l3_mesh_master_axi_BVALID   ( maxion_uc_to_l3_axi_BVALID                           ),
            .uc_to_l3_mesh_master_axi_BREADY   ( maxion_uc_to_l3_axi_BREADY                           ),
            .uc_to_l3_mesh_master_axi_R        ( maxion_uc_to_l3_axi_R                                ),
            .uc_to_l3_mesh_master_axi_RVALID   ( maxion_uc_to_l3_axi_RVALID                           ),
            .uc_to_l3_mesh_master_axi_RREADY   ( maxion_uc_to_l3_axi_RREADY                           ),

            .to_sys_mesh_master_axi_AR         ( maxion_to_sys_axi_AR                                 ),
            .to_sys_mesh_master_axi_ARVALID    ( maxion_to_sys_axi_ARVALID                            ),
            .to_sys_mesh_master_axi_ARREADY    ( maxion_to_sys_axi_ARREADY                            ),
            .to_sys_mesh_master_axi_AW         ( maxion_to_sys_axi_AW                                 ),
            .to_sys_mesh_master_axi_AWVALID    ( maxion_to_sys_axi_AWVALID                            ),
            .to_sys_mesh_master_axi_AWREADY    ( maxion_to_sys_axi_AWREADY                            ),
            .to_sys_mesh_master_axi_W          ( maxion_to_sys_axi_W                                  ),
            .to_sys_mesh_master_axi_WVALID     ( maxion_to_sys_axi_WVALID                             ),
            .to_sys_mesh_master_axi_WREADY     ( maxion_to_sys_axi_WREADY                             ),
            .to_sys_mesh_master_axi_B          ( maxion_to_sys_axi_B                                  ),
            .to_sys_mesh_master_axi_BVALID     ( maxion_to_sys_axi_BVALID                             ),
            .to_sys_mesh_master_axi_BREADY     ( maxion_to_sys_axi_BREADY                             ),
            .to_sys_mesh_master_axi_R          ( maxion_to_sys_axi_R                                  ),
            .to_sys_mesh_master_axi_RVALID     ( maxion_to_sys_axi_RVALID                             ),
            .to_sys_mesh_master_axi_RREADY     ( maxion_to_sys_axi_RREADY                             ),

            .uc_to_sys_mesh_master_axi_AR      ( maxion_uc_to_sys_axi_AR                              ),
            .uc_to_sys_mesh_master_axi_ARVALID ( maxion_uc_to_sys_axi_ARVALID                         ),
            .uc_to_sys_mesh_master_axi_ARREADY ( maxion_uc_to_sys_axi_ARREADY                         ),
            .uc_to_sys_mesh_master_axi_AW      ( maxion_uc_to_sys_axi_AW                              ),
            .uc_to_sys_mesh_master_axi_AWVALID ( maxion_uc_to_sys_axi_AWVALID                         ),
            .uc_to_sys_mesh_master_axi_AWREADY ( maxion_uc_to_sys_axi_AWREADY                         ),
            .uc_to_sys_mesh_master_axi_W       ( maxion_uc_to_sys_axi_W                               ),
            .uc_to_sys_mesh_master_axi_WVALID  ( maxion_uc_to_sys_axi_WVALID                          ),
            .uc_to_sys_mesh_master_axi_WREADY  ( maxion_uc_to_sys_axi_WREADY                          ),
            .uc_to_sys_mesh_master_axi_B       ( maxion_uc_to_sys_axi_B                               ),
            .uc_to_sys_mesh_master_axi_BVALID  ( maxion_uc_to_sys_axi_BVALID                          ),
            .uc_to_sys_mesh_master_axi_BREADY  ( maxion_uc_to_sys_axi_BREADY                          ),
            .uc_to_sys_mesh_master_axi_R       ( maxion_uc_to_sys_axi_R                               ),
            .uc_to_sys_mesh_master_axi_RVALID  ( maxion_uc_to_sys_axi_RVALID                          ),
            .uc_to_sys_mesh_master_axi_RREADY  ( maxion_uc_to_sys_axi_RREADY                          ),

            .l3_mesh_slave_axi_AR              ( maxion_l3_axi_AR                                     ),
            .l3_mesh_slave_axi_ARVALID         ( maxion_l3_axi_ARVALID                                ),
            .l3_mesh_slave_axi_ARREADY         ( maxion_l3_axi_ARREADY                                ),
            .l3_mesh_slave_axi_AW              ( maxion_l3_axi_AW                                     ),
            .l3_mesh_slave_axi_AWVALID         ( maxion_l3_axi_AWVALID                                ),
            .l3_mesh_slave_axi_AWREADY         ( maxion_l3_axi_AWREADY                                ),
            .l3_mesh_slave_axi_W               ( maxion_l3_axi_W                                      ),
            .l3_mesh_slave_axi_WVALID          ( maxion_l3_axi_WVALID                                 ),
            .l3_mesh_slave_axi_WREADY          ( maxion_l3_axi_WREADY                                 ),
            .l3_mesh_slave_axi_B               ( maxion_l3_axi_B                                      ),
            .l3_mesh_slave_axi_BVALID          ( maxion_l3_axi_BVALID                                 ),
            .l3_mesh_slave_axi_BREADY          ( maxion_l3_axi_BREADY                                 ),
            .l3_mesh_slave_axi_R               ( maxion_l3_axi_R                                      ),
            .l3_mesh_slave_axi_RVALID          ( maxion_l3_axi_RVALID                                 ),
            .l3_mesh_slave_axi_RREADY          ( maxion_l3_axi_RREADY                                 ),

            .to_l3_mesh_master_axi_AR          ( maxion_to_l3_axi_AR                                  ),
            .to_l3_mesh_master_axi_ARVALID     ( maxion_to_l3_axi_ARVALID                             ),
            .to_l3_mesh_master_axi_ARREADY     ( maxion_to_l3_axi_ARREADY                             ),
            .to_l3_mesh_master_axi_AW          ( maxion_to_l3_axi_AW                                  ),
            .to_l3_mesh_master_axi_AWVALID     ( maxion_to_l3_axi_AWVALID                             ),
            .to_l3_mesh_master_axi_AWREADY     ( maxion_to_l3_axi_AWREADY                             ),
            .to_l3_mesh_master_axi_W           ( maxion_to_l3_axi_W                                   ),
            .to_l3_mesh_master_axi_WVALID      ( maxion_to_l3_axi_WVALID                              ),
            .to_l3_mesh_master_axi_WREADY      ( maxion_to_l3_axi_WREADY                              ),
            .to_l3_mesh_master_axi_B           ( maxion_to_l3_axi_B                                   ),
            .to_l3_mesh_master_axi_BVALID      ( maxion_to_l3_axi_BVALID                              ),
            .to_l3_mesh_master_axi_BREADY      ( maxion_to_l3_axi_BREADY                              ),
            .to_l3_mesh_master_axi_R           ( maxion_to_l3_axi_R                                   ),
            .to_l3_mesh_master_axi_RVALID      ( maxion_to_l3_axi_RVALID                              ),
            .to_l3_mesh_master_axi_RREADY      ( maxion_to_l3_axi_RREADY                              ),

            .sys_mesh_slave_axi_AR             ( maxion_sys_axi_AR                                    ),
            .sys_mesh_slave_axi_ARVALID        ( maxion_sys_axi_ARVALID                               ),
            .sys_mesh_slave_axi_ARREADY        ( maxion_sys_axi_ARREADY                               ),
            .sys_mesh_slave_axi_AW             ( maxion_sys_axi_AW                                    ),
            .sys_mesh_slave_axi_AWVALID        ( maxion_sys_axi_AWVALID                               ),
            .sys_mesh_slave_axi_AWvcvalid      ( maxion_sys_axi_AWvcvalid                             ),
            .sys_mesh_slave_axi_AWcredit       ( maxion_sys_axi_AWcredit                              ),
            .sys_mesh_slave_axi_AWREADY        ( maxion_sys_axi_AWREADY                               ),
            .sys_mesh_slave_axi_W              ( maxion_sys_axi_W                                     ),
            .sys_mesh_slave_axi_WVALID         ( maxion_sys_axi_WVALID                                ),
            .sys_mesh_slave_axi_Wvcvalid       ( maxion_sys_axi_Wvcvalid                              ),
            .sys_mesh_slave_axi_WREADY         ( maxion_sys_axi_WREADY                                ),
            .sys_mesh_slave_axi_B              ( maxion_sys_axi_B                                     ),
            .sys_mesh_slave_axi_BVALID         ( maxion_sys_axi_BVALID                                ),
            .sys_mesh_slave_axi_BREADY         ( maxion_sys_axi_BREADY                                ),
            .sys_mesh_slave_axi_R              ( maxion_sys_axi_R                                     ),
            .sys_mesh_slave_axi_RVALID         ( maxion_sys_axi_RVALID                                ),
            .sys_mesh_slave_axi_RREADY         ( maxion_sys_axi_RREADY                                ),

            .sbm_sys_mesh_slave_axi_AR         ( sbm_mesh_slave_axi_AR                                ),
            .sbm_sys_mesh_slave_axi_ARVALID    ( sbm_mesh_slave_axi_ARVALID                           ),
            .sbm_sys_mesh_slave_axi_ARREADY    ( sbm_mesh_slave_axi_ARREADY                           ),
            .sbm_sys_mesh_slave_axi_AW         ( sbm_mesh_slave_axi_AW                                ),
            .sbm_sys_mesh_slave_axi_AWVALID    ( sbm_mesh_slave_axi_AWVALID                           ),
            .sbm_sys_mesh_slave_axi_AWREADY    ( sbm_mesh_slave_axi_AWREADY                           ),
            .sbm_sys_mesh_slave_axi_W          ( sbm_mesh_slave_axi_W                                 ),
            .sbm_sys_mesh_slave_axi_WVALID     ( sbm_mesh_slave_axi_WVALID                            ),
            .sbm_sys_mesh_slave_axi_WREADY     ( sbm_mesh_slave_axi_WREADY                            ),
            .sbm_sys_mesh_slave_axi_B          ( sbm_mesh_slave_axi_B                                 ),
            .sbm_sys_mesh_slave_axi_BVALID     ( sbm_mesh_slave_axi_BVALID                            ),
            .sbm_sys_mesh_slave_axi_BREADY     ( sbm_mesh_slave_axi_BREADY                            ),
            .sbm_sys_mesh_slave_axi_R          ( sbm_mesh_slave_axi_R                                 ),
            .sbm_sys_mesh_slave_axi_RVALID     ( sbm_mesh_slave_axi_RVALID                            ),
            .sbm_sys_mesh_slave_axi_RREADY     ( sbm_mesh_slave_axi_RREADY                            ),

            .maxdbg_maxshire_bpam_apb_paddr    ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_paddr    ),
            .maxdbg_maxshire_bpam_apb_penable  ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_penable  ),
            .maxdbg_maxshire_bpam_apb_prdata   ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_prdata   ),
            .maxdbg_maxshire_bpam_apb_pready   ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_pready   ),
            .maxdbg_maxshire_bpam_apb_psel     ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_psel     ),
            .maxdbg_maxshire_bpam_apb_pslverr  ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_pslverr  ),
            .maxdbg_maxshire_bpam_apb_pwdata   ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_pwdata   ),
            .maxdbg_maxshire_bpam_apb_pstrb    ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_pstrb    ),
            .maxdbg_maxshire_bpam_apb_pwrite   ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_apb_pwrite   ),
            .maxdbg_maxshire_bpam_ndreset      ( `EVL_MAXSHIRE_PATH.maxdbg_maxshire_bpam_ndreset      ),

            .maxneigh_sc_mem_req_valid         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_req_valid         ),
            .maxneigh_sc_mem_req_ready         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_req_ready         ),
            .maxneigh_sc_mem_req_info          ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_req_info          ),

            .maxneigh_sc_mem_rsp_valid         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_rsp_valid         ),
            .maxneigh_sc_mem_rsp_ready         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_rsp_ready         ),
            .maxneigh_sc_mem_rsp_info          ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mem_rsp_info          ),

            .maxneigh_sc_mmio_req_valid        ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_req_valid        ),
            .maxneigh_sc_mmio_req_ready        ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_req_ready        ),
            .maxneigh_sc_mmio_req_info         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_req_info         ),

            .maxneigh_sc_mmio_rsp_valid        ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_rsp_valid        ),
            .maxneigh_sc_mmio_rsp_ready        ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_rsp_ready        ),
            .maxneigh_sc_mmio_rsp_info         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_mmio_rsp_info         ),

            .maxneigh_sc_gag_req_valid         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_req_valid         ),
            .maxneigh_sc_gag_req_ready         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_req_ready         ),
            .maxneigh_sc_gag_req_info          ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_req_info          ),

            .maxneigh_sc_gag_rsp_valid         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_rsp_valid         ),
            .maxneigh_sc_gag_rsp_ready         ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_rsp_ready         ),
            .maxneigh_sc_gag_rsp_info          ( `EVL_MAXSHIRE_PATH.maxneigh_sc_gag_rsp_info          ),

            .neigh_sc_req_valid                ( `EVL_MAXSHIRE_PATH.neigh_sc_req_valid                ),
            .neigh_sc_req_ready                ( `EVL_MAXSHIRE_PATH.neigh_sc_req_ready                ),
            .neigh_sc_req_info                 ( `EVL_MAXSHIRE_PATH.neigh_sc_req_info                 ),

            .neigh_sc_rsp_valid                ( `EVL_MAXSHIRE_PATH.neigh_sc_rsp_valid                ),
            .neigh_sc_rsp_ready                ( `EVL_MAXSHIRE_PATH.neigh_sc_rsp_ready                ),
            .neigh_sc_rsp_info                 ( `EVL_MAXSHIRE_PATH.neigh_sc_rsp_info                 ),

            .maxion_busy                       ( maxion_busy                                          ),
            .have_activity                     ( have_activity                                        )
         );

      evl_maxion_tracer evl_maxion_tracer (
            .clk_core            ( `EVL_MAXSHIRE_PATH.clk_core            ),
            .reset_cold_max      ( `EVL_MAXSHIRE_PATH.reset_cold_max      ),
            .reset_warm_max_core ( `EVL_MAXSHIRE_PATH.reset_warm_max_core ),
            .maxion_busy         ( maxion_busy                            ),
            .have_activity       ( have_activity                          )
         );

   `else // ifdef EVL_MAXSHIRE_CHECKS }------------------------------------------------------------{

      tri0 [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]   maxion_to_l3_axi_aw_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]   maxion_to_l3_axi_w_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]   maxion_to_l3_axi_ar_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]                          maxion_to_l3_axi_buser;
      tri0 [`MAXSHIRE_SC_NUM_L3_MASTER_PORTS-1:0]                          maxion_to_l3_axi_ruser;
      tri0 [`EVL_BUS_REGION_WIDTH-1:0]                                     maxion_to_l3_axi_ar_region;
      tri0 [`EVL_BUS_REGION_WIDTH-1:0]                                     maxion_to_l3_axi_aw_region;
      tri0                                                                 maxion_to_l3_axi_w_user;

      genvar mesh_idx;
      generate
         for (mesh_idx = 0; mesh_idx < `MAXSHIRE_SC_NUM_L3_MASTER_PORTS; mesh_idx = mesh_idx + 1) begin : g_maxion_to_l3_master_if
            //
            // MAXION L2TOL3 Mesh Interface
            //
            evl_axi_interface #(.SHIRE_ID               (IOSHIRE_ID),
                                .PORT_ID                (mesh_idx),
                                .MAX_PORT_ID            (`MAXSHIRE_SC_NUM_L3_MASTER_PORTS),
                                .PORT_TYPE              (int'(evl_base_pkg::AXI_TO_L3MESH)),
                                .AXI_COV_NAME           ("AXI_SC_64"),
                                .ID_WIDTH               (`SC_MESH_MASTER_AXI_ID_SIZE),
                                .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                                .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                                .AXSIZE_WIDTH           (3),
                                .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                                .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_maxion_to_l3_master_axi (
                  .isolate              (1'b0),
                  .reset                (reset_ios_ckcore),
                  .clk                  (clk_ios),

                  `ifdef ET_AXI_DPI_MONITOR
                     .dpi_capture_enable (1'b0),
                     .dpi_read_clock     (1'b0),
                     .dpi_packet_pop     (1'b0),
                     .dpi_packet_valid   (),
                     .raw_dpi_packet     (),
                  `endif // ifdef ET_AXI_DPI_MONITOR

                  .aw_id                (maxion_to_l3_axi_AW[mesh_idx].ID),
                  .aw_addr              (maxion_to_l3_axi_AW[mesh_idx].ADDR),
                  .aw_len               (maxion_to_l3_axi_AW[mesh_idx].LEN),
                  .aw_size              (maxion_to_l3_axi_AW[mesh_idx].SIZE),
                  .aw_burst             (maxion_to_l3_axi_AW[mesh_idx].BURST),
                  .aw_lock              (maxion_to_l3_axi_AW[mesh_idx].LOCK),
                  .aw_cache             (maxion_to_l3_axi_AW[mesh_idx].CACHE),
                  .aw_prot              (maxion_to_l3_axi_AW[mesh_idx].PROT),
                  .aw_qos               (maxion_to_l3_axi_AW[mesh_idx].QOS),
                  .aw_region            (maxion_to_l3_axi_aw_region),
                  .aw_user              (maxion_to_l3_axi_AW[mesh_idx].USER),
                  .aw_valid             (maxion_to_l3_axi_AWVALID[mesh_idx]),
                  .aw_ready             (maxion_to_l3_axi_AWREADY[mesh_idx]),
                  .aw_vc_valid          (),
                  .aw_vc_credit         (),
                  .aw_trans_id          (maxion_to_l3_axi_aw_trans_id[mesh_idx]),

                  .w_user               (maxion_to_l3_axi_w_user),
                  .w_data               (maxion_to_l3_axi_W[mesh_idx].DATA),
                  .w_strb               (maxion_to_l3_axi_W[mesh_idx].STRB),
                  .w_last               (maxion_to_l3_axi_W[mesh_idx].LAST),
                  .w_valid              (maxion_to_l3_axi_WVALID[mesh_idx]),
                  .w_ready              (maxion_to_l3_axi_WREADY[mesh_idx]),
                  .w_vc_valid           (),
                  .w_trans_id           (maxion_to_l3_axi_w_trans_id[mesh_idx]),

                  .b_user               (maxion_to_l3_axi_buser[mesh_idx]),
                  .b_id                 (maxion_to_l3_axi_B[mesh_idx].ID),
                  .b_resp               (maxion_to_l3_axi_B[mesh_idx].RESP),
                  .b_valid              (maxion_to_l3_axi_BVALID[mesh_idx]),
                  .b_ready              (maxion_to_l3_axi_BREADY[mesh_idx]),
                  .b_trans_id           (),

                  .ar_id                (maxion_to_l3_axi_AR[mesh_idx].ID),
                  .ar_addr              (maxion_to_l3_axi_AR[mesh_idx].ADDR),
                  .ar_len               (maxion_to_l3_axi_AR[mesh_idx].LEN),
                  .ar_size              (maxion_to_l3_axi_AR[mesh_idx].SIZE),
                  .ar_burst             (maxion_to_l3_axi_AR[mesh_idx].BURST),
                  .ar_lock              (maxion_to_l3_axi_AR[mesh_idx].LOCK),
                  .ar_cache             (maxion_to_l3_axi_AR[mesh_idx].CACHE),
                  .ar_prot              (maxion_to_l3_axi_AR[mesh_idx].PROT),
                  .ar_qos               (maxion_to_l3_axi_AR[mesh_idx].QOS),
                  .ar_region            (maxion_to_l3_axi_ar_region),
                  .ar_user              (maxion_to_l3_axi_AR[mesh_idx].USER),
                  .ar_valid             (maxion_to_l3_axi_ARVALID[mesh_idx]),
                  .ar_ready             (maxion_to_l3_axi_ARREADY[mesh_idx]),
                  .ar_vc_valid          (),
                  .ar_vc_credit         (),
                  .ar_trans_id          (maxion_to_l3_axi_ar_trans_id[mesh_idx]),

                  .r_last               (maxion_to_l3_axi_R[mesh_idx].LAST),
                  .r_user               (maxion_to_l3_axi_ruser[mesh_idx]),
                  .r_id                 (maxion_to_l3_axi_R[mesh_idx].ID),
                  .r_data               (maxion_to_l3_axi_R[mesh_idx].DATA),
                  .r_resp               (maxion_to_l3_axi_R[mesh_idx].RESP),
                  .r_valid              (maxion_to_l3_axi_RVALID[mesh_idx]),
                  .r_ready              (maxion_to_l3_axi_RREADY[mesh_idx]),
                  .r_trans_id           ()
               );
         end
      endgenerate

      tri0 [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]                maxion_l3_axi_aw_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]                maxion_l3_axi_w_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0][`DV_TRANS_ID_SIZE-1:0]                maxion_l3_axi_ar_trans_id;
      tri0 [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0]                                       maxion_l3_axi_wuser;
      tri0 [`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS-1:0][`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]  maxion_l3_axi_aruser;
      tri0  [`EVL_BUS_REGION_WIDTH-1:0]                                                maxion_l3_axi_ar_region;
      tri0  [`EVL_BUS_REGION_WIDTH-1:0]                                                maxion_l3_axi_aw_region;

      generate
         for (mesh_idx = 0; mesh_idx < `MAXSHIRE_SC_NUM_L3_SLAVE_PORTS; mesh_idx = mesh_idx + 1) begin : g_maxion_l3_slave_if
            //
            // MAXION L2TOL3 Mesh Interface
            //
            evl_axi_interface #(.SHIRE_ID               (IOSHIRE_ID),
                                .PORT_ID                (mesh_idx),
                                .MAX_PORT_ID            (`MAXSHIRE_SC_NUM_L3_SLAVE_PORTS),
                                .PORT_TYPE              (int'(evl_base_pkg::AXI_FROM_L3MESH)),
                                .AXI_COV_NAME           ("AXI_SC_64"),
                                .ID_WIDTH               (`SC_MESH_SLAVE_AXI_ID_SIZE),
                                .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                                .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                                .AXSIZE_WIDTH           (3),
                                .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                                .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_maxion_l3_slave_axi (
                  .isolate              (1'b0),
                  .reset                (reset_ios_ckcore),
                  .clk                  (clk_ios),

                  `ifdef ET_AXI_DPI_MONITOR
                     .dpi_capture_enable (1'b0),
                     .dpi_read_clock     (1'b0),
                     .dpi_packet_pop     (1'b0),
                     .dpi_packet_valid   (),
                     .raw_dpi_packet     (),
                  `endif // ifdef ET_AXI_DPI_MONITOR

                  .aw_id                (maxion_l3_axi_AW[mesh_idx].ID),
                  .aw_addr              (maxion_l3_axi_AW[mesh_idx].ADDR),
                  .aw_len               (maxion_l3_axi_AW[mesh_idx].LEN),
                  .aw_size              (maxion_l3_axi_AW[mesh_idx].SIZE),
                  .aw_burst             (maxion_l3_axi_AW[mesh_idx].BURST),
                  .aw_lock              (maxion_l3_axi_AW[mesh_idx].LOCK),
                  .aw_cache             (maxion_l3_axi_AW[mesh_idx].CACHE),
                  .aw_prot              (maxion_l3_axi_AW[mesh_idx].PROT),
                  .aw_qos               (maxion_l3_axi_AW[mesh_idx].QOS),
                  .aw_region            (maxion_l3_axi_aw_region),
                  .aw_user              (maxion_l3_axi_AW[mesh_idx].USER),
                  .aw_valid             (maxion_l3_axi_AWVALID[mesh_idx]),
                  .aw_ready             (maxion_l3_axi_AWREADY[mesh_idx]),
                  .aw_vc_valid          (),
                  .aw_vc_credit         (),
                  .aw_trans_id          (maxion_l3_axi_aw_trans_id[mesh_idx]),

                  .w_user               (maxion_l3_axi_wuser[mesh_idx]),
                  .w_data               (maxion_l3_axi_W[mesh_idx].DATA),
                  .w_strb               (maxion_l3_axi_W[mesh_idx].STRB),
                  .w_last               (maxion_l3_axi_W[mesh_idx].LAST),
                  .w_valid              (maxion_l3_axi_WVALID[mesh_idx]),
                  .w_ready              (maxion_l3_axi_WREADY[mesh_idx]),
                  .w_vc_valid           (),
                  .w_trans_id           (maxion_l3_axi_w_trans_id[mesh_idx]),

                  .b_user               (maxion_l3_axi_b_user),
                  .b_id                 (maxion_l3_axi_B[mesh_idx].ID),
                  .b_resp               (maxion_l3_axi_B[mesh_idx].RESP),
                  `ifdef IOSHIRE_WRAPPER_GATESIM
                     .b_valid           (1'b0),
                     .b_ready           (1'b0),
                  `else
                     .b_valid           (maxion_l3_axi_BVALID[mesh_idx]),
                     .b_ready           (maxion_l3_axi_BREADY[mesh_idx]),
                  `endif
                  .b_trans_id           (),

                  .ar_id                (maxion_l3_axi_AR[mesh_idx].ID),
                  .ar_addr              (maxion_l3_axi_AR[mesh_idx].ADDR),
                  .ar_len               (maxion_l3_axi_AR[mesh_idx].LEN),
                  .ar_size              (maxion_l3_axi_AR[mesh_idx].SIZE),
                  .ar_burst             (maxion_l3_axi_AR[mesh_idx].BURST),
                  .ar_lock              (maxion_l3_axi_AR[mesh_idx].LOCK),
                  .ar_cache             (maxion_l3_axi_AR[mesh_idx].CACHE),
                  .ar_prot              (maxion_l3_axi_AR[mesh_idx].PROT),
                  .ar_qos               (maxion_l3_axi_AR[mesh_idx].QOS),
                  .ar_region            (maxion_l3_axi_ar_region),
                  .ar_user              (maxion_l3_axi_aruser[mesh_idx]),
                  `ifdef IOSHIRE_WRAPPER_GATESIM
                     .ar_valid          (1'b0),
                     .ar_ready          (1'b0),
                  `else
                     .ar_valid          (maxion_l3_axi_ARVALID[mesh_idx]),
                     .ar_ready          (maxion_l3_axi_ARREADY[mesh_idx]),
                  `endif
                  .ar_vc_valid          (),
                  .ar_vc_credit         (),
                  .ar_trans_id          (maxion_l3_axi_ar_trans_id[mesh_idx]),

                  .r_last               (maxion_l3_axi_R[mesh_idx].LAST),
                  .r_user               (maxion_l3_axi_r_user),
                  .r_id                 (maxion_l3_axi_R[mesh_idx].ID),
                  .r_data               (maxion_l3_axi_R[mesh_idx].DATA),
                  .r_resp               (maxion_l3_axi_R[mesh_idx].RESP),
                  `ifdef IOSHIRE_WRAPPER_GATESIM
                     .r_valid           (1'b0),
                     .r_ready           (1'b0),
                  `else
                     .r_valid           (maxion_l3_axi_RVALID[mesh_idx]),
                     .r_ready           (maxion_l3_axi_RREADY[mesh_idx]),
                  `endif
                  .r_trans_id           ()
               );
         end
      endgenerate

      //
      // MAXION UCTOL3 AXI Interface
      //

      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_l3_axi_aw_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_l3_axi_w_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_l3_axi_b_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_l3_axi_ar_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_l3_axi_r_trans_id;
      tri0                             maxion_uc_to_l3_axi_ar_region;
      tri0                             maxion_uc_to_l3_axi_aw_region;
      tri0                             maxion_uc_to_l3_axi_aw_user;
      tri0                             maxion_uc_to_l3_axi_w_user;
      tri0                             maxion_uc_to_l3_axi_ar_user;
      tri0                             maxion_uc_to_l3_axi_b_user;
      tri0                             maxion_uc_to_l3_axi_r_user;

      evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                          .ID_WIDTH       (9),
                          .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH   (3),
                          .AXLEN_WIDTH    (8),
                          .ARUSER_WIDTH   (1),
                          .AWUSER_WIDTH   (5),
                          .AXREGION_WIDTH (1),
                          .AXQOS_WIDTH    (4),
                          .AW_VC_WIDTH    (1),
                          .AW_VC_CREDITS  (0)) m_maxion_uc_to_l3_master_axi (
            .isolate      (1'b0),
            .reset        (reset_ios_ckcore),
            .clk          (clk_ios),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (maxion_uc_to_l3_axi_AW.ID),
            .aw_addr      (maxion_uc_to_l3_axi_AW.ADDR),
            .aw_len       (maxion_uc_to_l3_axi_AW.LEN),
            .aw_size      (maxion_uc_to_l3_axi_AW.SIZE),
            .aw_burst     (maxion_uc_to_l3_axi_AW.BURST),
            .aw_lock      (maxion_uc_to_l3_axi_AW.LOCK),
            .aw_cache     (maxion_uc_to_l3_axi_AW.CACHE),
            .aw_prot      (maxion_uc_to_l3_axi_AW.PROT),
            .aw_qos       (maxion_uc_to_l3_axi_AW.QOS),
            .aw_region    (maxion_uc_to_l3_axi_aw_region),
            .aw_user      (maxion_uc_to_l3_axi_AW.USER),
            .aw_valid     (maxion_uc_to_l3_axi_AWVALID),
            .aw_ready     (maxion_uc_to_l3_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (maxion_uc_to_l3_axi_aw_trans_id),

            .w_user       (maxion_uc_to_l3_axi_w_user),
            .w_data       (maxion_uc_to_l3_axi_W.DATA),
            .w_strb       (maxion_uc_to_l3_axi_W.STRB),
            .w_last       (maxion_uc_to_l3_axi_W.LAST),
            .w_valid      (maxion_uc_to_l3_axi_WVALID),
            .w_ready      (maxion_uc_to_l3_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (maxion_uc_to_l3_axi_w_trans_id),

            .b_user       (maxion_uc_to_l3_axi_b_user),
            .b_id         (maxion_uc_to_l3_axi_B.ID),
            .b_resp       (maxion_uc_to_l3_axi_B.RESP),
            .b_valid      (maxion_uc_to_l3_axi_BVALID),
            .b_ready      (maxion_uc_to_l3_axi_BREADY),
            .b_trans_id   (maxion_uc_to_l3_axi_b_trans_id),

            .ar_id        (maxion_uc_to_l3_axi_AR.ID),
            .ar_addr      (maxion_uc_to_l3_axi_AR.ADDR),
            .ar_len       (maxion_uc_to_l3_axi_AR.LEN),
            .ar_size      (maxion_uc_to_l3_axi_AR.SIZE),
            .ar_burst     (maxion_uc_to_l3_axi_AR.BURST),
            .ar_lock      (maxion_uc_to_l3_axi_AR.LOCK),
            .ar_cache     (maxion_uc_to_l3_axi_AR.CACHE),
            .ar_prot      (maxion_uc_to_l3_axi_AR.PROT),
            .ar_qos       (maxion_uc_to_l3_axi_AR.QOS),
            .ar_region    (maxion_uc_to_l3_axi_ar_region),
            .ar_user      (maxion_uc_to_l3_axi_AR.USER),
            .ar_valid     (maxion_uc_to_l3_axi_ARVALID),
            .ar_ready     (maxion_uc_to_l3_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (maxion_uc_to_l3_axi_ar_trans_id),

            .r_last       (maxion_uc_to_l3_axi_R.LAST),
            .r_user       (maxion_uc_to_l3_axi_r_user),
            .r_id         (maxion_uc_to_l3_axi_R.ID),
            .r_data       (maxion_uc_to_l3_axi_R.DATA),
            .r_resp       (maxion_uc_to_l3_axi_R.RESP),
            .r_valid      (maxion_uc_to_l3_axi_RVALID),
            .r_ready      (maxion_uc_to_l3_axi_RREADY),
            .r_trans_id   (maxion_uc_to_l3_axi_r_trans_id)
         );

      //
      // MAXION TOSYS AXI Interface
      //

      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_to_sys_axi_aw_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_to_sys_axi_w_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_to_sys_axi_b_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_to_sys_axi_ar_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_to_sys_axi_r_trans_id;
      tri0                             maxion_to_sys_axi_ar_region;
      tri0                             maxion_to_sys_axi_aw_region;
      tri0                             maxion_to_sys_axi_aw_user;
      tri0                             maxion_to_sys_axi_w_user;
      tri0                             maxion_to_sys_axi_ar_user;
      tri0                             maxion_to_sys_axi_b_user;
      tri0                             maxion_to_sys_axi_r_user;

      evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                          .ID_WIDTH       (9),
                          .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH   (3),
                          .AXLEN_WIDTH    (8),
                          .ARUSER_WIDTH   (1),
                          .AWUSER_WIDTH   (5),
                          .AXREGION_WIDTH (1),
                          .AXQOS_WIDTH    (4),
                          .AW_VC_WIDTH    (1),
                          .AW_VC_CREDITS  (0)) m_maxion_to_sys_master_axi (
            .isolate      (1'b0),
            .reset        (reset_ios_ckcore),
            .clk          (clk_ios),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (maxion_to_sys_axi_AW.ID),
            .aw_addr      (maxion_to_sys_axi_AW.ADDR),
            .aw_len       (maxion_to_sys_axi_AW.LEN),
            .aw_size      (maxion_to_sys_axi_AW.SIZE),
            .aw_burst     (maxion_to_sys_axi_AW.BURST),
            .aw_lock      (maxion_to_sys_axi_AW.LOCK),
            .aw_cache     (maxion_to_sys_axi_AW.CACHE),
            .aw_prot      (maxion_to_sys_axi_AW.PROT),
            .aw_qos       (maxion_to_sys_axi_AW.QOS),
            .aw_region    (maxion_to_sys_axi_aw_region),
            .aw_user      (maxion_to_sys_axi_AW.USER),
            .aw_valid     (maxion_to_sys_axi_AWVALID),
            .aw_ready     (maxion_to_sys_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (maxion_to_sys_axi_aw_trans_id),

            .w_user       (maxion_to_sys_axi_w_user),
            .w_data       (maxion_to_sys_axi_W.DATA),
            .w_strb       (maxion_to_sys_axi_W.STRB),
            .w_last       (maxion_to_sys_axi_W.LAST),
            .w_valid      (maxion_to_sys_axi_WVALID),
            .w_ready      (maxion_to_sys_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (maxion_to_sys_axi_w_trans_id),

            .b_user       (maxion_to_sys_axi_b_user),
            .b_id         (maxion_to_sys_axi_B.ID),
            .b_resp       (maxion_to_sys_axi_B.RESP),
            .b_valid      (maxion_to_sys_axi_BVALID),
            .b_ready      (maxion_to_sys_axi_BREADY),
            .b_trans_id   (maxion_to_sys_axi_b_trans_id),

            .ar_id        (maxion_to_sys_axi_AR.ID),
            .ar_addr      (maxion_to_sys_axi_AR.ADDR),
            .ar_len       (maxion_to_sys_axi_AR.LEN),
            .ar_size      (maxion_to_sys_axi_AR.SIZE),
            .ar_burst     (maxion_to_sys_axi_AR.BURST),
            .ar_lock      (maxion_to_sys_axi_AR.LOCK),
            .ar_cache     (maxion_to_sys_axi_AR.CACHE),
            .ar_prot      (maxion_to_sys_axi_AR.PROT),
            .ar_qos       (maxion_to_sys_axi_AR.QOS),
            .ar_region    (maxion_to_sys_axi_ar_region),
            .ar_user      (maxion_to_sys_axi_ar_user),
            .ar_valid     (maxion_to_sys_axi_ARVALID),
            .ar_ready     (maxion_to_sys_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (maxion_to_sys_axi_ar_trans_id),

            .r_last       (maxion_to_sys_axi_R.LAST),
            .r_user       (maxion_to_sys_axi_r_user),
            .r_id         (maxion_to_sys_axi_R.ID),
            .r_data       (maxion_to_sys_axi_R.DATA),
            .r_resp       (maxion_to_sys_axi_R.RESP),
            .r_valid      (maxion_to_sys_axi_RVALID),
            .r_ready      (maxion_to_sys_axi_RREADY),
            .r_trans_id   (maxion_to_sys_axi_r_trans_id)
         );

      //
      // MAXION UCTOSYS AXI Interface
      //

      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_sys_axi_aw_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_sys_axi_w_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_sys_axi_b_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_sys_axi_ar_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_uc_to_sys_axi_r_trans_id;
      tri0                             maxion_uc_to_sys_axi_ar_region;
      tri0                             maxion_uc_to_sys_axi_aw_region;
      tri0                             maxion_uc_to_sys_axi_aw_user;
      tri0                             maxion_uc_to_sys_axi_w_user;
      tri0                             maxion_uc_to_sys_axi_ar_user;
      tri0                             maxion_uc_to_sys_axi_b_user;
      tri0                             maxion_uc_to_sys_axi_r_user;

      evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                          .ID_WIDTH       (9),
                          .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH   (3),
                          .AXLEN_WIDTH    (8),
                          .ARUSER_WIDTH   (1),
                          .AWUSER_WIDTH   (5),
                          .AXREGION_WIDTH (1),
                          .AXQOS_WIDTH    (4),
                          .AW_VC_WIDTH    (1),
                          .AW_VC_CREDITS  (0)) m_maxion_uc_to_sys_master_axi (
            .isolate      (1'b0),
            .reset        (reset_ios_ckcore),
            .clk          (clk_ios),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (maxion_uc_to_sys_axi_AW.ID),
            .aw_addr      (maxion_uc_to_sys_axi_AW.ADDR),
            .aw_len       (maxion_uc_to_sys_axi_AW.LEN),
            .aw_size      (maxion_uc_to_sys_axi_AW.SIZE),
            .aw_burst     (maxion_uc_to_sys_axi_AW.BURST),
            .aw_lock      (maxion_uc_to_sys_axi_AW.LOCK),
            .aw_cache     (maxion_uc_to_sys_axi_AW.CACHE),
            .aw_prot      (maxion_uc_to_sys_axi_AW.PROT),
            .aw_qos       (maxion_uc_to_sys_axi_AW.QOS),
            .aw_region    (maxion_uc_to_sys_axi_aw_region),
            .aw_user      (maxion_uc_to_sys_axi_AW.USER),
            .aw_valid     (maxion_uc_to_sys_axi_AWVALID),
            .aw_ready     (maxion_uc_to_sys_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (maxion_uc_to_sys_axi_aw_trans_id),

            .w_user       (maxion_uc_to_sys_axi_w_user),
            .w_data       (maxion_uc_to_sys_axi_W.DATA),
            .w_strb       (maxion_uc_to_sys_axi_W.STRB),
            .w_last       (maxion_uc_to_sys_axi_W.LAST),
            .w_valid      (maxion_uc_to_sys_axi_WVALID),
            .w_ready      (maxion_uc_to_sys_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (maxion_uc_to_sys_axi_w_trans_id),

            .b_user       (maxion_uc_to_sys_axi_b_user),
            .b_id         (maxion_uc_to_sys_axi_B.ID),
            .b_resp       (maxion_uc_to_sys_axi_B.RESP),
            .b_valid      (maxion_uc_to_sys_axi_BVALID),
            .b_ready      (maxion_uc_to_sys_axi_BREADY),
            .b_trans_id   (maxion_uc_to_sys_axi_b_trans_id),

            .ar_id        (maxion_uc_to_sys_axi_AR.ID),
            .ar_addr      (maxion_uc_to_sys_axi_AR.ADDR),
            .ar_len       (maxion_uc_to_sys_axi_AR.LEN),
            .ar_size      (maxion_uc_to_sys_axi_AR.SIZE),
            .ar_burst     (maxion_uc_to_sys_axi_AR.BURST),
            .ar_lock      (maxion_uc_to_sys_axi_AR.LOCK),
            .ar_cache     (maxion_uc_to_sys_axi_AR.CACHE),
            .ar_prot      (maxion_uc_to_sys_axi_AR.PROT),
            .ar_qos       (maxion_uc_to_sys_axi_AR.QOS),
            .ar_region    (maxion_uc_to_sys_axi_ar_region),
            .ar_user      (maxion_uc_to_sys_axi_ar_user),
            .ar_valid     (maxion_uc_to_sys_axi_ARVALID),
            .ar_ready     (maxion_uc_to_sys_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (maxion_uc_to_sys_axi_ar_trans_id),

            .r_last       (maxion_uc_to_sys_axi_R.LAST),
            .r_user       (maxion_uc_to_sys_axi_r_user),
            .r_id         (maxion_uc_to_sys_axi_R.ID),
            .r_data       (maxion_uc_to_sys_axi_R.DATA),
            .r_resp       (maxion_uc_to_sys_axi_R.RESP),
            .r_valid      (maxion_uc_to_sys_axi_RVALID),
            .r_ready      (maxion_uc_to_sys_axi_RREADY),
            .r_trans_id   (maxion_uc_to_sys_axi_r_trans_id)
         );

      //
      // MAXION SYS Slave AXI Interface
      //

      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_sys_axi_aw_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_sys_axi_w_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_sys_axi_b_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_sys_axi_ar_trans_id;
      tri0 [`DV_TRANS_ID_SIZE-1:0]     maxion_sys_axi_r_trans_id;
      tri0                             maxion_sys_axi_ar_region;
      tri0                             maxion_sys_axi_aw_region;
      tri0                             maxion_sys_axi_aw_user;
      tri0                             maxion_sys_axi_w_user;
      tri0                             maxion_sys_axi_ar_user;
      tri0                             maxion_sys_axi_b_user;
      tri0                             maxion_sys_axi_r_user;

      evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                          .ID_WIDTH       (`SC_MESH_SLAVE_AXI_ID_SIZE),
                          .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH     (`SYS_MESH_SLAVE_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH   (3),
                          .AXLEN_WIDTH    (8),
                          .ARUSER_WIDTH   (1),
                          .AWUSER_WIDTH   (1),
                          .AXREGION_WIDTH (1),
                          .AXQOS_WIDTH    (4),
                          .AW_VC_WIDTH    (2),
                          .AW_VC_CREDITS  (8)) m_maxion_sys_slave_axi (
            .isolate      (1'b0),
            .reset        (reset_ios_ckcore),
            .clk          (clk_ios),

            `ifdef ET_AXI_DPI_MONITOR
               .dpi_capture_enable (1'b0),
               .dpi_read_clock     (1'b0),
               .dpi_packet_pop     (1'b0),
               .dpi_packet_valid   (),
               .raw_dpi_packet     (),
            `endif // ifdef ET_AXI_DPI_MONITOR

            .aw_id        (maxion_sys_axi_AW.ID),
            .aw_addr      (maxion_sys_axi_AW.ADDR),
            .aw_len       (maxion_sys_axi_AW.LEN),
            .aw_size      (maxion_sys_axi_AW.SIZE),
            .aw_burst     (maxion_sys_axi_AW.BURST),
            .aw_lock      (maxion_sys_axi_AW.LOCK),
            .aw_cache     (maxion_sys_axi_AW.CACHE),
            .aw_prot      (maxion_sys_axi_AW.PROT),
            .aw_qos       (maxion_sys_axi_AW.QOS),
            .aw_region    (maxion_sys_axi_aw_region),
            .aw_user      (maxion_sys_axi_aw_user),
            .aw_valid     (maxion_sys_axi_AWVALID),
            .aw_ready     (maxion_sys_axi_AWREADY),
            .aw_vc_valid  (maxion_sys_axi_AWvcvalid),
            .aw_vc_credit (maxion_sys_axi_AWcredit),
            .aw_trans_id  (maxion_sys_axi_aw_trans_id),

            .w_user       (maxion_sys_axi_w_user),
            .w_data       (maxion_sys_axi_W.DATA),
            .w_strb       (maxion_sys_axi_W.STRB),
            .w_last       (maxion_sys_axi_W.LAST),
            .w_valid      (maxion_sys_axi_WVALID),
            .w_ready      (maxion_sys_axi_WREADY),
            .w_vc_valid   (maxion_sys_axi_Wvcvalid),
            .w_trans_id   (maxion_sys_axi_w_trans_id),

            .b_user       (maxion_sys_axi_b_user),
            .b_id         (maxion_sys_axi_B.ID),
            .b_resp       (maxion_sys_axi_B.RESP),
            .b_valid      (maxion_sys_axi_BVALID),
            .b_ready      (maxion_sys_axi_BREADY),
            .b_trans_id   (maxion_sys_axi_b_trans_id),

            .ar_id        (maxion_sys_axi_AR.ID),
            .ar_addr      (maxion_sys_axi_AR.ADDR),
            .ar_len       (maxion_sys_axi_AR.LEN),
            .ar_size      (maxion_sys_axi_AR.SIZE),
            .ar_burst     (maxion_sys_axi_AR.BURST),
            .ar_lock      (maxion_sys_axi_AR.LOCK),
            .ar_cache     (maxion_sys_axi_AR.CACHE),
            .ar_prot      (maxion_sys_axi_AR.PROT),
            .ar_qos       (maxion_sys_axi_AR.QOS),
            .ar_region    (maxion_sys_axi_ar_region),
            .ar_user      (maxion_sys_axi_ar_user),
            .ar_valid     (maxion_sys_axi_ARVALID),
            .ar_ready     (maxion_sys_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (maxion_sys_axi_ar_trans_id),

            .r_last       (maxion_sys_axi_R.LAST),
            .r_user       (maxion_sys_axi_r_user),
            .r_id         (maxion_sys_axi_R.ID),
            .r_data       (maxion_sys_axi_R.DATA),
            .r_resp       (maxion_sys_axi_R.RESP),
            .r_valid      (maxion_sys_axi_RVALID),
            .r_ready      (maxion_sys_axi_RREADY),
            .r_trans_id   (maxion_sys_axi_r_trans_id)
         );

      wire unused_ok = &{ 1'b0,
                          clk_maxshire_uncore,
                          clk_maxshire_core,
                          1'b0 };

   `endif // !ifdef EVL_MAXSHIRE_CHECKS -----------------------------------------------------------}

   //
   // PU_NOC to PU AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_noc_pu_scram_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_noc_pu_scram_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_noc_pu_scram_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_noc_pu_scram_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_noc_pu_scram_axi_r_trans_id;
   tri0                             pu_noc_pu_scram_axi_aw_region;
   tri0                             pu_noc_pu_scram_axi_ar_region;
   tri0                             pu_noc_pu_scram_axi_aw_user;
   tri0                             pu_noc_pu_scram_axi_w_user;
   tri0                             pu_noc_pu_scram_axi_ar_user;
   tri0                             pu_noc_pu_scram_axi_b_user;
   tri0                             pu_noc_pu_scram_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS(1),
                       .AW_VC_CREDITS  (0)) m_pu_noc_pu_scram_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_noc_pu_scram_axi_AWID),
         .aw_addr      (pu_noc_pu_scram_axi_AWADDR),
         .aw_len       (pu_noc_pu_scram_axi_AWLEN),
         .aw_size      (pu_noc_pu_scram_axi_AWSIZE),
         .aw_burst     (pu_noc_pu_scram_axi_AWBURST),
         .aw_lock      (pu_noc_pu_scram_axi_AWLOCK),
         .aw_cache     (pu_noc_pu_scram_axi_AWCACHE),
         .aw_prot      (pu_noc_pu_scram_axi_AWPROT),
         .aw_qos       (pu_noc_pu_scram_axi_AWQOS),
         .aw_region    (pu_noc_pu_scram_axi_aw_region),
         .aw_user      (pu_noc_pu_scram_axi_aw_user),
         .aw_valid     (pu_noc_pu_scram_axi_AWVALID),
         .aw_ready     (pu_noc_pu_scram_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_noc_pu_scram_axi_aw_trans_id),

         .w_user       (pu_noc_pu_scram_axi_w_user),
         .w_data       (pu_noc_pu_scram_axi_WDATA),
         .w_strb       (pu_noc_pu_scram_axi_WSTRB),
         .w_last       (pu_noc_pu_scram_axi_WLAST),
         .w_valid      (pu_noc_pu_scram_axi_WVALID),
         .w_ready      (pu_noc_pu_scram_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_noc_pu_scram_axi_w_trans_id),

         .b_user       (pu_noc_pu_scram_axi_b_user),
         .b_id         (pu_noc_pu_scram_axi_BID),
         .b_resp       (pu_noc_pu_scram_axi_BRESP),
         .b_valid      (pu_noc_pu_scram_axi_BVALID),
         .b_ready      (pu_noc_pu_scram_axi_BREADY),
         .b_trans_id   (pu_noc_pu_scram_axi_b_trans_id),

         .ar_id        (pu_noc_pu_scram_axi_ARID),
         .ar_addr      (pu_noc_pu_scram_axi_ARADDR),
         .ar_len       (pu_noc_pu_scram_axi_ARLEN),
         .ar_size      (pu_noc_pu_scram_axi_ARSIZE),
         .ar_burst     (pu_noc_pu_scram_axi_ARBURST),
         .ar_lock      (pu_noc_pu_scram_axi_ARLOCK),
         .ar_cache     (pu_noc_pu_scram_axi_ARCACHE),
         .ar_prot      (pu_noc_pu_scram_axi_ARPROT),
         .ar_qos       (pu_noc_pu_scram_axi_ARQOS),
         .ar_region    (pu_noc_pu_scram_axi_ar_region),
         .ar_user      (pu_noc_pu_scram_axi_ar_user),
         .ar_valid     (pu_noc_pu_scram_axi_ARVALID),
         .ar_ready     (pu_noc_pu_scram_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_noc_pu_scram_axi_ar_trans_id),

         .r_last       (pu_noc_pu_scram_axi_RLAST),
         .r_user       (pu_noc_pu_scram_axi_r_user),
         .r_id         (pu_noc_pu_scram_axi_RID),
         .r_data       (pu_noc_pu_scram_axi_RDATA),
         .r_resp       (pu_noc_pu_scram_axi_RRESP),
         .r_valid      (pu_noc_pu_scram_axi_RVALID),
         .r_ready      (pu_noc_pu_scram_axi_RREADY),
         .r_trans_id   (pu_noc_pu_scram_axi_r_trans_id)
      );

   //
   // PU_NOC to PU AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_pu_noc_axi_r_trans_id;
   tri0                             pu_dma_pu_noc_axi_aw_region;
   tri0                             pu_dma_pu_noc_axi_ar_region;
   tri0                             pu_dma_pu_noc_axi_aw_user;
   tri0                             pu_dma_pu_noc_axi_w_user;
   tri0                             pu_dma_pu_noc_axi_ar_user;
   tri0                             pu_dma_pu_noc_axi_b_user;
   tri0                             pu_dma_pu_noc_axi_r_user;
   tri0                             pu_dma_pu_noc_axi_awqos;
   tri0                             pu_dma_pu_noc_axi_arqos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (4),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (1),
                       .AXLOCK_WIDTH   (2),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_pu_dma_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios_ahb),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_dma_pu_noc_axi_AWID),
         .aw_addr      (pu_dma_pu_noc_axi_AWADDR),
         .aw_len       (pu_dma_pu_noc_axi_AWLEN),
         .aw_size      (pu_dma_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_dma_pu_noc_axi_AWBURST),
         .aw_lock      (pu_dma_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_dma_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_dma_pu_noc_axi_AWPROT),
         .aw_qos       (pu_dma_pu_noc_axi_awqos),
         .aw_region    (pu_dma_pu_noc_axi_aw_region),
         .aw_user      (pu_dma_pu_noc_axi_aw_user),
         .aw_valid     (pu_dma_pu_noc_axi_AWVALID),
         .aw_ready     (pu_dma_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_dma_pu_noc_axi_aw_trans_id),

         .w_user       (pu_dma_pu_noc_axi_w_user),
         .w_data       (pu_dma_pu_noc_axi_WDATA),
         .w_strb       (pu_dma_pu_noc_axi_WSTRB),
         .w_last       (pu_dma_pu_noc_axi_WLAST),
         .w_valid      (pu_dma_pu_noc_axi_WVALID),
         .w_ready      (pu_dma_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_dma_pu_noc_axi_w_trans_id),

         .b_user       (pu_dma_pu_noc_axi_b_user),
         .b_id         (pu_dma_pu_noc_axi_BID),
         .b_resp       (pu_dma_pu_noc_axi_BRESP),
         .b_valid      (pu_dma_pu_noc_axi_BVALID),
         .b_ready      (pu_dma_pu_noc_axi_BREADY),
         .b_trans_id   (pu_dma_pu_noc_axi_b_trans_id),

         .ar_id        (pu_dma_pu_noc_axi_ARID),
         .ar_addr      (pu_dma_pu_noc_axi_ARADDR),
         .ar_len       (pu_dma_pu_noc_axi_ARLEN),
         .ar_size      (pu_dma_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_dma_pu_noc_axi_ARBURST),
         .ar_lock      (pu_dma_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_dma_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_dma_pu_noc_axi_ARPROT),
         .ar_qos       (pu_dma_pu_noc_axi_arqos),
         .ar_region    (pu_dma_pu_noc_axi_ar_region),
         .ar_user      (pu_dma_pu_noc_axi_ar_user),
         .ar_valid     (pu_dma_pu_noc_axi_ARVALID),
         .ar_ready     (pu_dma_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_dma_pu_noc_axi_ar_trans_id),

         .r_last       (pu_dma_pu_noc_axi_RLAST),
         .r_user       (pu_dma_pu_noc_axi_r_user),
         .r_id         (pu_dma_pu_noc_axi_RID),
         .r_data       (pu_dma_pu_noc_axi_RDATA),
         .r_resp       (pu_dma_pu_noc_axi_RRESP),
         .r_valid      (pu_dma_pu_noc_axi_RVALID),
         .r_ready      (pu_dma_pu_noc_axi_RREADY),
         .r_trans_id   (pu_dma_pu_noc_axi_r_trans_id)
      );

   //
   // PU_NOC to PU AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_am_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_am_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_am_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_am_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_dma_am_pu_noc_axi_r_trans_id;
   tri0                             pu_dma_am_pu_noc_axi_aw_region;
   tri0                             pu_dma_am_pu_noc_axi_ar_region;
   tri0                             pu_dma_am_pu_noc_axi_aw_user;
   tri0                             pu_dma_am_pu_noc_axi_w_user;
   tri0                             pu_dma_am_pu_noc_axi_ar_user;
   tri0                             pu_dma_am_pu_noc_axi_b_user;
   tri0                             pu_dma_am_pu_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (5),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (64),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_pu_dma_am_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_dma_am_pu_noc_axi_AWID),
         .aw_addr      (pu_dma_am_pu_noc_axi_AWADDR),
         .aw_len       (pu_dma_am_pu_noc_axi_AWLEN),
         .aw_size      (pu_dma_am_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_dma_am_pu_noc_axi_AWBURST),
         .aw_lock      (pu_dma_am_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_dma_am_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_dma_am_pu_noc_axi_AWPROT),
         .aw_qos       (pu_dma_am_pu_noc_axi_AWQOS),
         .aw_region    (pu_dma_am_pu_noc_axi_aw_region),
         .aw_user      (pu_dma_am_pu_noc_axi_aw_user),
         .aw_valid     (pu_dma_am_pu_noc_axi_AWVALID),
         .aw_ready     (pu_dma_am_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_dma_am_pu_noc_axi_aw_trans_id),

         .w_user       (pu_dma_am_pu_noc_axi_w_user),
         .w_data       (pu_dma_am_pu_noc_axi_WDATA),
         .w_strb       (pu_dma_am_pu_noc_axi_WSTRB),
         .w_last       (pu_dma_am_pu_noc_axi_WLAST),
         .w_valid      (pu_dma_am_pu_noc_axi_WVALID),
         .w_ready      (pu_dma_am_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_dma_am_pu_noc_axi_w_trans_id),

         .b_user       (pu_dma_am_pu_noc_axi_b_user),
         .b_id         (pu_dma_am_pu_noc_axi_BID),
         .b_resp       (pu_dma_am_pu_noc_axi_BRESP),
         .b_valid      (pu_dma_am_pu_noc_axi_BVALID),
         .b_ready      (pu_dma_am_pu_noc_axi_BREADY),
         .b_trans_id   (pu_dma_am_pu_noc_axi_b_trans_id),

         .ar_id        (pu_dma_am_pu_noc_axi_ARID),
         .ar_addr      (pu_dma_am_pu_noc_axi_ARADDR),
         .ar_len       (pu_dma_am_pu_noc_axi_ARLEN),
         .ar_size      (pu_dma_am_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_dma_am_pu_noc_axi_ARBURST),
         .ar_lock      (pu_dma_am_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_dma_am_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_dma_am_pu_noc_axi_ARPROT),
         .ar_qos       (pu_dma_am_pu_noc_axi_ARQOS),
         .ar_region    (pu_dma_am_pu_noc_axi_ar_region),
         .ar_user      (pu_dma_am_pu_noc_axi_ar_user),
         .ar_valid     (pu_dma_am_pu_noc_axi_ARVALID),
         .ar_ready     (pu_dma_am_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_dma_am_pu_noc_axi_ar_trans_id),

         .r_last       (pu_dma_am_pu_noc_axi_RLAST),
         .r_user       (pu_dma_am_pu_noc_axi_r_user),
         .r_id         (pu_dma_am_pu_noc_axi_RID),
         .r_data       (pu_dma_am_pu_noc_axi_RDATA),
         .r_resp       (pu_dma_am_pu_noc_axi_RRESP),
         .r_valid      (pu_dma_am_pu_noc_axi_RVALID),
         .r_ready      (pu_dma_am_pu_noc_axi_RREADY),
         .r_trans_id   (pu_dma_am_pu_noc_axi_r_trans_id)
      );

   //
   // PU_NOC to PU AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_emmc_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_emmc_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_emmc_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_emmc_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_emmc_pu_noc_axi_r_trans_id;
   tri0                             pu_emmc_pu_noc_axi_aw_region;
   tri0                             pu_emmc_pu_noc_axi_ar_region;
   tri0                             pu_emmc_pu_noc_axi_aw_user;
   tri0                             pu_emmc_pu_noc_axi_w_user;
   tri0                             pu_emmc_pu_noc_axi_ar_user;
   tri0                             pu_emmc_pu_noc_axi_b_user;
   tri0                             pu_emmc_pu_noc_axi_r_user;
   tri0                             pu_emmc_pu_noc_axi_awqos;
   tri0                             pu_emmc_pu_noc_axi_arqos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (64),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (4),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (1),
                       .AXLOCK_WIDTH   (2),
                       .AW_VC_WIDTH    (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS  (0)) m_pu_emmc_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios_ahb),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_emmc_pu_noc_axi_AWID),
         .aw_addr      (pu_emmc_pu_noc_axi_AWADDR),
         .aw_len       (pu_emmc_pu_noc_axi_AWLEN),
         .aw_size      (pu_emmc_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_emmc_pu_noc_axi_AWBURST),
         .aw_lock      (pu_emmc_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_emmc_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_emmc_pu_noc_axi_AWPROT),
         .aw_qos       (pu_emmc_pu_noc_axi_awqos),
         .aw_region    (pu_emmc_pu_noc_axi_aw_region),
         .aw_user      (pu_emmc_pu_noc_axi_aw_user),
         .aw_valid     (pu_emmc_pu_noc_axi_AWVALID),
         .aw_ready     (pu_emmc_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_emmc_pu_noc_axi_aw_trans_id),

         .w_user       (pu_emmc_pu_noc_axi_w_user),
         .w_data       (pu_emmc_pu_noc_axi_WDATA),
         .w_strb       (pu_emmc_pu_noc_axi_WSTRB),
         .w_last       (pu_emmc_pu_noc_axi_WLAST),
         .w_valid      (pu_emmc_pu_noc_axi_WVALID),
         .w_ready      (pu_emmc_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_emmc_pu_noc_axi_w_trans_id),

         .b_user       (pu_emmc_pu_noc_axi_b_user),
         .b_id         (pu_emmc_pu_noc_axi_BID),
         .b_resp       (pu_emmc_pu_noc_axi_BRESP),
         .b_valid      (pu_emmc_pu_noc_axi_BVALID),
         .b_ready      (pu_emmc_pu_noc_axi_BREADY),
         .b_trans_id   (pu_emmc_pu_noc_axi_b_trans_id),

         .ar_id        (pu_emmc_pu_noc_axi_ARID),
         .ar_addr      (pu_emmc_pu_noc_axi_ARADDR),
         .ar_len       (pu_emmc_pu_noc_axi_ARLEN),
         .ar_size      (pu_emmc_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_emmc_pu_noc_axi_ARBURST),
         .ar_lock      (pu_emmc_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_emmc_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_emmc_pu_noc_axi_ARPROT),
         .ar_qos       (pu_emmc_pu_noc_axi_arqos),
         .ar_region    (pu_emmc_pu_noc_axi_ar_region),
         .ar_user      (pu_emmc_pu_noc_axi_ar_user),
         .ar_valid     (pu_emmc_pu_noc_axi_ARVALID),
         .ar_ready     (pu_emmc_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_emmc_pu_noc_axi_ar_trans_id),

         .r_last       (pu_emmc_pu_noc_axi_RLAST),
         .r_user       (pu_emmc_pu_noc_axi_r_user),
         .r_id         (pu_emmc_pu_noc_axi_RID),
         .r_data       (pu_emmc_pu_noc_axi_RDATA),
         .r_resp       (pu_emmc_pu_noc_axi_RRESP),
         .r_valid      (pu_emmc_pu_noc_axi_RVALID),
         .r_ready      (pu_emmc_pu_noc_axi_RREADY),
         .r_trans_id   (pu_emmc_pu_noc_axi_r_trans_id)
      );

   //
   // USB0 to PU_NOC AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb0_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb0_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb0_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb0_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb0_pu_noc_axi_r_trans_id;
   tri0                             pu_usb0_pu_noc_axi_aw_region;
   tri0                             pu_usb0_pu_noc_axi_ar_region;
   tri0                             pu_usb0_pu_noc_axi_aw_user;
   tri0                             pu_usb0_pu_noc_axi_w_user;
   tri0                             pu_usb0_pu_noc_axi_ar_user;
   tri0                             pu_usb0_pu_noc_axi_b_user;
   tri0                             pu_usb0_pu_noc_axi_r_user;
   tri0                             pu_usb0_pu_noc_axi_awqos;
   tri0                             pu_usb0_pu_noc_axi_arqos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (4),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (1),
                       .AXLOCK_WIDTH   (2),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_pu_usb0_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios_ahb),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_usb0_pu_noc_axi_AWID),
         .aw_addr      (pu_usb0_pu_noc_axi_AWADDR),
         .aw_len       (pu_usb0_pu_noc_axi_AWLEN),
         .aw_size      (pu_usb0_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_usb0_pu_noc_axi_AWBURST),
         .aw_lock      (pu_usb0_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_usb0_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_usb0_pu_noc_axi_AWPROT),
         .aw_qos       (pu_usb0_pu_noc_axi_awqos),
         .aw_region    (pu_usb0_pu_noc_axi_aw_region),
         .aw_user      (pu_usb0_pu_noc_axi_aw_user),
         .aw_valid     (pu_usb0_pu_noc_axi_AWVALID),
         .aw_ready     (pu_usb0_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_usb0_pu_noc_axi_aw_trans_id),

         .w_user       (pu_usb0_pu_noc_axi_w_user),
         .w_data       (pu_usb0_pu_noc_axi_WDATA),
         .w_strb       (pu_usb0_pu_noc_axi_WSTRB),
         .w_last       (pu_usb0_pu_noc_axi_WLAST),
         .w_valid      (pu_usb0_pu_noc_axi_WVALID),
         .w_ready      (pu_usb0_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_usb0_pu_noc_axi_w_trans_id),

         .b_user       (pu_usb0_pu_noc_axi_b_user),
         .b_id         (pu_usb0_pu_noc_axi_BID),
         .b_resp       (pu_usb0_pu_noc_axi_BRESP),
         .b_valid      (pu_usb0_pu_noc_axi_BVALID),
         .b_ready      (pu_usb0_pu_noc_axi_BREADY),
         .b_trans_id   (pu_usb0_pu_noc_axi_b_trans_id),

         .ar_id        (pu_usb0_pu_noc_axi_ARID),
         .ar_addr      (pu_usb0_pu_noc_axi_ARADDR),
         .ar_len       (pu_usb0_pu_noc_axi_ARLEN),
         .ar_size      (pu_usb0_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_usb0_pu_noc_axi_ARBURST),
         .ar_lock      (pu_usb0_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_usb0_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_usb0_pu_noc_axi_ARPROT),
         .ar_qos       (pu_usb0_pu_noc_axi_arqos),
         .ar_region    (pu_usb0_pu_noc_axi_ar_region),
         .ar_user      (pu_usb0_pu_noc_axi_ar_user),
         .ar_valid     (pu_usb0_pu_noc_axi_ARVALID),
         .ar_ready     (pu_usb0_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_usb0_pu_noc_axi_ar_trans_id),

         .r_last       (pu_usb0_pu_noc_axi_RLAST),
         .r_user       (pu_usb0_pu_noc_axi_r_user),
         .r_id         (pu_usb0_pu_noc_axi_RID),
         .r_data       (pu_usb0_pu_noc_axi_RDATA),
         .r_resp       (pu_usb0_pu_noc_axi_RRESP),
         .r_valid      (pu_usb0_pu_noc_axi_RVALID),
         .r_ready      (pu_usb0_pu_noc_axi_RREADY),
         .r_trans_id   (pu_usb0_pu_noc_axi_r_trans_id)
      );

   //
   // USB1 to PU_NOC AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb1_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb1_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb1_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb1_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_usb1_pu_noc_axi_r_trans_id;
   tri0                             pu_usb1_pu_noc_axi_aw_region;
   tri0                             pu_usb1_pu_noc_axi_ar_region;
   tri0                             pu_usb1_pu_noc_axi_aw_user;
   tri0                             pu_usb1_pu_noc_axi_w_user;
   tri0                             pu_usb1_pu_noc_axi_ar_user;
   tri0                             pu_usb1_pu_noc_axi_b_user;
   tri0                             pu_usb1_pu_noc_axi_r_user;
   tri0                             pu_usb1_pu_noc_axi_awqos;
   tri0                             pu_usb1_pu_noc_axi_arqos;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (4),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (1),
                       .AXLOCK_WIDTH   (2),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_pu_usb1_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios_ahb),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_usb1_pu_noc_axi_AWID),
         .aw_addr      (pu_usb1_pu_noc_axi_AWADDR),
         .aw_len       (pu_usb1_pu_noc_axi_AWLEN),
         .aw_size      (pu_usb1_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_usb1_pu_noc_axi_AWBURST),
         .aw_lock      (pu_usb1_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_usb1_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_usb1_pu_noc_axi_AWPROT),
         .aw_qos       (pu_usb1_pu_noc_axi_awqos),
         .aw_region    (pu_usb1_pu_noc_axi_aw_region),
         .aw_user      (pu_usb1_pu_noc_axi_aw_user),
         .aw_valid     (pu_usb1_pu_noc_axi_AWVALID),
         .aw_ready     (pu_usb1_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_usb1_pu_noc_axi_aw_trans_id),

         .w_user       (pu_usb1_pu_noc_axi_w_user),
         .w_data       (pu_usb1_pu_noc_axi_WDATA),
         .w_strb       (pu_usb1_pu_noc_axi_WSTRB),
         .w_last       (pu_usb1_pu_noc_axi_WLAST),
         .w_valid      (pu_usb1_pu_noc_axi_WVALID),
         .w_ready      (pu_usb1_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_usb1_pu_noc_axi_w_trans_id),

         .b_user       (pu_usb1_pu_noc_axi_b_user),
         .b_id         (pu_usb1_pu_noc_axi_BID),
         .b_resp       (pu_usb1_pu_noc_axi_BRESP),
         .b_valid      (pu_usb1_pu_noc_axi_BVALID),
         .b_ready      (pu_usb1_pu_noc_axi_BREADY),
         .b_trans_id   (pu_usb1_pu_noc_axi_b_trans_id),

         .ar_id        (pu_usb1_pu_noc_axi_ARID),
         .ar_addr      (pu_usb1_pu_noc_axi_ARADDR),
         .ar_len       (pu_usb1_pu_noc_axi_ARLEN),
         .ar_size      (pu_usb1_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_usb1_pu_noc_axi_ARBURST),
         .ar_lock      (pu_usb1_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_usb1_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_usb1_pu_noc_axi_ARPROT),
         .ar_qos       (pu_usb1_pu_noc_axi_arqos),
         .ar_region    (pu_usb1_pu_noc_axi_ar_region),
         .ar_user      (pu_usb1_pu_noc_axi_ar_user),
         .ar_valid     (pu_usb1_pu_noc_axi_ARVALID),
         .ar_ready     (pu_usb1_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_usb1_pu_noc_axi_ar_trans_id),

         .r_last       (pu_usb1_pu_noc_axi_RLAST),
         .r_user       (pu_usb1_pu_noc_axi_r_user),
         .r_id         (pu_usb1_pu_noc_axi_RID),
         .r_data       (pu_usb1_pu_noc_axi_RDATA),
         .r_resp       (pu_usb1_pu_noc_axi_RRESP),
         .r_valid      (pu_usb1_pu_noc_axi_RVALID),
         .r_ready      (pu_usb1_pu_noc_axi_RREADY),
         .r_trans_id   (pu_usb1_pu_noc_axi_r_trans_id)
      );

   //
   // PU_NOC to PU AXI Interface
   //

   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_static_pu_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_static_pu_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_static_pu_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_static_pu_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     pu_static_pu_noc_axi_r_trans_id;
   tri0                             pu_static_pu_noc_axi_aw_region;
   tri0                             pu_static_pu_noc_axi_ar_region;
   tri0                             pu_static_pu_noc_axi_aw_user;
   tri0                             pu_static_pu_noc_axi_w_user;
   tri0                             pu_static_pu_noc_axi_ar_user;
   tri0                             pu_static_pu_noc_axi_b_user;
   tri0                             pu_static_pu_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (32),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_pu_static_pu_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (pu_static_pu_noc_axi_AWID),
         .aw_addr      (pu_static_pu_noc_axi_AWADDR),
         .aw_len       (pu_static_pu_noc_axi_AWLEN),
         .aw_size      (pu_static_pu_noc_axi_AWSIZE),
         .aw_burst     (pu_static_pu_noc_axi_AWBURST),
         .aw_lock      (pu_static_pu_noc_axi_AWLOCK),
         .aw_cache     (pu_static_pu_noc_axi_AWCACHE),
         .aw_prot      (pu_static_pu_noc_axi_AWPROT),
         .aw_qos       (pu_static_pu_noc_axi_AWQOS),
         .aw_region    (pu_static_pu_noc_axi_aw_region),
         .aw_user      (pu_static_pu_noc_axi_aw_user),
         .aw_valid     (pu_static_pu_noc_axi_AWVALID),
         .aw_ready     (pu_static_pu_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (pu_static_pu_noc_axi_aw_trans_id),

         .w_user       (pu_static_pu_noc_axi_w_user),
         .w_data       (pu_static_pu_noc_axi_WDATA),
         .w_strb       (pu_static_pu_noc_axi_WSTRB),
         .w_last       (pu_static_pu_noc_axi_WLAST),
         .w_valid      (pu_static_pu_noc_axi_WVALID),
         .w_ready      (pu_static_pu_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (pu_static_pu_noc_axi_w_trans_id),

         .b_user       (pu_static_pu_noc_axi_b_user),
         .b_id         (pu_static_pu_noc_axi_BID),
         .b_resp       (pu_static_pu_noc_axi_BRESP),
         .b_valid      (pu_static_pu_noc_axi_BVALID),
         .b_ready      (pu_static_pu_noc_axi_BREADY),
         .b_trans_id   (pu_static_pu_noc_axi_b_trans_id),

         .ar_id        (pu_static_pu_noc_axi_ARID),
         .ar_addr      (pu_static_pu_noc_axi_ARADDR),
         .ar_len       (pu_static_pu_noc_axi_ARLEN),
         .ar_size      (pu_static_pu_noc_axi_ARSIZE),
         .ar_burst     (pu_static_pu_noc_axi_ARBURST),
         .ar_lock      (pu_static_pu_noc_axi_ARLOCK),
         .ar_cache     (pu_static_pu_noc_axi_ARCACHE),
         .ar_prot      (pu_static_pu_noc_axi_ARPROT),
         .ar_qos       (pu_static_pu_noc_axi_ARQOS),
         .ar_region    (pu_static_pu_noc_axi_ar_region),
         .ar_user      (pu_static_pu_noc_axi_ar_user),
         .ar_valid     (pu_static_pu_noc_axi_ARVALID),
         .ar_ready     (pu_static_pu_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (pu_static_pu_noc_axi_ar_trans_id),

         .r_last       (pu_static_pu_noc_axi_RLAST),
         .r_user       (pu_static_pu_noc_axi_r_user),
         .r_id         (pu_static_pu_noc_axi_RID),
         .r_data       (pu_static_pu_noc_axi_RDATA),
         .r_resp       (pu_static_pu_noc_axi_RRESP),
         .r_valid      (pu_static_pu_noc_axi_RVALID),
         .r_ready      (pu_static_pu_noc_axi_RREADY),
         .r_trans_id   (pu_static_pu_noc_axi_r_trans_id)
      );

   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_sp_spio_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_sp_spio_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_sp_spio_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_sp_spio_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_sp_spio_noc_axi_r_trans_id;
   tri0                             spio_sp_spio_noc_axi_aw_region;
   tri0                             spio_sp_spio_noc_axi_ar_region;
   tri0                             spio_sp_spio_noc_axi_aw_user;
   tri0                             spio_sp_spio_noc_axi_aw_qos;
   tri0                             spio_sp_spio_noc_axi_w_user;
   tri0                             spio_sp_spio_noc_axi_ar_user;
   tri0                             spio_sp_spio_noc_axi_ar_qos;
   tri0                             spio_sp_spio_noc_axi_b_user;
   tri0                             spio_sp_spio_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (5),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS  (0)) m_spio_sp_spio_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_sp_spio_noc_axi_AWID),
         .aw_addr      (spio_sp_spio_noc_axi_AWADDR),
         .aw_len       (spio_sp_spio_noc_axi_AWLEN),
         .aw_size      (spio_sp_spio_noc_axi_AWSIZE),
         .aw_burst     (spio_sp_spio_noc_axi_AWBURST),
         .aw_lock      (spio_sp_spio_noc_axi_AWLOCK),
         .aw_cache     (spio_sp_spio_noc_axi_AWCACHE),
         .aw_prot      (spio_sp_spio_noc_axi_AWPROT),
         .aw_qos       (spio_sp_spio_noc_axi_AWQOS),
         .aw_region    (spio_sp_spio_noc_axi_aw_region),
         .aw_user      (spio_sp_spio_noc_axi_AWUSER),
         .aw_valid     (spio_sp_spio_noc_axi_AWVALID),
         .aw_ready     (spio_sp_spio_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_sp_spio_noc_axi_aw_trans_id),

         .w_user       (spio_sp_spio_noc_axi_w_user),
         .w_data       (spio_sp_spio_noc_axi_WDATA),
         .w_strb       (spio_sp_spio_noc_axi_WSTRB),
         .w_last       (spio_sp_spio_noc_axi_WLAST),
         .w_valid      (spio_sp_spio_noc_axi_WVALID),
         .w_ready      (spio_sp_spio_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_sp_spio_noc_axi_w_trans_id),

         .b_user       (spio_sp_spio_noc_axi_b_user),
         .b_id         (spio_sp_spio_noc_axi_BID),
         .b_resp       (spio_sp_spio_noc_axi_BRESP),
         .b_valid      (spio_sp_spio_noc_axi_BVALID),
         .b_ready      (spio_sp_spio_noc_axi_BREADY),
         .b_trans_id   (spio_sp_spio_noc_axi_b_trans_id),

         .ar_id        (spio_sp_spio_noc_axi_ARID),
         .ar_addr      (spio_sp_spio_noc_axi_ARADDR),
         .ar_len       (spio_sp_spio_noc_axi_ARLEN),
         .ar_size      (spio_sp_spio_noc_axi_ARSIZE),
         .ar_burst     (spio_sp_spio_noc_axi_ARBURST),
         .ar_lock      (spio_sp_spio_noc_axi_ARLOCK),
         .ar_cache     (spio_sp_spio_noc_axi_ARCACHE),
         .ar_prot      (spio_sp_spio_noc_axi_ARPROT),
         .ar_qos       (spio_sp_spio_noc_axi_ARQOS),
         .ar_region    (spio_sp_spio_noc_axi_ar_region),
         .ar_user      (spio_sp_spio_noc_axi_ar_user),
         .ar_valid     (spio_sp_spio_noc_axi_ARVALID),
         .ar_ready     (spio_sp_spio_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_sp_spio_noc_axi_ar_trans_id),

         .r_last       (spio_sp_spio_noc_axi_RLAST),
         .r_user       (spio_sp_spio_noc_axi_r_user),
         .r_id         (spio_sp_spio_noc_axi_RID),
         .r_data       (spio_sp_spio_noc_axi_RDATA),
         .r_resp       (spio_sp_spio_noc_axi_RRESP),
         .r_valid      (spio_sp_spio_noc_axi_RVALID),
         .r_ready      (spio_sp_spio_noc_axi_RREADY),
         .r_trans_id   (spio_sp_spio_noc_axi_r_trans_id)
      );

   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_noc_dbg_comm_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_noc_dbg_comm_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_noc_dbg_comm_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_noc_dbg_comm_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_noc_dbg_comm_axi_r_trans_id;
   tri0                             spio_noc_dbg_comm_axi_aw_region;
   tri0                             spio_noc_dbg_comm_axi_ar_region;
   tri0                             spio_noc_dbg_comm_axi_aw_user;
   tri0                             spio_noc_dbg_comm_axi_aw_qos;
   tri0                             spio_noc_dbg_comm_axi_w_user;
   tri0                             spio_noc_dbg_comm_axi_ar_user;
   tri0                             spio_noc_dbg_comm_axi_ar_qos;
   tri0                             spio_noc_dbg_comm_axi_b_user;
   tri0                             spio_noc_dbg_comm_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_noc_dbg_comm_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_noc_dbg_comm_axi_AWID),
         .aw_addr      (spio_noc_dbg_comm_axi_AWADDR),
         .aw_len       (spio_noc_dbg_comm_axi_AWLEN),
         .aw_size      (spio_noc_dbg_comm_axi_AWSIZE),
         .aw_burst     (spio_noc_dbg_comm_axi_AWBURST),
         .aw_lock      (spio_noc_dbg_comm_axi_AWLOCK),
         .aw_cache     (spio_noc_dbg_comm_axi_AWCACHE),
         .aw_prot      (spio_noc_dbg_comm_axi_AWPROT),
         .aw_qos       (spio_noc_dbg_comm_axi_AWQOS),
         .aw_region    (spio_noc_dbg_comm_axi_aw_region),
         .aw_user      (spio_noc_dbg_comm_axi_aw_user),
         .aw_valid     (spio_noc_dbg_comm_axi_AWVALID),
         .aw_ready     (spio_noc_dbg_comm_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_noc_dbg_comm_axi_aw_trans_id),

         .w_user       (spio_noc_dbg_comm_axi_w_user),
         .w_data       (spio_noc_dbg_comm_axi_WDATA),
         .w_strb       (spio_noc_dbg_comm_axi_WSTRB),
         .w_last       (spio_noc_dbg_comm_axi_WLAST),
         .w_valid      (spio_noc_dbg_comm_axi_WVALID),
         .w_ready      (spio_noc_dbg_comm_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_noc_dbg_comm_axi_w_trans_id),

         .b_user       (spio_noc_dbg_comm_axi_b_user),
         .b_id         (spio_noc_dbg_comm_axi_BID),
         .b_resp       (spio_noc_dbg_comm_axi_BRESP),
         .b_valid      (spio_noc_dbg_comm_axi_BVALID),
         .b_ready      (spio_noc_dbg_comm_axi_BREADY),
         .b_trans_id   (spio_noc_dbg_comm_axi_b_trans_id),

         .ar_id        (spio_noc_dbg_comm_axi_ARID),
         .ar_addr      (spio_noc_dbg_comm_axi_ARADDR),
         .ar_len       (spio_noc_dbg_comm_axi_ARLEN),
         .ar_size      (spio_noc_dbg_comm_axi_ARSIZE),
         .ar_burst     (spio_noc_dbg_comm_axi_ARBURST),
         .ar_lock      (spio_noc_dbg_comm_axi_ARLOCK),
         .ar_cache     (spio_noc_dbg_comm_axi_ARCACHE),
         .ar_prot      (spio_noc_dbg_comm_axi_ARPROT),
         .ar_qos       (spio_noc_dbg_comm_axi_ARQOS),
         .ar_region    (spio_noc_dbg_comm_axi_ar_region),
         .ar_user      (spio_noc_dbg_comm_axi_ar_user),
         .ar_valid     (spio_noc_dbg_comm_axi_ARVALID),
         .ar_ready     (spio_noc_dbg_comm_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_noc_dbg_comm_axi_ar_trans_id),

         .r_last       (spio_noc_dbg_comm_axi_RLAST),
         .r_user       (spio_noc_dbg_comm_axi_r_user),
         .r_id         (spio_noc_dbg_comm_axi_RID),
         .r_data       (spio_noc_dbg_comm_axi_RDATA),
         .r_resp       (spio_noc_dbg_comm_axi_RRESP),
         .r_valid      (spio_noc_dbg_comm_axi_RVALID),
         .r_ready      (spio_noc_dbg_comm_axi_RREADY),
         .r_trans_id   (spio_noc_dbg_comm_axi_r_trans_id)
      );

   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_am_spio_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_am_spio_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_am_spio_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_am_spio_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_am_spio_noc_axi_r_trans_id;
   tri0                             spio_am_spio_noc_axi_aw_region;
   tri0                             spio_am_spio_noc_axi_ar_region;
   tri0                             spio_am_spio_noc_axi_aw_user;
   tri0                             spio_am_spio_noc_axi_aw_qos;
   tri0                             spio_am_spio_noc_axi_w_user;
   tri0                             spio_am_spio_noc_axi_ar_user;
   tri0                             spio_am_spio_noc_axi_ar_qos;
   tri0                             spio_am_spio_noc_axi_b_user;
   tri0                             spio_am_spio_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (64),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_am_spio_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_am_spio_noc_axi_AWID),
         .aw_addr      (spio_am_spio_noc_axi_AWADDR),
         .aw_len       (spio_am_spio_noc_axi_AWLEN),
         .aw_size      (spio_am_spio_noc_axi_AWSIZE),
         .aw_burst     (spio_am_spio_noc_axi_AWBURST),
         .aw_lock      (spio_am_spio_noc_axi_AWLOCK),
         .aw_cache     (spio_am_spio_noc_axi_AWCACHE),
         .aw_prot      (spio_am_spio_noc_axi_AWPROT),
         .aw_qos       (spio_am_spio_noc_axi_AWQOS),
         .aw_region    (spio_am_spio_noc_axi_aw_region),
         .aw_user      (spio_am_spio_noc_axi_aw_user),
         .aw_valid     (spio_am_spio_noc_axi_AWVALID),
         .aw_ready     (spio_am_spio_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_am_spio_noc_axi_aw_trans_id),

         .w_user       (spio_am_spio_noc_axi_w_user),
         .w_data       (spio_am_spio_noc_axi_WDATA),
         .w_strb       (spio_am_spio_noc_axi_WSTRB),
         .w_last       (spio_am_spio_noc_axi_WLAST),
         .w_valid      (spio_am_spio_noc_axi_WVALID),
         .w_ready      (spio_am_spio_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_am_spio_noc_axi_w_trans_id),

         .b_user       (spio_am_spio_noc_axi_b_user),
         .b_id         (spio_am_spio_noc_axi_BID),
         .b_resp       (spio_am_spio_noc_axi_BRESP),
         .b_valid      (spio_am_spio_noc_axi_BVALID),
         .b_ready      (spio_am_spio_noc_axi_BREADY),
         .b_trans_id   (spio_am_spio_noc_axi_b_trans_id),

         .ar_id        (spio_am_spio_noc_axi_ARID),
         .ar_addr      (spio_am_spio_noc_axi_ARADDR),
         .ar_len       (spio_am_spio_noc_axi_ARLEN),
         .ar_size      (spio_am_spio_noc_axi_ARSIZE),
         .ar_burst     (spio_am_spio_noc_axi_ARBURST),
         .ar_lock      (spio_am_spio_noc_axi_ARLOCK),
         .ar_cache     (spio_am_spio_noc_axi_ARCACHE),
         .ar_prot      (spio_am_spio_noc_axi_ARPROT),
         .ar_qos       (spio_am_spio_noc_axi_ARQOS),
         .ar_region    (spio_am_spio_noc_axi_ar_region),
         .ar_user      (spio_am_spio_noc_axi_ar_user),
         .ar_valid     (spio_am_spio_noc_axi_ARVALID),
         .ar_ready     (spio_am_spio_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_am_spio_noc_axi_ar_trans_id),

         .r_last       (spio_am_spio_noc_axi_RLAST),
         .r_user       (spio_am_spio_noc_axi_r_user),
         .r_id         (spio_am_spio_noc_axi_RID),
         .r_data       (spio_am_spio_noc_axi_RDATA),
         .r_resp       (spio_am_spio_noc_axi_RRESP),
         .r_valid      (spio_am_spio_noc_axi_RVALID),
         .r_ready      (spio_am_spio_noc_axi_RREADY),
         .r_trans_id   (spio_am_spio_noc_axi_r_trans_id)
      );


   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_dma_spio_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_dma_spio_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_dma_spio_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_dma_spio_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_dma_spio_noc_axi_r_trans_id;
   tri0                             spio_dma_spio_noc_axi_aw_region;
   tri0                             spio_dma_spio_noc_axi_ar_region;
   tri0                             spio_dma_spio_noc_axi_aw_user;
   tri0                             spio_dma_spio_noc_axi_aw_qos;
   tri0                             spio_dma_spio_noc_axi_w_user;
   tri0                             spio_dma_spio_noc_axi_ar_user;
   tri0                             spio_dma_spio_noc_axi_ar_qos;
   tri0                             spio_dma_spio_noc_axi_b_user;
   tri0                             spio_dma_spio_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID                        (IOSHIRE_ID),
                       .ID_WIDTH                        (4),
                       .ADDR_WIDTH                      (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH                      (32),
                       .AXSIZE_WIDTH                    (3),
                       .AXLEN_WIDTH                     (4),
                       .ARUSER_WIDTH                    (1),
                       .AWUSER_WIDTH                    (1),
                       .AXREGION_WIDTH                  (1),
                       .AXQOS_WIDTH                     (1),
                       .AXLOCK_WIDTH                    (2),
                       .AW_VC_WIDTH                     (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS                   (0)) m_spio_dma_spio_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios_ahb),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_dma_spio_noc_axi_AWID),
         .aw_addr      (spio_dma_spio_noc_axi_AWADDR),
         .aw_len       (spio_dma_spio_noc_axi_AWLEN),
         .aw_size      (spio_dma_spio_noc_axi_AWSIZE),
         .aw_burst     (spio_dma_spio_noc_axi_AWBURST),
         .aw_lock      (spio_dma_spio_noc_axi_AWLOCK),
         .aw_cache     (spio_dma_spio_noc_axi_AWCACHE),
         .aw_prot      (spio_dma_spio_noc_axi_AWPROT),
         .aw_qos       (spio_dma_spio_noc_axi_aw_qos),
         .aw_region    (spio_dma_spio_noc_axi_aw_region),
         .aw_user      (spio_dma_spio_noc_axi_aw_user),
         .aw_valid     (spio_dma_spio_noc_axi_AWVALID),
         .aw_ready     (spio_dma_spio_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_dma_spio_noc_axi_aw_trans_id),

         .w_user       (spio_dma_spio_noc_axi_w_user),
         .w_data       (spio_dma_spio_noc_axi_WDATA),
         .w_strb       (spio_dma_spio_noc_axi_WSTRB),
         .w_last       (spio_dma_spio_noc_axi_WLAST),
         .w_valid      (spio_dma_spio_noc_axi_WVALID),
         .w_ready      (spio_dma_spio_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_dma_spio_noc_axi_w_trans_id),

         .b_user       (spio_dma_spio_noc_axi_b_user),
         .b_id         (spio_dma_spio_noc_axi_BID),
         .b_resp       (spio_dma_spio_noc_axi_BRESP),
         .b_valid      (spio_dma_spio_noc_axi_BVALID),
         .b_ready      (spio_dma_spio_noc_axi_BREADY),
         .b_trans_id   (spio_dma_spio_noc_axi_b_trans_id),

         .ar_id        (spio_dma_spio_noc_axi_ARID),
         .ar_addr      (spio_dma_spio_noc_axi_ARADDR),
         .ar_len       (spio_dma_spio_noc_axi_ARLEN),
         .ar_size      (spio_dma_spio_noc_axi_ARSIZE),
         .ar_burst     (spio_dma_spio_noc_axi_ARBURST),
         .ar_lock      (spio_dma_spio_noc_axi_ARLOCK),
         .ar_cache     (spio_dma_spio_noc_axi_ARCACHE),
         .ar_prot      (spio_dma_spio_noc_axi_ARPROT),
         .ar_qos       (spio_dma_spio_noc_axi_ar_qos),
         .ar_region    (spio_dma_spio_noc_axi_ar_region),
         .ar_user      (spio_dma_spio_noc_axi_ar_user),
         .ar_valid     (spio_dma_spio_noc_axi_ARVALID),
         .ar_ready     (spio_dma_spio_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_dma_spio_noc_axi_ar_trans_id),

         .r_last       (spio_dma_spio_noc_axi_RLAST),
         .r_user       (spio_dma_spio_noc_axi_r_user),
         .r_id         (spio_dma_spio_noc_axi_RID),
         .r_data       (spio_dma_spio_noc_axi_RDATA),
         .r_resp       (spio_dma_spio_noc_axi_RRESP),
         .r_valid      (spio_dma_spio_noc_axi_RVALID),
         .r_ready      (spio_dma_spio_noc_axi_RREADY),
         .r_trans_id   (spio_dma_spio_noc_axi_r_trans_id)
      );

   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_vault_spio_noc_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_vault_spio_noc_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_vault_spio_noc_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_vault_spio_noc_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_vault_spio_noc_axi_r_trans_id;
   tri0                             spio_vault_spio_noc_axi_aw_region;
   tri0                             spio_vault_spio_noc_axi_ar_region;
   tri0                             spio_vault_spio_noc_axi_aw_user;
   tri0                             spio_vault_spio_noc_axi_aw_qos;
   tri0                             spio_vault_spio_noc_axi_w_user;
   tri0                             spio_vault_spio_noc_axi_ar_user;
   tri0                             spio_vault_spio_noc_axi_ar_qos;
   tri0                             spio_vault_spio_noc_axi_b_user;
   tri0                             spio_vault_spio_noc_axi_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (64),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_vault_spio_noc_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_vault_spio_noc_axi_AWID),
         .aw_addr      (spio_vault_spio_noc_axi_AWADDR),
         .aw_len       (spio_vault_spio_noc_axi_AWLEN),
         .aw_size      (spio_vault_spio_noc_axi_AWSIZE),
         .aw_burst     (spio_vault_spio_noc_axi_AWBURST),
         .aw_lock      (spio_vault_spio_noc_axi_AWLOCK),
         .aw_cache     (spio_vault_spio_noc_axi_AWCACHE),
         .aw_prot      (spio_vault_spio_noc_axi_AWPROT),
         .aw_qos       (spio_vault_spio_noc_axi_AWQOS),
         .aw_region    (spio_vault_spio_noc_axi_aw_region),
         .aw_user      (spio_vault_spio_noc_axi_aw_user),
         .aw_valid     (spio_vault_spio_noc_axi_AWVALID),
         .aw_ready     (spio_vault_spio_noc_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_vault_spio_noc_axi_aw_trans_id),

         .w_user       (spio_vault_spio_noc_axi_w_user),
         .w_data       (spio_vault_spio_noc_axi_WDATA),
         .w_strb       (spio_vault_spio_noc_axi_WSTRB),
         .w_last       (spio_vault_spio_noc_axi_WLAST),
         .w_valid      (spio_vault_spio_noc_axi_WVALID),
         .w_ready      (spio_vault_spio_noc_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_vault_spio_noc_axi_w_trans_id),

         .b_user       (spio_vault_spio_noc_axi_b_user),
         .b_id         (spio_vault_spio_noc_axi_BID),
         .b_resp       (spio_vault_spio_noc_axi_BRESP),
         .b_valid      (spio_vault_spio_noc_axi_BVALID),
         .b_ready      (spio_vault_spio_noc_axi_BREADY),
         .b_trans_id   (spio_vault_spio_noc_axi_b_trans_id),

         .ar_id        (spio_vault_spio_noc_axi_ARID),
         .ar_addr      (spio_vault_spio_noc_axi_ARADDR),
         .ar_len       (spio_vault_spio_noc_axi_ARLEN),
         .ar_size      (spio_vault_spio_noc_axi_ARSIZE),
         .ar_burst     (spio_vault_spio_noc_axi_ARBURST),
         .ar_lock      (spio_vault_spio_noc_axi_ARLOCK),
         .ar_cache     (spio_vault_spio_noc_axi_ARCACHE),
         .ar_prot      (spio_vault_spio_noc_axi_ARPROT),
         .ar_qos       (spio_vault_spio_noc_axi_ARQOS),
         .ar_region    (spio_vault_spio_noc_axi_ar_region),
         .ar_user      (spio_vault_spio_noc_axi_ar_user),
         .ar_valid     (spio_vault_spio_noc_axi_ARVALID),
         .ar_ready     (spio_vault_spio_noc_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_vault_spio_noc_axi_ar_trans_id),

         .r_last       (spio_vault_spio_noc_axi_RLAST),
         .r_user       (spio_vault_spio_noc_axi_r_user),
         .r_id         (spio_vault_spio_noc_axi_RID),
         .r_data       (spio_vault_spio_noc_axi_RDATA),
         .r_resp       (spio_vault_spio_noc_axi_RRESP),
         .r_valid      (spio_vault_spio_noc_axi_RVALID),
         .r_ready      (spio_vault_spio_noc_axi_RREADY),
         .r_trans_id   (spio_vault_spio_noc_axi_r_trans_id)
      );


   //
   // AXI master driver to SPIO NOC
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_p10_pcie_s_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_p10_pcie_s_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_p10_pcie_s_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_p10_pcie_s_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]     spio_p10_pcie_s_r_trans_id;
   tri0                             spio_p10_pcie_s_aw_region;
   tri0                             spio_p10_pcie_s_ar_region;
   tri0                             spio_p10_pcie_s_aw_user;
   tri0                             spio_p10_pcie_s_aw_qos;
   tri0                             spio_p10_pcie_s_w_user;
   tri0                             spio_p10_pcie_s_ar_user;
   tri0                             spio_p10_pcie_s_ar_qos;
   tri0                             spio_p10_pcie_s_b_user;
   tri0                             spio_p10_pcie_s_r_user;

   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (6),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .AXLEN_WIDTH    (8),
                       .ARUSER_WIDTH   (1),
                       .AWUSER_WIDTH   (1),
                       .AXREGION_WIDTH (1),
                       .AXQOS_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_p10_pcie_s_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_p10_pcie_s_AWID),
         .aw_addr      (spio_p10_pcie_s_AWADDR),
         .aw_len       (spio_p10_pcie_s_AWLEN),
         .aw_size      (spio_p10_pcie_s_AWSIZE),
         .aw_burst     (spio_p10_pcie_s_AWBURST),
         .aw_lock      (spio_p10_pcie_s_AWLOCK),
         .aw_cache     (spio_p10_pcie_s_AWCACHE),
         .aw_prot      (spio_p10_pcie_s_AWPROT),
         .aw_qos       (spio_p10_pcie_s_AWQOS),
         .aw_region    (spio_p10_pcie_s_aw_region),
         .aw_user      (spio_p10_pcie_s_aw_user),
         .aw_valid     (spio_p10_pcie_s_AWVALID),
         .aw_ready     (spio_p10_pcie_s_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_p10_pcie_s_aw_trans_id),

         .w_user       (spio_p10_pcie_s_w_user),
         .w_data       (spio_p10_pcie_s_WDATA),
         .w_strb       (spio_p10_pcie_s_WSTRB),
         .w_last       (spio_p10_pcie_s_WLAST),
         .w_valid      (spio_p10_pcie_s_WVALID),
         .w_ready      (spio_p10_pcie_s_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_p10_pcie_s_w_trans_id),

         .b_user       (spio_p10_pcie_s_b_user),
         .b_id         (spio_p10_pcie_s_BID),
         .b_resp       (spio_p10_pcie_s_BRESP),
         .b_valid      (spio_p10_pcie_s_BVALID),
         .b_ready      (spio_p10_pcie_s_BREADY),
         .b_trans_id   (spio_p10_pcie_s_b_trans_id),

         .ar_id        (spio_p10_pcie_s_ARID),
         .ar_addr      (spio_p10_pcie_s_ARADDR),
         .ar_len       (spio_p10_pcie_s_ARLEN),
         .ar_size      (spio_p10_pcie_s_ARSIZE),
         .ar_burst     (spio_p10_pcie_s_ARBURST),
         .ar_lock      (spio_p10_pcie_s_ARLOCK),
         .ar_cache     (spio_p10_pcie_s_ARCACHE),
         .ar_prot      (spio_p10_pcie_s_ARPROT),
         .ar_qos       (spio_p10_pcie_s_ARQOS),
         .ar_region    (spio_p10_pcie_s_ar_region),
         .ar_user      (spio_p10_pcie_s_ar_user),
         .ar_valid     (spio_p10_pcie_s_ARVALID),
         .ar_ready     (spio_p10_pcie_s_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_p10_pcie_s_ar_trans_id),

         .r_last       (spio_p10_pcie_s_RLAST),
         .r_user       (spio_p10_pcie_s_r_user),
         .r_id         (spio_p10_pcie_s_RID),
         .r_data       (spio_p10_pcie_s_RDATA),
         .r_resp       (spio_p10_pcie_s_RRESP),
         .r_valid      (spio_p10_pcie_s_RVALID),
         .r_ready      (spio_p10_pcie_s_RREADY),
         .r_trans_id   (spio_p10_pcie_s_r_trans_id)
      );

   //
   // SPIO_NOC APB0 interface
   //
   evl_apb_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .AGENT_ID       (0),
                       .ADDR_WIDTH     (32),
                       .DATA_WIDTH     (32),
                       .SKIP_LEVELS    ({ 2 })) m_spio_noc_apb0 (
         .isolate      (1'b0),
         .pclk         (clk_ios),
         .presetn      (~reset_ios_ckcore),
         .paddr        (spio_p10_apb0_paddr),
         .pwdata       (spio_p10_apb0_pwdata),
         .pwrite       (spio_p10_apb0_pwrite),
         .psel         (spio_p10_apb0_psel),
         .penable      (spio_p10_apb0_penable),
         .pready       (spio_p10_apb0_pready),
         .prdata       (spio_p10_apb0_prdata),
         .pslverr      (spio_p10_apb0_pslverr)
      );

   //
   // SPIO_NOC APB1 interface
   //
   evl_apb_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .AGENT_ID       (1),
                       .ADDR_WIDTH     (32),
                       .DATA_WIDTH     (32),
                       .SKIP_LEVELS    ({ 2 })) m_spio_noc_apb1 (
         .isolate      (1'b0),
         .pclk         (clk_ios),
         .presetn      (~reset_ios_ckcore),
         .paddr        (spio_p10_apb1_paddr),
         .pwdata       (spio_p10_apb1_pwdata),
         .pwrite       (spio_p10_apb1_pwrite),
         .psel         (spio_p10_apb1_psel),
         .penable      (spio_p10_apb1_penable),
         .pready       (spio_p10_apb1_pready),
         .prdata       (spio_p10_apb1_prdata),
         .pslverr      (spio_p10_apb1_pslverr)
      );

   //-----------------------------------------------------------------------------------------------
   //
   // Create slave AXI mesh bus agent interface.
   //

   //-----------------------------------------------------------------------------------------------
   //
   // Add some unused signals.
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3_master_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3_master_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3_master_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3_master_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3_master_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_tol3_master_axi_arregion;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_tol3_master_axi_awregion;
   tri0                                         ioshire_tol3_master_axi_wuser;
   tri0                                         ioshire_tol3_master_axi_buser;
   tri0                                         ioshire_tol3_master_axi_ruser;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   ioshire_tol3_master_axi_aruser;

  evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                      .PORT_ID      (0),
                      .MAX_PORT_ID  (NUM_L3_PORTS),
                      .PORT_TYPE    (int'(evl_base_pkg::AXI_TO_L3MESH)),
                      .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                      .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                      .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                      .AXSIZE_WIDTH (3),
                      .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                      .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ioshire_tol3_mesh_master_axi (
        .isolate         (1'b0),
        .reset           (reset_ios_ckcore),
        .clk             (clk__noc),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

           .aw_id        (ioshire_tol3_master_axi_AW.ID),
           .aw_addr      (ioshire_tol3_master_axi_AW.ADDR),
           .aw_len       (ioshire_tol3_master_axi_AW.LEN),
           .aw_size      (ioshire_tol3_master_axi_AW.SIZE),
           .aw_burst     (ioshire_tol3_master_axi_AW.BURST),
           .aw_lock      (ioshire_tol3_master_axi_AW.LOCK),
           .aw_cache     (ioshire_tol3_master_axi_AW.CACHE),
           .aw_prot      (ioshire_tol3_master_axi_AW.PROT),
           .aw_qos       (ioshire_tol3_master_axi_AW.QOS),
           .aw_region    (ioshire_tol3_master_axi_awregion),
           .aw_user      (ioshire_tol3_master_axi_AW.USER),
           .aw_valid     (ioshire_tol3_master_axi_AWVALID),
           .aw_ready     (ioshire_tol3_master_axi_AWREADY),
           .aw_vc_valid  (),
           .aw_vc_credit (),
           .aw_trans_id  (ioshire_tol3_master_axi_aw_trans_id),

           .w_user       (ioshire_tol3_master_axi_wuser),
           .w_data       (ioshire_tol3_master_axi_W.DATA),
           .w_strb       (ioshire_tol3_master_axi_W.STRB),
           .w_last       (ioshire_tol3_master_axi_W.LAST),
           .w_valid      (ioshire_tol3_master_axi_WVALID),
           .w_ready      (ioshire_tol3_master_axi_WREADY),
           .w_vc_valid   (),
           .w_trans_id   (ioshire_tol3_master_axi_w_trans_id),

           .b_user       (ioshire_tol3_master_axi_buser),
           .b_id         (ioshire_tol3_master_axi_B.ID),
           .b_resp       (ioshire_tol3_master_axi_B.RESP),
           .b_valid      (ioshire_tol3_master_axi_BVALID),
           .b_ready      (ioshire_tol3_master_axi_BREADY),
           .b_trans_id   (ioshire_tol3_master_axi_b_trans_id),

           .ar_id        (ioshire_tol3_master_axi_AR.ID),
           .ar_addr      (ioshire_tol3_master_axi_AR.ADDR),
           .ar_len       (ioshire_tol3_master_axi_AR.LEN),
           .ar_size      (ioshire_tol3_master_axi_AR.SIZE),
           .ar_burst     (ioshire_tol3_master_axi_AR.BURST),
           .ar_lock      (ioshire_tol3_master_axi_AR.LOCK),
           .ar_cache     (ioshire_tol3_master_axi_AR.CACHE),
           .ar_prot      (ioshire_tol3_master_axi_AR.PROT),
           .ar_qos       (ioshire_tol3_master_axi_AR.QOS),
           .ar_region    (ioshire_tol3_master_axi_arregion),
           .ar_user      (ioshire_tol3_master_axi_aruser),
           .ar_valid     (ioshire_tol3_master_axi_ARVALID),
           .ar_ready     (ioshire_tol3_master_axi_ARREADY),
           .ar_vc_valid  (),
           .ar_vc_credit (),
           .ar_trans_id  (ioshire_tol3_master_axi_ar_trans_id),

           .r_last       (ioshire_tol3_master_axi_R.LAST),
           .r_user       (ioshire_tol3_master_axi_ruser),
           .r_id         (ioshire_tol3_master_axi_R.ID),
           .r_data       (ioshire_tol3_master_axi_R.DATA),
           .r_resp       (ioshire_tol3_master_axi_R.RESP),
           .r_valid      (ioshire_tol3_master_axi_RVALID),
           .r_ready      (ioshire_tol3_master_axi_RREADY),
           .r_trans_id   (ioshire_tol3_master_axi_r_trans_id)
     );

   //
   // Create slave AXI mesh bus agent interface.
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3b_master_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3b_master_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3b_master_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3b_master_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_tol3b_master_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_tol3b_master_axi_arregion;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_tol3b_master_axi_awregion;
   tri0                                         ioshire_tol3b_master_axi_wuser;
   tri0                                         ioshire_tol3b_master_axi_buser;
   tri0                                         ioshire_tol3b_master_axi_ruser;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   ioshire_tol3b_master_axi_aruser;

  evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                      .PORT_ID      (1),
                      .MAX_PORT_ID  (NUM_L3_PORTS),
                      .PORT_TYPE    (int'(evl_base_pkg::AXI_TO_L3MESH)),
                      .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                      .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                      .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                      .AXSIZE_WIDTH (3),
                      .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                      .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ioshire_tol3b_mesh_master_axi (
        .isolate         (1'b0),
        .reset           (reset_ios_ckcore),
        .clk             (clk__noc),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

           .aw_id        (ioshire_tol3b_master_axi_AW.ID),
           .aw_addr      (ioshire_tol3b_master_axi_AW.ADDR),
           .aw_len       (ioshire_tol3b_master_axi_AW.LEN),
           .aw_size      (ioshire_tol3b_master_axi_AW.SIZE),
           .aw_burst     (ioshire_tol3b_master_axi_AW.BURST),
           .aw_lock      (ioshire_tol3b_master_axi_AW.LOCK),
           .aw_cache     (ioshire_tol3b_master_axi_AW.CACHE),
           .aw_prot      (ioshire_tol3b_master_axi_AW.PROT),
           .aw_qos       (ioshire_tol3b_master_axi_AW.QOS),
           .aw_region    (ioshire_tol3b_master_axi_awregion),
           .aw_user      (ioshire_tol3b_master_axi_AW.USER),
           .aw_valid     (ioshire_tol3b_master_axi_AWVALID),
           .aw_ready     (ioshire_tol3b_master_axi_AWREADY),
           .aw_vc_valid  (),
           .aw_vc_credit (),
           .aw_trans_id  (ioshire_tol3b_master_axi_aw_trans_id),

           .w_user       (ioshire_tol3b_master_axi_wuser),
           .w_data       (ioshire_tol3b_master_axi_W.DATA),
           .w_strb       (ioshire_tol3b_master_axi_W.STRB),
           .w_last       (ioshire_tol3b_master_axi_W.LAST),
           .w_valid      (ioshire_tol3b_master_axi_WVALID),
           .w_ready      (ioshire_tol3b_master_axi_WREADY),
           .w_vc_valid   (),
           .w_trans_id   (ioshire_tol3b_master_axi_w_trans_id),

           .b_user       (ioshire_tol3b_master_axi_buser),
           .b_id         (ioshire_tol3b_master_axi_B.ID),
           .b_resp       (ioshire_tol3b_master_axi_B.RESP),
           .b_valid      (ioshire_tol3b_master_axi_BVALID),
           .b_ready      (ioshire_tol3b_master_axi_BREADY),
           .b_trans_id   (ioshire_tol3b_master_axi_b_trans_id),

           .ar_id        (ioshire_tol3b_master_axi_AR.ID),
           .ar_addr      (ioshire_tol3b_master_axi_AR.ADDR),
           .ar_len       (ioshire_tol3b_master_axi_AR.LEN),
           .ar_size      (ioshire_tol3b_master_axi_AR.SIZE),
           .ar_burst     (ioshire_tol3b_master_axi_AR.BURST),
           .ar_lock      (ioshire_tol3b_master_axi_AR.LOCK),
           .ar_cache     (ioshire_tol3b_master_axi_AR.CACHE),
           .ar_prot      (ioshire_tol3b_master_axi_AR.PROT),
           .ar_qos       (ioshire_tol3b_master_axi_AR.QOS),
           .ar_region    (ioshire_tol3b_master_axi_arregion),
           .ar_user      (ioshire_tol3b_master_axi_aruser),
           .ar_valid     (ioshire_tol3b_master_axi_ARVALID),
           .ar_ready     (ioshire_tol3b_master_axi_ARREADY),
           .ar_vc_valid  (),
           .ar_vc_credit (),
           .ar_trans_id  (ioshire_tol3b_master_axi_ar_trans_id),

           .r_last       (ioshire_tol3b_master_axi_R.LAST),
           .r_user       (ioshire_tol3b_master_axi_ruser),
           .r_id         (ioshire_tol3b_master_axi_R.ID),
           .r_data       (ioshire_tol3b_master_axi_R.DATA),
           .r_resp       (ioshire_tol3b_master_axi_R.RESP),
           .r_valid      (ioshire_tol3b_master_axi_RVALID),
           .r_ready      (ioshire_tol3b_master_axi_RREADY),
           .r_trans_id   (ioshire_tol3b_master_axi_r_trans_id)
     );

   // io_pm
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_esr_master_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_esr_master_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_esr_master_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_esr_master_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0]                 ioshire_esr_master_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_esr_master_axi_arregion;
   tri0 [`EVL_BUS_REGION_MSB:0]                 ioshire_esr_master_axi_awregion;
   tri0                                         ioshire_esr_master_axi_wuser;
   tri0                                         ioshire_esr_master_axi_buser;
   tri0                                         ioshire_esr_master_axi_ruser;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0]   ioshire_esr_master_axi_awuser;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   ioshire_esr_master_axi_aruser;

  evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                      .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                      .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                      .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                      .AXSIZE_WIDTH (3),
                      .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                      .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_pm_master_axi (
        .isolate      (1'b0),
        .reset        (reset_ios_ckcore),
        .clk          (clk__noc),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

        .aw_id        (ioshire_esr_master_axi_AW.ID),
        .aw_addr      (ioshire_esr_master_axi_AW.ADDR),
        .aw_len       (ioshire_esr_master_axi_AW.LEN),
        .aw_size      (ioshire_esr_master_axi_AW.SIZE),
        .aw_burst     (ioshire_esr_master_axi_AW.BURST),
        .aw_lock      (ioshire_esr_master_axi_AW.LOCK),
        .aw_cache     (ioshire_esr_master_axi_AW.CACHE),
        .aw_prot      (ioshire_esr_master_axi_AW.PROT),
        .aw_qos       (ioshire_esr_master_axi_AW.QOS),
        .aw_region    (ioshire_esr_master_axi_awregion),
        .aw_user      (ioshire_esr_master_axi_awuser),
        .aw_valid     (ioshire_esr_master_axi_AWVALID),
        .aw_ready     (ioshire_esr_master_axi_AWREADY),
        .aw_vc_valid  (),
        .aw_vc_credit (),
        .aw_trans_id  (ioshire_esr_master_axi_aw_trans_id),

        .w_user       (ioshire_esr_master_axi_wuser),
        .w_data       (ioshire_esr_master_axi_W.DATA),
        .w_strb       (ioshire_esr_master_axi_W.STRB),
        .w_last       (ioshire_esr_master_axi_W.LAST),
        .w_valid      (ioshire_esr_master_axi_WVALID),
        .w_ready      (ioshire_esr_master_axi_WREADY),
        .w_vc_valid   (),
        .w_trans_id   (ioshire_esr_master_axi_w_trans_id),

        .b_user       (ioshire_esr_master_axi_buser),
        .b_id         (ioshire_esr_master_axi_B.ID),
        .b_resp       (ioshire_esr_master_axi_B.RESP),
        .b_valid      (ioshire_esr_master_axi_BVALID),
        .b_ready      (ioshire_esr_master_axi_BREADY),
        .b_trans_id   (ioshire_esr_master_axi_b_trans_id),

        .ar_id        (ioshire_esr_master_axi_AR.ID),
        .ar_addr      (ioshire_esr_master_axi_AR.ADDR),
        .ar_len       (ioshire_esr_master_axi_AR.LEN),
        .ar_size      (ioshire_esr_master_axi_AR.SIZE),
        .ar_burst     (ioshire_esr_master_axi_AR.BURST),
        .ar_lock      (ioshire_esr_master_axi_AR.LOCK),
        .ar_cache     (ioshire_esr_master_axi_AR.CACHE),
        .ar_prot      (ioshire_esr_master_axi_AR.PROT),
        .ar_qos       (ioshire_esr_master_axi_AR.QOS),
        .ar_region    (ioshire_esr_master_axi_arregion),
        .ar_user      (ioshire_esr_master_axi_aruser),
        .ar_valid     (ioshire_esr_master_axi_ARVALID),
        .ar_ready     (ioshire_esr_master_axi_ARREADY),
        .ar_vc_valid  (),
        .ar_vc_credit (),
        .ar_trans_id  (ioshire_esr_master_axi_ar_trans_id),

        .r_last       (ioshire_esr_master_axi_R.LAST),
        .r_user       (ioshire_esr_master_axi_ruser),
        .r_id         (ioshire_esr_master_axi_R.ID),
        .r_data       (ioshire_esr_master_axi_R.DATA),
        .r_resp       (ioshire_esr_master_axi_R.RESP),
        .r_valid      (ioshire_esr_master_axi_RVALID),
        .r_ready      (ioshire_esr_master_axi_RREADY),
        .r_trans_id   (ioshire_esr_master_axi_r_trans_id)
     );


   // io_ps
   tri0 [`DV_TRANS_ID_SIZE-1:0] ioshire_slave_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ioshire_slave_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ioshire_slave_axi_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ioshire_slave_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ioshire_slave_axi_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] ioshire_slave_axi_arregion;
   tri0 [`EVL_BUS_REGION_MSB:0] ioshire_slave_axi_awregion;
   tri0                         ioshire_slave_axi_wuser;
   tri0                         ioshire_slave_axi_buser;
   tri0                         ioshire_slave_axi_ruser;

   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] ioshire_slave_axi_awuser;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] ioshire_slave_axi_aruser;

  evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                      .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                      .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                      .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                      .AXSIZE_WIDTH (3),
                      .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                      .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_ps_slave_axi (
        .isolate      (1'b0),
        .reset        (reset_ios_ckcore),
        .clk          (clk__noc),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

        .aw_id        (ioshire_slave_axi_AW.ID),
        .aw_addr      (ioshire_slave_axi_AW.ADDR),
        .aw_len       (ioshire_slave_axi_AW.LEN),
        .aw_size      (ioshire_slave_axi_AW.SIZE),
        .aw_burst     (ioshire_slave_axi_AW.BURST),
        .aw_lock      (ioshire_slave_axi_AW.LOCK),
        .aw_cache     (ioshire_slave_axi_AW.CACHE),
        .aw_prot      (ioshire_slave_axi_AW.PROT),
        .aw_qos       (ioshire_slave_axi_AW.QOS),
        .aw_region    (ioshire_slave_axi_awregion),
        .aw_user      (ioshire_slave_axi_AW.USER),
        .aw_valid     (ioshire_slave_axi_AWVALID),
        .aw_ready     (ioshire_slave_axi_AWREADY),
        .aw_vc_valid  (),
        .aw_vc_credit (),
        .aw_trans_id  (ioshire_slave_axi_aw_trans_id),

        .w_user       (ioshire_slave_axi_wuser),
        .w_data       (ioshire_slave_axi_W.DATA),
        .w_strb       (ioshire_slave_axi_W.STRB),
        .w_last       (ioshire_slave_axi_W.LAST),
        .w_valid      (ioshire_slave_axi_WVALID),
        .w_ready      (ioshire_slave_axi_WREADY),
        .w_vc_valid   (),
        .w_trans_id   (ioshire_slave_axi_w_trans_id),

        .b_user       (ioshire_slave_axi_buser),
        .b_id         (ioshire_slave_axi_B.ID),
        .b_resp       (ioshire_slave_axi_B.RESP),
        .b_valid      (ioshire_slave_axi_BVALID),
        .b_ready      (ioshire_slave_axi_BREADY),
        .b_trans_id   (ioshire_slave_axi_b_trans_id),

        .ar_id        (ioshire_slave_axi_AR.ID),
        .ar_addr      (ioshire_slave_axi_AR.ADDR),
        .ar_len       (ioshire_slave_axi_AR.LEN),
        .ar_size      (ioshire_slave_axi_AR.SIZE),
        .ar_burst     (ioshire_slave_axi_AR.BURST),
        .ar_lock      (ioshire_slave_axi_AR.LOCK),
        .ar_cache     (ioshire_slave_axi_AR.CACHE),
        .ar_prot      (ioshire_slave_axi_AR.PROT),
        .ar_qos       (ioshire_slave_axi_AR.QOS),
        .ar_region    (ioshire_slave_axi_arregion),
        .ar_user      (ioshire_slave_axi_aruser),
        .ar_valid     (ioshire_slave_axi_ARVALID),
        .ar_ready     (ioshire_slave_axi_ARREADY),
        .ar_vc_valid  (),
        .ar_vc_credit (),
        .ar_trans_id  (ioshire_slave_axi_ar_trans_id),

        .r_last       (ioshire_slave_axi_R.LAST),
        .r_user       (ioshire_slave_axi_ruser),
        .r_id         (ioshire_slave_axi_R.ID),
        .r_data       (ioshire_slave_axi_R.DATA),
        .r_resp       (ioshire_slave_axi_R.RESP),
        .r_valid      (ioshire_slave_axi_RVALID),
        .r_ready      (ioshire_slave_axi_RREADY),
        .r_trans_id   (ioshire_slave_axi_r_trans_id)
     );

     // tosys mesh master
     tri0 [`DV_TRANS_ID_SIZE-1:0]               ioshire_tosys_master_axi_aw_trans_id;
     tri0 [`DV_TRANS_ID_SIZE-1:0]               ioshire_tosys_master_axi_w_trans_id;
     tri0 [`DV_TRANS_ID_SIZE-1:0]               ioshire_tosys_master_axi_b_trans_id;
     tri0 [`DV_TRANS_ID_SIZE-1:0]               ioshire_tosys_master_axi_ar_trans_id;
     tri0 [`DV_TRANS_ID_SIZE-1:0]               ioshire_tosys_master_axi_r_trans_id;
     tri0 [`EVL_BUS_REGION_MSB:0]               ioshire_tosys_master_axi_arregion;
     tri0 [`EVL_BUS_REGION_MSB:0]               ioshire_tosys_master_axi_awregion;
     tri0                                       ioshire_tosys_master_axi_wuser;
     tri0                                       ioshire_tosys_master_axi_buser;
     tri0                                       ioshire_tosys_master_axi_ruser;
     tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] ioshire_tosys_master_axi_awuser;
     tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] ioshire_tosys_master_axi_aruser;

     evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                         .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                         .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                         .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                         .AXSIZE_WIDTH (3),
                         .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                         .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_tosys_mesh_master_axi (
        .isolate      (1'b0),
        .reset        (reset_ios_ckcore),
        .clk          (clk__noc),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

        .aw_id        (ioshire_tosys_master_axi_AW.ID),
        .aw_addr      (ioshire_tosys_master_axi_AW.ADDR),
        .aw_len       (ioshire_tosys_master_axi_AW.LEN),
        .aw_size      (ioshire_tosys_master_axi_AW.SIZE),
        .aw_burst     (ioshire_tosys_master_axi_AW.BURST),
        .aw_lock      (ioshire_tosys_master_axi_AW.LOCK),
        .aw_cache     (ioshire_tosys_master_axi_AW.CACHE),
        .aw_prot      (ioshire_tosys_master_axi_AW.PROT),
        .aw_qos       (ioshire_tosys_master_axi_AW.QOS),
        .aw_region    (ioshire_tosys_master_axi_awregion),
        .aw_user      (ioshire_tosys_master_axi_AW.USER),
        .aw_valid     (ioshire_tosys_master_axi_AWVALID),
        .aw_ready     (ioshire_tosys_master_axi_AWREADY),
        .aw_vc_valid  (),
        .aw_vc_credit (),
        .aw_trans_id  (),//(tosys_aw_trans_id),

        .w_user       (ioshire_tosys_master_axi_wuser),
        .w_data       (ioshire_tosys_master_axi_W.DATA),
        .w_strb       (ioshire_tosys_master_axi_W.STRB),
        .w_last       (ioshire_tosys_master_axi_W.LAST),
        .w_valid      (ioshire_tosys_master_axi_WVALID),
        .w_ready      (ioshire_tosys_master_axi_WREADY),
        .w_vc_valid   (),
        .w_trans_id   (),//(tosys_w_trans_id),

        .b_user       (ioshire_tosys_master_axi_buser),
        .b_id         (ioshire_tosys_master_axi_B.ID),
        .b_resp       (ioshire_tosys_master_axi_B.RESP),
        .b_valid      (ioshire_tosys_master_axi_BVALID),
        .b_ready      (ioshire_tosys_master_axi_BREADY),
        .b_trans_id   (),//(tosys_b_trans_id),

        .ar_id        (ioshire_tosys_master_axi_AR.ID),
        .ar_addr      (ioshire_tosys_master_axi_AR.ADDR),
        .ar_len       (ioshire_tosys_master_axi_AR.LEN),
        .ar_size      (ioshire_tosys_master_axi_AR.SIZE),
        .ar_burst     (ioshire_tosys_master_axi_AR.BURST),
        .ar_lock      (ioshire_tosys_master_axi_AR.LOCK),
        .ar_cache     (ioshire_tosys_master_axi_AR.CACHE),
        .ar_prot      (ioshire_tosys_master_axi_AR.PROT),
        .ar_qos       (ioshire_tosys_master_axi_AR.QOS),
        .ar_region    (ioshire_tosys_master_axi_arregion),
        .ar_user      (ioshire_tosys_master_axi_aruser),
        .ar_valid     (ioshire_tosys_master_axi_ARVALID),
        .ar_ready     (ioshire_tosys_master_axi_ARREADY),
        .ar_vc_valid  (),
        .ar_vc_credit (),
        .ar_trans_id  (),//(tosys_ar_trans_id),

        .r_last       (ioshire_tosys_master_axi_R.LAST),
        .r_user       (ioshire_tosys_master_axi_ruser),
        .r_id         (ioshire_tosys_master_axi_R.ID),
        .r_data       (ioshire_tosys_master_axi_R.DATA),
        .r_resp       (ioshire_tosys_master_axi_R.RESP),
        .r_valid      (ioshire_tosys_master_axi_RVALID),
        .r_ready      (ioshire_tosys_master_axi_RREADY),
        .r_trans_id   ()//(tosys_r_trans_id)
     );


   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_sp_main0_main_noc_rbm_s_aw_trans_id;
   tri0                                       spio_sp_main0_main_noc_rbm_s_aw_qos;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_sp_main0_main_noc_rbm_s_aw_user;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_sp_main0_main_noc_rbm_s_aw_region;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_sp_main0_main_noc_rbm_s_w_trans_id;
   tri0                                       spio_sp_main0_main_noc_rbm_s_w_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_sp_main0_main_noc_rbm_s_b_trans_id;
   tri0                                       spio_sp_main0_main_noc_rbm_s_b_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_sp_main0_main_noc_rbm_s_ar_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_sp_main0_main_noc_rbm_s_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_sp_main0_main_noc_rbm_s_ar_user;
   tri0                                       spio_sp_main0_main_noc_rbm_s_ar_qos;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_sp_main0_main_noc_rbm_s_r_trans_id;
   tri0                                       spio_sp_main0_main_noc_rbm_s_r_user;

   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_aw_id;
   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_aw_len;
   tri0 [2:0]                                 spio_sp_main0_main_noc_rbm_s_aw_size;
   tri0 [1:0]                                 spio_sp_main0_main_noc_rbm_s_aw_burst;
   tri0                                       spio_sp_main0_main_noc_rbm_s_aw_lock;
   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_aw_cache;

   tri1                                       spio_sp_main0_main_noc_rbm_s_w_last;

   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_b_id;

   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_ar_id;
   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_ar_len;
   tri0 [2:0]                                 spio_sp_main0_main_noc_rbm_s_ar_size;
   tri0 [1:0]                                 spio_sp_main0_main_noc_rbm_s_ar_burst;
   tri0                                       spio_sp_main0_main_noc_rbm_s_ar_lock;
   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_ar_cache;

   tri1                                       spio_sp_main0_main_noc_rbm_s_r_last;
   tri0 [3:0]                                 spio_sp_main0_main_noc_rbm_s_r_id;

   assign spio_sp_main0_main_noc_rbm_s_aw_size = 3'd2;
   assign spio_sp_main0_main_noc_rbm_s_ar_size = 3'd2;


   evl_axi_interface #(.SHIRE_ID                        (IOSHIRE_ID),
                       .ID_WIDTH                        (4),
                       .ADDR_WIDTH                      (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH                      (32),
                       .AXSIZE_WIDTH                    (3),
                       .ARUSER_WIDTH                    (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH                    (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH                  (4),
                       .AXQOS_WIDTH                     (1),
                       .AXLEN_WIDTH                     (4),
                       .AW_VC_WIDTH                     (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS                   (0)) m_spio_sp_main0_main_noc_rbm_s (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk__noc),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_sp_main0_main_noc_rbm_s_aw_id),
         .aw_addr      (spio_sp_main0_main_noc_rbm_s_AWADDR),
         .aw_len       (spio_sp_main0_main_noc_rbm_s_aw_len),
         .aw_size      (spio_sp_main0_main_noc_rbm_s_aw_size),
         .aw_burst     (spio_sp_main0_main_noc_rbm_s_aw_burst),
         .aw_lock      (spio_sp_main0_main_noc_rbm_s_aw_lock),
         .aw_cache     (spio_sp_main0_main_noc_rbm_s_aw_cache),
         .aw_prot      (spio_sp_main0_main_noc_rbm_s_AWPROT),
         .aw_qos       (spio_sp_main0_main_noc_rbm_s_aw_qos),
         .aw_region    (spio_sp_main0_main_noc_rbm_s_aw_region),
         .aw_user      (spio_sp_main0_main_noc_rbm_s_aw_user),
         .aw_valid     (spio_sp_main0_main_noc_rbm_s_AWVALID),
         .aw_ready     (spio_sp_main0_main_noc_rbm_s_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_sp_main0_main_noc_rbm_s_aw_trans_id),

         .w_user       (spio_sp_main0_main_noc_rbm_s_w_user),
         .w_data       (spio_sp_main0_main_noc_rbm_s_WDATA),
         .w_strb       (spio_sp_main0_main_noc_rbm_s_WSTRB),
         .w_last       (spio_sp_main0_main_noc_rbm_s_w_last),
         .w_valid      (spio_sp_main0_main_noc_rbm_s_WVALID),
         .w_ready      (spio_sp_main0_main_noc_rbm_s_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_sp_main0_main_noc_rbm_s_w_trans_id),

         .b_user       (spio_sp_main0_main_noc_rbm_s_b_user),
         .b_id         (spio_sp_main0_main_noc_rbm_s_b_id),
         .b_resp       (spio_sp_main0_main_noc_rbm_s_BRESP),
         .b_valid      (spio_sp_main0_main_noc_rbm_s_BVALID),
         .b_ready      (spio_sp_main0_main_noc_rbm_s_BREADY),
         .b_trans_id   (spio_sp_main0_main_noc_rbm_s_b_trans_id),

         .ar_id        (spio_sp_main0_main_noc_rbm_s_ar_id),
         .ar_addr      (spio_sp_main0_main_noc_rbm_s_ARADDR),
         .ar_len       (spio_sp_main0_main_noc_rbm_s_ar_len),
         .ar_size      (spio_sp_main0_main_noc_rbm_s_ar_size),
         .ar_burst     (spio_sp_main0_main_noc_rbm_s_ar_burst),
         .ar_lock      (spio_sp_main0_main_noc_rbm_s_ar_lock),
         .ar_cache     (spio_sp_main0_main_noc_rbm_s_ar_cache),
         .ar_prot      (spio_sp_main0_main_noc_rbm_s_ARPROT),
         .ar_qos       (spio_sp_main0_main_noc_rbm_s_ar_qos),
         .ar_region    (spio_sp_main0_main_noc_rbm_s_ar_region),
         .ar_user      (spio_sp_main0_main_noc_rbm_s_ar_user),
         .ar_valid     (spio_sp_main0_main_noc_rbm_s_ARVALID),
         .ar_ready     (spio_sp_main0_main_noc_rbm_s_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_sp_main0_main_noc_rbm_s_ar_trans_id),

         .r_last       (spio_sp_main0_main_noc_rbm_s_r_last),
         .r_user       (spio_sp_main0_main_noc_rbm_s_r_user),
         .r_id         (spio_sp_main0_main_noc_rbm_s_r_id),
         .r_data       (spio_sp_main0_main_noc_rbm_s_RDATA),
         .r_resp       (spio_sp_main0_main_noc_rbm_s_RRESP),
         .r_valid      (spio_sp_main0_main_noc_rbm_s_RVALID),
         .r_ready      (spio_sp_main0_main_noc_rbm_s_RREADY),
         .r_trans_id   (spio_sp_main0_main_noc_rbm_s_r_trans_id)
      );

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_p10_pcie_rbm_s_aw_trans_id;
   tri0                                       spio_p10_pcie_rbm_s_aw_qos;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_p10_pcie_rbm_s_aw_user;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_p10_pcie_rbm_s_aw_region;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_p10_pcie_rbm_s_w_trans_id;
   tri0                                       spio_p10_pcie_rbm_s_w_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_p10_pcie_rbm_s_b_trans_id;
   tri0                                       spio_p10_pcie_rbm_s_b_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_p10_pcie_rbm_s_ar_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_p10_pcie_rbm_s_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_p10_pcie_rbm_s_ar_user;
   tri0                                       spio_p10_pcie_rbm_s_ar_qos;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_p10_pcie_rbm_s_r_trans_id;
   tri0                                       spio_p10_pcie_rbm_s_r_user;

   tri0 [3:0]                                 spio_p10_pcie_rbm_s_aw_id;
   tri0 [3:0]                                 spio_p10_pcie_rbm_s_aw_len;
   tri0 [2:0]                                 spio_p10_pcie_rbm_s_aw_size;
   tri0 [1:0]                                 spio_p10_pcie_rbm_s_aw_burst;
   tri0                                       spio_p10_pcie_rbm_s_aw_lock;
   tri0 [3:0]                                 spio_p10_pcie_rbm_s_aw_cache;

   tri1                                       spio_p10_pcie_rbm_s_w_last;

   tri0 [3:0]                                 spio_p10_pcie_rbm_s_b_id;

   tri0 [3:0]                                 spio_p10_pcie_rbm_s_ar_id;
   tri0 [3:0]                                 spio_p10_pcie_rbm_s_ar_len;
   tri0 [2:0]                                 spio_p10_pcie_rbm_s_ar_size;
   tri0 [1:0]                                 spio_p10_pcie_rbm_s_ar_burst;
   tri0                                       spio_p10_pcie_rbm_s_ar_lock;
   tri0 [3:0]                                 spio_p10_pcie_rbm_s_ar_cache;

   tri1                                       spio_p10_pcie_rbm_s_r_last;
   tri0 [3:0]                                 spio_p10_pcie_rbm_s_r_id;

   assign spio_p10_pcie_rbm_s_aw_size = 3'd2;
   assign spio_p10_pcie_rbm_s_ar_size = 3'd2;


   evl_axi_interface #(.SHIRE_ID                        (IOSHIRE_ID),
                       .ID_WIDTH                        (4),
                       .ADDR_WIDTH                      (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH                      (32),
                       .AXSIZE_WIDTH                    (3),
                       .ARUSER_WIDTH                    (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH                    (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH                  (4),
                       .AXQOS_WIDTH                     (1),
                       .AXLEN_WIDTH                     (4),
                       .AW_VC_WIDTH                     (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS                   (0)) m_spio_p10_pcie_rbm_s (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_p10_pcie_rbm_s_aw_id),
         .aw_addr      (spio_p10_pcie_rbm_s_AWADDR),
         .aw_len       (spio_p10_pcie_rbm_s_aw_len),
         .aw_size      (spio_p10_pcie_rbm_s_aw_size),
         .aw_burst     (spio_p10_pcie_rbm_s_aw_burst),
         .aw_lock      (spio_p10_pcie_rbm_s_aw_lock),
         .aw_cache     (spio_p10_pcie_rbm_s_aw_cache),
         .aw_prot      (spio_p10_pcie_rbm_s_AWPROT),
         .aw_qos       (spio_p10_pcie_rbm_s_aw_qos),
         .aw_region    (spio_p10_pcie_rbm_s_aw_region),
         .aw_user      (spio_p10_pcie_rbm_s_aw_user),
         .aw_valid     (spio_p10_pcie_rbm_s_AWVALID),
         .aw_ready     (spio_p10_pcie_rbm_s_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_p10_pcie_rbm_s_aw_trans_id),

         .w_user       (spio_p10_pcie_rbm_s_w_user),
         .w_data       (spio_p10_pcie_rbm_s_WDATA),
         .w_strb       (spio_p10_pcie_rbm_s_WSTRB),
         .w_last       (spio_p10_pcie_rbm_s_w_last),
         .w_valid      (spio_p10_pcie_rbm_s_WVALID),
         .w_ready      (spio_p10_pcie_rbm_s_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_p10_pcie_rbm_s_w_trans_id),

         .b_user       (spio_p10_pcie_rbm_s_b_user),
         .b_id         (spio_p10_pcie_rbm_s_b_id),
         .b_resp       (spio_p10_pcie_rbm_s_BRESP),
         .b_valid      (spio_p10_pcie_rbm_s_BVALID),
         .b_ready      (spio_p10_pcie_rbm_s_BREADY),
         .b_trans_id   (spio_p10_pcie_rbm_s_b_trans_id),

         .ar_id        (spio_p10_pcie_rbm_s_ar_id),
         .ar_addr      (spio_p10_pcie_rbm_s_ARADDR),
         .ar_len       (spio_p10_pcie_rbm_s_ar_len),
         .ar_size      (spio_p10_pcie_rbm_s_ar_size),
         .ar_burst     (spio_p10_pcie_rbm_s_ar_burst),
         .ar_lock      (spio_p10_pcie_rbm_s_ar_lock),
         .ar_cache     (spio_p10_pcie_rbm_s_ar_cache),
         .ar_prot      (spio_p10_pcie_rbm_s_ARPROT),
         .ar_qos       (spio_p10_pcie_rbm_s_ar_qos),
         .ar_region    (spio_p10_pcie_rbm_s_ar_region),
         .ar_user      (spio_p10_pcie_rbm_s_ar_user),
         .ar_valid     (spio_p10_pcie_rbm_s_ARVALID),
         .ar_ready     (spio_p10_pcie_rbm_s_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_p10_pcie_rbm_s_ar_trans_id),

         .r_last       (spio_p10_pcie_rbm_s_r_last),
         .r_user       (spio_p10_pcie_rbm_s_r_user),
         .r_id         (spio_p10_pcie_rbm_s_r_id),
         .r_data       (spio_p10_pcie_rbm_s_RDATA),
         .r_resp       (spio_p10_pcie_rbm_s_RRESP),
         .r_valid      (spio_p10_pcie_rbm_s_RVALID),
         .r_ready      (spio_p10_pcie_rbm_s_RREADY),
         .r_trans_id   (spio_p10_pcie_rbm_s_r_trans_id)
      );

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_regbus_axi_aw_trans_id;
   tri0                                       spio_noc_pu_noc_regbus_axi_aw_qos;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] spio_noc_pu_noc_regbus_axi_aw_user;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_pu_noc_regbus_axi_aw_region;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_regbus_axi_w_trans_id;
   tri0                                       spio_noc_pu_noc_regbus_axi_w_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_regbus_axi_b_trans_id;
   tri0                                       spio_noc_pu_noc_regbus_axi_b_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_regbus_axi_ar_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           spio_noc_pu_noc_regbus_axi_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] spio_noc_pu_noc_regbus_axi_ar_user;
   tri0                                       spio_noc_pu_noc_regbus_axi_ar_qos;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               spio_noc_pu_noc_regbus_axi_r_trans_id;
   tri0                                       spio_noc_pu_noc_regbus_axi_r_user;

   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_aw_id;
   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_aw_len;
   tri0 [2:0]                                 spio_noc_pu_noc_regbus_axi_aw_size;
   tri0 [1:0]                                 spio_noc_pu_noc_regbus_axi_aw_burst;
   tri0                                       spio_noc_pu_noc_regbus_axi_aw_lock;
   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_aw_cache;

   tri1                                       spio_noc_pu_noc_regbus_axi_w_last;

   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_b_id;

   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_ar_id;
   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_ar_len;
   tri0 [2:0]                                 spio_noc_pu_noc_regbus_axi_ar_size;
   tri0 [1:0]                                 spio_noc_pu_noc_regbus_axi_ar_burst;
   tri0                                       spio_noc_pu_noc_regbus_axi_ar_lock;
   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_ar_cache;

   tri1                                       spio_noc_pu_noc_regbus_axi_r_last;
   tri0 [3:0]                                 spio_noc_pu_noc_regbus_axi_r_id;

   assign spio_noc_pu_noc_regbus_axi_aw_size = 3'd2;
   assign spio_noc_pu_noc_regbus_axi_ar_size = 3'd2;


   evl_axi_interface #(.SHIRE_ID                        (IOSHIRE_ID),
                       .ID_WIDTH                        (4),
                       .ADDR_WIDTH                      (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH                      (32),
                       .AXSIZE_WIDTH                    (3),
                       .ARUSER_WIDTH                    (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH                    (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH                  (4),
                       .AXQOS_WIDTH                     (1),
                       .AXLEN_WIDTH                     (4),
                       .AW_VC_WIDTH                     (1),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                       .AW_VC_CREDITS                   (0)) m_spio_noc_pu_noc_regbus_axi (
         .isolate      (1'b0),
         .reset        (reset_ios_ckcore),
         .clk          (clk_ios),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (spio_noc_pu_noc_regbus_axi_aw_id),
         .aw_addr      (spio_noc_pu_noc_regbus_axi_AWADDR),
         .aw_len       (spio_noc_pu_noc_regbus_axi_aw_len),
         .aw_size      (spio_noc_pu_noc_regbus_axi_aw_size),
         .aw_burst     (spio_noc_pu_noc_regbus_axi_aw_burst),
         .aw_lock      (spio_noc_pu_noc_regbus_axi_aw_lock),
         .aw_cache     (spio_noc_pu_noc_regbus_axi_aw_cache),
         .aw_prot      (spio_noc_pu_noc_regbus_axi_AWPROT),
         .aw_qos       (spio_noc_pu_noc_regbus_axi_aw_qos),
         .aw_region    (spio_noc_pu_noc_regbus_axi_aw_region),
         .aw_user      (spio_noc_pu_noc_regbus_axi_aw_user),
         .aw_valid     (spio_noc_pu_noc_regbus_axi_AWVALID),
         .aw_ready     (spio_noc_pu_noc_regbus_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (spio_noc_pu_noc_regbus_axi_aw_trans_id),

         .w_user       (spio_noc_pu_noc_regbus_axi_w_user),
         .w_data       (spio_noc_pu_noc_regbus_axi_WDATA),
         .w_strb       (spio_noc_pu_noc_regbus_axi_WSTRB),
         .w_last       (spio_noc_pu_noc_regbus_axi_w_last),
         .w_valid      (spio_noc_pu_noc_regbus_axi_WVALID),
         .w_ready      (spio_noc_pu_noc_regbus_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (spio_noc_pu_noc_regbus_axi_w_trans_id),

         .b_user       (spio_noc_pu_noc_regbus_axi_b_user),
         .b_id         (spio_noc_pu_noc_regbus_axi_b_id),
         .b_resp       (spio_noc_pu_noc_regbus_axi_BRESP),
         .b_valid      (spio_noc_pu_noc_regbus_axi_BVALID),
         .b_ready      (spio_noc_pu_noc_regbus_axi_BREADY),
         .b_trans_id   (spio_noc_pu_noc_regbus_axi_b_trans_id),

         .ar_id        (spio_noc_pu_noc_regbus_axi_ar_id),
         .ar_addr      (spio_noc_pu_noc_regbus_axi_ARADDR),
         .ar_len       (spio_noc_pu_noc_regbus_axi_ar_len),
         .ar_size      (spio_noc_pu_noc_regbus_axi_ar_size),
         .ar_burst     (spio_noc_pu_noc_regbus_axi_ar_burst),
         .ar_lock      (spio_noc_pu_noc_regbus_axi_ar_lock),
         .ar_cache     (spio_noc_pu_noc_regbus_axi_ar_cache),
         .ar_prot      (spio_noc_pu_noc_regbus_axi_ARPROT),
         .ar_qos       (spio_noc_pu_noc_regbus_axi_ar_qos),
         .ar_region    (spio_noc_pu_noc_regbus_axi_ar_region),
         .ar_user      (spio_noc_pu_noc_regbus_axi_ar_user),
         .ar_valid     (spio_noc_pu_noc_regbus_axi_ARVALID),
         .ar_ready     (spio_noc_pu_noc_regbus_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (spio_noc_pu_noc_regbus_axi_ar_trans_id),

         .r_last       (spio_noc_pu_noc_regbus_axi_r_last),
         .r_user       (spio_noc_pu_noc_regbus_axi_r_user),
         .r_id         (spio_noc_pu_noc_regbus_axi_r_id),
         .r_data       (spio_noc_pu_noc_regbus_axi_RDATA),
         .r_resp       (spio_noc_pu_noc_regbus_axi_RRESP),
         .r_valid      (spio_noc_pu_noc_regbus_axi_RVALID),
         .r_ready      (spio_noc_pu_noc_regbus_axi_RREADY),
         .r_trans_id   (spio_noc_pu_noc_regbus_axi_r_trans_id)
      );

   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   int m_ioshire_id;
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                               IO_SHIRE_RTL_AGENT,
                                                               IOSHIRE_ID,
                                                               "evl_ioshire_env", "", "", "", { 1 });


   //-----------------------------------------------------------------------------------------------
   //
   // Create a Shire RTL port instance and drive it appropriately.
   //
   evl_rtl_port m_rtl_port;
   bit          m_start_monitor = 1'b0;
   bit          m_reset_on      = 1'b0;
   bit          m_reset_off     = 1'b0;
   bit          m_mem_loaded    = 1'b0;
   string       m_rom_file_name = ROM_FILE_NAME;
   string       m_ram_file_name = RAM_FILE_NAME;
   string       m_full_file_name;
   string       m_plus_arg;

   initial begin
      m_ioshire_id = IOSHIRE_ID;
   end

   generate
      if (IOSHIRE_NOC_ONLY == 1'b0) begin :g_sw_tb
         `ifdef USE_HDL_TEST_COMPS
            `ifndef USE_SPIO_ES
               sw_tb_manager_if m_sw_tb_manager_if_0();
               sw_tb_manager_if m_sw_tb_manager_if_1();
               sw_tb_manager_if m_sw_tb_manager_if_2();

               assign m_sw_tb_manager_if_0.preload_sync = reset_ios_ckcore;
               assign m_sw_tb_manager_if_1.preload_sync = maxshire_top.maxneigh.maxion.reset_uncore;

               `ifdef ET_USE_ASIC_MEMS
                  `ifdef IOSHIRE_GATESIM
                     assign #30ns m_sw_tb_manager_if_0.func_id = {pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][32:32],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][28:28],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][24:24],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][20:20],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][16:16],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][12:12],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][8:8],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[144][4:4] };
                     assign #30ns m_sw_tb_manager_if_1.func_id = {pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][32:32],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][28:28],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][24:24],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][20:20],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][16:16],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][12:12],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][8:8],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[128][4:4] };
                     assign #30ns m_sw_tb_manager_if_2.func_id = {pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][32:32],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][28:28],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][24:24],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][20:20],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][16:16],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][12:12],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][8:8],
                                                                  pu.pu_scram_axi.ROWS_0__MEM_ROW_0__mem_ram.uut.mem_core_array[160][4:4] };
                  `elsif ET_USE_BIST_RTL
                     assign #30ns m_sw_tb_manager_if_0.func_id = {pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][32:32],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][28:28],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][24:24],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][20:20],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][16:16],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][12:12],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][8:8],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[144][4:4] };
                     assign #30ns m_sw_tb_manager_if_1.func_id = {pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][32:32],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][28:28],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][24:24],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][20:20],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][16:16],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][12:12],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][8:8],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[128][4:4] };
                     assign #30ns m_sw_tb_manager_if_2.func_id = {pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][32:32],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][28:28],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][24:24],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][20:20],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][16:16],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][12:12],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][8:8],
                                                                  pu.pu_scram_axi.\ROWS[0] .\MEM_ROW[0] .mem_ram.uut.mem_core_array[160][4:4] };

                  `else // !`ifdef IOSHIRE_GATESIM
                     assign #30ns m_sw_tb_manager_if_0.func_id = {pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][32:32],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][28:28],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][24:24],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][20:20],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][16:16],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][12:12],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][8:8],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[144][4:4] };
                     assign #30ns m_sw_tb_manager_if_1.func_id = {pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][32:32],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][28:28],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][24:24],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][20:20],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][16:16],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][12:12],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][8:8],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[128][4:4] };
                     assign #30ns m_sw_tb_manager_if_2.func_id = {pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][32:32],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][28:28],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][24:24],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][20:20],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][16:16],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][12:12],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][8:8],
                                                                  pu.pu_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.uut.mem_core_array[160][4:4] };
                  `endif // !`ifdef IOSHIRE_GATESIM
               `else  // !ET_USE_ASIC_MEMS
                  //         assign #30ns m_sw_tb_manager_if_0.func_id = spio.spio_scram_axi.ROWS[0].MEM_ROW[0].mem_ram.mem[0][7:0];
                  assign #30ns m_sw_tb_manager_if_0.func_id = pu.pu_scram_axi.ROWS[0].MEM_ROW[((`SP_TB_ID_ADDR - 'h20000000)/16)%4].mem_ram.mem[(`SP_TB_ID_ADDR - 'h20000000)/64][7:0];
                  assign #30ns m_sw_tb_manager_if_1.func_id = pu.pu_scram_axi.ROWS[0].MEM_ROW[((`MAX_TB_ID_ADDR - 'h20000000)/16)%4].mem_ram.mem[(`MAX_TB_ID_ADDR - 'h20000000)/64][7:0];
                  assign #30ns m_sw_tb_manager_if_2.func_id = pu.pu_scram_axi.ROWS[0].MEM_ROW[((`MSMIN_TB_ID_ADDR - 'h20000000)/16)%4].mem_ram.mem[(`MSMIN_TB_ID_ADDR - 'h20000000)/64][7:0];
               `endif //ET_USE_ASIC_MEMS

               initial begin
                  uvm_config_db #(virtual sw_tb_manager_if)::set(null, "*", $sformatf("sw_tb_manager_if_0_%0d",IOSHIRE_ID), m_sw_tb_manager_if_0);
                  uvm_config_db #(virtual sw_tb_manager_if)::set(null, "*", $sformatf("sw_tb_manager_if_1"), m_sw_tb_manager_if_1);
                  uvm_config_db #(virtual sw_tb_manager_if)::set(null, "*", $sformatf("sw_tb_manager_if_2"), m_sw_tb_manager_if_2);
               end
            `endif // !ifdef USE_SPIO_ES
         `endif // ifdef USE_HDL_TEST_COMPS
      end
   endgenerate


   initial begin
      #1ns m_start_monitor = 1'b1;
      @(posedge clk_ios);

      `ifndef USE_SPIO_ES
         `ifndef DV_IOSHIRE_NOC_ONLY
            m_full_file_name = evl_tcl_get_root_name(m_rom_file_name);
            if (m_full_file_name != "") begin
               m_plus_arg = $sformatf("%0s=%s", m_full_file_name, "%s");
               if (`et_value_plusargs(m_plus_arg, m_full_file_name) == 0) begin
                  m_full_file_name = "";
               end
               else begin
                  m_full_file_name = evl_tcl_find_file(m_full_file_name, 0);
               end
               if (m_full_file_name == "") begin
                  m_full_file_name = evl_tcl_find_file(m_rom_file_name, 0);
               end
            end
            else if (m_rom_file_name != "") begin
               m_full_file_name = evl_tcl_find_file(m_rom_file_name, 0);
            end
            else begin
               m_full_file_name = "";
            end
            if (m_full_file_name == "") begin
               `evl_log(UVM_NONE, VMOD_LOAD_MEM, m_rtl_port.get_abstract_name(), $sformatf("could not load ROM memory image file \"%s\"", m_rom_file_name))
            end
            else begin
               string test_name;
               int    return_val;

               `evl_log(UVM_MEDIUM, VMOD_LOAD_MEM, m_rtl_port.get_abstract_name(), $sformatf("loading the ROM image file \"%s\"", m_full_file_name))
               return_val = `et_value_plusargs("UVM_TESTNAME=%s", test_name);
               if (test_name.len() == 0) begin
                  bit[7:0] rom_tmp[bit[63:0]];
                  int      hex_file, temp_file;
                  string   temp_string,temp_string_1;
                  bit [63:0] address;
                  bit [511:0] location_val;
                  hex_file = $fopen(m_full_file_name,"r");
                  forever begin
                     temp_file = $fscanf(hex_file,"%s",temp_string);
                     if($feof(hex_file)) break;
                     if(temp_string == "@") begin
                        temp_string = temp_string.substr(1,temp_string.len() - 1);
                        address = temp_string.atohex();
                     end
                     if(temp_string != "@") begin
                        for(int i =0; i < temp_string.len(); i = i + 2) begin
                           temp_string_1 = string'({temp_string[temp_string.len() - i - 2],temp_string[temp_string.len() - i - 1]});
                           rom_tmp[address + i] = temp_string_1.atohex();
                        end
                     end
                  end//forever
                  $fclose(hex_file);
                  foreach(rom_tmp[i]) begin
                     bit[39:0] local_addr;
                     bit [7:0] val;
                     bit       is_write;
                     is_write = 1;
                     local_addr = i;
                     val = rom_tmp[i];
                     `ifdef ET_USE_ASIC_MEMS
                        `ifndef SPIO_ROM_USE_GATE
                           if(local_addr[39:3]%8 == 0) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_0__SPIO_ROM_SEL_0_0_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[0] .SPIO_ROM_SEL_0_0.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[0].SPIO_ROM_SEL_0_0.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_0__SPIO_ROM_SEL_0_0_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[0] .SPIO_ROM_SEL_0_0.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[0].SPIO_ROM_SEL_0_0.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 1) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_1__SPIO_ROM_SEL_0_1_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[1] .SPIO_ROM_SEL_0_1.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[1].SPIO_ROM_SEL_0_1.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_1__SPIO_ROM_SEL_0_1_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[1] .SPIO_ROM_SEL_0_1.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[1].SPIO_ROM_SEL_0_1.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 2) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_2__SPIO_ROM_SEL_0_2_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[2] .SPIO_ROM_SEL_0_2.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[2].SPIO_ROM_SEL_0_2.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_2__SPIO_ROM_SEL_0_2_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[2] .SPIO_ROM_SEL_0_2.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[2].SPIO_ROM_SEL_0_2.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 3) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_3__SPIO_ROM_SEL_0_3_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[3] .SPIO_ROM_SEL_0_3.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[3].SPIO_ROM_SEL_0_3.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_3__SPIO_ROM_SEL_0_3_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[3] .SPIO_ROM_SEL_0_3.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[3].SPIO_ROM_SEL_0_3.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 4) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_4__SPIO_ROM_SEL_0_4_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[4] .SPIO_ROM_SEL_0_4.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[4].SPIO_ROM_SEL_0_4.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_4__SPIO_ROM_SEL_0_4_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[4] .SPIO_ROM_SEL_0_4.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[4].SPIO_ROM_SEL_0_4.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 5) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_5__SPIO_ROM_SEL_0_5_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[5] .SPIO_ROM_SEL_0_5.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[5].SPIO_ROM_SEL_0_5.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_5__SPIO_ROM_SEL_0_5_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[5] .SPIO_ROM_SEL_0_5.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[5].SPIO_ROM_SEL_0_5.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 6) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_6__SPIO_ROM_SEL_0_6_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[6] .SPIO_ROM_SEL_0_6.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[6].SPIO_ROM_SEL_0_6.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_6__SPIO_ROM_SEL_0_6_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[6] .SPIO_ROM_SEL_0_6.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[6].SPIO_ROM_SEL_0_6.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                           if(local_addr[39:3]%8 == 7) begin
                              `ifdef SPIO_GATESIM
                                 location_val = spio.spio_rom_axi.ROWS_0__MEM_ROW_7__SPIO_ROM_SEL_0_7_mem_rom.uut.mem_core_array[local_addr/64];
                              `elsif ET_USE_BIST_RTL
                                 location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[7] .SPIO_ROM_SEL_0_7.mem_rom.uut.mem_core_array[local_addr/64];
                              `else
                                 location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[7].SPIO_ROM_SEL_0_7.mem_rom.uut.mem_core_array[local_addr/64];
                              `endif
                              if(is_write) begin
                                 for(int i=0; i<8;i++) begin
                                    location_val[local_addr[2:0]*8 + i] = val[i];
                                 end//for
                                 `ifdef SPIO_GATESIM
                                    spio.spio_rom_axi.ROWS_0__MEM_ROW_7__SPIO_ROM_SEL_0_7_mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `elsif ET_USE_BIST_RTL
                                    spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[7] .SPIO_ROM_SEL_0_7.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `else
                                    spio.spio_rom_axi.ROWS[0].MEM_ROW[7].SPIO_ROM_SEL_0_7.mem_rom.uut.mem_core_array[local_addr/64] = location_val;
                                 `endif
                              end//if write
                           end//if
                        `endif // SPIO_ROM_USE_GATE
                     `else
                        if(local_addr[39:3]%8 == 0) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[0].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[0] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[0].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[0] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 1) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[1].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[1] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[1].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[1] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 2) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[2].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[2] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[2].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[2] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 3) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[3].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[3] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[3].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[3] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 4) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[4].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[4] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[4].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[4] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 5) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[5].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[5] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[5].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[5] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 6) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[6].mem_rom.mem[local_addr/64];
                           `else
                              location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[6] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[6].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[6] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                        if(local_addr[39:3]%8 == 7) begin
                           `ifndef ET_USE_BIST_RTL
                              location_val = spio.spio_rom_axi.ROWS[0].MEM_ROW[7].mem_rom.mem[local_addr/64];
                           `else
                             location_val = spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[7] .mem_rom.mem[local_addr/64];
                           `endif
                           if(is_write) begin
                              for(int i=0; i<8;i++) begin
                                 location_val[local_addr[2:0]*8 + i] = val[i];
                              end//for
                              `ifndef ET_USE_BIST_RTL
                                 spio.spio_rom_axi.ROWS[0].MEM_ROW[7].mem_rom.mem[local_addr/64] = location_val;
                              `else
                                 spio.spio_rom_axi.\ROWS[0] .\MEM_ROW[7] .mem_rom.mem[local_addr/64] = location_val;
                              `endif
                           end//if write
                        end//if
                     `endif// ET_USE_ASIC_MEMS
                  end//foreach
                  //$readmemh(m_full_file_name, spio.spio_rom_axi.mem_rom.mem);
               end
            end
         `endif // ifndef DV_IOSHIRE_NOC_ONLY
      `endif // ifndef USE_SPIO_ES
   end


   `ifndef ET_USE_ASIC_MEMS
      `ifndef ET_USE_BIST_RTL
         `ifndef IOSHIRE_GATESIM
            `ifndef _IOSHIRE_ECC_SUPPORT
               `define _IOSHIRE_ECC_SUPPORT 1
            `endif // ifndef _IOSHIRE_ECC_SUPPORT
         `endif // ifndef IOSHIRE_GATESIM
      `endif // ifndef ET_USE_BIST_RTL
   `endif // ifndef ET_USE_ASIC_MEMS


   generate
      if (IOSHIRE_NOC_ONLY == 1'b0) begin :g_tag_func
         localparam int lp_mem_0    = 0;
         localparam int lp_mem_1    = (`ICACHE_NR_MEMS_PER_ROW > 0) ? 1 : 0;
         localparam int lp_mem_2    = (`ICACHE_NR_MEMS_PER_ROW > 1) ? 2 : 0;
         localparam int lp_mem_3    = (`ICACHE_NR_MEMS_PER_ROW > 2) ? 3 : 0;

         localparam int lp_tag_way0 = 0;
         localparam int lp_tag_way1 = (`ICACHE_WAYS < 1) ? 0 : 1;
         localparam int lp_tag_way2 = (`ICACHE_WAYS < 2) ? 0 : 2;
         localparam int lp_tag_way3 = (`ICACHE_WAYS < 3) ? 0 : 3;


         `ifdef _IOSHIRE_ECC_SUPPORT

            //--------------------------------------------------------------------------------------
            //
            // Function: mod_get_cache_tag
            //
            function automatic evl_paddr_t mod_get_cache_tag(input evl_paddr_t cindex,
                                                             input int         cway);
               int         set;
               int         way;
               evl_paddr_t index_mask;
               evl_paddr_t paddr;

               index_mask = ((evl_paddr_t'(1) << (`EVL_PADDR_WIDTH - `ICACHE_PA_TAG_SIZE)) - 1) & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));
               set        = (int'(cindex) >> `EVL_LINE_ADDR_LSB) & (`ICACHE_SETS - 1);
               way        = cway % `ICACHE_WAYS;
               paddr      = ~evl_paddr_t'(0);
               if (spio.sp.standalone_minion.icache.tag_array.f0_tag_valid[set][way] === 1'b1) begin
                  case (way)
                     0: paddr = (cindex & index_mask) | ((evl_paddr_t'(spio.sp.standalone_minion.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way0].rf_tag_data_array.rf[set])) << `ICACHE_DATA_ADDR_SIZE);
                     1: paddr = (cindex & index_mask) | ((evl_paddr_t'(spio.sp.standalone_minion.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way1].rf_tag_data_array.rf[set])) << `ICACHE_DATA_ADDR_SIZE);
                     2: paddr = (cindex & index_mask) | ((evl_paddr_t'(spio.sp.standalone_minion.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way2].rf_tag_data_array.rf[set])) << `ICACHE_DATA_ADDR_SIZE);
                     3: paddr = (cindex & index_mask) | ((evl_paddr_t'(spio.sp.standalone_minion.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way3].rf_tag_data_array.rf[set])) << `ICACHE_DATA_ADDR_SIZE);
                  endcase
               end
               return paddr;
            endfunction : mod_get_cache_tag


            //--------------------------------------------------------------------------------------
            //
            // Function: mod_get_cache_data
            //
            function automatic int mod_get_cache_data(output evl_dword_t cdata[],
                                                      input  evl_paddr_t caddr,
                                                      input  int         cway);
               int         set;
               int         mem;
               bit [143:0] qword0;
               bit [143:0] qword1;
               bit [143:0] qword2;
               bit [143:0] qword3;
               evl_paddr_t paddr;

               qword0 = {144{1'b1}};
               qword1 = {144{1'b1}};
               qword2 = {144{1'b1}};
               qword3 = {144{1'b1}};
               cdata  = new[`EVL_LINE_DWORDS];
               paddr  = ((caddr >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_SETS * `ICACHE_WAYS) - 1);
               set    = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
               mem    = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS)) & (`ICACHE_NR_MEMS_PER_ROW - 1);
               qword0 = spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[0].mem.mem[set];
               qword1 = spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_1].mem.mem[set];
               qword2 = spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_2].mem.mem[set];
               qword3 = spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_3].mem.mem[set];
               cdata[0] = qword0[63:0];
               cdata[1] = qword0[127:64];
               cdata[2] = qword1[63:0];
               cdata[3] = qword1[127:64];
               cdata[4] = qword2[63:0];
               cdata[5] = qword2[127:64];
               cdata[6] = qword3[63:0];
               cdata[7] = qword3[127:64];
               return 1;
            endfunction : mod_get_cache_data


            //-----------------------------------------------------------------------------------
            //
            // Function: mod_set_cache_bit_err
            //
            function automatic int mod_set_cache_bit_err(input int cindex,
                                                         input int cbit0,
                                                         input int cbit1 = -1,
                                                         input int cway  = 0,
                                                         input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
               int         set;
               int         mem;
               int         err_bit0;
               int         err_bit1;
               int         err_type;
               evl_paddr_t paddr;

               paddr = ((cindex >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_SETS * `ICACHE_WAYS) - 1);
               set   = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
               mem   = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS)) & (`ICACHE_NR_MEMS_PER_ROW - 1);
               err_bit0 = cbit0;
               err_bit1 = cbit1;
               err_type = cerr;
               if ((cbit0 > 71) || (cbit1 > 71)) begin
                  return -1;
               end
               if ((cindex & 8) != 0) begin
                  if (err_bit0 >= 0) begin
                     err_bit0 += 72;
                  end
                  if (err_bit1 >= 0) begin
                     err_bit1 += 72;
                  end
               end
               if (cbit0 < 0) begin
                  err_type = int'(EVL_ECC_ERR_NONE);
               end

               if (mem == 0) begin
                  return spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[0].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
               end
               else if ((`ICACHE_NR_MEMS_PER_ROW > 1) && (mem == 1)) begin
                  return spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_1].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
               end
               else if ((`ICACHE_NR_MEMS_PER_ROW > 2) && (mem == 2)) begin
                  return spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_2].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
               end
               else if ((`ICACHE_NR_MEMS_PER_ROW > 3) && (mem == 3)) begin
                  return spio.sp.standalone_minion.icache_data_ram.ROWS[0].MEM_ROW[lp_mem_3].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
               end
               return -1;
            endfunction : mod_set_cache_bit_err


            class my_ioshire_rtl_port extends evl_ioshire_rtl_port;

               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_set
               //
               virtual function int get_cache_set(input evl_paddr_t caddr,
                                                  input int         cway  = 0,
                                                  input int         ctype = 0);
                  evl_paddr_t paddr;
                  int         set;

                  if (!(evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA })) begin
                     return caddr;
                  end
                  paddr = ((caddr >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_MEM_WORDS * `ICACHE_NR_MEMS_PER_ROW) - 1);
                  set   = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
                  return set;
               endfunction : get_cache_set


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_bank
               //
               virtual function int get_cache_bank(input evl_paddr_t caddr,
                                                   input int         cway  = 0,
                                                   input int         ctype = 0);
                  return 0;
               endfunction : get_cache_bank


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_subbank
               //
               virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                                      input int         cway  = 0,
                                                      input int         ctype = 0);
                  return 0;
               endfunction : get_cache_subbank


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_tag
               //
               virtual function evl_paddr_t get_cache_tag(input int cindex,
                                                          input int cway  = 0,
                                                          input int ctype = 0);
                  if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_TAG, SC_RAM_TYPE_ICACHE0_TAG, SC_RAM_TYPE_ICACHE1_TAG, SC_RAM_TYPE_ICACHE2_TAG, SC_RAM_TYPE_ICACHE3_TAG }) begin
                     return mod_get_cache_tag(evl_paddr_t'(cindex), cway);
                  end
                  return ~evl_paddr_t'(0);
               endfunction : get_cache_tag


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_data
               //
               virtual function int get_cache_data(output evl_dword_t cdata[],
                                                   input  int         cindex,
                                                   input  int         cway  = 0,
                                                   input  int         ctype = 0);
                  if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA }) begin
                     return mod_get_cache_data(cdata, cindex, cway);
                  end
                  return -1;
               endfunction : get_cache_data


               //-----------------------------------------------------------------------------------
               //
               // Function: set_cache_bit_err
               //
               virtual function int set_cache_bit_err(input int cindex,
                                                      input int cbit0,
                                                      input int cbit1 = -1,
                                                      input int cway  = 0,
                                                      input int ctype = 0,
                                                      input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
                  if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA, SC_RAM_TYPE_ICACHE1_DATA, SC_RAM_TYPE_ICACHE2_DATA, SC_RAM_TYPE_ICACHE3_DATA }) begin
                     if (mod_set_cache_bit_err(cindex, cbit0, cbit1, cway, cerr) <= 0) begin
                        return -1;
                     end
                  end
                  return 1;
               endfunction : set_cache_bit_err


               //-----------------------------------------------------------------------------------
               //
               // Constructor
               //
               function new(input string         full_name_in     = "evl_shire_channel_rtl_port",
                            input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                            input string         param_list_in    = "",
                            input int            skip_levels_in[] = {});
                  super.new(full_name_in, agent_pid_in, param_list_in, skip_levels_in);
               endfunction
            endclass : my_ioshire_rtl_port


            my_ioshire_rtl_port my_rtl_port = new($sformatf("%m"), IOSHIRE_ID, "", { 1 });

         `else // ifdef _IOSHIRE_ECC_SUPPORT

            evl_ioshire_rtl_port my_rtl_port = new($sformatf("%m"), IOSHIRE_ID, "", { 1 });

         `endif // !ifdef _IOSHIRE_ECC_SUPPORT

         initial begin
            m_rtl_port = my_rtl_port;
            m_rtl_port.set_int_param_by_name("pk_sp_is_stub", ((SP_IS_STUB == 0) ? 0 : 1));
            m_rtl_port.set_int_param_by_name("pk_noc_is_stub", ((NOC_IS_STUB == 0) ? 0 : 1));
            m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_PORTS);
            m_rtl_port.set_int_param_by_name("pk_num_l3_master_ports", `MAXSHIRE_SC_NUM_L3_MASTER_PORTS);
            m_rtl_port.set_int_param_by_name("pk_num_l3_slave_ports", `MAXSHIRE_SC_NUM_L3_SLAVE_PORTS);
            m_rtl_port.set_int_param_by_name("pk_io_noc_only", IOSHIRE_NOC_ONLY);
            m_rtl_port.set_int_param_by_name("pk_io_no_pu", IOSHIRE_NO_PU);
         end

      end // if (IOSHIRE_NOC_ONLY == 1'b0)

      else begin

         evl_ioshire_rtl_port my_rtl_port = new($sformatf("%m"), IOSHIRE_ID, "", { 1 });

         initial begin
            m_rtl_port = my_rtl_port;
            m_rtl_port.set_int_param_by_name("pk_sp_is_stub", ((SP_IS_STUB == 0) ? 0 : 1));
            m_rtl_port.set_int_param_by_name("pk_noc_is_stub", ((NOC_IS_STUB == 0) ? 0 : 1));
            m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_PORTS);
            m_rtl_port.set_int_param_by_name("pk_num_l3_master_ports", `MAXSHIRE_SC_NUM_L3_MASTER_PORTS);
            m_rtl_port.set_int_param_by_name("pk_num_l3_slave_ports", `MAXSHIRE_SC_NUM_L3_SLAVE_PORTS);
            m_rtl_port.set_int_param_by_name("pk_io_noc_only", IOSHIRE_NOC_ONLY);
            m_rtl_port.set_int_param_by_name("pk_io_no_pu", IOSHIRE_NO_PU);
         end

      end // !if (IOSHIRE_NOC_ONLY == 1'b0)
   endgenerate

   always @(posedge clk_ios) begin
      if (m_start_monitor === 1'b1) begin
         if (reset_ios_ckcore === 1'b1) begin
            if (m_reset_on === 1'b0) begin
               m_rtl_port.assert_reset();
            end
         end
         else if (reset_ios_ckcore === 1'b0) begin
            if (m_reset_off === 1'b0) begin
               m_rtl_port.deassert_reset();
            end
         end
         m_rtl_port.ping_clock();
      end
   end


///////////////////////////////////////////////
/////////////   COVERAGE   ////////////////////
///////////////////////////////////////////////

`ifdef EVL_COVERAGE_ON
  `ifndef DV_IOSHIRE_NOC_ONLY

    `include "dv/soc/ioshire/ioshire_cov_capture.svh"

  `endif // ifndef DV_IOSHIRE_NOC_ONLY
`endif // ifdef EVL_COVERAGE_ON

endmodule

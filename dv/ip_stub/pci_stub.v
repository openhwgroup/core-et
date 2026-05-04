`include "soc.vh"
`include "axi_defines.vh"
`include "shire_cache_defines.vh"
`include "macros.vh"
`include "mesh_defines.vh"

module pci_stub
    #(
     parameter int   PSHIRE_ID      = 253,
     parameter int   NUM_L3_PORTS   = `SC_L3_MASTER_PORTS,
     parameter bit   DV_PSHIRE_STIM = 1'b0
    )
(
        //noc stamped
      input reset_n_system,
      input reset_n_system_debug,

      input clk__noc,

      `include "pshire_ns_meshtop_ports.v" 
      `include "pshire_ns_debug_meshtop_ports.v"
        //global
      input logic clock,
      input logic reset
    );


    //PS slave
    `EVL_WIRE sc_slave_axi_AR_channel_t   ps0_ps_AR;
    wire                                  ps0_ps_ARVALID;
    wire                                  ps0_ps_ARREADY;
    `EVL_WIRE sc_slave_axi_AW_channel_t   ps0_ps_AW;
    wire                                  ps0_ps_AWVALID;
    wire                                  ps0_ps_AWREADY;
    `EVL_WIRE sc_slave_axi_W_channel_t    ps0_ps_W;
    wire                                  ps0_ps_WVALID;
    wire                                  ps0_ps_WREADY;
    `EVL_WIRE sc_slave_axi_B_channel_t    ps0_ps_B;
    wire                                  ps0_ps_BVALID;
    wire                                  ps0_ps_BREADY;
    `EVL_WIRE sc_slave_axi_R_channel_t    ps0_ps_R;
    wire                                  ps0_ps_RVALID;
    wire                                  ps0_ps_RREADY;

    //UC to SYS master
    `EVL_WIRE sc_master_axi_AR_channel_t  ps0_tosys_m_AR;
    wire                                  ps0_tosys_m_ARVALID;
    wire                                  ps0_tosys_m_ARREADY;
    `EVL_WIRE sc_master_axi_AW_channel_t  ps0_tosys_m_AW;
    wire                                  ps0_tosys_m_AWVALID;
    wire                                  ps0_tosys_m_AWREADY;
    `EVL_WIRE sc_master_axi_W_channel_t   ps0_tosys_m_W;
    wire                                  ps0_tosys_m_WVALID;
    wire                                  ps0_tosys_m_WREADY;
    `EVL_WIRE sc_master_axi_B_channel_t   ps0_tosys_m_B;
    wire                                  ps0_tosys_m_BVALID;
    wire                                  ps0_tosys_m_BREADY;
    `EVL_WIRE sc_master_axi_R_channel_t   ps0_tosys_m_R;
    wire                                  ps0_tosys_m_RVALID;
    wire                                  ps0_tosys_m_RREADY;

    //UC to SYS master
    `EVL_WIRE sc_master_axi_AR_channel_t  [NUM_L3_PORTS-1:0]   ps0_tol3_m_AR;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_ARVALID;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_ARREADY;
    `EVL_WIRE sc_master_axi_AW_channel_t  [NUM_L3_PORTS-1:0]   ps0_tol3_m_AW;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_AWVALID;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_AWREADY;
    `EVL_WIRE sc_master_axi_W_channel_t   [NUM_L3_PORTS-1:0]   ps0_tol3_m_W;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_WVALID;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_WREADY;
    `EVL_WIRE sc_master_axi_B_channel_t   [NUM_L3_PORTS-1:0]   ps0_tol3_m_B;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_BVALID;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_BREADY;
    `EVL_WIRE sc_master_axi_R_channel_t   [NUM_L3_PORTS-1:0]   ps0_tol3_m_R;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_RVALID;
    wire                                  [NUM_L3_PORTS-1:0]   ps0_tol3_m_RREADY;


    sc_slave_axi_AR_channel_t ps0_ps_int_AR;
    logic                     ps0_ps_int_ARVALID;
    logic                     ps0_ps_int_ARREADY;
    sc_slave_axi_AW_channel_t ps0_ps_int_AW;
    logic                     ps0_ps_int_AWVALID;
    logic                     ps0_ps_int_AWREADY;
    sc_slave_axi_W_channel_t  ps0_ps_int_W;
    logic                     ps0_ps_int_WVALID;
    logic                     ps0_ps_int_WREADY;
    sc_slave_axi_B_channel_t  ps0_ps_int_B;
    logic                     ps0_ps_int_BVALID;
    logic                     ps0_ps_int_BREADY;
    sc_slave_axi_R_channel_t  ps0_ps_int_R;
    logic                     ps0_ps_int_RVALID;
    logic                     ps0_ps_int_RREADY;

    wire  [5:0] p0_spio_m_AWID;
    wire [39:0] p0_spio_m_AWADDR;
    wire  [7:0] p0_spio_m_AWLEN;
    wire  [2:0] p0_spio_m_AWSIZE;
    wire  [1:0] p0_spio_m_AWBURST;
    wire        p0_spio_m_AWLOCK;
    wire  [3:0] p0_spio_m_AWCACHE;
    wire  [2:0] p0_spio_m_AWPROT;
    wire  [3:0] p0_spio_m_AWQOS;
    wire        p0_spio_m_AWVALID;
    wire        p0_spio_m_AWREADY;
    wire [31:0] p0_spio_m_WDATA;
    wire  [3:0] p0_spio_m_WSTRB;
    wire        p0_spio_m_WLAST;
    wire        p0_spio_m_WVALID;
    wire        p0_spio_m_WREADY;
    wire        p0_spio_m_BREADY;
    wire  [5:0] p0_spio_m_BID;
    wire  [1:0] p0_spio_m_BRESP;
    wire        p0_spio_m_BVALID;
    wire  [5:0] p0_spio_m_ARID;
    wire [39:0] p0_spio_m_ARADDR;
    wire  [7:0] p0_spio_m_ARLEN;
    wire  [2:0] p0_spio_m_ARSIZE;
    wire  [1:0] p0_spio_m_ARBURST;
    wire        p0_spio_m_ARLOCK;
    wire  [3:0] p0_spio_m_ARCACHE;
    wire  [2:0] p0_spio_m_ARPROT;
    wire  [3:0] p0_spio_m_ARQOS;
    wire        p0_spio_m_ARVALID;
    wire        p0_spio_m_ARREADY;
    wire        p0_spio_m_RREADY;
    wire  [5:0] p0_spio_m_RID;
    wire [31:0] p0_spio_m_RDATA;
    wire  [1:0] p0_spio_m_RRESP;
    wire        p0_spio_m_RLAST;
    wire        p0_spio_m_RVALID;

    wire [31:0] rbm_m_awaddr;
    wire  [2:0] rbm_m_awprot;
    wire        rbm_m_awvalid;
    wire        rbm_m_awready;
    wire [31:0] rbm_m_wdata;
    wire  [3:0] rbm_m_wstrb;
    wire        rbm_m_wvalid;
    wire        rbm_m_wready;
    wire        rbm_m_bready;
    wire  [1:0] rbm_m_bresp;
    wire        rbm_m_bvalid;
    wire [31:0] rbm_m_araddr;
    wire  [2:0] rbm_m_arprot;
    wire        rbm_m_arvalid;
    wire        rbm_m_arready;
    wire        rbm_m_rready;
    wire [31:0] rbm_m_rdata;
    wire  [1:0] rbm_m_rresp;
    wire        rbm_m_rvalid;


    wire[31:0]         apbslv_paddr;  
    wire               apbslv_pclk;   
    wire               apbslv_penable;
    wire               apbslv_presetn;
    wire               apbslv_psel;   
    wire[3:0]          apbslv_pstrb;  
    wire[31:0]         apbslv_pwdata; 
    wire               apbslv_pwrite; 
    wire[31:0]         apbslv_prdata; 
    wire               apbslv_pready; 
    wire               apbslv_pslverr;


    wire[31:0]         pcie0_dbi_araddr; 
    wire[1:0]          pcie0_dbi_arburst;
    wire[3:0]          pcie0_dbi_arcache;
    wire[3:0]          pcie0_dbi_arid;   
    wire[3:0]          pcie0_dbi_arlen;  
    wire               pcie0_dbi_arlock; 
    wire[2:0]          pcie0_dbi_arprot; 
    wire[3:0]          pcie0_dbi_arqos;  
    wire[2:0]          pcie0_dbi_arsize; 
    wire               pcie0_dbi_arvalid;
    wire[31:0]         pcie0_dbi_awaddr; 
    wire[1:0]          pcie0_dbi_awburst;
    wire[3:0]          pcie0_dbi_awcache;
    wire[3:0]          pcie0_dbi_awid;   
    wire[3:0]          pcie0_dbi_awlen;  
    wire               pcie0_dbi_awlock; 
    wire[2:0]          pcie0_dbi_awprot; 
    wire[3:0]          pcie0_dbi_awqos;  
    wire[2:0]          pcie0_dbi_awsize; 
    wire               pcie0_dbi_awvalid;
    wire               pcie0_dbi_bready; 
    wire               pcie0_dbi_rready; 
    wire[31:0]         pcie0_dbi_wdata;  
    wire               pcie0_dbi_wlast;  
    wire[3:0]          pcie0_dbi_wstrb;  
    wire               pcie0_dbi_wvalid; 
    wire               pcie0_dbi_arready; 
    wire               pcie0_dbi_awready; 
    wire[3:0]          pcie0_dbi_bid;     
    wire[1:0]          pcie0_dbi_bresp;   
    wire               pcie0_dbi_bvalid;  
    wire[31:0]         pcie0_dbi_rdata;   
    wire[3:0]          pcie0_dbi_rid;     
    wire               pcie0_dbi_rlast;   
    wire[1:0]          pcie0_dbi_rresp;   
    wire               pcie0_dbi_rvalid;  
    wire               pcie0_dbi_wready;  


    wire               pcie0_mstr_arready;
    wire               pcie0_mstr_awready;
    wire[4:0]          pcie0_mstr_bid;    
    wire[1:0]          pcie0_mstr_bresp;  
    wire               pcie0_mstr_bvalid; 
    wire[255:0]        pcie0_mstr_rdata;  
    wire[4:0]          pcie0_mstr_rid;    
    wire               pcie0_mstr_rlast;  
    wire[1:0]          pcie0_mstr_rresp;  
    wire               pcie0_mstr_rvalid; 
    wire               pcie0_mstr_wready; 
    wire[63:0]         pcie0_mstr_araddr; 
    wire[1:0]          pcie0_mstr_arburst;
    wire[3:0]          pcie0_mstr_arcache;
    wire[4:0]          pcie0_mstr_arid;   
    wire[2:0]          pcie0_mstr_arlen;  
    wire               pcie0_mstr_arlock; 
    wire[2:0]          pcie0_mstr_arprot; 
    wire[3:0]          pcie0_mstr_arqos;  
    wire[2:0]          pcie0_mstr_arsize; 
    wire               pcie0_mstr_arvalid;
    wire[63:0]         pcie0_mstr_awaddr; 
    wire[1:0]          pcie0_mstr_awburst;
    wire[3:0]          pcie0_mstr_awcache;
    wire[4:0]          pcie0_mstr_awid;   
    wire[2:0]          pcie0_mstr_awlen;  
    wire               pcie0_mstr_awlock; 
    wire[2:0]          pcie0_mstr_awprot; 
    wire[3:0]          pcie0_mstr_awqos;  
    wire[2:0]          pcie0_mstr_awsize; 
    wire               pcie0_mstr_awvalid;
    wire               pcie0_mstr_bready; 
    wire               pcie0_mstr_rready; 
    wire[255:0]        pcie0_mstr_wdata;  
    wire               pcie0_mstr_wlast;  
    wire[31:0]         pcie0_mstr_wstrb;  
    wire               pcie0_mstr_wvalid; 


    wire[63:0]         pcie0_slv_araddr; 
    wire[1:0]          pcie0_slv_arburst;
    wire[3:0]          pcie0_slv_arcache;
    wire[15:0]         pcie0_slv_arid;   
    wire[2:0]          pcie0_slv_arlen;  
    wire               pcie0_slv_arlock; 
    wire[2:0]          pcie0_slv_arprot; 
    wire[3:0]          pcie0_slv_arqos;  
    wire[2:0]          pcie0_slv_arsize; 
    wire               pcie0_slv_arvalid;
    wire[63:0]         pcie0_slv_awaddr; 
    wire[1:0]          pcie0_slv_awburst;
    wire[3:0]          pcie0_slv_awcache;
    wire[15:0]         pcie0_slv_awid;   
    wire[2:0]          pcie0_slv_awlen;  
    wire               pcie0_slv_awlock; 
    wire[2:0]          pcie0_slv_awprot; 
    wire[3:0]          pcie0_slv_awqos;  
    wire[2:0]          pcie0_slv_awsize; 
    wire               pcie0_slv_awvalid;
    wire               pcie0_slv_bready; 
    wire               pcie0_slv_rready; 
    wire[255:0]        pcie0_slv_wdata;  
    wire               pcie0_slv_wlast;  
    wire[31:0]         pcie0_slv_wstrb;  
    wire               pcie0_slv_wvalid; 
    wire               pcie0_slv_arready; 
    wire               pcie0_slv_awready; 
    wire[15:0]         pcie0_slv_bid;     
    wire[1:0]          pcie0_slv_bresp;   
    wire               pcie0_slv_bvalid;  
    wire[255:0]        pcie0_slv_rdata;   
    wire[15:0]         pcie0_slv_rid;     
    wire               pcie0_slv_rlast;   
    wire[1:0]          pcie0_slv_rresp;   
    wire               pcie0_slv_rvalid;  
    wire               pcie0_slv_wready;  


    wire[31:0]         pcie1_dbi_araddr;  
    wire[1:0]          pcie1_dbi_arburst; 
    wire[3:0]          pcie1_dbi_arcache; 
    wire[3:0]          pcie1_dbi_arid;    
    wire[3:0]          pcie1_dbi_arlen;   
    wire               pcie1_dbi_arlock;  
    wire[2:0]          pcie1_dbi_arprot;  
    wire[3:0]          pcie1_dbi_arqos;   
    wire[2:0]          pcie1_dbi_arsize;  
    wire               pcie1_dbi_arvalid; 
    wire[31:0]         pcie1_dbi_awaddr;  
    wire[1:0]          pcie1_dbi_awburst; 
    wire[3:0]          pcie1_dbi_awcache; 
    wire[3:0]          pcie1_dbi_awid;    
    wire[3:0]          pcie1_dbi_awlen;   
    wire               pcie1_dbi_awlock;  
    wire[2:0]          pcie1_dbi_awprot;  
    wire[3:0]          pcie1_dbi_awqos;   
    wire[2:0]          pcie1_dbi_awsize;  
    wire               pcie1_dbi_awvalid; 
    wire               pcie1_dbi_bready;  
    wire               pcie1_dbi_rready;  
    wire[31:0]         pcie1_dbi_wdata;   
    wire               pcie1_dbi_wlast;   
    wire[3:0]          pcie1_dbi_wstrb;   
    wire               pcie1_dbi_wvalid;  
    wire               pcie1_dbi_arready; 
    wire               pcie1_dbi_awready; 
    wire[3:0]          pcie1_dbi_bid;     
    wire[1:0]          pcie1_dbi_bresp;   
    wire               pcie1_dbi_bvalid;  
    wire[31:0]         pcie1_dbi_rdata;   
    wire[3:0]          pcie1_dbi_rid;     
    wire               pcie1_dbi_rlast;   
    wire[1:0]          pcie1_dbi_rresp;   
    wire               pcie1_dbi_rvalid;  
    wire               pcie1_dbi_wready;  


    wire               pcie1_mstr_arready;
    wire               pcie1_mstr_awready;
    wire[4:0]          pcie1_mstr_bid;    
    wire[1:0]          pcie1_mstr_bresp;  
    wire               pcie1_mstr_bvalid; 
    wire[127:0]        pcie1_mstr_rdata;  
    wire[4:0]          pcie1_mstr_rid;    
    wire               pcie1_mstr_rlast;  
    wire[1:0]          pcie1_mstr_rresp;  
    wire               pcie1_mstr_rvalid; 
    wire               pcie1_mstr_wready; 
    wire[63:0]         pcie1_mstr_araddr; 
    wire[1:0]          pcie1_mstr_arburst;
    wire[3:0]          pcie1_mstr_arcache;
    wire[4:0]          pcie1_mstr_arid;   
    wire[2:0]          pcie1_mstr_arlen;  
    wire               pcie1_mstr_arlock; 
    wire[2:0]          pcie1_mstr_arprot; 
    wire[3:0]          pcie1_mstr_arqos;  
    wire[2:0]          pcie1_mstr_arsize; 
    wire               pcie1_mstr_arvalid;
    wire[63:0]         pcie1_mstr_awaddr; 
    wire[1:0]          pcie1_mstr_awburst;
    wire[3:0]          pcie1_mstr_awcache;
    wire[4:0]          pcie1_mstr_awid;   
    wire[2:0]          pcie1_mstr_awlen;  
    wire               pcie1_mstr_awlock; 
    wire[2:0]          pcie1_mstr_awprot; 
    wire[3:0]          pcie1_mstr_awqos;  
    wire[2:0]          pcie1_mstr_awsize; 
    wire               pcie1_mstr_awvalid;
    wire               pcie1_mstr_bready; 
    wire               pcie1_mstr_rready; 
    wire[127:0]        pcie1_mstr_wdata;  
    wire               pcie1_mstr_wlast;  
    wire[15:0]         pcie1_mstr_wstrb;  
    wire               pcie1_mstr_wvalid; 


    wire[63:0]         pcie1_slv_araddr;  
    wire[1:0]          pcie1_slv_arburst; 
    wire[3:0]          pcie1_slv_arcache; 
    wire[15:0]         pcie1_slv_arid;    
    wire[2:0]          pcie1_slv_arlen;   
    wire               pcie1_slv_arlock;  
    wire[2:0]          pcie1_slv_arprot;  
    wire[3:0]          pcie1_slv_arqos;   
    wire[2:0]          pcie1_slv_arsize;  
    wire               pcie1_slv_arvalid; 
    wire[63:0]         pcie1_slv_awaddr;  
    wire[1:0]          pcie1_slv_awburst; 
    wire[3:0]          pcie1_slv_awcache; 
    wire[15:0]         pcie1_slv_awid;    
    wire[2:0]          pcie1_slv_awlen;   
    wire               pcie1_slv_awlock;  
    wire[2:0]          pcie1_slv_awprot;  
    wire[3:0]          pcie1_slv_awqos;   
    wire[2:0]          pcie1_slv_awsize;  
    wire               pcie1_slv_awvalid; 
    wire               pcie1_slv_bready;  
    wire               pcie1_slv_rready;  
    wire[127:0]        pcie1_slv_wdata;   
    wire               pcie1_slv_wlast;   
    wire[15:0]         pcie1_slv_wstrb;   
    wire               pcie1_slv_wvalid;  
    wire               pcie1_slv_arready; 
    wire               pcie1_slv_awready; 
    wire[15:0]         pcie1_slv_bid;     
    wire[1:0]          pcie1_slv_bresp;   
    wire               pcie1_slv_bvalid;  
    wire[127:0]        pcie1_slv_rdata;   
    wire[15:0]         pcie1_slv_rid;     
    wire               pcie1_slv_rlast;   
    wire[1:0]          pcie1_slv_rresp;   
    wire               pcie1_slv_rvalid;  
    wire               pcie1_slv_wready;

     //utsoc
  wor      [1:0] host_dest_int_defer_sh0_dn_msg   = '0;
  wor      [1:0] host_dest_int_defer_sh0_dn_evt   = '0;
  wor      [7:0] host_dest_hp_defer_sh0_dn_msg    = '0;
  wor      [7:0] host_dest_hp_defer_sh0_dn_evt    = '0;
  wor      [3:0] host_xfer_qos_defer_sh0_dn_msg   = '0;
  wor      [3:0] host_xfer_qos_defer_sh0_dn_evt   = '0;
  wor            host_beat_sop_defer_sh0_dn_msg   = '0;
  wor            host_beat_sop_defer_sh0_dn_evt   = '0;
  wor            host_beat_eop_defer_sh0_dn_msg   = '0;
  wor            host_beat_eop_defer_sh0_dn_evt   = '0;
  wor            host_beat_valid_defer_sh0_dn_msg = '0;
  wor            host_beat_valid_defer_sh0_dn_evt = '0;
  wor    [127:0] host_beat_data_defer_sh0_dn_msg  = '0;
  wor     [31:0] host_beat_data_defer_sh0_dn_evt  = '0;
  wire           noc_credit_inc_defer_sh0_dn_msg;
  wire           noc_credit_inc_defer_sh0_dn_evt;
  wire           noc_beat_sop_defer_sh0_dn_msg;
  wire           noc_beat_sop_defer_sh0_dn_evt;
  wire           noc_beat_eop_defer_sh0_dn_msg;
  wire           noc_beat_eop_defer_sh0_dn_evt;
  wire           noc_beat_valid_defer_sh0_dn_msg;
  wire           noc_beat_valid_defer_sh0_dn_evt;
  wire   [127:0] noc_beat_data_defer_sh0_dn_msg;
  wire    [31:0] noc_beat_data_defer_sh0_dn_evt;
  wire           host_credit_inc_defer_sh0_dn_msg = '0;
  wire           host_credit_inc_defer_sh0_dn_evt = '0;
  wire           scan_mode = 1'b0;



    if (DV_PSHIRE_STIM == 1'b0) begin
       //tie to '0' unused mesh signals
       assign ps0_tol3_m_AR[0].USER = '0;
       //assign ps0_tol3_m_AR[1].USER = '0;
       //assign ps0_tol3_m_AR[2].USER = '0;
       //assign ps0_tol3_m_AR[3].USER = '0;

       //assign ps0_ps_B.ID   = ps0_ps_int_B.ID;
       //assign ps0_ps_B.RESP = ps0_ps_int_B.RESP;
       //assign ps0_ps_BVALID = ps0_ps_int_BVALID;

       //assign ps0_ps_R.ID   = ps0_ps_int_R.ID;
       //assign ps0_ps_R.RESP = ps0_ps_int_R.RESP;
       //assign ps0_ps_R.LAST = ps0_ps_int_R.LAST;
       //assign ps0_ps_RVALID = ps0_ps_int_RVALID;

       assign ps0_ps_int_AR      = ps0_ps_AR;
       assign ps0_ps_int_ARVALID = ps0_ps_ARVALID;
       assign ps0_ps_ARREADY     = ps0_ps_int_ARREADY;
       assign ps0_ps_int_AW      = ps0_ps_AW;
       assign ps0_ps_int_AWVALID = ps0_ps_AWVALID;
       assign ps0_ps_AWREADY     = ps0_ps_int_AWREADY;
       assign ps0_ps_int_W       = ps0_ps_W;
       assign ps0_ps_int_WVALID  = ps0_ps_WVALID;
       assign ps0_ps_WREADY      = ps0_ps_int_WREADY;
       assign ps0_ps_B           = ps0_ps_int_B;
       assign ps0_ps_BVALID      = ps0_ps_int_BVALID;
       assign ps0_ps_int_BREADY  = ps0_ps_BREADY;
       assign ps0_ps_R           = ps0_ps_int_R;
       assign ps0_ps_RVALID      = ps0_ps_int_RVALID;
       assign ps0_ps_int_RREADY  = ps0_ps_RREADY;


         //tie unused axi ports to '0'
      `UNPLUG_AXI_MASTER_PORT(ps0_tol3_m)
      `UNPLUG_AXI_MASTER_PORT(ps0_tosys_m)
    end
  

      // noc stamped instantiation
    `include "pshire_ns_debug_meshtop_nc_ifs.v"
    `include "ns_pci_pipeline_decl.v"
    `include "ns_pci_pipeline_inst.v"
    `include "pshire_ns_meshtop_inst.v"
    `include "ns_utsoc_pci_pipeline_decl.v"
    `include "ns_utsoc_pci_pipeline_inst.v"
    `include "pshire_ns_debug_meshtop_inst.v"
   
    `ifdef EVL_SIMULATION
       `ifndef NOC_DISABLE
          evl_pshire_meshstop_monitor #(.PSHIRE_ID (PSHIRE_ID), .NUM_L3_PORTS(1)) pshire_meshstop_mon (.*);
       `endif // ifndef NOC_DISABLE
    `endif // ifdef EVL_SIMULATION

   if (DV_PSHIRE_STIM == 1'b0) begin
      `ifndef ET_NO_IO_DPI
         import "DPI-C" function void nocEmu_tick(); 
         import "DPI-C" function void nocEmu_mc_in_ready(output bit [15:0] ready); 
         import "DPI-C" function void nocEmu_mc_tx(input int mcId, input int id, input logic isLoad, input bit[39:0] address, input bit[`SC_MESH_MASTER_AXI_DATA_SIZE-1:0] data); 
         import "DPI-C" function int nocEmu_mc_rx(input int mcId, output int id, output bit[`SC_MESH_MASTER_AXI_DATA_SIZE-1:0] data);
       `endif // ifndef ET_NO_IO_DPI

                    /* Request buffer queue */
                    mc_request [`MC_BUFFER_SIZE-1:0] req_buffer;
                    mc_request [`MC_BUFFER_SIZE-1:0] req_buffer_next;
                    int buffer_ocupation, buffer_ocupation_next;

                    function mc_request [`MC_BUFFER_SIZE-1:0] remove_entry;
                        input int id;
                        int i;
                        begin
                            for(i=0;i < `MC_BUFFER_SIZE; i++) begin
                                if(id <= i) begin
                                    if(i < `MC_BUFFER_SIZE-1) begin
                                        remove_entry[i] = req_buffer_next[i+1];
                                    end else begin
                                        remove_entry[i] = 0; 
                                    end
                                end else begin
                                    remove_entry[i] = req_buffer_next[i];
                                end
                            end
                        end
                    endfunction

                    `RST_FF(clock, reset, req_buffer, req_buffer_next, 'b0);
                    `RST_FF(clock, reset, buffer_ocupation, buffer_ocupation_next, 0);

                    mc_request response = '0;

                    sc_slave_axi_AR_channel_t AR;
                    sc_slave_axi_AW_channel_t AW;
                    sc_slave_axi_W_channel_t W;
                    logic   AR_valid;
                    logic   AW_valid;
                    logic   W_valid;

                    logic [2:0] mc_id;
                    logic [$bits(AR.ID)-1:0] mc_resp_id;

                    logic [15:0]  mc_state_tmp;
                    logic [15:0]  mc_state;

                    logic stall_in_AR;
                    logic stall_in_AW;
                    logic wait_for_W;
                    logic wait_for_AW;
                    logic mc_response = '0;
                    logic mc_response_tmp = '0; 
                    //                  RESET   EN          DOUT    DIN     0
                    `RST_EN_FF( clock,  reset,  !stall_in_AR,  {AR, AR_valid}, {ps0_ps_int_AR, ps0_ps_int_ARVALID}, '0)
                    `RST_EN_FF( clock,  reset,  !stall_in_AW | wait_for_AW,  {AW, AW_valid}, {ps0_ps_int_AW, ps0_ps_int_AWVALID}, '0)
                    `RST_EN_FF( clock,  reset,  !stall_in_AW | wait_for_W,  {W, W_valid}, {ps0_ps_int_W, ps0_ps_int_WVALID}, '0)

                    assign ps0_ps_int_ARREADY = !stall_in_AR;
                    assign ps0_ps_int_AWREADY = !stall_in_AW | wait_for_AW;
                    assign ps0_ps_int_WREADY = !stall_in_AW | wait_for_W;

                    int i, req_index, req_index_tmp;

                    //Set round robin for AW and AR requests
                    logic last_was_load, last_was_load_next;
                    `RST_FF( clock, reset, last_was_load, last_was_load_next, 'b0);


                    `ifndef ET_NO_IO_DPI
                    //MC responses management
                    always @ (posedge clock) begin
                        nocEmu_tick();
                        nocEmu_mc_in_ready(mc_state_tmp);

                        if((!mc_response) || mc_response&&((response.isRead && ps0_ps_int_RREADY) || (ps0_ps_int_BREADY && !response.isRead))) begin
                            mc_response_tmp = 0;
                            `ifndef ZEBU
                               for(i=0; i < buffer_ocupation && !mc_response_tmp; i++) begin //Check all current requests status
                            `else // ifndef ZEBU
                               for(i=0; i < `MC_BUFFER_SIZE && !mc_response_tmp; i++) begin //Check all current requests status
                            `endif // !ifndef ZEBU
                                   req_index_tmp = i;
                                   mc_response_tmp = nocEmu_mc_rx(req_buffer[i].mc_id, mc_resp_id, ps0_ps_int_R.DATA);
                               end
                            mc_response <= mc_response_tmp;
                            req_index <= req_index_tmp;
                        end

                        //Update all variables in a blocking way
                        mc_state <= mc_state_tmp;
                    end
                    `endif // ifndef ET_NO_IO_DPI

                    //Combinational part
                    always_comb 
                    begin

                        ps0_ps_int_BVALID = 'b0;
                        ps0_ps_int_RVALID = 'b0;

                        stall_in_AR = 'b0;
                        stall_in_AW = 'b0;
                        wait_for_W = 'b0;
                        wait_for_AW = 'b0;

                        //By default the state is saved
                        req_buffer_next = req_buffer;
                        buffer_ocupation_next = buffer_ocupation;
                        last_was_load_next = last_was_load;

                        `ifndef ET_NO_IO_DPI
                        //Input management
                        if(buffer_ocupation < `MC_BUFFER_SIZE) begin
                            if(AR_valid && !(last_was_load && (AW_valid | W_valid))) begin
                                stall_in_AW = AW_valid | W_valid;
                                last_was_load_next = 1;
                                stall_in_AR = 1;
                                if(mc_state[{1'b0, AR.ADDR[8:6]}]) begin
                                    stall_in_AR = 0;
                                    nocEmu_mc_tx(AR.ADDR[8:6], AR.ID, 1, AR.ADDR[39:0], `SC_MESH_MASTER_AXI_DATA_SIZE'h0);
                                    req_buffer_next[buffer_ocupation].mc_id = AR.ADDR[8:6];
                                    req_buffer_next[buffer_ocupation].id = AR.ID;
                                    req_buffer_next[buffer_ocupation].isRead = 1;
                                    buffer_ocupation_next = buffer_ocupation_next+1;
                                end
                            end else if(AW_valid && W_valid) begin
                                stall_in_AR=AR_valid;
                                last_was_load_next = 0;
                                stall_in_AW = 1;
                                if(mc_state[{1'b0, AW.ADDR[8:6]}]) begin
                                    stall_in_AW = 0;
                                    nocEmu_mc_tx(AW.ADDR[8:6], AW.ID, 1, AW.ADDR[39:0], W.DATA);
                                    req_buffer_next[buffer_ocupation].mc_id = AW.ADDR[8:6];
                                    req_buffer_next[buffer_ocupation].id = AW.ID;
                                    req_buffer_next[buffer_ocupation].isRead = 0;
                                    buffer_ocupation_next = buffer_ocupation_next+1;
                                end
                            end else if(AW_valid) begin
                                stall_in_AR=AR_valid;
                                last_was_load_next = 0;
                                stall_in_AW = 1;
                                wait_for_W = 1;
                            end else if(W_valid) begin
                                stall_in_AR=AR_valid;
                                last_was_load_next = 0;
                                stall_in_AW = 1;
                                wait_for_AW = 1;
                            end
                        end else begin
                            stall_in_AR = 1;
                            stall_in_AW = 1;
                        end
                        `endif

                        //Output management
                        if(mc_response) begin
                            response = req_buffer[req_index];
                            if(response.isRead) begin
                                ps0_ps_int_R.LAST='b1;
                                ps0_ps_int_R.RESP='b0;
                                ps0_ps_int_R.ID = response.id;
                                ps0_ps_int_RVALID = 'b1;
                            end else begin
                                ps0_ps_int_B.RESP='b0;
                                ps0_ps_int_B.ID= response.id;
                                ps0_ps_int_BVALID = 'b1;
                            end
                            req_buffer_next = remove_entry(req_index);
                            buffer_ocupation_next = buffer_ocupation_next-1;
                        end

                    end
   end

   `ifdef EVL_SIMULATION
      evl_pshire_stub_interface #(.PSHIRE_ID     (PSHIRE_ID),
                                  .SHIRE_STIM     (DV_PSHIRE_STIM),
                                  .NUM_L3_PORTS   (NUM_L3_PORTS)
                                 )
      pshire_ifc (.reset (reset),
                  .clock (clock),
                  .*);
   `endif // EVL_SIMULATION

   `ifdef ET_PCI_AXI_DRIVER
      evl_axi_synth_master m_pci_axi_driver (
            .reset     (reset),
            .clk       (clock),
            .aw_id     (),
            .aw_addr   (),
            .aw_len    (),
            .aw_size   (),
            .aw_burst  (),
            .aw_lock   (),
            .aw_cache  (),
            .aw_prot   (),
            .aw_qos    (),
            .aw_region (),
            .aw_user   (),
            .aw_valid  (),
            .aw_ready  (),
            .w_data    (),
            .w_strb    (),
            .w_last    (),
            .w_user    (),
            .w_valid   (),
            .w_ready   (),
            .b_id      (),
            .b_resp    (),
            .b_user    (),
            .b_valid   (),
            .b_ready   (),
            .ar_ready  (),
            .ar_valid  (),
            .ar_id     (),
            .ar_addr   (),
            .ar_len    (),
            .ar_size   (),
            .ar_burst  (),
            .ar_lock   (),
            .ar_cache  (),
            .ar_prot   (),
            .ar_qos    (),
            .ar_region (),
            .ar_user   (),
            .r_id      (),
            .r_data    (),
            .r_resp    (),
            .r_last    (),
            .r_user    (),
            .r_valid   (),
            .r_ready   ()
         );
   `endif // ifdef ET_PCI_AXI_DRIVER
endmodule

`ifndef __EVL_IOSHIRE_INTERFACE
   `define __EVL_IOSHIRE_INTERFACE 1
   `include "rtl/inc/soc.vh"

   `ifndef __EVL_IOSHIRE_PKG
      `include "dv/soc/ioshire/evl_ioshire_pkg.sv"
   `endif

   module evl_ioshire_stub_interface #(parameter int    IOSHIRE_ID    = 0,
                                       parameter bit    SP_IS_STUB = 1'b0,
                                       parameter bit    SHIRE_STIM = 1'b0,
                                       parameter int    NUM_L3_PORTS  = 1,
                                       parameter string ROM_FILE_NAME = "spROMImage.hex",
                                       parameter string RAM_FILE_NAME = "spRAMImage.hex") (
         input wire reset,
         input wire clock,

         inout wire sc_slave_axi_AR_channel_t  io0_ps_AR_cast,
         inout wire                            io0_ps_ARVALID,
         inout wire                            io0_ps_ARREADY,
         inout wire sc_slave_axi_AW_channel_t  io0_ps_AW_cast,
         inout wire                            io0_ps_AWVALID,
         inout wire                            io0_ps_AWREADY,
         inout wire sc_slave_axi_W_channel_t   io0_ps_W_cast,
         inout wire                            io0_ps_WVALID,
         inout wire                            io0_ps_WREADY,
         inout wire sc_slave_axi_B_channel_t   io0_ps_B_cast,
         inout wire                            io0_ps_BVALID,
         inout wire                            io0_ps_BREADY,
         inout wire sc_slave_axi_R_channel_t   io0_ps_R_cast,
         inout wire                            io0_ps_RVALID,
         inout wire                            io0_ps_RREADY,

         inout wire sc_master_axi_AR_channel_t io0_pm_AR_cast,
         inout wire                            io0_pm_ARVALID,
         inout wire                            io0_pm_ARREADY,
         inout wire sc_master_axi_AW_channel_t io0_pm_AW_cast,
         inout wire                            io0_pm_AWVALID,
         inout wire                            io0_pm_AWREADY,
         inout wire sc_master_axi_W_channel_t  io0_pm_W_cast,
         inout wire                            io0_pm_WVALID,
         inout wire                            io0_pm_WREADY,
         inout wire sc_master_axi_B_channel_t  io0_pm_B_cast,
         inout wire                            io0_pm_BVALID,
         inout wire                            io0_pm_BREADY,
         inout wire sc_master_axi_R_channel_t  io0_pm_R_cast,
         inout wire                            io0_pm_RVALID,
         inout wire                            io0_pm_RREADY,

         inout wire sc_master_axi_AR_channel_t io0_tosys_m_AR_cast,
         inout wire                            io0_tosys_m_ARVALID,
         inout wire                            io0_tosys_m_ARREADY,
         inout wire sc_master_axi_AW_channel_t io0_tosys_m_AW_cast,
         inout wire                            io0_tosys_m_AWVALID,
         inout wire                            io0_tosys_m_AWREADY,
         inout wire sc_master_axi_W_channel_t  io0_tosys_m_W_cast,
         inout wire                            io0_tosys_m_WVALID,
         inout wire                            io0_tosys_m_WREADY,
         inout wire sc_master_axi_B_channel_t  io0_tosys_m_B_cast,
         inout wire                            io0_tosys_m_BVALID,
         inout wire                            io0_tosys_m_BREADY,
         inout wire sc_master_axi_R_channel_t  io0_tosys_m_R_cast,
         inout wire                            io0_tosys_m_RVALID,
         inout wire                            io0_tosys_m_RREADY,

         inout wire sc_master_axi_AR_channel_t [NUM_L3_PORTS-1:0] io0_tol3_m_AR_cast,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_ARVALID,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_ARREADY,
         inout wire sc_master_axi_AW_channel_t [NUM_L3_PORTS-1:0] io0_tol3_m_AW_cast,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_AWVALID,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_AWREADY,
         inout wire sc_master_axi_W_channel_t  [NUM_L3_PORTS-1:0] io0_tol3_m_W_cast,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_WVALID,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_WREADY,
         inout wire sc_master_axi_B_channel_t  [NUM_L3_PORTS-1:0] io0_tol3_m_B_cast,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_BVALID,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_BREADY,
         inout wire sc_master_axi_R_channel_t  [NUM_L3_PORTS-1:0] io0_tol3_m_R_cast,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_RVALID,
         inout wire                            [NUM_L3_PORTS-1:0] io0_tol3_m_RREADY,

         inout  wire                           rbm_m_awvalid,    
         inout  wire                           rbm_m_awready,    
         inout  wire [ 39:  0]                 rbm_m_awaddr,    
         inout  wire [  2:0]                   rbm_m_awprot,    
         inout  wire                           rbm_m_wvalid,    
         inout  wire                           rbm_m_wready,    
         inout  wire [ 31:0]                   rbm_m_wdata,    
         inout  wire [  3:0]                   rbm_m_wstrb,    
         inout  wire                           rbm_m_bvalid,    
         inout  wire                           rbm_m_bready,    
         inout  wire [  1:0]                   rbm_m_bresp,    
         inout  wire                           rbm_m_arvalid,    
         inout  wire                           rbm_m_arready,    
         inout  wire [ 39:  0]                 rbm_m_araddr,    
         inout  wire [  2:0]                   rbm_m_arprot,    
         inout  wire                           rbm_m_rvalid,    
         inout  wire                           rbm_m_rready,    
         inout  wire [ 31:0]                   rbm_m_rdata,    
         inout  wire [  1:0]                   rbm_m_rresp,

         inout  wire [127:0]                   host_beat_data_io0_lf_msg,
         inout  wire                           host_beat_eop_io0_lf_msg,
         inout  wire                           host_beat_sop_io0_lf_msg,
         inout  wire                           host_beat_valid_io0_lf_msg,
         inout  wire                           host_credit_inc_io0_lf_msg,
         inout  wire   [7:0]                   host_dest_hp_io0_lf_msg,
         inout  wire   [1:0]                   host_dest_int_io0_lf_msg,
         inout  wire   [3:0]                   host_xfer_qos_io0_lf_msg,
         inout  wire [127:0]                   noc_beat_data_io0_lf_msg,
         inout  wire                           noc_beat_eop_io0_lf_msg,
         inout  wire                           noc_beat_sop_io0_lf_msg,
         inout  wire                           noc_beat_valid_io0_lf_msg,
         inout  wire                           noc_credit_inc_io0_lf_msg,

         inout  wire  [31:0]                   host_beat_data_io0_lf_evt,
         inout  wire                           host_beat_eop_io0_lf_evt,
         inout  wire                           host_beat_sop_io0_lf_evt,
         inout  wire                           host_beat_valid_io0_lf_evt,
         inout  wire                           host_credit_inc_io0_lf_evt,
         inout  wire   [7:0]                   host_dest_hp_io0_lf_evt,
         inout  wire   [1:0]                   host_dest_int_io0_lf_evt,
         inout  wire   [3:0]                   host_xfer_qos_io0_lf_evt,
         inout  wire  [31:0]                   noc_beat_data_io0_lf_evt,
         inout  wire                           noc_beat_eop_io0_lf_evt,
         inout  wire                           noc_beat_sop_io0_lf_evt,
         inout  wire                           noc_beat_valid_io0_lf_evt,
         inout  wire                           noc_credit_inc_io0_lf_evt,

         inout  wire [127:0]                   host_beat_data_io0_rt_msg,
         inout  wire                           host_beat_eop_io0_rt_msg,
         inout  wire                           host_beat_sop_io0_rt_msg,
         inout  wire                           host_beat_valid_io0_rt_msg,
         inout  wire                           host_credit_inc_io0_rt_msg,
         inout  wire   [7:0]                   host_dest_hp_io0_rt_msg,
         inout  wire   [1:0]                   host_dest_int_io0_rt_msg,
         inout  wire   [3:0]                   host_xfer_qos_io0_rt_msg,
         inout  wire [127:0]                   noc_beat_data_io0_rt_msg,
         inout  wire                           noc_beat_eop_io0_rt_msg,
         inout  wire                           noc_beat_sop_io0_rt_msg,
         inout  wire                           noc_beat_valid_io0_rt_msg,
         inout  wire                           noc_credit_inc_io0_rt_msg,

         inout  wire  [31:0]                   host_beat_data_io0_rt_evt,
         inout  wire                           host_beat_eop_io0_rt_evt,
         inout  wire                           host_beat_sop_io0_rt_evt,
         inout  wire                           host_beat_valid_io0_rt_evt,
         inout  wire                           host_credit_inc_io0_rt_evt,
         inout  wire   [7:0]                   host_dest_hp_io0_rt_evt,
         inout  wire   [1:0]                   host_dest_int_io0_rt_evt,
         inout  wire   [3:0]                   host_xfer_qos_io0_rt_evt,
         inout  wire  [31:0]                   noc_beat_data_io0_rt_evt,
         inout  wire                           noc_beat_eop_io0_rt_evt,
         inout  wire                           noc_beat_sop_io0_rt_evt,
         inout  wire                           noc_beat_valid_io0_rt_evt,
         inout  wire                           noc_credit_inc_io0_rt_evt

      );
    
    
      //-----------------------------------------------------------------------------------------------
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_ioshire_pkg::*;
    
    
    
      //
      // Debug NoC RT_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (IOSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_rt_msg (
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (host_dest_hp_io0_rt_msg),
            .tx_req_dest_int  (host_dest_int_io0_rt_msg),
            .tx_req_qos       (host_xfer_qos_io0_rt_msg),
            .tx_req_valid     (host_beat_valid_io0_rt_msg),
            .tx_req_data      (host_beat_data_io0_rt_msg),
            .tx_req_sop       (host_beat_sop_io0_rt_msg),
            .tx_req_eop       (host_beat_eop_io0_rt_msg),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_rt_msg),
            .rx_req_valid     (noc_beat_valid_io0_rt_msg),
            .rx_req_sop       (noc_beat_sop_io0_rt_msg),
            .rx_req_eop       (noc_beat_eop_io0_rt_msg),

            .host_credit_inc  (host_credit_inc_io0_rt_msg),
            .noc_credit_inc   (noc_credit_inc_io0_rt_msg)
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
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (host_dest_hp_io0_rt_evt),
            .tx_req_dest_int  (host_dest_int_io0_rt_evt),
            .tx_req_qos       (host_xfer_qos_io0_rt_evt),
            .tx_req_valid     (host_beat_valid_io0_rt_evt),
            .tx_req_data      (host_beat_data_io0_rt_evt),
            .tx_req_sop       (host_beat_sop_io0_rt_evt),
            .tx_req_eop       (host_beat_eop_io0_rt_evt),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_rt_evt),
            .rx_req_valid     (noc_beat_valid_io0_rt_evt),
            .rx_req_sop       (noc_beat_sop_io0_rt_evt),
            .rx_req_eop       (noc_beat_eop_io0_rt_evt),

            .host_credit_inc  (host_credit_inc_io0_rt_evt),
            .noc_credit_inc   (noc_credit_inc_io0_rt_evt)
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
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (host_dest_hp_io0_lf_msg),
            .tx_req_dest_int  (host_dest_int_io0_lf_msg),
            .tx_req_qos       (host_xfer_qos_io0_lf_msg),
            .tx_req_valid     (host_beat_valid_io0_lf_msg),
            .tx_req_data      (host_beat_data_io0_lf_msg),
            .tx_req_sop       (host_beat_sop_io0_lf_msg),
            .tx_req_eop       (host_beat_eop_io0_lf_msg),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_lf_msg),
            .rx_req_valid     (noc_beat_valid_io0_lf_msg),
            .rx_req_sop       (noc_beat_sop_io0_lf_msg),
            .rx_req_eop       (noc_beat_eop_io0_lf_msg),

            .host_credit_inc  (host_credit_inc_io0_lf_msg),
            .noc_credit_inc   (noc_credit_inc_io0_lf_msg)
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
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (host_dest_hp_io0_lf_evt),
            .tx_req_dest_int  (host_dest_int_io0_lf_evt),
            .tx_req_qos       (host_xfer_qos_io0_lf_evt),
            .tx_req_valid     (host_beat_valid_io0_lf_evt),
            .tx_req_data      (host_beat_data_io0_lf_evt),
            .tx_req_sop       (host_beat_sop_io0_lf_evt),
            .tx_req_eop       (host_beat_eop_io0_lf_evt),
            .tx_req_trans_id  (),

            .rx_req_data      (noc_beat_data_io0_lf_evt),
            .rx_req_valid     (noc_beat_valid_io0_lf_evt),
            .rx_req_sop       (noc_beat_sop_io0_lf_evt),
            .rx_req_eop       (noc_beat_eop_io0_lf_evt),

            .host_credit_inc  (host_credit_inc_io0_lf_evt),
            .noc_credit_inc   (noc_credit_inc_io0_lf_evt)
         );

         //-----------------------------------------------------------------------------------------------
         //
         // Add some unused signals.
         //

         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] io0_tol3_m_aw_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] io0_tol3_m_w_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] io0_tol3_m_b_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] io0_tol3_m_ar_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] io0_tol3_m_r_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`EVL_BUS_REGION_MSB:0] io0_tol3_m_ARREGION;
         tri0 [NUM_L3_PORTS-1:0] [`EVL_BUS_REGION_MSB:0] io0_tol3_m_AWREGION;
         tri0 [NUM_L3_PORTS-1:0]                         io0_tol3_m_WUSER;
         tri0 [NUM_L3_PORTS-1:0]                         io0_tol3_m_BUSER;
         tri0 [NUM_L3_PORTS-1:0]                         io0_tol3_m_RUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_pm_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_pm_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_pm_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_pm_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_pm_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_pm_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_pm_AWREGION;
         tri0                         io0_pm_WUSER;
         tri0                         io0_pm_BUSER;
         tri0                         io0_pm_RUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_ps_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_ps_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_ps_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_ps_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_ps_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_ps_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_ps_AWREGION;
         tri0                         io0_ps_WUSER;
         tri0                         io0_ps_BUSER;
         tri0                         io0_ps_RUSER;

         //tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] io0_ps_AWUSER;
         tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] io0_ps_ARUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_tosys_m_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_tosys_m_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_tosys_m_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_tosys_m_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] io0_tosys_m_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_tosys_m_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] io0_tosys_m_AWREGION;
         tri0                         io0_tosys_m_WUSER;
         tri0                         io0_tosys_m_BUSER;
         tri0                         io0_tosys_m_RUSER;
  //
  // Create slave AXI mesh bus agent interface.
  //
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
        .reset           (reset),
        .clk             (clock),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

           .aw_id        (io0_tol3_m_AW_cast[0].ID),
           .aw_addr      (io0_tol3_m_AW_cast[0].ADDR),
           .aw_len       (io0_tol3_m_AW_cast[0].LEN),
           .aw_size      (io0_tol3_m_AW_cast[0].SIZE),
           .aw_burst     (io0_tol3_m_AW_cast[0].BURST),
           .aw_lock      (io0_tol3_m_AW_cast[0].LOCK),
           .aw_cache     (io0_tol3_m_AW_cast[0].CACHE),
           .aw_prot      (io0_tol3_m_AW_cast[0].PROT),
           .aw_qos       (io0_tol3_m_AW_cast[0].QOS),
           .aw_region    (io0_tol3_m_AWREGION[0]),
           .aw_user      (io0_tol3_m_AW_cast[0].USER),
           .aw_valid     (io0_tol3_m_AWVALID[0]),
           .aw_ready     (io0_tol3_m_AWREADY[0]),
           .aw_vc_valid  (),
           .aw_vc_credit (),
           .aw_trans_id  (io0_tol3_m_aw_trans_id[0]),

           .w_user       (io0_tol3_m_WUSER[0]),
           .w_data       (io0_tol3_m_W_cast[0].DATA),
           .w_strb       (io0_tol3_m_W_cast[0].STRB),
           .w_last       (io0_tol3_m_W_cast[0].LAST),
           .w_valid      (io0_tol3_m_WVALID[0]),
           .w_ready      (io0_tol3_m_WREADY[0]),
           .w_vc_valid   (),
           .w_trans_id   (io0_tol3_m_w_trans_id[0]),

           .b_user       (io0_tol3_m_BUSER[0]),
           .b_id         (io0_tol3_m_B_cast[0].ID),
           .b_resp       (io0_tol3_m_B_cast[0].RESP),
           .b_valid      (io0_tol3_m_BVALID[0]),
           .b_ready      (io0_tol3_m_BREADY[0]),
           .b_trans_id   (io0_tol3_m_b_trans_id[0]),

           .ar_id        (io0_tol3_m_AR_cast[0].ID),
           .ar_addr      (io0_tol3_m_AR_cast[0].ADDR),
           .ar_len       (io0_tol3_m_AR_cast[0].LEN),
           .ar_size      (io0_tol3_m_AR_cast[0].SIZE),
           .ar_burst     (io0_tol3_m_AR_cast[0].BURST),
           .ar_lock      (io0_tol3_m_AR_cast[0].LOCK),
           .ar_cache     (io0_tol3_m_AR_cast[0].CACHE),
           .ar_prot      (io0_tol3_m_AR_cast[0].PROT),
           .ar_qos       (io0_tol3_m_AR_cast[0].QOS),
           .ar_region    (io0_tol3_m_ARREGION[0]),
           .ar_user      (io0_tol3_m_AR_cast[0].USER),
           .ar_valid     (io0_tol3_m_ARVALID[0]),
           .ar_ready     (io0_tol3_m_ARREADY[0]),
           .ar_vc_valid  (),
           .ar_vc_credit (),
           .ar_trans_id  (io0_tol3_m_ar_trans_id[0]),

           .r_last       (io0_tol3_m_R_cast[0].LAST),
           .r_user       (io0_tol3_m_RUSER[0]),
           .r_id         (io0_tol3_m_R_cast[0].ID),
           .r_data       (io0_tol3_m_R_cast[0].DATA),
           .r_resp       (io0_tol3_m_R_cast[0].RESP),
           .r_valid      (io0_tol3_m_RVALID[0]),
           .r_ready      (io0_tol3_m_RREADY[0]),
           .r_trans_id   (io0_tol3_m_r_trans_id[0])
     );

  //
  // Create slave AXI mesh bus agent interface.
  //
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
        .reset           (reset),
        .clk             (clock),

        `ifdef ET_AXI_DPI_MONITOR
           .dpi_capture_enable (1'b0),
           .dpi_read_clock     (1'b0),
           .dpi_packet_pop     (1'b0),
           .dpi_packet_valid   (),
           .raw_dpi_packet     (),
        `endif // ifdef ET_AXI_DPI_MONITOR

           .aw_id        (io0_tol3_m_AW_cast[1].ID),
           .aw_addr      (io0_tol3_m_AW_cast[1].ADDR),
           .aw_len       (io0_tol3_m_AW_cast[1].LEN),
           .aw_size      (io0_tol3_m_AW_cast[1].SIZE),
           .aw_burst     (io0_tol3_m_AW_cast[1].BURST),
           .aw_lock      (io0_tol3_m_AW_cast[1].LOCK),
           .aw_cache     (io0_tol3_m_AW_cast[1].CACHE),
           .aw_prot      (io0_tol3_m_AW_cast[1].PROT),
           .aw_qos       (io0_tol3_m_AW_cast[1].QOS),
           .aw_region    (io0_tol3_m_AWREGION[1]),
           .aw_user      (io0_tol3_m_AW_cast[1].USER),
           .aw_valid     (io0_tol3_m_AWVALID[1]),
           .aw_ready     (io0_tol3_m_AWREADY[1]),
           .aw_vc_valid  (),
           .aw_vc_credit (),
           .aw_trans_id  (io0_tol3_m_aw_trans_id[1]),

           .w_user       (io0_tol3_m_WUSER[1]),
           .w_data       (io0_tol3_m_W_cast[1].DATA),
           .w_strb       (io0_tol3_m_W_cast[1].STRB),
           .w_last       (io0_tol3_m_W_cast[1].LAST),
           .w_valid      (io0_tol3_m_WVALID[1]),
           .w_ready      (io0_tol3_m_WREADY[1]),
           .w_vc_valid   (),
           .w_trans_id   (io0_tol3_m_w_trans_id[1]),

           .b_user       (io0_tol3_m_BUSER[1]),
           .b_id         (io0_tol3_m_B_cast[1].ID),
           .b_resp       (io0_tol3_m_B_cast[1].RESP),
           .b_valid      (io0_tol3_m_BVALID[1]),
           .b_ready      (io0_tol3_m_BREADY[1]),
           .b_trans_id   (io0_tol3_m_b_trans_id[1]),

           .ar_id        (io0_tol3_m_AR_cast[1].ID),
           .ar_addr      (io0_tol3_m_AR_cast[1].ADDR),
           .ar_len       (io0_tol3_m_AR_cast[1].LEN),
           .ar_size      (io0_tol3_m_AR_cast[1].SIZE),
           .ar_burst     (io0_tol3_m_AR_cast[1].BURST),
           .ar_lock      (io0_tol3_m_AR_cast[1].LOCK),
           .ar_cache     (io0_tol3_m_AR_cast[1].CACHE),
           .ar_prot      (io0_tol3_m_AR_cast[1].PROT),
           .ar_qos       (io0_tol3_m_AR_cast[1].QOS),
           .ar_region    (io0_tol3_m_ARREGION[1]),
           .ar_user      (io0_tol3_m_AR_cast[1].USER),
           .ar_valid     (io0_tol3_m_ARVALID[1]),
           .ar_ready     (io0_tol3_m_ARREADY[1]),
           .ar_vc_valid  (),
           .ar_vc_credit (),
           .ar_trans_id  (io0_tol3_m_ar_trans_id[1]),

           .r_last       (io0_tol3_m_R_cast[1].LAST),
           .r_user       (io0_tol3_m_RUSER[1]),
           .r_id         (io0_tol3_m_R_cast[1].ID),
           .r_data       (io0_tol3_m_R_cast[1].DATA),
           .r_resp       (io0_tol3_m_R_cast[1].RESP),
           .r_valid      (io0_tol3_m_RVALID[1]),
           .r_ready      (io0_tol3_m_RREADY[1]),
           .r_trans_id   (io0_tol3_m_r_trans_id[1])
     );

         // io_pm
         evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                             .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_pm_master_axi (
               .isolate      (1'b0),
               .reset        (reset),
               .clk          (clock),

               `ifdef ET_AXI_DPI_MONITOR
                  .dpi_capture_enable (1'b0),
                  .dpi_read_clock     (1'b0),
                  .dpi_packet_pop     (1'b0),
                  .dpi_packet_valid   (),
                  .raw_dpi_packet     (),
               `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id        (io0_pm_AW_cast.ID),
               .aw_addr      (io0_pm_AW_cast.ADDR),
               .aw_len       (io0_pm_AW_cast.LEN),
               .aw_size      (io0_pm_AW_cast.SIZE),
               .aw_burst     (io0_pm_AW_cast.BURST),
               .aw_lock      (io0_pm_AW_cast.LOCK),
               .aw_cache     (io0_pm_AW_cast.CACHE),
               .aw_prot      (io0_pm_AW_cast.PROT),
               .aw_qos       (io0_pm_AW_cast.QOS),
               .aw_region    (io0_pm_AWREGION),
               .aw_user      (io0_pm_AW_cast.USER),
               .aw_valid     (io0_pm_AWVALID),
               .aw_ready     (io0_pm_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (io0_pm_aw_trans_id),

               .w_user       (io0_pm_WUSER),
               .w_data       (io0_pm_W_cast.DATA),
               .w_strb       (io0_pm_W_cast.STRB),
               .w_last       (io0_pm_W_cast.LAST),
               .w_valid      (io0_pm_WVALID),
               .w_ready      (io0_pm_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (io0_pm_w_trans_id),

               .b_user       (io0_pm_BUSER),
               .b_id         (io0_pm_B_cast.ID),
               .b_resp       (io0_pm_B_cast.RESP),
               .b_valid      (io0_pm_BVALID),
               .b_ready      (io0_pm_BREADY),
               .b_trans_id   (io0_pm_b_trans_id),

               .ar_id        (io0_pm_AR_cast.ID),
               .ar_addr      (io0_pm_AR_cast.ADDR),
               .ar_len       (io0_pm_AR_cast.LEN),
               .ar_size      (io0_pm_AR_cast.SIZE),
               .ar_burst     (io0_pm_AR_cast.BURST),
               .ar_lock      (io0_pm_AR_cast.LOCK),
               .ar_cache     (io0_pm_AR_cast.CACHE),
               .ar_prot      (io0_pm_AR_cast.PROT),
               .ar_qos       (io0_pm_AR_cast.QOS),
               .ar_region    (io0_pm_ARREGION),
               .ar_user      (io0_pm_AR_cast.USER),
               .ar_valid     (io0_pm_ARVALID),
               .ar_ready     (io0_pm_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (io0_pm_ar_trans_id),

               .r_last       (io0_pm_R_cast.LAST),
               .r_user       (io0_pm_RUSER),
               .r_id         (io0_pm_R_cast.ID),
               .r_data       (io0_pm_R_cast.DATA),
               .r_resp       (io0_pm_R_cast.RESP),
               .r_valid      (io0_pm_RVALID),
               .r_ready      (io0_pm_RREADY),
               .r_trans_id   (io0_pm_r_trans_id)
            );


         // io_ps
         evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                             .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_ps_slave_axi (
               .isolate      (1'b0),
               .reset        (reset),
               .clk          (clock),

               `ifdef ET_AXI_DPI_MONITOR
                  .dpi_capture_enable (1'b0),
                  .dpi_read_clock     (1'b0),
                  .dpi_packet_pop     (1'b0),
                  .dpi_packet_valid   (),
                  .raw_dpi_packet     (),
               `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id        (io0_ps_AW_cast.ID),
               .aw_addr      (io0_ps_AW_cast.ADDR),
               .aw_len       (io0_ps_AW_cast.LEN),
               .aw_size      (io0_ps_AW_cast.SIZE),
               .aw_burst     (io0_ps_AW_cast.BURST),
               .aw_lock      (io0_ps_AW_cast.LOCK),
               .aw_cache     (io0_ps_AW_cast.CACHE),
               .aw_prot      (io0_ps_AW_cast.PROT),
               .aw_qos       (io0_ps_AW_cast.QOS),
               .aw_region    (io0_ps_AWREGION),
               .aw_user      (io0_ps_AW_cast.USER),
               .aw_valid     (io0_ps_AWVALID),
               .aw_ready     (io0_ps_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (io0_ps_aw_trans_id),

               .w_user       (io0_ps_WUSER),
               .w_data       (io0_ps_W_cast.DATA),
               .w_strb       (io0_ps_W_cast.STRB),
               .w_last       (io0_ps_W_cast.LAST),
               .w_valid      (io0_ps_WVALID),
               .w_ready      (io0_ps_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (io0_ps_w_trans_id),

               .b_user       (io0_ps_BUSER),
               .b_id         (io0_ps_B_cast.ID),
               .b_resp       (io0_ps_B_cast.RESP),
               .b_valid      (io0_ps_BVALID),
               .b_ready      (io0_ps_BREADY),
               .b_trans_id   (io0_ps_b_trans_id),

               .ar_id        (io0_ps_AR_cast.ID),
               .ar_addr      (io0_ps_AR_cast.ADDR),
               .ar_len       (io0_ps_AR_cast.LEN),
               .ar_size      (io0_ps_AR_cast.SIZE),
               .ar_burst     (io0_ps_AR_cast.BURST),
               .ar_lock      (io0_ps_AR_cast.LOCK),
               .ar_cache     (io0_ps_AR_cast.CACHE),
               .ar_prot      (io0_ps_AR_cast.PROT),
               .ar_qos       (io0_ps_AR_cast.QOS),
               .ar_region    (io0_ps_ARREGION),
               .ar_user      (io0_ps_ARUSER),
               .ar_valid     (io0_ps_ARVALID),
               .ar_ready     (io0_ps_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (io0_ps_ar_trans_id),

               .r_last       (io0_ps_R_cast.LAST),
               .r_user       (io0_ps_RUSER),
               .r_id         (io0_ps_R_cast.ID),
               .r_data       (io0_ps_R_cast.DATA),
               .r_resp       (io0_ps_R_cast.RESP),
               .r_valid      (io0_ps_RVALID),
               .r_ready      (io0_ps_RREADY),
               .r_trans_id   (io0_ps_r_trans_id)
            );

            // tosys mesh master
            evl_axi_interface #(.SHIRE_ID     (IOSHIRE_ID),
                                .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                                .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                                .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                                .AXSIZE_WIDTH (3),
                                .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                                .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_io_tosys_mesh_master_axi (
               .isolate      (1'b0),
               .reset        (reset),
               .clk          (clock),

               `ifdef ET_AXI_DPI_MONITOR
                  .dpi_capture_enable (1'b0),
                  .dpi_read_clock     (1'b0),
                  .dpi_packet_pop     (1'b0),
                  .dpi_packet_valid   (),
                  .raw_dpi_packet     (),
               `endif // ifdef ET_AXI_DPI_MONITOR
            
               .aw_id        (io0_tosys_m_AW_cast.ID),
               .aw_addr      (io0_tosys_m_AW_cast.ADDR),
               .aw_len       (io0_tosys_m_AW_cast.LEN),
               .aw_size      (io0_tosys_m_AW_cast.SIZE),
               .aw_burst     (io0_tosys_m_AW_cast.BURST),
               .aw_lock      (io0_tosys_m_AW_cast.LOCK),
               .aw_cache     (io0_tosys_m_AW_cast.CACHE),
               .aw_prot      (io0_tosys_m_AW_cast.PROT),
               .aw_qos       (io0_tosys_m_AW_cast.QOS),
               .aw_region    (io0_tosys_m_AWREGION),
               .aw_user      (io0_tosys_m_AW_cast.USER),
               .aw_valid     (io0_tosys_m_AWVALID),
               .aw_ready     (io0_tosys_m_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (),//(tosys_aw_trans_id),
            
               .w_user       (io0_tosys_m_WUSER),
               .w_data       (io0_tosys_m_W_cast.DATA),
               .w_strb       (io0_tosys_m_W_cast.STRB),
               .w_last       (io0_tosys_m_W_cast.LAST),
               .w_valid      (io0_tosys_m_WVALID),
               .w_ready      (io0_tosys_m_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (),//(tosys_w_trans_id),
            
               .b_user       (io0_tosys_m_BUSER),
               .b_id         (io0_tosys_m_B_cast.ID),
               .b_resp       (io0_tosys_m_B_cast.RESP),
               .b_valid      (io0_tosys_m_BVALID),
               .b_ready      (io0_tosys_m_BREADY),
               .b_trans_id   (),//(tosys_b_trans_id),
            
               .ar_id        (io0_tosys_m_AR_cast.ID),
               .ar_addr      (io0_tosys_m_AR_cast.ADDR),
               .ar_len       (io0_tosys_m_AR_cast.LEN),
               .ar_size      (io0_tosys_m_AR_cast.SIZE),
               .ar_burst     (io0_tosys_m_AR_cast.BURST),
               .ar_lock      (io0_tosys_m_AR_cast.LOCK),
               .ar_cache     (io0_tosys_m_AR_cast.CACHE),
               .ar_prot      (io0_tosys_m_AR_cast.PROT),
               .ar_qos       (io0_tosys_m_AR_cast.QOS),
               .ar_region    (io0_tosys_m_ARREGION),
               .ar_user      (io0_tosys_m_AR_cast.USER),
               .ar_valid     (io0_tosys_m_ARVALID),
               .ar_ready     (io0_tosys_m_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (),//(tosys_ar_trans_id),
            
               .r_last       (io0_tosys_m_R_cast.LAST),
               .r_user       (io0_tosys_m_RUSER),
               .r_id         (io0_tosys_m_R_cast.ID),
               .r_data       (io0_tosys_m_R_cast.DATA),
               .r_resp       (io0_tosys_m_R_cast.RESP),
               .r_valid      (io0_tosys_m_RVALID),
               .r_ready      (io0_tosys_m_RREADY),
               .r_trans_id   ()//(tosys_r_trans_id)
            );


   tri0 [`DV_TRANS_ID_SIZE-1:0]               rbm_m_aw_trans_id;
   tri0                                       rbm_m_aw_qos;
   tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0] rbm_m_aw_user;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           rbm_m_aw_region;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               rbm_m_w_trans_id;
   tri0                                       rbm_m_w_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               rbm_m_b_trans_id;
   tri0                                       rbm_m_b_user;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               rbm_m_ar_trans_id;
   tri0 [`EVL_BUS_REGION_WIDTH-1:0]           rbm_m_ar_region;
   tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0] rbm_m_ar_user;
   tri0                                       rbm_m_ar_qos;

   tri0 [`DV_TRANS_ID_SIZE-1:0]               rbm_m_r_trans_id;
   tri0                                       rbm_m_r_user;

   tri0 [3:0]                                 rbm_m_aw_id;
   tri0 [3:0]                                 rbm_m_aw_len;
   tri0 [2:0]                                 rbm_m_aw_size;
   tri0 [1:0]                                 rbm_m_aw_burst;
   tri0                                       rbm_m_aw_lock;
   tri0 [3:0]                                 rbm_m_aw_cache;
   
   tri0                                       rbm_m_w_last;
   
   tri0 [3:0]                                 rbm_m_b_id;
   
   tri0 [3:0]                                 rbm_m_ar_id;
   tri0 [3:0]                                 rbm_m_ar_len;
   tri0 [2:0]                                 rbm_m_ar_size;
   tri0 [1:0]                                 rbm_m_ar_burst;
   tri0                                       rbm_m_ar_lock;
   tri0 [3:0]                                 rbm_m_ar_cache;
   
   tri0                                       rbm_m_r_last;
   tri0 [3:0]                                 rbm_m_r_id;


   evl_axi_interface #(.SHIRE_ID       (IOSHIRE_ID),
                       .ID_WIDTH       (4),
                       .ADDR_WIDTH     (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH     (32),
                       .AXSIZE_WIDTH   (3),
                       .ARUSER_WIDTH   (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH   (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AXREGION_WIDTH (4),
                       .AXQOS_WIDTH    (1),
                       .AXLEN_WIDTH    (4),
                       .AW_VC_WIDTH    (1),
                       .AW_VC_CREDITS  (0)) m_spio_sp_main0_main_noc_rbm_s (
         .isolate      (1'b0),
         .reset        (reset),
         .clk          (clock),

         `ifdef ET_AXI_DPI_MONITOR
            .dpi_capture_enable (1'b0),
            .dpi_read_clock     (1'b0),
            .dpi_packet_pop     (1'b0),
            .dpi_packet_valid   (),
            .raw_dpi_packet     (),
         `endif // ifdef ET_AXI_DPI_MONITOR

         .aw_id        (rbm_m_aw_id),
         .aw_addr      (rbm_m_awaddr),
         .aw_len       (rbm_m_aw_len),
         .aw_size      (rbm_m_aw_size),
         .aw_burst     (rbm_m_aw_burst),
         .aw_lock      (rbm_m_aw_lock),
         .aw_cache     (rbm_m_aw_cache),
         .aw_prot      (rbm_m_awprot),
         .aw_qos       (rbm_m_aw_qos),
         .aw_region    (rbm_m_aw_region),
         .aw_user      (rbm_m_aw_user),
         .aw_valid     (rbm_m_awvalid),
         .aw_ready     (rbm_m_awready),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (rbm_m_aw_trans_id),

         .w_user       (rbm_m_w_user),
         .w_data       (rbm_m_wdata),
         .w_strb       (rbm_m_wstrb),
         .w_last       (rbm_m_w_last),
         .w_valid      (rbm_m_wvalid),
         .w_ready      (rbm_m_wready),
         .w_vc_valid   (),
         .w_trans_id   (rbm_m_w_trans_id),

         .b_user       (rbm_m_b_user),
         .b_id         (rbm_m_b_id),
         .b_resp       (rbm_m_bresp),
         .b_valid      (rbm_m_bvalid),
         .b_ready      (rbm_m_bready),
         .b_trans_id   (rbm_m_b_trans_id),

         .ar_id        (rbm_m_ar_id),
         .ar_addr      (rbm_m_araddr),
         .ar_len       (rbm_m_ar_len),
         .ar_size      (rbm_m_ar_size),
         .ar_burst     (rbm_m_ar_burst),
         .ar_lock      (rbm_m_ar_lock),
         .ar_cache     (rbm_m_ar_cache),
         .ar_prot      (rbm_m_arprot),
         .ar_qos       (rbm_m_ar_qos),
         .ar_region    (rbm_m_ar_region),
         .ar_user      (rbm_m_ar_user),
         .ar_valid     (rbm_m_arvalid),
         .ar_ready     (rbm_m_arready),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (rbm_m_ar_trans_id),

         .r_last       (rbm_m_r_last),
         .r_user       (rbm_m_r_user),
         .r_id         (rbm_m_r_id),
         .r_data       (rbm_m_rdata),
         .r_resp       (rbm_m_rresp),
         .r_valid      (rbm_m_rvalid),
         .r_ready      (rbm_m_rready),
         .r_trans_id   (rbm_m_r_trans_id)
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
      evl_ioshire_rtl_port m_rtl_port      = new($sformatf("%m"), IOSHIRE_ID, "", { 1 });
      bit                  m_start_monitor = 1'b0;
      bit                  m_reset_on      = 1'b0;
      bit                  m_reset_off     = 1'b0;
      bit                  m_mem_loaded    = 1'b0;
      string               m_rom_file_name = ROM_FILE_NAME;
      string               m_ram_file_name = RAM_FILE_NAME;
      string               m_full_file_name;
      string               m_plus_arg;
    
      initial begin
         m_rtl_port.set_int_param_by_name("pk_sp_is_stub", ((SP_IS_STUB == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_ioshire_stim", ((SHIRE_STIM == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_PORTS);
         m_ioshire_id = IOSHIRE_ID;
         #1ns m_start_monitor = 1'b1;
         @(posedge clock);
      end
    
      always @(posedge clock) begin
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
    
      endmodule

`endif // ifndef __EVL_IOSHIRE_INTERFACE

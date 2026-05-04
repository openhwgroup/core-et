   `include "rtl/inc/soc.vh"

   `ifndef __EVL_PSHIRE_PKG
      `include "dv/soc/pshire/evl_pshire_pkg.sv"
   `endif

   module evl_pshire_interface #(parameter int PSHIRE_ID    = 0,
                                 parameter bit SHIRE_STIM   = 1'b0,
                                 parameter int NUM_L3_PORTS = 1,
                                 parameter bit PSHIRE_NOC_ONLY = 1'b0) (
         input wire reset,
         input wire clock,

         inout wire sc_slave_axi_AR_channel_t  sys_mesh_slave_axi_AR,
         inout wire                            sys_mesh_slave_axi_ARVALID,
         inout wire                            sys_mesh_slave_axi_ARREADY,
         inout wire sc_slave_axi_AW_channel_t  sys_mesh_slave_axi_AW,
         inout wire                            sys_mesh_slave_axi_AWVALID,
         inout wire                            sys_mesh_slave_axi_AWREADY,
         inout wire sc_slave_axi_W_channel_t   sys_mesh_slave_axi_W,
         inout wire                            sys_mesh_slave_axi_WVALID,
         inout wire                            sys_mesh_slave_axi_WREADY,
         inout wire sc_slave_axi_B_channel_t   sys_mesh_slave_axi_B,
         inout wire                            sys_mesh_slave_axi_BVALID,
         inout wire                            sys_mesh_slave_axi_BREADY,
         inout wire sc_slave_axi_R_channel_t   sys_mesh_slave_axi_R,
         inout wire                            sys_mesh_slave_axi_RVALID,
         inout wire                            sys_mesh_slave_axi_RREADY,

         inout wire sc_master_axi_AR_channel_t to_sys_mesh_master_axi_AR,
         inout wire                            to_sys_mesh_master_axi_ARVALID,
         inout wire                            to_sys_mesh_master_axi_ARREADY,
         inout wire sc_master_axi_AW_channel_t to_sys_mesh_master_axi_AW,
         inout wire                            to_sys_mesh_master_axi_AWVALID,
         inout wire                            to_sys_mesh_master_axi_AWREADY,
         inout wire sc_master_axi_W_channel_t  to_sys_mesh_master_axi_W,
         inout wire                            to_sys_mesh_master_axi_WVALID,
         inout wire                            to_sys_mesh_master_axi_WREADY,
         inout wire sc_master_axi_B_channel_t  to_sys_mesh_master_axi_B,
         inout wire                            to_sys_mesh_master_axi_BVALID,
         inout wire                            to_sys_mesh_master_axi_BREADY,
         inout wire sc_master_axi_R_channel_t  to_sys_mesh_master_axi_R,
         inout wire                            to_sys_mesh_master_axi_RVALID,
         inout wire                            to_sys_mesh_master_axi_RREADY,

         inout wire sc_master_axi_AR_channel_t to_l3_mesh_master_axi_AR,
         inout wire                            to_l3_mesh_master_axi_ARVALID,
         inout wire                            to_l3_mesh_master_axi_ARREADY,
         inout wire sc_master_axi_AW_channel_t to_l3_mesh_master_axi_AW,
         inout wire                            to_l3_mesh_master_axi_AWVALID,
         inout wire                            to_l3_mesh_master_axi_AWREADY,
         inout wire sc_master_axi_W_channel_t  to_l3_mesh_master_axi_W,
         inout wire                            to_l3_mesh_master_axi_WVALID,
         inout wire                            to_l3_mesh_master_axi_WREADY,
         inout wire sc_master_axi_B_channel_t  to_l3_mesh_master_axi_B,
         inout wire                            to_l3_mesh_master_axi_BVALID,
         inout wire                            to_l3_mesh_master_axi_BREADY,
         inout wire sc_master_axi_R_channel_t  to_l3_mesh_master_axi_R,
         inout wire                            to_l3_mesh_master_axi_RVALID,
         inout wire                            to_l3_mesh_master_axi_RREADY


      );
    
    
      //-----------------------------------------------------------------------------------------------
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_pshire_pkg::*;


      `ifdef DV_PSHIRE_DBGNOC_STIM
         localparam int DBGNOC_STIM = 1;
      `else
         localparam int DBGNOC_STIM = 0;
      `endif

         //-----------------------------------------------------------------------------------------------
         //
         // Add some unused signals.
         //

         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_l3_mesh_master_axi_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_l3_mesh_master_axi_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_l3_mesh_master_axi_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_l3_mesh_master_axi_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_l3_mesh_master_axi_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0]                 to_l3_mesh_master_axi_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0]                 to_l3_mesh_master_axi_AWREGION;
         tri0                                         to_l3_mesh_master_axi_WUSER;
         tri0                                         to_l3_mesh_master_axi_BUSER;
         tri0                                         to_l3_mesh_master_axi_RUSER;
         tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0]   to_l3_mesh_master_axi_AWUSER;
         tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   to_l3_mesh_master_axi_ARUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0]                 sys_mesh_slave_axi_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 sys_mesh_slave_axi_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 sys_mesh_slave_axi_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 sys_mesh_slave_axi_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 sys_mesh_slave_axi_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0]                 sys_mesh_slave_axi_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0]                 sys_mesh_slave_axi_AWREGION;
         tri0                                         sys_mesh_slave_axi_WUSER;
         tri0                                         sys_mesh_slave_axi_BUSER;
         tri0                                         sys_mesh_slave_axi_RUSER;
         tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0]   sys_mesh_slave_axi_AWUSER;
         tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   sys_mesh_slave_axi_ARUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_sys_mesh_master_axi_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_sys_mesh_master_axi_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_sys_mesh_master_axi_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_sys_mesh_master_axi_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0]                 to_sys_mesh_master_axi_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0]                 to_sys_mesh_master_axi_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0]                 to_sys_mesh_master_axi_AWREGION;
         tri0                                         to_sys_mesh_master_axi_WUSER;
         tri0                                         to_sys_mesh_master_axi_BUSER;
         tri0                                         to_sys_mesh_master_axi_RUSER;
         tri0 [`SC_MESH_MASTER_AXI_AWUSER_SIZE-1:0]   to_sys_mesh_master_axi_AWUSER;
         tri0 [`SC_MESH_MASTER_AXI_ARUSER_SIZE-1:0]   to_sys_mesh_master_axi_ARUSER;

         //
         // Create slave AXI mesh bus agent interface.
         //
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ps_tol3_mesh_master_axi (
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

                  .aw_id        (to_l3_mesh_master_axi_AW.ID),
                  .aw_addr      (to_l3_mesh_master_axi_AW.ADDR),
                  .aw_len       (to_l3_mesh_master_axi_AW.LEN),
                  .aw_size      (to_l3_mesh_master_axi_AW.SIZE),
                  .aw_burst     (to_l3_mesh_master_axi_AW.BURST),
                  .aw_lock      (to_l3_mesh_master_axi_AW.LOCK),
                  .aw_cache     (to_l3_mesh_master_axi_AW.CACHE),
                  .aw_prot      (to_l3_mesh_master_axi_AW.PROT),
                  .aw_qos       (to_l3_mesh_master_axi_AW.QOS),
                  .aw_region    (to_l3_mesh_master_axi_AWREGION),
                  .aw_user      (to_l3_mesh_master_axi_AWUSER),
                  .aw_valid     (to_l3_mesh_master_axi_AWVALID),
                  .aw_ready     (to_l3_mesh_master_axi_AWREADY),
                  .aw_vc_valid  (),
                  .aw_vc_credit (),
                  .aw_trans_id  (to_l3_mesh_master_axi_aw_trans_id),

                  .w_user       (to_l3_mesh_master_axi_WUSER),
                  .w_data       (to_l3_mesh_master_axi_W.DATA),
                  .w_strb       (to_l3_mesh_master_axi_W.STRB),
                  .w_last       (to_l3_mesh_master_axi_W.LAST),
                  .w_valid      (to_l3_mesh_master_axi_WVALID),
                  .w_ready      (to_l3_mesh_master_axi_WREADY),
                  .w_vc_valid   (),
                  .w_trans_id   (to_l3_mesh_master_axi_w_trans_id),

                  .b_user       (to_l3_mesh_master_axi_BUSER),
                  .b_id         (to_l3_mesh_master_axi_B.ID),
                  .b_resp       (to_l3_mesh_master_axi_B.RESP),
                  .b_valid      (to_l3_mesh_master_axi_BVALID),
                  .b_ready      (to_l3_mesh_master_axi_BREADY),
                  .b_trans_id   (to_l3_mesh_master_axi_b_trans_id),

                  .ar_id        (to_l3_mesh_master_axi_AR.ID),
                  .ar_addr      (to_l3_mesh_master_axi_AR.ADDR),
                  .ar_len       (to_l3_mesh_master_axi_AR.LEN),
                  .ar_size      (to_l3_mesh_master_axi_AR.SIZE),
                  .ar_burst     (to_l3_mesh_master_axi_AR.BURST),
                  .ar_lock      (to_l3_mesh_master_axi_AR.LOCK),
                  .ar_cache     (to_l3_mesh_master_axi_AR.CACHE),
                  .ar_prot      (to_l3_mesh_master_axi_AR.PROT),
                  .ar_qos       (to_l3_mesh_master_axi_AR.QOS),
                  .ar_region    (to_l3_mesh_master_axi_ARREGION),
                  .ar_user      (to_l3_mesh_master_axi_ARUSER),
                  .ar_valid     (to_l3_mesh_master_axi_ARVALID),
                  .ar_ready     (to_l3_mesh_master_axi_ARREADY),
                  .ar_vc_valid  (),
                  .ar_vc_credit (),
                  .ar_trans_id  (to_l3_mesh_master_axi_ar_trans_id),

                  .r_last       (to_l3_mesh_master_axi_R.LAST),
                  .r_user       (to_l3_mesh_master_axi_RUSER),
                  .r_id         (to_l3_mesh_master_axi_R.ID),
                  .r_data       (to_l3_mesh_master_axi_R.DATA),
                  .r_resp       (to_l3_mesh_master_axi_R.RESP),
                  .r_valid      (to_l3_mesh_master_axi_RVALID),
                  .r_ready      (to_l3_mesh_master_axi_RREADY),
                  .r_trans_id   (to_l3_mesh_master_axi_r_trans_id)
            );

         // ps_ps
         evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                             .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ps_ps_slave_axi (
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

               .aw_id        (sys_mesh_slave_axi_AW.ID),
               .aw_addr      (sys_mesh_slave_axi_AW.ADDR),
               .aw_len       (sys_mesh_slave_axi_AW.LEN),
               .aw_size      (sys_mesh_slave_axi_AW.SIZE),
               .aw_burst     (sys_mesh_slave_axi_AW.BURST),
               .aw_lock      (sys_mesh_slave_axi_AW.LOCK),
               .aw_cache     (sys_mesh_slave_axi_AW.CACHE),
               .aw_prot      (sys_mesh_slave_axi_AW.PROT),
               .aw_qos       (sys_mesh_slave_axi_AW.QOS),
               .aw_region    (sys_mesh_slave_axi_AWREGION),
               .aw_user      (sys_mesh_slave_axi_AW.USER),
               .aw_valid     (sys_mesh_slave_axi_AWVALID),
               .aw_ready     (sys_mesh_slave_axi_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (sys_mesh_slave_axi_aw_trans_id),

               .w_user       (sys_mesh_slave_axi_WUSER),
               .w_data       (sys_mesh_slave_axi_W.DATA),
               .w_strb       (sys_mesh_slave_axi_W.STRB),
               .w_last       (sys_mesh_slave_axi_W.LAST),
               .w_valid      (sys_mesh_slave_axi_WVALID),
               .w_ready      (sys_mesh_slave_axi_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (sys_mesh_slave_axi_w_trans_id),

               .b_user       (sys_mesh_slave_axi_BUSER),
               .b_id         (sys_mesh_slave_axi_B.ID),
               .b_resp       (sys_mesh_slave_axi_B.RESP),
               .b_valid      (sys_mesh_slave_axi_BVALID),
               .b_ready      (sys_mesh_slave_axi_BREADY),
               .b_trans_id   (sys_mesh_slave_axi_b_trans_id),

               .ar_id        (sys_mesh_slave_axi_AR.ID),
               .ar_addr      (sys_mesh_slave_axi_AR.ADDR),
               .ar_len       (sys_mesh_slave_axi_AR.LEN),
               .ar_size      (sys_mesh_slave_axi_AR.SIZE),
               .ar_burst     (sys_mesh_slave_axi_AR.BURST),
               .ar_lock      (sys_mesh_slave_axi_AR.LOCK),
               .ar_cache     (sys_mesh_slave_axi_AR.CACHE),
               .ar_prot      (sys_mesh_slave_axi_AR.PROT),
               .ar_qos       (sys_mesh_slave_axi_AR.QOS),
               .ar_region    (sys_mesh_slave_axi_ARREGION),
               .ar_user      (sys_mesh_slave_axi_ARUSER),
               .ar_valid     (sys_mesh_slave_axi_ARVALID),
               .ar_ready     (sys_mesh_slave_axi_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (sys_mesh_slave_axi_ar_trans_id),

               .r_last       (sys_mesh_slave_axi_R.LAST),
               .r_user       (sys_mesh_slave_axi_RUSER),
               .r_id         (sys_mesh_slave_axi_R.ID),
               .r_data       (sys_mesh_slave_axi_R.DATA),
               .r_resp       (sys_mesh_slave_axi_R.RESP),
               .r_valid      (sys_mesh_slave_axi_RVALID),
               .r_ready      (sys_mesh_slave_axi_RREADY),
               .r_trans_id   (sys_mesh_slave_axi_r_trans_id)
            );

            // tosys mesh master
            evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                                .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                                .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                                .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                                .AXSIZE_WIDTH (3),
                                .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                                .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ps_tosys_mesh_master_axi (
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
            
               .aw_id        (to_sys_mesh_master_axi_AW.ID),
               .aw_addr      (to_sys_mesh_master_axi_AW.ADDR),
               .aw_len       (to_sys_mesh_master_axi_AW.LEN),
               .aw_size      (to_sys_mesh_master_axi_AW.SIZE),
               .aw_burst     (to_sys_mesh_master_axi_AW.BURST),
               .aw_lock      (to_sys_mesh_master_axi_AW.LOCK),
               .aw_cache     (to_sys_mesh_master_axi_AW.CACHE),
               .aw_prot      (to_sys_mesh_master_axi_AW.PROT),
               .aw_qos       (to_sys_mesh_master_axi_AW.QOS),
               .aw_region    (to_sys_mesh_master_axi_AWREGION),
               .aw_user      (to_sys_mesh_master_axi_AWUSER),
               .aw_valid     (to_sys_mesh_master_axi_AWVALID),
               .aw_ready     (to_sys_mesh_master_axi_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (),//(tosys_aw_trans_id),
            
               .w_user       (to_sys_mesh_master_axi_WUSER),
               .w_data       (to_sys_mesh_master_axi_W.DATA),
               .w_strb       (to_sys_mesh_master_axi_W.STRB),
               .w_last       (to_sys_mesh_master_axi_W.LAST),
               .w_valid      (to_sys_mesh_master_axi_WVALID),
               .w_ready      (to_sys_mesh_master_axi_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (),//(tosys_w_trans_id),
            
               .b_user       (to_sys_mesh_master_axi_BUSER),
               .b_id         (to_sys_mesh_master_axi_B.ID),
               .b_resp       (to_sys_mesh_master_axi_B.RESP),
               .b_valid      (to_sys_mesh_master_axi_BVALID),
               .b_ready      (to_sys_mesh_master_axi_BREADY),
               .b_trans_id   (),//(tosys_b_trans_id),
            
               .ar_id        (to_sys_mesh_master_axi_AR.ID),
               .ar_addr      (to_sys_mesh_master_axi_AR.ADDR),
               .ar_len       (to_sys_mesh_master_axi_AR.LEN),
               .ar_size      (to_sys_mesh_master_axi_AR.SIZE),
               .ar_burst     (to_sys_mesh_master_axi_AR.BURST),
               .ar_lock      (to_sys_mesh_master_axi_AR.LOCK),
               .ar_cache     (to_sys_mesh_master_axi_AR.CACHE),
               .ar_prot      (to_sys_mesh_master_axi_AR.PROT),
               .ar_qos       (to_sys_mesh_master_axi_AR.QOS),
               .ar_region    (to_sys_mesh_master_axi_ARREGION),
               .ar_user      (to_sys_mesh_master_axi_ARUSER),
               .ar_valid     (to_sys_mesh_master_axi_ARVALID),
               .ar_ready     (to_sys_mesh_master_axi_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (),//(tosys_ar_trans_id),
            
               .r_last       (to_sys_mesh_master_axi_R.LAST),
               .r_user       (to_sys_mesh_master_axi_RUSER),
               .r_id         (to_sys_mesh_master_axi_R.ID),
               .r_data       (to_sys_mesh_master_axi_R.DATA),
               .r_resp       (to_sys_mesh_master_axi_R.RESP),
               .r_valid      (to_sys_mesh_master_axi_RVALID),
               .r_ready      (to_sys_mesh_master_axi_RREADY),
               .r_trans_id   ()//(tosys_r_trans_id)
            );

      //
      // Debug NoC DN_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (PSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_dn_msg (
            .reset            (pshire_top.reset_udb),
            .clk              (pshire_top.clock_udb),

            .tx_req_addr      (pshire_top.ust_out_nsip_msg_dest_hp_op),
            .tx_req_dest_int  (pshire_top.ust_out_nsip_msg_dest_int_op),
            .tx_req_qos       (pshire_top.ust_out_nsip_msg_xfer_qos_op),
            .tx_req_valid     (pshire_top.ust_out_nsip_msg_beat_valid_op),
            .tx_req_data      (pshire_top.ust_out_nsip_msg_beat_data_op),
            .tx_req_sop       (pshire_top.ust_out_nsip_msg_beat_sop_op),
            .tx_req_eop       (pshire_top.ust_out_nsip_msg_beat_eop_op),
            .tx_req_trans_id  (),

            .rx_req_data      (pshire_top.ust_in_nsip_msg_beat_data_ip),
            .rx_req_valid     (pshire_top.ust_in_nsip_msg_beat_valid_ip),
            .rx_req_sop       (pshire_top.ust_in_nsip_msg_beat_sop_ip),
            .rx_req_eop       (pshire_top.ust_in_nsip_msg_beat_eop_ip),

            .host_credit_inc  (pshire_top.ust_in_nsip_msg_credit_inc_op),
            .noc_credit_inc   (pshire_top.ust_out_nsip_msg_credit_inc_ip)
         );

      //
      // Debug NoC DN_EVT IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (PSHIRE_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (32),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_EVT_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_dn_evt (
            .reset            (pshire_top.reset_udb),
            .clk              (pshire_top.clock_udb),

            .tx_req_addr      (pshire_top.ust_out_nsip_evt_dest_hp_op),
            .tx_req_dest_int  (pshire_top.ust_out_nsip_evt_dest_int_op),
            .tx_req_qos       (pshire_top.ust_out_nsip_evt_xfer_qos_op),
            .tx_req_valid     (pshire_top.ust_out_nsip_evt_beat_valid_op),
            .tx_req_data      (pshire_top.ust_out_nsip_evt_beat_data_op),
            .tx_req_sop       (pshire_top.ust_out_nsip_evt_beat_sop_op),
            .tx_req_eop       (pshire_top.ust_out_nsip_evt_beat_eop_op),
            .tx_req_trans_id  (),

            .rx_req_data      (pshire_top.ust_in_nsip_evt_beat_data_ip),
            .rx_req_valid     (pshire_top.ust_in_nsip_evt_beat_valid_ip),
            .rx_req_sop       (pshire_top.ust_in_nsip_evt_beat_sop_ip),
            .rx_req_eop       (pshire_top.ust_in_nsip_evt_beat_eop_ip),

            .host_credit_inc  (pshire_top.ust_in_nsip_evt_credit_inc_op),
            .noc_credit_inc   (pshire_top.ust_out_nsip_evt_credit_inc_ip)
         );

      //-----------------------------------------------------------------------------------------------
      //
      // Create an instance identifier.
      //
      int m_pshire_id;
      bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                  PSHIRE_RTL_AGENT,
                                                                  PSHIRE_ID,
                                                                  "evl_pshire_env", "", "", "", { 1 });
    
    
      //-----------------------------------------------------------------------------------------------
      //
      // Create a Shire RTL port instance and drive it appropriately.
      //
      evl_pshire_rtl_port m_rtl_port      = new($sformatf("%m"), PSHIRE_ID, "", { 1 });
      bit                  m_start_monitor = 1'b0;
      bit                  m_reset_on      = 1'b0;
      bit                  m_reset_off     = 1'b0;
      string               m_plus_arg;
    
      initial begin
         m_rtl_port.set_int_param_by_name("pk_pshire_stim", ((SHIRE_STIM == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_PORTS);
         m_rtl_port.set_int_param_by_name("pk_pshire_noc_only", ((PSHIRE_NOC_ONLY == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_dbgnoc_stim", DBGNOC_STIM);

         m_pshire_id = PSHIRE_ID;
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


`include "rtl/inc/soc.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

`ifndef __EVL_SHIRE_PKG
   `include "dv/soc/shire/evl_shire_pkg.sv"
`endif
`ifndef __EVL_NOC_PKG
   `include "dv/noc/evl_noc_pkg.sv"
`endif

module evl_shire_stub_interface #(parameter int                SHIRE_INST_ID = 255,
                                  parameter int                NUM_NEIGH     = 1,
                                  parameter int                NUM_RBOX      = 1,
                                  parameter int                NUM_BANKS     = 1,
                                  parameter int                NUM_UC        = 1,
                                  parameter bit                NOC_STUB      = 1'b0,
                                  parameter bit                SC_STUB       = 1'b0,
                                  parameter bit[NUM_NEIGH-1:0] NEIGH_STUB    = {NUM_NEIGH{0}}) (
      input    wire                                                            reset,
      input    wire                                                            clock,
      inout    wire sc_master_axi_AR_channel_t                                 to_l3_mesh_master_axi_AR,
      inout    wire                                                            to_l3_mesh_master_axi_ARVALID,
      inout    wire                                                            to_l3_mesh_master_axi_ARREADY,
      inout    wire sc_master_axi_AW_channel_t                                 to_l3_mesh_master_axi_AW,
      inout    wire                                                            to_l3_mesh_master_axi_AWVALID,
      inout    wire                                                            to_l3_mesh_master_axi_AWREADY,
      inout    wire sc_master_axi_W_channel_t                                  to_l3_mesh_master_axi_W,
      inout    wire                                                            to_l3_mesh_master_axi_WVALID,
      inout    wire                                                            to_l3_mesh_master_axi_WREADY,
      inout    wire sc_master_axi_B_channel_t                                  to_l3_mesh_master_axi_B,
      inout    wire                                                            to_l3_mesh_master_axi_BVALID,
      inout    wire                                                            to_l3_mesh_master_axi_BREADY,
      inout    wire sc_master_axi_R_channel_t                                  to_l3_mesh_master_axi_R,
      inout    wire                                                            to_l3_mesh_master_axi_RVALID,
      inout    wire                                                            to_l3_mesh_master_axi_RREADY,
      inout    wire sc_master_axi_AR_channel_t                                 to_sys_mesh_master_axi_AR,
      inout    wire                                                            to_sys_mesh_master_axi_ARVALID,
      inout    wire                                                            to_sys_mesh_master_axi_ARREADY,
      inout    wire sc_master_axi_AW_channel_t                                 to_sys_mesh_master_axi_AW,
      inout    wire                                                            to_sys_mesh_master_axi_AWVALID,
      inout    wire                                                            to_sys_mesh_master_axi_AWREADY,
      inout    wire sc_master_axi_W_channel_t                                  to_sys_mesh_master_axi_W,
      inout    wire                                                            to_sys_mesh_master_axi_WVALID,
      inout    wire                                                            to_sys_mesh_master_axi_WREADY,
      inout    wire sc_master_axi_B_channel_t                                  to_sys_mesh_master_axi_B,
      inout    wire                                                            to_sys_mesh_master_axi_BVALID,
      inout    wire                                                            to_sys_mesh_master_axi_BREADY,
      inout    wire sc_master_axi_R_channel_t                                  to_sys_mesh_master_axi_R,
      inout    wire                                                            to_sys_mesh_master_axi_RVALID,
      inout    wire                                                            to_sys_mesh_master_axi_RREADY,

      inout    wire sc_slave_axi_AR_channel_t                                  l3_mesh_slave_axi_AR,
      inout    wire                                                            l3_mesh_slave_axi_ARVALID,
      inout    wire                                                            l3_mesh_slave_axi_ARREADY,
      inout    wire sc_slave_axi_AW_channel_t                                  l3_mesh_slave_axi_AW,
      inout    wire                                                            l3_mesh_slave_axi_AWVALID,
      inout    wire                                                            l3_mesh_slave_axi_AWREADY,
      inout    wire sc_slave_axi_W_channel_t                                   l3_mesh_slave_axi_W,
      inout    wire                                                            l3_mesh_slave_axi_WVALID,
      inout    wire                                                            l3_mesh_slave_axi_WREADY,
      inout    wire sc_slave_axi_B_channel_t                                   l3_mesh_slave_axi_B,
      inout    wire                                                            l3_mesh_slave_axi_BVALID,
      inout    wire                                                            l3_mesh_slave_axi_BREADY,
      inout    wire sc_slave_axi_R_channel_t                                   l3_mesh_slave_axi_R,
      inout    wire                                                            l3_mesh_slave_axi_RVALID,
      inout    wire                                                            l3_mesh_slave_axi_RREADY,

      inout    wire sc_slave_axi_AR_channel_t                                  sys_mesh_slave_axi_AR,
      inout    wire                                                            sys_mesh_slave_axi_ARVALID,
      inout    wire                                                            sys_mesh_slave_axi_ARREADY,
      inout    wire sc_slave_axi_AW_channel_t                                  sys_mesh_slave_axi_AW,
      inout    wire                                                            sys_mesh_slave_axi_AWVALID,
      inout    wire [1:0]                                                      sys_mesh_slave_axi_AWvcvalid,
      inout    wire [1:0]                                                      sys_mesh_slave_axi_AWcredit,
      inout    wire                                                            sys_mesh_slave_axi_AWREADY,
      inout    wire sc_slave_axi_W_channel_t                                   sys_mesh_slave_axi_W,
      inout    wire                                                            sys_mesh_slave_axi_WVALID,
      inout    wire [1:0]                                                      sys_mesh_slave_axi_Wvcvalid,
      inout    wire                                                            sys_mesh_slave_axi_WREADY,
      inout    wire sc_slave_axi_B_channel_t                                   sys_mesh_slave_axi_B,
      inout    wire                                                            sys_mesh_slave_axi_BVALID,
      inout    wire                                                            sys_mesh_slave_axi_BREADY,
      inout    wire sc_slave_axi_R_channel_t                                   sys_mesh_slave_axi_R,
      inout    wire                                                            sys_mesh_slave_axi_RVALID,
      inout    wire                                                            sys_mesh_slave_axi_RREADY,

      inout    wire sc_master_axi_AR_channel_t                                 uc_to_l3_mesh_master_axi_AR,
      inout    wire                                                            uc_to_l3_mesh_master_axi_ARVALID,
      inout    wire                                                            uc_to_l3_mesh_master_axi_ARREADY,
      inout    wire sc_master_axi_AW_channel_t                                 uc_to_l3_mesh_master_axi_AW,
      inout    wire                                                            uc_to_l3_mesh_master_axi_AWVALID,
      inout    wire                                                            uc_to_l3_mesh_master_axi_AWREADY,
      inout    wire sc_master_axi_W_channel_t                                  uc_to_l3_mesh_master_axi_W,
      inout    wire                                                            uc_to_l3_mesh_master_axi_WVALID,
      inout    wire                                                            uc_to_l3_mesh_master_axi_WREADY,
      inout    wire sc_master_axi_B_channel_t                                  uc_to_l3_mesh_master_axi_B,
      inout    wire                                                            uc_to_l3_mesh_master_axi_BVALID,
      inout    wire                                                            uc_to_l3_mesh_master_axi_BREADY,
      inout    wire sc_master_axi_R_channel_t                                  uc_to_l3_mesh_master_axi_R,
      inout    wire                                                            uc_to_l3_mesh_master_axi_RVALID,
      inout    wire                                                            uc_to_l3_mesh_master_axi_RREADY,

      inout    wire sc_master_axi_AR_channel_t                                 uc_to_sys_mesh_master_axi_AR,
      inout    wire                                                            uc_to_sys_mesh_master_axi_ARVALID,
      inout    wire                                                            uc_to_sys_mesh_master_axi_ARREADY,
      inout    wire sc_master_axi_AW_channel_t                                 uc_to_sys_mesh_master_axi_AW,
      inout    wire                                                            uc_to_sys_mesh_master_axi_AWVALID,
      inout    wire                                                            uc_to_sys_mesh_master_axi_AWREADY,
      inout    wire sc_master_axi_W_channel_t                                  uc_to_sys_mesh_master_axi_W,
      inout    wire                                                            uc_to_sys_mesh_master_axi_WVALID,
      inout    wire                                                            uc_to_sys_mesh_master_axi_WREADY,
      inout    wire sc_master_axi_B_channel_t                                  uc_to_sys_mesh_master_axi_B,
      inout    wire                                                            uc_to_sys_mesh_master_axi_BVALID,
      inout    wire                                                            uc_to_sys_mesh_master_axi_BREADY,
      inout    wire sc_master_axi_R_channel_t                                  uc_to_sys_mesh_master_axi_R,
      inout    wire                                                            uc_to_sys_mesh_master_axi_RVALID,
      inout    wire                                                            uc_to_sys_mesh_master_axi_RREADY
   );

   //--------------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the Shire top-level module.  It includes DV-specific
   // objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_shire_pkg::*;


   //--------------------------------------------------------------------------------------------------
   //
   // Add some unused signals.
   //
   tri0 [`DV_TRANS_ID_SIZE-1:0] noc_sbm_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] noc_sbm_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] noc_sbm_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] noc_sbm_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] noc_sbm_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] noc_sbm_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] noc_sbm_mesh_master_axi_AWREGION;
   tri0                         noc_sbm_mesh_master_axi_WUSER;
   tri0                         noc_sbm_mesh_master_axi_BUSER;
   tri0                         noc_sbm_mesh_master_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] to_l3_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_l3_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_l3_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_l3_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_l3_r_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] l3_mesh_slave_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] l3_mesh_slave_ar_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] to_l3_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] to_l3_mesh_master_axi_AWREGION;
   tri0                         to_l3_mesh_master_axi_WUSER;
   tri0                         to_l3_mesh_master_axi_BUSER;
   tri0                         to_l3_mesh_master_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] to_sys_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_sys_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_sys_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_sys_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] to_sys_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] to_sys_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] to_sys_mesh_master_axi_AWREGION;
   tri0                         to_sys_mesh_master_axi_WUSER;
   tri0                         to_sys_mesh_master_axi_BUSER;
   tri0                         to_sys_mesh_master_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] l3_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] l3_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] l3_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] l3_mesh_slave_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] l3_mesh_slave_axi_AWREGION;
   tri0                         l3_mesh_slave_axi_WUSER;
   tri0                         l3_mesh_slave_axi_BUSER;
   tri0                         l3_mesh_slave_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_l3_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_l3_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_l3_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_l3_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_l3_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] uc_to_l3_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] uc_to_l3_mesh_master_axi_AWREGION;
   tri0                         uc_to_l3_mesh_master_axi_WUSER;
   tri0                         uc_to_l3_mesh_master_axi_BUSER;
   tri0                         uc_to_l3_mesh_master_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_sys_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_sys_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_sys_b_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_sys_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] uc_to_sys_r_trans_id;
   tri0 [`EVL_BUS_REGION_MSB:0] uc_to_sys_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] uc_to_sys_mesh_master_axi_AWREGION;
   tri0                         uc_to_sys_mesh_master_axi_WUSER;
   tri0                         uc_to_sys_mesh_master_axi_BUSER;
   tri0                         uc_to_sys_mesh_master_axi_RUSER;

   assign sys_mesh_slave_axi_AWcredit = 2'b0;

      //
      // Create slave AXI mesh bus agent interface.
      //
      evl_axi_interface #(.SHIRE_ID      (SHIRE_INST_ID),
                          .ID_WIDTH      (`SC_MESH_SLAVE_AXI_ID_SIZE),
                          .ADDR_WIDTH    (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH    (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH  (3),
                          .ARUSER_WIDTH  (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                          .AWUSER_WIDTH  (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                          .AW_VC_CREDITS (16),
                          .AW_VC_WIDTH   (2)) m_sys_mesh_slave_axi (
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
            .aw_region    (noc_sbm_mesh_master_axi_AWREGION),
            .aw_user      (sys_mesh_slave_axi_AW.USER),
            .aw_valid     (sys_mesh_slave_axi_AWVALID),
            .aw_ready     (sys_mesh_slave_axi_AWREADY),
            .aw_vc_valid  (),//(sys_mesh_slave_axi_AWvcvalid),
            .aw_vc_credit (sys_mesh_slave_axi_AWcredit),
            .aw_trans_id  (noc_sbm_aw_trans_id),

            .w_user       (noc_sbm_mesh_master_axi_WUSER),
            .w_data       (sys_mesh_slave_axi_W.DATA),
            .w_strb       (sys_mesh_slave_axi_W.STRB),
            .w_last       (sys_mesh_slave_axi_W.LAST),
            .w_valid      (sys_mesh_slave_axi_WVALID),
            .w_ready      (sys_mesh_slave_axi_WREADY),
            .w_vc_valid   (sys_mesh_slave_axi_Wvcvalid),
            .w_trans_id   (noc_sbm_w_trans_id),

            .b_user       (noc_sbm_mesh_master_axi_BUSER),
            .b_id         (sys_mesh_slave_axi_B.ID),
            .b_resp       (sys_mesh_slave_axi_B.RESP),
            .b_valid      (sys_mesh_slave_axi_BVALID),
            .b_ready      (sys_mesh_slave_axi_BREADY),
            .b_trans_id   (noc_sbm_b_trans_id),

            .ar_id        (sys_mesh_slave_axi_AR.ID),
            .ar_addr      (sys_mesh_slave_axi_AR.ADDR),
            .ar_len       (sys_mesh_slave_axi_AR.LEN),
            .ar_size      (sys_mesh_slave_axi_AR.SIZE),
            .ar_burst     (sys_mesh_slave_axi_AR.BURST),
            .ar_lock      (sys_mesh_slave_axi_AR.LOCK),
            .ar_cache     (sys_mesh_slave_axi_AR.CACHE),
            .ar_prot      (sys_mesh_slave_axi_AR.PROT),
            .ar_qos       (sys_mesh_slave_axi_AR.QOS),
            .ar_region    (noc_sbm_mesh_master_axi_ARREGION),
            .ar_user      (sys_mesh_slave_axi_AR.USER),
            .ar_valid     (sys_mesh_slave_axi_ARVALID),
            .ar_ready     (sys_mesh_slave_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (noc_sbm_ar_trans_id),

            .r_last       (sys_mesh_slave_axi_R.LAST),
            .r_user       (noc_sbm_mesh_master_axi_RUSER),
            .r_id         (sys_mesh_slave_axi_R.ID),
            .r_data       (sys_mesh_slave_axi_R.DATA),
            .r_resp       (sys_mesh_slave_axi_R.RESP),
            .r_valid      (sys_mesh_slave_axi_RVALID),
            .r_ready      (sys_mesh_slave_axi_RREADY),
            .r_trans_id   (noc_sbm_r_trans_id)
         );

      //
      // Create slave AXI mesh bus agent interface.
      //
      evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                          .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                          .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH (3),
                          .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                          .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_to_l3_mesh_master_axi (
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
               .aw_user      (to_l3_mesh_master_axi_AW.USER),
               .aw_valid     (to_l3_mesh_master_axi_AWVALID),
               .aw_ready     (to_l3_mesh_master_axi_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (to_l3_aw_trans_id),

               .w_user       (to_l3_mesh_master_axi_WUSER),
               .w_data       (to_l3_mesh_master_axi_W.DATA),
               .w_strb       (to_l3_mesh_master_axi_W.STRB),
               .w_last       (to_l3_mesh_master_axi_W.LAST),
               .w_valid      (to_l3_mesh_master_axi_WVALID),
               .w_ready      (to_l3_mesh_master_axi_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (to_l3_w_trans_id),

               .b_user       (to_l3_mesh_master_axi_BUSER),
               .b_id         (to_l3_mesh_master_axi_B.ID),
               .b_resp       (to_l3_mesh_master_axi_B.RESP),
               .b_valid      (to_l3_mesh_master_axi_BVALID),
               .b_ready      (to_l3_mesh_master_axi_BREADY),
               .b_trans_id   (to_l3_b_trans_id),

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
               .ar_user      (to_l3_mesh_master_axi_AR.USER),
               .ar_valid     (to_l3_mesh_master_axi_ARVALID),
               .ar_ready     (to_l3_mesh_master_axi_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (to_l3_ar_trans_id),

               .r_last       (to_l3_mesh_master_axi_R.LAST),
               .r_user       (to_l3_mesh_master_axi_RUSER),
               .r_id         (to_l3_mesh_master_axi_R.ID),
               .r_data       (to_l3_mesh_master_axi_R.DATA),
               .r_resp       (to_l3_mesh_master_axi_R.RESP),
               .r_valid      (to_l3_mesh_master_axi_RVALID),
               .r_ready      (to_l3_mesh_master_axi_RREADY),
               .r_trans_id   (to_l3_r_trans_id)
         );

      // to_sys
      evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                          .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                          .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH (3),
                          .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                          .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_to_sys_mesh_master_axi (
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
            .aw_user      (to_sys_mesh_master_axi_AW.USER),
            .aw_valid     (to_sys_mesh_master_axi_AWVALID),
            .aw_ready     (to_sys_mesh_master_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (to_sys_aw_trans_id),

            .w_user       (to_sys_mesh_master_axi_WUSER),
            .w_data       (to_sys_mesh_master_axi_W.DATA),
            .w_strb       (to_sys_mesh_master_axi_W.STRB),
            .w_last       (to_sys_mesh_master_axi_W.LAST),
            .w_valid      (to_sys_mesh_master_axi_WVALID),
            .w_ready      (to_sys_mesh_master_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (to_sys_w_trans_id),

            .b_user       (to_sys_mesh_master_axi_BUSER),
            .b_id         (to_sys_mesh_master_axi_B.ID),
            .b_resp       (to_sys_mesh_master_axi_B.RESP),
            .b_valid      (to_sys_mesh_master_axi_BVALID),
            .b_ready      (to_sys_mesh_master_axi_BREADY),
            .b_trans_id   (to_sys_b_trans_id),

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
            .ar_user      (to_sys_mesh_master_axi_AR.USER),
            .ar_valid     (to_sys_mesh_master_axi_ARVALID),
            .ar_ready     (to_sys_mesh_master_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (to_sys_ar_trans_id),

            .r_last       (to_sys_mesh_master_axi_R.LAST),
            .r_user       (to_sys_mesh_master_axi_RUSER),
            .r_id         (to_sys_mesh_master_axi_R.ID),
            .r_data       (to_sys_mesh_master_axi_R.DATA),
            .r_resp       (to_sys_mesh_master_axi_R.RESP),
            .r_valid      (to_sys_mesh_master_axi_RVALID),
            .r_ready      (to_sys_mesh_master_axi_RREADY),
            .r_trans_id   (to_sys_r_trans_id)
         );


      // from l3 mesh slave
      evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                          .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                          .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                          .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                          .AXSIZE_WIDTH (3),
                          .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                          .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_l3_mesh_slave_axi (
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

            .aw_id        (l3_mesh_slave_axi_AW.ID),
            .aw_addr      (l3_mesh_slave_axi_AW.ADDR),
            .aw_len       (l3_mesh_slave_axi_AW.LEN),
            .aw_size      (l3_mesh_slave_axi_AW.SIZE),
            .aw_burst     (l3_mesh_slave_axi_AW.BURST),
            .aw_lock      (l3_mesh_slave_axi_AW.LOCK),
            .aw_cache     (l3_mesh_slave_axi_AW.CACHE),
            .aw_prot      (l3_mesh_slave_axi_AW.PROT),
            .aw_qos       (l3_mesh_slave_axi_AW.QOS),
            .aw_region    (l3_mesh_slave_axi_AWREGION),
            .aw_user      (l3_mesh_slave_axi_AW.USER),
            .aw_valid     (l3_mesh_slave_axi_AWVALID),
            .aw_ready     (l3_mesh_slave_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (l3_mesh_slave_aw_trans_id),

            .w_user       (l3_mesh_slave_axi_WUSER),
            .w_data       (l3_mesh_slave_axi_W.DATA),
            .w_strb       (l3_mesh_slave_axi_W.STRB),
            .w_last       (l3_mesh_slave_axi_W.LAST),
            .w_valid      (l3_mesh_slave_axi_WVALID),
            .w_ready      (l3_mesh_slave_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (l3_w_trans_id),

            .b_user       (l3_mesh_slave_axi_BUSER),
            .b_id         (l3_mesh_slave_axi_B.ID),
            .b_resp       (l3_mesh_slave_axi_B.RESP),
            .b_valid      (l3_mesh_slave_axi_BVALID),
            .b_ready      (l3_mesh_slave_axi_BREADY),
            .b_trans_id   (l3_b_trans_id),

            .ar_id        (l3_mesh_slave_axi_AR.ID),
            .ar_addr      (l3_mesh_slave_axi_AR.ADDR),
            .ar_len       (l3_mesh_slave_axi_AR.LEN),
            .ar_size      (l3_mesh_slave_axi_AR.SIZE),
            .ar_burst     (l3_mesh_slave_axi_AR.BURST),
            .ar_lock      (l3_mesh_slave_axi_AR.LOCK),
            .ar_cache     (l3_mesh_slave_axi_AR.CACHE),
            .ar_prot      (l3_mesh_slave_axi_AR.PROT),
            .ar_qos       (l3_mesh_slave_axi_AR.QOS),
            .ar_region    (l3_mesh_slave_axi_ARREGION),
            .ar_user      (l3_mesh_slave_axi_AR.USER),
            .ar_valid     (l3_mesh_slave_axi_ARVALID),
            .ar_ready     (l3_mesh_slave_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (l3_mesh_slave_ar_trans_id),

            .r_last       (l3_mesh_slave_axi_R.LAST),
            .r_user       (l3_mesh_slave_axi_RUSER),
            .r_id         (l3_mesh_slave_axi_R.ID),
            .r_data       (l3_mesh_slave_axi_R.DATA),
            .r_resp       (l3_mesh_slave_axi_R.RESP),
            .r_valid      (l3_mesh_slave_axi_RVALID),
            .r_ready      (l3_mesh_slave_axi_RREADY),
            .r_trans_id   (l3_r_trans_id)
         );

         // UC to l3 mesh master
         evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                             .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_uc_to_l3_mesh_master_axi (
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
        
            .aw_id        (uc_to_l3_mesh_master_axi_AW.ID),
            .aw_addr      (uc_to_l3_mesh_master_axi_AW.ADDR),
            .aw_len       (uc_to_l3_mesh_master_axi_AW.LEN),
            .aw_size      (uc_to_l3_mesh_master_axi_AW.SIZE),
            .aw_burst     (uc_to_l3_mesh_master_axi_AW.BURST),
            .aw_lock      (uc_to_l3_mesh_master_axi_AW.LOCK),
            .aw_cache     (uc_to_l3_mesh_master_axi_AW.CACHE),
            .aw_prot      (uc_to_l3_mesh_master_axi_AW.PROT),
            .aw_qos       (uc_to_l3_mesh_master_axi_AW.QOS),
            .aw_region    (uc_to_l3_mesh_master_axi_AWREGION),
            .aw_user      (uc_to_l3_mesh_master_axi_AW.USER),
            .aw_valid     (uc_to_l3_mesh_master_axi_AWVALID),
            .aw_ready     (uc_to_l3_mesh_master_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),//(uc_to_l3_aw_trans_id),
        
            .w_user       (uc_to_l3_mesh_master_axi_WUSER),
            .w_data       (uc_to_l3_mesh_master_axi_W.DATA),
            .w_strb       (uc_to_l3_mesh_master_axi_W.STRB),
            .w_last       (uc_to_l3_mesh_master_axi_W.LAST),
            .w_valid      (uc_to_l3_mesh_master_axi_WVALID),
            .w_ready      (uc_to_l3_mesh_master_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (),//(uc_to_l3_w_trans_id),
        
            .b_user       (uc_to_l3_mesh_master_axi_BUSER),
            .b_id         (uc_to_l3_mesh_master_axi_B.ID),
            .b_resp       (uc_to_l3_mesh_master_axi_B.RESP),
            .b_valid      (uc_to_l3_mesh_master_axi_BVALID),
            .b_ready      (uc_to_l3_mesh_master_axi_BREADY),
            .b_trans_id   (),//(uc_to_l3_b_trans_id),
        
            .ar_id        (uc_to_l3_mesh_master_axi_AR.ID),
            .ar_addr      (uc_to_l3_mesh_master_axi_AR.ADDR),
            .ar_len       (uc_to_l3_mesh_master_axi_AR.LEN),
            .ar_size      (uc_to_l3_mesh_master_axi_AR.SIZE),
            .ar_burst     (uc_to_l3_mesh_master_axi_AR.BURST),
            .ar_lock      (uc_to_l3_mesh_master_axi_AR.LOCK),
            .ar_cache     (uc_to_l3_mesh_master_axi_AR.CACHE),
            .ar_prot      (uc_to_l3_mesh_master_axi_AR.PROT),
            .ar_qos       (uc_to_l3_mesh_master_axi_AR.QOS),
            .ar_region    (uc_to_l3_mesh_master_axi_ARREGION),
            .ar_user      (uc_to_l3_mesh_master_axi_AR.USER),
            .ar_valid     (uc_to_l3_mesh_master_axi_ARVALID),
            .ar_ready     (uc_to_l3_mesh_master_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),//(uc_to_l3_ar_trans_id),
        
            .r_last       (uc_to_l3_mesh_master_axi_R.LAST),
            .r_user       (uc_to_l3_mesh_master_axi_RUSER),
            .r_id         (uc_to_l3_mesh_master_axi_R.ID),
            .r_data       (uc_to_l3_mesh_master_axi_R.DATA),
            .r_resp       (uc_to_l3_mesh_master_axi_R.RESP),
            .r_valid      (uc_to_l3_mesh_master_axi_RVALID),
            .r_ready      (uc_to_l3_mesh_master_axi_RREADY),
            .r_trans_id   ()//(uc_to_l3_r_trans_id)
         );

         // UC to sys mesh master
         evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                             .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH (3),
                             .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_uc_to_sys_mesh_master_axi (
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
         
            .aw_id        (uc_to_sys_mesh_master_axi_AW.ID),
            .aw_addr      (uc_to_sys_mesh_master_axi_AW.ADDR),
            .aw_len       (uc_to_sys_mesh_master_axi_AW.LEN),
            .aw_size      (uc_to_sys_mesh_master_axi_AW.SIZE),
            .aw_burst     (uc_to_sys_mesh_master_axi_AW.BURST),
            .aw_lock      (uc_to_sys_mesh_master_axi_AW.LOCK),
            .aw_cache     (uc_to_sys_mesh_master_axi_AW.CACHE),
            .aw_prot      (uc_to_sys_mesh_master_axi_AW.PROT),
            .aw_qos       (uc_to_sys_mesh_master_axi_AW.QOS),
            .aw_region    (uc_to_sys_mesh_master_axi_AWREGION),
            .aw_user      (uc_to_sys_mesh_master_axi_AW.USER),
            .aw_valid     (uc_to_sys_mesh_master_axi_AWVALID),
            .aw_ready     (uc_to_sys_mesh_master_axi_AWREADY),
            .aw_vc_valid  (),
            .aw_vc_credit (),
            .aw_trans_id  (),//(uc_to_sys_aw_trans_id),
         
            .w_user       (uc_to_sys_mesh_master_axi_WUSER),
            .w_data       (uc_to_sys_mesh_master_axi_W.DATA),
            .w_strb       (uc_to_sys_mesh_master_axi_W.STRB),
            .w_last       (uc_to_sys_mesh_master_axi_W.LAST),
            .w_valid      (uc_to_sys_mesh_master_axi_WVALID),
            .w_ready      (uc_to_sys_mesh_master_axi_WREADY),
            .w_vc_valid   (),
            .w_trans_id   (),//(uc_to_sys_w_trans_id),
         
            .b_user       (uc_to_sys_mesh_master_axi_BUSER),
            .b_id         (uc_to_sys_mesh_master_axi_B.ID),
            .b_resp       (uc_to_sys_mesh_master_axi_B.RESP),
            .b_valid      (uc_to_sys_mesh_master_axi_BVALID),
            .b_ready      (uc_to_sys_mesh_master_axi_BREADY),
            .b_trans_id   (),//(uc_to_sys_b_trans_id),
         
            .ar_id        (uc_to_sys_mesh_master_axi_AR.ID),
            .ar_addr      (uc_to_sys_mesh_master_axi_AR.ADDR),
            .ar_len       (uc_to_sys_mesh_master_axi_AR.LEN),
            .ar_size      (uc_to_sys_mesh_master_axi_AR.SIZE),
            .ar_burst     (uc_to_sys_mesh_master_axi_AR.BURST),
            .ar_lock      (uc_to_sys_mesh_master_axi_AR.LOCK),
            .ar_cache     (uc_to_sys_mesh_master_axi_AR.CACHE),
            .ar_prot      (uc_to_sys_mesh_master_axi_AR.PROT),
            .ar_qos       (uc_to_sys_mesh_master_axi_AR.QOS),
            .ar_region    (uc_to_sys_mesh_master_axi_ARREGION),
            .ar_user      (uc_to_sys_mesh_master_axi_AR.USER),
            .ar_valid     (uc_to_sys_mesh_master_axi_ARVALID),
            .ar_ready     (uc_to_sys_mesh_master_axi_ARREADY),
            .ar_vc_valid  (),
            .ar_vc_credit (),
            .ar_trans_id  (),//(uc_to_sys_ar_trans_id),
         
            .r_last       (uc_to_sys_mesh_master_axi_R.LAST),
            .r_user       (uc_to_sys_mesh_master_axi_RUSER),
            .r_id         (uc_to_sys_mesh_master_axi_R.ID),
            .r_data       (uc_to_sys_mesh_master_axi_R.DATA),
            .r_resp       (uc_to_sys_mesh_master_axi_R.RESP),
            .r_valid      (uc_to_sys_mesh_master_axi_RVALID),
            .r_ready      (uc_to_sys_mesh_master_axi_RREADY),
            .r_trans_id   ()//(uc_to_sys_r_trans_id)
         );

   //--------------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me   = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                 SHIRE_RTL_AGENT,
                                                                 SHIRE_INST_ID,
                                                                 "evl_shire_env", "", "", "", { 1 });


   //--------------------------------------------------------------------------------------------------
   //
   // Create a Shire RTL port instance and drive it appropriately.
   //
   int                m_shire_inst_id;
   bit                m_start_monitor = 1'b0;
   bit                m_reset_on      = 1'b0;
   bit                m_reset_off     = 1'b0;
   evl_shire_rtl_port m_rtl_port      = new($sformatf("%m"), SHIRE_INST_ID, "", { 1 });

   initial begin
      m_rtl_port.set_int_param_by_name("pk_noc_stub", NOC_STUB);
      m_rtl_port.set_int_param_by_name("pk_sc_stub", SC_STUB);
      m_rtl_port.set_int_param_by_name("pk_neigh_stub", NEIGH_STUB);
      m_shire_inst_id = SHIRE_INST_ID;
      #1ns m_start_monitor = 1'b1;
   end

   always @(posedge clock) begin
      if (m_start_monitor === 1'b1) begin
         if ((reset === 1'b1) && (m_reset_on === 1'b0)) begin
            m_rtl_port.assert_reset();
         end
         else if ((reset === 1'b0) && (m_reset_off === 1'b0)) begin
            m_rtl_port.deassert_reset();
         end
         m_reset_on  <= ~reset;
         m_reset_off <= reset;
         m_rtl_port.ping_clock();
      end
   end

endmodule

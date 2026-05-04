`include "rtl/inc/soc.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

`ifndef __EVL_MEMSHIRE_PKG
   `include "dv/soc/memshire/evl_memshire_pkg.sv"
`endif

module evl_memshire_stub_interface #(parameter int SHIRE_INST_ID = 255) (
      input wire                              reset,
      input wire                              clock,
      inout wire [1:0]                        mem_mesh_slave_axi_ARvcvalid,   // logh (1, maxion) & low (0, minion) virtual channels.  Netspeed naming
      inout wire [1:0]                        mem_mesh_slave_axi_ARcredit,
      inout wire [1:0]                        mem_mesh_slave_axi_AWvcvalid,
      inout wire [1:0]                        mem_mesh_slave_axi_AWcredit,
      inout wire [1:0]                        mem_mesh_slave_axi_Wvcvalid,
      inout wire sc_slave_axi_AR_channel_t    mem_mesh_slave_axi_AR,
      inout wire                              mem_mesh_slave_axi_ARVALID,
      inout wire                              mem_mesh_slave_axi_ARREADY,
      inout wire sc_slave_axi_AW_channel_t    mem_mesh_slave_axi_AW,
      inout wire                              mem_mesh_slave_axi_AWVALID,
      inout wire                              mem_mesh_slave_axi_AWREADY,
      inout wire sc_slave_axi_W_channel_t     mem_mesh_slave_axi_W,
      inout wire                              mem_mesh_slave_axi_WVALID,
      inout wire                              mem_mesh_slave_axi_WREADY,
      inout wire sc_slave_axi_B_channel_t     mem_mesh_slave_axi_B,
      inout wire                              mem_mesh_slave_axi_BVALID,
      inout wire                              mem_mesh_slave_axi_BREADY,
      inout wire sc_slave_axi_R_channel_t     mem_mesh_slave_axi_R,
      inout wire                              mem_mesh_slave_axi_RVALID,
      inout wire                              mem_mesh_slave_axi_RREADY,

      inout wire sc_master_axi_AR_channel_t   atomic_rsp_mesh_master_axi_AR,
      inout wire                              atomic_rsp_mesh_master_axi_ARVALID,
      inout wire                              atomic_rsp_mesh_master_axi_ARREADY,
      inout wire sc_master_axi_AW_channel_t   atomic_rsp_mesh_master_axi_AW,
      inout wire                              atomic_rsp_mesh_master_axi_AWVALID,
      inout wire                              atomic_rsp_mesh_master_axi_AWREADY,
      inout wire sc_master_axi_W_channel_t    atomic_rsp_mesh_master_axi_W,
      inout wire                              atomic_rsp_mesh_master_axi_WVALID,
      inout wire                              atomic_rsp_mesh_master_axi_WREADY,
      inout wire sc_master_axi_B_channel_t    atomic_rsp_mesh_master_axi_B,
      inout wire                              atomic_rsp_mesh_master_axi_BVALID,
      inout wire                              atomic_rsp_mesh_master_axi_BREADY,
      inout wire sc_master_axi_R_channel_t    atomic_rsp_mesh_master_axi_R,
      inout wire                              atomic_rsp_mesh_master_axi_RVALID,
      inout wire                              atomic_rsp_mesh_master_axi_RREADY,

      inout wire sc_slave_axi_AR_channel_t    sbm_mesh_slave_axi_AR,
      inout wire                              sbm_mesh_slave_axi_ARVALID,
      inout wire                              sbm_mesh_slave_axi_ARREADY,
      inout wire sc_slave_axi_AW_channel_t    sbm_mesh_slave_axi_AW,
      inout wire                              sbm_mesh_slave_axi_AWVALID,
      inout wire                              sbm_mesh_slave_axi_AWREADY,
      inout wire sc_slave_axi_W_channel_t     sbm_mesh_slave_axi_W,
      inout wire                              sbm_mesh_slave_axi_WVALID,
      inout wire                              sbm_mesh_slave_axi_WREADY,
      inout wire sc_slave_axi_B_channel_t     sbm_mesh_slave_axi_B,
      inout wire                              sbm_mesh_slave_axi_BVALID,
      inout wire                              sbm_mesh_slave_axi_BREADY,
      inout wire sc_slave_axi_R_channel_t     sbm_mesh_slave_axi_R,
      inout wire                              sbm_mesh_slave_axi_RVALID,
      inout wire                              sbm_mesh_slave_axi_RREADY
   );

   //-----------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the Shire top-level module.  It includes DV-specific
   // objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_memshire_pkg::*;

   //-----------------------------------------------------------------------------------------------
   //
   // Add some unused signals.
   //
   tri0 [`EVL_BUS_REGION_MSB:0] mem_mesh_slave_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] mem_mesh_slave_axi_AWREGION;

   tri0 [`EVL_BUS_REGION_MSB:0] atomic_rsp_mesh_master_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] atomic_rsp_mesh_master_axi_AWREGION;

   tri0 [`EVL_BUS_REGION_MSB:0] sbm_mesh_slave_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] sbm_mesh_slave_axi_AWREGION;

   tri0 [`DV_TRANS_ID_SIZE-1:0] mem_mesh_slave_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] mem_mesh_slave_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] mem_mesh_slave_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] mem_mesh_slave_axi_r_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] mem_mesh_slave_axi_b_trans_id;
   tri0                         mem_mesh_slave_axi_WUSER;
   tri0                         mem_mesh_slave_axi_BUSER;
   tri0                         mem_mesh_slave_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] atomic_rsp_mesh_master_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] atomic_rsp_mesh_master_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] atomic_rsp_mesh_master_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] atomic_rsp_mesh_master_axi_r_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] atomic_rsp_mesh_master_axi_b_trans_id;
   tri0                         atomic_rsp_mesh_master_axi_WUSER;
   tri0                         atomic_rsp_mesh_master_axi_BUSER;
   tri0                         atomic_rsp_mesh_master_axi_RUSER;

   tri0 [`DV_TRANS_ID_SIZE-1:0] sbm_mesh_slave_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] sbm_mesh_slave_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] sbm_mesh_slave_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] sbm_mesh_slave_axi_r_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] sbm_mesh_slave_axi_b_trans_id;
   tri0                         sbm_mesh_slave_axi_WUSER;
   tri0                         sbm_mesh_slave_axi_BUSER;
   tri0                         sbm_mesh_slave_axi_RUSER;

   //
   // Create the mesh-to-memory-shire AXI bus agent interface.
   //
   evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                       .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                       .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH (3),
                       .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AR_VC_WIDTH  (2),
                       .AW_VC_WIDTH  (2),
                       .AR_VC_CREDITS(`MEM_MESH_SLAVE_VSI_DEPTH),
                       .AW_VC_CREDITS(`MEM_MESH_SLAVE_VSI_DEPTH)) m_mesh_to_mem_axi (
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

         .aw_id        (mem_mesh_slave_axi_AW.ID),
         .aw_addr      (mem_mesh_slave_axi_AW.ADDR),
         .aw_len       (mem_mesh_slave_axi_AW.LEN),
         .aw_size      (mem_mesh_slave_axi_AW.SIZE),
         .aw_burst     (mem_mesh_slave_axi_AW.BURST),
         .aw_lock      (mem_mesh_slave_axi_AW.LOCK),
         .aw_cache     (mem_mesh_slave_axi_AW.CACHE),
         .aw_prot      (mem_mesh_slave_axi_AW.PROT),
         .aw_qos       (mem_mesh_slave_axi_AW.QOS),
         .aw_region    (mem_mesh_slave_axi_AWREGION),
         .aw_user      (mem_mesh_slave_axi_AW.USER),
         .aw_valid     (mem_mesh_slave_axi_AWVALID),
         .aw_ready     (mem_mesh_slave_axi_AWREADY),
         .aw_vc_valid  (mem_mesh_slave_axi_AWvcvalid),
         .aw_vc_credit (mem_mesh_slave_axi_AWcredit),
         .aw_trans_id  (mem_mesh_slave_axi_aw_trans_id),

         .w_user       (mem_mesh_slave_axi_WUSER),
         .w_data       (mem_mesh_slave_axi_W.DATA),
         .w_strb       (mem_mesh_slave_axi_W.STRB),
         .w_last       (mem_mesh_slave_axi_W.LAST),
         .w_valid      (mem_mesh_slave_axi_WVALID),
         .w_ready      (mem_mesh_slave_axi_WREADY),
         .w_vc_valid   (mem_mesh_slave_axi_Wvcvalid),
         .w_trans_id   (mem_mesh_slave_axi_w_trans_id),

         .b_user       (mem_mesh_slave_axi_BUSER),
         .b_id         (mem_mesh_slave_axi_B.ID),
         .b_resp       (mem_mesh_slave_axi_B.RESP),
         .b_valid      (mem_mesh_slave_axi_BVALID),
         .b_ready      (mem_mesh_slave_axi_BREADY),
         .b_trans_id   (mem_mesh_slave_axi_b_trans_id),

         .ar_id        (mem_mesh_slave_axi_AR.ID),
         .ar_addr      (mem_mesh_slave_axi_AR.ADDR),
         .ar_len       (mem_mesh_slave_axi_AR.LEN),
         .ar_size      (mem_mesh_slave_axi_AR.SIZE),
         .ar_burst     (mem_mesh_slave_axi_AR.BURST),
         .ar_lock      (mem_mesh_slave_axi_AR.LOCK),
         .ar_cache     (mem_mesh_slave_axi_AR.CACHE),
         .ar_prot      (mem_mesh_slave_axi_AR.PROT),
         .ar_qos       (mem_mesh_slave_axi_AR.QOS),
         .ar_region    (mem_mesh_slave_axi_ARREGION),
         .ar_user      (mem_mesh_slave_axi_AR.USER),
         .ar_valid     (mem_mesh_slave_axi_ARVALID),
         .ar_ready     (mem_mesh_slave_axi_ARREADY),
         .ar_vc_valid  (mem_mesh_slave_axi_ARvcvalid),
         .ar_vc_credit (mem_mesh_slave_axi_ARcredit),
         .ar_trans_id  (mem_mesh_slave_axi_ar_trans_id),

         .r_last       (mem_mesh_slave_axi_R.LAST),
         .r_user       (mem_mesh_slave_axi_RUSER),
         .r_id         (mem_mesh_slave_axi_R.ID),
         .r_data       (mem_mesh_slave_axi_R.DATA),
         .r_resp       (mem_mesh_slave_axi_R.RESP),
         .r_valid      (mem_mesh_slave_axi_RVALID),
         .r_ready      (mem_mesh_slave_axi_RREADY),
         .r_trans_id   (mem_mesh_slave_axi_r_trans_id)
      );

   //
   // Create the memory-shire-to-mesh atomic AXI bus agent interface.
   //
   evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                       .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                       .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH (3),
                       .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_mem_atomic_to_mesh_axi (
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

         .aw_id        (atomic_rsp_mesh_master_axi_AW.ID),
         .aw_addr      (atomic_rsp_mesh_master_axi_AW.ADDR),
         .aw_len       (atomic_rsp_mesh_master_axi_AW.LEN),
         .aw_size      (atomic_rsp_mesh_master_axi_AW.SIZE),
         .aw_burst     (atomic_rsp_mesh_master_axi_AW.BURST),
         .aw_lock      (atomic_rsp_mesh_master_axi_AW.LOCK),
         .aw_cache     (atomic_rsp_mesh_master_axi_AW.CACHE),
         .aw_prot      (atomic_rsp_mesh_master_axi_AW.PROT),
         .aw_qos       (atomic_rsp_mesh_master_axi_AW.QOS),
         .aw_region    (atomic_rsp_mesh_master_axi_AWREGION),
         .aw_user      (atomic_rsp_mesh_master_axi_AW.USER),
         .aw_valid     (atomic_rsp_mesh_master_axi_AWVALID),
         .aw_ready     (atomic_rsp_mesh_master_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (atomic_rsp_mesh_master_axi_aw_trans_id),

         .w_user       (atomic_rsp_mesh_master_axi_WUSER),
         .w_data       (atomic_rsp_mesh_master_axi_W.DATA),
         .w_strb       (atomic_rsp_mesh_master_axi_W.STRB),
         .w_last       (atomic_rsp_mesh_master_axi_W.LAST),
         .w_valid      (atomic_rsp_mesh_master_axi_WVALID),
         .w_ready      (atomic_rsp_mesh_master_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (atomic_rsp_mesh_master_axi_w_trans_id),

         .b_user       (atomic_rsp_mesh_master_axi_BUSER),
         .b_id         (atomic_rsp_mesh_master_axi_B.ID),
         .b_resp       (atomic_rsp_mesh_master_axi_B.RESP),
         .b_valid      (atomic_rsp_mesh_master_axi_BVALID),
         .b_ready      (atomic_rsp_mesh_master_axi_BREADY),
         .b_trans_id   (atomic_rsp_mesh_master_axi_b_trans_id),

         .ar_id        (atomic_rsp_mesh_master_axi_AR.ID),
         .ar_addr      (atomic_rsp_mesh_master_axi_AR.ADDR),
         .ar_len       (atomic_rsp_mesh_master_axi_AR.LEN),
         .ar_size      (atomic_rsp_mesh_master_axi_AR.SIZE),
         .ar_burst     (atomic_rsp_mesh_master_axi_AR.BURST),
         .ar_lock      (atomic_rsp_mesh_master_axi_AR.LOCK),
         .ar_cache     (atomic_rsp_mesh_master_axi_AR.CACHE),
         .ar_prot      (atomic_rsp_mesh_master_axi_AR.PROT),
         .ar_qos       (atomic_rsp_mesh_master_axi_AR.QOS),
         .ar_region    (atomic_rsp_mesh_master_axi_ARREGION),
         .ar_user      (atomic_rsp_mesh_master_axi_AR.USER),
         .ar_valid     (atomic_rsp_mesh_master_axi_ARVALID),
         .ar_ready     (atomic_rsp_mesh_master_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (atomic_rsp_mesh_master_axi_ar_trans_id),

         .r_last       (atomic_rsp_mesh_master_axi_R.LAST),
         .r_user       (atomic_rsp_mesh_master_axi_RUSER),
         .r_id         (atomic_rsp_mesh_master_axi_R.ID),
         .r_data       (atomic_rsp_mesh_master_axi_R.DATA),
         .r_resp       (atomic_rsp_mesh_master_axi_R.RESP),
         .r_valid      (atomic_rsp_mesh_master_axi_RVALID),
         .r_ready      (atomic_rsp_mesh_master_axi_RREADY),
         .r_trans_id   (atomic_rsp_mesh_master_axi_r_trans_id)
      );

   //
   // Create the mesh-to-memory-shire-SBM AXI bus agent interface.
   //
   evl_axi_interface #(.SHIRE_ID      (SHIRE_INST_ID),
                       .ID_WIDTH      (`SC_MESH_SLAVE_AXI_ID_SIZE),
                       .ADDR_WIDTH    (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH    (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH  (3),
                       .ARUSER_WIDTH  (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH  (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AR_VC_WIDTH   (1),
                       .AW_VC_WIDTH   (1),
                       .AR_VC_CREDITS (0),
                       .AW_VC_CREDITS (0)) m_mesh_to_mem_sbm_axi (
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

         .aw_id        (sbm_mesh_slave_axi_AW.ID),
         .aw_addr      (sbm_mesh_slave_axi_AW.ADDR),
         .aw_len       (sbm_mesh_slave_axi_AW.LEN),
         .aw_size      (sbm_mesh_slave_axi_AW.SIZE),
         .aw_burst     (sbm_mesh_slave_axi_AW.BURST),
         .aw_lock      (sbm_mesh_slave_axi_AW.LOCK),
         .aw_cache     (sbm_mesh_slave_axi_AW.CACHE),
         .aw_prot      (sbm_mesh_slave_axi_AW.PROT),
         .aw_qos       (sbm_mesh_slave_axi_AW.QOS),
         .aw_region    (sbm_mesh_slave_axi_AWREGION),
         .aw_user      (sbm_mesh_slave_axi_AW.USER),
         .aw_valid     (sbm_mesh_slave_axi_AWVALID),
         //.aw_valid     (),
         .aw_ready     (sbm_mesh_slave_axi_AWREADY),
         .aw_vc_valid  ({sbm_mesh_slave_axi_AWVALID,1'b0}),
         .aw_vc_credit ({sbm_mesh_slave_axi_AWVALID,1'b0}),
         //.aw_vc_valid  (),
         //.aw_vc_credit (),
         .aw_trans_id  (sbm_mesh_slave_axi_aw_trans_id),

         .w_user       (sbm_mesh_slave_axi_WUSER),
         .w_data       (sbm_mesh_slave_axi_W.DATA),
         .w_strb       (sbm_mesh_slave_axi_W.STRB),
         .w_last       (sbm_mesh_slave_axi_W.LAST),
         .w_valid      (sbm_mesh_slave_axi_WVALID),
         .w_ready      (sbm_mesh_slave_axi_WREADY),
         .w_vc_valid   ({sbm_mesh_slave_axi_AWVALID,1'b0}),
         //.w_vc_valid   (),
         .w_trans_id   (sbm_mesh_slave_axi_w_trans_id),

         .b_user       (sbm_mesh_slave_axi_BUSER),
         .b_id         (sbm_mesh_slave_axi_B.ID),
         .b_resp       (sbm_mesh_slave_axi_B.RESP),
         .b_valid      (sbm_mesh_slave_axi_BVALID),
         .b_ready      (sbm_mesh_slave_axi_BREADY),
         .b_trans_id   (sbm_mesh_slave_axi_b_trans_id),

         .ar_id        (sbm_mesh_slave_axi_AR.ID),
         .ar_addr      (sbm_mesh_slave_axi_AR.ADDR),
         .ar_len       (sbm_mesh_slave_axi_AR.LEN),
         .ar_size      (sbm_mesh_slave_axi_AR.SIZE),
         .ar_burst     (sbm_mesh_slave_axi_AR.BURST),
         .ar_lock      (sbm_mesh_slave_axi_AR.LOCK),
         .ar_cache     (sbm_mesh_slave_axi_AR.CACHE),
         .ar_prot      (sbm_mesh_slave_axi_AR.PROT),
         .ar_qos       (sbm_mesh_slave_axi_AR.QOS),
         .ar_region    (sbm_mesh_slave_axi_ARREGION),
         .ar_user      (sbm_mesh_slave_axi_AR.USER),
         .ar_valid     (sbm_mesh_slave_axi_ARVALID),
         .ar_ready     (sbm_mesh_slave_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (sbm_mesh_slave_axi_ar_trans_id),

         .r_last       (sbm_mesh_slave_axi_R.LAST),
         .r_user       (sbm_mesh_slave_axi_RUSER),
         .r_id         (sbm_mesh_slave_axi_R.ID),
         .r_data       (sbm_mesh_slave_axi_R.DATA),
         .r_resp       (sbm_mesh_slave_axi_R.RESP),
         .r_valid      (sbm_mesh_slave_axi_RVALID),
         .r_ready      (sbm_mesh_slave_axi_RREADY),
         .r_trans_id   (sbm_mesh_slave_axi_r_trans_id)
      );



   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                               MEM_SHIRE_RTL_AGENT,
                                                               SHIRE_INST_ID,
                                                               "evl_memshire_env", "", "", "", { 1 });


   //-----------------------------------------------------------------------------------------------
   //
   // Create a Shire RTL port instance and drive it appropriately.
   //
   int                   m_shire_inst_id;
   bit                   m_start_monitor = 1'b0;
   bit                   m_reset_on      = 1'b0;
   bit                   m_reset_off     = 1'b0;
   evl_memshire_rtl_port m_rtl_port      = new($sformatf("%m"), SHIRE_INST_ID, "", { 1 });

   initial begin
      m_shire_inst_id = SHIRE_INST_ID;
      #1ns m_start_monitor = 1'b1;
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
         m_reset_on  <= ~reset;
         m_reset_off <= reset;
         m_rtl_port.ping_clock();
      end
   end

endmodule

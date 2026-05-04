`ifndef __EVL_PSHIRE_INTERFACE
   `define __EVL_PSHIRE_INTERFACE 1
   `include "rtl/inc/soc.vh"

   `ifndef __EVL_PSHIRE_PKG
      `include "dv/soc/pshire/evl_pshire_pkg.sv"
   `endif

   module evl_pshire_stub_interface #(parameter int    PSHIRE_ID    = 0,
                                      parameter bit    SP_IS_STUB = 1'b0,
                                      parameter bit    SHIRE_STIM = 1'b0,
                                      parameter int    NUM_L3_PORTS  = 1,
                                      parameter string ROM_FILE_NAME = "spROMImage.hex",
                                      parameter string RAM_FILE_NAME = "spRAMImage.hex") (
         input wire reset,
         input wire clock,

         inout wire sc_slave_axi_AR_channel_t  ps0_ps_AR,
         inout wire                            ps0_ps_ARVALID,
         inout wire                            ps0_ps_ARREADY,
         inout wire sc_slave_axi_AW_channel_t  ps0_ps_AW,
         inout wire                            ps0_ps_AWVALID,
         inout wire                            ps0_ps_AWREADY,
         inout wire sc_slave_axi_W_channel_t   ps0_ps_W,
         inout wire                            ps0_ps_WVALID,
         inout wire                            ps0_ps_WREADY,
         inout wire sc_slave_axi_B_channel_t   ps0_ps_B,
         inout wire                            ps0_ps_BVALID,
         inout wire                            ps0_ps_BREADY,
         inout wire sc_slave_axi_R_channel_t   ps0_ps_R,
         inout wire                            ps0_ps_RVALID,
         inout wire                            ps0_ps_RREADY,

         inout wire sc_master_axi_AR_channel_t ps0_tosys_m_AR,
         inout wire                            ps0_tosys_m_ARVALID,
         inout wire                            ps0_tosys_m_ARREADY,
         inout wire sc_master_axi_AW_channel_t ps0_tosys_m_AW,
         inout wire                            ps0_tosys_m_AWVALID,
         inout wire                            ps0_tosys_m_AWREADY,
         inout wire sc_master_axi_W_channel_t  ps0_tosys_m_W,
         inout wire                            ps0_tosys_m_WVALID,
         inout wire                            ps0_tosys_m_WREADY,
         inout wire sc_master_axi_B_channel_t  ps0_tosys_m_B,
         inout wire                            ps0_tosys_m_BVALID,
         inout wire                            ps0_tosys_m_BREADY,
         inout wire sc_master_axi_R_channel_t  ps0_tosys_m_R,
         inout wire                            ps0_tosys_m_RVALID,
         inout wire                            ps0_tosys_m_RREADY,

         inout wire sc_master_axi_AR_channel_t [NUM_L3_PORTS-1:0] ps0_tol3_m_AR,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_ARVALID,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_ARREADY,
         inout wire sc_master_axi_AW_channel_t [NUM_L3_PORTS-1:0] ps0_tol3_m_AW,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_AWVALID,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_AWREADY,
         inout wire sc_master_axi_W_channel_t  [NUM_L3_PORTS-1:0] ps0_tol3_m_W,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_WVALID,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_WREADY,
         inout wire sc_master_axi_B_channel_t  [NUM_L3_PORTS-1:0] ps0_tol3_m_B,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_BVALID,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_BREADY,
         inout wire sc_master_axi_R_channel_t  [NUM_L3_PORTS-1:0] ps0_tol3_m_R,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_RVALID,
         inout wire                            [NUM_L3_PORTS-1:0] ps0_tol3_m_RREADY
      );
    
    
      //-----------------------------------------------------------------------------------------------
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_pshire_pkg::*;
    
    
    
         //-----------------------------------------------------------------------------------------------
         //
         // Add some unused signals.
         //

         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] ps0_tol3_m_aw_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] ps0_tol3_m_w_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] ps0_tol3_m_b_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] ps0_tol3_m_ar_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`DV_TRANS_ID_SIZE-1:0] ps0_tol3_m_r_trans_id;
         tri0 [NUM_L3_PORTS-1:0] [`EVL_BUS_REGION_MSB:0] ps0_tol3_m_ARREGION;
         tri0 [NUM_L3_PORTS-1:0] [`EVL_BUS_REGION_MSB:0] ps0_tol3_m_AWREGION;
         tri0 [NUM_L3_PORTS-1:0]                         ps0_tol3_m_WUSER;
         tri0 [NUM_L3_PORTS-1:0]                         ps0_tol3_m_BUSER;
         tri0 [NUM_L3_PORTS-1:0]                         ps0_tol3_m_RUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_pm_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_pm_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_pm_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_pm_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_pm_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_pm_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_pm_AWREGION;
         tri0                         ps0_pm_WUSER;
         tri0                         ps0_pm_BUSER;
         tri0                         ps0_pm_RUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_ps_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_ps_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_ps_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_ps_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_ps_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_ps_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_ps_AWREGION;
         tri0                         ps0_ps_WUSER;
         tri0                         ps0_ps_BUSER;
         tri0                         ps0_ps_RUSER;

         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_tosys_m_aw_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_tosys_m_w_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_tosys_m_b_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_tosys_m_ar_trans_id;
         tri0 [`DV_TRANS_ID_SIZE-1:0] ps0_tosys_m_r_trans_id;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_tosys_m_ARREGION;
         tri0 [`EVL_BUS_REGION_MSB:0] ps0_tosys_m_AWREGION;
         tri0                         ps0_tosys_m_WUSER;
         tri0                         ps0_tosys_m_BUSER;
         tri0                         ps0_tosys_m_RUSER;

         genvar mesh_idx;
         generate
            for (mesh_idx = 0; mesh_idx < NUM_L3_PORTS; mesh_idx = mesh_idx + 1) begin : g_ps_tol3mesh_if
               //
               // Create slave AXI mesh bus agent interface.
               //
               evl_axi_interface #(.SHIRE_ID     (PSHIRE_ID),
                                   .PORT_ID      (mesh_idx),
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

                        .aw_id        (ps0_tol3_m_AW[mesh_idx].ID),
                        .aw_addr      (ps0_tol3_m_AW[mesh_idx].ADDR),
                        .aw_len       (ps0_tol3_m_AW[mesh_idx].LEN),
                        .aw_size      (ps0_tol3_m_AW[mesh_idx].SIZE),
                        .aw_burst     (ps0_tol3_m_AW[mesh_idx].BURST),
                        .aw_lock      (ps0_tol3_m_AW[mesh_idx].LOCK),
                        .aw_cache     (ps0_tol3_m_AW[mesh_idx].CACHE),
                        .aw_prot      (ps0_tol3_m_AW[mesh_idx].PROT),
                        .aw_qos       (ps0_tol3_m_AW[mesh_idx].QOS),
                        .aw_region    (ps0_tol3_m_AWREGION[mesh_idx]),
                        .aw_user      (ps0_tol3_m_AW[mesh_idx].USER),
                        .aw_valid     (ps0_tol3_m_AWVALID[mesh_idx]),
                        .aw_ready     (ps0_tol3_m_AWREADY[mesh_idx]),
                        .aw_vc_valid  (),
                        .aw_vc_credit (),
                        .aw_trans_id  (ps0_tol3_m_aw_trans_id[mesh_idx]),

                        .w_user       (ps0_tol3_m_WUSER[mesh_idx]),
                        .w_data       (ps0_tol3_m_W[mesh_idx].DATA),
                        .w_strb       (ps0_tol3_m_W[mesh_idx].STRB),
                        .w_last       (ps0_tol3_m_W[mesh_idx].LAST),
                        .w_valid      (ps0_tol3_m_WVALID[mesh_idx]),
                        .w_ready      (ps0_tol3_m_WREADY[mesh_idx]),
                        .w_vc_valid   (),
                        .w_trans_id   (ps0_tol3_m_w_trans_id[mesh_idx]),

                        .b_user       (ps0_tol3_m_BUSER[mesh_idx]),
                        .b_id         (ps0_tol3_m_B[mesh_idx].ID),
                        .b_resp       (ps0_tol3_m_B[mesh_idx].RESP),
                        .b_valid      (ps0_tol3_m_BVALID[mesh_idx]),
                        .b_ready      (ps0_tol3_m_BREADY[mesh_idx]),
                        .b_trans_id   (ps0_tol3_m_b_trans_id[mesh_idx]),

                        .ar_id        (ps0_tol3_m_AR[mesh_idx].ID),
                        .ar_addr      (ps0_tol3_m_AR[mesh_idx].ADDR),
                        .ar_len       (ps0_tol3_m_AR[mesh_idx].LEN),
                        .ar_size      (ps0_tol3_m_AR[mesh_idx].SIZE),
                        .ar_burst     (ps0_tol3_m_AR[mesh_idx].BURST),
                        .ar_lock      (ps0_tol3_m_AR[mesh_idx].LOCK),
                        .ar_cache     (ps0_tol3_m_AR[mesh_idx].CACHE),
                        .ar_prot      (ps0_tol3_m_AR[mesh_idx].PROT),
                        .ar_qos       (ps0_tol3_m_AR[mesh_idx].QOS),
                        .ar_region    (ps0_tol3_m_ARREGION[mesh_idx]),
                        .ar_user      (ps0_tol3_m_AR[mesh_idx].USER),
                        .ar_valid     (ps0_tol3_m_ARVALID[mesh_idx]),
                        .ar_ready     (ps0_tol3_m_ARREADY[mesh_idx]),
                        .ar_vc_valid  (),
                        .ar_vc_credit (),
                        .ar_trans_id  (ps0_tol3_m_ar_trans_id[mesh_idx]),

                        .r_last       (ps0_tol3_m_R[mesh_idx].LAST),
                        .r_user       (ps0_tol3_m_RUSER[mesh_idx]),
                        .r_id         (ps0_tol3_m_R[mesh_idx].ID),
                        .r_data       (ps0_tol3_m_R[mesh_idx].DATA),
                        .r_resp       (ps0_tol3_m_R[mesh_idx].RESP),
                        .r_valid      (ps0_tol3_m_RVALID[mesh_idx]),
                        .r_ready      (ps0_tol3_m_RREADY[mesh_idx]),
                        .r_trans_id   (ps0_tol3_m_r_trans_id[mesh_idx])
                  );
            end
         endgenerate

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

               .aw_id        (ps0_ps_AW.ID),
               .aw_addr      (ps0_ps_AW.ADDR),
               .aw_len       (ps0_ps_AW.LEN),
               .aw_size      (ps0_ps_AW.SIZE),
               .aw_burst     (ps0_ps_AW.BURST),
               .aw_lock      (ps0_ps_AW.LOCK),
               .aw_cache     (ps0_ps_AW.CACHE),
               .aw_prot      (ps0_ps_AW.PROT),
               .aw_qos       (ps0_ps_AW.QOS),
               .aw_region    (ps0_ps_AWREGION),
               .aw_user      (ps0_ps_AW.USER),
               .aw_valid     (ps0_ps_AWVALID),
               .aw_ready     (ps0_ps_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (ps0_ps_aw_trans_id),

               .w_user       (ps0_ps_WUSER),
               .w_data       (ps0_ps_W.DATA),
               .w_strb       (ps0_ps_W.STRB),
               .w_last       (ps0_ps_W.LAST),
               .w_valid      (ps0_ps_WVALID),
               .w_ready      (ps0_ps_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (ps0_ps_w_trans_id),

               .b_user       (ps0_ps_BUSER),
               .b_id         (ps0_ps_B.ID),
               .b_resp       (ps0_ps_B.RESP),
               .b_valid      (ps0_ps_BVALID),
               .b_ready      (ps0_ps_BREADY),
               .b_trans_id   (ps0_ps_b_trans_id),

               .ar_id        (ps0_ps_AR.ID),
               .ar_addr      (ps0_ps_AR.ADDR),
               .ar_len       (ps0_ps_AR.LEN),
               .ar_size      (ps0_ps_AR.SIZE),
               .ar_burst     (ps0_ps_AR.BURST),
               .ar_lock      (ps0_ps_AR.LOCK),
               .ar_cache     (ps0_ps_AR.CACHE),
               .ar_prot      (ps0_ps_AR.PROT),
               .ar_qos       (ps0_ps_AR.QOS),
               .ar_region    (ps0_ps_ARREGION),
               .ar_user      (ps0_ps_AR.USER),
               .ar_valid     (ps0_ps_ARVALID),
               .ar_ready     (ps0_ps_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (ps0_ps_ar_trans_id),

               .r_last       (ps0_ps_R.LAST),
               .r_user       (ps0_ps_RUSER),
               .r_id         (ps0_ps_R.ID),
               .r_data       (ps0_ps_R.DATA),
               .r_resp       (ps0_ps_R.RESP),
               .r_valid      (ps0_ps_RVALID),
               .r_ready      (ps0_ps_RREADY),
               .r_trans_id   (ps0_ps_r_trans_id)
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
            
               .aw_id        (ps0_tosys_m_AW.ID),
               .aw_addr      (ps0_tosys_m_AW.ADDR),
               .aw_len       (ps0_tosys_m_AW.LEN),
               .aw_size      (ps0_tosys_m_AW.SIZE),
               .aw_burst     (ps0_tosys_m_AW.BURST),
               .aw_lock      (ps0_tosys_m_AW.LOCK),
               .aw_cache     (ps0_tosys_m_AW.CACHE),
               .aw_prot      (ps0_tosys_m_AW.PROT),
               .aw_qos       (ps0_tosys_m_AW.QOS),
               .aw_region    (ps0_tosys_m_AWREGION),
               .aw_user      (ps0_tosys_m_AW.USER),
               .aw_valid     (ps0_tosys_m_AWVALID),
               .aw_ready     (ps0_tosys_m_AWREADY),
               .aw_vc_valid  (),
               .aw_vc_credit (),
               .aw_trans_id  (),//(tosys_aw_trans_id),
            
               .w_user       (ps0_tosys_m_WUSER),
               .w_data       (ps0_tosys_m_W.DATA),
               .w_strb       (ps0_tosys_m_W.STRB),
               .w_last       (ps0_tosys_m_W.LAST),
               .w_valid      (ps0_tosys_m_WVALID),
               .w_ready      (ps0_tosys_m_WREADY),
               .w_vc_valid   (),
               .w_trans_id   (),//(tosys_w_trans_id),
            
               .b_user       (ps0_tosys_m_BUSER),
               .b_id         (ps0_tosys_m_B.ID),
               .b_resp       (ps0_tosys_m_B.RESP),
               .b_valid      (ps0_tosys_m_BVALID),
               .b_ready      (ps0_tosys_m_BREADY),
               .b_trans_id   (),//(tosys_b_trans_id),
            
               .ar_id        (ps0_tosys_m_AR.ID),
               .ar_addr      (ps0_tosys_m_AR.ADDR),
               .ar_len       (ps0_tosys_m_AR.LEN),
               .ar_size      (ps0_tosys_m_AR.SIZE),
               .ar_burst     (ps0_tosys_m_AR.BURST),
               .ar_lock      (ps0_tosys_m_AR.LOCK),
               .ar_cache     (ps0_tosys_m_AR.CACHE),
               .ar_prot      (ps0_tosys_m_AR.PROT),
               .ar_qos       (ps0_tosys_m_AR.QOS),
               .ar_region    (ps0_tosys_m_ARREGION),
               .ar_user      (ps0_tosys_m_AR.USER),
               .ar_valid     (ps0_tosys_m_ARVALID),
               .ar_ready     (ps0_tosys_m_ARREADY),
               .ar_vc_valid  (),
               .ar_vc_credit (),
               .ar_trans_id  (),//(tosys_ar_trans_id),
            
               .r_last       (ps0_tosys_m_R.LAST),
               .r_user       (ps0_tosys_m_RUSER),
               .r_id         (ps0_tosys_m_R.ID),
               .r_data       (ps0_tosys_m_R.DATA),
               .r_resp       (ps0_tosys_m_R.RESP),
               .r_valid      (ps0_tosys_m_RVALID),
               .r_ready      (ps0_tosys_m_RREADY),
               .r_trans_id   ()//(tosys_r_trans_id)
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
      bit                  m_mem_loaded    = 1'b0;
      string               m_rom_file_name = ROM_FILE_NAME;
      string               m_ram_file_name = RAM_FILE_NAME;
      string               m_full_file_name;
      string               m_plus_arg;
    
      initial begin
         m_rtl_port.set_int_param_by_name("pk_sp_is_stub", ((SP_IS_STUB == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_pshire_stim", ((SHIRE_STIM == 0) ? 0 : 1));
         m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_PORTS);
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

`endif // ifndef __EVL_PSHIRE_INTERFACE

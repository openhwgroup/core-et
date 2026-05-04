`include "rtl/inc/soc.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

`ifndef __EVL_SHIRE_PKG
   `include "dv/soc/shire/evl_shire_pkg.sv"
`endif

`ifndef EVL_IFC_PORT
   `ifdef EVL_SIMULATION
      `define EVL_IFC_PORT inout
   `else // ifdef EVL_SIMULATION
      `define EVL_IFC_PORT input
   `endif // !ifdef EVL_SIMULATION
`endif // ifndef EVL_IFC_PORT

`ifndef SHIRE_IFC
   `define SHIRE_IFC shire_top.shire_ifc
`endif // ifndef SHIRE_IFC

module evl_shire_interface #(parameter int SHIRE_INST_ID       = 255,
                             parameter int NUM_NEIGH           = 1,
                             parameter int NUM_RBOX            = 1,
                             parameter int NUM_BANKS           = 1,
                             parameter int NUM_UC              = 1,
                             parameter bit CAPTURE_DPI_EVENTS  = 1'b0,
                             parameter bit NOC_STUB            = 1'b0,
                             parameter bit SC_STUB             = 1'b0,
                             parameter bit SC_LITE             = 1'b0,
                             parameter int NUM_L3_MASTER_PORTS = 1,
                             parameter int NUM_L3_SLAVE_PORTS  = 1,
                             parameter int NEIGH_DISABLE       = 0,
                             parameter int NEIGH_STUB          = 0,
                             parameter bit DBGNOC_STIM         = 1'b0) (

      input         wire                                                            reset_cold,
      input         wire                                                            shire_clock,
      input         wire                                                            noc_clock,
      `ifdef ET_AXI_DPI_MONITOR
         input      wire                                                            dpi_capture_enable,
         input      wire                                                            dpi_read_clock,
         input      wire                                                            dpi_packet_pop,
         output     wire                                                            dpi_packet_valid,
         output     wire [`ET_DPI_PKT_MSB:0]                                        dpi_packet_data,
      `endif // ifdef ET_AXI_DPI_MONITOR
      output        wire                                                            dv_force_dmactive,
      input         wire                                                            use_rsp_source_is_uc,
      input         wire                                   [NUM_NEIGH+NUM_RBOX-1:0] rsp_source_is_uc,
      input         wire                    [NUM_NEIGH-1:0][`MIN_PER_N-1:0]         esr_pwr_ctrl_neigh_iso,
      input         wire                    [NUM_NEIGH-1:0]                         esr_pwr_ctrl_glb_iso,
      `EVL_IFC_PORT wire                    [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0] neigh_sc_req_ready,
      `EVL_IFC_PORT wire                    [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0] neigh_sc_req_valid,
      `EVL_IFC_PORT wire et_link_req_info_t [NUM_NEIGH-1:0]                         neigh_sc_req_info,
      `EVL_IFC_PORT wire                    [NUM_NEIGH-1:0]                         neigh_sc_rsp_ready,
      `EVL_IFC_PORT wire                    [NUM_NEIGH-1:0]                         neigh_sc_rsp_valid,
      `EVL_IFC_PORT wire et_link_rsp_info_t [NUM_NEIGH-1:0]                         neigh_sc_rsp_info,
      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_AR,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_ARVALID,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_AW,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_AWVALID,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t  [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_W,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_WVALID,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t  [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_B,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_BVALID,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t  [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_R,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_RVALID,
      `EVL_IFC_PORT wire                            [NUM_L3_MASTER_PORTS-1:0]       to_l3_mesh_master_axi_RREADY,
      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t                                 to_sys_mesh_master_axi_AR,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_ARVALID,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t                                 to_sys_mesh_master_axi_AW,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_AWVALID,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t                                  to_sys_mesh_master_axi_W,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_WVALID,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t                                  to_sys_mesh_master_axi_B,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_BVALID,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t                                  to_sys_mesh_master_axi_R,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_RVALID,
      `EVL_IFC_PORT wire                                                            to_sys_mesh_master_axi_RREADY,

      `EVL_IFC_PORT wire sc_slave_axi_AR_channel_t [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_AR,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_ARVALID,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_ARREADY,
      `EVL_IFC_PORT wire sc_slave_axi_AW_channel_t [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_AW,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_AWVALID,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_AWREADY,
      `EVL_IFC_PORT wire sc_slave_axi_W_channel_t  [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_W,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_WVALID,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_WREADY,
      `EVL_IFC_PORT wire sc_slave_axi_B_channel_t  [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_B,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_BVALID,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_BREADY,
      `EVL_IFC_PORT wire sc_slave_axi_R_channel_t  [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_R,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_RVALID,
      `EVL_IFC_PORT wire                           [NUM_L3_SLAVE_PORTS-1:0]         l3_mesh_slave_axi_RREADY,

      `EVL_IFC_PORT wire sys_slave_axi_AR_channel_t                                 sys_mesh_slave_axi_AR,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_ARVALID,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_ARREADY,
      `EVL_IFC_PORT wire sys_slave_axi_AW_channel_t                                 sys_mesh_slave_axi_AW,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_AWVALID,
      `EVL_IFC_PORT wire [1:0]                                                      sys_mesh_slave_axi_AWvcvalid,
      `EVL_IFC_PORT wire [1:0]                                                      sys_mesh_slave_axi_AWcredit,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_AWREADY,
      `EVL_IFC_PORT wire sys_slave_axi_W_channel_t                                  sys_mesh_slave_axi_W,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_WVALID,
      `EVL_IFC_PORT wire [1:0]                                                      sys_mesh_slave_axi_Wvcvalid,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_WREADY,
      `EVL_IFC_PORT wire sys_slave_axi_B_channel_t                                  sys_mesh_slave_axi_B,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_BVALID,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_BREADY,
      `EVL_IFC_PORT wire sys_slave_axi_R_channel_t                                  sys_mesh_slave_axi_R,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_RVALID,
      `EVL_IFC_PORT wire                                                            sys_mesh_slave_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t                                 uc_to_l3_mesh_master_axi_AR,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_ARVALID,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t                                 uc_to_l3_mesh_master_axi_AW,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_AWVALID,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t                                  uc_to_l3_mesh_master_axi_W,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_WVALID,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t                                  uc_to_l3_mesh_master_axi_B,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_BVALID,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t                                  uc_to_l3_mesh_master_axi_R,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_RVALID,
      `EVL_IFC_PORT wire                                                            uc_to_l3_mesh_master_axi_RREADY,

      `EVL_IFC_PORT wire sc_master_axi_AR_channel_t                                 uc_to_sys_mesh_master_axi_AR,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_ARVALID,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_ARREADY,
      `EVL_IFC_PORT wire sc_master_axi_AW_channel_t                                 uc_to_sys_mesh_master_axi_AW,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_AWVALID,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_AWREADY,
      `EVL_IFC_PORT wire sc_master_axi_W_channel_t                                  uc_to_sys_mesh_master_axi_W,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_WVALID,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_WREADY,
      `EVL_IFC_PORT wire sc_master_axi_B_channel_t                                  uc_to_sys_mesh_master_axi_B,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_BVALID,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_BREADY,
      `EVL_IFC_PORT wire sc_master_axi_R_channel_t                                  uc_to_sys_mesh_master_axi_R,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_RVALID,
      `EVL_IFC_PORT wire                                                            uc_to_sys_mesh_master_axi_RREADY,

      `EVL_IFC_PORT wire APB_to_neigh_t   [NUM_NEIGH-1:0]                           apb_esr_req_neigh,
      `EVL_IFC_PORT wire APB_from_neigh_t [NUM_NEIGH-1:0]                           apb_esr_rsp_neigh,

      input         wire                                                            ioshire_combined_err_int
   );


   //-----------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the Shire top-level module.  It includes DV-specific
   // objects and methods for the Esperanto Verification Library.
   //
   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_shire_pkg::*;
      import evl_etlink_types_pkg::evl_etlink_req_opcode_t;
      import evl_etlink_types_pkg::evl_etlink_rsp_opcode_t;
      import evl_etlink_types_pkg::evl_etlink_size_t;
      import evl_etlink_types_pkg::evl_etlink_req_info_t;
      import evl_etlink_types_pkg::evl_etlink_rsp_info_t;
      import evl_etlink_types_pkg::evl_etlink_neigh_source_t;
   `endif // ifdef EVL_SIMULATION


   localparam int MESH_TO_SBM_LAYERS   = ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? 1 : 0;
   localparam int L3_TO_SYSMESH_LAYERS = ((SC_LITE == 1'b1) || ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? 1 : 0;
   localparam int UC_TO_SYSMESH_LAYERS = ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? 1 : 0;
   localparam int L2_TO_L3MESH_LAYERS  = ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? NUM_L3_MASTER_PORTS : 0;
   localparam int L3MESH_TO_L3_LAYERS  = ((SC_LITE == 1'b1) || ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? NUM_L3_SLAVE_PORTS  : 0;
   localparam int UC_TO_L3MESH_LAYERS  = ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || (NOC_STUB == 1'b0))) ? 1 : 0;

   localparam int REAL_AXI_COUNT       = MESH_TO_SBM_LAYERS + L3_TO_SYSMESH_LAYERS + UC_TO_SYSMESH_LAYERS + L2_TO_L3MESH_LAYERS + L3MESH_TO_L3_LAYERS + UC_TO_L3MESH_LAYERS;
   localparam int TOTAL_AXI_COUNT      = (REAL_AXI_COUNT <= 0) ? 1 : REAL_AXI_COUNT;

   localparam int MESH_TO_SBM_DPI_BASE   = 0;
   localparam int L3_TO_SYSMESH_DPI_BASE = MESH_TO_SBM_DPI_BASE   + MESH_TO_SBM_LAYERS;
   localparam int UC_TO_SYSMESH_DPI_BASE = L3_TO_SYSMESH_DPI_BASE + L3_TO_SYSMESH_LAYERS;
   localparam int L2_TO_L3MESH_DPI_BASE  = UC_TO_SYSMESH_DPI_BASE + UC_TO_SYSMESH_LAYERS;
   localparam int L3MESH_TO_L3_DPI_BASE  = L2_TO_L3MESH_DPI_BASE  + L2_TO_L3MESH_LAYERS;
   localparam int UC_TO_L3MESH_DPI_BASE  = L3MESH_TO_L3_DPI_BASE  + L3MESH_TO_L3_LAYERS;

   `ifdef EVL_SIMULATION
      localparam int L2_TO_L3MESH_NO_DRIVE  = ((SC_STUB == 1'b0) && (SC_LITE == 1'b0)) ? 0 : 1;
      localparam int L3_TO_SYSMESH_NO_DRIVE = ((SC_STUB == 1'b0) && (SC_LITE == 1'b0)) ? 0 : 1;
   `else // ifdef EVL_SIMULATION
      localparam int L2_TO_L3MESH_NO_DRIVE  = 1;
      localparam int L3_TO_SYSMESH_NO_DRIVE = 1;
   `endif // !ifdef EVL_SIMULATION

   wire [7:0]     dn_msg_host_dest_hp;
   wire [1:0]     dn_msg_host_dest_int;
   wire [3:0]     dn_msg_host_xfer_qos;
   wire           dn_msg_host_beat_valid;
   wire [127:0]   dn_msg_host_beat_data;
   wire           dn_msg_host_beat_sop;
   wire           dn_msg_host_beat_eop;

   wire [127:0]   dn_msg_noc_beat_data;
   wire           dn_msg_noc_beat_valid;
   wire           dn_msg_noc_beat_sop;
   wire           dn_msg_noc_beat_eop;

   wire           dn_msg_host_credit_inc;
   wire           dn_msg_noc_credit_inc;

   wire [7:0]     dn_evt_host_dest_hp;
   wire [1:0]     dn_evt_host_dest_int;
   wire [3:0]     dn_evt_host_xfer_qos;
   wire           dn_evt_host_beat_valid;
   wire [31:0]    dn_evt_host_beat_data;
   wire           dn_evt_host_beat_sop;
   wire           dn_evt_host_beat_eop;

   wire [31:0]    dn_evt_noc_beat_data;
   wire           dn_evt_noc_beat_valid;
   wire           dn_evt_noc_beat_sop;
   wire           dn_evt_noc_beat_eop;

   wire           dn_evt_host_credit_inc;
   wire           dn_evt_noc_credit_inc;

   //-----------------------------------------------------------------------------------------------
   //
   // Add some unused signals.
   //
   // It is necessary to use wired-or signals in order to prevent the AXI assertions from misfiring
   // for undriven signals (tri0 does not seem to work when no debug mode is used).
   //
   wire  [L2_TO_L3MESH_LAYERS-1:0][`DV_TRANS_ID_SIZE-1:0] to_l3_mesh_master_axi_aw_trans_id;
   wire  [L2_TO_L3MESH_LAYERS-1:0][`DV_TRANS_ID_SIZE-1:0] to_l3_mesh_master_axi_w_trans_id;
   wire  [L2_TO_L3MESH_LAYERS-1:0][`DV_TRANS_ID_SIZE-1:0] to_l3_mesh_master_axi_ar_trans_id;

   wire  [`DV_TRANS_ID_SIZE-1:0]                          to_sys_mesh_master_aw_trans_id;
   wire  [`DV_TRANS_ID_SIZE-1:0]                          to_sys_mesh_master_w_trans_id;
   wire  [`DV_TRANS_ID_SIZE-1:0]                          to_sys_mesh_master_ar_trans_id;

   wire  [L3MESH_TO_L3_LAYERS-1:0][`DV_TRANS_ID_SIZE-1:0] l3_mesh_slave_axi_aw_trans_id;
   wire  [L3MESH_TO_L3_LAYERS-1:0][`DV_TRANS_ID_SIZE-1:0] l3_mesh_slave_axi_ar_trans_id;

   wire  [NUM_NEIGH-1:0][`DV_TRANS_ID_SIZE-1:0]           neigh_bus_trans_id;

   wire  unused_ok = &{ 1'b0,
                        esr_pwr_ctrl_neigh_iso,
                        esr_pwr_ctrl_glb_iso,
                        to_l3_mesh_master_axi_aw_trans_id,
                        to_l3_mesh_master_axi_w_trans_id,
                        to_l3_mesh_master_axi_ar_trans_id,
                        to_sys_mesh_master_aw_trans_id,
                        to_sys_mesh_master_w_trans_id,
                        to_sys_mesh_master_ar_trans_id,
                        l3_mesh_slave_axi_aw_trans_id,
                        l3_mesh_slave_axi_ar_trans_id,
                        neigh_bus_trans_id,
                        use_rsp_source_is_uc,
                        rsp_source_is_uc,
                        1'b0 };

   `ifdef ET_AXI_DPI_MONITOR
      wire [TOTAL_AXI_COUNT-1:0]                    axi_dpi_packet_pop;
      wire [TOTAL_AXI_COUNT-1:0]                    axi_dpi_packet_valid;
      wire [TOTAL_AXI_COUNT-1:0][`ET_DPI_PKT_MSB:0] axi_dpi_packet;
      wire                                          shire_dpi_packet_valid;
      wire [`ET_DPI_PKT_MSB:0]                      shire_dpi_packet_data;
      wire                                          shire_dpi_mem_empty;
      wire                                          shire_dpi_mem_full;


      generate
         if (CAPTURE_DPI_EVENTS == 1'b0) begin :g_dpi_capture
            assign dpi_packet_valid = 1'b0;
            assign dpi_packet_data  = {(`ET_DPI_PKT_MSB+1){1'b0}};
         end
         else begin :g_dpi_capture
            assign dpi_packet_valid = ~shire_dpi_mem_empty;
            dpi_event_select #(.EVENT_COUNT(TOTAL_AXI_COUNT)) dpi_event_select (
                  .reset     (reset_cold),
                  .clk       (dpi_read_clock),
                  .full_in   (shire_dpi_mem_full),
                  .valid_in  (axi_dpi_packet_valid),
                  .data_in   (axi_dpi_packet),
                  .pop_out   (axi_dpi_packet_pop),
                  .valid_out (shire_dpi_packet_valid),
                  .data_out  (shire_dpi_packet_data)
               );

            dpi_event_capture #(.DEPTH(16)) dpi_event_capture (
                  .reset     (reset_cold),
                  .wr_clk    (dpi_read_clock),
                  .rd_clk    (dpi_read_clock),
                  .enable    (dpi_capture_enable & ~reset_cold),
                  .valid_in  (shire_dpi_packet_valid),
                  .data_in   (shire_dpi_packet_data),
                  .pop_in    (dpi_packet_pop),
                  .data_out  (dpi_packet_data),
                  .empty_out (shire_dpi_mem_empty),
                  .full_out  (shire_dpi_mem_full)
               );
         end
      endgenerate
   `endif // ifdef ET_AXI_DPI_MONITOR


   genvar mesh_idx;

   `ifdef EVL_SIMULATION

      generate
         if ((SC_LITE == 1'b1) && (NOC_STUB == 1'b0)) begin
            assign sys_mesh_slave_axi_AWcredit = 2'b00;
         end

         if ((SC_STUB == 1'b0) && (SC_LITE == 1'b0)) begin
            for (mesh_idx = 0; mesh_idx < L2_TO_L3MESH_LAYERS; mesh_idx = mesh_idx + 1) begin
               assign to_l3_mesh_master_axi_aw_trans_id[mesh_idx] = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_l3_mesh_master.axi_port[mesh_idx].mesh_master_axi_port.axi_ar_fifo_trans_id;
               assign to_l3_mesh_master_axi_ar_trans_id[mesh_idx] = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_l3_mesh_master.axi_port[mesh_idx].mesh_master_axi_port.axi_aw_fifo_trans_id;
               assign to_l3_mesh_master_axi_w_trans_id[mesh_idx]  = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_l3_mesh_master.axi_port[mesh_idx].mesh_master_axi_port.axi_w_fifo_trans_id;
            end
            for (mesh_idx = 0; mesh_idx < L3MESH_TO_L3_LAYERS; mesh_idx = mesh_idx + 1) begin
               assign shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.l3_mesh_slave.axi_port[mesh_idx].mesh_slave_axi_port.aw_trans_id = l3_mesh_slave_axi_aw_trans_id[mesh_idx];
               assign shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.l3_mesh_slave.axi_port[mesh_idx].mesh_slave_axi_port.ar_trans_id = l3_mesh_slave_axi_ar_trans_id[mesh_idx];
            end
            assign to_sys_mesh_master_aw_trans_id = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_sys_mesh_master.axi_port[0].mesh_master_axi_port.axi_ar_fifo_trans_id;
            assign to_sys_mesh_master_ar_trans_id = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_sys_mesh_master.axi_port[0].mesh_master_axi_port.axi_aw_fifo_trans_id;
            assign to_sys_mesh_master_w_trans_id  = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.to_sys_mesh_master.axi_port[0].mesh_master_axi_port.axi_w_fifo_trans_id;
         end
      endgenerate


      //
      // Create neighborhood ETLink bus agent interfaces for each neighborhood.
      //
      genvar neighborhood_idx;
      generate
         for (neighborhood_idx = 0; neighborhood_idx < NUM_NEIGH; neighborhood_idx = neighborhood_idx + 1) begin : g_neigh
            if ((SC_STUB == 1'b0) && (SC_LITE == 1'b0) && (NEIGH_STUB[neighborhood_idx] == 1'b0) && (NEIGH_DISABLE[neighborhood_idx] == 1'b0)) begin
               assign neigh_bus_trans_id[neighborhood_idx] = shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.trans_id[neighborhood_idx];
            end
            else if ((SC_STUB == 1'b0) && (SC_LITE == 1'b0) && (NEIGH_STUB[neighborhood_idx] == 1'b1)) begin
               assign shire_channel_wrap_inst.shire_channel_gen.channel_inst.g_sc.shire_cache.trans_id[neighborhood_idx] = neigh_bus_trans_id[neighborhood_idx];
            end
            else begin
               assign neigh_bus_trans_id[neighborhood_idx] = {(`DV_TRANS_ID_SIZE){1'b0}};
            end
            if ((SC_LITE == 1'b0) && (SC_STUB == 1'b0) && (NEIGH_STUB[neighborhood_idx] == 1'b1) && (NEIGH_DISABLE[neighborhood_idx] == 1'b0)) begin : m_neigh_apb
               evl_apb_interface #(.AGENT_ID   (neighborhood_idx),
                                   .ADDR_WIDTH (`NEIGH_DM_APB_ADDR_WIDTH),
                                   .DATA_WIDTH (`ESR_APB_D_WIDTH)) m_apb_esr (
                     .isolate ( 1'b0                                            ),
                     .pclk    ( shire_clock                                     ),
                     .presetn ( ~reset_cold                                     ),
                     .penable ( apb_esr_req_neigh[neighborhood_idx].apb_penable ),
                     .psel    ( apb_esr_req_neigh[neighborhood_idx].apb_psel    ),
                     .pwrite  ( apb_esr_req_neigh[neighborhood_idx].apb_pwrite  ),
                     .paddr   ( apb_esr_req_neigh[neighborhood_idx].apb_paddr   ),
                     .pwdata  ( apb_esr_req_neigh[neighborhood_idx].apb_pwdata  ),
                     .pready  ( apb_esr_rsp_neigh[neighborhood_idx].apb_pready  ),
                     .prdata  ( apb_esr_rsp_neigh[neighborhood_idx].apb_prdata  ),
                     .pslverr ( apb_esr_rsp_neigh[neighborhood_idx].apb_pslverr )
                  );
            end
            else begin : m_neigh_apb
               wire unused_ok = &{ 1'b0, apb_esr_req_neigh[neighborhood_idx], apb_esr_rsp_neigh[neighborhood_idx] };
            end
            if ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || ((NEIGH_STUB[neighborhood_idx] == 1'b0) && (NEIGH_DISABLE[neighborhood_idx] == 1'b0)))) begin : m_neighborhood
               evl_etlink_interface #(.AGENT_ID        (neighborhood_idx),
                                      .NUM_BANKS       (NUM_BANKS),
                                      .NUM_UC          (NUM_UC),
                                      .ETLINK_COV_TYPE (int'(EVL_COV_ETLINK_NEIGH_SC))) m_neighborhood (
                     .isolate              ( 1'b0                                  ),
                     .reset                ( reset_cold                            ),
                     .clk                  ( shire_clock                           ),
                     .req_trans_id         ( neigh_bus_trans_id [neighborhood_idx] ),
                     .req_valid            ( neigh_sc_req_valid [neighborhood_idx] ),
                     .req_ready            ( neigh_sc_req_ready [neighborhood_idx] ),
                     .req_info             ( neigh_sc_req_info  [neighborhood_idx] ),
                     .rsp_valid            ( neigh_sc_rsp_valid [neighborhood_idx] ),
                     .rsp_ready            ( neigh_sc_rsp_ready [neighborhood_idx] ),
                     .rsp_info             ( neigh_sc_rsp_info  [neighborhood_idx] ),
                     .rsp_source_is_uc     ( rsp_source_is_uc   [neighborhood_idx] ),
                     .use_rsp_source_is_uc ( use_rsp_source_is_uc                  )
                  );

            end
         end
      endgenerate

      generate
         if (DBGNOC_STIM == 1'b1) begin
            assign shire_channel_wrap_inst.ust_out_nsip_msg_dest_hp     = dn_msg_host_dest_hp;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_dest_int    = dn_msg_host_dest_int;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_xfer_qos    = dn_msg_host_xfer_qos;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_beat_valid  = dn_msg_host_beat_valid;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_beat_data   = dn_msg_host_beat_data;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_beat_sop    = dn_msg_host_beat_sop;
            assign shire_channel_wrap_inst.ust_out_nsip_msg_beat_eop    = dn_msg_host_beat_eop;

            assign dn_msg_noc_beat_data      = shire_channel_wrap_inst.ust_in_nsip_msg_beat_data;
            assign dn_msg_noc_beat_valid     = shire_channel_wrap_inst.ust_in_nsip_msg_beat_valid;
            assign dn_msg_noc_beat_sop       = shire_channel_wrap_inst.ust_in_nsip_msg_beat_sop;
            assign dn_msg_noc_beat_eop       = shire_channel_wrap_inst.ust_in_nsip_msg_beat_eop;

            assign shire_channel_wrap_inst.ust_in_nsip_msg_credit_inc   = dn_msg_host_credit_inc;
            assign dn_msg_noc_credit_inc     = shire_channel_wrap_inst.ust_out_nsip_msg_credit_inc;

            assign shire_channel_wrap_inst.ust_out_nsip_evt_dest_hp     = dn_evt_host_dest_hp;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_dest_int    = dn_evt_host_dest_int;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_xfer_qos    = dn_evt_host_xfer_qos;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_beat_valid  = dn_evt_host_beat_valid;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_beat_data   = dn_evt_host_beat_data;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_beat_sop    = dn_evt_host_beat_sop;
            assign shire_channel_wrap_inst.ust_out_nsip_evt_beat_eop    = dn_evt_host_beat_eop;

            assign dn_evt_noc_beat_data      = shire_channel_wrap_inst.ust_in_nsip_evt_beat_data;
            assign dn_evt_noc_beat_valid     = shire_channel_wrap_inst.ust_in_nsip_evt_beat_valid;
            assign dn_evt_noc_beat_sop       = shire_channel_wrap_inst.ust_in_nsip_evt_beat_sop;
            assign dn_evt_noc_beat_eop       = shire_channel_wrap_inst.ust_in_nsip_evt_beat_eop;

            assign shire_channel_wrap_inst.ust_in_nsip_evt_credit_inc   = dn_evt_host_credit_inc;
            assign dn_evt_noc_credit_inc     = shire_channel_wrap_inst.ust_out_nsip_evt_credit_inc;
         end
         else begin
            assign dn_msg_host_dest_hp       = shire_channel_wrap_inst.ust_out_nsip_msg_dest_hp;
            assign dn_msg_host_dest_int      = shire_channel_wrap_inst.ust_out_nsip_msg_dest_int;
            assign dn_msg_host_xfer_qos      = shire_channel_wrap_inst.ust_out_nsip_msg_xfer_qos;
            assign dn_msg_host_beat_valid    = shire_channel_wrap_inst.ust_out_nsip_msg_beat_valid;
            assign dn_msg_host_beat_data     = shire_channel_wrap_inst.ust_out_nsip_msg_beat_data;
            assign dn_msg_host_beat_sop      = shire_channel_wrap_inst.ust_out_nsip_msg_beat_sop;
            assign dn_msg_host_beat_eop      = shire_channel_wrap_inst.ust_out_nsip_msg_beat_eop;

            assign dn_msg_noc_beat_data      = shire_channel_wrap_inst.ust_in_nsip_msg_beat_data;
            assign dn_msg_noc_beat_valid     = shire_channel_wrap_inst.ust_in_nsip_msg_beat_valid;
            assign dn_msg_noc_beat_sop       = shire_channel_wrap_inst.ust_in_nsip_msg_beat_sop;
            assign dn_msg_noc_beat_eop       = shire_channel_wrap_inst.ust_in_nsip_msg_beat_eop;

            assign dn_msg_host_credit_inc    = shire_channel_wrap_inst.ust_in_nsip_msg_credit_inc;
            assign dn_msg_noc_credit_inc     = shire_channel_wrap_inst.ust_out_nsip_msg_credit_inc;

            assign dn_evt_host_dest_hp       = shire_channel_wrap_inst.ust_out_nsip_evt_dest_hp;
            assign dn_evt_host_dest_int      = shire_channel_wrap_inst.ust_out_nsip_evt_dest_int;
            assign dn_evt_host_xfer_qos      = shire_channel_wrap_inst.ust_out_nsip_evt_xfer_qos;
            assign dn_evt_host_beat_valid    = shire_channel_wrap_inst.ust_out_nsip_evt_beat_valid;
            assign dn_evt_host_beat_data     = shire_channel_wrap_inst.ust_out_nsip_evt_beat_data;
            assign dn_evt_host_beat_sop      = shire_channel_wrap_inst.ust_out_nsip_evt_beat_sop;
            assign dn_evt_host_beat_eop      = shire_channel_wrap_inst.ust_out_nsip_evt_beat_eop;

            assign dn_evt_noc_beat_data      = shire_channel_wrap_inst.ust_in_nsip_evt_beat_data;
            assign dn_evt_noc_beat_valid     = shire_channel_wrap_inst.ust_in_nsip_evt_beat_valid;
            assign dn_evt_noc_beat_sop       = shire_channel_wrap_inst.ust_in_nsip_evt_beat_sop;
            assign dn_evt_noc_beat_eop       = shire_channel_wrap_inst.ust_in_nsip_evt_beat_eop;

            assign dn_evt_host_credit_inc    = shire_channel_wrap_inst.ust_in_nsip_evt_credit_inc;
            assign dn_evt_noc_credit_inc     = shire_channel_wrap_inst.ust_out_nsip_evt_credit_inc;
         end
      endgenerate


      //
      // Debug NoC DN_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (SHIRE_INST_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_dn_msg (
            .reset            (reset_cold),
            .clk              (shire_clock),

            .tx_req_addr      (dn_msg_host_dest_hp),
            .tx_req_dest_int  (dn_msg_host_dest_int),
            .tx_req_qos       (dn_msg_host_xfer_qos),
            .tx_req_valid     (dn_msg_host_beat_valid),
            .tx_req_data      (dn_msg_host_beat_data),
            .tx_req_sop       (dn_msg_host_beat_sop),
            .tx_req_eop       (dn_msg_host_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (dn_msg_noc_beat_data),
            .rx_req_valid     (dn_msg_noc_beat_valid),
            .rx_req_sop       (dn_msg_noc_beat_sop),
            .rx_req_eop       (dn_msg_noc_beat_eop),

            .host_credit_inc  (dn_msg_host_credit_inc),
            .noc_credit_inc   (dn_msg_noc_credit_inc)
         );

      //
      // Debug NoC DN_EVT IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (SHIRE_INST_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (32),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_EVT_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_dn_evt (
            .reset            (reset_cold),
            .clk              (shire_clock),

            .tx_req_addr      (dn_evt_host_dest_hp),
            .tx_req_dest_int  (dn_evt_host_dest_int),
            .tx_req_qos       (dn_evt_host_xfer_qos),
            .tx_req_valid     (dn_evt_host_beat_valid),
            .tx_req_data      (dn_evt_host_beat_data),
            .tx_req_sop       (dn_evt_host_beat_sop),
            .tx_req_eop       (dn_evt_host_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (dn_evt_noc_beat_data),
            .rx_req_valid     (dn_evt_noc_beat_valid),
            .rx_req_sop       (dn_evt_noc_beat_sop),
            .rx_req_eop       (dn_evt_noc_beat_eop),

            .host_credit_inc  (dn_evt_host_credit_inc),
            .noc_credit_inc   (dn_evt_noc_credit_inc)
         );

   `endif // ifdef EVL_SIMULATION

   generate
      for (mesh_idx = 0; mesh_idx < MESH_TO_SBM_LAYERS; mesh_idx = mesh_idx + 1) begin : g_mesh_to_sbm_if
         //
         // Mesh-to-SBM Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (MESH_TO_SBM_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_FROM_SYSMESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_SYS_ESR_32),
                             .AXI_COV_NAME           ("AXI_SYS_ESR_32"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (1),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ALLOW_DUPLICATE_OUTSTANDING_IDS (1),
                             .ID_WIDTH               (`SC_MESH_SLAVE_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SYS_MESH_SLAVE_AXI_DATA_SIZE),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                             .LOCK_W_RDY             (0),
                             .AW_VC_CREDITS          (8),
                             .AW_VC_WIDTH            (2)) m_sys_mesh_slave_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[MESH_TO_SBM_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[MESH_TO_SBM_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[MESH_TO_SBM_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (sys_mesh_slave_axi_AW.ID),
               .aw_addr              (sys_mesh_slave_axi_AW.ADDR),
               .aw_len               (sys_mesh_slave_axi_AW.LEN),
               .aw_size              (sys_mesh_slave_axi_AW.SIZE),
               .aw_burst             (sys_mesh_slave_axi_AW.BURST),
               .aw_lock              (sys_mesh_slave_axi_AW.LOCK),
               .aw_cache             (sys_mesh_slave_axi_AW.CACHE),
               .aw_prot              (sys_mesh_slave_axi_AW.PROT),
               .aw_qos               (sys_mesh_slave_axi_AW.QOS),
               .aw_region            (),
               .aw_user              (sys_mesh_slave_axi_AW.USER),
               .aw_valid             (sys_mesh_slave_axi_AWVALID),
               .aw_ready             (sys_mesh_slave_axi_AWREADY),
               .aw_vc_valid          (sys_mesh_slave_axi_AWvcvalid),
               .aw_vc_credit         (sys_mesh_slave_axi_AWcredit),
               .aw_trans_id          (),

               .w_user               (),
               .w_data               (sys_mesh_slave_axi_W.DATA),
               .w_strb               (sys_mesh_slave_axi_W.STRB),
               .w_last               (sys_mesh_slave_axi_W.LAST),
               .w_valid              (sys_mesh_slave_axi_WVALID),
               .w_ready              (sys_mesh_slave_axi_WREADY),
               .w_vc_valid           (sys_mesh_slave_axi_Wvcvalid),
               .w_trans_id           (),

               .b_user               (),
               .b_id                 (sys_mesh_slave_axi_B.ID),
               .b_resp               (sys_mesh_slave_axi_B.RESP),
               .b_valid              (sys_mesh_slave_axi_BVALID),
               .b_ready              (sys_mesh_slave_axi_BREADY),
               .b_trans_id           (),

               .ar_id                (sys_mesh_slave_axi_AR.ID),
               .ar_addr              (sys_mesh_slave_axi_AR.ADDR),
               .ar_len               (sys_mesh_slave_axi_AR.LEN),
               .ar_size              (sys_mesh_slave_axi_AR.SIZE),
               .ar_burst             (sys_mesh_slave_axi_AR.BURST),
               .ar_lock              (sys_mesh_slave_axi_AR.LOCK),
               .ar_cache             (sys_mesh_slave_axi_AR.CACHE),
               .ar_prot              (sys_mesh_slave_axi_AR.PROT),
               .ar_qos               (sys_mesh_slave_axi_AR.QOS),
               .ar_region            (),
               .ar_user              (sys_mesh_slave_axi_AR.USER),
               .ar_valid             (sys_mesh_slave_axi_ARVALID),
               .ar_ready             (sys_mesh_slave_axi_ARREADY),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (),

               .r_last               (sys_mesh_slave_axi_R.LAST),
               .r_user               (),
               .r_id                 (sys_mesh_slave_axi_R.ID),
               .r_data               (sys_mesh_slave_axi_R.DATA),
               .r_resp               (sys_mesh_slave_axi_R.RESP),
               .r_valid              (sys_mesh_slave_axi_RVALID),
               .r_ready              (sys_mesh_slave_axi_RREADY),
               .r_trans_id           ()
            );
      end
   endgenerate


   generate
      if (L2_TO_L3MESH_LAYERS > 0) begin :g_l2_to_l3mesh_assign
         assign to_l3_mesh_master_axi_aw_trans_id = {(`DV_TRANS_ID_SIZE*L2_TO_L3MESH_LAYERS){1'b0}};
         assign to_l3_mesh_master_axi_w_trans_id  = {(`DV_TRANS_ID_SIZE*L2_TO_L3MESH_LAYERS){1'b0}};
         assign to_l3_mesh_master_axi_ar_trans_id = {(`DV_TRANS_ID_SIZE*L2_TO_L3MESH_LAYERS){1'b0}};
      end

      for (mesh_idx = 0; mesh_idx < L2_TO_L3MESH_LAYERS; mesh_idx = mesh_idx + 1) begin : g_l2_to_l3mesh_if
         //
         // L2 to L3 Mesh Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (L2_TO_L3MESH_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_TO_L3MESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_SC_64),
                             .AXI_COV_NAME           ("AXI_SC_64"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (L2_TO_L3MESH_NO_DRIVE),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ID_WIDTH               (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .LOCK_W_RDY             (0),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_to_l3_mesh_master_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[L2_TO_L3MESH_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[L2_TO_L3MESH_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[L2_TO_L3MESH_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (to_l3_mesh_master_axi_AW[mesh_idx].ID),
               .aw_addr              (to_l3_mesh_master_axi_AW[mesh_idx].ADDR),
               .aw_len               (to_l3_mesh_master_axi_AW[mesh_idx].LEN),
               .aw_size              (to_l3_mesh_master_axi_AW[mesh_idx].SIZE),
               .aw_burst             (to_l3_mesh_master_axi_AW[mesh_idx].BURST),
               .aw_lock              (to_l3_mesh_master_axi_AW[mesh_idx].LOCK),
               .aw_cache             (to_l3_mesh_master_axi_AW[mesh_idx].CACHE),
               .aw_prot              (to_l3_mesh_master_axi_AW[mesh_idx].PROT),
               .aw_qos               (to_l3_mesh_master_axi_AW[mesh_idx].QOS),
               .aw_region            (),
               .aw_user              (to_l3_mesh_master_axi_AW[mesh_idx].USER),
               .aw_valid             (to_l3_mesh_master_axi_AWVALID[mesh_idx]),
               .aw_ready             (to_l3_mesh_master_axi_AWREADY[mesh_idx]),
               .aw_vc_valid          (),
               .aw_vc_credit         (),
               .aw_trans_id          (to_l3_mesh_master_axi_aw_trans_id[mesh_idx]),

               .w_user               (),
               .w_data               (to_l3_mesh_master_axi_W[mesh_idx].DATA),
               .w_strb               (to_l3_mesh_master_axi_W[mesh_idx].STRB),
               .w_last               (to_l3_mesh_master_axi_W[mesh_idx].LAST),
               .w_valid              (to_l3_mesh_master_axi_WVALID[mesh_idx]),
               .w_ready              (to_l3_mesh_master_axi_WREADY[mesh_idx]),
               .w_vc_valid           (),
               .w_trans_id           (to_l3_mesh_master_axi_w_trans_id[mesh_idx]),

               .b_user               (),
               .b_id                 (to_l3_mesh_master_axi_B[mesh_idx].ID),
               .b_resp               (to_l3_mesh_master_axi_B[mesh_idx].RESP),
               .b_valid              (to_l3_mesh_master_axi_BVALID[mesh_idx]),
               .b_ready              (to_l3_mesh_master_axi_BREADY[mesh_idx]),
               .b_trans_id           (),

               .ar_id                (to_l3_mesh_master_axi_AR[mesh_idx].ID),
               .ar_addr              (to_l3_mesh_master_axi_AR[mesh_idx].ADDR),
               .ar_len               (to_l3_mesh_master_axi_AR[mesh_idx].LEN),
               .ar_size              (to_l3_mesh_master_axi_AR[mesh_idx].SIZE),
               .ar_burst             (to_l3_mesh_master_axi_AR[mesh_idx].BURST),
               .ar_lock              (to_l3_mesh_master_axi_AR[mesh_idx].LOCK),
               .ar_cache             (to_l3_mesh_master_axi_AR[mesh_idx].CACHE),
               .ar_prot              (to_l3_mesh_master_axi_AR[mesh_idx].PROT),
               .ar_qos               (to_l3_mesh_master_axi_AR[mesh_idx].QOS),
               .ar_region            (),
               .ar_user              (to_l3_mesh_master_axi_AR[mesh_idx].USER),
               .ar_valid             (to_l3_mesh_master_axi_ARVALID[mesh_idx]),
               .ar_ready             (to_l3_mesh_master_axi_ARREADY[mesh_idx]),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (to_l3_mesh_master_axi_ar_trans_id[mesh_idx]),

               .r_last               (to_l3_mesh_master_axi_R[mesh_idx].LAST),
               .r_user               (),
               .r_id                 (to_l3_mesh_master_axi_R[mesh_idx].ID),
               .r_data               (to_l3_mesh_master_axi_R[mesh_idx].DATA),
               .r_resp               (to_l3_mesh_master_axi_R[mesh_idx].RESP),
               .r_valid              (to_l3_mesh_master_axi_RVALID[mesh_idx]),
               .r_ready              (to_l3_mesh_master_axi_RREADY[mesh_idx]),
               .r_trans_id           ()
            );
      end
   endgenerate


   generate
      for (mesh_idx = 0; mesh_idx < L3_TO_SYSMESH_LAYERS; mesh_idx = mesh_idx + 1) begin : g_l3_to_sysmesh_if
         //
         // L3 to Sys Mesh Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (L3_TO_SYSMESH_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_TO_SYSMESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_SYS_MEM_64),
                             .AXI_COV_NAME           ("AXI_SYS_MEM_64"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (L3_TO_SYSMESH_NO_DRIVE),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ID_WIDTH               (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .LOCK_W_RDY             (0),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_to_sys_mesh_master_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[L3_TO_SYSMESH_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[L3_TO_SYSMESH_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[L3_TO_SYSMESH_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (to_sys_mesh_master_axi_AW.ID),
               .aw_addr              (to_sys_mesh_master_axi_AW.ADDR),
               .aw_len               (to_sys_mesh_master_axi_AW.LEN),
               .aw_size              (to_sys_mesh_master_axi_AW.SIZE),
               .aw_burst             (to_sys_mesh_master_axi_AW.BURST),
               .aw_lock              (to_sys_mesh_master_axi_AW.LOCK),
               .aw_cache             (to_sys_mesh_master_axi_AW.CACHE),
               .aw_prot              (to_sys_mesh_master_axi_AW.PROT),
               .aw_qos               (to_sys_mesh_master_axi_AW.QOS),
               .aw_region            (),
               .aw_user              (to_sys_mesh_master_axi_AW.USER),
               .aw_valid             (to_sys_mesh_master_axi_AWVALID),
               .aw_ready             (to_sys_mesh_master_axi_AWREADY),
               .aw_vc_valid          (),
               .aw_vc_credit         (),
               .aw_trans_id          (to_sys_mesh_master_aw_trans_id),

               .w_user               (),
               .w_data               (to_sys_mesh_master_axi_W.DATA),
               .w_strb               (to_sys_mesh_master_axi_W.STRB),
               .w_last               (to_sys_mesh_master_axi_W.LAST),
               .w_valid              (to_sys_mesh_master_axi_WVALID),
               .w_ready              (to_sys_mesh_master_axi_WREADY),
               .w_vc_valid           (),
               .w_trans_id           (to_sys_mesh_master_w_trans_id),

               .b_user               (),
               .b_id                 (to_sys_mesh_master_axi_B.ID),
               .b_resp               (to_sys_mesh_master_axi_B.RESP),
               .b_valid              (to_sys_mesh_master_axi_BVALID),
               .b_ready              (to_sys_mesh_master_axi_BREADY),
               .b_trans_id           (),

               .ar_id                (to_sys_mesh_master_axi_AR.ID),
               .ar_addr              (to_sys_mesh_master_axi_AR.ADDR),
               .ar_len               (to_sys_mesh_master_axi_AR.LEN),
               .ar_size              (to_sys_mesh_master_axi_AR.SIZE),
               .ar_burst             (to_sys_mesh_master_axi_AR.BURST),
               .ar_lock              (to_sys_mesh_master_axi_AR.LOCK),
               .ar_cache             (to_sys_mesh_master_axi_AR.CACHE),
               .ar_prot              (to_sys_mesh_master_axi_AR.PROT),
               .ar_qos               (to_sys_mesh_master_axi_AR.QOS),
               .ar_region            (),
               .ar_user              (to_sys_mesh_master_axi_AR.USER),
               .ar_valid             (to_sys_mesh_master_axi_ARVALID),
               .ar_ready             (to_sys_mesh_master_axi_ARREADY),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (to_sys_mesh_master_ar_trans_id),

               .r_last               (to_sys_mesh_master_axi_R.LAST),
               .r_user               (),
               .r_id                 (to_sys_mesh_master_axi_R.ID),
               .r_data               (to_sys_mesh_master_axi_R.DATA),
               .r_resp               (to_sys_mesh_master_axi_R.RESP),
               .r_valid              (to_sys_mesh_master_axi_RVALID),
               .r_ready              (to_sys_mesh_master_axi_RREADY),
               .r_trans_id           ()
            );
      end
   endgenerate


   generate
      for (mesh_idx = 0; mesh_idx < L3MESH_TO_L3_LAYERS; mesh_idx = mesh_idx + 1) begin : g_l3mesh_to_l3_if
         //
         // L3 Mesh to L3 Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (L3MESH_TO_L3_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_FROM_L3MESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_SC_64),
                             .AXI_COV_NAME           ("AXI_SC_64"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (1),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ID_WIDTH               (`SC_MESH_SLAVE_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .LOCK_W_RDY             (0),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_l3_mesh_slave_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[L3MESH_TO_L3_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[L3MESH_TO_L3_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[L3MESH_TO_L3_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (l3_mesh_slave_axi_AW[mesh_idx].ID),
               .aw_addr              (l3_mesh_slave_axi_AW[mesh_idx].ADDR),
               .aw_len               (l3_mesh_slave_axi_AW[mesh_idx].LEN),
               .aw_size              (l3_mesh_slave_axi_AW[mesh_idx].SIZE),
               .aw_burst             (l3_mesh_slave_axi_AW[mesh_idx].BURST),
               .aw_lock              (l3_mesh_slave_axi_AW[mesh_idx].LOCK),
               .aw_cache             (l3_mesh_slave_axi_AW[mesh_idx].CACHE),
               .aw_prot              (l3_mesh_slave_axi_AW[mesh_idx].PROT),
               .aw_qos               (l3_mesh_slave_axi_AW[mesh_idx].QOS),
               .aw_region            (),
               .aw_user              (l3_mesh_slave_axi_AW[mesh_idx].USER),
               .aw_valid             (l3_mesh_slave_axi_AWVALID[mesh_idx]),
               .aw_ready             (l3_mesh_slave_axi_AWREADY[mesh_idx]),
               .aw_vc_valid          (),
               .aw_vc_credit         (),
               .aw_trans_id          (l3_mesh_slave_axi_aw_trans_id[mesh_idx]),

               .w_user               (),
               .w_data               (l3_mesh_slave_axi_W[mesh_idx].DATA),
               .w_strb               (l3_mesh_slave_axi_W[mesh_idx].STRB),
               .w_last               (l3_mesh_slave_axi_W[mesh_idx].LAST),
               .w_valid              (l3_mesh_slave_axi_WVALID[mesh_idx]),
               .w_ready              (l3_mesh_slave_axi_WREADY[mesh_idx]),
               .w_vc_valid           (),
               .w_trans_id           (),

               .b_user               (),
               .b_id                 (l3_mesh_slave_axi_B[mesh_idx].ID),
               .b_resp               (l3_mesh_slave_axi_B[mesh_idx].RESP),
               .b_valid              (l3_mesh_slave_axi_BVALID[mesh_idx]),
               .b_ready              (l3_mesh_slave_axi_BREADY[mesh_idx]),
               .b_trans_id           (),

               .ar_id                (l3_mesh_slave_axi_AR[mesh_idx].ID),
               .ar_addr              (l3_mesh_slave_axi_AR[mesh_idx].ADDR),
               .ar_len               (l3_mesh_slave_axi_AR[mesh_idx].LEN),
               .ar_size              (l3_mesh_slave_axi_AR[mesh_idx].SIZE),
               .ar_burst             (l3_mesh_slave_axi_AR[mesh_idx].BURST),
               .ar_lock              (l3_mesh_slave_axi_AR[mesh_idx].LOCK),
               .ar_cache             (l3_mesh_slave_axi_AR[mesh_idx].CACHE),
               .ar_prot              (l3_mesh_slave_axi_AR[mesh_idx].PROT),
               .ar_qos               (l3_mesh_slave_axi_AR[mesh_idx].QOS),
               .ar_region            (),
               .ar_user              (l3_mesh_slave_axi_AR[mesh_idx].USER),
               .ar_valid             (l3_mesh_slave_axi_ARVALID[mesh_idx]),
               .ar_ready             (l3_mesh_slave_axi_ARREADY[mesh_idx]),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (),

               .r_last               (l3_mesh_slave_axi_R[mesh_idx].LAST),
               .r_user               (),
               .r_id                 (l3_mesh_slave_axi_R[mesh_idx].ID),
               .r_data               (l3_mesh_slave_axi_R[mesh_idx].DATA),
               .r_resp               (l3_mesh_slave_axi_R[mesh_idx].RESP),
               .r_valid              (l3_mesh_slave_axi_RVALID[mesh_idx]),
               .r_ready              (l3_mesh_slave_axi_RREADY[mesh_idx]),
               .r_trans_id           ()
            );
      end
   endgenerate


   generate
      for (mesh_idx = 0; mesh_idx < UC_TO_L3MESH_LAYERS; mesh_idx = mesh_idx + 1) begin : g_uc_to_l3mesh_if
         //
         // UC to L3 Mesh Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (UC_TO_L3MESH_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_UC_TO_L3MESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_UC_64),
                             .AXI_COV_NAME           ("AXI_UC_64"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (1),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ID_WIDTH               (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .LOCK_W_RDY             (1),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_uc_to_l3_mesh_master_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[UC_TO_L3MESH_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[UC_TO_L3MESH_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[UC_TO_L3MESH_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (uc_to_l3_mesh_master_axi_AW.ID),
               .aw_addr              (uc_to_l3_mesh_master_axi_AW.ADDR),
               .aw_len               (uc_to_l3_mesh_master_axi_AW.LEN),
               .aw_size              (uc_to_l3_mesh_master_axi_AW.SIZE),
               .aw_burst             (uc_to_l3_mesh_master_axi_AW.BURST),
               .aw_lock              (uc_to_l3_mesh_master_axi_AW.LOCK),
               .aw_cache             (uc_to_l3_mesh_master_axi_AW.CACHE),
               .aw_prot              (uc_to_l3_mesh_master_axi_AW.PROT),
               .aw_qos               (uc_to_l3_mesh_master_axi_AW.QOS),
               .aw_region            (),
               .aw_user              (uc_to_l3_mesh_master_axi_AW.USER),
               .aw_valid             (uc_to_l3_mesh_master_axi_AWVALID),
               .aw_ready             (uc_to_l3_mesh_master_axi_AWREADY),
               .aw_vc_valid          (),
               .aw_vc_credit         (),
               .aw_trans_id          (),

               .w_user               (),
               .w_data               (uc_to_l3_mesh_master_axi_W.DATA),
               .w_strb               (uc_to_l3_mesh_master_axi_W.STRB),
               .w_last               (uc_to_l3_mesh_master_axi_W.LAST),
               .w_valid              (uc_to_l3_mesh_master_axi_WVALID),
               .w_ready              (uc_to_l3_mesh_master_axi_WREADY),
               .w_vc_valid           (),
               .w_trans_id           (),

               .b_user               (),
               .b_id                 (uc_to_l3_mesh_master_axi_B.ID),
               .b_resp               (uc_to_l3_mesh_master_axi_B.RESP),
               .b_valid              (uc_to_l3_mesh_master_axi_BVALID),
               .b_ready              (uc_to_l3_mesh_master_axi_BREADY),
               .b_trans_id           (),

               .ar_id                (uc_to_l3_mesh_master_axi_AR.ID),
               .ar_addr              (uc_to_l3_mesh_master_axi_AR.ADDR),
               .ar_len               (uc_to_l3_mesh_master_axi_AR.LEN),
               .ar_size              (uc_to_l3_mesh_master_axi_AR.SIZE),
               .ar_burst             (uc_to_l3_mesh_master_axi_AR.BURST),
               .ar_lock              (uc_to_l3_mesh_master_axi_AR.LOCK),
               .ar_cache             (uc_to_l3_mesh_master_axi_AR.CACHE),
               .ar_prot              (uc_to_l3_mesh_master_axi_AR.PROT),
               .ar_qos               (uc_to_l3_mesh_master_axi_AR.QOS),
               .ar_region            (),
               .ar_user              (uc_to_l3_mesh_master_axi_AR.USER),
               .ar_valid             (uc_to_l3_mesh_master_axi_ARVALID),
               .ar_ready             (uc_to_l3_mesh_master_axi_ARREADY),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (),

               .r_last               (uc_to_l3_mesh_master_axi_R.LAST),
               .r_user               (),
               .r_id                 (uc_to_l3_mesh_master_axi_R.ID),
               .r_data               (uc_to_l3_mesh_master_axi_R.DATA),
               .r_resp               (uc_to_l3_mesh_master_axi_R.RESP),
               .r_valid              (uc_to_l3_mesh_master_axi_RVALID),
               .r_ready              (uc_to_l3_mesh_master_axi_RREADY),
               .r_trans_id           ()
            );
      end
   endgenerate


   generate
      for (mesh_idx = 0; mesh_idx < UC_TO_SYSMESH_LAYERS; mesh_idx = mesh_idx + 1) begin : g_uc_to_sysmesh_if
         //
         // UC to Sys Mesh Interface
         //
         evl_axi_interface #(.SHIRE_ID               (SHIRE_INST_ID),
                             .PORT_ID                (mesh_idx),
                             .MAX_PORT_ID            (UC_TO_SYSMESH_LAYERS),
                             .PORT_TYPE              (int'(evl_base_pkg::AXI_UC_TO_SYSMESH)),
                             .AXI_COV_TYPE           (evl_base_pkg::EVL_COV_AXI_UC_64),
                             .AXI_COV_NAME           ("AXI_UC_64"),
                             .DPI_MEM_DEPTH          (256),
                             .CAPTURE_DPI_EVENTS     (CAPTURE_DPI_EVENTS),
                             .IGNORE_REGION          (1),
                             .IGNORE_NON_REQ_USER    (1),
                             .NO_TRANS_ID_REQ_DRIVER (1),
                             .NO_TRANS_ID_RSP_DRIVER (1),
                             .ID_WIDTH               (`SC_MESH_MASTER_AXI_ID_SIZE),
                             .ADDR_WIDTH             (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                             .DATA_WIDTH             (`SC_MESH_MASTER_AXI_DATA_SIZE),
                             .LOCK_W_RDY             (1),
                             .AXSIZE_WIDTH           (3),
                             .ARUSER_WIDTH           (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                             .AWUSER_WIDTH           (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_uc_to_sys_mesh_master_axi (
               .isolate              (1'b0),
               .reset                (reset_cold),
               .clk                  (noc_clock),

                `ifdef ET_AXI_DPI_MONITOR
                   .dpi_capture_enable (dpi_capture_enable),
                   .dpi_read_clock     (dpi_read_clock),
                   .dpi_packet_pop     (axi_dpi_packet_pop[UC_TO_SYSMESH_DPI_BASE + mesh_idx]),
                   .dpi_packet_valid   (axi_dpi_packet_valid[UC_TO_SYSMESH_DPI_BASE + mesh_idx]),
                   .raw_dpi_packet     (axi_dpi_packet[UC_TO_SYSMESH_DPI_BASE + mesh_idx]),
                `endif // ifdef ET_AXI_DPI_MONITOR

               .aw_id                (uc_to_sys_mesh_master_axi_AW.ID),
               .aw_addr              (uc_to_sys_mesh_master_axi_AW.ADDR),
               .aw_len               (uc_to_sys_mesh_master_axi_AW.LEN),
               .aw_size              (uc_to_sys_mesh_master_axi_AW.SIZE),
               .aw_burst             (uc_to_sys_mesh_master_axi_AW.BURST),
               .aw_lock              (uc_to_sys_mesh_master_axi_AW.LOCK),
               .aw_cache             (uc_to_sys_mesh_master_axi_AW.CACHE),
               .aw_prot              (uc_to_sys_mesh_master_axi_AW.PROT),
               .aw_qos               (uc_to_sys_mesh_master_axi_AW.QOS),
               .aw_region            (),
               .aw_user              (uc_to_sys_mesh_master_axi_AW.USER),
               .aw_valid             (uc_to_sys_mesh_master_axi_AWVALID),
               .aw_ready             (uc_to_sys_mesh_master_axi_AWREADY),
               .aw_vc_valid          (),
               .aw_vc_credit         (),
               .aw_trans_id          (),

               .w_user               (),
               .w_data               (uc_to_sys_mesh_master_axi_W.DATA),
               .w_strb               (uc_to_sys_mesh_master_axi_W.STRB),
               .w_last               (uc_to_sys_mesh_master_axi_W.LAST),
               .w_valid              (uc_to_sys_mesh_master_axi_WVALID),
               .w_ready              (uc_to_sys_mesh_master_axi_WREADY),
               .w_vc_valid           (),
               .w_trans_id           (),

               .b_user               (),
               .b_id                 (uc_to_sys_mesh_master_axi_B.ID),
               .b_resp               (uc_to_sys_mesh_master_axi_B.RESP),
               .b_valid              (uc_to_sys_mesh_master_axi_BVALID),
               .b_ready              (uc_to_sys_mesh_master_axi_BREADY),
               .b_trans_id           (),

               .ar_id                (uc_to_sys_mesh_master_axi_AR.ID),
               .ar_addr              (uc_to_sys_mesh_master_axi_AR.ADDR),
               .ar_len               (uc_to_sys_mesh_master_axi_AR.LEN),
               .ar_size              (uc_to_sys_mesh_master_axi_AR.SIZE),
               .ar_burst             (uc_to_sys_mesh_master_axi_AR.BURST),
               .ar_lock              (uc_to_sys_mesh_master_axi_AR.LOCK),
               .ar_cache             (uc_to_sys_mesh_master_axi_AR.CACHE),
               .ar_prot              (uc_to_sys_mesh_master_axi_AR.PROT),
               .ar_qos               (uc_to_sys_mesh_master_axi_AR.QOS),
               .ar_region            (),
               .ar_user              (uc_to_sys_mesh_master_axi_AR.USER),
               .ar_valid             (uc_to_sys_mesh_master_axi_ARVALID),
               .ar_ready             (uc_to_sys_mesh_master_axi_ARREADY),
               .ar_vc_valid          (),
               .ar_vc_credit         (),
               .ar_trans_id          (),

               .r_last               (uc_to_sys_mesh_master_axi_R.LAST),
               .r_user               (),
               .r_id                 (uc_to_sys_mesh_master_axi_R.ID),
               .r_data               (uc_to_sys_mesh_master_axi_R.DATA),
               .r_resp               (uc_to_sys_mesh_master_axi_R.RESP),
               .r_valid              (uc_to_sys_mesh_master_axi_RVALID),
               .r_ready              (uc_to_sys_mesh_master_axi_RREADY),
               .r_trans_id           ()
            );
      end
   endgenerate


   `ifdef EVL_SIMULATION

      wire m_shire_iso = |{ esr_pwr_ctrl_neigh_iso, esr_pwr_ctrl_glb_iso };

      //-----------------------------------------------------------------------------------------------
      //
      // Create an instance identifier.
      //
      bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                  SHIRE_RTL_AGENT,
                                                                  SHIRE_INST_ID,
                                                                  "evl_shire_env", "", "", "", { 1 });


      //-----------------------------------------------------------------------------------------------
      //
      // Create a Shire RTL port instance and drive it appropriately.
      //
      int                m_shire_inst_id;
      bit                m_start_monitor = 1'b0;
      bit                m_reset_on      = 1'b0;
      bit                m_reset_off     = 1'b0;
      evl_shire_rtl_port m_rtl_port      = new($sformatf("%m"), SHIRE_INST_ID, "", { 1 });

      `evl_monitor_misc_signal(ioshire_combined_err_int)

      `evl_drive_misc_signal(dv_force_dmactive, 1'b0)


      initial begin
         m_rtl_port.set_int_param_by_name("pk_noc_stub", NOC_STUB);
         m_rtl_port.set_int_param_by_name("pk_sc_stub", SC_STUB);
         m_rtl_port.set_int_param_by_name("pk_sc_lite", SC_LITE);
         m_rtl_port.set_int_param_by_name("pk_neigh_disable", NEIGH_DISABLE);
         m_rtl_port.set_int_param_by_name("pk_neigh_stub", NEIGH_STUB);
         m_rtl_port.set_int_param_by_name("pk_dbgnoc_stim", DBGNOC_STIM);
         m_rtl_port.set_int_param_by_name("pk_num_l3_ports", NUM_L3_MASTER_PORTS);
         m_rtl_port.set_int_param_by_name("pk_num_l3_master_ports", NUM_L3_MASTER_PORTS);
         m_rtl_port.set_int_param_by_name("pk_num_l3_slave_ports", NUM_L3_SLAVE_PORTS);
         m_shire_inst_id = SHIRE_INST_ID;
         #1ns m_start_monitor = 1'b1;
      end

      always @(posedge shire_clock) begin
         if (m_start_monitor === 1'b1) begin
            if ((reset_cold === 1'b1) && (m_reset_on === 1'b0)) begin
               m_rtl_port.assert_reset();
            end
            else if ((reset_cold === 1'b0) && (m_reset_off === 1'b0)) begin
               m_rtl_port.deassert_reset();
            end
            m_reset_on  <= ~reset_cold;
            m_reset_off <= reset_cold;
            m_rtl_port.ping_clock();
         end
      end


      generate
         if (SC_STUB == 0) begin
            //
            // Force ESRs to enable/disable correct HARTs
            // FUTURE: Emmanuel Marie Remove when we have a RAL
            //
            initial begin
               `ifdef UPF
                  longint shire_mask;
               `endif // ifdef UPF

               if (`et_test_plusargs("shire_coop_mode_en")) begin
                  force shire_channel_wrap_inst.shire_channel_gen.channel_inst.esrs.reg_shire_coop_mode = '1;
               end
               
            end
            //
            // The following should likely be moved (at least partially) into evl_shire_channel_interface.sv.
            //
            `include "dv/soc/shire/ucblock/ucblock_assert.svh"
            `ifdef EVL_COVERAGE_ON
               `include "dv/soc/shire/ucblock/ucblock_cov_capture.svh"
               `include "dv/shire_common/sbm/shire_bus_master_cvg_capture.svh"
            `endif
         end // if (SC_STUB == 0)
      endgenerate

      // Shire Bus Master assertions
      `include "dv/shire_common/sbm/shire_bus_master_assert.sv"

      // shire_top SVA assertions:
      `include "dv/shire_common/assert/shire_top_assert.svh"
 

      //
      // Performance monitoring
      //
      logic dreams;

      initial begin
         if (!`et_value_plusargs("DREAMS=%d", dreams)) begin
            dreams = 0;
         end
         if (dreams != 0) begin
            forever @(negedge shire_clock) begin
               if (~reset_cold) begin
                  $timeformat(-9, 3, "", 8);
                  // L2 read requests outgoing to other shire
                  for (integer axi_count = 0; axi_count < NUM_L3_MASTER_PORTS; axi_count++) begin
                     if (to_l3_mesh_master_axi_ARVALID[axi_count]) begin
                        $display("EVENT %9.0f 300 %d %d %d %h %h", $floor($realtime + 0.25), SHIRE_INST_ID, axi_count, to_l3_mesh_master_axi_ARREADY[axi_count], to_l3_mesh_master_axi_AR[axi_count].ADDR, to_l3_mesh_master_axi_AR[axi_count].ID);
                     end
                  end
                  // L2 read requests incoming to other shire
                  for (integer axi_count = 0; axi_count < NUM_L3_SLAVE_PORTS; axi_count++) begin
                     if (l3_mesh_slave_axi_ARVALID[axi_count]) begin
                        $display("EVENT %9.0f 301 %d %d %d %h %h", $floor($realtime + 0.25), SHIRE_INST_ID, axi_count, l3_mesh_slave_axi_ARREADY[axi_count], l3_mesh_slave_axi_AR[axi_count].ADDR, l3_mesh_slave_axi_AR[axi_count].ID);
                     end
                  end
                  // L3 read requests outgoing to MC
                  if (to_sys_mesh_master_axi_ARVALID) begin
                     $display("EVENT %9.0f 302 %d %d %h %h", $floor($realtime + 0.25), SHIRE_INST_ID, to_sys_mesh_master_axi_ARREADY, to_sys_mesh_master_axi_AR.ADDR, to_sys_mesh_master_axi_AR.ID);
                  end
                  // L3 read fills incoming from MC
                  if (to_sys_mesh_master_axi_RVALID) begin
                     $display("EVENT %9.0f 303 %d %d %h", $floor($realtime + 0.25), SHIRE_INST_ID, to_sys_mesh_master_axi_RREADY, to_sys_mesh_master_axi_R.ID);
                  end
                  // L2 read fill outgoing to dest shire
                  for (integer axi_count = 0; axi_count < NUM_L3_SLAVE_PORTS; axi_count++) begin
                     if (l3_mesh_slave_axi_RVALID[axi_count]) begin
                        $display("EVENT %9.0f 304 %d %d %d %h", $floor($realtime + 0.25), SHIRE_INST_ID, axi_count, l3_mesh_slave_axi_RREADY[axi_count], l3_mesh_slave_axi_R[axi_count].ID);
                     end
                  end
                  // L2 read fill incoming to dest shire
                  for (integer axi_count = 0; axi_count < NUM_L3_MASTER_PORTS; axi_count++) begin
                     if (to_l3_mesh_master_axi_RVALID[axi_count]) begin
                        $display("EVENT %9.0f 305 %d %d %d %h", $floor($realtime + 0.25), SHIRE_INST_ID, axi_count, to_l3_mesh_master_axi_RREADY[axi_count], to_l3_mesh_master_axi_R[axi_count].ID);
                     end
                  end
               end
            end
         end
      end
   `endif // ifdef EVL_SIMULATION


   `ifdef ENABLE_REPLAY_STIMULI
   //
   // NoC stimuli vectors dump
   //
   generate
       if ((NOC_STUB == 1'b0) && (SHIRE_INST_ID inside {24, 29, 6, 10, 3, 23})) begin
         int desc;
         bit log_noc_vectors;

         initial begin
            if (`et_value_plusargs("dump_noc_vectors=%d", log_noc_vectors)) begin
               if (log_noc_vectors == 1'b1) begin
                  desc = $save_stimuli_open($sformatf("compare_log_%0d_0_repl_noc.txt", SHIRE_INST_ID), shire_top.shire_top_logic_wrapper_inst.meshstop_wrapper, "clk__noc");
               end
            end
         end

         always @(negedge noc_clock) begin
            if (log_noc_vectors == 1'b1) begin
               $save_stimuli_log(desc);
            end
         end
      end
   endgenerate
   `endif

// PLL DVFS programming time assertions
`ifdef EVL_SIMULATION

   `ifndef ET_ASSERT_OFF

      generate
         if ((SC_STUB == 1'b0) && (SC_LITE == 1'b0)) begin
            logic[2:0] clock_mux_sel;
            realtime mux_change_time;
            realtime pll_program_time;;
            logic enable_pll_prog_assertion = 0;
            logic check_pll_prog_time = 0;
            bit[7:0] shire_id;
            logic pll_strobe_update;
            logic pll_clock;
            realtime clk_change;
            realtime clk_period;
            realtime prev_clk_period;
            realtime freq_change_time;
            
            assign clock_mux_sel = shire_channel_wrap_inst.shire_channel_gen.channel_inst.esrs.reg_shire_ctrl_clockmux.muxsel[2:0];
            assign shire_id = shire_channel_wrap_inst.shire_channel_gen.channel_inst.esrs.reg_shire_config.shire_id[7:0];
            assign pll_strobe_update = shire_channel_wrap_inst.shire_pll.pll_apb_reg_update;
            assign pll_clock = shire_channel_wrap_inst.shire_pll.o_clkpll[0];
            
            initial begin
              //Skip first couple of PLL programming (APB injector, normalization...)
              for(int i=0; i<3; i++) begin
                wait(clock_mux_sel == 3'b11);
                wait(clock_mux_sel != 3'b11);
                if(i==0) begin
                  @(posedge pll_clock);
                  clk_change = $realtime;
                  @(posedge pll_clock);
                  prev_clk_period = $realtime - clk_change;
                end
                else begin
                  @(posedge pll_clock);
                  clk_change = $realtime;
                  @(posedge pll_clock);
                  clk_period = $realtime - clk_change;
                  `uvm_info("LVDPLL_Frequency_change",$sformatf("Shire: %d , Frequency changed from %f MHz to %f MHz", shire_id, (1000/prev_clk_period), (1000/clk_period)),UVM_NONE)
                  prev_clk_period = clk_period;
                end
              end
            
              if(`et_test_plusargs("DVFS_ENABLE_LVDPLL_PROGRAM_TIME_ASRT") == 1) begin
                  enable_pll_prog_assertion = 1;
              end
              else enable_pll_prog_assertion = 0;
            
              forever begin
                wait(clock_mux_sel == 3'b11);
                mux_change_time = $realtime();
                @(posedge pll_strobe_update);
                pll_program_time = $realtime() - mux_change_time;
                check_pll_prog_time = 1;
                `uvm_info("LVDPLL_Programming_time",$sformatf("Shire: %d , LVDPLL programming time: %f ns", shire_id, pll_program_time),UVM_NONE)
                wait(clock_mux_sel != 3'b11);
                check_pll_prog_time = 0;
                freq_change_time = $realtime() - mux_change_time;
                @(posedge pll_clock);
                clk_change = $realtime;
                @(posedge pll_clock);
                clk_period = $realtime - clk_change;
                `uvm_info("LVDPLL_Frequency_change",$sformatf("Shire: %d , Frequency changed from %f MHz to %f MHz, time needed %f ns", shire_id, (1000/prev_clk_period), (1000/clk_period), freq_change_time),UVM_NONE)
                prev_clk_period = clk_period;
              end
            end

            `ASSERT_ALWAYS_MSG(ERROR_LVDPLL_Programming_time   , check_pll_prog_time, ~enable_pll_prog_assertion, ( pll_program_time < 5500 ) , " LVDPLL DVFS: LVDPLL program time is over 5.5us !") 
         end
      endgenerate
   `endif
`endif

///////////////////////////////////////////////
/////////////   COVERAGE   ////////////////////
//////////////////////////////////////////////

`ifdef EVL_COVERAGE_ON
  `ifndef NOC_COVERAGE

    `include "dv/soc/shire/shire_cov_capture.svh"

  `endif // ifndef NOC_COVERAGE
`endif // ifdef EVL_COVERAGE_ON

endmodule

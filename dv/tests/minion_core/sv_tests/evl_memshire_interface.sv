`include "rtl/inc/soc.vh"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"
`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

`ifndef DV_MEMSHIRE_USE_PLL_BEH
   `define DV_MEMSHIRE_USE_PLL_BEH 0
`endif // ifndef DV_MEMSHIRE_USE_PLL_BEH

`ifndef __EVL_MEMSHIRE_PKG
   `include "dv/soc/memshire/evl_memshire_pkg.sv"
`endif

module evl_memshire_interface #(parameter int SHIRE_INST_ID             = 255,
                                parameter int MEMSHIRE_ID               = 0,
                                parameter bit SHIRE_STUB                = 1'b0,
                                parameter bit NOC_STUB                  = 1'b0,
                                parameter bit MEMSHIRE_DBGNOC_STIM      = 1'b0,
                                parameter bit DV_MEMSHIRE_DEBUG_NO_INST = 1'b0) (
      input  wire                                reset,
      input  wire                                clock,
      input  wire                                reset__noc,
      input  wire                                clk__noc,
      inout  wire sc_slave_axi_AR_channel_t      mem_mesh_slave_axi_AR,
      inout  wire                                mem_mesh_slave_axi_ARVALID,
      inout  wire                                mem_mesh_slave_axi_ARREADY,
      inout  wire sc_slave_axi_AW_channel_t      mem_mesh_slave_axi_AW,
      inout  wire                                mem_mesh_slave_axi_AWVALID,
      inout  wire                                mem_mesh_slave_axi_AWREADY,
      inout  wire sc_slave_axi_W_channel_t       mem_mesh_slave_axi_W,
      inout  wire                                mem_mesh_slave_axi_WVALID,
      inout  wire                                mem_mesh_slave_axi_WREADY,
      inout  wire sc_slave_axi_B_channel_t       mem_mesh_slave_axi_B,
      inout  wire                                mem_mesh_slave_axi_BVALID,
      inout  wire                                mem_mesh_slave_axi_BREADY,
      inout  wire sc_slave_axi_R_channel_t       mem_mesh_slave_axi_R,
      inout  wire                                mem_mesh_slave_axi_RVALID,
      inout  wire                                mem_mesh_slave_axi_RREADY,

      inout  wire sc_master_axi_AR_channel_t     atomic_rsp_mesh_master_axi_AR,
      inout  wire                                atomic_rsp_mesh_master_axi_ARVALID,
      inout  wire                                atomic_rsp_mesh_master_axi_ARREADY,
      inout  wire sc_master_axi_AW_channel_t     atomic_rsp_mesh_master_axi_AW,
      inout  wire                                atomic_rsp_mesh_master_axi_AWVALID,
      inout  wire                                atomic_rsp_mesh_master_axi_AWREADY,
      inout  wire mem_atomic_rsp_axi_W_channel_t atomic_rsp_mesh_master_axi_W,
      inout  wire                                atomic_rsp_mesh_master_axi_WVALID,
      inout  wire                                atomic_rsp_mesh_master_axi_WREADY,
      inout  wire sc_master_axi_B_channel_t      atomic_rsp_mesh_master_axi_B,
      inout  wire                                atomic_rsp_mesh_master_axi_BVALID,
      inout  wire                                atomic_rsp_mesh_master_axi_BREADY,
      inout  wire mem_atomic_rsp_axi_R_channel_t atomic_rsp_mesh_master_axi_R,
      inout  wire                                atomic_rsp_mesh_master_axi_RVALID,
      inout  wire                                atomic_rsp_mesh_master_axi_RREADY,

      inout  wire sc_slave_axi_AR_channel_t      sbm_mesh_slave_axi_AR,
      inout  wire                                sbm_mesh_slave_axi_ARVALID,
      inout  wire                                sbm_mesh_slave_axi_ARREADY,
      inout  wire sc_slave_axi_AW_channel_t      sbm_mesh_slave_axi_AW,
      inout  wire                                sbm_mesh_slave_axi_AWVALID,
      inout  wire                                sbm_mesh_slave_axi_AWREADY,
      inout  wire mem_sbm_slave_axi_W_channel_t  sbm_mesh_slave_axi_W,
      inout  wire                                sbm_mesh_slave_axi_WVALID,
      inout  wire                                sbm_mesh_slave_axi_WREADY,
      inout  wire sc_slave_axi_B_channel_t       sbm_mesh_slave_axi_B,
      inout  wire                                sbm_mesh_slave_axi_BVALID,
      inout  wire                                sbm_mesh_slave_axi_BREADY,
      inout  wire mem_sbm_slave_axi_R_channel_t  sbm_mesh_slave_axi_R,
      inout  wire                                sbm_mesh_slave_axi_RVALID,
      inout  wire                                sbm_mesh_slave_axi_RREADY,

      // Names are intentionally different here.  For memshire standalone
      // DV we need to insert a delay on the AR/AW/W channels going into
      // the memshire so these signals connect a little differently
      inout  wire sc_slave_axi_AR_channel_t      ifc_ust_smb_axi_AR,
      inout  wire                                ifc_ust_smb_axi_ARVALID,
      inout  wire                                ifc_ust_smb_axi_ARREADY,
      inout  wire sc_slave_axi_AW_channel_t      ifc_ust_smb_axi_AW,
      inout  wire                                ifc_ust_smb_axi_AWVALID,
      inout  wire                                ifc_ust_smb_axi_AWREADY,
      inout  wire sc_slave_axi_W_channel_t       ifc_ust_smb_axi_W,
      inout  wire                                ifc_ust_smb_axi_WVALID,
      inout  wire                                ifc_ust_smb_axi_WREADY,
      inout  wire sc_slave_axi_B_channel_t       ust_smb_axi_B,
      inout  wire                                ust_smb_axi_BVALID,
      inout  wire                                ust_smb_axi_BREADY,
      inout  wire sc_slave_axi_R_channel_t       ust_smb_axi_R,
      inout  wire                                ust_smb_axi_RVALID,
      inout  wire                                ust_smb_axi_RREADY,

      inout  wire [127:0]                        smb_bridge_nsip_tx_msg_beat_data,
      inout  wire                                smb_bridge_nsip_tx_msg_beat_eop,
      inout  wire                                smb_bridge_nsip_tx_msg_beat_sop,
      inout  wire                                smb_bridge_nsip_tx_msg_beat_valid,
      inout  wire                                smb_bridge_nsip_rx_msg_credit_inc,
      inout  wire   [7:0]                        smb_bridge_nsip_tx_msg_dest_hp,
      inout  wire   [1:0]                        smb_bridge_nsip_tx_msg_dest_int,
      inout  wire   [3:0]                        smb_bridge_nsip_tx_msg_xfer_qos,
      inout  wire [127:0]                        smb_bridge_nsip_rx_msg_beat_data,
      inout  wire                                smb_bridge_nsip_rx_msg_beat_eop,
      inout  wire                                smb_bridge_nsip_rx_msg_beat_sop,
      inout  wire                                smb_bridge_nsip_rx_msg_beat_valid,
      inout  wire                                smb_bridge_nsip_tx_msg_credit_inc,

      inout  wire  [31:0]                        smb_bridge_nsip_tx_evt_beat_data,
      inout  wire                                smb_bridge_nsip_tx_evt_beat_eop,
      inout  wire                                smb_bridge_nsip_tx_evt_beat_sop,
      inout  wire                                smb_bridge_nsip_tx_evt_beat_valid,
      inout  wire                                smb_bridge_nsip_rx_evt_credit_inc,
      inout  wire   [7:0]                        smb_bridge_nsip_tx_evt_dest_hp,
      inout  wire   [1:0]                        smb_bridge_nsip_tx_evt_dest_int,
      inout  wire   [3:0]                        smb_bridge_nsip_tx_evt_xfer_qos,
      inout  wire  [31:0]                        smb_bridge_nsip_rx_evt_beat_data,
      inout  wire                                smb_bridge_nsip_rx_evt_beat_eop,
      inout  wire                                smb_bridge_nsip_rx_evt_beat_sop,
      inout  wire                                smb_bridge_nsip_rx_evt_beat_valid,
      inout  wire                                smb_bridge_nsip_tx_evt_credit_inc,

      inout  wire [127:0]                        normal_bridge_nsip_tx_msg_beat_data,
      inout  wire                                normal_bridge_nsip_tx_msg_beat_eop,
      inout  wire                                normal_bridge_nsip_tx_msg_beat_sop,
      inout  wire                                normal_bridge_nsip_tx_msg_beat_valid,
      inout  wire                                normal_bridge_nsip_rx_msg_credit_inc,
      inout  wire   [7:0]                        normal_bridge_nsip_tx_msg_dest_hp,
      inout  wire   [1:0]                        normal_bridge_nsip_tx_msg_dest_int,
      inout  wire   [3:0]                        normal_bridge_nsip_tx_msg_xfer_qos,
      inout  wire [127:0]                        normal_bridge_nsip_rx_msg_beat_data,
      inout  wire                                normal_bridge_nsip_rx_msg_beat_eop,
      inout  wire                                normal_bridge_nsip_rx_msg_beat_sop,
      inout  wire                                normal_bridge_nsip_rx_msg_beat_valid,
      inout  wire                                normal_bridge_nsip_tx_msg_credit_inc,

      inout  wire  [31:0]                        normal_bridge_nsip_tx_evt_beat_data,
      inout  wire                                normal_bridge_nsip_tx_evt_beat_eop,
      inout  wire                                normal_bridge_nsip_tx_evt_beat_sop,
      inout  wire                                normal_bridge_nsip_tx_evt_beat_valid,
      inout  wire                                normal_bridge_nsip_rx_evt_credit_inc,
      inout  wire   [7:0]                        normal_bridge_nsip_tx_evt_dest_hp,
      inout  wire   [1:0]                        normal_bridge_nsip_tx_evt_dest_int,
      inout  wire   [3:0]                        normal_bridge_nsip_tx_evt_xfer_qos,
      inout  wire  [31:0]                        normal_bridge_nsip_rx_evt_beat_data,
      inout  wire                                normal_bridge_nsip_rx_evt_beat_eop,
      inout  wire                                normal_bridge_nsip_rx_evt_beat_sop,
      inout  wire                                normal_bridge_nsip_rx_evt_beat_valid,
      inout  wire                                normal_bridge_nsip_tx_evt_credit_inc,

      // Interrupts (for monitoring only)
      input  wire                                u0_umctl2_ecc_uncorrected_err_intr_vmc,
      input  wire                                u0_umctl2_dfi_alert_err_intr_vmc,
      input  wire                                u0_umctl2_ecc_corrected_err_intr_vmc,
      input  wire                                u0_umctl2_sbr_done_intr_vmc,
      input  wire                                u0_umctl2_derate_temp_limit_intr_vmc,
      input  wire                                dfi0_error_vmc,
      input  wire [`DDR_DFI_ERR_SIZE-1:0]        dfi0_error_info_vmc,
      input  wire                                u1_umctl2_ecc_uncorrected_err_intr_vmc,
      input  wire                                u1_umctl2_dfi_alert_err_intr_vmc,
      input  wire                                u1_umctl2_ecc_corrected_err_intr_vmc,
      input  wire                                u1_umctl2_sbr_done_intr_vmc,
      input  wire                                u1_umctl2_derate_temp_limit_intr_vmc,
      input  wire                                dfi1_error_vmc,
      input  wire [`DDR_DFI_ERR_SIZE-1:0]        dfi1_error_info_vmc,
      input  wire                                dwc_ddrphy_int_n_vmc,
      input  wire                                p0_cntr_overflow_intr,
      input  wire                                p1_cntr_overflow_intr,
      input  wire [`MEMSHIRE_NOC_ERR_WIDTH-1:0]  noc_err_int_hv,
      input  wire                                memshire_critical_int_gv,
      input  wire                                memshire_normal_int_gv,

      // Clocks to monitor
      input  wire                                pclk_vmc,
      input  wire                                ddr_ss_clk_vmc,
      input  wire                                atpg_TxDllClk_vmc,
      input  wire                                atpg_Pclk_vmc,
      input  wire                                atpg_RDQSClk_vmc,

      input  wire                                ms_auto_init_inprog,
      input  wire                                dv_ms_auto_init_done,
      output wire                                ms_auto_init_enable,
      output wire [15:0]                         ms_auto_init_config,
      output wire                                ms_dv_force_dmactive
   );


   //-----------------------------------------------------------------------------------------------
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_memshire_pkg::*;

   wire [2:0]       mon_lo_mem_AR_arb_grants;
   evl_paddr_t      mon_lo_mem_AR_arb_addr;
   evl_bus_req_id_t mon_lo_mem_AR_arb_req_id;

   wire [2:0]       mon_lo_mem_AW_arb_grants;
   evl_paddr_t      mon_lo_mem_AW_arb_addr;
   evl_bus_req_id_t mon_lo_mem_AW_arb_req_id;

   wire [2:0]       mon_hi_mem_AR_arb_grants;
   evl_paddr_t      mon_hi_mem_AR_arb_addr;
   evl_bus_req_id_t mon_hi_mem_AR_arb_req_id;

   wire [2:0]       mon_hi_mem_AW_arb_grants;
   evl_paddr_t      mon_hi_mem_AW_arb_addr;
   evl_bus_req_id_t mon_hi_mem_AW_arb_req_id;

   wire               mon_AR_atomic_hit;
   wire [3:0]         mon_AR_atomic_hit_vector;
   evl_paddr_t        mon_lo_mem_AR_input_addr;
   evl_bus_req_id_t   mon_lo_mem_AR_input_req_id;
   evl_bus_req_user_t mon_lo_mem_AR_input_user;

   wire               mon_AW_atomic_hit;
   wire [3:0]         mon_AW_atomic_hit_vector;
   evl_paddr_t        mon_lo_mem_AW_input_addr;
   evl_bus_req_id_t   mon_lo_mem_AW_input_req_id;
   evl_bus_req_user_t mon_lo_mem_AW_input_user;

   // if this signal is true, then we are pushing an atomic response into the queue
   wire               mon_atomic_rsp_push;
   wire [3:0]         mon_atomic_hit;

   // if this signal is true then we ignore the aribtration for the MESH MEM only (UST/Evict still okay)
   wire               mon_AW_non_atomic_hit_valid;

   // this signal indicates the write is being completed to the atomic cache
   wire               mon_AW_cache_wr_complete;

   // reduce to single bit
   wire               noc_err_int_or_hv;
   assign noc_err_int_or_hv = |noc_err_int_hv;

   generate
      if (SHIRE_STUB == 1'b1) begin
         assign mon_lo_mem_AR_arb_grants = {3{1'b0}};
         assign mon_lo_mem_AR_arb_addr   = evl_paddr_t'(0);
         assign mon_lo_mem_AR_arb_req_id = evl_bus_req_id_t'(0);

         assign mon_lo_mem_AW_arb_grants = {3{1'b0}};
         assign mon_lo_mem_AW_arb_addr   = evl_paddr_t'(0);
         assign mon_lo_mem_AW_arb_req_id = evl_bus_req_id_t'(0);

         assign mon_hi_mem_AR_arb_grants = {3{1'b0}};
         assign mon_hi_mem_AR_arb_addr   = evl_paddr_t'(0);
         assign mon_hi_mem_AR_arb_req_id = evl_bus_req_id_t'(0);

         assign mon_hi_mem_AW_arb_grants = {3{1'b0}};
         assign mon_hi_mem_AW_arb_addr   = evl_paddr_t'(0);
         assign mon_hi_mem_AW_arb_req_id = evl_bus_req_id_t'(0);

         assign mon_AR_atomic_hit          = 1'b0;
         assign mon_AR_atomic_hit_vector   = {4{1'b0}};
         assign mon_lo_mem_AR_input_addr   = evl_paddr_t'(0);
         assign mon_lo_mem_AR_input_req_id = evl_bus_req_id_t'(0);
         assign mon_lo_mem_AR_input_user   = evl_bus_req_user_t'(0);

         assign mon_AW_atomic_hit          = 1'b0;
         assign mon_AW_atomic_hit_vector   = {4{1'b0}};
         assign mon_lo_mem_AW_input_addr   = evl_paddr_t'(0);
         assign mon_lo_mem_AW_input_req_id = evl_bus_req_id_t'(0);
         assign mon_lo_mem_AW_input_user   = evl_bus_req_user_t'(0);

         assign mon_AW_non_atomic_hit_valid = 1'b0;
         assign mon_AW_cache_wr_complete    = 1'b0;

         assign mon_atomic_rsp_push         = 1'b0;
         assign mon_atomic_hit              = 4'h0;
      end
      else begin
         assign mon_lo_mem_AR_arb_grants = {3{memshire_top.memshire_top.mem_logic.lo_mem_AR_fifo.push && !memshire_top.memshire_top.mem_logic.lo_mem_AR_fifo.full  }} & memshire_top.memshire_top.mem_logic.lo_AR_arb_grants;
         assign mon_lo_mem_AR_arb_addr   = evl_paddr_t'(memshire_top.memshire_top.mem_logic.lo_AR_arb_AR.ADDR);
         assign mon_lo_mem_AR_arb_req_id = evl_bus_req_id_t'(memshire_top.memshire_top.mem_logic.lo_AR_arb_AR.ID);

         // Condition the grants on the mon_AW_non_atomic_hit_valid signal- if true then ignore the MESH MEM grant (UST/Evict unaffected by this signal)
         assign mon_lo_mem_AW_arb_grants = {3{memshire_top.memshire_top.mem_logic.lo_mem_AW_fifo.push && !memshire_top.memshire_top.mem_logic.lo_mem_AW_fifo.full  }} & memshire_top.memshire_top.mem_logic.lo_AW_arb_grants & {1'b1, 1'b1, !mon_AW_non_atomic_hit_valid};
         assign mon_lo_mem_AW_arb_addr   = evl_paddr_t'(memshire_top.memshire_top.mem_logic.lo_AW_arb_AW.ADDR);
         assign mon_lo_mem_AW_arb_req_id = evl_bus_req_id_t'(memshire_top.memshire_top.mem_logic.lo_AW_arb_AW.ID);

         assign mon_hi_mem_AR_arb_grants = {3{memshire_top.memshire_top.mem_logic.hi_mem_AR_fifo.push && !memshire_top.memshire_top.mem_logic.hi_mem_AR_fifo.full  }} & 3'b001;
         assign mon_hi_mem_AR_arb_addr   = evl_paddr_t'(memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_AR.ADDR);
         assign mon_hi_mem_AR_arb_req_id = evl_bus_req_id_t'(memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_AR.ID);

         assign mon_hi_mem_AW_arb_grants = {3{memshire_top.memshire_top.mem_logic.hi_mem_AW_fifo.push && !memshire_top.memshire_top.mem_logic.hi_mem_AW_fifo.full }} & 3'b001;
         assign mon_hi_mem_AW_arb_addr   = evl_paddr_t'(memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_AW.ADDR);
         assign mon_hi_mem_AW_arb_req_id = evl_bus_req_id_t'(memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_AW.ID);

         assign mon_AR_atomic_hit          = |memshire_top.memshire_top.mem_logic.ms_atomic.ar_hit && (memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_valid && memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_pop);
         assign mon_AR_atomic_hit_vector   = memshire_top.memshire_top.mem_logic.ms_atomic.ar_hit;
         assign mon_lo_mem_AR_input_addr   = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_AR.ADDR;
         assign mon_lo_mem_AR_input_req_id = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_AR.ID;
         assign mon_lo_mem_AR_input_user   = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AR_fifo_AR.USER;

         assign mon_AW_atomic_hit          = |memshire_top.memshire_top.mem_logic.ms_atomic.aw_hit && (memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_valid && memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_pop);
         assign mon_AW_atomic_hit_vector   = memshire_top.memshire_top.mem_logic.ms_atomic.aw_hit;
         assign mon_lo_mem_AW_input_addr   = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_AW.ADDR;
         assign mon_lo_mem_AW_input_req_id = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_AW.ID;
         assign mon_lo_mem_AW_input_user   = memshire_top.memshire_top.mem_logic.mem_mesh_slave_AWW_fifo_AW.USER;

         assign mon_AW_non_atomic_hit_valid = memshire_top.memshire_top.mem_logic.ms_atomic.aw_non_atomic_hit_valid;
         assign mon_AW_cache_wr_complete    = memshire_top.memshire_top.mem_logic.ms_atomic.dv_aw_mem_visible_unused;

         assign mon_atomic_rsp_push         = memshire_top.memshire_top.mem_logic.ms_atomic.pipe_valid &
                                              memshire_top.memshire_top.mem_logic.ms_atomic.atomic_rsp_fifo_ready;
         assign mon_atomic_hit              = memshire_top.memshire_top.mem_logic.ms_atomic.aw_hit;
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
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (normal_bridge_nsip_tx_msg_dest_hp),
            .tx_req_dest_int  (normal_bridge_nsip_tx_msg_dest_int),
            .tx_req_qos       (normal_bridge_nsip_tx_msg_xfer_qos),
            .tx_req_valid     (normal_bridge_nsip_tx_msg_beat_valid),
            .tx_req_data      (normal_bridge_nsip_tx_msg_beat_data),
            .tx_req_sop       (normal_bridge_nsip_tx_msg_beat_sop),
            .tx_req_eop       (normal_bridge_nsip_tx_msg_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (normal_bridge_nsip_rx_msg_beat_data),
            .rx_req_valid     (normal_bridge_nsip_rx_msg_beat_valid),
            .rx_req_sop       (normal_bridge_nsip_rx_msg_beat_sop),
            .rx_req_eop       (normal_bridge_nsip_rx_msg_beat_eop),

            .host_credit_inc  (normal_bridge_nsip_rx_msg_credit_inc),
            .noc_credit_inc   (normal_bridge_nsip_tx_msg_credit_inc)
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
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (normal_bridge_nsip_tx_evt_dest_hp),
            .tx_req_dest_int  (normal_bridge_nsip_tx_evt_dest_int),
            .tx_req_qos       (normal_bridge_nsip_tx_evt_xfer_qos),
            .tx_req_valid     (normal_bridge_nsip_tx_evt_beat_valid),
            .tx_req_data      (normal_bridge_nsip_tx_evt_beat_data),
            .tx_req_sop       (normal_bridge_nsip_tx_evt_beat_sop),
            .tx_req_eop       (normal_bridge_nsip_tx_evt_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (normal_bridge_nsip_rx_evt_beat_data),
            .rx_req_valid     (normal_bridge_nsip_rx_evt_beat_valid),
            .rx_req_sop       (normal_bridge_nsip_rx_evt_beat_sop),
            .rx_req_eop       (normal_bridge_nsip_rx_evt_beat_eop),

            .host_credit_inc  (normal_bridge_nsip_rx_evt_credit_inc),
            .noc_credit_inc   (normal_bridge_nsip_tx_evt_credit_inc)
         );

      //
      // Debug NoC LF_MSG IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (SHIRE_INST_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (128),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_MSG_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_lf_msg (
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (smb_bridge_nsip_tx_msg_dest_hp),
            .tx_req_dest_int  (smb_bridge_nsip_tx_msg_dest_int),
            .tx_req_qos       (smb_bridge_nsip_tx_msg_xfer_qos),
            .tx_req_valid     (smb_bridge_nsip_tx_msg_beat_valid),
            .tx_req_data      (smb_bridge_nsip_tx_msg_beat_data),
            .tx_req_sop       (smb_bridge_nsip_tx_msg_beat_sop),
            .tx_req_eop       (smb_bridge_nsip_tx_msg_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (smb_bridge_nsip_rx_msg_beat_data),
            .rx_req_valid     (smb_bridge_nsip_rx_msg_beat_valid),
            .rx_req_sop       (smb_bridge_nsip_rx_msg_beat_sop),
            .rx_req_eop       (smb_bridge_nsip_rx_msg_beat_eop),

            .host_credit_inc  (smb_bridge_nsip_rx_msg_credit_inc),
            .noc_credit_inc   (smb_bridge_nsip_tx_msg_credit_inc)
         );

      //
      // Debug NoC LF_EVT IFC
      //

      evl_dbg_noc_interface #(.SHIRE_ID       (SHIRE_INST_ID),
                              .ADDR_WIDTH     (8),
                              .DATA_WIDTH     (32),
                              .QOS_WIDTH      (4),
                              .PORT_TYPE      (`EVL_DBGNOC_EVT_IFC_ID),
                              .VC_CREDITS     (8)) m_dbg_lf_evt (
            .reset            (reset),
            .clk              (clock),

            .tx_req_addr      (smb_bridge_nsip_tx_evt_dest_hp),
            .tx_req_dest_int  (smb_bridge_nsip_tx_evt_dest_int),
            .tx_req_qos       (smb_bridge_nsip_tx_evt_xfer_qos),
            .tx_req_valid     (smb_bridge_nsip_tx_evt_beat_valid),
            .tx_req_data      (smb_bridge_nsip_tx_evt_beat_data),
            .tx_req_sop       (smb_bridge_nsip_tx_evt_beat_sop),
            .tx_req_eop       (smb_bridge_nsip_tx_evt_beat_eop),
            .tx_req_trans_id  (),

            .rx_req_data      (smb_bridge_nsip_rx_evt_beat_data),
            .rx_req_valid     (smb_bridge_nsip_rx_evt_beat_valid),
            .rx_req_sop       (smb_bridge_nsip_rx_evt_beat_sop),
            .rx_req_eop       (smb_bridge_nsip_rx_evt_beat_eop),

            .host_credit_inc  (smb_bridge_nsip_rx_evt_credit_inc),
            .noc_credit_inc   (smb_bridge_nsip_tx_evt_credit_inc)
         );


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

   tri0 [`EVL_BUS_REGION_MSB:0] ust_smb_axi_ARREGION;
   tri0 [`EVL_BUS_REGION_MSB:0] ust_smb_axi_AWREGION;

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

   tri0 [`DV_TRANS_ID_SIZE-1:0] ust_smb_axi_ar_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ust_smb_axi_aw_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ust_smb_axi_w_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ust_smb_axi_r_trans_id;
   tri0 [`DV_TRANS_ID_SIZE-1:0] ust_smb_axi_b_trans_id;
   tri0                         ust_smb_axi_WUSER;
   tri0                         ust_smb_axi_BUSER;
   tri0                         ust_smb_axi_RUSER;

   //
   // Create the mesh-to-memory-shire AXI bus agent interface (64 bytes wide).
   //
   evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                       .PORT_TYPE    (int'(evl_base_pkg::AXI_FROM_SYSMESH)),
                       .AXI_COV_TYPE (evl_base_pkg::EVL_COV_AXI_SYS_MEM_64),
                       .AXI_COV_NAME ("AXI_SYS_MEM_64"),
                       .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                       .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH (3),
                       .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AR_VC_WIDTH  (2),
                       .AW_VC_WIDTH  (2),
                       .ALLOW_DRV_W_BEFORE_AW (1),
`ifdef DV_MEMSHIRE_MESH_SLAVE_AXI_VSI
                       .AR_VC_CREDITS(`MEM_MESH_SLAVE_VSI_DEPTH),
                       .AW_VC_CREDITS(`MEM_MESH_SLAVE_VSI_DEPTH)
`else
                       .AR_VC_CREDITS(0),
                       .AW_VC_CREDITS(0)
`endif
                     ) m_mesh_to_mem_axi (
         .isolate      (1'b0),
         .reset        (reset__noc),
         .clk          (clk__noc),

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
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (mem_mesh_slave_axi_aw_trans_id),

         .w_user       (mem_mesh_slave_axi_WUSER),
         .w_data       (mem_mesh_slave_axi_W.DATA),
         .w_strb       (mem_mesh_slave_axi_W.STRB),
         .w_last       (mem_mesh_slave_axi_W.LAST),
         .w_valid      (mem_mesh_slave_axi_WVALID),
         .w_ready      (mem_mesh_slave_axi_WREADY),
         .w_vc_valid   (),
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
         .ar_vc_valid  (),
         .ar_vc_credit (),
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
   // Create the memory-shire-to-mesh atomic AXI bus agent interface (32 bytes wide).
   //
   evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                       .PORT_TYPE    (int'(evl_base_pkg::AXI_TO_SYSMESH)),
                       .AXI_COV_TYPE (evl_base_pkg::EVL_COV_AXI_SYS_ESR_32),
                       .AXI_COV_NAME ("AXI_SYS_ESR_32"),
                       .ID_WIDTH     (`SC_MESH_MASTER_AXI_ID_SIZE),
                       .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH   (`MEM_ATOMIC_RSP_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH (3),
                       .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .ALLOW_DUPLICATE_OUTSTANDING_IDS (1)
                      ) m_mem_atomic_to_mesh_axi (
         .isolate      (1'b0),
         .reset        (reset__noc),
         .clk          (clk__noc),

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
   // Create the mesh-to-memory-shire-SBM AXI bus agent interface (8 bytes wide).
   //
   evl_axi_interface #(.SHIRE_ID      (SHIRE_INST_ID),
                       .PORT_TYPE     (int'(evl_base_pkg::AXI_FROM_SYSMESH_TO_SBM)),
                       .AXI_COV_TYPE  (evl_base_pkg::EVL_COV_AXI_MEM_SBM_8),
                       .AXI_COV_NAME ("AXI_MEM_SBM_8"),
                       .ID_WIDTH      (`SC_MESH_SLAVE_AXI_ID_SIZE),
                       .ADDR_WIDTH    (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH    (`MEM_SBM_MESH_SLAVE_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH  (3),
                       .ARUSER_WIDTH  (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH  (`SC_MESH_MASTER_AXI_AWUSER_SIZE),
                       .AR_VC_WIDTH   (1),
                       .AW_VC_WIDTH   (1),
                       .AR_VC_CREDITS (0),
                       .AW_VC_CREDITS (0)) m_mesh_to_mem_sbm_axi (
         .isolate      (1'b0),
         .reset        (reset__noc),
         .clk          (clk__noc),

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
         .aw_ready     (sbm_mesh_slave_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (sbm_mesh_slave_axi_aw_trans_id),

         .w_user       (sbm_mesh_slave_axi_WUSER),
         .w_data       (sbm_mesh_slave_axi_W.DATA),
         .w_strb       (sbm_mesh_slave_axi_W.STRB),
         .w_last       (sbm_mesh_slave_axi_W.LAST),
         .w_valid      (sbm_mesh_slave_axi_WVALID),
         .w_ready      (sbm_mesh_slave_axi_WREADY),
         .w_vc_valid   (),
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

   if (SHIRE_STUB == 1'b1) begin :g_ust2mem
   end
   else begin :g_ust2mem
   //
   // Create the UltraSoc-to-memory AXI bus agent interface (64 bytes wide).
   //
   evl_axi_interface #(.SHIRE_ID     (SHIRE_INST_ID),
                       .PORT_TYPE    (int'(evl_base_pkg::AXI_FROM_UST)),
                       .AXI_COV_TYPE (evl_base_pkg::EVL_COV_AXI_UST_MEM_64),
                       .AXI_COV_NAME ("AXI_UST_MEM_64"),
                       .ID_WIDTH     (`SC_MESH_SLAVE_AXI_ID_SIZE),
                       .ADDR_WIDTH   (`SC_MESH_MASTER_AXI_ADDR_SIZE),
                       .DATA_WIDTH   (`SC_MESH_MASTER_AXI_DATA_SIZE),
                       .AXSIZE_WIDTH (3),
                       .ARUSER_WIDTH (`SC_MESH_MASTER_AXI_ARUSER_SIZE),
                       .AWUSER_WIDTH (`SC_MESH_MASTER_AXI_AWUSER_SIZE)) m_ust_smb_to_mem_axi (
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

         .aw_id        (ifc_ust_smb_axi_AW.ID),
         .aw_addr      (ifc_ust_smb_axi_AW.ADDR),
         .aw_len       (ifc_ust_smb_axi_AW.LEN),
         .aw_size      (ifc_ust_smb_axi_AW.SIZE),
         .aw_burst     (ifc_ust_smb_axi_AW.BURST),
         .aw_lock      (ifc_ust_smb_axi_AW.LOCK),
         .aw_cache     (ifc_ust_smb_axi_AW.CACHE),
         .aw_prot      (ifc_ust_smb_axi_AW.PROT),
         .aw_qos       (ifc_ust_smb_axi_AW.QOS),
         .aw_region    (ust_smb_axi_AWREGION),
         .aw_user      (ifc_ust_smb_axi_AW.USER),
         .aw_valid     (ifc_ust_smb_axi_AWVALID),
         .aw_ready     (ifc_ust_smb_axi_AWREADY),
         .aw_vc_valid  (),
         .aw_vc_credit (),
         .aw_trans_id  (ust_smb_axi_aw_trans_id),

         .w_user       (ust_smb_axi_WUSER),
         .w_data       (ifc_ust_smb_axi_W.DATA),
         .w_strb       (ifc_ust_smb_axi_W.STRB),
         .w_last       (ifc_ust_smb_axi_W.LAST),
         .w_valid      (ifc_ust_smb_axi_WVALID),
         .w_ready      (ifc_ust_smb_axi_WREADY),
         .w_vc_valid   (),
         .w_trans_id   (ust_smb_axi_w_trans_id),

         .b_user       (ust_smb_axi_BUSER),
         .b_id         (ust_smb_axi_B.ID),
         .b_resp       (ust_smb_axi_B.RESP),
         .b_valid      (ust_smb_axi_BVALID),
         .b_ready      (ust_smb_axi_BREADY),
         .b_trans_id   (ust_smb_axi_b_trans_id),

         .ar_id        (ifc_ust_smb_axi_AR.ID),
         .ar_addr      (ifc_ust_smb_axi_AR.ADDR),
         .ar_len       (ifc_ust_smb_axi_AR.LEN),
         .ar_size      (ifc_ust_smb_axi_AR.SIZE),
         .ar_burst     (ifc_ust_smb_axi_AR.BURST),
         .ar_lock      (ifc_ust_smb_axi_AR.LOCK),
         .ar_cache     (ifc_ust_smb_axi_AR.CACHE),
         .ar_prot      (ifc_ust_smb_axi_AR.PROT),
         .ar_qos       (ifc_ust_smb_axi_AR.QOS),
         .ar_region    (ust_smb_axi_ARREGION),
         .ar_user      (ifc_ust_smb_axi_AR.USER),
         .ar_valid     (ifc_ust_smb_axi_ARVALID),
         .ar_ready     (ifc_ust_smb_axi_ARREADY),
         .ar_vc_valid  (),
         .ar_vc_credit (),
         .ar_trans_id  (ust_smb_axi_ar_trans_id),

         .r_last       (ust_smb_axi_R.LAST),
         .r_user       (ust_smb_axi_RUSER),
         .r_id         (ust_smb_axi_R.ID),
         .r_data       (ust_smb_axi_R.DATA),
         .r_resp       (ust_smb_axi_R.RESP),
         .r_valid      (ust_smb_axi_RVALID),
         .r_ready      (ust_smb_axi_RREADY),
         .r_trans_id   (ust_smb_axi_r_trans_id)
      );
   end // (SHIRE_STUB == 1'b1)

   // APB/ESR register stuff
   wire [`MEM_REG_APB_A_WIDTH-1:0] mon_apb_paddr;
   wire                            mon_apb_pwrite;
   wire                            mon_apb_psel;
   wire                            mon_apb_penable;
   wire [`MEM_REG_APB_D_WIDTH-1:0] mon_apb_pwdata;
   wire                            mon_apb_pready;
   wire [`MEM_REG_APB_D_WIDTH-1:0] mon_apb_prdata;
   wire                            mon_apb_pslverr;
   wire                            mon_apb_sel_bpam_mux;
   wire                            mon_apb_sel_ai_mux;

   if (SHIRE_STUB == 1'b1) begin :g_esr_apb
      assign mon_apb_paddr        = {`MEM_REG_APB_A_WIDTH{1'bz}};
      assign mon_apb_pwrite       = 1'bz;
      assign mon_apb_psel         = 1'bz;
      assign mon_apb_penable      = 1'bz;
      assign mon_apb_pwdata       = {`MEM_REG_APB_D_WIDTH{1'bz}};
      assign mon_apb_pready       = 1'bz;
      assign mon_apb_prdata       = {`MEM_REG_APB_D_WIDTH{1'bz}};
      assign mon_apb_pslverr      = 1'bz;
      assign mon_apb_sel_bpam_mux = 1'bz;
      assign mon_apb_sel_ai_mux   = 1'bz;
   end
   else begin :g_esr_apb
      assign mon_apb_paddr   = memshire_top.memshire_top.reg_ctl.muxed_paddr;
      assign mon_apb_pwrite  = memshire_top.memshire_top.reg_ctl.muxed_pwrite;
      assign mon_apb_psel    = memshire_top.memshire_top.reg_ctl.muxed_psel;
      assign mon_apb_penable = memshire_top.memshire_top.reg_ctl.muxed_penable;
      assign mon_apb_pwdata  = memshire_top.memshire_top.reg_ctl.muxed_pwdata;
      assign mon_apb_pready  = memshire_top.memshire_top.reg_ctl.muxed_pready;
      assign mon_apb_prdata  = memshire_top.memshire_top.reg_ctl.muxed_prdata;
      assign mon_apb_pslverr = memshire_top.memshire_top.reg_ctl.muxed_pslverr;
      assign mon_apb_sel_bpam_mux = memshire_top.memshire_top.reg_ctl.sel_bpam_mux;
      assign mon_apb_sel_ai_mux   = memshire_top.memshire_top.reg_ctl.sel_ai_mux;

      evl_apb_interface #(.AGENT_ID    (SHIRE_INST_ID),
                          .ADDR_WIDTH  (`MEM_REG_APB_A_WIDTH),
                          .DATA_WIDTH  (`MEM_REG_APB_D_WIDTH)) m_esr_apb (
            .isolate      ( 1'b0 ),
            .pclk         (  memshire_top.memshire_top.reg_ctl.clock ),
            .presetn      ( ~memshire_top.memshire_top.reg_ctl.reset ),
            .penable      ( mon_apb_penable ),
            .psel         ( mon_apb_psel    ),
            .pwrite       ( mon_apb_pwrite  ),
            .paddr        ( mon_apb_paddr   ),
            .pwdata       ( mon_apb_pwdata  ),
            .pready       ( mon_apb_pready  ),
            .prdata       ( mon_apb_prdata  ),
            .pslverr      ( mon_apb_pslverr )
         );
   end // (SHIRE_STUB == 1'b1)

   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                               MEM_SHIRE_RTL_AGENT,
                                                               SHIRE_INST_ID,
                                                               "evl_memshire_env", $sformatf("pk_memshire_id=%0d, pk_memshire_use_pll_beh=%0d", MEMSHIRE_ID, `DV_MEMSHIRE_USE_PLL_BEH), "", "", { 1 });


   //-----------------------------------------------------------------------------------------------
   //
   // Create a Shire RTL port instance and drive it appropriately.
   //
   int                   m_shire_inst_id;
   bit                   m_start_monitor = 1'b0;
   bit                   m_reset_on      = 1'b0;
   bit                   m_reset_off     = 1'b0;
   bit                   m_debug_no_inst;
   evl_memshire_rtl_port m_rtl_port      = new($sformatf("%m"), SHIRE_INST_ID, $sformatf("pk_memshire_id=%0d, pk_memshire_use_pll_beh=%0d", MEMSHIRE_ID, `DV_MEMSHIRE_USE_PLL_BEH), { 1 });

   initial begin
      m_debug_no_inst = DV_MEMSHIRE_DEBUG_NO_INST;
      m_shire_inst_id = SHIRE_INST_ID;
      m_rtl_port.set_int_param_by_name("pk_shire_stub", SHIRE_STUB);
      m_rtl_port.set_int_param_by_name("pk_noc_stub", NOC_STUB);
      m_rtl_port.set_int_param_by_name("pk_debug_no_inst", m_debug_no_inst);
      m_rtl_port.set_int_param_by_name("pk_dbgnoc_stim", MEMSHIRE_DBGNOC_STIM);
      m_rtl_port.set_int_param_by_name("pk_memshire_use_pll_beh", `DV_MEMSHIRE_USE_PLL_BEH);
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
            if (mon_atomic_rsp_push === 1'b1) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.atomic_info.info.is_atomic = 1'b1;
               packet.atomic_info.info.hit_vector = mon_atomic_hit;
               m_rtl_port.send_packet(MEMSHIRE_ATOMIC_RSP_PUSH, packet);
            end
            if (mon_AR_atomic_hit === 1'b1) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.atomic_info.info.hit_vector = mon_AR_atomic_hit_vector;
               packet.atomic_info.info.write_req  = 1'b0;
               packet.atomic_info.info.is_atomic  = 1'b0;
               packet.atomic_info.info.req_id     = mon_lo_mem_AR_input_req_id;
               packet.atomic_info.info.addr       = mon_lo_mem_AR_input_addr;
               m_rtl_port.send_packet(MEMSHIRE_ATOMIC_HIT, packet);
            end
            if (mon_AW_atomic_hit === 1'b1) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.atomic_info.info.hit_vector = mon_AW_atomic_hit_vector;
               packet.atomic_info.info.write_req  = 1'b1;
               packet.atomic_info.info.is_atomic  = (mon_lo_mem_AW_input_user == evl_bus_req_user_t'(evl_etlink_types_pkg::ET_LINK_REQ_Atomic)) ? 1'b1 : 1'b0;
               packet.atomic_info.info.req_id     = mon_lo_mem_AW_input_req_id;
               packet.atomic_info.info.addr       = mon_lo_mem_AW_input_addr;
               m_rtl_port.send_packet(MEMSHIRE_ATOMIC_HIT, packet);
            end
            if (mon_lo_mem_AR_arb_grants !== 3'b000) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.serialize_info.info.grants    = (mon_lo_mem_AR_arb_grants[0] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_MESH   :
                                                      (mon_lo_mem_AR_arb_grants[1] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_UST    :
                                                      (mon_lo_mem_AR_arb_grants[2] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC : `EVL_MEMSHIRE_REQ_ID_FOR_MESH;
               packet.serialize_info.info.write_req = 1'b0;
               packet.serialize_info.info.req_id    = mon_lo_mem_AR_arb_req_id;
               packet.serialize_info.info.addr      = mon_lo_mem_AR_arb_addr;
               // If the real Debug logic is in place and this is a Debug event, wait until the next negedge to send the packet.
               // Otherwise if we don't delay, the serialization event might arrive to the scoreboard before the request descriptor
               // from the AXI interface VALID && READY
               if ((m_debug_no_inst == 1'b0) && (mon_lo_mem_AR_arb_grants[1] == 1'b1)) begin
                  delay_and_send_packet(MEMSHIRE_LO_SERIALIZE, packet, 0, 1);
               end
               else begin
                  m_rtl_port.send_packet(MEMSHIRE_LO_SERIALIZE, packet);
               end
            end
            if (mon_lo_mem_AW_arb_grants !== 3'b000) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.serialize_info.info.grants    = (mon_lo_mem_AW_arb_grants[0] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_MESH   :
                                                      (mon_lo_mem_AW_arb_grants[1] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_UST    :
                                                      (mon_lo_mem_AW_arb_grants[2] == 1'b1) ? `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC : `EVL_MEMSHIRE_REQ_ID_FOR_MESH;
               packet.serialize_info.info.write_req = 1'b1;
               packet.serialize_info.info.req_id    = mon_lo_mem_AW_arb_req_id;
               packet.serialize_info.info.addr      = mon_lo_mem_AW_arb_addr;
               // If the real Debug logic is in place and this is a Debug event, wait until the next negedge to send the packet
               // Otherwise if we don't delay, the serialization event might arrive to the scoreboard before the request descriptor
               // from the AXI interface VALID && READY
               if ((m_debug_no_inst == 1'b0) && (mon_lo_mem_AW_arb_grants[1] == 1'b1)) begin
                  delay_and_send_packet(MEMSHIRE_LO_SERIALIZE, packet, 0, 1);
               end
               else begin
                  m_rtl_port.send_packet(MEMSHIRE_LO_SERIALIZE, packet);
               end
            end
            if (mon_hi_mem_AR_arb_grants !== 3'b000) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.serialize_info.info.grants    = `EVL_MEMSHIRE_REQ_ID_FOR_MESH;
               packet.serialize_info.info.write_req = 1'b0;
               packet.serialize_info.info.req_id    = mon_hi_mem_AR_arb_req_id;
               packet.serialize_info.info.addr      = mon_hi_mem_AR_arb_addr;
               m_rtl_port.send_packet(MEMSHIRE_HI_SERIALIZE, packet);
            end
            if (mon_hi_mem_AW_arb_grants !== 3'b000) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.serialize_info.info.grants    = `EVL_MEMSHIRE_REQ_ID_FOR_MESH;
               packet.serialize_info.info.write_req = 1'b1;
               packet.serialize_info.info.req_id    = mon_hi_mem_AW_arb_req_id;
               packet.serialize_info.info.addr      = mon_hi_mem_AW_arb_addr;
               m_rtl_port.send_packet(MEMSHIRE_HI_SERIALIZE, packet);
            end
            if (mon_AW_cache_wr_complete === 1'b1) begin
               evl_memshire_packet_u packet;

               // no real info here, just telling the scoreboard that
               // a write (or atomic) was completed.
               packet.packet = 0;
               m_rtl_port.send_packet(MEMSHIRE_CACHE_WR, packet);
            end
         end
         m_reset_on  <= reset;
         m_reset_off <= ~reset;

         m_rtl_port.ping_clock();
      end
   end

   always @(negedge clock) begin
      if (m_start_monitor === 1'b1) begin
         if (reset === 1'b0) begin
            // Do the negedge for this event so the scoreboard can know this info before APB requests are sent to the scoreboard
            if ((mon_apb_psel === 1'b1) && (mon_apb_penable === 1'b0)) begin
               evl_memshire_packet_u packet;

               packet.packet = 0;
               packet.reg_arb_info.info.sel_bpam_mux = mon_apb_sel_bpam_mux;
               packet.reg_arb_info.info.sel_ai_mux   = mon_apb_sel_ai_mux;
               m_rtl_port.send_packet(MEMSHIRE_REG_ARB, packet);
            end
         end
      end
   end


   //--------------------------------------------------------------------------------------------
   //
   // Task: delay_and_send_packet
   //
   // This task is used to delay a specified amount of clock cycles (either
   // posedge or negedge) before sending a serialization event packet to the
   // scoreboard.
   //
   task automatic delay_and_send_packet(input int                   packet_type_in,
                                        input evl_memshire_packet_u packet_in,
                                        input int unsigned          num_cycles,
                                        input bit                   use_negedge = 1'b0);
      fork
         begin
            if (use_negedge == 1'b1) begin
               repeat (num_cycles+1) begin
                  @(negedge clock);
               end
            end
            else begin
               repeat (num_cycles+1) begin
                  @(posedge clock);
               end
            end
            m_rtl_port.send_packet(packet_type_in, packet_in);
         end
      join_none
   endtask : delay_and_send_packet

   //
   // To enable auto-init, perform m_rtl_port.set_int_param_by_name("pk_ms_auto_init_enable_drv", 1).
   //
   `evl_drive_misc_signal(ms_auto_init_enable, 0)

   //
   // As part of auto-init, we can set up DDRC parameters with pk_ms_auto_init_config_drv.
   //
   `evl_drive_misc_bus(ms_auto_init_config, 0, 14, 0)

   //
   // ms_auto_init_inprog can be monitored with pk_ms_auto_init_config_mon.
   //
   `evl_monitor_misc_signal(ms_auto_init_inprog)

   //
   // dv_ms_auto_init_done can be monitored with pk_dv_ms_auto_init_done_mon.
   //
   `evl_monitor_misc_signal(dv_ms_auto_init_done)

   //
   // dv_force_dmactive is used to force the debug reset to be turned off. m_rtl_port.set_int_param_by_name("pk_ms_dv_force_dmactive_drv", 1).
   //
   `evl_drive_misc_signal(ms_dv_force_dmactive, 1'b0)

   //
   // Interrupt signals for monitoring (pk_<signal_name>_mon).
   //
   `evl_monitor_misc_signal(u0_umctl2_ecc_uncorrected_err_intr_vmc)
   `evl_monitor_misc_signal(u0_umctl2_dfi_alert_err_intr_vmc)
   `evl_monitor_misc_signal(u0_umctl2_ecc_corrected_err_intr_vmc)
   `evl_monitor_misc_signal(u0_umctl2_sbr_done_intr_vmc)
   `evl_monitor_misc_signal(u0_umctl2_derate_temp_limit_intr_vmc)
   `evl_monitor_misc_signal(dfi0_error_vmc)
   `evl_monitor_misc_signal(u1_umctl2_ecc_uncorrected_err_intr_vmc)
   `evl_monitor_misc_signal(u1_umctl2_dfi_alert_err_intr_vmc)
   `evl_monitor_misc_signal(u1_umctl2_ecc_corrected_err_intr_vmc)
   `evl_monitor_misc_signal(u1_umctl2_sbr_done_intr_vmc)
   `evl_monitor_misc_signal(u1_umctl2_derate_temp_limit_intr_vmc)
   `evl_monitor_misc_signal(dfi1_error_vmc)
   `evl_monitor_misc_signal(dwc_ddrphy_int_n_vmc)

   `evl_monitor_misc_signal(p0_cntr_overflow_intr)
   `evl_monitor_misc_signal(p1_cntr_overflow_intr)

   `evl_monitor_misc_bus(dfi0_error_info_vmc, (`DDR_DFI_ERR_SIZE-1), 0)
   `evl_monitor_misc_bus(dfi1_error_info_vmc, (`DDR_DFI_ERR_SIZE-1), 0)
   `evl_monitor_misc_bus(noc_err_int_hv, (`MEMSHIRE_NOC_ERR_WIDTH-1), 0)

   `evl_monitor_misc_signal(noc_err_int_or_hv)
   `evl_monitor_misc_signal(memshire_critical_int_gv)
   `evl_monitor_misc_signal(memshire_normal_int_gv)

   `evl_monitor_misc_signal(clock)
   `evl_monitor_misc_signal(pclk_vmc)
   `evl_monitor_misc_signal(ddr_ss_clk_vmc)
   `evl_monitor_misc_signal(atpg_TxDllClk_vmc)
   `evl_monitor_misc_signal(atpg_Pclk_vmc)
   `evl_monitor_misc_signal(atpg_RDQSClk_vmc)

   //
   // Performance monitoring
   //
   initial begin
      int dreams;

      if (!`et_value_plusargs("DREAMS=%d", dreams)) begin
         dreams = 0;
      end
      if (dreams != 0) begin
         forever @(negedge clk__noc) begin
            $timeformat(-9, 3, "", 8);
            if (mem_mesh_slave_axi_ARVALID) begin
               $display("EVENT %9.0f 400 %d %h %d %h", $floor($realtime + 0.25), MEMSHIRE_ID, mem_mesh_slave_axi_AR.ADDR, mem_mesh_slave_axi_ARREADY, mem_mesh_slave_axi_AR.ID);
            end
            if (mem_mesh_slave_axi_RVALID) begin
               $display("EVENT %9.0f 401 %d %d %h", $floor($realtime + 0.25), MEMSHIRE_ID, mem_mesh_slave_axi_RREADY, mem_mesh_slave_axi_R.ID);
            end
            if (mem_mesh_slave_axi_AWVALID) begin
               $display("EVENT %9.0f 402 %d %h %d %h", $floor($realtime + 0.25), MEMSHIRE_ID, mem_mesh_slave_axi_AW.ADDR, mem_mesh_slave_axi_AWREADY, mem_mesh_slave_axi_AW.ID);
            end
            if (mem_mesh_slave_axi_BVALID) begin
               $display("EVENT %9.0f 403 %d %d %h", $floor($realtime + 0.25), MEMSHIRE_ID, mem_mesh_slave_axi_BREADY, mem_mesh_slave_axi_B.ID);
            end
         end
      end
   end

   generate
      if (SHIRE_STUB == 1'b0) begin :DREAMS
         initial begin
            int dreams;

            if (!`et_value_plusargs("DREAMS=%d", dreams)) begin
               dreams = 0;
            end
            if (dreams != 0) begin
               forever @(negedge clock) begin
                  if (memshire_top.memshire_top.u0_umctl2_arvalid_0_vmc) begin
                     $display("EVENT %d 404 %d %h %d %h", $time, {MEMSHIRE_ID, 1'b0}, memshire_top.memshire_top.u0_umctl2_ar_0_vmc.ADDR, memshire_top.memshire_top.u0_umctl2_arready_0_vmc, memshire_top.memshire_top.u0_umctl2_ar_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u0_umctl2_rvalid_0_vmc) begin
                     $display("EVENT %d 405 %d %d %h", $time, {MEMSHIRE_ID, 1'b0}, memshire_top.memshire_top.u0_umctl2_rready_0_vmc, memshire_top.memshire_top.u0_umctl2_r_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u0_umctl2_awvalid_0_vmc) begin
                     $display("EVENT %d 406 %d %h %d %h", $time, {MEMSHIRE_ID, 1'b0}, memshire_top.memshire_top.u0_umctl2_aw_0_vmc.ADDR, memshire_top.memshire_top.u0_umctl2_awready_0_vmc, memshire_top.memshire_top.u0_umctl2_aw_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u0_umctl2_bvalid_0_vmc) begin
                     $display("EVENT %d 407 %d %d %h", $time, {MEMSHIRE_ID, 1'b0}, memshire_top.memshire_top.u0_umctl2_bready_0_vmc, memshire_top.memshire_top.u0_umctl2_b_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u1_umctl2_arvalid_0_vmc) begin
                     $display("EVENT %d 404 %d %h %d %h", $time, {MEMSHIRE_ID, 1'b1}, memshire_top.memshire_top.u1_umctl2_ar_0_vmc.ADDR, memshire_top.memshire_top.u1_umctl2_arready_0_vmc, memshire_top.memshire_top.u1_umctl2_ar_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u1_umctl2_rvalid_0_vmc) begin
                     $display("EVENT %d 405 %d %d %h", $time, {MEMSHIRE_ID, 1'b1}, memshire_top.memshire_top.u1_umctl2_rready_0_vmc, memshire_top.memshire_top.u1_umctl2_r_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u1_umctl2_awvalid_0_vmc) begin
                     $display("EVENT %d 406 %d %h %d %h", $time, {MEMSHIRE_ID, 1'b1}, memshire_top.memshire_top.u1_umctl2_aw_0_vmc.ADDR, memshire_top.memshire_top.u1_umctl2_awready_0_vmc, memshire_top.memshire_top.u1_umctl2_aw_0_vmc.ID);
                  end
                  if (memshire_top.memshire_top.u1_umctl2_bvalid_0_vmc) begin
                     $display("EVENT %d 407 %d %d %h", $time, {MEMSHIRE_ID, 1'b1}, memshire_top.memshire_top.u1_umctl2_bready_0_vmc, memshire_top.memshire_top.u1_umctl2_b_0_vmc.ID);
                  end
               end
            end
         end
      end
   endgenerate

   assign ms_auto_init_config[15:15] = (MEMSHIRE_ID[1:0] == 2'b00) ? 1'b1 : 1'b0;

endmodule

`include "soc.vh"

// This is a wrapper to instantiate RTL shire_top or Netlist shire_top
// Parameters and ports must be identical to those of shire_top

module shire_top_gate_wrap
   #(
        // Start of simulation parameters: should all default to zero but DV_NO_OBJECTION
        parameter int            SHIRE_INST_ID              = 0,
        parameter bit            NOC_STUB                   = 1'b0,
        parameter bit            DV_NO_OBJECTION            = 1'b1,
        parameter bit            CAPTURE_DPI_EVENTS         = 1'b0,
        parameter bit            MINION_SHIRE_STIM          = 1'b0,
        parameter bit            MINION_SHIRE_STUB          = 1'b0,
        parameter bit            MINION_SHIRE_LITE          = 1'b0,
        parameter bit            MINION_SHIRE_CACHE_STUB    = 1'b0,
        parameter bit            MINION_SHIRE_CHANNEL_STUB  = 1'b0,
        parameter bit            MINION_SHIRE_CHANNEL_LITE  = 1'b0,
        parameter bit [7:0]      MINION_SHIRE_NEIGH_DISABLE = {8{1'b0}},  // Do not instantiate a neighborhood and lock down some wires
        parameter bit            DV_GATESIM_ENABLE          = 1'b0,

        parameter bit [7:0]      MINION_SHIRE_NEIGH_STUB    = {8{1'b0}},  // Do not instantiate a neighborhood and allow wires to float
        parameter bit [7:0][7:0] MINION_DISABLE             = {64{1'b0}}, // Do not instantiate a Minion and lock down some wires
        parameter bit [7:0][7:0] MINION_STUB                = {64{1'b0}}, // Do not instantiate a Minion and allow wires to float
        // End of simulation parameters
        parameter int            NUM_NEIGH                  = `NUM_NEIGH,
        parameter int            NUM_BANKS                  = `SC_BANKS,
        parameter int            NUM_L3_MASTER_PORTS        = `SC_L3_MASTER_PORTS,
        parameter int            NUM_L3_SLAVE_PORTS         = `SC_L3_SLAVE_PORTS,
        parameter int            NUM_RBOX                   = `RBOX_PER_SHIRE,
        parameter int            NUM_UC                     = 1
    )
(
    // System inputs
    input logic                                               reset_cold,
    input logic                                               reset_warm,
    input logic                                               ref_clock,    // 100 Mhz Clock
    input logic                                               step_clock,   // 1 Ghz Clock
    input [`NUM_SHIRE_IDS_R]                                  shire_id_reset_val,
    input [`NUM_SHIRES_R]                                     shire_phy_id,

    `ifdef ET_AXI_DPI_MONITOR
      input  wire                                             dpi_capture_enable,
      input  wire                                             dpi_read_clock,
      input  wire                                             dpi_packet_pop,
      output wire                                             dpi_packet_valid,
      output wire [`ET_DPI_PKT_MSB:0]                         dpi_packet_data,
    `endif // ifdef ET_AXI_DPI_MONITOR

    // DFT ports for all partitions
    input  logic [`SHIRE_DFT_SCANIN_E2W_SHIRE_TOP_WIDTH-1:0]  dft__scanin_e2w,        //Scan In
    input  logic [`SHIRE_DFT_SCANIN_W2E_SHIRE_TOP_WIDTH-1:0]  dft__scanin_w2e,        //Scan In
    output logic [`SHIRE_DFT_SCANOUT_E2W_SHIRE_TOP_WIDTH-1:0] dft__scanout_e2w,       //Scan Out
    output logic [`SHIRE_DFT_SCANOUT_W2E_SHIRE_TOP_WIDTH-1:0] dft__scanout_w2e,       //Scan Out
    input  logic [`SHIRE_DFT_BC_CTL_WIDTH-1:0]                dft__bc_cntl,           //Broadcast Control
    input  logic                                              dft__p2p_cntl,          //Point-to-point control
    input  logic                                              ate_clock_in,           //Common ate_clock
    output logic                                              ate_clock_out,          //Feedtthrough ate_clock


    // ECO ports
    input  logic [`ET_ECO_SHIRE_TOP_INPUT_WIDTH-1:0]          eco_i,
    output logic [`ET_ECO_SHIRE_TOP_OUTPUT_WIDTH-1:0]         eco_o,


    // jtag tdr control
    input  `toVector(et_tdr_cntl_t)                           minshire_tdr_cntl,
    input  logic                                              minshire_tdr_tck_in,
    output logic                                              minshire_tdr_tck_out,
    input  logic                                              minshire_tdr_tdi_e2w,
    input  logic                                              minshire_tdr_tdi_w2e,
    input  logic                                              minshire_tdr_tdi_s2n_fb,
    output logic                                              minshire_tdr_tdo_e2w,
    output logic                                              minshire_tdr_tdo_w2e,
    output logic                                              minshire_tdr_tdo_s2n_fb,

    // PLL debug yin/yang
    input  logic                                              yin_in_e2w,
    input  logic                                              yin_in_w2e,
    output logic                                              yin_out_e2w,
    output logic                                              yin_out_w2e,
    input  logic                                              yang_in_e2w,
    input  logic                                              yang_in_w2e,
    output logic                                              yang_out_e2w,
    output logic                                              yang_out_w2e,

    `ifndef NOC_DISABLE
      //noc stamped
      `include "minionshire_ns_meshtop_ports.v"
      `include "minionshire_ns_debug_meshtop_ports.v"
    `else
       input  logic                                           clk__noc,
    `endif // ifdef NOC_DISABLE

    // ESR
    input  `toVector(esr_ms_dmctrl_t)                         dmctrl_in,
    input  `toVector(esr_and_or_tree_L2_t)                    debug_and_or_tree_L2_in_e2w,
    input  `toVector(esr_and_or_tree_L2_t)                    debug_and_or_tree_L2_in_w2e,
    output `toVector(esr_and_or_tree_L2_t)                    debug_and_or_tree_L2_out_e2w,
    output `toVector(esr_and_or_tree_L2_t)                    debug_and_or_tree_L2_out_w2e,

    // Connection with debug noc bridge
    input   logic   [`DBG_NOC_BRIDGE_EVENT_BITS - 1:0]        dbg_noc_bridge_event_dest,
    input   logic   [`US_INDEX_LENGTH - 1: 0]                 dbg_noc_bridge_index_adjust_ip,

    //Test mode for moortec sensor IP

    //Sensors:  Temperature
    input   logic                                             clk_ts,
    input   logic                                             rstn_ts,
    input   logic                                             ts_sdi,
    output  logic                                             ts_sdo,
    inout   wire       [1:0]                                  ts_an_io,      // TS Analog access bus

    //Sensors:  Process
    input   logic                                             clk_pd,
    input   logic                                             rstn_pd,
    input   logic                                             pd_sdi,
    output  logic                                             pd_sdo,
    inout   wire       [1:0]                                  pd_an_io,      // PD Analog access bus

    //PLIC external ints
    input   logic                                             plic_mtip,
    input   logic                                             plic_meip,
    input   logic                                             plic_seip,

    input   logic                                             msg_lock_enable_riscv,
    input   logic                                             msg_lock_enable_esperanto,

    //Interrupt to ioshire
    output  logic                                             ioshire_combined_err_int,
    output  logic                                             ioshire_noc_err_int,

    // Voltage Monitor connections
    output wire [`SOCTOP_VM_MINSHIRE_POINTS-1:0]              minionshire_vin_hi,
    output wire [`SOCTOP_VM_MINSHIRE_POINTS-1:0]              minionshire_vin_lo,

    //SMS signals
    input  logic                                              dft__qualified_mbist_en,
    input  logic                                              vl_sms_shire_top_wrck_in,
    output logic                                              vl_sms_shire_top_wrck_out,
    input  logic                                              vl_sms_shire_top_wrstn,
    input  logic                                              vl_sms_shire_top_rst,
    input  logic [`SHIRE_SMS_FB_CHAINS-1:0]                   vl_srv_u_sms_shire_top_wsi_n2s_ff,
    input  logic                                              vl_srv_u_sms_shire_top_wsi_e2w,
    input  logic                                              vl_srv_u_sms_shire_top_wsi_w2e_fb,
    input  logic [`SHIRE_SMS_FB_CHAINS-1:0]                   vl_srv_u_sms_shire_top_wsi_s2n_fb,
    input  logic                                              vl_srv_u_sms_shire_top_selectwir,
    input  logic                                              vl_srv_u_sms_shire_top_capturewr,
    input  logic                                              vl_srv_u_sms_shire_top_shiftwr,
    input  logic                                              vl_srv_u_sms_shire_top_updatewr,
    output logic [`SHIRE_SMS_FB_CHAINS-1:0]                   vl_srv_u_sms_shire_top_wso_n2s_ff,
    output logic                                              vl_srv_u_sms_shire_top_wso_e2w,
    output logic                                              vl_srv_u_sms_shire_top_wso_w2e_fb,
    output logic [`SHIRE_SMS_FB_CHAINS-1:0]                   vl_srv_u_sms_shire_top_wso_s2n_fb,
    input  logic                                              dm0_et_blocks,
    input  logic                                              dm1_et_blocks,
    input  logic                                              dm2_et_blocks

);

generate
  if (DV_GATESIM_ENABLE == 1'b0)
  begin : shire_top
    shire_top
      `ifdef USE_ET_PARAMS_IN_BUILD
      #( .SHIRE_INST_ID             (SHIRE_INST_ID				    ),
         .NOC_STUB                  (NOC_STUB                   ),
         .DV_NO_OBJECTION           (DV_NO_OBJECTION            ),
         .CAPTURE_DPI_EVENTS        (CAPTURE_DPI_EVENTS         ),
         .MINION_SHIRE_STIM         (MINION_SHIRE_STIM          ),
         .MINION_SHIRE_STUB         (MINION_SHIRE_STUB          ),
         .MINION_SHIRE_LITE         (MINION_SHIRE_LITE          ),
         .MINION_SHIRE_CACHE_STUB   (MINION_SHIRE_CACHE_STUB    ),
         .MINION_SHIRE_CHANNEL_STUB (MINION_SHIRE_CHANNEL_STUB  ),
         .MINION_SHIRE_CHANNEL_LITE (MINION_SHIRE_CHANNEL_LITE  ),
         .MINION_SHIRE_NEIGH_DISABLE(MINION_SHIRE_NEIGH_DISABLE ),

         .MINION_SHIRE_NEIGH_STUB   (MINION_SHIRE_NEIGH_STUB    ),
         .MINION_DISABLE            (MINION_DISABLE             ),
         .MINION_STUB               (MINION_STUB                ),

         .NUM_NEIGH                 (NUM_NEIGH                  ),
         .NUM_BANKS                 (NUM_BANKS                  ),
         .NUM_L3_MASTER_PORTS       (NUM_L3_MASTER_PORTS        ),
         .NUM_L3_SLAVE_PORTS        (NUM_L3_SLAVE_PORTS         ),
         .NUM_RBOX                  (NUM_RBOX                   ),
         .NUM_UC                    (NUM_UC                     )
      )
      `endif
    shire_top(.*);
  end else
  begin : shire_top
    shire_top_gate shire_top(.*);

  end
endgenerate

endmodule

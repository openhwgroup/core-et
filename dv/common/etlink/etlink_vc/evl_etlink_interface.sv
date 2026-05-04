//--------------------------------------------------------------------------------------------------
//
//
// Interface: evl_etlink_interface
//
`include "rtl/inc/soc.vh"

`ifndef EVL_IFC_PORT
   `ifdef EVL_SIMULATION
      `define EVL_IFC_PORT inout
   `else
      `define EVL_IFC_PORT input
   `endif
`endif

`include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"

`ifndef ET_MONITOR_IFCS
   `ifdef EVL_SIMULATION
      `define ET_MONITOR_IFCS 1
   `else
      `ifdef ET_IP_SIMULATION
         `define ET_MONITOR_IFCS 1
      `endif
   `endif
`endif

`ifdef EVL_SIMULATION
   `ifdef ET_INCLUDE_USR_PORT
      `define ET_ETLINK_INCLUDE_USR_PORT
   `endif
`endif

module evl_etlink_interface #(parameter int    AGENT_ID            = 0,
                              parameter string ABSTRACT_NAME       = "ETLink",
                              parameter bit    USE_BANKS           = 1'b1,
                              parameter int    NUM_BANKS           = 1,
                              parameter int    NUM_UC              = 0,
                              parameter int    ETLINK_COV_TYPE     = 0,
                              parameter type   REQ_TYPE            = et_link_req_info_t,
                              parameter type   RSP_TYPE            = et_link_rsp_info_t,
                              parameter int    IGNORE_SRC_DST      = 0,
                              parameter int    IGNORE_MESSAGE      = 0,
                              parameter int    IGNORE_DST_RSP      = 0,
                              parameter int    DELAY_REQ_ZTIME     = 0,
                              parameter int    DELAY_RSP_ZTIME     = 0,
                              parameter int    SKIP_LEVELS[]       = {0},
                              parameter int    IGNORED_RSP_IDS[]   = {}) ( // Disable response checking for specific IDs (used by Minion ETLink)
      input         wire                          isolate,
      input         wire                          reset,
      input         wire                          clk,

      // request signals
      `EVL_IFC_PORT wire [(NUM_BANKS+NUM_UC-1):0] req_valid,
      `EVL_IFC_PORT wire [(NUM_BANKS+NUM_UC-1):0] req_ready,
      `EVL_IFC_PORT wire REQ_TYPE                 req_info,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0]    req_trans_id,

      // response signals
      `EVL_IFC_PORT wire                          rsp_valid,
      `EVL_IFC_PORT wire                          rsp_ready,
      `EVL_IFC_PORT wire RSP_TYPE                 rsp_info,

      input         wire                          rsp_source_is_uc,
      input         wire                          use_rsp_source_is_uc
   );

   import evl_base_pkg::*;

   localparam int    ALL_PORTS = (USE_BANKS == 1'b0) ? 1 : NUM_BANKS + NUM_UC;
   localparam [10:0] PORT_MASK = (1 << ALL_PORTS) - 1;

   //-----------------------------------------------------------------------------------------------
   //
   // Monitor clock & reset
   //
   int        m_ifc_cycle_count  = 0;
   bit        m_start_watching   = 1'b0;
   bit        m_reset_asserted   = 1'b0;
   bit        m_reset_deasserted = 1'b0;
   bit        m_power_on         = 1'b0;
   bit        m_power_off        = 1'b0;
   bit        m_req_drive        = 1'b0;
   bit        m_rsp_drive        = 1'b0;
   wire       m_clock            = clk & m_start_watching;

   wire [9:0] m_req_valid_mon;
   wire [9:0] m_req_ready_mon;
   wire [9:0] m_req_select_mon;
   wire [3:0] m_req_bank_mon;

   assign m_req_valid_mon  = { {(10-ALL_PORTS){1'b0}}, req_valid[ALL_PORTS-1:0] } & {ALL_PORTS{m_reset_deasserted}} & {ALL_PORTS{m_power_on}};
   assign m_req_ready_mon  = { {(10-ALL_PORTS){1'b0}}, req_ready[ALL_PORTS-1:0] };
   assign m_req_select_mon = m_req_valid_mon & m_req_ready_mon;
   assign m_req_bank_mon   = ( (m_req_select_mon[0:0] === 1'b1) ? 4'h0 :
                               (m_req_select_mon[1:1] === 1'b1) ? 4'h1 :
                               (m_req_select_mon[2:2] === 1'b1) ? 4'h2 :
                               (m_req_select_mon[3:3] === 1'b1) ? 4'h3 :
                               (m_req_select_mon[4:4] === 1'b1) ? 4'h4 :
                               (m_req_select_mon[5:5] === 1'b1) ? 4'h5 :
                               (m_req_select_mon[6:6] === 1'b1) ? 4'h6 :
                               (m_req_select_mon[7:7] === 1'b1) ? 4'h7 :
                               (m_req_select_mon[8:8] === 1'b1) ? 4'h8 :
                               (m_req_select_mon[9:9] === 1'b1) ? 4'h9 : 4'hf );


   `ifdef EVL_SIMULATION // -----------------------------------------------------------------------{

      import evl_etlink_cov_pkg::*;
      import evl_etlink_pkg::evl_etlink_rtl_port;


      //
      // The following block of logic is used to drive requests and responses from EVL agents.
      //
      localparam int REQ_INFO_ID_WIDTH        = $bits(req_info.id);
      localparam int REQ_INFO_SOURCE_WIDTH    = $bits(req_info.source);
      localparam int REQ_INFO_WDATA_WIDTH     = $bits(req_info.wdata);
      localparam int REQ_INFO_OPCODE_WIDTH    = $bits(req_info.opcode);
      localparam int REQ_INFO_ADDRESS_WIDTH   = $bits(req_info.address);
      localparam int REQ_INFO_DATA_WIDTH      = $bits(req_info.data);
      localparam int REQ_INFO_SIZE_WIDTH      = $bits(req_info.size);
      localparam int REQ_INFO_QWEN_WIDTH      = $bits(req_info.qwen);
      localparam int REQ_INFO_SUBOPCODE_WIDTH = $bits(req_info.subopcode);

      localparam int RSP_INFO_ID_WIDTH        = $bits(rsp_info.id);
      localparam int RSP_INFO_DEST_WIDTH      = $bits(rsp_info.dest);
      localparam int RSP_INFO_WDATA_WIDTH     = $bits(rsp_info.wdata);
      localparam int RSP_INFO_OPCODE_WIDTH    = $bits(rsp_info.opcode);
      localparam int RSP_INFO_DATA_WIDTH      = $bits(rsp_info.data);
      localparam int RSP_INFO_SIZE_WIDTH      = $bits(rsp_info.size);
      localparam int RSP_INFO_QWEN_WIDTH      = $bits(rsp_info.qwen);

      `evl_param_key(pk_ignore_dst_rsp)
      `evl_param_key(pk_ignore_message)
      `evl_param_key(pk_ignore_src_dst)
      `evl_param_key(pk_num_banks)
      `evl_param_key(pk_rdata_bytes)
      `evl_param_key(pk_wdata_bytes)

      wire [3:0]                    m_sel_req;

      bit                           m_selected          = 1'b0;
      reg  [9:0]                    m_req_valid_pre     = {10{1'b0}};
      reg  [9:0]                    m_req_valid_pend    = {10{1'b0}};
      reg  [9:0]                    m_req_valid_drv     = {10{1'b0}};
      reg  [9:0]                    m_req_ready_rr_mask = 10'b0111111111 & PORT_MASK[9:0];
      reg  [9:0]                    m_req_valid_sel1;
      reg  [9:0]                    m_req_valid_sel2;
      wire [9:0]                    m_req_ready_drv;
      reg  [9:0]                    m_req_ready_backp;
      wire [9:0]                    m_req_select_drv;
      wire [9:0]                    m_req_valid_rr1;
      wire [9:0]                    m_req_valid_rr2;
      reg  [`EVL_TRANS_ID_MSB:0]    m_req_trans_id_drv[10];
      et_link_req_info_t            m_req_info_drv[10];

      reg                           m_use_one_hot_drv   = 1'b0;
      reg                           m_use_one_hot_rdy   = 1'b1;
      reg                           m_rsp_valid_drv     = 1'b0;
      reg                           m_rsp_ready_drv_pre = 1'b1;
      wire                          m_rsp_ready_drv;
      reg                           m_rsp_ready_backp;
      et_link_rsp_info_t            m_rsp_info_drv      = et_link_rsp_info_t'(0);

      int                           pk_req_backp_assert_key;
      int                           pk_req_backp_deassert_key;
      int                           pk_rsp_backp_assert_key;
      int                           pk_rsp_backp_deassert_key;


      assign m_sel_req        = ( (m_req_drive           !== 1'b1) ? 4'h0 :
                                  (m_req_select_drv[0:0] === 1'b1) ? 4'h0 :
                                  (m_req_select_drv[1:1] === 1'b1) ? 4'h1 :
                                  (m_req_select_drv[2:2] === 1'b1) ? 4'h2 :
                                  (m_req_select_drv[3:3] === 1'b1) ? 4'h3 :
                                  (m_req_select_drv[4:4] === 1'b1) ? 4'h4 :
                                  (m_req_select_drv[5:5] === 1'b1) ? 4'h5 :
                                  (m_req_select_drv[6:6] === 1'b1) ? 4'h6 :
                                  (m_req_select_drv[7:7] === 1'b1) ? 4'h7 :
                                  (m_req_select_drv[8:8] === 1'b1) ? 4'h8 : 4'h0 );

      assign m_req_select_drv = m_req_valid_drv & m_req_ready_mon;

      assign m_req_valid_rr1  =  m_req_ready_rr_mask & m_req_valid_mon & PORT_MASK[9:0];
      assign m_req_valid_rr2  = ~m_req_ready_rr_mask & m_req_valid_mon & PORT_MASK[9:0];

      assign m_req_ready_drv  = m_req_ready_backp &
                              ( (m_use_one_hot_rdy    === 1'b0) ? PORT_MASK[9:0] :
                                (m_req_valid_rr1[0:0] === 1'b1) ? 10'b0000000001 :
                                (m_req_valid_rr1[1:1] === 1'b1) ? 10'b0000000010 :
                                (m_req_valid_rr1[2:2] === 1'b1) ? 10'b0000000100 :
                                (m_req_valid_rr1[3:3] === 1'b1) ? 10'b0000001000 :
                                (m_req_valid_rr1[4:4] === 1'b1) ? 10'b0000010000 :
                                (m_req_valid_rr1[5:5] === 1'b1) ? 10'b0000100000 :
                                (m_req_valid_rr1[6:6] === 1'b1) ? 10'b0001000000 :
                                (m_req_valid_rr1[7:7] === 1'b1) ? 10'b0010000000 :
                                (m_req_valid_rr1[8:8] === 1'b1) ? 10'b0100000000 :
                                (m_req_valid_rr1[9:9] === 1'b1) ? 10'b1000000000 :
                                (m_req_valid_rr2[0:0] === 1'b1) ? 10'b0000000001 :
                                (m_req_valid_rr2[1:1] === 1'b1) ? 10'b0000000010 :
                                (m_req_valid_rr2[2:2] === 1'b1) ? 10'b0000000100 :
                                (m_req_valid_rr2[3:3] === 1'b1) ? 10'b0000001000 :
                                (m_req_valid_rr2[4:4] === 1'b1) ? 10'b0000010000 :
                                (m_req_valid_rr2[5:5] === 1'b1) ? 10'b0000100000 :
                                (m_req_valid_rr2[6:6] === 1'b1) ? 10'b0001000000 :
                                (m_req_valid_rr2[7:7] === 1'b1) ? 10'b0010000000 :
                                (m_req_valid_rr2[8:8] === 1'b1) ? 10'b0100000000 :
                                (m_req_valid_rr2[9:9] === 1'b1) ? 10'b1000000000 : 10'b0000000000 );

      assign m_rsp_ready_drv  = m_rsp_ready_drv_pre & m_rsp_ready_backp;


      //--------------------------------------------------------------------------------------------
      //
      // Drive the request pins if this is an ET Link requester.
      //
      assign req_valid                                  = ((m_req_drive !== 1'b1) ? {ALL_PORTS{1'bz}} : m_req_valid_drv[ALL_PORTS-1:0]);
      assign req_ready                                  = ((m_rsp_drive !== 1'b1) ? {ALL_PORTS{1'bz}} : m_req_ready_drv[ALL_PORTS-1:0]);

      assign req_info.id[REQ_INFO_ID_WIDTH-1:0]         = ((m_req_drive !== 1'b1) ? {REQ_INFO_ID_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].id[REQ_INFO_ID_WIDTH-1:0]);
      assign req_info.source[REQ_INFO_SOURCE_WIDTH-1:0] = ((m_req_drive !== 1'b1) ? {REQ_INFO_SOURCE_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].source[REQ_INFO_SOURCE_WIDTH-1:0]);
      assign req_info.wdata                             = ((m_req_drive !== 1'b1) ? {REQ_INFO_WDATA_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].wdata);
      assign req_info.opcode                            = ((m_req_drive !== 1'b1) ? {REQ_INFO_OPCODE_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].opcode[REQ_INFO_OPCODE_WIDTH-1:0]);
      assign req_info.address                           = ((m_req_drive !== 1'b1) ? {REQ_INFO_ADDRESS_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].address[REQ_INFO_ADDRESS_WIDTH-1:0]);
      assign req_info.data[REQ_INFO_DATA_WIDTH-1:0]     = ((m_req_drive !== 1'b1) ? {REQ_INFO_DATA_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].data[REQ_INFO_DATA_WIDTH-1:0]);
      assign req_info.size                              = ((m_req_drive !== 1'b1) ? {REQ_INFO_SIZE_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].size[REQ_INFO_SIZE_WIDTH-1:0]);
      assign req_info.qwen                              = ((m_req_drive !== 1'b1) ? {REQ_INFO_QWEN_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].qwen[REQ_INFO_QWEN_WIDTH-1:0]);
      assign req_info.subopcode                         = ((m_req_drive !== 1'b1) ? {REQ_INFO_SUBOPCODE_WIDTH{1'bz}} :
                                                                                  m_req_info_drv[m_sel_req].subopcode[REQ_INFO_SUBOPCODE_WIDTH-1:0]);

      assign req_trans_id                               = ((m_req_drive !== 1'b1) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} :
                                                                                  m_req_trans_id_drv[m_sel_req]);

      //--------------------------------------------------------------------------------------------
      //
      // Drive the request pins if this is an ET Link responder.
      //
      assign rsp_valid                                  = ((m_rsp_drive !== 1'b1) ? 1'bz :
                                                                                  m_rsp_valid_drv);
      assign rsp_ready                                  = ((m_req_drive !== 1'b1) ? 1'bz :
                                                                                  m_rsp_ready_drv);

      assign rsp_info.id                                = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_ID_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.id[RSP_INFO_ID_WIDTH-1:0]);
      assign rsp_info.dest[RSP_INFO_DEST_WIDTH-1:0]     = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_DEST_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.dest[RSP_INFO_DEST_WIDTH-1:0]);
      assign rsp_info.wdata                             = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_WDATA_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.wdata);
      assign rsp_info.opcode                            = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_OPCODE_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.opcode[RSP_INFO_OPCODE_WIDTH-1:0]);
      assign rsp_info.data[RSP_INFO_DATA_WIDTH-1:0]     = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_DATA_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.data[RSP_INFO_DATA_WIDTH-1:0]);
      assign rsp_info.size                              = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_SIZE_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.size[RSP_INFO_SIZE_WIDTH-1:0]);
      assign rsp_info.qwen                              = ((m_rsp_drive !== 1'b1) ? {RSP_INFO_QWEN_WIDTH{1'bz}} :
                                                                                  m_rsp_info_drv.qwen[RSP_INFO_QWEN_WIDTH-1:0]);


      bit                 m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), ETLINK_BUS_AGENT, AGENT_ID, "evl_etlink_verif_comp", "", "", "", { SKIP_LEVELS });
      evl_etlink_rtl_port m_rtl_port    = new($sformatf("%m"), AGENT_ID, { SKIP_LEVELS });


      initial begin
         pk_req_backp_assert_key   = evl_param_xref::get_param_key("pk_etlink_req_ready_assert_count");
         pk_req_backp_deassert_key = evl_param_xref::get_param_key("pk_etlink_req_ready_deassert_count");
         pk_rsp_backp_assert_key   = evl_param_xref::get_param_key("pk_etlink_rsp_ready_assert_count");
         pk_rsp_backp_deassert_key = evl_param_xref::get_param_key("pk_etlink_rsp_ready_deassert_count");

         m_rtl_port.set_int_param(pk_wdata_bytes, $bits(req_info.data) / 8);
         m_rtl_port.set_int_param(pk_rdata_bytes, $bits(rsp_info.data) / 8);
         m_rtl_port.set_int_param(pk_ignore_src_dst, IGNORE_SRC_DST);
         m_rtl_port.set_int_param(pk_ignore_message, IGNORE_MESSAGE);
         m_rtl_port.set_int_param(pk_ignore_dst_rsp, IGNORE_DST_RSP);
         m_rtl_port.m_req_data_bus_width = $bits(req_info.data) / 8;
         m_rtl_port.m_rsp_data_bus_width = $bits(rsp_info.data) / 8;
         foreach (IGNORED_RSP_IDS[ii]) begin
            m_rtl_port.add_ignored_rsp_id(IGNORED_RSP_IDS[ii]);
         end

         m_use_one_hot_drv = 1'b1;
         m_use_one_hot_rdy = 1'b0;

         if (USE_BANKS == 0) begin
            m_rtl_port.set_int_param(pk_num_banks, 1);
         end
         else begin
            m_rtl_port.set_int_param(pk_num_banks, NUM_BANKS);
         end
      end

      //
      // Request backpressure generation
      //
      initial begin
         m_req_ready_backp <= `ET_TB_HOLD 10'b1111111111;
         wait (m_rsp_drive === 1'b1);
         wait (reset === 1'b0);
         forever begin
            #0;
            // pk_req_backp_assert_key is a delay key that contains the number of cycles to keep req_ready asserted.
            `evl_log(`UVM_FULL, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("asserting req_ready"))
            m_req_ready_backp <= `ET_TB_HOLD 10'b1111111111;
            m_rtl_port.wait_to_assert_backp(pk_req_backp_assert_key);
            #0;
            // pk_req_backp_deassert_key is a delay key that contains the number of cycles to keep req_ready deasserted.
            `evl_log(`UVM_FULL, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("deasserting req_ready"))
            m_req_ready_backp <= `ET_TB_HOLD 10'b0000000000;
            m_rtl_port.wait_to_deassert_backp(pk_req_backp_deassert_key);
         end
      end

      //
      // Response backpressure generation
      //
      initial begin
         m_rsp_ready_backp <= `ET_TB_HOLD 1'b1;
         wait (m_req_drive == 1'b1);
         wait (reset === 1'b0);
         forever begin
            #0;
            // pk_rsp_backp_assert_key is a delay key that contains the number of cycles to keep rsp_ready asserted.
            `evl_log(`UVM_FULL, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("asserting rsp_ready"))
            m_rsp_ready_backp <= `ET_TB_HOLD 1'b1;
            m_rtl_port.wait_to_assert_backp(pk_rsp_backp_assert_key);
            #0;
            // pk_rsp_backp_deassert_key is a delay key that contains the number of cycles to keep rsp_ready deasserted.
            `evl_log(`UVM_FULL, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("deasserting rsp_ready"))
            m_rsp_ready_backp <= `ET_TB_HOLD 1'b0;
            m_rtl_port.wait_to_deassert_backp(pk_rsp_backp_deassert_key);
         end
      end

      initial begin
         #0 wait (m_start_watching === 1'b1);
         m_rtl_port.set_port_count(ALL_PORTS);
         m_rtl_port.get_req_drive(m_req_drive);
         m_rtl_port.get_rsp_drive(m_rsp_drive);
      end

      initial begin
         bit err_on_clock_x;

         wait (m_reset_asserted == 1'b1);
         wait (m_reset_deasserted == 1'b1);
         err_on_clock_x = (`et_test_plusargs("err_on_clock_x") != 0) ? 1'b1 : 1'b0;
         forever begin
            //
            // Wait for clock to initially go to a non-X value.
            //
            wait ((m_clock === 1'b0) || (m_clock === 1'b1));

            //
            // Wait for clock go to X (as long as isolate is not asserted).
            //
            wait ((m_clock !== 1'b0) && (m_clock !== 1'b1) && (isolate !== 1'b1));
            if (err_on_clock_x == 1'b1) begin
               `dut_error($sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("input clock is %b ... forcing clocks", m_clock))
            end
            else begin
               `dut_warning($sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("input clock is %b ... forcing clocks", m_clock))
            end
            while ((m_clock !== 1'b0) && (m_clock !== 1'b1)) begin
               #10ns;
               m_ifc_cycle_count++;
               m_rtl_port.ping_clock();
            end
         end
      end

      task drive_reset();
         m_req_ready_rr_mask <= PORT_MASK[9:0];
         m_req_valid_pre     = {10{1'b0}};
         m_req_valid_drv     = {10{1'b0}};
         m_req_valid_pend    = {10{1'b0}};
         m_req_valid_sel1    = {10{1'b0}};
         m_req_valid_sel2    = {10{1'b0}};
         return;
      endtask : drive_reset

      task pre_drive_request();
         m_req_valid_pend = m_req_valid_pre & ~(m_req_ready_mon & m_req_valid_mon);
      endtask : pre_drive_request

      task drive_request();
         m_selected = 1'b0;
         if ((m_req_valid_pre & PORT_MASK[9:0]) !== {10{1'b0}}) begin
            `evl_log(`UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("req_valid:0x%x, req_ready:0x%x, pending requests:0x%x", m_req_valid_mon, m_req_ready_mon, m_req_valid_pre))
         end
         if ((m_req_valid_pre & m_req_ready_mon & m_req_valid_mon & PORT_MASK[9:0]) !== {10{1'b0}}) begin
            m_selected = 1'b1;
            if (m_use_one_hot_drv === 1'b1) begin
               //
               // If a pending request has been sent and accepted, change the arbitration priority.
               //
               if (((m_req_ready_rr_mask << 1) & PORT_MASK[9:0]) === {10{1'b0}}) begin
                  m_req_ready_rr_mask <= `ET_TB_HOLD PORT_MASK[9:0];
               end
               else begin
                  m_req_ready_rr_mask <= `ET_TB_HOLD (m_req_ready_rr_mask << 1) & PORT_MASK[9:0];
               end
            end
         end
         else if ((m_req_valid_pre & m_req_ready_mon) === {10{1'b0}}) begin
            m_selected = 1'b1;
         end
         @(negedge m_clock);
         m_rtl_port.get_rsp_ready(m_rsp_ready_drv_pre);
         m_req_valid_pre = m_req_valid_pend;
         if (m_req_valid_pend[0:0] === 1'b0) begin
            m_rtl_port.get_req(0, m_req_valid_pre[0:0], m_req_info_drv[0], m_req_trans_id_drv[0]);
         end
         if ((ALL_PORTS > 0) && m_req_valid_pend[1:1] === 1'b0) begin
            m_rtl_port.get_req(1, m_req_valid_pre[1:1], m_req_info_drv[1], m_req_trans_id_drv[1]);
         end
         if ((ALL_PORTS > 1) && (m_req_valid_pend[2:2] === 1'b0)) begin
            m_rtl_port.get_req(2, m_req_valid_pre[2:2], m_req_info_drv[2], m_req_trans_id_drv[2]);
         end
         if ((ALL_PORTS > 2) && (m_req_valid_pend[3:3] === 1'b0)) begin
            m_rtl_port.get_req(3, m_req_valid_pre[3:3], m_req_info_drv[3], m_req_trans_id_drv[3]);
         end
         if ((ALL_PORTS > 3) && (m_req_valid_pend[4:4] === 1'b0)) begin
            m_rtl_port.get_req(4, m_req_valid_pre[4:4], m_req_info_drv[4], m_req_trans_id_drv[4]);
         end
         if ((ALL_PORTS > 4) && (m_req_valid_pend[5:5] === 1'b0)) begin
            m_rtl_port.get_req(5, m_req_valid_pre[5:5], m_req_info_drv[5], m_req_trans_id_drv[5]);
         end
         if ((ALL_PORTS > 5) && (m_req_valid_pend[6:6] === 1'b0)) begin
            m_rtl_port.get_req(6, m_req_valid_pre[6:6], m_req_info_drv[6], m_req_trans_id_drv[6]);
         end
         if ((ALL_PORTS > 6) && (m_req_valid_pend[7:7] === 1'b0)) begin
            m_rtl_port.get_req(7, m_req_valid_pre[7:7], m_req_info_drv[7], m_req_trans_id_drv[7]);
         end
         if ((ALL_PORTS > 7) && (m_req_valid_pend[8:8] === 1'b0)) begin
            m_rtl_port.get_req(8, m_req_valid_pre[8:8], m_req_info_drv[8], m_req_trans_id_drv[8]);
         end
         if (m_use_one_hot_drv === 1'b0) begin
            m_req_valid_drv = m_req_valid_pre;
         end
         else if (m_selected === 1'b1) begin
            m_req_valid_sel1 =  m_req_ready_rr_mask & PORT_MASK[9:0] & m_req_valid_pre & m_req_ready_mon;
            m_req_valid_sel2 = ~m_req_ready_rr_mask & PORT_MASK[9:0] & m_req_valid_pre & m_req_ready_mon;
            if (m_req_valid_sel1[0:0] === 1'b1) begin
               m_req_valid_drv = 10'b0000000001;
            end
            else if (m_req_valid_sel1[1:1] === 1'b1) begin
               m_req_valid_drv = 10'b0000000010;
            end
            else if (m_req_valid_sel1[2:2] === 1'b1) begin
               m_req_valid_drv = 10'b0000000100;
            end
            else if (m_req_valid_sel1[3:3] === 1'b1) begin
               m_req_valid_drv = 10'b0000001000;
            end
            else if (m_req_valid_sel1[4:4] === 1'b1) begin
               m_req_valid_drv = 10'b0000010000;
            end
            else if (m_req_valid_sel1[5:5] === 1'b1) begin
               m_req_valid_drv = 10'b0000100000;
            end
            else if (m_req_valid_sel1[6:6] === 1'b1) begin
               m_req_valid_drv = 10'b0001000000;
            end
            else if (m_req_valid_sel1[7:7] === 1'b1) begin
               m_req_valid_drv = 10'b0010000000;
            end
            else if (m_req_valid_sel1[8:8] === 1'b1) begin
               m_req_valid_drv = 10'b0100000000;
            end
            else if (m_req_valid_sel2[0:0] === 1'b1) begin
               m_req_valid_drv = 10'b0000000001;
            end
            else if (m_req_valid_sel2[1:1] === 1'b1) begin
               m_req_valid_drv = 10'b0000000010;
            end
            else if (m_req_valid_sel2[2:2] === 1'b1) begin
               m_req_valid_drv = 10'b0000000100;
            end
            else if (m_req_valid_sel2[3:3] === 1'b1) begin
               m_req_valid_drv = 10'b0000001000;
            end
            else if (m_req_valid_sel2[4:4] === 1'b1) begin
               m_req_valid_drv = 10'b0000010000;
            end
            else if (m_req_valid_sel2[5:5] === 1'b1) begin
               m_req_valid_drv = 10'b0000100000;
            end
            else if (m_req_valid_sel2[6:6] === 1'b1) begin
               m_req_valid_drv = 10'b0001000000;
            end
            else if (m_req_valid_sel2[7:7] === 1'b1) begin
               m_req_valid_drv = 10'b0010000000;
            end
            else if (m_req_valid_sel2[8:8] === 1'b1) begin
               m_req_valid_drv = 10'b0100000000;
            end
            else begin
               m_req_valid_drv = 10'b0000000000;
            end
         end
         if (m_rsp_drive == 1'b1) begin
            // Only get a new response to drive if the previous one got accepted
            if ((m_rsp_valid_drv == 1'b0) || (rsp_ready === 1'b1)) begin
               m_rtl_port.get_rsp(m_rsp_valid_drv, m_rsp_info_drv);
            end
         end
      endtask : drive_request

      task drive_response();
         @(negedge m_clock);
         // Only get a new response to drive if the previous one got accepted
         if ((m_rsp_valid_drv == 1'b0) || (rsp_ready === 1'b1)) begin
            m_rtl_port.get_rsp(m_rsp_valid_drv, m_rsp_info_drv);
         end
      endtask : drive_response

      `include "dv/common/cov/etlink_int_ass.svh"

      evl_etlink_cov#(.ETLINK_COV_TYPE(evl_base_pkg::etlink_cov_type_t'(ETLINK_COV_TYPE))) m_etlink_cov;


      initial begin
         int req_idle_count;
         int req_busy_count;
         int rsp_idle_count;
         int rsp_busy_count;
         int cov_type;

         #0 cov_type = ETLINK_COV_TYPE;
         if (cov_type != int'(evl_base_pkg::EVL_COV_ETLINK_NONE)) begin
            m_rtl_port.wait_for_coverage_enabled();
            m_etlink_cov = new();
            req_idle_count = 0;
            req_busy_count = 0;
            rsp_idle_count = 0;
            rsp_busy_count = 0;
            forever @(posedge clk) begin
               if (m_req_valid_mon !== 0) begin
                  if ((m_req_valid_mon & m_req_ready_mon) !== 0) begin
                     m_etlink_cov.sample_req(req_info.address, evl_etlink_types_pkg::evl_etlink_req_opcode_t'(req_info.opcode), req_info.id, req_info.size, req_info.source, req_info.qwen, req_info.subopcode, req_idle_count, req_busy_count);
                     req_idle_count = 0;
                     req_busy_count = 0;
                  end
                  else begin
                     req_busy_count++;
                  end
               end
               else begin
                  req_idle_count++;
                  req_busy_count = 0;
               end
               if (rsp_valid !== 0) begin
                  if ((rsp_valid & rsp_ready) !== 0) begin
                     m_etlink_cov.sample_rsp(evl_etlink_types_pkg::evl_etlink_rsp_opcode_t'(rsp_info.opcode), rsp_info.id, rsp_info.size, rsp_info.dest, rsp_info.qwen, rsp_idle_count, rsp_busy_count);
                     rsp_idle_count = 0;
                     rsp_busy_count = 0;
                  end
                  else begin
                     rsp_busy_count++;
                  end
               end
               else begin
                  rsp_idle_count++;
                  rsp_busy_count = 0;
               end
            end
         end
      end

      `ifdef ET_ETLINK_INCLUDE_USR_PORT
         import evl_etlink_pkg::usr_etlink_rtl_port;
         usr_etlink_rtl_port m_rtl_port2 = new($sformatf("%m"), AGENT_ID);

         initial begin
            if (ABSTRACT_NAME != "") begin
               m_rtl_port2.set_abstract_name(ABSTRACT_NAME);
            end
            m_rtl_port2.set_rtl_name($sformatf("%m"));
            m_rtl_port2.set_bus_req_data_width($bits(req_info.data) / 8);
            m_rtl_port2.set_bus_rsp_data_width($bits(rsp_info.data) / 8);
         end
      `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT

   `else // ifdef EVL_SIMULATION }-----------------------------------------------------------------{

      import evl_etlink_pkg::usr_etlink_rtl_port;

      usr_etlink_rtl_port m_rtl_port = new($sformatf("%m"), AGENT_ID);

      task drive_reset();
         return;
      endtask : drive_reset

      task pre_drive_request();
         return;
      endtask : pre_drive_request

      task drive_request();
         return;
      endtask : drive_request

      task drive_response();
         return;
      endtask : drive_response

      initial begin
         if (ABSTRACT_NAME != "") begin
            m_rtl_port.set_abstract_name(ABSTRACT_NAME);
         end
         m_rtl_port.set_rtl_name($sformatf("%m"));
         m_rtl_port.set_bus_req_data_width($bits(req_info.data) / 8);
         m_rtl_port.set_bus_rsp_data_width($bits(rsp_info.data) / 8);
      end

   `endif // !ifdef EVL_SIMULATION ----------------------------------------------------------------}


   //
   // Registers to handle zero-time delay requests and responses.
   //
   et_link_req_info_t        req_info_mon;
   et_link_rsp_info_t        rsp_info_mon;
   bit [3:0]                 m_req_bank_mon_r;
   bit [`EVL_TRANS_ID_MSB:0] req_trans_id_r;
   bit                       use_rsp_source_is_uc_r;
   bit                       rsp_source_is_uc_r;
   event                     req_seen;
   event                     rsp_seen;


   initial begin
      #0 wait (clk === 1'b0);
      #0 wait (clk === 1'b1);
      m_start_watching = 1'b1;
   end

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b1) && (m_power_off === 1'b0));
         m_rtl_port.power_off();
         `ifdef ET_ETLINK_INCLUDE_USR_PORT
            m_rtl_port2.power_off();
         `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         m_power_off = 1'b1;
         m_power_on  = 1'b0;
         #0 wait (isolate !== 1'b1);
      end
   end

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b0) && (m_power_on === 1'b0));
         m_rtl_port.power_on();
         `ifdef ET_ETLINK_INCLUDE_USR_PORT
            m_rtl_port2.power_on();
         `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         m_power_on  = 1'b1;
         m_power_off = 1'b0;
         #0 wait (isolate !== 1'b0);
      end
   end

   always @(posedge m_clock) begin
      m_ifc_cycle_count++;
      if ((reset === 1'b1) && (m_reset_asserted == 1'b0)) begin
         m_reset_asserted   <= 1'b1;
         m_reset_deasserted <= 1'b0;
         drive_reset();
         m_rtl_port.assert_reset();
         `ifdef ET_ETLINK_INCLUDE_USR_PORT
            m_rtl_port2.assert_reset();
         `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
      end
      else if ((m_reset_deasserted == 1'b0) && (reset === 1'b0)) begin
         m_reset_asserted   <= 1'b0;
         m_reset_deasserted <= 1'b1;
         m_rtl_port.deassert_reset();
         `ifdef ET_ETLINK_INCLUDE_USR_PORT
            m_rtl_port2.deassert_reset();
         `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
      end

      //
      // Capture an ET Link response.
      //
      if ((rsp_valid === 1'b1) && (rsp_ready === 1'b1) && (reset !== 1'b1)) begin
         rsp_info_mon.id        = rsp_info.id;
         rsp_info_mon.dest      = rsp_info.dest;
         rsp_info_mon.wdata     = rsp_info.wdata;
         rsp_info_mon.opcode    = rsp_info.opcode;
         rsp_info_mon.data      = rsp_info.data;
         rsp_info_mon.size      = rsp_info.size;
         rsp_info_mon.qwen      = rsp_info.qwen;
         use_rsp_source_is_uc_r = use_rsp_source_is_uc;
         rsp_source_is_uc_r     = rsp_source_is_uc;
         `ifdef EVL_SIMULATION
            `evl_log(`UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("capturing response id %0d (%p 0x%x)", rsp_info.id, rsp_info.opcode, rsp_info.data))
         `endif // ifdef EVL_SIMULATION
         if ((DELAY_REQ_ZTIME != 0) || (DELAY_RSP_ZTIME == 0)) begin
            m_rtl_port.capture_rsp(rsp_info_mon, use_rsp_source_is_uc_r, rsp_source_is_uc_r);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_rsp(rsp_info_mon, use_rsp_source_is_uc_r, rsp_source_is_uc_r);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         else begin
            -> rsp_seen;
         end
      end

      //
      // Capture an ET Link request.
      //
      if ((m_req_bank_mon !== 4'hf) && (reset !== 1'b1)) begin
         req_info_mon.id        = req_info.id;
         req_info_mon.source    = req_info.source;
         req_info_mon.wdata     = req_info.wdata;
         req_info_mon.opcode    = req_info.opcode;
         req_info_mon.address   = req_info.address;
         req_info_mon.data      = req_info.data;
         req_info_mon.size      = req_info.size;
         req_info_mon.qwen      = req_info.qwen;
         req_info_mon.subopcode = req_info.subopcode;
         m_req_bank_mon_r         = m_req_bank_mon;
         req_trans_id_r         = req_trans_id;
         `ifdef EVL_SIMULATION
            if (req_info.wdata == 1'b1) begin
               `evl_log(`UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("capturing request on port %0d (%p 0x%x <- 0x%x)", m_req_bank_mon, req_info.opcode, req_info.address, req_info.data))
            end
            else begin
               `evl_log(`UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("capturing request on port %0d (%p 0x%x)", m_req_bank_mon, req_info.opcode, req_info.address))
            end
         `endif // ifdef EVL_SIMULATION
         if ((DELAY_REQ_ZTIME == 0) || (DELAY_RSP_ZTIME != 0)) begin
            m_rtl_port.capture_req((USE_BANKS != 0) ? m_req_bank_mon_r : 4'h0, req_trans_id_r, req_info_mon, (int'(m_req_bank_mon_r) < NUM_BANKS) ? USE_BANKS : ((NUM_UC == 0) ? 1'b1 : 1'b0));
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_req((USE_BANKS != 0) ? m_req_bank_mon_r : 4'h0, req_trans_id_r, req_info_mon, (int'(m_req_bank_mon_r) < NUM_BANKS) ? USE_BANKS : ((NUM_UC == 0) ? 1'b1 : 1'b0));
            `endif //ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         else begin
            -> req_seen;
         end
      end

      //
      // If we want to drive a particular bank request, but it has neither been driven nor accepted,
      // then it continues to be a pending request.  This is handled by the call to
      // pre_drive_request().
      //
      if ((m_req_drive === 1'b1) && (reset === 1'b0)) begin
         pre_drive_request();
      end

      //
      // Notify the System Verilog class objects that a rising clock has occurred.
      //
      m_rtl_port.ping_clock();
      `ifdef ET_ETLINK_INCLUDE_USR_PORT
         m_rtl_port.ping_clock();
      `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT

      //
      // The following code fragment is only used for driving requests and/or responses.
      //
      if (reset === 1'b0) begin
         if (m_req_drive === 1'b1) begin
            //
            // If this module is supposed to drive requests, do this with a call to drive_request().
            // Note that this task is also capable of driving responses.
            //
            drive_request();
         end
         else if (m_rsp_drive === 1'b1) begin
            //
            // If this module is supposed to solely drive responses, do this with a call to
            // drive_response().
            //
            drive_response();
         end
      end
   end


   //
   // Due to potential zero-time event processing in Verilog, it may be necessary to delay capturing
   // responses.  This behavior is defined by input parameters set when this module is instantiated.
   //
   initial begin
      if ((DELAY_REQ_ZTIME == 0) && (DELAY_RSP_ZTIME != 0)) begin
         forever @(rsp_seen) begin
            #0 m_rtl_port.capture_rsp(rsp_info_mon, use_rsp_source_is_uc_r, rsp_source_is_uc_r);
               `ifdef ET_ETLINK_INCLUDE_USR_PORT
                  m_rtl_port2.capture_rsp(rsp_info_mon, use_rsp_source_is_uc_r, rsp_source_is_uc_r);
               `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
      end
   end


   //
   // Due to potential zero-time event processing in Verilog, it may be necessary to delay capturing
   // requests.  This behavior is defined by input parameters set when this module is instantiated.
   //
   initial begin
      if ((DELAY_REQ_ZTIME != 0) && (DELAY_RSP_ZTIME == 0)) begin
         forever @(req_seen) begin
            #0 m_rtl_port.capture_req((USE_BANKS != 0) ? m_req_bank_mon_r : 4'h0, req_trans_id_r, req_info_mon, (int'(m_req_bank_mon_r) < NUM_BANKS) ? USE_BANKS : ((NUM_UC == 0) ? 1'b1 : 1'b0));
               `ifdef ET_ETLINK_INCLUDE_USR_PORT
                  m_rtl_port2.capture_req((USE_BANKS != 0) ? m_req_bank_mon_r : 4'h0, req_trans_id_r, req_info_mon, (int'(m_req_bank_mon_r) < NUM_BANKS) ? USE_BANKS : ((NUM_UC == 0) ? 1'b1 : 1'b0));
               `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
      end
   end

endmodule

//--------------------------------------------------------------------------------------------------
//
//
// Interface: evl_axi_interface
//
`include "rtl/inc/soc.vh"

`ifndef EVL_IFC_PORT
   `ifdef EVL_SIMULATION
      `define EVL_IFC_PORT inout
   `else
      `define EVL_IFC_PORT input
   `endif
`endif

`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

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
      `define ET_AXI_INCLUDE_USR_PORT
   `endif
`endif

`ifdef EVL_SIMULATION
   `include "dv/common/axi/axi_vc/evl_axi_cov_pkg.sv"
`endif // EVL_SIMULATION

module evl_axi_interface #(parameter int    SHIRE_ID                           = 0,
                           parameter int    PORT_ID                            = 0,
                           parameter int    MAX_PORT_ID                        = 0,
                           parameter int    PORT_TYPE                          = 0,
                           parameter bit    CAPTURE_DPI_EVENTS                 = 1'b0,
                           parameter int    ID_WIDTH                           = `EVL_BUS_REQ_ID_WIDTH,
                           parameter int    ADDR_WIDTH                         = `EVL_PADDR_WIDTH,
                           parameter int    DATA_WIDTH                         = `EVL_XWORD_WIDTH,
                           parameter int    ARUSER_WIDTH                       = `EVL_BUS_REQ_USER_WIDTH,
                           parameter int    AWUSER_WIDTH                       = `EVL_BUS_REQ_USER_WIDTH,
                           parameter int    AXLEN_WIDTH                        = `EVL_BUS_REQ_LEN_WIDTH,
                           parameter int    AXSIZE_WIDTH                       = `EVL_BUS_REQ_SIZE_WIDTH,
                           parameter int    AXBURST_WIDTH                      = `EVL_BUS_REQ_BURST_WIDTH,
                           parameter int    AXCACHE_WIDTH                      = `EVL_BUS_CACHE_ATTR_WIDTH,
                           parameter int    AXPROT_WIDTH                       = `EVL_BUS_PROT_WIDTH,
                           parameter int    AXQOS_WIDTH                        = `EVL_BUS_QOS_WIDTH,
                           parameter int    AXREGION_WIDTH                     = `EVL_BUS_REGION_WIDTH,
                           parameter int    AXLOCK_WIDTH                       = 1,
                           parameter int    BUSER_WIDTH                        = 1,
                           parameter int    RUSER_WIDTH                        = 1,
                           parameter int    WUSER_WIDTH                        = 1,
                           parameter int    WSTRB_WIDTH                        = DATA_WIDTH / 8,
                           parameter int    BRESP_WIDTH                        = `AXI_BRESP_SIZE,
                           parameter int    RRESP_WIDTH                        = `AXI_RRESP_SIZE,
                           parameter int    AR_VC_WIDTH                        = 1,
                           parameter int    AW_VC_WIDTH                        = 1,
                           parameter int    AR_VC_CREDITS                      = 0,
                           parameter int    AW_VC_CREDITS                      = 0,
                           parameter int    ALLOW_DUPLICATE_OUTSTANDING_IDS    = 0,
                           parameter int    IGNORE_REGION                      = 0,
                           parameter int    IGNORE_NON_REQ_USER                = 0,
                           parameter int    NO_TRANS_ID_REQ_DRIVER             = 0,
                           parameter int    NO_TRANS_ID_RSP_DRIVER             = 0,
                           parameter int    LOCK_W_RDY                         = 0,
                           parameter int    DPI_MEM_DEPTH                      = 4,
                           parameter int    SKIP_LEVELS[]                      = {0},
                           parameter int    AR_REQ_LIMIT                       = 8,
                           parameter int    AW_REQ_LIMIT                       = 8,
                           parameter int    ALLOW_DRV_W_BEFORE_AW              = 0,
                           parameter int    DELAY_REQ_ZTIME                    = 0,
                           parameter int    DELAY_RSP_ZTIME                    = 0,
                           parameter string ABSTRACT_NAME                      = "AXI",
                           parameter string AXI_COV_NAME                       = "AXI_NONE",
                           parameter evl_base_pkg::axi_cov_type_t AXI_COV_TYPE = evl_base_pkg::EVL_COV_AXI_NONE
                          ) (
      input         wire                       isolate,
      input         wire                       reset,
      input         wire                       clk,

      // AW channel
      `EVL_IFC_PORT wire [ID_WIDTH-1:0]        aw_id,
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]      aw_addr,
      `EVL_IFC_PORT wire [AXLEN_WIDTH-1:0]     aw_len,
      `EVL_IFC_PORT wire [AXSIZE_WIDTH-1:0]    aw_size,
      `EVL_IFC_PORT wire [AXBURST_WIDTH-1:0]   aw_burst,
      `EVL_IFC_PORT wire [AXLOCK_WIDTH-1:0]    aw_lock,
      `EVL_IFC_PORT wire [AXCACHE_WIDTH-1:0]   aw_cache,
      `EVL_IFC_PORT wire [AXPROT_WIDTH-1:0]    aw_prot,
      `EVL_IFC_PORT wire [AXQOS_WIDTH-1:0]     aw_qos,
      `EVL_IFC_PORT wire [AXREGION_WIDTH-1:0]  aw_region,
      `EVL_IFC_PORT wire [AWUSER_WIDTH-1:0]    aw_user,
      `EVL_IFC_PORT wire                       aw_valid,
      `EVL_IFC_PORT wire                       aw_ready,
      `EVL_IFC_PORT wire [AW_VC_WIDTH-1:0]     aw_vc_valid,
      `EVL_IFC_PORT wire [AW_VC_WIDTH-1:0]     aw_vc_credit,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0] aw_trans_id,

      // W channel
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]      w_data,
      `EVL_IFC_PORT wire [WSTRB_WIDTH-1:0]     w_strb,
      `EVL_IFC_PORT wire                       w_last,
      `EVL_IFC_PORT wire [WUSER_WIDTH-1:0]     w_user,
      `EVL_IFC_PORT wire                       w_valid,
      `EVL_IFC_PORT wire                       w_ready,
      `EVL_IFC_PORT wire [AW_VC_WIDTH-1:0]     w_vc_valid,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0] w_trans_id,

      // B channel
      `EVL_IFC_PORT wire [ID_WIDTH-1:0]        b_id,
      `EVL_IFC_PORT wire [BRESP_WIDTH-1:0]     b_resp,
      `EVL_IFC_PORT wire [BUSER_WIDTH-1:0]     b_user,
      `EVL_IFC_PORT wire                       b_valid,
      `EVL_IFC_PORT wire                       b_ready,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0] b_trans_id,

      // AR channel
      `EVL_IFC_PORT wire                       ar_ready,
      `EVL_IFC_PORT wire                       ar_valid,
      `EVL_IFC_PORT wire [ID_WIDTH-1:0]        ar_id,
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]      ar_addr,
      `EVL_IFC_PORT wire [AXLEN_WIDTH-1:0]     ar_len,
      `EVL_IFC_PORT wire [AXSIZE_WIDTH-1:0]    ar_size,
      `EVL_IFC_PORT wire [AXBURST_WIDTH-1:0]   ar_burst,
      `EVL_IFC_PORT wire [AXLOCK_WIDTH-1:0]    ar_lock,
      `EVL_IFC_PORT wire [AXCACHE_WIDTH-1:0]   ar_cache,
      `EVL_IFC_PORT wire [AXPROT_WIDTH-1:0]    ar_prot,
      `EVL_IFC_PORT wire [AXQOS_WIDTH-1:0]     ar_qos,
      `EVL_IFC_PORT wire [AXREGION_WIDTH-1:0]  ar_region,
      `EVL_IFC_PORT wire [ARUSER_WIDTH-1:0]    ar_user,
      `EVL_IFC_PORT wire [AR_VC_WIDTH-1:0]     ar_vc_valid,
      `EVL_IFC_PORT wire [AR_VC_WIDTH-1:0]     ar_vc_credit,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0] ar_trans_id,

      // R channel
      `EVL_IFC_PORT wire [ID_WIDTH-1:0]        r_id,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]      r_data,
      `EVL_IFC_PORT wire [RRESP_WIDTH-1:0]     r_resp,
      `EVL_IFC_PORT wire                       r_last,
      `EVL_IFC_PORT wire [RUSER_WIDTH-1:0]     r_user,
      `EVL_IFC_PORT wire                       r_valid,
      `EVL_IFC_PORT wire                       r_ready,
      `EVL_IFC_PORT wire [`EVL_TRANS_ID_MSB:0] r_trans_id
   );

   localparam int MAX_VC             = 8;
   localparam int TRUE_AW_VC_WIDTH   = ((AW_VC_CREDITS > 0) && (AW_VC_WIDTH > 1) && (AW_VC_WIDTH < MAX_VC)) ? AW_VC_WIDTH : 1;
   localparam int TRUE_AR_VC_WIDTH   = ((AR_VC_CREDITS > 0) && (AR_VC_WIDTH > 1) && (AR_VC_WIDTH < MAX_VC)) ? AR_VC_WIDTH : 1;
   localparam int TRUE_AW_VC_CREDITS = ((AW_VC_CREDITS > 0) && (AW_VC_CREDITS < 255)) ? AW_VC_CREDITS : 0;
   localparam int TRUE_AR_VC_CREDITS = ((AR_VC_CREDITS > 0) && (AR_VC_CREDITS < 255)) ? AR_VC_CREDITS : 0;
   localparam int LINE_ADDR_MASK     = (`EVL_LINE_BYTES - 1) & (~(WSTRB_WIDTH - 1));

   `ifdef ET_MONITOR_IFCS // ----------------------------------------------------------------------{

      import evl_base_pkg::*;
      import evl_axi_types_pkg::*;
      import evl_axi_pkg::evl_axi_req_info_t;
      import evl_axi_pkg::evl_axi_w_data_info_t;
      import evl_axi_pkg::evl_axi_rsp_info_t;
      import evl_axi_pkg::evl_axi_bus_req;
      import evl_axi_pkg::evl_axi_bus_rsp;


      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         //-----------------------------------------------------------------------------------------
         //
         // Class: evl_axi_rtl_port
         //
         // This is an extensioon of the base RTL port.
         //
         class evl_axi_rtl_port extends evl_axi_pkg::evl_axi_rtl_port_base;

            //--------------------------------------------------------------------------------------
            //
            // Constructor
            //
            function new(input string         full_name_in     = "axi_rtl_port",
                         input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                         input int            skip_levels_in[] = {});
               super.new(full_name_in, AXI_BUS_AGENT, agent_pid_in, "evl_axi_verif_comp", "", "", "", skip_levels_in, ((MAX_PORT_ID > 0) ? PORT_ID : 0), MAX_PORT_ID);
               set_int_param_by_name("pk_addr_width", ADDR_WIDTH / 8);
               set_int_param_by_name("pk_data_width", DATA_WIDTH / 8);
               set_int_param_by_name("pk_read_vchannels", TRUE_AR_VC_WIDTH);
               set_int_param_by_name("pk_write_vchannels", TRUE_AW_VC_WIDTH);
               set_int_param_by_name("pk_read_vc_credits", TRUE_AR_VC_CREDITS);
               set_int_param_by_name("pk_write_vc_credits", TRUE_AW_VC_CREDITS);
               set_int_param_by_name("pk_allow_duplicate_outstanding_ids", ALLOW_DUPLICATE_OUTSTANDING_IDS);
            endfunction : new

         endclass : evl_axi_rtl_port

         bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), AXI_BUS_AGENT, SHIRE_ID, "evl_axi_verif_comp", "", "", "", { SKIP_LEVELS });
         evl_axi_rtl_port m_rtl_port = new($sformatf("%m"), SHIRE_ID, { SKIP_LEVELS });

         `ifdef ET_AXI_INCLUDE_USR_PORT
            evl_axi_pkg::usr_axi_rtl_port_base m_rtl_port2;

            initial begin
               m_rtl_port2 = new($sformatf("%m"), SHIRE_ID);
               if (ABSTRACT_NAME != "") begin
                  m_rtl_port2.set_abstract_name(ABSTRACT_NAME);
               end
               m_rtl_port2.set_rtl_name($sformatf("%m"));
               m_rtl_port2.set_bus_req_data_width(DATA_WIDTH/8);
               m_rtl_port2.set_bus_rsp_data_width(DATA_WIDTH/8);
            end
         `endif // ifdef ET_AXI_INCLUDE_USR_PORT

         evl_axi_cov_pkg::evl_axi_cov#(.AXI_COV_TYPE(AXI_COV_TYPE), .AXI_COV_NAME(AXI_COV_NAME)) m_axi_cov;

      `else // ifdef EVL_SIMULATION }--------------------------------------------------------------{

         //-----------------------------------------------------------------------------------------
         //
         // Class: evl_axi_rtl_port
         //
         // This is an extensioon of the base RTL port.
         //
         class evl_axi_rtl_port extends evl_axi_pkg::usr_axi_rtl_port_base;

            //--------------------------------------------------------------------------------------
            //
            // Constructor
            //
            function new(input string         full_name_in = "axi_rtl_port",
                         input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0));
               super.new(full_name_in, agent_pid_in);
            endfunction : new

         endclass : evl_axi_rtl_port

         evl_axi_rtl_port m_rtl_port;

         initial begin
            m_rtl_port = new($sformatf("%m"), SHIRE_ID);
            if (ABSTRACT_NAME != "") begin
               m_rtl_port.set_abstract_name(ABSTRACT_NAME);
            end
            m_rtl_port.set_rtl_name($sformatf("%m"));
            m_rtl_port.set_bus_req_data_width(DATA_WIDTH/8);
            m_rtl_port.set_bus_rsp_data_width(DATA_WIDTH/8);
         end

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}

      //
      // The following variables are used for general control and information.
      //
      string     m_abstract_name;
      reg        m_request_master;
      reg        m_response_master;
      bit        m_delays_enabled;
      bit        m_reset_asserted;
      bit        m_reset_deasserted;
      reg        m_lock_w_rdy;

      //
      // The following variables are used for monitoring AXI signals.
      //
      evl_axi_req_info_t    m_ar_req_mon;
      evl_axi_req_info_t    m_aw_req_mon;
      evl_axi_w_data_info_t m_w_req_mon;
      evl_axi_rsp_info_t    m_r_rsp_mon;
      evl_axi_rsp_info_t    m_b_rsp_mon;
      bit                   m_ar_valid_req;
      bit                   m_aw_valid_req;
      bit                   m_w_valid_req;
      bit                   m_r_valid_rsp;
      bit                   m_b_valid_rsp;
      bit                   m_delay_ping;
      event                 m_capture_req;
      event                 m_capture_rsp;

      //
      // The following variables are used to determine the appropriate timing for issuing requests
      // and responses when this particular module is configured as a driver.  Note that delays must
      // be enabled to enable cycle timing.
      //
      bit [31:0] m_cycle_count;
      bit [31:0] m_cycle_count_posedge;

      //
      // The following variables can be used to force backpressure on AXI ready signals.  By
      // default, there is no backpressure.
      //
      bit        m_ar_ready_mask;
      bit        m_aw_ready_mask;
      bit        m_w_ready_mask;
      bit        m_r_ready_mask;
      bit        m_b_ready_mask;

      //
      // The following variables are used to handle virtual channels for AR and AW requests.
      //
      int                              m_ar_vc_count_value;
      int                              m_aw_vc_count_value;
      bit [((TRUE_AR_VC_WIDTH*8)-1):0] m_ar_credits;
      bit [((TRUE_AW_VC_WIDTH*8)-1):0] m_aw_credits;
      bit [((TRUE_AW_VC_WIDTH*8)-1):0] m_w_credits;
      reg [(TRUE_AR_VC_WIDTH-1):0]     m_ar_vc_credit_mon;
      reg [(TRUE_AW_VC_WIDTH-1):0]     m_aw_vc_credit_mon;
      bit [AR_VC_WIDTH-1:0]            m_ar_vc_valid_r;
      bit [AW_VC_WIDTH-1:0]            m_aw_vc_valid_r;
      bit [AW_VC_WIDTH-1:0]            m_aw_vc_credit_r;

      //
      // The following variables are for driving AW channel requests.
      //
      reg [ID_WIDTH-1:0]        aw_id_out;
      reg [ADDR_WIDTH-1:0]      aw_addr_out;
      reg [AXLEN_WIDTH-1:0]     aw_len_out;
      reg [AXSIZE_WIDTH-1:0]    aw_size_out;
      reg [AXBURST_WIDTH-1:0]   aw_burst_out;
      reg                       aw_lock_out;
      reg [AXCACHE_WIDTH-1:0]   aw_cache_out;
      reg [AXPROT_WIDTH-1:0]    aw_prot_out;
      reg [AXQOS_WIDTH-1:0]     aw_qos_out;
      reg [AXREGION_WIDTH-1:0]  aw_region_out;
      reg [AWUSER_WIDTH-1:0]    aw_user_out;
      reg                       aw_valid_out;
      reg                       aw_ready_out;
      reg [AW_VC_WIDTH-1:0]     aw_vc_valid_out;
      reg [AW_VC_WIDTH-1:0]     aw_vc_credit_out;
      reg [`EVL_TRANS_ID_MSB:0] aw_trans_id_out;

      //
      // The following variables are for driving W channel requests.
      //
      reg [DATA_WIDTH-1:0]      w_data_out;
      reg [WSTRB_WIDTH-1:0]     w_strb_out;
      reg                       w_last_out;
      reg [WUSER_WIDTH-1:0]     w_user_out;
      reg                       w_valid_out;
      reg                       w_ready_out;
      reg [AW_VC_WIDTH-1:0]     w_vc_valid_out;
      reg [`EVL_TRANS_ID_MSB:0] w_trans_id_out;

      //
      // The following variables are for driving B channel responses.
      //
      reg [ID_WIDTH-1:0]        b_id_out;
      reg [BRESP_WIDTH-1:0]     b_resp_out;
      reg [BUSER_WIDTH-1:0]     b_user_out;
      reg                       b_valid_out;
      reg                       b_ready_out;
      reg [`EVL_TRANS_ID_MSB:0] b_trans_id_out;

      //
      // The following variables are for driving AR channel requests.
      //
      reg                       ar_ready_out;
      reg                       ar_valid_out;
      reg [ID_WIDTH-1:0]        ar_id_out;
      reg [ADDR_WIDTH-1:0]      ar_addr_out;
      reg [AXLEN_WIDTH-1:0]     ar_len_out;
      reg [AXSIZE_WIDTH-1:0]    ar_size_out;
      reg [AXBURST_WIDTH-1:0]   ar_burst_out;
      reg                       ar_lock_out;
      reg [AXCACHE_WIDTH-1:0]   ar_cache_out;
      reg [AXPROT_WIDTH-1:0]    ar_prot_out;
      reg [AXQOS_WIDTH-1:0]     ar_qos_out;
      reg [AXREGION_WIDTH-1:0]  ar_region_out;
      reg [ARUSER_WIDTH-1:0]    ar_user_out;
      reg [AR_VC_WIDTH-1:0]     ar_vc_valid_out;
      reg [AR_VC_WIDTH-1:0]     ar_vc_credit_out;
      reg [`EVL_TRANS_ID_MSB:0] ar_trans_id_out;

      //
      // The following variables are for driving R channel responses.
      //
      reg [ID_WIDTH-1:0]        r_id_out;
      reg [DATA_WIDTH-1:0]      r_data_out;
      reg [RRESP_WIDTH-1:0]     r_resp_out;
      reg                       r_last_out;
      reg [RUSER_WIDTH-1:0]     r_user_out;
      reg                       r_valid_out;
      reg                       r_ready_out;
      reg [`EVL_TRANS_ID_MSB:0] r_trans_id_out;


      //--------------------------------------------------------------------------------------------
      //
      // IO Assignments for Drivers
      //
      assign aw_id        = aw_id_out;
      assign aw_addr      = aw_addr_out;
      assign aw_len       = aw_len_out;
      assign aw_size      = aw_size_out;
      assign aw_burst     = aw_burst_out;
      assign aw_lock      = aw_lock_out;
      assign aw_cache     = aw_cache_out;
      assign aw_prot      = aw_prot_out;
      assign aw_qos       = aw_qos_out;
      assign aw_region    = aw_region_out;
      assign aw_user      = aw_user_out;
      assign aw_valid     = aw_valid_out;
      assign aw_vc_valid  = aw_vc_valid_out;
      assign aw_vc_credit = aw_vc_credit_out;
      assign aw_ready     = (m_response_master !== 1'b1) ? 1'bz : aw_ready_out & m_aw_ready_mask;
      assign aw_trans_id  = aw_trans_id_out;
      assign w_data       = w_data_out;
      assign w_strb       = w_strb_out;
      assign w_last       = w_last_out;
      assign w_user       = w_user_out;
      assign w_valid      = w_valid_out;
      assign w_vc_valid   = w_vc_valid_out;
      assign w_ready      = (m_response_master !== 1'b1) ? 1'bz : w_ready_out & m_w_ready_mask;
      assign w_trans_id   = w_trans_id_out;
      assign b_id         = b_id_out;
      assign b_resp       = b_resp_out;
      assign b_user       = b_user_out;
      assign b_valid      = b_valid_out;
      assign b_ready      = (m_request_master !== 1'b1) ? 1'bz : b_ready_out & m_b_ready_mask;
      assign b_trans_id   = b_trans_id_out;
      assign ar_valid     = ar_valid_out;
      assign ar_id        = ar_id_out;
      assign ar_addr      = ar_addr_out;
      assign ar_len       = ar_len_out;
      assign ar_size      = ar_size_out;
      assign ar_burst     = ar_burst_out;
      assign ar_lock      = ar_lock_out;
      assign ar_cache     = ar_cache_out;
      assign ar_prot      = ar_prot_out;
      assign ar_qos       = ar_qos_out;
      assign ar_region    = ar_region_out;
      assign ar_user      = ar_user_out;
      assign ar_vc_valid  = ar_vc_valid_out;
      assign ar_vc_credit = ar_vc_credit_out;
      assign ar_ready     = (m_response_master !== 1'b1) ? 1'bz : ar_ready_out & m_ar_ready_mask;
      assign ar_trans_id  = ar_trans_id_out;
      assign r_id         = r_id_out;
      assign r_data       = r_data_out;
      assign r_resp       = r_resp_out;
      assign r_last       = r_last_out;
      assign r_user       = r_user_out;
      assign r_valid      = r_valid_out;
      assign r_ready      = (m_request_master !== 1'b1) ? 1'bz : r_ready_out & m_r_ready_mask;
      assign r_trans_id   = r_trans_id_out;


      initial begin
         m_reset_asserted    = 1'b0;
         m_reset_deasserted  = 1'b0;
         m_request_master    = 1'b0;
         m_response_master   = 1'b0;
         m_ar_ready_mask     = 1'b1;
         m_aw_ready_mask     = 1'b1;
         m_w_ready_mask      = 1'b1;
         m_r_ready_mask      = 1'b1;
         m_b_ready_mask      = 1'b1;
         m_lock_w_rdy        = LOCK_W_RDY;
         for (m_ar_vc_count_value = 0; m_ar_vc_count_value < TRUE_AR_VC_WIDTH; m_ar_vc_count_value++) begin
            m_ar_credits[8 * m_ar_vc_count_value +: 8] = AR_VC_CREDITS;
         end
         for (m_aw_vc_count_value = 0; m_aw_vc_count_value < TRUE_AW_VC_WIDTH; m_aw_vc_count_value++) begin
            m_aw_credits[8 * m_aw_vc_count_value +: 8] = AW_VC_CREDITS;
            m_w_credits[8 * m_aw_vc_count_value +: 8]  = AW_VC_CREDITS;
         end
         m_ar_vc_count_value = 0;
         m_aw_vc_count_value = 0;
         aw_id_out           = {ID_WIDTH{1'bz}};
         aw_addr_out         = {ADDR_WIDTH{1'bz}};
         aw_len_out          = {AXLEN_WIDTH{1'bz}};
         aw_size_out         = {AXSIZE_WIDTH{1'bz}};
         aw_burst_out        = {AXBURST_WIDTH{1'bz}};
         aw_lock_out         = 1'bz;
         aw_cache_out        = {AXCACHE_WIDTH{1'bz}};
         aw_prot_out         = {AXPROT_WIDTH{1'bz}};
         aw_qos_out          = {AXQOS_WIDTH{1'bz}};
         aw_region_out       = {AXREGION_WIDTH{1'bz}};
         aw_user_out         = {AWUSER_WIDTH{1'bz}};
         aw_valid_out        = 1'bz;
         aw_ready_out        = 1'bz;
         aw_vc_valid_out     = {AW_VC_WIDTH{1'bz}};
         aw_vc_credit_out    = {AW_VC_WIDTH{1'bz}};
         aw_trans_id_out     = (NO_TRANS_ID_REQ_DRIVER == 0) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} : {(`EVL_TRANS_ID_MSB+1){1'b0}};
         w_data_out          = {DATA_WIDTH{1'bz}};
         w_strb_out          = {WSTRB_WIDTH{1'bz}};
         w_last_out          = 1'bz;
         w_user_out          = {WUSER_WIDTH{1'bz}};
         w_valid_out         = 1'bz;
         w_ready_out         = 1'bz;
         w_vc_valid_out      = {AW_VC_WIDTH{1'bz}};
         w_trans_id_out      = (NO_TRANS_ID_REQ_DRIVER == 0) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} : {(`EVL_TRANS_ID_MSB+1){1'b0}};
         b_id_out            = {ID_WIDTH{1'bz}};
         b_resp_out          = {BRESP_WIDTH{1'bz}};
         b_user_out          = {BUSER_WIDTH{1'bz}};
         b_valid_out         = 1'bz;
         b_ready_out         = 1'bz;
         b_trans_id_out      = (NO_TRANS_ID_RSP_DRIVER == 0) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} : {(`EVL_TRANS_ID_MSB+1){1'b0}};
         ar_ready_out        = 1'bz;
         ar_valid_out        = 1'bz;
         ar_id_out           = {ID_WIDTH{1'bz}};
         ar_addr_out         = {ADDR_WIDTH{1'bz}};
         ar_len_out          = {AXLEN_WIDTH{1'bz}};
         ar_size_out         = {AXSIZE_WIDTH{1'bz}};
         ar_burst_out        = {AXBURST_WIDTH{1'bz}};
         ar_lock_out         = 1'bz;
         ar_cache_out        = {AXCACHE_WIDTH{1'bz}};
         ar_prot_out         = {AXPROT_WIDTH{1'bz}};
         ar_qos_out          = {AXQOS_WIDTH{1'bz}};
         ar_region_out       = {AXREGION_WIDTH{1'bz}};
         ar_user_out         = {ARUSER_WIDTH{1'bz}};
         ar_vc_valid_out     = {AR_VC_WIDTH{1'bz}};
         ar_vc_credit_out    = {AR_VC_WIDTH{1'bz}};
         ar_trans_id_out     = (NO_TRANS_ID_REQ_DRIVER == 0) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} : {(`EVL_TRANS_ID_MSB+1){1'b0}};
         r_id_out            = {ID_WIDTH{1'bz}};
         r_data_out          = {DATA_WIDTH{1'bz}};
         r_resp_out          = {RRESP_WIDTH{1'bz}};
         r_last_out          = 1'bz;
         r_user_out          = {RUSER_WIDTH{1'bz}};
         r_valid_out         = 1'bz;
         r_ready_out         = 1'bz;
         r_trans_id_out      = (NO_TRANS_ID_RSP_DRIVER == 0) ? {(`EVL_TRANS_ID_MSB+1){1'bz}} : {(`EVL_TRANS_ID_MSB+1){1'b0}};
         m_cycle_count         <= 32'h0;
         m_cycle_count_posedge <= 32'h0;
         #0;
         fork
            begin
               forever begin
                  wait (reset === 1'b1);
                  m_rtl_port.assert_reset();
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.assert_reset();
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
                  m_reset_asserted   = 1'b1;
                  m_reset_deasserted = 1'b0;
                  for (m_ar_vc_count_value = 0; m_ar_vc_count_value < TRUE_AR_VC_WIDTH; m_ar_vc_count_value++) begin
                     m_ar_credits[8 * m_ar_vc_count_value +: 8] = AR_VC_CREDITS;
                  end
                  for (m_aw_vc_count_value = 0; m_aw_vc_count_value < TRUE_AW_VC_WIDTH; m_aw_vc_count_value++) begin
                     m_aw_credits[8 * m_aw_vc_count_value +: 8] = AW_VC_CREDITS;
                     m_w_credits[8 * m_aw_vc_count_value +: 8]  = AW_VC_CREDITS;
                  end
                  m_ar_vc_count_value = 0;
                  m_aw_vc_count_value = 0;
                  wait (reset === 1'b0);
                  m_rtl_port.deassert_reset();
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.deassert_reset();
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
                  m_reset_asserted   = 1'b0;
                  m_reset_deasserted = 1'b1;
               end
            end
            begin
               wait (clk === 1'b0);
               forever begin
                  @(posedge clk);
                  // Capture pin values here
                  m_ar_vc_count_value = 8'h00;
                  if (TRUE_AR_VC_CREDITS > 0) begin
                     m_ar_vc_credit_mon = ar_vc_credit;
                     while (m_ar_vc_credit_mon != {TRUE_AR_VC_WIDTH{1'b0}}) begin
                        if (m_ar_vc_credit_mon[0:0] == 1'b1) begin
                           m_ar_credits[8 * m_ar_vc_count_value +: 8] = m_ar_credits[8 * m_ar_vc_count_value +: 8] + 8'h01;
                        end
                        m_ar_vc_credit_mon >>= 1;
                        m_ar_vc_count_value++;
                     end
                  end
                  m_aw_vc_count_value = 8'h00;
                  if (TRUE_AW_VC_CREDITS > 0) begin
                     m_aw_vc_credit_mon = aw_vc_credit;
                     while (m_aw_vc_credit_mon != {TRUE_AW_VC_WIDTH{1'b0}}) begin
                        if (m_aw_vc_credit_mon[0:0] == 1'b1) begin
                           m_aw_credits[8 * m_aw_vc_count_value +: 8] = m_aw_credits[8 * m_aw_vc_count_value +: 8] + 8'h01;
                           m_w_credits[8 * m_aw_vc_count_value +: 8]  = m_w_credits[8 * m_aw_vc_count_value +: 8] + 8'h01;
                        end
                        m_aw_vc_credit_mon >>= 1;
                        m_aw_vc_count_value++;
                     end
                  end
               end
            end
            begin
               #0 wait (reset === 1'b1);
               #0 wait ((m_response_master == 1'b1) && (reset === 1'b0));
               @(posedge clk);
               ar_ready_out <= `ET_TB_HOLD 1'b1;
               aw_ready_out <= `ET_TB_HOLD 1'b1;
               w_ready_out  <= `ET_TB_HOLD 1'b1;
            end
            begin
               #0 wait (reset === 1'b1);
               #0 wait ((m_request_master == 1'b1) && (reset === 1'b0));
               @(posedge clk);
               r_ready_out <= `ET_TB_HOLD 1'b1;
               b_ready_out <= `ET_TB_HOLD 1'b1;
            end
         join
      end


      //--------------------------------------------------------------------------------------------
      //
      // m_ar_req:        This bit vector contains request bits for AR bus requests on a per VC
      //                  basis.  In addition, the most significant bit will be set if there is a
      //                  request in progress.
      // m_ar_cycle:      This bit vector contains cycle count information for delaying AR bus
      //                  requests on a per VC basis.  Cycle count vectors are currently 32 bits
      //                  wide.
      // m_ar_bus_reqs[]: This memory array contains the bus request infomation per VC.
      //
      bit [TRUE_AR_VC_WIDTH:0]   m_ar_req             = {(TRUE_AR_VC_WIDTH + 1){1'b0}};
      bit [31:0]                 m_ar_req_cycle_delay = 0;
      mailbox #(evl_axi_bus_req) m_ar_bus_req_mbs[TRUE_AR_VC_WIDTH];

      genvar ar_idx;
      generate
         for (ar_idx = 0; ar_idx < TRUE_AR_VC_WIDTH; ar_idx++) begin :ar_req_loop
            evl_axi_bus_req          bus_req;
            bit [31:0]               req_cycle;
            bit [31:0]               req_cycle_delay;
            bit [TRUE_AR_VC_WIDTH:0] req_mask  = 8'hff << ar_idx;
            bit [TRUE_AR_VC_WIDTH:0] req_value = 8'h01 << ar_idx;
            bit [TRUE_AR_VC_WIDTH:0] req_next  = ~((8'h01 << TRUE_AR_VC_WIDTH) | (8'h01 << ar_idx));

            initial begin
               m_ar_bus_req_mbs[ar_idx] = new();
               forever begin
                  m_ar_bus_req_mbs[ar_idx].get(bus_req);
                  m_ar_req[ar_idx] = 1'b1;
                  wait (m_ar_req[ar_idx] === 1'b0);
               end
            end

            always begin
               wait (reset === 1'b0);
               #0 wait ((m_ar_req[ar_idx] === 1'b1) || (reset !== 1'b0));
               req_cycle_delay = 0;
               if (m_ar_req[ar_idx] === 1'b1) begin
                  req_cycle = bus_req.get_cycle();
                  if ((m_delays_enabled == 1'b1) && (req_cycle > m_cycle_count_posedge[31:0]) && (ar_idx == 0)) begin
                     req_cycle_delay = req_cycle - m_cycle_count_posedge[31:0];
                  end
               end
               #0 wait ( (reset !== 1'b0) ||
                         ( ((m_delays_enabled == 1'b0) || (req_cycle <= m_cycle_count_posedge[31:0])) &&
                           ((TRUE_AR_VC_CREDITS <= 0) || (m_ar_credits[(8 * ar_idx) +: 8] > 8'h00)) ) );
               if (reset === 1'b0) begin
                  #0 wait ((m_ar_req & req_mask) === req_value);
                  m_ar_req[TRUE_AR_VC_WIDTH] = 1'b1;

                  if ((m_delays_enabled == 1'b1) && (req_cycle <= m_cycle_count_posedge[31:0]) && (ar_idx == 0)) begin
                     m_ar_req_cycle_delay <= req_cycle_delay;
                  end
                  ar_id_out       <= `ET_TB_HOLD bus_req.get_ax_id();
                  ar_addr_out     <= `ET_TB_HOLD bus_req.get_ax_addr();
                  ar_len_out      <= `ET_TB_HOLD bus_req.get_ax_len();
                  ar_size_out     <= `ET_TB_HOLD bus_req.get_ax_size();
                  ar_burst_out    <= `ET_TB_HOLD bus_req.get_ax_burst();
                  ar_lock_out     <= `ET_TB_HOLD bus_req.get_ax_lock();
                  ar_cache_out    <= `ET_TB_HOLD bus_req.get_ax_cache();
                  ar_prot_out     <= `ET_TB_HOLD bus_req.get_ax_prot();
                  ar_qos_out      <= `ET_TB_HOLD bus_req.get_ax_qos();
                  ar_user_out     <= `ET_TB_HOLD bus_req.get_ax_user();
                  ar_trans_id_out <= `ET_TB_HOLD bus_req.get_trans_id();
                  if (IGNORE_REGION == 0) begin
                     ar_region_out <= `ET_TB_HOLD bus_req.get_ax_region();
                  end
                  if (TRUE_AR_VC_CREDITS > 0) begin
                     ar_vc_valid_out <= `ET_TB_HOLD (1 << ar_idx);
                  end
                  ar_valid_out <= `ET_TB_HOLD 1'b1;
                  do begin
                     @(posedge clk);
                  end while (ar_ready !== 1'b1);
                  if (TRUE_AR_VC_CREDITS > 0) begin
                     m_ar_credits[(8 * ar_idx) +: 8] = m_ar_credits[(8 * ar_idx) +: 8] - 8'h01;
                  end
                  ar_valid_out <= `ET_TB_HOLD 1'b0;
                  m_ar_req &= req_next;
               end
            end
         end
      endgenerate


      //--------------------------------------------------------------------------------------------
      //
      // m_aw_req:        This bit vector contains request bits for AW bus requests on a per VC
      //                  basis.  In addition, the most significant bit will be set if there is a
      //                  request in progress.
      // m_aw_cycle:      This bit vector contains cycle count information for delaying AW bus
      //                  requests on a per VC basis.  Cycle count vectors awe currently 32 bits
      //                  wide.
      // m_aw_bus_reqs[]: This memory awray contains the bus request infomation per VC.
      //
      bit [TRUE_AW_VC_WIDTH:0]   m_aw_req             = {(TRUE_AW_VC_WIDTH + 1){1'b0}};
      bit [31:0]                 m_aw_req_cycle_delay = 0;
      mailbox #(evl_axi_bus_req) m_aw_bus_req_mbs[TRUE_AW_VC_WIDTH];

      genvar aw_idx;
      generate
         for (aw_idx = 0; aw_idx < TRUE_AW_VC_WIDTH; aw_idx++) begin :aw_req_loop
            evl_axi_bus_req          bus_req;
            bit [31:0]               req_cycle;
            bit [31:0]               req_cycle_delay;
            bit [TRUE_AW_VC_WIDTH:0] req_mask  = 8'hff << aw_idx;
            bit [TRUE_AW_VC_WIDTH:0] req_value = 8'h01 << aw_idx;
            bit [TRUE_AW_VC_WIDTH:0] req_next  = ~((8'h01 << TRUE_AW_VC_WIDTH) | (8'h01 << aw_idx));

            initial begin
               m_aw_bus_req_mbs[aw_idx] = new();
               forever begin
                  m_aw_bus_req_mbs[aw_idx].get(bus_req);
                  m_aw_req[aw_idx] = 1'b1;
                  wait (m_aw_req[aw_idx] === 1'b0);
               end
            end

            always begin
               wait (reset === 1'b0);
               #0 wait ((m_aw_req[aw_idx] === 1'b1) || (reset !== 1'b0));
               req_cycle_delay = 0;
               if (m_aw_req[aw_idx] === 1'b1) begin
                  req_cycle = bus_req.get_cycle();
                  if ((m_delays_enabled == 1'b1) && (req_cycle > m_cycle_count_posedge[31:0]) && (aw_idx == 0)) begin
                     req_cycle_delay = req_cycle - m_cycle_count_posedge[31:0];
                  end
               end
               #0 wait ( (reset !== 1'b0) ||
                         ( ((m_delays_enabled == 1'b0) || (req_cycle <= m_cycle_count_posedge[31:0])) &&
                           ((TRUE_AW_VC_CREDITS <= 0) || (m_aw_credits[(8 * aw_idx) +: 8] > 8'h00)) ) );
               if (reset === 1'b0) begin
                  #0 wait ((m_aw_req & req_mask) === req_value);
                  m_aw_req[TRUE_AW_VC_WIDTH] = 1'b1;

                  if ((m_delays_enabled == 1'b1) && (req_cycle <= m_cycle_count_posedge[31:0]) && (aw_idx == 0)) begin
                     m_aw_req_cycle_delay <= req_cycle_delay;
                  end
                  aw_id_out       <= `ET_TB_HOLD bus_req.get_ax_id();
                  aw_addr_out     <= `ET_TB_HOLD bus_req.get_ax_addr();
                  aw_len_out      <= `ET_TB_HOLD bus_req.get_ax_len();
                  aw_size_out     <= `ET_TB_HOLD bus_req.get_ax_size();
                  aw_burst_out    <= `ET_TB_HOLD bus_req.get_ax_burst();
                  aw_lock_out     <= `ET_TB_HOLD bus_req.get_ax_lock();
                  aw_cache_out    <= `ET_TB_HOLD bus_req.get_ax_cache();
                  aw_prot_out     <= `ET_TB_HOLD bus_req.get_ax_prot();
                  aw_qos_out      <= `ET_TB_HOLD bus_req.get_ax_qos();
                  aw_user_out     <= `ET_TB_HOLD bus_req.get_ax_user();
                  aw_trans_id_out <= `ET_TB_HOLD bus_req.get_trans_id();
                  if (IGNORE_REGION == 0) begin
                     aw_region_out <= `ET_TB_HOLD bus_req.get_ax_region();
                  end
                  if (TRUE_AW_VC_CREDITS > 0) begin
                     aw_vc_valid_out <= `ET_TB_HOLD (1 << aw_idx);
                  end
                  aw_valid_out <= `ET_TB_HOLD 1'b1;
                  do begin
                     @(posedge clk);
                  end while (aw_ready !== 1'b1);
                  if (TRUE_AW_VC_CREDITS > 0) begin
                     m_aw_credits[(8 * aw_idx) +: 8] = m_aw_credits[(8 * aw_idx) +: 8] - 8'h01;
                  end
                  aw_valid_out <= `ET_TB_HOLD 1'b0;
                  m_aw_req &= req_next;
               end
            end
         end
      endgenerate


      //--------------------------------------------------------------------------------------------
      //
      // m_w_req:        This bit vector contains request bits for AW bus requests on a per VC
      //                 basis. In addition, the most significant bit will be set if there is a
      //                 request in progress.
      // m_w_cycle:      This bit vector contains cycle count information for delaying AW bus
      //                 requests on a per VC basis.  Cycle count vectors we currently 32 bits wide.
      // m_w_bus_reqs[]: This memory array contains the bus request infomation per VC.
      //
      bit [TRUE_AW_VC_WIDTH:0]   m_w_req             = {(TRUE_AW_VC_WIDTH + 1){1'b0}};
      bit [31:0]                 m_w_req_cycle_delay = 0;
      mailbox #(evl_axi_bus_req) m_w_bus_req_mbs[TRUE_AW_VC_WIDTH];

      genvar w_idx;
      generate
         for (w_idx = 0; w_idx < TRUE_AW_VC_WIDTH; w_idx++) begin :w_req_loop
            evl_axi_bus_req          bus_req;
            bit [31:0]               req_cycle;
            bit [31:0]               req_cycle_delay;
            bit [TRUE_AW_VC_WIDTH:0] req_mask  = 8'hff << w_idx;
            bit [TRUE_AW_VC_WIDTH:0] req_value = 8'h01 << w_idx;
            bit [TRUE_AW_VC_WIDTH:0] req_next  = ~((8'h01 << TRUE_AW_VC_WIDTH) | (8'h01 << w_idx));

            initial begin
               m_w_bus_req_mbs[w_idx] = new();
               forever begin
                  m_w_bus_req_mbs[w_idx].get(bus_req);
                  m_w_req[w_idx] = 1'b1;
                  wait (m_w_req[w_idx] === 1'b0);
               end
            end

            always begin
               wait (reset === 1'b0);
               #0 wait ((m_w_req[w_idx] === 1'b1) || (reset !== 1'b0));
               if (m_w_req[w_idx] === 1'b1) begin
                  req_cycle = bus_req.get_data_cycle();
                  if ((m_delays_enabled == 1'b1) && (req_cycle > m_cycle_count_posedge[31:0]) && (w_idx == 0)) begin
                     req_cycle_delay = req_cycle - m_cycle_count_posedge[31:0];
                  end
               end
               #0 wait ( (reset !== 1'b0) ||
                         ( ((m_delays_enabled == 1'b0) || (req_cycle <= m_cycle_count_posedge[31:0])) &&
                           ((TRUE_AW_VC_CREDITS <= 0) || (m_w_credits[(8 * w_idx) +: 8] > 8'h00)) ) );
               if (reset === 1'b0) begin
                  bit                 last;
                  int                 limit;
                  evl_xword_t         w_data_r;
                  evl_xword_byte_en_t w_strb_r;

                  #0 wait ((m_w_req & req_mask) === req_value);
                  m_w_req[TRUE_AW_VC_WIDTH] = 1'b1;

                  if ((m_delays_enabled == 1'b1) && (req_cycle <= m_cycle_count_posedge[31:0]) && (w_idx == 0)) begin
                     m_w_req_cycle_delay <= req_cycle_delay;
                  end
                  w_data_r = bus_req.get_w_data();
                  w_strb_r = bus_req.get_w_strb();
                  limit    = bus_req.get_ax_len();
                  if (WSTRB_WIDTH < `EVL_LINE_BYTES) begin
                     w_data_r = w_data_r >> (8 * (bus_req.get_addr_offset() & LINE_ADDR_MASK));
                     w_strb_r = w_strb_r >> (bus_req.get_addr_offset() & LINE_ADDR_MASK);
                  end
                  for (int ii = 0; ii <= limit; ii++) begin
                     w_data_out     <= `ET_TB_HOLD w_data_r;
                     w_strb_out     <= `ET_TB_HOLD w_strb_r;
                     w_last_out     <= `ET_TB_HOLD ((ii == limit) ? 1'b1 : 1'b0);
                     w_trans_id_out <= `ET_TB_HOLD bus_req.get_trans_id();
                     if (IGNORE_NON_REQ_USER == 0) begin
                        w_user_out <= `ET_TB_HOLD bus_req.get_w_user();
                     end
                     if (TRUE_AW_VC_CREDITS > 0) begin
                        w_vc_valid_out <= `ET_TB_HOLD (1 << w_idx);
                     end
                     w_valid_out <= `ET_TB_HOLD 1'b1;
                     do begin
                        @(posedge clk);
                     end while (w_ready !== 1'b1);
                     w_data_r = w_data_r >> (WSTRB_WIDTH * 8);
                     w_strb_r = w_strb_r >> (WSTRB_WIDTH);
                  end
                  if (TRUE_AW_VC_CREDITS > 0) begin
                     m_w_credits[(8 * w_idx) +: 8] = m_w_credits[(8 * w_idx) +: 8] - 8'h01;
                  end
                  w_valid_out <= `ET_TB_HOLD 1'b0;
                  m_w_req &= req_next;
               end
            end
         end
      endgenerate


      //--------------------------------------------------------------------------------------------
      //
      // Response Processing
      //
      bit [31:0]      m_trigger_r_rsp_cycle;
      bit [31:0]      m_trigger_b_rsp_cycle;
      evl_axi_bus_rsp m_r_rsp_q[$];
      evl_axi_bus_rsp m_b_rsp_q[$];
      mailbox         m_aw_vc_credit_mb;


      //--------------------------------------------------------------------------------------------
      //
      // Function: update_r_rsp_trigger
      //
      // This function is used to update the B response trigger cycle.
      //
      function void update_r_rsp_trigger(input int trigger_in, input bit reset);
         if (reset == 1'b1) begin
            m_trigger_r_rsp_cycle = 32'h7fffffff;
         end
         else if (m_trigger_r_rsp_cycle > trigger_in) begin
            m_trigger_r_rsp_cycle = trigger_in;
         end
      endfunction : update_r_rsp_trigger


      //--------------------------------------------------------------------------------------------
      //
      // Task: send_r
      //
      // This task drives the response interface signals with values from the bus response object
      // received from the driver via the mailbox.
      //
      task send_r(evl_axi_bus_rsp bus_rsp);
         r_id_out       <= `ET_TB_HOLD bus_rsp.get_x_id();
         r_data_out     <= `ET_TB_HOLD bus_rsp.get_x_data();
         r_resp_out     <= `ET_TB_HOLD bus_rsp.get_x_resp();
         r_last_out     <= `ET_TB_HOLD bus_rsp.get_x_last();
         r_trans_id_out <= `ET_TB_HOLD bus_rsp.get_trans_id();
         if (IGNORE_NON_REQ_USER == 0) begin
            r_user_out <= `ET_TB_HOLD bus_rsp.get_x_user();
         end
         r_valid_out <= `ET_TB_HOLD 1'b1;
         do begin
            @(posedge clk);
         end while (r_ready !== 1'b1);
         r_valid_out <= 1'b0;
      endtask : send_r


      //--------------------------------------------------------------------------------------------
      //
      // Task: send_next_r_rsp
      //
      // This task parses the R response queue and sends the next possible response.
      //
      task send_next_r_rsp;
         int idx;
         int last_idx;
         bit rsp_sent;
         bit out_of_order;

         update_r_rsp_trigger(32'h7fffffff, 1'b1);
         last_idx = m_r_rsp_q.size();
         rsp_sent = 1'b0;
         if (m_delays_enabled == 1'b0) begin
            while (last_idx > 0) begin
               rsp_sent = 1'b1;
               send_r(m_r_rsp_q[0]);
               m_r_rsp_q.delete(0);
               last_idx--;
            end
            if (m_delays_enabled == 1'b0) begin
               if (m_r_rsp_q.size() > 0) begin
                  update_r_rsp_trigger(0, 1'b0);
               end
               return;
            end
            last_idx = m_r_rsp_q.size();
         end
         if (last_idx > 0) begin
            //
            // Paw through the list of responses to send.  Each response contains the cycle number
            // at which to issue.  If we have reached the issue time, push the response on the
            // response mailbox and remove it from the response queue.  If we have not reached the
            // issue time, set the next issue time to the lowest possible value.
            //
            idx          = 0;
            out_of_order = 1'b0;
            while ((idx < last_idx) && (m_delays_enabled == 1'b1)) begin
               int cur_trigger;

               cur_trigger = m_r_rsp_q[idx].get_cycle();
               if (cur_trigger <= m_cycle_count_posedge) begin
                  `ifdef EVL_SIMULATION // --------------------------------------------------------{
                     if (m_axi_cov != null) begin
                        int delay;

                        delay = m_r_rsp_q[idx].get_mem_bus_rsp_delay();
                        if (delay > 0) begin
                           m_axi_cov.sample_r_delay(delay, out_of_order);
                        end
                     end
                  `endif // ifdef EVL_SIMULATION --------------------------------------------------}
                  rsp_sent = 1'b1;
                  send_r(m_r_rsp_q[idx]);
                  m_r_rsp_q.delete(idx);
                  last_idx--;
               end
               else begin
                  if (cur_trigger < m_trigger_r_rsp_cycle) begin
                     update_r_rsp_trigger(cur_trigger, 1'b0);
                  end
                  out_of_order = 1'b1;
                  idx++;
               end
               if (m_delays_enabled == 1'b0) begin
                  update_r_rsp_trigger(0, 1'b0);
               end
            end
         end
         if (rsp_sent == 1'b0) begin
            @(posedge clk);
         end
      endtask : send_next_r_rsp


      //--------------------------------------------------------------------------------------------
      //
      // Function: update_b_rsp_trigger
      //
      // This function is used to update the B response trigger cycle.
      //
      function void update_b_rsp_trigger(input int trigger_in, input bit reset);
         if (reset == 1'b1) begin
            m_trigger_b_rsp_cycle = 32'h7fffffff;
         end
         else if (m_trigger_b_rsp_cycle > trigger_in) begin
            m_trigger_b_rsp_cycle = trigger_in;
         end
      endfunction : update_b_rsp_trigger


      //--------------------------------------------------------------------------------------------
      //
      // Task: send_b
      //
      // This task drives the response interface signals with values from the bus response object.
      //
      task send_b(evl_axi_bus_rsp bus_rsp);
         b_id_out       <= `ET_TB_HOLD bus_rsp.get_x_id();
         b_resp_out     <= `ET_TB_HOLD bus_rsp.get_x_resp();
         b_trans_id_out <= `ET_TB_HOLD bus_rsp.get_trans_id();
         if (IGNORE_NON_REQ_USER == 0) begin
            b_user_out <= `ET_TB_HOLD bus_rsp.get_x_user();
         end
         b_valid_out <= `ET_TB_HOLD 1'b1;
         do begin
            @(posedge clk);
         end while (b_ready !== 1'b1);
         b_valid_out <= `ET_TB_HOLD 1'b0;

         if (AW_VC_CREDITS != 0) begin
            m_aw_vc_credit_mb.get(m_aw_vc_credit_r);
            aw_vc_credit_out <= `ET_TB_HOLD m_aw_vc_credit_r;
            @(posedge clk);
            aw_vc_credit_out <= `ET_TB_HOLD {AW_VC_WIDTH{1'b0}};
         end
      endtask : send_b


      //--------------------------------------------------------------------------------------------
      //
      // Task: send_next_b_rsp
      //
      // This task parses the B response queue and sends the next possible response.
      //
      task send_next_b_rsp;
         int idx;
         int last_idx;
         bit rsp_sent;
         bit out_of_order;

         update_b_rsp_trigger(32'h7fffffff, 1'b1);
         last_idx = m_b_rsp_q.size();
         rsp_sent = 1'b0;
         if (m_delays_enabled == 1'b0) begin
            while (last_idx > 0) begin
               rsp_sent = 1'b1;
               send_b(m_b_rsp_q[0]);
               m_b_rsp_q.delete(0);
               last_idx--;
            end
            if (m_delays_enabled == 1'b0) begin
               if (m_b_rsp_q.size() > 0) begin
                  update_b_rsp_trigger(0, 1'b0);
               end
               return;
            end
            last_idx = m_b_rsp_q.size();
         end
         if (last_idx > 0) begin
            int idx;
            bit out_of_order;

            //
            // Paw through the list of responses to send.  Each response contains the cycle number
            // at which to issue.  If we have reached the issue time, push the response on the
            // response mailbox and remove it from the response queue.  If we have not reached the
            // issue time, set the next issue time to the lowest possible value.
            //
            idx          = 0;
            out_of_order = 1'b0;
            while ((idx < last_idx) && (m_delays_enabled == 1'b1)) begin
               int cur_trigger;

               cur_trigger = m_b_rsp_q[idx].get_cycle();
               if (cur_trigger <= m_cycle_count_posedge) begin
                  `ifdef EVL_SIMULATION // --------------------------------------------------------{
                     if (m_axi_cov != null) begin
                        int delay;

                        delay = m_b_rsp_q[idx].get_mem_bus_rsp_delay();
                        if (delay > 0) begin
                           m_axi_cov.sample_b_delay(delay, out_of_order);
                        end
                     end
                  `endif // ifdef EVL_SIMULATION --------------------------------------------------}
                  rsp_sent = 1'b1;
                  send_b(m_b_rsp_q[idx]);
                  m_b_rsp_q.delete(idx);
                  last_idx--;
               end
               else begin
                  if (cur_trigger < m_trigger_b_rsp_cycle) begin
                     update_b_rsp_trigger(cur_trigger, 1'b0);
                  end
                  out_of_order = 1'b1;
                  idx++;
               end
               if (m_delays_enabled == 1'b0) begin
                  update_b_rsp_trigger(0, 1'b0);
               end
            end
         end
         if (rsp_sent == 1'b0) begin
            @(posedge clk);
         end
      endtask : send_next_b_rsp


      //--------------------------------------------------------------------------------------------
      //
      // Task: get_and_send_reqs
      //
      task get_and_send_reqs;
         evl_axi_bus_req  bus_req;
         evl_delay_t      req_delay;
         evl_delay_t      req_data_delay;
         evl_bus_req_vc_t vc;
         int unsigned     next_aw_cycle;
         int unsigned     next_w_cycle;
         int unsigned     next_ar_cycle;

         next_aw_cycle = 0;
         next_w_cycle  = 0;
         next_ar_cycle = 0;
         forever begin
            m_rtl_port.get_req(bus_req);

            req_delay      = bus_req.get_bus_req_delay();
            req_data_delay = bus_req.get_bus_req_data_delay();
            vc             = evl_bus_req_vc_t'(bus_req.get_ax_vc());
            if (bus_req.get_req_type() == AXI_REQ_WRITE) begin
               // figure out when we can send the AW
               next_aw_cycle = ((m_cycle_count[31:0] + req_delay) > next_aw_cycle) ? m_cycle_count[31:0] + req_delay : next_aw_cycle + 1;
               bus_req.set_cycle(next_aw_cycle);

               // figure out when we can send the W
               next_w_cycle = ((m_cycle_count[31:0] + req_data_delay) > next_w_cycle) ? m_cycle_count[31:0] + req_data_delay : next_w_cycle + 1;
               // If this stim agent is not allowed to send W before AW, fix that:
               if (ALLOW_DRV_W_BEFORE_AW == 0) begin
                  if (next_w_cycle < next_aw_cycle) begin
                     next_w_cycle = next_aw_cycle;
                  end
               end
               bus_req.set_data_cycle(next_w_cycle);

               if (vc < TRUE_AW_VC_WIDTH) begin
                  m_aw_bus_req_mbs[vc].put(bus_req);
                  m_w_bus_req_mbs[vc].put(bus_req);
               end
               else begin
                  m_aw_bus_req_mbs[0].put(bus_req);
                  m_w_bus_req_mbs[0].put(bus_req);
               end
            end
            else begin // AXI_REQ_READ
               // figure out when we can send the AR
               next_ar_cycle = ((m_cycle_count[31:0] + req_delay) > next_ar_cycle) ? m_cycle_count[31:0] + req_delay : next_ar_cycle + 1;
               bus_req.set_cycle(next_ar_cycle);

               if (vc < TRUE_AR_VC_WIDTH) begin
                  m_ar_bus_req_mbs[vc].put(bus_req);
               end
               else begin
                  m_ar_bus_req_mbs[0].put(bus_req);
               end
            end
         end
      endtask : get_and_send_reqs


      //--------------------------------------------------------------------------------------------
      //
      // Task: get_and_send_rsps
      //
      task get_and_send_rsps;
         evl_axi_bus_rsp bus_rsp;
         int             next_rsp_cycle;

         forever begin
            m_rtl_port.get_rsp(bus_rsp);

            if (m_delays_enabled == 1'b0) begin
               next_rsp_cycle = m_cycle_count_posedge;
               bus_rsp.set_mem_bus_rsp_delay(0);
            end
            else begin
               next_rsp_cycle = bus_rsp.get_mem_bus_rsp_delay() + m_cycle_count_posedge;
            end
            bus_rsp.set_cycle(next_rsp_cycle);
            if (bus_rsp.rsp_is_for_write() == 1'b0) begin
               m_r_rsp_q.push_back(bus_rsp);
               update_r_rsp_trigger(next_rsp_cycle, 1'b0);
            end
            else begin
               m_b_rsp_q.push_back(bus_rsp);
               update_b_rsp_trigger(next_rsp_cycle, 1'b0);
            end
         end
      endtask : get_and_send_rsps;


      //--------------------------------------------------------------------------------------------
      //
      // Request and Response Stimulus Processing
      //
      initial begin
         m_aw_vc_credit_mb = new();
         m_trigger_r_rsp_cycle = 32'h7fffffff;
         m_trigger_b_rsp_cycle = 32'h7fffffff;
         #0;
         fork
            begin
               get_and_send_reqs();
            end
            begin
               get_and_send_rsps();
            end
            forever begin
               wait (m_trigger_r_rsp_cycle <= m_cycle_count_posedge);
               send_next_r_rsp();
            end
            forever begin
               wait (m_trigger_b_rsp_cycle <= m_cycle_count_posedge);
               send_next_b_rsp();
            end
         join
      end


      initial begin
         #0 m_rtl_port.wait_for_master_driver();
         aw_id_out        <= {ID_WIDTH{1'b0}};
         aw_addr_out      <= {ADDR_WIDTH{1'b0}};
         aw_len_out       <= {AXLEN_WIDTH{1'b0}};
         aw_size_out      <= {AXSIZE_WIDTH{1'b0}};
         aw_burst_out     <= {AXBURST_WIDTH{1'b0}};
         aw_lock_out      <= 1'b0;
         aw_cache_out     <= {AXCACHE_WIDTH{1'b0}};
         aw_prot_out      <= {AXPROT_WIDTH{1'b0}};
         aw_qos_out       <= {AXQOS_WIDTH{1'b0}};
         aw_region_out    <= {AXREGION_WIDTH{1'b0}};
         aw_user_out      <= {AWUSER_WIDTH{1'b0}};
         aw_vc_valid_out  <= {AW_VC_WIDTH{1'b0}};
         aw_trans_id_out  <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
         aw_valid_out     <= 1'b0;
         w_data_out       <= {DATA_WIDTH{1'b0}};
         w_strb_out       <= {WSTRB_WIDTH{1'b0}};
         w_last_out       <= 1'b0;
         w_user_out       <= {WUSER_WIDTH{1'b0}};
         w_vc_valid_out   <= {AW_VC_WIDTH{1'b0}};
         w_trans_id_out   <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
         w_valid_out      <= 1'b0;
         ar_id_out        <= {ID_WIDTH{1'b0}};
         ar_addr_out      <= {ADDR_WIDTH{1'b0}};
         ar_len_out       <= {AXLEN_WIDTH{1'b0}};
         ar_size_out      <= {AXSIZE_WIDTH{1'b0}};
         ar_burst_out     <= {AXBURST_WIDTH{1'b0}};
         ar_lock_out      <= 1'b0;
         ar_cache_out     <= {AXCACHE_WIDTH{1'b0}};
         ar_prot_out      <= {AXPROT_WIDTH{1'b0}};
         ar_qos_out       <= {AXQOS_WIDTH{1'b0}};
         ar_region_out    <= {AXREGION_WIDTH{1'b0}};
         ar_user_out      <= {ARUSER_WIDTH{1'b0}};
         ar_vc_valid_out  <= {AR_VC_WIDTH{1'b0}};
         ar_trans_id_out  <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
         ar_valid_out     <= 1'b0;
         b_ready_out      <= 1'b0;
         r_ready_out      <= 1'b0;
         m_request_master <= 1'b1;
//       #0 m_rtl_port.wait_for_master_driver_enable();
//       b_ready_out      <= 1'b1;
//       r_ready_out      <= 1'b1;
      end


      initial begin
         #0 m_rtl_port.wait_for_slave_driver();
         b_id_out          <= {ID_WIDTH{1'b0}};
         b_resp_out        <= {BRESP_WIDTH{1'b0}};
         b_user_out        <= {BUSER_WIDTH{1'b0}};
         b_trans_id_out    <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
         aw_vc_credit_out  <= {AW_VC_WIDTH{1'b0}};
         b_valid_out       <= 1'b0;
         r_id_out          <= {ID_WIDTH{1'b0}};
         r_data_out        <= {DATA_WIDTH{1'b0}};
         r_resp_out        <= {RRESP_WIDTH{1'b0}};
         r_last_out        <= 1'b0;
         r_user_out        <= {RUSER_WIDTH{1'b0}};
         r_trans_id_out    <= {(`EVL_TRANS_ID_MSB+1){1'b0}};
         r_valid_out       <= 1'b0;
         aw_ready_out      <= 1'b0;
         w_ready_out       <= 1'b0;
         ar_ready_out      <= 1'b0;
         m_response_master <= 1'b1;
//       #0 m_rtl_port.wait_for_slave_driver_enable();
//       aw_ready_out      <= 1'b1;
//       w_ready_out       <= 1'b1;
//       ar_ready_out      <= 1'b1;
      end


      initial begin
         #0 m_delays_enabled = 1'b0;
         forever begin
            m_rtl_port.wait_for_delay_enable();
            #0 m_delays_enabled = 1'b1;
            m_rtl_port.wait_for_delay_disable();
            #0 m_delays_enabled = 1'b0;
         end
      end


      wire unused_ok = &{ 1'b0,
                          ar_vc_valid,
                          ar_vc_credit,
                          aw_vc_valid,
                          aw_vc_credit,
                          r_last,
                          w_last,
                          isolate,
                          1'b0 };


      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         int pk_data_bytes = evl_param_xref::get_param_key("pk_data_bytes");
         int pk_gate_ar_ready_key;
         int pk_gate_aw_ready_key;
         int pk_gate_w_ready_key;
         int pk_gate_r_ready_key;
         int pk_gate_b_ready_key;
         int pk_ar_backp_assert_key;
         int pk_ar_backp_deassert_key;
         int pk_aw_backp_assert_key;
         int pk_aw_backp_deassert_key;
         int pk_w_backp_assert_key;
         int pk_w_backp_deassert_key;
         int pk_b_backp_assert_key;
         int pk_b_backp_deassert_key;
         int pk_r_backp_assert_key;
         int pk_r_backp_deassert_key;


         initial begin
            m_rtl_port.set_int_param(pk_data_bytes, (DATA_WIDTH/8));
            m_rtl_port.m_data_bytes      = (DATA_WIDTH/8);
            m_rtl_port.m_data_bytes_log2 = $clog2(DATA_WIDTH/8);

            pk_gate_ar_ready_key     = evl_param_xref::get_param_key("pk_gate_ar_ready");
            pk_gate_aw_ready_key     = evl_param_xref::get_param_key("pk_gate_aw_ready");
            pk_gate_w_ready_key      = evl_param_xref::get_param_key("pk_gate_w_ready");
            pk_gate_r_ready_key      = evl_param_xref::get_param_key("pk_gate_r_ready");
            pk_gate_b_ready_key      = evl_param_xref::get_param_key("pk_gate_b_ready");
            pk_ar_backp_assert_key   = evl_param_xref::get_param_key("pk_rreq_ready_assert_count");
            pk_ar_backp_deassert_key = evl_param_xref::get_param_key("pk_rreq_ready_deassert_count");
            pk_aw_backp_assert_key   = evl_param_xref::get_param_key("pk_wreq_ready_assert_count");
            pk_aw_backp_deassert_key = evl_param_xref::get_param_key("pk_wreq_ready_deassert_count");
            pk_w_backp_assert_key    = evl_param_xref::get_param_key("pk_wdata_ready_assert_count");
            pk_w_backp_deassert_key  = evl_param_xref::get_param_key("pk_wdata_ready_deassert_count");
            pk_b_backp_assert_key    = evl_param_xref::get_param_key("pk_wrsp_ready_assert_count");
            pk_b_backp_deassert_key  = evl_param_xref::get_param_key("pk_wrsp_ready_deassert_count");
            pk_r_backp_assert_key    = evl_param_xref::get_param_key("pk_rrsp_ready_assert_count");
            pk_r_backp_deassert_key  = evl_param_xref::get_param_key("pk_rrsp_ready_deassert_count");

            #0;
            if (MAX_PORT_ID > 0) begin
               m_abstract_name = $sformatf("%0s(AXI-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
            end
            else begin
               m_abstract_name = $sformatf("%0s(IAXI-FC)", m_rtl_port.get_root_abstract_name());
            end
            wait (m_reset_asserted == 1'b1);
            if (MAX_PORT_ID > 0) begin
               m_abstract_name = $sformatf("%0s(AXI-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
            end
            else begin
               m_abstract_name = $sformatf("%0s(AXI-IFC)", m_rtl_port.get_root_abstract_name());
            end
            wait (m_reset_deasserted == 1'b1);
            if (MAX_PORT_ID > 0) begin
               m_abstract_name = $sformatf("%0s(AXI-IFC%0d)", m_rtl_port.get_root_abstract_name(), PORT_ID);
            end
            else begin
               m_abstract_name = $sformatf("%0s(AXI-IFC)", m_rtl_port.get_root_abstract_name());
            end
            #0;

            //-------------------------------------------------------------------------------------{
            //
            // The following code fragment is used solely for AXI ready backpressure.
            //
            fork
               begin
                  wait (m_response_master == 1'b1);
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_ar_backp_assert_key is a delay key that contains the number of cycles to
                     // keep AR_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting ARREADY"))
                     ar_ready_out <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_ar_backp_assert_key);
                     //
                     // pk_ar_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep AR_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting ARREADY"))
                     ar_ready_out <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_ar_backp_deassert_key);
                  end
               end
               begin
                  wait ((m_response_master == 1'b1) && (m_lock_w_rdy == 1'b1));
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_aw_backp_assert_key is a delay key that contains the number of cycles to
                     // keep AW_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting AWREADY and WREADY"))
                     aw_ready_out <= `ET_TB_HOLD 1'b1;
                     w_ready_out  <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_aw_backp_assert_key);
                     //
                     // pk_aw_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep AW_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting AWREADY and WREADY"))
                     aw_ready_out <= `ET_TB_HOLD 1'b0;
                     w_ready_out  <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_aw_backp_deassert_key);
                  end
               end
               begin
                  wait ((m_response_master == 1'b1) && (m_lock_w_rdy == 1'b0));
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_aw_backp_assert_key is a delay key that contains the number of cycles to
                     // keep AW_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting AWREADY"))
                     aw_ready_out <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_aw_backp_assert_key);
                     //
                     // pk_aw_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep AW_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting AWREADY"))
                     aw_ready_out <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_aw_backp_deassert_key);
                  end
               end
               begin
                  wait ((m_response_master == 1'b1) && (m_lock_w_rdy == 1'b0));
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_w_backp_assert_key is a delay key that contains the number of cycles to
                     // keep W_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting WREADY"))
                     w_ready_out <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_w_backp_assert_key);
                     //
                     // pk_w_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep W_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting WREADY"))
                     w_ready_out <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_w_backp_deassert_key);
                  end
               end
               begin
                  wait (m_request_master == 1'b1);
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_r_backp_assert_key is a delay key that contains the number of cycles to
                     // keep R_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting RREADY"))
                     r_ready_out <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_r_backp_assert_key);
                     //
                     // pk_r_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep R_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting RREADY"))
                     r_ready_out <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_r_backp_deassert_key);
                  end
               end
               begin
                  wait (m_request_master == 1'b1);
                  wait (reset === 1'b0);
                  forever begin
                     //
                     // pk_b_backp_assert_key is a delay key that contains the number of cycles to
                     // keep B_READY asserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("asserting BREADY"))
                     b_ready_out <= `ET_TB_HOLD 1'b1;
                     m_rtl_port.wait_to_assert_backp(pk_b_backp_assert_key);
                     //
                     // pk_b_backp_deassert_key is a delay key that contains the number of cycles to
                     // keep B_READY deasserted.
                     //
                     #0;
                     `evl_log(`UVM_FULL, VMOD_AXI_IFC, m_abstract_name, $sformatf("deasserting BREADY"))
                     b_ready_out <= `ET_TB_HOLD 1'b0;
                     m_rtl_port.wait_to_deassert_backp(pk_b_backp_deassert_key);
                  end
               end
               begin
                  wait (m_response_master == 1'b1);
                  wait (reset === 1'b0);
                  fork
                     begin
                        m_rtl_port.wait_for_int_param(pk_gate_ar_ready_key, 1, 1);
                        `evl_log(`UVM_MEDIUM, VMOD_AXI_IFC, m_abstract_name, $sformatf("configuring ARREADY assertion to depend on ARVALID assertion"))
                        forever @(negedge clk) begin
                           m_ar_ready_mask <= (ar_valid === 1'b1) ? 1'b1 : 1'b0;
                        end
                     end
                     begin
                        m_rtl_port.wait_for_int_param(pk_gate_aw_ready_key, 1, 1);
                        `evl_log(`UVM_MEDIUM, VMOD_AXI_IFC, m_abstract_name, $sformatf("configuring AWREADY assertion to depend on AWVALID assertion"))
                        forever @(negedge clk) begin
                           m_aw_ready_mask <= (aw_valid === 1'b1) ? 1'b1 : 1'b0;
                        end
                     end
                     begin
                        m_rtl_port.wait_for_int_param(pk_gate_w_ready_key, 1, 1);
                        `evl_log(`UVM_MEDIUM, VMOD_AXI_IFC, m_abstract_name, $sformatf("configuring WREADY assertion to depend on WVALID assertion"))
                        forever @(negedge clk) begin
                           m_w_ready_mask <= (w_valid === 1'b1) ? 1'b1 : 1'b0;
                        end
                     end
                  join
               end
               begin
                  wait (m_request_master == 1'b1);
                  wait (reset === 1'b0);
                  fork
                     begin
                        m_rtl_port.wait_for_int_param(pk_gate_r_ready_key, 1, 1);
                        `evl_log(`UVM_MEDIUM, VMOD_AXI_IFC, m_abstract_name, $sformatf("configuring RREADY assertion to depend on RVALID assertion"))
                        forever @(negedge clk) begin
                           m_r_ready_mask <= (r_valid === 1'b1) ? 1'b1 : 1'b0;
                        end
                     end
                     begin
                        m_rtl_port.wait_for_int_param(pk_gate_b_ready_key, 1, 1);
                        `evl_log(`UVM_MEDIUM, VMOD_AXI_IFC, m_abstract_name, $sformatf("configuring BREADY assertion to depend on BVALID assertion"))
                        forever @(negedge clk) begin
                           m_b_ready_mask <= (b_valid === 1'b1) ? 1'b1 : 1'b0;
                        end
                     end
                  join
               end
            join
            // End of AXI ready backpressure ------------------------------------------------------}
         end

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}


      always @(posedge clk) begin // Main monitor loop --------------------------------------------{
         m_ar_valid_req = 1'b0;
         m_aw_valid_req = 1'b0;
         m_w_valid_req  = 1'b0;
         m_r_valid_rsp  = 1'b0;
         m_b_valid_rsp  = 1'b0;
         m_delay_ping   = 1'b0;
         if (reset === 1'b0) begin
            if ((ar_valid === 1'b1) && (ar_ready === 1'b1)) begin
               m_ar_valid_req          = 1'b1;
               m_ar_req_mon.ax_is_read = 1'b1;
               m_ar_req_mon.ax_id      = ar_id;
               m_ar_req_mon.ax_addr    = ar_addr;
               m_ar_req_mon.ax_len     = ar_len;
               m_ar_req_mon.ax_size    = ar_size;
               m_ar_req_mon.ax_burst   = ar_burst;
               m_ar_req_mon.ax_lock    = ar_lock;
               m_ar_req_mon.ax_cache   = ar_cache;
               m_ar_req_mon.ax_prot    = ar_prot;
               m_ar_req_mon.ax_qos     = ar_qos;
               m_ar_req_mon.ax_region  = ar_region;
               m_ar_req_mon.ax_user    = ar_user;
               m_ar_req_mon.trans_id   = ar_trans_id;
               m_ar_vc_valid_r         = ar_vc_valid;
            end

            if ((aw_valid === 1'b1) && (aw_ready === 1'b1)) begin
               m_aw_valid_req          = 1'b1;
               m_aw_req_mon.ax_is_read = 1'b0;
               m_aw_req_mon.ax_id      = aw_id;
               m_aw_req_mon.ax_addr    = aw_addr;
               m_aw_req_mon.ax_len     = aw_len;
               m_aw_req_mon.ax_size    = aw_size;
               m_aw_req_mon.ax_burst   = aw_burst;
               m_aw_req_mon.ax_lock    = aw_lock;
               m_aw_req_mon.ax_cache   = aw_cache;
               m_aw_req_mon.ax_prot    = aw_prot;
               m_aw_req_mon.ax_qos     = aw_qos;
               m_aw_req_mon.ax_region  = aw_region;
               m_aw_req_mon.ax_user    = aw_user;
               m_aw_req_mon.trans_id   = aw_trans_id;
               m_aw_vc_valid_r         = aw_vc_valid;
            end

            if ((w_valid === 1'b1) && (w_ready === 1'b1)) begin
               m_w_valid_req        = 1'b1;
               m_w_req_mon.w_data   = w_data;
               m_w_req_mon.w_strb   = w_strb;
               m_w_req_mon.w_user   = w_user;
               m_w_req_mon.w_last   = w_last;
               m_w_req_mon.trans_id = w_trans_id;
            end

            if ((r_valid === 1'b1) && (r_ready === 1'b1)) begin
               m_r_valid_rsp         = 1'b1;
               m_r_rsp_mon.x_is_read = 1'b1;
               m_r_rsp_mon.x_id      = r_id;
               m_r_rsp_mon.x_resp    = evl_axi_resp_t'(r_resp);
               m_r_rsp_mon.x_data    = r_data;
               m_r_rsp_mon.x_user    = r_user;
               m_r_rsp_mon.x_last    = r_last;
               m_r_rsp_mon.trans_id  = r_trans_id;
            end

            if ((b_valid === 1'b1) && (b_ready === 1'b1)) begin
               m_b_valid_rsp         = 1'b1;
               m_b_rsp_mon.x_is_read = 1'b0;
               m_b_rsp_mon.x_id      = b_id;
               m_b_rsp_mon.x_resp    = evl_axi_resp_t'(b_resp);
               m_b_rsp_mon.x_user    = b_user;
               m_b_rsp_mon.x_last    = 1'b1;
               m_b_rsp_mon.trans_id  = b_trans_id;
            end

            if (m_ar_valid_req == 1'b1) begin
               m_ar_req_mon.ax_vc = 0;
               if (TRUE_AR_VC_CREDITS > 0) begin
                  evl_bus_req_vc_t           vc;
                  bit [TRUE_AR_VC_WIDTH-1:0] ar_vc;

                  vc    = evl_bus_req_vc_t'(0);
                  ar_vc = m_ar_vc_valid_r;
                  while (ar_vc != {TRUE_AR_VC_WIDTH{1'b0}}) begin
                     if (ar_vc[0:0] == 1'b1) begin
                        m_ar_req_mon.ax_vc = vc;
                        break;
                     end
                     ar_vc = ar_vc >> 1;
                     vc    = vc + evl_bus_req_vc_t'(1);
                  end
               end
            end

            if (m_aw_valid_req == 1'b1) begin
               m_aw_req_mon.ax_vc = 0;
               if (TRUE_AW_VC_CREDITS > 0) begin
                  evl_bus_req_vc_t           vc;
                  bit [TRUE_AW_VC_WIDTH-1:0] aw_vc;

                  vc    = evl_bus_req_vc_t'(0);
                  aw_vc = m_aw_vc_valid_r;
                  while (aw_vc != {TRUE_AW_VC_WIDTH{1'b0}}) begin
                     if (aw_vc[0:0] == 1'b1) begin
                        m_aw_req_mon.ax_vc = vc;
                        break;
                     end
                     aw_vc = aw_vc >> 1;
                     vc    = vc + evl_bus_req_vc_t'(1);
                  end
               end
            end

            `ifdef EVL_SIMULATION // --------------------------------------------------------------{

               if (`evl_log_check_verbosity(`UVM_HIGH, VMOD_AXI_IFC) != 0) begin
                  if (m_ar_valid_req == 1'b1) begin
                     `evl_log_addr_trid_id(`UVM_HIGH, VMOD_AXI_IFC, m_abstract_name, m_ar_req_mon.ax_addr, m_ar_req_mon.trans_id, m_ar_req_mon.ax_id, $sformatf("AXI_REQ_READ 0x%010x, qos=%0d, user=0x%x", m_ar_req_mon.ax_addr, m_ar_req_mon.ax_qos, m_ar_req_mon.ax_user))
                  end
                  if (m_aw_valid_req == 1'b1) begin
                     `evl_log_addr_trid_id(`UVM_HIGH, VMOD_AXI_IFC, m_abstract_name, m_aw_req_mon.ax_addr, m_aw_req_mon.trans_id, m_aw_req_mon.ax_id, $sformatf("AXI_REQ_WRITE 0x%010x, qos=%0d, user=0x%x", m_aw_req_mon.ax_addr, m_aw_req_mon.ax_qos, m_aw_req_mon.ax_user))
                  end
                  if (m_w_valid_req == 1'b1) begin
                     `evl_log_addr_trid_id(`UVM_HIGH, VMOD_AXI_IFC, m_abstract_name, ~(evl_paddr_t'(0)), m_w_req_mon.trans_id, ~(evl_bus_req_id_t'(0)), $sformatf("W Data 0x%x, user=0x%x, strb=0x%x, last=%b", m_w_req_mon.w_data, m_w_req_mon.w_user, m_w_req_mon.w_strb, m_w_req_mon.w_last))
                  end
                  if (m_r_valid_rsp == 1'b1) begin
                     axi_resp_t rsp_type;

                     rsp_type = axi_resp_t'(m_r_rsp_mon.x_resp);
                     `evl_log_addr_trid_id(`UVM_HIGH, VMOD_AXI_IFC, m_abstract_name, ~(evl_paddr_t'(0)), m_r_rsp_mon.trans_id, m_r_rsp_mon.x_id, $sformatf("%p 0x%x, user=0x%x, last=%b", rsp_type, m_r_rsp_mon.x_data, m_r_rsp_mon.x_user, m_r_rsp_mon.x_last))
                  end
                  if (m_b_valid_rsp == 1'b1) begin
                     axi_resp_t rsp_type;

                     rsp_type = axi_resp_t'(m_b_rsp_mon.x_resp);
                     `evl_log_addr_trid_id(`UVM_HIGH, VMOD_AXI_IFC, m_abstract_name, ~(evl_paddr_t'(0)), m_b_rsp_mon.trans_id, m_b_rsp_mon.x_id, $sformatf("%p, user=0x%x", rsp_type, m_b_rsp_mon.x_user))
                  end
               end

            `endif // ifdef EVL_SIMULATION --------------------------------------------------------}

            if (m_aw_valid_req == 1'b1) begin
               if (DELAY_REQ_ZTIME == 0) begin
                  m_rtl_port.capture_req(m_aw_req_mon);
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req(m_aw_req_mon);
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_ping = 1'b1;
               end
            end
            if (m_w_valid_req == 1'b1) begin
               if (DELAY_REQ_ZTIME == 0) begin
                  m_rtl_port.capture_req_data(m_w_req_mon);
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req_data(m_w_req_mon);
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_ping = 1'b1;
               end
            end
            if (m_ar_valid_req == 1'b1) begin
               if (DELAY_REQ_ZTIME == 0) begin
                  m_rtl_port.capture_req(m_ar_req_mon);
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req(m_ar_req_mon);
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_ping = 1'b1;
               end
            end
            if (m_b_valid_rsp == 1'b1) begin
               if (DELAY_RSP_ZTIME == 0) begin
                  m_rtl_port.capture_rsp(m_b_rsp_mon);
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.capture_rsp(m_b_rsp_mon);
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_ping = 1'b1;
               end
            end
            if (m_r_valid_rsp == 1'b1) begin
               if (DELAY_RSP_ZTIME == 0) begin
                  m_rtl_port.capture_rsp(m_r_rsp_mon);
                  `ifdef ET_AXI_INCLUDE_USR_PORT
                     m_rtl_port2.capture_rsp(m_r_rsp_mon);
                  `endif // ifdef ET_AXI_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_ping = 1'b1;
               end
            end

            if ((aw_valid === 1'b1) && (aw_ready === 1'b1)) begin
               m_aw_vc_credit_mb.put(aw_vc_valid);
            end
         end


         if (m_delay_ping == 1'b0) begin
            m_rtl_port.ping_clock();
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.ping_clock();
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         else begin
            if (DELAY_REQ_ZTIME != 0) begin
               -> m_capture_req;
            end
            if (DELAY_RSP_ZTIME != 0) begin
               -> m_capture_rsp;
            end
         end
      end // End of main monitor loop -------------------------------------------------------------}


      always @(m_capture_req) begin
         repeat (DELAY_REQ_ZTIME) #0;
         if (m_aw_valid_req == 1'b1) begin
            m_rtl_port.capture_req(m_aw_req_mon);
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.capture_req(m_aw_req_mon);
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         if (m_w_valid_req == 1'b1) begin
            m_rtl_port.capture_req_data(m_w_req_mon);
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.capture_req_data(m_w_req_mon);
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         if (m_ar_valid_req == 1'b1) begin
            m_rtl_port.capture_req(m_ar_req_mon);
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.capture_req(m_ar_req_mon);
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         if (DELAY_REQ_ZTIME > DELAY_RSP_ZTIME) begin
            m_rtl_port.ping_clock();
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.ping_clock();
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
      end


      always @(m_capture_rsp) begin
         repeat (DELAY_RSP_ZTIME) #0;
         if (m_b_valid_rsp == 1'b1) begin
             m_rtl_port.capture_rsp(m_b_rsp_mon);
             `ifdef ET_AXI_INCLUDE_USR_PORT
                m_rtl_port2.capture_rsp(m_b_rsp_mon);
             `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         if (m_r_valid_rsp == 1'b1) begin
             m_rtl_port.capture_rsp(m_r_rsp_mon);
             `ifdef ET_AXI_INCLUDE_USR_PORT
                m_rtl_port2.capture_rsp(m_r_rsp_mon);
             `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
         if (DELAY_RSP_ZTIME >= DELAY_REQ_ZTIME) begin
            m_rtl_port.ping_clock();
            `ifdef ET_AXI_INCLUDE_USR_PORT
               m_rtl_port2.ping_clock();
            `endif // ifdef ET_AXI_INCLUDE_USR_PORT
         end
      end

      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         initial begin
            bit err_on_clock_x;

            wait (m_reset_asserted == 1'b1);
            wait (m_reset_deasserted == 1'b1);
            err_on_clock_x = (`et_test_plusargs("err_on_clock_x") != 0) ? 1'b1 : 1'b0;
            forever begin
               //
               // Wait for clock to initially go to a non-X value.
               //
               wait ((clk === 1'b0) || (clk === 1'b1));

               //
               // Wait for clock go to X (as long as isolate is not asserted).
               //
               wait ((clk !== 1'b0) && (clk !== 1'b1) && (isolate !== 1'b1));
               if (err_on_clock_x == 1'b1) begin
                  `dut_error($sformatf("%0s(AXI-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("input clock is %b ... forcing clocks", clk))
               end
               else begin
                  `dut_warning($sformatf("%0s(AXI-IFC)", m_rtl_port.get_root_abstract_name()), $sformatf("input clock is %b ... forcing clocks", clk))
               end
               while ((clk !== 1'b0) && (clk !== 1'b1)) begin
                  #5ns;
                  m_cycle_count_posedge <= m_cycle_count_posedge + 32'h1;
                  m_rtl_port.ping_clock();
                  #5ns;
                  m_cycle_count <= m_cycle_count_posedge;
               end
            end
         end

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}


      always @(posedge clk) begin
         m_cycle_count_posedge <= m_cycle_count_posedge + 32'h1;
      end


      always @(negedge clk) begin
         m_cycle_count <= m_cycle_count_posedge;
      end


      generate
         if (IGNORE_REGION != 0) begin
            assign ar_region = {AXREGION_WIDTH{1'b0}};
            assign aw_region = {AXREGION_WIDTH{1'b0}};
         end
         if (IGNORE_NON_REQ_USER != 0) begin
            assign w_user = {WUSER_WIDTH{1'b0}};
            assign r_user = {RUSER_WIDTH{1'b0}};
            assign b_user = {BUSER_WIDTH{1'b0}};
         end
      endgenerate

      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         `include "dv/common/cov/evl_axi_int_ass.svh"

         wire [64:0]  w_strb_cov = { {(65-WSTRB_WIDTH){1'b0}}, w_strb[(WSTRB_WIDTH-1):0] };
         wire [512:0] w_data_cov = { {(512-DATA_WIDTH){1'b0}}, w_data[(DATA_WIDTH-1):0] };
         wire [512:0] r_data_cov = { {(512-DATA_WIDTH){1'b0}}, r_data[(DATA_WIDTH-1):0] };

         initial begin
            int       cov_type;
            int       ar_idle_count;
            int       ar_busy_count;
            int       aw_idle_count;
            int       aw_busy_count;
            int       w_idle_count;
            int       w_busy_count;
            int       r_idle_count;
            int       r_busy_count;
            int       b_idle_count;
            int       b_busy_count;
            int       ar_req_cycle_delay_last;
            int       aw_req_cycle_delay_last;
            int       w_req_cycle_delay_last;
            bit [2:0] concurrent_req;
            bit [1:0] concurrent_rsp;

            #0;
            m_rtl_port.wait_for_coverage_enabled();
            cov_type = int'(AXI_COV_TYPE);
            if (cov_type != int'(evl_base_pkg::EVL_COV_AXI_NONE)) begin
               m_axi_cov = new(m_request_master, m_response_master, AR_REQ_LIMIT, AW_REQ_LIMIT);
               m_rtl_port.set_object_param_by_name("pk_axi_cov_object", m_axi_cov);
               ar_idle_count = 0;
               ar_busy_count = 0;
               aw_idle_count = 0;
               aw_busy_count = 0;
               w_idle_count  = 0;
               w_busy_count  = 0;
               r_idle_count  = 0;
               r_busy_count  = 0;
               b_idle_count  = 0;
               b_busy_count  = 0;
               ar_req_cycle_delay_last = 0;
               aw_req_cycle_delay_last = 0;
               w_req_cycle_delay_last  = 0;
               forever @(posedge clk) begin
                  if (ar_valid === 1'b1) begin
                     if (ar_ready === 1'b1) begin
                        m_axi_cov.sample_ar(ar_addr, ar_id, ar_size, ar_qos, ar_idle_count, ar_busy_count);
                        if ((m_ar_req_cycle_delay != 0) && (m_ar_req_cycle_delay != ar_req_cycle_delay_last)) begin
                           m_axi_cov.sample_ar_delay(m_ar_req_cycle_delay);
                        end
                        ar_req_cycle_delay_last = m_ar_req_cycle_delay;
                        ar_idle_count = 0;
                        ar_busy_count = 0;
                     end
                     else if (ar_ready === 1'b0) begin
                        ar_busy_count++;
                     end
                  end
                  else begin
                     ar_idle_count++;
                     ar_busy_count = 0;
                  end

                  if (aw_valid === 1'b1) begin
                     if (aw_ready === 1'b1) begin
                        m_axi_cov.sample_aw(aw_addr, aw_id, aw_size, aw_qos, aw_user, aw_idle_count, aw_busy_count);
                        if ((m_aw_req_cycle_delay != 0) && (m_aw_req_cycle_delay != aw_req_cycle_delay_last)) begin
                           m_axi_cov.sample_aw_delay(m_aw_req_cycle_delay);
                        end
                        aw_req_cycle_delay_last = m_aw_req_cycle_delay;
                        aw_idle_count = 0;
                        aw_busy_count = 0;
                     end
                     else if (aw_ready === 1'b0) begin
                        aw_busy_count++;
                     end
                  end
                  else begin
                     aw_idle_count++;
                     aw_busy_count = 0;
                  end

                  if (w_valid === 1'b1) begin
                     if (w_ready === 1'b1) begin
                        m_axi_cov.sample_w(w_strb_cov[63:0], w_data_cov[511:0], w_idle_count, w_busy_count);
                        if ((m_w_req_cycle_delay != 0) && (m_w_req_cycle_delay != w_req_cycle_delay_last)) begin
                           m_axi_cov.sample_w_delay(m_w_req_cycle_delay);
                        end
                        w_req_cycle_delay_last = m_w_req_cycle_delay;
                        w_idle_count = 0;
                        w_busy_count = 0;
                     end
                     else if (w_ready === 1'b0) begin
                        w_busy_count++;
                     end
                  end
                  else begin
                     w_idle_count++;
                     w_busy_count = 0;
                  end

                  if (r_valid === 1'b1) begin
                     if (r_ready === 1'b1) begin
                        m_axi_cov.sample_r(r_resp, r_data_cov[511:0], r_idle_count, r_busy_count);
                        r_idle_count = 0;
                        r_busy_count = 0;
                     end
                     else if (r_ready === 1'b0) begin
                        r_busy_count++;
                     end
                  end
                  else begin
                     r_idle_count++;
                     r_busy_count = 0;
                  end

                  if (b_valid === 1'b1) begin
                     if (b_ready === 1'b1) begin
                        m_axi_cov.sample_b(b_resp, b_idle_count, b_busy_count);
                        b_idle_count = 0;
                        b_busy_count = 0;
                     end
                     else if (b_ready === 1'b0) begin
                        b_busy_count++;
                     end
                  end
                  else begin
                     b_idle_count++;
                     b_busy_count = 0;
                  end

                  concurrent_req = { (w_ready & w_valid), (aw_ready & aw_valid), (ar_ready & ar_valid) };
                  concurrent_rsp = { (b_ready & b_valid), (r_ready & r_valid) };
                  if (concurrent_req !== 3'b000) begin
                     m_axi_cov.sample_concurrent_req(concurrent_req);
                  end
                  if (concurrent_rsp !== 2'b00) begin
                     m_axi_cov.sample_concurrent_rsp(concurrent_rsp);
                  end
               end
            end
         end

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}

   `endif // ifdef ET_MONITOR_IFCS ----------------------------------------------------------------}

endmodule

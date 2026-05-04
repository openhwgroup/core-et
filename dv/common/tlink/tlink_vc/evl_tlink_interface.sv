//--------------------------------------------------------------------------------------------------
//
//
// Interface: evl_tlink_interface
//
`include "rtl/inc/soc.vh"

`ifndef EVL_IFC_PORT
   `ifdef EVL_SIMULATION
      `define EVL_IFC_PORT inout
   `else
      `define EVL_IFC_PORT input
   `endif
`endif

`include "dv/common/tlink/tlink_vc/evl_tlink_pkg.sv"

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

module evl_tlink_interface #(parameter int    AGENT_ID        = 0,
                             parameter string ABSTRACT_NAME   = "",
                             parameter int    TLINK_COV_TYPE  = 0,
                             parameter int    ADDR_WIDTH      = 40,
                             parameter int    DATA_WIDTH      = 128,
                             parameter int    DATA_EN_WIDTH   = DATA_WIDTH/8,
                             parameter int    DATA_SZ_WIDTH   = 4,
                             parameter int    PARAM_WIDTH     = 3,
                             parameter int    ASOURCE_WIDTH   = 5,
                             parameter int    BSOURCE_WIDTH   = 5,
                             parameter int    SINK_WIDTH      = 4,
                             parameter int    DELAY_REQ_ZTIME = 0,
                             parameter int    DELAY_RSP_ZTIME = 0,
                             parameter int    EARLY_ACQUIRE   = 0,
                             parameter int    SKIP_LEVELS[]   = {0}) (
      input         wire                     isolate,
      input         wire                     reset,
      input         wire                     clk,
      `EVL_IFC_PORT wire                     a_ready,
      `EVL_IFC_PORT wire                     a_valid,
      `EVL_IFC_PORT wire [2:0]               a_opcode,
      `EVL_IFC_PORT wire [PARAM_WIDTH-1:0]   a_param,
      `EVL_IFC_PORT wire [DATA_SZ_WIDTH-1:0] a_size,
      `EVL_IFC_PORT wire [ASOURCE_WIDTH-1:0] a_source,
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]    a_address,
      `EVL_IFC_PORT wire [DATA_EN_WIDTH-1:0] a_mask,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]    a_data,
      `EVL_IFC_PORT wire                     a_corrupt,
      `EVL_IFC_PORT wire                     b_ready,
      `EVL_IFC_PORT wire                     b_valid,
      `EVL_IFC_PORT wire [2:0]               b_opcode,
      `EVL_IFC_PORT wire [1:0]               b_param,
      `EVL_IFC_PORT wire [DATA_SZ_WIDTH-1:0] b_size,
      `EVL_IFC_PORT wire [BSOURCE_WIDTH-1:0] b_source,
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]    b_address,
      `EVL_IFC_PORT wire [DATA_EN_WIDTH-1:0] b_mask,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]    b_data,
      `EVL_IFC_PORT wire                     b_corrupt,
      `EVL_IFC_PORT wire                     c_ready,
      `EVL_IFC_PORT wire                     c_valid,
      `EVL_IFC_PORT wire [2:0]               c_opcode,
      `EVL_IFC_PORT wire [PARAM_WIDTH-1:0]   c_param,
      `EVL_IFC_PORT wire [DATA_SZ_WIDTH-1:0] c_size,
      `EVL_IFC_PORT wire [BSOURCE_WIDTH-1:0] c_source,
      `EVL_IFC_PORT wire [ADDR_WIDTH-1:0]    c_address,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]    c_data,
      `EVL_IFC_PORT wire                     c_corrupt,
      `EVL_IFC_PORT wire                     d_ready,
      `EVL_IFC_PORT wire                     d_valid,
      `EVL_IFC_PORT wire [2:0]               d_opcode,
      `EVL_IFC_PORT wire [1:0]               d_param,
      `EVL_IFC_PORT wire [DATA_SZ_WIDTH-1:0] d_size,
      `EVL_IFC_PORT wire [ASOURCE_WIDTH-1:0] d_source,
      `EVL_IFC_PORT wire [SINK_WIDTH-1:0]    d_sink,
      `EVL_IFC_PORT wire                     d_denied,
      `EVL_IFC_PORT wire [DATA_WIDTH-1:0]    d_data,
      `EVL_IFC_PORT wire                     d_corrupt,
      `EVL_IFC_PORT wire                     e_ready,
      `EVL_IFC_PORT wire                     e_valid,
      `EVL_IFC_PORT wire [SINK_WIDTH-1:0]    e_sink
   );

   `ifdef ET_MONITOR_IFCS // ----------------------------------------------------------------------{

      localparam int DELAY_CRSP_ZTIME = ((DELAY_RSP_ZTIME & 32'h00000100) != 0) ? (DELAY_RSP_ZTIME & 32'h0000000f) : 32'h00000000;
      localparam int DELAY_DRSP_ZTIME = ((DELAY_RSP_ZTIME & 32'h00000200) != 0) ? (DELAY_RSP_ZTIME & 32'h0000000f) : 32'h00000000;
      localparam int DELAY_ERSP_ZTIME = ((DELAY_RSP_ZTIME & 32'h00000400) != 0) ? (DELAY_RSP_ZTIME & 32'h0000000f) : 32'h00000000;

      import evl_base_pkg::*;
      import evl_tlink_pkg::*;


      //--------------------------------------------------------------------------------------------
      //
      // Monitor clock & reset
      //
      int   m_ifc_cycle_count  = 0;
      bit   m_start_watching   = 1'b0;
      bit   m_reset_asserted   = 1'b0;
      bit   m_reset_deasserted = 1'b0;
      wire  m_clock            = clk & m_start_watching;
      bit   m_gen_areq;
      bit   m_gen_breq;
      bit   m_gen_creq;
      bit   m_gen_crsp;
      bit   m_gen_drsp;
      bit   m_gen_ersp;
      bit   m_delay_rsp;
      bit   m_delay_ping;
      event m_req_seen;
      event m_rsp_seen;

      evl_tlink_bus_req_s m_bus_areq;
      evl_tlink_bus_req_s m_bus_breq;
      evl_tlink_bus_req_s m_bus_creq;
      evl_tlink_bus_rsp_s m_bus_crsp;
      evl_tlink_bus_rsp_s m_bus_drsp;
      evl_tlink_bus_rsp_s m_bus_ersp;


      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         bit   m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), TLINK_BUS_AGENT, AGENT_ID, "evl_tlink_verif_comp", "", "", "", { SKIP_LEVELS });

         evl_tlink_rtl_port m_rtl_port = new($sformatf("%m"), AGENT_ID, { SKIP_LEVELS });

         initial begin
            if (ABSTRACT_NAME != "") begin
               m_rtl_port.set_abstract_name(ABSTRACT_NAME);
            end
            m_rtl_port.set_rtl_name($sformatf("%m"));
            m_rtl_port.set_int_param_by_name("pk_wdata_bytes", DATA_WIDTH / 8);
            m_rtl_port.set_int_param_by_name("pk_rdata_bytes", DATA_WIDTH / 8);
            m_rtl_port.m_req_data_bus_width = DATA_WIDTH / 8;
            m_rtl_port.m_rsp_data_bus_width = DATA_WIDTH / 8;
            if (EARLY_ACQUIRE != 0) begin
               m_rtl_port.set_int_param_by_name("pk_check_for_early_acquire", 1);
            end
         end

         `ifdef ET_ETLINK_INCLUDE_USR_PORT
            usr_tlink_rtl_port m_rtl_port2 = new($sformatf("%m"), AGENT_ID);

            initial begin
               if (ABSTRACT_NAME != "") begin
                  m_rtl_port2.set_abstract_name(ABSTRACT_NAME);
               end
               m_rtl_port2.set_rtl_name($sformatf("%m"));
               m_rtl_port2.set_bus_req_data_width(DATA_WIDTH / 8);
               m_rtl_port2.set_bus_rsp_data_width(DATA_WIDTH / 8);
            end
         `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT

      `else // ifdef EVL_SIMULATION }--------------------------------------------------------------{

         usr_tlink_rtl_port m_rtl_port = new($sformatf("%m"), AGENT_ID);

         initial begin
            if (ABSTRACT_NAME != "") begin
               m_rtl_port.set_abstract_name(ABSTRACT_NAME);
            end
            m_rtl_port.set_rtl_name($sformatf("%m"));
            m_rtl_port.set_bus_req_data_width(DATA_WIDTH / 8);
            m_rtl_port.set_bus_rsp_data_width(DATA_WIDTH / 8);
         end

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}


      evl_tlink_cov_pkg::evl_tlink_cov#(.TLINK_COV_TYPE(evl_tlink_pkg::tlink_cov_type_t'(TLINK_COV_TYPE))) m_tlink_cov;


      initial begin
         int req_idle_count;
         int req_busy_count;
         int rsp_idle_count;
         int rsp_busy_count;
         int cov_type;

         #0 cov_type = TLINK_COV_TYPE;
         if ((`et_test_plusargs("cover_on") != 0) && (cov_type != int'(EVL_COV_TLINK_NONE))) begin
//          m_rtl_port.wait_for_coverage_enabled();
            m_tlink_cov = new();
//          req_idle_count = 0;
//          req_busy_count = 0;
//          rsp_idle_count = 0;
//          rsp_busy_count = 0;
//          forever @(posedge clk) begin
//             if (m_req_valid_mon !== 0) begin
//                if ((m_req_valid_mon & m_req_ready_mon) !== 0) begin
//                   m_etlink_cov.sample_req(req_info.address, evl_etlink_types_pkg::evl_etlink_req_opcode_t'(req_info.opcode), req_info.id, req_info.size, req_info.source, req_info.qwen, req_info.subopcode, req_idle_count, req_busy_count);
//                   req_idle_count = 0;
//                   req_busy_count = 0;
//                end
//                else begin
//                   req_busy_count++;
//                end
//             end
//             else begin
//                req_idle_count++;
//                req_busy_count = 0;
//             end
//             if (rsp_valid !== 0) begin
//                if ((rsp_valid & rsp_ready) !== 0) begin
//                   m_etlink_cov.sample_rsp(evl_etlink_types_pkg::evl_etlink_rsp_opcode_t'(rsp_info.opcode), rsp_info.id, rsp_info.size, rsp_info.dest, rsp_info.qwen, rsp_idle_count, rsp_busy_count);
//                   rsp_idle_count = 0;
//                   rsp_busy_count = 0;
//                end
//                else begin
//                   rsp_busy_count++;
//                end
//             end
//             else begin
//                rsp_idle_count++;
//                rsp_busy_count = 0;
//             end
//          end
         end
      end


      //--------------------------------------------------------------------------------------------
      //
      // Main Monitor Loop
      //
      initial begin
         m_reset_asserted   <= 1'b0;
         m_reset_deasserted <= 1'b0;
         m_start_watching   <= 1'b1;
         #0 wait (m_clock == 1'b0);
         forever @(posedge m_clock) begin
            m_gen_areq   = 1'b0;
            m_gen_breq   = 1'b0;
            m_gen_creq   = 1'b0;
            m_gen_crsp   = 1'b0;
            m_gen_drsp   = 1'b0;
            m_gen_ersp   = 1'b0;
            m_delay_rsp  = 1'b0;
            m_delay_ping = 1'b0;
            if (reset !== 1'b0) begin
               if (m_reset_asserted == 1'b0) begin
                  m_reset_asserted   <= 1'b1;
                  m_reset_deasserted <= 1'b0;
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.assert_reset();
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
            end
            else begin
               if (m_reset_deasserted == 1'b0) begin
                  m_reset_asserted   <= 1'b0;
                  m_reset_deasserted <= 1'b1;
                  m_rtl_port.deassert_reset();
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.deassert_reset();
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               if ((e_valid === 1'b1) && (e_ready === 1'b1)) begin
                  m_bus_ersp.opcode    = evl_tlink_rsp_opcode_t'(`EVL_TL_RSP_CHANNEL_E);
                  m_bus_ersp.param     = evl_tlink_param_t'(0);
                  m_bus_ersp.data_size = evl_bus_req_size_t'(0);
                  m_bus_ersp.source    = ~evl_bus_req_id_t'(0);
                  m_bus_ersp.sink      = evl_bus_req_id_t'(e_sink);
                  m_bus_ersp.data      = evl_xword_t'(0);
                  m_bus_ersp.denied    = 1'b0;
                  m_bus_ersp.corrupt   = 1'b0;
                  m_gen_ersp           = 1'b1;
               end
               if ((d_valid === 1'b1) && (d_ready === 1'b1)) begin
                  m_bus_drsp.opcode    = evl_tlink_rsp_opcode_t'(`EVL_TL_RSP_CHANNEL_D | d_opcode);
                  m_bus_drsp.data_size = evl_bus_req_size_t'(d_size);
                  m_bus_drsp.source    = evl_bus_req_id_t'(d_source);
                  m_bus_drsp.sink      = evl_bus_req_id_t'(d_sink);
                  m_bus_drsp.data      = evl_xword_t'(d_data);
                  m_bus_drsp.denied    = d_denied;
                  m_bus_drsp.corrupt   = d_corrupt;
                  if (m_bus_drsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
                     m_bus_drsp.param = evl_tlink_param_t'(`EVL_TL_PARAM_GRANT | d_param);
                  end
                  else begin
                     m_bus_drsp.param = evl_tlink_param_t'(d_param);
                  end
                  m_gen_drsp = 1'b1;
               end
               if ((c_valid === 1'b1) && (c_ready === 1'b1)) begin
                  if ((c_opcode == 3'b110) || (c_opcode == 3'b111)) begin
                     m_bus_creq.trans_id = evl_trans_id_t'(0);
                     m_bus_creq.opcode   = evl_tlink_req_opcode_t'(`EVL_TL_REQ_CHANNEL_C | c_opcode);
                     m_bus_creq.cmd_size = evl_bus_req_size_t'(c_size);
                     m_bus_creq.source   = evl_bus_req_id_t'(c_source);
                     m_bus_creq.address  = evl_paddr_t'(c_address);
                     m_bus_creq.mask     = evl_xword_byte_en_t'((1 << DATA_WIDTH) - 1);
                     m_bus_creq.data     = evl_xword_t'(c_data);
                     m_bus_creq.corrupt  = c_corrupt;
                     if (m_bus_creq.opcode inside { TLC_Release, TLC_ReleaseData }) begin
                        m_bus_creq.param = evl_tlink_param_t'(`EVL_TL_PARAM_RELEASE | c_param);
                     end
                     else begin
                        m_bus_creq.param = evl_tlink_param_t'(c_param);
                     end
                     m_gen_creq = 1'b1;
                  end
                  else begin
                     m_bus_crsp.opcode    = evl_tlink_rsp_opcode_t'(`EVL_TL_RSP_CHANNEL_C | c_opcode);
                     m_bus_crsp.data_size = evl_bus_req_size_t'(c_size);
                     m_bus_crsp.source    = evl_bus_req_id_t'(c_source);
                     m_bus_crsp.sink      = ~evl_bus_req_id_t'(0);
                     m_bus_crsp.data      = evl_xword_t'(c_data);
                     m_bus_crsp.denied    = 1'b0;
                     m_bus_crsp.corrupt   = c_corrupt;
                     if (m_bus_crsp.opcode inside { TLC_ProbeAck, TLC_ProbeAckData }) begin
                        m_bus_crsp.param = evl_tlink_param_t'(`EVL_TL_PARAM_PROBEACK | c_param);
                     end
                     else begin
                        m_bus_crsp.param = evl_tlink_param_t'(c_param);
                     end
                     m_gen_crsp = 1'b1;
                  end
               end
               if ((b_valid === 1'b1) && (b_ready === 1'b1)) begin
                  m_bus_breq.trans_id = evl_trans_id_t'(0);
                  m_bus_breq.opcode   = evl_tlink_req_opcode_t'(`EVL_TL_REQ_CHANNEL_B | b_opcode);
                  m_bus_breq.cmd_size = evl_bus_req_size_t'(b_size);
                  m_bus_breq.source   = evl_bus_req_id_t'(b_source);
                  m_bus_breq.address  = evl_paddr_t'(b_address);
                  m_bus_breq.mask     = evl_xword_byte_en_t'(b_mask);
                  m_bus_breq.data     = evl_xword_t'(b_data);
                  m_bus_breq.corrupt  = b_corrupt;
                  if (m_bus_breq.opcode inside { TLB_ProbeBlock, TLB_ProbePerm }) begin
                     m_bus_breq.param = evl_tlink_param_t'(`EVL_TL_PARAM_PROBE | b_param);
                  end
                  else if (m_bus_breq.opcode inside { TLB_AtomicArith }) begin
                     m_bus_breq.param = evl_tlink_param_t'(`EVL_TL_PARAM_ARITH_ATOMIC | b_param);
                  end
                  else if (m_bus_breq.opcode inside { TLB_AtomicLogic }) begin
                     m_bus_breq.param = evl_tlink_param_t'(`EVL_TL_PARAM_LOGIC_ATOMIC | b_param);
                  end
                  else if (m_bus_breq.opcode inside { TLB_Intent }) begin
                     m_bus_breq.param = evl_tlink_param_t'(`EVL_TL_PARAM_INTENT | b_param);
                  end
                  else begin
                     m_bus_breq.param = evl_tlink_param_t'(b_param);
                  end
                  m_gen_breq = 1'b1;
               end
               if ((a_valid === 1'b1) && (a_ready === 1'b1)) begin
                  m_bus_areq.trans_id = evl_trans_id_t'(0);
                  m_bus_areq.opcode   = evl_tlink_req_opcode_t'(`EVL_TL_REQ_CHANNEL_A | a_opcode);
                  m_bus_areq.cmd_size = evl_bus_req_size_t'(a_size);
                  m_bus_areq.source   = evl_bus_req_id_t'(a_source);
                  m_bus_areq.address  = evl_paddr_t'(a_address);
                  m_bus_areq.mask     = evl_xword_byte_en_t'(a_mask);
                  m_bus_areq.data     = evl_xword_t'(a_data);
                  m_bus_areq.corrupt  = a_corrupt;
                  if (m_bus_areq.opcode inside { TLA_AcquireBlock, TLA_AcquirePerm }) begin
                     m_bus_areq.param = evl_tlink_param_t'(`EVL_TL_PARAM_ACQUIRE | a_param);
                  end
                  else if (m_bus_areq.opcode inside { TLA_AtomicArith }) begin
                     m_bus_areq.param = evl_tlink_param_t'(`EVL_TL_PARAM_ARITH_ATOMIC | a_param);
                  end
                  else if (m_bus_areq.opcode inside { TLA_AtomicLogic }) begin
                     m_bus_areq.param = evl_tlink_param_t'(`EVL_TL_PARAM_LOGIC_ATOMIC | a_param);
                  end
                  else if (m_bus_areq.opcode inside { TLA_Intent }) begin
                     m_bus_areq.param = evl_tlink_param_t'(`EVL_TL_PARAM_INTENT | a_param);
                  end
                  else begin
                     m_bus_areq.param = evl_tlink_param_t'(a_param);
                  end
                  m_gen_areq = 1'b1;
               end
            end

            if (DELAY_REQ_ZTIME == 0) begin
               if (m_gen_creq == 1'b1) begin
                  m_rtl_port.capture_req(m_bus_creq);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req(m_bus_creq);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               if (m_gen_areq == 1'b1) begin
                  m_rtl_port.capture_req(m_bus_areq);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req(m_bus_areq);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               if (m_gen_breq == 1'b1) begin
                  m_rtl_port.capture_req(m_bus_breq);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_req(m_bus_breq);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
            end

            if (m_gen_ersp == 1'b1) begin
               if (DELAY_ERSP_ZTIME == 0) begin
                  m_rtl_port.capture_rsp(m_bus_ersp);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_rsp(m_bus_ersp);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_rsp = 1'b1;
               end
            end
            if (m_gen_drsp == 1'b1) begin
               if (DELAY_DRSP_ZTIME == 0) begin
                  m_rtl_port.capture_rsp(m_bus_drsp);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_rsp(m_bus_drsp);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_rsp = 1'b1;
               end
            end
            if (m_gen_crsp == 1'b1) begin
               if (DELAY_CRSP_ZTIME == 0) begin
                  m_rtl_port.capture_rsp(m_bus_crsp);
                  `ifdef ET_ETLINK_INCLUDE_USR_PORT
                     m_rtl_port2.capture_rsp(m_bus_crsp);
                  `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
               end
               else begin
                  m_delay_rsp = 1'b1;
               end
            end

            if ((DELAY_REQ_ZTIME != 0) && ((m_gen_areq == 1'b1) || (m_gen_breq == 1'b1) || (m_gen_creq == 1'b1))) begin
               m_delay_ping = 1'b1;
               -> m_req_seen;
            end
            if (m_delay_rsp == 1'b1) begin
               m_delay_ping = 1'b1;
               -> m_rsp_seen;
            end

            if (m_delay_ping == 1'b0) begin
               m_ifc_cycle_count++;
               m_rtl_port.ping_clock();
               `ifdef ET_ETLINK_INCLUDE_USR_PORT
                  m_rtl_port2.ping_clock();
               `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
            end

            if (m_tlink_cov != null) begin
               if (m_gen_areq == 1'b1) begin
                  m_tlink_cov.sample_req(m_bus_areq.opcode, m_bus_areq.address, m_bus_areq.param, m_bus_areq.cmd_size, m_bus_areq.source);
               end
               if (m_gen_breq == 1'b1) begin
                  m_tlink_cov.sample_req(m_bus_breq.opcode, m_bus_breq.address, m_bus_breq.param, m_bus_breq.cmd_size, m_bus_breq.source);
               end
               if (m_gen_creq == 1'b1) begin
                  m_tlink_cov.sample_req(m_bus_creq.opcode, m_bus_creq.address, m_bus_creq.param, m_bus_creq.cmd_size, m_bus_creq.source);
               end
               if (m_gen_crsp == 1'b1) begin
                  m_tlink_cov.sample_rsp(m_bus_crsp.opcode, m_bus_crsp.param, m_bus_crsp.source);
               end
               if (m_gen_drsp == 1'b1) begin
                  m_tlink_cov.sample_rsp(m_bus_drsp.opcode, m_bus_drsp.param, m_bus_drsp.source);
               end
               if (m_gen_ersp == 1'b1) begin
                  m_tlink_cov.sample_rsp(m_bus_ersp.opcode, m_bus_ersp.param, m_bus_ersp.sink);
               end
            end
         end
      end


      always @(m_req_seen) begin
         repeat (DELAY_REQ_ZTIME[3:0]) #0;
         if (m_gen_creq == 1'b1) begin
            m_rtl_port.capture_req(m_bus_creq);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_req(m_bus_creq);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if (m_gen_areq == 1'b1) begin
            m_rtl_port.capture_req(m_bus_areq);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_req(m_bus_areq);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if (m_gen_breq == 1'b1) begin
            m_rtl_port.capture_req(m_bus_breq);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_req(m_bus_breq);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if (DELAY_REQ_ZTIME[3:0] > DELAY_RSP_ZTIME[3:0]) begin
            m_ifc_cycle_count++;
            m_rtl_port.ping_clock();
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.ping_clock();
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
      end


      always @(m_rsp_seen) begin
         repeat (DELAY_RSP_ZTIME[3:0]) #0;
         if ((DELAY_ERSP_ZTIME != 0) && (m_gen_ersp == 1'b1)) begin
            m_rtl_port.capture_rsp(m_bus_ersp);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_rsp(m_bus_ersp);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if ((DELAY_DRSP_ZTIME != 0) && (m_gen_drsp == 1'b1)) begin
            m_rtl_port.capture_rsp(m_bus_drsp);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_rsp(m_bus_drsp);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if ((DELAY_CRSP_ZTIME != 0) && (m_gen_crsp == 1'b1)) begin
            m_rtl_port.capture_rsp(m_bus_crsp);
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.capture_rsp(m_bus_crsp);
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
         if (DELAY_RSP_ZTIME[3:0] >= DELAY_REQ_ZTIME[3:0]) begin
            m_ifc_cycle_count++;
            m_rtl_port.ping_clock();
            `ifdef ET_ETLINK_INCLUDE_USR_PORT
               m_rtl_port2.ping_clock();
            `endif // ifdef ET_ETLINK_INCLUDE_USR_PORT
         end
      end

   `endif // ifdef ET_MONITOR_IFCS ----------------------------------------------------------------}

endmodule

//--------------------------------------------------------------------------------------------------
//
// Interface: evl_jtag_interface
//
`include "rtl/inc/soc.vh"

`ifdef ET_SIMULATION
   `include "dv/common/jtag/jtag_vc/evl_jtag_pkg.sv"
`endif // ifdef ET_SIMULATION

module evl_jtag_interface(
      input  wire reset,
      input  wire clk,
      input  wire tdi,
      output wire tdo,
      output wire tms,
      output wire tck,
      output wire trst_n
   );

   `ifdef EVL_SIMULATION
      import uvm_pkg::*;
   `endif
   import evl_jtag_pkg::*;
   import evl_base_pkg::*;


   //-----------------------------------------------------------------------------------------------
   //
   // Registered IO Signals
   //
   reg m_tdi;
   reg m_tms;
   reg m_tck;
   reg m_trst;
   reg m_tdo_fe;
   reg m_tms_fe;
   reg m_trst_fe;
   reg m_clk_enable_fe;

   assign tck    = m_tck & m_clk_enable_fe;
   assign trst_n = ~m_trst_fe;
   assign tms    = m_tms_fe;
   assign tdo    = m_tdo_fe;


   //-----------------------------------------------------------------------------------------------
   //
   // Local Variables
   //
   jtag_state_t      m_next_state;
   jtag_state_t      m_state;
   jtag_state_t      m_last_state;
   bit [3:0]         m_reset_count;
   bit               m_skip_trst; 
   bit               m_allow_scan;

   bit               m_cmd_active;
   bit               m_last_cmd_active;
   bit               m_allow_shift;
   bit               m_use_tdi;
   bit               m_last_was_partial;
   bit               m_clk_enable;
   bit               m_clk_free_running;
   evl_cmd_t         m_jtag_cmd;
   evl_jtag_length_t m_chain_length;
   evl_jtag_length_t m_chain_count;
   evl_jtag_data_t   m_chain;

   bit               m_jtag_valid;
   bit               m_really_shift_zero;
   evl_cmd_t         m_next_jtag_cmd;
   evl_jtag_length_t m_next_chain_count;
   evl_jtag_data_t   m_next_chain;
   event             m_clk_event;

   reg               m_shift_ir;
   reg               m_shift_dr;


   initial begin
      m_tck = 1'b0;
      forever @(posedge clk) begin
         m_tck = ~m_tck;
      end
   end

   initial begin
      m_tdo_fe        <= 1'b1;
      m_tms_fe        <= 1'b1;
      m_trst_fe       <= 1'b1;
      m_shift_ir      <= 1'b0;
      m_shift_dr      <= 1'b0;
      m_clk_enable_fe <= 1'b1;
      forever @(negedge m_tck) begin
         m_tdo_fe        <= (m_use_tdi == 1'b0) ? m_chain[0:0] : m_tdi;
         m_tms_fe        <= m_tms;
         m_trst_fe       <= m_trst;
         m_clk_enable_fe <= m_clk_enable;
         m_shift_ir      <= (m_state == SHIFT_IR) ? 1'b1 : 1'b0;
         m_shift_dr      <= (m_state == SHIFT_DR) ? 1'b1 : 1'b0;
      end
   end


   //
   // Initialize the basic state.
   //
   initial begin
      m_tdi              <= 1'b0;
      m_tms              <= 1'b1;
      m_trst             <= 1'b0;
      m_reset_count      <= 4'h0;
      m_skip_trst        <= 1'h0;
      m_cmd_active       <= 1'b0;
      m_use_tdi          <= 1'b0;
      m_last_was_partial <= 1'b0;
      m_clk_enable       <= 1'b1;
      m_allow_shift      <= 1'b0;
      m_state            <= TAP_RESET;
      m_last_state       <= TAP_RESET;

      // By default the clk turns off when it's not needed.
      if (`et_test_plusargs("jtag_clk_free_running") != 0) begin
         m_clk_free_running = 1;
      end
      else begin
         m_clk_free_running = 0;
      end
   end


   //
   // Reset State Machine
   //
   always @(posedge m_tck) begin
      if (reset == 1'b1) begin
         m_tdi         <= 1'b0;
         m_tms         <= 1'b1;
         m_trst        <= 1'b1;
         m_reset_count <= 4'h0;
         m_skip_trst   <= 1'h0;
         m_allow_scan  <= 1'b0;
      end
      else if (m_reset_count < 4'h5) begin
         m_tdi         <= 1'b0;
         m_tms         <= 1'b1;
         m_trst        <= (m_skip_trst) ? 1'b0 : 1'b1;
         m_reset_count <= m_reset_count + 4'h1;
      end
      else if (m_reset_count == 4'h5) begin
         m_tdi         <= 1'b0;
         m_tms         <= 1'b1;
         m_trst        <= 1'b0;
         m_reset_count <= m_reset_count + 4'h1;
      end
      else if (m_reset_count == 4'h6) begin
         m_tdi         <= 1'b0;
         m_tms         <= 1'b1;
         m_trst        <= 1'b0;
         m_clk_enable  <= 1'b0;
         m_reset_count <= m_reset_count + 4'h1;
      end
      else begin
         m_tdi         <= ((m_allow_shift == 1'b1) && ((m_state == SHIFT_IR) || (m_state == SHIFT_DR))) ? tdi : m_tdi;
         m_allow_scan  <= 1'b1;
      end
   end


   always @(posedge m_tck) begin
      if (m_clk_enable_fe == 1'b0) begin
         m_next_state = m_state;
      end
      else begin
         //
         // JTAG State Machine
         //
         `ifdef EVL_SIMULATION
            if ((m_state == SHIFT_IR) || (m_state == SHIFT_DR)) begin
               `evl_log(UVM_HIGH, VMOD_JTAG, "JTAG(RTL)", $sformatf("JTAG state: %p (tdo:%b, tdi:%b)", m_state, tdo, tdi))
            end
            else if (m_state != m_last_state) begin
               `evl_log(UVM_HIGH, VMOD_JTAG, "JTAG(RTL)", $sformatf("JTAG state: %p", m_state))
            end
            m_last_state <= m_state;
         `endif // ifdef EVL_SIMULATION
         if (m_trst == 1'b1) begin
            m_next_state = TAP_RESET;
         end
         else begin
            case (m_state)
               TAP_RESET:  m_next_state = (m_tms_fe == 1'b1) ? TAP_RESET : RUN_TEST;
               RUN_TEST:   m_next_state = (m_tms_fe == 1'b1) ? SELECT_DR : RUN_TEST;
               SELECT_DR:  m_next_state = (m_tms_fe == 1'b1) ? SELECT_IR : CAPTURE_DR;
               CAPTURE_DR: m_next_state = (m_tms_fe == 1'b1) ? EXIT1_DR  : SHIFT_DR;
               SHIFT_DR:   m_next_state = (m_tms_fe == 1'b1) ? EXIT1_DR  : SHIFT_DR;
               EXIT1_DR:   m_next_state = (m_tms_fe == 1'b1) ? UPDATE_DR : PAUSE_DR;
               PAUSE_DR:   m_next_state = (m_tms_fe == 1'b1) ? EXIT2_DR  : PAUSE_DR;
               EXIT2_DR:   m_next_state = (m_tms_fe == 1'b1) ? UPDATE_DR : SHIFT_DR;
               UPDATE_DR:  m_next_state = (m_tms_fe == 1'b1) ? SELECT_DR : RUN_TEST;
               SELECT_IR:  m_next_state = (m_tms_fe == 1'b1) ? TAP_RESET : CAPTURE_IR;
               CAPTURE_IR: m_next_state = (m_tms_fe == 1'b1) ? EXIT1_IR  : SHIFT_IR;
               SHIFT_IR:   m_next_state = (m_tms_fe == 1'b1) ? EXIT1_IR  : SHIFT_IR;
               EXIT1_IR:   m_next_state = (m_tms_fe == 1'b1) ? UPDATE_IR : PAUSE_IR;
               PAUSE_IR:   m_next_state = (m_tms_fe == 1'b1) ? EXIT2_IR  : PAUSE_IR;
               EXIT2_IR:   m_next_state = (m_tms_fe == 1'b1) ? UPDATE_IR : SHIFT_IR;
               UPDATE_IR:  m_next_state = (m_tms_fe == 1'b1) ? SELECT_IR : RUN_TEST;
            endcase
         end
      end
      if (!(m_next_state inside { SHIFT_DR, EXIT1_DR, PAUSE_DR, EXIT2_DR })) begin
         m_last_was_partial <= 1'b0;
      end

      //
      // Scan the internal chain only when it is allowed.
      //
      if ((m_allow_shift == 1'b1) && (m_clk_enable_fe == 1'b1) && (m_state inside { SHIFT_IR, SHIFT_DR })) begin
         m_chain <= { tdi, m_chain[`EVL_MAX_JTAG_LENGTH - 1:1] };
      end

      //
      // If there is not a currently active command, attempt to get one.
      //
      if (m_cmd_active == 1'b0) begin
         if (m_jtag_valid == 1'b1) begin
            if (m_next_chain_count > `EVL_MAX_JTAG_LENGTH) begin
               m_chain_length <= `EVL_MAX_JTAG_LENGTH;
            end
            else begin
               m_chain_length <= m_next_chain_count;
            end
            m_jtag_cmd    <= m_next_jtag_cmd;
            if (m_next_jtag_cmd == JTAG_RESET) begin
               m_reset_count <= 4'h0; 
               m_skip_trst   <= 1'h1;
            end
            m_chain_count <= m_next_chain_count;
            m_chain       <= m_next_chain;
            m_cmd_active  <= 1'b1;
            m_jtag_valid  <= 1'b0;
            m_clk_enable  <= (m_next_jtag_cmd == JTAG_STEP) ? 1'b0 : 1'b1;
            m_really_shift_zero     <= (m_next_chain_count == 0) ? 1'b1 : 1'b0;
         end
         else begin
            if (m_clk_free_running == 1) begin
               m_clk_enable  <= (m_jtag_cmd == JTAG_STEP) ? 1'b0 : m_clk_enable;
            end
            else begin
               m_clk_enable  <=  1'b0;
            end
         end
         m_allow_shift <= 1'b0;
      end
      else begin
         //
         // Perform a step operation.
         //
         if (m_jtag_cmd == JTAG_STEP) begin
            if (m_chain_count != 0) begin
               m_tms         <= m_chain_count[0];
               m_use_tdi     <= 1'b0;
               m_allow_shift <= 1'b1;
               m_clk_enable  <= 1'b1;
               m_chain_count <= evl_jtag_length_t'(0);
            end
            else begin
               m_allow_shift <= 1'b0;
               m_clk_enable  <= 1'b0;
               m_cmd_active  <= 1'b0;
            end
         end
         //
         // Go to the test logic reset state.
         //
         if (m_jtag_cmd == JTAG_RESET) begin
            m_tms <= 1'b1;
            if ((m_next_state == TAP_RESET) && (m_reset_count > 5)) begin
               m_cmd_active <= 1'b0;
            end
         end

         //
         // Go to the run/test state.
         //
         else if (m_jtag_cmd == JTAG_RUN_TEST) begin
            if (m_next_state == RUN_TEST) begin
               m_tms        <= 1'b0;
               m_cmd_active <= 1'b0;
            end
            else begin
               case (m_next_state)
                  TAP_RESET:  m_tms <= 1'b0; // Go to RUN_TEST
                  RUN_TEST:   m_tms <= 1'b0; // Stay in RUN_TEST
                  SELECT_DR:  m_tms <= 1'b1; // Should not really be in this state
                  CAPTURE_DR: m_tms <= 1'b1; // Should not really be in this state
                  SHIFT_DR:   m_tms <= 1'b1; // Should not really be in this state
                  EXIT1_DR:   m_tms <= 1'b1; // Should not really be in this state
                  PAUSE_DR:   m_tms <= 1'b1; // Go to EXIT2_DR
                  EXIT2_DR:   m_tms <= 1'b1; // Go to UPDATE_DR
                  UPDATE_DR:  m_tms <= 1'b0; // Go to RUN_TEST
                  SELECT_IR:  m_tms <= 1'b1; // Should not really be in this state
                  CAPTURE_IR: m_tms <= 1'b1; // Should not really be in this state
                  SHIFT_IR:   m_tms <= 1'b1; // Should not really be in this state
                  EXIT1_IR:   m_tms <= 1'b1; // Should not really be in this state
                  PAUSE_IR:   m_tms <= 1'b1; // Go to EXIT2_IR
                  EXIT2_IR:   m_tms <= 1'b1; // Go to UPDATE_IR
                  UPDATE_IR:  m_tms <= 1'b0; // Go to RUN_TEST
               endcase
            end
         end

         //
         // Select a scan chain.
         //
         else if (m_jtag_cmd == SELECT_CHAIN) begin
            if (m_next_state == SHIFT_IR) begin
               m_allow_shift <= 1'b1;
               if (m_chain_count <= 1) begin
                  m_tms <= 1'b1; // Go to EXIT1_IR
               end
               else begin
                  m_tms <= 1'b0; // Stay in SHIFT_IR
               end
               m_chain_count <= m_chain_count - 1;
            end
            else if (m_chain_count == 0) begin
               m_allow_shift <= 1'b0;
               if ((m_next_state == RUN_TEST) || (m_next_state == UPDATE_IR)) begin
                  m_cmd_active <= 1'b0;
               end
               case (m_next_state)
                  TAP_RESET:  m_tms <= 1'b0; // Go to RUN_TEST
                  RUN_TEST:   m_tms <= 1'b0; // Stay in RUN_TEST
                  SELECT_DR:  m_tms <= 1'b1; // Should not really be in this state (to SELECT_IR)
                  CAPTURE_DR: m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                  SHIFT_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                  EXIT1_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_DR)
                  PAUSE_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_DR)
                  EXIT2_DR:   m_tms <= 1'b1; // Should not really be in this state (to UPDATE_DR)
                  UPDATE_DR:  m_tms <= 1'b0; // Should not really be in this state (to RUN_TEST)
                  SELECT_IR:  m_tms <= 1'b1; // Should not really be in this state (to TAP_RESET)
                  CAPTURE_IR: m_tms <= 1'b1; // Should not really be in this state (to SHIFT_IR)
                  SHIFT_IR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT1_IR)
                  EXIT1_IR:   m_tms <= 1'b1; // Go to UPDATE_IR
                  PAUSE_IR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_IR)
                  EXIT2_IR:   m_tms <= 1'b1; // Should not really be in this state (to UPDATE_IR)
                  UPDATE_IR:  m_tms <= 1'b0; // Go to RUN_TEST
               endcase
            end
            else begin // Get into the SHIFT_IR state
               m_allow_shift <= 1'b1;
               case (m_next_state)
                  TAP_RESET:  m_tms <= 1'b0; // Go to RUN_TEST
                  RUN_TEST:   m_tms <= 1'b1; // Go to SELECT_DR
                  SELECT_DR:  m_tms <= 1'b1; // Go to SELECT_IR
                  CAPTURE_DR: m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                  SHIFT_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                  EXIT1_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_DR)
                  PAUSE_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_DR)
                  EXIT2_DR:   m_tms <= 1'b1; // Should not really be in this state (to UPDATE_DR)
                  UPDATE_DR:  m_tms <= 1'b0; // Should not really be in this state (to RUN_TEST)
                  SELECT_IR:  m_tms <= 1'b0; // Go to CAPTURE_IR
                  CAPTURE_IR: m_tms <= 1'b0; // Go to SHIFT_IR
                  SHIFT_IR:   m_tms <= 1'b0; // Should not really be in this state (to EXIT1_IR)
                  EXIT1_IR:   m_tms <= 1'b0; // Should not really be in this state (to PAUSE_IR)
                  PAUSE_IR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_IR)
                  EXIT2_IR:   m_tms <= 1'b0; // Should not really be in this state (to SHIFT_IR)
                  UPDATE_IR:  m_tms <= 1'b0; // Should not really be in this state (to RUN_TEST)
               endcase
            end
         end

         //
         // Scan data.
         //
         else if ((m_jtag_cmd == JTAG_RD) || (m_jtag_cmd == JTAG_RD_PARTIAL) || (m_jtag_cmd == JTAG_WR) || (m_jtag_cmd == JTAG_WR_PARTIAL)) begin
            m_use_tdi <= ((m_jtag_cmd == JTAG_RD) || (m_jtag_cmd == JTAG_RD_PARTIAL)) ? 1'b1 : 1'b0;
            if (m_next_state == SHIFT_DR) begin
               m_really_shift_zero <= 1'b0;
               m_last_was_partial <= ((m_jtag_cmd == JTAG_RD_PARTIAL) || (m_jtag_cmd == JTAG_WR_PARTIAL)) ? 1'b1 : 1'b0;
               if (m_chain_count == 0) begin
                  m_allow_shift <= 1'b0;
                  m_tms         <= 1'b1; // Go to EXIT1_DR
               end
               else begin
                  m_allow_shift <= 1'b1;
                  if ((m_chain_count == 1) && (m_jtag_cmd != JTAG_RD)) begin
                     m_tms <= 1'b1; // Go to EXIT1_DR
                  end
                  else begin
                     m_tms <= 1'b0; // Stay in SHIFT_DR
                  end
                  m_chain_count <= m_chain_count - 1;
               end
            end
            else if ((m_chain_count == 0) && (m_really_shift_zero == 1'b0)) begin
               m_allow_shift <= 1'b0;
               if ((m_jtag_cmd == JTAG_RD_PARTIAL) || (m_jtag_cmd == JTAG_WR_PARTIAL)) begin
                  m_tms        <= 1'b0; // Stay in PAUSE_DR
                  m_cmd_active <= 1'b0;
                  m_use_tdi    <= (m_jtag_cmd == JTAG_RD_PARTIAL) ? 1'b1 : 1'b0;
               end
               else if ((m_next_state == RUN_TEST) || (m_next_state == UPDATE_DR)) begin
                  m_tms        <= 1'b0;
                  m_cmd_active <= 1'b0;
                  m_use_tdi    <= (m_jtag_cmd == JTAG_RD_PARTIAL) ? 1'b1 : 1'b0;
               end
               else begin
                  case (m_next_state)
                     TAP_RESET:  m_tms <= 1'b0; // Go to RUN_TEST
                     RUN_TEST:   m_tms <= 1'b0; // Stay in RUN_TEST
                     SELECT_DR:  m_tms <= 1'b1; // Should not really be in this state (to SELECT_IR)
                     CAPTURE_DR: m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                     SHIFT_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT1_DR)
                     EXIT1_DR:   m_tms <= 1'b1; // Go to UPDATE_DR
                     PAUSE_DR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_DR)
                     EXIT2_DR:   m_tms <= 1'b1; // Go to UPDATE_DR
                     UPDATE_DR:  m_tms <= 1'b0; // Go to RUN_TEST
                     SELECT_IR:  m_tms <= 1'b1; // Should not really be in this state (to TAP_RESET)
                     CAPTURE_IR: m_tms <= 1'b1; // Should not really be in this state (to EXIT1_IR)
                     SHIFT_IR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT1_IR)
                     EXIT1_IR:   m_tms <= 1'b1; // Should not really be in this state (to UPDATE_IR)
                     PAUSE_IR:   m_tms <= 1'b1; // Should not really be in this state (to EXIT2_IR)
                     EXIT2_IR:   m_tms <= 1'b1; // Should not really be in this state (to UPDATE_IR)
                     UPDATE_IR:  m_tms <= 1'b0; // Go to RUN_TEST
                  endcase
               end
            end
            else begin // Get into the SHIFT_DR state
               m_allow_shift <= 1'b1;
               case (m_next_state)
                  TAP_RESET:  m_tms <= 1'b0;                // Go to RUN_TEST
                  RUN_TEST:   m_tms <= 1'b1;                // Go to SELECT_DR
                  SELECT_DR:  m_tms <= 1'b0;                // Go to CAPTURE_DR
                  CAPTURE_DR: m_tms <= ((m_chain_count == 0) && (m_really_shift_zero == 1'b1)) ? 1'b1 : 1'b0;                // Go to SHIFT_DR
                  SHIFT_DR:   m_tms <= 1'b1;                // Should not really be in this state (to EXIT1_DR)
                  EXIT1_DR:   m_tms <= 1'b1;                // Should not really be in this state (to UPDATE_DR)
                  PAUSE_DR:   m_tms <= 1'b1;                // Go to EXIT2_DR
                  EXIT2_DR:   m_tms <= ~m_last_was_partial; // Go to SHIFT_DR or UPDATE_DR
                  UPDATE_DR:  m_tms <= 1'b0;                // Should not really be in this state (to RUN_TEST)
                  SELECT_IR:  m_tms <= 1'b1;                // Should not really be in this state (to TAP_RESET)
                  CAPTURE_IR: m_tms <= 1'b1;                // Should not really be in this state (to EXIT1_IR)
                  SHIFT_IR:   m_tms <= 1'b1;                // Should not really be in this state (to EXIT1_IR)
                  EXIT1_IR:   m_tms <= 1'b1;                // Should not really be in this state (to UPDATE_IR)
                  PAUSE_IR:   m_tms <= 1'b1;                // Should not really be in this state (to EXIT2_IR)
                  EXIT2_IR:   m_tms <= 1'b0;                // Should not really be in this state (to UPDATE_IR)
                  UPDATE_IR:  m_tms <= 1'b0;                // Should not really be in this state (to RUN_TEST)
               endcase // case (m_next_state)
               if (m_next_state == CAPTURE_DR) begin
                  m_really_shift_zero <= 1'b0;
               end
            end
         end
      end
      m_state <= m_next_state;
      -> m_clk_event;
   end


   `ifdef EVL_SIMULATION // -----------------------------------------------------------------------{
      bit                 m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), JTAG_BUS_AGENT, ~0, "evl_jtag_verif_comp");
      evl_jtag_rtl_port   m_rtl_port    = new($sformatf("%m"));
      evl_jtag_req_info_t m_jtag_req;
      evl_jtag_rsp_info_t m_jtag_rsp;

      //
      // Capture new commands from the stimulus agent
      //
      always @(posedge m_tck) begin
         if ((reset == 1'b0) && (m_allow_scan == 1'b1)) begin
            if ((m_cmd_active == 1'b0) && (m_last_cmd_active == 1'b1)) begin
               if (m_jtag_cmd == JTAG_STEP) begin
                  m_jtag_rsp.count = 1;
                  m_jtag_rsp.data  = { {(`EVL_MAX_JTAG_LENGTH - 1){1'b0}}, m_tdi };
               end
               else begin
                  m_jtag_rsp.count = m_chain_length;
                  m_jtag_rsp.data  = m_chain >> (`EVL_MAX_JTAG_LENGTH - m_chain_length);
               end
               m_rtl_port.put_jtag_rsp(m_jtag_rsp);
            end
         end
         m_last_cmd_active <= m_cmd_active;
      end

      always @(posedge m_tck) begin
         if ((reset == 1'b1) || (m_allow_scan == 1'b0)) begin
            m_jtag_valid <= 1'b0;
         end
         else begin
            m_rtl_port.get_jtag_req(m_jtag_req);
            forever @(posedge m_tck) begin
               if ((reset == 1'b1) || (m_allow_scan == 1'b0)) begin
                  break;
               end
               if (m_jtag_valid == 1'b0) begin
                  if (m_jtag_req.cmd == JTAG_STEP) begin
                     m_next_jtag_cmd    <= m_jtag_req.cmd;
                     m_next_chain_count <= (m_jtag_req.count & evl_jtag_length_t'(1)) | evl_jtag_length_t'(2);
                     m_next_chain       <= evl_jtag_data_t'({8{m_jtag_req.data[0:0]}});
                  end
                  else begin
                     m_next_jtag_cmd    <= m_jtag_req.cmd;
                     m_next_chain_count <= m_jtag_req.count;
                     m_next_chain       <= (m_jtag_req.cmd inside { JTAG_RD, JTAG_RD_PARTIAL }) ? {`EVL_MAX_JTAG_LENGTH{1'b0}} :
                                                                                                  m_jtag_req.data;
                  end
                  m_jtag_valid <= 1'b1;
                  break;
               end
            end
         end
      end

      always begin
         wait (reset === 1'b1);
         m_rtl_port.assert_reset();
         wait (reset === 1'b0);
         m_rtl_port.deassert_reset();
      end

      always @(m_clk_event) begin
         m_rtl_port.ping_clock();
      end
   `endif // ifdef EVL_SIMULATION

endmodule

//--------------------------------------------------------------------------------------------------
//
//
// Class: usr_axi_rtl_port_base
//
// This class object is used by the evl_axi_interface to capture AXI requests and responses and
// (potentially) to drive either requests or responses.
//
// When monitoring requests and responses, it can be used simply to print requests and responses as
// they are observed, or it can be used to pass packaged requests and responses to an external
// monitor (the external monitor must have a handle to the same object instantiated in the
// evl_axi_interface module; the get_obs_req() and get_obs_rsp() task calls can then be used get
// captured requests and responses).
//
// Similarly, when driving requests or responses, the driving agent must have a handle to the same
// object instantiated in the evl_axi_interface module; the put_req() or put_rsp() function calls
// can then be used send requests or responses.
//
// NOTE: This object handles only single-burst requests!!
//
// This class can either be modified or extended by users of this library; any modifications are the
// sole responsibility of the modifier.
//
class usr_axi_rtl_port_base;

   //
   // Create a couple of strings to identify messages from this particular object.  In addition,
   // define an agent ID that can be used to abstractly identify the bus for this object.
   //
   evl_agent_id_t m_agent_id      = ~evl_agent_id_t'(0);
   string         m_abstract_name = "AXI";
   string         m_rtl_name      = "unknown";


   //
   // Variables for clocks and resets, etc.
   //
   int   m_cycle_count = -1;
   bit   m_power_on    = 1'b1;
   bit   m_reset_asserted[];
   bit   m_reset_deasserted[];
   event m_reset_change;
   event m_clock_event;


   //
   // Define the request and response data width in bytes.  The default width is currently 64 bytes.
   //
   int m_req_data_bus_width;
   int m_rsp_data_bus_width;
   int m_req_data_bus_clog;
   int m_rsp_data_bus_clog;


   //
   // Define a queue type for holding pending AXI requests on a per-ID basis.
   //
   typedef evl_axi_bus_req evl_axi_bus_req_queue[$];


   //
   // The following variables are per-source-ID objects used to correlate responses back to their
   // original requests.
   //
   evl_axi_bus_req_queue m_ar_req_by_id[evl_bus_req_id_t];
   evl_axi_bus_req_queue m_aw_req_by_id[evl_bus_req_id_t];
   evl_axi_bus_req       m_aw_req_pend[$];
   evl_axi_bus_req       m_w_data_pend[$];


   //
   // Mailboxes for collecting bus requests from the interface to send to an external monitor.
   //
   mailbox #(evl_axi_bus_req) m_obs_req_mb;
   mailbox #(evl_axi_bus_rsp) m_obs_rsp_mb;


   //
   // Mailboxes for collecting bus requests and responses from an external driver to send to the
   // interface.
   //
   mailbox #(evl_axi_bus_req) m_snd_req_mb;
   mailbox #(evl_axi_bus_rsp) m_snd_rsp_mb;


   //
   // Variables for enabling drivers.
   //
   bit m_master_driver       = 1'b0;
   bit m_slave_driver        = 1'b0;
   int m_delays_are_disabled = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name of this object.
   //
   virtual function void set_abstract_name(input string abstract_name_in);
      m_abstract_name = $sformatf("%s", abstract_name_in);
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_abstract_name
   //
   // This function is used to get the abstract name for this object.
   //
   virtual function string get_abstract_name();
      return m_abstract_name;
   endfunction : get_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_name
   //
   // This function is used to set the RTL name of this object.
   //
   virtual function void set_rtl_name(input string rtl_name_in);
      m_rtl_name = $sformatf("%s", rtl_name_in);
      log_message($sformatf("these messages map to %0s", m_rtl_name));
   endfunction : set_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_name
   //
   // This function is used to get the RTL name for this object.
   //
   virtual function string get_rtl_name();
      return m_rtl_name;
   endfunction : get_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_id
   //
   // This function is used to set the agent ID for this object.  The agent ID can be used to
   // identify a particular AXI bus within a specific module hierarchy.
   //
   virtual function void set_agent_id(input evl_agent_id_t agent_id_in);
      m_agent_id = agent_id_in;
   endfunction : set_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_id
   //
   // This function is used to get the agent ID for this object.
   //
   virtual function evl_agent_id_t get_agent_id();
      return m_agent_id;
   endfunction : get_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_on
   //
   virtual function void power_on();
      if (m_power_on == 1'b0) begin
         `evl_log(`UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power on (%s)", m_rtl_name))
      end
      m_power_on = 1'b1;
   endfunction : power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_off
   //
   virtual function void power_off();
      if (m_power_on == 1'b1) begin
         `evl_log(`UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power off (%s)", m_rtl_name))
      end
      m_power_on = 1'b0;
   endfunction : power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_on
   //
   function bit power_is_on();
      return m_power_on;
   endfunction : power_is_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_off
   //
   function bit power_is_off();
      return ~m_power_on;
   endfunction : power_is_off


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_on
   //
   task wait_for_power_on();
      #0 wait (m_power_on == 1'b1);
   endtask : wait_for_power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_off
   //
   task wait_for_power_off();
      #0 wait (m_power_on == 1'b0);
   endtask : wait_for_power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assert_reset
   //
   function void assert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b1;
         m_reset_deasserted[reset_index_in] = 1'b0;
         -> m_reset_change;
      end
   endfunction : assert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: deassert_reset
   //
   function void deassert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b0;
         m_reset_deasserted[reset_index_in] = 1'b1;
         -> m_reset_change;
      end
   endfunction : deassert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_deasserted
   //
   function bit reset_is_deasserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b0) && (m_reset_deasserted[reset_index_in] == 1'b1)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_deasserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_asserted
   //
   function bit reset_is_asserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b1) && (m_reset_deasserted[reset_index_in] == 1'b0)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_asserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_deassertion
   //
   task wait_for_reset_deassertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_deasserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_deassertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_assertion
   //
   task wait_for_reset_assertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_asserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_assertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_clock
   //
   virtual function void ping_clock(input int clock_index_in = 0);
      ->m_clock_event;
      m_cycle_count++;
   endfunction : ping_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_verbosity
   //
   // This function is used to check verbosity for this object before printing information.  A
   // value of 1'b1 should be returned if printing is enabled.
   //
   virtual function bit check_verbosity();
      return `evl_log_check_verbosity(`UVM_MEDIUM, VMOD_GLOBAL);
   endfunction : check_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: log_message
   //
   // This function is used to print a log message.
   //
   virtual function void log_message(input string message);
      `evl_log(`UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), message)
   endfunction : log_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_data_width
   //
   // This function is used to set the bus data width (in bytes) for bus write requests.
   //
   virtual function void set_bus_req_data_width(input int bus_data_width_in);
      m_req_data_bus_width = bus_data_width_in;
      m_req_data_bus_clog  = $clog2(m_req_data_bus_width);
   endfunction : set_bus_req_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_data_width
   //
   // This function is used to get the bus data width (in bytes) for bus write requests.
   //
   virtual function int get_bus_req_data_width();
      return m_req_data_bus_width;
   endfunction : get_bus_req_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_data_width
   //
   // This function is used to set the bus data width (in bytes) for bus read responses.
   //
   virtual function void set_bus_rsp_data_width(input int bus_data_width_in);
      m_rsp_data_bus_width = bus_data_width_in;
      m_rsp_data_bus_clog  = $clog2(m_rsp_data_bus_width);
   endfunction : set_bus_rsp_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_data_width
   //
   // This function is used to get the bus data width (in bytes) for bus read responses.
   //
   virtual function int get_bus_rsp_data_width();
      return m_rsp_data_bus_width;
   endfunction : get_bus_rsp_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   // This function returns 1'b1 if the specified bus command is a read operation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in);
      if (evl_axi_req_type_t'(bus_cmd_in) inside { AXI_REQ_READ }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   // This function returns 1'b1 if the specified bus command is a write operation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in);
      if (evl_axi_req_type_t'(bus_cmd_in) inside { AXI_REQ_WRITE }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   // This function returns 1'b1 if the specified bus command will have associated read data.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in);
      if (evl_axi_req_type_t'(bus_cmd_in) inside { AXI_REQ_READ }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   // This function returns 1'b1 if the specified bus command has associated write data.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in);
      if (evl_axi_req_type_t'(bus_cmd_in) inside { AXI_REQ_WRITE }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   // This function returns the bus response opcode as a string.
   //
   // Input Variables:
   //
   // bus_rsp_in: This is the bus response opcode.
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in);
      evl_axi_resp_t rsp_opcode = evl_axi_resp_t'(bus_rsp_in);

      return $sformatf("%p", rsp_opcode);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   // This function returns 1'b1 if the bus response has associated data with it.
   //
   // Input Variables:
   //
   // bus_rsp_in: This is the bus response opcode.
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in);
      return 1'b1;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_data
   //
   // This function is used to format data appropriately.
   //
   // Input Variables:
   //
   // size_addr_lsb_in: This is the size of the data to print.  This size is the number of bytes in
   //                   $clog2() format (in other words, for a single byte the value is 0, for 2
   //                   bytes the value is 1, etc.).
   //                   ----------------------------------------------------------------------------
   // data_line_in:     This is a cache line's worth of data to print.  The data in this variable
   //                   should be cache-line aligned.
   //                   ----------------------------------------------------------------------------
   // req_addr_in:      This is the address of the request.
   //                   ----------------------------------------------------------------------------
   // mask_in:          This is a byte mask of valid bytes.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the contents of the data.
   //
   virtual function string format_data(input int                 size_addr_lsb_in,
                                       input evl_xword_t         data_line_in,
                                       input evl_paddr_t         req_addr_in,
                                       input evl_xword_byte_en_t mask_in);
      int                 size_addr_lsb;
      int                 byte_start;
      int                 byte_end;
      int                 byte_count;
      evl_xword_t         data_line;
      evl_xword_byte_en_t mask;
      string              data_str;

      if (size_addr_lsb_in < 0) begin
         return "-- null data --";
      end
      size_addr_lsb = (size_addr_lsb_in > `EVL_XWORD_ADDR_LSB) ? `EVL_XWORD_ADDR_LSB : size_addr_lsb_in;
      byte_start    = int'(req_addr_in & (~(evl_paddr_t'((1 << size_addr_lsb) - 1))) & (`EVL_XWORD_BYTES - 1));
      byte_end      = byte_start + (1 << size_addr_lsb);
      byte_count    = byte_end - byte_start;
      data_line     = data_line_in << ((`EVL_XWORD_BYTES - byte_end) << 3);
      mask          = mask_in << (`EVL_XWORD_BYTES - byte_end);
      data_str      = "0x";

      while (byte_count > 0) begin
         byte_count--;
         if ((byte_count == 0) || ((byte_count & 7) != 0)) begin
            if (mask[`EVL_XWORD_BYTES-1] == 1'b0) begin
               data_str = $sformatf("%0s--", data_str);
            end
            else begin
               data_str = $sformatf("%0s%02x", data_str, data_line[`EVL_XWORD_MSB:(`EVL_XWORD_MSB-7)]);
            end
         end
         else begin
            if (mask[`EVL_XWORD_BYTES-1] == 1'b0) begin
               data_str = $sformatf("%0s--_", data_str);
            end
            else begin
               data_str = $sformatf("%0s%02x_", data_str, data_line[`EVL_XWORD_MSB:(`EVL_XWORD_MSB-7)]);
            end
         end
         data_line = data_line << 8;
         mask      = mask << 1;
      end
      return data_str;
   endfunction : format_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to format a AXI request in a user-friendly form.  It returns a
   // string that is the formatted output.
   //
   // Input Variables:
   //
   // bus_req: This is the bus request structure that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the request information.
   //
   virtual function string sprint_obj(evl_axi_bus_req bus_req);
      int    req_size;
      string message;

      req_size = (1 << bus_req.m_ax_size) * (bus_req.m_ax_len + 1);
      if (bus_req.m_data_only == 1'b1) begin
         if (bus_req.m_xref_valid == 1'b1) begin
            if (bus_req.m_ax_vc <= evl_bus_req_vc_t'(1)) begin
               message = $sformatf("VC %0d, %0d byte%0s, user 0x%0x, cache 0x%0x, QOS 0x%0x", bus_req.m_ax_vc, req_size, ((req_size == 1) ? "" : "s"), bus_req.m_ax_user, bus_req.m_ax_cache, bus_req.m_ax_qos);
            end
            else begin
               message = $sformatf("%0d byte%0s, user 0x%0x, cache 0x%0x, QOS 0x%0x", req_size, ((req_size == 1) ? "" : "s"), bus_req.m_ax_user, bus_req.m_ax_cache, bus_req.m_ax_qos);
            end
            //
            // For atomic AXI writes, always print all of the write data.
            //
            if (bus_req.m_ax_user == 5) begin
               return $sformatf("W Data 0x%0s (AXI_REQ_WRITE 0x%x, %0s, ID:0x%02x)",
                         format_data(`EVL_XWORD_ADDR_LSB,  bus_req.m_w_data, 0, bus_req.m_w_strb), bus_req.m_ax_addr, message, bus_req.m_ax_id);
            end
            else begin
               return $sformatf("W Data 0x%0s (AXI_REQ_WRITE 0x%x, %0s, ID:0x%02x)",
                         format_data(bus_req.m_ax_size, bus_req.m_w_data, bus_req.m_ax_addr, bus_req.m_w_strb), bus_req.m_ax_addr, message, bus_req.m_ax_id);
            end
         end
         return $sformatf("W Data 0x%0s", format_data(`EVL_XWORD_ADDR_LSB,  bus_req.m_w_data, 0, bus_req.m_w_strb));
      end
      if (bus_req.m_ax_vc <= evl_bus_req_vc_t'(1)) begin
         message = $sformatf("VC %0d, %0d byte%0s, user 0x%0x, cache 0x%0x, QOS 0x%0x", bus_req.m_ax_vc, req_size, ((req_size == 1) ? "" : "s"), bus_req.m_ax_user, bus_req.m_ax_cache, bus_req.m_ax_qos);
      end
      else begin
         message = $sformatf("%0d byte%0s, user 0x%0x, cache 0x%0x, QOS 0x%0x", req_size, ((req_size == 1) ? "" : "s"), bus_req.m_ax_user, bus_req.m_ax_cache, bus_req.m_ax_qos);
      end
      if (bus_req.m_req_type == AXI_REQ_WRITE) begin
         if (bus_req.m_xref_valid == 1'b1) begin
            return $sformatf("AXI_REQ_WRITE 0x%x (%0s, ID:0x%02x) <- 0x%0s",
                      bus_req.m_ax_addr, message, bus_req.m_ax_id, format_data(bus_req.m_ax_size, bus_req.m_w_data, bus_req.m_ax_addr, bus_req.m_w_strb));
         end
         else begin
            return $sformatf("AXI_REQ_WRITE 0x%x (%0s, ID:0x%02x)", bus_req.m_ax_addr, message, bus_req.m_ax_id);
         end
      end
      return $sformatf("AXI_REQ_READ 0x%x (%0s, ID:0x%02x)", bus_req.m_ax_addr, message, bus_req.m_ax_id);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to format a AXI response in a user-friendly form.  It returns a
   // string that is the formatted output.
   //
   // Input Variables:
   //
   // bus_rsp:       This is the bus response structure that contains essential response
   //                information.
   //                -------------------------------------------------------------------------------
   // bus_req:       This is the bus request handle that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the response information.
   //
   virtual function string sprint_rsp(input evl_axi_bus_rsp bus_rsp,
                                      input evl_axi_bus_req bus_req = null);
      if (bus_rsp.m_rsp_for_write == 1'b0) begin
         if (bus_req != null) begin
            return $sformatf("%0p %0s (ID:0x%02x%0s)", bus_rsp.m_x_resp, format_data(bus_req.m_ax_size, bus_rsp.m_x_data, bus_req.m_ax_addr, ~evl_xword_byte_en_t'(0)), bus_rsp.m_x_id, ((bus_rsp.m_x_last == 1'b1) ? ", last" : ""));
         end
         return $sformatf("%0p %0s (ID:0x%02x%0s)", bus_rsp.m_x_resp, format_data(`EVL_XWORD_ADDR_LSB, bus_rsp.m_x_data, evl_paddr_t'(0), ~evl_xword_byte_en_t'(0)), bus_rsp.m_x_id, ((bus_rsp.m_x_last == 1'b1) ? ", last" : ""));
      end
      return $sformatf("%0p (ID:0x%02x, last)", bus_rsp.m_x_resp, bus_rsp.m_x_id);
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   // This function is called by the evl_axi_interface module when a request is detected.  For
   // multi-beat requests, all of the request information is gathered before sending a single
   // request to the AXI monitor.  As a result, you will only see request logging when the
   // last beat is detected.
   //
   // Note that the final request data and mask are cache line aligned.  What this means is that
   // the bus_req.data value pushed onto the mailbox will be data bits 511-0 ... all of the data
   // will be shifted as appropriate to be offset in the cache line.
   //
   // As an example, if the command is an 4-byte write at address 0x38, the write data will be in
   // bits 479-448, regardless of the width of the request data bus.
   //
   // Input Variables:
   //
   // is_write:    This input is set to 1'b1 if the request is a write.
   //              ---------------------------------------------------------------------------------
   // req_info: This is the bus request structure that contains essential request information.
   //
   virtual function void capture_req(input evl_axi_req_info_t req_info);
      bit             push_req;
      evl_axi_bus_req bus_req;

      //
      // Transfer request information from the packed structure to a class object.
      //
      bus_req = new();
      if (req_info.ax_is_read == 1'b1) begin
         bus_req.m_req_type  = AXI_REQ_READ;
         bus_req.m_data_only = 1'b0;
         push_req = 1'b1;
      end
      else begin
         bus_req.m_req_type  = AXI_REQ_WRITE;
         bus_req.m_data_only = 1'b0;
         push_req = 1'b0;
      end

      bus_req.m_ax_id     = req_info.ax_id;
      bus_req.m_ax_addr   = req_info.ax_addr;
      bus_req.m_ax_len    = req_info.ax_len;
      bus_req.m_ax_size   = req_info.ax_size;
      bus_req.m_ax_burst  = req_info.ax_burst;
      bus_req.m_ax_lock   = req_info.ax_lock;
      bus_req.m_ax_cache  = req_info.ax_cache;
      bus_req.m_ax_prot   = req_info.ax_prot;
      bus_req.m_ax_qos    = req_info.ax_qos;
      bus_req.m_ax_region = req_info.ax_region;
      bus_req.m_ax_user   = req_info.ax_user;
      bus_req.m_ax_vc     = req_info.ax_vc;
      bus_req.m_trans_id  = req_info.trans_id;

      if (req_info.ax_is_read == 1'b1) begin
         //
         // Push the created object onto a per-ID queue for subsequent response processing.  AXI
         // requests for a particular channel that share an ID must be in order.
         //
         m_ar_req_by_id[bus_req.m_ax_id].push_back(bus_req);
      end
      else begin
         //
         // Push the created object onto a per-ID queue for subsequent response processing.  AXI
         // requests for a particular channel that share an ID must be in order.
         //
         m_aw_req_by_id[bus_req.m_ax_id].push_back(bus_req);

         //
         // Push the created object onto an in-order write queue.  All write data from the W channel
         // is processed in order (there is no ID field in the W channel).
         //
         m_aw_req_pend.push_back(bus_req);

         //
         // If we have already received W data, consume an entry.
         //
         if (m_w_data_pend.size() > 0) begin
            evl_axi_bus_req bus_req_data;

            //
            // Transfer data from the bus_req_data object to the bus_req object.  Note that data is
            // cache-line aligned in the bus_req object.
            //
            bus_req_data = m_w_data_pend.pop_front();

            bus_req.m_xref_valid = 1'b1;
            bus_req.m_w_data     = bus_req_data.m_w_data;
            bus_req.m_w_strb     = bus_req_data.m_w_strb;
            bus_req.m_w_user     = bus_req_data.m_w_user;
            bus_req.m_w_last     = bus_req_data.m_w_last;
            if (m_req_data_bus_width < `EVL_XWORD_BYTES) begin
               int byte_shift;

               byte_shift = int'(bus_req.m_ax_addr & (`EVL_XWORD_BYTES - 1)) >> m_req_data_bus_clog;
               if (byte_shift != 0) begin
                  byte_shift = byte_shift << m_req_data_bus_clog;
                  bus_req.m_w_strb <<= byte_shift;
                  bus_req.m_w_data <<= (byte_shift << 3);
               end
            end
            push_req = 1'b1;
            void'(m_aw_req_pend.pop_front());
         end
      end

      if (check_verbosity() == 1'b1) begin
         log_message(sprint_obj(bus_req));
      end
      if ((m_obs_req_mb != null) && (push_req == 1'b1)) begin
         m_obs_req_mb.try_put(bus_req);
      end
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req_data
   //
   // This function is called by the evl_axi_interface module when a request is detected.  For
   // multi-beat requests, all of the request information is gathered before sending a single
   // request to the AXI monitor.  As a result, you will only see request logging when the
   // last beat is detected.
   //
   // Note that the final request data and mask are cache line aligned.  What this means is that
   // the bus_req.data value pushed onto the mailbox will be data bits 511-0 ... all of the data
   // will be shifted as appropriate to be offset in the cache line.
   //
   // As an example, if the command is an 4-byte write at address 0x38, the write data will be in
   // bits 479-448, regardless of the width of the request data bus.
   //
   // Input Variables:
   //
   // req_info: This is the bus request structure that contains essential request information.
   //
   virtual function void capture_req_data(input evl_axi_w_data_info_t req_info);
      evl_axi_bus_req bus_req_data;
      bit             push_req;

      //
      // Write data is different ... the monitor does NOT necessarily consume the class object
      // immediately, so we need to create a new one each time.
      //
      push_req     = 1'b0;
      bus_req_data = new();
      bus_req_data.m_data_only = 1'b1;
      bus_req_data.m_req_type  = AXI_REQ_WRITE;
      bus_req_data.m_w_data    = req_info.w_data;
      bus_req_data.m_w_strb    = req_info.w_strb;
      bus_req_data.m_w_user    = req_info.w_user;
      bus_req_data.m_w_last    = req_info.w_last;
      bus_req_data.m_trans_id  = req_info.trans_id;

      //
      // If we have already received an AW request, transfer the data as necessary.
      //
      if (m_aw_req_pend.size() > 0) begin
         evl_axi_bus_req bus_req;

         if (req_info.w_last == 1'b1) begin
            bus_req = m_aw_req_pend.pop_front();
         end
         else begin
            bus_req = m_aw_req_pend[0];
         end

         bus_req_data.m_xref_valid = 1'b1;
         bus_req_data.m_ax_id      = bus_req.m_ax_id;
         bus_req_data.m_ax_addr    = bus_req.m_ax_addr;
         bus_req_data.m_ax_len     = bus_req.m_ax_len;
         bus_req_data.m_ax_size    = bus_req.m_ax_size;
         bus_req_data.m_ax_burst   = bus_req.m_ax_burst;
         bus_req_data.m_ax_lock    = bus_req.m_ax_lock;
         bus_req_data.m_ax_cache   = bus_req.m_ax_cache;
         bus_req_data.m_ax_prot    = bus_req.m_ax_prot;
         bus_req_data.m_ax_qos     = bus_req.m_ax_qos;
         bus_req_data.m_ax_region  = bus_req.m_ax_region;
         bus_req_data.m_ax_user    = bus_req.m_ax_user;
         bus_req_data.m_ax_vc      = bus_req.m_ax_vc;
         if (m_req_data_bus_width < `EVL_XWORD_BYTES) begin
            int byte_shift;

            byte_shift = int'(bus_req.m_ax_addr & (`EVL_XWORD_BYTES - 1)) >> m_req_data_bus_clog;
            if (byte_shift != 0) begin
               byte_shift = byte_shift << m_req_data_bus_clog;
               bus_req_data.m_w_strb <<= byte_shift;
               bus_req_data.m_w_data <<= (byte_shift << 3);
            end
         end
         push_req = 1'b1;
      end
      else begin
         m_w_data_pend.push_back(bus_req_data);
      end
      if (check_verbosity() == 1'b1) begin
         log_message(sprint_obj(bus_req_data));
      end
      if ((m_obs_req_mb != null) && (push_req == 1'b1)) begin
         m_obs_req_mb.try_put(bus_req_data);
      end
   endfunction : capture_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   // This function is called by the evl_axi_interface module when a response is detected.  For
   // multi-beat responses, all of the response information is gathered before sending a single
   // response to the AXI monitor.  As a result, you will only see response logging when the
   // last beat is detected.
   //
   // Input Variables:
   //
   // bus_rsp: This is the bus response structure that contains essential response information.
   //
   // Note that the final response data is cache line aligned *if possible* (the only way this is
   // possible is if the corresponding original request can be found).  What this means is that the
   // bus_rsp.data value pushed onto the mailbox will be data bits 511-0 ... all of the data will be
   // shifted as appropriate to be offset in the cache line.
   //
   // As an example, if the command is an 4-byte read at address 0x38, the read data will be in bits
   // 479-448, regardless of the width of the response data bus.
   //
   virtual function void capture_rsp(input evl_axi_rsp_info_t rsp_info);
      evl_axi_bus_req bus_req;
      evl_axi_bus_rsp bus_rsp;

      bus_req = null;
      bus_rsp = new();
      if (rsp_info.x_is_read == 1'b1) begin
         bus_rsp.m_rsp_for_write = 1'b0;
         if (m_ar_req_by_id.exists(rsp_info.x_id)) begin
            bus_req = m_ar_req_by_id[rsp_info.x_id].pop_front();
            if (m_rsp_data_bus_width < `EVL_XWORD_BYTES) begin
               int byte_shift;

               byte_shift = int'(bus_req.m_ax_addr & (`EVL_XWORD_BYTES - 1)) >> m_rsp_data_bus_clog;
               if (byte_shift != 0) begin
                  byte_shift = (byte_shift << m_rsp_data_bus_clog) << 3;
                  rsp_info.x_data <<= byte_shift;
               end
            end
            if ((bus_req != null) && (rsp_info.x_last != 1'b1)) begin
               m_ar_req_by_id[rsp_info.x_id].push_front(bus_req);
            end
         end
      end
      else begin
         bus_rsp.m_rsp_for_write = 1'b1;
         if (m_aw_req_by_id.exists(rsp_info.x_id)) begin
            bus_req = m_aw_req_by_id[rsp_info.x_id].pop_front();
            if ((bus_req != null) && (rsp_info.x_last != 1'b1)) begin
               m_aw_req_by_id[rsp_info.x_id].push_front(bus_req);
            end
         end
      end
      bus_rsp.m_x_id     = rsp_info.x_id;
      bus_rsp.m_x_resp   = rsp_info.x_resp;
      bus_rsp.m_x_data   = rsp_info.x_data;
      bus_rsp.m_x_last   = rsp_info.x_last;
      bus_rsp.m_x_user   = rsp_info.x_user;
      bus_rsp.m_trans_id = rsp_info.trans_id;
      if (check_verbosity() == 1'b1) begin
         log_message(sprint_rsp(bus_rsp, bus_req));
      end
      if (m_obs_rsp_mb != null) begin
         void'(m_obs_rsp_mb.try_put(bus_rsp));
      end
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_request_mailbox
   //
   // This function is to create a request mailbox and should only be used if there is an external
   // driver or monitor that intends to use it (via get_obs_req() calls).
   //
   virtual function create_request_mailbox();
      if (m_obs_req_mb == null) begin
         m_obs_req_mb = new();
      end
   endfunction : create_request_mailbox


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_response_mailbox
   //
   // This function is to create a response mailbox and should only be used if there is an external
   // driver or monitor that intends to use it (via get_obs_rsp() calls).
   //
   virtual function create_response_mailbox();
      if (m_obs_rsp_mb == null) begin
         m_obs_rsp_mb = new();
      end
   endfunction : create_response_mailbox


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_mailboxes
   //
   // This function is to create request and response mailboxes and should only be used if there is
   // an external monitor that intends to use them (via get_obs_req() and get_obs_rsp() calls).
   //
   virtual function create_mailboxes();
      create_request_mailbox();
      create_response_mailbox();
   endfunction : create_mailboxes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_req
   //
   // This function receives a bus request object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_req(input evl_axi_bus_req bus_req);
      void'(m_snd_req_mb.try_put(bus_req));
   endfunction : put_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_rsp
   //
   // This function receives a bus response object from the driver and keeps it in the mailbox
   // until it is ready to be placed on the bus interface.
   //
   virtual function void put_rsp(input evl_axi_bus_rsp bus_rsp);
      void'(m_snd_rsp_mb.try_put(bus_rsp));
   endfunction : put_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_req
   //
   // This task is used by the AXI interface module to get the next available request to send.
   //
   virtual task get_req(output evl_axi_bus_req bus_req);
      m_snd_req_mb.get(bus_req);
   endtask : get_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_rsp
   //
   // This task is used by the AXI interface module to get the next available response to send.
   //
   virtual task get_rsp(output evl_axi_bus_rsp bus_rsp);
      m_snd_rsp_mb.get(bus_rsp);
   endtask : get_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_master
   //
   virtual function void init_master();
      m_master_driver = 1'b1;
   endfunction : init_master


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_slave
   //
   virtual function void init_slave();
      m_slave_driver = 1'b1;
   endfunction : init_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_master_driver
   //
   virtual task wait_for_master_driver();
      #0 wait (m_master_driver == 1'b1);
   endtask : wait_for_master_driver


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_slave_driver
   //
   virtual task wait_for_slave_driver();
      #0 wait (m_slave_driver == 1'b1);
   endtask : wait_for_slave_driver


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_delays
   //
   function void disable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled |= disable_type;
   endfunction : disable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_delays
   //
   function void enable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled &= ~disable_type;
   endfunction : enable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_enabled
   //
   function bit delays_are_enabled();
      return ((m_delays_are_disabled == 0) ? 1'b1 : 1'b0);
   endfunction : delays_are_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_disabled
   //
   function bit delays_are_disabled();
      return ((m_delays_are_disabled == 0) ? 1'b0 : 1'b1);
   endfunction : delays_are_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_enable
   //
   task wait_for_delay_enable();
      if (m_delays_are_disabled != 0) begin
         #0 wait (m_delays_are_disabled == 0);
      end
   endtask : wait_for_delay_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_disable
   //
   task wait_for_delay_disable();
      if (m_delays_are_disabled == 0) begin
         #0 wait (m_delays_are_disabled != 0);
      end
   endtask : wait_for_delay_disable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_req
   //
   // This task retrieves a bus request object from the mailbox.  This task is intended to be used
   // by an external monitor (should it exist).  Note that this task is blocking; it will not return
   // until there is something to observe.  A bus request structure is returned as an output
   // variable upon completion.
   //
   virtual task get_obs_req(output evl_axi_bus_req bus_req);
      if (m_obs_req_mb != null) begin
         m_obs_req_mb.get(bus_req);
      end
      else begin
         wait (0);
      end
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_rsp
   //
   // This task retrieves a bus response object from the mailbox.  This task is intended to be used
   // by an external monitor (should it exist).  Note that this task is blocking; it will not return
   // until there is something to observe.  A bus response structure is returned as an output
   // variable upon completion.
   //
   virtual task get_obs_rsp(output evl_axi_bus_rsp bus_rsp);
      if (m_obs_rsp_mb != null) begin
         m_obs_rsp_mb.get(bus_rsp);
      end
      else begin
         wait (0);
      end
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in = "usr_axi_rtl_port_base",
                input evl_agent_id_t agent_id_in  = ~evl_agent_id_t'(0));
      m_agent_id           = agent_id_in;
      m_req_data_bus_width = 64;
      m_rsp_data_bus_width = 64;
      m_req_data_bus_clog  = $clog2(m_req_data_bus_width);
      m_rsp_data_bus_clog  = $clog2(m_rsp_data_bus_width);

      m_snd_req_mb = new();
      m_snd_rsp_mb = new();

      //
      // If there is an external monitor available, mailboxes can be created by a call to
      // create_mailboxes().
      //
      // create_mailboxes();
   endfunction : new

endclass : usr_axi_rtl_port_base

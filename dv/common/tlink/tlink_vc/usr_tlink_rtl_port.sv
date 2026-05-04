//-----------------------------------------------------------------------------------------------------
//
//
// Class: usr_tlink_rtl_port
//
// This class object is used by the evl_tlink_interface to capture TileLink requests and responses.
// It can be used simply to print requests and responses as they are observed, or it can be used to
// pass packaged requests and responses to an external monitor (the external monitor would need to
// have a handle to the same object instantiated in the evl_tlink_interface module; the get_obs_req()
// and get_obs_rsp() task calls could then be used get captured requests and responses).
//
// This class can either be modified or extended by users of this library; any modifications are the
// sole responsibility of the modifier.
//
class usr_tlink_rtl_port;

   //
   // Create a couple of strings to identify messages from this particular object.  In addition,
   // define an agent ID that can be used to abstractly identify the bus for this object.
   //
   string         m_abstract_name = "TLINK";
   string         m_rtl_name      = "unknown";
   evl_agent_id_t m_agent_id      = ~evl_agent_id_t'(0);


   //
   // Define the request and response data width in bytes.  The default width is currently 16 bytes.
   //
   int m_req_data_bus_width;
   int m_rsp_data_bus_width;
   int m_req_data_bus_clog;
   int m_rsp_data_bus_clog;


   //
   // The following variables are per-request-channel objects used to track multi-beat operations.
   //
   evl_trans_id_t      m_req_trans_id[4];
   int                 m_req_beat_cnt[4];
   evl_xword_byte_en_t m_req_mask[4];
   evl_xword_t         m_req_data[4];


   //
   // The following variables are per-source-ID objects used to correlate responses back to their
   // original requests.  For GrantAck responses, the per-sink-ID list is used.
   //
   evl_tlink_bus_req_s m_req_by_source[evl_bus_req_id_t];
   evl_tlink_bus_req_s m_req_by_sink[evl_bus_req_id_t];


   //
   // The following variables are per-response-channel objects used to track multi-beat operations.
   //
   int         m_rsp_beat_cnt[4];
   evl_xword_t m_rsp_data[4];


   //
   // Mailboxes for collecting bus requests from the interface to send to an external monitor.
   //
   mailbox #(evl_tlink_bus_req_s) m_obs_req_mb;
   mailbox #(evl_tlink_bus_rsp_s) m_obs_rsp_mb;


   //
   //
   //
   int   m_cycle_count = -1;
   bit   m_power_on    = 1'b1;
   bit   m_reset_asserted[];
   bit   m_reset_deasserted[];
   event m_reset_change;
   event m_clock_event;
   event m_int_clock_event;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name of this object.
   //
   virtual function void set_abstract_name(input string abstract_name_in);
      m_abstract_name = $sformatf("%s(TLINK)", abstract_name_in);
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
   // identify a particular TileLink bus within a specific module hierarchy.
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
      `evl_log(`UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), message);
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
   // Function: get_bus_cmd_name
   //
   // This function returns the bus request opcode as a string.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in);
      evl_tlink_req_opcode_t req_opcode = evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK);

      return $sformatf("%p", req_opcode);
   endfunction : get_bus_cmd_name


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
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic, TLA_Get, TLA_AcquireBlock,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic, TLB_Get, TLB_ProbeBlock }) begin
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
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_PutFull, TLA_PutPartial, TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_PutFull, TLB_PutPartial, TLB_AtomicArith, TLB_AtomicLogic,
                                                                                 TLC_Release, TLC_ReleaseData }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   // This function returns 1'b1 if the specified bus command is an atomic operation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_acquire
   //
   // This function returns 1'b1 if the specified bus command is an acquire operation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus request opcode.
   //
   virtual function bit bus_cmd_is_acquire(input evl_bus_cmd_t bus_cmd_in);
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AcquireBlock, TLA_AcquirePerm }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_acquire


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
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_AtomicArith, TLA_AtomicLogic, TLA_Get, TLA_AcquireBlock,
                                                                                 TLB_AtomicArith, TLB_AtomicLogic, TLB_Get, TLB_ProbeBlock }) begin
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
      if (evl_tlink_req_opcode_t'(bus_cmd_in & `EVL_TL_REQ_OPCODE_MASK) inside { TLA_PutFull, TLA_PutPartial, TLA_AtomicArith, TLA_AtomicLogic,
                                                                                 TLB_PutFull, TLB_PutPartial, TLB_AtomicArith, TLB_AtomicLogic,
                                                                                 TLC_ReleaseData }) begin
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
      evl_tlink_rsp_opcode_t rsp_opcode = evl_tlink_rsp_opcode_t'(bus_rsp_in & `EVL_TL_RSP_OPCODE_MASK);

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
      if (evl_tlink_rsp_opcode_t'(bus_rsp_in & `EVL_TL_RSP_OPCODE_MASK) inside { TLC_AccessAckData, TLC_ProbeAckData,
                                                                                 TLD_AccessAckData, TLD_GrantData }) begin
         return 1'b1;
      end
      return 1'b0;
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
         if ((byte_count == 0) || ((byte_count & 15) != 0)) begin
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
   // This function is used to format a TileLink request in a user-friendly form.  It returns a
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
   virtual function string sprint_obj(ref evl_tlink_bus_req_s bus_req);
      int    req_size;
      string message;

      req_size = (1 << bus_req.cmd_size);
      if (bus_req.param == TL_param_none) begin
         message = $sformatf("0x%0x, %0d byte%0s", bus_req.source, req_size, ((req_size == 1) ? "" : "s"));
      end
      else begin
         message = $sformatf("%p, 0x%0x, %0d byte%0s", bus_req.param, bus_req.source, req_size, ((req_size == 1) ? "" : "s"));
      end
      if (bus_cmd_has_write_data(bus_req.opcode) == 1'b1) begin
         return $sformatf("%0s 0x%x (%0s) <- %0s", get_bus_cmd_name(bus_req.opcode), bus_req.address, message, format_data(bus_req.cmd_size, bus_req.data, bus_req.address, bus_req.mask));
      end
      return $sformatf("%0s 0x%x (%0s)", get_bus_cmd_name(bus_req.opcode), bus_req.address, message);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to format a TileLink response in a user-friendly form.  It returns a
   // string that is the formatted output.
   //
   // Input Variables:
   //
   // bus_rsp:       This is the bus response structure that contains essential response
   //                information.
   //                -------------------------------------------------------------------------------
   // bus_req_valid: This input should be set to 1'b1 if a valid bus request structure is being
   //                passed in.
   //                -------------------------------------------------------------------------------
   // bus_req:       This is the bus request structure that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the response information.
   //
   virtual function string sprint_rsp(ref   evl_tlink_bus_rsp_s bus_rsp,
                                      input bit                 bus_req_valid = 1'b0,
                                      input evl_tlink_bus_req_s bus_req       = evl_tlink_bus_req_s'(0));
      int    rsp_size;
      string message;

      rsp_size = (1 << bus_rsp.data_size);
      if (bus_rsp.param == TL_param_none) begin
         if (bus_rsp.opcode inside { TLE_GrantAck }) begin
            message = $sformatf("0x%0x, %0d byte%0s", bus_rsp.sink, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
         else if (bus_rsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
            message = $sformatf("0x%0x,0x%0x, %0d byte%0s", bus_rsp.source, bus_rsp.sink, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
         else begin
            message = $sformatf("0x%0x, %0d byte%0s", bus_rsp.source, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
      end
      else begin
         if (bus_rsp.opcode inside { TLE_GrantAck }) begin
            message = $sformatf("%p, 0x%0x, %0d byte%0s", bus_rsp.param, bus_rsp.sink, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
         else if (bus_rsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
            message = $sformatf("%p, 0x%0x,0x%0x, %0d byte%0s", bus_rsp.param, bus_rsp.source, bus_rsp.sink, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
         else begin
            message = $sformatf("%p, 0x%0x, %0d byte%0s", bus_rsp.param, bus_rsp.source, rsp_size, ((rsp_size == 1) ? "" : "s"));
         end
      end
      if (bus_rsp_has_data(bus_rsp.opcode) == 1'b1) begin
         if (bus_req_valid == 1'b1) begin
            return $sformatf("%0s (%0s) %0s", get_bus_rsp_name(bus_rsp.opcode), message, format_data(bus_rsp.data_size, bus_rsp.data, bus_req.address, ~evl_xword_byte_en_t'(0)));
         end
         return $sformatf("%0s (%0s) %0s", get_bus_rsp_name(bus_rsp.opcode), message, format_data(`EVL_XWORD_ADDR_LSB, bus_req.data, evl_paddr_t'(0), ~evl_xword_byte_en_t'(0)));
      end
      return $sformatf("%0s (%0s)", get_bus_rsp_name(bus_rsp.opcode), message);
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   // This function is called by the evl_tlink_interface module when a request is detected.  For
   // multi-beat requests, all of the request information is gathered before sending a single
   // request to the TileLink monitor.  As a result, you will only see request logging when the
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
   // bus_req: This is the bus request structure that contains essential request information.
   //
   virtual function void capture_req(input evl_tlink_bus_req_s bus_req);
      int channel;
      int num_beats;
      int req_bytes;
      bit has_data;

      req_bytes = 1 << bus_req.cmd_size;
      channel   = (int'(bus_req.opcode) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;

      if (bus_cmd_has_write_data(bus_req.opcode) == 1'b0) begin
         num_beats = 1;
         has_data  = 1'b0;
      end
      else begin
         num_beats = ((req_bytes > m_req_data_bus_width) ? (req_bytes / m_req_data_bus_width) : 1);
         has_data  = 1'b1;
      end

      if (m_req_beat_cnt[channel] == 0) begin
         m_req_trans_id[channel] = bus_req.trans_id;
      end
      m_req_beat_cnt[channel]++;

      m_req_mask[channel] = m_req_mask[channel] | (bus_req.mask << ((m_req_beat_cnt[channel] - 1) * m_req_data_bus_width));
      m_req_data[channel] = m_req_data[channel] | (bus_req.data << ((m_req_beat_cnt[channel] - 1) * (m_req_data_bus_width << 3)));
      bus_req.mask        = m_req_mask[channel];
      bus_req.data        = m_req_data[channel];

      //
      // If this is the last data beat, clean things up, print the request, and pass it on to an
      // external monitor.
      //
      if (m_req_beat_cnt[channel] >= num_beats) begin
         int beat_offset;

         beat_offset             = ((bus_req.address & (`EVL_XWORD_BYTES - 1)) >> m_req_data_bus_clog) + 1 - num_beats;
         m_req_beat_cnt[channel] = 0;
         m_req_mask[channel]     = evl_xword_byte_en_t'(0);
         m_req_data[channel]     = evl_xword_t'(0);
         m_req_by_source[bus_req.source] = bus_req;
         if (beat_offset > 0) begin
            bus_req.mask = bus_req.mask << (beat_offset * m_req_data_bus_width);
            bus_req.data = bus_req.data << ((beat_offset * m_req_data_bus_width) << 3);
         end
         if (check_verbosity() == 1'b1) begin
            log_message(sprint_obj(bus_req));
         end
         if (m_obs_req_mb != null) begin
            m_obs_req_mb.try_put(bus_req);
         end
      end
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   // This function is called by the evl_tlink_interface module when a response is detected.  For
   // multi-beat responses, all of the response information is gathered before sending a single
   // response to the TileLink monitor.  As a result, you will only see response logging when the
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
   //
   virtual function void capture_rsp(input evl_tlink_bus_rsp_s bus_rsp);
      int channel;
      int num_beats;
      int rsp_bytes;
      bit has_data;

      channel   = (int'(bus_rsp.opcode) & `EVL_TL_RSP_CHANNEL_MASK) >> `EVL_TL_RSP_CHANNEL_SHIFT;
      rsp_bytes = (1 << bus_rsp.data_size);

      if ((bus_rsp_has_data(bus_rsp.opcode) == 1'b0) || (channel == (`EVL_TL_RSP_CHANNEL_E >> `EVL_TL_RSP_CHANNEL_SHIFT))) begin
         num_beats = 1;
         has_data  = 1'b0;
      end
      else begin
         num_beats = ((rsp_bytes > m_rsp_data_bus_width) ? (rsp_bytes / m_rsp_data_bus_width) : 1);
         has_data  = 1'b1;
      end
      m_rsp_beat_cnt[channel]++;

      m_rsp_data[channel] = m_rsp_data[channel] | (bus_rsp.data << ((m_rsp_beat_cnt[channel] - 1) * (m_rsp_data_bus_width << 3)));
      bus_rsp.data        = m_rsp_data[channel];

      //
      // If this is the last data beat, clean things up, print the response, and pass it on to an
      // external monitor.
      //
      if (m_rsp_beat_cnt[channel] >= num_beats) begin
         m_rsp_beat_cnt[channel] = 0;
         m_rsp_data[channel]     = evl_xword_t'(0);
         if (bus_rsp.opcode inside { TLE_GrantAck }) begin
            if (check_verbosity() == 1'b1) begin
               log_message(sprint_rsp(bus_rsp));
            end
            m_req_by_sink.delete(bus_rsp.sink);
         end
         else begin
            if (m_req_by_source.exists(bus_rsp.source)) begin
               int beat_offset;

               beat_offset = ((m_req_by_source[bus_rsp.source].address & (`EVL_XWORD_BYTES - 1)) >> m_rsp_data_bus_clog) + 1 - num_beats;
               if (beat_offset > 0) begin
                  bus_rsp.data = bus_rsp.data << ((beat_offset * m_rsp_data_bus_width) << 3);
               end
            end
            if (bus_rsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
               if (m_req_by_source.exists(bus_rsp.source)) begin
                  if (check_verbosity() == 1'b1) begin
                     log_message(sprint_rsp(bus_rsp, 1'b1, m_req_by_source[bus_rsp.source]));
                  end
                  m_req_by_sink[bus_rsp.sink] = m_req_by_source[bus_rsp.source];
               end
               else begin
                  if (check_verbosity() == 1'b1) begin
                     log_message(sprint_rsp(bus_rsp));
                  end
               end
               m_req_by_source.delete(bus_rsp.source);
            end
            else begin
               if (m_req_by_source.exists(bus_rsp.source)) begin
                  if (check_verbosity() == 1'b1) begin
                     log_message(sprint_rsp(bus_rsp, 1'b1, m_req_by_source[bus_rsp.source]));
                  end
                  m_req_by_sink[bus_rsp.sink] = m_req_by_source[bus_rsp.source];
               end
               else begin
                  if (check_verbosity() == 1'b1) begin
                     log_message(sprint_rsp(bus_rsp));
                  end
               end
               m_req_by_source.delete(bus_rsp.source);
            end
         end
         if (m_obs_rsp_mb != null) begin
            m_obs_rsp_mb.try_put(bus_rsp);
         end
      end
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_mailboxes
   //
   // This function is to create request and response mailboxes and should only be used if there is
   // an external monitor that intends to use them (via get_obs_req() and get_obs_rsp() calls).
   //
   virtual function create_mailboxes();
      if (m_obs_req_mb == null) begin
         m_obs_req_mb = new();
      end
      if (m_obs_rsp_mb == null) begin
         m_obs_rsp_mb = new();
      end
   endfunction : create_mailboxes


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_obs_req
   //
   // This task retrieves a bus request object from the mailbox.  This task is intended to be used
   // by an external monitor (should it exist).  Note that this task is blocking; it will not return
   // until there is something to observe.  A bus request structure is returned as an output
   // variable upon completion.
   //
   virtual task get_obs_req(output evl_tlink_bus_req_s bus_req);
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
   virtual task get_obs_rsp(output evl_tlink_bus_rsp_s bus_rsp);
      if (m_obs_rsp_mb != null) begin
         m_obs_rsp_mb.get(bus_rsp);
      end
      else begin
         wait (0);
      end
   endtask : get_obs_rsp


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
   // Constructor
   //
   function new(input string         full_name_in = "usr_tlink_rtl_port",
                input evl_agent_id_t agent_id_in  = ~evl_agent_id_t'(0));
      m_agent_id = agent_id_in;

      for (int ii = 0; ii < 4; ii++) begin
         m_req_beat_cnt[ii] = 0;
         m_req_trans_id[ii] = evl_trans_id_t'(0);
         m_req_mask[ii]     = evl_xword_byte_en_t'(0);
         m_req_data[ii]     = evl_xword_t'(0);
      end

      for (int ii = 0; ii < 4; ii++) begin
         m_rsp_beat_cnt[ii] = 0;
         m_rsp_data[ii]     = evl_xword_t'(0);
      end

      m_req_data_bus_width = 16;
      m_rsp_data_bus_width = 16;
      m_req_data_bus_clog  = $clog2(m_req_data_bus_width);
      m_rsp_data_bus_clog  = $clog2(m_rsp_data_bus_width);

      //
      // If there is an external monitor available, mailboxes can be created by a call to
      // create_mailboxes().
      //
      // create_mailboxes();
   endfunction : new
endclass : usr_tlink_rtl_port

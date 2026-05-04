//-----------------------------------------------------------------------------------------------------
//
//
// Class: usr_etlink_rtl_port
//
// This class object is used by the evl_etlink_interface to capture ET Link requests and responses.
// It can be used simply to print requests and responses as they are observed, or it can be used to
// pass packaged requests and responses to an external monitor (the external monitor would need to
// have a handle to the same object instantiated in the evl_etlink_interface module; the get_obs_req()
// and get_obs_rsp() task calls could then be used get captured requests and responses).
//
// This class can either be modified or extended by users of this library; any modifications are the
// sole responsibility of the modifier.
//
class usr_etlink_rtl_port;

   //
   // Create a couple of strings to identify messages from this particular object.  In addition,
   // define an agent ID that can be used to abstractly identify the bus for this object.
   //
   evl_agent_id_t  m_agent_id      = ~evl_agent_id_t'(0);
   string          m_abstract_name = "ETLINK";
   string          m_rtl_name      = "unknown";
   string          m_maxion_source_names[$];
   string          m_minion_source_names[$];
   evl_core_type_t m_core_type;


   //
   // Define the request and response data width in bytes.  The default width is currently 16 bytes.
   //
   int m_req_data_bus_width;
   int m_rsp_data_bus_width;
   int m_req_data_bus_clog;
   int m_rsp_data_bus_clog;


   //
   // The following variables are per-source-ID objects used to correlate responses back to their
   // original requests.
   //
   evl_etlink_bus_req_s m_req_by_id[evl_bus_req_id_t];


   //
   // The following variables are per-request-bank objects used to track multi-beat operations.
   //
   evl_trans_id_t m_req_trans_id[16];
   int            m_req_beat_cnt[16];
   evl_xword_t    m_req_data[16];


   //
   // The following variables are used to track multi-beat responses.
   //
   int         m_rsp_beat_cnt;
   evl_xword_t m_rsp_data;


   //
   // Mailboxes for collecting bus requests from the interface to send to an external monitor.
   //
   mailbox #(evl_etlink_bus_req_s) m_obs_req_mb;
   mailbox #(evl_etlink_bus_rsp_s) m_obs_rsp_mb;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name of this object.
   //
   virtual function void set_abstract_name(input string abstract_name_in);
      m_abstract_name = $sformatf("%s(ETLINK)", abstract_name_in);
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
   // identify a particular ET Link bus within a specific module hierarchy.
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
   // Function: set_core_type
   //
   // This function is used to set the core type for this object.
   //
   virtual function void set_core_type(input evl_core_type_t core_type_in);
      m_core_type = core_type_in;
   endfunction : set_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_type
   //
   // This function is used to get the core type for this object.
   //
   virtual function evl_core_type_t get_core_type();
      return m_core_type;
   endfunction : get_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_off
   //
   // This function can be used to notify a parent class object that the power is now off.
   //
   virtual function void power_off();
      return;
   endfunction : power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_on
   //
   // This function can be used to notify a parent class object that the power is now on.
   //
   virtual function void power_on();
      return;
   endfunction : power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assert_reset
   //
   // This function can be used to notify a parent class object that reset has been asserterd.
   //
   virtual function void assert_reset();
      return;
   endfunction : assert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: deassert_reset
   //
   // This function can be used to notify a parent class object that reset has been deasserted.
   //
   virtual function void deassert_reset();
      return;
   endfunction : deassert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_clock
   //
   // This function can be used to notify a parent class object that a rising clock edge has been
   // detected.
   //
   virtual function void ping_clock();
      return;
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
      evl_etlink_req_opcode_t req_opcode = evl_etlink_req_opcode_t'(bus_cmd_in);

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
      if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_Read, ET_LINK_REQ_Atomic }) begin
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
      if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_Write, ET_LINK_REQ_Atomic }) begin
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
      if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_Atomic }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : bus_cmd_is_atomic


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
      if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_Read, ET_LINK_REQ_Atomic }) begin
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
      if (evl_etlink_req_opcode_t'(bus_cmd_in) inside { ET_LINK_REQ_Write, ET_LINK_REQ_Atomic }) begin
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
      evl_etlink_rsp_opcode_t rsp_opcode = evl_etlink_rsp_opcode_t'(bus_rsp_in);

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
      if (evl_etlink_rsp_opcode_t'(bus_rsp_in) inside { ET_LINK_RSP_AckData }) begin
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
   // Function: get_source_name
   //
   // This function is used to decode the source field in an ET Link request.
   //
   // Input Variables:
   //
   // bus_req: This is the bus request structure that contains essential request information.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the request information.
   //
   virtual function string get_source_name(input evl_bus_req_source_t bus_source_in);
      if (m_core_type == EVL_MaxionCore) begin
         return m_maxion_source_names[bus_source_in];
      end
      return m_minion_source_names[bus_source_in];
   endfunction : get_source_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to format a ET Link request in a user-friendly form.  It returns a
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
   virtual function string sprint_obj(ref evl_etlink_bus_req_s bus_req);
      int                 req_size;
      evl_xword_byte_en_t mask;
      string              message;

      req_size = (1 << bus_req.data_size);
      mask     = evl_xword_byte_en_t'(0);
      if (bus_req.qwen[0:0] == 1'b1) begin
         mask |= (evl_xword_byte_en_t'(1) << `EVL_QWORD_BYTES) - 1;
      end
      if (bus_req.qwen[1:1] == 1'b1) begin
         mask |= ((evl_xword_byte_en_t'(1) << `EVL_QWORD_BYTES) - 1) << `EVL_QWORD_BYTES;
      end
      if (bus_req.qwen[2:2] == 1'b1) begin
         mask |= ((evl_xword_byte_en_t'(1) << `EVL_QWORD_BYTES) - 1) << (`EVL_QWORD_BYTES * 2);
      end
      if (bus_req.qwen[3:3] == 1'b1) begin
         mask |= ((evl_xword_byte_en_t'(1) << `EVL_QWORD_BYTES) - 1) << (`EVL_QWORD_BYTES * 3);
      end
      if (et_link_req_opcode_t'(bus_req.opcode) inside { ET_LINK_REQ_Read, ET_LINK_REQ_Write }) begin
         message = $sformatf("%s, %0p, %0d byte%0s", get_source_name(bus_req.source), bus_req.subopcode, req_size, ((req_size == 1) ? "" : "s"));
      end
      else begin
         message = $sformatf("%s, %0d byte%0s", get_source_name(bus_req.source), req_size, ((req_size == 1) ? "" : "s"));
      end
//    if (bus_cmd_is_cache_op(bus_req.opcode) == 1'b1) begin
//       string start_level;
//       string dest_level;

//       case (req_desc.get_bus_cop_start_level())
//          0:       start_level = "L1";
//          1:       start_level = "L2";
//          2:       start_level = "L3";
//          default: start_level = "--";
//       endcase

//       case (req_desc.get_bus_cop_dest_level())
//          0:       dest_level = "--";
//          1:       dest_level = "L2";
//          2:       dest_level = "L3";
//          default: dest_level = "Mem";
//       endcase // case (req_desc.get_bus_cop_dest_level())
//       case (et_link_req_opcode_t'(bus_req.opcode))
//          ET_LINK_REQ_FlushToMem,
//          ET_LINK_REQ_EvictToMem: message = $sformatf("%0s 0x%x (%0s, ID:0x%02x) <- 0x%0s",
//                                               get_bus_cmd_name(bus_req.opcode), bus_req.address, message, bus_req.id, format_data(bus_req.data_size, bus_req.data, bus_req.address, mask));
//          ET_LINK_REQ_Lock:       message = $sformatf("%0s 0x%x (%0s, ID:0x%02x)",
//                                               get_bus_cmd_name(bus_req.opcode), bus_req.address, message, bus_req.id));
//          ET_LINK_REQ_Unlock:     begin
//                                     message = $sformatf("%0s (%0s) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
//                                                  req_desc.get_bus_cmd_name(), ((req_desc.get_bus_cop_final_state() == 1'b1) ? "preserve" : "invalidate"), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
//                                  end

//          ET_LINK_REQ_ScpFill:    begin
//                                     message = $sformatf("%0s 0x%x -> 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
//                                                  req_desc.get_bus_cmd_name(), req_desc.get_bus_scp_fill_addr(), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
//                                  end

//          ET_LINK_REQ_Prefetch:   begin
//                                     message = $sformatf("%0s (%0s) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
//                                                  req_desc.get_bus_cmd_name(), dest_level, req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
//                                  end

//          default:                begin
//                                     message = $sformatf("%0s (UNKNOWN) 0x%x (ID:0x%02x, TrID 0x%x, id %0d%0s)%0s",
//                                                  req_desc.get_bus_cmd_name(), req_desc.get_paddr(), req_desc.get_bus_req_id(), req_desc.get_trans_id(), req_desc.get_inst_id(), ((req_desc.is_done() == 1'b1) ? ", done" : ""), ((req_desc.is_bus_child() == 1'b1) ? "*" : ""));
//                                  end
//       endcase
//    end
      if (bus_cmd_has_write_data(bus_req.opcode) == 1'b1) begin
         return $sformatf("%0s 0x%x (%0s, ID:0x%02x) <- 0x%0s",
                   get_bus_cmd_name(bus_req.opcode), bus_req.address, message, bus_req.id, format_data(bus_req.data_size, bus_req.data, bus_req.address, mask));
      end
      return $sformatf("%0s 0x%x (%0s, ID:0x%02x)",
                get_bus_cmd_name(bus_req.opcode), bus_req.address, message, bus_req.id);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to format a ET Link response in a user-friendly form.  It returns a
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
   virtual function string sprint_rsp(ref   evl_etlink_bus_rsp_s bus_rsp,
                                      input bit                  bus_req_valid = 1'b0,
                                      input evl_etlink_bus_req_s bus_req       = evl_etlink_bus_req_s'(0));
      if (bus_rsp_has_data(bus_rsp.opcode) == 1'b1) begin
         if (bus_req_valid == 1'b1) begin
            return $sformatf("%0s (ID:0x%02x) %0s", get_bus_rsp_name(bus_rsp.opcode), bus_rsp.id, format_data(bus_req.data_size, bus_rsp.data, bus_req.address, ~evl_xword_byte_en_t'(0)));
         end
         return $sformatf("%0s (ID:0x%02x) %0s", get_bus_rsp_name(bus_rsp.opcode), bus_rsp.id, format_data(`EVL_XWORD_ADDR_LSB, bus_req.data, evl_paddr_t'(0), ~evl_xword_byte_en_t'(0)));
      end
      return $sformatf("%0s (ID:0x%02x)", get_bus_rsp_name(bus_rsp.opcode), bus_rsp.id);
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   // This function is called by the evl_etlink_interface module when a request is detected.  For
   // multi-beat requests, all of the request information is gathered before sending a single
   // request to the ET Link monitor.  As a result, you will only see request logging when the
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
   virtual function void capture_req(input bit [3:0]             bank_in,
                                     input evl_trans_id_t        trans_id_in,
                                     input evl_etlink_req_info_t req_info_in,
                                     input bit                   cached);
      evl_etlink_bus_req_s bus_req;
      et_link_req_opcode_t bus_cmd;
      int                  num_beats;
      int                  status = 0;
      int                  req_bytes;
      int                  bus_bytes;

      req_bytes = (1 << req_info_in.size);
      bus_bytes = m_req_data_bus_width;
      bus_cmd   = req_info_in.opcode;

      if ((req_bytes > bus_bytes) && (req_info_in.wdata == 1'b1)) begin
         // Requesting more bytes than the bus width ... need multiple beats.
         num_beats = (req_bytes / bus_bytes);
      end
      else begin
         num_beats = 1;
      end

      bus_req.bank = bank_in;

      if (m_req_beat_cnt[bank_in] == 0) begin
         m_req_trans_id[bank_in] = trans_id_in;
      end
      m_req_data[bank_in] = m_req_data[bank_in] | (req_info_in.data  << (m_req_beat_cnt[bank_in] * (m_req_data_bus_width << 3)));

      bus_req.trans_id    = m_req_trans_id[bank_in];
      bus_req.id          = evl_bus_req_id_t'(req_info_in.id);
      bus_req.source      = evl_bus_req_source_t'(req_info_in.source);
      bus_req.opcode      = evl_etlink_req_opcode_t'(req_info_in.opcode);
      bus_req.cached      = cached;
      bus_req.address     = evl_paddr_t'(req_info_in.address);
      bus_req.wdata       = bit'(req_info_in.wdata);
      bus_req.data        = m_req_data[bank_in];
      bus_req.data_size   = evl_bus_req_size_t'(req_info_in.size);
      bus_req.qwen        = evl_line_qword_en_t'(req_info_in.qwen);
      bus_req.subopcode   = evl_bus_subopcode_t'(req_info_in.subopcode);

      m_req_beat_cnt[bank_in] += 1;
      if (m_req_beat_cnt[bank_in] >= num_beats) begin
         m_req_beat_cnt[bank_in] = 0;
         m_req_data[bank_in]     = evl_xword_t'(0);

         //
         // Minions post all writes to the same ID.
         //
         if ((bus_cmd_is_read(bus_req.opcode) == 1'b1) || (m_core_type == EVL_MaxionCore)) begin
            m_req_by_id[bus_req.id] = bus_req;
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
   // This function is called by the evl_etlink_interface module when a response is detected.  For
   // multi-beat responses, all of the response information is gathered before sending a single
   // response to the ET Link monitor.  As a result, you will only see response logging when the
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
   function void capture_rsp(input evl_etlink_rsp_info_t rsp_info_in, input bit use_rsp_source_is_uc, input bit rsp_source_is_uc);
      evl_etlink_bus_rsp_s bus_rsp;
      int                  num_beats;
      int                  status;
      int                  rsp_bytes;

      rsp_bytes = (1 << rsp_info_in.size);
      num_beats = ((rsp_bytes > m_rsp_data_bus_width) && (rsp_info_in.wdata == 1'b1)) ? (rsp_bytes / m_rsp_data_bus_width) : 1;

      bus_rsp.id        = evl_bus_req_id_t'(rsp_info_in.id);
      bus_rsp.dest      = evl_bus_rsp_dest_t'(rsp_info_in.dest);
      bus_rsp.opcode    = evl_etlink_rsp_opcode_t'(rsp_info_in.opcode);
      bus_rsp.data      = evl_xword_t'(rsp_info_in.data);
      m_rsp_data        = m_rsp_data | (rsp_info_in.data  << (m_rsp_beat_cnt * (m_rsp_data_bus_width << 3)));
      bus_rsp.data      = m_rsp_data;
      bus_rsp.data_size = evl_etlink_size_t'(rsp_info_in.size);
      bus_rsp.qwen      = evl_line_qword_en_t'(rsp_info_in.qwen);
      bus_rsp.wdata     = rsp_info_in.wdata;
      bus_rsp.cached    = { use_rsp_source_is_uc, use_rsp_source_is_uc & rsp_source_is_uc };
      m_rsp_beat_cnt    = m_rsp_beat_cnt + 1;

      if (m_rsp_beat_cnt == num_beats) begin
         m_rsp_beat_cnt = 0;
         m_rsp_data     = evl_xword_t'(0);
         if (m_req_by_id.exists(bus_rsp.id)) begin
            if (check_verbosity() == 1'b1) begin
               log_message(sprint_rsp(bus_rsp, 1'b1, m_req_by_id[bus_rsp.id]));
            end
            m_req_by_id.delete(bus_rsp.id);
         end
         else begin
            if (check_verbosity() == 1'b1) begin
               log_message(sprint_rsp(bus_rsp));
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
   virtual task get_obs_req(output evl_etlink_bus_req_s bus_req);
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
   virtual task get_obs_rsp(output evl_etlink_bus_rsp_s bus_rsp);
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
   function new(input string         full_name_in = "usr_etlink_rtl_port",
                input evl_agent_id_t agent_id_in  = ~evl_agent_id_t'(0));
      evl_bus_req_source_t      req_source;
      evl_etlink_neigh_source_t neigh_source;
      int                       index;
      string                    source_name;

      m_agent_id = agent_id_in;

      for (int ii = 0; ii < 16; ii++) begin
         m_req_beat_cnt[ii] = 0;
         m_req_trans_id[ii] = evl_trans_id_t'(0);
         m_req_data[ii]     = evl_xword_t'(0);
      end

      m_rsp_data     = evl_xword_t'(0);
      m_rsp_beat_cnt = 0;

      m_req_data_bus_width = 16;
      m_rsp_data_bus_width = 16;
      m_req_data_bus_clog  = $clog2(m_req_data_bus_width);
      m_rsp_data_bus_clog  = $clog2(m_rsp_data_bus_width);

      m_maxion_source_names.push_back("Src0x0");
      m_minion_source_names.push_back("Src0x0");
      req_source = evl_bus_req_source_t'(1);
      while (req_source != evl_bus_req_source_t'(0)) begin
         m_maxion_source_names.push_back($sformatf("Src0x%0x", req_source));
         m_minion_source_names.push_back($sformatf("Src0x%0x", req_source));
         req_source += evl_bus_req_source_t'(1);
      end
      m_maxion_source_names[0] = "SC_Mem";
      m_maxion_source_names[1] = "SC_MMIO";
      m_maxion_source_names[2] = "SC_GAG";

      neigh_source = neigh_source.first();
      while (neigh_source != neigh_source.last()) begin
         source_name  = neigh_source.name();
         index        = source_name.len() - 1;
         while (index > 0) begin
            if (source_name.getc(index) == byte'("_")) begin
               index++;
               break;
            end
            index--;
         end
         if (index > 0) begin
            source_name = source_name.substr(index, source_name.len() - 1);
         end
         m_minion_source_names[int'(neigh_source)] = source_name;
         neigh_source = neigh_source.next();
      end
      source_name  = neigh_source.name();
      index        = source_name.len() - 1;
      while (index > 0) begin
         if (source_name.getc(index) == byte'("_")) begin
            index++;
            break;
         end
         index--;
      end
      if (index > 0) begin
         source_name = source_name.substr(index, source_name.len() - 1);
      end
      m_minion_source_names[int'(neigh_source)] = source_name;

      m_core_type = EVL_MaxionCore;

      //
      // If there is an external monitor available, mailboxes can be created by a call to
      // create_mailboxes().
      //
      // create_mailboxes();
   endfunction : new

endclass : usr_etlink_rtl_port

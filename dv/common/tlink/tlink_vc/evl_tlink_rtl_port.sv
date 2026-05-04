//-----------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_rtl_port
//
class evl_tlink_rtl_port extends evl_rtl_port;

   //
   // The bus parser to use for TileLink requests is evl_tlink_bus_parser.
   //
   static evl_tlink_bus_parser m_bus_parser = evl_tlink_bus_parser::get_inst();


   //
   // Define the request and response data width in bytes.  The default width is currently 16 bytes.
   //
   int m_req_data_bus_width = 16;
   int m_rsp_data_bus_width = 16;


   //
   // The following variables are per-request-channel objects used to track multi-beat operations.
   //
   evl_trans_id_t      m_req_trans_id[4];
   int                 m_req_beat_cnt[4];
   evl_xword_byte_en_t m_req_mask[4];
   evl_xword_t         m_req_data[4];


   //
   // The following variables are per-response-channel objects used to track multi-beat operations.
   //
   int                 m_rsp_beat_cnt[4];
   evl_xword_t         m_rsp_data[4];


   //
   // Mailboxes for monitoring/collecting bus requests from the interface to send to the monitor.
   //
   mailbox #(evl_tlink_bus_req_s) m_obs_req_mb;
   mailbox #(evl_tlink_bus_rsp_s) m_obs_rsp_mb;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_req
   //
   // This function receives a bus_req object from the master driver and keeps it in the mailbox
   // until ready to be placed on the bus interface.  There is currently no master stimulus driver
   // for TileLink.
   //
   virtual function void put_req(input evl_tlink_bus_req bus_req);
      return;
   endfunction : put_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_rsp
   //
   // This function receives a bus_rsp object from the slave driver and keeps it in the mailbox
   // until ready to be placed on the bus interface.  There is currently no slave stimulus driver
   // for TileLink.
   //
   virtual function void put_rsp(input evl_tlink_bus_rsp bus_rsp);
      return;
   endfunction : put_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   // This function is called by the evl_tlink_interface module when a request is detected.  For
   // multi-beat requests, all of the request information is gathered before sending a single
   // request to the TileLink monitor.  As a result, you will only see request logging when the
   // last beat is detected.
   //
   function void capture_req(input evl_tlink_bus_req_s bus_req);
      int channel;
      int num_beats;
      int req_bytes;
      bit has_data;

      req_bytes = 1 << bus_req.cmd_size;
      channel   = (int'(bus_req.opcode) & `EVL_TL_REQ_CHANNEL_MASK) >> `EVL_TL_REQ_CHANNEL_SHIFT;

      if (m_bus_parser.bus_cmd_has_write_data(bus_req.opcode) == 1'b0) begin
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

      `evl_log(UVM_HIGH, VMOD_TLINK_IFC, $sformatf("%0s(TLINK-IFC)", get_root_abstract_name()), $sformatf("capturing request %s 0x%x (ID 0x%0x) (beat %0d/%0d)%0s",
         m_bus_parser.get_bus_cmd_name(bus_req.opcode), bus_req.address, bus_req.source, m_req_beat_cnt[channel], num_beats, (has_data == 1'b1) ? $sformatf(" data: 0x%0x", bus_req.data) : ""));

      m_req_mask[channel] = m_req_mask[channel] | (bus_req.mask << ((m_req_beat_cnt[channel] - 1) * m_req_data_bus_width));
      m_req_data[channel] = m_req_data[channel] | (bus_req.data << ((m_req_beat_cnt[channel] - 1) * (m_req_data_bus_width << 3)));
      bus_req.mask        = m_req_mask[channel];
      bus_req.data        = m_req_data[channel];

      if (m_req_beat_cnt[channel] >= num_beats) begin
         m_obs_req_mb.try_put(bus_req);
         m_req_beat_cnt[channel] = 0;
         m_req_mask[channel]     = evl_xword_byte_en_t'(0);
         m_req_data[channel]     = evl_xword_t'(0);
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
   function void capture_rsp(input evl_tlink_bus_rsp_s bus_rsp);
      int channel;
      int num_beats;
      int rsp_bytes;
      bit has_data;

      channel   = (int'(bus_rsp.opcode) & `EVL_TL_RSP_CHANNEL_MASK) >> `EVL_TL_RSP_CHANNEL_SHIFT;
      rsp_bytes = (1 << bus_rsp.data_size);

      if ((m_bus_parser.bus_rsp_has_data(bus_rsp.opcode) == 1'b0) || (channel == (`EVL_TL_RSP_CHANNEL_E >> `EVL_TL_RSP_CHANNEL_SHIFT))) begin
         num_beats = 1;
         has_data  = 1'b0;
      end
      else begin
         num_beats = ((rsp_bytes > m_rsp_data_bus_width) ? (rsp_bytes / m_rsp_data_bus_width) : 1);
         has_data  = 1'b1;
      end
      m_rsp_beat_cnt[channel]++;

      `evl_log(UVM_HIGH, VMOD_TLINK_IFC, $sformatf("%0s(TLINK-IFC)", get_root_abstract_name()), $sformatf("capturing response %0s (ID 0x%0x, 0x%0x) (beat %0d/%0d)%0s",
         m_bus_parser.get_bus_rsp_name(bus_rsp.opcode), bus_rsp.source, bus_rsp.sink, m_rsp_beat_cnt[channel], num_beats, (has_data == 1) ? $sformatf(" data: 0x%0x", bus_rsp.data) : ""));

      m_rsp_data[channel] = m_rsp_data[channel] | (bus_rsp.data << ((m_rsp_beat_cnt[channel] - 1) * (m_rsp_data_bus_width << 3)));
      bus_rsp.data        = m_rsp_data[channel];

      if (m_rsp_beat_cnt[channel] >= num_beats) begin
         m_obs_rsp_mb.try_put(bus_rsp);
         m_rsp_beat_cnt[channel] = 0;
         m_rsp_data[channel]     = evl_xword_t'(0);
      end
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_rsp
   //
   // This function retrieves a bus response object from the mailbox (it is used by the monitor).
   //
   virtual task get_obs_rsp(output evl_tlink_bus_rsp_s bus_rsp);
      m_obs_rsp_mb.get(bus_rsp);
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_req
   //
   // This function retrieves a bus request object from the mailbox (it is used by the monitor).
   //
   virtual task get_obs_req(output evl_tlink_bus_req_s bus_req);
      m_obs_req_mb.get(bus_req);
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in     = "tlink_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {},
                input int            additional_hier  = 0);
      super.new(full_name_in, TLINK_BUS_AGENT, agent_pid_in, "evl_tlink_verif_comp", "", "", "", skip_levels_in, 0, 0, additional_hier);

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

      m_obs_req_mb = new();
      m_obs_rsp_mb = new();
   endfunction : new
endclass

//-----------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_rtl_port
//
class evl_etlink_rtl_port extends evl_rtl_port;

   int m_last_req_cycle     = 0;
   int m_port_count         = 1;
   int m_port_mask          = 0;
   int m_req_data_bus_width = 64;
   int m_rsp_data_bus_width = 64;


   bit       m_req_drive = 1'b0;
   bit       m_rsp_drive = 1'b0;

   bit [7:0] m_req_ready = 8'h01;
   bit       m_rsp_ready = 1'b1;

   evl_etlink_bus_req_queue m_req_queues[];
   evl_etlink_bus_rsp       m_rsp_queue[$];

   evl_xword_t              m_req_data[];
   evl_xword_t              m_rsp_data = '0;
   evl_trans_id_t           m_req_trans_id[];


   // mailboxes for monitoring/collecting bus requests from the interface to send to the monitor
   mailbox #(evl_etlink_bus_rsp_s)  m_obs_rsp_mb;
   mailbox #(evl_etlink_bus_req_s)  m_obs_req_mb;

   int m_req_beat_cnt[];
   int m_rsp_beat_cnt = 0;

   evl_bus_req_id_t m_ignored_rsp_ids[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_ignored_rsp_id
   //
   //   This function is used to add specific IDs which should not be checked by the bus VC
   //   This is used by Minion interfaces which share the same response bus, so not all transactions
   //   apply to the VC.
   //
   function void add_ignored_rsp_id(input evl_bus_req_id_t id_in);
      m_ignored_rsp_ids.push_back(id_in);
   endfunction : add_ignored_rsp_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_id_ignored
   //
   //   This function is used to add specific IDs which should not be checked by the bus VC
   //   This is used by Minion interfaces which share the same response bus, so not all transactions
   //   apply to the VC.
   //
   function bit is_id_ignored(input evl_bus_req_id_t id_in);
      bit found = 0;
      foreach (m_ignored_rsp_ids[ii]) begin
         if (m_ignored_rsp_ids[ii] == id_in) return 1'b1;
      end
      return 1'b0;
   endfunction : is_id_ignored


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_req
   //
   // This function receives a bus_req object from the driver and keeps it in the mailbox until
   // ready to be placed on the bus interface.
   //
   virtual function void put_req(input evl_etlink_bus_req bus_req);
      int                      port_num;
      int                      req_cycle;
      int                      num_beats;
      evl_etlink_bus_req_s     bus_req_info;
      evl_etlink_bus_req_queue req_queue;

      bus_req_info = bus_req.get_bus_req_info();
      if (m_port_mask <= 0) begin
         port_num = 0;
      end
      else if (bus_req_info.opcode == ET_LINK_REQ_MsgSendData) begin
         port_num = m_global_attrs.get_bank_from_message(m_shire_id, bus_req_info.address) & m_port_mask;
      end
      else if (bus_req_info.cached == 1'b0) begin
         port_num = m_port_count - 1;
      end
      else begin
         port_num = (bus_req_info.address[`SC_BANK_SEL_ADDR_RANGE] & (`SC_BANKS - 1)) & m_port_mask;
      end
      req_queue = m_req_queues[port_num];
      num_beats = (((2**bus_req_info.data_size) > m_req_data_bus_width) && (bus_req_info.opcode inside {ET_LINK_REQ_Write, ET_LINK_REQ_WriteAround})) ? ((2**bus_req_info.data_size)/m_req_data_bus_width) : 1;
      if (m_delays_are_disabled == 0) begin
         for (int ii = 0; ii < num_beats; ii++) begin
            if (ii == 0) begin
               if (bus_req.get_bus_req_delay() > 1) begin
                  req_cycle = m_cycle_count + int'(bus_req.get_bus_req_delay()) - 1;
               end
               else begin
                  req_cycle = m_cycle_count;
               end
            end
            else begin
               bus_req = bus_req.clone();
               bus_req.m_continue = 1'b1;
               bus_req.m_bus_req_info.data >>= (m_req_data_bus_width << 3); // 8 bit per byte
               bus_req.m_bus_req_info.qwen >>= (m_req_data_bus_width >> 4); // 16 bytes per quadword
               if (bus_req.get_bus_req_data_delay() > 1) begin
                  req_cycle = m_cycle_count + int'(bus_req.get_bus_req_data_delay()) - 1;
               end
               else begin
                  req_cycle = m_cycle_count;
               end
            end
            if ((m_last_req_cycle <= req_cycle) || (req_queue.queue_size() == 0)) begin
               m_last_req_cycle = req_cycle;
            end
            else begin
               req_cycle = m_last_req_cycle;
            end
            bus_req.set_req_cycle(req_cycle);
            req_queue.push_back(bus_req);
         end
      end
      else begin
         m_last_req_cycle = m_cycle_count;
         bus_req.set_req_cycle(m_cycle_count);
         for (int ii = 0; ii < num_beats; ii++) begin
            if (ii > 0) begin
               bus_req = bus_req.clone();
               bus_req.m_continue = 1'b1;
               bus_req.m_bus_req_info.data >>= (m_req_data_bus_width << 3); // 8 bit per byte
               bus_req.m_bus_req_info.qwen >>= (m_req_data_bus_width >> 4); // 16 bytes per quadword
            end
            req_queue.push_back(bus_req);
         end
      end
   endfunction : put_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_rsp
   //
   // This function receives a bus_rsp object from the driver and keeps it in the mailbox until
   // ready to be placed on the bus interface.
   //
   // FUTURE: Emmanuel Marie support for smaller size bus
   //
   virtual function void put_rsp(input evl_etlink_bus_rsp bus_rsp);
      int rsp_cycle;

      if (m_delays_are_disabled == 0) begin
         evl_delay_t rsp_delay = bus_rsp.get_mem_bus_rsp_delay();

         if (rsp_delay > 1) begin
            rsp_cycle = m_cycle_count + int'(rsp_delay) - 1;
         end
         else begin
            rsp_cycle = m_cycle_count;
         end
         bus_rsp.set_rsp_cycle(rsp_cycle);
         if ((m_rsp_queue.size() == 0) || (rsp_cycle >= m_rsp_queue[$].get_rsp_cycle())) begin
            m_rsp_queue.push_back(bus_rsp);
         end
         else begin
            int ii = 0;
            while ((ii < m_rsp_queue.size()) && (rsp_cycle >= m_rsp_queue[ii].get_rsp_cycle())) ii++;
            m_rsp_queue.insert(ii, bus_rsp);
         end
      end
      else begin
         bus_rsp.set_rsp_cycle(m_cycle_count);
         m_rsp_queue.push_back(bus_rsp);
      end
   endfunction : put_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_rsp
   //
   // This function retrieves a bus_rsp object from the mailbox to send to the monitor.
   //
   virtual task get_obs_rsp(output evl_etlink_bus_rsp_s bus_rsp);
      m_obs_rsp_mb.get(bus_rsp);
   endtask : get_obs_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_req
   //
   // This function retrieves a bus_req object from the mailbox to send to the monitor.
   //
   virtual task get_obs_req(output evl_etlink_bus_req_s bus_req);
      m_obs_req_mb.get(bus_req);
   endtask : get_obs_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_drive
   //
   virtual function void set_req_drive();
      m_req_drive = 1'b1;
   endfunction : set_req_drive


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_port_count
   //
   function void set_port_count(input int port_count);
      if (port_count > 9) begin
         m_req_queues = new[9];
      end
      else if (port_count <= 1) begin
         m_req_queues = new[1];
      end
      else begin
         m_req_queues = new[port_count];
      end
      foreach (m_req_queues[ii]) begin
         m_req_queues[ii] = new();
      end
      m_port_count = m_req_queues.size();
      if (m_port_count > 1) begin
         m_port_mask  = (1 << $clog2(port_count)) - 1;
      end

      m_req_beat_cnt = new[m_port_count];
      m_req_trans_id = new[m_port_count];
      m_req_data     = new[m_port_count];
      foreach (m_req_beat_cnt[ii]) m_req_beat_cnt[ii] = '0;
      foreach (m_req_data[ii]) m_req_data[ii] = '0;

   endfunction : set_port_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_drive
   //
   function void get_req_drive(output bit req_drive_out);
      req_drive_out = m_req_drive;
   endfunction : get_req_drive


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_drive
   //
   virtual function void set_rsp_drive();
      m_rsp_drive = 1'b1;
   endfunction : set_rsp_drive


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_drive
   //
   function void get_rsp_drive(output bit rsp_drive_out);
      rsp_drive_out = m_rsp_drive;
   endfunction : get_rsp_drive


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_ready
   //
   function void get_req_ready(output bit [7:0] req_ready_out);
      req_ready_out = m_req_ready;
   endfunction : get_req_ready


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req
   //
   function void get_req(input int port_num, output bit req_valid_out, output evl_etlink_req_info_t req_info_out, output evl_trans_id_t trans_id_out);
      evl_etlink_bus_req_queue req_queue;

      if (port_num >= m_port_count) begin
         req_valid_out = 1'b0;
         req_info_out  = 0;
         trans_id_out  = 0;
         return;
      end
      if (port_num < 0) begin
         req_queue = m_req_queues[0];
      end
      else if (port_num >= m_port_count) begin
         req_queue = m_req_queues[m_port_count - 1];
      end
      else begin
         req_queue = m_req_queues[port_num];
      end
      if (req_queue.queue_size() == 0) begin
         req_valid_out = 1'b0;
         req_info_out  = 0;
         trans_id_out  = 0;
      end
      else if ((m_delays_are_disabled != 0) || (req_queue.get_req_cycle() <= m_cycle_count)) begin
         evl_etlink_bus_req   bus_req;
         evl_etlink_bus_req_s bus_req_info;

         bus_req                = req_queue.pop_front();
         bus_req_info           = bus_req.get_bus_req_info();
         req_valid_out          = 1'b1;
         trans_id_out           = bus_req_info.trans_id;
         req_info_out.id        = bus_req_info.id;
         req_info_out.source    = bus_req_info.source;
         req_info_out.opcode    = bus_req_info.opcode;
         req_info_out.address   = bus_req_info.address;
         req_info_out.wdata     = bus_req_info.wdata;
         req_info_out.data      = bus_req_info.data;
         req_info_out.size      = et_link_size_t'(bus_req_info.data_size);
         req_info_out.qwen      = bus_req_info.qwen;
         req_info_out.subopcode = bus_req_info.subopcode;
      end
   endfunction : get_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_ready
   //
   function void get_rsp_ready(output bit rsp_ready_out);
      rsp_ready_out = m_rsp_ready;
   endfunction : get_rsp_ready


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp
   //
   function void get_rsp(output bit rsp_valid_out, output evl_etlink_rsp_info_t rsp_info_out);
      if (m_rsp_queue.size() == 0) begin
         rsp_valid_out = 1'b0;
         rsp_info_out  = 0;
      end
      else if ((m_delays_are_disabled != 0) || (m_rsp_queue[0].get_rsp_cycle() <= m_cycle_count)) begin
         evl_etlink_bus_rsp   bus_rsp;
         evl_etlink_bus_rsp_s bus_rsp_info;

         rsp_valid_out = 1'b1;
         bus_rsp = m_rsp_queue.pop_front();
         bus_rsp_info = bus_rsp.get_bus_rsp_info();
         rsp_info_out.id     = bus_rsp_info.id;
         rsp_info_out.dest   = bus_rsp_info.dest;
         rsp_info_out.wdata  = bus_rsp_info.wdata;
         rsp_info_out.opcode = bus_rsp_info.opcode;
         rsp_info_out.data   = bus_rsp_info.data;
         rsp_info_out.size   = bus_rsp_info.data_size;
         rsp_info_out.qwen   = bus_rsp_info.qwen;
      end
   endfunction : get_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_req
   //
   //   Note: for multi-beats transaction, assumes that address is always naturally aligned to the
   //   size of the transaction
   //
   function void capture_req(input bit [3:0] bank_in, input evl_trans_id_t trans_id_in, input evl_etlink_req_info_t req_info_in, input bit cached);
      evl_etlink_bus_req_s bus_req;
      et_link_req_opcode_t bus_cmd;
      int                  num_beats;
      int                  status = 0;
      int                  req_bytes;
      int                  bus_bytes;

      req_bytes = 2**req_info_in.size;
      bus_bytes = m_req_data_bus_width;
      bus_cmd   = req_info_in.opcode;

      if ((req_bytes > bus_bytes) && (req_info_in.wdata == 1'b1)) begin
         // I am requesting more bytes than the bus width => I need multiple beats
         if (bus_cmd inside {ET_LINK_REQ_ReadCoop}) begin
            // A ReadCoop is a special case. Only one beat is needed but wdata is set to 1.
            // Even if it does not write data it carries metadata
            num_beats = 1;
         end
         else begin
            // Compute the number of beats
            num_beats = (req_bytes / bus_bytes);
         end
      end
      else begin
         // With a single beat I am getting all the data
         num_beats = 1;
      end

      bus_req.bank = bank_in;

      if (m_req_beat_cnt[bank_in] == 0) begin
         m_req_trans_id[bank_in]  = trans_id_in;
      end
      bus_req.trans_id    = m_req_trans_id[bank_in];
      bus_req.id          = evl_bus_req_id_t'(req_info_in.id);
      bus_req.source      = evl_bus_req_source_t'(req_info_in.source);
      bus_req.opcode      = evl_etlink_req_opcode_t'(req_info_in.opcode);
      bus_req.cached      = cached;
      bus_req.address     = evl_paddr_t'(req_info_in.address);
      bus_req.wdata       = bit'(req_info_in.wdata);
      m_req_data[bank_in] = m_req_data[bank_in] |(req_info_in.data  << (m_req_beat_cnt[bank_in] * (m_req_data_bus_width*8)));
      bus_req.data        = m_req_data[bank_in];
      bus_req.data_size   = evl_bus_req_size_t'(req_info_in.size);
      bus_req.qwen        = evl_line_qword_en_t'(req_info_in.qwen);
      bus_req.subopcode   = evl_bus_subopcode_t'(req_info_in.subopcode);
      m_req_beat_cnt[bank_in] += 1;
      `evl_log(UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", get_root_abstract_name()), $sformatf("capturing request for port %0d (%p 0x%x) (ID 0x%0x) (beat %0d/%0d)%0s",
         bank_in, req_info_in.opcode, req_info_in.address, req_info_in.id, m_req_beat_cnt[bank_in], num_beats, (req_info_in.wdata) ? $sformatf(" data: 0x%0x", req_info_in.data) : ""));

      if (m_req_beat_cnt[bank_in] == num_beats) begin
         m_obs_req_mb.try_put(bus_req);
         m_req_beat_cnt[bank_in] = 0;
         m_req_data[bank_in] = '0;
      end
   endfunction : capture_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_rsp
   //
   //   Note: for multi-beats transaction, assumes that address is always naturally aligned to the
   //   size of the transaction
   //
   function void capture_rsp(input evl_etlink_rsp_info_t rsp_info_in, input bit use_rsp_source_is_uc, input bit rsp_source_is_uc);
      evl_etlink_bus_rsp_s bus_rsp;
      int                  num_beats;
      int                  status;
      int                  rsp_bytes;

      rsp_bytes = 2**rsp_info_in.size;
      num_beats = ((rsp_bytes > m_rsp_data_bus_width) && (rsp_info_in.wdata == 1'b1)) ? (rsp_bytes / m_rsp_data_bus_width) : 1;

      bus_rsp.id        = evl_bus_req_id_t'(rsp_info_in.id);
      bus_rsp.dest      = evl_bus_rsp_dest_t'(rsp_info_in.dest);
      bus_rsp.opcode    = evl_etlink_rsp_opcode_t'(rsp_info_in.opcode);
      bus_rsp.data      = evl_xword_t'(rsp_info_in.data);
      m_rsp_data        = m_rsp_data | (rsp_info_in.data  << (m_rsp_beat_cnt * (m_rsp_data_bus_width*8)));
      bus_rsp.data      = m_rsp_data;
      bus_rsp.data_size = evl_etlink_size_t'(rsp_info_in.size);
      bus_rsp.qwen      = evl_line_qword_en_t'(rsp_info_in.qwen);
      bus_rsp.wdata     = rsp_info_in.wdata;
      bus_rsp.cached    = { use_rsp_source_is_uc, use_rsp_source_is_uc & rsp_source_is_uc };
      m_rsp_beat_cnt    = m_rsp_beat_cnt + 1;

      `evl_log(UVM_HIGH, VMOD_ETLINK_IFC, $sformatf("%0s(ETLINK-IFC)", get_root_abstract_name()), $sformatf("capturing response %p (ID 0x%0x) (beat %0d/%0d)%0s",
         rsp_info_in.opcode, rsp_info_in.id, m_rsp_beat_cnt, num_beats, (rsp_info_in.wdata) ? $sformatf(" data: 0x%0x", rsp_info_in.data) : ""));
      if (m_rsp_beat_cnt == num_beats) begin
         m_obs_rsp_mb.try_put(bus_rsp);
         m_rsp_beat_cnt = 0;
         m_rsp_data = '0;
      end
   endfunction : capture_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in     = "etlink_rtl_port",
                input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {},
                input int            additional_hier  = 0);
      super.new(full_name_in, ETLINK_BUS_AGENT, agent_pid_in, "evl_etlink_verif_comp", "", "", "", skip_levels_in, 0, 0, additional_hier);
      m_obs_req_mb = new();
      m_obs_rsp_mb = new();
   endfunction : new
endclass

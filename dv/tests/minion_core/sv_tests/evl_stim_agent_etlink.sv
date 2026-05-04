//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_agent_etlink
//
// This class contains all of the ports and tasks necessary to be a master
// or a slave. It also includes a cache model that can be used as a memory model
// for a slave or an L2 cache model for a master.
//
// This class tries to be generic and not define exactly what will happen when
// it receives a transaction. Instead it makes a call to a function which returns
// the action it should take. (ex: I'm a master and I received a LD from the sequencer.
// What request should I drive on the bus, and what should I do to my cache model?)
//
// This class is used by first extending it, and in the extension
// creating only the ports that are needed (ie: for a slave or a master) and
// making function calls to fill out a table of what actions to take when.
// Please see evl_neigh_agent_etlink.sv for an example of a master agent or
// evl_l2_agent_etlink.sv for an example of a slave agent.
//
virtual class evl_stim_agent_etlink extends evl_verif_comp;

   typedef evl_bus_req_id_t evl_bus_req_id_q[$];

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Delay Keys
   //
   // pk_req_delay:         This is the delay before processing the transaction in a master stimulus
   //                       agent.
   //                       ------------------------------------------------------------------------
   // pk_bus_req_delay:     This is the delay before sending a bus request (processed in the
   //                       driver).
   //                       ------------------------------------------------------------------------
   // pk_mem_req_delay:     This is the delay before processing an incoming slave request.
   //                       ------------------------------------------------------------------------
   // pk_mem_bus_rsp_delay: This is the delay before sending a bus response (processed in the
   //                       driver).
   //
   `evl_param_key(pk_req_delay)
   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_mem_req_delay)
   `evl_param_key(pk_mem_bus_rsp_delay)
   `evl_param_key(pk_tlm_req_port)


   //-----------------------------------------------------------------------------------------------
   //
   // Member TLM Ports
   //
   uvm_analysis_port     #(evl_req_desc) m_snd_req_ports[$];
   uvm_analysis_port     #(evl_req_desc) m_snd_rsp_ports[$];
   uvm_analysis_export   #(evl_req_desc) m_obs_req_exports[$];
   uvm_analysis_export   #(evl_req_desc) m_rcv_rsp_exports[$];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifos[$];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rcv_rsp_fifos[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int                   m_tables_built          = 0;
   bit                   m_stimulus_busy         = 1'b0;
   bit                   m_count_stimulus        = 1'b0;
   int                   m_active_limit          = 64;    // maximum number of active (waiting for response) descs
   int                   m_issue_limit           = 32;    // maximum number of not-yet-issued descs
   int                   m_trans_count           = 0;
   int                   m_req_count             = 0;
   bit                   m_allow_remote_msg      = 1'b1;
   bit                   m_allow_duplicate_id    = 1'b0;
   bit                   m_print_trans_hierarchy = 1'b0;
   bit                   m_fixup_message         = 1'b1;
   bit                   m_neigh_message         = 1'b0;
   evl_bus_req_id_t      m_bus_req_id_limit      = evl_bus_req_id_t'(255);
   evl_stim_cache_etlink m_cache;
   evl_line_data         m_template_cache_data;
   evl_line_data         m_template_req_data;
   evl_line_desc         m_template_line_desc;
   evl_trans_desc        m_template_trans_desc;
   evl_req_desc          m_template_req_desc;
   evl_bus_req_id_q      m_bus_req_id_queues[evl_bus_req_source_t];
   evl_bus_req_id_t      m_bus_ack_only_req_id[evl_bus_req_source_t];
   evl_req_desc_queue    m_bus_req_issue_pend_list[evl_sc_bank_id_t];
   evl_sequencer_mem     m_mem_sequencer;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_req_id_list
   //
   virtual function void build_req_id_list();
      evl_bus_req_source_t source_id = evl_bus_req_source_t'(0);

      do begin
         for (int ii = 0; ii < int'(m_bus_req_id_limit); ii++) begin
            m_bus_req_id_queues[source_id].push_back(ii);
         end
         source_id += evl_bus_req_source_t'(1);
      end while (source_id != evl_bus_req_source_t'(0));
   endfunction : build_req_id_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_ack_only_req_id
   //
   virtual function void find_ack_only_req_id();
      evl_bus_req_source_t source_id = evl_bus_req_source_t'(0);

      if (m_allow_duplicate_id == 1'b0) begin
         return;
      end

      do begin
         if (m_bus_req_id_queues[source_id].size() < 16) begin
            m_allow_duplicate_id = 1'b0;
            return;
         end
         source_id += evl_bus_req_source_t'(1);
      end while (source_id != evl_bus_req_source_t'(0));

      do begin
         m_bus_ack_only_req_id[source_id] = m_bus_req_id_queues[source_id].pop_front();
         source_id += evl_bus_req_source_t'(1);
      end while (source_id != evl_bus_req_source_t'(0));
   endfunction : find_ack_only_req_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_info
   //
   virtual function string format_req_info(input evl_trans_desc trans_desc);
      evl_cmd_t cmd;
      string    message;

      cmd     = trans_desc.get_cmd();
      message = "";
      if ((cmd inside { `EVL_COP_CMDS }) && (!(cmd inside { CACHE_FLUSH_TO_MEM, CACHE_EVICT_TO_MEM }))) begin
         if (cmd inside { CACHE_EVICT, CACHE_FLUSH }) begin
            case (trans_desc.get_bus_cop_start_level())
               1:       message = " l2";
               2:       message = " l3";
               default: message = " l1";
            endcase
            case (trans_desc.get_bus_cop_dest_level())
               1:       message = $sformatf("%s l2", message);
               2:       message = $sformatf("%s l3", message);
               default: message = $sformatf("%s mem", message);
            endcase
         end
         else if (cmd inside { CACHE_PREFETCH }) begin
            case (trans_desc.get_bus_cop_dest_level())
               1:       message = " l2";
               2:       message = " l3";
               default: message = " mem";
            endcase
         end
         else if (cmd inside { CACHE_SCP_FILL }) begin
            message = $sformatf(" 0x%x", trans_desc.get_bus_scp_fill_addr());
         end
         else if (cmd inside { CACHE_UNLOCK }) begin
            message = $sformatf(" %0d", trans_desc.get_bus_cop_final_state());
         end
         return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
      end
      else begin
         int req_size;

         req_size = trans_desc.get_cmd_length();
         if (cmd == MESSAGE) begin
            evl_line_data wr_data;

            wr_data = trans_desc.get_write_data();
            message = $sformatf(" %0d (%0d byte%0s)", int'(trans_desc.get_bus_message_conf()), req_size, ((req_size == 1) ? "" : "s"));
            if (wr_data != null) begin
               int                 dword_length;
               int                 beg_dword;
               int                 end_dword;
               evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

               dword_length = int'((req_size + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES);
               if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
                  byte_ens = (evl_dword_byte_en_t'(1) << req_size) - evl_dword_byte_en_t'(1);
               end
               beg_dword = 0;
               end_dword = dword_length - 1 + beg_dword;
               message   = $sformatf("%0s <- 0x%s", message, wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
               for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
                  message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
               end
            end
            return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
         end
         else if (cmd == ATOMIC) begin
            evl_line_data wr_data;

            wr_data = trans_desc.get_write_data();
            message = trans_desc.sprint_cmd();
            if (wr_data != null) begin
               int                 dword_length;
               int                 beg_dword;
               int                 end_dword;
               evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

               dword_length = int'((trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES);
               if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
                  byte_ens = (evl_dword_byte_en_t'(1) << `EVL_WORD_BYTES) - evl_dword_byte_en_t'(1);
               end
               beg_dword = 0;
               if (int'(trans_desc.get_bus_atomic_conf() & (`EVL_BUS_ATOMIC_CONF_VEC_OP | `EVL_BUS_ATOMIC_OP_MASK)) == int'(SC_AMOCOMPEXCH)) begin
                  end_dword = 1;
               end
               else begin
                  end_dword = dword_length - 1 + beg_dword;
               end
               message = $sformatf("%0s <- 0x%s", message, wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
               for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
                  message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
               end
            end
            return message;
         end
         else if (cmd inside { LD_COOP }) begin
            return $sformatf("%0p 0x%x minion coop mask:0x%0x, coop_id:0x%0x", cmd, trans_desc.get_paddr(), trans_desc.get_bus_minion_coop_mask(), trans_desc.get_bus_coop_id());
         end
         else if (cmd inside { ST }) begin
            evl_line_data wr_data;

            wr_data = trans_desc.get_normalized_write_data();
            message = $sformatf(" (%0d byte%0s)", req_size, ((req_size == 1) ? "" : "s"));
            if (wr_data != null) begin
               int                 dword_length;
               int                 beg_dword;
               int                 end_dword;
               evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

               dword_length = int'((trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES);
               if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
                  byte_ens = (evl_dword_byte_en_t'(1) << trans_desc.get_cmd_length()) - evl_dword_byte_en_t'(1);
                  if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                     byte_ens = byte_ens << `EVL_WORD_BYTES;
                  end
               end
               beg_dword = 0;
               end_dword = dword_length - 1 + beg_dword;
               message   = $sformatf(" <- 0x%s", wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
               for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
                  message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
               end
            end
            return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
         end
         else if (cmd inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
            evl_line_data wr_data;

            wr_data = trans_desc.get_write_data();
            message = $sformatf(" (%0d byte%0s)", req_size, ((req_size == 1) ? "" : "s"));
            if (wr_data != null) begin
               int                 dword_length;
               int                 beg_dword;
               int                 end_dword;
               evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

               dword_length = int'((trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES);
               if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
                  byte_ens = (evl_dword_byte_en_t'(1) << trans_desc.get_cmd_length()) - evl_dword_byte_en_t'(1);
                  if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                     byte_ens = byte_ens << `EVL_WORD_BYTES;
                  end
               end
               beg_dword = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
               end_dword = dword_length - 1 + beg_dword;
               message   = $sformatf(" <- 0x%s", wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
               for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
                  message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
               end
            end
            return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
         end
         else begin
            message = $sformatf(" (%0d byte%0s)", req_size, ((req_size == 1) ? "" : "s"));
            return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
         end
      end
   endfunction : format_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req
   //
   virtual function void print_req(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s (%0d)", format_req_info(trans_desc), trans_desc.get_cmd_length()))
      if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_REPLAY) != 1'b0) begin
         print_replay_info(trans_desc);
      end
   endfunction : print_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req_done
   //
   virtual function void print_req_done(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s complete", format_req_info(trans_desc)))
   endfunction : print_req_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_replay_info
   //
   virtual function void print_replay_info(input evl_trans_desc trans_desc);
      string message;

      message = trans_desc.sprint_delays();
      if (trans_desc.get_cmd() inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         if (message == "") begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p set_qword_en=0x%x", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), trans_desc.get_write_qword_en()))
         end
         else begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p set_qword_en=0x%x %0s", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), trans_desc.get_write_qword_en(), message))
         end
      end
      else begin
         if (message == "") begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source()))
         end
         else begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p %0s", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), message))
         end
      end
   endfunction : print_replay_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem(input string file_name);
      //
      // If this is not a memory agent, do not load the memory from a file.
      //
      if (m_memory_agent == 1'b0) begin
         return;
      end
      if (file_name != "") begin
         m_cache.load_mem(file_name);
      end
   endfunction : perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_random_data_use
   //
   virtual function void set_random_data_use();
       m_cache.set_random_data_use();
   endfunction : set_random_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_snd_req_port") || (port_name == "sa_snd_req") || (port_name == "snd_req")) begin
         if (port_number < m_snd_req_ports.size()) begin
            return m_snd_req_ports[port_number];
         end
      end
      else if ((port_name == "m_snd_rsp_port") || (port_name == "sa_snd_rsp") || (port_name == "snd_rsp")) begin
         if (port_number < m_snd_rsp_ports.size()) begin
            return m_snd_rsp_ports[port_number];
         end
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_obs_req_export") || (port_name == "obs_req")) begin
         if (port_number < m_obs_req_exports.size()) begin
            return m_obs_req_exports[port_number];
         end
      end
      else if ((port_name == "m_rcv_rsp_export") || (port_name == "rcv_rsp")) begin
         if (port_number < m_rcv_rsp_exports.size()) begin
            return m_rcv_rsp_exports[port_number];
         end
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its bus agent.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      if ((port_number < m_snd_req_ports.size()) && (m_snd_req_ports[port_number] != null)) begin
         bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_req_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_req_ports[port_number].connect(bus_export);
      end
      if ((port_number < m_snd_rsp_ports.size()) && (m_snd_rsp_ports[port_number] != null)) begin
         bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_rsp_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_rsp_ports[port_number].connect(bus_export);
      end
      if ((port_number < m_obs_req_exports.size()) && (m_obs_req_exports[port_number] != null)) begin
         bus_port = bus_verif_comp.get_req_desc_port("m_obs_req_port", port_number);
         if (bus_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_obs_req_exports[port_number].get_full_name()))
            bus_port.connect(m_obs_req_exports[port_number]);
         end
      end
      if ((port_number < m_rcv_rsp_exports.size()) && (m_rcv_rsp_exports[port_number] != null)) begin
         bus_port = bus_verif_comp.get_req_desc_port("m_rcv_rsp_port", port_number);
         if (bus_port != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_rcv_rsp_exports[port_number].get_full_name()))
            bus_port.connect(m_rcv_rsp_exports[port_number]);
         end
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   // This virtual function will be used by classes extended from this one to create
   // tables to describe the specific actions of a master or slave agent.
   //
   virtual function void build_tables();
      return;
   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop
   //
   virtual task fetch_and_process_loop();
      wait_for_stimulus_enable();
      if (m_mem_sequencer != null) begin
         build_req_id_list();
         find_ack_only_req_id();
         forever begin
            wait_cycles(1);
            while (stimulus_is_enabled() == 1'b1) begin
               fetch_and_process_trans_desc();
               wait_cycles(1);
            end
            wait_for_stimulus_enable();
         end
      end
   endtask : fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fetch_and_process_trans_desc
   //
   virtual task fetch_and_process_trans_desc();
      evl_paddr_t      paddr;
      evl_trans_desc   trans_desc;
      evl_sequence_mem sequence_mem;
      evl_req_desc     req_desc;
      evl_table_desc   table_desc;
      evl_line_desc    line_desc;
      int              delay_value;

      req_desc = null;
      m_mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      if (trans_desc == null) begin
         return;
      end
      add_active_mem_desc(trans_desc);
      `evl_log_addr(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("fetch_and_process() got this cmd: %0s (%0d unissued)", trans_desc.sprint_obj(), non_issued_mem_desc_count()))
      if (trans_desc.get_cmd() == BARRIER) begin
         int barrier_limit;

         m_stimulus_busy = 1'b1;
         print_req(trans_desc);
         barrier_limit = int'(trans_desc.get_paddr());
         if (barrier_limit >= 0) begin
            barrier_limit++;
            wait (m_active_desc_count <= barrier_limit);
         end
         print_req_done(trans_desc);
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         m_stimulus_busy = 1'b0;
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         return;
      end
      else if ((stimulus_is_enabled() == 1'b0) && (trans_desc.can_drop_trans() == 1'b1)) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         m_stimulus_busy = 1'b0;
         return;
      end

      //
      // Wait for the request delay time before moving on.
      //
      m_stimulus_busy = 1'b1;
      if (get_next_delay(delay_value, pk_req_delay, trans_desc) == 1'b1) begin
         if (delay_value > 1) begin
            wait_cycles_with_release(delay_value - 1);
         end
      end

      if (trans_desc.get_cmd() == NOP) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         m_stimulus_busy = 1'b0;
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         return;
      end

      preprocess_trans_desc(trans_desc);

      if (trans_desc.get_cmd() == MESSAGE) begin
         evl_bus_message_conf_t message_conf;

         paddr = trans_desc.get_paddr();
         paddr[`EVL_BUS_MESSAGE_CONF_MSB:`EVL_BUS_MESSAGE_CONF_LSB] = trans_desc.get_bus_message_conf();
         if (m_fixup_message == 1'b1) begin
            //
            // Make sure the message is something that the UC block can consume.
            //
            paddr = (paddr & `EVL_MSG_ADDR_AND_MASK) | `EVL_MSG_ADDR_OR_MASK;
            if (m_neigh_message == 1'b1) begin
               if ((paddr & `EVL_MSG_REDUCE_CHK_MASK) == `EVL_MSG_REDUCE_CHK_MASK) begin
                  paddr = (paddr & `EVL_MSG_REDUCE_AND_MASK) | `EVL_MSG_REDUCE_OR_MASK;
               end
               else begin
                  paddr = (paddr & `EVL_MSG_HART_AND_MASK) | `EVL_MSG_HART_OR_MASK;
               end
            end
            trans_desc.set_bus_message_conf(evl_bus_message_conf_t'(paddr[`EVL_BUS_MESSAGE_CONF_MSB:`EVL_BUS_MESSAGE_CONF_LSB]));
         end
         if (m_allow_remote_msg == 1'b0) begin
            paddr = (paddr & (~`EVL_MSG_SHIRE_ID_AND_MASK)) | ((evl_paddr_t'(m_shire_id) << `EVL_MSG_SHIRE_ID_LSB) & `EVL_MSG_SHIRE_ID_AND_MASK);
         end
         trans_desc.set_paddr(paddr);
      end

      forever begin
         bit resource_conflict = 1'b0;

         //
         // Now perform a lookup.  This line of code does a lot.  It takes in a transaction
         // descriptor from the sequencer side of the world, looks it up in a table and based on the
         // output creates a request descriptor to describe a bus request.  It also gives back
         // table_desc which includes what needs to be done with the cache model for this
         // transaction.
         //
         paddr = trans_desc.get_paddr();
         if (int'(paddr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
            req_desc = m_cache.int_req_lookup(table_desc, line_desc, trans_desc, req_desc, 1);
         end
         else begin
            req_desc = m_cache.int_req_lookup(table_desc, line_desc, trans_desc, req_desc, 0);
         end

         if (table_desc.pending_conflict() == 1'b1) begin
            resource_conflict = 1'b1;
         end
         else if (table_desc.pending_bus_cmd() == 1'b1) begin
            if (m_active_desc_count > m_active_limit) begin
               resource_conflict = 1'b1;
            end
            else if (non_issued_mem_desc_count() > m_issue_limit) begin
               resource_conflict = 1'b1;
            end
            else if ((req_desc != null) && (req_desc.get_bus_cmd() != `EVL_Bus_Idle)) begin
               evl_bus_req_source_t source_id;

               source_id = req_desc.get_bus_req_source();
               if ((m_bus_req_id_queues[source_id].size() == 0) && ((m_allow_duplicate_id == 1'b0) || (req_desc.bus_cmd_has_unique_tag() == 1'b1))) begin
                  resource_conflict = 1'b1;
               end
            end
         end

         //
         // Do we need to drop the request?
         //
         if (((stimulus_is_enabled() == 1'b0) || (resource_conflict == 1'b1)) && (trans_desc.can_drop_trans() == 1'b1)) begin
            trans_desc.set_done();
            delete_active_mem_desc(trans_desc);
            m_stimulus_busy = 1'b0;
            return;
         end

         if (resource_conflict == 1'b0) begin
            break;
         end
         wait_cycles(1);
      end // forever begin

      print_req(trans_desc);
      if (m_count_stimulus == 1'b1) begin
         m_trans_count++;
      end

      if ((req_desc != null) && (req_desc.get_bus_cmd() != `EVL_Bus_Idle)) begin
         evl_bus_req_id_t     req_id;
         evl_bus_req_source_t source_id;

         source_id = req_desc.get_bus_req_source();
         if ((m_allow_duplicate_id == 1'b1) && (req_desc.bus_cmd_has_unique_tag() == 1'b0)) begin
            req_id = m_bus_ack_only_req_id[source_id];
         end
         else begin
            req_id = m_bus_req_id_queues[source_id].pop_front();
         end
         req_desc.set_bus_req_id(req_id);
         if (trans_desc.get_cmd() inside { MESSAGE }) begin
            m_bus_req_id_queues[source_id].push_back(req_id);
         end

         // Copy write data from trans_desc to req_desc when necessary
         if (table_desc.get_data_action() == Data_ReqWr) begin
            evl_line_data line_data;

            if (trans_desc.get_write_data() == null) begin
               `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: data_action is Data_ReqWr but trans_desc has null write data"))
            end
            line_data = new();
            line_data.copy_data_from_line(trans_desc.get_write_data());
            req_desc.set_req_data(line_data);
         end

         //
         // Add in appropriate bus delays.
         //
         if (get_next_delay(delay_value, pk_bus_req_delay, trans_desc) == 1'b1) begin
            req_desc.add_delay(pk_bus_req_delay, delay_value);
         end

         // Give the specific master agent a chance to modify the request before it's driven.
         final_fixup(trans_desc, req_desc);

         if (req_desc.bus_cmd_requires_rsp() == 1'b0) begin
            evl_sc_bank_id_t bus_bank;

            bus_bank = req_desc.get_bus_bank();
            void'(m_bus_req_issue_pend_list[bus_bank].push_desc(req_desc));
         end

         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), req_desc, $sformatf("sending this request descriptor"))
         if (m_count_stimulus == 1'b1) begin
            m_req_count++;
         end
         m_snd_req_ports[select_req_port(req_desc)].write(req_desc);
      end
      m_stimulus_busy = 1'b0;
   endtask : fetch_and_process_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   virtual function int select_req_port(input evl_req_desc req_desc);
      return 0;
   endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
      return req_desc.get_int_param(pk_tlm_req_port, 0);
   endfunction : select_rsp_port


   //----------------------------------------------------------------------------------------------
   //
   // Function: preprocess_trans_desc
   //
   virtual function void preprocess_trans_desc(input evl_trans_desc trans_desc);
      if (trans_desc.get_cmd() == ATOMIC) begin
         evl_bus_atomic_conf_t atomic_conf;
         evl_line_data         req_data;

         req_data = trans_desc.get_write_data();
         if (trans_desc.is_cached() == 1'b1) begin
            atomic_conf = ~`EVL_BUS_ATOMIC_CONF_L3_OP & trans_desc.get_bus_atomic_conf();
         end
         else begin
            atomic_conf = `EVL_BUS_ATOMIC_CONF_L3_OP | trans_desc.get_bus_atomic_conf();
         end
         if ((int'(atomic_conf) & `EVL_BUS_ATOMIC_OP_MASK) == int'(SC_AMOCOMPEXCH)) begin
            atomic_conf = ~`EVL_BUS_ATOMIC_CONF_VEC_OP & atomic_conf;
            if ((atomic_conf & `EVL_BUS_ATOMIC_CONF_DW_OP) != 0) begin
               trans_desc.set_cmd_length(4);
            end
            else begin
               trans_desc.set_cmd_length(8);
            end
            req_data.invalidate_qword(1);
         end
         req_data.invalidate_qword(2);
         req_data.invalidate_qword(3);
         trans_desc.set_bus_atomic_conf(atomic_conf);
         trans_desc.set_bus_subopcode_dest(evl_bus_subopcode_t'(atomic_conf));
      end // if (trans_desc.get_cmd() == ATOMIC)
   endfunction : preprocess_trans_desc


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   // A specific agent may want to use this function to modify the request before it is driven.
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      req_desc.set_bus_req_qword_en(trans_desc.get_write_qword_en());
   endfunction : final_fixup


   //----------------------------------------------------------------------------------------------
   //
   // Function: reuse_rsp_id
   //
   // This default version of the function puts the ID back on the appropriate queue. A specific
   // agent might want their own version to treat certain commands differently.
   //
   virtual function void reuse_rsp_id(input evl_trans_desc trans_desc, input evl_req_desc rcv_rsp_desc);
      evl_bus_req_id_t     bus_rsp_id;
      evl_bus_req_source_t source_id;

      bus_rsp_id = rcv_rsp_desc.get_bus_req_id();
      source_id  = rcv_rsp_desc.get_bus_req_source();
      m_bus_req_id_queues[source_id].push_back(bus_rsp_id);
   endfunction : reuse_rsp_id


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_issued_requests_task()
   //
   task watch_for_issued_requests_task(input evl_sc_bank_id_t bus_bank);
      evl_trans_desc trans_desc;
      evl_req_desc   req_desc;

      forever begin
         m_bus_req_issue_pend_list[bus_bank].wait_for_desc();
         req_desc = m_bus_req_issue_pend_list[bus_bank].pop();
         if (req_desc != null) begin
            req_desc.wait_for_issue();
            trans_desc = req_desc.get_root_trans_desc();
            if (trans_desc != null) begin
               trans_desc.set_done();
               //
               // Messages never receive a response ... mark the transaction as complete once it has been issued.
               //
               if (trans_desc.get_cmd() == MESSAGE) begin
                  delete_active_mem_desc(trans_desc);
               end
            end
         end
      end
   endtask : watch_for_issued_requests_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_issued_requests()
   //
   function void watch_for_issued_requests(input evl_sc_bank_id_t bus_bank);
      m_bus_req_issue_pend_list[bus_bank] = new();
      m_bus_req_issue_pend_list[bus_bank].set_abstract_name($sformatf("%s(IssueQ-%0d)", get_root_abstract_name(), bus_bank));
      fork
         watch_for_issued_requests_task(bus_bank);
      join_none
   endfunction : watch_for_issued_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_responses_task()
   //
   task watch_for_received_responses_task(input uvm_tlm_analysis_fifo #(evl_req_desc) rcv_rsp_fifo);
      evl_req_desc rcv_rsp_desc;

      forever begin
         rcv_rsp_fifo.get(rcv_rsp_desc);
         process_received_response(rcv_rsp_desc);
      end
   endtask : watch_for_received_responses_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_received_responses()
   //
   function void watch_for_received_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) rcv_rsp_fifo);
      if (rcv_rsp_fifo != null) begin
         fork
            watch_for_received_responses_task(rcv_rsp_fifo);
         join_none
      end
   endfunction : watch_for_received_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_observed_requests_task()
   //
   task watch_for_received_requests_task(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo);
      evl_req_desc rcv_req_desc;

      forever begin
         obs_req_fifo.get(rcv_req_desc);
         rcv_req_desc.set_observed();
         pre_process_received_request(rcv_req_desc);
         process_received_request(rcv_req_desc);
      end
   endtask : watch_for_received_requests_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_observed_requests()
   //
   function void watch_for_received_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo);
      if (obs_req_fifo != null) begin
         fork
            watch_for_received_requests_task(obs_req_fifo);
         join_none
      end
   endfunction : watch_for_received_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_received_response()
   //
   // This function will get called for Master Agents when a response is observed on the bus.  The
   // evl_req_desc sent is actually the same one that was originally sent on sa_snd_req_port.
   //
   function void process_received_response(input evl_req_desc rcv_rsp_desc);
      evl_table_desc       table_desc;
      evl_line_desc        line_desc;
      evl_trans_desc       trans_desc;
      evl_bus_req_id_t     bus_rsp_id;
      evl_bus_req_source_t source_id;

      trans_desc = rcv_rsp_desc.get_root_trans_desc();

      m_cache.bus_rsp_lookup(table_desc, line_desc, rcv_rsp_desc, 0);
      if (table_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), rcv_rsp_desc, $sformatf("process_received_response got a null table_desc, sad!"))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), rcv_rsp_desc, $sformatf("process_received_response got a table_desc back from the table with data action %p", table_desc.get_data_action()))
      end

      //Temp:
      if (table_desc.get_data_action() == Data_ReqRd) begin
         //Copy the read data from the req_desc to the trans_desc
         if (trans_desc != null) begin
            evl_line_data line_data;

            line_data = new();
            line_data.copy_data_from_line(rcv_rsp_desc.get_rsp_data());
            trans_desc.set_read_data(line_data);
         end
      end
      else if (table_desc.get_data_action() == Data_ReqRdNorm) begin
         //Copy the read data from the req_desc to the trans_desc
         if (trans_desc != null) begin
            evl_paddr_t   addr;
            evl_line_data line_data;

            addr      = trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1);
            line_data = new();
            line_data.invalidate_line();
            if ((int'(trans_desc.get_bus_atomic_conf()) & `EVL_BUS_ATOMIC_CONF_VEC_OP) != 0) begin
               addr = (addr & (~(`EVL_OWORD_BYTES - 1))) >> `EVL_OWORD_ADDR_LSB;
               line_data.set_oword(addr, rcv_rsp_desc.get_rsp_oword(addr));
            end
            else if ((int'(trans_desc.get_bus_atomic_conf()) & `EVL_BUS_ATOMIC_CONF_DW_OP) != 0) begin
               addr = (addr & (~(`EVL_DWORD_BYTES - 1))) >> `EVL_DWORD_ADDR_LSB;
               line_data.set_dword(addr, rcv_rsp_desc.get_rsp_dword(addr));
            end
            else begin
               addr = (addr & (~(`EVL_WORD_BYTES - 1))) >> `EVL_WORD_ADDR_LSB;
               line_data.set_word(addr, rcv_rsp_desc.get_rsp_word(addr));
            end
            trans_desc.set_read_data(line_data);
         end
      end

      // This function pushes IDs back on a queue for reuse. It's a virtual function
      // so that it can be overwritten for specific agents (ex: for special treatment for ReadCoop)
      reuse_rsp_id(trans_desc, rcv_rsp_desc);

      //mark the transaction as done.
      if (trans_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), rcv_rsp_desc, $sformatf("process_received_response not expecting trans_desc to be null"))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), rcv_rsp_desc, $sformatf("process_received_response marking this transaction done"))
         rcv_rsp_desc.set_done();
         trans_desc.set_done();
         if (m_print_trans_hierarchy == 1'b1) begin
            trans_desc.print_from_root(UVM_MEDIUM, VMOD_TRANS_DESC);
         end
         delete_active_mem_desc(trans_desc);
      end
   endfunction : process_received_response


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_process_received_request()
   //
   // This task will get called for Slave Agents when a request is observed on the bus.
   // It is meant to be used by derived agents for their specific needs
   //
   virtual task pre_process_received_request(input evl_req_desc rcv_req_desc);
      return;
   endtask : pre_process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_appropriate_amount_of_rsp_data
   //
   function evl_line_data copy_appropriate_amount_of_rsp_data(input evl_line_data rsp_data_in, input evl_bus_req_size_t req_size_in, input evl_paddr_t addr_in);
      evl_xword_t xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_line_data rsp_data_out;
      evl_paddr_t addr_mask;
      int shift_value;

      addr_mask = ((1<<(`EVL_LINE_ADDR_LSB))-1) & ~((1<<req_size_in)-1);
      shift_value = addr_mask & addr_in;

      xword_data = rsp_data_in.get_xword(0);
      case (req_size_in)
         'd0 : xword_byte_en = 64'h1;
         'd1 : xword_byte_en = 64'h3;
         'd2 : xword_byte_en = 64'hF;
         'd3 : xword_byte_en = 64'hFF;
         'd4 : xword_byte_en = 64'hFFFF;
         'd5 : xword_byte_en = 64'hFFFF_FFFF;
         'd6 : xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
      endcase // case
      xword_byte_en = xword_byte_en << shift_value;

      rsp_data_out = new();
      rsp_data_out.set_xword(0, xword_data, xword_byte_en);
      return rsp_data_out;
   endfunction : copy_appropriate_amount_of_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_process_received_request()
   //
   // This function is called inside process_received_request to post-process the request.
   //
   virtual function void post_process_received_request(inout evl_req_desc req_desc);
      return;
   endfunction : post_process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_received_request()
   //
   // This task will get called for Slave Agents when a request is observed on the bus.
   //
   virtual task process_received_request(input evl_req_desc req_desc);
      evl_trans_desc trans_desc;
      evl_table_desc table_desc;
      evl_line_desc  line_desc;
      evl_line_data  line_data;
      int            delay_value;

      trans_desc = req_desc.get_root_trans_desc();

      // FUTURE: eventually un-comment this error, once the desc linking is set up
      if (trans_desc == null) begin
      // `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("get_root_trans_desc got a null trans_desc, sad!"))
      end

      if (get_next_delay(delay_value, pk_mem_req_delay, trans_desc) == 1'b1) begin
         if (delay_value > 1) begin
            wait_cycles_with_release(delay_value - 1);
         end
      end

      m_cache.bus_req_lookup(table_desc, line_desc, req_desc, 1);
      if (table_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_request got a null table_desc, sad!"))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), req_desc, $sformatf("process_received_request got a table_desc back from the table with cmd=%0p and data action=%p", table_desc.get_bus_cmd(), table_desc.get_data_action()))
      end

      if (table_desc.get_data_action() == Data_MemRd) begin
         // line_data = line_desc.get_line_data_copy();
         line_data = copy_appropriate_amount_of_rsp_data(line_desc.get_line_data(), req_desc.get_bus_req_size(), req_desc.get_paddr());
         req_desc.set_rsp_data(line_data);
      end
      else if (table_desc.get_data_action() == Data_MemWr) begin
         line_data = req_desc.get_req_data();
         line_desc.merge_data_from_line(line_data);
      end
      else if (table_desc.get_data_action() == Data_MemWrRdNorm) begin
         req_desc.set_rsp_data(line_desc.process_atomic(req_desc));
      end
      else if (table_desc.get_data_action() == Data_MemWrRd) begin
         req_desc.set_rsp_data(line_desc.process_atomic(req_desc, 1'b0));
      end

      if (table_desc.get_data_action() == Data_ForwardMsg) begin
         evl_line_data rand_data;
         evl_line_data src_req_line_data;

         // Retrieve the msg config through the get_bus_message_conf instead
         // According to the shire interconnect bits 10:3 of the address go to the transid
         evl_paddr_t paddr = req_desc.get_paddr();

         // Extracting message id and copying it in the reply id according
         // to the Shire Interconnect spec (4.1)
         req_desc.set_bus_rsp_id(paddr[`EVL_BUS_MESSAGE_CONF_MSB:`EVL_BUS_MESSAGE_CONF_LSB]);

         // Set response fields
         req_desc.set_bus_message_conf(paddr[`EVL_BUS_MESSAGE_CONF_MSB:`EVL_BUS_MESSAGE_CONF_LSB]);
         req_desc.set_bus_rsp_type(table_desc.get_bus_cmd());
         req_desc.set_bus_rsp_size(req_desc.get_bus_req_size());
         //req_desc.set_rsp_data(req_desc.get_req_data_copy());

         // Randomizing the rest of the cache line however keeping the valid dwords
         // according to the request size.
         // This will enable us to flag an error should something goes wrong with the write masks.
         // The problem with the original approach was that 0s were generated on the MsgSendData
         // which may cause an error with the QWEN to go undetected
         src_req_line_data = req_desc.get_req_data();
         rand_data = new();
         rand_data.create_data(m_rand_gen);
         rand_data.invalidate_line();

         for (int i=0; i<=(2**req_desc.get_bus_req_size())/8; i++) begin
            rand_data.set_dword(i, src_req_line_data.get_dword(i));
         end

         req_desc.set_rsp_data(rand_data);

         // In case the reply goes to a neighborhood, then we encode the thread and minion id in the dest
         // field of the reply according to the shire interconnecte spec (4.2)
         req_desc.set_bus_rsp_dest(paddr[`EVL_BUS_MSG_DST_MINION_MSB:`EVL_BUS_MSG_DST_MINION_LSB]);
         req_desc.set_bus_rsp_valid(0, get_cycle_count());

         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), req_desc, $sformatf("process_received_request %s", req_desc.sprint_obj()))
      end
      else begin
         if (table_desc.get_data_action() inside {Data_MemWrRdNorm, Data_MemWrRd, Data_MemRd}) begin
            req_desc.set_bus_rsp_size(req_desc.get_bus_req_size());
         end
         else begin
            req_desc.set_bus_rsp_size(3'b110);
         end
         req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
         req_desc.set_bus_rsp_type(table_desc.get_bus_cmd()); // FUTURE: this seems wrong ... opcode
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end


      //
      // Add in appropriate bus delays to send out.
      //
      if (get_next_delay(delay_value, pk_mem_bus_rsp_delay, trans_desc) == 1'b1) begin
         req_desc.add_delay(pk_mem_bus_rsp_delay, delay_value);
      end

      post_process_received_request(req_desc);

      `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_STIM, get_abstract_name(), req_desc, $sformatf("sending a response for %0s", req_desc.sprint_obj()))
      m_snd_rsp_ports[select_rsp_port(req_desc)].write(req_desc);
   endtask : process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_int_req_act
   //
   // This function is used to define a table of actions to take when a transaction descriptor
   // is received from the stimulus world. The first 4 inputs to this function are table inputs
   // and the last 3 are outputs of the table.
   // Lists are used as a way to collapse the table to save typing (and sanity) but you can
   // think of the inputs in terms of a single entry:
   // cmd: The command from the stimulus (ex: LD, ST). These are generic and bus agnostic.
   // cache_attr: Is it cached or uncached?
   // model_st: What is the current state of this agent's cache model? (Invalid, Exclusive, Modified)
   // qualifier: This is an optional extra input that you can use for whatever you want.
   // tag_action: How should we modify the cache state of our model based on the transaction?
   // data_action: What should we do with the data based on the transaction?
   // bus_cmd: What (if any) command should we drive on the bus?
   //
   function void add_int_req_act(input evl_cmd_t         cmd_list_in[],
                                 input bit               cache_attr_list_in[],
                                 input evl_cache_st_t    model_st_list_in[],
                                 input byte              qualifier_list_in[],
                                 input evl_cache_st_t    tag_action_in,
                                 input evl_data_action_t data_action_in,
                                 input evl_bus_cmd_t     bus_cmd_in);
      m_cache.add_int_req_act(cmd_list_in, cache_attr_list_in, model_st_list_in, qualifier_list_in, tag_action_in, data_action_in, bus_cmd_in);
   endfunction : add_int_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act
   //
   // This function is used to define a table of actions to take when a request is observed
   // on the bus. The first 3 inputs will be inputs to the table and the last 3 inputs
   // will be the outputs of the table.
   // Lists are used as a way to collapse the table to save typing but you can think
   // of the inputs in terms of a single entry:
   // bus_cmd: The command observed on the bus
   // model_st: The current state of the agent's cache model
   // qualifier: This is an optional extra input
   // tag_action: How should we modifiy the cache state of our model based on the request?
   // data_action: What should we do with the data based on the request?
   // bus_cmd: What (if any) command we should drive on the bus?
   //
   function void add_bus_req_act(input evl_bus_cmd_t     bus_cmd_list_in[],
                                 input evl_cache_st_t    model_st_list_in[],
                                 input byte              qualifier_list_in[],
                                 input evl_cache_st_t    tag_action_in,
                                 input evl_data_action_t data_action_in,
                                 input evl_bus_cmd_t     bus_cmd_in);
      m_cache.add_bus_req_act(bus_cmd_list_in, model_st_list_in, qualifier_list_in, tag_action_in, data_action_in, bus_cmd_in);
   endfunction : add_bus_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   // This function is used to define a table of actions to take when a response is observed
   // on the bus. The first 3 inputs will be inputs to the table and the last 2 inputs will
   // be outputs of the table.
   // Lists are used as a way to collapse the table to save typing but you can think of the
   // inputs in terms of a single entry:
   // bus_cmd: The (response) command observed on the bus
   // model_st: The current state of the agent's cache model
   // qualifier: this is an option extra input
   // tag_action: How should we modify the cache state of our model based on the response?
   // data_action: What should we do with the data based on the response?
   //
   function void add_bus_rsp_act(input evl_bus_cmd_t     bus_cmd_list_in[],
                                 input evl_cache_st_t    model_st_list_in[],
                                 input byte              qualifier_list_in[],
                                 input evl_cache_st_t    tag_action_in,
                                 input evl_data_action_t data_action_in);
      m_cache.add_bus_rsp_act(bus_cmd_list_in, model_st_list_in, qualifier_list_in, tag_action_in, data_action_in);
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      evl_sequencer_mem mem_sequencer;

      super.build_phase(phase);

      mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
      set_sequencer_wrapper(mem_sequencer.get_wrapper());

      m_cache = evl_stim_cache_etlink::type_id::create("cache", this);
      m_cache.set_abstract_name($sformatf("%0s($)", get_root_abstract_name()));
      m_cache.set_rand_gen(m_rand_gen);

      if (m_tables_built++ == 0) begin
         build_tables();
      end
      m_tables_built = 1;
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_obs_req_exports[ii]) begin
         if (m_obs_req_exports[ii] != null) begin
            m_obs_req_exports[ii].connect(m_obs_req_fifos[ii].analysis_export);
         end
      end
      foreach (m_rcv_rsp_exports[ii]) begin
         if (m_rcv_rsp_exports[ii] != null) begin
            m_rcv_rsp_exports[ii].connect(m_rcv_rsp_fifos[ii].analysis_export);
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      evl_sc_bank_id_t bus_bank;

      super.start_of_simulation_phase(phase);

      m_template_cache_data = evl_line_data::type_id::create(get_abstract_name());
      m_template_req_data   = evl_line_data::type_id::create($sformatf("%0s(ETLINK)", get_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create(get_abstract_name());
      m_template_trans_desc = evl_trans_desc::type_id::create(get_abstract_name());
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%0s(ETLINK)", get_abstract_name()));

      m_template_trans_desc.set_shire_id(m_shire_id);
      m_template_trans_desc.set_neighborhood_id(m_neighborhood_id);
      m_template_trans_desc.set_core_id(m_core_id);

      m_template_req_desc.set_shire_id(m_shire_id);
      m_template_req_desc.set_neighborhood_id(m_neighborhood_id);
      m_template_req_desc.set_core_id(m_core_id);

      m_template_line_desc.set_rand_gen(m_rand_gen);
      m_template_trans_desc.set_rand_gen(m_rand_gen);
      m_template_req_desc.set_rand_gen(m_rand_gen);

      m_template_line_desc.set_master_agent(this);
      m_template_trans_desc.set_master_agent(this);
      m_template_req_desc.set_master_agent(this);

      m_template_line_desc.set_data_template(m_template_cache_data);
      m_template_trans_desc.set_data_template(m_template_cache_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_ETLINK);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

      m_cache.set_req_desc_template(m_template_req_desc);
      m_cache.set_line_template(m_template_line_desc);

      foreach (m_sequencer_wrappers[ii]) begin
         if ((m_sequencer_wrappers[ii] != null) && (m_sequencer_wrappers[ii].get_object() != null)) begin
            $cast(m_mem_sequencer, m_sequencer_wrappers[ii].get_object());
            m_mem_sequencer.set_trans_desc_template(m_template_trans_desc);
         end
      end

      foreach (m_obs_req_fifos[ii]) begin
         watch_for_received_requests(m_obs_req_fifos[ii]);
      end

      foreach (m_rcv_rsp_fifos[ii]) begin
         watch_for_received_responses(m_rcv_rsp_fifos[ii]);
      end

      bus_bank = evl_sc_bank_id_t'(1);
      watch_for_issued_requests(evl_sc_bank_id_t'(0));
      while (bus_bank != evl_sc_bank_id_t'(0)) begin
         watch_for_issued_requests(bus_bank);
         bus_bank++;
      end

      fork
         fetch_and_process_loop();
         watch_for_timeouts();
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      fork
         begin
            super.post_reset_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostResetPhase), 1'b0);
            phase.drop_objection(this);
         end
      join
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      fork
         begin
            super.post_configure_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostConfigurePhase), 1'b0);
            phase.drop_objection(this);
         end
      join
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      m_count_stimulus = 1'b1;
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      fork
         begin
            super.post_main_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostMainPhase), 1'b0, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      fork
         begin
            super.shutdown_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: report_phase
   //
   virtual function void report_phase(uvm_phase phase);
      super.report_phase(phase);
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of transactions performed during test: %0d", m_trans_count))
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of bus requests sent during test:      %0d", m_req_count))
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_verif_comp",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = NO_AGENT);
      int value;

      super.new(name_in, parent_in, agent_type_in);
      if (`et_value_plusargs("fixupMessage=%d", value) == 0) begin
         value = 1'b1;
      end
      m_fixup_message = (value == 0) ? 1'b0 : 1'b1;
      if (`et_value_plusargs("allowRemoteMessage=%d", value) == 0) begin
         value = 1'b1;
      end
      m_allow_remote_msg = (value == 0) ? 1'b0 : 1'b1;
      if (`et_test_plusargs("printTransHierarchy") != 0) begin
         m_print_trans_hierarchy = 1'b1;
      end
      if (`et_value_plusargs("etlink_active_limit=%d", value) != 0) begin
         if (value < 1) begin
            m_active_limit = 1;
         end
         else if (value > 256) begin
            m_active_limit = 256;
         end
         else begin
            m_active_limit = value;
         end
      end // if (`et_value_plusargs("etlink_active_limit=%d", value) != 0)
      if (`et_value_plusargs("etlink_issue_limit=%d", value) != 0) begin
         if (value < 1) begin
            m_issue_limit = 1;
         end
         else if (value > 256) begin
            m_issue_limit = 256;
         end
         else begin
            m_issue_limit = value;
         end
      end // if (`et_value_plusargs("etlink_issue_limit=%d", value) != 0)
   endfunction : new

endclass : evl_stim_agent_etlink

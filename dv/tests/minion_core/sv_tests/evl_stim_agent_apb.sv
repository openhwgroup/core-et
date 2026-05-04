//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_agent_apb
//
// This class contains all of the ports and tasks necessary to be a master or a slave.  It also
// includes a cache model that can be used as a memory model for a slave or a cache model for a
// master.
//
// This class tries to be generic and not define exactly what will happen when it receives a
// transaction.  Instead it makes a call to a function which returns // the action it should take
// (ex: I am a master and I received a LD from the sequencer ... what request should I drive on the
// bus, and what should I do to my cache model?).
//
// This class is used by first extending it, and in the extension creating only the ports that are
// needed (i.e., for a slave or a master) and making function calls to fill out a table of what
// actions to take when.  Please see evl_bpam_master_agent_apb.sv for an example of a master agent.
//
virtual class evl_stim_agent_apb extends evl_verif_comp;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Delay Keys
   //
   // pk_req_delay:       This is the delay before processing the transaction in a master stimulus
   //                     agent.
   // pk_bus_req_delay:   This is the delay before sending the bus request (this is processed at the
   //                     pin level).
   // pk_from_stim_agent: This is used to mark a request descriptor as coming from a stimulus agent.
   //
   `evl_param_key(pk_req_delay)
   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_from_stim_agent)


   //-----------------------------------------------------------------------------------------------
   //
   // Member TLM Ports
   //
   uvm_analysis_port     #(evl_req_desc) m_snd_req_port;
   uvm_analysis_port     #(evl_req_desc) m_snd_rsp_port;
   uvm_analysis_export   #(evl_req_desc) m_obs_req_export;
   uvm_analysis_export   #(evl_req_desc) m_rcv_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifo;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rcv_rsp_fifo;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int                m_tables_built        = 0;
   bit                m_count_stimulus      = 1'b0;
   bit                m_stimulus_busy       = 1'b0;
   int                m_active_limit        = 1;
   int                m_issue_limit         = 1;
   int                m_data_width          = 0;
   int                m_data_width_clog2    = 0;
   int                m_trans_count         = 0;
   int                m_req_sent_count      = 0;
   int                m_req_rcv_count       = 0;
   semaphore          m_get_trans_desc_busy = new(1);
   evl_cmd_length_t   m_max_cmd_length;
   evl_stim_cache_apb m_cache;
   evl_sequencer_mem  m_mem_sequencer;
   evl_trans_desc     m_active_bus_reqs[$];
   evl_req_desc       m_pending_reqs[$];
   evl_line_data      m_template_cache_data;
   evl_line_data      m_template_req_data;
   evl_line_desc      m_template_line_desc;
   evl_trans_desc     m_template_trans_desc;
   evl_req_desc       m_template_req_desc;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_trans_desc_early
   //
   virtual function void fix_stimulus_trans_desc_early(input evl_trans_desc trans_desc_in);
      if (trans_desc_in.get_cmd() inside { `EVL_LD_ST_CMDS }) begin
         evl_cmd_length_t cmd_length;
         evl_paddr_t      paddr;

         cmd_length = trans_desc_in.get_cmd_length();
         paddr      = trans_desc_in.get_paddr();
         if (cmd_length > m_max_cmd_length) begin
            cmd_length = m_max_cmd_length;
            trans_desc_in.set_cmd_length(cmd_length);
         end
         if (cmd_length > 32) begin
            paddr[5:0] = {6{1'b0}};
         end
         else if (cmd_length > 16) begin
            paddr[4:0] = {5{1'b0}};
         end
         else if (cmd_length > 8) begin
            paddr[3:0] = {4{1'b0}};
         end
         else if (cmd_length > 4) begin
            paddr[2:0] = {3{1'b0}};
         end
         else if (cmd_length > 2) begin
            paddr[1:0] = {2{1'b0}};
         end
         else if (cmd_length > 1) begin
            paddr[0:0] = {1{1'b0}};
         end
         trans_desc_in.set_paddr(paddr);
      end
   endfunction : fix_stimulus_trans_desc_early


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_info
   //
   virtual function string format_req_info(input evl_trans_desc trans_desc);
      string message;

      message = "";
      if (trans_desc.get_cmd() inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         evl_line_data wr_data;

         wr_data = trans_desc.get_write_data();
         if (wr_data != null) begin
            int cmd_length;
            int beg_dword;
            int end_dword;

            cmd_length = (trans_desc.get_cmd_length() + 7) / 8;
            beg_dword  = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
            end_dword  = cmd_length - 1 + beg_dword;
            message = $sformatf(" <- 0x%s", wr_data.format_invalid_and_unknown_by_index(end_dword));
            for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
               message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
            end
         end
         return $sformatf("%0p 0x%x%0s", trans_desc.get_cmd(), trans_desc.get_paddr(), message);
      end
      else begin
         return $sformatf("%0p 0x%x", trans_desc.get_cmd(), trans_desc.get_paddr());
      end
   endfunction : format_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req
   //
   virtual function void print_req(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s (%0d)", format_req_info(trans_desc), trans_desc.get_cmd_length()))
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
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_snd_req_port") || (port_name == "sa_snd_req") || (port_name == "snd_req")) begin
         return m_snd_req_port;
      end
      else if ((port_name == "m_snd_rsp_port") || (port_name == "sa_snd_rsp") || (port_name == "snd_rsp")) begin
         return m_snd_rsp_port;
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
         return m_obs_req_export;
      end
      else if ((port_name == "m_rcv_rsp_export") || (port_name == "rcv_rsp")) begin
         return m_rcv_rsp_export;
      end
      return null;
   endfunction : get_req_desc_export


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
      evl_trans_desc   trans_desc;
      evl_sequence_mem sequence_mem;
      evl_req_desc     req_desc;
      evl_table_desc   table_desc;
      evl_line_desc    line_desc;
      evl_cmd_t        cmd;
      int              delay_value;

      req_desc = null;
      m_mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      if (trans_desc == null) begin
         wait_cycles(1);
         return;
      end
      cmd = trans_desc.get_cmd();
      `evl_log_addr(UVM_HIGH, VMOD_APB_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("fetch_and_process got this cmd: %0p (from %0s, id %0d)", cmd, m_mem_sequencer.get_abstract_name(), m_mem_sequencer.get_inst_id()))
      add_active_mem_desc(trans_desc);
      if (cmd == BARRIER) begin
         int barrier_limit;

         m_stimulus_busy++;
         print_req(trans_desc);
         barrier_limit = int'(trans_desc.get_paddr());
         if (barrier_limit >= 0) begin
            barrier_limit++;
            wait (m_active_desc_count <= barrier_limit);
         end
         print_req_done(trans_desc);
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         m_stimulus_busy--;
         wait_cycles(1);
         return;
      end
      else if ((stimulus_is_enabled() == 1'b0) && (trans_desc.can_drop_trans() == 1'b1)) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         wait_cycles(1);
         return;
      end

      //
      // Wait for the request delay time before moving on.
      //
      m_stimulus_busy++;
      if (get_next_delay(delay_value, pk_req_delay, trans_desc) == 1'b1) begin
         if (delay_value > 1) begin
            wait_cycles_with_release(delay_value - 1);
         end
      end

      if (cmd == NOP) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         m_stimulus_busy--;
         wait_cycles(1);
         return;
      end

      forever begin
         bit resource_conflict = 1'b0;

         //
         // Serialize accesses between sequencers.
         //
         m_get_trans_desc_busy.get(1);

         //
         // Now perform a lookup.  This line of code does a lot.  It takes in a transaction
         // descriptor from the sequencer side of the world, looks it up in a table and based on the
         // output creates a request descriptor to describe a bus request.  It also gives back
         // table descriptor which includes what needs to be done with the cache model for this
         // transaction.
         //
         req_desc = m_cache.int_req_lookup(table_desc, line_desc, trans_desc, req_desc, 0);

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
         end

         //
         // Do we need to drop the request?
         //
         if (((stimulus_is_enabled() == 1'b0) || (resource_conflict == 1'b1)) && (trans_desc.can_drop_trans() == 1'b1)) begin
            trans_desc.set_done();
            delete_active_mem_desc(trans_desc);
            m_stimulus_busy--;

            //
            // Allow another sequencer to proceed.
            //
            wait_cycles(1);
            m_get_trans_desc_busy.put(1);
            return;
         end

         if ((req_desc != null) && (!(trans_desc inside { m_active_bus_reqs }))) begin
            m_active_bus_reqs.push_back(trans_desc);
         end

         if (resource_conflict == 1'b0) begin
            break;
         end

         //
         // Allow another sequencer to proceed and wait one cycle to see if the resource conflict
         // has been resolved.
         //
         m_get_trans_desc_busy.put(1);
         wait_cycles(1);
      end // forever begin

      print_req(trans_desc);
      if (m_count_stimulus == 1'b1) begin
         m_trans_count++;
      end

      if (req_desc == null) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
      else begin
         //
         // Copy write data from trans_desc to req_desc when necessary.
         //
         if (table_desc.get_data_action() inside { Data_ReqWr }) begin
            evl_line_data line_data;

            line_data = new();
            if (trans_desc.get_write_data() == null) begin
               `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: data_action is Data_ReqWr but trans_desc has null write data"))
            end
            line_data.copy_data_from_line(trans_desc.get_write_data());
            req_desc.set_req_data(line_data);
         end

         req_desc.set_bus_req_id(0);

         //
         // Add in appropriate bus delays.
         //
         if (get_next_delay(delay_value, pk_bus_req_delay, trans_desc) == 1'b1) begin
            req_desc.add_delay(pk_bus_req_delay, delay_value);
         end

         //
         // Give the specific master agent a chance to modify the request before it's driven.
         //
         final_fixup(trans_desc, req_desc);

         if ((1 << req_desc.get_bus_req_size()) > m_data_width) begin
            req_desc.set_bus_req_len(0);
            req_desc.set_bus_req_size(m_data_width_clog2);
         end
         else begin
            req_desc.set_bus_req_len(0);
         end

         req_desc.set_as_original_req();

         `evl_log_mdesc(UVM_HIGH, VMOD_APB_STIM, get_abstract_name(), req_desc, $sformatf("sending this request descriptor: %s (%0d)", req_desc.sprint_obj(), int'(req_desc.get_bus_cmd())))
         m_pending_reqs.push_back(req_desc);
         if (m_count_stimulus == 1'b1) begin
            m_req_sent_count++;
         end
         trans_desc.set_int_param(pk_from_stim_agent, 1);
         m_snd_req_port.write(req_desc);
      end

      m_stimulus_busy--;
      wait_cycles(1);
      m_get_trans_desc_busy.put(1);
   endtask : fetch_and_process_trans_desc


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   // A specific agent may want to use this function to modify the request before it is driven.
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      return;
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_responses()
   //
   task watch_for_received_responses();
      evl_req_desc rcv_rsp_desc;

      if (m_rcv_rsp_fifo != null) begin
         forever begin
            m_rcv_rsp_fifo.get(rcv_rsp_desc);
            process_received_response(rcv_rsp_desc);
         end
      end
   endtask : watch_for_received_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_observed_requests()
   //
   task watch_for_received_requests();
      evl_req_desc rcv_req_desc;
      if (m_obs_req_fifo != null) begin
         forever begin
            m_obs_req_fifo.get(rcv_req_desc);
            process_received_request(rcv_req_desc);
         end
      end
   endtask : watch_for_received_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_received_response()
   //
   function void process_received_response(input evl_req_desc rcv_rsp_desc);
      evl_table_desc    table_desc;
      evl_req_desc      req_desc;
      evl_line_desc     line_desc;
      evl_trans_desc    trans_desc;
      evl_data_action_t data_action;

      `evl_log_mdesc(UVM_HIGH, VMOD_APB_STIM, get_abstract_name(), rcv_rsp_desc, $sformatf("response received for %0s", rcv_rsp_desc.sprint_obj()))
      foreach (m_pending_reqs[ii]) begin
         if (rcv_rsp_desc == m_pending_reqs[ii]) begin
            m_pending_reqs.delete(ii);
            break;
         end
      end
      req_desc = rcv_rsp_desc;

      m_cache.bus_rsp_lookup(table_desc, line_desc, req_desc, 0);
      req_desc.set_done();
      if (m_active_bus_reqs.size() == 0) begin
         `dut_error(get_abstract_name(), $sformatf("received an APB response with no pending request"))
         trans_desc = req_desc.get_root_trans_desc();
      end
      else begin
         trans_desc = m_active_bus_reqs.pop_front();
         if (trans_desc != req_desc.get_root_trans_desc()) begin
            evl_trans_desc exp_trans_desc = req_desc.get_root_trans_desc();

            `dut_error(get_abstract_name(), $sformatf("received an APB response for %0s ... expected it to be for %0s", trans_desc.sprint_obj(), exp_trans_desc.sprint_obj()))
         end
      end

      data_action = table_desc.get_data_action();
      if (data_action inside { Data_ReqRd }) begin
         evl_line_data rsp_data;

         rsp_data = new();
         if (req_desc.get_rsp_data() == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_response not expecting response data to be null"))
         end
         else begin
            rsp_data.copy_data_from_line(req_desc.get_rsp_data());
         end
         if (trans_desc != null) begin
            trans_desc.set_read_data(rsp_data);
         end
      end

      //
      // Mark the transaction as done.
      //
      if (trans_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_response not expecting trans_desc to be null"))
      end
      else begin
         print_req_done(trans_desc);
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
   endfunction : process_received_response


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_received_request()
   //
   // This task will get called for agents when an incoming request is observed on the bus.
   //
   virtual task process_received_request(input evl_req_desc req_desc);
      evl_trans_desc trans_desc;
      evl_table_desc table_desc;
      evl_line_desc  line_desc;
      evl_line_data  line_data;

      m_cache.bus_req_lookup(table_desc, line_desc, req_desc, 0);
      if (table_desc.get_data_action() == Data_MemRd) begin
         line_data = new();
         line_data.invalidate_line();
         case (req_desc.get_bus_req_size())
            3:       begin
                        line_data.set_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB, line_desc.get_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB));
                     end
            4:       begin
                        line_data.set_qword(req_desc.get_paddr() >> `EVL_QWORD_ADDR_LSB, line_desc.get_qword(req_desc.get_paddr() >> `EVL_QWORD_ADDR_LSB));
                     end
            5:       begin
                        line_data.set_oword(req_desc.get_paddr() >> `EVL_OWORD_ADDR_LSB, line_desc.get_oword(req_desc.get_paddr() >> `EVL_OWORD_ADDR_LSB));
                     end
            6:       begin
                        line_data.set_xword(req_desc.get_paddr() >> `EVL_XWORD_ADDR_LSB, line_desc.get_xword(req_desc.get_paddr() >> `EVL_XWORD_ADDR_LSB));
                     end
            default: begin
                        line_data.set_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB, line_desc.get_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB));
                     end
         endcase
         req_desc.set_rsp_data(line_data);
         req_desc.set_bus_rsp_size(req_desc.get_bus_req_size());
      end
      else if (table_desc.get_data_action() == Data_MemWr) begin
         case (req_desc.get_bus_req_size())
            3:       begin
                        line_desc.set_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB, req_desc.get_req_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB));
                     end
            4:       begin
                        line_desc.set_qword(req_desc.get_paddr() >> `EVL_QWORD_ADDR_LSB, req_desc.get_req_qword(req_desc.get_paddr() >> `EVL_QWORD_ADDR_LSB));
                     end
            5:       begin
                        line_desc.set_oword(req_desc.get_paddr() >> `EVL_OWORD_ADDR_LSB, req_desc.get_req_oword(req_desc.get_paddr() >> `EVL_OWORD_ADDR_LSB));
                     end
            6:       begin
                        line_desc.set_xword(req_desc.get_paddr() >> `EVL_XWORD_ADDR_LSB, req_desc.get_req_xword(req_desc.get_paddr() >> `EVL_XWORD_ADDR_LSB));
                     end
            default: begin
                        line_desc.set_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB, req_desc.get_req_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB));
                     end
         endcase
         req_desc.set_rsp_data(line_data);
         req_desc.set_bus_rsp_size(0);
      end
      req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
      req_desc.set_bus_rsp_valid(0, get_cycle_count());
      m_snd_rsp_port.write(req_desc);
   endtask : process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_pending_request
   //
   function void find_pending_request(output evl_req_desc  first_req_desc,
                                      output evl_req_desc  match_req_desc,
                                      input  evl_bus_cmd_t bus_cmd,
                                      input  evl_paddr_t   address);
      evl_req_desc req_desc;

      first_req_desc = null;
      match_req_desc = null;
      foreach (m_pending_reqs[ii]) begin
         req_desc = m_pending_reqs[ii];
         if (first_req_desc == null) begin
            first_req_desc = req_desc;
         end
         if ((req_desc.get_bus_cmd() == bus_cmd) && (req_desc.get_paddr() == address)) begin
            match_req_desc = req_desc;
            m_pending_reqs.delete(ii);
            return;
         end
         else begin
            m_pending_reqs.delete(ii);
         end
      end
      return;
   endfunction : find_pending_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      evl_req_desc  first_req_desc;
      evl_req_desc  match_req_desc;
      evl_bus_cmd_t bus_cmd;
      evl_paddr_t   address;

      if (paired_req_desc == null) begin
         bus_cmd = bus_cmd_in;
         address = address_in;
      end
      else begin
         bus_cmd = (bus_cmd_in  != `EVL_Bus_Idle)    ? bus_cmd_in : paired_req_desc.get_bus_cmd();
         address = (address_in  != ~evl_paddr_t'(0)) ? address_in : paired_req_desc.get_paddr();
      end
      find_pending_request(first_req_desc, match_req_desc, bus_cmd, address);
      if (match_req_desc != null) begin
         if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc)) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end
      return null;
   endfunction : find_observed_request


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
   function void add_int_req_act(input evl_cmd_t              cmd_in,
                                 input evl_cache_st_t         model_st_in,
                                 input byte                   qualifier_in,
                                 input evl_cache_st_t         tag_action_in,
                                 input evl_data_action_t      data_action_in,
                                 input evl_abstract_bus_cmd_t bus_cmd_in);
      m_cache.add_int_req_act(cmd_in, model_st_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in);
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
   function void add_bus_req_act(input evl_bus_cmd_t           bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input byte                    qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in);
      m_cache.add_bus_req_act(bus_cmd_in, model_st_in, qualifier_in, tag_action_in, data_action_in, bus_rsp_in);
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
   function void add_bus_rsp_act(input evl_bus_cmd_t     bus_cmd_in,
                                 input evl_cache_st_t    model_st_in,
                                 input byte              qualifier_in,
                                 input evl_cache_st_t    tag_action_in,
                                 input evl_data_action_t data_action_in);
      m_cache.add_bus_rsp_act(bus_cmd_in, model_st_in, qualifier_in, tag_action_in, data_action_in);
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_rsp", port_number);
      if ((port_number == 0) && (m_snd_rsp_port != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_rsp_port.get_full_name(), bus_export.get_full_name()))
         m_snd_rsp_port.connect(bus_export);
      end

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
      if ((port_number == 0) && (m_snd_req_port != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_req_port.get_full_name(), bus_export.get_full_name()))
         m_snd_req_port.connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("obs_req", port_number);
      if ((port_number == 0) && (m_obs_req_export != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_obs_req_export.get_full_name()))
         bus_port.connect(m_obs_req_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("sa_rcv_rsp", port_number);
      if ((port_number == 0) && (m_rcv_rsp_export != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_rcv_rsp_export.get_full_name()))
         bus_port.connect(m_rcv_rsp_export);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      evl_sequencer_mem mem_sequencer;

      super.build_phase(phase);

      mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
      set_sequencer_wrapper(mem_sequencer.get_wrapper());

      m_cache = evl_stim_cache_apb::type_id::create("cache", this);
      m_cache.set_abstract_name($sformatf("%0s($)", get_root_abstract_name()));
      m_cache.set_rand_gen(m_rand_gen);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_obs_req_export != null) begin
         m_obs_req_export.connect(m_obs_req_fifo.analysis_export);
      end
      if (m_rcv_rsp_export != null) begin
         m_rcv_rsp_export.connect(m_rcv_rsp_fifo.analysis_export);
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      if (m_rtl_port != null) begin
         m_data_width = m_rtl_port.get_bus_data_width();
      end
      if (m_data_width < 1) begin
         m_data_width = `EVL_WORD_BYTES;
      end
      m_data_width_clog2    = $clog2(m_data_width);
      m_max_cmd_length      = m_data_width;

      m_template_cache_data = evl_line_data::type_id::create(get_abstract_name());
      m_template_req_data   = evl_line_data::type_id::create(get_abstract_name());
      m_template_line_desc  = evl_line_desc::type_id::create(get_abstract_name());
      m_template_trans_desc = evl_trans_desc::type_id::create(get_abstract_name());
      m_template_req_desc   = evl_req_desc::type_id::create(get_abstract_name());

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
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_APB);

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

      if (m_tables_built++ == 0) begin
         build_tables();
      end
      m_tables_built = 1;

      fork
         fetch_and_process_loop();
         watch_for_received_responses();
         watch_for_received_requests();
         watch_for_timeouts();
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      fork
         begin
            super.post_reset_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostResetPhase), 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      fork
         begin
            super.post_configure_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostConfigurePhase), 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      fork
         begin
            super.post_main_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostMainPhase), 1'b1, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_phase
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
   // Function: report_phase
   //
   virtual function void report_phase(uvm_phase phase);
      super.report_phase(phase);
      if (m_trans_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of transactions performed during test: %0d", m_trans_count))
      end
      if (m_req_sent_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of bus requests sent during test:      %0d", m_req_sent_count))
      end
      if (m_req_rcv_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of bus requests received during test:  %0d", m_req_rcv_count))
      end
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_verif_comp",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = NO_AGENT);
      super.new(name_in, parent_in, agent_type_in);
   endfunction : new

endclass : evl_stim_agent_apb

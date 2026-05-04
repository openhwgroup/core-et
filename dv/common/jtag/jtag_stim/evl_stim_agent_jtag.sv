//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_agent_jtag
//
class evl_stim_agent_jtag extends evl_verif_comp;

   `uvm_component_utils(evl_stim_agent_jtag)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Delay Keys
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member TLM Ports
   //
   uvm_analysis_port     #(evl_req_desc) m_snd_req_ports[$];
   uvm_analysis_export   #(evl_req_desc) m_rcv_rsp_exports[$];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rcv_rsp_fifos[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit               m_count_stimulus      = 1'b0;
   int               m_stimulus_busy       = 0;
   int               m_req_sent_count      = 0;
   int               m_trans_count         = 0;
   semaphore         m_get_trans_desc_busy = new(1);
   evl_sequencer_mem m_mem_sequencer;
   evl_req_desc      m_pending_reqs[$];
   evl_line_data     m_template_line_data;
   evl_line_desc     m_template_line_desc;
   evl_trans_desc    m_template_trans_desc;
   evl_req_desc      m_template_req_desc;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_info
   //
   virtual function string format_req_info(input evl_trans_desc trans_desc);
      evl_cmd_t cmd;

      cmd = trans_desc.get_cmd();
      if (cmd inside { JTAG_STEP }) begin
         return $sformatf("%0p tms:%0d tdo:%0d", cmd, trans_desc.get_paddr() & evl_paddr_t'(1), trans_desc.get_write_xword(0) & evl_xword_t'(1));
      end
      else if (cmd inside { SELECT_CHAIN, JTAG_WR, JTAG_WR_PARTIAL }) begin
         return $sformatf("%0p %0d 0x%x", cmd, trans_desc.get_paddr(), trans_desc.get_write_xword(0));
      end
      else if (cmd inside { JTAG_RD, JTAG_RD_PARTIAL }) begin
         return $sformatf("%0p %0d", cmd, trans_desc.get_paddr());
      end
      else begin
         return $sformatf("%0p", cmd);
      end
   endfunction : format_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req
   //
   virtual function void print_req(input evl_trans_desc trans_desc);
      if (trans_desc.get_cmd() inside { JTAG_STEP, JTAG_RESET, JTAG_RUN_TEST }) begin
         `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s", format_req_info(trans_desc)))
      end
      else begin
         `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s (%0d)", format_req_info(trans_desc), trans_desc.get_cmd_length()))
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
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_rcv_rsp_export") || (port_name == "rcv_rsp")) begin
         if (port_number < m_rcv_rsp_exports.size()) begin
            return m_rcv_rsp_exports[port_number];
         end
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
   // Task: fetch_and_process_loop_task
   //
   virtual task fetch_and_process_loop_task(input evl_sequencer_mem mem_sequencer);
      forever begin
         wait_cycles(1);
         while (stimulus_is_enabled() == 1'b1) begin
            fetch_and_process_trans_desc(mem_sequencer);
         end
         wait_for_stimulus_enable();
      end
   endtask : fetch_and_process_loop_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop
   //
   virtual function void fetch_and_process_loop(input evl_sequencer_mem mem_sequencer);
      fork
         fetch_and_process_loop_task(mem_sequencer);
      join_none
   endfunction : fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Task: master_fetch_and_process_loop
   //
   virtual task master_fetch_and_process_loop();
      int sequencer_count = 0;

      wait_for_stimulus_enable();
      fetch_and_process_loop(m_mem_sequencer);
      wait (sequencer_count == 0);
   endtask : master_fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fetch_and_process_trans_desc
   //
   virtual task fetch_and_process_trans_desc(input evl_sequencer_mem mem_sequencer);
      evl_trans_desc   trans_desc;
      evl_req_desc     req_desc;
      evl_sequence_mem sequence_mem;
      evl_cmd_t        cmd;

      req_desc = null;
      mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      if (trans_desc == null) begin
         wait_cycles(1);
         return;
      end
      cmd = trans_desc.get_cmd();
      `evl_log_addr(UVM_HIGH, VMOD_JTAG_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("fetch_and_process got this cmd: %0p", cmd))
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
         m_stimulus_busy = 0;
         return;
      end

      //
      // Wait for the request delay time before moving on.
      //
      m_stimulus_busy++;

      if (!(cmd inside { `EVL_JTAG_CMDS })) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
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
         $cast(req_desc, m_template_req_desc.clone());
         req_desc.set_bus_cmd(evl_bus_cmd_t'(cmd));
         trans_desc.copy_to_desc(req_desc);
         trans_desc.add_child_desc(req_desc);
         if (cmd inside { JTAG_RESET, JTAG_RUN_TEST }) begin
            req_desc.set_paddr(0);
         end
         else begin
            req_desc.set_paddr(trans_desc.get_paddr());
            req_desc.set_req_xword(0, trans_desc.get_write_xword(0));
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
      m_trans_count++;

      if (req_desc == null) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
      else begin
         req_desc.set_as_original_req();

         `evl_log_mdesc(UVM_HIGH, VMOD_JTAG_STIM, get_abstract_name(), req_desc, $sformatf("sending this request descriptor"))
         m_pending_reqs.push_back(req_desc);
         m_req_sent_count++;
         m_snd_req_ports[0].write(req_desc);
      end

      //
      // Now that we have processed the request, allow another sequencer to proceed.
      //
      m_stimulus_busy--;
      wait_cycles(1);
      m_get_trans_desc_busy.put(1);
   endtask : fetch_and_process_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_responses()
   //
   task watch_for_received_responses();
      uvm_tlm_analysis_fifo #(evl_req_desc) rcv_rsp_fifo;
      evl_req_desc                          rcv_rsp_desc;

      if ((m_rcv_rsp_fifos.size() == 0) || (m_rcv_rsp_fifos[0] == null)) begin
         return;
      end
      rcv_rsp_fifo = m_rcv_rsp_fifos[0];
      forever begin
         rcv_rsp_fifo.get(rcv_rsp_desc);
         process_received_response(rcv_rsp_desc);
      end
   endtask : watch_for_received_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_received_response()
   //
   // This function will get called for Master Agents when a response is
   // observed on the bus. The evl_req_desc sent is actually the same
   // one that was originally sent on sa_snd_req_port.
   //
   function void process_received_response(input evl_req_desc req_desc);
      evl_trans_desc trans_desc;

      trans_desc = req_desc.get_root_trans_desc();
      `evl_log_mdesc(UVM_HIGH, VMOD_JTAG_STIM, get_abstract_name(), req_desc, $sformatf("response received for %0s", req_desc.sprint_obj()))

      foreach (m_pending_reqs[ii]) begin
         if (req_desc == m_pending_reqs[ii]) begin
            m_pending_reqs.delete(ii);
            break;
         end
      end
      // Mark the transaction as done.
      if (trans_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_response not expecting trans_desc to be null"))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_JTAG_STIM, get_abstract_name(), req_desc, $sformatf("process_received_response marking this transaction done"))
         trans_desc.set_read_xword(0, req_desc.get_rsp_xword(0));
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
   endfunction : process_received_response


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

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
      if ((port_number < m_snd_req_ports.size()) && (m_snd_req_ports[port_number] != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_req_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_req_ports[port_number].connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_rcv_rsp_port", port_number);
      if ((port_number < m_rcv_rsp_exports.size()) && (m_rcv_rsp_exports[port_number] != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_rcv_rsp_exports[port_number].get_full_name()))
         bus_port.connect(m_rcv_rsp_exports[port_number]);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[0]   = new($sformatf("m_snd_req_port"),   this);
      m_rcv_rsp_exports[0] = new($sformatf("m_rcv_rsp_export"), this);
      m_rcv_rsp_fifos[0]   = new($sformatf("m_rcv_rsp_fifo"),   this);

      m_mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
      set_sequencer_wrapper(m_mem_sequencer.get_wrapper());
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
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
      super.start_of_simulation_phase(phase);

      m_template_line_data  = evl_line_data::type_id::create(get_abstract_name());
      m_template_line_desc  = evl_line_desc::type_id::create(get_abstract_name());
      m_template_trans_desc = evl_trans_desc::type_id::create(get_abstract_name());
      m_template_req_desc   = evl_req_desc::type_id::create(get_abstract_name());

      m_template_trans_desc.set_shire_id(~0);
      m_template_trans_desc.set_neighborhood_id(~0);
      m_template_trans_desc.set_core_id(~0);

      m_template_req_desc.set_shire_id(~0);
      m_template_req_desc.set_neighborhood_id(~0);
      m_template_req_desc.set_core_id(~0);

      m_template_line_desc.set_rand_gen(m_rand_gen);
      m_template_trans_desc.set_rand_gen(m_rand_gen);
      m_template_req_desc.set_rand_gen(m_rand_gen);

      m_template_line_desc.set_master_agent(this);
      m_template_trans_desc.set_master_agent(this);
      m_template_req_desc.set_master_agent(this);

      m_template_line_desc.set_data_template(m_template_line_data);
      m_template_trans_desc.set_data_template(m_template_line_data);
      m_template_req_desc.set_data_template(m_template_line_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_JTAG);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

      m_mem_sequencer.set_trans_desc_template(m_template_trans_desc);

      fork
         master_fetch_and_process_loop();
         watch_for_received_responses();
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
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostResetPhase), 1'b1);
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
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostConfigurePhase), 1'b1);
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
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostMainPhase), 1'b1, 1'b1);
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
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "JTAG",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = JTAG_STIM_AGENT);
      super.new(name_in, parent_in, agent_type_in);
      set_active_master();
      m_allow_sys_clk = 1'b0;
   endfunction : new

endclass : evl_stim_agent_jtag

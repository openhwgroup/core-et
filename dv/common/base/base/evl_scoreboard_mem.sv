//--------------------------------------------------------------------------------------------------
//
// Class: evl_scoreboard_mem
//
virtual class evl_scoreboard_mem extends evl_scoreboard;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static int m_all_active_desc_count = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_active_desc_count:  This integer is a count of the number of currently outstanding
   //                       descriptors on this verification component.  For bus verification
   //                       components, this will be the number of request descriptors currently
   //                       waiting for responses.  For verification components creating root
   //                       transactions, this will be the number of transaction descriptors
   //                       currently waiting for completion.  There are specific methods for
   //                       manipulating this variable (add_active_mem_desc(),
   //                       delete_active_mem_desc(), check_active_mem_desc(),
   //                       wait_for_active_mem_desc(), and watch_for_timeouts())).
   //                       ------------------------------------------------------------------------
   // m_active_mem_descs[]: This is a list of active descriptors (refer to m_active_desc_count).
   //
   int          m_active_desc_count = 0;
   evl_mem_desc m_active_mem_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_export_to_my_export
   //
   virtual function void connect_export_to_my_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                     input string                             port_name      = "unknown",
                                                     input int                                port_number    = 0,
                                                     input int                                subport_number = 0);
      return;
   endfunction : connect_export_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      return;
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_export_to_export
   //
   virtual function void connect_my_export_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                     input string                             port_name      = "unknown",
                                                     input int                                port_number    = 0,
                                                     input int                                subport_number = 0);
      return;
   endfunction : connect_my_export_to_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_port_to_export
   //
   virtual function void connect_my_port_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                   input string                             port_name      = "unknown",
                                                   input int                                port_number    = 0,
                                                   input int                                subport_number = 0);
      return;
   endfunction : connect_my_port_to_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      return;
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_active_mem_desc
   //
   virtual function void add_active_mem_desc(input evl_mem_desc mem_desc_in);
      if (!(mem_desc_in inside { m_active_mem_descs })) begin
         m_active_mem_descs.push_back(mem_desc_in);
         m_all_active_desc_count++;
         m_active_desc_count++;
      end
   endfunction : add_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_mem_desc
   //
   virtual function void delete_active_mem_desc(input evl_mem_desc mem_desc_in);
      foreach (m_active_mem_descs[ii]) begin
         if (mem_desc_in == m_active_mem_descs[ii]) begin
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
            return;
         end
      end
   endfunction : delete_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_active_mem_desc
   //
   virtual function check_active_mem_desc(input int cycle_decrement);
      for (int ii = 0; ii < m_active_mem_descs.size(); ii++) begin
         bit now_done;

         if (m_active_mem_descs[ii].check_for_timeout(now_done, cycle_decrement) != 1'b0) begin
            sb_error($sformatf("%s timed out", m_active_mem_descs[ii].sprint_obj()), m_active_mem_descs[ii], {}, "", `__FILE__, `__LINE__);
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
         end
         else if (now_done == 1'b1) begin
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
         end
      end
   endfunction : check_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_active_mem_desc
   //
   virtual task wait_for_active_mem_desc(input int timeout_value, input bit allow_progress = 1'b0);
      if ((m_rtl_port != null) && (m_rtl_port.power_is_off() == 1'b1)) begin
         return;
      end
      wait_cycles(1);
      if ((m_active_desc_count <= 0) && (allow_progress == 1'b0)) begin
         return;
      end
      if (allow_progress == 1'b1) begin
         fork
            begin
               //
               // Wait for the timeout value ... if we reach it, things are a mess.
               //
               wait_cycles(timeout_value);
            end
            begin
               #0 if (m_all_active_desc_count <= 0) begin
                  repeat (20) begin
                     wait_cycles(1);
                     #0 if (m_all_active_desc_count > 0) begin
                        break;
                     end
                  end
               end
               while (m_all_active_desc_count > 0) begin
                  #0 wait (m_all_active_desc_count <= 0);
                  repeat (20) begin
                     wait_cycles(1);
                     #0 if (m_all_active_desc_count > 0) begin
                        break;
                     end
                  end
               end
            end
         join_any
         disable fork;
      end
      if (m_active_desc_count <= 0) begin
         return;
      end

      //
      // Now perform the final check.
      //
      fork
         begin
            wait (m_active_desc_count <= 0);
         end
         begin
            wait_cycles(timeout_value);
         end
      join_any
      disable fork;
      #0 check_active_mem_desc(0);
      if (m_active_desc_count > 0) begin
         sb_error($sformatf("timed out waiting for %0d transaction%0s to complete", m_active_desc_count, ((m_active_desc_count == 1) ? "" : "s")), null, {}, "", `__FILE__, `__LINE__);
         `evl_log_no_addr(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), "Currently active transactions:")
         foreach (m_active_mem_descs[ii]) begin
            `evl_log_mdesc(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), m_active_mem_descs[ii], $sformatf("%p 0x%x", m_active_mem_descs[ii].get_cmd(), m_active_mem_descs[ii].get_paddr()))
         end
      end
   endtask : wait_for_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   //    A bus VC might ask a scoreboard if requests coming from certain sources are to be checked
   //    useful for scoreboards that don't model the whole behaviour of a block, like neigh SB, or
   //    if the scoreboard is disabled
   //
   virtual function bit is_req_checked(input evl_req_desc req_desc_in);
      return m_enabled;
   endfunction : is_req_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_rsp_checked
   //
   //    A bus VC might ask a scoreboard if responses coming from certain sources are to be checked
   //    useful for scoreboards that don't model the whole behaviour of a block, like neigh SB, or
   //    if the scoreboard is disabled
   //
   virtual function bit is_rsp_checked(input evl_req_desc req_desc_in);
      return m_enabled;
   endfunction : is_rsp_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_pending_request
   //
   // This function is used to search a queue of descriptors and find one matching the request ID,
   // command, and address (the request ID can be ignored)
   //
   // Input Variables:
   //
   // pending_reqs:     This is the descriptor queue to search
   // req_id:           This is the request ID to match against
   // bus_cmd:          This is the bus command to match against
   // address:          This is the physical address to match against
   // ignore_id:        This flag indicates that the request ID should not be used for matching
   //
   // Output Variables:
   //
   // first_req_desc:   This will hold the first descriptor in the queue (null if empty)
   // match_req_desc:   This will hold the matching  descriptor in the queue (null if not found)
   //
   function void find_pending_request(input  evl_req_desc         pending_reqs[$],
                                      output evl_req_desc         first_req_desc,
                                      output evl_req_desc         match_req_desc,
                                      input  evl_bus_req_id_t     req_id,
                                      input  evl_bus_cmd_t        bus_cmd,
                                      input  evl_paddr_t          address,
                                      input  evl_bus_req_source_t source    = evl_bus_req_source_t'(0),
                                      input  bit                  ignore_id = 1'b0);
      evl_req_desc req_desc;

      first_req_desc = null;
      match_req_desc = null;
      foreach (pending_reqs[ii]) begin
         req_desc = pending_reqs[ii];

         if (req_desc == null) begin
            sb_error("find_pending_request: req_desc is null, SAD!", null, {}, "", `__FILE__, `__LINE__);
         end
         if ((ignore_id == 1'b1) || (req_desc.get_bus_req_id() == req_id)) begin
            if (first_req_desc == null) begin
               first_req_desc = req_desc;
            end
            if ((req_desc.get_bus_cmd() == bus_cmd) && (req_desc.get_paddr() == address) && (req_desc.get_bus_req_source() == source)) begin
               match_req_desc = req_desc;
               return;
            end
         end
      end
      return;
   endfunction : find_pending_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_predicted_request
   //
   // This function is used to perform some checks when a request predicted by this scoreboard is
   // observed on a bus. The scoreboard should have queued a golden descriptor on a pending
   // descriptor queue, and when a match is found the descriptor will be queued on the provided
   // active descriptor queue.
   //
   // Input Variables:
   //
   // obs_req_desc:     This is the descriptor observed on the bus
   // pending_reqs:     This is the descriptor queue to search
   // active_reqs:      This is the descriptor queue to push the matched decriptor on
   // ignore_id:        This flag indicates that the request ID should not be used for matching
   // relaxed_order:    If this flag is set, order check will only consider descriptors
   //                   explicitely marked as dependencies
   //
   // Input Variables:
   //
   // first_req_desc:   This will hold the first descriptor in the queue (null if empty)
   // match_req_desc:   This will hold the matching  descriptor in the queue (null if not found)
   //
   function void process_predicted_request(input  evl_req_desc     obs_req_desc,
                                           ref    evl_req_desc     pending_reqs[$],
                                           ref    evl_req_desc     active_reqs[$],
                                           input  bit              ignore_id     = 1'b0,
                                           input  bit              relaxed_order = 1'b0);

      evl_req_desc first_req_desc;
      evl_req_desc match_req_desc;

      find_pending_request(pending_reqs, first_req_desc, match_req_desc, obs_req_desc.get_bus_req_id(), obs_req_desc.get_bus_cmd(), obs_req_desc.get_paddr(), obs_req_desc.get_bus_req_source(), ignore_id);

      if (match_req_desc != null) begin
         string err_messages[];

         if ((match_req_desc != first_req_desc) && ((relaxed_order == 1'b0) || (first_req_desc.depends_on(match_req_desc)))) begin
            sb_error($sformatf("expected request %0sto be %0s, not %0s", (ignore_id == 1'b0) ? $sformatf("ID 0x%0x ", first_req_desc.get_bus_req_id()) : "", first_req_desc.sprint_obj(), obs_req_desc.sprint_obj()), obs_req_desc, {}, "", `__FILE__, `__LINE__);
         end
         if (obs_req_desc.get_parent_desc() != match_req_desc) begin
            match_req_desc.add_child_desc(obs_req_desc);
         end

         // Remove the matched descriptor from the pending list and add it to the active list
         foreach (pending_reqs[ii]) begin
            if (pending_reqs[ii] == match_req_desc) begin
               pending_reqs.delete(ii);
               break;
            end
         end
         active_reqs.push_back(match_req_desc);

         // Compare the descriptors attributes
         if (obs_req_desc.compare_attrs(match_req_desc, err_messages)) begin
            foreach (err_messages[ii]) begin
               sb_error(err_messages[ii], obs_req_desc, {}, "", `__FILE__, `__LINE__);
            end
         end
         if (obs_req_desc.compare_req_attrs(match_req_desc, err_messages)) begin
            foreach (err_messages[ii]) begin
               sb_error(err_messages[ii], obs_req_desc, {}, "", `__FILE__, `__LINE__);
            end
         end
      end
      else begin
         sb_error($sformatf("no such request was expected (%0s)", obs_req_desc.sprint_obj()), obs_req_desc, {}, "", `__FILE__, `__LINE__);
         if (first_req_desc != null) begin
            sb_error($sformatf("expected request %0s", first_req_desc.sprint_obj()), obs_req_desc, {}, "", `__FILE__, `__LINE__);
         end
         active_reqs.push_back(obs_req_desc);
      end
   endfunction : process_predicted_request


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_timeouts
   //
   virtual task watch_for_timeouts();
      forever begin
         wait (m_active_desc_count > 0);
         wait_cycles(1000);
         #0 check_active_mem_desc(1000);
      end
   endtask : watch_for_timeouts


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: end_of_elaboration_phase
   //
   virtual function void end_of_elaboration_phase(uvm_phase phase);
      super.end_of_elaboration_phase(phase);
   endfunction : end_of_elaboration_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      super.reset_phase(phase);
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      super.post_reset_phase(phase);
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      super.pre_configure_phase(phase);
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      super.configure_phase(phase);
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      super.post_configure_phase(phase);
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      super.pre_main_phase(phase);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      super.post_main_phase(phase);
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      super.pre_shutdown_phase(phase);
   endtask : pre_shutdown_phase


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
            wait_for_active_mem_desc((m_enabled == 1'b1) ? m_global_attrs.get_timeout_value(ShutdownPhase) : 1, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_scoreboard_mem", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_scoreboard_mem

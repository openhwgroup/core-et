//--------------------------------------------------------------------------------------------------
//
// Class: evl_verif_comp_map
//
class evl_verif_comp_map extends evl_component;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_verif_comp_map        m_verif_comp_map            = evl_verif_comp_map::get_inst();
   static bit                       m_start_sequences           = 1'b0;
   static bit                       m_restart_sequences         = 1'b0;
   static bit                       m_terminating_seq           = 1'b0;
   static bit                       m_disable_configure_timeout = 1'b0;
   static bit                       m_disable_shutdown_timeout  = 1'b0;
   static bit                       m_display_objections        = 1'b0;
   static bit                       m_start_final_phase         = 1'b0;
   static bit                       m_fast_finish               = 1'b0;
   static int                       m_final_count               = 0;
   static int                       m_quiescent_value           = 0;
   static evl_agent_id_t            m_agent_vids[evl_agent_id_key_t];
   static evl_agent_type_t          m_agent_type_xrefs[string];
   static evl_verif_comp            m_system_agent;
   static evl_verif_comp            m_verif_comp_list[$];
   static evl_sequence_wrapper_base m_sequence_wrappers[$];
   static evl_tcl_handler           m_global_tcl_scipt;
   static uvm_phase                 m_uvm_phases[evl_phase_type_t];
   static uvm_phase                 m_cur_uvm_phase;
   static evl_rtl_port              m_rtl_port;
   static string                    m_ust_start_phase;

   // These 4 variables are used for simulation performance calculation
   //
   static longint                   m_start_time;            // records the start time of the sim in seconds since start of unix time
   static longint                   m_end_time;              // records the end time of the sim in seconds since start of unix time
   static longint                   m_duration_time;         // Holds the calculated duration of the sim (end_time-start_time)
   static longint                   m_cycles_per_second;     // performance of the simulation
   static longint                   m_cur_time;

   // What is the maximum cycle count we've set for this simulation?
   static int                       m_max_cycle_count;
   static int                       m_max_shutdown_cycle_count;


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_inst
   //
   static function evl_verif_comp_map get_inst();
      if (m_verif_comp_map == null) begin
         evl_verif_comp_map verif_comp_map;

         verif_comp_map = new();
      end
      return m_verif_comp_map;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_verif_comp
   //
   static function evl_verif_comp get_verif_comp(input  evl_agent_type_t      agent_type_in,
                                                 input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                 input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                 input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                 input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      return m_verif_comp_map.get_verif_comp_local(agent_type_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_verif_comp_by_name
   //
   static function evl_verif_comp get_verif_comp_by_name(input  string                agent_type_name_in,
                                                         input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                         input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                         input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                         input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      return m_verif_comp_map.get_verif_comp_by_name_local(agent_type_name_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_verif_comp_list
   //
   static function int get_verif_comp_list(output evl_verif_comp        verif_comp_list[],
                                           input  evl_agent_type_t      agent_type_in,
                                           input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                           input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                           input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                           input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      return m_verif_comp_map.get_verif_comp_list_local(verif_comp_list, agent_type_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp_list


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_verif_comp_list_by_name
   //
   static function int get_verif_comp_list_by_name(output evl_verif_comp        verif_comp_list[],
                                                   input  string                agent_type_name_in,
                                                   input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                   input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                   input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                   input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      return m_verif_comp_map.get_verif_comp_list_by_name_local(verif_comp_list, agent_type_name_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp_list_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::add_sequence_wrapper_to_list
   //
   static function void add_sequence_wrapper_to_list(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      m_verif_comp_map.add_me_to_sequence_wrapper_list(sequence_wrapper_in);
   endfunction : add_sequence_wrapper_to_list


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::set_terminating_sequence
   //
   static function void set_terminating_sequence();
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      m_verif_comp_map.set_terminating_sequence_local();
   endfunction : set_terminating_sequence


   //-----------------------------------------------------------------------------------------------
   //
   // Static Task: evl_verif_comp_map::display_objections
   //
   static task display_objections();
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      m_verif_comp_map.display_objections_local();
   endtask : display_objections


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: assert_final_phase
   //
   static function void assert_final_phase(input bit fast_finish = 1'b0);
      if (m_verif_comp_map == null) begin
         void'(evl_verif_comp_map::get_inst());
      end
      m_verif_comp_map.assert_final_phase_local(fast_finish);
   endfunction : assert_final_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_me_to_verif_comp_list
   //
   function void add_me_to_verif_comp_list(input evl_verif_comp agent_in);
      if ((agent_in != null) && (!(agent_in inside { m_verif_comp_list }))) begin
         m_verif_comp_list.push_back(agent_in);
         if (agent_in.agent_match(SYSTEM_AGENT, 0) != 0) begin
            m_system_agent = agent_in;
         end
      end
      m_verif_comp_list.sort with ({ item.m_shire_id, item.m_neighborhood_id, item.m_core_id, item.m_agent_type, item.m_agent_id });
   endfunction : add_me_to_verif_comp_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_me_to_sequence_wrapper_list
   //
   function void add_me_to_sequence_wrapper_list(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (!(sequence_wrapper_in inside { m_sequence_wrappers })) begin
         m_sequence_wrappers.push_back(sequence_wrapper_in);
         if (m_start_sequences == 1'b1) begin
            m_restart_sequences = 1'b1;
         end
      end
   endfunction : add_me_to_sequence_wrapper_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_terminating_sequence_local
   //
   function void set_terminating_sequence_local();
      m_terminating_seq = 1'b1;
   endfunction : set_terminating_sequence_local


   //-----------------------------------------------------------------------------------------------
   //
   // Task: display_objections_local
   //
   task display_objections_local();
      uvm_objection objection;

      if (m_cur_uvm_phase != null) begin
         objection = m_cur_uvm_phase.get_objection();
         #0 objection.display_objections();
      end
   endtask : display_objections_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assert_final_phase_local
   //
   function void assert_final_phase_local(input bit fast_finish = 1'b0);
      m_fast_finish       = fast_finish;
      m_start_final_phase = 1'b1;
   endfunction : assert_final_phase_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_vid_for_alias
   //
   function evl_agent_id_t get_agent_vid_for_alias(input evl_verif_comp agent_in, input evl_agent_type_t agent_alias_in, input parent_count = 1);
      if (agent_in != null) begin
         evl_verif_comp        vc_parent;
         evl_shire_id_t        shire_id;
         evl_neighborhood_id_t neighborhood_id;
         evl_core_id_t         core_id;
         evl_agent_id_key_t    agent_id_key;
         string                alias_name;

         vc_parent = agent_in.get_vc_parent();
         for (int ii = 1; ii < parent_count; ii++) begin
            if (vc_parent == null) begin
               break;
            end
            vc_parent = vc_parent.get_vc_parent();
         end
         if (vc_parent == null) begin
            shire_id        = ~evl_shire_id_t'(0);
            neighborhood_id = ~evl_neighborhood_id_t'(0);
            core_id         = ~evl_core_id_t'(0);
         end
         else begin
            shire_id        = vc_parent.get_shire_id();
            neighborhood_id = vc_parent.get_neighborhood_id();
            core_id         = vc_parent.get_core_id();
         end
         if (shire_id == ~evl_shire_id_t'(0)) begin
            agent_id_key = { ~evl_shire_id_t'(0), ~evl_neighborhood_id_t'(0), ~evl_core_id_t'(0), agent_alias_in };
         end
         else if (neighborhood_id == ~evl_neighborhood_id_t'(0)) begin
            agent_id_key = { shire_id, ~evl_neighborhood_id_t'(0), ~evl_core_id_t'(0), agent_alias_in };
         end
         else if (core_id == ~evl_core_id_t'(0)) begin
            agent_id_key = { shire_id, neighborhood_id, ~evl_core_id_t'(0), agent_alias_in };
         end
         else begin
            agent_id_key = { shire_id, neighborhood_id, core_id, agent_alias_in };
         end
         if (!m_agent_vids.exists(agent_id_key)) begin
            m_agent_vids[agent_id_key] = evl_agent_id_t'(0);
         end
         else begin
            m_agent_vids[agent_id_key] += evl_agent_id_t'(1);
         end
         alias_name = $sformatf("%p-%0d", agent_alias_in, m_agent_vids[agent_id_key]);
         return m_agent_vids[agent_id_key];
      end
      return ~evl_agent_id_t'(0);
   endfunction : get_agent_vid_for_alias


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verif_comp_list_local
   //
   function int get_verif_comp_list_local(output evl_verif_comp        verif_comp_list[],
                                          input  evl_agent_type_t      agent_type_in,
                                          input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                          input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                          input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                          input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      evl_verif_comp verif_comps[$];

      foreach (m_verif_comp_list[ii]) begin
         if (m_verif_comp_list[ii].agent_match(agent_type_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in) == 1'b1) begin
            verif_comps.push_back(m_verif_comp_list[ii]);
         end
      end
      if (verif_comps.size() == 0) begin
         verif_comp_list = new[0];
         return 0;
      end
      verif_comp_list = new[verif_comps.size()];
      foreach (m_verif_comp_list[ii]) begin
         verif_comp_list[ii] = verif_comps[ii];
      end
      return verif_comps.size();
   endfunction : get_verif_comp_list_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verif_comp_list_by_name_local
   //
   function int get_verif_comp_list_by_name_local(output evl_verif_comp        verif_comp_list[],
                                                  input  string                agent_type_name_in,
                                                  input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                  input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                  input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                  input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (!(m_agent_type_xrefs.exists(agent_type_name_in))) begin
         verif_comp_list = new[0];
         return 0;
      end
      return get_verif_comp_list_local(verif_comp_list, m_agent_type_xrefs[agent_type_name_in], agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp_list_by_name_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verif_comp_local
   //
   function evl_verif_comp get_verif_comp_local(input  evl_agent_type_t      agent_type_in,
                                                input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      evl_verif_comp verif_comp_list[];

      if (get_verif_comp_list_local(verif_comp_list, agent_type_in, agent_id_in, shire_id_in, neighborhood_id_in, core_id_in) <= 0) begin
         return null;
      end
      return verif_comp_list[0];
   endfunction : get_verif_comp_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verif_comp_by_name_local
   //
   function evl_verif_comp get_verif_comp_by_name_local(input string                agent_type_name_in,
                                                        input evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                                        input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                        input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                        input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (!(m_agent_type_xrefs.exists(agent_type_name_in))) begin
         return null;
      end
      return get_verif_comp_local(m_agent_type_xrefs[agent_type_name_in], agent_id_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : get_verif_comp_by_name_local


   //-----------------------------------------------------------------------------------------------
   //
   // Task: start_sequences
   //
   task start_sequences();
      #0 wait (m_start_sequences == 1'b1);
      foreach (m_sequence_wrappers[ii]) begin
         m_sequence_wrappers[ii].start_sequence();
      end
      forever begin
         #0 wait (m_restart_sequences == 1'b1);
         #0;
         foreach (m_sequence_wrappers[ii]) begin
            m_sequence_wrappers[ii].start_sequence();
         end
         m_restart_sequences = 1'b0;
      end
   endtask : start_sequences


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_errors
   //
   task watch_for_errors();
      evl_phase_type_t cur_phase;

      #0 m_global_attrs.wait_for_error_finish();
      #0;
      while (m_global_attrs.get_cur_phase() < PreShutdownPhase) begin
         #0 cur_phase = m_global_attrs.get_cur_phase();
         if (m_system_agent != null) begin
            m_system_agent.wait_cycles(20);
         end
         if (m_global_attrs.get_cur_phase() == cur_phase) begin
            if ((cur_phase < PreShutdownPhase) && (m_uvm_phases.exists(cur_phase))) begin
               uvm_objection phase_objection;

               `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("killing %p", cur_phase))
               phase_objection = m_uvm_phases[cur_phase].get_objection();
               if (phase_objection != null) begin
                  phase_objection.clear(null);
               end
               m_global_attrs.wait_for_end_of_phase(cur_phase);
            end
         end
      end
      while (m_global_attrs.get_cur_phase() < ShutdownPhase) begin
         #0 cur_phase = m_global_attrs.get_cur_phase();
         if (m_system_agent != null) begin
            m_system_agent.wait_cycles(20);
         end
         if (m_global_attrs.get_cur_phase() == cur_phase) begin
            if ((cur_phase < ShutdownPhase) && (m_uvm_phases.exists(cur_phase))) begin
               uvm_objection phase_objection;

               `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("killing %p", cur_phase))
               phase_objection = m_uvm_phases[cur_phase].get_objection();
               if (phase_objection != null) begin
                  phase_objection.clear(null);
               end
               m_global_attrs.wait_for_end_of_phase(cur_phase);
            end
         end
      end
      while (m_global_attrs.get_cur_phase() < PostShutdownPhase) begin
         #0 cur_phase = m_global_attrs.get_cur_phase();
         if (m_system_agent != null) begin
            m_system_agent.wait_cycles(20);
         end
         if (m_global_attrs.get_cur_phase() == cur_phase) begin
            if ((cur_phase < PostShutdownPhase) && (m_uvm_phases.exists(cur_phase))) begin
               uvm_objection phase_objection;

               `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("killing %p", cur_phase))
               phase_objection = m_uvm_phases[cur_phase].get_objection();
               if (phase_objection != null) begin
                  phase_objection.clear(null);
               end
               m_global_attrs.wait_for_end_of_phase(cur_phase);
            end
         end
      end
      while (m_global_attrs.get_cur_phase() == ShutdownPhase) begin
         #0 cur_phase = m_global_attrs.get_cur_phase();
         if (m_system_agent != null) begin
            m_system_agent.wait_cycles(1);
         end
         if (m_global_attrs.get_cur_phase() == cur_phase) begin
            if ((cur_phase == ShutdownPhase) && (m_uvm_phases.exists(cur_phase))) begin
               uvm_objection phase_objection;

               `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("killing %p", cur_phase))
               phase_objection = m_uvm_phases[cur_phase].get_objection();
               if (phase_objection != null) begin
                  phase_objection.clear(null);
               end
               m_global_attrs.wait_for_end_of_phase(cur_phase);
            end
         end
      end
   endtask : watch_for_errors


   //-----------------------------------------------------------------------------------------------
   //
   task handle_timeout(input evl_phase_type_t phase_type_in, input bit force_max_error_count = 1'b1, input int timeout_count_in = 0);
      int timeout_count;

      if (m_global_attrs.get_error_count() > 0) begin
         return;
      end
      m_global_attrs.wait_for_start_of_phase(phase_type_in);
      timeout_count = (timeout_count_in > 0) ? timeout_count_in : m_global_attrs.get_timeout_value(phase_type_in);
      if (force_max_error_count == 1'b1) begin
         m_global_attrs.set_max_error_count(1);
      end
      if (m_global_attrs.get_error_count() > 0) begin
         return;
      end
      if (m_system_agent == null) begin
         `dut_error(get_abstract_name(), "unable to find the system agent, so no end of simulation timeout checks can be performed")
         return;
      end
      fork
         begin :timeout_block
           `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "", $sformatf("handle_timeout() is starting timeout timer = %0d clock cycles", timeout_count))
            m_system_agent.wait_cycles(timeout_count * 2);
            if ((m_global_attrs.get_cur_phase() == phase_type_in) && (m_global_attrs.get_error_count() <= 0)) begin
               `dut_error(get_abstract_name(), $sformatf("simulation timed out in %p (%0d cycle%0s)", phase_type_in, timeout_count, ((timeout_count == 1) ? "" : "s")))
            end
            disable fork;
         end
         begin :phase_block
            m_global_attrs.wait_for_end_of_phase(phase_type_in);
            disable fork;
         end
         begin :error_block
            m_global_attrs.wait_for_error_finish();
            disable fork;
         end
      join
   endtask : handle_timeout


   //-----------------------------------------------------------------------------------------------
   //
   // Function: record_start_of_test
   //
   virtual function void record_start_of_test();
      m_start_time = `evl_tcl_get_time_of_day;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      string           agent_type_name;
      evl_agent_type_t agent_type;

      record_start_of_test();  // Saves the time of test starting to be used later for performance calculation
      m_cur_time = `evl_tcl_get_time_of_day;

      super.build_phase(phase);
      agent_type = agent_type.first();
      while (agent_type != agent_type.last()) begin
         agent_type_name = $sformatf("%p", agent_type);
         m_agent_type_xrefs[agent_type_name] = agent_type;
         agent_type = agent_type.next();
      end
      agent_type_name = $sformatf("%p", agent_type);
      m_agent_type_xrefs[agent_type_name] = agent_type;
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      longint new_time;
      longint time_diff;

      new_time   = `evl_tcl_get_time_of_day;
      time_diff  = new_time - m_cur_time;
      m_cur_time = new_time;
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Current phase is ConnectPhase (%0d second%0s, %0s)", time_diff, ((time_diff == 1) ? "" : "s"), `evl_tcl_get_hhmmss))
      super.connect_phase(phase);
      foreach (m_verif_comp_list[ii]) begin
         if (m_verif_comp_list[ii].get_vc_parent() == null) begin
            m_verif_comp_list[ii].assign_alias_ids();
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: end_of_elaboration_phase
   //
   virtual function void end_of_elaboration_phase(uvm_phase phase);
      longint new_time;
      longint time_diff;

      new_time   = `evl_tcl_get_time_of_day;
      time_diff  = new_time - m_cur_time;
      m_cur_time = new_time;
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Current phase is EndOfElaborationPhase (%0d second%0s, %0s)", time_diff, ((time_diff == 1) ? "" : "s"), `evl_tcl_get_hhmmss))
      super.end_of_elaboration_phase(phase);
   endfunction : end_of_elaboration_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      evl_verif_comp verif_comp_list[$];
      string         debug_file_name;
      longint        new_time;
      longint        time_diff;

      new_time   = `evl_tcl_get_time_of_day;
      time_diff  = new_time - m_cur_time;
      m_cur_time = new_time;
      //
      // Cause one final sort of the agent list
      //
      add_me_to_verif_comp_list(null);
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Current phase is StartOfSimulationPhase (%0d second%0s, %0s)", time_diff, ((time_diff == 1) ? "" : "s"), `evl_tcl_get_hhmmss))
      super.start_of_simulation_phase(phase);
      foreach (m_verif_comp_list[ii]) begin
         m_verif_comp_list[ii].disable_delays(`EVL_PHASE_DISABLE);
         m_verif_comp_list[ii].disable_stimulus(`EVL_PHASE_DISABLE);
         m_verif_comp_list[ii].enable_delays(`EVL_GLOBAL_DISABLE);
         m_verif_comp_list[ii].enable_stimulus(`EVL_GLOBAL_DISABLE);
      end
      foreach (m_verif_comp_list[ii]) begin
         m_verif_comp_list[ii].reorder_children();
      end
      `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
      `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "Verification Component Hierarchy")
      `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "================================")
      foreach (m_verif_comp_list[ii]) begin
         if (m_verif_comp_list[ii].get_vc_parent() == null) begin
            m_verif_comp_list[ii].print_hierarchy("");
         end
      end
      `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
      if (`evl_log_check_verbosity(UVM_HIGH, VMOD_TEST) != 0) begin
         foreach (m_verif_comp_list[ii]) begin
            if (m_verif_comp_list[ii].get_vc_parent() == null) begin
               m_verif_comp_list[ii].print_delays();
            end
         end
      end

      //
      // Reorder the verif_comp_list so that unspecified shires, neighborhoods, and cores are first
      // in line (this allows searches to work as expected).
      //
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                       != null)                       &&
              (m_verif_comp_list[ii].get_shire_id()        == ~evl_shire_id_t'(0))        &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) &&
              (m_verif_comp_list[ii].get_core_id()         == ~evl_core_id_t'(0))         &&
              (m_verif_comp_list[ii].get_agent_id()        == ~evl_agent_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                       != null)                       &&
              (m_verif_comp_list[ii].get_shire_id()        == ~evl_shire_id_t'(0))        &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) &&
              (m_verif_comp_list[ii].get_core_id()         == ~evl_core_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      for (int ii = m_verif_comp_list.size(); ii >= 0; ii--) begin
         if ( (m_verif_comp_list[ii]                       != null)                &&
              (m_verif_comp_list[ii].get_shire_id()        == ~evl_shire_id_t'(0)) &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      for (int ii = m_verif_comp_list.size(); ii >= 0; ii--) begin
         if ( (m_verif_comp_list[ii]                != null) &&
              (m_verif_comp_list[ii].get_shire_id() == ~evl_shire_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                       != null)                       &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) &&
              (m_verif_comp_list[ii].get_core_id()         == ~evl_core_id_t'(0))         &&
              (m_verif_comp_list[ii].get_agent_id()        == ~evl_agent_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                       != null)                       &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) &&
              (m_verif_comp_list[ii].get_core_id()         == ~evl_core_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                       != null) &&
              (m_verif_comp_list[ii].get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]                != null)               &&
              (m_verif_comp_list[ii].get_core_id()  == ~evl_core_id_t'(0)) &&
              (m_verif_comp_list[ii].get_agent_id() == ~evl_agent_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if ( (m_verif_comp_list[ii]               != null) &&
              (m_verif_comp_list[ii].get_core_id() == ~evl_core_id_t'(0)) ) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      foreach (m_verif_comp_list[ii]) begin
         if (m_verif_comp_list[ii] != null) begin
            verif_comp_list.push_back(m_verif_comp_list[ii]);
            m_verif_comp_list[ii] = null;
         end
      end
      m_verif_comp_list = {};
      foreach (verif_comp_list[ii]) begin
         m_verif_comp_list.push_back(verif_comp_list[ii]);
      end

      debug_file_name = m_global_attrs.get_plus_arg_value("globalDebug");
      if (debug_file_name == "") begin
         debug_file_name = "globalDebug.tcl";
      end
      debug_file_name    = evl_tcl_find_file(debug_file_name, 0);
      m_global_tcl_scipt = new(get_abstract_name());
      if ((debug_file_name != "") && (m_system_agent != null)) begin
         `evl_log_no_addr(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("processing \"%s\"", debug_file_name))
         m_global_tcl_scipt.set_agent(m_system_agent, 0);
         m_global_tcl_scipt.set_file_name(debug_file_name);
         m_global_tcl_scipt.start_script();
      end
      m_rtl_port = m_system_agent.get_rtl_port();
      if (`et_value_plusargs("ust_start_phase=%s", m_ust_start_phase) == 0) begin
         m_ust_start_phase = "";
      end
      else begin
         if (m_ust_start_phase == "PreResetPhase") begin
            m_ust_start_phase = "pre_reset_phase";
         end
         else if (m_ust_start_phase == "ResetPhase") begin
            m_ust_start_phase = "reset_phase";
         end
         else if (m_ust_start_phase == "PostResetPhase") begin
            m_ust_start_phase = "post_reset_phase";
         end
         else if (m_ust_start_phase == "PreConfigurePhase") begin
            m_ust_start_phase = "pre_configure_phase";
         end
         else if (m_ust_start_phase == "ConfigurePhase") begin
            m_ust_start_phase = "configure_phase";
         end
         else if (m_ust_start_phase == "PostConfigurePhase") begin
            m_ust_start_phase = "post_configure_phase";
         end
         else if (m_ust_start_phase == "PreMainPhase") begin
            m_ust_start_phase = "pre_main_phase";
         end
         else if (m_ust_start_phase == "MainPhase") begin
            m_ust_start_phase = "main_phase";
         end
         else begin
            m_ust_start_phase = "";
         end
         if (m_ust_start_phase != "") begin
            m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 0);
         end
      end
      m_global_attrs.set_config_done();
      if (get_verif_comp_local(CORE_RTL_AGENT) == null) begin
         m_global_attrs.cosim_clear();
      end
      fork
         start_sequences();
         watch_for_errors();
         if (m_disable_configure_timeout == 1'b0) begin
            handle_timeout(ConfigurePhase);
         end
         if (m_disable_shutdown_timeout == 1'b0) begin
            handle_timeout(ShutdownPhase);
         end
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PreResetPhase] = phase;
      m_cur_uvm_phase             = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "pre_reset_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.pre_reset_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            repeat (5) begin
               #0 m_start_sequences = m_start_sequences & 1'b1;
            end
            #0 m_start_sequences = 1'b1;
            foreach (m_verif_comp_list[ii]) begin
               m_verif_comp_list[ii].enable_stimulus(`EVL_PHASE_DISABLE);
            end
            m_global_attrs.set_cur_phase(PreResetPhase, phase, m_cur_time);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[ResetPhase] = phase;
      m_cur_uvm_phase          = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "reset_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.reset_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("reset_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(ResetPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PostResetPhase] = phase;
      m_cur_uvm_phase              = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "post_reset_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.post_reset_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("post_reset_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(PostResetPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(90);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PreConfigurePhase] = phase;
      m_cur_uvm_phase                 = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "pre_configure_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.pre_configure_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("pre_configure_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(PreConfigurePhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[ConfigurePhase] = phase;
      m_cur_uvm_phase              = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "configure_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.configure_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("configure_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(ConfigurePhase, phase);
            if (m_system_agent != null) begin
               if (get_verif_comp_local(CORE_RTL_AGENT) != null) begin
                  m_system_agent.wait_cycles(200);
               end
               else begin
                  m_system_agent.wait_cycles(9);
               end
               m_global_attrs.wait_for_all_threads_booted();
               m_system_agent.wait_cycles(1);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PostConfigurePhase] = phase;
      m_cur_uvm_phase                  = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "post_configure_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.post_configure_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("post_configure_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(PostConfigurePhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               //
               // The Ultrasoc USB link is started in the post-configure phase.  We need to wait
               // until the high-speed link is set up before continuing (see the code in
               // ust_dpi_if_m.v).
               //
               if (m_global_attrs.get_int_param_by_name("pk_ultrasoc_hs_link_not_done", 0) != 0) begin
                  fork
                     begin :ust_link_init_start_wait
                        `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("waiting for the Ultrasoc USB link initialization to start"))
                        m_global_attrs.wait_for_int_param_by_name("pk_ultrasoc_hs_link_not_done", 0, 1);
                        disable ust_link_init_start_timeout;
                        `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("done waiting for the Ultrasoc USB link initialization to start"))
                     end
                     begin :ust_link_init_start_timeout
                        m_system_agent.wait_cycles(4000000);
                        disable ust_link_init_start_wait;
                        `dut_error(get_abstract_name(), "timed out waiting for the Ultrasoc USB link initialization to start (you may want to rerun with +ust_link_no_init_delay)")
                     end
                  join
                  if (m_global_attrs.get_int_param_by_name("pk_ultrasoc_hs_link_not_done", 0) != 0) begin
                     fork
                        begin :ust_link_init_done_wait
                           `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("waiting for the Ultrasoc USB link initialization to complete"))
                           m_global_attrs.wait_for_int_param_by_name("pk_ultrasoc_hs_link_not_done", 0, 0);
                           disable ust_link_init_done_timeout;
                           `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("done waiting for the Ultrasoc USB link initialization to complete"))
                        end
                        begin :ust_link_init_done_timeout
                           m_system_agent.wait_cycles(4000000);
                           //m_system_agent.wait_cycles(20000);
                           disable ust_link_init_done_wait;
                           `dut_error(get_abstract_name(), "timed out waiting for the Ultrasoc USB link initialization to complete (you may want to rerun with +ust_link_no_delay)")
                        end
                     join
                  end
               end
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PreMainPhase] = phase;
      m_cur_uvm_phase            = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "pre_main_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.pre_main_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("pre_main_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(PreMainPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[MainPhase] = phase;
      m_cur_uvm_phase         = phase;
      objection = phase.get_objection();
      if (m_ust_start_phase == "main_phase") begin
         m_system_agent.set_rtl_int_param_by_name("pk_enable_ust_dpi_drv", 1);
      end
      fork
         begin
            super.main_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("main_phase", phase, get_abstract_name());
            foreach (m_verif_comp_list[ii]) begin
               m_verif_comp_list[ii].enable_delays(`EVL_PHASE_DISABLE);
            end
            #0;
            m_global_attrs.set_cur_phase(MainPhase, phase);
            if (m_global_attrs.get_thread_count() > 0) begin
               set_terminating_sequence_local();
            end
            fork
               begin :normal_path
                  fork
                     begin
                        #0 m_global_attrs.wait_for_all_threads_done();
                     end
                     `ifdef ENABLE_BEMU_OR_ZSIM
                        begin
                           #0 m_global_attrs.wait_for_cosim_end();
                           m_system_agent.wait_cycles(200);
                        end
                     `endif // ifdef ENABLE_BEMU_OR_ZSIM
                  join_any
                  disable err_path;
               end
               begin :err_path
                  #0 m_global_attrs.wait_for_error_finish();
                  disable normal_path;
               end
            join
            if (m_system_agent != null) begin
               #0 if (m_terminating_seq == 1'b0) begin
                  if (`et_value_plusargs("cycleCount=%d", m_max_cycle_count) == 0) begin
                     m_max_cycle_count = `EVL_MAX_CYCLE_COUNT;
                  end
                  if (m_max_cycle_count < 10) begin
                     m_max_cycle_count = 10;
                  end
                  `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("main_phase() is starting timeout counter for %0d clock cycles", m_max_cycle_count))
                  m_system_agent.wait_cycles(m_max_cycle_count);
               end
               else begin
                  m_system_agent.wait_cycles(10);
               end
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PostMainPhase] = phase;
      m_cur_uvm_phase             = phase;
      objection = phase.get_objection();
      fork
         begin
            super.post_main_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("post_main_phase", phase, get_abstract_name());
            foreach (m_verif_comp_list[ii]) begin
               m_verif_comp_list[ii].disable_delays(`EVL_PHASE_DISABLE);
            end
            #0;
            m_global_attrs.set_cur_phase(PostMainPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(10);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PreShutdownPhase] = phase;
      m_cur_uvm_phase                = phase;
      objection = phase.get_objection();
      fork
         begin
            super.pre_shutdown_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("pre_shutdown_phase", phase, get_abstract_name());
            foreach (m_verif_comp_list[ii]) begin
               m_verif_comp_list[ii].disable_stimulus(`EVL_PHASE_DISABLE);
            end
            #0;
            m_global_attrs.set_cur_phase(PreShutdownPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(9);
               m_global_attrs.wait_for_ultrasoc();
               m_system_agent.wait_cycles(1);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[ShutdownPhase] = phase;
      m_cur_uvm_phase             = phase;
      objection = phase.get_objection();
      fork
         begin
            super.shutdown_phase(phase);
         end
         begin
            if (m_system_agent != null) begin
               phase.raise_objection(this);
            end
            m_global_attrs.raise_objection("shutdown_phase", phase, get_abstract_name());
            #0;
            m_global_attrs.set_cur_phase(ShutdownPhase, phase);
            if (m_system_agent != null) begin
               if (`et_value_plusargs("shutdownCycleCount=%d", m_max_shutdown_cycle_count) == 0) begin
                  m_max_shutdown_cycle_count = 10;
               end
               if (m_max_shutdown_cycle_count < 10) begin
                  m_max_shutdown_cycle_count = 10;
               end
               `evl_log(UVM_NONE, VMOD_GLOBAL, "", $sformatf("shutdown_phase() is starting timeout counter for %0d clock cycles", m_max_shutdown_cycle_count))
               m_system_agent.wait_cycles(m_max_shutdown_cycle_count);
               phase.drop_objection(this);
            end
            else begin
               repeat (5) begin
                  #0 m_start_sequences = m_start_sequences | 1'b1;
               end
            end
         end
      join
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      uvm_objection objection;

      m_uvm_phases[PostShutdownPhase] = phase;
      m_cur_uvm_phase                 = phase;
      phase.raise_objection(this);
      m_global_attrs.raise_objection("post_shutdown_phase", phase, get_abstract_name());
      objection = phase.get_objection();
      fork
         begin
            super.post_shutdown_phase(phase);
         end
         begin
            #0 m_global_attrs.set_cur_phase(PostShutdownPhase, phase);
            if (m_system_agent != null) begin
               m_system_agent.wait_cycles(20);
            end
         end
      join
      //
      // If we were supposed to run an Ultrasoc test but Ultrasoc was not built, then flag an error.
      //
      if (m_global_attrs.get_int_param_by_name("pk_ultrasoc_hs_link_not_done", -1) < 0) begin
         if ((`et_test_plusargs("py_test=") != 0) && (`et_test_plusargs("py_start_script=") != 0) && (`et_test_plusargs("py_err_if_missing") != 0)) begin
            `dut_error(get_abstract_name(), "an Ultrasoc test was supposed to start, but it never did ... is the build compiled correctly?")
         end
      end
      repeat (5) begin
         #0 m_start_sequences = m_start_sequences | 1'b1;
      end
      #0 `evl_quiescent_check = m_quiescent_value;
      #0.001ns phase.drop_objection(this);
      repeat (5) begin
         #0 m_start_sequences = m_start_sequences | 1'b1;
      end
      if (m_display_objections == 1'b1) begin
         #0 objection.display_objections();
      end
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: extract_phase
   //
   virtual function void extract_phase(uvm_phase phase);
      super.extract_phase(phase);
      m_global_attrs.set_cur_phase(ExtractPhase, phase);
   endfunction : extract_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_phase
   //
   virtual function void check_phase(uvm_phase phase);
      super.check_phase(phase);
      m_global_attrs.set_cur_phase(CheckPhase, phase);
      fork
         begin
            `evl_sim_complete = 1'b1;
            #0;
            `cosim_object_delete
         end
      join_none
   endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: report_phase
   //
   virtual function void report_phase(uvm_phase phase);
      super.report_phase(phase);
      m_global_attrs.set_cur_phase(ReportPhase, phase);
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: final_phase
   //
   virtual function void final_phase(uvm_phase phase);
      super.final_phase(phase);
      m_global_attrs.set_cur_phase(FinalPhase, phase);
      assert_final_phase_local();
   endfunction : final_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_sim_performance
   //
   virtual function void print_sim_performance();
      int cycle_count;

      cycle_count         = m_system_agent.get_cycle_count();
      m_end_time          = `evl_tcl_get_time_of_day;
      m_duration_time     = m_end_time - m_start_time;
      m_cycles_per_second = cycle_count / m_duration_time;

      `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
      `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("Duration (walltime) in seconds: %0d (%0d)", m_duration_time, m_end_time))
      `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("Duration in clock cycles      : %0d", cycle_count))
      if (m_cycles_per_second == 0) begin
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("Simulation *seconds/cycle*    : %0d", m_duration_time / cycle_count))
      end
      else begin
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("Simulation cycles/second      : %0d", m_cycles_per_second))
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Task: final_phase_task
   //
   task final_phase_task();
      wait (m_start_final_phase == 1'b1);
      if (m_system_agent != null)  begin
         m_system_agent.wait_zero_time(10);
      end
      else begin
         repeat (10) begin
            #0 m_final_count = m_final_count + 1;
         end
      end
      if (m_fast_finish == 1'b0) begin
         m_global_attrs.print_all_trans_descs();
         print_sim_performance();
         `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
         `evl_certify_plusargs
      end
      if (m_global_attrs.get_total_error_count() == 0) begin
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAS PASSED *** \\    /\\ "))
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAS PASSED ***  )  ( ')  "))
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAS PASSED *** (  /  )   "))
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAS PASSED ***  \\(__)|  "))
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("***   PURRRRRFECT!  ***           "))
      end
      else begin
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAS FAILED (DUT Error count: %0d, UVM_ERROR count: %0d) ***", m_global_attrs.get_error_count(), m_global_attrs.get_uvm_error_count()))
      end
      if (m_global_attrs.get_total_warning_count() > 0) begin
         `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("*** TEST HAD WARNINGS (DUT Warning count: %0d, UVM_WARNING count: %0d) ***", m_global_attrs.get_warning_count(), m_global_attrs.get_uvm_warning_count()))
      end
      `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")

      #0 m_global_attrs.close_dump();
      #0 `et_close_dpi_event_files;
      `ifdef EVL_TOP_LEVEL_HAS_FINISH
         #0 m_rtl_port.set_int_param_by_name("pk_assert_finish", 1);
      `else // ifdef EVL_TOP_LEVEL_HAS_FINISH
         #0 $finish;
      `endif // !ifdef EVL_TOP_LEVEL_HAS_FINISH
   endtask : final_phase_task


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_max_cycle_count
   //
   static function int get_max_cycle_count();
      return m_max_cycle_count;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_verif_comp_map::get_cycles_until_max_cycle_count
   //
   static function int get_cycles_until_max_cycle_count();
      if (m_system_agent == null) begin
         return 0;
      end
      else begin
         if (m_system_agent.get_cycle_count() > m_max_cycle_count) begin
            return 0;
         end
         else begin
            return m_max_cycle_count - m_system_agent.get_cycle_count();
         end
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new(input string name_in = "evl_verif_comp_map", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("VerifCompMap");
      if (m_verif_comp_map == null) begin
         m_verif_comp_map = this;
         void'(evl_base_pkg::evl_log_create());
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("created verification component map (id %0d)", get_inst_id()))
      `ifdef EVL_DISABLE_GLOBAL_TIMEOUT_CHECK
         m_disable_shutdown_timeout = 1'b0;
      `else // ifdef EVL_DISABLE_GLOBAL_TIMEOUT_CHECK
         m_disable_shutdown_timeout = 1'b1;
      `endif // !ifndef EVL_DISABLE_GLOBAL_TIMEOUT_CHECK
      m_disable_configure_timeout = 1'b1;
      if (`et_test_plusargs("disable_configure_timeout") != 0) begin
         m_disable_configure_timeout = 1'b1;
      end
      if (`et_test_plusargs("enable_configure_timeout") != 0) begin
         m_disable_configure_timeout = 1'b0;
      end
      if (`et_test_plusargs("disable_shutdown_timeout") != 0) begin
         m_disable_shutdown_timeout = 1'b1;
      end
      if (`et_test_plusargs("enable_shutdown_timeout") != 0) begin
         m_disable_shutdown_timeout = 1'b0;
      end
      if (`et_test_plusargs("displayObjections") != 0) begin
         m_display_objections = 1'b1;
      end
      if (`et_test_plusargs("display_objections") != 0) begin
         m_display_objections = 1'b1;
      end
      if (`et_value_plusargs("enable_quiescent_check=%d", m_quiescent_value) == 0) begin
         if (`et_test_plusargs("enable_quiescent_check") != 0) begin
            m_quiescent_value = 1;
         end
         else begin
            `ifdef EVL_ENABLE_QUIESCENT_CHECK
               m_quiescent_value = 1;
            `else
               m_quiescent_value = 0;
            `endif
         end
      end
      if (`et_test_plusargs("final_fifo_check") != 0) begin
         m_quiescent_value |= `QUIESCENT_FIFO_MASK;
      end
      m_global_attrs.m_disable_configure_timeout = m_disable_configure_timeout;
      fork
         final_phase_task();
      join_none
   endfunction : new

endclass : evl_verif_comp_map

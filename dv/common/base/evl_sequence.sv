//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence
//
virtual class evl_sequence #(type REQ = evl_sequence_item, type RSP = REQ) extends uvm_sequence#(REQ, RSP);

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_stimulus_is_disabled: This field is a bit map of stimulus disables.  For example, it is
   //                         possible for stimulus to be both globally disabled and disabled due to
   //                         the current phase ... subsequent release of either the global or phase
   //                         disable will still leave the other type of disable intact, allowing a
   //                         way to disable delays without fear of having them enabled by another
   //                         code fragment.
   //
   evl_sequence_wrapper #(REQ, RSP) m_sequence_wrapper           = null;
   evl_verif_comp                   m_agent                      = null;
   int                              m_started                    = 0;
   int                              m_stimulus_is_disabled       = 0;
   int                              m_weight                     = 1;
   bit                              m_background_seq             = 1'b1;
   bit                              m_start_pre_reset_phase      = 1'b0;
   bit                              m_start_reset_phase          = 1'b0;
   bit                              m_start_post_reset_phase     = 1'b0;
   bit                              m_start_pre_configure_phase  = 1'b0;
   bit                              m_start_configure_phase      = 1'b0;
   bit                              m_start_post_configure_phase = 1'b0;
   bit                              m_start_pre_main_phase       = 1'b0;
   bit                              m_start_main_phase           = 1'b0;
   bit                              m_start_post_main_phase      = 1'b0;
   bit                              m_start_pre_shutdown_phase   = 1'b0;
   bit                              m_start_shutdown_phase       = 1'b0;
   bit                              m_start_post_shutdown_phase  = 1'b0;
   uvm_phase                        m_phase_for_pre_reset        = null;
   uvm_phase                        m_phase_for_reset            = null;
   uvm_phase                        m_phase_for_post_reset       = null;
   uvm_phase                        m_phase_for_pre_configure    = null;
   uvm_phase                        m_phase_for_configure        = null;
   uvm_phase                        m_phase_for_post_configure   = null;
   uvm_phase                        m_phase_for_pre_main         = null;
   uvm_phase                        m_phase_for_main             = null;
   uvm_phase                        m_phase_for_post_main        = null;
   uvm_phase                        m_phase_for_pre_shutdown     = null;
   uvm_phase                        m_phase_for_shutdown         = null;
   uvm_phase                        m_phase_for_post_shutdown    = null;
   evl_sequencer_wrapper_base       m_sequencer_wrappers[$];
   int                              m_seq_groups[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_wrapper
   //
   virtual function evl_sequence_wrapper_base get_wrapper();
      return m_sequence_wrapper;
   endfunction : get_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      m_agent = agent_in;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   virtual function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_me_on_ice
   //
   virtual function void put_me_on_ice();
      foreach (m_sequencer_wrappers[ii]) begin
         m_sequencer_wrappers[ii].put_sequence_wrapper_on_ice(m_sequence_wrapper);
      end
   endfunction : put_me_on_ice


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reheat_me
   //
   virtual function void reheat_me();
      foreach (m_sequencer_wrappers[ii]) begin
         m_sequencer_wrappers[ii].reheat_sequence_wrapper(m_sequence_wrapper);
      end
   endfunction : reheat_me


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_stimulus
   //
   function void enable_stimulus(input int disable_type = `EVL_MASTER_DISABLE);
      if (((m_stimulus_is_disabled & `EVL_ON_ICE_DISABLE) != 0) && ((disable_type & `EVL_ON_ICE_DISABLE) != 0)) begin
         reheat_me();
      end
      m_stimulus_is_disabled &= ~disable_type;
   endfunction : enable_stimulus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_stimulus
   //
   function void disable_stimulus(input int disable_type = `EVL_MASTER_DISABLE);
      if (((m_stimulus_is_disabled & `EVL_ON_ICE_DISABLE) == 0) && ((disable_type & `EVL_ON_ICE_DISABLE) != 0)) begin
         put_me_on_ice();
      end
      m_stimulus_is_disabled |= disable_type;
   endfunction : disable_stimulus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: stimulus_is_enabled
   //
   function bit stimulus_is_enabled();
      return ((m_stimulus_is_disabled == 0) ? 1'b1 : 1'b0);
   endfunction : stimulus_is_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: stimulus_is_disabled
   //
   function bit stimulus_is_disabled();
      return ((m_stimulus_is_disabled == 0) ? 1'b0 : 1'b1);
   endfunction : stimulus_is_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_stimulus_enable
   //
   task wait_for_stimulus_enable();
      if (m_stimulus_is_disabled != 0) begin
         #0 wait (m_stimulus_is_disabled == 0);
      end
   endtask : wait_for_stimulus_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_stimulus_disable
   //
   task wait_for_stimulus_disable();
      if (m_stimulus_is_disabled == 0) begin
         #0 wait (m_stimulus_is_disabled != 0);
      end
   endtask : wait_for_stimulus_disable


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequencer_wrapper
   //
   virtual function void add_sequencer_wrapper(input evl_sequencer_wrapper_base sequencer_wrapper_in);
      if ((sequencer_wrapper_in != null) && (!(sequencer_wrapper_in inside { m_sequencer_wrappers }))) begin
         m_sequencer_wrappers.push_back(sequencer_wrapper_in);
      end
   endfunction : add_sequencer_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_weight
   //
   virtual function void set_weight(input int weight_in);
      m_weight = (weight_in < 0) ? 0 : ((weight_in > 10) ? 10 : weight_in);
   endfunction : set_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_weight
   //
   virtual function int get_weight();
      return m_weight;
   endfunction : get_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_seq_group
   //
   function void add_seq_group(input int seq_group_in);
      if (!(seq_group_in inside { m_seq_groups })) begin
         m_seq_groups.push_back(seq_group_in);
      end
   endfunction : add_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_seq_group_list
   //
   function int get_seq_group_list(output int seq_group_list[]);
      if (m_seq_groups.size() == 0) begin
         seq_group_list = {};
         return 0;
      end
      seq_group_list = new[m_seq_groups.size()];
      foreach (m_seq_groups[ii]) begin
         seq_group_list[ii] = m_seq_groups[ii];
      end
      return m_seq_groups.size();
   endfunction : get_seq_group_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_for_seq_group
   //
   function bit use_for_seq_group(input int seq_group_in);
      if (m_seq_groups.size() == 0) begin
         return 1'b1;
      end
      if (seq_group_in inside { m_seq_groups }) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : use_for_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_file_path_list
   //
   function int get_full_file_path_list(output string         file_name_list_out[],
                                        input  string         default_file_name,
                                        input  string         plus_arg          = "",
                                        input  evl_verif_comp agent_in          = null,
                                        input  int            use_include_paths = 1,
                                        input  bit            notify_on_fail    = 1'b0);
      string file_name_list[$];
      string file_name = "";

      file_name_list = {};
      if (plus_arg != "") begin
         string all_file_names;

         if (agent_in != null) begin
            all_file_names = agent_in.get_plus_arg_value(plus_arg);
         end
         else begin
            all_file_names = m_global_attrs.get_plus_arg_value(plus_arg);
         end
         if (all_file_names != "") begin
            int start_idx = 0;
            int limit     = all_file_names.len();

            for (int ii = 0; ii < limit; ii++) begin
               if (all_file_names[ii] == "+") begin
                  if (ii > start_idx) begin
                     file_name = evl_tcl_find_file(all_file_names.substr(start_idx, ii - 1), use_include_paths);
                     if (file_name != "") begin
                        file_name_list.push_back(file_name);
                     end
                  end
                  start_idx = ii + 1;
               end
            end
            if (limit > start_idx) begin
               file_name = evl_tcl_find_file(all_file_names.substr(start_idx, limit - 1), use_include_paths);
               if (file_name != "") begin
                  file_name_list.push_back(file_name);
               end
            end
         end
      end
      if ((file_name_list.size() == 0) && (default_file_name != "")) begin
         file_name = evl_tcl_find_file(default_file_name, use_include_paths);
         if (file_name != "") begin
            file_name_list.push_back(file_name);
         end
      end
      if (file_name_list.size() == 0) begin
         file_name_list_out    = new[1];
         file_name_list_out[0] = "";
         return 0;
      end
      file_name_list_out = new[file_name_list.size()];
      for (int ii = 0; ii < file_name_list.size(); ii++) begin
         file_name_list_out[ii] = file_name_list[ii];
      end
      return file_name_list_out.size();
   endfunction : get_full_file_path_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_file_path
   //
   function string get_full_file_path(input string         default_file_name,
                                      input string         plus_arg          = "",
                                      input evl_verif_comp agent_in          = null,
                                      input int            use_include_paths = 1);
      string file_name = "";

      if (plus_arg != "") begin
         if (agent_in != null) begin
            file_name = evl_tcl_find_file(agent_in.get_plus_arg_value(plus_arg), use_include_paths);
         end
         else begin
            file_name = evl_tcl_find_file(m_global_attrs.get_plus_arg_value(plus_arg), use_include_paths);
         end
         if (file_name != "") begin
            return file_name;
         end
      end
      if (default_file_name != "") begin
         file_name = evl_tcl_find_file(default_file_name, use_include_paths);
      end
      return file_name;
   endfunction : get_full_file_path


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_pre_reset_phase
   //
   protected function void start_pre_reset_phase(input uvm_phase phase);
      m_phase_for_pre_reset   = phase;
      m_start_pre_reset_phase = 1'b1;
   endfunction : start_pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_reset_phase
   //
   protected function void start_reset_phase(input uvm_phase phase);
      m_phase_for_reset   = phase;
      m_start_reset_phase = 1'b1;
   endfunction : start_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_post_reset_phase
   //
   protected function void start_post_reset_phase(input uvm_phase phase);
      m_phase_for_post_reset   = phase;
      m_start_post_reset_phase = 1'b1;
   endfunction : start_post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_pre_configure_phase
   //
   protected function void start_pre_configure_phase(input uvm_phase phase);
      m_phase_for_pre_configure   = phase;
      m_start_pre_configure_phase = 1'b1;
   endfunction : start_pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_configure_phase
   //
   protected function void start_configure_phase(input uvm_phase phase);
      m_phase_for_configure   = phase;
      m_start_configure_phase = 1'b1;
   endfunction : start_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_post_configure_phase
   //
   protected function void start_post_configure_phase(input uvm_phase phase);
      m_phase_for_post_configure   = phase;
      m_start_post_configure_phase = 1'b1;
   endfunction : start_post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_pre_main_phase
   //
   protected function void start_pre_main_phase(input uvm_phase phase);
      m_phase_for_pre_main   = phase;
      m_start_pre_main_phase = 1'b1;
   endfunction : start_pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_main_phase
   //
   protected function void start_main_phase(input uvm_phase phase);
      m_phase_for_main   = phase;
      m_start_main_phase = 1'b1;
   endfunction : start_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_post_main_phase
   //
   protected function void start_post_main_phase(input uvm_phase phase);
      m_phase_for_post_main   = phase;
      m_start_post_main_phase = 1'b1;
   endfunction : start_post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_pre_shutdown_phase
   //
   protected function void start_pre_shutdown_phase(input uvm_phase phase);
      m_phase_for_pre_shutdown   = phase;
      m_start_pre_shutdown_phase = 1'b1;
   endfunction : start_pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_shutdown_phase
   //
   protected function void start_shutdown_phase(input uvm_phase phase);
      m_phase_for_shutdown   = phase;
      m_start_shutdown_phase = 1'b1;
   endfunction : start_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_post_shutdown_phase
   //
   protected function void start_post_shutdown_phase(input uvm_phase phase);
      m_phase_for_post_shutdown   = phase;
      m_start_post_shutdown_phase = 1'b1;
   endfunction : start_post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pre_reset_phase
   //
   protected task watch_pre_reset_phase();
      #0 wait (m_start_pre_reset_phase == 1'b1);
      pre_reset_phase(m_phase_for_pre_reset);
   endtask : watch_pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_reset_phase
   //
   protected task watch_reset_phase();
      #0 wait (m_start_reset_phase == 1'b1);
      reset_phase(m_phase_for_reset);
   endtask : watch_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_post_reset_phase
   //
   protected task watch_post_reset_phase();
      #0 wait (m_start_post_reset_phase == 1'b1);
      post_reset_phase(m_phase_for_post_reset);
   endtask : watch_post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pre_configure_phase
   //
   protected task watch_pre_configure_phase();
      #0 wait (m_start_pre_configure_phase == 1'b1);
      pre_configure_phase(m_phase_for_pre_configure);
   endtask : watch_pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_configure_phase
   //
   protected task watch_configure_phase();
      #0 wait (m_start_configure_phase == 1'b1);
      configure_phase(m_phase_for_configure);
   endtask : watch_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_post_configure_phase
   //
   protected task watch_post_configure_phase();
      #0 wait (m_start_post_configure_phase == 1'b1);
      post_configure_phase(m_phase_for_post_configure);
   endtask : watch_post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pre_main_phase
   //
   protected task watch_pre_main_phase();
      #0 wait (m_start_pre_main_phase == 1'b1);
      pre_main_phase(m_phase_for_pre_main);
   endtask : watch_pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_main_phase
   //
   protected task watch_main_phase();
      #0 wait (m_start_main_phase == 1'b1);
      main_phase(m_phase_for_main);
   endtask : watch_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_post_main_phase
   //
   protected task watch_post_main_phase();
      #0 wait (m_start_post_main_phase == 1'b1);
      post_main_phase(m_phase_for_post_main);
   endtask : watch_post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pre_shutdown_phase
   //
   protected task watch_pre_shutdown_phase();
      #0 wait (m_start_pre_shutdown_phase == 1'b1);
      pre_shutdown_phase(m_phase_for_pre_shutdown);
   endtask : watch_pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_shutdown_phase
   //
   protected task watch_shutdown_phase();
      #0 wait (m_start_shutdown_phase == 1'b1);
      shutdown_phase(m_phase_for_shutdown);
   endtask : watch_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_post_shutdown_phase
   //
   protected task watch_post_shutdown_phase();
      #0 wait (m_start_post_shutdown_phase == 1'b1);
      post_shutdown_phase(m_phase_for_post_shutdown);
   endtask : watch_post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      return;
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      return;
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      return;
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      return;
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      return;
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      return;
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      return;
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      //
      // If this is not a background sequence, then we need to notify the virtual component map that
      // we have a terminating sequence.
      //
      if (m_background_seq == 1'b0) begin
         evl_verif_comp_map::set_terminating_sequence();
      end
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      return;
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      return;
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      return;
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      return;
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_phase_changes
   //
   protected task watch_phase_changes();
      evl_phase_type_t cur_phase;
      evl_phase_type_t new_phase;
      uvm_phase        phase;

      cur_phase = BuildPhase;
      forever begin
         m_global_attrs.get_next_phase_change(phase, new_phase, cur_phase);
         cur_phase = new_phase;
         if (phase != null) begin
            case (cur_phase)
               PreResetPhase:      start_pre_reset_phase(phase);
               ResetPhase:         start_reset_phase(phase);
               PostResetPhase:     start_post_reset_phase(phase);
               PreConfigurePhase:  start_pre_configure_phase(phase);
               ConfigurePhase:     start_configure_phase(phase);
               PostConfigurePhase: start_post_configure_phase(phase);
               PreMainPhase:       start_pre_main_phase(phase);
               MainPhase:          start_main_phase(phase);
               PostMainPhase:      start_post_main_phase(phase);
               PreShutdownPhase:   start_pre_shutdown_phase(phase);
               ShutdownPhase:      start_shutdown_phase(phase);
               PostShutdownPhase:  start_post_shutdown_phase(phase);
            endcase
         end
      end
   endtask : watch_phase_changes


   //-----------------------------------------------------------------------------------------------
   //
   // Task: start_sequence
   //
   task start_sequence();
      if (m_started++ != 0) begin
         m_started = 1;
         return;
      end
      fork
         watch_phase_changes();
         watch_pre_reset_phase();
         watch_reset_phase();
         watch_post_reset_phase();
         watch_pre_configure_phase();
         watch_configure_phase();
         watch_post_configure_phase();
         watch_pre_main_phase();
         watch_main_phase();
         watch_post_main_phase();
         watch_pre_shutdown_phase();
         watch_shutdown_phase();
         watch_post_shutdown_phase();
      join_none
   endtask : start_sequence


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_terminating_seq
   //
   virtual task convert_to_terminating_seq();
      if (m_background_seq == 1'b1) begin
         #0 m_background_seq = 1'b0;
      end
   endtask : convert_to_terminating_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_background_seq
   //
   virtual task convert_to_background_seq();
      if (m_background_seq == 1'b0) begin
         #0 m_background_seq = 1'b1;
      end
   endtask : convert_to_background_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_sequence #(REQ, RSP) source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      set_abstract_name(source_obj.get_abstract_name());
      m_agent          = source_obj.m_agent;
      m_background_seq = source_obj.m_background_seq;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence");
      super.new(name_in);
      m_sequence_wrapper = new(name_in, this);
   endfunction : new

endclass : evl_sequence

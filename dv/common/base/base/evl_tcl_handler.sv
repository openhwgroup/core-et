//--------------------------------------------------------------------------------------------------
//
// Class: evl_tcl_handler
//
// This is the base class for processing TCL scripts.  There should be no need to extend it ... all
// personalization of specific TCL commands should occur in an extension of evl_tcl_cmd_parser.
// There are currently 2 class extensions of evl_tcl_cmd_parser ...
//
// information).  The TCL handler (evl_tcl_handler) creates a TCL interpreter instance and processes
// a TCL script.  While processing a TCL script, it goes through the following steps:
//
// - The TCL handler first calls the DPI function evl_tcl_prepare_tcl_script(), which creates a TCL
//   interpreter instance, initializes it, and reads the specified TCL script into local storage in
//   the DPI code.  This happens as soon as the file name is set within evl_tcl_handler.
// - For every evl_tcl_cmd_parser instance within evl_tcl_handler, the handler sets the verification
//   component and TCL handler within the evl_tcl_cmd_parser instance and then calls the
//   add_tcl_commands() function in the evl_tcl_cmd_parser instance.
// - The TCL handler then calls the DPI function evl_tcl_start_tcl_script(), which starts the TCL
//   script and then transfers control back to Verilog.
// - For every evl_tcl_cmd_parser instance within evl_tcl_handler, the handler calls the
//   add_initial_commands() function in the evl_tcl_cmd_parser instance.  This allows each
//   evl_tcl_cmd_parser instance to perform some initial TCL commands before starting to process the
//   script.
// - The TCL handler then waits for its start_script() function to be called, and then it begins
//   processing the TCL script.
// - As the DPI makes Verilog callbacks (i.e., when it returns from the evl_tcl_get_nex_cmd() DPI
//   call), each evl_tcl_cmd_parser instance is given an opportunity to parse the returned command.
//   Once a evl_tcl_cmd_parser instance successfully parses a command, no additional parsing is
//   performed.
//
// The implication here as that the TCL commands to execute in Verilog are added and parsed by an
// extended evl_tcl_cmd_parser class.  This extended class must be created and then added to TCL
// handler instance (via a add_tcl_cmd_parser() call in the TCL handler instance) before the TCL
// handler's set_file_name() call is performed.  The following virtual methods in the extended class
// need to be defined:
//
// - add_tcl_commands():     This method will add the definitions of TCL-to-Verilog commands to be
//                           processed.
// - add_initial_commands(): This method will add any TCL commands that need to be preprocessed
//                           before starting TCL script evaluation.
// - parse_tcl_command():    This method processesd the defined TCL-to-Verilog commands.  It will be
//                           called whenever the TCL script requires command evaluation.
//
class evl_tcl_handler extends evl_object;

   `uvm_object_utils(evl_tcl_handler)

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   static int                       m_tcl_thread_id     = 1;
          bit                       m_file_name_set     = 1'b0;
          bit                       m_start_script      = 1'b0;
          bit                       m_in_initialization = 1'b1;
          bit                       m_watch_phases      = 1'b0;
          bit                       m_script_done       = 1'b0;
          bit                       m_finish_from_this  = 1'b0;
          int                       m_tcl_thread_num    = -1;
          int                       m_tcl_seed          = 1;
          int                       m_replay_script     = 0;
          int                       m_tcl_handler_id    = 0;
          evl_sequencer_id_t        m_sequencer_id      = 0;
          string                    m_file_name         = "";
          evl_verif_comp            m_agent             = null;
          evl_verif_comp            m_cur_agent         = null;
          evl_verif_comp            m_target_agent      = null;
          evl_verif_comp            m_system_agent      = null;
          evl_agent_key_t           m_cur_agent_key     = ~evl_agent_key_t'(0);
          evl_phase_type_t          m_starting_phase    = PreResetPhase;
          evl_semaphore_wrapper     m_semaphore_wrapper;
          evl_message_wrapper       m_message_wrapper;
          evl_sequence_wrapper_base m_sequence_wrappers[evl_agent_key_t];
          evl_tcl_cmd_parser        m_tcl_cmd_parsers[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tcl_handler_id
   //
   function void set_tcl_handler_id(input int id_in);
      m_tcl_handler_id = id_in;
   endfunction : set_tcl_handler_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tcl_handler_id
   //
   function int get_tcl_handler_id();
      return m_tcl_handler_id;
   endfunction : get_tcl_handler_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_sequencer_id
   //
   function void set_sequencer_id(input evl_sequencer_id_t id_in);
      m_sequencer_id = id_in;
   endfunction : set_sequencer_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sequencer_id
   //
   function evl_sequencer_id_t get_sequencer_id();
      return m_sequencer_id;
   endfunction : get_sequencer_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   function void set_agent(input evl_verif_comp agent_in, input evl_sequencer_id_t sequencer_id_in);
      m_agent = agent_in;
      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_agent(agent_in);
      end
      set_cur_agent(agent_in, sequencer_id_in);
      set_target_agent(agent_in);
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cur_agent
   //
   virtual function void set_cur_agent(input evl_verif_comp agent_in, input evl_sequencer_id_t sequencer_id_in);
      m_cur_agent     = agent_in;
      m_cur_agent_key = m_cur_agent.get_agent_key() | evl_agent_key_t'(sequencer_id_in);
      if (!(m_sequence_wrappers.exists(m_cur_agent_key))) begin
         foreach (m_sequence_wrappers[ii]) begin
            m_cur_agent_key = ii;
            break;
         end
      end
      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_cur_agent(agent_in, sequencer_id_in);
      end
      set_target_agent(agent_in);
   endfunction : set_cur_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_target_agent
   //
   virtual function void set_target_agent(input evl_verif_comp agent_in);
      m_target_agent = agent_in;
      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_target_agent(agent_in);
      end
   endfunction : set_target_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cur_agent
   //
   function evl_verif_comp get_cur_agent();
      return m_cur_agent;
   endfunction : get_cur_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_target_agent
   //
   function evl_verif_comp get_target_agent();
      return m_target_agent;
   endfunction : get_target_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequence_wrapper_for_agent
   //
   virtual function void add_sequence_wrapper_for_agent(input evl_verif_comp            agent_in,
                                                        input evl_sequence_wrapper_base sequence_wrapper_in,
                                                        input evl_sequencer_id_t        sequencer_id_in);
      if ((agent_in != null) && (sequence_wrapper_in != null)) begin
         evl_agent_key_t agent_key;

         agent_key = agent_in.get_agent_key() | evl_agent_key_t'(sequencer_id_in);
         m_sequence_wrappers[agent_key] = sequence_wrapper_in;
         if ((agent_in == m_cur_agent) && (m_cur_agent == null)) begin
            m_cur_agent_key = m_cur_agent.get_agent_key() | evl_agent_key_t'(sequencer_id_in);
         end
         else if (m_cur_agent_key == ~evl_agent_key_t'(0)) begin
            m_cur_agent_key = agent_in.get_agent_key() | evl_agent_key_t'(sequencer_id_in);
         end
      end
      if (sequence_wrapper_in != null) begin
         evl_verif_comp_map::add_sequence_wrapper_to_list(sequence_wrapper_in);
      end
   endfunction : add_sequence_wrapper_for_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_sequence
   //
   virtual function bit have_sequence(input evl_agent_key_t agent_key_in, input evl_sequencer_id_t sequencer_id_in);
      evl_sequencer_id_t sequencer_mask;
      evl_agent_key_t    agent_key;

      sequencer_mask = ~evl_sequencer_id_t'(0);
      agent_key      = (agent_key_in & (~evl_agent_key_t'(sequencer_mask))) | evl_agent_key_t'(sequencer_id_in);
      return ((m_sequence_wrappers.exists(agent_key)) ? 1'b1 : 1'b0);
   endfunction : have_sequence


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tcl_cmd_parser
   //
   function void add_tcl_cmd_parser(input evl_tcl_cmd_parser tcl_cmd_parser_in);
      if (!(tcl_cmd_parser_in inside { m_tcl_cmd_parsers }) && (m_file_name_set == 0)) begin
         m_tcl_cmd_parsers.push_back(tcl_cmd_parser_in);
         tcl_cmd_parser_in.set_agent(m_agent);
         tcl_cmd_parser_in.set_tcl_handler(this);
         if (m_semaphore_wrapper == null) begin
            m_semaphore_wrapper = new(get_abstract_name());
         end
         if (m_message_wrapper == null) begin
            m_message_wrapper = new(get_abstract_name());
         end
         tcl_cmd_parser_in.set_semaphore_wrapper(m_semaphore_wrapper);
         tcl_cmd_parser_in.set_message_wrapper(m_message_wrapper);
      end
   endfunction : add_tcl_cmd_parser


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_semaphore_wrapper
   //
   function void set_semaphore_wrapper(input evl_semaphore_wrapper wrapper_in);
      m_semaphore_wrapper = wrapper_in;
      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_semaphore_wrapper(wrapper_in);
      end
   endfunction : set_semaphore_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_message_wrapper
   //
   function void set_message_wrapper(input evl_message_wrapper wrapper_in);
      m_message_wrapper = wrapper_in;
      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_message_wrapper(wrapper_in);
      end
   endfunction : set_message_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_file_name
   //
   function void set_file_name(input string file_name_in);
      m_file_name     = file_name_in;
      m_file_name_set = 1'b1;
   endfunction : set_file_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_file_name
   //
   function string get_file_name();
      return m_file_name;
   endfunction : get_file_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: open_replay_file
   //
   function void open_replay_file(input string file_name_in);
      if (m_replay_script == 0) begin
         m_replay_script = evl_tcl_open_replay_file(m_tcl_thread_num, file_name_in);
      end
   endfunction : open_replay_file


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_replay_write
   //
   function int perform_replay_write();
      return m_replay_script;
   endfunction : perform_replay_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: replay_file_write
   //
   function void replay_file_write(input string message_in);
      if (m_replay_script != 0) begin
         evl_tcl_replay_file_write(m_tcl_thread_num, message_in);
      end
   endfunction : replay_file_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_script
   //
   function void start_script();
      m_start_script = 1'b1;
   endfunction : start_script


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_script_done
   //
   task wait_for_script_done();
      #0 wait (m_script_done == 1'b1);
   endtask : wait_for_script_done


   //-----------------------------------------------------------------------------------------------
   //
   // Task: set_script_done
   //
   function void set_script_done();
      m_script_done = 1'b1;
   endfunction : set_script_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_starting_phase
   //
   function void set_starting_phase(input evl_phase_type_t phase_in);
      if ((m_in_initialization == 1'b1) && ((int'(phase_in) < int'(ShutdownPhase)) && (int'(m_global_attrs.get_cur_phase()) < int'(phase_in)))) begin
         m_starting_phase = phase_in;
         m_watch_phases   = 1'b1;
      end
   endfunction : set_starting_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_starting_phase
   //
   function evl_phase_type_t get_starting_phase();
      return m_starting_phase;
   endfunction : get_starting_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: out_of_initialization
   //
   function void out_of_initialization();
      m_in_initialization = 1'b0;
   endfunction : out_of_initialization


   //-----------------------------------------------------------------------------------------------
   //
   // Function: in_initialization
   //
   function bit in_initialization();
      return m_in_initialization;
   endfunction : in_initialization


   //-----------------------------------------------------------------------------------------------
   //
   // Function: finish_sim_called
   //
   function void finish_sim_called();
      m_finish_from_this = 1'b1;
   endfunction : finish_sim_called


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_initial_commands
   //
   virtual function int add_initial_commands(input int thread_num);
      return 0;
   endfunction : add_initial_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_start_of_phase
   //
   task wait_for_start_of_phase();
      if ((m_watch_phases == 1'b1) && (m_in_initialization == 1'b0) && (int'(m_global_attrs.get_cur_phase()) < int'(m_starting_phase))) begin
         m_global_attrs.wait_for_start_of_phase(m_starting_phase);
      end
   endtask : wait_for_start_of_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_zero_time
   //
   task wait_zero_time(input int unsigned count);
      if (m_system_agent != null) begin
         m_system_agent.wait_zero_time(count);
      end
      #0;
   endtask : wait_zero_time


   //-----------------------------------------------------------------------------------------------
   //
   // Task: open_and_run_tcl_script
   //
   task open_and_run_tcl_script();
      string file_name;
      string error_result;
      int    result;

      wait (m_file_name_set == 1'b1);
      file_name = evl_tcl_find_file(m_file_name, 1);
      if (file_name == "") begin
         set_script_done();
         return;
      end

      m_tcl_thread_num = evl_tcl_prepare_tcl_script(m_tcl_thread_id++, file_name);
      if (m_tcl_thread_num < 0) begin
         set_script_done();
         return;
      end

      foreach (m_tcl_cmd_parsers[ii]) begin
         m_tcl_cmd_parsers[ii].set_agent(m_agent);
         m_tcl_cmd_parsers[ii].set_tcl_handler(this);
         if (m_tcl_cmd_parsers[ii].add_tcl_commands(m_tcl_thread_num) < 0) begin
            set_script_done();
            return;
         end
      end

      result = evl_tcl_start_tcl_script(m_tcl_thread_num);
      if (result < 0) begin
         set_script_done();
         return;
      end

      if (m_agent != null) begin
         m_tcl_seed = evl_rand_gen::gen_seed_from_string($sformatf("%0s%x%0s%01x", m_agent.get_full_name(), m_agent.get_agent_id(), m_agent.get_abstract_name(), m_tcl_thread_id));
         set_abstract_name($sformatf("%0s(TCL)", m_agent.get_root_abstract_name()));
      end
      else begin
         m_tcl_seed = evl_rand_gen::gen_seed_from_string($sformatf("%0s-%0d", file_name, m_tcl_thread_num));
         set_abstract_name($sformatf("TCL-%0d", m_tcl_thread_num));
      end

      if (evl_tcl_eval_string(m_tcl_thread_num, "namespace import ::tcl::mathfunc::*;") < 0) begin
         set_script_done();
         return;
      end

      `evl_log_no_addr(UVM_HIGH, VMOD_TCL, get_abstract_name(), $sformatf("setting TCL random seed to 0x%x (%0s)", m_tcl_seed, file_name))
      if (evl_tcl_eval_string(m_tcl_thread_num, $sformatf("srand %0d;", m_tcl_seed)) < 0) begin
         set_script_done();
         return;
      end

      if (add_initial_commands(m_tcl_thread_num) < 0) begin
         set_script_done();
         return;
      end
      foreach (m_tcl_cmd_parsers[ii]) begin
         if (m_tcl_cmd_parsers[ii].add_initial_commands(m_tcl_thread_num) < 0) begin
            set_script_done();
            return;
         end
      end

      wait (m_start_script == 1'b1);
      fork
         begin : end_of_sim_watcher;
            m_global_attrs.wait_for_terminate();
            if (m_finish_from_this == 1'b0) begin
               disable parse_loop;
            end
         end
      join_none

      #0 result = 0;
      while ((result >= 0) && (m_finish_from_this == 1'b0)) begin : parse_loop
         int                       cmd_from_tcl;
         evl_sequence_wrapper_base sequence_wrapper;

         result = evl_tcl_get_next_cmd(m_tcl_thread_num);
         if ((result < 0) || (m_finish_from_this == 1'b1)) begin
            break;
         end

         cmd_from_tcl = result;
         result       = 0;
         if (m_sequence_wrappers.exists(m_cur_agent_key)) begin
            sequence_wrapper = m_sequence_wrappers[m_cur_agent_key];
         end
         else begin
            sequence_wrapper = null;
         end
         if (m_in_initialization == 1'b1) begin
            m_tcl_cmd_parsers[0].parse_tcl_command(result, m_tcl_thread_num, cmd_from_tcl, sequence_wrapper);
            if (m_in_initialization == 1'b0) begin
               wait_for_start_of_phase();
            end
         end
         if (m_in_initialization == 1'b0) begin
            foreach (m_tcl_cmd_parsers[ii]) begin
               m_tcl_cmd_parsers[ii].parse_tcl_command(result, m_tcl_thread_num, cmd_from_tcl, sequence_wrapper);
               if (result != 0) begin
                  break;
               end
            end
         end
      end
      disable end_of_sim_watcher;
      if (result < -1) begin
         error_result = evl_tcl_get_str_arg(m_tcl_thread_num, 0);
         `dut_error("TCL", $sformatf("error while processing \"%s\"", file_name))
         `dut_error("TCL", $sformatf("%s", error_result))
      end
      set_script_done();
   endtask : open_and_run_tcl_script


   //-----------------------------------------------------------------------------------------------
   //
   // Read and start the script
   //
   task read_and_start_script();
      open_and_run_tcl_script();
      if ((m_tcl_thread_num >= 0) && (m_replay_script != 0)) begin
         evl_tcl_close_replay_file(m_tcl_thread_num);
      end
   endtask : read_and_start_script


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tcl_handler");
      evl_tcl_default_parser default_parser;

      super.new(name_in);
      default_parser = new();
      add_tcl_cmd_parser(default_parser);
      m_system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT);
      fork
         read_and_start_script();
      join_none
   endfunction : new

endclass : evl_tcl_handler

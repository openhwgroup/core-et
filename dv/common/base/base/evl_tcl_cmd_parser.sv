//--------------------------------------------------------------------------------------------------
//
// Class: evl_tcl_cmd_parser
//
// This is the base class for the TCL command parser (refer to evl_tcl_handler.sv for additional
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
virtual class evl_tcl_cmd_parser extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_tcl_handler       m_tcl_handler       = null;
   evl_verif_comp        m_agent             = null;
   evl_verif_comp        m_cur_agent         = null;
   evl_verif_comp        m_target_agent      = null;
   evl_verif_comp        m_system_agent      = null;
   evl_delay_wrapper     m_delay_wrapper     = null;
   evl_semaphore_wrapper m_semaphore_wrapper = null;
   evl_message_wrapper   m_message_wrapper   = null;
   evl_sequencer_id_t    m_sequencer_id      = 0;
   int                   m_tcl_commands[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_semaphore_wrapper
   //
   function void set_semaphore_wrapper(input evl_semaphore_wrapper wrapper_in);
      m_semaphore_wrapper = wrapper_in;
   endfunction : set_semaphore_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_message_wrapper
   //
   function void set_message_wrapper(input evl_message_wrapper wrapper_in);
      m_message_wrapper = wrapper_in;
   endfunction : set_message_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_arg_value_from_scanf
   //
   function bit get_arg_value_from_scanf(input string format_string_in, input string arg_string_in, ref longint value);
      string format_string;
      string val_string;

      format_string = $sformatf("%0s=%0s", format_string_in, "%s");
      if ($sscanf(arg_string_in, format_string, val_string) <= 0) begin
         return 1'b0;
      end
      if (val_string.len() == 0) begin
         return 1'b0;
      end
      val_string = val_string.tolower();
      if (val_string.len() > 1) begin
         if (val_string.substr(0, 1) == "0x") begin
            value = convert_from_hex(val_string.substr(2, val_string.len() - 1));
         end
         else if (val_string.substr(0, 1) == "0b") begin
            value = convert_from_bin(val_string.substr(2, val_string.len() - 1));
         end
         else if (val_string.substr(0, 0) == "0") begin
            value = convert_from_oct(val_string.substr(1, val_string.len() - 1));
         end
         else begin
            value = convert_from_dec(val_string);
         end
      end
      else begin
         value = convert_from_dec(val_string);
      end
      return 1'b1;
   endfunction : get_arg_value_from_scanf


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_from_bin
   //
   function longint convert_from_bin(input string arg_in);
      longint value = longint'(0);
      int     limit = arg_in.len();

      for (int ii = 0; ii < limit; ii++) begin
         byte cc;

         value = value * 2;
         cc = arg_in.getc(ii);
         if ((cc >= byte'("0")) && (cc <= byte'("1"))) begin
            value = value + longint'(cc - byte'("0"));
         end
         else begin
            return value;
         end
      end
      return value;
   endfunction : convert_from_bin


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_from_oct
   //
   function longint convert_from_oct(input string arg_in);
      longint value = longint'(0);
      int     limit = arg_in.len();

      for (int ii = 0; ii < limit; ii++) begin
         byte cc;

         value = value * 8;
         cc = arg_in.getc(ii);
         if ((cc >= byte'("0")) && (cc <= byte'("7"))) begin
            value = value + longint'(cc - byte'("0"));
         end
         else begin
            return value;
         end
      end
      return value;
   endfunction : convert_from_oct


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_from_dec
   //
   function longint convert_from_dec(input string arg_in);
      longint value = longint'(0);
      int     limit = arg_in.len();

      for (int ii = 0; ii < limit; ii++) begin
         byte cc;

         value = value * 10;
         cc = arg_in.getc(ii);
         if ((cc >= byte'("0")) && (cc <= byte'("9"))) begin
            value = value + longint'(cc - byte'("0"));
         end
         else begin
            return value;
         end
      end
      return value;
   endfunction : convert_from_dec


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_from_hex
   //
   function longint convert_from_hex(input string arg_in);
      longint value = longint'(0);
      int     limit = arg_in.len();

      for (int ii = 0; ii < limit; ii++) begin
         byte cc;

         value = value * 16;
         cc = arg_in.getc(ii);
         if ((cc >= byte'("0")) && (cc <= byte'("9"))) begin
            value = value + longint'(cc - byte'("0"));
         end
         else if ((cc >= byte'("a")) && (cc <= byte'("f"))) begin
            value = value + longint'(cc - (byte'("a") - 10));
         end
         else begin
            return value;
         end
      end
      return value;
   endfunction : convert_from_hex


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tcl_command
   //
   function int add_tcl_command(input int    parser_cmd,
                                input int    thread_num,
                                input string cmd_name,
                                input int    min_arg_count,
                                input int    max_arg_count,
                                input string return_type,
                                input string parse_type = "");
      int cmd_encoding;

      cmd_encoding = evl_tcl_add_cmd_enc(thread_num, cmd_name, min_arg_count, max_arg_count, return_type, parse_type);
      if (cmd_encoding >= 0) begin
          m_tcl_commands[cmd_encoding] = parser_cmd;
          return 0;
      end
      return -1;
   endfunction : add_tcl_command


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tcl_handler
   //
   function void set_tcl_handler(input evl_tcl_handler tcl_handler_in);
      m_tcl_handler = tcl_handler_in;
   endfunction : set_tcl_handler


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tcl_handler_id
   //
   function int get_tcl_handler_id();
      if (m_tcl_handler == null) begin
         return 0;
      end
      return m_tcl_handler.get_tcl_handler_id();
   endfunction : get_tcl_handler_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   function void set_agent(input evl_verif_comp agent_in);
      m_agent        = agent_in;
      m_cur_agent    = agent_in;
      m_target_agent = agent_in;
      if (m_system_agent == null) begin
         m_system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT, 0);
      end
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cur_agent
   //
   function void set_cur_agent(input evl_verif_comp agent_in, input evl_sequencer_id_t sequencer_id_in);
      m_cur_agent    = agent_in;
      m_target_agent = agent_in;
      m_sequencer_id = sequencer_id_in;
   endfunction : set_cur_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_target_agent
   //
   function void set_target_agent(input evl_verif_comp agent_in);
      m_target_agent = agent_in;
   endfunction : set_target_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tcl_handler
   //
   function evl_tcl_handler get_tcl_handler();
      return m_tcl_handler;
   endfunction : get_tcl_handler


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sequencer_id
   //
   function evl_sequencer_id_t get_sequencer_id();
      return m_sequencer_id;
   endfunction : get_sequencer_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cur_agent
   //
   function evl_verif_comp get_cur_agent();
      return m_cur_agent;
   endfunction : get_cur_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cur_agent_key
   //
   function evl_agent_key_t get_cur_agent_key();
      return m_cur_agent.get_agent_key() | evl_agent_key_t'(m_sequencer_id);
   endfunction : get_cur_agent_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: parse_agent_name
   //
   virtual function void parse_agent_name(input  int                   thread_num,
                                          input  string                agent_name_in,
                                          output string                agent_name_out,
                                          output evl_agent_id_t        agent_id_out,
                                          output evl_shire_id_t        shire_id_out,
                                          output evl_neighborhood_id_t neighborhood_id_out,
                                          output evl_core_id_t         core_id_out);
         evl_tcl_parse_agent_name(thread_num, agent_name_in, 0);
         agent_name_out      = evl_tcl_get_str_arg(thread_num, 0);
         agent_id_out        = evl_agent_id_t'(evl_tcl_get_int_arg(thread_num, 1));
         shire_id_out        = evl_shire_id_t'(evl_tcl_get_int_arg(thread_num, 2));
         neighborhood_id_out = evl_neighborhood_id_t'(evl_tcl_get_int_arg(thread_num, 3));
         core_id_out         = evl_core_id_t'(evl_tcl_get_int_arg(thread_num, 4));
   endfunction : parse_agent_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tcl_commands
   //
   virtual function int add_tcl_commands(input int thread_num);
      return 0;
   endfunction : add_tcl_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_initial_commands
   //
   virtual function int add_initial_commands(input int thread_num);
      return 0;
   endfunction : add_initial_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Task: parse_tcl_command
   //
   virtual task parse_tcl_command(output int                       result,
                                  input  int                       thread_num,
                                  input  int                       cmd_from_tcl,
                                  input  evl_sequence_wrapper_base sequence_wrapper);
      result = 0;
      return;
   endtask : parse_tcl_command


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tcl_cmd_parser");
      super.new(name_in);
   endfunction : new

endclass : evl_tcl_cmd_parser

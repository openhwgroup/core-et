//--------------------------------------------------------------------------------------------------
//
// Class: evl_tcl_mem_seq_parser
//
class evl_tcl_mem_seq_parser extends evl_tcl_cmd_parser;

   `uvm_object_utils(evl_tcl_mem_seq_parser)


   //-----------------------------------------------------------------------------------------------
   //
   // Local Types
   //
   // The following enumerated types are local to this file.  These are the command encodings
   // supported by this parser.  To simplify adding commands, note that the names are the string
   // names with the "TCL_" prefix.
   //
   typedef enum int {
         TCL_set_read_limit,
         TCL_set_write_limit,
         TCL_set_blocking,
         TCL_set_length,
         TCL_set_cache_attr,
         TCL_set_mem_space,
         TCL_set_qword_enable,
         TCL_set_req_source,
         TCL_set_atomic_transid,
         TCL_set_thread,
         TCL_set_qos,
         TCL_set_vc,
         TCL_set_subopcode_dest,
         TCL_get_read_limit,
         TCL_get_write_limit,
         TCL_get_blocking,
         TCL_get_length,
         TCL_get_cache_attr,
         TCL_get_mem_space,
         TCL_get_qword_enable,
         TCL_get_req_source,
         TCL_get_atomic_transid,
         TCL_get_qos,
         TCL_get_vc,
         TCL_get_subopcode_dest,
         TCL_add_trans_delay,
         TCL_set_trans_delay,
         TCL_reset_trans_delay,
         TCL_print_trans_delay,
         TCL_set_drop_on_conflict,
         TCL_clr_drop_on_conflict,
         TCL_entry_has_read_data,
         TCL_entry_has_read_err,
         TCL_get_read_data,
         TCL_entry_has_write_err,
         TCL_NOP,
         TCL_BARRIER,
         TCL_LD,
         TCL_LD_COOP,
         TCL_LD_NO_MOD,
         TCL_ST,
         TCL_ST_WA,
         TCL_ST_NO_MOD,
         TCL_ST_DBG_NOC,
         TCL_EVICT,
         TCL_CACHE_LOCK,
         TCL_CACHE_UNLOCK,
         TCL_CACHE_FLUSH,
         TCL_CACHE_FLUSH_TO_MEM,
         TCL_CACHE_EVICT,
         TCL_CACHE_EVICT_TO_MEM,
         TCL_CACHE_PREFETCH,
         TCL_CACHE_SCP_FILL,
         TCL_MESSAGE,
         TCL_ATOMIC,
         TCL_IFETCH,
         TCL_REFETCH,
         TCL_CANCEL,
         TCL_JTAG_STEP,
         TCL_JTAG_RESET,
         TCL_JTAG_RUN_TEST,
         TCL_SELECT_CHAIN,
         TCL_JTAG_READ,
         TCL_JTAG_WRITE,
         TCL_JTAG_RD,
         TCL_JTAG_WR,
         TCL_JTAG_RD_PARTIAL,
         TCL_JTAG_WR_PARTIAL
      } evl_tcl_command_t;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                      m_blocking         = 1'b0;
   int                      m_cmd_length       = 1;
   bit                      m_cache_attr       = 1'b1;
   bit                      m_drop_on_conflict = 1'b0;
   int                      m_read_limit       = 32;
   int                      m_write_limit      = 32;
   evl_bus_atomic_transid_t m_atomic_transid   = evl_bus_atomic_transid_t'(0);
   evl_addr_sp_t            m_addr_sp          = Mem;
   evl_line_qword_en_t      m_qword_en         = ~evl_line_qword_en_t'(0);
   evl_bus_req_source_t     m_req_source       = ~evl_bus_req_source_t'(0);
   evl_bus_qos_t            m_qos              = evl_bus_qos_t'(0);
   evl_bus_req_vc_t         m_vc               = ~evl_bus_req_vc_t'(0);
   evl_thread_id_t          m_thread_id        = evl_thread_id_t'(0);
   evl_subopcode_dest_t     m_subopcode_dest   = dest_l2;
   evl_trans_desc           m_read_trans_descs[$];
   evl_trans_desc           m_write_trans_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_my_tcl_command
   //
   // This function is only included to simplify generating TCL commands from enumerated parser
   // commands.  The TCL command string will be the enumerated string with the "TCL_" prefix
   // removed.
   //
   function int add_my_tcl_command(input evl_tcl_command_t parser_cmd,
                                   input int               thread_num,
                                   input int               min_arg_count,
                                   input int               max_arg_count,
                                   input string            return_type,
                                   input string            parse_type = "");
      string cmd_name;

      cmd_name = $sformatf("%p", parser_cmd);
      cmd_name = cmd_name.substr(4, cmd_name.len() - 1);
      return add_tcl_command(int'(parser_cmd), thread_num, cmd_name, min_arg_count, max_arg_count, return_type, parse_type);
   endfunction : add_my_tcl_command


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tcl_commands
   //
   // This is the redefinition of the add_tcl_commands() function in evl_tcl_cmd_parser.  This
   // function is called automatically (at the correct time) for all evl_tcl_cmd_parser objects
   // instantiated in an evl_tcl_handler instantiation.
   //
   virtual function int add_tcl_commands(input int thread_num);
      int result = 0;

      result |= add_my_tcl_command(TCL_set_read_limit,       thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_write_limit,      thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_blocking,         thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_length,           thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_cache_attr,       thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_mem_space,        thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_qword_enable,     thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_req_source,       thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_atomic_transid,   thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_thread,           thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_qos,              thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_vc,               thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_subopcode_dest,   thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_get_read_limit,       thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_write_limit,      thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_blocking,         thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_length,           thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_cache_attr,       thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_mem_space,        thread_num, 0, 0, "string");
      result |= add_my_tcl_command(TCL_get_qword_enable,     thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_req_source,       thread_num, 0, 0, "string");
      result |= add_my_tcl_command(TCL_get_atomic_transid,   thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_qos,              thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_vc,               thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_subopcode_dest,   thread_num, 0, 0, "string");
      result |= add_my_tcl_command(TCL_add_trans_delay,      thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_set_trans_delay,      thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_reset_trans_delay,    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_print_trans_delay,    thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_set_drop_on_conflict, thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_clr_drop_on_conflict, thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_entry_has_read_data,  thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_entry_has_read_err,   thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_read_data,        thread_num, 0, 1, "int list");
      result |= add_my_tcl_command(TCL_entry_has_write_err,  thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_NOP,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_BARRIER,              thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_LD,                   thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_LD_COOP,              thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_LD_NO_MOD,            thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_ST,                   thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_ST_WA,                thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_ST_NO_MOD,            thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_ST_DBG_NOC,           thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_EVICT,                thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_CACHE_LOCK,           thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_CACHE_UNLOCK,         thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_CACHE_EVICT,          thread_num, 3, 3, "int");
      result |= add_my_tcl_command(TCL_CACHE_EVICT_TO_MEM,   thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_CACHE_FLUSH,          thread_num, 3, 3, "int");
      result |= add_my_tcl_command(TCL_CACHE_FLUSH_TO_MEM,   thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_CACHE_PREFETCH,       thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_CACHE_SCP_FILL,       thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_MESSAGE,              thread_num, 3, 10, "int");
      result |= add_my_tcl_command(TCL_ATOMIC,               thread_num, 2, 6, "int");
      result |= add_my_tcl_command(TCL_IFETCH,               thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_REFETCH,              thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_CANCEL,               thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_JTAG_STEP,            thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_JTAG_RESET,           thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_JTAG_RUN_TEST,        thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_SELECT_CHAIN,         thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_JTAG_READ,            thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_JTAG_WRITE,           thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_JTAG_RD,              thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_JTAG_WR,              thread_num, 2, 9, "int");
      result |= add_my_tcl_command(TCL_JTAG_RD_PARTIAL,      thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_JTAG_WR_PARTIAL,      thread_num, 2, 9, "int");
      return result;
   endfunction : add_tcl_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_initial_commands
   //
   virtual function int add_initial_commands(input int thread_num);
      int result;

      if (m_delay_wrapper == null) begin
         m_delay_wrapper = new($sformatf("%0s(DLY)", get_abstract_name()));
      end
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
      evl_trans_desc    trans_desc;
      evl_tcl_command_t tcl_command;
      evl_sequence_mdir stimulus_seq;

      result = 0;
      if (!(m_tcl_commands.exists(cmd_from_tcl)) || (sequence_wrapper == null) || (sequence_wrapper.get_object() == null)) begin
         return;
      end

      $cast(stimulus_seq, sequence_wrapper.get_object());

      result      = cmd_from_tcl;
      tcl_command = evl_tcl_command_t'(m_tcl_commands[cmd_from_tcl]);

      //
      // "set_read_limit" Evaluation
      //
      if (tcl_command == TCL_set_read_limit) begin
         m_read_limit = evl_tcl_get_int_arg(thread_num, 0);
         if (m_read_limit < 1) begin
            m_read_limit = 1;
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_write_limit" Evaluation
      //
      if (tcl_command == TCL_set_write_limit) begin
         m_write_limit = evl_tcl_get_int_arg(thread_num, 0);
         if (m_write_limit < 1) begin
            m_write_limit = 1;
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_blocking" Evaluation
      //
      else if (tcl_command == TCL_set_blocking) begin
         if (evl_tcl_get_int_arg(thread_num, 0) == 0) begin
            m_blocking = 1'b0;
         end
         else if (evl_tcl_get_int_arg(thread_num, 0) == 1) begin
            m_blocking = 1'b1;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_blocking %0d;", $realtime, m_blocking));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_length" Evaluation
      //
      else if (tcl_command == TCL_set_length) begin
         m_cmd_length = evl_tcl_get_int_arg(thread_num, 0);
         if (m_cmd_length < 0) begin
            m_cmd_length = 1;
         end
         else if (m_cmd_length > 64) begin
            m_cmd_length = 64;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_length %0d;", $realtime, m_cmd_length));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_cache_attr" Evaluation
      //
      else if (tcl_command == TCL_set_cache_attr) begin
         string cache_attr_str;

         cache_attr_str = evl_tcl_get_str_arg(thread_num, 0);
         if ((cache_attr_str == "0") || (cache_attr_str == "uncached")) begin
            m_cache_attr = 1'b0;
         end
         else if ((cache_attr_str == "1") || (cache_attr_str == "cached")) begin
            m_cache_attr = 1'b1;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_cache_attr %0d;", $realtime, m_cache_attr));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_mem_space" Evaluation
      //
      else if (tcl_command == TCL_set_mem_space) begin
         string addr_sp_str;

         addr_sp_str = evl_tcl_get_str_arg(thread_num, 0);
         if ((addr_sp_str == "mem") || (addr_sp_str == "Mem")) begin
            m_addr_sp = Mem;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_mem_space %p;", $realtime, m_addr_sp));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_qword_enable" Evaluation
      //
      else if (tcl_command == TCL_set_qword_enable) begin
         m_qword_en = evl_tcl_get_int_arg(thread_num, 0);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_qword_enable 0x%0x;", $realtime, m_qword_en));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_req_source" Evaluation
      //
      else if (tcl_command == TCL_set_req_source) begin
         string req_source_name;

         req_source_name = evl_tcl_get_str_arg(thread_num, 0);
         req_source_name = req_source_name.tolower();
         m_req_source    = 0;
         if ((req_source_name == "minion0,thread0") || (req_source_name == "minion0") || (req_source_name == "0")) begin
            m_req_source = 0;
         end
         else if ((req_source_name == "minion0,thread1") || (req_source_name == "minion1") || (req_source_name == "1")) begin
            m_req_source = 1;
         end
         else if ((req_source_name == "minion1,thread0") || (req_source_name == "minion2") || (req_source_name == "2")) begin
            m_req_source = 2;
         end
         else if ((req_source_name == "minion1,thread1") || (req_source_name == "minion3") || (req_source_name == "3")) begin
            m_req_source = 3;
         end
         else if ((req_source_name == "minion2,thread0") || (req_source_name == "minion4") || (req_source_name == "4")) begin
            m_req_source = 4;
         end
         else if ((req_source_name == "minion2,thread1") || (req_source_name == "minion5") || (req_source_name == "5")) begin
            m_req_source = 5;
         end
         else if ((req_source_name == "minion3,thread0") || (req_source_name == "minion6") || (req_source_name == "6")) begin
            m_req_source = 6;
         end
         else if ((req_source_name == "minion3,thread1") || (req_source_name == "minion7") || (req_source_name == "7")) begin
            m_req_source = 7;
         end
         else if ((req_source_name == "minion4,thread0") || (req_source_name == "icache") || (req_source_name == "8")) begin
            m_req_source = 8;
         end
         else if ((req_source_name == "minion4,thread1") || (req_source_name == "ptw0") || (req_source_name == "9")) begin
            m_req_source = 9;
         end
         else if ((req_source_name == "minion5,thread0") || (req_source_name == "ptw1") || (req_source_name == "10")) begin
            m_req_source = 10;
         end
         else if ((req_source_name == "minion5,thread1") || (req_source_name == "tbox") || (req_source_name == "11")) begin
            m_req_source = 11;
         end
         else if ((req_source_name == "minion6,thread0") || (req_source_name == "coop") || (req_source_name == "12")) begin
            m_req_source = 12;
         end
         else if ((req_source_name == "minion6,thread1") || (req_source_name == "13")) begin
            m_req_source = 13;
         end
         else if ((req_source_name == "minion7,thread0") || (req_source_name == "14")) begin
            m_req_source = 14;
         end
         else if ((req_source_name == "minion7,thread1") || (req_source_name == "15")) begin
            m_req_source = 15;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_req_source %0s;", $realtime, req_source_name));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_atomic_transid" Evaluation
      //
      else if (tcl_command == TCL_set_atomic_transid) begin
         m_atomic_transid = evl_bus_atomic_transid_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_atomic_transid 0x%0x;", $realtime, m_atomic_transid));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_thread" Evaluation
      //
      else if (tcl_command == TCL_set_thread) begin
         m_thread_id = evl_thread_id_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_thread 0x%0x;", $realtime, m_thread_id));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_qos" Evaluation
      //
      else if (tcl_command == TCL_set_qos) begin
         m_qos = evl_bus_qos_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_qos 0x%0x;", $realtime, m_qos));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_vc" Evaluation
      //
      else if (tcl_command == TCL_set_vc) begin
         m_vc = evl_bus_req_vc_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_vc 0x%0x;", $realtime, m_vc));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "set_subopcode_dest" Evaluation
      //
      else if (tcl_command == TCL_set_subopcode_dest) begin
         string subopcode_dest;

         subopcode_dest = evl_tcl_get_str_arg(thread_num, 0);
         subopcode_dest = subopcode_dest.tolower();
         if (subopcode_dest == "l3") begin
            m_subopcode_dest = dest_l3;
         end
         else if (subopcode_dest == "scp") begin
            m_subopcode_dest = dest_scp;
         end
         else begin
            subopcode_dest   = "l2";
            m_subopcode_dest = dest_l2;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_subopcode_dest %0s;", $realtime, subopcode_dest));
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "get_read_limit" Evaluation
      //
      else if (tcl_command == TCL_get_read_limit) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_read_limit));
      end

      //
      // "get_write_limit" Evaluation
      //
      else if (tcl_command == TCL_get_write_limit) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_write_limit));
      end

      //
      // "get_blocking" Evaluation
      //
      else if (tcl_command == TCL_get_blocking) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_blocking));
      end

      //
      // "get_length" Evaluation
      //
      else if (tcl_command == TCL_get_length) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_cmd_length));
      end

      //
      // "get_cache_attr" Evaluation
      //
      else if (tcl_command == TCL_get_cache_attr) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_cache_attr));
      end

      //
      // "get_mem_space" Evaluation
      //
      else if (tcl_command == TCL_get_mem_space) begin
         string mem_space;

         mem_space = m_addr_sp.name();
         void'(evl_tcl_set_str_res(thread_num, 0, mem_space));
      end

      //
      // "get_qword_enable" Evaluation
      //
      else if (tcl_command == TCL_get_qword_enable) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_qword_en));
      end

      //
      // "get_req_source" Evaluation
      //
      else if (tcl_command == TCL_get_req_source) begin
         string req_source;

         case (m_req_source)
            0:       req_source = "minion0";
            1:       req_source = "minion1";
            2:       req_source = "minion2";
            3:       req_source = "minion3";
            4:       req_source = "minion4";
            5:       req_source = "minion5";
            6:       req_source = "minion6";
            7:       req_source = "minion7";
            8:       req_source = "icache";
            9:       req_source = "ptw0";
            10:      req_source = "ptw1";
            11:      req_source = "tbox";
            12:      req_source = "coop";
            13:      req_source = "minion7,thread1";
            14:      req_source = "minion7,thread0";
            default: req_source = "minion7,thread1";
         endcase
         void'(evl_tcl_set_str_res(thread_num, 0, req_source));
      end

      //
      // "get_atomic_transid" Evaluation
      //
      else if (tcl_command == TCL_get_atomic_transid) begin
         void'(evl_tcl_set_int_res(thread_num, 0, m_atomic_transid));
      end

      //
      // "get_qos" Evaluation
      //
      else if (tcl_command == TCL_get_qos) begin
         void'(evl_tcl_set_int_res(thread_num, 0, longint'(m_qos)));
      end

      //
      // "get_vc" Evaluation
      //
      else if (tcl_command == TCL_get_vc) begin
         void'(evl_tcl_set_int_res(thread_num, 0, longint'(m_vc)));
      end

      //
      // "get_subopcode_dest" Evaluation
      //
      else if (tcl_command == TCL_get_subopcode_dest) begin
         string subopcode_dest;

         case (m_subopcode_dest)
            dest_l3:  subopcode_dest = "l3";
            dest_scp: subopcode_dest = "scp";
            default:  subopcode_dest = "l2";
         endcase
         void'(evl_tcl_set_str_res(thread_num, 0, subopcode_dest));
      end

      //
      // "add_trans_delay" Evaluation
      //
      else if (tcl_command == TCL_add_trans_delay) begin
         string      delay_key_name;
         int         delay_key;
         evl_delay_t delay_value;

         delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
         delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 1));
         if (delay_key_name.substr(0, 2) != "pk_") begin
            delay_key_name = $sformatf("pk_%0s", delay_key_name);
         end
         delay_key = evl_param_xref::get_param_key(delay_key_name);
         m_delay_wrapper.set_abstract_name(m_cur_agent.get_abstract_name());
         m_delay_wrapper.add_delay(delay_key, delay_value, 1'b1);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nadd_trans_delay %0s %0d;", $realtime, delay_key_name, delay_value));
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_trans_delay" Evaluation
      //
      else if (tcl_command == TCL_set_trans_delay) begin
         string      delay_key_name;
         int         delay_key;
         evl_delay_t delay_value;

         delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
         delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 1));
         if (delay_key_name.substr(0, 2) != "pk_") begin
            delay_key_name = $sformatf("pk_%0s", delay_key_name);
         end
         delay_key = evl_param_xref::get_param_key(delay_key_name);
         m_delay_wrapper.set_abstract_name(m_cur_agent.get_abstract_name());
         m_delay_wrapper.reset_delay(delay_key);
         m_delay_wrapper.add_delay(delay_key, delay_value);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nset_trans_delay %0s %0d;", $realtime, delay_key_name, delay_value));
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_drop_on_conflict" Evaluation
      //
      else if (tcl_command == TCL_set_drop_on_conflict) begin
         m_drop_on_conflict = 1'b1;
         result = cmd_from_tcl;
      end

      //
      // "clr_drop_on_conflict" Evaluation
      //
      else if (tcl_command == TCL_clr_drop_on_conflict) begin
         m_drop_on_conflict = 1'b0;
         result = cmd_from_tcl;
      end

      //
      // "reset_trans_delay" Evaluation
      //
      else if (tcl_command == TCL_reset_trans_delay) begin
         string delay_key_name;
         int    delay_key;

         delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
         if (delay_key_name.substr(0, 2) != "pk_") begin
            delay_key_name = $sformatf("pk_%0s", delay_key_name);
         end
         delay_key = evl_param_xref::get_param_key(delay_key_name);
         m_delay_wrapper.set_abstract_name(m_cur_agent.get_abstract_name());
         m_delay_wrapper.reset_delay(delay_key);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nreset_trans_delay %0s;", $realtime, delay_key_name));
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "print_trans_delay" Evaluation
      //
      else if (tcl_command == TCL_print_trans_delay) begin
         m_delay_wrapper.print_obj($sformatf("%0s (TCL script)", m_cur_agent.get_abstract_name()));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nprint_trans_delay;", $realtime));
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "entry_has_read_data" Evaluation
      //
      else if (tcl_command == TCL_entry_has_read_data) begin
         int trans_desc_index;

         if (evl_tcl_get_arg_count(thread_num) == 0) begin
            trans_desc_index = 0;
         end
         else begin
            trans_desc_index = int'(evl_tcl_get_int_arg(thread_num, 0));
         end
         if ((trans_desc_index < 0) || (trans_desc_index >= m_read_trans_descs.size())) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         else begin
            evl_trans_desc trans_desc;

            trans_desc = m_read_trans_descs[trans_desc_index];
            if (trans_desc.is_done() == 1'b0) begin
               trans_desc.wait_for_done();
            end
            if (trans_desc.get_read_data() == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
            end
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "entry_has_read_err" Evaluation
      //
      else if (tcl_command == TCL_entry_has_read_err) begin
         int trans_desc_index;

         if (evl_tcl_get_arg_count(thread_num) == 0) begin
            trans_desc_index = 0;
         end
         else begin
            trans_desc_index = int'(evl_tcl_get_int_arg(thread_num, 0));
         end
         if ((trans_desc_index < 0) || (trans_desc_index >= m_read_trans_descs.size())) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         else begin
            evl_trans_desc trans_desc;

            trans_desc = m_read_trans_descs[trans_desc_index];
            if (trans_desc.is_done() == 1'b0) begin
               trans_desc.wait_for_done();
            end
            if (trans_desc.bus_rsp_has_error() == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
            end
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_read_data" Evaluation
      //
      else if (tcl_command == TCL_get_read_data) begin
         int arg_count;
         int trans_desc_index;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 0) begin
            trans_desc_index = 0;
         end
         else begin
            trans_desc_index = int'(evl_tcl_get_int_arg(thread_num, 0));
         end
         if ((trans_desc_index < 0) || (trans_desc_index >= m_read_trans_descs.size())) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(-1));
         end
         else begin
            int            cmd_length;
            evl_trans_desc trans_desc;

            trans_desc = m_read_trans_descs[trans_desc_index];
            result     = 0;
            if (trans_desc.is_done() == 1'b0) begin
               trans_desc.wait_for_done();
            end
            if (trans_desc.get_cmd() inside { `EVL_JTAG_CMDS_WITH_READ_DATA }) begin
               cmd_length = int'(trans_desc.get_paddr());
               result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(0))) | result;
               result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(1))) | result;
               result = evl_tcl_set_int_res(thread_num, 2, longint'(trans_desc.get_read_dword(2))) | result;
               result = evl_tcl_set_int_res(thread_num, 3, longint'(trans_desc.get_read_dword(3))) | result;
               result = evl_tcl_set_int_res(thread_num, 4, longint'(trans_desc.get_read_dword(4))) | result;
               result = evl_tcl_set_int_res(thread_num, 5, longint'(trans_desc.get_read_dword(5))) | result;
               result = evl_tcl_set_int_res(thread_num, 6, longint'(trans_desc.get_read_dword(6))) | result;
               result = evl_tcl_set_int_res(thread_num, 7, longint'(trans_desc.get_read_dword(7))) | result;
            end
            else if (trans_desc.get_cmd() inside { ATOMIC }) begin
               int atomic_conf;

               atomic_conf = int'(trans_desc.get_bus_atomic_conf());
               if (atomic_conf[4:4] == 1'b1) begin
                  if (trans_desc.get_bus_atomic_offset() < 32) begin
                     result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(0))) | result;
                     result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(1))) | result;
                     result = evl_tcl_set_int_res(thread_num, 2, longint'(trans_desc.get_read_dword(2))) | result;
                     result = evl_tcl_set_int_res(thread_num, 3, longint'(trans_desc.get_read_dword(3))) | result;
                  end
                  else begin
                     result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(4))) | result;
                     result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(5))) | result;
                     result = evl_tcl_set_int_res(thread_num, 2, longint'(trans_desc.get_read_dword(6))) | result;
                     result = evl_tcl_set_int_res(thread_num, 3, longint'(trans_desc.get_read_dword(7))) | result;
                  end
               end
               else begin
                  evl_paddr_t atomic_offset;

                  atomic_offset = int'(trans_desc.get_bus_atomic_offset() >> `EVL_DWORD_ADDR_LSB) & 7;
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(atomic_offset))) | result;
               end
            end
            else begin
               cmd_length = trans_desc.get_cmd_length();
               if (cmd_length < 2) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_byte(-1))) | result;
               end
               else if (cmd_length < 4) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_hword(-1))) | result;
               end
               else if (cmd_length < 8) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_word(-1))) | result;
               end
               else if (cmd_length < 16) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(-1))) | result;
               end
               else if (cmd_length < 32) begin
                  int offset;

                  offset = (int'(trans_desc.get_paddr() & ~const_line_paddr_mask) >> `EVL_QWORD_ADDR_LSB) << 1;
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(offset))) | result;
                  result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(offset + 1))) | result;
               end
               else if (cmd_length < 64) begin
                  int offset;

                  offset = (int'(trans_desc.get_paddr() & ~const_line_paddr_mask) >> `EVL_OWORD_ADDR_LSB) << 2;
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(offset))) | result;
                  result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(offset + 1))) | result;
                  result = evl_tcl_set_int_res(thread_num, 2, longint'(trans_desc.get_read_dword(offset + 2))) | result;
                  result = evl_tcl_set_int_res(thread_num, 3, longint'(trans_desc.get_read_dword(offset + 3))) | result;
               end
               else begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(trans_desc.get_read_dword(0))) | result;
                  result = evl_tcl_set_int_res(thread_num, 1, longint'(trans_desc.get_read_dword(1))) | result;
                  result = evl_tcl_set_int_res(thread_num, 2, longint'(trans_desc.get_read_dword(2))) | result;
                  result = evl_tcl_set_int_res(thread_num, 3, longint'(trans_desc.get_read_dword(3))) | result;
                  result = evl_tcl_set_int_res(thread_num, 4, longint'(trans_desc.get_read_dword(4))) | result;
                  result = evl_tcl_set_int_res(thread_num, 5, longint'(trans_desc.get_read_dword(5))) | result;
                  result = evl_tcl_set_int_res(thread_num, 6, longint'(trans_desc.get_read_dword(6))) | result;
                  result = evl_tcl_set_int_res(thread_num, 7, longint'(trans_desc.get_read_dword(7))) | result;
               end
            end
            `evl_log_mdesc(UVM_HIGH, VMOD_TCL, m_cur_agent.get_abstract_name(), trans_desc, $sformatf("getting read data for %0s", trans_desc.sprint_obj()))
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "entry_has_write_err" Evaluation
      //
      else if (tcl_command == TCL_entry_has_write_err) begin
         int trans_desc_index;

         if (evl_tcl_get_arg_count(thread_num) == 0) begin
            trans_desc_index = 0;
         end
         else begin
            trans_desc_index = int'(evl_tcl_get_int_arg(thread_num, 0));
         end
         if ((trans_desc_index < 0) || (trans_desc_index >= m_write_trans_descs.size())) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         else begin
            evl_trans_desc trans_desc;

            trans_desc = m_write_trans_descs[trans_desc_index];
            if (trans_desc.is_done() == 1'b0) begin
               trans_desc.wait_for_done();
            end
            if (trans_desc.bus_rsp_has_error() == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
            end
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "NOP" Evaluation
      //
      else if (tcl_command == TCL_NOP) begin
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nNOP;", $realtime));
         end
         stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
         stimulus_seq.issue(trans_desc, NOP, 0);
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "BARRIER" Evaluation
      //
      else if (tcl_command == TCL_BARRIER) begin
         int         arg_count;
         evl_paddr_t limit;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 0) begin
            limit = evl_paddr_t'(0);
         end
         else begin
            limit = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         end
         if (limit < 0) begin
            limit = evl_paddr_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nBARRIER %0d;", $realtime, limit));
         end
         stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
         stimulus_seq.blocking_issue(trans_desc, BARRIER, limit);
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "LD" Evaluation
      //
      else if (tcl_command == TCL_LD) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.subopcode_dest = evl_bus_subopcode_t'(m_subopcode_dest);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nLD 0x%x;", $realtime, paddr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, LD, paddr, m_cmd_length, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, LD, paddr, m_cmd_length, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "LD_COOP" Evaluation
      //
      else if (tcl_command == TCL_LD_COOP) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr                  = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr.coop_target = evl_bus_coop_target_t'(evl_tcl_get_int_arg(thread_num, 1));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nLD_COOP 0x%x 0x%0x;", $realtime, paddr, trans_attr.coop_target));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, LD_COOP, paddr, m_cmd_length, m_req_source, { evl_dword_t'(0) }, m_delay_wrapper, evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, LD_COOP, paddr, m_cmd_length, m_req_source, { evl_dword_t'(0) }, m_delay_wrapper, evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "LD_NO_MOD" Evaluation
      //
      else if (tcl_command == TCL_LD_NO_MOD) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.subopcode_dest = evl_bus_subopcode_t'(m_subopcode_dest);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nLD_NO_MOD 0x%x;", $realtime, paddr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, LD_NO_MOD, paddr, m_cmd_length, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, LD_NO_MOD, paddr, m_cmd_length, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "ST" Evaluation
      //
      else if (tcl_command == TCL_ST) begin
         int              limit;
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         evl_dword_t      dwords[`EVL_LINE_DWORDS];

         limit      = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.subopcode_dest = evl_bus_subopcode_t'(m_subopcode_dest);
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("ST 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, ST, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, ST, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "ST_WA" Evaluation
      //
      else if (tcl_command == TCL_ST_WA) begin
         int         limit;
         evl_paddr_t paddr;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         limit = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("ST_WA 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, ST_WA, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, ST_WA, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "ST_NO_MOD" Evaluation
      //
      else if (tcl_command == TCL_ST_NO_MOD) begin
         int              limit;
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         evl_dword_t      dwords[`EVL_LINE_DWORDS];

         limit      = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.subopcode_dest = evl_bus_subopcode_t'(m_subopcode_dest);
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("ST_NO_MOD 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, ST_NO_MOD, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, ST_NO_MOD, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "ST_DBG_NOC" Evaluation
      //
      else if (tcl_command == TCL_ST_DBG_NOC) begin
         int              limit;
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         evl_dword_t      dwords[`EVL_LINE_DWORDS];

         limit      = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.subopcode_dest = evl_bus_subopcode_t'(m_subopcode_dest);
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("ST_DBG_NOC 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, ST_DBG_NOC, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, ST_DBG_NOC, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_LOCK" Evaluation
      //
      else if (tcl_command == TCL_CACHE_LOCK) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_LOCK 0x%x;", $realtime, paddr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_LOCK, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_LOCK, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end // if (tcl_command == TCL_CACHE_LOCK)

      //
      // "CACHE_UNLOCK" Evaluation
      //
      else if (tcl_command == TCL_CACHE_UNLOCK) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.cop_final_state = bit'(evl_tcl_get_int_arg(thread_num,1));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_UNLOCK 0x%x %0d;", $realtime, paddr, trans_attr.cop_final_state));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_UNLOCK, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_UNLOCK, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end // if (tcl_command == TCL_CACHE_UNLOCK)

      //
      // "CACHE_EVICT" Evaluation
      //
      else if (tcl_command == TCL_CACHE_EVICT) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         string           start_level_str;
         string           dest_level_str;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         start_level_str = evl_tcl_get_str_arg(thread_num, 1);
         dest_level_str  = evl_tcl_get_str_arg(thread_num, 2);
         start_level_str = start_level_str.tolower();
         dest_level_str  = dest_level_str.tolower();
         if (start_level_str == "l2") begin
            trans_attr.cop_start_level = 1;
         end
         else if (start_level_str == "l3") begin
            trans_attr.cop_start_level = 2;
         end
         else begin
            trans_attr.cop_start_level = 0;
         end
         if (dest_level_str == "l2") begin
            trans_attr.cop_dest_level = 1;
         end
         else if (dest_level_str == "l3") begin
            trans_attr.cop_dest_level = 2;
         end
         else begin
            trans_attr.cop_dest_level = 3;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_EVICT 0x%x %0s %0s;", $realtime, paddr, start_level_str, dest_level_str));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_EVICT, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_EVICT, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_EVICT_TO_MEM" Evaluation
      //
      else if (tcl_command == TCL_CACHE_EVICT_TO_MEM) begin
         int         limit;
         evl_paddr_t paddr;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         limit = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("CACHE_EVICT_TO_MEM 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_EVICT_TO_MEM, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_EVICT_TO_MEM, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_FLUSH" Evaluation
      //
      else if (tcl_command == TCL_CACHE_FLUSH) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         string           start_level_str;
         string           dest_level_str;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         start_level_str = evl_tcl_get_str_arg(thread_num, 1);
         dest_level_str  = evl_tcl_get_str_arg(thread_num, 2);
         start_level_str = start_level_str.tolower();
         dest_level_str  = dest_level_str.tolower();
         if (start_level_str == "l2") begin
            trans_attr.cop_start_level = 1;
         end
         else if (start_level_str == "l3") begin
            trans_attr.cop_start_level = 2;
         end
         else begin
            trans_attr.cop_start_level = 0;
         end
         if (dest_level_str == "l2") begin
            trans_attr.cop_dest_level = 1;
         end
         else if (dest_level_str == "l3") begin
            trans_attr.cop_dest_level = 2;
         end
         else begin
            trans_attr.cop_dest_level = 3;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_FLUSH 0x%x %0s %0s;", $realtime, paddr, start_level_str, dest_level_str));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_FLUSH, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_FLUSH, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_FLUSH_TO_MEM" Evaluation
      //
      else if (tcl_command == TCL_CACHE_FLUSH_TO_MEM) begin
         int         limit;
         evl_paddr_t paddr;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         limit = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("CACHE_FLUSH_TO_MEM 0x%x", paddr);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_FLUSH_TO_MEM, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_FLUSH_TO_MEM, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, m_qword_en, evl_trans_attr_s'(0), m_cache_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_PREFETCH" Evaluation
      //
      else if (tcl_command == TCL_CACHE_PREFETCH) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         string           dest_level_str;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         dest_level_str = evl_tcl_get_str_arg(thread_num, 1);
         dest_level_str = dest_level_str.tolower();
         if (dest_level_str == "l2") begin
            trans_attr.cop_dest_level = 1;
         end
         else if (dest_level_str == "l3") begin
            trans_attr.cop_dest_level = 2;
         end
         else begin
            trans_attr.cop_dest_level = 3;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_PREFETCH 0x%x %0s;", $realtime, paddr, dest_level_str));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_PREFETCH, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_PREFETCH, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CACHE_SCP_FILL" Evaluation
      //
      else if (tcl_command == TCL_CACHE_SCP_FILL) begin
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;

         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.scp_fill_addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCACHE_SCP_FILL 0x%x 0x%x;", $realtime, paddr, trans_attr.scp_fill_addr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CACHE_SCP_FILL, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CACHE_SCP_FILL, paddr, `EVL_LINE_BYTES, m_req_source, {}, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "MESSAGE" Evaluation
      //
      else if (tcl_command == TCL_MESSAGE) begin
         int              limit;
         evl_paddr_t      paddr;
         evl_trans_attr_s trans_attr;
         evl_dword_t      dwords[`EVL_LINE_DWORDS];

         limit      = (m_cmd_length + `EVL_DWORD_BYTES - 1) / `EVL_DWORD_BYTES;
         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.message_conf = evl_bus_message_conf_t'(evl_tcl_get_int_arg(thread_num, 1));
         if (limit > `EVL_LINE_DWORDS) begin
            limit = `EVL_LINE_DWORDS;
         end
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 2));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("MESSAGE 0x%x %0d", paddr, trans_attr.message_conf);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, MESSAGE, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, MESSAGE, paddr, m_cmd_length, m_req_source, dwords, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // ATOMIC Evaluation
      //
      else if (tcl_command == TCL_ATOMIC) begin
         evl_paddr_t      paddr;
         int              atomic_cmd_length;
         evl_trans_attr_s trans_attr;
         evl_dword_t      dwords[4];

         atomic_cmd_length = 32;
         paddr      = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         trans_attr = evl_trans_attr_s'(0);
         trans_attr.atomic_conf    = evl_bus_atomic_conf_t'(evl_tcl_get_int_arg(thread_num, 1));
         trans_attr.atomic_transid = m_atomic_transid;
         dwords[0] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 2));
         dwords[1] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 3));
         dwords[2] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 4));
         dwords[3] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 5));
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nATOMIC 0x%x %0d 0x%x 0x%x 0x%x 0x%x;", $realtime, paddr, trans_attr.atomic_conf, dwords[0], dwords[1], dwords[2], dwords[3]));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, ATOMIC, paddr, atomic_cmd_length, m_req_source, { dwords }, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, ATOMIC, paddr, atomic_cmd_length, m_req_source, { dwords }, m_delay_wrapper, ~evl_line_qword_en_t'(0), trans_attr, m_cache_attr, m_qos, m_vc, m_thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         m_write_trans_descs.push_front(trans_desc);
         while ((m_write_trans_descs.size() >= m_write_limit) && (m_write_limit > 0)) begin
            m_write_trans_descs.delete(m_write_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "IFETCH" Evaluation
      //
      else if (tcl_command == TCL_IFETCH) begin
         evl_thread_id_t  thread_id;
         evl_vaddr_t      vaddr;
         evl_trans_attr_s trans_attr;

         if (evl_tcl_get_arg_count(thread_num) == 2) begin
            thread_id = evl_thread_id_t'(evl_tcl_get_int_arg(thread_num, 0));
            vaddr     = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, 1));
         end
         else begin
            thread_id = m_thread_id;
            vaddr     = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         end
         trans_attr = evl_trans_attr_s'(0);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nIFETCH %0d 0x%x;", $realtime, thread_id, vaddr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, IFETCH, vaddr, `EVL_OWORD_BYTES, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, IFETCH, vaddr, `EVL_OWORD_BYTES, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "REFETCH" Evaluation
      //
      else if (tcl_command == TCL_REFETCH) begin
         evl_thread_id_t  thread_id;
         evl_vaddr_t      vaddr;
         evl_trans_attr_s trans_attr;

         if (evl_tcl_get_arg_count(thread_num) == 2) begin
            thread_id = evl_thread_id_t'(evl_tcl_get_int_arg(thread_num, 0));
            vaddr     = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, 1));
         end
         else begin
            thread_id = m_thread_id;
            vaddr     = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, 0));
         end
         trans_attr = evl_trans_attr_s'(0);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nREFETCH %0d 0x%x;", $realtime, thread_id, vaddr));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, REFETCH, vaddr, `EVL_OWORD_BYTES, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, REFETCH, vaddr, `EVL_OWORD_BYTES, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "CANCEL" Evaluation
      //
      else if (tcl_command == TCL_CANCEL) begin
         evl_thread_id_t  thread_id;
         evl_trans_attr_s trans_attr;

         if (evl_tcl_get_arg_count(thread_num) == 1) begin
            thread_id = evl_thread_id_t'(evl_tcl_get_int_arg(thread_num, 0));
         end
         else begin
            thread_id = m_thread_id;
         end
         trans_attr = evl_trans_attr_s'(0);
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nCANCEL %0d;", $realtime, thread_id));
         end
         if (m_blocking == 1'b1) begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.blocking_issue(trans_desc, CANCEL, 0, 1, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         else begin
            stimulus_seq.set_drop_on_conflict(m_drop_on_conflict);
            stimulus_seq.issue(trans_desc, CANCEL, 0, 1, m_req_source, {}, m_delay_wrapper,, trans_attr, m_cache_attr, m_qos, m_vc, thread_id);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "JTAG_STEP" Evaluation
      //
      else if (tcl_command == TCL_JTAG_STEP) begin
         int         length;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         length    = int'(evl_tcl_get_int_arg(thread_num, 0)) & int'(1);
         dwords[0] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 1)) & evl_dword_t'(1);
         for (int ii = 1; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("JTAG_STEP %0d %0d", length, dwords[0]);
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         stimulus_seq.blocking_issue(trans_desc, JTAG_STEP, evl_paddr_t'(length), 0, 0, dwords, m_delay_wrapper);
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "JTAG_RESET" Evaluation
      //
      else if (tcl_command == TCL_JTAG_RESET) begin
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nJTAG_RESET;", $realtime));
         end
         stimulus_seq.blocking_issue(trans_desc, JTAG_RESET, 0);
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "JTAG_RUN_TEST" Evaluation
      //
      else if (tcl_command == TCL_JTAG_RUN_TEST) begin
         if (m_tcl_handler.perform_replay_write() != 0) begin
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nJTAG_RUN_TEST;", $realtime));
         end
         stimulus_seq.blocking_issue(trans_desc, JTAG_RUN_TEST, 0);
         result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "SELECT_CHAIN" Evaluation
      //
      else if (tcl_command == TCL_SELECT_CHAIN) begin
         int         length;
         int         limit;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         length = int'(evl_tcl_get_int_arg(thread_num, 0));
         if (length < 0) begin
            length = 1;
         end
         else if (length > 512) begin
            length = 512;
         end
         limit = (length + (`EVL_DWORD_BYTES * 3) - 1) / (`EVL_DWORD_BYTES * 3);
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
//          dwords[ii] = ~evl_dword_t'(0);
            dwords[ii] =  evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            message = $sformatf("SELECT_CHAIN %0d", length);
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         stimulus_seq.blocking_issue(trans_desc, SELECT_CHAIN, evl_paddr_t'(length), 0, 0, dwords, m_delay_wrapper);
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "JTAG_READ", "JTAG_RD", or "JTAG_RD_PARTIAL" Evaluation
      //
      else if (tcl_command inside { TCL_JTAG_READ, TCL_JTAG_RD, TCL_JTAG_RD_PARTIAL }) begin
         int length;

         length = int'(evl_tcl_get_int_arg(thread_num, 0));
         if (length < 0) begin
            length = 1;
         end
         else if (length > 512) begin
            length = 512;
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            if (tcl_command inside { TCL_JTAG_READ, TCL_JTAG_RD }) begin
               m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nJTAG_RD %0d;", $realtime, length));
            end
            else begin
               m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\nJTAG_RD_PARTIAL %0d;", $realtime, length));
            end
         end
         if (tcl_command inside { TCL_JTAG_READ, TCL_JTAG_RD }) begin
            stimulus_seq.blocking_issue(trans_desc, JTAG_RD, evl_paddr_t'(length), 0, 0, {}, m_delay_wrapper);
         end
         else begin
            stimulus_seq.blocking_issue(trans_desc, JTAG_RD_PARTIAL, evl_paddr_t'(length), 0, 0, {}, m_delay_wrapper);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "JTAG_WRITE", "JTAG_WR", or "JTAG_WR_PARTIAL" Evaluation
      //
      else if (tcl_command inside { TCL_JTAG_WRITE, TCL_JTAG_WR, TCL_JTAG_WR_PARTIAL }) begin
         int         length;
         int         limit;
         evl_dword_t dwords[`EVL_LINE_DWORDS];

         length = int'(evl_tcl_get_int_arg(thread_num, 0));
         if (length < 0) begin
            length = 1;
         end
         else if (length > 512) begin
            length = 512;
         end
         limit = (length + (`EVL_DWORD_BYTES * 3) - 1) / (`EVL_DWORD_BYTES * 3);
         for (int ii = 0; ii < limit; ii++) begin
            dwords[ii] = evl_dword_t'(evl_tcl_get_int_arg(thread_num, ii + 1));
         end
         for (int ii = limit; ii < `EVL_LINE_DWORDS; ii++) begin
            dwords[ii] = ~evl_dword_t'(0);
         end
         if (m_tcl_handler.perform_replay_write() != 0) begin
            string message;

            if (tcl_command inside { TCL_JTAG_WRITE, TCL_JTAG_WR }) begin
               message = $sformatf("JTAG_WR %0d", length);
            end
            else begin
               message = $sformatf("JTAG_WR_PARTIAL %0d", length);
            end
            for (int ii = 0; ii < limit; ii++) begin
               message = $sformatf("%0s 0x%x", message, dwords[ii]);
            end
            m_tcl_handler.replay_file_write($sformatf("wait_until_time %t;\n%s;", $realtime, message));
         end
         if (tcl_command inside { TCL_JTAG_WRITE, TCL_JTAG_WR }) begin
            stimulus_seq.blocking_issue(trans_desc, JTAG_WR, evl_paddr_t'(length), 0, 0, dwords, m_delay_wrapper);
         end
         else begin
            stimulus_seq.blocking_issue(trans_desc, JTAG_WR_PARTIAL, evl_paddr_t'(length), 0, 0, dwords, m_delay_wrapper);
         end
         m_read_trans_descs.push_front(trans_desc);
         while ((m_read_trans_descs.size() >= m_read_limit) && (m_read_limit > 0)) begin
            m_read_trans_descs.delete(m_read_limit - 1);
         end
         if (trans_desc.is_done() == 1'b1) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(1));
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end
      return;
   endtask : parse_tcl_command


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tcl_mem_seq_parser");
      super.new(name_in);
   endfunction : new

endclass : evl_tcl_mem_seq_parser

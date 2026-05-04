   `ifdef EVL_SIMULATION
      import "DPI-C" context function void evl_tcl_add_include_path (
         input string path_name_in);

      import "DPI-C" context function string evl_tcl_get_root_name (
         input string name_in);

      import "DPI-C" context function string evl_tcl_find_file (
         input string file_name_in,
         input int    use_include_paths);

      import "DPI-C" context function int evl_tcl_prepare_tcl_script (
         input int    thread_id,
         input string file_name);

      import "DPI-C" context function int evl_tcl_add_cmd_enc (
         input int    thread_index_in,
         input string cmd_name,
         input int    min_arg_count,
         input int    max_arg_count,
         input string return_type,
         input string parse_type);

      import "DPI-C" context function int evl_tcl_start_tcl_script (
         input int thread_index_in);

      import "DPI-C" context function int evl_tcl_eval_string (
         input int    thread_index_in,
         input string eval_str);

      import "DPI-C" context function int evl_tcl_get_next_cmd (
         input int thread_index_in);

      import "DPI-C" context function int evl_tcl_get_cmd_encoding (
         input int thread_index_in);

      import "DPI-C" context function int evl_tcl_get_arg_count (
         input int thread_index_in);

      import "DPI-C" context function int evl_tcl_arg_is_string (
         input int thread_index_in,
         input int arg_index_in);

      import "DPI-C" context function string evl_tcl_get_str_arg (
         input int thread_index_in,
         input int arg_index_in);

      import "DPI-C" context function longint evl_tcl_get_int_arg (
         input int thread_index_in,
         input int arg_index_in);

      import "DPI-C" context function int evl_tcl_parse_agent_name (
         input int    thread_index_in,
         input string agent_name_in,
         input int    default_is_0_in);

      import "DPI-C" context function int evl_tcl_set_str_res (
         input int    thread_index_in,
         input int    res_index_in,
         input string str_res);

      import "DPI-C" context function int evl_tcl_set_int_res (
         input int     thread_index_in,
         input int     res_index_in,
         input longint value);

      import "DPI-C" context function int evl_tcl_open_replay_file (
         input int    thread_index_in,
         input string file_name_in);

      import "DPI-C" context function int evl_tcl_close_replay_file (
         input int    thread_index_in);

      import "DPI-C" context function int evl_tcl_replay_file_write (
         input int    thread_index_in,
         input string message_in);

      import "DPI-C" context function string evl_tcl_get_net_binary_value (
         input string path);

      import "DPI-C" context function string evl_tcl_get_net_hex_value (
         input string path);

      import "DPI-C" context function string evl_tcl_get_next_plus_arg ();

      import "DPI-C" context function longint evl_tcl_get_time_of_day ();

      import "DPI-C" context function longint evl_tcl_get_cur_time ();

      import "DPI-C" context function string evl_tcl_get_hhmmss ();

      import "DPI-C" context function longint evl_tcl_get_vpi_time ();

      import "DPI-C" context function string evl_tcl_get_env (
         input string message_in);

      import "DPI-C" context function int evl_tcl_set_env (
         input string env_name,
         input string env_value);

      import "DPI-C" context function int evl_tcl_system (
         input string message_in);

      import "DPI-C" context function void evl_tcl_scan_module (
         input string hierarchy_name,
         input int    depth);
   `endif // ifdef EVL_SIMULATION

   `ifdef ENABLE_BEMU
      import "DPI-C" context function void cosim_object_delete ();
      import "DPI-C" context function void cosim_set_verbosity (
         input int bemu_log_level);
   `endif // ifdef ENABLE_BEMU

   `ifdef ET_SIMULATION
      import "DPI-C" context function int et_open_dpi_event_file (
         input string file_name);

      import "DPI-C" context function void et_close_dpi_event_files ();

      import "DPI-C" context function void et_write_dpi_event (
         input int                         fd_in,
         input bit [`ET_DPI_PKT_WIDTH-1:0] packed_data);

      import "DPI-C" context function longint et_get_time_of_day ();

`ifndef VERILATOR
      import "DPI-C" context function longint et_get_cur_time ();

      import "DPI-C" context function longint et_get_vpi_time ();
`endif
      import "DPI-C" context function string et_get_env (
         input string message_in);
   `endif // ifdef ET_SIMULATION

//--------------------------------------------------------------------------------------------------
//
// Class: evl_tcl_default_parser
//
class evl_tcl_default_parser extends evl_tcl_cmd_parser;

   `uvm_object_utils(evl_tcl_default_parser)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_l2_index_msb)
   `evl_param_key(pk_l3_index_msb)
   `evl_param_key(pk_scp_index_msb)
   `evl_param_key(pk_l2_set_size)
   `evl_param_key(pk_l3_set_size)
   `evl_param_key(pk_scp_set_size)


   //-----------------------------------------------------------------------------------------------
   //
   // Local Variables and Objects
   //
   int             m_wait_timeout     = 0;
   string          m_wait_timeout_err = "";
   evl_paddr_t     m_l2_addrs[$];
   evl_paddr_t     m_l3_addrs[$];
   evl_bus_watch   m_bus_watch;
   evl_instr_watch m_instr_watch;


   //-----------------------------------------------------------------------------------------------
   //
   // Local Types
   //
   // The following enumerated types are local to this file.  These are the command encodings
   // supported by this parser.  To simplify adding commands, note that the names are the string
   // names with the "TCL_" prefix.
   //
   typedef enum int {
         TCL_None,
         TCL_print_number,
         TCL_gen_replay_file,
         TCL_evl_log,
         TCL_exit_script,
         TCL_finish_sim,
         TCL_dut_error,
         TCL_dut_warning,
         TCL_enable_dump,
         TCL_disable_dump,
         TCL_enable_assertions,
         TCL_disable_assertions,
         TCL_use_system_clk,
         TCL_use_local_clk,
         TCL_set_verbosity,
         TCL_set_error_intensity,
         TCL_clr_error_intensity,
         TCL_get_time,
         TCL_get_vpi_time,
         TCL_get_cycle,
         TCL_wait_time,
         TCL_wait_until_time,
         TCL_wait_cycles,
         TCL_get_agent_count,
         TCL_agent_match,
         TCL_have_agent,
         TCL_select_agent,
         TCL_select_target_agent,
         TCL_set_target_agent,
         TCL_get_cur_agent,
         TCL_get_agent_info,
         TCL_get_target_agent_info,
         TCL_get_shire_id,
         TCL_add_agent_delay,
         TCL_set_agent_delay,
         TCL_reset_agent_delay,
         TCL_set_agent_param,
         TCL_get_agent_param,
         TCL_wait_for_agent_param,
         TCL_wait_for_agent_param_value,
         TCL_wait_for_agent_param_not_value,
         TCL_wait_for_agent_param_range,
         TCL_wait_for_agent_param_not_range,
         TCL_set_agent_rtl_param,
         TCL_get_agent_rtl_param,
         TCL_wait_for_agent_rtl_param,
         TCL_wait_for_agent_rtl_param_value,
         TCL_wait_for_agent_rtl_param_not_value,
         TCL_wait_for_agent_rtl_param_range,
         TCL_wait_for_agent_rtl_param_not_range,
         TCL_wait_for_phase,
         TCL_wait_for_bus_addr,
         TCL_wait_for_pc,
         TCL_wait_for_instr,
         TCL_get_last_pc,
         TCL_get_last_instr,
         TCL_get_last_source,
         TCL_add_agent_addr_region,
         TCL_set_agent_addr_region,
         TCL_clr_agent_addr_region,
         TCL_terminating_script,
         TCL_background_script,
         TCL_display_objections,
         TCL_get_num_banks,
         TCL_get_num_subbanks,
         TCL_get_l2_bank_sel_msb,
         TCL_get_l3_bank_sel_msb,
         TCL_get_l3_shire_sel_msb,
         TCL_get_shire_sel_msb,
         TCL_get_scp_shire_sel_lsb,
         TCL_get_scp_shire_sel_msb,
         TCL_get_scp_mmio_addr,
         TCL_get_scp_mmio_mask,
         TCL_get_scp_index_mask,
         TCL_get_l2_index_msb,
         TCL_get_l3_index_msb,
         TCL_get_scp_index_msb,
         TCL_get_scp_index_lsb,
         TCL_get_l2_set_size,
         TCL_get_l3_set_size,
         TCL_get_scp_set_size,
         TCL_get_l2_bank,
         TCL_get_l3_bank,
         TCL_get_scp_bank,
         TCL_get_l2_subbank,
         TCL_get_l3_subbank,
         TCL_get_scp_subbank,
         TCL_get_l2_shire,
         TCL_get_l3_shire,
         TCL_get_scp_shire,
         TCL_convert_to_l2_address,
         TCL_convert_to_l3_address,
         TCL_get_l2_index,
         TCL_get_l3_index,
         TCL_get_scp_index,
         TCL_get_l2_addresses,
         TCL_get_l3_addresses,
         TCL_get_scp_addresses,
         TCL_get_l2_aliases,
         TCL_get_l3_aliases,
         TCL_clr_l2_addresses,
         TCL_clr_l3_addresses,
         TCL_get_mem_addresses,
         TCL_create_semaphore,
         TCL_put_semaphore,
         TCL_get_semaphore,
         TCL_create_sync_count,
         TCL_incr_sync_count,
         TCL_decr_sync_count,
         TCL_get_sync_count,
         TCL_subscribe,
         TCL_send_message,
         TCL_get_message,
         TCL_get_blocking_message,
         TCL_get_cache_set,
         TCL_get_cache_bank,
         TCL_get_cache_subbank,
         TCL_get_cache_tag,
         TCL_get_cache_data,
         TCL_set_cache_bit_err,
         TCL_clr_cache_bit_err,
         TCL_get_net_binary_value,
         TCL_get_net_hex_value,
         TCL_set_wait_timeout,
         TCL_raise_objection,
         TCL_drop_objection,
         TCL_scan_module,
         TCL_use_pre_reset_phase,
         TCL_use_reset_phase,
         TCL_use_post_reset_phase,
         TCL_use_pre_configure_phase,
         TCL_use_configure_phase,
         TCL_use_post_configure_phase,
         TCL_use_pre_main_phase,
         TCL_use_main_phase,
         TCL_use_post_main_phase
      } evl_tcl_command_t;


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

      result |= add_my_tcl_command(TCL_print_number,                       thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_gen_replay_file,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_evl_log,                            thread_num, 1, 3, "int");
      result |= add_my_tcl_command(TCL_exit_script,                        thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_finish_sim,                         thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_dut_error,                          thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_dut_warning,                        thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_enable_dump,                        thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_disable_dump,                       thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_enable_assertions,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_disable_assertions,                 thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_system_clk,                     thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_local_clk,                      thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_set_verbosity,                      thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_set_error_intensity,                thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_clr_error_intensity,                thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_get_time,                           thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_vpi_time,                       thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_cycle,                          thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_wait_time,                          thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_wait_until_time,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_wait_cycles,                        thread_num, 1, 3, "int");
      result |= add_my_tcl_command(TCL_get_agent_count,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_agent_match,                        thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_have_agent,                         thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_select_agent,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_select_target_agent,                thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_set_target_agent,                   thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_get_cur_agent,                      thread_num, 0, 0, "string");
      result |= add_my_tcl_command(TCL_get_agent_info,                     thread_num, 0, 0, "string list");
      result |= add_my_tcl_command(TCL_get_target_agent_info,              thread_num, 0, 0, "string list");
      result |= add_my_tcl_command(TCL_get_shire_id,                       thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_add_agent_delay,                    thread_num, 2, 5, "int");
      result |= add_my_tcl_command(TCL_set_agent_delay,                    thread_num, 2, 5, "int");
      result |= add_my_tcl_command(TCL_reset_agent_delay,                  thread_num, 1, 4, "int");
      result |= add_my_tcl_command(TCL_set_agent_param,                    thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_get_agent_param,                    thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_param,               thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_param_value,         thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_param_not_value,     thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_param_range,         thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_param_not_range,     thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_set_agent_rtl_param,                thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_get_agent_rtl_param,                thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_rtl_param,           thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_rtl_param_value,     thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_rtl_param_not_value, thread_num, 2, 3, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_rtl_param_range,     thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_wait_for_agent_rtl_param_not_range, thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_wait_for_bus_addr,                  thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_phase,                     thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_wait_for_pc,                        thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_wait_for_instr,                     thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_last_pc,                        thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_last_instr,                     thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_last_source,                    thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_add_agent_addr_region,              thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_set_agent_addr_region,              thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_clr_agent_addr_region,              thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_terminating_script,                 thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_background_script,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_display_objections,                 thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_num_banks,                      thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_num_subbanks,                   thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_l2_bank_sel_msb,                thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_l3_bank_sel_msb,                thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_l3_shire_sel_msb,               thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_shire_sel_msb,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_scp_shire_sel_lsb,              thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_scp_shire_sel_msb,              thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_scp_mmio_addr,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_scp_mmio_mask,                  thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_scp_index_mask,                 thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_get_l2_index_msb,                   thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_l3_index_msb,                   thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_scp_index_msb,                  thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_scp_index_lsb,                  thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_l2_set_size,                    thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_l3_set_size,                    thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_scp_set_size,                   thread_num, 0, 1, "int");
      result |= add_my_tcl_command(TCL_get_l2_bank,                        thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l3_bank,                        thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_scp_bank,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l2_subbank,                     thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l3_subbank,                     thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_scp_subbank,                    thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l2_shire,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l3_shire,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_scp_shire,                      thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_convert_to_l2_address,              thread_num, 1, 4, "int");
      result |= add_my_tcl_command(TCL_convert_to_l3_address,              thread_num, 1, 4, "int");
      result |= add_my_tcl_command(TCL_get_l2_index,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l3_index,                       thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_scp_index,                      thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_get_l2_addresses,                   thread_num, 0, 8, "int list");
      result |= add_my_tcl_command(TCL_get_l3_addresses,                   thread_num, 0, 8, "int list");
      result |= add_my_tcl_command(TCL_get_scp_addresses,                  thread_num, 0, 7, "int list");
      result |= add_my_tcl_command(TCL_get_l2_aliases,                     thread_num, 0, 1, "int list");
      result |= add_my_tcl_command(TCL_get_l3_aliases,                     thread_num, 0, 1, "int list");
      result |= add_my_tcl_command(TCL_clr_l2_addresses,                   thread_num, 0, 0, "int list");
      result |= add_my_tcl_command(TCL_clr_l3_addresses,                   thread_num, 0, 0, "int list");
      result |= add_my_tcl_command(TCL_get_mem_addresses,                  thread_num, 0, 2, "int list");
      result |= add_my_tcl_command(TCL_create_semaphore,                   thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_put_semaphore,                      thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_get_semaphore,                      thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_create_sync_count,                  thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_incr_sync_count,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_decr_sync_count,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_get_sync_count,                     thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_subscribe,                          thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_send_message,                       thread_num, 2, 4, "int");
      result |= add_my_tcl_command(TCL_get_message,                        thread_num, 2, 2, "int list");
      result |= add_my_tcl_command(TCL_get_blocking_message,               thread_num, 2, 2, "int list");
      result |= add_my_tcl_command(TCL_get_cache_set,                      thread_num, 2, 4, "int");
      result |= add_my_tcl_command(TCL_get_cache_bank,                     thread_num, 2, 4, "int");
      result |= add_my_tcl_command(TCL_get_cache_subbank,                  thread_num, 2, 4, "int");
      result |= add_my_tcl_command(TCL_get_cache_tag,                      thread_num, 2, 4, "int");
      result |= add_my_tcl_command(TCL_get_cache_data,                     thread_num, 2, 4, "int list");
      result |= add_my_tcl_command(TCL_set_cache_bit_err,                  thread_num, 3, 6, "int");
      result |= add_my_tcl_command(TCL_clr_cache_bit_err,                  thread_num, 3, 4, "int");
      result |= add_my_tcl_command(TCL_get_net_binary_value,               thread_num, 1, 1, "string");
      result |= add_my_tcl_command(TCL_get_net_hex_value,                  thread_num, 1, 1, "string");
      result |= add_my_tcl_command(TCL_set_wait_timeout,                   thread_num, 1, 2, "int");
      result |= add_my_tcl_command(TCL_raise_objection,                    thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_drop_objection,                     thread_num, 1, 1, "int");
      result |= add_my_tcl_command(TCL_scan_module,                        thread_num, 2, 2, "int");
      result |= add_my_tcl_command(TCL_use_pre_reset_phase,                thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_reset_phase,                    thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_post_reset_phase,               thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_pre_configure_phase,            thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_configure_phase,                thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_post_configure_phase,           thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_pre_main_phase,                 thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_main_phase,                     thread_num, 0, 0, "int");
      result |= add_my_tcl_command(TCL_use_post_main_phase,                thread_num, 0, 0, "int");
      return result;
   endfunction : add_tcl_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_initial_commands
   //
   virtual function int add_initial_commands(input int thread_num);
      int    limit;
      int    result;
      string plus_arg_names[];
      string plus_arg_roots[];
      string plus_arg_vals[];

      m_global_attrs.extract_plus_args(plus_arg_names, plus_arg_roots, plus_arg_vals);
      limit  = plus_arg_names.size();
      result = evl_tcl_eval_string(thread_num, "set tcl_plus_arg_names {tcl_thread_num};");
      if (result < 0) begin
         return result;
      end
      result = evl_tcl_eval_string(thread_num, "set tcl_plus_arg_attrs {\"\"};");
      if (result < 0) begin
         return result;
      end
      result = evl_tcl_eval_string(thread_num, $sformatf("set tcl_plus_arg_vals  {%0d};", thread_num));
      if (result < 0) begin
         return result;
      end
      result = evl_tcl_eval_string(thread_num, "lappend tcl_plus_arg_names {tcl_file_name};");
      if (result < 0) begin
         return result;
      end
      result = evl_tcl_eval_string(thread_num, "lappend tcl_plus_arg_attrs {};");
      if (result < 0) begin
         return result;
      end
      result = evl_tcl_eval_string(thread_num, $sformatf("lappend tcl_plus_arg_vals  {%0s};", m_tcl_handler.get_file_name()));
      if (result < 0) begin
         return result;
      end
      for (int ii = 0; ii < limit; ii++) begin
         result = evl_tcl_eval_string(thread_num, $sformatf("lappend tcl_plus_arg_names {%0s};", plus_arg_names[ii]));
         if (result < 0) begin
            return result;
         end
         result = evl_tcl_eval_string(thread_num, $sformatf("lappend tcl_plus_arg_attrs {%0s};", plus_arg_roots[ii]));
         if (result < 0) begin
            return result;
         end
         result = evl_tcl_eval_string(thread_num, $sformatf("lappend tcl_plus_arg_vals  {%0s};", plus_arg_vals[ii]));
         if (result < 0) begin
            return result;
         end
      end
      result = evl_tcl_eval_string(thread_num, $sformatf("set scp_index_msb   %0d;", `SC_SUB_BANK_MSB));
   endfunction : add_initial_commands


   //-----------------------------------------------------------------------------------------------
   //
   // Task: parse_tcl_command
   //
   virtual task parse_tcl_command(output int                       result,
                                  input  int                       thread_num,
                                  input  int                       cmd_from_tcl,
                                  input  evl_sequence_wrapper_base sequence_wrapper);
      evl_tcl_command_t     tcl_command;
      int                   arg_count;
      string                vc_name;
      evl_agent_id_t        agent_id;
      evl_shire_id_t        shire_id;
      evl_neighborhood_id_t neighborhood_id;
      evl_core_id_t         core_id;
      evl_verif_comp        agent;

      result = 0;
      if (!(m_tcl_commands.exists(cmd_from_tcl))) begin
         tcl_command = TCL_None;
         //
         // If this is a TCL command we do not understand, mark this script is no longer in the
         // initialization code.
         //
         if (m_tcl_handler.in_initialization() == 1'b1) begin
            m_tcl_handler.out_of_initialization();
         end
      end
      else begin
         tcl_command = evl_tcl_command_t'(m_tcl_commands[cmd_from_tcl]);
      end

      if (tcl_command == TCL_None) begin
         return;
      end

      //
      // "evl_log" Evaluation
      //
      if (tcl_command == TCL_evl_log) begin
         int                 verbosity;
         evl_verbosity_mod_t verbosity_module;
         string              message;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 1) begin
            verbosity        = int'(UVM_MEDIUM);
            verbosity_module = VMOD_TCL;
            message          = evl_tcl_get_str_arg(thread_num, 0);
         end
         else if (arg_count == 2) begin
            verbosity        = `evl_get_verbosity_by_name(evl_tcl_get_str_arg(thread_num, 0));
            verbosity_module = VMOD_TCL;
            message          = evl_tcl_get_str_arg(thread_num, 1);
         end
         else begin
            verbosity        = `evl_get_verbosity_by_name(evl_tcl_get_str_arg(thread_num, 0));
            verbosity_module = `evl_get_verbosity_mod_by_name(evl_tcl_get_str_arg(thread_num, 1));
            message          = evl_tcl_get_str_arg(thread_num, 2);
         end
         if (m_cur_agent != null) begin
            `evl_log(verbosity, verbosity_module, $sformatf("%0s(TCL)", m_cur_agent.get_abstract_name()), message)
         end
         else begin
            `evl_log(verbosity, verbosity_module, $sformatf("TCL-%0d", thread_num), message)
         end
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "print_number" Evaluation
      //
      else if (tcl_command == TCL_print_number) begin
         `evl_log_no_addr(UVM_MEDIUM, VMOD_TCL, get_abstract_name(), $sformatf("number from TCL: 0x%x (%0s)", evl_tcl_get_int_arg(thread_num, 0), evl_tcl_get_str_arg(thread_num, 0)))
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "gen_replay_file" Evaluation
      //
      else if (tcl_command == TCL_gen_replay_file) begin
         m_tcl_handler.open_replay_file(evl_tcl_get_str_arg(thread_num, 0));
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "dut_error" Evaluation
      //
      else if (tcl_command == TCL_dut_error) begin
         string message;

         message = evl_tcl_get_str_arg(thread_num, 0);
         if (m_cur_agent != null) begin
            `dut_error($sformatf("%0s(TCL)", m_cur_agent.get_abstract_name()), message)
         end
         else begin
            `dut_error($sformatf("TCL-%0d", thread_num), message)
         end
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "dut_warning" Evaluation
      //
      else if (tcl_command == TCL_dut_warning) begin
         string message;

         message = evl_tcl_get_str_arg(thread_num, 0);
         if (m_cur_agent != null) begin
            `dut_warning($sformatf("%0s(TCL)", m_cur_agent.get_abstract_name()), message)
         end
         else begin
            `dut_warning($sformatf("TCL-%0d", thread_num), message)
         end
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "enable_dump" Evaluation
      //
      else if (tcl_command == TCL_enable_dump) begin
         m_global_attrs.enable_dump();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "disable_dump" Evaluation
      //
      else if (tcl_command == TCL_disable_dump) begin
         m_global_attrs.disable_dump();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "enable_assertions" Evaluation
      //
      else if (tcl_command == TCL_enable_assertions) begin
         m_global_attrs.enable_assertions();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "disable_assertions" Evaluation
      //
      else if (tcl_command == TCL_disable_assertions) begin
         m_global_attrs.disable_assertions();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "use_system_clk" Evaluation
      //
      else if (tcl_command == TCL_use_system_clk) begin
         m_global_attrs.force_system_clk_use();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "use_local_clk" Evaluation
      //
      else if (tcl_command == TCL_use_local_clk) begin
         m_global_attrs.force_local_clk_use();
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_verbosity" Evaluation
      //
      else if (tcl_command == TCL_set_verbosity) begin
         string              verbosity_name;
         string              verbosity_mod_name;
         int                 verbosity;
         evl_verbosity_mod_t verbosity_mod;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 1) begin
            verbosity_name     = evl_tcl_get_str_arg(thread_num, 0);
            verbosity_mod_name = "VMOD_GLOBAL";
         end
         else begin
            verbosity_name     = evl_tcl_get_str_arg(thread_num, 0);
            verbosity_mod_name = evl_tcl_get_str_arg(thread_num, 1);
         end
         verbosity_name.toupper();
         verbosity_mod_name.toupper();
         if (verbosity_name.substr(0, 3) != "UVM_") begin
            verbosity_name = $sformatf("UVM_%0s", verbosity_name);
         end
         if (verbosity_mod_name.substr(0, 4) != "VMOD_") begin
            verbosity_mod_name = $sformatf("VMOD_%0s", verbosity_mod_name);
         end
         verbosity     = `evl_get_verbosity_by_name(verbosity_name);
         verbosity_mod = `evl_get_verbosity_mod_by_name(verbosity_mod_name);
         `evl_log_set_verbosity(verbosity, verbosity_mod)
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_error_intensity" Evaluation
      //
      else if (tcl_command == TCL_set_error_intensity) begin
         string intensity_key_name;
         string verbosity_name;

         intensity_key_name = evl_tcl_get_str_arg(thread_num, 0);
         verbosity_name     = evl_tcl_get_str_arg(thread_num, 1);
         intensity_key_name.tolower();
         verbosity_name.toupper();
         if (intensity_key_name.substr(0, 2) != "pk_") begin
            intensity_key_name = $sformatf("pk_%0s", intensity_key_name);
         end
         if (verbosity_name.substr(0, 3) != "UVM_") begin
            verbosity_name = $sformatf("UVM_%0s", verbosity_name);
         end
         `evl_set_error_intensity_by_name(intensity_key_name, `evl_get_verbosity_by_name(verbosity_name));
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "clr_error_intensity" Evaluation
      //
      else if (tcl_command == TCL_clr_error_intensity) begin
         string intensity_key_name;

         intensity_key_name = evl_tcl_get_str_arg(thread_num, 0);
         intensity_key_name.tolower();
         if (intensity_key_name.substr(0, 2) != "pk_") begin
            intensity_key_name = $sformatf("pk_%0s", intensity_key_name);
         end
         `evl_set_error_intensity_by_name(intensity_key_name, -1);
         result = evl_tcl_set_int_res(thread_num, 0, 0);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "exit_script" Evaluation
      //
      else if (tcl_command == TCL_exit_script) begin
         result = -1;
      end

      //
      // "finish_sim" Evaluation
      //
      else if (tcl_command == TCL_finish_sim) begin
         m_tcl_handler.finish_sim_called();
         m_global_attrs.set_all_threads_done();
         m_global_attrs.terminate_now();
         arg_count = evl_tcl_get_arg_count(thread_num);
         if ((arg_count < 1) || ((evl_tcl_get_str_arg(thread_num, 0) != "graceful") && (evl_tcl_get_str_arg(thread_num, 0) != "gracefully"))) begin
            #10ns;
            evl_verif_comp_map::assert_final_phase(1'b1);
         end
         result = cmd_from_tcl;
      end

      //
      // "get_time" Evaluation
      //
      else if (tcl_command == TCL_get_time) begin
         longint cur_time;

         cur_time = $time;
         result = evl_tcl_set_int_res(thread_num, 0, cur_time);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_time" Evaluation
      //
      else if (tcl_command == TCL_get_vpi_time) begin
         longint cur_time;

         cur_time = `evl_tcl_get_vpi_time;
         result = evl_tcl_set_int_res(thread_num, 0, cur_time);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_cycle" Evaluation
      //
      else if (tcl_command == TCL_get_cycle) begin
         longint cur_cycle;

         if (agent != null) begin
            cur_cycle = agent.get_cycle_count();
         end
         else if (m_system_agent != null) begin
            cur_cycle = m_system_agent.get_cycle_count();
         end
         else begin
            cur_cycle = -1;
         end
         result = evl_tcl_set_int_res(thread_num, 0, cur_cycle);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "terminating_script" Evaluation
      //
      else if (tcl_command == TCL_terminating_script) begin
         if (sequence_wrapper != null) begin
            sequence_wrapper.convert_to_terminating_seq();
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "background_script" Evaluation
      //
      else if (tcl_command == TCL_background_script) begin
         if (sequence_wrapper != null) begin
            sequence_wrapper.convert_to_background_seq();
         end
         void'(evl_tcl_set_int_res(thread_num, 0, 0));
      end

      //
      // "get_agent_count" Evaluation
      //
      else if (tcl_command == TCL_get_agent_count) begin
         evl_verif_comp vc_list[];
         int            vc_count;

         parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
         vc_count = evl_verif_comp_map::get_verif_comp_list_by_name(vc_list, vc_name, agent_id, shire_id, neighborhood_id, core_id);
         result = evl_tcl_set_int_res(thread_num, 0, longint'(vc_count));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "agent_match" Evaluation
      //
      else if (tcl_command == TCL_agent_match) begin
         if (m_cur_agent == null) begin
            result = evl_tcl_set_int_res(thread_num, 0, 0);
         end
         else begin
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            result = int'(m_cur_agent.agent_match_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id));
            result = evl_tcl_set_int_res(thread_num, 0, longint'(result));
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "have_agent" Evaluation
      //
      else if (tcl_command == TCL_have_agent) begin
         evl_agent_key_t agent_key;
         int             sequencer_id;
         bit             have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            sequencer_id = int'(evl_tcl_get_int_arg(thread_num, 1));
         end
         else begin
            sequencer_id = -1;
         end
         parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
         agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            if (sequencer_id < 0) begin
               have_agent = 1'b1;
            end
            else begin
               agent_key = agent.get_agent_key();
               if (m_tcl_handler.have_sequence(agent_key, evl_sequencer_id_t'(sequencer_id)) == 1'b1) begin
                  have_agent = 1'b1;
               end
               else begin
                  have_agent = 1'b0;
               end
            end
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "select_agent" Evaluation
      //
      else if (tcl_command == TCL_select_agent) begin
         evl_agent_key_t    agent_key;
         evl_sequencer_id_t sequencer_id;
         bit                have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            sequencer_id = evl_sequencer_id_t'(evl_tcl_get_int_arg(thread_num, 1));
         end
         else begin
            sequencer_id = evl_sequencer_id_t'(0);
         end
         parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
         agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            agent_key = agent.get_agent_key();
            if (m_tcl_handler.have_sequence(agent_key, sequencer_id) == 1'b1) begin
               have_agent = 1'b1;
               m_tcl_handler.set_cur_agent(agent, sequencer_id);
            end
            else begin
               have_agent = 1'b0;
            end
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "select_target_agent" Evaluation
      //
      else if ((tcl_command == TCL_select_target_agent) || (tcl_command == TCL_set_target_agent)) begin
         bit have_agent;

         parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
         agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            m_tcl_handler.set_target_agent(agent);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_shire_id" Evaluation
      //
      else if (tcl_command == TCL_get_shire_id) begin
         int shire_result;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 0) begin
            agent = m_cur_agent;
         end
         else begin
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         if (agent == null) begin
            shire_result = -1;
         end
         else begin
            shire_result = agent.get_shire_id();
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(shire_result));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_cur_agent" Evaluation
      //
      else if (tcl_command == TCL_get_cur_agent) begin
         if (m_cur_agent == null) begin
            result = evl_tcl_set_str_res(thread_num, 0, "NO_AGENT");
         end
         else begin
            result = evl_tcl_set_str_res(thread_num, 0, m_cur_agent.get_agent_tcl_str());
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_agent_info" Evaluation
      //
      else if (tcl_command == TCL_get_agent_info) begin
         if (m_cur_agent == null) begin
            result = evl_tcl_set_str_res(thread_num, 0, "NO_AGENT");
            result = evl_tcl_set_str_res(thread_num, 1, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 2, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 3, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
         end
         else begin
            result = evl_tcl_set_str_res(thread_num, 0, $sformatf("%p", m_cur_agent.get_agent_type()));
            if (m_cur_agent.get_agent_id() == ~evl_agent_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 1, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 1, $sformatf("%0d", m_cur_agent.get_agent_id())) | result;
            end
            if (m_cur_agent.get_shire_id() == ~evl_shire_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 2, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 2, $sformatf("%0d", m_cur_agent.get_shire_id())) | result;
            end
            if (m_cur_agent.get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 3, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 3, $sformatf("%0d", m_cur_agent.get_neighborhood_id())) | result;
            end
            if (m_cur_agent.get_core_id() == ~evl_core_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 4, $sformatf("%0d", m_cur_agent.get_core_id())) | result;
            end
            result = evl_tcl_set_str_res(thread_num, 5, $sformatf("%0d", get_sequencer_id())) | result;
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_target_agent_info" Evaluation
      //
      else if (tcl_command == TCL_get_target_agent_info) begin
         if (m_target_agent == null) begin
            result = evl_tcl_set_str_res(thread_num, 0, "NO_AGENT");
            result = evl_tcl_set_str_res(thread_num, 1, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 2, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 3, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
            result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
         end
         else begin
            result = evl_tcl_set_str_res(thread_num, 0, $sformatf("%p", m_target_agent.get_agent_type()));
            if (m_target_agent.get_agent_id() == ~evl_agent_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 1, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 1, $sformatf("%0d", m_target_agent.get_agent_id())) | result;
            end
            if (m_target_agent.get_shire_id() == ~evl_shire_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 2, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 2, $sformatf("%0d", m_target_agent.get_shire_id())) | result;
            end
            if (m_target_agent.get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 3, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 3, $sformatf("%0d", m_target_agent.get_neighborhood_id())) | result;
            end
            if (m_target_agent.get_core_id() == ~evl_core_id_t'(0)) begin
               result = evl_tcl_set_str_res(thread_num, 4, "-1") | result;
            end
            else begin
               result = evl_tcl_set_str_res(thread_num, 4, $sformatf("%0d", m_target_agent.get_core_id())) | result;
            end
            result = evl_tcl_set_str_res(thread_num, 5, $sformatf("%0d", get_sequencer_id())) | result;
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "add_agent_delay" Evaluation
      //
      else if (tcl_command == TCL_add_agent_delay) begin
         evl_paddr_t start_addr = ~evl_paddr_t'(0);
         evl_paddr_t end_addr   = evl_paddr_t'(0);
         string      delay_key_name;
         int         delay_key;
         evl_delay_t delay_value;
         bit         have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 5) begin
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 3);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 4));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 4) begin
            agent          = m_cur_agent;
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 2);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 3));
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 3) begin
            delay_key_name = evl_tcl_get_str_arg(thread_num, 1);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 2));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent          = m_cur_agent;
            delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 1));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            if (delay_key_name.substr(0, 2) != "pk_") begin
               delay_key_name = $sformatf("pk_%0s", delay_key_name);
            end
            have_agent = 1'b1;
            delay_key  = evl_param_xref::get_param_key(delay_key_name);
            if (start_addr <= end_addr) begin
               agent.add_delay_range(delay_key, start_addr, end_addr, delay_value, 1'b1);
            end
            else begin
               agent.add_delay(delay_key, delay_value, 1'b1);
            end
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_agent_delay" Evaluation
      //
      else if (tcl_command == TCL_set_agent_delay) begin
         evl_paddr_t start_addr = ~evl_paddr_t'(0);
         evl_paddr_t end_addr   = evl_paddr_t'(0);
         string      delay_key_name;
         int         delay_key;
         evl_delay_t delay_value;
         bit         have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 5) begin
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 3);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 4));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 4) begin
            agent          = m_cur_agent;
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 2);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 3));
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 3) begin
            delay_key_name = evl_tcl_get_str_arg(thread_num, 1);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 2));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent          = m_cur_agent;
            delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
            delay_value    = evl_delay_t'(evl_tcl_get_int_arg(thread_num, 1));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            if (delay_key_name.substr(0, 2) != "pk_") begin
               delay_key_name = $sformatf("pk_%0s", delay_key_name);
            end
            have_agent = 1'b1;
            delay_key  = evl_param_xref::get_param_key(delay_key_name);
            if (start_addr <= end_addr) begin
               agent.reset_delay_range(delay_key, start_addr, end_addr);
               agent.add_delay_range(delay_key, start_addr, end_addr, delay_value);
            end
            else begin
               agent.reset_delay(delay_key);
               agent.add_delay(delay_key, delay_value);
            end
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "reset_agent_delay" Evaluation
      //
      else if (tcl_command == TCL_reset_agent_delay) begin
         evl_paddr_t start_addr = ~evl_paddr_t'(0);
         evl_paddr_t end_addr   = evl_paddr_t'(0);
         string      delay_key_name;
         int         delay_key;
         evl_delay_t delay_value;
         bit         have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 4) begin
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 3);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 3) begin
            agent          = m_cur_agent;
            start_addr     = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end_addr       = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            delay_key_name = evl_tcl_get_str_arg(thread_num, 2);
            if (start_addr > end_addr) begin
               evl_paddr_t tmp_addr;

               tmp_addr   = start_addr;
               start_addr = end_addr;
               end_addr   = tmp_addr;
            end
         end
         else if (arg_count == 2) begin
            delay_key_name = evl_tcl_get_str_arg(thread_num, 1);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent          = m_cur_agent;
            delay_key_name = evl_tcl_get_str_arg(thread_num, 0);
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            if (delay_key_name.substr(0, 2) != "pk_") begin
               delay_key_name = $sformatf("pk_%0s", delay_key_name);
            end
            have_agent = 1'b1;
            delay_key  = evl_param_xref::get_param_key(delay_key_name);
            if (start_addr <= end_addr) begin
               agent.reset_delay_range(delay_key, start_addr, end_addr);
            end
            else begin
               agent.reset_delay(delay_key);
            end
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_agent_param" Evaluation
      //
      else if (tcl_command == TCL_set_agent_param) begin
         string param_name;
         int    param_value;
         bit    have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 3) begin
            param_name  = evl_tcl_get_str_arg(thread_num, 1);
            param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent       = m_cur_agent;
            param_name  = evl_tcl_get_str_arg(thread_num, 0);
            param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            agent.set_int_param_by_name(param_name, param_value);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_agent_param" Evaluation
      //
      else if (tcl_command == TCL_get_agent_param) begin
         string param_name;
         int    param_value;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            param_name = evl_tcl_get_str_arg(thread_num, 1);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent      = m_cur_agent;
            param_name = evl_tcl_get_str_arg(thread_num, 0);
         end
         if (agent == null) begin
            param_value = -1;
         end
         else begin
            param_value = agent.get_int_param_by_name(param_name, -1);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(param_value));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_agent_rtl_param" Evaluation
      //
      else if (tcl_command == TCL_set_agent_rtl_param) begin
         string param_name;
         int    param_value;
         bit    have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 3) begin
            param_name  = evl_tcl_get_str_arg(thread_num, 1);
            param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent       = m_cur_agent;
            param_name  = evl_tcl_get_str_arg(thread_num, 0);
            param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            agent.set_rtl_int_param_by_name(param_name, param_value, -1);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_agent_rtl_param" Evaluation
      //
      else if (tcl_command == TCL_get_agent_rtl_param) begin
         string param_name;
         int    param_value;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            param_name = evl_tcl_get_str_arg(thread_num, 1);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent      = m_cur_agent;
            param_name = evl_tcl_get_str_arg(thread_num, 0);
         end
         if (agent == null) begin
            param_value = -1;
         end
         else begin
            param_value = agent.get_rtl_int_param_by_name(param_name, -1);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(param_value));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "add_agent_addr_region" Evaluation
      //
      else if (tcl_command == TCL_add_agent_addr_region) begin
         string      region_name;
         evl_paddr_t region_base;
         evl_paddr_t region_top;
         bit         have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 4) begin
            region_name = evl_tcl_get_str_arg(thread_num, 1);
            region_base = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
            region_top  = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 3));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent       = m_cur_agent;
            region_name = evl_tcl_get_str_arg(thread_num, 0);
            region_base = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            region_top  = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            agent.add_addr_region_by_name(region_name, region_base, region_top);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_agent_addr_region" Evaluation
      //
      else if (tcl_command == TCL_set_agent_addr_region) begin
         string      region_name;
         evl_paddr_t region_base;
         evl_paddr_t region_top;
         bit         have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 4) begin
            region_name = evl_tcl_get_str_arg(thread_num, 1);
            region_base = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
            region_top  = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 3));
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent       = m_cur_agent;
            region_name = evl_tcl_get_str_arg(thread_num, 0);
            region_base = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            region_top  = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 2));
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            agent.set_addr_region_by_name(region_name, region_base, region_top);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "clr_agent_addr_region" Evaluation
      //
      else if (tcl_command == TCL_clr_agent_addr_region) begin
         string region_name;
         bit    have_agent;

         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            region_name = evl_tcl_get_str_arg(thread_num, 1);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
         end
         else begin
            agent       = m_cur_agent;
            region_name = evl_tcl_get_str_arg(thread_num, 0);
         end
         if (agent == null) begin
            have_agent = 1'b0;
         end
         else begin
            have_agent = 1'b1;
            agent.clr_addr_region_by_name(region_name);
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_num_banks" Evaluation
      //
      else if (tcl_command == TCL_get_num_banks) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_BANKS));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_num_subbanks" Evaluation
      //
      else if (tcl_command == TCL_get_num_subbanks) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_SUB_BANKS));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_l2_bank_sel_msb" Evaluation
      //
      else if (tcl_command == TCL_get_l2_bank_sel_msb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_BANK_MSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_l3_bank_sel_msb" Evaluation
      //
      else if (tcl_command == TCL_get_l3_bank_sel_msb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_L3_BANK_MSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_l3_shire_sel_msb" Evaluation
      //
      else if (tcl_command == TCL_get_l3_shire_sel_msb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_L3_SHIRE_MSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_shire_sel_msb" Evaluation
      //
      else if (tcl_command == TCL_get_shire_sel_msb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_L3_SHIRE_MSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_scp_shire_sel_lsb" Evaluation
      //
      else if (tcl_command == TCL_get_scp_shire_sel_lsb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_SCP_SHIRE_LSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_scp_shire_sel_msb" Evaluation
      //
      else if (tcl_command == TCL_get_scp_shire_sel_msb) begin
         result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_SCP_SHIRE_MSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_scp_mmio_addr" Evaluation
      //
      else if (tcl_command == TCL_get_scp_mmio_addr) begin
         result = evl_tcl_set_int_res(thread_num, 0, (longint'(`SC_SCP_REGION_ID) << `SC_SCP_REGION_LSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_scp_mmio_mask" Evaluation
      //
      else if (tcl_command == TCL_get_scp_mmio_mask) begin
         evl_paddr_t addr_mask = evl_paddr_t'((1 << `SC_SCP_REGION_ID_SIZE) - 1) << `SC_SCP_REGION_LSB;

         if (evl_verif_comp_map::get_verif_comp(NOC_RTL_AGENT) != null) begin
            addr_mask[`SC_SCP_SHIRE_SEL_ADDR_RANGE] = 31;
         end
         result = evl_tcl_set_int_res(thread_num, 0, longint'(addr_mask));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "get_scp_index_mask" Evaluation
      //
      else if (tcl_command == TCL_get_scp_index_mask) begin
         result = evl_tcl_set_int_res(thread_num, 0, (longint'((1 << (`SC_SCP_SHIRE_LSB-`SC_SCP_WAY_LSB)) - 1) << `SC_SCP_WAY_LSB));
         //result = evl_tcl_set_int_res(thread_num, 0, (longint'((1 << `SC_SCP_REGION_ID_SIZE) - 1) << `SC_SCP_REGION_LSB));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "subscribe" Evaluation
      //
      else if (tcl_command == TCL_subscribe) begin
         int subscriber_id;

         if (m_message_wrapper != null) begin
            subscriber_id = m_message_wrapper.subscribe(evl_tcl_get_str_arg(thread_num, 0));
         end
         else begin
            subscriber_id = -1;
         end
         result = evl_tcl_set_int_res(thread_num, 0, (longint'(subscriber_id)));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "send_message" Evaluation
      //
      else if (tcl_command == TCL_send_message) begin
         int success;

         if (m_message_wrapper != null) begin
            int         msg_source;
            int         msg_type;
            evl_dword_t msg_value;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 2) begin
               msg_source = 0;
               msg_type   = 0;
               msg_value  = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            else if (arg_count == 3) begin
               msg_source = 0;
               msg_type   = int'(evl_tcl_get_int_arg(thread_num, 1));
               msg_value  = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 2));
            end
            else begin
               msg_source = int'(evl_tcl_get_int_arg(thread_num, 1));
               msg_type   = int'(evl_tcl_get_int_arg(thread_num, 2));
               msg_value  = evl_dword_t'(evl_tcl_get_int_arg(thread_num, 3));
            end
            m_message_wrapper.send_message(evl_tcl_get_str_arg(thread_num, 0), msg_source, msg_type, msg_value);
            success = 1;
         end
         else begin
            success = 0;
         end
         result = evl_tcl_set_int_res(thread_num, 0, (longint'(success)));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "set_wait_timeout" Evaluation
      //
      else if (tcl_command == TCL_set_wait_timeout) begin
         m_wait_timeout = int'(evl_tcl_get_int_arg(thread_num, 0));
         if (evl_tcl_get_arg_count(thread_num) > 1) begin
            m_wait_timeout_err = $sformatf("%0s", evl_tcl_get_str_arg(thread_num, 1));
         end
         else begin
            m_wait_timeout_err = "";
         end
         result = evl_tcl_set_int_res(thread_num, 0, 1);
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "raise_objection" Evaluation
      //
      else if (tcl_command == TCL_raise_objection) begin
//       m_global_attrs.incr_objection(evl_tcl_get_str_arg(thread_num, 0), m_tcl_handler.get_file_name());
         `evl_incr_objection(evl_tcl_get_str_arg(thread_num, 0), m_tcl_handler.get_file_name())
         result = cmd_from_tcl;
      end

      //
      // "drop_objection" Evaluation
      //
      else if (tcl_command == TCL_drop_objection) begin
//       m_global_attrs.decr_objection(evl_tcl_get_str_arg(thread_num, 0), m_tcl_handler.get_file_name());
         `evl_decr_objection(evl_tcl_get_str_arg(thread_num, 0), m_tcl_handler.get_file_name())
         result = cmd_from_tcl;
      end

      //
      // "scan_module" Evaluation
      //
      else if (tcl_command == TCL_scan_module) begin
         arg_count = evl_tcl_get_arg_count(thread_num);
         if (arg_count == 2) begin
            evl_tcl_scan_module(evl_tcl_get_str_arg(thread_num, 0), evl_tcl_get_int_arg(thread_num, 1));
            result = evl_tcl_set_int_res(thread_num, 0, 0);
         end
         else begin
            result = evl_tcl_set_int_res(thread_num, 0, -1);
         end
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end

      //
      // "use_pre_reset_phase" Evaluation
      //
      else if (tcl_command == TCL_use_pre_reset_phase) begin
         m_tcl_handler.set_starting_phase(PreResetPhase);
         result = cmd_from_tcl;
      end

      //
      // "use_reset_phase" Evaluation
      //
      else if (tcl_command == TCL_use_reset_phase) begin
         m_tcl_handler.set_starting_phase(ResetPhase);
         result = cmd_from_tcl;
      end

      //
      // "use_post_reset_phase" Evaluation
      //
      else if (tcl_command == TCL_use_post_reset_phase) begin
         m_tcl_handler.set_starting_phase(PostResetPhase);
         result = cmd_from_tcl;
      end

      //
      // "use_pre_configure_phase" Evaluation
      //
      else if (tcl_command == TCL_use_pre_configure_phase) begin
         m_tcl_handler.set_starting_phase(PreConfigurePhase);
         result = cmd_from_tcl;
      end

      //
      // "use_configure_phase" Evaluation
      //
      else if (tcl_command == TCL_use_configure_phase) begin
         m_tcl_handler.set_starting_phase(ConfigurePhase);
         result = cmd_from_tcl;
      end

      //
      // "use_post_configure_phase" Evaluation
      //
      else if (tcl_command == TCL_use_post_configure_phase) begin
         m_tcl_handler.set_starting_phase(PostConfigurePhase);
         result = cmd_from_tcl;
      end

      //
      // "use_pre_main_phase" Evaluation
      //
      else if (tcl_command == TCL_use_pre_main_phase) begin
         m_tcl_handler.set_starting_phase(PreMainPhase);
         result = cmd_from_tcl;
      end

      //
      // "use_main_phase" Evaluation
      //
      else if (tcl_command == TCL_use_main_phase) begin
         m_tcl_handler.set_starting_phase(MainPhase);
         result = cmd_from_tcl;
      end

      //
      // "use_post_main_phase" Evaluation
      //
      else if (tcl_command == TCL_use_post_main_phase) begin
         m_tcl_handler.set_starting_phase(PostMainPhase);
         result = cmd_from_tcl;
      end

      //
      // "create_semaphore" Evaluation
      //
      else if (tcl_command == TCL_create_semaphore) begin
         m_semaphore_wrapper.create_semaphore(evl_tcl_get_str_arg(thread_num, 0));
         result = cmd_from_tcl;
      end

      //
      // "put_semaphore" Evaluation
      //
      else if (tcl_command == TCL_put_semaphore) begin
         m_semaphore_wrapper.put_semaphore(evl_tcl_get_str_arg(thread_num, 0));
         result = cmd_from_tcl;
      end

      //
      // "create_sync_count" Evaluation
      //
      else if (tcl_command == TCL_create_sync_count) begin
         m_semaphore_wrapper.create_sync_count(evl_tcl_get_str_arg(thread_num, 0), evl_tcl_get_int_arg(thread_num, 1));
         result = cmd_from_tcl;
      end

      //
      // "incr_sync_count" Evaluation
      //
      else if (tcl_command == TCL_incr_sync_count) begin
         m_semaphore_wrapper.incr_sync_count(evl_tcl_get_str_arg(thread_num, 0));
         result = cmd_from_tcl;
      end

      //
      // "get_sync_count" Evaluation
      //
      else if (tcl_command == TCL_get_sync_count) begin
         int sync_count;

         #0 sync_count = m_semaphore_wrapper.get_sync_count(evl_tcl_get_str_arg(thread_num, 0));
         result = evl_tcl_set_int_res(thread_num, 0, longint'(sync_count));
         if (result < 0) begin
            return;
         end
         result = cmd_from_tcl;
      end
      else begin
         //
         // The following TCL commands may consume time.  Mark this script is no longer in the
         // initialization code.
         //
         if (m_tcl_handler.in_initialization() == 1'b1) begin
            m_tcl_handler.out_of_initialization();
            result = 0;
            return;
         end

         //
         // "display_objections" Evaluation
         //
         else if (tcl_command == TCL_display_objections) begin
            evl_verif_comp_map::display_objections();
            void'(evl_tcl_set_int_res(thread_num, 0, 0));
         end

         //
         // "get_semaphore" Evaluation
         //
         else if (tcl_command == TCL_get_semaphore) begin
            #0 m_semaphore_wrapper.get_semaphore(evl_tcl_get_str_arg(thread_num, 0));
            result = cmd_from_tcl;
         end

         //
         // "decr_sync_count" Evaluation
         //
         else if (tcl_command == TCL_decr_sync_count) begin
            #0 m_semaphore_wrapper.decr_sync_count_and_wait(evl_tcl_get_str_arg(thread_num, 0));
            result = cmd_from_tcl;
         end

         //
         // "get_message" Evaluation
         //
         else if (tcl_command == TCL_get_message) begin
            evl_message_packet packet;

            packet = null;
            if (m_message_wrapper != null) begin
               #0 m_message_wrapper.get_message_packet(packet, evl_tcl_get_str_arg(thread_num, 0), evl_tcl_get_int_arg(thread_num, 1));
            end
            if (packet == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, (longint'(-1)));
            end
            else begin
               result = evl_tcl_set_int_res(thread_num, 0, (longint'(packet.get_packet_value())));
               result = evl_tcl_set_int_res(thread_num, 1, (longint'(packet.get_packet_type())))   | result;
               result = evl_tcl_set_int_res(thread_num, 2, (longint'(packet.get_packet_source()))) | result;
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_blocking_message" Evaluation
         //
         else if (tcl_command == TCL_get_blocking_message) begin
            evl_message_packet packet;

            packet = null;
            if (m_message_wrapper != null) begin
               #0 m_message_wrapper.get_blocking_message_packet(packet, evl_tcl_get_str_arg(thread_num, 0), evl_tcl_get_int_arg(thread_num, 1));
            end
            if (packet == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, (longint'(-1)));
            end
            else begin
               result = evl_tcl_set_int_res(thread_num, 0, (longint'(packet.get_packet_value())));
               result = evl_tcl_set_int_res(thread_num, 1, (longint'(packet.get_packet_type())))   | result;
               result = evl_tcl_set_int_res(thread_num, 2, (longint'(packet.get_packet_source()))) | result;
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_param" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_param) begin
            string param_name;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 2) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_int_param_by_name(param_name, 32'h7fffffff, 32'h7ffffffe, 1'b0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_param_value" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_param_value) begin
            string param_name;
            int    param_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 3) begin
               param_name  = evl_tcl_get_str_arg(thread_num, 1);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent       = m_cur_agent;
               param_name  = evl_tcl_get_str_arg(thread_num, 0);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_int_param_by_name(param_name, param_value, param_value, 1'b0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_param_not_value" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_param_not_value) begin
            string param_name;
            int    param_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 3) begin
               param_name  = evl_tcl_get_str_arg(thread_num, 1);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent       = m_cur_agent;
               param_name  = evl_tcl_get_str_arg(thread_num, 0);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_int_param_by_name(param_name, param_value, param_value, 1'b1, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_param_range" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_param_range) begin
            string param_name;
            int    min_value;
            int    max_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 4) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               end
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               end
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_int_param_by_name(param_name, min_value, max_value, 1'b0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_param_not_range" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_param_not_range) begin
            string param_name;
            int    min_value;
            int    max_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 4) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               end
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               end
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_int_param_by_name(param_name, min_value, max_value, 1'b1, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_rtl_param" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_rtl_param) begin
            string param_name;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 2) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_rtl_int_param_by_name(param_name, 32'h7fffffff, 32'h7ffffffe, 1'b0, 0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_rtl_param_value" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_rtl_param_value) begin
            string param_name;
            int    param_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 3) begin
               param_name  = evl_tcl_get_str_arg(thread_num, 1);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent       = m_cur_agent;
               param_name  = evl_tcl_get_str_arg(thread_num, 0);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_rtl_int_param_by_name(param_name, param_value, param_value, 1'b0, 0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_rtl_param_not_value" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_rtl_param_not_value) begin
            string param_name;
            int    param_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 3) begin
               param_name  = evl_tcl_get_str_arg(thread_num, 1);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 2));
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent       = m_cur_agent;
               param_name  = evl_tcl_get_str_arg(thread_num, 0);
               param_value = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_rtl_int_param_by_name(param_name, param_value, param_value, 1'b1, 0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_rtl_param_range" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_rtl_param_range) begin
            string param_name;
            int    min_value;
            int    max_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 4) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               end
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               end
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_rtl_int_param_by_name(param_name, min_value, max_value, 1'b0, 0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_agent_rtl_param_not_range" Evaluation
         //
         else if (tcl_command == TCL_wait_for_agent_rtl_param_not_range) begin
            string param_name;
            int    min_value;
            int    max_value;
            bit    have_agent;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 4) begin
               param_name = evl_tcl_get_str_arg(thread_num, 1);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 3));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               end
               //
               // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
               // done last.
               //
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
               agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               agent      = m_cur_agent;
               param_name = evl_tcl_get_str_arg(thread_num, 0);
               min_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               max_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
               if (max_value < min_value) begin
                  min_value  = int'(evl_tcl_get_int_arg(thread_num, 2));
                  max_value  = int'(evl_tcl_get_int_arg(thread_num, 1));
               end
            end
            if (agent == null) begin
               have_agent = 1'b0;
            end
            else begin
               have_agent = 1'b1;
               agent.wait_for_rtl_int_param_by_name(param_name, min_value, max_value, 1'b1, 0, m_wait_timeout, m_wait_timeout_err);
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(have_agent));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_phase" Evaluation
         //
         else if (tcl_command == TCL_wait_for_phase) begin
            string           phase_name;
            string           phase_name_lc;
            evl_phase_type_t phase_type;
            int              value;

            phase_name    = evl_tcl_get_str_arg(thread_num, 0);
            phase_name_lc = phase_name.tolower();
            value         = 1;
            if ((phase_name_lc == "pre_reset_phase") || (phase_name_lc == "preresetphase")) begin
               m_global_attrs.wait_for_start_of_phase(PreResetPhase);
            end
            else if ((phase_name_lc == "reset_phase") || (phase_name_lc == "resetphase")) begin
               m_global_attrs.wait_for_start_of_phase(ResetPhase);
            end
            else if ((phase_name_lc == "post_reset_phase") || (phase_name_lc == "postresetphase")) begin
               m_global_attrs.wait_for_start_of_phase(PostResetPhase);
            end
            else if ((phase_name_lc == "pre_configure_phase") || (phase_name_lc == "preconfigurephase")) begin
               m_global_attrs.wait_for_start_of_phase(PreConfigurePhase);
            end
            else if ((phase_name_lc == "configure_phase") || (phase_name_lc == "configurephase")) begin
               m_global_attrs.wait_for_start_of_phase(ConfigurePhase);
            end
            else if ((phase_name_lc == "post_configure_phase") || (phase_name_lc == "postconfigurephase")) begin
               m_global_attrs.wait_for_start_of_phase(PostConfigurePhase);
            end
            else if ((phase_name_lc == "pre_main_phase") || (phase_name_lc == "premainphase")) begin
               m_global_attrs.wait_for_start_of_phase(PreMainPhase);
            end
            else if ((phase_name_lc == "main_phase") || (phase_name_lc == "mainphase")) begin
               m_global_attrs.wait_for_start_of_phase(MainPhase);
            end
            else if ((phase_name_lc == "post_main_phase") || (phase_name_lc == "postmainphase")) begin
               m_global_attrs.wait_for_start_of_phase(PostMainPhase);
            end
            else if ((phase_name_lc == "pre_shutdown_phase") || (phase_name_lc == "preshutdownphase")) begin
               m_global_attrs.wait_for_start_of_phase(PreShutdownPhase);
            end
            else if ((phase_name_lc == "shutdown_phase") || (phase_name_lc == "shutdownphase")) begin
               m_global_attrs.wait_for_start_of_phase(ShutdownPhase);
            end
            else if ((phase_name_lc == "post_shutdown_phase") || (phase_name_lc == "postshutdownphase")) begin
               m_global_attrs.wait_for_start_of_phase(PostShutdownPhase);
            end
            else begin
               `dut_warning("TCL", $sformatf("\"%0s\" is not a valid phase name (%0s)", phase_name, m_tcl_handler.get_file_name()))
               value = 0;
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_bus_addr" Evaluation
         //
         else if (tcl_command == TCL_wait_for_bus_addr) begin
            evl_paddr_t addr;
            int         value;

            addr = evl_tcl_get_int_arg(thread_num, 1);
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            value = -1;
            if (agent != null) begin
               m_bus_watch = m_global_attrs.set_bus_watch_params(addr, evl_bus_req_id_t'(0), evl_bus_req_id_t'(0), agent);
               value       = 0;
               if (m_bus_watch != null) begin
                  if (m_wait_timeout <= 0) begin
                     m_bus_watch.wait_for_match();
                  end
                  else begin
                     fork
                        begin
                           #(m_wait_timeout);
                           value = -1;
                           if (m_wait_timeout_err != "") begin
                              `dut_error(m_target_agent.get_abstract_name(), $sformatf("wait_for_bus_addr() timeout: %s", m_wait_timeout_err))
                           end
                        end
                        begin
                           m_bus_watch.wait_for_match();
                        end
                     join_any
                     disable fork;
                  end
               end
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_pc" Evaluation
         //
         else if (tcl_command == TCL_wait_for_pc) begin
            evl_vaddr_t pc;
            int         source;
            bit         not_valid;

            arg_count = evl_tcl_get_arg_count(thread_num);
            pc        = ~evl_vaddr_t'(0);
            source    = -1;
            not_valid = 1'b0;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               if (arg.substr(0, 6) == "hartid=") begin
                  if (source != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_pc argument (the HART ID has already been selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "hartid=%d", source) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_pc argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (pc != ~evl_vaddr_t'(0)) begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_pc argument (the PC value has already been selected)", arg))
                  not_valid = 1'b1;
                  break;
               end
               else begin
                  pc = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, ii));
               end
            end
            if (not_valid == 1'b0) begin
               m_instr_watch = m_global_attrs.set_instr_watch_params(1'b1, pc, 1'b0, evl_instr_t'(0), ((source < 0) ? 1'b0 : 1'b1), source);
               if (m_instr_watch != null) begin
                  if (m_wait_timeout <= 0) begin
                     m_instr_watch.wait_for_match();
                  end
                  else begin
                     fork
                        begin
                           #(m_wait_timeout);
                           if (m_wait_timeout_err != "") begin
                              `dut_error(m_target_agent.get_abstract_name(), $sformatf("wait_for_pc() timeout: %s", m_wait_timeout_err))
                           end
                        end
                        begin
                           m_instr_watch.wait_for_match();
                        end
                     join_any
                     disable fork;
                  end
               end
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_for_instr" Evaluation
         //
         else if (tcl_command == TCL_wait_for_instr) begin
            evl_instr_t instr;
            int         source;
            bit         instr_valid;
            bit         not_valid;

            arg_count   = evl_tcl_get_arg_count(thread_num);
            instr       = evl_instr_t'(0);
            source      = -1;
            not_valid   = 1'b0;
            instr_valid = 1'b0;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               if (arg.substr(0, 6) == "hartid=") begin
                  if (source != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_instr argument (the HART ID has already been selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "hartid=%d", source) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_instr argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (instr_valid != 1'b0) begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid wait_for_instr argument (the instruction value has already been selected)", arg))
                  not_valid = 1'b1;
                  break;
               end
               else begin
                  instr       = evl_vaddr_t'(evl_tcl_get_int_arg(thread_num, ii));
                  instr_valid = 1'b1;
               end
            end
            if (not_valid == 1'b0) begin
               m_instr_watch = m_global_attrs.set_instr_watch_params(1'b0, evl_vaddr_t'(0), 1'b1, instr, ((source < 0) ? 1'b0 : 1'b1), source);
               if (m_instr_watch != null) begin
                  if (m_wait_timeout <= 0) begin
                     m_instr_watch.wait_for_match();
                  end
                  else begin
                     fork
                        begin
                           #(m_wait_timeout);
                           if (m_wait_timeout_err != "") begin
                              `dut_error(m_target_agent.get_abstract_name(), $sformatf("wait_for_instr() timeout: %s", m_wait_timeout_err))
                           end
                        end
                        begin
                           m_instr_watch.wait_for_match();
                        end
                     join_any
                     disable fork;
                  end
               end
            end
            result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_last_pc" Evaluation
         //
         else if (tcl_command == TCL_get_last_pc) begin
            longint value;

            value = (m_instr_watch == null) ? -1 : longint'(m_instr_watch.get_pc());
            result = evl_tcl_set_int_res(thread_num, 0, value);
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_last_instr" Evaluation
         //
         else if (tcl_command == TCL_get_last_instr) begin
            longint value;

            value = (m_instr_watch == null) ? -1 : longint'(m_instr_watch.get_instr());
            result = evl_tcl_set_int_res(thread_num, 0, value);
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_last_source" Evaluation
         //
         else if (tcl_command == TCL_get_last_source) begin
            longint value;

            value = (m_instr_watch == null) ? -1 : longint'(m_instr_watch.get_source());
            result = evl_tcl_set_int_res(thread_num, 0, value);
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_index_msb" Evaluation
         //
         else if (tcl_command == TCL_get_l2_index_msb) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l2_index_msb);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_index_msb" Evaluation
         //
         else if (tcl_command == TCL_get_l3_index_msb) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l3_index_msb);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_index_msb" Evaluation
         //
         else if (tcl_command == TCL_get_scp_index_msb) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_scp_index_msb);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_index_lsb" Evaluation
         //
         else if (tcl_command == TCL_get_scp_index_lsb) begin
            result = evl_tcl_set_int_res(thread_num, 0, longint'(`SC_SCP_WAY_LSB));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_set_size" Evaluation
         //
         else if (tcl_command == TCL_get_l2_set_size) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l2_set_size);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_set_size" Evaluation
         //
         else if (tcl_command == TCL_get_l3_set_size) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l3_set_size);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_set_size" Evaluation
         //
         else if (tcl_command == TCL_get_scp_set_size) begin
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               parse_agent_name(thread_num, evl_tcl_get_str_arg(thread_num, 0), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else if (m_cur_agent != null) begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               parse_agent_name(thread_num, "S0:SHIRE_CACHE_RTL_AGENT", vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(0));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_scp_set_size);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_bank" Evaluation
         //
         else if (tcl_command == TCL_get_l2_bank) begin
            evl_paddr_t addr;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr >> `EVL_LINE_ADDR_LSB;
            addr = addr & ((1 << `SC_BANK_ID_SIZE) - 1);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_bank" Evaluation
         //
         else if (tcl_command == TCL_get_l3_bank) begin
            int bank;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               bank = m_global_attrs.get_l3_bank_from_address(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               bank = m_global_attrs.get_l3_bank_from_address(evl_tcl_get_int_arg(thread_num, 1));
            end
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(bank));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_bank" Evaluation
         //
         else if (tcl_command == TCL_get_scp_bank) begin
            evl_paddr_t addr;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr >> `EVL_LINE_ADDR_LSB;
            addr = addr & ((1 << `SC_BANK_ID_SIZE) - 1);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_subbank" Evaluation
         //
         else if (tcl_command == TCL_get_l2_subbank) begin
            evl_paddr_t addr;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE);
            addr = addr & ((1 << `SC_SUB_BANK_ID_SIZE) - 1);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_subbank" Evaluation
         //
         else if (tcl_command == TCL_get_l3_subbank) begin
            int subbank;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               subbank = m_global_attrs.get_l3_subbank_from_address(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               subbank = m_global_attrs.get_l3_subbank_from_address(evl_tcl_get_int_arg(thread_num, 1));
            end
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(subbank));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_subbank" Evaluation
         //
         else if (tcl_command == TCL_get_scp_subbank) begin
            evl_paddr_t addr;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE);
            addr = addr & ((1 << `SC_SUB_BANK_ID_SIZE) - 1);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_index" Evaluation
         //
         else if (tcl_command == TCL_get_l2_index) begin
            evl_paddr_t          addr;
            evl_sc_config_attr_s sc_config;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               sc_config = m_global_attrs.get_sc_config(m_target_agent.get_shire_id());
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               sc_config = m_global_attrs.get_sc_config(evl_tcl_get_int_arg(thread_num, 0));
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr & sc_config.l2_index_mask;
            addr = addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_index" Evaluation
         //
         else if (tcl_command == TCL_get_l3_index) begin
            evl_paddr_t          addr;
            evl_sc_config_attr_s sc_config;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               sc_config = m_global_attrs.get_sc_config(m_target_agent.get_shire_id());
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               sc_config = m_global_attrs.get_sc_config(evl_tcl_get_int_arg(thread_num, 0));
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr & sc_config.l3_index_mask;
            addr = addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_index" Evaluation
         //
         else if (tcl_command == TCL_get_scp_index) begin
            evl_paddr_t          addr;
            evl_sc_config_attr_s sc_config;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               sc_config = m_global_attrs.get_sc_config(m_target_agent.get_shire_id());
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               sc_config = m_global_attrs.get_sc_config(evl_tcl_get_int_arg(thread_num, 0));
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr & sc_config.scp_index_mask;
            addr = addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_shire" Evaluation
         //
         else if (tcl_command == TCL_get_l2_shire) begin
            int shire_id;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               shire_id = m_global_attrs.get_l3_shire_from_address(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               shire_id = m_global_attrs.get_l3_shire_from_address(evl_tcl_get_int_arg(thread_num, 1));
            end
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(shire_id));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_shire" Evaluation
         //
         else if (tcl_command == TCL_get_l3_shire) begin
            int shire_id;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               shire_id = m_global_attrs.get_l3_shire_from_address(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               shire_id = m_global_attrs.get_l3_shire_from_address(evl_tcl_get_int_arg(thread_num, 1));
            end
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(shire_id));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_shire" Evaluation
         //
         else if (tcl_command == TCL_get_scp_shire) begin
            evl_paddr_t addr;

            if (evl_tcl_get_arg_count(thread_num) == 1) begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 0));
            end
            else begin
               addr = evl_paddr_t'(evl_tcl_get_int_arg(thread_num, 1));
            end
            addr = addr >> `SC_SCP_SHIRE_LSB;
            addr = addr & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
            result = evl_tcl_set_int_res(thread_num, 0, evl_dword_t'(addr));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "convert_to_l2_address" Evaluation
         //
         else if (tcl_command == TCL_convert_to_l2_address) begin
            int         arg_count;
            int         shire_id;
            int         bank;
            int         subbank;
            int         set;
            evl_paddr_t addr;

            bank      = 0;
            subbank   = 0;
            set       = 0;
            arg_count = evl_tcl_get_arg_count(thread_num);
            shire_id  = int'(evl_tcl_get_int_arg(thread_num, 0));
            if (arg_count > 1) begin
               bank = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (arg_count > 2) begin
               subbank = int'(evl_tcl_get_int_arg(thread_num, 2));
            end
            if (arg_count > 3) begin
               set = int'(evl_tcl_get_int_arg(thread_num, 3));
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", shire_id), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(-1));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l2_set_size);
               if (value <= 0) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(-1));
               end
               else begin
                  if ((`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE) > `SC_L3_SHIRE_ID_SIZE) begin
                     addr = evl_paddr_t'(set) << (`SC_LINE_OFFSET_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
                  end
                  else begin
                     addr = evl_paddr_t'(set) << (`SC_LINE_OFFSET_ID_SIZE + `SC_L3_SHIRE_ID_SIZE);
                  end
                  addr   = addr | (evl_paddr_t'(bank % `SC_BANKS)) << `SC_LINE_OFFSET_ID_SIZE;
                  addr   = addr | (evl_paddr_t'(subbank % `SC_SUB_BANKS)) << (`SC_LINE_OFFSET_ID_SIZE + `SC_BANK_ID_SIZE);
                  addr   = addr | (evl_paddr_t'((shire_id % `SC_L3_SHIRES) & (~((`SC_BANKS * `SC_SUB_BANKS) - 1))));
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(addr));
               end
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "convert_to_l3_address" Evaluation
         //
         else if (tcl_command == TCL_convert_to_l3_address) begin
            int arg_count;
            int shire_id;
            int bank;
            int subbank;
            int set;

            bank      = 0;
            subbank   = 0;
            set       = 0;
            arg_count = evl_tcl_get_arg_count(thread_num);
            shire_id = int'(evl_tcl_get_int_arg(thread_num, 0));
            if (arg_count > 1) begin
               bank = int'(evl_tcl_get_int_arg(thread_num, 1));
            end
            if (arg_count > 2) begin
               subbank = int'(evl_tcl_get_int_arg(thread_num, 2));
            end
            if (arg_count > 3) begin
               set = int'(evl_tcl_get_int_arg(thread_num, 3));
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_CACHE_RTL_AGENT", shire_id), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (agent == null) begin
               result = evl_tcl_set_int_res(thread_num, 0, longint'(-1));
            end
            else begin
               int value;

               value  = agent.get_int_param(pk_l3_set_size);
               if (value <= 0) begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(-1));
               end
               else begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(evl_paddr_t'(m_global_attrs.convert_to_l3_address(bank, subbank, shire_id, set))));
               end
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_addresses" Evaluation
         //
         else if (tcl_command == TCL_get_l2_addresses) begin
            bit         not_valid     = 1'b0;
            int         use_local     = -2;
            int         use_remote    = -2;
            int         bank_count    = -2;
            int         subbank_count = -2;
            int         index_count   = -2;
            int         alias_count   = -2;
            int         bank_id       = -2;
            int         subbank_id    = -2;
            int         index_id      = -2;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string  arg;
               longint value;
               int     arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 5) == "local=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("local", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_local = int'(value);
               end
               else if (arg.substr(0, 4) == "local") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_local = -1;
               end
               else if (arg.substr(0, 6) == "remote=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("remote", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_remote = int'(value);
               end
               else if (arg.substr(0, 5) == "remote") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_remote = -1;
               end
               else if (arg.substr(0, 10) == "bank_count=") begin
                  if (bank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("bank_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  bank_count = int'(value);
               end
               else if (arg.substr(0, 13) == "subbank_count=") begin
                  if (subbank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("subbank_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  subbank_count = int'(value);
               end
               else if (arg.substr(0, 11) == "index_count=") begin
                  if (index_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("index_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  index_count = int'(value);
               end
               else if (arg.substr(0, 11) == "alias_count=") begin
                  if (alias_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("alias_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  alias_count = int'(value);
               end
               else if (arg.substr(0, 4) == "bank=") begin
                  if (bank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("bank", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  bank_id = int'(value);
               end
               else if (arg.substr(0, 7) == "subbank=") begin
                  if (subbank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("subbank", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  subbank_id = int'(value);
               end
               else if (arg.substr(0, 5) == "index=") begin
                  if (index_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("index", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  index_id = int'(value);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            if (use_local == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L2, 1'b1, 1'b0, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_remote == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L2, 1'b0, 1'b1, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_local >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L2, 1'b0, 1'b0, use_local, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_remote >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L2, 1'b0, 1'b0, use_remote, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L2, 1'b0, 1'b0, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  m_l2_addrs.push_back(addrs[ii]);
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_addresses" Evaluation
         //
         else if (tcl_command == TCL_get_l3_addresses) begin
            bit         not_valid     = 1'b0;
            int         use_local     = -2;
            int         use_remote    = -2;
            int         bank_count    = -2;
            int         subbank_count = -2;
            int         index_count   = -2;
            int         alias_count   = -2;
            int         bank_id       = -2;
            int         subbank_id    = -2;
            int         index_id      = -2;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;
               int    arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 5) == "local=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "local=%d", use_local) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 4) == "local") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_local = -1;
               end
               else if (arg.substr(0, 6) == "remote=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "remote=%d", use_remote) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 5) == "remote") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_remote = -1;
               end
               else if (arg.substr(0, 10) == "bank_count=") begin
                  if (bank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "bank_count=%d", bank_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 13) == "subbank_count=") begin
                  if (subbank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "subbank_count=%d", subbank_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 11) == "index_count=") begin
                  if (index_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "index_count=%d", index_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 11) == "alias_count=") begin
                  if (alias_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "alias_count=%d", alias_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 4) == "bank=") begin
                  if (bank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "bank=%d", bank_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 7) == "subbank=") begin
                  if (subbank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "subbank=%d", subbank_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 5) == "index=") begin
                  if (index_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "index=%d", index_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_addresses argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            if (use_local == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L3, 1'b1, 1'b0, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_remote == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L3, 1'b0, 1'b1, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_local >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L3, 1'b0, 1'b0, use_local, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else if (use_remote >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L3, 1'b0, 1'b0, use_remote, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            else begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_L3, 1'b0, 1'b0, -1, bank_count, subbank_count, index_count, alias_count, bank_id, subbank_id, index_id);
            end
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  m_l3_addrs.push_back(addrs[ii]);
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "get_scp_addresses" Evaluation
         //
         else if (tcl_command == TCL_get_scp_addresses) begin
            bit         not_valid     = 1'b0;
            int         use_local     = -2;
            int         use_remote    = -2;
            int         bank_count    = -2;
            int         subbank_count = -2;
            int         index_count   = -2;
            int         alias_count   = -2;
            int         bank_id       = -2;
            int         subbank_id    = -2;
            int         index_id      = -2;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;
               int    arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 5) == "local=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "local=%d", use_local) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 4) == "local") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_local = -1;
               end
               else if (arg.substr(0, 6) == "remote=") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "remote=%d", use_remote) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 5) == "remote") begin
                  if ((use_local != -2) || (use_remote != -2)) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_remote = -1;
               end
               else if (arg.substr(0, 10) == "bank_count=") begin
                  if (bank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "bank_count=%d", bank_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 13) == "subbank_count=") begin
                  if (subbank_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "subbank_count=%d", subbank_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 11) == "index_count=") begin
                  if (index_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "index_count=%d", index_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 4) == "bank=") begin
                  if (bank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "bank=%d", bank_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 7) == "subbank=") begin
                  if (subbank_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "subbank=%d", subbank_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 5) == "index=") begin
                  if (index_id != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "index=%d", index_id) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_scp_addresses argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            if (use_local == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_SCP, 1'b1, 1'b0, -1, bank_count, subbank_count, index_count, 1, bank_id, subbank_id, index_id);
            end
            else if (use_remote == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_SCP, 1'b0, 1'b1, -1, bank_count, subbank_count, index_count, 1, bank_id, subbank_id, index_id);
            end
            else if (use_local >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_SCP, 1'b0, 1'b0, use_local, bank_count, subbank_count, index_count, 1, bank_id, subbank_id, index_id);
            end
            else if (use_remote >= 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_SCP, 1'b0, 1'b0, use_remote, bank_count, subbank_count, index_count, 1, bank_id, subbank_id, index_id);
            end
            else begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_SCP, 1'b0, 1'b0, -1, bank_count, subbank_count, index_count, 1, bank_id, subbank_id, index_id);
            end
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l2_aliases" Evaluation
         //
         else if (tcl_command == TCL_get_l2_aliases) begin
            bit         not_valid   = 1'b0;
            int         alias_count = -2;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string  arg;
               longint value;
               int     arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 11) == "alias_count=") begin
                  if (alias_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_aliases argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("alias_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_aliases argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  alias_count = int'(value);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l2_aliases argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            addr_count = m_target_agent.get_random_aliases(addrs, m_l2_addrs, `EVL_GEN_ADDR_L2, alias_count);
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  m_l2_addrs.push_back(addrs[ii]);
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "get_l3_aliases" Evaluation
         //
         else if (tcl_command == TCL_get_l3_aliases) begin
            bit         not_valid   = 1'b0;
            int         alias_count = -2;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string  arg;
               longint value;
               int     arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 11) == "alias_count=") begin
                  if (alias_count != -2) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_aliases argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if (get_arg_value_from_scanf("alias_count", arg, value) == 1'b0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_aliases argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  alias_count = int'(value);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_l3_aliases argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            addr_count = m_target_agent.get_random_aliases(addrs, m_l3_addrs, `EVL_GEN_ADDR_L3, alias_count);
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  m_l3_addrs.push_back(addrs[ii]);
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "clr_l2_addresses" Evaluation
         //
         else if (tcl_command == TCL_clr_l2_addresses) begin
            m_l2_addrs = {};
            result = cmd_from_tcl;
         end

         //
         // "clr_l3_addresses" Evaluation
         //
         else if (tcl_command == TCL_clr_l3_addresses) begin
            m_l3_addrs = {};
            result = cmd_from_tcl;
         end

         //
         // "get_mem_addresses" Evaluation
         //
         else if (tcl_command == TCL_get_mem_addresses) begin
            bit         not_valid   = 1'b0;
            int         use_odd     = -1;
            int         index_count = -1;
            int         addr_count;
            evl_paddr_t addrs[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;
               int    arg_value;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg == "odd") begin
                  if (use_odd != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_mem_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_odd = 1;
               end
               else if (arg == "even") begin
                  if (use_odd != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_mem_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  use_odd = 0;
               end
               else if (arg.substr(0, 11) == "index_count=") begin
                  if (index_count != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_mem_addresses argument (already set)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "index_count=%d", index_count) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_mem_addresses argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
            end
            if (not_valid == 1'b1) begin
               result = -1;
               return;
            end
            if (use_odd == -1) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_MEM, 1'b0, 1'b0, -1, 1, 1, index_count, 1, -1, -1, -1);
            end
            else if (use_odd == 0) begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_MEM, 1'b1, 1'b0, -1, 1, 1, index_count, 1, -1, -1, -1);
            end
            else begin
               addr_count = m_target_agent.get_random_addresses(addrs, `EVL_GEN_ADDR_MEM, 1'b0, 1'b1, -1, 1, 1, index_count, 1, -1, -1, -1);
            end
            if (addr_count > 0) begin
               for (int ii = 0; ii < addr_count; ii++) begin
                  result = evl_tcl_set_int_res(thread_num, ii, evl_dword_t'(addrs[ii])) | result;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_time" Evaluation
         //
         m_tcl_handler.wait_for_start_of_phase();
         if (tcl_command == TCL_wait_time) begin
            string   wait_time_str;
            realtime wait_time;

            wait_time_str = evl_tcl_get_str_arg(thread_num, 0);
            wait_time     = realtime'(wait_time_str.atoreal());
            if (wait_time >= 0.0) begin
               #(wait_time);
            end
            result = evl_tcl_set_int_res(thread_num, 0, 0);
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_until_time" Evaluation
         //
         else if (tcl_command == TCL_wait_until_time) begin
            string   wait_time_str;
            realtime wait_time;

            wait_time_str = evl_tcl_get_str_arg(thread_num, 0);
            wait_time     = realtime'(wait_time_str.atoreal());
            if ((wait_time >= 0) && (wait_time > $realtime)) begin
               wait_time = wait_time - $realtime;
               #(wait_time);
            end
            result = evl_tcl_set_int_res(thread_num, 0, 0);
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "wait_cycles" Evaluation
         //
         else if (tcl_command == TCL_wait_cycles) begin
            int     clock_index;
            longint cycles;

            arg_count = evl_tcl_get_arg_count(thread_num);
            if (arg_count == 3) begin
               vc_name     = evl_tcl_get_str_arg(thread_num, 0);
               clock_index = evl_tcl_get_int_arg(thread_num, 1);
               cycles      = evl_tcl_get_int_arg(thread_num, 2);
            end
            else if (arg_count == 2) begin
               vc_name     = evl_tcl_get_str_arg(thread_num, 0);
               cycles      = evl_tcl_get_int_arg(thread_num, 1);
               if ((vc_name == "1") || (vc_name == "2")) begin
                  clock_index = evl_tcl_get_int_arg(thread_num, 0);
                  vc_name     = "";
               end
               else begin
                  clock_index = 0;
               end
            end
            else begin
               vc_name     = "";
               cycles      = evl_tcl_get_int_arg(thread_num, 0);
               clock_index = 0;
            end
            if (cycles < 0) begin
               if (m_cur_agent != null) begin
                  `dut_error($sformatf("%0s(TCL)", m_cur_agent.get_abstract_name()), $sformatf("wait_cycles count is a negative number (%0d)", cycles))
               end
               else begin
                  `dut_error($sformatf("TCL-%0d", thread_num), $sformatf("wait_cycles count is a negative number (%0d)", cycles))
               end
               return;
            end
            if (cycles == 0) begin
               #0;
            end
            else begin
               if (vc_name == "") begin
                  agent = m_cur_agent;
               end
               else begin
                  //
                  // parse_agent_name() wipes out any existing arguments and replaces them, so it
                  // must be done last.
                  //
                  parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
                  agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
               end
               if (agent != null) begin
                  if (m_wait_timeout <= 0) begin
                     agent.wait_cycles(int'(cycles), clock_index);
                  end
                  else begin
                     fork
                        begin
                           #(m_wait_timeout);
                           if (m_wait_timeout_err != "") begin
                              `dut_error(m_target_agent.get_abstract_name(), $sformatf("wait_cycles() timeout: %s", m_wait_timeout_err))
                           end
                        end
                        begin
                           agent.wait_cycles(int'(cycles), clock_index);
                        end
                     join_any
                     disable fork;
                  end
               end
               else if (m_system_agent != null) begin
                  if (m_wait_timeout <= 0) begin
                     m_system_agent.wait_cycles(int'(cycles), clock_index);
                  end
                  else begin
                     fork
                        begin
                           #(m_wait_timeout);
                           if (m_wait_timeout_err != "") begin
                              `dut_error(m_target_agent.get_abstract_name(), $sformatf("wait_cycles() timeout: %s", m_wait_timeout_err))
                           end
                        end
                        begin
                           m_system_agent.wait_cycles(int'(cycles), clock_index);
                        end
                     join_any
                     disable fork;
                  end
               end
               else begin
                  #0;
               end
            end
            result = cmd_from_tcl;
         end

         //
         // "get_cache_set" Evaluation
         //
         else if (tcl_command == TCL_get_cache_set) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_set argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid get_cache_set argument)"))
               not_valid = 1'b1;
            end
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               int set;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               if (index == -1) begin
                  index = 0;
               end
               set    = agent.get_cache_set(evl_paddr_t'(index), ram_way, ram_type);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(set));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_cache_bank" Evaluation
         //
         else if (tcl_command == TCL_get_cache_bank) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_bank argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid get_cache_bank argument)"))
               not_valid = 1'b1;
            end
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               int bank;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               if (index == -1) begin
                  index = 0;
               end
               bank   = agent.get_cache_bank(evl_paddr_t'(index), ram_way, ram_type);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(bank));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_cache_subbank" Evaluation
         //
         else if (tcl_command == TCL_get_cache_subbank) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_subbank argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid get_cache_subbank argument)"))
               not_valid = 1'b1;
            end
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               int subbank;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               if (index == -1) begin
                  index = 0;
               end
               subbank = agent.get_cache_subbank(evl_paddr_t'(index), ram_way, ram_type);
               result  = evl_tcl_set_int_res(thread_num, 0, longint'(subbank));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_cache_tag" Evaluation
         //
         else if (tcl_command == TCL_get_cache_tag) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else if (ram_str == "icache") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE_TAG);
                  end
                  else if (ram_str == "icache0") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE0_TAG);
                  end
                  else if (ram_str == "icache1") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE1_TAG);
                  end
                  else if (ram_str == "icache2") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE2_TAG);
                  end
                  else if (ram_str == "icache3") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE3_TAG);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_tag argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid get_cache_tag argument)"))
               not_valid = 1'b1;
            end
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               evl_paddr_t paddr;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               if (index == -1) begin
                  index = 0;
               end
               paddr = agent.get_cache_tag(evl_paddr_t'(index), ram_way, ram_type);
               if (paddr == ~evl_paddr_t'(0)) begin
                  result = evl_tcl_set_int_res(thread_num, 0, -1);
               end
               else begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(paddr));
               end
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_cache_data" Evaluation
         //
         else if (tcl_command == TCL_get_cache_data) begin
            bit         not_valid = 1'b0;
            int         ram_type;
            int         ram_way;
            longint     index;
            evl_dword_t dwords[];

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else if (ram_str == "icache") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE_DATA);
                  end
                  else if (ram_str == "icache0") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE0_DATA);
                  end
                  else if (ram_str == "icache1") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE1_DATA);
                  end
                  else if (ram_str == "icache2") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE2_DATA);
                  end
                  else if (ram_str == "icache3") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE3_DATA);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid get_cache_data argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid get_cache_data argument)"))
               not_valid = 1'b1;
            end
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
               result = evl_tcl_set_int_res(thread_num, 1, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 2, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 3, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 4, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 5, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 6, -1) | result;
               result = evl_tcl_set_int_res(thread_num, 7, -1) | result;
            end
            else begin
               evl_paddr_t paddr;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               if (index == -1) begin
                  index = 0;
               end
               paddr = agent.get_cache_data(dwords, evl_paddr_t'(index), ram_way, ram_type);
               if (paddr == ~evl_paddr_t'(0)) begin
                  result = evl_tcl_set_int_res(thread_num, 0, -1);
                  result = evl_tcl_set_int_res(thread_num, 1, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 2, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 3, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 4, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 5, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 6, -1) | result;
                  result = evl_tcl_set_int_res(thread_num, 7, -1) | result;
               end
               else begin
                  result = evl_tcl_set_int_res(thread_num, 0, longint'(dwords[0]));
                  result = evl_tcl_set_int_res(thread_num, 1, longint'(dwords[1])) | result;
                  result = evl_tcl_set_int_res(thread_num, 2, longint'(dwords[2])) | result;
                  result = evl_tcl_set_int_res(thread_num, 3, longint'(dwords[3])) | result;
                  result = evl_tcl_set_int_res(thread_num, 4, longint'(dwords[4])) | result;
                  result = evl_tcl_set_int_res(thread_num, 5, longint'(dwords[5])) | result;
                  result = evl_tcl_set_int_res(thread_num, 6, longint'(dwords[6])) | result;
                  result = evl_tcl_set_int_res(thread_num, 7, longint'(dwords[7])) | result;
               end
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "set_cache_bit_err" Evaluation
         //
         else if (tcl_command == TCL_set_cache_bit_err) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            int     bit0;
            int     bit1;
            int     value;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            bit0      = -1;
            bit1      = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else if (ram_str == "icache") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE_DATA);
                  end
                  else if (ram_str == "icache0") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE0_DATA);
                  end
                  else if (ram_str == "icache1") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE1_DATA);
                  end
                  else if (ram_str == "icache2") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE2_DATA);
                  end
                  else if (ram_str == "icache3") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE3_DATA);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else if (bit0 == -1) begin
                  bit0 = evl_tcl_get_int_arg(thread_num, ii);
               end
               else if (bit1 == -1) begin
                  bit1 = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid set_cache_bit_err argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid set_cache_bit_err argument)"))
               not_valid = 1'b1;
            end
            else if (bit0 < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the bit index has not been specified or is negative (missing/invalid set_cache_bit_err argument)"))
               not_valid = 1'b1;
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               evl_paddr_t paddr;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               value  = agent.set_cache_bit_err(evl_paddr_t'(index), bit0, bit1, ram_way, ram_type);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "clr_cache_bit_err" Evaluation
         //
         else if (tcl_command == TCL_clr_cache_bit_err) begin
            bit     not_valid = 1'b0;
            int     ram_type;
            int     ram_way;
            int     value;
            longint index;

            arg_count = evl_tcl_get_arg_count(thread_num);
            vc_name   = "";
            index     = -1;
            ram_type  = -1;
            ram_way   = -1;
            for (int ii = 0; ii < arg_count; ii++) begin
               string arg;

               arg = evl_tcl_get_str_arg(thread_num, ii);
               arg = arg.tolower();
               if (arg.substr(0, 3) == "ram=") begin
                  string ram_str;

                  if (ram_type != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument (RAM type already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "ram=%s", ram_str) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ((ram_str == "sc") || (ram_str == "scp")) begin
                     ram_type = int'(SC_RAM_TYPE_SC);
                  end
                  else if (ram_str == "l2") begin
                     ram_type = int'(SC_RAM_TYPE_L2);
                  end
                  else if (ram_str == "l2data") begin
                     ram_type = int'(SC_RAM_TYPE_L2_DATA);
                  end
                  else if (ram_str == "l2tag") begin
                     ram_type = int'(SC_RAM_TYPE_L2_TAG);
                  end
                  else if (ram_str == "l2state") begin
                     ram_type = int'(SC_RAM_TYPE_L2_STATE);
                  end
                  else if (ram_str == "l3") begin
                     ram_type = int'(SC_RAM_TYPE_L3);
                  end
                  else if (ram_str == "l3data") begin
                     ram_type = int'(SC_RAM_TYPE_L3_DATA);
                  end
                  else if (ram_str == "l3tag") begin
                     ram_type = int'(SC_RAM_TYPE_L3_TAG);
                  end
                  else if (ram_str == "l3state") begin
                     ram_type = int'(SC_RAM_TYPE_L3_STATE);
                  end
                  else if (ram_str == "icache") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE_DATA);
                  end
                  else if (ram_str == "icache0") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE0_DATA);
                  end
                  else if (ram_str == "icache1") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE1_DATA);
                  end
                  else if (ram_str == "icache2") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE2_DATA);
                  end
                  else if (ram_str == "icache3") begin
                     ram_type = int'(SC_RAM_TYPE_ICACHE3_DATA);
                  end
                  else begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument (RAM type is invalid)", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (arg.substr(0, 3) == "way=") begin
                  if (ram_way != -1) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument (RAM way already selected)", arg))
                     not_valid = 1'b1;
                     break;
                  end
                  if ($sscanf(arg, "way=%d", ram_way) <= 0) begin
                     `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument", arg))
                     not_valid = 1'b1;
                     break;
                  end
               end
               else if (vc_name == "") begin
                  vc_name = arg;
               end
               else if (index == -1) begin
                  index = evl_tcl_get_int_arg(thread_num, ii);
               end
               else begin
                  `dut_error(m_target_agent.get_abstract_name(), $sformatf("\"%0s\" is an invalid clr_cache_bit_err argument", arg))
                  not_valid = 1'b1;
                  break;
               end
            end
            if (index < 0) begin
               `dut_error(m_target_agent.get_abstract_name(), $sformatf("the cache index has not been specified or is negative (missing/invalid clr_cache_bit_err argument)"))
               not_valid = 1'b1;
            end
            //
            // parse_agent_name() wipes out any existing arguments and replaces them, so it must be
            // done last.
            //
            if (vc_name == "") begin
               parse_agent_name(thread_num, $sformatf("S%0d:SHIRE_RTL_AGENT", m_cur_agent.get_shire_id()), vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            else begin
               vc_name = vc_name.toupper();
               parse_agent_name(thread_num, vc_name, vc_name, agent_id, shire_id, neighborhood_id, core_id);
            end
            agent = evl_verif_comp_map::get_verif_comp_by_name(vc_name, agent_id, shire_id, neighborhood_id, core_id);
            if ((agent == null) || (not_valid == 1'b1)) begin
               result = evl_tcl_set_int_res(thread_num, 0, -1);
            end
            else begin
               evl_paddr_t paddr;

               if (ram_type == -1) begin
                  ram_type = int'(SC_RAM_TYPE_L2);
               end
               if (ram_way == -1) begin
                  ram_way = 0;
               end
               value  = agent.set_cache_bit_err(evl_paddr_t'(index), -1, -1, ram_way, ram_type);
               result = evl_tcl_set_int_res(thread_num, 0, longint'(value));
            end
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_net_binary_value" Evaluation
         //
         else if (tcl_command == TCL_get_net_binary_value) begin
            result = evl_tcl_set_str_res(thread_num, 0, evl_tcl_get_net_binary_value(evl_tcl_get_str_arg(thread_num, 0)));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end

         //
         // "get_net_hex_value" Evaluation
         //
         else if (tcl_command == TCL_get_net_hex_value) begin
            result = evl_tcl_set_str_res(thread_num, 0, evl_tcl_get_net_hex_value(evl_tcl_get_str_arg(thread_num, 0)));
            if (result < 0) begin
               return;
            end
            result = cmd_from_tcl;
         end
      end
      return;
   endtask : parse_tcl_command


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tcl_default_parser");
      super.new(name_in);
      m_instr_watch = null;
   endfunction : new

endclass : evl_tcl_default_parser

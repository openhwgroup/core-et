//--------------------------------------------------------------------------------------------------
//
//
// Class: evl_verbosity_mon
//
class evl_verbosity_mon;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables
   //
   int m_verbosity;
   int m_global_verbosity;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: set_verbosity
   //
   task wait_for_verbosity_change(output int cur_verbosity, input int verbosity_in);
      #0 wait (((m_verbosity >= 0) && (m_verbosity != verbosity_in)) || ((m_verbosity < 0) && (m_global_verbosity != verbosity_in)));
      cur_verbosity = (m_verbosity >= 0) ? m_verbosity : m_global_verbosity;
   endtask : wait_for_verbosity_change


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_verbosity
   //
   function void set_verbosity(input int verbosity_in);
      m_verbosity = verbosity_in;
   endfunction : set_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_global_verbosity
   //
   function void set_global_verbosity(input int verbosity_in);
      m_global_verbosity = verbosity_in;
   endfunction : set_global_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input int verbosity_in, input int global_verbosity_in);
      m_verbosity        = verbosity_in;
      m_global_verbosity = global_verbosity_in;
   endfunction : new
endclass : evl_verbosity_mon


//--------------------------------------------------------------------------------------------------
//
// Class: evl_log
//
class evl_log;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_log m_log = evl_log::get_inst();


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   static int                 m_verbosities[256];
   static int                 m_verbosity_xref[string];
   static evl_verbosity_mod_t m_verbosity_mod_xref[string];
   static evl_verbosity_mon   m_verbosity_mons[evl_verbosity_mod_t];
   static int                 m_plusarg_counts[string];
   static int                 m_dut_err_mask;
   static int                 m_int_params[int];
   static int                 m_error_count      = 0;
   static int                 m_warning_count    = 0;
   static int                 m_max_error_count  = 1;
   static bit                 m_assert_disabled  = 1'b0;
   static bit                 m_dump_started     = 1'b0;
   static bit                 m_dump_enabled     = 1'b0;
   static bit                 m_use_vpd          = 1'b0;
   static bit [8191:0]        m_fsdb_file_name   = {8192{1'b0}};;
   static bit [8191:0]        m_fsdb_config_file = {8192{1'b0}};;

   `evl_param_key(pk_global_intensity)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_log::get_inst
   //
   static function evl_log get_inst();
      if (m_log == null) begin
         evl_log tmp_log;

         tmp_log = new();
      end
      return m_log;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   // This function is used to set an integer parameter using a particular key.
   //
   // Input Variables:
   //
   // param_key:   This is the integer key (i.e., the associative array index) to use for the
   //              integer parameter.
   // param_value: This is the value to assign for the given integer parameter.
   //
   function void set_int_param(input int param_key, input int param_value);
      m_int_params[param_key] = param_value;
   endfunction : set_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int_param
   //
   // This function is used to get the integer parameter at the specified key.
   //
   // Input Variables:
   //
   // param_key:     This is the integer key (i.e., the associative array index) to use for the
   //                integer parameter.
   // param_default: This is the default value to return if no integer parameter exists at the
   //                specified key.
   //
   // Return Value:
   //
   // int: This is the value of the integer parameter at the specified key.  If no integer parameter
   //      exists at that key, then the param_default value will be returned.
   //
   function int get_int_param(input int param_key, input int param_default = -1);
      if (!m_int_params.exists(param_key)) begin
         return param_default;
      end
      return m_int_params[param_key];
   endfunction : get_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_verbosity
   //
   function void set_verbosity(input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
      if (m_verbosities[int'(verbosity_module_in)] != verbosity_in) begin
         `ifdef ET_USE_UVM
            uvm_pkg::uvm_verbosity verbosity = uvm_pkg::uvm_verbosity'(verbosity_in);

            print("", $sformatf("setting %p verbosity to %p", verbosity_module_in, verbosity));
         `else
            print("", $sformatf("setting %p verbosity to %0d", verbosity_module_in, verbosity_in));
         `endif
      end
      m_verbosities[int'(verbosity_module_in)] = verbosity_in;
      if (verbosity_module_in == VMOD_GLOBAL) begin
         foreach (m_verbosity_mons[ii]) begin
            m_verbosity_mons[ii].set_global_verbosity(verbosity_in);
         end
      end
      else if (m_verbosity_mons.exists(verbosity_module_in)) begin
         m_verbosity_mons[verbosity_module_in].set_verbosity(verbosity_in);
      end
   endfunction : set_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_verbosity
   //
   function bit check_verbosity(input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
      if (m_verbosities[verbosity_module_in] == -1) begin
         return ((verbosity_in <= m_verbosities[0]) ? 1'b1 : 1'b0);
      end
      return ((verbosity_in <= m_verbosities[int'(verbosity_module_in)]) ? 1'b1 : 1'b0);
   endfunction : check_verbosity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verbosity_by_name
   //
   function int get_verbosity_by_name(input string verbosity_name_in);
      if (!(m_verbosity_xref.exists(verbosity_name_in))) begin
         return int'(`UVM_MEDIUM);
      end
      return m_verbosity_xref[verbosity_name_in];
   endfunction : get_verbosity_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_verbosity_mod_by_name
   //
   function evl_verbosity_mod_t get_verbosity_mod_by_name(input string verbosity_mod_name_in);
      if (!(m_verbosity_mod_xref.exists(verbosity_mod_name_in))) begin
         return VMOD_GLOBAL;
      end
      return m_verbosity_mod_xref[verbosity_mod_name_in];
   endfunction : get_verbosity_mod_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_verbosity_change
   //
   task wait_for_verbosity_change(output int cur_verbosity, input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
      #0;
      if (!m_verbosity_mons.exists(verbosity_module_in)) begin
         m_verbosity_mons[verbosity_module_in] = new(m_verbosities[verbosity_module_in], m_verbosities[0]);
      end
      m_verbosity_mons[verbosity_module_in].wait_for_verbosity_change(cur_verbosity, verbosity_in);
   endtask : wait_for_verbosity_change


   //-----------------------------------------------------------------------------------------------
   //
   // Function: parse_trid_info
   //
   function string parse_trid_info(input bit            expand_on_null,
                                   input evl_paddr_t    paddr_in    = ~(evl_paddr_t'(0)),
                                   input evl_trans_id_t trans_id_in = ~(evl_trans_id_t'(0)),
                                   input int            tag_in      = ~(0));
      evl_bus_req_id_t req_id;
      string           message;

      req_id = evl_bus_req_id_t'(tag_in);
      if (expand_on_null == 1'b1) begin
         if (paddr_in != ~(evl_paddr_t'(0))) begin
            if (trans_id_in != ~(evl_trans_id_t'(0))) begin
               if (tag_in != ~(0)) begin
                  return $sformatf("%x-%x-%x", paddr_in & const_line_paddr_mask, trans_id_in, req_id);
               end
               else begin
                  return $sformatf("%x-%x-#####", paddr_in & const_line_paddr_mask, trans_id_in);
               end
            end
            else begin
               if (tag_in != ~(0)) begin
                  return $sformatf("%x-##########-%x", paddr_in & const_line_paddr_mask, req_id);
               end
               else begin
                  return $sformatf("%x-##########-#####", paddr_in & const_line_paddr_mask);
               end
            end
         end
         else begin
            if (trans_id_in != ~(evl_trans_id_t'(0))) begin
               if (tag_in != ~(0)) begin
                  return $sformatf("##########-%x-%x", trans_id_in, req_id);
               end
               else begin
                  return $sformatf("##########-%x-#####", trans_id_in);
               end
            end
            else begin
               if (tag_in != ~(0)) begin
                  return $sformatf("##########-##########-%x", req_id);
               end
               else begin
                  return $sformatf("                           ");
               end
            end
         end
      end
      else begin
         if (paddr_in != ~(evl_paddr_t'(0))) begin
            if (trans_id_in != ~(evl_trans_id_t'(0))) begin
               if (tag_in != ~(0)) begin
                  return $sformatf("%x-%x-%x", paddr_in & const_line_paddr_mask, trans_id_in, req_id);
               end
               else begin
                  return $sformatf("%x-%x", paddr_in & const_line_paddr_mask, trans_id_in);
               end
            end
            else begin
               return $sformatf("%x", paddr_in & const_line_paddr_mask);
            end
         end
      end
      return "";
   endfunction : parse_trid_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_no_time
   //
   function void print_no_time(input string message_in = "");
      $display("%0s", message_in);
   endfunction : print_no_time


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_no_name
   //
   function void print_no_name(input string message_in = "");
      print_no_time($sformatf("%012t %0s", $realtime, message_in));
   endfunction : print_no_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_no_addr
   //
   function void print_no_addr(input string abstract_name_in   = "",
                               input string message_in         = "",
                               input bit    wide_abstract_name = 1'b0);
      if (wide_abstract_name == 1'b0) begin
         print_no_time($sformatf("%012t %-28s %0s", $realtime, abstract_name_in, message_in));
      end
      else begin
         print_no_time($sformatf("%012t %-56s %0s", $realtime, abstract_name_in, message_in));
      end
   endfunction : print_no_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print
   //
   function void print(input string         abstract_name_in = "",
                       input string         message_in       = "",
                       input bit [1:0]      use_addr         = 2'b00,
                       input evl_paddr_t    paddr_in         = ~(evl_paddr_t'(0)),
                       input evl_trans_id_t trans_id_in      = ~(evl_trans_id_t'(0)),
                       input int            tag_in           = ~(0));
      if (use_addr == 2'b01) begin
         print_no_addr(abstract_name_in, $sformatf("%0s %0s", parse_trid_info(1'b1, paddr_in, trans_id_in, tag_in), message_in));
      end
      else if (use_addr == 2'b00) begin
         print_no_addr(abstract_name_in, $sformatf("%0s %0s", parse_trid_info(1'b1), message_in));
      end
      else begin
         print_no_addr(abstract_name_in, message_in, 1'b1);
      end
   endfunction : print


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_intensity
   //
   function void set_intensity(input int intensity_key, input int verbosity);
      if (intensity_key > 0) begin
         set_int_param(intensity_key, verbosity);
      end
   endfunction : set_intensity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_intensity_by_name
   //
   function void set_intensity_by_name(input string intensity_name, input int verbosity);
      set_intensity(evl_param_xref::get_param_key(intensity_name), verbosity);
   endfunction : set_intensity_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_intensity
   //
   function int get_intensity(input int intensity_key);
      if (intensity_key > 0) begin
         return get_int_param(intensity_key, -1);
      end
      return -1;
   endfunction : get_intensity


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_intensity_by_name
   //
   function int get_intensity_by_name(input string intensity_name);
      return get_intensity(evl_param_xref::get_param_key(intensity_name));
   endfunction : get_intensity_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dut_error
   //
   function void dut_error(input string              abstract_name_in    = "",
                           input string              message_in          = "",
                           input int                 intensity_key       = 0,
                           input evl_verbosity_mod_t verbosity_mods_in[] = {},
                           input evl_paddr_t         paddr_in            = ~(evl_paddr_t'(0)),
                           input evl_trans_id_t      trans_id_in         = ~(evl_trans_id_t'(0)),
                           input int                 tag_in              = ~(0),
                           input string              rtl_path_in         = "");
      string message;

      if (m_dut_err_mask > 1) begin
         return;
      end
      if (m_dut_err_mask > 0) begin
         dut_warning(abstract_name_in, message_in, pk_global_intensity, { VMOD_GLOBAL }, paddr_in, trans_id_in, tag_in, rtl_path_in);
         return;
      end
      if ((intensity_key != 0) && (get_int_param(intensity_key, -1) >= 0)) begin
         dut_warning(abstract_name_in, message_in, intensity_key, verbosity_mods_in, paddr_in, trans_id_in, tag_in, rtl_path_in);
         return;
      end
      message = parse_trid_info(1'b0, paddr_in, trans_id_in, tag_in);
      if (message == "") begin
         if (rtl_path_in == "") begin
            print_no_name($sformatf("DUT Error (ERROR): %0s: %0s", abstract_name_in, message_in));
         end
         else begin
            print_no_name($sformatf("DUT Error (ERROR): %0s: %0s (%0s)", abstract_name_in, message_in, rtl_path_in));
         end
      end
      else begin
         if (rtl_path_in == "") begin
            print_no_name($sformatf("DUT Error (ERROR): %0s: %0s (%0s)", abstract_name_in, message_in, message));
         end
         else begin
            print_no_name($sformatf("DUT Error (ERROR): %0s: %0s (%0s, %0s)", abstract_name_in, message_in, message, rtl_path_in));
         end
      end
      incr_errors();
   endfunction : dut_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dut_warning
   //
   function void dut_warning(input string              abstract_name_in    = "",
                             input string              message_in          = "",
                             input int                 intensity_key       = 0,
                             input evl_verbosity_mod_t verbosity_mods_in[] = {},
                             input evl_paddr_t         paddr_in            = ~(evl_paddr_t'(0)),
                             input evl_trans_id_t      trans_id_in         = ~(evl_trans_id_t'(0)),
                             input int                 tag_in              = ~(0),
                             input string              rtl_path_in         = "");
      int    intensity = (intensity_key == 0) ? -1 : get_int_param(intensity_key, -1);
      string message;

      if (intensity >= 0) begin
         bit have_verbosity = 1'b0;

         foreach (verbosity_mods_in[ii]) begin
            if (m_verbosities[verbosity_mods_in[ii]] >= intensity) begin
               intensity = -1;
               break;
            end
            if (m_verbosities[verbosity_mods_in[ii]] != -1) begin
               have_verbosity = 1'b1;
            end
         end
         if ((intensity >= 0) && (have_verbosity == 1'b0)) begin
            if (m_verbosities[0] >= intensity) begin
               intensity = -1;
            end
         end
         if (intensity >= 0) begin
            return;
         end
      end
      message = parse_trid_info(1'b0, paddr_in, trans_id_in, tag_in);
      if (message == "") begin
         if (rtl_path_in == "") begin
            print_no_name($sformatf("DUT Warning (WARNING): %0s: %0s", abstract_name_in, message_in));
         end
         else begin
            print_no_name($sformatf("DUT Warning (WARNING): %0s: %0s (%0s)", abstract_name_in, message_in, rtl_path_in));
         end
      end
      else begin
         if (rtl_path_in == "") begin
            print_no_name($sformatf("DUT Warning (WARNING): %0s: %0s (%0s)", abstract_name_in, message_in, message));
         end
         else begin
            print_no_name($sformatf("DUT Warning (WARNING): %0s: %0s (%0s, %0s)", abstract_name_in, message_in, message, rtl_path_in));
         end
      end
      incr_warnings();
   endfunction : dut_warning


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_errors
   //
   function void incr_errors();
      m_error_count++;
   endfunction : incr_errors


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_error_count
   //
   function int get_error_count();
      return m_error_count;
   endfunction : get_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_error_finish
   //
   task wait_for_error_finish();
      #0 wait ((m_error_count > 0) && (m_error_count >= m_max_error_count));
   endtask : wait_for_error_finish


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_max_error_count
   //
   function void set_max_error_count(input int max_error_count_in);
      m_max_error_count = max_error_count_in;
   endfunction : set_max_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_warnings
   //
   function void incr_warnings();
      m_warning_count++;
   endfunction : incr_warnings


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_warning_count
   //
   function int get_warning_count();
      return m_warning_count;
   endfunction : get_warning_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_plusarg
   //
   function void check_plusarg(input string plus_arg_in, input string file_name, input int line_number);
      int    limit;
      string plus_arg;

      limit = plus_arg_in.len();
      for (int ii = 0; ii < limit; ii++) begin
         if (plus_arg_in.getc(ii) == "=") begin
            limit = ii;
            break;
         end
      end
      if (limit < 1) begin
         return;
      end
      plus_arg = $sformatf("%0s (%0s, %0d)", plus_arg_in.substr(0, limit - 1), file_name, line_number);
      if (!m_plusarg_counts.exists(plus_arg)) begin
         m_plusarg_counts[plus_arg] = 1;
      end
      else begin
         m_plusarg_counts[plus_arg]++;
      end
   endfunction : check_plusarg


   //-----------------------------------------------------------------------------------------------
   //
   // Task: enable_dump
   //
   //   This task can be used to turn on dumping regardless of the +dump plusarg.
   //
   task enable_dump();
      if (m_dump_enabled == 1'b0) begin
         if (m_use_vpd == 1'b1) begin
            `ifdef EVL_ALLOW_VPD
               m_dump_enabled = 1'b1;
               #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("enabling VPD dump"))
               $vcdplusmemon;
               $vcdpluson();
            `else // ifdef EVL_ALLOW_VPD
               `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("dumping not enabled ... recompile to allow dumping"))
            `endif // !ifdef EVL_ALLOW_VPD
         end
         else begin
            `ifdef EVL_ALLOW_FSDB
               #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("enabling FSDB dump"))
               m_dump_enabled = 1'b1;
               if (m_dump_started == 0) begin
                  int max_size;
                  int file_num;

                  m_dump_started = 1;
                  if (!`et_value_plusargs("fsdbTraceFile=%s", m_fsdb_file_name)) begin
                     m_fsdb_file_name = "trace.fsdb";
                  end
                  if (`et_value_plusargs("fsdbSizeMax=%d", max_size) == 0) begin
                     max_size = 0;
                  end
                  if (`et_value_plusargs("fsdbFileNum=%d", file_num) == 0) begin
                     file_num = 20;
                  end

                  `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("FSDB Parameters: max_size=%0d, m_fsdb_file_name=%0s, file_num=%0d", max_size, m_fsdb_file_name, file_num))

                  if (max_size >= 10) begin
                    `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("Generating Size Limited FSDB: max_size=%0d, m_fsdb_file_name=%0s, file_num=%0d", max_size, m_fsdb_file_name, file_num))
                     $fsdbAutoSwitchDumpfile(max_size, m_fsdb_file_name, file_num);
                  end
                  else begin
                     $fsdbDumpfile(m_fsdb_file_name);
                  end
                  if (`et_value_plusargs("fsdbConfigFile=%s", m_fsdb_config_file)) begin
                    `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("using FSDB config file %0s", m_fsdb_config_file))
                     $fsdbDumpvarsByFile(m_fsdb_config_file);
                  end
                  else begin
                     `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("Using FSDB +all"))
                     $fsdbDumpvars("+all");
                  end
               end
               $fsdbDumpon;
            `else // ifdef EVL_ALLOW_FSDB
               `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("dumping not enabled ... recompile to allow dumping"))
            `endif // !ifdef EVL_ALLOW_FSDB
         end
      end
   endtask : enable_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: disable_dump
   //
   task disable_dump();
      if (m_dump_enabled == 1'b1) begin
         m_dump_enabled = 1'b0;
         if (m_use_vpd == 1'b1) begin
            `ifdef EVL_ALLOW_VPD
               #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("disabling VPD dump"))
               $vcdplusoff();
            `endif // ifdef EVL_ALLOW_VPD
         end
         else begin
            `ifdef EVL_ALLOW_FSDB
               #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("disabling FSDB dump"))
               $fsdbDumpoff();
            `endif // ifdef EVL_ALLOW_FSDB
         end
      end
   endtask : disable_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: close_dump
   //
   task close_dump();
      disable_dump();
      `ifdef EVL_ALLOW_FSDB
         if (m_dump_started == 1'b1) begin
            #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("closing FSDB dump"))
            $fsdbDumpFinish();
         end
      `endif // ifdef EVL_ALLOW_FSDB
   endtask : close_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: check_dump_plusarg
   //
   //   This task should be used to check the dump plusarg at the beginning of the sim.
   //
   task check_dump_plusarg();
      int value;

      if (`et_value_plusargs("dump=%d", value) == 0) begin
         if (`et_value_plusargs("traceEnable=%d", value) == 0) begin
            value = 0;
         end
      end
      if (value != 0) begin
         enable_dump();
      end
   endtask : check_dump_plusarg


   //-----------------------------------------------------------------------------------------------
   //
   // Task: check_assert_plusarg
   //
   task check_assert_plusarg();
      int value;

      `ifdef ET_ASSERT_OFF
         value = 0;
      `else // ifdef ET_ASSERT_OFF
         if (`et_value_plusargs("assertEnable=%d", value) == 0) begin
            if (`et_value_plusargs("assertDisable=%d", value) == 0) begin
               value = 1;
            end
            else if (value != 0) begin
               value = 0;
            end
            else begin
               value = 1;
            end
         end
      `endif // !ifdef ET_ASSERT_OFF
      if (value == 0) begin
         disable_assertions();
      end
   endtask : check_assert_plusarg


   //-----------------------------------------------------------------------------------------------
   //
   // Task: enable_assertions
   //
   task enable_assertions();
      `ifdef ET_ASSERT_OFF
         if (m_assert_disabled == 1'b1) begin
            #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("cannot enable assertions ... they have not been compiled in"))
         end
      `else // !ifdef ET_ASSERT_OFF
         if (m_assert_disabled == 1'b1) begin
            #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("enabling assertions"))
            $asserton;
         end
         m_assert_disabled = 1'b0;
      `endif // !ifdef ET_ASSERT_OFF
   endtask : enable_assertions


   //-----------------------------------------------------------------------------------------------
   //
   // Task: disable_assertions
   //
   task disable_assertions();
      if (m_assert_disabled == 1'b0) begin
         #0 `evl_log(`UVM_NONE, VMOD_GLOBAL, "SYSTEM", $sformatf("disabling assertions"))
         $assertoff;
         $assertkill;
      end
      m_assert_disabled = 1'b1;
   endtask : disable_assertions


   //-----------------------------------------------------------------------------------------------
   //
   // Function: certify_plusargs
   //
   function void certify_plusargs();
      string plus_arg_list[$];

      foreach (m_plusarg_counts[plus_arg]) begin
         plus_arg_list.push_back(plus_arg);
      end
      if (plus_arg_list.size() == 0) begin
         return;
      end
      plus_arg_list.sort();
      foreach (plus_arg_list[ii]) begin
         print_no_addr("PLUSARG", $sformatf("%0s: %0d", plus_arg_list[ii], m_plusarg_counts[plus_arg_list[ii]]), 1'b0);
      end
   endfunction : certify_plusargs


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new(input string name_in = "evl_log");
      $timeformat(-9, 3, " ns");
      if (m_log == null) begin
         evl_verbosity_mod_t verbosity_mod;
         int                 value;
         string              verbosity_string;
         string              verbosity_mod_string;

         m_log = this;
         m_verbosities[0] = int'(`UVM_MEDIUM);
         for (int ii = 1; ii < $size(m_verbosities); ii++) begin
            m_verbosities[ii] = -1;
         end

         m_verbosity_xref["UVM_NONE"]   = int'(`UVM_NONE);
         m_verbosity_xref["UVM_LOW"]    = int'(`UVM_LOW);
         m_verbosity_xref["UVM_MEDIUM"] = int'(`UVM_MEDIUM);
         m_verbosity_xref["UVM_HIGH"]   = int'(`UVM_HIGH);
         m_verbosity_xref["UVM_FULL"]   = int'(`UVM_FULL);
         m_verbosity_xref["UVM_DEBUG"]  = int'(`UVM_DEBUG);
         m_verbosity_xref["NONE"]       = int'(`UVM_NONE);
         m_verbosity_xref["LOW"]        = int'(`UVM_LOW);
         m_verbosity_xref["MEDIUM"]     = int'(`UVM_MEDIUM);
         m_verbosity_xref["HIGH"]       = int'(`UVM_HIGH);
         m_verbosity_xref["FULL"]       = int'(`UVM_FULL);
         m_verbosity_xref["DEBUG"]      = int'(`UVM_DEBUG);

         verbosity_mod = verbosity_mod.first();
         while (1) begin
            string verbosity_mod_name;

            verbosity_mod_name = $sformatf("%p", verbosity_mod);
            if (verbosity_mod_name != "") begin
               m_verbosity_mod_xref[verbosity_mod_name] = verbosity_mod;
               verbosity_mod_name = verbosity_mod_name.substr(5, verbosity_mod_name.len() - 1);
               if (verbosity_mod_name != "") begin
                  m_verbosity_mod_xref[verbosity_mod_name] = verbosity_mod;
               end
            end
            if (verbosity_mod == verbosity_mod.last()) begin
               break;
            end
            verbosity_mod = verbosity_mod.next();
         end

         verbosity_mod_string = $sformatf("verbosityALL=%s", "%0s");
         if ($value$plusargs(verbosity_mod_string, verbosity_string) != 0) begin
            if (m_verbosity_xref.exists(verbosity_string)) begin
               verbosity_mod = verbosity_mod.first();
               while (1) begin
                  if (verbosity_mod != VMOD_GLOBAL) begin
                     set_verbosity(m_verbosity_xref[verbosity_string], verbosity_mod);
                  end
                  if (verbosity_mod == verbosity_mod.last()) begin
                     break;
                  end
                  verbosity_mod = verbosity_mod.next();
               end
            end
         end

         verbosity_mod_string = $sformatf("verbosity=%s", "%0s");
         if ($value$plusargs(verbosity_mod_string, verbosity_string) != 0) begin
            if (m_verbosity_xref.exists(verbosity_string)) begin
               set_verbosity(m_verbosity_xref[verbosity_string], VMOD_GLOBAL);
            end
         end
         verbosity_mod = verbosity_mod.first();
         while (1) begin
            verbosity_mod_string = $sformatf("%p", verbosity_mod);
            verbosity_mod_string = $sformatf("verbosity%s=%s", verbosity_mod_string.substr(5, verbosity_mod_string.len() - 1), "%s");
            if ($value$plusargs(verbosity_mod_string, verbosity_string) != 0) begin
               if (m_verbosity_xref.exists(verbosity_string)) begin
                  set_verbosity(m_verbosity_xref[verbosity_string], verbosity_mod);
               end
            end
            if (verbosity_mod == verbosity_mod.last()) begin
               break;
            end
            verbosity_mod = verbosity_mod.next();
         end
         if (m_verbosities[int'(VMOD_TIMER)] < 0) begin
            set_verbosity(int'(`UVM_MEDIUM), VMOD_TIMER);
         end
         if (m_verbosities[int'(VMOD_REPLAY)] < 0) begin
            set_verbosity(int'(`UVM_NONE), VMOD_REPLAY);
         end
         if (m_verbosities[int'(VMOD_MESH_HIST)] < 0) begin
            set_verbosity(int'(`UVM_NONE), VMOD_MESH_HIST);
         end

         m_dut_err_mask = 0;
         if (`et_value_plusargs("all_dut_err_off=%d", m_dut_err_mask) != 0) begin
            if (m_dut_err_mask != 0) begin
               m_dut_err_mask = 2;
            end
         end
         else if (`et_value_plusargs("all_dut_err_to_warning=%d", m_dut_err_mask) != 0) begin
            if (m_dut_err_mask != 0) begin
               m_dut_err_mask = 1;
            end
         end
         else begin
            m_dut_err_mask = 0;
         end
         set_intensity(pk_global_intensity, `UVM_LOW);

         if (`et_value_plusargs("maxErrorCount=%d", value) != 0) begin
            m_max_error_count = value;
         end

         if (`et_value_plusargs("use_vpd=%d", value) == 0) begin
            value = -1;
         end
         if (value == -1) begin
            `ifdef USE_VPD_OVER_FSDB
               m_use_vpd = 1'b1;
            `else // ifdef USE_VPD_OVER_FSDB
               m_use_vpd = 1'b0;
            `endif // !ifdef USE_VPD_OVER_FSDB
         end
         else begin
            m_use_vpd = (value == 0) ? 1'b0 : 1'b1;
         end
         `ifndef EVL_ALLOW_FSDB
            m_use_vpd = 1'b1;
         `endif // ifndef EVL_ALLOW_FSDB
         `ifndef EVL_ALLOW_VPD
            m_use_vpd = 1'b0;
         `endif // ifndef EVL_ALLOW_VPD
         fork
            check_dump_plusarg();
            check_assert_plusarg();
         join_none
      end
   endfunction : new

endclass : evl_log

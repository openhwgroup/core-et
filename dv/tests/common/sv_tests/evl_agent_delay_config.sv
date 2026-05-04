//--------------------------------------------------------------------------------------------------
//
// Class: evl_agent_delay_config
//
class evl_agent_delay_config extends evl_stimulus;

   `uvm_object_utils(evl_agent_delay_config)

   typedef evl_verif_comp evl_verif_comp_list_t[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_agent_delay_config m_agent_delay_config = new();
          evl_verif_comp_list_t  m_ignore_agent_list[evl_agent_type_t];

   `evl_param_key(pk_req_delay)
   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_bus_req_data_delay)
   `evl_param_key(pk_mem_req_delay)
   `evl_param_key(pk_mem_bus_rsp_delay)
   `evl_param_key(pk_rreq_ready_assert_count)
   `evl_param_key(pk_rreq_ready_deassert_count)
   `evl_param_key(pk_wreq_ready_assert_count)
   `evl_param_key(pk_wreq_ready_deassert_count)
   `evl_param_key(pk_wdata_ready_assert_count)
   `evl_param_key(pk_wdata_ready_deassert_count)
   `evl_param_key(pk_rrsp_ready_assert_count)
   `evl_param_key(pk_rrsp_ready_deassert_count)
   `evl_param_key(pk_wrsp_ready_assert_count)
   `evl_param_key(pk_wrsp_ready_deassert_count)
   `evl_param_key(pk_etlink_req_ready_assert_count)
   `evl_param_key(pk_etlink_req_ready_deassert_count)
   `evl_param_key(pk_etlink_rsp_ready_assert_count)
   `evl_param_key(pk_etlink_rsp_ready_deassert_count)
   `evl_param_key(pk_gate_ar_ready)
   `evl_param_key(pk_gate_aw_ready)
   `evl_param_key(pk_gate_w_ready)
   `evl_param_key(pk_gate_r_ready)
   `evl_param_key(pk_gate_b_ready)

   int m_axi_rdy_extreme_dly;
   int m_etlink_rdy_starvation_dly;

   //-----------------------------------------------------------------------------------------------
   class evl_agent_delays extends evl_delay_list;
      constraint c_num_delays { r_delays.size() inside { [100:200] }; }

      function new(input string name_in = "evl_agent_delays");
         super.new(name_in);
      endfunction : new
   endclass : evl_agent_delays


   //-----------------------------------------------------------------------------------------------
   class evl_agent_burst_delays extends evl_delay_list;
      constraint c_num_delays { r_delays.size() dist { [10:20]:/2, [30:40]:/4, [60:80]:/2 }; }

      function new(input string name_in = "evl_agent_delays");
         super.new(name_in);
      endfunction : new
   endclass : evl_agent_burst_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_etlink_req_delays
   //
   function void process_long_etlink_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:100] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long ETLINK stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_etlink_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_etlink_req_delays
   //
   function void process_medium_etlink_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium ETLINK stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_etlink_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_etlink_req_delays
   //
   function void process_short_etlink_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short ETLINK stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_etlink_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_etlink_req_delays
   //
   function void process_bursty_etlink_req_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_req_delay");
      agent_burst_delays = new("pk_req_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays       = new("pk_bus_req_delay");
      agent_burst_delays = new("pk_bus_req_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:8] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays       = new("pk_etlink_rsp_ready_assert_count");
      agent_burst_delays = new("pk_etlink_rsp_ready_assert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays       = new("pk_etlink_rsp_ready_deassert_count");
      agent_burst_delays = new("pk_etlink_rsp_ready_deassert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty ETLINK stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_etlink_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_axi_req_delays
   //
   function void process_long_axi_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_data_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_bus_req_data_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_axi_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_axi_req_delays
   //
   function void process_medium_axi_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_data_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
      delay_wrapper.add_delay_list(pk_bus_req_data_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_axi_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_axi_req_delays
   //
   function void process_short_axi_req_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays = new("pk_bus_req_data_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
      delay_wrapper.add_delay_list(pk_bus_req_data_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_axi_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_axi_req_delays
   //
   function void process_bursty_axi_req_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_req_delay");
      agent_burst_delays = new("pk_req_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_req_delay, agent_delays);

      agent_delays       = new("pk_bus_req_delay");
      agent_burst_delays = new("pk_bus_req_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:8] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_bus_req_delay, agent_delays);

      agent_delays       = new("pk_bus_req_data_delay");
      agent_burst_delays = new("pk_bus_req_data_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:8] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_bus_req_data_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI stimulus agent delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_axi_req_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_mem_rsp_delays
   //
   function void process_long_mem_rsp_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_mem_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [10:15] }); };
      delay_wrapper.add_delay_list(pk_mem_req_delay, agent_delays);

      agent_delays = new("pk_mem_bus_rsp_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
      delay_wrapper.add_delay_list(pk_mem_bus_rsp_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long memory response delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_mem_rsp_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_mem_rsp_delays
   //
   function void process_medium_mem_rsp_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_mem_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [3:5] }); };
      delay_wrapper.add_delay_list(pk_mem_req_delay, agent_delays);

      agent_delays = new("pk_mem_bus_rsp_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [10:20] }); };
      delay_wrapper.add_delay_list(pk_mem_bus_rsp_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium memory response delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_mem_rsp_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_mem_rsp_delays
   //
   function void process_short_mem_rsp_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_mem_req_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:4] }); };
      delay_wrapper.add_delay_list(pk_mem_req_delay, agent_delays);

      agent_delays = new("pk_mem_bus_rsp_delay");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:4] }); };
      delay_wrapper.add_delay_list(pk_mem_bus_rsp_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short memory response delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_mem_rsp_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_mem_rsp_delays
   //
   function void process_bursty_mem_rsp_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_mem_req_delay");
      agent_burst_delays = new("pk_mem_req_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:4] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:15] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_mem_req_delay, agent_delays);

      agent_delays       = new("pk_mem_bus_rsp_delay");
      agent_burst_delays = new("pk_mem_bus_rsp_delay");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:4] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [20:40] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_mem_bus_rsp_delay, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty memory response delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_mem_rsp_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_etlink_req_backp
   //
   function void process_long_etlink_req_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;
      int               min_delay;
      int               max_delay;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_req_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_assert_count, agent_delays);

      if (m_etlink_rdy_starvation_dly == 1) begin
         min_delay = (1024 + 512);
         max_delay = (2048);
         `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using starvation ETLINK req_ready delays")
      end
      else begin
         min_delay = 1;
         max_delay = 100;
      end

      agent_delays = new("pk_etlink_req_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [min_delay:max_delay] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long ETLINK req_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_etlink_req_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_etlink_req_backp
   //
   function void process_medium_etlink_req_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_req_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_req_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium ETLINK req_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_etlink_req_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_etlink_req_backp
   //
   function void process_short_etlink_req_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_req_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_req_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_etlink_req_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short ETLINK req_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_etlink_req_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_etlink_req_backp
   //
   function void process_bursty_etlink_req_backp(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_etlink_req_ready_assert_count");
      agent_burst_delays = new("pk_etlink_req_ready_assert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_req_ready_assert_count, agent_delays);

      agent_delays       = new("pk_etlink_req_ready_deassert_count");
      agent_burst_delays = new("pk_etlink_req_ready_deassert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_req_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty ETLINK req_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_etlink_req_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_etlink_rsp_backp
   //
   function void process_long_etlink_rsp_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:100] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long ETLINK rsp_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_etlink_rsp_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_etlink_rsp_backp
   //
   function void process_medium_etlink_rsp_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium ETLINK rsp_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_etlink_rsp_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_etlink_rsp_backp
   //
   function void process_short_etlink_rsp_backp(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_etlink_rsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_etlink_rsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short ETLINK rsp_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_etlink_rsp_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_etlink_rsp_backp
   //
   function void process_bursty_etlink_rsp_backp(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_etlink_rsp_ready_assert_count");
      agent_burst_delays = new("pk_etlink_rsp_ready_assert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_assert_count, agent_delays);

      agent_delays       = new("pk_etlink_rsp_ready_deassert_count");
      agent_burst_delays = new("pk_etlink_rsp_ready_deassert_count");
      repeat (10) begin
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_etlink_rsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty ETLINK rsp_ready delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_etlink_rsp_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_ar_rdy_delays
   //
   function void process_long_ar_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;
      int               min_delay;
      int               max_delay;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_rreq_ready_deassert_count");
      if (m_axi_rdy_extreme_dly == 1) begin
         min_delay = (16*1024);
         max_delay = (16*1024)+50;
      end
      else begin
         min_delay = 1;
         max_delay = 100;
      end
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [min_delay:max_delay] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI ARREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_ar_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_ar_rdy_delays
   //
   function void process_medium_ar_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_rreq_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI ARREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_ar_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_ar_rdy_delays
   //
   function void process_short_ar_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_rreq_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_rreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI ARREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_ar_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_ar_rdy_delays
   //
   function void process_bursty_ar_rdy_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_rreq_ready_assert_count");
      agent_burst_delays = new("pk_rreq_ready_assert_count");
      repeat (10) begin
         //
         // Assert AXI ARREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Assert AXI ARREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_rreq_ready_assert_count, agent_delays);

      agent_delays       = new("pk_rreq_ready_deassert_count");
      agent_burst_delays = new("pk_rreq_ready_deassert_count");
      repeat (10) begin
         //
         // Deassert AXI ARREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Deassert AXI ARREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_rreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI ARREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_ar_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_aw_rdy_delays
   //
   function void process_long_aw_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;
      int               min_delay;
      int               max_delay;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_wreq_ready_deassert_count");
      if (m_axi_rdy_extreme_dly == 1) begin
         min_delay = (16*1024);
         max_delay = (16*1024)+50;
      end
      else begin
         min_delay = 1;
         max_delay = 100;
      end
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [min_delay:max_delay] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI AWREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_aw_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_aw_rdy_delays
   //
   function void process_medium_aw_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_wreq_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI AWREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_aw_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_aw_rdy_delays
   //
   function void process_short_aw_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wreq_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_assert_count, agent_delays);

      agent_delays = new("pk_wreq_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_wreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI AWREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_aw_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_aw_rdy_delays
   //
   function void process_bursty_aw_rdy_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_wreq_ready_assert_count");
      agent_burst_delays = new("pk_wreq_ready_assert_count");
      repeat (10) begin
         //
         // Assert AXI AWREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Assert AXI AWREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wreq_ready_assert_count, agent_delays);

      agent_delays       = new("pk_wreq_ready_deassert_count");
      agent_burst_delays = new("pk_wreq_ready_deassert_count");
      repeat (10) begin
         //
         // Deassert AXI AWREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Deassert AXI AWREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wreq_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI AWREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_aw_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_w_rdy_delays
   //
   function void process_long_w_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wdata_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_assert_count, agent_delays);

      agent_delays = new("pk_wdata_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:100] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI WREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_w_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_w_rdy_delays
   //
   function void process_medium_w_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wdata_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_assert_count, agent_delays);

      agent_delays = new("pk_wdata_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI WREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_w_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_w_rdy_delays
   //
   function void process_short_w_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wdata_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_assert_count, agent_delays);

      agent_delays = new("pk_wdata_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_wdata_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI WREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_w_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_w_rdy_delays
   //
   function void process_bursty_w_rdy_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_wdata_ready_assert_count");
      agent_burst_delays = new("pk_wdata_ready_assert_count");
      repeat (10) begin
         //
         // Assert AXI WREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Assert AXI WREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wdata_ready_assert_count, agent_delays);

      agent_delays       = new("pk_wdata_ready_deassert_count");
      agent_burst_delays = new("pk_wdata_ready_deassert_count");
      repeat (10) begin
         //
         // Deassert AXI WREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Deassert AXI WREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wdata_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI WREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_w_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_r_rdy_delays
   //
   function void process_long_r_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_rrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:100] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI RREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_r_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_r_rdy_delays
   //
   function void process_medium_r_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_rrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI RREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_r_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_r_rdy_delays
   //
   function void process_short_r_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_rrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_rrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_rrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI RREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_r_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_r_rdy_delays
   //
   function void process_bursty_r_rdy_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_rrsp_ready_assert_count");
      agent_burst_delays = new("pk_rrsp_ready_assert_count");
      repeat (10) begin
         //
         // Assert AXI RREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Assert AXI RREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_rrsp_ready_assert_count, agent_delays);

      agent_delays       = new("pk_rrsp_ready_deassert_count");
      agent_burst_delays = new("pk_rrsp_ready_deassert_count");
      repeat (10) begin
         //
         // Deassert AXI RREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Deassert AXI RREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_rrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI RREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_r_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_long_b_rdy_delays
   //
   function void process_long_b_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:10] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_wrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:100] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using long AXI BREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_long_b_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_medium_b_rdy_delays
   //
   function void process_medium_b_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_wrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:20] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using medium AXI BREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_medium_b_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_short_b_rdy_delays
   //
   function void process_short_b_rdy_delays(input evl_verif_comp verif_comp);
      evl_agent_delays  agent_delays;
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays = new("pk_wrsp_ready_assert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:25] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_assert_count, agent_delays);

      agent_delays = new("pk_wrsp_ready_deassert_count");
      agent_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:3] }); };
      delay_wrapper.add_delay_list(pk_wrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using short AXI BREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_short_b_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bursty_b_rdy_delays
   //
   function void process_bursty_b_rdy_delays(input evl_verif_comp verif_comp);
      evl_delay_list         agent_delays;
      evl_agent_burst_delays agent_burst_delays;
      evl_delay_wrapper      delay_wrapper;
      evl_delay_t            delays[];

      delay_wrapper = new($sformatf("%0s(DLY)", verif_comp.get_root_abstract_name()));

      agent_delays       = new("pk_wrsp_ready_assert_count");
      agent_burst_delays = new("pk_wrsp_ready_assert_count");
      repeat (10) begin
         //
         // Assert AXI BREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Assert AXI BREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wrsp_ready_assert_count, agent_delays);

      agent_delays       = new("pk_wrsp_ready_deassert_count");
      agent_burst_delays = new("pk_wrsp_ready_deassert_count");
      repeat (10) begin
         //
         // Deassert AXI BREADY for a "short" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [1:5] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
         //
         // Deassert AXI BREADY for a "long" time.
         //
         agent_burst_delays.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { [5:50] }); };
         if (agent_burst_delays.get_delay_list(delays) > 0) begin
            agent_delays.concat_delay_list(delays);
         end
      end
      delay_wrapper.add_delay_list(pk_wrsp_ready_deassert_count, agent_delays);

      `evl_log(UVM_MEDIUM, VMOD_TEST, verif_comp.get_abstract_name(), "using bursty AXI BREADY delays")
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         delay_wrapper.print_obj();
      end
      verif_comp.set_delay_wrapper(delay_wrapper);
   endfunction : process_bursty_b_rdy_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: remove_agent_from_alias_list
   //
   function void remove_agent_from_alias_list(input evl_verif_comp agent);
      evl_agent_type_t agent_aliases[];

      if (agent.get_agent_aliases(agent_aliases) > 0) begin
         foreach (agent_aliases[ii]) begin
            m_ignore_agent_list[agent_aliases[ii]].push_back(agent);
         end
      end
   endfunction : remove_agent_from_alias_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      evl_verif_comp verif_comp_list[];
      evl_verif_comp verif_comp;

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, NEIGH_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("neighStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_etlink_req_delays(verif_comp);
                  2: process_medium_etlink_req_delays(verif_comp);
                  3: process_long_etlink_req_delays(verif_comp);
                  4: process_bursty_etlink_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, CORE_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("coreStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_etlink_req_delays(verif_comp);
                  2: process_medium_etlink_req_delays(verif_comp);
                  3: process_long_etlink_req_delays(verif_comp);
                  4: process_bursty_etlink_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end
      
      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, ETLINK_SLAVE_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("etlinkReqRdyDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_etlink_req_backp(verif_comp);
                  2: process_medium_etlink_req_backp(verif_comp);
                  3: process_long_etlink_req_backp(verif_comp);
                  4: process_bursty_etlink_req_backp(verif_comp);
               endcase
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, ETLINK_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value; 

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("etlinkRspRdyDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_etlink_rsp_backp(verif_comp);
                  2: process_medium_etlink_rsp_backp(verif_comp);
                  3: process_long_etlink_rsp_backp(verif_comp);
                  4: process_bursty_etlink_rsp_backp(verif_comp);
               endcase
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, L3_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("l3StimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, L2TOL3_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("l2tol3StimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, TOSYS_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("tosysStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, UCTOSYS_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("uctosysStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, ATOMIC_RSP_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("amorspStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, IOTOSYS_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("iotosysStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, PSHIRE_TOSYS_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("pstosysStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, IO_PM_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("iopmStimDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_axi_req_delays(verif_comp);
                  2: process_medium_axi_req_delays(verif_comp);
                  3: process_long_axi_req_delays(verif_comp);
                  4: process_bursty_axi_req_delays(verif_comp);
               endcase
               remove_agent_from_alias_list(verif_comp);
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, AXI_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            if (!m_ignore_agent_list.exists(AXI_MASTER_STIM_AGENT) || (!(verif_comp inside { m_ignore_agent_list[AXI_MASTER_STIM_AGENT] }))) begin
               arg_value  = verif_comp.get_plus_arg_value("axiReqDelays");
               arg_value  = arg_value.tolower();
               if (arg_value == "none") begin
                  arg_type = 0;
               end
               else if (arg_value == "short") begin
                  arg_type = 1;
               end
               else if (arg_value == "medium") begin
                  arg_type = 2;
               end
               else if (arg_value == "long") begin
                  arg_type = 3;
               end
               else if (arg_value == "bursty") begin
                  arg_type = 4;
               end
               else if (arg_value == "random") begin
                  arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
               end
               if (arg_type >= 0) begin
                  case (arg_type)
                     1: process_short_axi_req_delays(verif_comp);
                     2: process_medium_axi_req_delays(verif_comp);
                     3: process_long_axi_req_delays(verif_comp);
                     4: process_bursty_axi_req_delays(verif_comp);
                  endcase
               end
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, MEM_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type = -1;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("memDelays");
            arg_value  = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_mem_rsp_delays(verif_comp);
                  2: process_medium_mem_rsp_delays(verif_comp);
                  3: process_long_mem_rsp_delays(verif_comp);
                  4: process_bursty_mem_rsp_delays(verif_comp);
               endcase
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, AXI_SLAVE_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type;
            string arg_value;

            verif_comp = verif_comp_list[ii];

            arg_type  = -1;
            arg_value = verif_comp.get_plus_arg_value("axiReadReqRdyDelays");
            arg_value = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_ar_rdy_delays(verif_comp);
                  2: process_medium_ar_rdy_delays(verif_comp);
                  3: process_long_ar_rdy_delays(verif_comp);
                  4: process_bursty_ar_rdy_delays(verif_comp);
               endcase
            end

            arg_type  = -1;
            arg_value = verif_comp.get_plus_arg_value("axiWriteReqRdyDelays");
            arg_value = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_aw_rdy_delays(verif_comp);
                  2: process_medium_aw_rdy_delays(verif_comp);
                  3: process_long_aw_rdy_delays(verif_comp);
                  4: process_bursty_aw_rdy_delays(verif_comp);
               endcase
            end

            arg_type  = -1;
            arg_value = verif_comp.get_plus_arg_value("axiWriteDataRdyDelays");
            arg_value = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_w_rdy_delays(verif_comp);
                  2: process_medium_w_rdy_delays(verif_comp);
                  3: process_long_w_rdy_delays(verif_comp);
                  4: process_bursty_w_rdy_delays(verif_comp);
               endcase
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, AXI_MASTER_STIM_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type;
            string arg_value;

            verif_comp = verif_comp_list[ii];

            arg_type  = -1;
            arg_value = verif_comp.get_plus_arg_value("axiReadRspRdyDelays");
            arg_value = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_r_rdy_delays(verif_comp);
                  2: process_medium_r_rdy_delays(verif_comp);
                  3: process_long_r_rdy_delays(verif_comp);
                  4: process_bursty_r_rdy_delays(verif_comp);
               endcase
            end

            arg_type  = -1;
            arg_value = verif_comp.get_plus_arg_value("axiWriteRspRdyDelays");
            arg_value = arg_value.tolower();
            if (arg_value == "none") begin
               arg_type = 0;
            end
            else if (arg_value == "short") begin
               arg_type = 1;
            end
            else if (arg_value == "medium") begin
               arg_type = 2;
            end
            else if (arg_value == "long") begin
               arg_type = 3;
            end
            else if (arg_value == "bursty") begin
               arg_type = 4;
            end
            else if (arg_value == "random") begin
               arg_type = (`et_test_plusargs("allow_no_delay") == 0) ? (($urandom() % 4) + 1) : ($urandom() % 5);
            end
            if (arg_type >= 0) begin
               case (arg_type)
                  1: process_short_b_rdy_delays(verif_comp);
                  2: process_medium_b_rdy_delays(verif_comp);
                  3: process_long_b_rdy_delays(verif_comp);
                  4: process_bursty_b_rdy_delays(verif_comp);
               endcase
            end
         end
      end

      if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, AXI_BUS_AGENT) != 0) begin
         foreach (verif_comp_list[ii]) begin
            int    arg_type;
            string arg_value;

            verif_comp = verif_comp_list[ii];
            arg_value  = verif_comp.get_plus_arg_value("axiGateReady");
            if (arg_value == "on") begin
               verif_comp.set_rtl_int_param(pk_gate_ar_ready, 1);
               verif_comp.set_rtl_int_param(pk_gate_aw_ready, 1);
               verif_comp.set_rtl_int_param(pk_gate_w_ready, 1);
               verif_comp.set_rtl_int_param(pk_gate_r_ready, 1);
               verif_comp.set_rtl_int_param(pk_gate_b_ready, 1);
            end
            else if (arg_value == "random") begin
               arg_type = $urandom() & 32'h31;
               verif_comp.set_rtl_int_param(pk_gate_ar_ready, arg_type[0:0]);
               verif_comp.set_rtl_int_param(pk_gate_aw_ready, arg_type[1:1]);
               verif_comp.set_rtl_int_param(pk_gate_w_ready,  arg_type[2:2]);
               verif_comp.set_rtl_int_param(pk_gate_r_ready,  arg_type[3:3]);
               verif_comp.set_rtl_int_param(pk_gate_b_ready,  arg_type[4:4]);
            end
         end
      end
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_agent_delay_config");
      super.new(name_in);
      if (`et_test_plusargs("ultrasoc_axi_long_delay") != 0) begin
         m_axi_rdy_extreme_dly = 1;
      end
      else begin
         m_axi_rdy_extreme_dly = 0;
      end

      if (`et_test_plusargs("etlinkStarvationDelay") != 0) begin
         m_etlink_rdy_starvation_dly = 1;
      end
      else begin
         m_etlink_rdy_starvation_dly = 0;
      end

   endfunction

endclass : evl_agent_delay_config

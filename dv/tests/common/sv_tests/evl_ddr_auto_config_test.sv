//--------------------------------------------------------------------------------------------------
//
// Class: evl_ddr_auto_config_test
//
class evl_ddr_auto_config_test extends evl_stimulus;

   `uvm_object_utils(evl_ddr_auto_config_test)

   `evl_param_key(pk_ddr_stub)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_ddr_auto_config_test m_ddr_auto_config_test = new();


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ddr_auto_config_test_seq
   //
   class evl_ddr_auto_config_test_seq extends evl_sequence_mdir;

      bit            m_auto_init_start    = 1'b0;
      bit            m_auto_init_started  = 1'b0;
      bit            m_auto_init_complete = 1'b0;
      bit            m_is_memshire        = 1'b0;
      int unsigned   m_use_pll_beh;
      evl_verif_comp m_ms_rtl_agent;
      int            m_shire_id;

      //--------------------------------------------------------------------------------------------
      //
      // Task: perform_auto_init
      //
      task perform_auto_init();
         int config_value = 0;
         int value;

         value = m_agent.get_plus_arg_value_as_int("ddr_ecc_en");
         if (value > 0) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_ECC_BIT] = 1'b1;
         end

         if (m_agent.plus_arg_match("mem800mhz") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_FREQ_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_FREQ_800;
         end
         else if (m_agent.plus_arg_match("mem933mhz") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_FREQ_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_FREQ_933;
         end

         // total amount of memory in the system. Default is 16GB
         if (m_agent.plus_arg_match("ddr_4gbyte") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_4GB;
         end
         else if (m_agent.plus_arg_match("ddr_8gbyte") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_8GB;
         end
         else if (m_agent.plus_arg_match("ddr_32gbyte") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_GBYTES_32GB;
         end

         // find out if we're using the behavioral pll or the real model
         if (m_is_memshire == 1'b1) begin
            m_ms_rtl_agent = evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), m_shire_id);
         end
         else begin
            m_ms_rtl_agent = null;
         end
         if (m_ms_rtl_agent != null) begin
            m_use_pll_beh = m_ms_rtl_agent.get_rtl_int_param_by_name("pk_memshire_use_pll_beh", -1);
         end
         else begin
            `ifndef DV_MEMSHIRE_USE_PLL_BEH
               m_use_pll_beh = 1'b0;
            `else // ifndef DV_MEMSHIRE_USE_PLL_BEH
               m_use_pll_beh = (`DV_MEMSHIRE_USE_PLL_BEH == 0) ? 1'b0 : 1'b1;
            `endif // !ifndef DV_MEMSHIRE_USE_PLL_BEH
         end

         value = m_agent.get_plus_arg_value_as_int("skip_pll_init");

         // if not using beh pll and not skipping the pll initialization, then auto init the real pll
         if ((m_use_pll_beh == 0) && (!(value > 0))) begin
           config_value[`MEMSHIRE_AUTO_INIT_CONFIG_PLL_REAL_EN] = 1'b1;
         end
         else begin
           config_value[`MEMSHIRE_AUTO_INIT_CONFIG_PLL_REAL_EN] = 1'b0;
         end

         if (m_agent.plus_arg_match("mem800mhz") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_FREQ_BITS] = `MEMSHIRE_AUTO_INIT_CONFIG_FREQ_800;
         end

         if (m_agent.plus_arg_match("ddr_ap") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_AUTO_PRECHARGE] = 1'b1;
         end

         if (m_agent.plus_arg_match("ms_pwr_run") == 1'b1) begin
            config_value[`MEMSHIRE_AUTO_INIT_CONFIG_PWR_RUN] = 1'b1;
         end

         wait (m_auto_init_start == 1'b1);
         m_agent.wait_for_reset_deassertion();
         `evl_log(UVM_NONE, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("%0s: starting DDR auto-init sequence (0x%04x)", get_abstract_name(), config_value))
         m_agent.set_rtl_int_param_by_name("pk_ms_auto_init_config_drv", config_value);
         m_agent.set_rtl_int_param_by_name("pk_ms_auto_init_enable_drv", 1);
         m_agent.wait_cycles(50);
         fork
            begin
               m_agent.wait_for_rtl_int_param_by_name("pk_ms_auto_init_inprog_mon", 1, 1);
               m_auto_init_started = 1'b1;
            end
            begin
               m_agent.wait_for_rtl_int_param_by_name("pk_dv_ms_auto_init_done_mon", 1, 1);
               m_auto_init_started  = 1'b1;
               m_auto_init_complete = 1'b1;
            end
         join
      endtask : perform_auto_init


      //--------------------------------------------------------------------------------------------
      //
      // Task: post_reset_phase
      //
      virtual task post_reset_phase(input uvm_phase phase);
         phase.raise_objection(phase);
         m_auto_init_start = 1'b1;
         fork
            begin
               super.post_reset_phase(phase);
            end
            begin
               if (m_agent.get_plus_arg_value_as_int("no_ddr_auto_init_wait") <= 0) begin
                  int auto_init_timeout;

                  if (`et_value_plusargs("ddr_auto_init_timeout=%d", auto_init_timeout) == 0) begin
                     auto_init_timeout = 40000;
                  end
                  fork
                     begin :ddrc_start_timeout
                        m_agent.wait_cycles(auto_init_timeout);
                        `dut_error(m_agent.get_abstract_name(), $sformatf("timed out waiting for DDR auto-init to start"))
                        disable ddrc_wait_for_start;
                     end
                     begin :ddrc_wait_for_start
                        wait (m_auto_init_started == 1'b1);
                        disable ddrc_start_timeout;
                     end
                  join
                  fork
                     begin :ddrc_complete_timeout
                        m_agent.wait_cycles(auto_init_timeout);
                        `dut_error(m_agent.get_abstract_name(), $sformatf("timed out waiting for DDR auto-init to complete"))
                        disable ddrc_wait_for_complete;
                     end
                     begin :ddrc_wait_for_complete
                        wait (m_auto_init_complete == 1'b1);
                        disable ddrc_complete_timeout;
                        `evl_log(UVM_NONE, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("%0s: DDR auto-init sequence complete", get_abstract_name()))
                     end
                  join
               end
            end
         join
         phase.drop_objection(phase);
      endtask : post_reset_phase


      //--------------------------------------------------------------------------------------------
      //
      // Task: pre_configure_phase
      //
      virtual task pre_configure_phase(input uvm_phase phase);
         disable_stimulus(`EVL_ON_ICE_DISABLE);
         super.pre_configure_phase(phase);
      endtask : pre_configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_ddr_auto_config_test_seq", input evl_verif_comp agent_in = null, input bit is_memshire_in);
         super.new(name_in);
         if (agent_in != null) begin
            m_shire_id = agent_in.get_shire_id();
         end
         m_is_memshire = is_memshire_in;
         fork
            perform_auto_init();
         join_none
      endfunction

   endclass : evl_ddr_auto_config_test_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      longint        value;
      bit            use_sys_agent;
      evl_verif_comp verif_comp_list[];

      if (`et_test_plusargs("ddr_auto_init") == 0) begin
         return;
      end
      `ifdef MEMSHIRE_AUTO_INIT_ENABLE
         if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, DDR_SHIRE_RTL_AGENT) != 0) begin
            evl_verif_comp verif_comp;

            foreach (verif_comp_list[ii]) begin
               if (verif_comp_list[ii].get_rtl_int_param(pk_ddr_stub, 0) == 0) begin
                  evl_shire_id_t shire_id;

                  shire_id   = verif_comp_list[ii].get_shire_id();
                  verif_comp = evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id);
                  if (verif_comp != null) begin
                     value = verif_comp.get_plus_arg_value_as_int("ddr_auto_init");
                     if (value > 0) begin
                        evl_ddr_auto_config_test_seq ddr_auto_config_test_seq;

                        ddr_auto_config_test_seq = new($sformatf("DDRConfig-%0d", verif_comp.get_shire_id()), verif_comp, 1'b1);
                        ddr_auto_config_test_seq.set_abstract_name("DDR Auto-init Configuration");
                        verif_comp.add_sequence_wrapper(ddr_auto_config_test_seq.get_wrapper());
                     end
                  end
                  else begin
                     use_sys_agent = 1'b1;
                  end
               end
            end
            if (use_sys_agent == 1'b1) begin
               verif_comp = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT);
               if (verif_comp != null) begin
                  value = verif_comp.get_plus_arg_value_as_int("ddr_auto_init");
                  if (value > 0) begin
                     evl_ddr_auto_config_test_seq ddr_auto_config_test_seq;

                     ddr_auto_config_test_seq = new($sformatf("DDRConfig"), verif_comp, 1'b0);
                     ddr_auto_config_test_seq.set_abstract_name("DDR Auto-init Configuration");
                     verif_comp.add_sequence_wrapper(ddr_auto_config_test_seq.get_wrapper());
                  end
               end
            end
         end
      `endif
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_ddr_auto_config_test");
      super.new(name_in);
   endfunction

endclass : evl_ddr_auto_config_test

//--------------------------------------------------------------------------------------------------
//
// Class: evl_top_env
//
class evl_top_env extends evl_verif_comp;

   `uvm_component_utils(evl_top_env)

   `ifdef USE_HDL_TEST_COMPS
      ioshire_comps_env m_ioshire_comps_env;
      `ifdef USE_PSHIRE_ENV
         pshire_comps_env  m_pshire_comps_env;
      `endif
   `endif

   `include "dv/common/base/evl_mem_map_pk_inc.svh"

   evl_verif_comp    m_jtag_bus_agent;
   evl_verif_comp    m_jtag_stim_agent;
   evl_sequencer_mem m_mem_sequencer;
   bit               m_ignore_cosim;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop_task
   //
   virtual task fetch_and_process_loop_task(input evl_sequencer_mem mem_sequencer);
      forever begin
         wait_cycles(1);
         while (stimulus_is_enabled() == 1'b1) begin
            fetch_and_process_trans_desc(mem_sequencer);
         end
         wait_for_stimulus_enable();
      end
   endtask : fetch_and_process_loop_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop
   //
   virtual function void fetch_and_process_loop(input evl_sequencer_mem mem_sequencer);
      fork
         fetch_and_process_loop_task(mem_sequencer);
      join_none
   endfunction : fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Task: master_fetch_and_process_loop
   //
   virtual task master_fetch_and_process_loop();
      int sequencer_count = 0;

      wait_for_stimulus_enable();
      fetch_and_process_loop(m_mem_sequencer);
      wait (sequencer_count == 0);
   endtask : master_fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fetch_and_process_trans_desc
   //
   virtual task fetch_and_process_trans_desc(input evl_sequencer_mem mem_sequencer);
      evl_trans_desc   trans_desc;
      evl_sequence_mem sequence_mem;

      mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      wait_cycles(1);
      return;
   endtask : fetch_and_process_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      uvm_factory       factory;
      uvm_component     component;
      evl_verif_comp    verif_comp;
      evl_rtl_instance  instance_list[];
      evl_verif_comp    parent_comp;
      evl_verif_comp    parent_comps[string];
      evl_sys_reg_model reg_model;

      super.build_phase(phase);
      factory = uvm_factory::get();
      evl_rtl_instance_map::get_my_children_list(instance_list, get_full_name());
      foreach (instance_list[ii]) begin
         string inst_name;
         string parent_names[$];

         parent_comp = this;
         inst_name   = evl_base_pkg::extract_parent_list(parent_names, get_full_name(), instance_list[ii].get_full_name());
         foreach (parent_names[jj]) begin
            string full_parent_name;

            full_parent_name = $sformatf("%0s.%0s", parent_comp.get_full_name(), parent_names[jj]);
            if (!parent_comps.exists(full_parent_name)) begin
               parent_comp = evl_verif_comp::type_id::create(parent_names[jj], parent_comp);
               parent_comps[full_parent_name] = parent_comp;
            end
            else begin
               parent_comp = parent_comps[full_parent_name];
            end
         end
         component = factory.create_component_by_name(instance_list[ii].get_sv_comp(), "", inst_name, parent_comp);
         if (component != null) begin
            $cast(verif_comp, component);
            if (verif_comp.get_agent_type() == JTAG_BUS_AGENT) begin
               verif_comp.set_abstract_name($sformatf("JTAG(BUS)"));
               m_jtag_bus_agent = verif_comp;
               add_vc_child(verif_comp);
               component = factory.create_component_by_name("evl_stim_agent_jtag", "", $sformatf("jtag"), this);
               if (component != null) begin
                  verif_comp.set_active_master();
                  $cast(verif_comp, component);
                  verif_comp.set_abstract_name($sformatf("JTAG(STIM)"));
                  verif_comp.set_rtl_port(m_jtag_bus_agent.get_rtl_port());
                  m_jtag_stim_agent = verif_comp;
                  add_vc_child(verif_comp);
               end
            end
            else if (verif_comp.get_agent_type() == LPDDR_RTL_AGENT) begin
               verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
               verif_comp.set_agent_id(instance_list[ii].get_agent_pid() - 232);
               verif_comp.set_abstract_name($sformatf("S%0d:LPDDR", instance_list[ii].get_agent_pid()));
            end
            else if (instance_list[ii].get_agent_pid() != ~evl_agent_id_t'(0)) begin
               verif_comp.set_agent_id(instance_list[ii].get_agent_pid());
               verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), instance_list[ii].get_agent_pid()));
            end
            add_vc_child(verif_comp);
         end
      end

      //
      // Create a sequencer (which will basically be unused).
      //
      m_mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
      set_sequencer_wrapper(m_mem_sequencer.get_wrapper());

      //
      // Create a root register block.
      //
      reg_model = new("SysMap");
      reg_model.set_agent(this);
      set_evl_reg_block(reg_model);

      //
      // Configure the HDL path to this block.
      //
      reg_model.configure(null, get_full_name());

      //
      // Create an AXI address map of the entire register space.
      //
      void'(reg_model.create_map("AXI", evl_paddr_t'(40'h0100000000), 8, UVM_LITTLE_ENDIAN, 0));

      `ifdef USE_HDL_TEST_COMPS
          m_ioshire_comps_env = ioshire_comps_env::type_id::create("m_ioshire_comps_env", this);
          `ifdef USE_PSHIRE_ENV
             m_pshire_comps_env = pshire_comps_env::type_id::create("m_pshire_comps_env", this);
          `endif
      `endif

      //-----------------------------------------------------------------------------------------
      // ET-SOC Memory Map
      //-----------------------------------------------------------------------------------------

      // Main regions
      add_addr_region(pk_systemreg_region,   `EVL_SYSTEMREG_REGION_BASE,   `EVL_SYSTEMREG_REGION_TOP);
      add_addr_region(pk_mramreg_region,     `EVL_MRAMREG_REGION_BASE,     `EVL_MRAMREG_REGION_TOP);
      add_addr_region(pk_io_region,          `EVL_IO_REGION_BASE,          `EVL_IO_REGION_TOP);
      add_addr_region(pk_hyperbus_region,    `EVL_HYPERBUS_REGION_BASE,    `EVL_HYPERBUS_REGION_TOP);
      add_addr_region(pk_bootrom_region,     `EVL_BOOTROM_REGION_BASE,     `EVL_BOOTROM_REGION_TOP);
      add_addr_region(pk_sram_region,        `EVL_SRAM_REGION_BASE,        `EVL_SRAM_REGION_TOP);
      add_addr_region(pk_mram_region,        `EVL_MRAM_REGION_BASE,        `EVL_MRAM_REGION_TOP);
      add_addr_region(pk_otp_region,         `EVL_OTP_REGION_BASE,         `EVL_OTP_REGION_TOP);
      add_addr_region(pk_reserved0_region,   `EVL_RESERVED0_REGION_BASE,   `EVL_RESERVED0_REGION_TOP);
      add_addr_region(pk_esr_region,         `EVL_ESR_REGION_BASE,         `EVL_ESR_REGION_TOP);
      add_addr_region(pk_plic_region,        `EVL_PLIC_REGION_BASE,        `EVL_PLIC_REGION_TOP);
      add_addr_region(pk_scp_region,         `EVL_SCP_REGION_BASE,         `EVL_SCP_REGION_TOP);
      add_addr_region(pk_reserved_region,    `EVL_RESERVED_REGION_BASE,    `EVL_RESERVED_REGION_TOP);

      // DRAM: cached subregions
      //add_addr_region(pk_c_mcode_region,    `EVL_C_MCODE_REGION_BASE,    `EVL_C_MCODE_REGION_TOP);    // when "secure" disabled
      //add_addr_region(pk_c_os_region,       `EVL_C_OS_REGION_BASE,       `EVL_C_OS_REGION_TOP);       // when "secure" disabled
      //add_addr_region(pk_c_mem_region,      `EVL_C_MEMORY_REGION_BASE,   `EVL_C_MEMORY_REGION_TOP);
//
      //// DRAM: cached subregions: new PMA: RTLMIN-5656: when mprot.enable_secure_mem=1:
      //add_addr_region(pk_secure_c_m_code_region,    `EVL_SECURE_C_M_CODE_REGION_BASE,    `EVL_SECURE_C_M_CODE_REGION_TOP);
      //add_addr_region(pk_secure_c_m_data_region,    `EVL_SECURE_C_M_DATA_REGION_BASE,    `EVL_SECURE_C_M_DATA_REGION_TOP);
      //add_addr_region(pk_secure_c_s_code_region,    `EVL_SECURE_C_S_CODE_REGION_BASE,    `EVL_SECURE_C_S_CODE_REGION_TOP);
      //add_addr_region(pk_secure_c_s_data_region,    `EVL_SECURE_C_S_DATA_REGION_BASE,    `EVL_SECURE_C_S_DATA_REGION_TOP);
      //add_addr_region(pk_secure_c_os_region,        `EVL_SECURE_C_OS_REGION_BASE,        `EVL_SECURE_C_OS_REGION_TOP);
//
      //// DRAM: uncached subregions
      //add_addr_region(pk_uc_mcode_region, `EVL_UC_MCODE_REGION_BASE,   `EVL_UC_MCODE_REGION_TOP);
      //add_addr_region(pk_uc_os_region,    `EVL_UC_OS_REGION_BASE,      `EVL_UC_OS_REGION_TOP);
      //add_addr_region(pk_uc_mem_region,   `EVL_UC_MEMORY_REGION_BASE,  `EVL_UC_MEMORY_REGION_TOP);
//
      //// IO subregions
      //add_addr_region(pk_maxion_region,   `EVL_MAXION_REGION_BASE,     `EVL_MAXION_REGION_TOP);
      //add_addr_region(pk_periph_region,   `EVL_PERIPHERAL_REGION_BASE, `EVL_PERIPHERAL_REGION_TOP);
      //add_addr_region(pk_mailbox_region,  `EVL_MAILBOX_REGION_BASE,    `EVL_MAILBOX_REGION_TOP);
//
      //// Some useful SP subregions
      //add_addr_region(pk_sp_rom_region,   `EVL_SP_ROM_REGION_BASE,     `EVL_SP_ROM_REGION_TOP);
      //add_addr_region(pk_sp_ram_region,   `EVL_SP_SRAM_REGION_BASE,    `EVL_SP_SRAM_REGION_TOP);
//
      //// RTLMIN-5656: new PMA: two SP_SRam subregions: lower & upper:
      //add_addr_region(pk_sp_ram_low_region,   `EVL_SP_SRAM_LOW_REGION_BASE,    `EVL_SP_SRAM_LOW_REGION_TOP);
      //add_addr_region(pk_sp_ram_upp_region,   `EVL_SP_SRAM_UPP_REGION_BASE,    `EVL_SP_SRAM_UPP_REGION_TOP);

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      if (m_jtag_stim_agent != null) begin
         m_jtag_stim_agent.connect_my_bus_ports(m_jtag_bus_agent);
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      uvm_reg_map    axi_map;
      evl_line_data  template_line_data;
      evl_line_desc  template_line_desc;
      evl_trans_desc template_trans_desc;

      super.start_of_simulation_phase(phase);

      //
      // Create templates for the basically unused sequencer.
      //
      template_line_data  = evl_line_data::type_id::create(get_abstract_name());
      template_line_desc  = evl_line_desc::type_id::create(get_abstract_name());
      template_trans_desc = evl_trans_desc::type_id::create(get_abstract_name());

      template_trans_desc.set_shire_id(~0);
      template_trans_desc.set_neighborhood_id(~0);
      template_trans_desc.set_core_id(~0);

      template_line_desc.set_rand_gen(m_rand_gen);
      template_trans_desc.set_rand_gen(m_rand_gen);

      template_line_desc.set_master_agent(this);
      template_trans_desc.set_master_agent(this);

      template_line_desc.set_data_template(template_line_data);
      template_trans_desc.set_data_template(template_line_data);

      template_trans_desc.set_line_template(template_line_desc);

      m_mem_sequencer.set_trans_desc_template(template_trans_desc);

      //
      // Configure all of the register blocks in the children VCs as a child of the root register
      // block.
      //
      configure_evl_reg_block();

      //
      // Add register maps in children register blocks to the root register map.
      //
      axi_map = m_evl_reg_block.get_map_by_name("AXI");
      m_evl_reg_block.add_evl_map(axi_map);

      //
      // Lock down the register blocks.
      //
      m_evl_reg_block.lock_evl_reg_block();

      if (`evl_log_check_verbosity(UVM_HIGH, VMOD_REG) != 0) begin
         m_evl_reg_block.print();
      end

      if (m_rtl_port != null) begin
         fork
            m_rtl_port.heartbeat_timer(get_abstract_name());
            master_fetch_and_process_loop();
         join_none
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.reset_phase(phase);
         end
         begin
            if (m_rtl_port != null) begin
               if (`et_test_plusargs("ignore_system_reset") == 0) begin
                  m_rtl_port.wait_for_reset_assertion();
                  m_rtl_port.wait_for_reset_deassertion();
                  wait_cycles(1);
               end
               else begin
                  wait_cycles(100);
               end
            end
         end
      join
      phase.drop_objection(this);
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.main_phase(phase);
         end
         begin
            wait_cycles(10);
            if ((m_global_attrs.cosim_has_started() == 1'b1) && (m_ignore_cosim == 1'b0)) begin
               `evl_log_no_addr(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), "waiting for the co-simulator C code to finish")
               m_global_attrs.wait_for_cosim_end();
               wait_cycles(1);
            end
         end
      join
      phase.drop_objection(this);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.pre_shutdown_phase(phase);
         end
         begin
            if (m_global_attrs.cosim_has_started() == 1'b1) begin
               wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PreShutdownPhase), 1'b1, 1'b1, 1'b1);
            end
         end
      join
      phase.drop_objection(this);
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.shutdown_phase(phase);
         end
         begin
            if (m_global_attrs.cosim_has_started() == 1'b1) begin
               wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
            end
         end
      join
      phase.drop_objection(this);
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_top_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, SYSTEM_AGENT);
      set_abstract_name("SYSTEM");
      set_active_master();
      m_allow_sys_clk = 1'b0;
      m_ignore_cosim  = (`et_test_plusargs("ignoreCosimObjection") == 0) ? 1'b0 : 1'b1;
   endfunction : new

endclass : evl_top_env

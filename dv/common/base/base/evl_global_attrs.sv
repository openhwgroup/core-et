//==================================================================================================
//
// Class: evl_global_attrs
//
class evl_global_attrs extends evl_void;

   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_rand_gen        m_rand_gen;
   static bit                 m_instr_watch               = 1'b0;
   static bit                 m_bus_watch                 = 1'b0;
   static bit                 m_cosim_started             = 1'b0;
   static bit                 m_cosim_cleared             = 1'b0;
   static bit                 m_cosim_ended               = 1'b0;
   static int                 m_cosim_result              = 0;
   static bit                 m_ultrasoc_started          = 1'b0;
   static bit                 m_ultrasoc_ended            = 1'b1;
   static bit                 m_terminate_now             = 1'b0;
   static bit                 m_save_all_descs            = 1'b0;
   static longint             m_cur_time                  = 0;
   static bit                 m_allow_cover               = 1'b0;
   static bit                 m_cover_en                  = 1'b0;
   static bit                 m_disable_configure_timeout = 1'b0;
   static bit                 m_use_sys_clk               = 1'b0;
   static bit                 m_wait_for_sp               = 1'b0;
   static int                 m_minion_mask               = 0;
   static int                 m_thread_mask               = 0;
   static longint             m_shire_mask                = 0;
   static evl_log             m_evl_log;
   static evl_verif_comp      m_system_agent;
   static evl_message_wrapper m_message_wrapper;
   static evl_objection       m_objections[string];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_inst
   //
   static function evl_global_attrs get_inst();
      if (m_global_attrs == null) begin
         evl_global_attrs global_attrs;

         global_attrs = new();
      end
      return m_global_attrs;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: cosim_start
   //
   static function void cosim_start();
      m_cosim_started = ~m_cosim_cleared;
   endfunction : cosim_start


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: cosim_clear
   //
   static function void cosim_clear();
      m_cosim_cleared = 1'b1;
      m_cosim_started = 1'b0;
   endfunction : cosim_clear


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: cosim_end
   //
   static function void cosim_end(input int result, input int minion_id_in, input int thread_id, input bit source);
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      m_global_attrs.cosim_end_local(result, minion_id_in, thread_id, source);
   endfunction : cosim_end


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: minion_thread_done
   //
   static function void minion_thread_done(input int minion_id_in, input int thread_id);
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      m_global_attrs.cosim_end_local(0, minion_id_in, thread_id, 1'b1);
   endfunction : minion_thread_done


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_get_plus_arg_value
   //
   static function string evl_get_plus_arg_value(input string                plus_arg_name_in,
                                                 input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                 input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                 input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      return m_global_attrs.get_plus_arg_value(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : evl_get_plus_arg_value


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_get_plus_arg_value_as_int
   //
   static function longint evl_get_plus_arg_value_as_int(input string                plus_arg_name_in,
                                                         input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                                         input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                                         input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      return m_global_attrs.get_plus_arg_value_as_int(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : evl_get_plus_arg_value_as_int


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_sprint_plus_arg
   //
   static function string evl_sprint_plus_arg(input string                plus_arg_name_in,
                                              input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                              input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                              input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      return m_global_attrs.sprint_plus_arg(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in);
   endfunction : evl_sprint_plus_arg


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_instr_trigger
   //
   static function void evl_instr_trigger(input evl_vaddr_t pc_in,
                                          input evl_instr_t instr_in,
                                          input int         source_in);
      if (m_instr_watch == 1'b0) begin
         return;
      end
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      m_global_attrs.instr_trigger(pc_in, instr_in, source_in);
   endfunction : evl_instr_trigger


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_bus_trigger
   //
   static function void evl_bus_trigger(input evl_paddr_t      addr_in,
                                        input evl_bus_req_id_t req_id_in,
                                        input int              source_in);
      if (m_bus_watch == 1'b0) begin
         return;
      end
      if (m_global_attrs == null) begin
         void'(evl_global_attrs::get_inst());
      end
      m_global_attrs.bus_trigger(addr_in, req_id_in, source_in);
   endfunction : evl_bus_trigger


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_objection
   //
   static function void incr_objection(input string phase_name, input string src_name);
      if (!m_objections.exists(phase_name)) begin
         m_objections[phase_name] = new(phase_name, src_name);
      end
      m_objections[phase_name].incr(src_name);
   endfunction : incr_objection


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_objection
   //
   static function void decr_objection(input string phase_name, input string src_name);
      if (m_objections.exists(phase_name)) begin
         m_objections[phase_name].decr(src_name);
      end
   endfunction : decr_objection


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                  m_config_done         = 1'b0;
   bit                  m_random_data         = 1'b0;
   int                  m_thread_count        = 0;
   int                  m_thread_enable_count = 0;
   int                  m_thread_boot_start   = 0;
   int                  m_thread_boot_done    = 0;
   int                  m_thread_start        = 0;
   int                  m_thread_done         = 0;
   int                  m_thread_fail         = 0;
   int                  m_thread_power_off    = 0;
   bit                  m_all_threads_done    = 1'b0;
   evl_phase_type_t     m_cur_phase           = BuildPhase;
   uvm_phase            m_phases[evl_phase_type_t];
   int                  m_timeouts[evl_phase_type_t];
   evl_plus_arg         m_plus_args[$];
   int                  m_l3_bank_shift[`SC_BANK_ID_SIZE];
   int                  m_l3_subbank_shift[`SC_SUB_BANK_ID_SIZE];
   int                  m_l3_shire_shift;
   int                  m_l3_set_shift;
   int                  m_axi_port_lists[int][int];
   int                  m_ioshire_axi_port_list[int];
   int                  m_pshire_axi_port_list[int];
   bit                  m_neigh_power_off[evl_full_cpu_id_t];
   bit                  m_cpu_enable[evl_full_cpu_id_t];
   bit                  m_cpu_boot_done[evl_full_cpu_id_t];
   bit                  m_cpu_done[evl_full_cpu_id_t];
   evl_sc_config_attr_s m_sc_config[`NUM_SHIRES];
   string               m_stdout_buf[32096];
   static bit           m_putchar_error[32096];
   uvm_report_server    m_uvm_report_server;
   int                  m_current_shire_ids[33:0];
   evl_instr_watch      m_instr_watches[$];
   evl_bus_watch        m_bus_watches[$];
   evl_trans_desc       m_all_trans_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_minion_count
   //
   function int get_minion_count();
      int minion_count;

      minion_count = 0;
      for (int shire = 0; shire < 34; shire++) begin
         for (int neigh = 0; neigh < 8; neigh++) begin
            for (int minion = 0; minion < 8; minion++) begin
               if (evl_verif_comp_map::get_verif_comp(CORE_RTL_AGENT, ~evl_agent_id_t'(0), evl_shire_id_t'(shire), evl_neighborhood_id_t'(neigh), evl_core_id_t'(minion)) != null) begin
                  minion_count++;
               end
            end
         end
      end
      return minion_count;
   endfunction : get_minion_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_minion_thread_count
   //
   function int get_minion_thread_count();
      int thread_count;
      int base_thread_count;

      thread_count      = 0;
      base_thread_count = $countones(m_thread_mask);
      for (int shire = 0; shire < 34; shire++) begin
         for (int neigh = 0; neigh < 8; neigh++) begin
            for (int minion = 0; minion < 8; minion++) begin
               if (evl_verif_comp_map::get_verif_comp(CORE_RTL_AGENT, ~evl_agent_id_t'(0), evl_shire_id_t'(shire), evl_neighborhood_id_t'(neigh), evl_core_id_t'(minion)) != null) begin
                  thread_count = thread_count + base_thread_count;
               end
            end
         end
      end
      return thread_count;
   endfunction : get_minion_thread_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_sc_config
   //
   function void set_sc_config(input int shire_number, input evl_sc_config_attr_s sc_config_in);
      if ((shire_number >= 0) && (shire_number < `NUM_SHIRES)) begin
         m_sc_config[shire_number] = sc_config_in;
      end
   endfunction : set_sc_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sc_config
   //
   function evl_sc_config_attr_s get_sc_config(input int shire_number);
      if ((shire_number >= 0) && (shire_number < `NUM_SHIRES)) begin
         return m_sc_config[shire_number];
      end
      return evl_sc_config_attr_s'(0);
   endfunction : get_sc_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l2_config
   //
   function void set_l2_config(input int         shire_number,
                               input bit         bypass,
                               input int         mem_size,
                               input evl_paddr_t base_addr,
                               input evl_paddr_t top_addr,
                               input evl_paddr_t index_mask,
                               input evl_paddr_t alias_mask);
      if ((shire_number >= 0) && (shire_number < `NUM_SHIRES)) begin
         m_sc_config[shire_number].l2_base_addr  = base_addr;
         m_sc_config[shire_number].l2_top_addr   = top_addr;
         m_sc_config[shire_number].l2_index_mask = index_mask;
         m_sc_config[shire_number].l2_alias_mask = alias_mask;
         m_sc_config[shire_number].l2_size       = mem_size;
         m_sc_config[shire_number].l2_bypass     = bypass;
      end
   endfunction : set_l2_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_config
   //
   function void set_l3_config(input int         shire_number,
                               input bit         bypass,
                               input bit         remote_enable,
                               input int         mem_size,
                               input evl_paddr_t base_addr,
                               input evl_paddr_t top_addr,
                               input evl_paddr_t index_mask,
                               input evl_paddr_t alias_mask);
      if ((shire_number >= 0) && (shire_number < `NUM_SHIRES)) begin
         m_sc_config[shire_number].l3_base_addr     = base_addr;
         m_sc_config[shire_number].l3_top_addr      = top_addr;
         m_sc_config[shire_number].l3_index_mask    = index_mask;
         m_sc_config[shire_number].l3_alias_mask    = alias_mask;
         m_sc_config[shire_number].l3_size          = mem_size;
         m_sc_config[shire_number].l3_remote_enable = remote_enable;
         m_sc_config[shire_number].l3_bypass        = bypass;
      end
   endfunction : set_l3_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_scp_index_mask
   //
   function void set_scp_config(input int         shire_number,
                                input bit         remote_enable,
                                input int         mem_size,
                                input evl_paddr_t base_addr,
                                input evl_paddr_t top_addr,
                                input evl_paddr_t index_mask,
                                input evl_paddr_t alias_mask);
      if ((shire_number >= 0) && (shire_number < `NUM_SHIRES)) begin
         m_sc_config[shire_number].scp_base_addr     = base_addr;
         m_sc_config[shire_number].scp_top_addr      = top_addr;
         m_sc_config[shire_number].scp_index_mask    = index_mask;
         m_sc_config[shire_number].scp_alias_mask    = alias_mask;
         m_sc_config[shire_number].scp_size          = mem_size;
         m_sc_config[shire_number].scp_remote_enable = remote_enable;
      end
   endfunction : set_scp_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_bank_shift
   //
   function void set_l3_bank_shift(input int bank_bit, input int bank_bit_position);
      int bit_shift;

      if ((bank_bit < 0) || (bank_bit >= `SC_BANK_ID_SIZE)) begin
         return;
      end
      bit_shift = `EVL_LINE_ADDR_LSB + bank_bit_position - bank_bit;
      if ((bit_shift < `EVL_LINE_ADDR_LSB) || ((bit_shift + bank_bit) > `EVL_PADDR_MSB)) begin
         return;
      end
      m_l3_bank_shift[bank_bit] = bit_shift;
   endfunction : set_l3_bank_shift


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_subbank_shift
   //
   function void set_l3_subbank_shift(input int subbank_bit, input int subbank_bit_position);
      int bit_shift;

      if ((subbank_bit < 0) || (subbank_bit >= `SC_SUB_BANK_ID_SIZE)) begin
         return;
      end
      bit_shift = `EVL_LINE_ADDR_LSB + subbank_bit_position - subbank_bit;
      if ((bit_shift < `EVL_LINE_ADDR_LSB) || ((bit_shift + subbank_bit) > `EVL_PADDR_MSB)) begin
         return;
      end
      m_l3_subbank_shift[subbank_bit] = bit_shift;
   endfunction : set_l3_subbank_shift


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_shire_shift
   //
   function void set_l3_shire_shift(input int shire_bit_position);
      int bit_shift;

      bit_shift = `EVL_LINE_ADDR_LSB + shire_bit_position;
      if ((bit_shift < `EVL_LINE_ADDR_LSB) || (bit_shift > `EVL_PADDR_MSB)) begin
         return;
      end
      m_l3_shire_shift = bit_shift;
   endfunction : set_l3_shire_shift


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_l3_bank_from_address
   //
   function int get_l3_bank_from_address(input evl_paddr_t addr);
      int bit_mask    = 1;
      int bank_number = 0;

      for (int ii = 0; ii < `SC_BANK_ID_SIZE; ii++) begin
         bank_number |= (int'(addr >> m_l3_bank_shift[ii]) & bit_mask);
         bit_mask <<= 1;
      end
      return bank_number;
   endfunction : get_l3_bank_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_l3_subbank_from_address
   //
   function int get_l3_subbank_from_address(input evl_paddr_t addr);
      int bit_mask       = 1;
      int subbank_number = 0;

      for (int ii = 0; ii < `SC_SUB_BANK_ID_SIZE; ii++) begin
         subbank_number |= (int'(addr >> m_l3_subbank_shift[ii]) & bit_mask);
         bit_mask <<= 1;
      end
      return subbank_number;
   endfunction : get_l3_subbank_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_l3_shire_from_address
   //
   function int get_l3_shire_from_address(input evl_paddr_t addr);
      if (int'(addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
         return int'(addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE]);
      end
      return (int'(addr >> m_l3_shire_shift) & ((1 << `SC_L3_SHIRE_ID_SIZE) - 1));
   endfunction : get_l3_shire_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bank_from_address
   //
   function int get_bank_from_address(input evl_paddr_t addr);
      return int'((addr >> `EVL_LINE_ADDR_LSB) & {$clog2(`SC_BANKS){1'b1}});
   endfunction : get_bank_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bank_from_message
   //
   function int get_bank_from_message(input evl_shire_id_t shire_id, input evl_paddr_t addr);
      evl_shire_id_t msg_shire_id;

      msg_shire_id = evl_shire_id_t'(addr >> `EVL_MSG_SHIRE_ID_LSB);
      if ((msg_shire_id != shire_id) && (msg_shire_id != evl_shire_id_t'(`EVL_MSG_LOCAL_SHIRE_ID))) begin
         return int'(`SC_BANKS);
      end
      return int'((addr >> `EVL_MSG_NEIGH_ID_LSB) & (`SC_BANKS - 1));
   endfunction : get_bank_from_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id_from_address
   //
   function int get_shire_id_from_address(input evl_paddr_t addr, input int shire_bit_position);
      return (int'(addr >> shire_bit_position) & ((1 << `EVL_SHIRE_ID_WIDTH) - 1));
   endfunction : get_shire_id_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id_from_message
   //
   function evl_shire_id_t get_shire_id_from_message(input evl_paddr_t addr);
      return evl_shire_id_t'(addr >> `EVL_MSG_SHIRE_ID_LSB);
   endfunction : get_shire_id_from_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: instr_trigger
   //
   function void instr_trigger(input evl_vaddr_t pc_in,
                               input evl_instr_t instr_in,
                               input int         source_in);
      if (m_instr_watch == 1'b0) begin
         return;
      end
      foreach (m_instr_watches[ii]) begin
         m_instr_watches[ii].check_for_match(pc_in, instr_in, source_in);
      end
   endfunction : instr_trigger


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_trigger
   //
   function void bus_trigger(input evl_paddr_t      addr_in,
                             input evl_bus_req_id_t req_id_in,
                             input int              source_in);
      if (m_bus_watch == 1'b0) begin
         return;
      end
      foreach (m_bus_watches[ii]) begin
         m_bus_watches[ii].check_for_match(addr_in, req_id_in, source_in);
      end
   endfunction : bus_trigger


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_instr_watch_params
   //
   function evl_instr_watch set_instr_watch_params(input bit         use_pc,
                                                   input evl_vaddr_t pc_in,
                                                   input bit         use_instr,
                                                   input evl_instr_t instr_in,
                                                   input bit         use_source,
                                                   input int         source_in);
      evl_instr_watch instr_watch;

      foreach (m_instr_watches[ii]) begin
         if (m_instr_watches[ii].have_watch_params(use_pc, pc_in, use_instr, instr_in, use_source, source_in) == 1'b1) begin
            return m_instr_watches[ii];
         end
      end
      instr_watch = new();
      instr_watch.set_watch_params(use_pc, pc_in, use_instr, instr_in, use_source, source_in);
      m_instr_watches.push_back(instr_watch);
      m_instr_watch = 1'b1;
      return instr_watch;
   endfunction : set_instr_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_watch_params
   //
   function evl_bus_watch set_bus_watch_params(input evl_paddr_t      addr_in,
                                               input evl_bus_req_id_t req_id_in,
                                               input evl_bus_req_id_t req_id_mask_in,
                                               input evl_verif_comp   agent_in);
      evl_bus_watch bus_watch;

      if ((agent_in == null) || (!(agent_in.get_agent_type() inside { `EVL_BUS_AGENT_TYPES }))) begin
         return null;
      end
      foreach (m_bus_watches[ii]) begin
         if (m_bus_watches[ii].have_watch_params(addr_in, req_id_in, req_id_mask_in, agent_in) == 1'b1) begin
            return m_bus_watches[ii];
         end
      end
      bus_watch = new();
      bus_watch.set_watch_params(addr_in, req_id_in, req_id_mask_in, agent_in);
      m_bus_watches.push_back(bus_watch);
      m_bus_watch = 1'b1;
      return bus_watch;
   endfunction : set_bus_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_shire_axi_port_index_list
   //
   // This function is used to create an m_axi_port_list, which is a port-type to TLM index mapping
   // for AXI ports on Minion shires.  There are a number of AXI port types attached to a Minion
   // shire (such as a shire-cache-to-L3-mesh port and a shire-cache-to-sys-mesh port), and to make
   // matters worse a number of them can have multiple "layers".  The m_axi_port_list array contains
   // a port-type-to-base-TLM-index mapping for each port type.
   //
   function void create_shire_axi_port_index_list(input int num_l3_ports, input int shire_num);
      if (m_axi_port_lists[shire_num].size() == 0) begin
         evl_shire_axi_ifc_type_t port_type;
         int                      cur_index;

         cur_index = 0;
         port_type = port_type.first();
         while (1) begin
            m_axi_port_lists[shire_num][int'(port_type)] = cur_index;
            if (port_type inside { SHIRE_L2TOL3_MESH_AXI, SHIRE_MESH_TOL3_AXI }) begin
               cur_index = cur_index + num_l3_ports;
            end
            else begin
               cur_index++;
            end
            if (port_type == port_type.last()) begin
               break;
            end
            port_type = port_type.next();
         end
      end
   endfunction : create_shire_axi_port_index_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_axi_port_index
   //
   function int get_shire_axi_port_index(input int port_type, input int port_num, input int num_l3_ports, input int shire_num);
      if (m_axi_port_lists[shire_num].size() == 0) begin
         create_shire_axi_port_index_list(num_l3_ports, shire_num);
      end
      if (!m_axi_port_lists[shire_num].exists(port_type)) begin
         return -1;
      end
      if (port_type inside { int'(SHIRE_L2TOL3_MESH_AXI), int'(SHIRE_MESH_TOL3_AXI) }) begin
         return m_axi_port_lists[shire_num][port_type] + port_num;
      end
      if (port_num > 0) begin
         return -1;
      end
      return m_axi_port_lists[shire_num][port_type];
   endfunction : get_shire_axi_port_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_ioshire_axi_port_index_list
   //
   function void create_ioshire_axi_port_index_list(input int num_l3_ports,
                                                    input int num_maxion_l3_master_ports,
                                                    input int num_maxion_l3_slave_ports);
      if (m_ioshire_axi_port_list.size() == 0) begin
         evl_ioshire_axi_ifc_type_t port_type;
         int                        cur_index;

         cur_index = 0;
         port_type = port_type.first();
         while (1) begin
            m_ioshire_axi_port_list[int'(port_type)] = cur_index;
            if (port_type inside { IO_TOL3_AXI }) begin
               cur_index = cur_index + num_l3_ports;
            end
            else if (port_type inside { MAXION_TOL3_AXI }) begin
               cur_index = cur_index + num_maxion_l3_master_ports;
            end
            else if (port_type inside { MAXION_L3_SLV_AXI }) begin
               cur_index = cur_index + num_maxion_l3_slave_ports;
            end
            else begin
               cur_index++;
            end
            if (port_type == port_type.last()) begin
               break;
            end
            port_type = port_type.next();
         end
      end
   endfunction : create_ioshire_axi_port_index_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ioshire_axi_port_index
   //
   function int get_ioshire_axi_port_index(input int port_type,
                                           input int port_num,
                                           input int num_l3_ports,
                                           input int num_maxion_l3_master_ports,
                                           input int num_maxion_l3_slave_ports);
      if (m_ioshire_axi_port_list.size() == 0) begin
         create_ioshire_axi_port_index_list(num_l3_ports, num_maxion_l3_master_ports, num_maxion_l3_slave_ports);
      end
      if (!m_ioshire_axi_port_list.exists(port_type)) begin
         return -1;
      end
      if (port_type inside { int'(IO_TOL3_AXI), int'(MAXION_TOL3_AXI), int'(MAXION_L3_SLV_AXI) }) begin
         return m_ioshire_axi_port_list[port_type] + port_num;
      end
      if (port_num > 0) begin
         return -1;
      end
      return m_ioshire_axi_port_list[port_type];
   endfunction : get_ioshire_axi_port_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_pshire_axi_port_index_list
   //
   function void create_pshire_axi_port_index_list(input int num_l3_ports);
      if (m_pshire_axi_port_list.size() == 0) begin
         evl_pshire_axi_ifc_type_t port_type;
         int                       cur_index;

         cur_index = 0;
         port_type = port_type.first();
         while (1) begin
            m_pshire_axi_port_list[int'(port_type)] = cur_index;
            if (port_type inside { PSHIRE_TOL3_AXI }) begin
               cur_index = cur_index + num_l3_ports;
            end
            else begin
               cur_index++;
            end
            if (port_type == port_type.last()) begin
               break;
            end
            port_type = port_type.next();
         end
      end
   endfunction : create_pshire_axi_port_index_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pshire_axi_port_index
   //
   function int get_pshire_axi_port_index(input int port_type, input int port_num, input int num_l3_ports);
      if (m_pshire_axi_port_list.size() == 0) begin
         create_pshire_axi_port_index_list(num_l3_ports);
      end
      if (!m_pshire_axi_port_list.exists(port_type)) begin
         return -1;
      end
      if (port_type inside { int'(PSHIRE_TOL3_AXI) }) begin
         return m_pshire_axi_port_list[port_type] + port_num;
      end
      if (port_num > 0) begin
         return -1;
      end
      return m_pshire_axi_port_list[port_type];
   endfunction : get_pshire_axi_port_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_axi_port_index_from_address
   //
   function int get_axi_port_index_from_address(input evl_paddr_t addr);
      if (int'(addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
         return int'(addr[`SC_BANK_MSB:`SC_BANK_LSB]);
      end
      return get_l3_bank_from_address(addr);
   endfunction : get_axi_port_index_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_memshire_virt_id_from_address
   //
   function int get_memshire_virt_id_from_address(input evl_paddr_t addr, input int shire_bit_position = 6);
      evl_shire_id_t decode_addr;

      decode_addr = evl_shire_id_t'(get_shire_id_from_address(addr, shire_bit_position));
      case (decode_addr[2:0])
         'd0     : return 'd232;
         'd1     : return 'd233;
         'd2     : return 'd234;
         'd3     : return 'd235;
         'd4     : return 'd236;
         'd5     : return 'd237;
         'd6     : return 'd238;
         'd7     : return 'd239;
         default : return evl_shire_id_t'(0);
      endcase
   endfunction : get_memshire_virt_id_from_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_to_l3_address
   //
   function evl_paddr_t convert_to_l3_address(input int bank_in, input int subbank_in, input int shire_in, input int set_in = 0);
      int         bit_mask;
      evl_paddr_t addr;

      addr = evl_paddr_t'(shire_in & ((1 << `SC_L3_SHIRE_ID_SIZE) - 1)) << m_l3_shire_shift;

      bit_mask = 1;
      for (int ii = 0; ii < `SC_BANK_ID_SIZE; ii++) begin
         addr |= evl_paddr_t'(bank_in & bit_mask) << m_l3_bank_shift[ii];
         bit_mask <<= 1;
      end

      bit_mask = 1;
      for (int ii = 0; ii < `SC_SUB_BANK_ID_SIZE; ii++) begin
         addr |= evl_paddr_t'(subbank_in & bit_mask) << m_l3_subbank_shift[ii];
         bit_mask <<= 1;
      end
      addr |= evl_paddr_t'(set_in) << m_l3_set_shift;
      return addr;
   endfunction : convert_to_l3_address


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_in_my_l3
   //
   function bit address_is_in_my_l3(input evl_shire_id_t shire_in, input evl_paddr_t addr_in);
      int shire_mask;

      if (shire_in >= `NUM_SHIRES) begin
         return 1'b0;
      end
      if (int'(addr_in[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
         if (m_sc_config[shire_in].scp_remote_enable == 1'b1) begin
            if (evl_shire_id_t'(addr_in[`SC_SCP_SHIRE_SEL_ADDR_RANGE]) == shire_in) begin
               return 1'b1;
            end
         end
         return 1'b0;
      end
      if ((m_sc_config[shire_in].l3_size == 0) || (m_sc_config[shire_in].l3_remote_enable == 1'b0)) begin
         return 1'b0;
      end
      shire_mask = (1 << `SC_L3_SHIRE_ID_SIZE) - 1;
      if (((get_l3_shire_from_address(addr_in) ^ shire_in) & shire_mask) == 0) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : address_is_in_my_l3


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_in_my_l3_slave
   //
   function bit address_is_in_my_l3_slave(input evl_shire_id_t shire_in, input evl_paddr_t addr_in);
      int shire_mask;

      if (shire_in >= `NUM_SHIRES) begin
         return 1'b0;
      end
      if (int'(addr_in[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
         if (m_sc_config[shire_in].scp_remote_enable == 1'b1) begin
            if (evl_shire_id_t'(addr_in[`SC_SCP_SHIRE_SEL_ADDR_RANGE]) == shire_in) begin
               return 1'b1;
            end
         end
         else begin
            if (evl_shire_id_t'(addr_in[`SC_SCP_SHIRE_SEL_ADDR_RANGE]) == shire_in) begin
               `dut_warning(get_abstract_name(), $sformatf("S%0d: address 0x%0x is in Scratchpad but Scratchpad is disabled", shire_in, addr_in))
               return 1'b1;
            end
         end
         return 1'b0;
      end
      if ((m_sc_config[shire_in].l3_size == 0) || (m_sc_config[shire_in].l3_remote_enable == 1'b0)) begin
         if (m_sc_config[shire_in].l3_size == 0) begin
            `dut_warning(get_abstract_name(), $sformatf("S%0d: address 0x%0x is in L3, but L3 size is 0", shire_in, addr_in))
         end
         else begin
            `dut_warning(get_abstract_name(), $sformatf("S%0d: address 0x%0x is in L3, but L3 remote is disabled", shire_in, addr_in))
         end
      end
      shire_mask = (1 << `SC_L3_SHIRE_ID_SIZE) - 1;
      if (((get_l3_shire_from_address(addr_in) ^ shire_in) & shire_mask) == 0) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : address_is_in_my_l3_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_in_my_shire
   //
   function bit address_is_in_my_shire(input evl_shire_id_t shire_in, input evl_paddr_t addr_in, input int shire_bit_position);
      int shire_mask;

      shire_mask = (1 << `EVL_SHIRE_ID_WIDTH) - 1;
      if (((get_shire_id_from_address(addr_in, shire_bit_position) ^ shire_in) & shire_mask) == 0) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : address_is_in_my_shire


   //-----------------------------------------------------------------------------------------------
   //
   // Function: address_is_in_my_memshire
   //
   function bit address_is_in_my_memshire(input evl_shire_id_t shire_in, input evl_paddr_t addr_in, input int shire_bit_position = 6);
      int shire_mask;

      shire_mask = (1 << `EVL_SHIRE_ID_WIDTH) - 1;
      if (((get_memshire_virt_id_from_address(addr_in, shire_bit_position) ^ shire_in) & shire_mask) == 0) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : address_is_in_my_memshire


   //-------------------------------------------------------------------------
   //
   // Function: change_current_shire_id
   //
   virtual function void change_current_shire_id(input evl_shire_id_t reset_shire_id, input evl_shire_id_t new_shire_id);
      m_current_shire_ids[reset_shire_id] = new_shire_id;
   endfunction : change_current_shire_id


   //-------------------------------------------------------------------------
   //
   // Function: get_current_shire_id
   //
   virtual function evl_shire_id_t get_current_shire_id(input evl_shire_id_t reset_shire_id);
      if (reset_shire_id <= 33) begin
         return m_current_shire_ids[reset_shire_id];
      end
      else begin
         return reset_shire_id;
      end
   endfunction : get_current_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_config_done
   //
   function void set_config_done();
      m_config_done = 1'b1;
   endfunction : set_config_done


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_config_done
   //
   task wait_for_config_done();
      #0 wait (m_config_done == 1'b1);
   endtask : wait_for_config_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cosim_has_started
   //
   function bit cosim_has_started();
      return m_cosim_started;
   endfunction : cosim_has_started


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_cosim_end
   //
   task wait_for_cosim_end();
      if (m_cosim_started == 1'b0) begin
         return;
      end
      #0 wait ((m_cosim_ended == 1'b1) || (m_terminate_now == 1'b1));
      return;
   endtask : wait_for_cosim_end


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_minion_end
   //
   // This function will only be called when the cosimulator determines all threads are done.
   //
   function void process_minion_end(input int result, input int minion_id_in, input int thread_id);
      m_cosim_result = result;
      if ((minion_id_in == -1) && (thread_id == -1)) begin
         return;
      end
      if (m_cosim_result != 0) begin
         int shire_id  = (minion_id_in >> 5) & 'hff;
         int neigh_id  = (minion_id_in & 'h1f) / 8;
         int minion_id = (minion_id_in & 'h07);

         if (result < 0) begin
            if (minion_id_in < 0) begin
               `dut_error("SYSTEM", $sformatf("BEMU reported an error (one or more threads timed out)"))
            end
            else begin
               if (shire_id >= `NUM_MINION_SHIRES) begin
                  `dut_error("SYSTEM", $sformatf("S%0d:C%0d thread %0d timed out", shire_id, minion_id, thread_id))
               end
               else begin
                  `dut_error("SYSTEM", $sformatf("S%0d:N%0d:C%0d thread %0d timed out", shire_id, neigh_id, minion_id, thread_id))
               end
            end
         end
         else begin
            if (minion_id_in < 0) begin
               `dut_error("SYSTEM", $sformatf("BEMU reported an error (one or more threads reported an error)"))
            end
            else begin
               if (shire_id >= `NUM_MINION_SHIRES) begin
                  `dut_error("SYSTEM", $sformatf("S%0d:C%0d thread %0d reported an error", shire_id, minion_id, thread_id))
               end
               else begin
                  `ifndef SPIO_GATESIM
                     `dut_error("SYSTEM", $sformatf("S%0d:N%0d:C%0d thread %0d reported an error", shire_id, neigh_id, minion_id, thread_id))
                  `endif
               end
            end
         end
      end
      else begin
         if (m_disable_configure_timeout == 1'b0) begin
            foreach (m_cpu_boot_done[cpu_id]) begin
               evl_shire_id_t        shire_id;
               evl_neighborhood_id_t neigh_id;
               evl_core_id_t         core_id;
               bit                   thread_id;

               { shire_id, neigh_id, core_id, thread_id } = cpu_id;
               if (m_cpu_boot_done[cpu_id] == 1'b0) begin
                  if (shire_id >= `NUM_MINION_SHIRES) begin
                     `dut_error(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, core_id, thread_id))
                  end
                  else begin
                     `dut_error(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
                  end
                  incr_thread_boot_done(shire_id, neigh_id, core_id, thread_id);
               end
            end
         end
         foreach (m_cpu_enable[cpu_id]) begin
            evl_shire_id_t        shire_id;
            evl_neighborhood_id_t neigh_id;
            evl_core_id_t         core_id;
            bit                   thread_id;

            { shire_id, neigh_id, core_id, thread_id } = cpu_id;
            if ((m_cpu_enable[cpu_id] == 1'b1) && (!m_cpu_boot_done.exists(cpu_id) || (m_cpu_boot_done[cpu_id] == 1'b0))) begin
               if (m_cpu_boot_done.exists(cpu_id)) begin
                  if (shire_id >= `NUM_MINION_SHIRES) begin
                     `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, core_id, thread_id))
                  end
                  else begin
                     `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
                  end
               end
               else begin
                  if (shire_id >= `NUM_MINION_SHIRES) begin
                     `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d completed the test before the boot code started ... there is a code problem of some sort", shire_id, core_id, thread_id))
                  end
                  else begin
                     `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d completed the test before the boot code started ... there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
                  end
               end
               incr_thread_boot_done(shire_id, neigh_id, core_id, thread_id);
            end
         end
         m_thread_boot_done = m_thread_count;
      end
   endfunction : process_minion_end


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cosim_end_local
   //
   // This function will only be called when the cosimulator determines all threads are done.
   //
   function void cosim_end_local(input int result, input int minion_id_in, input int thread_id, input bit source);
      if ((m_all_threads_done == 1'b0) && (m_cosim_ended == 1'b0)) begin
         process_minion_end(result, minion_id_in, thread_id);
      end
      if ((source == 1'b1) || (result != 0) || (m_thread_done >= m_thread_count) || (m_wait_for_sp == 1'b0)) begin
         m_all_threads_done = 1'b1;
      end
      m_cosim_ended = 1'b1;
   endfunction : cosim_end_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_ultrasoc
   //
   function void start_ultrasoc();
      m_ultrasoc_started = 1'b1;
      m_ultrasoc_ended   = 1'b0;
      `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("UltraSoc Python script has either started or is expected to start"))
   endfunction : start_ultrasoc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: stop_ultrasoc
   //
   function void stop_ultrasoc();
      m_ultrasoc_ended = 1'b1;
   endfunction : stop_ultrasoc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_ultrasoc
   //
   task wait_for_ultrasoc();
      if (m_ultrasoc_started == 1'b1) begin
         `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("Waiting for UltraSoc Python script to complete"))
         #0 wait (m_ultrasoc_ended == 1'b1);
         `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("UltraSoc Python script has completed"))
      end
   endtask : wait_for_ultrasoc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_terminate
   //
   task wait_for_terminate();
      wait (m_terminate_now == 1'b1);
   endtask : wait_for_terminate


   //-----------------------------------------------------------------------------------------------
   //
   // Function: terminate_now
   //
   function void terminate_now();
      m_terminate_now = 1'b1;
      stop_ultrasoc();
   endfunction : terminate_now


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cur_phase
   //
   function void set_cur_phase(input evl_phase_type_t phase_type_in, input uvm_phase phase_in, input longint prev_time = -1);
      longint new_time;
      longint time_diff;

      if (prev_time > 0) begin
         m_cur_time = prev_time;
      end
      else if (m_cur_time <= 0) begin
         m_cur_time = `evl_tcl_get_time_of_day;
      end
      new_time   = `evl_tcl_get_time_of_day;
      time_diff  = (new_time <= m_cur_time) ? 0 : new_time - m_cur_time;
      m_cur_time = new_time;
      m_phases[phase_type_in] = phase_in;
      m_cur_phase = phase_type_in;
      if (m_cur_phase == MainPhase) begin
         m_cover_en = m_allow_cover;
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Functional coverage is %0s", m_cover_en ? "allowed" : "not allowed"))
      end
      `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Current phase is %p (%0d second%0s, %0s)", m_cur_phase, time_diff, ((time_diff == 1) ? "" : "s"), `evl_tcl_get_hhmmss))
   endfunction : set_cur_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cur_phase
   //
   function evl_phase_type_t get_cur_phase();
      return m_cur_phase;
   endfunction : get_cur_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_start_of_phase
   //
   task wait_for_start_of_phase(input evl_phase_type_t phase_in);
      #0 wait (int'(m_cur_phase) >= int'(phase_in));
   endtask : wait_for_start_of_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_end_of_phase
   //
   task wait_for_end_of_phase(input evl_phase_type_t phase_in);
      #0 wait (int'(m_cur_phase) > int'(phase_in));
   endtask : wait_for_end_of_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_next_phase_change
   //
   task get_next_phase_change(output uvm_phase phase_out, output evl_phase_type_t cur_phase_out, input evl_phase_type_t phase_in);
      wait_for_end_of_phase(phase_in);
      #0 cur_phase_out = m_cur_phase;
      if (m_phases.exists(m_cur_phase) && (m_phases[m_cur_phase] != null)) begin
         phase_out = m_phases[m_cur_phase];
      end
      else begin
         phase_out = null;
      end
   endtask : get_next_phase_change


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_agent_key
   //
   function evl_agent_key_t gen_agent_key(input evl_shire_id_t        shire_id,
                                          input evl_neighborhood_id_t neighborhood_id,
                                          input evl_core_id_t         core_id,
                                          input evl_agent_id_t        agent_id,
                                          input evl_agent_type_t      agent_type,
                                          input evl_sequencer_id_t    sequencer_id = evl_sequencer_id_t'(0));
      if (shire_id == ~evl_shire_id_t'(0)) begin
         return { ~evl_shire_id_t'(0), ~evl_neighborhood_id_t'(0), ~evl_core_id_t'(0), agent_id, agent_type, sequencer_id };
      end
      else if (neighborhood_id == ~evl_neighborhood_id_t'(0)) begin
         return { shire_id, ~evl_neighborhood_id_t'(0), ~evl_core_id_t'(0), agent_id, agent_type, sequencer_id };
      end
      return { shire_id, neighborhood_id, core_id, agent_id, agent_type, sequencer_id };
   endfunction : gen_agent_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_dump_plusarg
   //
   //   This funcion should be used to check the dump plusarg at the beginning of the sim
   //
   task check_dump_plusarg();
      m_evl_log.check_dump_plusarg();
   endtask : check_dump_plusarg


   //-----------------------------------------------------------------------------------------------
   //
   // Function: coverage_allowed
   //
   function bit coverage_allowed();
      return m_allow_cover;
   endfunction : coverage_allowed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: coverage_enabled
   //
   function bit coverage_enabled();
      return m_cover_en;
   endfunction : coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_coverage_enabled
   //
   task wait_for_coverage_enabled();
      wait (m_cover_en == 1'b1);
   endtask : wait_for_coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: enable_dump
   //
   //   This funcion can be used to turn on dumping regardless of the +dump plusarg
   //
   task enable_dump();
      m_evl_log.enable_dump();
   endtask : enable_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: disable_dump
   //
   task disable_dump();
      m_evl_log.disable_dump();
   endtask : disable_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: close_dump
   //
   task close_dump();
      m_evl_log.close_dump();
   endtask : close_dump


   //-----------------------------------------------------------------------------------------------
   //
   // Task: check_assert_plusarg
   //
   task check_assert_plusarg();
      m_evl_log.check_assert_plusarg();
   endtask : check_assert_plusarg


   //-----------------------------------------------------------------------------------------------
   //
   // Task: enable_assertions
   //
   task enable_assertions();
      m_evl_log.enable_assertions();
   endtask : enable_assertions


   //-----------------------------------------------------------------------------------------------
   //
   // Task: disable_assertions
   //
   task disable_assertions();
      m_evl_log.disable_assertions();
   endtask : disable_assertions


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_errors
   //
   function void incr_errors();
      m_evl_log.incr_errors();
   endfunction : incr_errors


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_error_count
   //
   function int get_error_count();
      return m_evl_log.get_error_count();
   endfunction : get_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_uvm_error_count
   //
   function int get_uvm_error_count();
      return m_uvm_report_server.get_severity_count(UVM_ERROR);
   endfunction : get_uvm_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_total_error_count
   //
   function int get_total_error_count();
      return (get_error_count() + get_uvm_error_count());
   endfunction : get_total_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_error_finish
   //
   task wait_for_error_finish();
      fork
         m_evl_log.wait_for_error_finish();
         #0 wait (m_terminate_now == 1'b1);
      join_any
   endtask : wait_for_error_finish


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_max_error_count
   //
   function void set_max_error_count(input int max_error_count_in);
      m_evl_log.set_max_error_count(max_error_count_in);
   endfunction : set_max_error_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_warnings
   //
   function void incr_warnings();
      m_evl_log.incr_warnings();
   endfunction : incr_warnings


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_warning_count
   //
   function int get_warning_count();
      return m_evl_log.get_warning_count();
   endfunction : get_warning_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_uvm_warning_count
   //
   function int get_uvm_warning_count();
      return m_uvm_report_server.get_severity_count(UVM_WARNING);
   endfunction : get_uvm_warning_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_total_warning_count
   //
   function int get_total_warning_count();
      return (get_warning_count() + get_uvm_warning_count());
   endfunction : get_total_warning_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_random_data
   //
   function bit use_random_data();
      return m_random_data;
   endfunction : use_random_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_random_data_use
   //
   function void set_random_data_use();
      m_random_data = 1'b1;
   endfunction : set_random_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_zero_data_use
   //
   function void set_zero_data_use();
      m_random_data = 1'b0;
   endfunction : set_zero_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: plus_arg_match
   //
   function bit plus_arg_match(input string                plus_arg_name_in,
                               input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                               input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                               input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0),
                               input bit                   use_specific       = 1'b0);
      `evl_check_plusargs(plus_arg_name_in)
      foreach (m_plus_args[ii]) begin
         if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, use_specific) == 1'b1) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : plus_arg_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_plus_arg_value
   //
   function string get_plus_arg_value(input string                plus_arg_name_in,
                                      input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                      input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                      input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      //
      // Search for the most-restrictive +arg first, and then work your way to less restrictive +args.
      //
      // Search for sX,nY,cZ, sX,nY, sX,cZ, and sX
      //
      `evl_check_plusargs(plus_arg_name_in)
      if (shire_id_in != ~evl_shire_id_t'(0)) begin
         if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
            if (core_id_in != ~evl_core_id_t'(0)) begin
               //
               // Search for sX,nY,cZ
               //
               foreach (m_plus_args[ii]) begin
                  if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b111) == 1'b1) begin
                     return m_plus_args[ii].get_arg_value();
                  end
               end
            end
            //
            // Search for sX,nY
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b110) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value();
               end
            end
         end
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for sX,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b101) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value();
               end
            end
         end
         //
         // Search for sX
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b100) == 1'b1) begin
               return m_plus_args[ii].get_arg_value();
            end
         end
      end

      //
      // Search for nY and cZ
      //
      if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for nY,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b011) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value();
               end
            end
         end
         //
         // Search for nY
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b010) == 1'b1) begin
               return m_plus_args[ii].get_arg_value();
            end
         end
      end

      //
      // Search for cZ
      //
      if (core_id_in != ~evl_core_id_t'(0)) begin
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b001) == 1'b1) begin
               return m_plus_args[ii].get_arg_value();
            end
         end
      end

      //
      // Search for any match
      //
      foreach (m_plus_args[ii]) begin
         if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in) == 1'b1) begin
            return m_plus_args[ii].get_arg_value();
         end
      end
      return "";
   endfunction : get_plus_arg_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_plus_arg_value_as_int
   //
   function longint get_plus_arg_value_as_int(input string                plus_arg_name_in,
                                              input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                              input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                              input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      //
      // Search for the most-restrictive +arg first, and then work your way to less restrictive +args.
      //
      // Search for sX,nY,cZ, sX,nY, sX,cZ, and sX
      //
      `evl_check_plusargs(plus_arg_name_in)
      if (shire_id_in != ~evl_shire_id_t'(0)) begin
         if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
            if (core_id_in != ~evl_core_id_t'(0)) begin
               //
               // Search for sX,nY,cZ
               //
               foreach (m_plus_args[ii]) begin
                  if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 1'b1) == 1'b1) begin
                     return m_plus_args[ii].get_arg_value_as_int();
                  end
               end
            end
            //
            // Search for sX,nY
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, ~evl_core_id_t'(0), 1'b1) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value_as_int();
               end
            end
         end
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for sX,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, ~evl_neighborhood_id_t'(0), core_id_in, 1'b1) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value_as_int();
               end
            end
         end
         //
         // Search for sX
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, ~evl_neighborhood_id_t'(0), ~evl_core_id_t'(0), 1'b1) == 1'b1) begin
               return m_plus_args[ii].get_arg_value_as_int();
            end
         end
      end

      //
      // Search for nY,cZ and nY
      //
      if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for nY,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, ~evl_shire_id_t'(0), neighborhood_id_in, core_id_in, 1'b1) == 1'b1) begin
                  return m_plus_args[ii].get_arg_value_as_int();
               end
            end
         end
         //
         // Search for nY
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, ~evl_shire_id_t'(0), neighborhood_id_in, ~evl_core_id_t'(0), 1'b1) == 1'b1) begin
               return m_plus_args[ii].get_arg_value_as_int();
            end
         end
      end

      //
      // Search for cZ
      //
      if (core_id_in != ~evl_core_id_t'(0)) begin
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, ~evl_shire_id_t'(0), ~evl_neighborhood_id_t'(0), core_id_in, 1'b1) == 1'b1) begin
               return m_plus_args[ii].get_arg_value_as_int();
            end
         end
      end

      //
      // Search for any match
      //
      foreach (m_plus_args[ii]) begin
         if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in) == 1'b1) begin
            return m_plus_args[ii].get_arg_value_as_int();
         end
      end
      return -1;
   endfunction : get_plus_arg_value_as_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_plus_arg
   //
   function string sprint_plus_arg(input string                plus_arg_name_in,
                                   input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                   input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                   input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      //
      // Search for the most-restrictive +arg first, and then work your way to less restrictive +args.
      //
      // Search for sX,nY,cZ, sX,nY, sX,cZ, and sX
      //
      `evl_check_plusargs(plus_arg_name_in)
      if (shire_id_in != ~evl_shire_id_t'(0)) begin
         if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
            if (core_id_in != ~evl_core_id_t'(0)) begin
               //
               // Search for sX,nY,cZ
               //
               foreach (m_plus_args[ii]) begin
                  if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b111) == 1'b1) begin
                     return m_plus_args[ii].sprint_obj();
                  end
               end
            end
            //
            // Search for sX,nY
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b110) == 1'b1) begin
                  return m_plus_args[ii].sprint_obj();
               end
            end
         end
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for sX,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b101) == 1'b1) begin
                  return m_plus_args[ii].sprint_obj();
               end
            end
         end
         //
         // Search for sX
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b100) == 1'b1) begin
               return m_plus_args[ii].sprint_obj();
            end
         end
      end

      //
      // Search for nY and cZ
      //
      if (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) begin
         if (core_id_in != ~evl_core_id_t'(0)) begin
            //
            // Search for nY,cZ
            //
            foreach (m_plus_args[ii]) begin
               if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b011) == 1'b1) begin
                  return m_plus_args[ii].sprint_obj();
               end
            end
         end
         //
         // Search for nY
         //
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b010) == 1'b1) begin
               return m_plus_args[ii].sprint_obj();
            end
         end
      end

      //
      // Search for cZ
      //
      if (core_id_in != ~evl_core_id_t'(0)) begin
         foreach (m_plus_args[ii]) begin
            if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in, 3'b001) == 1'b1) begin
               return m_plus_args[ii].sprint_obj();
            end
         end
      end

      //
      // Search for any match
      //
      foreach (m_plus_args[ii]) begin
         if (m_plus_args[ii].arg_match(plus_arg_name_in, shire_id_in, neighborhood_id_in, core_id_in) == 1'b1) begin
            return m_plus_args[ii].sprint_obj();
         end
      end
      return "";
   endfunction : sprint_plus_arg


   //-----------------------------------------------------------------------------------------------
   //
   // Function: extract_plus_args
   //
   function void extract_plus_args(output string plus_arg_names[],
                                   output string plus_arg_roots[],
                                   output string plus_arg_vals[]);
      int limit;

      limit = m_plus_args.size();
      plus_arg_names= new[limit];
      plus_arg_roots= new[limit];
      plus_arg_vals = new[limit];
      foreach (m_plus_args[ii]) begin
         string arg_root_str;
         string arg_roots[];

         m_plus_args[ii].get_root_arg_list(arg_roots);
         arg_root_str = "";
         for (int jj = arg_roots.size() - 1; jj > 0; jj--) begin
            if (arg_root_str == "") begin
               arg_root_str = $sformatf("%s", arg_roots[jj]);
            end
            else begin
               arg_root_str = $sformatf("%s,%s", arg_roots[jj], arg_root_str);
            end
         end
         plus_arg_names[ii] = $sformatf("%0s", arg_roots[0]);
         plus_arg_roots[ii] = $sformatf("%0s", arg_root_str);
         plus_arg_vals[ii]  = $sformatf("%0s", m_plus_args[ii].get_arg_value());
      end
   endfunction : extract_plus_args


   //-----------------------------------------------------------------------------------------------
   //
   // Function: putchar
   //
   // This function is used for code running on cores to print messages to stdout
   //
   function void putchar(input evl_shire_id_t        shire_id,
                         input evl_neighborhood_id_t neigh_id,
                         input evl_core_id_t         core_id,
                         input bit                   thread_id,
                         input byte                  character);
      int idx = int'({ shire_id, neigh_id, core_id, thread_id });

      // A value of 4 (End Of Transmission) forces the end of the test.
      if (character == 4) begin
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_NONE, VMOD_GLOBAL, $sformatf("S%0d:C%0d.%0d-DIAG", shire_id, core_id, thread_id), "Received EOT (End Of Transmission) - Terminating test")
         end
         else begin
            `evl_log(UVM_NONE, VMOD_GLOBAL, $sformatf("S%0d:N%0d:C%0d.%0d-DIAG", shire_id, neigh_id, core_id, thread_id), "Received EOT (End Of Transmission) - Terminating test")
         end
         m_all_threads_done = 1'b1;
         m_cosim_ended      = 1'b1;
         return;
      end

      // ignore LF and CR
      if (!(character inside {10, 13})) begin
         m_stdout_buf[idx] = {m_stdout_buf[idx], character};
      end

      // Flush buffer when LF or maximum (arbitrary) buffer size is reached
      if (character == "\n") begin
         // Specific support to print Sting errors in DUT Error
         if (m_putchar_error[idx] == 1'b1) begin
            if (shire_id >= `NUM_MINION_SHIRES) begin
               `dut_error(get_abstract_name(), $sformatf("S%0d:C%0d.%0d-DIAG ERROR: %0s", shire_id, core_id, thread_id, m_stdout_buf[idx]));
            end
            else begin
               `dut_error(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d.%0d-DIAG ERROR: %0s", shire_id, neigh_id, core_id, thread_id, m_stdout_buf[idx]));
            end
         end
         else begin
            if (shire_id >= `NUM_MINION_SHIRES) begin
               `evl_log(UVM_NONE, VMOD_GLOBAL, $sformatf("S%0d:C%0d.%0d-DIAG", shire_id, core_id, thread_id), m_stdout_buf[idx])
            end
            else begin
               `evl_log(UVM_NONE, VMOD_GLOBAL, $sformatf("S%0d:N%0d:C%0d.%0d-DIAG", shire_id, neigh_id, core_id, thread_id), m_stdout_buf[idx])
            end
         end
         if (strstr(m_stdout_buf[idx], "ERROR MESSAGE STARTS") != -1) begin
            m_putchar_error[idx] = 1'b1;
         end
         else if (strstr(m_stdout_buf[idx], "ERROR MESSAGE ENDS") != -1) begin
            m_putchar_error[idx] = 1'b0;
         end
         else if (strstr(m_stdout_buf[idx], "error", 1) != -1) begin
            if (shire_id >= `NUM_MINION_SHIRES) begin
               `dut_error(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d reported an error (stdout string containing keyword <error>)", shire_id, core_id, thread_id))
            end
            else begin
               `dut_error(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d reported an error (stdout string containing keyword <error>)", shire_id, neigh_id, core_id, thread_id))
            end
         end
         m_stdout_buf[idx] = "";
      end
   endfunction: putchar


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_thread_count
   //
   function int get_thread_count();
      return m_thread_count;
   endfunction: get_thread_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_thread
   //
   function void enable_thread(input evl_shire_id_t        shire_id,
                               input evl_neighborhood_id_t neigh_id,
                               input evl_core_id_t         core_id,
                               input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (!m_cpu_enable.exists(cpu_id) || (m_cpu_enable[cpu_id] == 1'b0)) begin
         m_thread_count++;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d enabled - %0d thread%0s now alive", shire_id, core_id, thread_id, m_thread_count, (m_thread_count != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d enabled - %0d thread%0s now alive", shire_id, neigh_id, core_id, thread_id, m_thread_count, (m_thread_count != 1) ? "s" : ""))
         end
      end
      m_cpu_enable[cpu_id] = 1'b1;
   endfunction: enable_thread


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_thread
   //
   function void disable_thread(input evl_shire_id_t        shire_id,
                                input evl_neighborhood_id_t neigh_id,
                                input evl_core_id_t         core_id,
                                input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (m_cpu_enable.exists(cpu_id) && (m_cpu_enable[cpu_id] == 1'b1)) begin
         m_thread_count--;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d disabled - %0d thread%0s now alive", shire_id, core_id, thread_id, m_thread_count, (m_thread_count != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d disabled - %0d thread%0s now alive", shire_id, neigh_id, core_id, thread_id, m_thread_count, (m_thread_count != 1) ? "s" : ""))
         end
      end
      m_cpu_enable[cpu_id] = 1'b0;
   endfunction: disable_thread


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_thread_started
   //
   function void incr_thread_started(input evl_shire_id_t        shire_id,
                                     input evl_neighborhood_id_t neigh_id,
                                     input evl_core_id_t         core_id,
                                     input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (!m_cpu_enable.exists(cpu_id) || (m_cpu_enable[cpu_id] == 1'b0)) begin
         m_cpu_enable[cpu_id] = 1'b1;
         m_thread_start++;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d started ... %0d thread%0s started", shire_id, core_id, thread_id, m_thread_start, (m_thread_start != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d started ... %0d thread%0s started", shire_id, neigh_id, core_id, thread_id, m_thread_start, (m_thread_start != 1) ? "s" : ""))
         end
      end
   endfunction: incr_thread_started


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_thread_started
   //
   function int get_thread_started();
      return m_thread_start;
   endfunction: get_thread_started


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_neigh_power
   //
   function void set_neigh_power(input bit power_on, input evl_shire_id_t shire_id, input evl_neighborhood_id_t neigh_id);
      evl_core_id_t     core_id   = 0;
      bit               thread_id = 1'b0;
      evl_full_cpu_id_t cpu_id;

      cpu_id = { shire_id, neigh_id, core_id, thread_id };
      if (m_neigh_power_off.exists(cpu_id) && (m_neigh_power_off[cpu_id] == 1'b1)) begin
         m_neigh_power_off[cpu_id] = (power_on == 1'b1) ? 1'b0 : 1'b1;
         if (power_on == 1'b1) begin
            do begin
               thread_id = 1'b0;
               do begin
                  cpu_id = { shire_id, neigh_id, core_id, thread_id };
                  if (m_cpu_enable.exists(cpu_id) && (m_cpu_enable[cpu_id] == 1'b1)) begin
                     if (m_thread_power_off > 0) begin
                        m_thread_power_off--;
                     end
                  end
                  thread_id++;
               end while (thread_id != 0);
               core_id++;
            end while (core_id != 0);
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d powered on - %0d enabled thread%0s now powered off", shire_id, neigh_id, m_thread_power_off, (m_thread_power_off != 1) ? "s" : ""))
         end
      end
      else begin
         m_neigh_power_off[cpu_id] = (power_on == 1'b1) ? 1'b0 : 1'b1;
         if (power_on == 1'b0) begin
            do begin
               thread_id = 1'b0;
               do begin
                  cpu_id = { shire_id, neigh_id, core_id, thread_id };
                  if (m_cpu_enable.exists(cpu_id) && (m_cpu_enable[cpu_id] == 1'b1)) begin
                     m_thread_power_off++;
                  end
                  thread_id++;
               end while (thread_id != 0);
               core_id++;
            end while (core_id != 0);
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d powered off - %0d enabled thread%0s now powered off", shire_id, neigh_id, m_thread_power_off, (m_thread_power_off != 1) ? "s" : ""))
         end
      end
   endfunction: set_neigh_power


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_thread_boot_started
   //
   function void incr_thread_boot_started(input evl_shire_id_t        shire_id,
                                          input evl_neighborhood_id_t neigh_id,
                                          input evl_core_id_t         core_id,
                                          input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (m_cpu_boot_done.exists(cpu_id)) begin
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d started boot code ... %0d thread%0s started boot code", shire_id, core_id, thread_id, m_thread_boot_start, (m_thread_boot_start != 1) ? "s" : ""))
            if (m_cpu_boot_done[cpu_id] == 1'b1) begin
               `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d has already completed the boot code ... either it was restarted or there is a code problem of some sort", shire_id, core_id, thread_id))
            end
            else begin
               `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d has already started the boot code ... either it was restarted or there is a code problem of some sort", shire_id, core_id, thread_id))
            end
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d started boot code ... %0d thread%0s started boot code", shire_id, neigh_id, core_id, thread_id, m_thread_boot_start, (m_thread_boot_start != 1) ? "s" : ""))
            if (m_cpu_boot_done[cpu_id] == 1'b1) begin
               `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d has already completed the boot code ... either it was restarted or there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
            end
            else begin
               `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d has already started the boot code ... either it was restarted or there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
            end
         end
         return;
      end

      m_cpu_boot_done[cpu_id] = 1'b0;
      if (m_thread_boot_start++ == 0) begin
         m_thread_enable_count = get_minion_thread_count();
         `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("%0d Minion thread%0s present", m_thread_enable_count, ((m_thread_enable_count == 1) ? "" : "s")))
      end
      if (m_thread_boot_start > m_thread_enable_count) begin
         m_thread_enable_count = m_thread_boot_start;
      end
      if (shire_id >= `NUM_MINION_SHIRES) begin
         `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d started boot code ... %0d thread%0s started boot code", shire_id, core_id, thread_id, m_thread_boot_start, (m_thread_boot_start != 1) ? "s" : ""))
      end
      else begin
         `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d started boot code ... %0d thread%0s started boot code", shire_id, neigh_id, core_id, thread_id, m_thread_boot_start, (m_thread_boot_start != 1) ? "s" : ""))
      end
   endfunction: incr_thread_boot_started


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_thread_boot_done
   //
   function void incr_thread_boot_done(input evl_shire_id_t        shire_id,
                                       input evl_neighborhood_id_t neigh_id,
                                       input evl_core_id_t         core_id,
                                       input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (!m_cpu_boot_done.exists(cpu_id)) begin
         m_thread_boot_done++;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d boot done ... %0d thread%0s boot done", shire_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
            `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d finished the boot code before it was started ... there is a code problem of some sort", shire_id, core_id, thread_id))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d boot done ... %0d thread%0s boot done", shire_id, neigh_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
            `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d finished the boot code before it was started ... there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
         end
      end
      else if (m_cpu_boot_done[cpu_id] == 1'b1) begin
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d boot done ... %0d thread%0s boot done (again)", shire_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
            `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d finished the boot code more than once ... there is a code problem of some sort", shire_id, core_id, thread_id))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d boot done ... %0d thread%0s boot done (again)", shire_id, neigh_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
            `dut_warning(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d finished the boot code more than once ... there is a code problem of some sort", shire_id, neigh_id, core_id, thread_id))
         end
      end
      else begin
         m_thread_boot_done++;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d boot done ... %0d thread%0s boot done", shire_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d boot done ... %0d thread%0s boot done", shire_id, neigh_id, core_id, thread_id, m_thread_boot_done, (m_thread_boot_done != 1) ? "s" : ""))
         end
      end
      m_cpu_boot_done[cpu_id] = 1'b1;
   endfunction: incr_thread_boot_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_thread_done
   //
   function int get_thread_done();
      return m_thread_done;
   endfunction: get_thread_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_thread_done
   //
   function void incr_thread_done(input evl_shire_id_t        shire_id,
                                  input evl_neighborhood_id_t neigh_id,
                                  input evl_core_id_t         core_id,
                                  input bit                   thread_id);
      evl_full_cpu_id_t cpu_id = { shire_id, neigh_id, core_id, thread_id };

      if (!m_cpu_done.exists(cpu_id) || (m_cpu_done[cpu_id] == 1'b0)) begin
         m_cpu_done[cpu_id] = 1'b1;
         m_thread_done++;
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d done ... %0d thread%0s done", shire_id, core_id, thread_id, m_thread_done, (m_thread_done != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d done ... %0d thread%0s done", shire_id, neigh_id, core_id, thread_id, m_thread_done, (m_thread_done != 1) ? "s" : ""))
         end
      end
      else begin
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:C%0d thread %0d done (again) ... %0d thread%0s done", shire_id, core_id, thread_id, m_thread_done, (m_thread_done != 1) ? "s" : ""))
         end
         else begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d done (again) ... %0d thread%0s done", shire_id, neigh_id, core_id, thread_id, m_thread_done, (m_thread_done != 1) ? "s" : ""))
         end
      end
      if ((!m_cpu_boot_done.exists(cpu_id)) || (m_cpu_boot_done[cpu_id] == 1'b0)) begin
         if (shire_id >= `NUM_MINION_SHIRES) begin
            `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, core_id, thread_id))
         end
         else begin
            `dut_warning(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d completed the test before the boot code completed ... there is a code problem of some sort", shire_id, core_id, thread_id))
         end
         incr_thread_boot_done(shire_id, neigh_id, core_id, thread_id);
      end
      if (m_thread_done >= m_thread_count) begin
         m_all_threads_done = 1'b1;
      end
   endfunction: incr_thread_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_thread_fail
   //
   function void incr_thread_fail(input evl_shire_id_t        shire_id,
                                  input evl_neighborhood_id_t neigh_id,
                                  input evl_core_id_t         core_id,
                                  input bit                   thread_id);
      m_thread_fail++;
      if (shire_id >= `NUM_MINION_SHIRES) begin
         `dut_error(get_abstract_name(), $sformatf("S%0d:C%0d thread %0d jumped to fail ... %0d thread%0s failed", shire_id, core_id, thread_id, m_thread_fail, (m_thread_fail != 1) ? "s" : ""));
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("S%0d:N%0d:C%0d thread %0d jumped to fail ... %0d thread%0s failed", shire_id, neigh_id, core_id, thread_id, m_thread_fail, (m_thread_fail != 1) ? "s" : ""));
      end
      incr_thread_done(shire_id, neigh_id, core_id, thread_id);
   endfunction: incr_thread_fail


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_all_threads_booted
   //
   // The assumption in this code fragment is that if 1 Minion thread executes the boot code, all
   // of them will (unless threads get disabled along the way).
   //
   task wait_for_all_threads_booted();
      if (m_thread_boot_start > 0) begin
         `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("Waiting for %0d thread%0s to complete the boot code", m_thread_count, (m_thread_count == 1) ? "" : "s"))
         #0 wait ((m_thread_boot_done >= (m_thread_count - m_thread_power_off)) || (m_all_threads_done == 1'b1));
      end
   endtask : wait_for_all_threads_booted


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_all_threads_done
   //
   task wait_for_all_threads_done();
      if (m_thread_count > 0) begin
         #0 wait (m_all_threads_done == 1'b1);
      end
   endtask: wait_for_all_threads_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_all_threads_done
   //
   function void set_all_threads_done();
      m_all_threads_done = 1'b1;
   endfunction: set_all_threads_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_timeout_value
   //
   function void set_timeout_value(input evl_phase_type_t phase_type_in, input int timeout_value_in);
      if (timeout_value_in > 0) begin
         m_timeouts[phase_type_in] = timeout_value_in;
      end
   endfunction: set_timeout_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_timeout_value
   //
   function int get_timeout_value(input evl_phase_type_t phase_type_in);
      if (m_timeouts.exists(phase_type_in)) begin
         return m_timeouts[phase_type_in];
      end
      return 10000;
   endfunction: get_timeout_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message
   //
   function void send_message(input string name_in, input int source_in, input int type_in, input evl_dword_t value_in);
      m_message_wrapper.send_message(name_in, source_in, type_in, value_in);
   endfunction : send_message


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_blocking_message_packet
   //
   task get_blocking_message_packet(output evl_message_packet packet_out, input string name_in, input int subscriber_id);
      m_message_wrapper.get_blocking_message_packet(packet_out, name_in, subscriber_id);
   endtask : get_blocking_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: subscribe
   //
   function int subscribe(input string name_in);
      return m_message_wrapper.subscribe(name_in);
   endfunction : subscribe


   //-----------------------------------------------------------------------------------------------
   //
   // Function: force_system_clk_use
   //
   function void force_system_clk_use();
      m_use_sys_clk = 1'b1;
   endfunction : force_system_clk_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: force_local_clk_use
   //
   function void force_local_clk_use();
      m_use_sys_clk = 1'b0;
   endfunction : force_local_clk_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_sys_clk
   //
   function bit use_sys_clk();
      return m_use_sys_clk;
   endfunction : use_sys_clk


   //-----------------------------------------------------------------------------------------------
   //
   // Function: save_trans_desc
   //
   function void save_trans_desc(input evl_trans_desc trans_desc);
      if (m_save_all_descs == 1'b1) begin
         m_all_trans_descs.push_back(trans_desc);
      end
   endfunction : save_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_all_trans_descs
   //
   function void print_all_trans_descs();
      if ((m_save_all_descs == 1'b0) || (`evl_log_check_verbosity(UVM_LOW, VMOD_GLOBAL) == 0)) begin
         return;
      end
      `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("Transaction descriptors with children:"))
      foreach (m_all_trans_descs[ii]) begin
         evl_trans_desc trans_desc = m_all_trans_descs[ii];

         if ((trans_desc.has_parent() == 1'b0) && (trans_desc.has_children() == 1'b1)) begin
            trans_desc.print_from_this();
         end
      end
      `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("End of descriptor list"))
   endfunction : print_all_trans_descs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: raise_objection
   //
   function void raise_objection(input string phase_name, input uvm_phase phase_in, input string src_name = "");
      if (m_objections.exists(phase_name)) begin
         m_objections[phase_name].raise_objection(phase_in, src_name);
      end
   endfunction : raise_objection


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_zero_time
   //
   // This task is used to force zero-time waits, but it requires support in the system agent.
   //
   task wait_zero_time(input int unsigned count);
      if (m_system_agent == null) begin
         m_system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT, 0);
      end
      if (m_system_agent != null) begin
         m_system_agent.wait_zero_time(count);
      end
   endtask : wait_zero_time


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new(input string name_in = "evl_global_attrs");
      super.new("SYS_ATTRS");
      if (m_global_attrs == null) begin
         int    value;
         string plus_arg;

         m_global_attrs = this;
         m_rand_gen     = evl_rand_gen::get_inst();
         m_evl_log      = evl_log::get_inst();
         m_timeouts[PreResetPhase]      = 10000;
         m_timeouts[ResetPhase]         = 10000;
         m_timeouts[PostResetPhase]     = 10000;
         m_timeouts[PreConfigurePhase]  = 10000;
         m_timeouts[ConfigurePhase]     = 200000;
         m_timeouts[PostConfigurePhase] = 10000;
         m_timeouts[PreMainPhase]       = 10000;
         m_timeouts[MainPhase]          = 10000;
         m_timeouts[PostMainPhase]      = 10000;
         m_timeouts[PreShutdownPhase]   = 40000;
         m_timeouts[ShutdownPhase]      = 40000;
         m_timeouts[PostShutdownPhase]  = 40000;
         m_l3_shire_shift = `EVL_LINE_ADDR_LSB;
         m_l3_set_shift   = `EVL_LINE_ADDR_LSB + `SC_L3_SHIRE_ID_SIZE + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE;
         for (int ii = 0; ii < `SC_BANK_ID_SIZE; ii++) begin
            m_l3_bank_shift[ii] = `EVL_LINE_ADDR_LSB + `SC_L3_SHIRE_ID_SIZE;
         end
         for (int ii = 0; ii < `SC_SUB_BANK_ID_SIZE; ii++) begin
            m_l3_subbank_shift[ii] = `EVL_LINE_ADDR_LSB + `SC_L3_SHIRE_ID_SIZE + `SC_BANK_ID_SIZE;
         end
         for (int ii = 0; ii < `NUM_SHIRES; ii++) begin
            m_sc_config[ii] = evl_sc_config_attr_s'(0);
         end
         foreach (m_current_shire_ids[ii]) begin
            m_current_shire_ids[ii] = ii;
         end
         void'(evl_base_pkg::evl_log_create());
         plus_arg = evl_tcl_get_next_plus_arg();
         while (plus_arg != "") begin
            evl_plus_arg plus_arg_info;

            plus_arg_info = new(plus_arg);
            if (plus_arg_info.get_root_arg() != "") begin
               m_plus_args.push_back(plus_arg_info);
               if ((plus_arg_info.get_root_arg() == "tclPath") && (plus_arg_info.get_arg_value() != "")) begin
                  evl_tcl_add_include_path(plus_arg_info.get_arg_value());
               end
            end
            plus_arg = evl_tcl_get_next_plus_arg();
         end
         if (`et_test_plusargs("MINION_MASK=") != 0) begin
            m_minion_mask = int'(get_plus_arg_value_as_int("MINION_MASK"));
         end
         else begin
            m_minion_mask = 0;
         end
         if (`et_test_plusargs("THREAD_MASK=") != 0) begin
            m_thread_mask = int'(get_plus_arg_value_as_int("THREAD_MASK")) & 3;
         end
         else begin
            m_thread_mask = 0;
         end
         if (`et_test_plusargs("SHIRE_MASK=") != 0) begin
            m_shire_mask = get_plus_arg_value_as_int("SHIRE_MASK") & ((longint'(1) << `NUM_MINION_SHIRES) - 1);
         end
         else begin
            m_shire_mask = 1;
         end
         if (`et_value_plusargs("configTimeout=%d", value) != 0) begin
            if (value > 0) begin
               m_timeouts[ConfigurePhase] = value;
            end
         end
         if (`et_value_plusargs("postConfigTimeout=%d", value) != 0) begin
            if (value > 0) begin
               m_timeouts[PostConfigurePhase] = value;
            end
         end
         if (`et_value_plusargs("shutdownTimeout=%d", value) != 0) begin
            if (value > 0) begin
               m_timeouts[ShutdownPhase] = value;
            end
         end
         if (`et_value_plusargs("cover_on=%d", value) == 0) begin
            value = 1;
         end
         m_allow_cover = (value == 0) ? 1'b0 : 1'b1;
         if (`et_test_plusargs("use_system_clk") != 0) begin
            m_use_sys_clk = 1'b1;
         end
         if (`et_test_plusargs("saveAllDescs") != 0) begin
            m_save_all_descs = 1'b1;
         end
         if (`et_test_plusargs("waitForSP") != 0) begin
            m_wait_for_sp = 1'b1;
         end
         m_uvm_report_server = m_uvm_report_server.get_server();
         m_message_wrapper = new(get_abstract_name());
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("created global attributes object (id %0d)", get_inst_id()))
      `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("et_test_plusargs macro:  \"%s\"", `et_test_plusargs_def))
      `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("et_value_plusargs macro: \"%s\"", `et_value_plusargs_def))
   endfunction : new

endclass : evl_global_attrs

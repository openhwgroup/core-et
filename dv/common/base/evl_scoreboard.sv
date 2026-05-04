//--------------------------------------------------------------------------------------------------
//
// Class: evl_scoreboard
//
virtual class evl_scoreboard extends uvm_scoreboard;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_enabled:         This attribute specifies if the scoreboard's checks are throwing errors
   //                    or simply warnings
   //                    ---------------------------------------------------------------------------
   // m_allow_sys_clk:   If this variable is set to 1, the wait_cycles() call can be redirected to
   //                    the system agent with a +arg.
   //                    ---------------------------------------------------------------------------
   // m_err_pk:          This is a parameter key used to control the error intensity of this
   //                    scoreboard's DUT errors.
   //                    ---------------------------------------------------------------------------
   // m_vmod:            This is the verbosity modifier for this component
   //                    ---------------------------------------------------------------------------
   // m_shire_id:        This is the shire ID for this component.  If this component is not local to
   //                    a shire, the value will be all 1s.
   //                    ---------------------------------------------------------------------------
   // m_neighborhood_id: This is the neighborhood ID for this component.  If this component is not
   //                    local to a neighborhood, the value will be all 1s.
   //                    ---------------------------------------------------------------------------
   // m_core_id:         This is the core ID for this component.  If this component is not local to
   //                    a core, the value will be all 1s.
   //                    ---------------------------------------------------------------------------
   // m_agent:           This is a handle to the verification component that is the parent of this
   //                    scoreboard.
   //                    ---------------------------------------------------------------------------
   // m_rtl_port:        This is a handle to the RTL port.  RTL ports are used to connect a
   //                    particular verification component to one or more blocks of Verilog RTL,
   //                    allowing access to clocks, resets, and any integer parameters implemented
   //                    in the RTL.
   //
   bit                   m_enabled         = 1'b1;
   bit                   m_allow_sys_clk   = 1'b1;
   int                   m_err_pk          = 0;
   evl_verbosity_mod_t   m_vmod            = VMOD_GLOBAL;
   evl_shire_id_t        m_shire_id        = ~evl_shire_id_t'(0);
   evl_neighborhood_id_t m_neighborhood_id = ~evl_neighborhood_id_t'(0);
   evl_core_id_t         m_core_id         = ~evl_core_id_t'(0);
   evl_verif_comp        m_agent;
   evl_verif_comp        m_system_agent;
   evl_rtl_port          m_rtl_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   // This function is used to set the shire ID for this component.
   //
   // Input Variables:
   //
   // shire_id_in: This is the shire ID to set for this component.
   //
   virtual function void set_shire_id(input evl_shire_id_t shire_id_in);
      m_shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   // This function is used to get the shire ID for this component.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the shire ID of this component.
   //
   virtual function evl_shire_id_t get_shire_id();
      return m_global_attrs.get_current_shire_id(m_shire_id);
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_neighborhood_id
   //
   // This function is used to set the neighborhood ID for this component.
   //
   // Input Variables:
   //
   // neighborhood_id_in: This is the neighborhood ID to set for this component.
   //
   virtual function void set_neighborhood_id(input evl_neighborhood_id_t neighborhood_id_in);
      m_neighborhood_id = neighborhood_id_in;
   endfunction : set_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_neighborhood_id
   //
   // This function is used to get the neighborhood ID for this component.
   //
   // Return Value:
   //
   // evl_neighborhood_id_t: This is the neighborhood ID of this component.
   //
   virtual function evl_neighborhood_id_t get_neighborhood_id();
      return m_neighborhood_id;
   endfunction : get_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_id
   //
   // This function is used to set the core ID for this component.
   //
   // Input Variables:
   //
   // core_id_in: This is the core ID to set for this component.
   //
   virtual function void set_core_id(input evl_core_id_t core_id_in);
      m_core_id = core_id_in;
   endfunction : set_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_id
   //
   // This function is used to get the core ID for this component.
   //
   // Return Value:
   //
   // evl_core_id_t: This is the core ID of this component.
   //
   virtual function evl_core_id_t get_core_id();
      return m_core_id;
   endfunction : get_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_port
   //
   function void set_rtl_port(input evl_rtl_port rtl_port_in);
      m_rtl_port = rtl_port_in;
   endfunction : set_rtl_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_port
   //
   function evl_rtl_port get_rtl_port();
      return m_rtl_port;
   endfunction : get_rtl_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   // This function is used to set the agent for this scoreboard.
   //
   // Input Variables:
   //
   // agent_in: This is the agent to set for this scoreboard.
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      m_agent = agent_in;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   // This function is used to get the agent for this scoreboard.
   //
   // Return Value:
   //
   // evl_verif_comp: This is the agent of this scoreboard.
   //
   virtual function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle_count
   //
   virtual function int get_cycle_count(input int clock_index_in = 0);
      return m_rtl_port.get_cycle_count(clock_index_in);
   endfunction : get_cycle_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cyles
   //
   task wait_cycles(input int cycle_count_in, input int clock_index_in = 0);
      if (cycle_count_in > 0) begin
         if ((m_global_attrs.use_sys_clk() == 1'b1) && (m_allow_sys_clk == 1'b1) && (m_system_agent != null)) begin
            m_system_agent.wait_cycles(cycle_count_in, clock_index_in);
         end
         else if (m_rtl_port == null) begin
            #(100 * cycle_count_in);
         end
         else begin
            m_rtl_port.wait_cycles(cycle_count_in, clock_index_in);
         end
      end
   endtask : wait_cycles


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cycles_with_release
   //
   task wait_cycles_with_release(input int cycle_count_in, input int clock_index_in = 0);
      if (cycle_count_in > 0) begin
         if ((m_global_attrs.use_sys_clk() == 1'b1) && (m_allow_sys_clk == 1'b1) && (m_system_agent != null)) begin
            m_system_agent.wait_cycles_with_release(cycle_count_in, clock_index_in);
         end
         else if (m_rtl_port == null) begin
            #(100 * cycle_count_in);
         end
         else begin
            m_rtl_port.wait_cycles_with_release(cycle_count_in, clock_index_in);
         end
      end
   endtask : wait_cycles_with_release


   //-----------------------------------------------------------------------------------------------
   //
   // Function: plus_arg_match
   //
   function bit plus_arg_match(input string plus_arg_name_in, input bit use_specific = 1'b0);
      return m_global_attrs.plus_arg_match(plus_arg_name_in, get_shire_id(), get_neighborhood_id(), get_core_id(), use_specific);
   endfunction : plus_arg_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_plus_arg_value
   //
   function string get_plus_arg_value(input string plus_arg_name_in);
      return m_global_attrs.get_plus_arg_value(plus_arg_name_in, get_shire_id(), get_neighborhood_id(), get_core_id());
   endfunction : get_plus_arg_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_plus_arg_value_as_int
   //
   function longint get_plus_arg_value_as_int(input string plus_arg_name_in);
      return m_global_attrs.get_plus_arg_value_as_int(plus_arg_name_in, get_shire_id(), get_neighborhood_id(), get_core_id());
   endfunction : get_plus_arg_value_as_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_region_key
   //
   function int get_addr_region_key(input evl_paddr_t addr_in);
      return m_agent.get_addr_region_key(addr_in);
   endfunction : get_addr_region_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_plus_arg
   //
   function string sprint_plus_arg(input string plus_arg_name_in);
      return m_global_attrs.sprint_plus_arg(plus_arg_name_in, get_shire_id(), get_neighborhood_id(), get_core_id());
   endfunction : sprint_plus_arg


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_region
   //
   //    Returns the address region that the provided address falls in, or null if it doesn't fall
   //    in any region.
   //
   function evl_addr_region_list get_addr_region(input evl_paddr_t addr_in);
      return m_agent.get_addr_region(addr_in);
   endfunction : get_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_region
   //
   //    Returns 1'b1 if the provided address matches the given region
   //
   function bit addr_in_region(input int region_key, input evl_paddr_t addr_in);
      return m_agent.addr_in_region(region_key, addr_in);
   endfunction : addr_in_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_regions
   //
   //    Returns 1'b1 if the provided address matches one of the given regions
   //
   function bit addr_in_regions(input int region_keys[$], input evl_paddr_t addr_in);
      return m_agent.addr_in_regions(region_keys, addr_in);
   endfunction : addr_in_regions


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_response
   //
   virtual function evl_req_desc find_observed_response(input int                 port_in,
                                                        input evl_req_desc        paired_req_desc,
                                                        input bit                 compare_fields = 1'b0,
                                                        input byte                qualifier      = 0,
                                                        input evl_bus_rsp_type_t  rsp_type_in    = `EVL_Rsp_Null,
                                                        input evl_bus_req_id_t    req_id_in      = ~evl_bus_req_id_t'(0),
                                                        input evl_bus_rsp_dest_t  dest_id_in     = ~evl_bus_rsp_dest_t'(0),
                                                        input evl_xword_byte_en_t rsp_byte_en_in = evl_xword_byte_en_t'(0),
                                                        input evl_xword_t         rsp_data_in    = evl_xword_t'(0));
      return null;
   endfunction : find_observed_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sb_error
   //
   //   This function is used to throw DUT errors that will be qualified with this scoreboard's
   //   error intensity key, if it has been defined.
   //
   function void sb_error(input string              msg_in,
                          input evl_mem_desc        mem_desc_in = null,
                          input evl_verbosity_mod_t vmods_in[]  = {},
                          input string              name_in     = "",
                          input string              file_in     = "",
                          input int                 line_in     = 0);
      string              name;
      evl_verbosity_mod_t vmods[];

      name  = (name_in         != "") ? name_in  : get_abstract_name();
      vmods = (vmods_in.size() == 0 ) ? vmods_in : {m_vmod};
      if (mem_desc_in != null) begin
         `dut_error_mdesc(name, mem_desc_in, msg_in, m_err_pk, vmods, "", file_in, line_in)
      end
      else begin
         `dut_error(name, msg_in, m_err_pk, vmods, "", file_in, line_in)
      end
   endfunction : sb_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: coverage_enabled
   //
   function bit coverage_enabled();
      return m_global_attrs.coverage_enabled();
   endfunction : coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_coverage_enabled
   //
   task wait_for_coverage_enabled();
      m_global_attrs.wait_for_coverage_enabled();
   endtask : wait_for_coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      m_system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT, 0);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_scoreboard

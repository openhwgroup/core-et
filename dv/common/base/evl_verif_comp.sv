//--------------------------------------------------------------------------------------------------
//
// Class: evl_verif_comp
//
// This is the base class from which EVL verification components are to be derived.  There are 3
// major types of verification components:
//
// - Bus Verification Component:
//   This type of VC is a wrapper around the traditional UVM bus components.  It includes a master
//   agent (with driver), a slave agent (with driver), a monitor, and an RTL port (which is an
//   object used to connect the verification component to RTL signals).
//
// - RTL Verification Component:
//   This type of VC correlates to a major block of RTL (such as the SOC module, a shire module,
//   etc.) and is essentially equivalent to a UVM environment.  It contains an RTL port and may also
//   contain one or more scoreboards.
//
// - Stimulus Verification Component:
//   This type of VC is used for driving request and/or response stimulus.  Like other VCs, it
//   contains an RTL port.  It is attached to a bus VC through TLM ports.
//
class evl_verif_comp extends evl_agent;

   `uvm_component_utils(evl_verif_comp)

   typedef int evl_subbank_list_t[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static int m_all_active_desc_count = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_shire_id:             This is the shire ID for this component.  If this component is not
   //                         local to a shire, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_neighborhood_id:      This is the neighborhood ID for this component.  If this component is
   //                         not local to a neighborhood, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_core_id:              This is the core ID for this component.  If this component is not
   //                         local to a core, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_agent_type:           This is the enumerated agent type for this component.  Every extension
   //                         of this class ought to have a unique agent type, but multiple classes
   //                         may share aliased agent types.
   //                         ----------------------------------------------------------------------
   // m_agent_id:             This is the virtual ID for this component.  It is possible that this
   //                         value is unused, or that it matches one of the other fields (like the
   //                         neighborhood ID).  The virtual ID is intended to be unique within its
   //                         scope (the scope includes the agent type, shire ID, neighborhood ID,
   //                         and core ID).
   //                         ----------------------------------------------------------------------
   // m_port_id:              This is the port ID for this component.  It is possible that this
   //                         value is unused, or that it matches one of the other fields (like the
   //                         agent ID).
   //                         ----------------------------------------------------------------------
   // m_subport_id:           This is the subport ID for this component.  It is possible that this
   //                         value is unused, or that it matches one of the other fields (like the
   //                         agent ID).
   //                         ----------------------------------------------------------------------
   // m_delays_are_disabled:  This field is a bit map of delay disables.  For example, it is
   //                         possible for delays to be both globally disabled and disabled due to
   //                         the current phase ... subsequent release of either the global or phase
   //                         disable will still leave the other type of disable intact, allowing a
   //                         way to disable delays without fear of having them enabled by another
   //                         code fragment.
   //                         ----------------------------------------------------------------------
   // m_stimulus_is_disabled: This field is a bit map of stimulus disables.  It works in the same
   //                         manner as m_delays_are_disabled.
   //                         ----------------------------------------------------------------------
   // m_vc_parent:            This is a handle to the verification component that created this one.
   //                         Although this information is available through UVM methods, this is
   //                         simply a shortcut to get the correctly-typed parent.
   //                         ----------------------------------------------------------------------
   // m_sequencer_wrappers[]: This list contains handles to this verification component's memory
   //                         sequencer wrappers.  Currently, only those components providing memory
   //                         stimulus will have non-null m_sequencer_wrapper handles.
   //                         ----------------------------------------------------------------------
   // m_delay_wrapper:        This is a handle to an object that contains component-specific delays.
   //                         When providing master stimulus or slave responses, a verification
   //                         component will attempt to get a specific delay from the memory
   //                         descriptor and, if it is not found, attempt to get the delay from the
   //                         m_delay_wrapper object.  Note: an extension of this is to provide an
   //                         array of delay wrappers to differentiate delays for address ranges or
   //                         request types.
   //                         ----------------------------------------------------------------------
   // m_active_master:        This bit should be set to 1'b1 if a bus verification component is
   //                         intended to operate as an active master.  For anything other than bus
   //                         verification components, this bit is meaningless.
   //                         ----------------------------------------------------------------------
   // m_active_slave:         This bit should be set to 1'b1 if a bus verification component is
   //                         intended to operate as an active slave.  For anything other than bus
   //                         verification components, this bit is meaningless.
   //                         ----------------------------------------------------------------------
   // m_passive_slave:        This bit should be set to 1'b1 if a bus verification component is
   //                         intended to operate as a passive slave.  For anything other than bus
   //                         verification components, this bit is meaningless.
   //                         ----------------------------------------------------------------------
   // m_stimulus_agent:       This bit should be set to 1'b1 if this verification component provides
   //                         transaction stimulus (such as loads and stores).
   //                         ----------------------------------------------------------------------
   // m_rtl_stimulus_agent:   This bit should be set to 1'b1 if this verification component provides
   //                         miscellaneous RTL stimulus (such as backpressure).
   //                         ----------------------------------------------------------------------
   // m_memory_agent:         This bit should be set to 1'b1 if this verification component is a
   //                         memory agent.  The implication here is that a memory image may need to
   //                         be pre-loaded in this agent.
   //                         ----------------------------------------------------------------------
   // m_allow_mem_load:       This bit should be set to 1'b1 if this verification component can load
   //                         a memory image.
   //                         ----------------------------------------------------------------------
   // m_caching_agent:        This bit should be set to 1'b1 if this verification component is a
   //                         caching agent.
   //                         ----------------------------------------------------------------------
   // m_allow_sys_clk:        If this variable is set to 1, the wait_cycles() call can be redirected
   //                         to the system agent with a +arg.
   //                         ----------------------------------------------------------------------
   // m_active_desc_count:    This integer is a count of the number of currently outstanding
   //                         descriptors on this verification component.  For bus verification
   //                         components, this will be the number of request descriptors currently
   //                         waiting for responses.  For verification components creating root
   //                         transactions, this will be the number of transaction descriptors
   //                         currently waiting for completion.  There are specific methods for
   //                         manipulating this variable (add_active_mem_desc(),
   //                         delete_active_mem_desc(), check_active_mem_desc(),
   //                         wait_for_active_mem_desc(), and watch_for_timeouts())).
   //                         ----------------------------------------------------------------------
   // m_verif_comp_map:       This is a handle to the verification component map.  Every
   //                         verification component created should have an entry in this map.
   //                         ----------------------------------------------------------------------
   // m_rand_gen:             This is a handle to a random number generator that is guaranteed to be
   //                         name-instance specific.  It can be passed to children objects (such as
   //                         descriptors) to more repeatably generate random values.  This object
   //                         may not be necessary, but there have been problems in the past with
   //                         uniqueness of randomization across objects of the same type.
   //                         ----------------------------------------------------------------------
   // m_agent_aliases[]:      This is a list of alias agent types by which this component can be
   //                         referenced.  The 0th entry in this list will be the true agent type of
   //                         this component.  For every entry in this list, there is a companion
   //                         virtual ID in m_agent_alias_ids[].
   //                         ----------------------------------------------------------------------
   // m_agent_alias_ids[]:    This is a list of virtual IDs (a one-for-one match to an entry in the
   //                         m_agent_aliases[] list).
   //                         ----------------------------------------------------------------------
   // m_master_port_agent:    This variable is used only in bus verification components and is used
   //                         to specify the master agent driving this bus verification component.
   //                         ----------------------------------------------------------------------
   // m_slave_port_agent:     This variable is used only in bus verification components and is used
   //                         to specify the slave agent driving this bus verification component.
   //                         ----------------------------------------------------------------------
   // m_vc_children[]:        This is a list of verification component children for this object.
   //                         ---------------------------------------------------------------------------
   // m_active_mem_descs[]:   This is a list of active descriptors (refer to m_active_desc_count).
   //                         ----------------------------------------------------------------------
   // m_err_pk:               This is a parameter key used to control the error intensity of this
   //                         component's DUT errors
   //                         ---------------------------------------------------------------------------
   // m_vmod:                 This is the verbosity modifier for this component
   //
   evl_shire_id_t             m_shire_id               = ~evl_shire_id_t'(0);
   evl_neighborhood_id_t      m_neighborhood_id        = ~evl_neighborhood_id_t'(0);
   evl_core_id_t              m_core_id                = ~evl_core_id_t'(0);
   evl_agent_type_t           m_agent_type             = NO_AGENT;
   evl_agent_id_t             m_agent_id               = ~evl_agent_id_t'(0);
   evl_core_type_t            m_core_type              = EVL_MinionCore;
   bit                        m_use_shire_id_in_name   = 1'b0;
   int                        m_port_id                = 0;
   int                        m_port_type              = 0;
   int                        m_subport_id             = 0;
   int                        m_delays_are_disabled    = `EVL_GLOBAL_DISABLE;
   int                        m_stimulus_is_disabled   = `EVL_GLOBAL_DISABLE;
   int                        m_alias_parent_count     = 1;
   evl_verif_comp             m_vc_parent              = null;
   evl_delay_wrapper          m_delay_wrapper          = null;
   bit                        m_active_master          = 1'b0;
   bit                        m_active_slave           = 1'b0;
   bit                        m_passive_slave          = 1'b0;
   bit                        m_stimulus_agent         = 1'b0;
   bit                        m_rtl_stimulus_agent     = 1'b0;
   bit                        m_memory_agent           = 1'b0;
   bit                        m_allow_mem_load         = 1'b1;
   bit                        m_caching_agent          = 1'b0;
   bit                        m_allow_sys_clk          = 1'b1;
   bit                        m_use_my_core_id         = 1'b0;
   int                        m_active_desc_count      = 0;
   int                        m_delay_change           = 0;
   int                        m_sequencer_create_count = 1;
   int                        m_err_pk                 = 0;
   evl_verbosity_mod_t        m_vmod                   = VMOD_GLOBAL;
   string                     m_sort_name;
   string                     m_agent_tcl_str;
   evl_sequencer_wrapper_base m_sequencer_wrappers[$];
   evl_verif_comp_map         m_verif_comp_map;
   evl_rand_gen               m_rand_gen;
   evl_agent_type_t           m_agent_aliases[$];
   evl_agent_id_t             m_agent_alias_ids[$];
   evl_verif_comp             m_master_port_agent;
   evl_verif_comp             m_slave_port_agent;
   evl_verif_comp             m_vc_children[$];
   evl_verif_comp             m_system_agent;
   evl_mem_desc               m_active_mem_descs[$];
   evl_reg_block              m_evl_reg_block;
   evl_addr_region_list       m_addr_regions[int];
   evl_delay_wrapper          m_delays_by_paddr[$];
   int                        m_timeout_wait;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_progeny
   //
   // This virtual function is intended to be redefined as necessary to initialize any child
   // components after setting the shire ID, abstract name, etc.
   //
   // Input Variables:
   //
   // type_in: This integer is made available to pass information in as necessary.
   //
   virtual function void init_progeny(input int type_in = 0);
      return;
   endfunction : init_progeny


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This is a redefinition of the base set_abstract_name() function.  It includes shire,
   // neighborhood, and core information as appropriate.
   //
   // Input Variables:
   //
   // name_in: This is the string to which to set the abstract name.
   //
   virtual function void set_abstract_name(input string name_in);
      int wrapper_count;

      super.set_abstract_name(name_in);
      if ((m_vc_parent != null) && (m_vc_parent.get_shire_id() != ~evl_shire_id_t'(0))) begin
         string prefix;

         if (m_vc_parent.get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) begin
            prefix = $sformatf("S%0d:", m_vc_parent.get_shire_id());
         end
         else if (m_vc_parent.get_core_id() == ~evl_core_id_t'(0)) begin
            prefix = $sformatf("S%0d:N%0d:", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id());
         end
         else begin
            prefix = $sformatf("S%0d:N%0d:C%0d:", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id(), m_vc_parent.get_core_id());
         end
         for (int ii = m_abstract_name.len() - 1; ii >= 3; ii--) begin
            if (m_abstract_name.getc(ii) == byte'(":")) begin
               m_abstract_name = m_abstract_name.substr(ii + 1, m_abstract_name.len() - 1);
               break;
            end
         end
         m_abstract_name = $sformatf("%0s%0s", prefix, m_abstract_name);
      end
      else if ((m_use_shire_id_in_name == 1'b1) && (get_shire_id() != ~evl_shire_id_t'(0))) begin
         string prefix;

         prefix = $sformatf("S%0d:", get_shire_id());
         for (int ii = m_abstract_name.len() - 1; ii >= 3; ii--) begin
            if (m_abstract_name.getc(ii) == byte'(":")) begin
               m_abstract_name = m_abstract_name.substr(ii + 1, m_abstract_name.len() - 1);
               break;
            end
         end
         m_abstract_name = $sformatf("%0s%0s", prefix, m_abstract_name);
      end
      wrapper_count = 0;
      foreach (m_sequencer_wrappers[ii]) begin
         if (m_sequencer_wrappers[ii] != null) begin
            wrapper_count++;
         end
      end
      foreach (m_sequencer_wrappers[ii]) begin
         if (m_sequencer_wrappers[ii] != null) begin
            if (wrapper_count > 1) begin
               m_sequencer_wrappers[ii].set_abstract_name($sformatf("%0s(SEQ%0d)", get_root_abstract_name(), ii));
            end
            else begin
               m_sequencer_wrappers[ii].set_abstract_name($sformatf("%0s(SEQ)", get_root_abstract_name()));
            end
         end
      end
      if (m_rtl_ports.size() > 1) begin
         foreach (m_rtl_ports[ii]) begin
            if (m_rtl_ports[ii] != null) begin
               m_rtl_ports[ii].set_abstract_name($sformatf("%0s(RTL%0d)", get_root_abstract_name(), ii));
            end
         end
      end
      else if (m_rtl_port != null) begin
         m_rtl_port.set_abstract_name($sformatf("%0s(RTL)", get_root_abstract_name()));
      end
      init_progeny();
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_name
   //
   function string get_rtl_name();
      if (m_rtl_port == null) begin
         return "";
      end
      return m_rtl_port.get_rtl_name();
   endfunction : get_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_tcl_str
   //
   function string get_agent_tcl_str();
      return m_agent_tcl_str;
   endfunction : get_agent_tcl_str


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_int_param
   //
   // This function is used to set an integer parameter using a particular key.  It is similar to
   // set_int_param(), but it is used to access RTL port integer parameters instead.
   //
   // Input Variables:
   //
   // param_key:   This is the integer key (i.e., the associative array index) to use for the
   //              integer parameter.
   // param_value: This is the value to assign for the given integer parameter.
   // port_num:    This is the RTL port number to use for the set_int_param() call.  If it is
   //              negative, all RTL ports are targets.
   //
   virtual function void set_rtl_int_param(input int param_key, input int param_value, input int port_num = 0);
      if ((port_num == 0) && (m_rtl_port != null)) begin
         m_rtl_port.set_int_param(param_key, param_value);
      end
      else if (port_num < 0) begin
         foreach (m_rtl_ports[ii]) begin
            if (m_rtl_ports[ii] != null) begin
               m_rtl_ports[ii].set_int_param(param_key, param_value);
            end
         end
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].set_int_param(param_key, param_value);
      end
   endfunction : set_rtl_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_rtl_int_param
   //
   // This function is used to increment an integer parameter using a particular key.  It is similar
   // to incr_int_param(), but it is used to access RTL port integer parameters instead.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the
   //            integer parameter.
   // port_num:  This is the RTL port number to use for the set_int_param() call.  If it is
   //            negative, all RTL ports are targets.
   //
   virtual function int incr_rtl_int_param(input int param_key, input int port_num = 0);
      if ((port_num == 0) && (m_rtl_port != null)) begin
         m_rtl_port.incr_int_param(param_key);
      end
      else if (port_num < 0) begin
         foreach (m_rtl_ports[ii]) begin
            if (m_rtl_ports[ii] != null) begin
               m_rtl_ports[ii].incr_int_param(param_key);
            end
         end
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].incr_int_param(param_key);
      end
   endfunction : incr_rtl_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_rtl_int_param
   //
   // This function is used to decrement an integer parameter using a particular key.  It is similar
   // to decr_int_param(), but it is used to access RTL port integer parameters instead.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the
   //            integer parameter.
   // min_value: This is the minimum value the integer paramter should take.
   // port_num:  This is the RTL port number to use for the set_int_param() call.  If it is
   //            negative, all RTL ports are targets.
   //
   virtual function int decr_rtl_int_param(input int param_key, input int min_value = 0, input int port_num = 0);
      if ((port_num == 0) && (m_rtl_port != null)) begin
         m_rtl_port.decr_int_param(param_key, min_value);
      end
      else if (port_num < 0) begin
         foreach (m_rtl_ports[ii]) begin
            if (m_rtl_ports[ii] != null) begin
               m_rtl_ports[ii].decr_int_param(param_key, min_value);
            end
         end
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].decr_int_param(param_key, min_value);
      end
   endfunction : decr_rtl_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_int_param
   //
   // This function is used to get an integer parameter using a particular key.  It is similar to
   // get_int_param(), but it is used to access RTL port integer parameters instead.
   //
   // Input Variables:
   //
   // param_key:     This is the integer key (i.e., the associative array index) to use for the
   //                integer parameter.
   // param_default: This is the default value to return if no integer parameter exists at the
   //                specified key.
   // port_num:      This is the RTL port number to use for the get_int_param() call.  If it is
   //                negative, port 0 is the target.
   //
   // Return Value:
   //
   // int: This is the value of the integer parameter at the specified key.  If no integer parameter
   //      exists at that key, then the param_default value will be returned.
   //
   virtual function int get_rtl_int_param(input int param_key, input int param_default = -1, input int port_num = 0);
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         return m_rtl_port.get_int_param(param_key, param_default);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         return m_rtl_ports[port_num].get_int_param(param_key, param_default);
      end
      return param_default;
   endfunction : get_rtl_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_rtl_int_param
   //
   // This task is used to wait for an integer parameter at a particular key to get modified.  With
   // different input variables, it is possible to wait for a particular value, wait for a
   // particular range, or wait for any change.
   //
   // Input Variables:
   //
   // param_key:       This is the integer key (i.e., the associative array index) to use for the
   //                  integer parameter ping.
   // min_param_value: This is the minimum integer parameter value to wait for; it this value is
   //                  greater than max_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // max_param_value: This is the maximum integer parameter value to wait for; it this value is
   //                  less than min_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // invert_context:  If this is 1'b1, return on condition == FALSE rather than TRUE.
   // port_num:        This is the RTL port number to use for the wait_for_rtl_int_param() call.  If
   //                  it is negative, port 0 is the target.
   //
   virtual task wait_for_rtl_int_param(input int    param_key,
                                       input int    min_param_value = 32'h7fffffff,
                                       input int    max_param_value = 32'h7ffffffe,
                                       input bit    invert_context  = 1'b0,
                                       input int    port_num        = 0,
                                       input int    timeout         = 0,
                                       input string timeout_err     = "");
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         m_rtl_port.wait_for_int_param(param_key, min_param_value, max_param_value, invert_context, timeout, timeout_err);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].wait_for_int_param(param_key, min_param_value, max_param_value, invert_context, timeout, timeout_err);
      end
   endtask : wait_for_rtl_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_int_param_by_name
   //
   // This function is used to set an integer parameter using a particular key, but the key in this
   // case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name:  This is the string name for the key.
   // param_value: This is the value to assign for the given integer parameter.
   // port_num:    This is the RTL port number to use for the set_int_param() call.  If it is
   //              negative, all RTL ports are targets.
   //
   function void set_rtl_int_param_by_name(input string param_name, input int param_value, input int port_num = 0);
      set_rtl_int_param(evl_param_xref::get_param_key(param_name), param_value, port_num);
   endfunction : set_rtl_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_rtl_int_param_by_name
   //
   // This function is used to increment an integer parameter using a particular key, but the key in
   // this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   // port_num:   This is the RTL port number to use for the incr_int_param() call.  If it is
   //             negative, all RTL ports are targets.
   //
   virtual function int incr_rtl_int_param_by_name(input string param_name, input int port_num = 0);
      return incr_rtl_int_param(evl_param_xref::get_param_key(param_name), port_num);
   endfunction : incr_rtl_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_rtl_int_param_by_name
   //
   // This function is used to decrement an integer parameter using a particular key, but the key in
   // this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   // min_value:  This is the minimum value the integer paramter should take.
   // port_num:   This is the RTL port number to use for the decr_int_param() call.  If it is
   //             negative, all RTL ports are targets.
   //
   virtual function int decr_rtl_int_param_by_name(input string param_name, input int min_value = 0, input int port_num = 0);
      return decr_rtl_int_param(evl_param_xref::get_param_key(param_name), min_value);
   endfunction : decr_rtl_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_int_param_by_name
   //
   // This function is used to get the integer parameter at the specified key, but the key in this
   // case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name:    This is the string name for the key.
   // param_default: This is the default value to return if no integer parameter exists at the
   //                specified key.
   // port_num:      This is the RTL port number to use for the get_int_param() call.  If it is
   //                negative, port 0 is the target.
   //
   // Return Value:
   //
   // int: This is the value of the integer parameter at the specified key.  If no integer parameter
   //      exists at that key, then the param_default value will be returned.
   //
   function int get_rtl_int_param_by_name(input string param_name, input int param_default = -1, input int port_num = 0);
      return get_rtl_int_param(evl_param_xref::get_param_key(param_name), param_default, port_num);
   endfunction : get_rtl_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_rtl_int_param_by_name
   //
   // This task is used to wait for an integer parameter at a particular key to get modified.  but
   // the key in this case the key is specified as the string name for it.  With different input
   // variables, it is possible to wait for a particular value, wait for a particular range, or wait
   // for any change.
   //
   // Input Variables:
   //
   // param_name:      This is the string name for the key.
   // min_param_value: This is the minimum integer parameter value to wait for; it this value is
   //                  greater than max_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // max_param_value: This is the maximum integer parameter value to wait for; it this value is
   //                  less than min_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // invert_context:  If this is 1'b1, return on condition == FALSE rather than TRUE.
   // port_num:        This is the RTL port number to use for the wait_for_rtl_int_param() call.  If
   //                  it is negative, port 0 is the target.
   //
   task wait_for_rtl_int_param_by_name(input string param_name,
                                       input int    min_param_value = 32'h7fffffff,
                                       input int    max_param_value = 32'h7ffffffe,
                                       input bit    invert_context  = 1'b0,
                                       input int    port_num        = 0,
                                       input int    timeout         = 0,
                                       input string timeout_err     = "");
      wait_for_rtl_int_param(evl_param_xref::get_param_key(param_name), min_param_value, max_param_value, invert_context, port_num, timeout, timeout_err);
   endtask : wait_for_rtl_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_deasserted
   //
   function bit reset_is_deasserted(input int reset_index_in = 0, input int port_num = 0);
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         return m_rtl_port.reset_is_deasserted(reset_index_in);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         return m_rtl_ports[port_num].reset_is_deasserted(reset_index_in);
      end
      return 1'b0;
   endfunction : reset_is_deasserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_asserted
   //
   function bit reset_is_asserted(input int reset_index_in = 0, input int port_num = 0);
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         return m_rtl_port.reset_is_asserted(reset_index_in);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         return m_rtl_ports[port_num].reset_is_asserted(reset_index_in);
      end
      return 1'b0;
   endfunction : reset_is_asserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_deassertion
   //
   task wait_for_reset_deassertion(input int reset_index_in = 0, input int port_num = 0);
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         m_rtl_port.wait_for_reset_deassertion(reset_index_in);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].wait_for_reset_deassertion(reset_index_in);
      end
   endtask : wait_for_reset_deassertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_assertion
   //
   task wait_for_reset_assertion(input int reset_index_in = 0, input int port_num = 0);
      if ((port_num <= 0) && (m_rtl_port != null)) begin
         m_rtl_port.wait_for_reset_assertion(reset_index_in);
      end
      else if (m_rtl_ports.exists(port_num) && (m_rtl_ports[port_num] != null)) begin
         m_rtl_ports[port_num].wait_for_reset_assertion(reset_index_in);
      end
   endtask : wait_for_reset_assertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_type
   //
   // This function is used to set the agent type and virtual agent ID for this component.
   //
   // Input Variables:
   //
   // agent_type_in: This is the agent type to use for this component.
   // agent_id_in:   This is the virtual agent ID to use for this component.
   //
   function void set_agent_type(input evl_agent_type_t agent_type_in, input evl_agent_id_t agent_id_in);
      if ((m_agent_type != NO_AGENT) && (m_agent_aliases.size() > 0)) begin
         if (m_agent_aliases[0] == m_agent_type) begin
            m_agent_aliases.delete(0);
            m_agent_alias_ids.delete(0);
         end
      end
      m_agent_type = agent_type_in;
      m_agent_id   = agent_id_in;
      m_agent_aliases.push_front(m_agent_type);
      m_agent_alias_ids.push_front(m_agent_id);
   endfunction : set_agent_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_type
   //
   // This function is used to get the agent type for this component.
   //
   // Return Value:
   //
   // evl_agent_type_t: This is the agent type of this component.
   //
   function evl_agent_type_t get_agent_type();
      return m_agent_type;
   endfunction : get_agent_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_id
   //
   // This function is used to set the virtual agent ID for this component.
   //
   // Input Variables:
   //
   // agent_id_in: This is the virtual agent ID to use for this component.
   //
   function void set_agent_id(input evl_agent_id_t agent_id_in);
      set_agent_type(m_agent_type, agent_id_in);
      init_progeny();
   endfunction : set_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_id
   //
   // This function is used to get the virtual agent ID for this component.
   //
   // Return Value:
   //
   // evl_agent_id_t: This is the virtual agent ID of this component.
   //
   function evl_agent_id_t get_agent_id();
      return m_agent_id;
   endfunction : get_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_port_id
   //
   // This function is used to set the port ID for this component.  Port IDs are used by bus
   // verification components to identify connections to objects (like the Shire Cache scoreboard)
   // that support multiple ports of a particular type (like AXI).
   //
   // Input Variables:
   //
   // port_id_in: This is the port ID to use for this component.
   //
   function void set_port_id(input int port_id_in);
      m_port_id = port_id_in;
   endfunction : set_port_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_port_id
   //
   // This function is used to get the port ID for this component.
   //
   // Return Value:
   //
   // int: This is the port ID of this component.
   //
   function int get_port_id();
      return m_port_id;
   endfunction : get_port_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_port_type
   //
   // This function is used to set the port ID for this component.  Port IDs are used by bus
   // verification components to identify connections to objects (like the Shire Cache scoreboard)
   // that support multiple subports of a particular type (like AXI).
   //
   // Input Variables:
   //
   // port_type_in: This is the port type to use for this component.
   //
   function void set_port_type(input int port_type_in);
      m_port_type = port_type_in;
   endfunction : set_port_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_port_type
   //
   // This function is used to get the port ID for this component.
   //
   // Return Value:
   //
   // int: This is the port type of this component.
   //
   function int get_port_type();
      return m_port_type;
   endfunction : get_port_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_subport_id
   //
   // This function is used to set the subport ID for this component.  Port IDs are used by bus
   // verification components to identify connections to objects (like the Shire Cache scoreboard)
   // that support multiple subports of a particular type (like AXI).
   //
   // Input Variables:
   //
   // subport_id_in: This is the subport ID to use for this component.
   //
   function void set_subport_id(input int subport_id_in);
      m_subport_id = subport_id_in;
   endfunction : set_subport_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_subport_id
   //
   // This function is used to get the subport ID for this component.
   //
   // Return Value:
   //
   // int: This is the subport ID of this component.
   //
   function int get_subport_id();
      return m_subport_id;
   endfunction : get_subport_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_type
   //
   // This function is used to set the core type for this component.  Core types are used by bus
   // verification components to control sprint_obj() output (among potentially other things).
   //
   // Input Variables:
   //
   // core_type_in: This is the core type to use for this component.
   //
   virtual function void set_core_type(input evl_core_type_t core_type_in);
      m_core_type = core_type_in;
   endfunction : set_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_type
   //
   // This function is used to get the core type for this component.
   //
   // Return Value:
   //
   // evl_core_type_t: This is the core type of this component.
   //
   virtual function evl_core_type_t get_core_type();
      return m_core_type;
   endfunction : get_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_port
   //
   virtual function void set_rtl_port(input evl_rtl_port rtl_port_in, input int index = 0);
      super.set_rtl_port(rtl_port_in, index);
      if (m_rtl_ports.size() > 1) begin
         foreach (m_rtl_ports[ii]) begin
            if (m_rtl_ports[ii] != null) begin
               m_rtl_ports[ii].set_abstract_name($sformatf("%0s(RTL%0d)", get_root_abstract_name(), ii));
            end
         end
      end
      else if (m_rtl_port != null) begin
         m_rtl_port.set_abstract_name($sformatf("%0s(RTL)", get_root_abstract_name()));
      end
      if ((m_delay_wrapper != null) && m_rtl_ports.exists(index) && (m_rtl_ports[index] != null)) begin
         m_rtl_ports[index].set_delay_wrapper(m_delay_wrapper);
      end
      foreach (m_rtl_ports[ii]) begin
         if (m_rtl_ports[ii] != null) begin
            m_rtl_ports[ii].set_shire_id(m_shire_id);
         end
      end
   endfunction : set_rtl_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_agent_alias
   //
   // This function is used to add an agent type alias for this component.
   //
   // Input Variables:
   //
   // agent_alias_in: This is the agent type alias to add for this component.
   //
   function void add_agent_alias(input evl_agent_type_t agent_alias_in);
      if ((m_agent_type != NO_AGENT) && (!(m_agent_type inside { m_agent_aliases }))) begin
         m_agent_aliases.push_front(m_agent_type);
         m_agent_alias_ids.push_front(m_agent_id);
      end
      if (!(agent_alias_in inside { m_agent_aliases })) begin
         m_agent_aliases.push_back(agent_alias_in);
         m_agent_alias_ids.push_back(~evl_agent_type_t'(0));
      end
   endfunction : add_agent_alias


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_aliases
   //
   // Blah
   //
   function int get_agent_aliases(output evl_agent_type_t agent_aliases[]);
      if (m_agent_aliases.size() <= 1) begin
         agent_aliases = new[0];
         return 0;
      end
      agent_aliases = new[m_agent_aliases.size() - 1];
      for (int ii = 1; ii < m_agent_aliases.size(); ii++) begin
         agent_aliases[ii-1] = m_agent_aliases[ii];
      end
      return agent_aliases.size();
   endfunction : get_agent_aliases


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
      if (m_shire_id != ~evl_shire_id_t'(0)) begin
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].set_shire_id(m_shire_id);
         end
      end
      foreach (m_vc_children[ii]) begin
         m_vc_children[ii].set_abstract_name(m_vc_children[ii].get_abstract_name());
      end
      foreach (m_rtl_ports[ii]) begin
         if (m_rtl_ports[ii] != null) begin
            m_rtl_ports[ii].set_shire_id(m_shire_id);
         end
      end
      init_progeny();
   endfunction : set_shire_id


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
   function void set_neighborhood_id(input evl_neighborhood_id_t neighborhood_id_in, input bit current_only = 1'b0);
      m_neighborhood_id = neighborhood_id_in;
      if (current_only == 1'b0) begin
         if (m_neighborhood_id != ~evl_neighborhood_id_t'(0)) begin
            foreach (m_vc_children[ii]) begin
               m_vc_children[ii].set_neighborhood_id(m_neighborhood_id);
            end
         end
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].set_abstract_name(m_vc_children[ii].get_abstract_name());
         end
         init_progeny();
      end
   endfunction : set_neighborhood_id


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
   function void set_core_id(input evl_core_id_t core_id_in, input bit current_only = 1'b0);
      m_core_id = core_id_in;
      if (current_only == 1'b0) begin
         if (m_core_id != ~evl_core_id_t'(0)) begin
            foreach (m_vc_children[ii]) begin
               m_vc_children[ii].set_core_id(m_core_id);
            end
         end
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].set_abstract_name(m_vc_children[ii].get_abstract_name());
         end
         init_progeny();
      end
   endfunction : set_core_id


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
   function evl_shire_id_t get_shire_id();
      return m_global_attrs.get_current_shire_id(m_shire_id);
   endfunction : get_shire_id


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
   function evl_neighborhood_id_t get_neighborhood_id();
      return m_neighborhood_id;
   endfunction : get_neighborhood_id


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
   function evl_core_id_t get_core_id();
      return m_core_id;
   endfunction : get_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_key
   //
   // This function is used to get the agent key for this component.  The agent key is a number that
   // uniquely identifies this component.
   //
   // Return Value:
   //
   // evl_agent_key_t: This is the agent key of this component.
   //
   function evl_agent_key_t get_agent_key();
      return m_global_attrs.gen_agent_key(m_shire_id, m_neighborhood_id, m_core_id, m_agent_id, m_agent_type, 0);
   endfunction : get_agent_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_active_master
   //
   function bit set_active_master();
      m_active_master = 1'b1;
      m_active_slave  = 1'b0;
      m_passive_slave = 1'b0;
   endfunction : set_active_master


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_active_master
   //
   function bit is_active_master();
      return m_active_master;
   endfunction : is_active_master


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_active_slave
   //
   function bit set_active_slave();
      m_active_master = 1'b0;
      m_active_slave  = 1'b1;
      m_passive_slave = 1'b0;
   endfunction : set_active_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_active_slave
   //
   function bit is_active_slave();
      return m_active_slave;
   endfunction : is_active_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_active_slave
   //
   function bit set_passive_slave();
      m_active_master = 1'b0;
      m_active_slave  = 1'b0;
      m_passive_slave = 1'b1;
   endfunction : set_passive_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_active_slave
   //
   function bit is_passive_slave();
      return m_passive_slave;
   endfunction : is_passive_slave


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_master_port_agent = agent_in;
      if ((m_master_port_agent != this) && (m_master_port_agent != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("setting %0s as a master agent for %0s", m_master_port_agent.get_abstract_name(), get_abstract_name()))
      end
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_master_port_agent;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: this_is_master_port_agent
   //
   virtual function bit this_is_master_port_agent(input int port_id = 0);
      return ((m_master_port_agent == this) || (m_master_port_agent == null)) ? 1'b1 : 1'b0;
   endfunction : this_is_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_port_agent
   //
   virtual function void set_slave_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_slave_port_agent = agent_in;
      if ((m_slave_port_agent != this) && (m_slave_port_agent != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("setting %0s as a slave agent for %0s", m_slave_port_agent.get_abstract_name(), get_abstract_name()))
      end
   endfunction : set_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_slave_port_agent;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: this_is_slave_port_agent
   //
   virtual function bit this_is_slave_port_agent(input int port_id = 0);
      return ((m_slave_port_agent == this) || (m_slave_port_agent == null)) ? 1'b1 : 1'b0;
   endfunction : this_is_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_delays
   //
   function void enable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      if (m_delays_are_disabled != 0) begin
         m_delays_are_disabled &= ~disable_type;
         if (((m_stimulus_agent == 1'b1) || (m_rtl_stimulus_agent == 1'b1) || (m_memory_agent == 1'b1) || (m_caching_agent == 1'b1)) && (m_delays_are_disabled == 0)) begin
            `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), "enabling delays")
         end
      end
      else begin
         m_delays_are_disabled &= ~disable_type;
      end
      foreach (m_rtl_ports[ii]) begin
         if (m_rtl_ports[ii] != null) begin
            m_rtl_ports[ii].enable_delays(disable_type);
         end
      end
   endfunction : enable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_delays
   //
   function void disable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      if (m_delays_are_disabled == 0) begin
         m_delays_are_disabled |= disable_type;
         if (((m_stimulus_agent == 1'b1) || (m_rtl_stimulus_agent == 1'b1) || (m_memory_agent == 1'b1) || (m_caching_agent == 1'b1)) && (m_delays_are_disabled != 0)) begin
            `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), "disabling delays")
         end
      end
      else begin
         m_delays_are_disabled |= disable_type;
      end
      foreach (m_rtl_ports[ii]) begin
         if (m_rtl_ports[ii] != null) begin
            m_rtl_ports[ii].disable_delays(disable_type);
         end
      end
   endfunction : disable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_enabled
   //
   function bit delays_are_enabled();
      return ((m_delays_are_disabled == 0) ? 1'b1 : 1'b0);
   endfunction : delays_are_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_disabled
   //
   function bit delays_are_disabled();
      return ((m_delays_are_disabled == 0) ? 1'b0 : 1'b1);
   endfunction : delays_are_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_enable
   //
   task wait_for_delay_enable();
      if (m_delays_are_disabled != 0) begin
         #0 wait (m_delays_are_disabled == 0);
      end
   endtask : wait_for_delay_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_disable
   //
   task wait_for_delay_disable();
      if (m_delays_are_disabled == 0) begin
         #0 wait (m_delays_are_disabled != 0);
      end
   endtask : wait_for_delay_disable


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_stimulus
   //
   function void enable_stimulus(input int disable_type = `EVL_MASTER_DISABLE);
      if ((m_stimulus_agent == 1'b1) || (m_rtl_stimulus_agent == 1'b1)) begin
         if ((~disable_type & m_stimulus_is_disabled) != m_stimulus_is_disabled) begin
            if ((~disable_type & m_stimulus_is_disabled) == 0) begin
               `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), $sformatf("enabling stimulus (0x%0x -> 0x%0x)", m_stimulus_is_disabled, ~disable_type & m_stimulus_is_disabled))
            end
            else begin
               `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), $sformatf("modifying stimulus disable ... 0x%0x -> 0x%0x", m_stimulus_is_disabled, ~disable_type & m_stimulus_is_disabled))
            end
         end
      end
      m_stimulus_is_disabled &= ~disable_type;
   endfunction : enable_stimulus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_stimulus
   //
   function void disable_stimulus(input int disable_type = `EVL_MASTER_DISABLE);
      if ((m_stimulus_agent == 1'b1) || (m_rtl_stimulus_agent == 1'b1)) begin
         if ((m_stimulus_is_disabled | disable_type) != m_stimulus_is_disabled) begin
            if (m_stimulus_is_disabled == 0) begin
               `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), $sformatf("disabling stimulus (0x%0x -> 0x%0x)", m_stimulus_is_disabled, disable_type | m_stimulus_is_disabled))
            end
            else begin
               `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), $sformatf("modifying stimulus disable ... 0x%0x -> 0x%0x", m_stimulus_is_disabled, disable_type | m_stimulus_is_disabled))
            end
         end
      end
      m_stimulus_is_disabled |= disable_type;
   endfunction : disable_stimulus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: stimulus_is_enabled
   //
   function bit stimulus_is_enabled(input int disable_mask = 0);
      return (((~disable_mask & m_stimulus_is_disabled) == 0) ? 1'b1 : 1'b0);
   endfunction : stimulus_is_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: stimulus_is_disabled
   //
   function bit stimulus_is_disabled(input int disable_mask = 0);
      return (((~disable_mask & m_stimulus_is_disabled) == 0) ? 1'b0 : 1'b1);
   endfunction : stimulus_is_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_stimulus_enable
   //
   task wait_for_stimulus_enable(input int disable_mask = 0);
      if ((~disable_mask & m_stimulus_is_disabled) != 0) begin
         #0 wait ((~disable_mask & m_stimulus_is_disabled) == 0);
      end
   endtask : wait_for_stimulus_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_stimulus_disable
   //
   task wait_for_stimulus_disable(input int disable_mask = 0);
      if ((~disable_mask & m_stimulus_is_disabled) == 0) begin
         #0 wait ((~disable_mask & m_stimulus_is_disabled) != 0);
      end
   endtask : wait_for_stimulus_disable


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
   // Function: add_active_mem_desc
   //
   virtual function void add_active_mem_desc(input evl_mem_desc mem_desc_in);
      if (!(mem_desc_in inside { m_active_mem_descs })) begin
         m_active_mem_descs.push_back(mem_desc_in);
         m_all_active_desc_count++;
         m_active_desc_count++;
      end
   endfunction : add_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_mem_desc
   //
   virtual function void delete_active_mem_desc(input evl_mem_desc mem_desc_in);
      foreach (m_active_mem_descs[ii]) begin
         if (mem_desc_in == m_active_mem_descs[ii]) begin
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
            return;
         end
      end
   endfunction : delete_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_all_active_mem_desc
   //
   virtual function void delete_all_active_mem_desc();
      if (m_active_mem_descs.size() == 0) begin
         return;
      end
      check_active_mem_desc(0);
      if (m_active_mem_descs.size() == 0) begin
         return;
      end
      `dut_warning(get_abstract_name(), $sformatf("deleting %0d active transaction%0s", m_active_mem_descs.size(), ((m_active_mem_descs.size() == 1) ? "" : "s")))
      for (int ii = m_active_mem_descs.size() - 1; ii >= 0; ii--) begin
         `evl_log_mdesc(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), m_active_mem_descs[ii], $sformatf("deleting %0s", m_active_mem_descs[ii].sprint_obj()))
         m_active_mem_descs[ii].set_done();
         m_active_mem_descs[ii].set_issued();
         delete_active_mem_desc(m_active_mem_descs[ii]);
      end
   endfunction : delete_all_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_active_mem_desc
   //
   virtual function check_active_mem_desc(input int cycle_decrement);
      for (int ii = m_active_mem_descs.size() - 1; ii >= 0; ii--) begin
         bit now_done;

         if (m_active_mem_descs[ii].check_for_timeout(now_done, cycle_decrement) != 1'b0) begin
            string extra_info;

            extra_info = (m_active_mem_descs[ii].has_issued() == 1'b1) ? "(was issued)" : "(never issued)";
            `dut_error_mdesc(get_abstract_name(), m_active_mem_descs[ii], $sformatf("%0s timed out %s", m_active_mem_descs[ii].sprint_obj(), extra_info), m_err_pk, { m_vmod })
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
         end
         else if (now_done == 1'b1) begin
            m_active_mem_descs.delete(ii);
            m_active_desc_count--;
            m_all_active_desc_count--;
         end
      end
   endfunction : check_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: non_issued_mem_desc_count()
   //
   virtual function int non_issued_mem_desc_count();
      int count;

      for (int ii = m_active_mem_descs.size() - 1; ii >= 0; ii--) begin
         if ((m_active_mem_descs[ii].has_issued() == 1'b0) && (m_active_mem_descs[ii].is_done() == 1'b0)) begin
            count++;
         end
      end
      return count;
   endfunction : non_issued_mem_desc_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_active_mem_desc
   //
   virtual task wait_for_active_mem_desc(input int timeout_value,
                                         input bit allow_progress = 1'b0,
                                         input bit ignore_timeout = 1'b0,
                                         input bit use_global     = 1'b0);
      if ((m_rtl_port != null) && (m_rtl_port.power_is_off() == 1'b1)) begin
         return;
      end
      wait_cycles(1);
      if ((m_active_desc_count <= 0) && (allow_progress == 1'b0)) begin
         return;
      end
      if (allow_progress == 1'b1) begin
         fork
            begin
               //
               // Wait for the timeout value ... if we reach it, things are a mess.
               //
               wait_cycles(timeout_value);
            end
            begin
               if (use_global == 1'b0) begin
                  #0 if (m_active_desc_count <= 0) begin
                     repeat (20) begin
                        wait_cycles(1);
                        #0 if (m_active_desc_count > 0) begin
                           break;
                        end
                     end
                  end
                  while (m_active_desc_count > 0) begin
                     #0 wait (m_active_desc_count <= 0);
                     repeat (20) begin
                        wait_cycles(1);
                        #0 if (m_active_desc_count > 0) begin
                           break;
                        end
                     end
                  end
               end
               else begin
                  #0 if (m_all_active_desc_count <= 0) begin
                     repeat (20) begin
                        wait_cycles(1);
                        #0 if (m_all_active_desc_count > 0) begin
                           break;
                        end
                     end
                  end
                  while (m_all_active_desc_count > 0) begin
                     #0 wait (m_all_active_desc_count <= 0);
                     repeat (20) begin
                        wait_cycles(1);
                        #0 if (m_all_active_desc_count > 0) begin
                           break;
                        end
                     end
                  end
               end
            end
         join_any
         disable fork;
      end
      if (m_active_desc_count <= 0) begin
         return;
      end

      //
      // Now perform the final check.
      //
      fork
         begin
            wait (m_active_desc_count <= 0);
         end
         begin
            wait_cycles(timeout_value);
         end
      join_any
      disable fork;
      #0 check_active_mem_desc(0);
      if (ignore_timeout == 1'b0) begin
         if (m_active_desc_count > 0) begin
            `dut_error(get_abstract_name(), $sformatf("timed out waiting for %0d transaction%0s to complete", m_active_desc_count, ((m_active_desc_count == 1) ? "" : "s")), m_err_pk, { m_vmod })
            `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), "Currently active transactions:")
            foreach (m_active_mem_descs[ii]) begin
               `evl_log_mdesc(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), m_active_mem_descs[ii], $sformatf("%0s", m_active_mem_descs[ii].sprint_obj()))
            end
         end
         else if (m_all_active_desc_count > 0) begin
            `dut_error(get_abstract_name(), $sformatf("timed out waiting for %0d transaction%0s (somewhere in the system) to complete", m_all_active_desc_count, ((m_all_active_desc_count == 1) ? "" : "s")), m_err_pk, { m_vmod })
         end
      end
   endtask : wait_for_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_timeouts
   //
   virtual task watch_for_timeouts();
      forever begin
         wait (m_active_desc_count > 0);
         wait_cycles(m_timeout_wait);
         #0 check_active_mem_desc(1000);
      end
   endtask : watch_for_timeouts


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_vc_parent
   //
   virtual function void set_vc_parent(input evl_verif_comp verif_comp_in);
      m_vc_parent = verif_comp_in;
      set_abstract_name(m_abstract_name);
   endfunction : set_vc_parent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: agent_match
   //
   function bit agent_match(input  evl_agent_type_t      agent_type_in,
                            input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                            input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                            input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                            input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_agent_aliases.size() == 0) begin
         set_agent_type(m_agent_type, m_agent_id);
      end
      foreach (m_agent_aliases[ii]) begin
         if (m_agent_aliases[ii] == agent_type_in) begin
            if ( (m_agent_alias_ids[ii] == agent_id_in)         ||
                 (m_agent_alias_ids[ii] == ~evl_agent_id_t'(0)) ||
                 (agent_id_in           == ~evl_agent_id_t'(0)) ) begin
               if ( (m_shire_id  == shire_id_in)         ||
                    (m_shire_id  == ~evl_shire_id_t'(0)) ||
                    (shire_id_in == ~evl_shire_id_t'(0)) ) begin
                  if ( (m_neighborhood_id  == neighborhood_id_in)         ||
                       (m_neighborhood_id  == ~evl_neighborhood_id_t'(0)) ||
                       (neighborhood_id_in == ~evl_neighborhood_id_t'(0)) ) begin
                     if ( (m_core_id  == core_id_in)         ||
                          (m_core_id  == ~evl_core_id_t'(0)) ||
                          (core_id_in == ~evl_core_id_t'(0)) ) begin
                        return 1'b1;
                     end
                  end
               end
            end
         end
      end
      return 1'b0;
   endfunction : agent_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: agent_match_by_name
   //
   function bit agent_match_by_name(input  string                agent_type_name_in,
                                    input  evl_agent_id_t        agent_id_in        = ~evl_agent_id_t'(0),
                                    input  evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                                    input  evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                                    input  evl_core_id_t         core_id_in         = ~evl_core_id_t'(0));
      if (m_agent_aliases.size() == 0) begin
         set_agent_type(m_agent_type, m_agent_id);
      end
      foreach (m_agent_aliases[ii]) begin
         string agent_alias_name;

         agent_alias_name = $sformatf("%p", m_agent_aliases[ii]);
         if (agent_alias_name == agent_type_name_in) begin
            if ( (m_agent_alias_ids[ii] == agent_id_in)         ||
                 (m_agent_alias_ids[ii] == ~evl_agent_id_t'(0)) ||
                 (agent_id_in           == ~evl_agent_id_t'(0)) ) begin
               if ( (m_shire_id  == shire_id_in)         ||
                    (m_shire_id  == ~evl_shire_id_t'(0)) ||
                    (shire_id_in == ~evl_shire_id_t'(0)) ) begin
                  if ( (m_neighborhood_id  == neighborhood_id_in)         ||
                       (m_neighborhood_id  == ~evl_neighborhood_id_t'(0)) ||
                       (neighborhood_id_in == ~evl_neighborhood_id_t'(0)) ) begin
                     if ( (m_core_id  == core_id_in)         ||
                          (m_core_id  == ~evl_core_id_t'(0)) ||
                          (core_id_in == ~evl_core_id_t'(0)) ) begin
                        return 1'b1;
                     end
                  end
               end
            end
         end
      end
      return 1'b0;
   endfunction : agent_match_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_vc_parent
   //
   virtual function evl_verif_comp get_vc_parent();
      return m_vc_parent;
   endfunction : get_vc_parent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_vc_child
   //
   virtual function void add_vc_child(input evl_verif_comp verif_comp_in);
      if (!(verif_comp_in inside { m_vc_children })) begin
         m_vc_children.push_back(verif_comp_in);
         verif_comp_in.set_vc_parent(this);
         if (m_vc_children.size() > 0) begin
            m_vc_children.sort with ({ item.m_shire_id, item.m_neighborhood_id, item.m_core_id, item.m_agent_type, item.m_agent_id });
         end
      end
   endfunction : add_vc_child


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_vc_children_by_type
   //
   virtual function void get_vc_children_by_type(output evl_verif_comp children[$], input evl_agent_type_t agent_type_t);
      children = m_vc_children.find with ( { item.m_agent_type == agent_type_t} );
   endfunction : get_vc_children_by_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_sequencer_wrapper
   //
   virtual function void set_sequencer_wrapper(input evl_sequencer_wrapper_base sequencer_wrapper_in);
      if ((sequencer_wrapper_in != null) && (!(sequencer_wrapper_in inside { m_sequencer_wrappers }))) begin
         int index = m_sequencer_wrappers.size();

         m_sequencer_wrappers.push_back(sequencer_wrapper_in);
         m_sequencer_wrappers[index].set_agent(this);
         if (index > 0) begin
            m_sequencer_wrappers[0].set_abstract_name($sformatf("%0s(SEQ0)", get_root_abstract_name()));
            m_sequencer_wrappers[index].set_abstract_name($sformatf("%0s(SEQ%0d)", get_root_abstract_name(), index));
         end
         else begin
            m_sequencer_wrappers[index].set_abstract_name($sformatf("%0s(SEQ)", get_root_abstract_name()));
         end
         m_sequencer_wrappers[index].set_rand_gen(m_rand_gen);
      end
   endfunction : set_sequencer_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sequencer_wrapper
   //
   virtual function evl_sequencer_wrapper_base get_sequencer_wrapper(input int index = 0);
      if (index < m_sequencer_wrappers.size()) begin
         return null;
      end
      return m_sequencer_wrappers[index];
   endfunction : get_sequencer_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sequencer_count
   //
   virtual function int get_sequencer_count();
      return m_sequencer_wrappers.size();
   endfunction : get_sequencer_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequence_wrapper
   //
   virtual function void add_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in, input int index = 0);
      if (sequence_wrapper_in == null) begin
         return;
      end
      sequence_wrapper_in.set_agent(this);
      if (index < m_sequencer_wrappers.size()) begin
         m_sequencer_wrappers[index].add_sequence_wrapper(sequence_wrapper_in);
      end
      m_verif_comp_map.add_me_to_sequence_wrapper_list(sequence_wrapper_in);
   endfunction : add_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ptw_desc_port
   //
   virtual function uvm_analysis_port#(evl_ptw_desc) get_ptw_desc_port(input string port_name   = "unknown",
                                                                       input int    port_number = 0);
      return null;
   endfunction : get_ptw_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ptw_desc_export
   //
   virtual function uvm_analysis_export#(evl_ptw_desc) get_ptw_desc_export(input string port_name   = "unknown",
                                                                           input int    port_number = 0);
      return null;
   endfunction : get_ptw_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pmu_desc_port
   //
   virtual function uvm_analysis_port#(evl_pmu_desc) get_pmu_desc_port(input string port_name   = "unknown",
                                                                       input int    port_number = 0);
      return null;
   endfunction : get_pmu_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pmu_desc_export
   //
   virtual function uvm_analysis_export#(evl_pmu_desc) get_pmu_desc_export(input string port_name   = "unknown",
                                                                           input int    port_number = 0);
      return null;
   endfunction : get_pmu_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_export_to_my_export
   //
   virtual function void connect_export_to_my_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                     input string                             port_name      = "unknown",
                                                     input int                                port_number    = 0,
                                                     input int                                subport_number = 0);
      uvm_analysis_export#(evl_req_desc) req_desc_export;

      req_desc_export = get_req_desc_export(port_name, port_number, subport_number);
      if ((export_in == null) || (req_desc_export == null)) begin
         return;
      end
      `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", export_in.get_full_name(), req_desc_export.get_full_name()))
      export_in.connect(req_desc_export);
   endfunction : connect_export_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);
      uvm_analysis_export#(evl_req_desc) req_desc_export;

      req_desc_export = get_req_desc_export(port_name, port_number, subport_number);
      if ((port_in == null) || (req_desc_export == null)) begin
         return;
      end
      `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), req_desc_export.get_full_name()))
      port_in.connect(req_desc_export);
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_export_to_export
   //
   virtual function void connect_my_export_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                     input string                             port_name      = "unknown",
                                                     input int                                port_number    = 0,
                                                     input int                                subport_number = 0);
      uvm_analysis_export#(evl_req_desc) req_desc_export;

      req_desc_export = get_req_desc_export(port_name, port_number, subport_number);
      if ((export_in == null) || (req_desc_export == null)) begin
         return;
      end
      `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", req_desc_export.get_full_name(), export_in.get_full_name()))
      req_desc_export.connect(export_in);
   endfunction : connect_my_export_to_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_port_to_export
   //
   virtual function void connect_my_port_to_export(input uvm_analysis_export#(evl_req_desc) export_in,
                                                   input string                             port_name      = "unknown",
                                                   input int                                port_number    = 0,
                                                   input int                                subport_number = 0);
      uvm_analysis_port#(evl_req_desc) req_desc_port;

      req_desc_port = get_req_desc_port(port_name, port_number, subport_number);
      if ((export_in == null) || (req_desc_port == null)) begin
         return;
      end
      `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", req_desc_port.get_full_name(), export_in.get_full_name()))
      req_desc_port.connect(export_in);
   endfunction : connect_my_port_to_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      return;
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_ptw_export
   // FUTURE: Emmanuel Marie use wrapper on ports
   //
   virtual function void connect_port_to_my_ptw_export(input uvm_analysis_port#(evl_ptw_desc) port_in,
                                                       input string                           port_name   = "unknown",
                                                       input int                              port_number = 0);
      return;
   endfunction : connect_port_to_my_ptw_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_sequencer
   //
   virtual function evl_push_sequencer#(evl_req_desc) get_slave_sequencer();
      return null;
   endfunction : get_slave_sequencer


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_sequencer
   //
   virtual function evl_push_sequencer#(evl_req_desc) get_master_sequencer();
      return null;
   endfunction : get_master_sequencer


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_reg_predictor_to_monitor
   //
   virtual function void connect_reg_predictor_to_monitor(uvm_reg_predictor #(evl_req_desc) predictor_in);
      return;
   endfunction : connect_reg_predictor_to_monitor


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_delay_wrapper
   //
   virtual function void set_delay_wrapper(input evl_delay_wrapper delay_wrapper_in, input bit overwrite = 1'b0);
      if ((m_delay_wrapper == null) || (overwrite == 1'b1)) begin
         m_delay_wrapper = delay_wrapper_in;
      end
      else begin
         m_delay_wrapper.absorb_delay_wrapper(delay_wrapper_in);
      end
      foreach (m_rtl_ports[ii]) begin
         if (m_rtl_ports[ii] != null) begin
            m_rtl_ports[ii].set_delay_wrapper(m_delay_wrapper);
         end
      end
      m_delay_change++;
   endfunction : set_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay_wrapper
   //
   virtual function evl_delay_wrapper get_delay_wrapper();
      return m_delay_wrapper;
   endfunction : get_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_delay
   //
   virtual function void reset_delay(input int delay_key);
      if (m_delay_wrapper == null) begin
         return;
      end
      m_delay_wrapper.reset_delay(delay_key);
   endfunction : reset_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay
   //
   virtual function void add_delay(input int delay_key, input evl_delay_t delay_in, input bit force_on_delay_list = 1'b0);
      if (m_delay_wrapper == null) begin
         m_delay_wrapper = new($sformatf("%0s(DLY)", get_abstract_name()));
         set_delay_wrapper(m_delay_wrapper);
      end
      m_delay_wrapper.add_delay(delay_key, delay_in, force_on_delay_list);
   endfunction : add_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_delay_range
   //
   virtual function void reset_delay_range(input int         delay_key,
                                           input evl_paddr_t start_addr,
                                           input evl_paddr_t end_addr);
      evl_delay_wrapper delay_wrapper;

      foreach (m_delays_by_paddr[ii]) begin
         if (m_delays_by_paddr[ii].range_and_key_match(delay_key, start_addr, end_addr) == 1'b1) begin
            m_delays_by_paddr[ii].reset();
            return;
         end
      end
      delay_wrapper = new($sformatf("%0s(DLY-RANGE)", get_abstract_name()));
      delay_wrapper.set_addr_range(delay_key, start_addr, end_addr);
      delay_wrapper.reset();
      m_delays_by_paddr.push_back(delay_wrapper);
   endfunction : reset_delay_range


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay_range
   //
   virtual function void add_delay_range(input int         delay_key,
                                         input evl_paddr_t start_addr,
                                         input evl_paddr_t end_addr,
                                         input evl_delay_t delay_in,
                                         input bit         force_on_delay_list = 1'b0);
      evl_delay_wrapper delay_wrapper;

      foreach (m_delays_by_paddr[ii]) begin
         if (m_delays_by_paddr[ii].range_and_key_match(delay_key, start_addr, end_addr) == 1'b1) begin
            m_delays_by_paddr[ii].add_delay(delay_key, delay_in, force_on_delay_list);
            return;
         end
      end
      delay_wrapper = new($sformatf("%0s(DLY-RANGE)", get_abstract_name()));
      delay_wrapper.set_addr_range(delay_key, start_addr, end_addr);
      delay_wrapper.add_delay(delay_key, delay_in, force_on_delay_list);
      m_delays_by_paddr.push_back(delay_wrapper);
   endfunction : add_delay_range


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_delay
   //
   virtual function bit have_delay(input int          delay_key,
                                   input int          index       = 0,
                                   input evl_mem_desc mem_desc_in = null);
      if (mem_desc_in != null) begin
         if (mem_desc_in.have_delay(delay_key, index) == 1'b1) begin
            return 1'b1;
         end
      end
      if (m_delay_wrapper == null) begin
         return 1'b0;
      end
      return m_delay_wrapper.have_delay(delay_key, index);
   endfunction : have_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay
   //
   virtual function bit get_delay(output evl_delay_t  delay_out,
                                  input  int          delay_key,
                                  input  int          index       = 0,
                                  input  evl_mem_desc mem_desc_in = null);
      if (delays_are_enabled() == 1'b0) begin
         delay_out = evl_delay_t'(0);
         return 1'b0;
      end
      if (mem_desc_in != null) begin
         if (mem_desc_in.get_delay(delay_out, delay_key, index) == 1'b1) begin
            return 1'b1;
         end
      end
      if (m_delay_wrapper == null) begin
         delay_out = evl_delay_t'(0);
         return 1'b0;
      end
      return m_delay_wrapper.get_delay(delay_out, delay_key, index);
   endfunction : get_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_delay
   //
   virtual function bit get_next_delay(output evl_delay_t  delay_out,
                                       input  int          delay_key,
                                       input  evl_mem_desc mem_desc_in   = null,
                                       input  evl_delay_t  default_delay = evl_delay_t'(0));
      if (delays_are_enabled() == 1'b0) begin
         delay_out = default_delay;
         return 1'b0;
      end
      if (mem_desc_in != null) begin
         evl_paddr_t paddr = mem_desc_in.get_paddr();

         if (mem_desc_in.get_next_delay(delay_out, delay_key, default_delay) == 1'b1) begin
            return 1'b1;
         end
         foreach (m_delays_by_paddr[ii]) begin
            if (m_delays_by_paddr[ii].has_addr_and_key(delay_out, delay_key, paddr, default_delay) == 1'b1) begin
               return 1'b1;
            end
         end
      end
      if (m_delay_wrapper == null) begin
         delay_out = default_delay;
         return 1'b0;
      end
      return m_delay_wrapper.get_next_delay(delay_out, delay_key, default_delay);
   endfunction : get_next_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay
   //
   virtual task wait_for_delay(input int delay_key);
      if (m_delay_wrapper == null) begin
         int delay_change;

         forever begin
            #0 delay_change = m_delay_change;
            if (m_delay_wrapper != null) begin
               break;
            end
            #0 wait (delay_change != m_delay_change);
         end
      end
      m_delay_wrapper.wait_for_delay(delay_key);
   endtask : wait_for_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_trans_desc_early
   //
   virtual function void fix_stimulus_trans_desc_early(input evl_trans_desc trans_desc_in);
      return;
   endfunction : fix_stimulus_trans_desc_early


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_trans_desc_late
   //
   virtual function void fix_stimulus_trans_desc_late(input evl_trans_desc trans_desc_in);
      return;
   endfunction : fix_stimulus_trans_desc_late


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_req_desc
   //
   virtual function void fix_stimulus_req_desc(input evl_trans_desc trans_desc_in, input evl_req_desc req_desc_in);
      return;
   endfunction : fix_stimulus_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_table_action
   //
   virtual function bit process_table_action(input evl_table_desc table_desc,
                                             input evl_trans_desc trans_desc,
                                             input evl_req_desc   req_desc);
      return 1'b0;
   endfunction : process_table_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_data_action
   //
   virtual function bit process_data_action(input evl_data_action_t data_action,
                                            input evl_trans_desc    trans_desc,
                                            input evl_req_desc      req_desc);
      return 1'b0;
   endfunction : process_data_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tag_action
   //
   virtual function bit process_tag_action(input evl_cache_st_t tag_action,
                                           input evl_trans_desc trans_desc,
                                           input evl_req_desc   req_desc);
      return 1'b0;
   endfunction : process_tag_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_addr_region
   //
   function void clr_addr_region(input int region_key);
      if (m_addr_regions.exists(region_key)) begin
         m_addr_regions[region_key].clr_addr_region();
      end
   endfunction : clr_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_addr_region_by_name
   //
   function void clr_addr_region_by_name(input string region_name);
      int region_key;

      region_key = evl_param_xref::get_param_key(region_name);
      clr_addr_region(region_key);
   endfunction : clr_addr_region_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_region
   //
   function void add_addr_region(input int         region_key,
                                 input evl_paddr_t base_addr,
                                 input evl_paddr_t top_addr);
      if (!m_addr_regions.exists(region_key)) begin
         m_addr_regions[region_key] = new(evl_param_xref::get_param_name(region_key), this);
      end
      m_addr_regions[region_key].add_addr_region(base_addr, top_addr);
   endfunction : add_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_region_by_name
   //
   function void add_addr_region_by_name(input string      region_name,
                                         input evl_paddr_t base_addr,
                                         input evl_paddr_t top_addr);
      int region_key;

      region_key = evl_param_xref::get_param_key(region_name);
      add_addr_region(region_key, base_addr, top_addr);
   endfunction : add_addr_region_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_region
   //
   function void set_addr_region(input int         region_key,
                                 input evl_paddr_t base_addr,
                                 input evl_paddr_t top_addr);
      if (!m_addr_regions.exists(region_key)) begin
         m_addr_regions[region_key] = new(evl_param_xref::get_param_name(region_key), this);
      end
      m_addr_regions[region_key].set_addr_region(base_addr, top_addr);
   endfunction : set_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_region_by_name
   //
   function void set_addr_region_by_name(input string      region_name,
                                         input evl_paddr_t base_addr,
                                         input evl_paddr_t top_addr);
      int region_key;

      region_key = evl_param_xref::get_param_key(region_name);
      set_addr_region(region_key, base_addr, top_addr);
   endfunction : set_addr_region_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_region
   //
   function bit addr_in_region(input int region_key, input evl_paddr_t addr_in);
      if (!m_addr_regions.exists(region_key)) begin
         return (m_vc_parent != null) ? m_vc_parent.addr_in_region(region_key, addr_in) : 1'b0;
      end
      return m_addr_regions[region_key].addr_in_region(addr_in);
   endfunction : addr_in_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_regions
   //
   function bit addr_in_regions(input int region_keys[], input evl_paddr_t addr_in);
      foreach (region_keys[ii]) begin
         if (!m_addr_regions.exists(region_keys[ii])) continue;
         if (m_addr_regions[region_keys[ii]].addr_in_region(addr_in)) return 1'b1;
      end
      return (m_vc_parent != null) ? m_vc_parent.addr_in_regions(region_keys, addr_in): 1'b0;
   endfunction : addr_in_regions


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_region_by_name
   //
   function bit addr_in_region_by_name(input string region_name, input evl_paddr_t addr_in);
      int region_key;

      region_key = evl_param_xref::get_param_key(region_name);
      return addr_in_region(region_key, addr_in);
   endfunction : addr_in_region_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_region_key
   //
   //    Returns the address region that the provided address falls in, or null if it doesn't fall
   //    in any region.
   //
   function int get_addr_region_key(input evl_paddr_t addr_in);
      int region_key;

      foreach (m_addr_regions[region_key]) begin
         if (addr_in_region(region_key, addr_in)) begin
            return region_key;
         end
      end
      return (m_vc_parent != null) ? m_vc_parent.get_addr_region_key(addr_in): null;
   endfunction : get_addr_region_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_region
   //
   //    Returns the address region that the provided address falls in, or null if it doesn't fall
   //    in any region.
   //
   function evl_addr_region_list get_addr_region(input evl_paddr_t addr_in);
      int region_key;

      foreach (m_addr_regions[region_key]) begin
         if (addr_in_region(region_key, addr_in)) begin
            return m_addr_regions[region_key];
         end
      end
      return (m_vc_parent != null) ? m_vc_parent.get_addr_region(addr_in): null;
   endfunction : get_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assign_alias_ids
   //
   function void assign_alias_ids();
      for (int ii = 1; ii < m_agent_aliases.size(); ii++) begin
         m_agent_alias_ids[ii] = m_verif_comp_map.get_agent_vid_for_alias(this, m_agent_aliases[ii], m_alias_parent_count);
      end
      foreach (m_vc_children[ii]) begin
         m_vc_children[ii].assign_alias_ids();
      end
   endfunction : assign_alias_ids


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   //    A bus VC might ask a scoreboard if requests coming from certain sources are to be checked
   //    useful for scoreboards that don't model the whole behaviour of a block, like neigh SB, or
   //    if the scoreboard is disabled
   //
   virtual function bit is_req_checked(input evl_req_desc req_desc_in);
      return 1'b1;
   endfunction : is_req_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_rsp_checked
   //
   //    A bus VC might ask a scoreboard if responses coming from certain sources are to be checked
   //    useful for scoreboards that don't model the whole behaviour of a block, like neigh SB, or
   //    if the scoreboard is disabled
   //
   virtual function bit is_rsp_checked(input evl_req_desc req_desc_in);
      return 1'b1;
   endfunction : is_rsp_checked


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
   // Function: is_bus_vc
   //
   function bit is_bus_vc();
      return ((m_agent_type inside { `EVL_BUS_AGENT_TYPES }) ? 1'b1 : 1'b0);
   endfunction : is_bus_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_stimulus_vc
   //
   function bit is_stimulus_vc();
      return ((m_agent_type inside { `EVL_STIM_AGENT_TYPES }) ? 1'b1 : 1'b0);
   endfunction : is_stimulus_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_rtl_vc
   //
   function bit is_rtl_vc();
      return ((m_agent_type inside { `EVL_RTL_AGENT_TYPES }) ? 1'b1 : 1'b0);
   endfunction : is_rtl_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_random_addresses
   //
   virtual function int get_random_addresses(output evl_paddr_t addrs[],
                                             input  int         addr_type,
                                             input  bit         use_local_in     = 1'b1,
                                             input  bit         use_remote_in    = 1'b0,
                                             input  int         use_shire_in     = -1,
                                             input  int         bank_count_in    = 1,
                                             input  int         subbank_count_in = 1,
                                             input  int         index_count_in   = 1,
                                             input  int         alias_count_in   = 1,
                                             input  int         bank_in          = -1,
                                             input  int         subbank_in       = -1,
                                             input  int         index_in         = -1);
      return evl_addr_alloc::get_random_addresses(addrs, get_shire_id(), addr_type, use_local_in, use_remote_in, use_shire_in, bank_count_in, subbank_count_in, index_count_in, alias_count_in, bank_in, subbank_in, index_in);
   endfunction : get_random_addresses


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_random_aliases
   //
   virtual function int get_random_aliases(output evl_paddr_t addrs_out[],
                                           input  evl_paddr_t addrs_in[],
                                           input  int         addr_type,
                                           input  int         alias_count_in = -1);
      return evl_addr_alloc::get_random_aliases(addrs_out, addrs_in, get_shire_id(), addr_type, alias_count_in);
   endfunction : get_random_aliases


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
   // Function: sprint_plus_arg
   //
   function string sprint_plus_arg(input string plus_arg_name_in);
      return m_global_attrs.sprint_plus_arg(plus_arg_name_in, get_shire_id(), get_neighborhood_id(), get_core_id());
   endfunction : sprint_plus_arg


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_my_core_id
   //
   function void use_my_core_id();
      m_use_my_core_id = 1'b1;
   endfunction : use_my_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reorder_children
   //
   function void reorder_children();
      string prefix;

      if (m_agent_type != NO_AGENT) begin
         string aliases;

         if (m_vc_parent == null) begin
            prefix = " ";
         end
         else if (m_vc_parent.get_shire_id() == ~evl_shire_id_t'(0)) begin
            prefix = " ";
         end
         else if (m_vc_parent.get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) begin
            prefix = $sformatf("S%03d ", m_vc_parent.get_shire_id());
         end
         else if (m_vc_parent.get_core_id() == ~evl_core_id_t'(0)) begin
            if ((m_use_my_core_id == 1'b1) && (get_core_id() != ~evl_core_id_t'(0))) begin
               prefix = $sformatf("S%03d:N%03d:C%03d ", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id(), get_core_id());
            end
            else begin
               prefix = $sformatf("S%03d:N%03d ", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id());
            end
         end
         else begin
            prefix = $sformatf("S%03d:N%03d:C%03d ", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id(), m_vc_parent.get_core_id());
         end

         if (m_agent_type inside { `EVL_RTL_AGENT_TYPES }) begin
            m_sort_name = $sformatf("1 %s%p-%03d", prefix, m_agent_type, m_agent_id);
         end
         else if (m_agent_type inside { `EVL_STIM_AGENT_TYPES }) begin
            m_sort_name = $sformatf("2 %s%p-%03d", prefix, m_agent_type, m_agent_id);
         end
         else if (m_agent_type inside { `EVL_BUS_AGENT_TYPES }) begin
            m_sort_name = $sformatf("3 %s%p-%03d", prefix, m_agent_type, m_agent_id);
         end
         else begin
            m_sort_name = $sformatf("4 %s%p-%03d", prefix, m_agent_type, m_agent_id);
         end
      end
      foreach (m_vc_children[ii]) begin
         m_vc_children[ii].reorder_children();
      end
      if (m_vc_children.size() > 1) begin
         m_vc_children.sort with ( item.m_sort_name );
      end
   endfunction : reorder_children


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_hierarchy
   //
   function void print_hierarchy(input string prefix_in, input bit non_recursive = 1'b0);
      string prefix;

      if (m_agent_type != NO_AGENT) begin
         string aliases;

         if (m_vc_parent == null) begin
            prefix = "";
         end
         else if (m_vc_parent.get_shire_id() == ~evl_shire_id_t'(0)) begin
            prefix = "";
         end
         else if (m_vc_parent.get_neighborhood_id() == ~evl_neighborhood_id_t'(0)) begin
            prefix = $sformatf("S%0d:", m_vc_parent.get_shire_id());
         end
         else if (m_vc_parent.get_core_id() == ~evl_core_id_t'(0)) begin
            if ((m_use_my_core_id == 1'b1) && (get_core_id() != ~evl_core_id_t'(0))) begin
               prefix = $sformatf("S%0d:N%0d:C%0d:", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id(), get_core_id());
            end
            else begin
               prefix = $sformatf("S%0d:N%0d:", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id());
            end
         end
         else begin
            prefix = $sformatf("S%0d:N%0d:C%0d:", m_vc_parent.get_shire_id(), m_vc_parent.get_neighborhood_id(), m_vc_parent.get_core_id());
         end
         foreach (m_agent_aliases[ii]) begin
            if (m_agent_aliases[ii] != m_agent_type) begin
               if (aliases == "") begin
                  if (m_agent_alias_ids[ii] == ~evl_agent_id_t'(0)) begin
                     aliases = $sformatf("%0s%p", prefix, m_agent_aliases[ii]);
                  end
                  else begin
                     aliases = $sformatf("%0s%p-%0d", prefix, m_agent_aliases[ii], m_agent_alias_ids[ii]);
                  end
               end
               else begin
                  if (m_agent_alias_ids[ii] == ~evl_agent_id_t'(0)) begin
                     aliases = $sformatf("%0s, %0s%p", aliases, prefix, m_agent_aliases[ii]);
                  end
                  else begin
                     aliases = $sformatf("%0s, %0s%p-%0d", aliases, prefix, m_agent_aliases[ii], m_agent_alias_ids[ii]);
                  end
               end
            end
         end
         if (aliases == "") begin
            aliases = $sformatf("id %0d", get_inst_id());
         end
         else begin
            aliases = $sformatf("%0s, id %0d", aliases, get_inst_id());
         end
         if (m_agent_id == ~evl_agent_id_t'(0)) begin
            m_agent_tcl_str = $sformatf("%0s%p", prefix, m_agent_type);
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("%0s%0s%0p (%0s) (%0s)", prefix_in, prefix, m_agent_type, get_abstract_name(), aliases))
         end
         else begin
            m_agent_tcl_str = $sformatf("%0s%0s%p-%0d", prefix_in, prefix, m_agent_type, m_agent_id);
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("%0s%0s%0p-%0d (%0s) (%0s)", prefix_in, prefix, m_agent_type, m_agent_id, get_abstract_name(), aliases))
         end
      end
      if (non_recursive == 1'b1) begin
         return;
      end
      foreach (m_vc_children[ii]) begin
         if (m_vc_children[ii].get_shire_id() == ~evl_shire_id_t'(0)) begin
            m_vc_children[ii].print_hierarchy($sformatf("  %0s", prefix_in));
         end
      end
      foreach (m_vc_children[ii]) begin
         if (m_vc_children[ii].get_shire_id() != ~evl_shire_id_t'(0)) begin
            m_vc_children[ii].print_hierarchy($sformatf("  %0s", prefix_in));
         end
      end
   endfunction : print_hierarchy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_delays
   //
   virtual function void print_delays();
      if (m_delay_wrapper != null) begin
         m_delay_wrapper.print_obj(get_abstract_name());
      end
      foreach (m_vc_children[ii]) begin
         m_vc_children[ii].print_delays();
      end
   endfunction : print_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_template
   //
   virtual function void add_addr_template(input evl_paddr_t addr_mask_in,
                                           input evl_paddr_t addr_value_in,
                                           input bit         addr_ignore,
                                           input bit         clear_first = 1'b0);
      return;
   endfunction : add_addr_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem(input string file_name);
      return;
   endfunction : perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_random_data_use
   //
   virtual function void set_random_data_use();
      return;
   endfunction : set_random_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle_count
   //
   virtual function int get_cycle_count(input int port_in = 0, input int clock_index_in = 0);
      if ((port_in <= 0) && (m_rtl_port != null)) begin
         return m_rtl_port.get_cycle_count(clock_index_in);
      end
      else if (m_rtl_ports.exists(port_in) && (m_rtl_ports[port_in] != null)) begin
         return m_rtl_ports[port_in].get_cycle_count(clock_index_in);
      end
      return 0;
   endfunction : get_cycle_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: load_mem
   //
   virtual function void load_mem();
      string file_name;
      string path_name;

      file_name = get_plus_arg_value("memImage");
      if (file_name == "") begin
         file_name = "memImage.hex";
      end
      path_name = get_plus_arg_value("memImagePath");
      if (path_name == "") begin
         path_name = "";
      end
      else if ((path_name != "") && (path_name.substr(path_name.len() - 1, path_name.len() - 1) != "/")) begin
         path_name = $sformatf("%0s/", path_name);
      end
      file_name = $sformatf("%0s%0s", path_name, file_name);
      perform_load_mem(file_name);
   endfunction : load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_derived_hdl_path_name
   //
   virtual function string get_derived_hdl_path_name(input evl_verif_comp agent_in = null);
      evl_verif_comp agent;
      string         parent_hdl_name;
      string         derived_hdl_name;

      agent = (agent_in == null) ? m_vc_parent : agent_in;
      if (agent == null) begin
         return get_full_name();
      end
      parent_hdl_name  = agent_in.get_full_name();
      derived_hdl_name = get_full_name();
      if (derived_hdl_name.substr(0, parent_hdl_name.len() - 1) == parent_hdl_name) begin
         derived_hdl_name = derived_hdl_name.substr(parent_hdl_name.len() + 1);
      end
      return derived_hdl_name;
   endfunction : get_derived_hdl_path_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_evl_reg_block
   //
   virtual function void set_evl_reg_block(input evl_reg_block reg_block_in);
      if (m_evl_reg_block == null) begin
         m_evl_reg_block = reg_block_in;
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("attempting to overwrite an existing register block (%0s, line %0d)", `__FILE__, `__LINE__), m_err_pk, { m_vmod })
      end
   endfunction : set_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_evl_reg_block
   //
   virtual function evl_reg_block get_evl_reg_block();
      return m_evl_reg_block;
   endfunction : get_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: configure_evl_reg_block
   //
   virtual function void configure_evl_reg_block(input evl_reg_block reg_block_in = null, input evl_verif_comp agent_in = null);
      if (m_evl_reg_block != null) begin
         if ((agent_in != null) && (reg_block_in != null)) begin
            reg_block_in.add_evl_reg_block(-1, m_evl_reg_block, get_derived_hdl_path_name(agent_in));
         end
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].configure_evl_reg_block(m_evl_reg_block, this);
         end
      end
      else begin
         foreach (m_vc_children[ii]) begin
            m_vc_children[ii].configure_evl_reg_block(reg_block_in, agent_in);
         end
      end
   endfunction : configure_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: coverage_enabled
   //
   function bit coverage_enabled();
      return m_global_attrs.coverage_enabled();
   endfunction : coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set
   //
   virtual function int get_cache_set(input evl_paddr_t caddr,
                                      input int cway  = 0,
                                      input int ctype = 0);
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_set(caddr, cway, ctype);
   endfunction : get_cache_set


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_bank
   //
   virtual function int get_cache_bank(input evl_paddr_t caddr,
                                       input int cway  = 0,
                                       input int ctype = 0);
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_bank(caddr, cway, ctype);
   endfunction : get_cache_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_subbank
   //
   virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                          input int cway  = 0,
                                          input int ctype = 0);
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.get_cache_subbank(caddr, cway, ctype);
   endfunction : get_cache_subbank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_tag
   //
   virtual function evl_paddr_t get_cache_tag(input int cindex,
                                              input int cway  = 0,
                                              input int ctype = 0);
      if (m_rtl_port == null) begin
         return ~evl_paddr_t'(0);
      end
      return m_rtl_port.get_cache_tag(cindex, cway, ctype);
   endfunction : get_cache_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_data
   //
   virtual function int get_cache_data(output evl_dword_t cdata[],
                                       input  int         cindex,
                                       input  int         cway  = 0,
                                       input  int         ctype = 0);
      if (m_rtl_port == null) begin
         cdata = new[`EVL_LINE_DWORDS];
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            cdata[ii] = ~evl_dword_t'(0);
         end
         return -1;
      end
      return m_rtl_port.get_cache_data(cdata, cindex, cway, ctype);
   endfunction : get_cache_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_bit_err
   //
   virtual function int set_cache_bit_err(input int cindex,
                                          input int cbit0,
                                          input int cbit1 = -1,
                                          input int cway  = 0,
                                          input int ctype = 0,
                                          input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
      if (m_rtl_port == null) begin
         return -1;
      end
      return m_rtl_port.set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr);
   endfunction : set_cache_bit_err


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_coverage_enabled
   //
   task wait_for_coverage_enabled();
      m_global_attrs.wait_for_coverage_enabled();
   endtask : wait_for_coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_zero_time
   //
   // This task is used to force zero-time waits, but it requires support in the RTL interface
   // module.  Within the interface module, an extended version of evl_rtl_port must be created and
   // the module must have a loop that performs non-blocking assigns across 2 variables
   // (non-blocking assigns occur at the end of a simulation tick according to the Verilog spec)
   // repeatedly waits for mismatches and then matches for the specified count.
   //
   virtual task wait_zero_time(input int unsigned count);
      if ((m_system_agent != this) && (m_system_agent != null)) begin
         m_system_agent.wait_zero_time(count);
      end
      else if (m_rtl_port != null) begin
         m_rtl_port.wait_zero_time(count);
      end
   endtask : wait_zero_time


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_verif_comp_map = evl_verif_comp_map::get_inst();
      if (m_agent_type != NO_AGENT) begin
         m_rand_gen = new(get_full_name());
         if (m_rtl_port == null) begin
            if (!uvm_config_db#(evl_rtl_port)::get(null, get_full_name(), "rtl_port", m_rtl_port)) begin
               m_rtl_port = null;
            end
            else begin
               set_rtl_port(m_rtl_port);
            end
         end
         m_verif_comp_map.add_me_to_verif_comp_list(this);
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: end_of_elaboration_phase
   //
   virtual function void end_of_elaboration_phase(uvm_phase phase);
      super.end_of_elaboration_phase(phase);
      if (m_vc_children.size() > 0) begin
         m_vc_children.sort with ({ item.m_shire_id, item.m_neighborhood_id, item.m_core_id, item.m_agent_type, item.m_agent_id });
      end
   endfunction : end_of_elaboration_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      m_system_agent = evl_verif_comp_map::get_verif_comp(SYSTEM_AGENT, 0);
      if (m_system_agent == this) begin
         m_system_agent = null;
      end
      if (m_agent_type != NO_AGENT) begin
         if (get_rtl_name() == "") begin
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("full path is %0s (id %0d)", get_full_name(), get_inst_id()))
         end
         else begin
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("full path is %0s (id %0d)", get_rtl_name(), get_inst_id()))
         end
         if ((m_memory_agent == 1'b1) && (m_allow_mem_load == 1'b1)) begin
            load_mem();
         end
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_verif_comp",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = NO_AGENT);
      super.new(name_in, parent_in);
      m_agent_type    = agent_type_in;
      m_agent_tcl_str = $sformatf("%p", m_agent_type);
      if (`et_value_plusargs("vc_timeout_wait=%d", m_timeout_wait) == 0) begin
         m_timeout_wait = 1000;
      end
      if ((agent_type_in != NO_AGENT) && (m_rtl_port == null)) begin
         if (!uvm_config_db#(evl_rtl_port)::get(null, get_full_name(), "rtl_port", m_rtl_port)) begin
            m_rtl_port = null;
         end
         else begin
            set_rtl_port(m_rtl_port);
         end
      end
   endfunction : new

endclass : evl_verif_comp

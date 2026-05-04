//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_side_env
//
class evl_memshire_side_env extends evl_verif_comp;

   `uvm_component_utils(evl_memshire_side_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_noc_stub)
   `evl_param_key(pk_memshire_use_pll_beh)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_memshire_agents[evl_agent_id_t];


   //-----------------------------------------------------------------------------------------------
   //
   // Memory Shire ID Calculations
   //
   `include "rtl/inc/get_shire_id_functions.vh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      int              mem_agent_id = 0;
      evl_shire_id_t   min_shire_id = ~evl_shire_id_t'(0);
      evl_shire_id_t   max_shire_id = ~evl_shire_id_t'(0);
      uvm_factory      factory;
      uvm_component    component;
      evl_verif_comp   verif_comp;
      evl_rtl_instance instance_list[];
      evl_verif_comp   parent_comp;
      evl_verif_comp   parent_comps[string];

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
            if (instance_list[ii].get_agent_pid() != ~evl_agent_id_t'(0)) begin
               instance_list[ii].propagate_param_strings(verif_comp);
               if (verif_comp.get_agent_type() inside { MEM_SHIRE_RTL_AGENT }) begin
                  verif_comp.set_agent_id((get_agent_id() * 4) + mem_agent_id);
                  verif_comp.set_shire_id(instance_list[ii].get_agent_pid());
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
                  verif_comp.set_int_param(pk_noc_stub, get_int_param(pk_noc_stub));
                  verif_comp.set_int_param(pk_memshire_use_pll_beh, get_int_param(pk_memshire_use_pll_beh));
                  if (min_shire_id == ~evl_shire_id_t'(0)) begin
                     min_shire_id = verif_comp.get_shire_id();
                     max_shire_id = verif_comp.get_shire_id();
                  end
                  else begin
                     if (verif_comp.get_shire_id() < min_shire_id) begin
                        min_shire_id = verif_comp.get_shire_id();
                     end
                     if (verif_comp.get_shire_id() > max_shire_id) begin
                        max_shire_id = verif_comp.get_shire_id();
                     end
                  end
                  mem_agent_id++;
               end
               else begin
                  verif_comp.set_abstract_name($sformatf("%0s-%0d", verif_comp.get_root_abstract_name(), verif_comp.get_agent_id()));
               end
               add_vc_child(verif_comp);
            end
         end
      end
      set_int_param_by_name("pk_memshire_id_min", int'(min_shire_id));
      set_int_param_by_name("pk_memshire_id_max", int'(max_shire_id));
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   // The connect phase for memshires is actually processed in evl_soc_env.
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_memshire_side_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, MEM_SHIRE_SIDE_RTL_AGENT);
      set_abstract_name("MEM_SHIRE_SIDE");
   endfunction : new

endclass : evl_memshire_side_env

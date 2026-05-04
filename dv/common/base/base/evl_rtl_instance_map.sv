//--------------------------------------------------------------------------------------------------
//
// Class: evl_rtl_instance_map
//
class evl_rtl_instance_map extends evl_void;

   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_list_sorted:      Blah.
   //                     --------------------------------------------------------------------------
   // m_rtl_instance_map: Blah.
   //                     --------------------------------------------------------------------------
   // m_instance_list[]:  Blah.
   //
   static bit                  m_list_sorted      = 1'b0;
   static evl_rtl_instance_map m_rtl_instance_map = evl_rtl_instance_map::get_inst();
   static evl_rtl_instance     m_instance_list[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_rtl_instance_map::get_inst
   //
   static function evl_rtl_instance_map get_inst();
      if (m_rtl_instance_map == null) begin
         evl_rtl_instance_map rtl_instance_map;

         rtl_instance_map = new();
      end
      return m_rtl_instance_map;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_rtl_instance_map::add_instance
   //
   static function void add_instance(input evl_rtl_instance rtl_instance_in);
      if (m_rtl_instance_map == null) begin
         void'(evl_rtl_instance_map::get_inst());
      end
      if (!(rtl_instance_in inside { m_instance_list })) begin
         m_instance_list.push_back(rtl_instance_in);
      end
   endfunction : add_instance


   //----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_rtl_instance_map::get_full_list
   //
   static function void get_full_list(output evl_rtl_instance rtl_instance_list[]);
      //
      // Make sure the instance list is sorted (this speeds up subsequent checking).
      //
      if (m_list_sorted == 1'b0) begin
         int              instance_index;
         string           instance_name_list[$];
         evl_rtl_instance instance_list[string];

         instance_index = 0;
         foreach (m_instance_list[ii]) begin
            string sort_name;

            sort_name = m_instance_list[ii].get_full_name();
            instance_list[sort_name] = m_instance_list[ii];
            instance_name_list.push_back(sort_name);
         end
         //
         // Sort the list of non-bus-agents and then create a list of them in reverse name order.
         //
         instance_name_list.sort();
         m_instance_list.delete();
         for (int ii = 0; ii < instance_name_list.size(); ii++) begin
            m_instance_list.push_back(instance_list[instance_name_list[ii]]);
         end
         m_list_sorted = 1'b1;
         if (`et_test_plusargs("print_instance_map") != 0) begin
            `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "RTL Instance Map")
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "================")
            foreach (m_instance_list[ii]) begin
               `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("%0s (%0p)", m_instance_list[ii].get_full_name(), m_instance_list[ii].get_agent_type()))
            end
         end
      end

      rtl_instance_list = new[m_instance_list.size()];
      foreach (m_instance_list[ii]) begin
         rtl_instance_list[ii] = m_instance_list[ii];
      end
   endfunction : get_full_list


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_rtl_instance_map::get_my_children_list
   //
   static function void get_my_children_list(output evl_rtl_instance rtl_instance_list[], input string parent_name);
      int              limit;
      int              index;
      int              parent_name_length;
      evl_rtl_instance child_instance_list[$];

      if (m_rtl_instance_map == null) begin
         void'(evl_rtl_instance_map::get_inst());
      end

      //
      // Make sure the instance list is sorted (this speeds up subsequent checking).
      //
      if (m_list_sorted == 1'b0) begin
         int              instance_index;
         string           instance_name_list[$];
         evl_rtl_instance instance_list[string];

         instance_index = 0;
         foreach (m_instance_list[ii]) begin
            string sort_name;

            sort_name = m_instance_list[ii].get_full_name();
            instance_list[sort_name] = m_instance_list[ii];
            instance_name_list.push_back(sort_name);
         end
         //
         // Sort the list of non-bus-agents and then create a list of them in reverse name order.
         //
         instance_name_list.sort();
         m_instance_list.delete();
         for (int ii = 0; ii < instance_name_list.size(); ii++) begin
            m_instance_list.push_back(instance_list[instance_name_list[ii]]);
         end
         m_list_sorted = 1'b1;
         if (`et_test_plusargs("print_instance_map") != 0) begin
            `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "RTL Instance Map")
            `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, "================")
            foreach (m_instance_list[ii]) begin
               `evl_log_no_name(UVM_NONE, VMOD_GLOBAL, $sformatf("%0s (%0p)", m_instance_list[ii].get_full_name(), m_instance_list[ii].get_agent_type()))
            end
         end
      end

      //
      // Now paw through the entire instance list to find the children of the named parent.
      //
      limit              = m_instance_list.size();
      index              = limit;
      parent_name_length = parent_name.len() - 1;
      for (int ii = 0; ii < limit; ii++) begin
         if (parent_name == m_instance_list[ii].get_full_name()) begin
            index = ii + 1;
            break;
         end
      end
      for (int ii = index; ii < limit; ii++) begin
         int    my_name_length;
         string my_name;

         my_name        = m_instance_list[ii].get_full_name();
         my_name_length = my_name.len() - 1;
         if ((my_name != "") && (my_name_length > parent_name_length) && (my_name.substr(0, parent_name_length) == parent_name)) begin
            bit allow_usage;

            //
            // We have found a child ... now let's make sure it is not a child of a different
            // agent.
            //
            allow_usage = 1'b1;
            for (int jj = index; jj < ii; jj++) begin
               int    potential_parent_name_length;
               string potential_parent_name;

               potential_parent_name        = m_instance_list[jj].get_full_name();
               potential_parent_name_length = potential_parent_name.len() - 1;
               if ((my_name_length > potential_parent_name_length) && (potential_parent_name_length > parent_name_length) && (my_name.substr(0, potential_parent_name_length) == potential_parent_name)) begin
                  allow_usage = 1'b0;
                  break;
               end
            end
            if (allow_usage == 1'b1) begin
               child_instance_list.push_back(m_instance_list[ii]);
            end
         end
      end
      if (child_instance_list.size() <= 0) begin
         rtl_instance_list = new[0];
      end
      else begin
         int              instance_index;
         string           instance_name_list[$];
         evl_rtl_instance instance_list[string];

         //
         // Sort the resulting list by agent type and agent ID.  If we do not do this, we end up
         // creating SHIRE[10] before SHIRE[2].
         //
         instance_index = 0;
         foreach (child_instance_list[ii]) begin
            string sort_name;

            sort_name = $sformatf("%p-%08d %x", child_instance_list[ii].get_agent_type(), child_instance_list[ii].get_agent_pid(), instance_index++);
            instance_list[sort_name] = child_instance_list[ii];
            instance_name_list.push_back(sort_name);
         end
         instance_name_list.sort();

         rtl_instance_list = new[child_instance_list.size()];
         for (int ii = 0; ii < instance_name_list.size(); ii++) begin
            rtl_instance_list[ii] = instance_list[instance_name_list[ii]];
         end
      end
   endfunction : get_my_children_list


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_rtl_instance_map::add_instance_info
   //
   static function bit add_instance_info(input string           name_in,
                                         input evl_agent_type_t agent_type      = NO_AGENT,
                                         input evl_agent_id_t   agent_pid       = ~evl_agent_id_t'(0),
                                         input string           sv_comp         = "unknown",
                                         input string           param_list      = "",
                                         input string           name_prefix     = "",
                                         input string           name_postfix    = "",
                                         input int              skip_levels[]   = {},
                                         input int              additional_hier = 0);
      string           full_name;
      evl_rtl_instance rtl_instance;

      full_name = evl_base_pkg::parse_hier_levels(name_in, name_prefix, name_postfix, skip_levels, 1'b0, additional_hier);
      if (m_rtl_instance_map == null) begin
         void'(evl_rtl_instance_map::get_inst());
      end
      foreach (m_instance_list[ii]) begin
         if ( (m_instance_list[ii].get_full_name()  == full_name)  &&
              (m_instance_list[ii].get_agent_type() == agent_type) &&
              (m_instance_list[ii].get_agent_pid()  == agent_pid)  &&
              (m_instance_list[ii].get_sv_comp()    == sv_comp) ) begin
            return 1'b0;
         end
      end
      rtl_instance = new(full_name, agent_type, agent_pid, sv_comp, param_list);
      return 1'b1;
   endfunction : add_instance_info


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new();
      super.new("InstMap");
      if (m_rtl_instance_map == null) begin
         m_rtl_instance_map = this;
         void'(evl_base_pkg::evl_log_create());
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("created instance map (id %0d)", get_inst_id()))
   endfunction : new

endclass : evl_rtl_instance_map

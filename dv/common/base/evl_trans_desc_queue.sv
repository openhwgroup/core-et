//------------------------------------------------------------------------------------------------
//
// Class: evl_trans_desc_queue
//
// This class extends the evl_desc_queue class to specialize it for evl_trans_desc
// and add useful functions specific to this type of descriptor.
//
class evl_trans_desc_queue extends evl_desc_queue #(evl_trans_desc);

   //-------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   virtual function void print_obj(input string              name_in             = "",
                                   input int                 verbosity_in        = int'(UVM_MEDIUM),
                                   input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL);
      string my_name;

      if (m_descs.size() > 0) begin
         my_name = (name_in == "") ? get_abstract_name() : name_in;
         `evl_log(verbosity_in, verbosity_module_in, my_name, $sformatf("%0s: %0d entr%0s", get_abstract_name(), m_descs.size(), ((m_descs.size() == 1) ? "y" : "ies")))
         foreach (m_descs[ii]) begin
            `evl_log_mdesc(verbosity_in, verbosity_module_in, my_name, m_descs[ii], $sformatf("%3s %0s", ((ii < 10) ? $sformatf("%0d: ", ii) : $sformatf("%0d:", ii)), m_descs[ii].sprint_obj()))
         end
      end
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_from_this
   //
   // This function is used to print the descriptor chain starting from this descriptor.
   //
   virtual function void print_from_this(input int                 verbosity_in        = int'(UVM_MEDIUM),
                                         input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL,
                                         input string              prefix              = "");
      if (`evl_log_check_verbosity(verbosity_in, verbosity_module_in) == 0) begin
         return;
      end
      foreach (m_descs[ii]) begin
         m_descs[ii].print_from_this(verbosity_in, verbosity_module_in, prefix);
      end
   endfunction : print_from_this


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_children_are_done
   //
   // This function will return 1'b1 if this descriptor is complete and all its children are done.
   //
   virtual function bit all_children_are_done();
      foreach (m_descs[ii]) begin
         if (m_descs[ii].is_done() == 1'b0) begin
            return 1'b0;
         end
      end
      return 1'b1;
   endfunction : all_children_are_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gather_desc_stats
   //
   // This function is used to gather descriptor statistics.
   //
   virtual function void gather_desc_stats(input int level, ref evl_desc_stat_t desc_stats[$]);
      foreach (m_descs[ii]) begin
         m_descs[ii].gather_desc_stats(level, desc_stats);
      end
   endfunction : gather_desc_stats


   //-------------------------------------------------------------------------
   //
   // Function: filter_cmd
   //
   function evl_trans_desc_queue filter_cmd(input evl_cmd_t cmd);
      evl_trans_desc       tmp_trans_desc;
      evl_trans_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_trans_desc = m_descs[ii];
         if (tmp_trans_desc.get_cmd() == cmd) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s-cmd(%0p)", get_abstract_name(), cmd));
            end
            filter_queue.push(tmp_trans_desc);
         end
      end
      return filter_queue;
   endfunction : filter_cmd


   //--------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_trans_desc_queue");
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_trans_desc_queue

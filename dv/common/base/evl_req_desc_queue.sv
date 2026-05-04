//--------------------------------------------------------------------------------------------------
//
// Class: evl_req_desc_queue_base
//
class evl_req_desc_queue_base extends evl_desc_queue#(evl_req_desc);

   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_req_desc_queue_base", input evl_desc_type_t desc_type_in = EVL_DESC_UNKNOWN);
      super.new(name_in, desc_type_in);
   endfunction : new

endclass : evl_req_desc_queue_base


//--------------------------------------------------------------------------------------------------
//
// Class: evl_req_desc_queue
//
class evl_req_desc_queue extends evl_req_desc_queue_base;

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
   // Function: filter_bus_req_id
   //
   virtual function evl_req_desc_queue filter_bus_req_id(input evl_bus_req_id_t bus_req_id);
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if (tmp_req_desc.get_bus_req_id() == bus_req_id) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s-id(%0x)", get_abstract_name(), bus_req_id));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_bus_req_id


   //-------------------------------------------------------------------------
   //
   // Function: filter_write_bus_req_id
   //
   virtual function evl_req_desc_queue filter_write_bus_req_id(input evl_bus_req_id_t bus_req_id);
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if ((tmp_req_desc.get_bus_req_id() == bus_req_id) && (tmp_req_desc.bus_cmd_is_write() == 1'b1)) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s-id(%0x)", get_abstract_name(), bus_req_id));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_write_bus_req_id


   //-------------------------------------------------------------------------
   //
   // Function: filter_read_bus_req_id
   //
   virtual function evl_req_desc_queue filter_read_bus_req_id(input evl_bus_req_id_t bus_req_id);
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if ((tmp_req_desc.get_bus_req_id() == bus_req_id) && (tmp_req_desc.bus_cmd_is_write() == 1'b0)) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s-id(%0x)", get_abstract_name(), bus_req_id));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_read_bus_req_id


   //-------------------------------------------------------------------------
   //
   // Function: filter_bus_tag_id
   //
   virtual function evl_req_desc_queue filter_bus_tag_id(input int bus_tag_id);
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if (tmp_req_desc.get_req_tag_id() == bus_tag_id) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s(ID-%04x)", get_root_abstract_name(), bus_tag_id));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_bus_tag_id


   //-------------------------------------------------------------------------
   //
   // Function: filter_paddr
   //
   virtual function evl_req_desc_queue filter_paddr(input evl_paddr_t paddr_in, input evl_paddr_t addr_mask_in = ~evl_paddr_t'(0));
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if ((tmp_req_desc.get_paddr() & addr_mask_in) == (paddr_in & addr_mask_in)) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s(ID-%04x)", get_root_abstract_name(), paddr_in));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_paddr


   //-------------------------------------------------------------------------
   //
   // Function: filter_bus_type
   //
   virtual function evl_req_desc_queue filter_bus_type(input evl_bus_type_t bus_type_in);
      evl_req_desc       tmp_req_desc;
      evl_req_desc_queue filter_queue;

      for (int ii = 0; ii < m_descs.size(); ii++) begin
         tmp_req_desc = m_descs[ii];
         if (tmp_req_desc.get_bus_type() == bus_type_in) begin
            if (filter_queue == null) begin
               filter_queue = new($sformatf("%s(ID-%04x)", get_root_abstract_name(), bus_type_in));
            end
            filter_queue.push(tmp_req_desc);
         end
      end
      return filter_queue;
   endfunction : filter_bus_type


   //-------------------------------------------------------------------------
   //
   // Function: find_first_read
   //
   virtual function evl_req_desc find_first_read();
      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_read() == 1'b1) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_read


   //-------------------------------------------------------------------------
   //
   // Function: find_all_reads
   //
   virtual function int find_all_reads(output evl_req_desc descs[]);
      int count = 0;

      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_read() == 1'b1) begin
            count++;
         end
      end
      if (count == 0) begin
         descs = new[0];
         return 0;
      end
      descs = new[count];
      count = 0;
      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_read() == 1'b1) begin
            descs[count] = m_descs[ii];
            count++;
         end
      end
      return count;
   endfunction : find_all_reads


   //-------------------------------------------------------------------------
   //
   // Function: find_first_write
   //
   virtual function evl_req_desc find_first_write();
      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_write() == 1'b1) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_write


   //-------------------------------------------------------------------------
   //
   // Function: find_all_writes
   //
   virtual function int find_all_writes(output evl_req_desc descs[]);
      int count = 0;

      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_write() == 1'b1) begin
            count++;
         end
      end
      if (count == 0) begin
         descs = new[0];
         return 0;
      end
      descs = new[count];
      count = 0;
      foreach (m_descs[ii]) begin
         if (m_descs[ii].bus_cmd_is_write() == 1'b1) begin
            descs[count] = m_descs[ii];
            count++;
         end
      end
      return count;
   endfunction : find_all_writes


   //-------------------------------------------------------------------------
   //
   // Function: find_oldest_rsp
   //
   virtual function evl_req_desc find_oldest_rsp();
      int cur_cycle = 32'h7fffffff;
      int cur_idx   = 0;

      if (m_descs.size() < 1) begin
         return null;
      end
      else if (m_descs.size() < 2) begin
         return m_descs[0];
      end
      foreach (m_descs[ii]) begin
         if (m_descs[ii].get_bus_rsp_type() != `EVL_Rsp_Null) begin
            cur_cycle = m_descs[ii].get_rsp_cycle();
            cur_idx   = ii;
            break;
         end
      end
      foreach (m_descs[ii]) begin
         int rsp_cycle;

         rsp_cycle = m_descs[ii].get_rsp_cycle();
         if ((rsp_cycle != 0) && (rsp_cycle < cur_cycle) && (m_descs[ii].get_bus_rsp_type() != `EVL_Rsp_Null)) begin
            cur_cycle = rsp_cycle;
            cur_idx   = ii;
         end
      end
      return get_desc(cur_idx);
   endfunction : find_oldest_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_req_desc_queue", input evl_desc_type_t desc_type_in = EVL_DESC_UNKNOWN);
      super.new(name_in, desc_type_in);
   endfunction : new

endclass : evl_req_desc_queue

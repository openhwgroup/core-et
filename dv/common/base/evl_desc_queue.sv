//------------------------------------------------------------------------------------------------
//
// Class: evl_desc_queue
//
// This class contains a queue of DESC_TYPE objects
// and some handy functions to go along with it.
//
class evl_desc_queue #(type DESC_TYPE = evl_mem_desc) extends uvm_void;

   `include "dv/common/base/evl_base_inc_name.svh"

   evl_desc_type_t m_desc_type = EVL_DESC_UNKNOWN;
   DESC_TYPE       m_descs[$];
   int             m_size;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_desc_type
   //
   function evl_desc_type_t get_desc_type();
      return m_desc_type;
   endfunction : get_desc_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push
   //
   function void push(input DESC_TYPE desc_in);
      m_descs.push_back(desc_in);
      m_size = m_descs.size();
   endfunction: push


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push_desc
   //
   function int push_desc(input DESC_TYPE desc_in);
      if (desc_in inside { m_descs }) begin
         foreach (m_descs[ii]) begin
            if (m_descs[ii] == desc_in) begin
               return ii;
            end
         end
      end
      push(desc_in);
      return (m_descs.size() - 1);
   endfunction : push_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: peek
   //
   function DESC_TYPE peek();
      if (m_descs.size() > 0) begin
         return m_descs[0];
      end
      return null;
   endfunction: peek


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get
   //
   function DESC_TYPE get(input int index_in);
      if (m_descs.size() > index_in) begin
         return m_descs[index_in];
      end
      return null;
   endfunction: get


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_desc
   //
   function DESC_TYPE get_desc(input int index_in = 0);
      return get(index_in);
   endfunction : get_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pop
   //
   function DESC_TYPE pop(input DESC_TYPE desc = null);
      if (m_descs.size() == 0) begin
         return null;
      end
      if (desc == null) begin
         DESC_TYPE desc_out;

         desc_out = m_descs.pop_front();
         m_size   = m_descs.size();
         return desc_out;
      end
      foreach (m_descs[ii]) begin
         if (m_descs[ii] == desc) begin
            m_descs.delete(ii);
            m_size = m_descs.size();
            return desc;
         end
      end
      return null;
   endfunction: pop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: replace
   //
   function DESC_TYPE replace(input DESC_TYPE desc_in, input DESC_TYPE replace_desc_in);
      foreach (m_descs[ii]) begin
         if (m_descs[ii] == desc_in) begin
            m_descs[ii] = replace_desc_in;
            return desc_in;
         end
      end
      push(replace_desc_in);
      return null;
   endfunction: replace


   //-----------------------------------------------------------------------------------------------
   //
   // Function: size
   //
   function int size();
      return m_descs.size();
   endfunction: size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_desc
   //
   function int has_desc(input DESC_TYPE desc_in);
      foreach (m_descs[ii]) begin
         if (m_descs[ii] == desc_in) begin
            return 1;
         end
      end
      return 0;
   endfunction : has_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_desc
   //
   task wait_for_desc();
      #0 wait (m_size > 0);
   endtask : wait_for_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_and_set_mru
   //
   virtual function DESC_TYPE find_and_set_mru(input evl_paddr_t addr_in);
      foreach (m_descs[ii]) begin
         if (m_descs[ii].line_addr_match(addr_in) == 1'b1) begin
            DESC_TYPE desc;

            desc = m_descs[ii];
            m_descs.delete(ii);
            m_descs.push_back(desc);
            m_size = m_descs.size();
            return desc;
         end
      end
      return null;
   endfunction: find_and_set_mru


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_and_remove_by_addr
   //
   virtual function void find_and_remove_by_addr(input evl_paddr_t paddr_in, input evl_paddr_t addr_mask = const_line_paddr_mask);
      for (int ii = m_descs.size() - 1; ii >= 0; ii--) begin
         if (((m_descs[ii].get_paddr() ^ paddr_in) & addr_mask) == 0) begin
            m_descs.delete(ii);
            m_size = m_descs.size();
         end
      end
   endfunction : find_and_remove_by_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_pk
   //
   virtual function DESC_TYPE find_first_by_pk(input int param_key_in, input int pk_val_in = 1);
      foreach (m_descs[ii]) begin
         if (m_descs[ii].get_int_param(param_key_in) == pk_val_in) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_by_pk


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_cache_addr
   //
   virtual function DESC_TYPE find_first_by_cache_addr(input evl_paddr_t paddr_in);
      foreach (m_descs[ii]) begin
         if (((m_descs[ii].get_paddr() ^ paddr_in) & const_line_paddr_mask) == 0) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_by_cache_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_thread_id
   //
   virtual function DESC_TYPE find_first_by_thread_id(input evl_thread_id_t thread_id);
      foreach (m_descs[ii]) begin
         if (m_descs[ii].get_thread_id() == thread_id) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_by_thread_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_cache_addr_and_trid
   //
   virtual function DESC_TYPE find_first_by_cache_addr_and_trid(input evl_paddr_t    paddr_in,
                                                                input evl_trans_id_t trans_id_in);
      foreach (m_descs[ii]) begin
         if (((m_descs[ii].get_paddr() ^ paddr_in) & const_line_paddr_mask) == 0) begin
            if ( (m_descs[ii].get_trans_id() == trans_id_in) ||
                 (trans_id_in inside { ~evl_trans_id_t'(0), evl_trans_id_t'(0) }) ||
                 (m_descs[ii].get_trans_id() inside { ~evl_trans_id_t'(0), evl_trans_id_t'(0) }) ) begin
               return m_descs[ii];
            end
         end
      end
      return null;
   endfunction : find_first_by_cache_addr_and_trid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_all_by_cache_addr
   //
   virtual function int find_all_by_cache_addr(output DESC_TYPE descs_out[], input evl_paddr_t paddr_in);
      DESC_TYPE desc_queue[$];

      foreach (m_descs[ii]) begin
         if (((m_descs[ii].get_paddr() ^ paddr_in) & const_line_paddr_mask) == 0) begin
            desc_queue.push_back(m_descs[ii]);
         end
      end
      if (desc_queue.size() == 0) begin
         return 0;
      end
      descs_out = new[desc_queue.size()](desc_queue);
      return desc_queue.size();
   endfunction : find_all_by_cache_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_vaddr
   //
   virtual function DESC_TYPE find_first_by_vaddr(input evl_vaddr_t vaddr_in,
                                                  input evl_vaddr_t addr_mask = const_line_vaddr_mask,
                                                  input int         param_key = 0,
                                                  input int         pk_val_in = 1);
      foreach (m_descs[ii]) begin
         if ((((m_descs[ii].get_vaddr() ^ vaddr_in) & addr_mask) == 0) && ((param_key == 0) || (m_descs[ii].get_int_param(param_key) == pk_val_in))) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_by_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_by_paddr
   //
   virtual function DESC_TYPE find_first_by_paddr(input evl_paddr_t paddr_in,
                                                  input evl_paddr_t addr_mask = const_line_paddr_mask,
                                                  input int         param_key = 0,
                                                  input int         pk_val_in = 1);
      foreach (m_descs[ii]) begin
         if ((((m_descs[ii].get_paddr() ^ paddr_in) & addr_mask) == 0) && ((param_key == 0) || (m_descs[ii].get_int_param(param_key) == pk_val_in))) begin
            return m_descs[ii];
         end
      end
      return null;
   endfunction : find_first_by_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_all_by_vaddr
   //
   virtual function int find_all_by_vaddr(output DESC_TYPE   descs_out[],
                                          input  evl_vaddr_t vaddr_in,
                                          input  evl_vaddr_t addr_mask = const_line_vaddr_mask,
                                          input  int         param_key = 0,
                                          input  int         pk_val_in = 1);
      DESC_TYPE desc_queue[$];

      foreach (m_descs[ii]) begin
         if ((((m_descs[ii].get_vaddr() ^ vaddr_in) & addr_mask) == 0) && ((param_key == 0) || (m_descs[ii].get_int_param(param_key) == pk_val_in))) begin
            desc_queue.push_back(m_descs[ii]);
         end
      end
      if (desc_queue.size() == 0) begin
         return 0;
      end
      descs_out = new[desc_queue.size()](desc_queue);
      return desc_queue.size();
   endfunction : find_all_by_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_all_by_paddr
   //
   virtual function int find_all_by_paddr(output DESC_TYPE   descs_out[],
                                          input  evl_paddr_t paddr_in,
                                          input  evl_paddr_t addr_mask = const_line_paddr_mask,
                                          input  int         param_key = 0,
                                          input  int         pk_val_in = 1);
      DESC_TYPE desc_queue[$];

      foreach (m_descs[ii]) begin
         if ((((m_descs[ii].get_paddr() ^ paddr_in) & addr_mask) == 0) && ((param_key == 0) || (m_descs[ii].get_int_param(param_key) == pk_val_in))) begin
            desc_queue.push_back(m_descs[ii]);
         end
      end
      if (desc_queue.size() == 0) begin
         return 0;
      end
      descs_out = new[desc_queue.size()](desc_queue);
      return desc_queue.size();
   endfunction : find_all_by_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: filter_bus_req_id
   //
   virtual function evl_desc_queue #(DESC_TYPE) filter_bus_req_id(input evl_bus_req_id_t bus_req_id);
      return null;
   endfunction : filter_bus_req_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: filter_bus_tag_id
   //
   virtual function evl_desc_queue #(DESC_TYPE) filter_bus_tag_id(input int bus_tag_id);
      return null;
   endfunction : filter_bus_tag_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_read
   //
   virtual function DESC_TYPE find_first_read();
      return null;
   endfunction : find_first_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_all_reads
   //
   virtual function int find_all_reads(output DESC_TYPE descs[]);
      descs = new[0];
      return 0;
   endfunction : find_all_reads


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_first_write
   //
   virtual function DESC_TYPE find_first_write();
      return null;
   endfunction : find_first_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_all_writes
   //
   virtual function int find_all_writes(output DESC_TYPE descs[]);
      descs = new[0];
      return 0;
   endfunction : find_all_writes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_all_descs
   //
   virtual function void delete_all_descs();
      m_descs = {};
      m_size  = 0;
   endfunction : delete_all_descs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   virtual function void print_obj(input string              name_in             = "",
                                   input int                 verbosity_in        = int'(UVM_MEDIUM),
                                   input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL);
      return;
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
      return;
   endfunction : print_from_this


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_children_are_done
   //
   // This function will return 1'b1 if this descriptor is complete and all its children are done.
   //
   virtual function bit all_children_are_done();
      return 1'b1;
   endfunction : all_children_are_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gather_desc_stats
   //
   // This function is used to gather descriptor statistics.
   //
   virtual function void gather_desc_stats(input int level, ref evl_desc_stat_t desc_stats[$]);
      return;
   endfunction : gather_desc_stats


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_desc_queue", input evl_desc_type_t desc_type_in = EVL_DESC_UNKNOWN);
      set_abstract_name(name_in);
      m_desc_type = desc_type_in;
      m_size      = 0;
   endfunction : new

endclass : evl_desc_queue

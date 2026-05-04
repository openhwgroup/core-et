//--------------------------------------------------------------------------------------------------
//
// Class: evl_table_wrapper
//
class evl_table_wrapper #(type CACHE_TYPE = evl_cache, parameter TABLE_NAME = "unknown") extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_table_cover #(CACHE_TYPE, TABLE_NAME) m_table_cover;
   evl_table_desc                            m_table_entries[longint];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_table_entry
   //
   virtual function evl_table_desc get_table_entry(input longint index);
      if (m_table_entries.exists(index)) begin
         return m_table_entries[index];
      end
      return null;
   endfunction : get_table_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_coverage
   //
   function void enable_coverage();
      if (m_table_cover == null) begin
         m_table_cover = new(get_abstract_name());
      end
   endfunction : enable_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_coverage
   //
   function void build_coverage();
      if (m_table_cover != null) begin
         m_table_cover.build();
      end
   endfunction : build_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_table_entry
   //
   virtual function void add_table_entry(input longint index, input evl_table_desc table_desc_in, input string name_in = "", input bit gen_cover = 1'b1);
      if (table_desc_in.index_is_set() == 1'b0) begin
         table_desc_in.set_index(index);
         if (gen_cover == 1'b1) begin
            table_desc_in.set_table_cover(m_table_cover, name_in);
         end
      end
      if (m_table_entries.exists(index)) begin
         `dut_error(get_abstract_name(), $sformatf("entry for index 0x%0x (%0s) already exists", index, name_in))
      end
      m_table_entries[index] = table_desc_in;
   endfunction : add_table_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dump_tables
   //
   virtual function void dump_tables();
      longint index;

      foreach (m_table_entries[index]) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("0x%08x: %s (%0d)", index, m_table_entries[index].sprint_obj(), m_table_entries[index].m_cover_index))
      end
   endfunction : dump_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_table_wrapper");
      super.new(name_in);
   endfunction : new

endclass : evl_table_wrapper

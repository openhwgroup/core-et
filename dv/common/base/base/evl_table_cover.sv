//--------------------------------------------------------------------------------------------------
//
// Class: evl_table_cover_base
//
class evl_table_cover_base extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   string m_table_name;
   int    m_limit;
   int    m_cover_index_xref[longint];
   string m_entry_names[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_table_cover_entry
   //
   function int add_table_cover_entry(input longint index_in, input string entry_name_in);
      if (!m_cover_index_xref.exists(index_in)) begin
         m_cover_index_xref[index_in] = m_entry_names.size();
         m_entry_names.push_back(entry_name_in);
      end
      return m_cover_index_xref[index_in];
   endfunction : add_table_cover_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   virtual function void build();
      return;
   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample
   //
   virtual function void sample(input int cover_index_in);
      return;
   endfunction : sample


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_table_cover_base");
      super.new(name_in);
      m_table_name  = name_in;
      m_limit       = 0;
      m_entry_names.push_back("full_table");
   endfunction : new

endclass : evl_table_cover_base


//--------------------------------------------------------------------------------------------------
//
// Class: evl_table_cover
//
class evl_table_cover #(type CACHE_TYPE = evl_cache, parameter string TABLE_NAME = "unknown") extends evl_table_cover_base;

   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_table_cover_entry #(CACHE_TYPE, TABLE_NAME) m_entries[];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   virtual function void build();
      if ((m_entry_names.size() > 1) && (m_limit == 0)) begin
         m_limit      = m_entry_names.size();
         m_entries    = new[m_limit];
         m_entries[0] = new("full_table", m_limit - 2);
         for (int ii = 1; ii < m_limit; ii++) begin
            m_entries[ii] = new(m_entry_names[ii], 0);
         end
      end
   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample
   //
   virtual function void sample(input int cover_index_in);
      if ((cover_index_in < 0) || (m_limit <= 0)) begin
         return;
      end
      if (cover_index_in < m_limit) begin
         m_entries[cover_index_in].sample(0);
      end
      m_entries[0].sample(cover_index_in - 1);
   endfunction : sample


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_table_cover");
      super.new(name_in);
   endfunction : new

endclass : evl_table_cover

//--------------------------------------------------------------------------------------------------
//
// Class: evl_table_cover_entry
//
// This class is used to create coverage points for lookup tables.  To allow both summarized
// coverage (i.e., percentage of the coverable entries covered) and per-entry table name coverage
// (i.e., to be able to determine which table entries are not covered by something other than an
// obtuse number), two forms of table entries are created.
//
class evl_table_cover_entry #(type CACHE_TYPE = evl_cache, parameter string TABLE_NAME = "unknown");

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static bit m_print_coverage = 1'b0;
   static bit m_print_checked  = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int    m_limit    = 0;
   int    m_accessed = 1;
   string m_name     = "";


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_lookup_table
   //
   covergroup cg_lookup_table (input string name_in, input int limit_in);
      option.per_instance = 1;
      option.name         = name_in;

      coverpoint m_accessed {
         bins m_accessed[] = { [0:limit_in] };
      }
   endgroup : cg_lookup_table


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample
   //
   function void sample(input int index_in = 0);
      if ((index_in >= 0) && (index_in <= m_limit) && (cg_lookup_table != null)) begin
         m_accessed = index_in;
         if ((m_print_coverage == 1'b1) && (m_limit <= 1)) begin
            `evl_log(UVM_MEDIUM, VMOD_COVER, "TableCover", $sformatf("%0s covered", m_name))
         end
         cg_lookup_table.sample();
      end
   endfunction : sample


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string cover_name_in, input int limit_in);
      m_limit = limit_in;
      if (m_limit >= 0) begin
         if (m_print_checked == 1'b0) begin
            m_print_checked = 1'b1;
            if (`et_test_plusargs("printCoverage") != 0) begin
               m_print_coverage = 1'b1;
            end
         end
         if (m_print_coverage == 1'b1) begin
            m_name = cover_name_in;
         end
         cg_lookup_table = new(cover_name_in, limit_in);
      end
   endfunction : new

endclass : evl_table_cover_entry

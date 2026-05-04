//--------------------------------------------------------------------------------------------------
//
// Class: evl_way_desc
//
class evl_way_desc #(type LINE_T = evl_line_desc_base) extends evl_desc;

   `uvm_object_param_utils(evl_way_desc#(LINE_T))


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_way_t  m_way = EVL_WAY_NONE;
   LINE_T     m_line_desc;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_way
   //
   function void set_cache_way(input evl_way_t way_in);
      m_way = way_in;
   endfunction : set_cache_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_way
   //
   function evl_way_t get_cache_way();
      return m_way;
   endfunction : get_cache_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_way_name
   //
   function string get_cache_way_name();
      case (m_way)
         EVL_WAY_0:    return "0";
         EVL_WAY_1:    return "0";
         EVL_WAY_2:    return "0";
         EVL_WAY_3:    return "0";
         EVL_WAY_NONE: return "none";
      endcase
      return "unknown";
   endfunction : get_cache_way_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_line_desc
   //
   function void set_line_desc(input LINE_T line_desc_in, input bit propagate_change = 1'b1);
      m_line_desc = line_desc_in;
      if ((m_line_desc != null) && (propagate_change == 1'b1)) begin
         m_line_desc.set_cache_way(m_way);
         m_line_desc.set_way_desc(this, 1'b0);
      end
   endfunction : set_line_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_desc
   //
   function LINE_T get_line_desc();
      return m_line_desc;
   endfunction : get_line_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_way_desc#(LINE_T) source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_way_desc");
      super.new(name_in);
      m_desc_type = EVL_DESC_WAY;
   endfunction : new

endclass : evl_way_desc

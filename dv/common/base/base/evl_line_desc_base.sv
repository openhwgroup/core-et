//--------------------------------------------------------------------------------------------------
//
// Class: evl_line_desc_base
//
class evl_line_desc_base #(type TAG_T = evl_paddr_t, type LINE_T = evl_line_desc_base) extends evl_desc;

   `uvm_object_param_utils(evl_line_desc_base#(TAG_T, LINE_T))


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_cache_st_t        m_cache_st = I;
   evl_way_t             m_way      = EVL_WAY_NONE;
   TAG_T                 m_tag;
   TAG_T                 m_tag_mask;
   evl_way_desc#(LINE_T) m_way_desc;
   evl_mem_desc          m_cur_mem_desc;
   evl_mem_desc          m_mem_desc_list[$];
   evl_rand_gen          m_rand_gen;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   function void set_rand_gen(input evl_rand_gen rand_gen_in);
      m_rand_gen = rand_gen_in;
   endfunction : set_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_st
   //
   function void set_cache_st(input evl_cache_st_t cache_st_in);
      m_cache_st = cache_st_in;
   endfunction : set_cache_st


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_st
   //
   function evl_cache_st_t get_cache_st();
      return m_cache_st;
   endfunction : get_cache_st


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_data
   //
   virtual function void create_data(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
      return;
   endfunction : create_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_template
   //
   virtual function void set_data_template(input evl_line_data template_line_data_in);
      return;
   endfunction : set_data_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_all_line_data
   //
   virtual function void zero_all_line_data(input evl_line_data template_line_data_in = null);
      return;
   endfunction : zero_all_line_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tag
   //
   virtual function void set_tag(input TAG_T tag_in);
      m_tag = tag_in & m_tag_mask;
   endfunction : set_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag
   //
   virtual function TAG_T get_tag();
      return m_tag;
   endfunction : get_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tag_mask
   //
   function void set_tag_mask(input TAG_T tag_mask_in);
      m_tag_mask = tag_mask_in;
   endfunction : set_tag_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_mask
   //
   function TAG_T get_tag_mask();
      return m_tag_mask;
   endfunction : get_tag_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cur_mem_desc
   //
   virtual function void set_cur_mem_desc(input evl_mem_desc mem_desc_in);
      m_cur_mem_desc = mem_desc_in;
   endfunction : set_cur_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_mem_desc
   //
   virtual function void add_mem_desc(input evl_mem_desc mem_desc_in);
      if (!(mem_desc_in inside { m_mem_desc_list })) begin
         m_mem_desc_list.push_back(mem_desc_in);
      end
   endfunction : add_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: remove_mem_desc
   //
   virtual function void remove_mem_desc(input evl_mem_desc mem_desc_in);
      if (mem_desc_in inside { m_mem_desc_list }) begin
         for (int ii = m_mem_desc_list.size() - 1; ii >= 0; ii--) begin
            if (m_mem_desc_list[ii] == mem_desc_in) begin
               m_mem_desc_list.delete(ii);
            end
         end
      end
      if (m_cur_mem_desc == mem_desc_in) begin
         m_cur_mem_desc = null;
      end
   endfunction : remove_mem_desc


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
      if (m_way_desc != null) begin
         return m_way_desc.get_cache_way_name();
      end
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
   // Function: set_way_desc
   //
   function void set_way_desc(input evl_way_desc#(LINE_T) way_desc_in, input bit propagate_change = 1'b1);
      LINE_T line;

      m_way_desc = way_desc_in;
      if (m_way_desc == null) begin
         set_cache_way(EVL_WAY_NONE);
      end
      else begin
         set_cache_way(m_way_desc.get_cache_way());
         if (propagate_change == 1'b1) begin
            $cast(line, this);
            m_way_desc.set_line_desc(line, 1'b0);
         end
      end
   endfunction : set_way_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_way_desc
   //
   function evl_way_desc#(LINE_T) get_way_desc();
      return m_way_desc;
   endfunction : get_way_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_object
   //
   virtual function void init_object();
      return;
   endfunction : init_object


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   // This function is used to print the contents of the cache line.
   //
   // Input Variables:
   //
   // abstract_name_in: This is the abstract name to use when printing.  If it is "", then the
   //                   abstract name in this object will be used.
   //
   virtual function void print_obj(input string abstract_name_in = "");
      string abstract_name = abstract_name_in;

      if (abstract_name == "") begin
         abstract_name = get_abstract_name();
      end
      `evl_log_addr(UVM_NONE, VMOD_GLOBAL, abstract_name, get_tag(), $sformatf("cache line 0x%x (state %p) (id %0d)", get_tag(), m_cache_st, get_inst_id()))
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj();
      return $sformatf("cache line 0x%x (state %p)", get_tag(), m_cache_st);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_line_desc_base#(TAG_T, LINE_T) source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_line_desc_base");
      super.new(name_in);
      m_desc_type = EVL_DESC_LINE_BASE;
   endfunction : new

endclass : evl_line_desc_base

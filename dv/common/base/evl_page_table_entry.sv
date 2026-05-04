//--------------------------------------------------------------------------------------------------
//
// Class: evl_page_table_entry
//
class evl_page_table_entry extends evl_line_desc_base #(evl_vaddr_t, evl_page_table_entry);

   `uvm_object_utils(evl_page_table_entry)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_pte_attr_t m_attr;
   bit            m_user_mode;
   bit            m_global;
   evl_paddr_t    m_paddr;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_data
   //
   virtual function void create_data(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
   endfunction : create_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_attr
   //
   function evl_pte_attr_t get_attr();
      return m_attr;
   endfunction : get_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_user_mode
   //
   function bit get_user_mode();
      return m_user_mode;
   endfunction : get_user_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_global
   //
   function bit get_global();
      return m_global;
   endfunction : get_global


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_paddr
   //
   function evl_paddr_t get_paddr();
      return m_paddr;
   endfunction : get_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_attr
   //
   // This function is used to set the page table entry's attributes
   //
   // Input Variables:
   //
   // attr_in: This is the new attribute value
   //
   function void set_attr(input evl_pte_attr_t attr_in);
      m_attr = attr_in;
   endfunction : set_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_user_mode
   //
   // This function is used to allow this pte to be accessed by user mode
   //
   function void set_user_mode();
      m_user_mode = 1'b1;
   endfunction : set_user_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_user_mode
   //
   // This function is used to forbid this pte to be accessed by user mode
   //
   function void clr_user_mode();
      m_user_mode = 1'b0;
   endfunction : clr_user_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_global
   //
   function void set_global();
      m_global = 1'b1;
   endfunction : set_global


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_global
   //
   function void clr_global();
      m_global = 1'b0;
   endfunction : clr_global


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_paddr
   //
   // This function is used to set the physical address pointed by this page table entry
   //
   // Input Variables:
   //
   // paddr_in: This is the new physical address value
   //
   function void set_paddr(input evl_paddr_t paddr_in);
      m_paddr = paddr_in & const_page_paddr_mask;
   endfunction : set_paddr


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
      `evl_log_addr(UVM_NONE, VMOD_GLOBAL, abstract_name, get_tag(), $sformatf("PTE VA=0x%x PA=0x%x (state %p) (xwr:%0p - user:%0b - global:%0b)", get_tag(), m_paddr, m_cache_st, m_attr, m_user_mode, m_global))
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to print the contents of the cache line.
   //
   virtual function string sprint_obj();
      return $sformatf("PTE VA=0x%x PA=0x%x (state %p) (xwr:%0p - user:%0b - global:%0b)", get_tag(), m_paddr, m_cache_st, m_attr, m_user_mode, m_global);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   // This function is a redefinition of the UVM do_copy() function.  do_copy() is called by the
   // UVM clone() function.  Note that not all of the information in the descriptor is copied with
   // this function; that is intentional ... only limited information is supposed to be copied when
   // cloning a descriptor.
   //
   // Input Variables:
   //
   // rhs: This is a handle to the descriptor from which to copy information.
   //
   virtual function void do_copy(uvm_object rhs);
      evl_page_table_entry source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_attr      = source_obj.m_attr;
      m_user_mode = source_obj.m_user_mode;
      m_global    = source_obj.m_global;
      m_paddr     = source_obj.m_paddr;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_page_table_entry");
      super.new(name_in);
      m_desc_type = EVL_PTE_DESC;

      m_attr      = pte_pointer;
      m_user_mode = 1'b0;
      m_global    = 1'b0;
      m_paddr     = evl_paddr_t'(0);
   endfunction : new

endclass : evl_page_table_entry


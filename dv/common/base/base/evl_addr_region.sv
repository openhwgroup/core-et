//--------------------------------------------------------------------------------------------------
//
// Class: evl_addr_region
//
class evl_addr_region extends evl_object;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp m_agent;
   string         m_region_name;
   evl_paddr_t    m_addr_base;
   evl_paddr_t    m_addr_top;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_region_name
   //
   function void set_region_name(input string name_in);
      m_region_name = name_in;
   endfunction : set_region_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_region_name
   //
   function string get_region_name();
      return m_region_name;
   endfunction : get_region_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_base
   //
   function void set_addr_base(input evl_paddr_t addr_in);
      m_addr_base = addr_in;
   endfunction : set_addr_base


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_top
   //
   function void set_addr_top(input evl_paddr_t addr_in);
      m_addr_top = addr_in;
   endfunction : set_addr_top


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_base
   //
   function evl_paddr_t get_addr_base();
      return m_addr_base;
   endfunction : get_addr_base


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_top
   //
   function evl_paddr_t get_addr_top();
      return m_addr_top;
   endfunction : get_addr_top


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_region
   //
   function bit addr_in_region(input evl_paddr_t addr_in);
      if ((m_addr_base <= addr_in) && (m_addr_top >= addr_in)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : addr_in_region


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_addr_region", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_region_name = name_in;
      m_agent       = agent_in;
   endfunction : new

endclass : evl_addr_region

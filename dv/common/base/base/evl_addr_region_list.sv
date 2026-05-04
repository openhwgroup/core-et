//--------------------------------------------------------------------------------------------------
//
// Class: evl_addr_region_list
//
class evl_addr_region_list extends evl_object;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp  m_agent;
   string          m_region_name;
   evl_addr_region m_addr_regions[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_region_name
   //
   function void set_region_name(input string name_in);
      m_region_name = name_in;
      foreach (m_addr_regions[ii]) begin
         m_addr_regions[ii].set_region_name(name_in);
      end
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
   // Function: clr_addr_region
   //
   function void clr_addr_region();
      m_addr_regions = {};
   endfunction : clr_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clean_regions
   //
   function void clean_regions();
      if (m_addr_regions.size() > 1) begin
         bit clean = 1'b0;

         m_addr_regions.sort with ( item.m_addr_base );
         while (clean == 1'b0) begin
            clean = 1'b1;
            for (int ii = m_addr_regions.size() - 1; ii > 0; ii--) begin
               if ( (m_addr_regions[ii - 1].addr_in_region(m_addr_regions[ii].get_addr_base()) == 1'b1) ||
                    ((m_addr_regions[ii - 1].get_addr_top() + 1) == m_addr_regions[ii].get_addr_base()) ) begin
                  clean = 1'b0;
                  if (m_addr_regions[ii - 1].addr_in_region(m_addr_regions[ii].get_addr_top()) == 1'b0) begin
                     m_addr_regions[ii - 1].set_addr_top(m_addr_regions[ii].get_addr_top());
                  end
                  m_addr_regions.delete(ii);
               end
               else if (m_addr_regions[ii].addr_in_region(m_addr_regions[ii - 1].get_addr_top()) == 1'b1) begin
                  clean = 1'b0;
                  m_addr_regions[ii - 1].set_addr_top(m_addr_regions[ii].get_addr_top());
                  m_addr_regions.delete(ii);
               end
            end
         end
      end
   endfunction : clean_regions


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_region
   //
   function void add_addr_region(input evl_paddr_t addr_base_in, input evl_paddr_t addr_top_in);
      evl_addr_region addr_region;
      evl_paddr_t     addr_base = (addr_base_in <= addr_top_in) ? addr_base_in : addr_top_in;
      evl_paddr_t     addr_top  = (addr_base_in <= addr_top_in) ? addr_top_in  : addr_base_in;

      foreach (m_addr_regions[ii]) begin
         if (m_addr_regions[ii].addr_in_region(addr_base) == 1'b1) begin
            if (m_addr_regions[ii].addr_in_region(addr_top) == 1'b1) begin
               return;
            end
            m_addr_regions[ii].set_addr_top(addr_top);
            clean_regions();
            return;
         end
         else if (m_addr_regions[ii].addr_in_region(addr_top) == 1'b1) begin
            m_addr_regions[ii].set_addr_base(addr_base);
            clean_regions();
            return;
         end
      end
      addr_region = new(m_region_name, m_agent);
      addr_region.set_addr_base(addr_base);
      addr_region.set_addr_top(addr_top);
      m_addr_regions.push_back(addr_region);
      clean_regions();
   endfunction : add_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_region
   //
   function void set_addr_region(input evl_paddr_t addr_base, input evl_paddr_t addr_top);
      clr_addr_region();
      add_addr_region(addr_base, addr_top);
   endfunction : set_addr_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_in_region
   //
   function bit addr_in_region(input evl_paddr_t addr_in);
      foreach (m_addr_regions[ii]) begin
         if (m_addr_regions[ii].addr_in_region(addr_in) == 1'b1) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : addr_in_region


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_addr_region_list", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_region_name = name_in;
      m_agent       = agent_in;
   endfunction : new

endclass : evl_addr_region_list

//==================================================================================================
//
// Class: evl_bus_watch
//
class evl_bus_watch extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_paddr_t      m_addr;
   evl_bus_req_id_t m_req_id;
   evl_bus_req_id_t m_req_id_mask;
   evl_verif_comp   m_agent;
   int              m_source;
   event            m_match;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr
   //
   function evl_paddr_t get_addr();
      return m_addr;
   endfunction : get_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_id
   //
   function evl_bus_req_id_t get_req_id();
      return m_req_id;
   endfunction : get_req_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_watch_params
   //
   function void set_watch_params(input evl_paddr_t      addr,
                                  input evl_bus_req_id_t req_id,
                                  input evl_bus_req_id_t req_id_mask,
                                  input evl_verif_comp   agent);
      m_addr        = addr;
      m_req_id      = req_id;
      m_req_id_mask = req_id_mask;
      m_agent       = agent;
      m_source      = agent.get_inst_id();
   endfunction : set_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_watch_params
   //
   function bit have_watch_params(input evl_paddr_t      addr,
                                  input evl_bus_req_id_t req_id,
                                  input evl_bus_req_id_t req_id_mask,
                                  input evl_verif_comp   agent);
      if ((m_addr == addr) && (m_req_id == req_id) && (m_req_id_mask == req_id_mask) && (m_agent == agent)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : have_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_match
   //
   task wait_for_match();
      #0 @(m_match);
   endtask : wait_for_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_for_match
   //
   function void check_for_match(input evl_paddr_t      addr,
                                 input evl_bus_req_id_t req_id,
                                 input int              source);
      if ((m_source == source) && (m_addr == addr) && (((m_req_id ^ req_id) & m_req_id_mask) == 0)) begin
         m_req_id = req_id;
         -> m_match;
      end
   endfunction : check_for_match


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      m_addr        = ~evl_paddr_t'(0);
      m_req_id      = ~evl_bus_req_id_t'(0);
      m_req_id_mask = evl_bus_req_id_t'(0);
      m_agent       = null;
      m_source      = -1;
   endfunction : new

endclass : evl_bus_watch

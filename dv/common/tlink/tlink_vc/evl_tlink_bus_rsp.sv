//-----------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_bus_rsp
//
// This class object is simply a container for passing response information between the Tile Link
// interface and the Tile Link VC components.
//
class evl_tlink_bus_rsp;

   evl_delay_t         m_mem_bus_rsp_delay;
   int                 m_rsp_cycle;
   evl_tlink_bus_rsp_s m_bus_rsp_info;


   //--------------------------------------------------------------------------------------------------
   function set_bus_rsp_info(evl_tlink_bus_rsp_s bus_rsp_in);
      m_bus_rsp_info = bus_rsp_in;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function evl_tlink_bus_rsp_s get_bus_rsp_info();
      return m_bus_rsp_info;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function set_mem_bus_rsp_delay(evl_delay_t delay_in);
      m_mem_bus_rsp_delay = delay_in;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function evl_delay_t get_mem_bus_rsp_delay();
      return m_mem_bus_rsp_delay;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function void set_rsp_cycle(input int cycle_in);
      m_rsp_cycle = cycle_in;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function int get_rsp_cycle();
      return m_rsp_cycle;
   endfunction : get_rsp_cycle


   //--------------------------------------------------------------------------------------------------
   function new();
      m_bus_rsp_info = '{ opcode    : evl_tlink_rsp_opcode_t'(0),
                          param     : evl_tlink_param_t'(0),
                          data_size : evl_bus_req_size_t'(0),
                          source    : evl_bus_req_id_t'(0),
                          sink      : evl_bus_req_id_t'(0),
                          data      : evl_xword_t'(0),
                          denied    : 1'b0,
                          corrupt   : 1'b0
                        };
   endfunction

endclass

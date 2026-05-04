//-----------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_bus_req
//
// This class object is simply a container for passing request information between the ET link
// interface and the ET link VC components.
//
class evl_etlink_bus_req;

   evl_delay_t          m_bus_req_delay;
   evl_delay_t          m_bus_req_data_delay;
   int                  m_req_cycle;
   bit                  m_continue;
   evl_etlink_bus_req_s m_bus_req_info;


   //--------------------------------------------------------------------------------------------------
   //
   // Function: clone
   //
   // This function is used to create an exact copy of this object.
   //
   function evl_etlink_bus_req clone();
      evl_etlink_bus_req bus_req;

      bus_req = new();
      bus_req.m_bus_req_delay      = m_bus_req_delay;
      bus_req.m_bus_req_data_delay = m_bus_req_data_delay;
      bus_req.m_req_cycle          = m_req_cycle;
      bus_req.m_continue           = m_continue;
      bus_req.m_bus_req_info       = m_bus_req_info;
      return bus_req;
   endfunction : clone


   //--------------------------------------------------------------------------------------------------
   function void set_trans_id(evl_trans_id_t trans_id_in);
      m_bus_req_info.trans_id = trans_id_in;
   endfunction : set_trans_id


   //--------------------------------------------------------------------------------------------------
   function evl_trans_id_t get_trans_id();
      return m_bus_req_info.trans_id;
   endfunction : get_trans_id


   //--------------------------------------------------------------------------------------------------
   function void set_bus_req_info(evl_etlink_bus_req_s bus_req_in);
      m_bus_req_info = bus_req_in;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function evl_etlink_bus_req_s get_bus_req_info();
      return m_bus_req_info;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function void set_bus_req_delay(evl_delay_t delay_in);
      if (delay_in < 0) begin
         m_bus_req_delay = 0;
      end
      else begin
         m_bus_req_delay = delay_in;
      end
   endfunction


   //--------------------------------------------------------------------------------------------------
   function evl_delay_t get_bus_req_delay();
      return m_bus_req_delay;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function void set_bus_req_data_delay(evl_delay_t delay_in);
      if (delay_in < 0) begin
         m_bus_req_data_delay = 0;
      end
      else begin
         m_bus_req_data_delay = delay_in;
      end
   endfunction


   //--------------------------------------------------------------------------------------------------
   function evl_delay_t get_bus_req_data_delay();
      return m_bus_req_data_delay;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function void set_req_cycle(input int cycle_in);
      m_req_cycle = cycle_in;
   endfunction


   //--------------------------------------------------------------------------------------------------
   function int get_req_cycle();
      return m_req_cycle;
   endfunction : get_req_cycle


   //--------------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      m_req_cycle          = 0;
      m_bus_req_delay      = 0;
      m_bus_req_data_delay = 0;
      m_bus_req_info       = '{ bank      : 0,
                                trans_id  : 0,
                                id        : 0,
                                source    : evl_bus_req_source_t'(0),
                                opcode    : evl_etlink_req_opcode_t'(0),
                                cached    : 0,
                                address   : 0,
                                data      : 0,
                                wdata     : 0,
                                data_size : 0,
                                qwen      : 0,
                                subopcode : evl_bus_subopcode_t'(0)
                              };
   endfunction

endclass

class evl_etlink_bus_rsp;

   evl_delay_t          m_mem_bus_rsp_delay;
   int                  m_rsp_cycle;
   evl_etlink_bus_rsp_s m_bus_rsp_info;


   function set_bus_rsp_info(evl_etlink_bus_rsp_s bus_rsp_in);
      m_bus_rsp_info = bus_rsp_in;
   endfunction

   function evl_etlink_bus_rsp_s get_bus_rsp_info();
      return m_bus_rsp_info;
   endfunction

   function set_mem_bus_rsp_delay(evl_delay_t delay_in);
      m_mem_bus_rsp_delay = delay_in;
   endfunction

   function evl_delay_t get_mem_bus_rsp_delay();
      return m_mem_bus_rsp_delay;
   endfunction

   function void set_rsp_cycle(input int cycle_in);
      m_rsp_cycle = cycle_in;
   endfunction

   function int get_rsp_cycle();
      return m_rsp_cycle;
   endfunction : get_rsp_cycle

   function new();
      m_bus_rsp_info = '{ id        : 0,
                          dest      : evl_bus_rsp_dest_t'(0),
                          wdata     : 0,
                          opcode    : evl_etlink_rsp_opcode_t'(0),
                          data      : 0,
                          data_size : evl_etlink_size_t'(0),
                          qwen      : 0,
                          cached    : 2'b00
                        };
   endfunction

endclass

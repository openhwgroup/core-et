class evl_dbg_noc_bus_req;

   static int m_global_inst_id = 1;

   int                  m_inst_id;

   // m_bus_req_delay:      the delay before req is placed onto the wires
   evl_delay_t          m_bus_req_delay;
   evl_trans_id_t       m_trans_id;    // dv-related-ID

   // signals for req
   evl_paddr_t          m_req_addr;
   evl_bus_qos_t        m_req_qos;
   bit                  m_req_eop;
   evl_xword_t          m_req_data;
   evl_bus_req_vc_t     m_req_dest_int = ~evl_bus_req_vc_t'(0);

   int                  m_cycle;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_inst_id
   //
   function int get_inst_id();
      return m_inst_id;
   endfunction : get_inst_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //    This function is used to set the trans_id of this bus request
   //
   // Input Variables:
   //    trans_id_in: This is the trans_id of the request
   //
   // Return Value:
   //    none
   //
   function void set_trans_id(input evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //    This function is used to get the trans_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_trans_id_t: This is the trans_id of the request
   //
   function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id



   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_delay
   //    This function is used to set the bus request (i.e. AW or AR channel) delay of this request
   //
   // Input Variables:
   //    delay_in: This is the bus delay of the request
   //
   // Return Value:
   //    none
   //
   function void set_bus_req_delay(input evl_delay_t delay_in);
      if (delay_in < 0) begin
         m_bus_req_delay = 0;
      end
      else begin
         m_bus_req_delay = delay_in;
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_delay
   //    This function is used to get the bus request (i.e. AW or AR channel) delay of this request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_delay_t: This is the bus delay of the request
   //
   function evl_delay_t get_bus_req_delay();
      return m_bus_req_delay;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_addr
   //    This function is used to set the req_addr of this bus request
   //
   // Input Variables:
   //    req_addr_in: This is the ADDR of the request
   //
   // Return Value:
   //    none
   //
   function void set_req_addr(input evl_paddr_t req_addr_in);
      m_req_addr = req_addr_in;
   endfunction : set_req_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_addr
   //    This function is used to get the req_addr of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_paddr_t: This is the ADDR of the request
   //
   function evl_paddr_t get_req_addr();
      return m_req_addr;
   endfunction : get_req_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_qos
   //    This function is used to set the req_qos of this bus request
   //
   // Input Variables:
   //    req_qos_in: This is the QOS of the request
   //
   // Return Value:
   //    none
   //
   function void set_req_qos(input evl_bus_qos_t req_qos_in);
      m_req_qos = req_qos_in;
   endfunction : set_req_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_qos
   //    This function is used to get the req_qos of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_qos_t: This is the QOS of the request
   //
   function evl_bus_qos_t get_req_qos();
      return m_req_qos;
   endfunction : get_req_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_dest_int
   //    This function is used to set the req_dest_int field of this bus request
   //
   // Input Variables:
   //    req_dest_int_in: This is the virtual channel for this request
   //
   // Return Value:
   //    none
   //
   function void set_req_dest_int(input evl_bus_req_vc_t req_dest_int_in);
      m_req_dest_int = req_dest_int_in;
   endfunction : set_req_dest_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_dest_int
   //    This function is used to get the req_dest_int field of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_vc_t: This is the virtual channel for this request
   //
   function evl_bus_req_vc_t get_req_dest_int();
      return m_req_dest_int;
   endfunction : get_req_dest_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_data
   //    This function is used to set the req_data of this bus request
   //
   // Input Variables:
   //    req_data_in: This is the DATA of the request
   //
   // Return Value:
   //    none
   //
   function void set_req_data(input evl_xword_t req_data_in);
      m_req_data = req_data_in;
   endfunction : set_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_data
   //    This function is used to get the req_data of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_xword_t: This is the DATA of the request
   //
   function evl_xword_t get_req_data();
      return m_req_data;
   endfunction : get_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_eop
   //    This function is used to BLAH.
   //
   // Return Value:
   //    none
   //
   function void set_req_eop();
      m_req_eop = 1'b1;
   endfunction : set_req_eop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_eop
   //    This function is used to BLAH.
   //
   // Return Value:
   //    bit: BLAH
   //
   function bit get_req_eop();
      return m_req_eop;
   endfunction : get_req_eop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cycle
   //    This function is used to set the cycle of this bus request
   //
   // Input Variables:
   //     cycle_in: This is the cycle of the request
   //
   // Return Value:
   //    none
   //
   function void set_cycle(input int cycle_in);
      m_cycle = cycle_in;
   endfunction : set_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle
   //    This function is used to get the cycle of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    int: This is the cycle of the request
   //
   function int get_cycle();
      return m_cycle;
   endfunction : get_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      m_inst_id = m_global_inst_id++;
   endfunction : new

endclass


class evl_apb_bus_req;

   evl_apb_req_s  m_bus_req;
   evl_trans_id_t m_trans_id;


   //-----------------------------------------------------------------------------------------------
   function void set_valid();
      m_bus_req.valid = 1'b1;
   endfunction : set_valid


   //-----------------------------------------------------------------------------------------------
   function void set_trans_id(input evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id


   //-----------------------------------------------------------------------------------------------
   function void set_addr(input evl_paddr_t addr_in);
      m_bus_req.paddr = addr_in;
   endfunction : set_addr


   //-----------------------------------------------------------------------------------------------
   function evl_paddr_t get_addr();
      return m_bus_req.paddr;
   endfunction : get_addr


   //-----------------------------------------------------------------------------------------------
   function void set_write(input bit write_in);
      m_bus_req.pwrite = write_in;
   endfunction : set_write


   //-----------------------------------------------------------------------------------------------
   function bit get_write();
      return m_bus_req.pwrite;
   endfunction : get_write


   //-----------------------------------------------------------------------------------------------
   function void set_wdata(input evl_dword_t data_in);
      m_bus_req.pwdata = data_in;
   endfunction : set_wdata


   //-----------------------------------------------------------------------------------------------
   function evl_dword_t get_wdata();
      return m_bus_req.pwdata;
   endfunction : get_wdata

endclass : evl_apb_bus_req


class evl_apb_bus_rsp;

   evl_apb_rsp_s m_bus_rsp;


   //-----------------------------------------------------------------------------------------------
   function void set_valid();
      m_bus_rsp.valid = 1'b1;
   endfunction : set_valid


   //-----------------------------------------------------------------------------------------------
   function void set_rdata(input evl_dword_t data_in);
      m_bus_rsp.prdata = data_in;
   endfunction : set_rdata


   //-----------------------------------------------------------------------------------------------
   function evl_dword_t get_rdata();
      return m_bus_rsp.prdata;
   endfunction : get_rdata


   //-----------------------------------------------------------------------------------------------
   function void set_slv_err(input bit slverr_in);
      m_bus_rsp.pslverr = slverr_in;
   endfunction : set_slv_err


   //-----------------------------------------------------------------------------------------------
   function bit get_slv_err();
      return m_bus_rsp.pslverr;
   endfunction : get_slv_err

endclass : evl_apb_bus_rsp

class evl_axi_cov_base extends uvm_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar
   //
   virtual function void sample_ar(input evl_paddr_t      ar_addr_in,
                                   input evl_bus_req_id_t ar_id_in,
                                   input bit [2:0]        ar_size_in,
                                   input bit [3:0]        ar_qos_in,
                                   input int              ar_idle_count_in,
                                   input int              ar_busy_count_in);
      return;
   endfunction : sample_ar


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw
   //
   virtual function void sample_aw(input evl_paddr_t        aw_addr_in,
                                   input evl_bus_req_id_t   aw_id_in,
                                   input bit [2:0]          aw_size_in,
                                   input bit [3:0]          aw_qos_in,
                                   input evl_bus_req_user_t aw_user_in,
                                   input int                aw_idle_count_in,
                                   input int                aw_busy_count_in);
      return;
   endfunction : sample_aw


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w
   //
   virtual function void sample_w(input bit [63:0]  w_strb_in,
                                  input bit [511:0] w_data_in,
                                  input int         w_idle_count_in,
                                  input int         w_busy_count_in);
      return;
   endfunction : sample_w


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r
   //
   virtual function void sample_r(input bit [1:0]   r_resp_in,
                                  input bit [511:0] r_data_in,
                                  input int         r_idle_count_in,
                                  input int         r_busy_count_in);
      return;
   endfunction : sample_r


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b
   //
   virtual function void sample_b(input bit [1:0] b_resp_in,
                                  input int       b_idle_count_in,
                                  input int       b_busy_count_in);
      return;
   endfunction : sample_b


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_delay
   //
   virtual function void sample_ar_delay(input bit [31:0] delay_in);
      return;
   endfunction : sample_ar_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_delay
   //
   virtual function void sample_aw_delay(input bit [31:0] delay_in);
      return;
   endfunction : sample_aw_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w_delay
   //
   virtual function void sample_w_delay(input bit [31:0] delay_in);
      return;
   endfunction : sample_w_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r_delay
   //
   virtual function void sample_r_delay(input bit [31:0] delay_in, input bit out_of_order_in);
      return;
   endfunction : sample_r_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b_delay
   //
   virtual function void sample_b_delay(input bit [31:0] delay_in, input bit out_of_order_in);
      return;
   endfunction : sample_b_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_req
   //
   virtual function void sample_concurrent_req(input bit [2:0] value_in);
      return;
   endfunction : sample_concurrent_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_rsp
   //
   virtual function void sample_concurrent_rsp(input bit [1:0] value_in);
      return;
   endfunction : sample_concurrent_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_active_count
   //
   virtual function void sample_ar_active_count(input int value_in);
      return;
   endfunction : sample_ar_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_active_count
   //
   virtual function void sample_aw_active_count(input int value_in);
      return;
   endfunction : sample_aw_active_count

   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_vs_w
   //
   virtual function void sample_aw_vs_w(input bit count_aw_w_in, input int count_in);
      return;
   endfunction: sample_aw_vs_w
   
endclass : evl_axi_cov_base

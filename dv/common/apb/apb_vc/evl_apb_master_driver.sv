//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_master_driver
//
class evl_apb_master_driver extends evl_push_driver #(evl_req_desc);


  `uvm_component_utils(evl_apb_master_driver)

   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   `evl_param_key(pk_bus_req_delay)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_apb_rtl_port m_apb_rtl_port;
   int              m_bus_data_width = `EVL_DWORD_BYTES;
   int              m_bus_data_width_log2;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: put
   //
   virtual task put(input evl_req_desc item);
      drive_request(item);
   endtask : put


   //-----------------------------------------------------------------------------------------------
   //
   // Function: drive_request
   //
   // This function splits up the transaction into bus cycles and sends those to the interface
   // module.
   //
   task drive_request(input evl_req_desc req_desc);
      evl_apb_bus_req bus_req;
      evl_delay_t     request_delay;

      bus_req = new();
      bus_req.set_trans_id(req_desc.get_trans_id());
      bus_req.set_addr(req_desc.get_paddr());

      if (req_desc.bus_cmd_is_write() == 1'b1) begin
         evl_line_data line_data;

         line_data = req_desc.get_req_data();
         if (line_data == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("TB_ISSUE: drive_request(): line_data is null (%0s, line %0d)", `__FILE__, `__LINE__))
         end
         else if (m_bus_data_width == 4) begin
            bus_req.set_wdata(evl_dword_t'(line_data.get_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB)));
         end
         else begin
            bus_req.set_wdata(line_data.get_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB));
         end
         bus_req.set_write(1'b1);
      end

      // Get any request delay
      void'(req_desc.get_delay(request_delay, pk_bus_req_delay));
//    bus_req.set_bus_req_delay(request_delay);

      m_apb_rtl_port.send_req(bus_req);
   endtask : drive_request


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_apb_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
      if (m_rtl_port != null) begin
         m_bus_data_width = m_rtl_port.get_bus_data_width();
      end
      m_bus_data_width_log2 = $clog2(m_bus_data_width);
      m_apb_rtl_port.zero_req();
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_apb_master_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_apb_master_driver

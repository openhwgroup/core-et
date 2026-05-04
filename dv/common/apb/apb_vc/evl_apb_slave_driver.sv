//--------------------------------------------------------------------------------------------------
//
// Class: evl_apb_slave_driver
//
class evl_apb_slave_driver extends evl_push_driver #(evl_req_desc);

  `uvm_component_utils(evl_apb_slave_driver)


   `evl_param_key(pk_mem_bus_rsp_delay)


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
      drive_response(item);
   endtask : put


   //-----------------------------------------------------------------------------------------------
   //
   // Function: drive_response
   //
   // This function splits up the transaction into bus cycles and sends those to the interface
   // module.
   //
   function void drive_response(input evl_req_desc req_desc);
      evl_apb_bus_rsp bus_rsp;
      evl_delay_t     rsp_delay;

      bus_rsp = new();

      // FUTURE if we ever use this
      bus_rsp.set_slv_err(0);

      if (req_desc.bus_cmd_is_read() == 1'b1) begin
         evl_line_data line_data;

         line_data = req_desc.get_rsp_data();
         if (line_data == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("TB_ISSUE: drive_response(): line_data is null (%0s, line %0d)", `__FILE__, `__LINE__))
         end
         else if (m_bus_data_width == 4) begin
            bus_rsp.set_rdata(evl_dword_t'(line_data.get_word(req_desc.get_paddr() >> `EVL_WORD_ADDR_LSB)));
         end
         else begin
            bus_rsp.set_rdata(line_data.get_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB));
         end
      end

      // Get any response delay
      void'(req_desc.get_delay(rsp_delay, pk_mem_bus_rsp_delay));
//    bus_rsp.set_mem_bus_rsp_delay(rsp_delay);

      m_apb_rtl_port.send_rsp(bus_rsp);
   endfunction : drive_response


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
      m_apb_rtl_port.zero_rsp();
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_apb_slave_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_apb_slave_driver

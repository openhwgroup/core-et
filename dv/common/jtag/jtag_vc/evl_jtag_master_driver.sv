//--------------------------------------------------------------------------------------------------
//
// Class: evl_jtag_master_driver
//
class evl_jtag_master_driver extends evl_push_driver #(evl_req_desc);


   `uvm_component_utils(evl_jtag_master_driver)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_jtag_rtl_port   m_jtag_rtl_port;
   evl_jtag_bus_parser m_bus_parser = evl_jtag_bus_parser::get_inst();


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
   function void drive_request(input evl_req_desc req_desc);
      evl_jtag_req_info_t bus_req;

      bus_req.cmd = evl_cmd_t'(req_desc.get_bus_cmd());
      if (bus_req.count >= `EVL_MAX_JTAG_LENGTH) begin
         bus_req.count = `EVL_MAX_JTAG_LENGTH;
      end
      else begin
         bus_req.count = evl_jtag_length_t'(req_desc.get_paddr());
      end
      bus_req.data = evl_jtag_data_t'(req_desc.get_req_xword(0));
      m_jtag_rtl_port.put_jtag_req(bus_req);
      req_desc.set_issued();
   endfunction : drive_request


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_jtag_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_jtag_master_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_jtag_master_driver

//--------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_slave_driver
//
class evl_tlink_slave_driver extends evl_push_driver #(evl_req_desc);


  `uvm_component_utils(evl_tlink_slave_driver)

   `evl_param_key(pk_mem_bus_rsp_delay)

   evl_tlink_rtl_port m_tlink_rtl_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: put
   //
   virtual task put(input evl_req_desc item);

      drive_response(item);

   endtask // put


   //-----------------------------------------------------------------------------------------------
   //
   // Function: drive_response
   // This function splits up the transaction into bus cycles
   // and sends those to the interface module.
   //
   function void drive_response(input evl_req_desc req_desc);
//    evl_tlink_bus_rsp      bus_rsp;
//    evl_tlink_bus_rsp_s    bus_rsp_info;
//    evl_tlink_rsp_opcode_t rsp_opcode;
//    bit                     has_data;
//    evl_delay_t             response_delay;
//    bit                     success;

//    bus_rsp = new();

//    rsp_opcode = evl_tlink_rsp_opcode_t'(req_desc.get_bus_rsp_type());
//    `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("Going to drive this %0p", rsp_opcode))
//    has_data = (rsp_opcode inside { ET_LINK_RSP_AckData, ET_LINK_RSP_MsgRcvData }) ? 1'b1 : 1'b0;

//    bus_rsp_info.id     = req_desc.get_bus_rsp_id();
//    bus_rsp_info.dest   = req_desc.get_bus_rsp_dest();
//    bus_rsp_info.opcode = rsp_opcode;
//    if (has_data) begin
//       evl_line_data line_data;

//       line_data = req_desc.get_rsp_data();
//       if (line_data == null) begin
//          `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("drive_response(): line_data is null"))
//       end
//       else begin
//          bus_rsp_info.data  = line_data.get_xword(0);
//          bus_rsp_info.wdata = 1;
//          bus_rsp_info.qwen = line_data.get_qword_en();
//       end
//    end
//    bus_rsp_info.data_size = req_desc.get_bus_rsp_size();

//    //Get any request delay
//    success = req_desc.get_delay(response_delay, pk_mem_bus_rsp_delay);
//    bus_rsp.set_mem_bus_rsp_delay(response_delay);
//    bus_rsp.set_bus_rsp_info(bus_rsp_info);

//    m_tlink_rtl_port.put_rsp(bus_rsp);
      return;
   endfunction // drive_response


   virtual task pre_reset_phase(uvm_phase phase);
//    m_tlink_rtl_port.set_rsp_drive();
      super.pre_reset_phase(phase);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tlink_slave_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_tlink_slave_driver


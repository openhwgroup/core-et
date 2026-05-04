//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_master_driver
//
class evl_axi_master_driver extends evl_push_driver #(evl_req_desc);


  `uvm_component_utils(evl_axi_master_driver)

   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_bus_req_data_delay)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_axi_rtl_port_base m_axi_rtl_port;
   evl_axi_bus_parser    m_bus_parser = evl_axi_bus_parser::get_inst();


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
      evl_axi_bus_req bus_req;
      evl_delay_t     request_delay;
      evl_delay_t     request_data_delay;

      bus_req = new();
      bus_req.set_req_type(evl_axi_req_type_t'(req_desc.get_bus_cmd()));
      bus_req.set_trans_id(req_desc.get_trans_id());
      bus_req.set_ax_id(req_desc.get_bus_req_id());
      bus_req.set_ax_addr(req_desc.get_paddr());
      bus_req.set_ax_len(req_desc.get_bus_req_len());
      bus_req.set_ax_size(req_desc.get_bus_req_size());
      bus_req.set_ax_burst(1); // increment for now (FUTURE)
      bus_req.set_ax_cache(req_desc.get_bus_cache_attr());
      bus_req.set_ax_prot(req_desc.get_bus_prot());
      bus_req.set_ax_qos(req_desc.get_bus_qos());
      bus_req.set_ax_user(req_desc.get_bus_req_user());
      bus_req.set_ax_lock(req_desc.get_bus_lock());
      bus_req.set_ax_vc(req_desc.get_vc());

      if (req_desc.bus_cmd_has_write_data() == 1'b1) begin
         evl_line_data line_data;

         line_data = req_desc.get_req_data();
         if (line_data == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("TB_ISSUE: drive_request(): line_data is null (%0s, line %0d)", `__FILE__, `__LINE__))
         end
         else begin
            bus_req.set_w_data(line_data.get_xword(0));
            bus_req.set_w_strb(line_data.get_xword_valids(0));
         end

      end

      // Get any request delay
      void'(req_desc.get_delay(request_delay, pk_bus_req_delay));
      void'(req_desc.get_delay(request_data_delay, pk_bus_req_data_delay));

      bus_req.set_bus_req_delay(request_delay);
      bus_req.set_bus_req_data_delay(request_data_delay);

      `evl_log_mdesc(UVM_HIGH, VMOD_AXI_DRV, get_abstract_name(), req_desc, $sformatf("Driving %0s (delays: %0d, %0d)", req_desc.sprint_obj(), request_delay, request_data_delay))

      m_axi_rtl_port.put_req(bus_req);
   endfunction : drive_request


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_axi_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
      if (m_axi_rtl_port == null) begin
         return;
      end
      m_axi_rtl_port.init_master();
   endtask : pre_reset_phase


// //-----------------------------------------------------------------------------------------------
// virtual task post_reset_phase(uvm_phase phase);
//    super.post_reset_phase(phase);
//    if (m_axi_rtl_port == null) begin
//       return;
//    end
//    m_axi_rtl_port.enable_master();
// endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_axi_master_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_axi_master_driver

//--------------------------------------------------------------------------------------------------
//
// Class: evl_axi_slave_driver
//
class evl_axi_slave_driver extends evl_push_driver #(evl_req_desc);

  `uvm_component_utils(evl_axi_slave_driver)


   `evl_param_key(pk_mem_bus_rsp_delay)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_axi_rtl_port_base m_axi_rtl_port;
   evl_paddr_t           m_data_addr_mask;
   int                   m_data_width;
   int                   m_data_width_clog2;


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
      evl_axi_bus_rsp bus_rsp;
      evl_delay_t     response_delay;
      int             limit;
      evl_paddr_t     cur_addr;

      if (req_desc.bus_cmd_is_read() == 1'b0) begin
         limit = 0;
      end
      else begin
         //
         // Based on the bus width and the request length, set valids in the sending request descriptor
         // appropriately.
         //
         evl_line_data rsp_data = req_desc.get_rsp_data();

         limit    = req_desc.get_bus_req_len();
         cur_addr = req_desc.get_paddr();
         if (((limit + 1) * m_data_width) < `EVL_XWORD_BYTES) begin
            int cur_index;

            rsp_data.invalidate_line();
            cur_index = int'(cur_addr & evl_paddr_t'(`EVL_XWORD_BYTES - 1));
            case (m_data_width)
               `EVL_HWORD_BYTES: cur_index = cur_index >> 1;
               `EVL_WORD_BYTES:  cur_index = cur_index >> 2;
               `EVL_DWORD_BYTES: cur_index = cur_index >> 3;
               `EVL_QWORD_BYTES: cur_index = cur_index >> 4;
               `EVL_OWORD_BYTES: cur_index = cur_index >> 5;
            endcase
            for (int ii = 0; ii <= limit; ii++) begin
               case (m_data_width)
                  1:                rsp_data.set_byte(cur_index, rsp_data.get_byte(cur_index));
                  `EVL_HWORD_BYTES: rsp_data.set_hword(cur_index, rsp_data.get_hword(cur_index));
                  `EVL_WORD_BYTES:  rsp_data.set_word(cur_index, rsp_data.get_word(cur_index));
                  `EVL_DWORD_BYTES: rsp_data.set_dword(cur_index, rsp_data.get_dword(cur_index));
                  `EVL_QWORD_BYTES: rsp_data.set_qword(cur_index, rsp_data.get_qword(cur_index));
                  `EVL_OWORD_BYTES: rsp_data.set_oword(cur_index, rsp_data.get_oword(cur_index));
               endcase
            end
         end
      end

      // Get the response delay
      void'(req_desc.get_delay(response_delay, pk_mem_bus_rsp_delay));

      for (int ii = 0; ii <= limit; ii++) begin
         bus_rsp = new();
         bus_rsp.set_x_id(req_desc.get_bus_req_id());
         bus_rsp.set_x_resp(req_desc.get_bus_rsp_type());
         bus_rsp.set_x_user(req_desc.get_bus_rsp_user());
         bus_rsp.set_mem_bus_rsp_delay(response_delay);
         if (req_desc.bus_cmd_is_read() == 1'b0) begin
            bus_rsp.set_rsp_for_write();
            bus_rsp.set_x_last();
         end
         else begin
            evl_line_data line_data;

            line_data = req_desc.get_rsp_data(req_desc.get_line_burst_idx(cur_addr));
            if (line_data == null) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("TB_ISSUE: drive_response(): line_data is null (%0s, line %0d)", `__FILE__, `__LINE__))
               bus_rsp.set_x_last();
            end
            else begin
               bus_rsp.set_x_data(line_data.get_xword(0) >> ((cur_addr & m_data_addr_mask) << 3));
               cur_addr += m_data_width;
               if (ii == limit) begin
                  bus_rsp.set_x_last();
               end
            end
         end

         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_DRV, get_abstract_name(), req_desc, $sformatf("Driving response %0d for %0s (delay: %0d)", ii, req_desc.sprint_obj(), response_delay))
         m_axi_rtl_port.put_rsp(bus_rsp);
      end
   endfunction : drive_response


   //-----------------------------------------------------------------------------------------------
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if (m_rtl_port == null) begin
         m_data_width = 64;
      end
      else begin
         m_data_width = m_rtl_port.get_int_param_by_name("pk_data_width", 64);
      end
      m_data_width_clog2 = $clog2(m_data_width);
      m_data_addr_mask   = ~((evl_paddr_t'(1) << m_data_width_clog2) - 1) & (`EVL_LINE_BYTES - 1);
   endfunction : start_of_simulation_phase


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
      m_axi_rtl_port.init_slave();
   endtask : pre_reset_phase


// //-----------------------------------------------------------------------------------------------
// virtual task post_reset_phase(uvm_phase phase);
//    super.post_reset_phase(phase);
//    if (m_axi_rtl_port == null) begin
//       return;
//    end
//    m_axi_rtl_port.enable_slave();
// endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_axi_slave_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_axi_slave_driver

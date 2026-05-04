//--------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_master_driver
//
class evl_etlink_master_driver extends evl_push_driver #(evl_req_desc);


  `uvm_component_utils(evl_etlink_master_driver)

   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_bus_req_data_gap_delay)
   `evl_param_key(pk_wdata_bytes)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_etlink_rtl_port      m_etlink_rtl_port;
   evl_etlink_bus_parser    m_bus_parser = evl_etlink_bus_parser::get_inst();
   int                      m_data_width;
   int                      m_data_width_clog2;


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
   // This function splits up the transaction into bus cycles
   // and sends those to the interface module.
   //
   function void drive_request(input evl_req_desc req_desc);
      evl_etlink_bus_req      bus_req;
      evl_etlink_bus_req_s    bus_req_info;
      evl_etlink_req_opcode_t req_opcode;
      bit                     has_write_data;
      evl_delay_t             delay;
      bit                     success;

      req_opcode = req_desc.get_bus_cmd();

      // What the driver really wants to know here is "does this have line_data". For cacheops the answer is no
      // because we have fields in the req_desc for stuff that's driven on the data bus.
      has_write_data = ((req_desc.bus_cmd_has_write_data() == 1'b1) && (req_desc.bus_cmd_is_cache_op() == 1'b0)) ? 1'b1 : 1'b0;

      bus_req = new();

      bus_req_info.id      = req_desc.get_bus_req_id();
      bus_req_info.source  = req_desc.get_bus_req_source();
      bus_req_info.opcode  = req_desc.get_bus_cmd();
      bus_req_info.cached  = req_desc.get_bus_cache_attr();
      bus_req_info.address = req_desc.get_paddr();

      if (has_write_data == 1'b1) begin
         evl_line_data line_data;

         line_data = req_desc.get_req_data();
         if (line_data == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("drive_request(): line_data is null"))
         end
         else begin
            `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_DRV, get_abstract_name(), req_desc, $sformatf("Going to drive %0s", req_desc.sprint_obj()))
            bus_req_info.data = line_data.get_xword(0);
         end
      end // if (has_write_data)

      if (req_opcode == ET_LINK_REQ_Atomic) begin
         bus_req_info.subopcode = evl_bus_atomic_conf_t'(req_desc.get_bus_atomic_conf());
      end
      else begin
         bus_req_info.subopcode = req_desc.get_bus_subopcode_dest();
      end

      if (req_opcode inside { ET_LINK_REQ_Write }) begin
         bus_req_info.data_size = req_desc.get_bus_req_size();
         bus_req_info.qwen      = get_qword_data_mask(req_desc.get_req_data());
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_Read }) begin
         bus_req_info.data_size = req_desc.get_bus_req_size();
         bus_req_info.qwen      = 4'b0000;
         bus_req_info.wdata     = 1'b0;
      end
      else if (req_opcode inside { ET_LINK_REQ_WriteAround }) begin
         bus_req_info.data_size = req_desc.get_bus_req_size();
         bus_req_info.qwen      = get_qword_data_mask(req_desc.get_req_data());
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_ReadCoop }) begin
         bus_req_info.data_size  = req_desc.get_bus_req_size();
         bus_req_info.data       = evl_xword_t'(0);
         bus_req_info.data[23:0] = { req_desc.get_bus_neigh_coop_mask(), req_desc.get_bus_coop_id(), req_desc.get_bus_minion_coop_mask() };
         bus_req_info.qwen       = 4'b0001;
         bus_req_info.wdata      = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_MsgSendData, ET_LINK_REQ_Atomic }) begin
         bus_req_info.data_size = req_desc.get_bus_req_size();
         bus_req_info.qwen      = get_qword_data_mask(req_desc.get_req_data());
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_Flush, ET_LINK_REQ_Evict }) begin
         bus_req_info.data      = evl_xword_t'(0);
         bus_req_info.data[4:3] = req_desc.get_bus_cop_start_level();
         bus_req_info.data[6:5] = req_desc.get_bus_cop_dest_level();
         bus_req_info.data_size = 0;
         bus_req_info.qwen      = 4'b0001;
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_FlushToMem, ET_LINK_REQ_EvictToMem }) begin
         bus_req_info.data_size = `EVL_LINE_ADDR_LSB;
         bus_req_info.qwen      = 4'b1111;
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_Lock }) begin
         bus_req_info.data      = evl_xword_t'(0);
         bus_req_info.data_size = 0;
         bus_req_info.qwen      = 4'b0000;
         bus_req_info.wdata     = 1'b0;
      end
      else if (req_opcode inside { ET_LINK_REQ_Unlock }) begin
         bus_req_info.data      = evl_xword_t'(0);
         bus_req_info.data[3:3] = req_desc.get_bus_cop_final_state();
         bus_req_info.data_size = 0;
         bus_req_info.qwen      = 4'b0001;
         bus_req_info.wdata     = 1'b1;
      end
      else if (req_opcode == ET_LINK_REQ_ScpFill) begin
         evl_paddr_t scp_fill_addr;

         bus_req_info.data       = evl_xword_t'(0);
         scp_fill_addr           = req_desc.get_bus_scp_fill_addr();
         bus_req_info.data[39:6] = scp_fill_addr[39:6];
         bus_req_info.data_size  = `EVL_DWORD_ADDR_LSB;
         bus_req_info.qwen       = 4'b0001;
         bus_req_info.wdata      = 1'b1;
      end
      else if (req_opcode inside { ET_LINK_REQ_Prefetch }) begin
         bus_req_info.data      = evl_xword_t'(0);
         bus_req_info.data[6:5] = req_desc.get_bus_cop_dest_level();
         bus_req_info.data_size = 0;
         bus_req_info.qwen      = 4'b0001;
         bus_req_info.wdata     = 1'b1;
      end
      else begin
         bus_req_info.data_size = req_desc.get_bus_req_size();
         bus_req_info.qwen      = 4'b0000;
         bus_req_info.wdata     = 1'b0;
      end

      `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_DRV, get_abstract_name(), req_desc, $sformatf("Driving %0s", req_desc.sprint_obj()))

      //Get any request delay
      if (req_desc.get_delay(delay, pk_bus_req_delay) == 1'b1) begin
         bus_req.set_bus_req_delay(delay);
         if (delay > 1) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_DRV, get_abstract_name(), req_desc, $sformatf("using a request delay of %0d for %0s", delay, req_desc.sprint_obj()))
         end
      end
      if (m_data_width_clog2 < req_desc.get_bus_req_size()) begin
         if (req_desc.get_delay(delay, pk_bus_req_data_gap_delay) == 1'b1) begin
            bus_req.set_bus_req_data_delay(delay);
            if (delay > 1) begin
               `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_DRV, get_abstract_name(), req_desc, $sformatf("using a request data gap delay of %0d for %0s", delay, req_desc.sprint_obj()))
            end
         end
      end
      bus_req.set_bus_req_info(bus_req_info);
      bus_req.set_trans_id(req_desc.get_trans_id());

      m_etlink_rtl_port.put_req(bus_req);
   endfunction : drive_request


   //----------------------------------------------------------------------------------------------
   //
   // Function: get_qword_data_mask
   //
   function evl_line_qword_en_t get_qword_data_mask(input evl_line_data line_data);
      evl_line_qword_en_t mask;

      if (line_data == null) begin
         `dut_error(get_abstract_name(), $sformatf("get_qword_data_mask received null line_data"))
         return evl_line_qword_en_t'(0);
      end
      mask[0] = ((line_data.get_dword_valids(0) == evl_dword_byte_en_t'(0)) && (line_data.get_dword_valids(1) == evl_dword_byte_en_t'(0))) ? 1'b0 : 1'b1;
      mask[1] = ((line_data.get_dword_valids(2) == evl_dword_byte_en_t'(0)) && (line_data.get_dword_valids(3) == evl_dword_byte_en_t'(0))) ? 1'b0 : 1'b1;
      mask[2] = ((line_data.get_dword_valids(4) == evl_dword_byte_en_t'(0)) && (line_data.get_dword_valids(5) == evl_dword_byte_en_t'(0))) ? 1'b0 : 1'b1;
      mask[3] = ((line_data.get_dword_valids(6) == evl_dword_byte_en_t'(0)) && (line_data.get_dword_valids(7) == evl_dword_byte_en_t'(0))) ? 1'b0 : 1'b1;
      return mask;
   endfunction : get_qword_data_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
      if (m_etlink_rtl_port != null) begin
         m_etlink_rtl_port.set_req_drive();
         m_data_width = m_etlink_rtl_port.get_int_param(pk_wdata_bytes, 64);
      end
      else begin
         m_data_width = 64;
      end
      m_data_width_clog2 = $clog2(m_data_width);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_etlink_master_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_etlink_master_driver

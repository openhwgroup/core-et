//--------------------------------------------------------------------------------------------------
//
// Class: evl_noc_axi_meshq
//
//
class evl_noc_axi_meshq extends evl_object;

   `uvm_object_utils(evl_noc_axi_meshq)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_sc_sb_error_intensity)
   `evl_param_key(pk_sc_scoreboard_type)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_req_desc m_pending_axi_req_descs[$];
   evl_req_desc m_active_mesh_req_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_pending_axi_req_desc
   //
   function void add_pending_axi_req_desc(input evl_req_desc req_desc, input int sb_type);
      evl_bus_req_id_t req_id = req_desc.get_bus_req_id();

      foreach (m_pending_axi_req_descs[ii]) begin
         if ((m_pending_axi_req_descs[ii] != null) && (m_pending_axi_req_descs[ii].get_bus_req_id() == req_id)) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("add_pending_axi_req_desc() called for ID 0x%x ... %0s is replacing %0s", req_id, req_desc.sprint_obj(), m_pending_axi_req_descs[ii].sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN })
            m_pending_axi_req_descs.delete(ii);
            break;
         end
      end
      req_desc.set_int_param(pk_sc_scoreboard_type, sb_type);
      m_pending_axi_req_descs.push_back(req_desc);
   endfunction : add_pending_axi_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_active_mesh_request
   //
   function evl_req_desc enqueue_active_mesh_request(input evl_req_desc rcv_req_desc);
      evl_req_desc first_req_desc;
      evl_req_desc match_req_desc;

      //find_pending_request(first_req_desc, match_req_desc, rcv_req_desc.get_bus_req_id(), rcv_req_desc.get_bus_cmd(), rcv_req_desc.get_paddr());
      //if (match_req_desc != null) begin
      //   if ((rcv_req_desc != null) && (rcv_req_desc.get_parent_desc() != match_req_desc)) begin
      //      match_req_desc.add_child_desc(rcv_req_desc);
      //   end
      //end
      //else begin
      //   if (first_req_desc == null) begin
      //      `dut_error_mdesc(get_abstract_name(), rcv_req_desc, $sformatf("no AXI request with ID 0x%0x expected (%0s)", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN })
      //   end
      //   else if (match_req_desc == null) begin
      //      `dut_error_mdesc(get_abstract_name(), rcv_req_desc, $sformatf("expected AXI request ID 0x%0x to be %0s, not %0s", first_req_desc.get_bus_req_id(), first_req_desc.sprint_obj(), rcv_req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN })
      //   end
      //end
      rcv_req_desc.print_from_root(UVM_FULL, VMOD_SC_SB);
      //if (match_req_desc != null) begin
      //   m_active_mesh_req_descs.push_back(match_req_desc);
      //   foreach (m_pending_axi_req_descs[ii]) begin
      //      if (m_pending_axi_req_descs[ii] == match_req_desc) begin
      //         m_pending_axi_req_descs.delete(ii);
      //         break;
      //      end
      //   end
      //   return match_req_desc;
      //end
      //else if (first_req_desc != null) begin
      //   m_active_mesh_req_descs.push_back(first_req_desc);
      //   foreach (m_pending_axi_req_descs[ii]) begin
      //      if (m_pending_axi_req_descs[ii] == first_req_desc) begin
      //         m_pending_axi_req_descs.delete(ii);
      //         break;
      //      end
      //   end
      //   return first_req_desc;
      //end
      m_active_mesh_req_descs.push_back(rcv_req_desc);
      return rcv_req_desc;
   endfunction : enqueue_active_mesh_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_active_l3_mesh_request
   //
   function evl_req_desc enqueue_active_l3_mesh_request(input evl_req_desc rcv_req_desc);
      return enqueue_active_mesh_request(rcv_req_desc);
   endfunction : enqueue_active_l3_mesh_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_active_sys_mesh_request
   //
   function evl_req_desc enqueue_active_sys_mesh_request(input evl_req_desc rcv_req_desc);
      return enqueue_active_mesh_request(rcv_req_desc);
   endfunction : enqueue_active_sys_mesh_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_pending_request
   //
   function void find_pending_request(output evl_req_desc     first_req_desc,
                                      output evl_req_desc     match_req_desc,
                                      input  evl_bus_req_id_t req_id,
                                      input  evl_bus_cmd_t    bus_cmd,
                                      input  evl_paddr_t      address);
      evl_req_desc req_desc;

      first_req_desc = null;
      match_req_desc = null;
      foreach (m_pending_axi_req_descs[ii]) begin
         req_desc = m_pending_axi_req_descs[ii];
         if (req_desc.get_bus_req_id() == req_id) begin
            if (first_req_desc == null) begin
               first_req_desc = req_desc;
            end
            if ((req_desc.get_bus_cmd() == bus_cmd) && (req_desc.get_paddr() == address)) begin
               match_req_desc = req_desc;
               return;
            end
         end
      end
      return;
   endfunction : find_pending_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  subport_in,
                                                       input int                  port_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      evl_req_desc     first_req_desc;
      evl_req_desc     match_req_desc;
      evl_bus_cmd_t    bus_cmd;
      evl_paddr_t      address;
      evl_bus_req_id_t req_id;

      if (paired_req_desc == null) begin
         bus_cmd  = bus_cmd_in;
         req_id   = req_id_in;
         address  = address_in;
      end
      else begin
         bus_cmd  = (bus_cmd_in  != `EVL_Bus_Idle)         ? bus_cmd_in  : paired_req_desc.get_bus_cmd();
         req_id   = (req_id_in   != ~evl_bus_req_id_t'(0)) ? req_id_in   : paired_req_desc.get_bus_req_id();
         address  = (address_in  != ~evl_paddr_t'(0))      ? address_in  : paired_req_desc.get_paddr();
      end
      if ((port_type_in == AXI_BUS_AGENT) && (port_in inside { 0, 1 })) begin
         find_pending_request(first_req_desc, match_req_desc, req_id, bus_cmd, address);
         if (match_req_desc != null) begin
            if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc)) begin
               match_req_desc.add_child_desc(paired_req_desc);
            end
            return match_req_desc;
         end
      end
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Task: dequeue_active_mesh_request
   //
   function evl_req_desc dequeue_active_mesh_request(input evl_req_desc req_desc);
      foreach (m_active_mesh_req_descs[ii]) begin
         if (m_active_mesh_req_descs[ii] == req_desc) begin
            m_active_mesh_req_descs.delete(ii);
            return req_desc;
         end
      end
      return null;
   endfunction : dequeue_active_mesh_request


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_noc_axi_meshq");
      super.new(name_in);
      set_abstract_name("AXI_MESHQ");
   endfunction : new

endclass : evl_noc_axi_meshq

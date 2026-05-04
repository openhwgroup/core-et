//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_reqq
//
//
class evl_shire_cache_reqq extends evl_object;

   `uvm_object_utils(evl_shire_cache_reqq)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_idx_cache_op)
   `evl_param_key(pk_l2_opcode)
   `evl_param_key(pk_paired_reqq_id)
   `evl_param_key(pk_reqq_id)
   `evl_param_key(pk_sc_addr_space)
   `evl_param_key(pk_sc_sb_error_intensity)
   `evl_param_key(pk_scratchpad_req)
   `evl_param_key(pk_upstream_req_port)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int                m_reqq_active_count = 0;
   int                m_final_print_done  = 0;
   evl_shire_id_t     m_shire_id          = evl_shire_id_t'(0);
   evl_paddr_t        m_bank_addr_mask    = evl_paddr_t'(0);
   evl_req_desc_queue m_port_queues[int];
   // L2 index cacheops don't block L3 ones and vice versa, so keep them separate:
   evl_req_desc       m_pending_l2_idx_cop_req_descs[int];
   evl_req_desc       m_pending_l3_idx_cop_req_descs[int];
   evl_req_desc       m_reqq_entries[`SC_BANKS << 1][`SC_REQQ_DEPTH << 1];
   evl_req_desc       m_template_req_desc_et;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   function int set_shire_id(input evl_shire_id_t shire_id_in);
      m_shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bank_info
   //
   function int set_bank_info(input int num_banks, input evl_req_desc template_req_desc_et);
      m_bank_addr_mask       = evl_paddr_t'(num_banks) - 1;
      m_template_req_desc_et = template_req_desc_et;
   endfunction : set_bank_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_port_index
   //
   function int gen_port_index(input int bank_id, input int port_num);
      return (port_num | (bank_id << 8));
   endfunction : gen_port_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_port_request
   //
   function void enqueue_port_request(input int port_num, input int bank_id_in, input evl_req_desc req_desc);
      int bank_id;
      int port_index;

      if (req_desc == null) begin
         return;
      end
      // Make sure all requests received are marked as observed. This seems like a fine place to do that.
      req_desc.set_observed();

      bank_id    = bank_id_in;
      port_index = gen_port_index(bank_id, port_num);
      req_desc.set_int_param(pk_upstream_req_port, port_num);
      if (!m_port_queues.exists(port_index)) begin
         m_port_queues[port_index] = new($sformatf("%0s(PORT-%0d.%0d)", get_root_abstract_name(), port_num, bank_id));
      end
      if (req_desc.get_bus_type() == EVL_BUS_TYPE_ETLINK) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("received %0s on ET Link port %0d (port index 0x%0x)", req_desc.sprint_obj(), port_num, port_index))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("received %0s on the MESH->L3 port (port index 0x%0x)", req_desc.sprint_obj(), port_index))
      end
      m_port_queues[port_index].push(req_desc);
   endfunction : enqueue_port_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_idx_cop_request
   //
   function void enqueue_idx_cop_request(input evl_trans_id_t trid, input evl_sc_bank_id_t bank_id, input evl_sc_idx_cop_req_t rtl_info);
      evl_req_desc req_desc;

      $cast(req_desc, m_template_req_desc_et.clone());
      if (rtl_info.opcode inside { SC_Idx_Read }) begin
         req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_Read));
         req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Read));
      end
      else begin
         req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_Write));
         req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Write));
      end
      req_desc.set_paddr(rtl_info.address);
      req_desc.set_trans_id(trid);
      req_desc.set_bus_req_id(~evl_bus_req_id_t'(0));
      req_desc.set_bus_req_size(`EVL_LINE_BYTES);
      req_desc.set_int_param(pk_l2_opcode, int'(rtl_info.opcode));
      req_desc.set_int_param(pk_idx_cache_op, 1);
      if (rtl_info.opcode inside {SC_Idx_L3_Inv, SC_Idx_L3_Flush, SC_Idx_L3_Evict, SC_Sync}) begin
         m_pending_l3_idx_cop_req_descs[bank_id] = req_desc;
      end
      else begin
         m_pending_l2_idx_cop_req_descs[bank_id] = req_desc;
      end
      `evl_log_mdesc(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("Index Cache Op Req (bank %0d): %p 0x%x (%0s)", bank_id, rtl_info.opcode, rtl_info.address, req_desc.sprint_obj()))
   endfunction : enqueue_idx_cop_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_reqq_entry
   //
   function void enqueue_reqq_entry(input evl_sc_bank_id_t    bank_id,
                                    input evl_sc_reqq_id_t    reqq_id,
                                    input evl_req_desc        req_desc,
                                    input evl_req_desc        prev_req_desc = null,
                                    input bit                 paired_req    = 1'b0,
                                    input evl_verbosity_mod_t verbosity_mod = VMOD_SC_SB_WARN);
      if (req_desc != null) begin
         `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("allocating bank %0d, reqq ID 0x%x%0s: %0s", bank_id, reqq_id, ((paired_req == 1'b1) ? " (paired req)" : ""), req_desc.sprint_obj()))
      end
      else begin
         `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), ~evl_trans_id_t'(0), reqq_id, $sformatf("allocating bank %0d, reqq ID 0x%x%0s", bank_id, reqq_id, ((paired_req == 1'b1) ? " (paired req)" : "")))
      end
      if ((m_reqq_entries[bank_id][reqq_id] != null) && (m_reqq_entries[bank_id][reqq_id] != prev_req_desc)) begin
         `dut_error(get_abstract_name(), $sformatf("attempting to allocate bank %0d, reqq ID 0x%x ... contains %0s", bank_id, reqq_id, m_reqq_entries[bank_id][reqq_id].sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
      end
      if (paired_req == 1'b1) begin
         m_reqq_entries[bank_id][reqq_id] = null;
      end
      else begin
         m_reqq_entries[bank_id][reqq_id] = req_desc;
      end
   endfunction : enqueue_reqq_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: mark_paired_reqq_entry
   //
   function evl_req_desc mark_paired_reqq_entry(input evl_sc_bank_id_t bank_id,
                                                input evl_sc_reqq_id_t reqq_id,
                                                input evl_sc_reqq_id_t paired_id,
                                                input bit              extract_etlink_cmd);
      evl_req_desc req_desc = m_reqq_entries[bank_id][paired_id];

      if (req_desc != null) begin
         evl_etlink_req_opcode_t bus_cmd = (extract_etlink_cmd         == 1'b0) ? evl_etlink_req_opcode_t'(req_desc.get_bus_cmd()) :
                                           (req_desc.bus_cmd_is_read() == 1'b1) ? ET_LINK_REQ_Read                                 :
                                                                                  evl_etlink_req_opcode_t'(req_desc.get_bus_req_user());

         if (bus_cmd inside { ET_LINK_REQ_WriteAround, ET_LINK_REQ_Atomic, ET_LINK_REQ_Write }) begin
            if (req_desc.have_int_param(pk_paired_reqq_id) == 1'b0) begin
               req_desc.set_int_param(pk_paired_reqq_id, reqq_id);
            end
         end
      end
      return req_desc;
   endfunction : mark_paired_reqq_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_req_addr
   //
   function bit check_req_addr(input evl_sc_address_space_t addr_space,
                               input evl_paddr_t            obs_addr,
                               input evl_bus_req_id_t       obs_req_id,
                               input evl_req_desc           req_desc,
                               input bit                    no_error,
                               input evl_verbosity_mod_t    verbosity_mod = VMOD_SC_SB_WARN);
      evl_paddr_t      exp_addr   = req_desc.get_paddr();
      evl_bus_req_id_t exp_req_id = req_desc.get_bus_req_id();

      if ( (obs_addr == exp_addr) &&
           ((obs_req_id == exp_req_id) || (obs_req_id == ~evl_bus_req_id_t'(0)) || (exp_req_id == ~evl_bus_req_id_t'(0))) ) begin
         return 1'b1;
      end
      if ( (addr_space == SC_Address_Scp) &&
           (int'(obs_addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) &&
           (exp_addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE] == ~(`SC_SCP_SHIRE_ID_SIZE'(1'b0))) ) begin
         evl_paddr_t modified_addr = exp_addr;

         //
         // If this is a remote SCP access and the shire id is 0xff, that really means we're
         // accessing this shire.  The RTL has changed the shire id bits of the address in that
         // case, so a mismatch on those bits is expected.
         //
         modified_addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE] = m_shire_id;
         if ((obs_addr == modified_addr) && ((obs_req_id == exp_req_id) || (obs_req_id == ~evl_bus_req_id_t'(0)) || (exp_req_id == ~evl_bus_req_id_t'(0)))) begin
            return 1'b1;
         end
         if (no_error == 1'b0) begin
            if (obs_addr != modified_addr) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("address mismatch for this scp access with 0xff shire id ... expected 0x%x, but received 0x%x (original req_desc paddr = 0x%x)", modified_addr, obs_addr, exp_addr), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
            end
         end
      end
      return 1'b0;
   endfunction : check_req_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_reqq_alloc
   //
   function evl_req_desc perform_reqq_alloc(input evl_trans_id_t         trans_id,
                                            input evl_sc_bank_id_t       bank_id,
                                            input int                    port_num,
                                            input evl_bus_req_id_t       tag_id,
                                            input evl_sc_reqq_id_t       reqq_id,
                                            input bit                    second_alloc,
                                            input int                    paired_id,
                                            input evl_paddr_t            addr,
                                            input evl_sc_address_space_t addr_space,
                                            input bit                    from_l3_mesh,
                                            input bit                    read_req,
                                            input evl_verbosity_mod_t    verbosity_mod);
      int          port_index = -1;
      evl_req_desc req_desc;

      req_desc = null;
      m_reqq_active_count++;

      if (addr_space == SC_Address_L3) begin
         if (!m_pending_l3_idx_cop_req_descs.exists(bank_id)) begin
            m_pending_l3_idx_cop_req_descs[bank_id] = null;
         end
         if (m_pending_l3_idx_cop_req_descs[bank_id] == null) begin
            if (from_l3_mesh == 1'b1) begin
               port_index = gen_port_index(int'(bank_id), `EVL_BASE_PORT_NUM_AXI);
            end
            else begin
               port_index = gen_port_index(int'(bank_id), port_num);
            end
         end
      end
      else begin
         if (!m_pending_l2_idx_cop_req_descs.exists(bank_id)) begin
            m_pending_l2_idx_cop_req_descs[bank_id] = null;
         end
         if (m_pending_l2_idx_cop_req_descs[bank_id] == null) begin
            if (from_l3_mesh == 1'b1) begin
               port_index = gen_port_index(int'(bank_id), `EVL_BASE_PORT_NUM_AXI);
            end
            else begin
               port_index = gen_port_index(int'(bank_id), port_num);
            end
         end
      end

      if (!m_port_queues.exists(port_index)) begin
         if (from_l3_mesh == 1'b1) begin
            m_port_queues[port_index] = new($sformatf("%0s(MESH-L3.%0d)", get_root_abstract_name(), bank_id));
            if (!m_port_queues.exists(gen_port_index(0, `EVL_BASE_PORT_NUM_AXI))) begin
               m_port_queues[gen_port_index(0, `EVL_BASE_PORT_NUM_AXI)] = new($sformatf("%0s(MESH-L3.%0d)", get_root_abstract_name(), 0));
            end
         end
         else begin
            m_port_queues[port_index] = new($sformatf("%0s(PORT-%0d.%0d)", get_root_abstract_name(), port_num, bank_id));
         end
      end

      if (second_alloc == 1'b1) begin
         req_desc = mark_paired_reqq_entry(bank_id, reqq_id, paired_id, from_l3_mesh);
         if (req_desc != null) begin
            if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
               `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), addr, req_desc.get_trans_id(), reqq_id, $sformatf("REQQ alloc (bank %0d, port %0d, %p, 0x%x, 2nd): %0s", bank_id, port_num, addr_space, addr, req_desc.sprint_obj()))
            end
            else begin
               `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), addr, trans_id, reqq_id, $sformatf("REQQ alloc (bank %0d, port %0d, %p, 0x%x, 2nd): %0s", bank_id, port_num, addr_space, addr, req_desc.sprint_obj()))
            end
         end
         else begin
            `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), addr, trans_id, reqq_id, $sformatf("REQQ alloc (bank %0d, port %0d, %p, 0x%x, 2nd): null", bank_id, port_num, addr_space, addr))
         end
         enqueue_reqq_entry(bank_id, reqq_id, req_desc, null, 1'b1, verbosity_mod);
         return null;
      end

      if (port_index < 0) begin
         if (addr_space == SC_Address_L3) begin
            req_desc = m_pending_l3_idx_cop_req_descs[bank_id];
         end
         else begin
            req_desc = m_pending_l2_idx_cop_req_descs[bank_id];
         end
      end
      else if (from_l3_mesh == 1'b0) begin
         req_desc = m_port_queues[port_index].peek();
      end
      else begin
         //
         // For incoming L3 requests, there is a special case in which the incoming port may not
         // actually match what is expected ... it may come in on port 0 rather than the expected
         // port.
         //
         if (read_req == 1'b1) begin
            req_desc = m_port_queues[port_index].find_first_read();
         end
         else begin
            req_desc = m_port_queues[port_index].find_first_write();
         end
         if ((req_desc == null) || ((bank_id != 0) && (check_req_addr(addr_space, addr, tag_id, req_desc, 1'b1, verbosity_mod) == 1'b0))) begin
            port_index = gen_port_index(int'(`SC_BANKS + bank_id), `EVL_BASE_PORT_NUM_AXI);
            if (!m_port_queues.exists(port_index)) begin
               m_port_queues[port_index] = new($sformatf("%0s(PORT-%0d.%0d)", get_root_abstract_name(), port_num, int'(`SC_BANKS + bank_id)));
            end
            if (read_req == 1'b1) begin
               req_desc = m_port_queues[port_index].find_first_read();
            end
            else begin
               req_desc = m_port_queues[port_index].find_first_write();
            end
            //
            // If our attempt to find the request in the fake AXI-0 port failed, then go back to the
            // original.
            //
            if ((req_desc == null) || (check_req_addr(addr_space, addr, tag_id, req_desc, 1'b1, verbosity_mod) == 1'b0)) begin
               port_index = gen_port_index(int'(bank_id), `EVL_BASE_PORT_NUM_AXI);
               if (read_req == 1'b1) begin
                  req_desc = m_port_queues[port_index].find_first_read();
               end
               else begin
                  req_desc = m_port_queues[port_index].find_first_write();
               end
            end
         end
      end

      if (req_desc == null) begin
         `dut_error(get_abstract_name(), $sformatf("perform_reqq_alloc(): attempting to allocate bank %0d, reqq ID 0x%x (port %0d) ... found no request", bank_id, reqq_id, port_num), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
         enqueue_reqq_entry(bank_id, reqq_id, null, null, 1'b0, verbosity_mod);
         return null;
      end

      if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
         `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), addr, req_desc.get_trans_id(), reqq_id, $sformatf("REQQ alloc (bank %0d, port %0d, %p, 0x%x): %0s", bank_id, port_num, addr_space, addr, req_desc.sprint_obj()))
      end
      else begin
         `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), addr, trans_id, reqq_id, $sformatf("REQQ alloc (bank %0d, port %0d, %p, 0x%x): %0s", bank_id, port_num, addr_space, addr, req_desc.sprint_obj()))
      end

      //
      // If there is an address mismatch, let the appropriate authorities know.
      //
      if (check_req_addr(addr_space, addr, tag_id, req_desc, 1'b0, verbosity_mod) == 1'b0) begin
         if ((addr != req_desc.get_paddr())) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("perform_reqq_alloc(): address mismatch ... expected 0x%x, but received 0x%x", req_desc.get_paddr(), addr), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
         end
         if ((tag_id != req_desc.get_bus_req_id()) && (tag_id != ~evl_bus_req_id_t'(0)) && (req_desc.get_bus_req_id() != ~evl_bus_req_id_t'(0))) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("perform_reqq_alloc(): request ID mismatch ... expected 0x%x, but received 0x%x", req_desc.get_bus_req_id(), tag_id), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
         end
      end

      //
      // If there is a TrID mismatch, nullify the request descriptor and wait for a match.
      //
      if ((trans_id != evl_trans_id_t'(0)) && (req_desc.get_trans_id() != trans_id)) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("perform_reqq_alloc(): transaction ID mismatch ... expected 0x%x, but received 0x%x", req_desc.get_trans_id(), trans_id), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, verbosity_mod })
         req_desc = null;
      end

      if ((req_desc == null) && (port_index >= 0)) begin
         req_desc = m_port_queues[port_index].find_first_by_cache_addr_and_trid(addr, trans_id);
      end

      enqueue_reqq_entry(bank_id, reqq_id, req_desc, null, 1'b0, verbosity_mod);

      if (req_desc == null) begin
         return null;
      end

      if (port_index < 0) begin
         if (addr_space == SC_Address_L3) begin
            m_pending_l3_idx_cop_req_descs[bank_id] = null;
         end
         else begin
            m_pending_l2_idx_cop_req_descs[bank_id] = null;
         end
      end
      else begin
         void'(m_port_queues[port_index].pop(req_desc));
      end
      req_desc.set_int_param(pk_reqq_id, reqq_id);
      req_desc.set_int_param(pk_sc_addr_space, int'(addr_space));
      return req_desc;
   endfunction : perform_reqq_alloc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reqq_entry
   //
   function evl_req_desc get_reqq_entry(input evl_sc_bank_id_t bank_id, input evl_sc_reqq_id_t reqq_id);
      return m_reqq_entries[bank_id][reqq_id];
   endfunction : get_reqq_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dequeue_reqq_entry
   //
   function void dequeue_reqq_entry(input evl_sc_bank_id_t bank_id, input evl_sc_reqq_id_t reqq_id);
      evl_req_desc req_desc = m_reqq_entries[bank_id][reqq_id];

      if (req_desc != null) begin
         `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("REQQ dealloc (bank %0d): deallocating bank %0d, reqq ID 0x%x: %0s", bank_id, bank_id, reqq_id, req_desc.sprint_obj()))
      end
      else begin
         `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), ~evl_trans_id_t'(0), reqq_id, $sformatf("REQQ dealloc (bank %0d): deallocating bank %0d, reqq ID 0x%x", bank_id, bank_id, reqq_id))
      end
      m_reqq_entries[bank_id][reqq_id] = null;
      if (m_reqq_active_count > 0) begin
         m_reqq_active_count--;
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("attempting to decrement bank %0d, reqq_id 0x%x (count is already 0)", bank_id, reqq_id))
      end
   endfunction : dequeue_reqq_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reqq_is_empty
   //
   function bit reqq_is_empty();
      return ((m_reqq_active_count <= 0) ? 1'b1 : 1'b0);
   endfunction : reqq_is_empty


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reqq_is_not_empty
   //
   function bit reqq_is_not_empty();
      return ((m_reqq_active_count <= 0) ? 1'b0 : 1'b1);
   endfunction : reqq_is_not_empty


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reqq_active_count
   //
   function int get_reqq_active_count();
      return m_reqq_active_count;
   endfunction : get_reqq_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_reqq_empty
   //
   task wait_for_reqq_empty();
      #0 wait (m_reqq_active_count <= 0);
   endtask : wait_for_reqq_empty


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_reqq_not_empty
   //
   task wait_for_reqq_not_empty();
      #0 wait (m_reqq_active_count > 0);
   endtask : wait_for_reqq_not_empty


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_active_reqq_entries
   //
   function void print_active_reqq_entries(input bit print_once = 1'b1);
      bit          have_active_desc = 1'b0;
      evl_req_desc req_desc;

      if (print_once == 1'b1) begin
         if (m_final_print_done++ != 0) begin
            m_final_print_done = 1;
            return;
         end
      end
      for (int bank = 0; bank < (`SC_BANKS << 1); bank++) begin
         for (int reqq_id = 0; reqq_id < (`SC_REQQ_DEPTH << 1); reqq_id++) begin
            if (m_reqq_entries[bank][reqq_id] != null) begin
               have_active_desc = 1'b1;
               break;
            end
         end
      end
      if ((have_active_desc == 1'b0) && (m_reqq_active_count <= 0)) begin
         return;
      end
      `evl_log(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), $sformatf("Number of active request queue entries: %0d", m_reqq_active_count))
      for (int bank = 0; bank < (`SC_BANKS << 1); bank++) begin
         for (int reqq_id = 0; reqq_id < (`SC_REQQ_DEPTH << 1); reqq_id++) begin
            req_desc = m_reqq_entries[bank][reqq_id];
            if (req_desc != null) begin
               `evl_log_mdesc(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("bank %0d, entry 0x%02x: %0s", bank, reqq_id, req_desc.sprint_obj()))
            end
         end
      end
   endfunction : print_active_reqq_entries


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_cache_reqq");
      super.new(name_in);
      set_abstract_name("SHIRE_CACHE(SB)");
   endfunction : new

endclass : evl_shire_cache_reqq

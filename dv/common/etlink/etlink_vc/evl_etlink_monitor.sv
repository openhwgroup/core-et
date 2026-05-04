//--------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_monitor
//
`ifndef ET_AMOCMPEXCH_FIX
   `define ET_AMOCMPEXCH_FIX 1
`endif // ifndef ET_AMOCMPEXCH_FIX

class evl_etlink_monitor extends evl_monitor;

   `uvm_component_utils(evl_etlink_monitor)

   static evl_etlink_bus_parser m_bus_parser = evl_etlink_bus_parser::get_inst();

   `evl_param_key(pk_allow_unique_tags)
   `evl_param_key(pk_ignore_message)
   `evl_param_key(pk_ignore_dst_rsp)
   `evl_param_key(pk_is_evict_if)
   `evl_param_key(pk_ignore_src_dst)
   `evl_param_key(pk_rdata_bytes)
   `evl_param_key(pk_sc_etlink_error_intensity)
   `evl_param_key(pk_warning_on_unmatched_etlink_response)
   `evl_param_key(pk_wdata_bytes)

   // This is the handle to the interface
   evl_etlink_rtl_port m_etlink_rtl_port;

   // This is a handle back to the verification component
   int            m_port_id           = 0;
   evl_verif_comp m_verif_comp        = null;
   evl_verif_comp m_master_port_agent = null;
   evl_verif_comp m_slave_port_agent  = null;

   //-----------------------------------------------------------------------------------------------
   //
   // analysis port, to send the transactions to scoreboard/monitor/etc...
   //
   uvm_analysis_port #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port #(evl_req_desc) m_obs_rsp_port;

   // This is the queue to hold requests so we can match up the responses
   evl_req_desc_queue m_request_queues[int];
   evl_req_desc       m_coop_id_queues[int][$];

   //
   evl_line_data  m_template_req_data;
   evl_line_desc  m_template_line_desc;
   evl_trans_desc m_template_trans_desc;
   evl_req_desc   m_template_req_desc;

   int            m_wdata_bytes;
   int            m_rdata_bytes;
   int            m_wdata_bytes_log2;
   int            m_rdata_bytes_log2;
   bit            m_amocmpexch_fix;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_master_port_agent = agent_in;
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_master_port_agent;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_port_agent
   //
   virtual function void set_slave_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_slave_port_agent = agent_in;
   endfunction : set_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_slave_port_agent;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_tag_id
   //
   function int gen_bus_tag_id(input evl_bus_req_id_t     id,
                               input evl_bus_req_source_t source);
      if (m_etlink_rtl_port.get_int_param(pk_ignore_src_dst)) begin
         return (int'(id));
      end
      else begin
         return ((int'(source) << `EVL_BUS_REQ_ID_WIDTH) | int'(id));
      end
   endfunction : gen_bus_tag_id


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_requests
   //
   // Get request objects from the interface, transfer the information
   // into a request descriptor, then send it on the obs_req_port.
   //
   task process_requests();
      evl_req_desc           req_desc;
      evl_req_desc           exp_req_desc;
      evl_line_data          line_data;

      evl_xword_t            xword_data;
      evl_xword_byte_en_t    xword_byte_en;
      evl_line_qword_en_t    qword_en;
      evl_etlink_bus_req_s   bus_req;
      int                    bus_tag;
      et_link_neigh_source_t etlink_src;
      string                 extra_info;

      forever begin
         m_etlink_rtl_port.get_obs_req(bus_req);

         $cast(req_desc, m_template_req_desc.clone());
         req_desc.mark_as_bus_child();
         req_desc.set_bus_cmd(bus_req.opcode);
         if (bus_req.cached) begin
            req_desc.set_cached();
         end
         req_desc.set_paddr(bus_req.address);
         req_desc.set_bus_req_size(bus_req.data_size);
         req_desc.set_bus_req_id(bus_req.id);
         req_desc.set_bus_req_source(bus_req.source);
         req_desc.set_bus_rsp_dest(bus_req.source);
         req_desc.set_bus_bank(evl_sc_bank_id_t'(bus_req.bank));
         bus_tag  = gen_bus_tag_id(bus_req.id, bus_req.source);
         qword_en = evl_line_qword_en_t'(0);

         //
         // The following code fragment will link the observed request to a predicted request.  This
         // will cause the transaction ID to be set appropriately.
         //
         if (m_master_port_agent != null) begin
            exp_req_desc = m_master_port_agent.find_observed_request(ETLINK_BUS_AGENT, m_port_id, 0, req_desc);
            req_desc.set_parent_check();
            if (exp_req_desc != null) begin
               exp_req_desc.set_bus_req_desc(req_desc);
               req_desc.set_correlated();
               req_desc.reset_create_time();
            end
            else if ((req_desc.get_parent_req_desc() == null) && (!(req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }))) begin
               req_desc.set_trans_id();
            end
         end

         //
         // If the upstream component didn't predict anything for this request
         // create a transaction descriptor out of thin air to set it as the root
         //
         if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
            if (req_desc.get_root_trans_desc() == null) begin
               evl_cmd_t               cmd;
               evl_etlink_req_opcode_t bus_cmd;
               evl_trans_desc          trans_desc;

               bus_cmd = evl_etlink_req_opcode_t'(req_desc.get_bus_cmd());
               $cast(trans_desc, m_template_trans_desc.clone());
               case (bus_cmd)
                  ET_LINK_REQ_Read:        cmd = LD;
                  ET_LINK_REQ_Write:       cmd = ST;
                  ET_LINK_REQ_WriteAround: cmd = ST;
                  ET_LINK_REQ_ReadCoop:    cmd = LD_COOP;
                  ET_LINK_REQ_MsgSendData: cmd = MESSAGE;
                  ET_LINK_REQ_Atomic:      cmd = ATOMIC;
                  ET_LINK_REQ_ScpRead:     cmd = LD;
                  ET_LINK_REQ_ScpWrite:    cmd = ST;
                  ET_LINK_REQ_Flush:       cmd = CACHE_FLUSH;
                  ET_LINK_REQ_FlushToMem:  cmd = CACHE_FLUSH_TO_MEM;
                  ET_LINK_REQ_Evict:       cmd = CACHE_EVICT;
                  ET_LINK_REQ_EvictToMem:  cmd = CACHE_EVICT_TO_MEM;
                  ET_LINK_REQ_Lock:        cmd = CACHE_LOCK;
                  ET_LINK_REQ_Unlock:      cmd = CACHE_UNLOCK;
                  ET_LINK_REQ_ScpFill:     cmd = CACHE_SCP_FILL;
                  ET_LINK_REQ_Prefetch:    cmd = CACHE_PREFETCH;
                  ET_LINK_REQ_AtomicRsp:   cmd = ATOMIC;
                  default:                 cmd = NOCMD;
               endcase
               trans_desc.set_cmd(cmd);
               trans_desc.set_paddr(req_desc.get_paddr());

               if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  trans_desc.set_trans_id();
               end
               else begin
                  trans_desc.set_trans_id(req_desc.get_trans_id());
               end
               trans_desc.set_created_by_bus();
               trans_desc.add_child_desc(req_desc);
            end
         end
         if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            req_desc.set_trans_id();
         end

         extra_info = "";
         if (bus_req.opcode == ET_LINK_REQ_WriteAround) begin
            req_desc.set_bus_coop_way(bus_req.subopcode);
            extra_info = $sformatf(" (data: 0x%x) (coop_id: 0x%x) (coop_way: 0x%x)", bus_req.data[31:0], req_desc.get_bus_coop_id(), bus_req.subopcode);
         end
         else if (bus_req.opcode == ET_LINK_REQ_ReadCoop) begin
            req_desc.set_bus_minion_coop_mask(bus_req.data[`EVL_BUS_COOP_MINION_MASK_MSB:`EVL_BUS_COOP_MINION_MASK_LSB]);
            req_desc.set_bus_neigh_coop_mask(bus_req.data[`EVL_BUS_COOP_NEIGH_MASK_MSB:`EVL_BUS_COOP_NEIGH_MASK_LSB]);
            req_desc.set_bus_coop_id(bus_req.data[`EVL_BUS_COOP_ID_MSB:`EVL_BUS_COOP_ID_LSB]);
            extra_info = $sformatf(" (neigh_mask 0x%8b) (minion_mask 0x%8b) (coop_id 0x%x)", req_desc.get_bus_neigh_coop_mask(), req_desc.get_bus_minion_coop_mask(), req_desc.get_bus_coop_id());
         end
         else if (bus_req.opcode inside { ET_LINK_REQ_Read, ET_LINK_REQ_Write }) begin
            evl_subopcode_dest_t my_subopccode;

            my_subopccode = evl_subopcode_dest_t'(bus_req.subopcode);
            req_desc.set_bus_subopcode_dest(bus_req.subopcode);
            if (!(my_subopccode inside { dest_l2, dest_scp })) begin
               extra_info = $sformatf(" (%0p)", bus_req.subopcode);
            end
         end
         etlink_src = et_link_neigh_source_t'(bus_req.source);
         `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), req_desc, $sformatf("saw a request: %p 0x%x, source %p, id 0x%x%0s", bus_req.opcode, bus_req.address, etlink_src, bus_req.id, extra_info))

         if (req_desc.bus_cmd_has_write_data() == 1'b1) begin
            evl_dword_t         dword_data;
            evl_dword_byte_en_t dword_byte_en;

            $cast(line_data, m_template_req_data.clone());

            xword_data    = evl_xword_t'(0);
            xword_byte_en = evl_xword_byte_en_t'(0);
            qword_en      = bus_req.qwen;

            // For Write and WriteAround to the L2 (cached), size[3:0] are
            // a mask for 128 bit chunks. For everyone else, it's an encoded size
            if ((bus_req.opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_WriteAround }) && (bus_req.data_size == `EVL_LINE_ADDR_LSB) && (bus_req.cached == 1'b1)) begin
               xword_byte_en = {((bus_req.qwen[3] == 1'b1) ? ~(evl_qword_byte_en_t'(0)) : evl_qword_byte_en_t'(0)),
                                ((bus_req.qwen[2] == 1'b1) ? ~(evl_qword_byte_en_t'(0)) : evl_qword_byte_en_t'(0)),
                                ((bus_req.qwen[1] == 1'b1) ? ~(evl_qword_byte_en_t'(0)) : evl_qword_byte_en_t'(0)),
                                ((bus_req.qwen[0] == 1'b1) ? ~(evl_qword_byte_en_t'(0)) : evl_qword_byte_en_t'(0))};
               xword_data = bus_req.data;
            end
            else begin
               if (bus_req.opcode inside { ET_LINK_REQ_Atomic }) begin
                  xword_data = bus_req.data;
               end
               else if (bus_req.opcode inside { ET_LINK_REQ_MsgSendData }) begin
                  xword_data = bus_req.data;
               end
               else begin
                  xword_data = bus_req.data << (m_wdata_bytes * 8 * (bus_req.address[5:0] >> m_wdata_bytes_log2));
               end

               case (bus_req.data_size)
                  3'b000:  xword_byte_en = 64'h1         << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 :  bus_req.address[5:0]);
                  3'b001:  xword_byte_en = 64'h3         << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 : {bus_req.address[5:1], 1'b0});
                  3'b010:  xword_byte_en = 64'hF         << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 : {bus_req.address[5:2], 2'b0});
                  3'b011:  xword_byte_en = 64'hFF        << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 : {bus_req.address[5:3], 3'b0});
                  3'b100:  xword_byte_en = 64'hFFFF      << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 : {bus_req.address[5:4], 4'b0});
                  3'b101:  xword_byte_en = 64'hFFFF_FFFF << ((bus_req.opcode == ET_LINK_REQ_Atomic) ? 0 : {bus_req.address[5], 5'b0});
                  default: xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
               endcase

               if (bus_req.opcode inside {ET_LINK_REQ_MsgSendData}) begin
                  xword_byte_en = (1 << (1 << bus_req.data_size)) - 1;
               end
               else if (bus_req.opcode == ET_LINK_REQ_Atomic) begin
                  req_desc.set_bus_subopcode_dest(bus_req.subopcode);
                  req_desc.set_bus_atomic_conf(bus_req.subopcode);
                  req_desc.set_bus_atomic_offset(bus_req.address & `EVL_LINE_OFFSET_MASK);

                  if ((m_amocmpexch_fix == 1'b1) && (int'(req_desc.get_bus_atomic_conf() & (`EVL_BUS_ATOMIC_CONF_VEC_OP | `EVL_BUS_ATOMIC_OP_MASK)) == int'(SC_AMOCOMPEXCH))) begin
                     if (bus_req.data_size == 3'b011) begin
                        xword_byte_en |= 64'h000000000000ff00;
                     end
                     else begin
                        xword_byte_en |= 64'h0000000000000f00;
                     end
                  end

                  // this looks weird but has reasons - in atomic requests
                  // there can be data in the lower 256 bits of the data, but
                  // the size/byte_en reflects the atomic operation size.
                  // E.g. some atomic operations provide 2 64-bit data but the
                  // size indicates only 64-bit.  So we can't rely on the
                  // size/byte_en for how much data to grab, only the qwen.
                  // The set_qword also sets the byte enables, so we need to
                  // then invalidate the line which clears out the byte_ens
                  // and leaves the data, so we can then set the appropriate
                  // byte_en later on
                  if (bus_req.qwen[1] == 1'b1) begin
                     line_data.set_qword(1, evl_qword_t'(xword_data));
                  end
                  if (bus_req.qwen[0] == 1'b1) begin
                     line_data.set_qword(0, evl_qword_t'(xword_data));
                  end
                  line_data.invalidate_line();
               end
               // Cacheops use some data bits for type information, etc...
               else if (bus_req.opcode == ET_LINK_REQ_Flush) begin
                  req_desc.set_bus_cop_start_level(xword_data[4:3]);
                  req_desc.set_bus_cop_dest_level(xword_data[6:5]);
               end
               else if (bus_req.opcode == ET_LINK_REQ_Unlock) begin
                  req_desc.set_bus_cop_final_state(xword_data[3]);
               end
               else if (bus_req.opcode == ET_LINK_REQ_Evict) begin
                  req_desc.set_bus_cop_start_level(xword_data[4:3]);
                  req_desc.set_bus_cop_dest_level(xword_data[6:5]);
               end
               else if (bus_req.opcode == ET_LINK_REQ_Prefetch) begin
                  req_desc.set_bus_cop_dest_level(xword_data[6:5]);
               end
               else if (bus_req.opcode == ET_LINK_REQ_ScpFill) begin
                  evl_paddr_t scp_fill_addr;

                  scp_fill_addr = evl_paddr_t'(0);
                  scp_fill_addr[39:6] = xword_data[39:6];
                  req_desc.set_bus_scp_fill_addr(scp_fill_addr);
               end
            end

            line_data.set_xword(0, xword_data, xword_byte_en);
            req_desc.set_req_data(line_data);
            req_desc.set_bus_req_qword_en(qword_en);
         end // if (req_desc.bus_cmd_has_write_data() == 1'b1)

         m_obs_req_port.write(req_desc);
         req_desc.set_issued();

         // If we don't expect a response, this request is done
         if (req_desc.bus_cmd_requires_rsp() == 1'b0) begin
            req_desc.set_done();
         end
         // Otherwise add it to the request queues
         // (Unless the source is TensorLoad Coop, this is a very special case
         //  where the shire cache scoreboard will provide the correct expected desc)
         else if ((req_desc.get_bus_req_source() != ET_LINK_NEIGH_SRC_Coop) || (req_desc.get_bus_cmd() == ET_LINK_REQ_Read)) begin
            if (!m_request_queues.exists(bus_tag)) begin
               m_request_queues[bus_tag] = new($sformatf("%0s(REQQ-%04x)", get_root_abstract_name(), bus_tag));
            end
            if ((get_int_param(pk_allow_unique_tags, 0) != 0) && (m_request_queues[bus_tag].size() > 0) && (req_desc.bus_cmd_has_unique_tag() == 1'b1)) begin
               `dut_error(get_abstract_name(), $sformatf("received a request for source %s, request id 0x%x, but there is already one in progress", m_bus_parser.get_source_name(bus_req.opcode, bus_req.source), bus_req.id), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB }, m_etlink_rtl_port.get_rtl_name())
            end
            m_request_queues[bus_tag].push(req_desc);

            if (req_desc.get_bus_cmd() == ET_LINK_REQ_ReadCoop) begin
               if (m_coop_id_queues[req_desc.get_bus_coop_id()].size() > 0) begin
                  `dut_error(get_abstract_name(), $sformatf("received a request with coop_id %0d, but there is already one in progress: %0s", req_desc.get_bus_coop_id(), m_coop_id_queues[req_desc.get_bus_coop_id()][0].sprint_obj()), pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB }, m_etlink_rtl_port.get_rtl_name())
               end
               m_coop_id_queues[req_desc.get_bus_coop_id()].push_back(req_desc);
            end

            // also add the request to the list that is checked for timeouts
            m_verif_comp.add_active_mem_desc(req_desc);
         end
      end
   endtask : process_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get response objects from the interface, transfer the information
   // into a request descriptor, then send it on the obs_rsp_port.
   //
   task process_responses();
      evl_req_desc         snt_req_desc;
      evl_req_desc         rcv_req_desc;
      evl_xword_byte_en_t  xword_byte_en;
      evl_xword_t          xword_data;
      evl_paddr_t          req_addr;
      evl_line_data        line_data;
      evl_etlink_bus_rsp_s bus_rsp;
      bit                  found_request;
      int                  bus_tag;
      string               err_messages[$];

      forever begin
         found_request = 1'b0;
         m_etlink_rtl_port.get_obs_rsp(bus_rsp);

         `evl_log(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), $sformatf("etlink_monitor got a response with cmd %p, source 0x%x, id 0x%x bus_tag=%0x", bus_rsp.opcode, bus_rsp.dest, bus_rsp.id, {bus_rsp.id, bus_rsp.dest} ))

         //
         // If we are receiving a message, don't try to find anything in the request queue
         // Instead, ask the downstream component if it knows about it.
         //
         if (bus_rsp.opcode inside { ET_LINK_RSP_MsgRcvData }) begin
            // For shared response channel (Minion's case), only one instance handles messages, the other drops it
            if (m_etlink_rtl_port.get_int_param(pk_ignore_message)) begin
               continue;
            end
            if (m_slave_port_agent != null) begin
               case (bus_rsp.data_size[2:0])
                  3'b000:  xword_byte_en = 64'h1;
                  3'b001:  xword_byte_en = 64'h3;
                  3'b010:  xword_byte_en = 64'hF;
                  3'b011:  xword_byte_en = 64'hFF;
                  3'b100:  xword_byte_en = 64'hFFFF;
                  3'b101:  xword_byte_en = 64'hFFFF_FFFF;
                  default: xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
               endcase

               // ET LINK L2 Agent is not implementing find_observed_response() therefore slow-pair messages are not working
               // NeighSB takes care of correctly checking TensorReduce slow-pair messages
               snt_req_desc = m_slave_port_agent.find_observed_response(m_port_id, null, 1'b1, byte'(bus_rsp.cached), evl_bus_rsp_type_t'(ET_LINK_RSP_MsgRcvData), bus_rsp.id, bus_rsp.dest, xword_byte_en, evl_xword_t'(bus_rsp.data));
               if (snt_req_desc != null) begin
                  found_request = 1'b1;
                  if (m_slave_port_agent.is_rsp_checked(snt_req_desc) == 1'b1) begin
                     snt_req_desc.set_observed();
                  end
               end
               else begin
                  `dut_error(get_abstract_name(), $sformatf("This %p, source 0x%x, id 0x%x bus_tag=%0x was unexpected!", bus_rsp.opcode, bus_rsp.dest, bus_rsp.id, {bus_rsp.id, bus_rsp.dest}), pk_warning_on_unmatched_etlink_response, { VMOD_ETLINK_SB })
               end
            end
         end
         else begin
            // Minion has 2 ETLink request channel but they share the same response bus
            // IDs are used to determine which request channel this response belongs to.
            if (m_etlink_rtl_port.is_id_ignored(bus_rsp.id) == 1'b1) continue;

            // find this id in the request queue
            bus_tag = gen_bus_tag_id(bus_rsp.id, bus_rsp.dest);

            if (!m_request_queues.exists(bus_tag)) begin
               m_request_queues[bus_tag] = new($sformatf("%0s(REQQ-%04x)", get_root_abstract_name(), bus_tag));
            end

            snt_req_desc = m_request_queues[bus_tag].find_oldest_rsp();
//          snt_req_desc = m_request_queues[bus_tag].get_desc(0);
//          if (snt_req_desc != null) begin
//             if (snt_req_desc.bus_cmd_has_unique_tag() == 1'b0) begin
//                snt_req_desc = m_request_queues[bus_tag].find_oldest_rsp();
//             end
//          end

            // Special case for ReadCoop response from SC to Neigh. We need to ask the SC scoreboard for help
            // finding the corresponding request.
            if ((snt_req_desc == null) && (m_slave_port_agent != null)) begin
               `evl_log(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), $sformatf("Didn't find a predicted response in our request queues.  Asking the downstream scoreboard..."))
               snt_req_desc = m_slave_port_agent.find_observed_response(m_port_id, null, 1'b1, 0, evl_bus_rsp_type_t'(bus_rsp.opcode), bus_rsp.id, bus_rsp.dest, ~evl_xword_byte_en_t'(0), evl_xword_t'(bus_rsp.data));
            end

            // Assumption: There's only one request outstanding with this tag_id
            if (snt_req_desc == null) begin
               `dut_error(get_abstract_name(), $sformatf("received a response with destination %s (0x%x), ID 0x%x, but there is no matching pending request", m_bus_parser.get_source_name(bus_rsp.opcode, bus_rsp.dest), bus_rsp.dest, bus_rsp.id), pk_warning_on_unmatched_etlink_response, { VMOD_ETLINK_SB })
            end
            else begin
               found_request = 1'b1;
               `evl_log_mdesc(UVM_HIGH, VMOD_ETLINK_MON, get_abstract_name(), snt_req_desc, $sformatf("Found the request matching the response just observed on the bus (%0s)", snt_req_desc.sprint_obj()))
               // Also remove it from the m_active_mem_descs queue that checks for timeouts.
               m_verif_comp.delete_active_mem_desc(snt_req_desc);

               // Remove from the used coop_id queue to allow the same coop_id to be reused
               if (snt_req_desc.get_bus_cmd() == ET_LINK_REQ_ReadCoop) begin
                  void'(m_coop_id_queues[snt_req_desc.get_bus_coop_id()].pop_front());
               end
            end
         end

         // if we didn't find the request, then create a req_desc
         if (found_request == 1'b0) begin
            $cast(snt_req_desc, m_template_req_desc.clone());
            snt_req_desc.set_bus_rsp_type(bus_rsp.opcode);
            snt_req_desc.set_bus_req_size(bus_rsp.data_size);
            snt_req_desc.set_bus_rsp_id(bus_rsp.id);
            snt_req_desc.set_bus_rsp_dest(bus_rsp.dest);
            if (bus_rsp.opcode inside { ET_LINK_RSP_MsgRcvData }) begin
               snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData));
               snt_req_desc.set_bus_rsp_size(bus_rsp.data_size);
            end
            else if (bus_rsp.opcode inside { ET_LINK_RSP_Ack }) begin
               snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_Write));
               snt_req_desc.set_bus_req_id(bus_rsp.id);
               snt_req_desc.set_bus_req_source(bus_rsp.dest);
            end
            else begin
               snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_Read));
               snt_req_desc.set_bus_req_id(bus_rsp.id);
               snt_req_desc.set_bus_req_source(bus_rsp.dest);
            end
            snt_req_desc.set_bus_rsp_valid(0, m_etlink_rtl_port.get_cycle_count());
            snt_req_desc.reset_create_time();
         end

         line_data = null;
         if (snt_req_desc.bus_rsp_has_data(evl_bus_rsp_type_t'(bus_rsp.opcode)) == 1'b1) begin
            evl_line_qword_en_t exp_qwen;

            if (bus_rsp.wdata == 1'b0) begin
               evl_bus_rsp_type_t bus_rsp_type = evl_bus_rsp_type_t'(bus_rsp.opcode);

               err_messages.push_back($sformatf("expecting wdata to be 1 for response type %0p (%s)", bus_rsp_type, snt_req_desc.sprint_obj()));
            end

            req_addr = (bus_rsp.opcode == ET_LINK_RSP_MsgRcvData) ? evl_paddr_t'(0) : snt_req_desc.get_paddr();
            xword_data = bus_rsp.data << (m_rdata_bytes * 8 * (req_addr[5:0] >> m_rdata_bytes_log2));

            case (bus_rsp.data_size[2:0])
               3'b000:  xword_byte_en = 64'h1         <<  req_addr[5:0];
               3'b001:  xword_byte_en = 64'h3         << {req_addr[5:1], 1'b0};
               3'b010:  xword_byte_en = 64'hF         << {req_addr[5:2], 2'b0};
               3'b011:  xword_byte_en = 64'hFF        << {req_addr[5:3], 3'b0};
               3'b100:  xword_byte_en = 64'hFFFF      << {req_addr[5:4], 4'b0};
               3'b101:  xword_byte_en = 64'hFFFF_FFFF << {req_addr[5], 5'b0};
               default: xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
            endcase

            $cast(line_data, m_template_req_data.clone());
            line_data.set_xword(0, xword_data, xword_byte_en);

            //Check the qwen
            exp_qwen = {((xword_byte_en[63:48] == 16'h0) ? 1'b0 : 1'b1),
                        ((xword_byte_en[47:32] == 16'h0) ? 1'b0 : 1'b1),
                        ((xword_byte_en[31:16] == 16'h0) ? 1'b0 : 1'b1),
                        ((xword_byte_en[15:0]  == 16'h0) ? 1'b0 : 1'b1)};
            if (bus_rsp.qwen != exp_qwen) begin
               err_messages.push_back($sformatf("expected the response qwen to be 0x%x based on req_addr 0x%x and size field %0d, but the qwen on the bus was 0x%x (%s)", exp_qwen, req_addr, bus_rsp.data_size, bus_rsp.qwen, snt_req_desc.sprint_obj()));
            end
         end

         //
         // If the request has been observed by a downstream object, perform bus checks.
         // FUTURE: Emmanuel Marie move this checking in etlink_verif_comp
         //
         if (snt_req_desc.has_been_observed() == 1'b1) begin
            evl_line_data rsp_data;

            rsp_data = snt_req_desc.get_rsp_data();

            if ((rsp_data == null) && (snt_req_desc.bus_rsp_has_data(evl_bus_rsp_type_t'(bus_rsp.opcode)) == 1'b1)) begin
               err_messages.push_back($sformatf("not expecting rsp data to be null (%s)", snt_req_desc.sprint_obj()));
            end

            if (snt_req_desc.get_bus_rsp_type() == `EVL_Rsp_Null) begin
               err_messages.push_back($sformatf("unpredicted response received (%s)", snt_req_desc.sprint_obj()));
            end
            else if (snt_req_desc.get_bus_rsp_type() != evl_bus_rsp_type_t'(bus_rsp.opcode)) begin
               evl_bus_rsp_type_t bus_rsp_type = evl_bus_rsp_type_t'(bus_rsp.opcode);

               err_messages.push_back($sformatf("expected the response type to be %s, not %s (%0s)", snt_req_desc.get_bus_rsp_name(), snt_req_desc.get_bus_rsp_name(bus_rsp_type), snt_req_desc.sprint_obj()));
            end

            if (snt_req_desc.get_bus_rsp_dest() != evl_bus_rsp_dest_t'(bus_rsp.dest)) begin
               // (Final agents) such as the Core ET-Link interfaces don't use the dest field
               if (m_etlink_rtl_port.get_int_param(pk_ignore_dst_rsp, 0) == 0) begin
                  err_messages.push_back($sformatf("expected the response destination to be %s, not %s (%0s)", m_bus_parser.get_source_name(snt_req_desc.get_bus_cmd(), snt_req_desc.get_bus_rsp_dest()), m_bus_parser.get_source_name(snt_req_desc.get_bus_cmd(), bus_rsp.dest), snt_req_desc.sprint_obj()));
               end
            end

            if ((line_data != null) && (rsp_data != null)) begin
               string line_err_messages[];

               if (line_data.compare_data_and_byte_ens(rsp_data, line_err_messages) == 1'b1) begin
                  foreach (line_err_messages[ii]) begin
                     err_messages.push_back($sformatf("%0s (%0s)", line_err_messages[ii], snt_req_desc.sprint_obj()));
                  end
               end
            end
         end

         if (line_data != null) begin
            snt_req_desc.set_rsp_data(line_data);
         end
         snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode));
         snt_req_desc.set_bus_rsp_dest(bus_rsp.dest);
         snt_req_desc.set_bus_rsp_valid(0);
         snt_req_desc.set_bus_rsp_id(bus_rsp.id);
         snt_req_desc.set_bus_rsp_size(bus_rsp.data_size);
         `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), snt_req_desc, snt_req_desc.sprint_rsp())

         foreach (err_messages[ii]) begin
            `dut_error_mdesc(m_verif_comp.get_abstract_name(), snt_req_desc, err_messages[ii], pk_sc_etlink_error_intensity, { VMOD_ETLINK_SB }, m_etlink_rtl_port.get_rtl_name())
         end
         if (err_messages.size() > 0) begin
            if (m_request_queues.exists(bus_tag) && (m_request_queues[bus_tag].size() > 1)) begin
               int limit = m_request_queues[bus_tag].size();

               `evl_log(UVM_LOW, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), $sformatf("Contents of request queue for bus tag 0x%0x:", bus_tag))
               for (int ii = 0; ii < limit; ii++) begin
                  evl_req_desc q_req_desc;

                  q_req_desc = m_request_queues[bus_tag].get_desc(ii);
                  if (q_req_desc.get_bus_rsp_type() == `EVL_Rsp_Null) begin
                     `evl_log_mdesc(UVM_LOW, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), q_req_desc, $sformatf("entry %0d: %s (RspNull)", ii, q_req_desc.sprint_obj()))
                  end
                  else begin
                     `evl_log_mdesc(UVM_LOW, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), q_req_desc, $sformatf("entry %0d: %s (%s, %0d)", ii, q_req_desc.sprint_obj(), q_req_desc.get_bus_rsp_name(), q_req_desc.get_rsp_cycle))
                  end
               end
            end
            `evl_log(UVM_LOW, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), $sformatf("Request in question:"))
            snt_req_desc.print_from_root(int'(UVM_LOW), VMOD_GLOBAL);
         end
         err_messages = {};

         if (bus_rsp.opcode inside { ET_LINK_RSP_MsgRcvData }) begin
            snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData));
         end

         //
         // Remove the request descriptor from the request queue.
         //
         if (m_request_queues.exists(bus_tag)) begin
            void'(m_request_queues[bus_tag].pop(snt_req_desc));
         end

//       snt_req_desc.set_done();
         m_obs_rsp_port.write(snt_req_desc);
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_resets
   //
   task watch_resets();
      if (m_rtl_port == null) begin
         return;
      end
      forever begin
         m_etlink_rtl_port.wait_for_reset_assertion();
         m_etlink_rtl_port.wait_cycles(1);
         foreach (m_request_queues[ii]) begin
            if (m_request_queues[ii] != null) begin
               m_request_queues[ii].delete_all_descs();
            end
         end
         foreach (m_coop_id_queues[ii]) begin
            m_coop_id_queues[ii] = {};
         end
      end
   endtask : watch_resets


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(ETLINK)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(ETLINK)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(ETLINK)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(ETLINK)", get_root_abstract_name()));

      m_template_trans_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_trans_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_trans_desc.set_core_id(m_verif_comp.get_core_id());
      m_template_trans_desc.set_core_type(m_verif_comp.get_core_type());

      m_template_req_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_req_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_req_desc.set_core_id(m_verif_comp.get_core_id());
      m_template_req_desc.set_core_type(m_verif_comp.get_core_type());

      m_template_line_desc.set_data_template(m_template_req_data);
      m_template_trans_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_ETLINK);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

      fork
         watch_resets();
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
      fork
         process_requests();
         process_responses();
      join_none;

      m_wdata_bytes      = m_etlink_rtl_port.get_int_param(pk_wdata_bytes);
      m_rdata_bytes      = m_etlink_rtl_port.get_int_param(pk_rdata_bytes);
      m_wdata_bytes_log2 = $clog2(m_wdata_bytes);
      m_rdata_bytes_log2 = $clog2(m_rdata_bytes);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_etlink_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
      if (`et_test_plusargs("disableETLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_warning_on_unmatched_etlink_response, UVM_HIGH)
         `evl_set_error_intensity(pk_sc_etlink_error_intensity, UVM_HIGH)
      end
      if (`et_test_plusargs("enableETLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_warning_on_unmatched_etlink_response, -1)
         `evl_set_error_intensity(pk_sc_etlink_error_intensity, -1)
      end
      `ifdef ET_AMOCMPEXCH_FIX
         m_amocmpexch_fix = 1'b1;
      `else // ifdef ET_AMOCMPEXCH_FIX
         m_amocmpexch_fix = (`et_test_plusargs("amocmpexch_fix") != 0) ? 1'b1 : 1'b0;
      `endif // !ifdef ET_AMOCMPEXCH_FIX
   endfunction : new

endclass: evl_etlink_monitor

//--------------------------------------------------------------------------------------------------
//
// Class: evl_tlink_monitor
//
class evl_tlink_monitor extends evl_monitor;

   `uvm_component_utils(evl_tlink_monitor)

   static evl_tlink_bus_parser m_bus_parser = evl_tlink_bus_parser::get_inst();

   `evl_param_key(pk_allow_unique_tags)
   `evl_param_key(pk_check_for_early_acquire)
   `evl_param_key(pk_rdata_bytes)
   `evl_param_key(pk_bus_rsp_count)
   `evl_param_key(pk_tlink_error_intensity)
   `evl_param_key(pk_warning_on_unmatched_tlink_response)
   `evl_param_key(pk_wdata_bytes)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_tlink_rtl_port:
   // m_port_id:
   // m_wdata_bytes:
   // m_rdata_bytes:
   // m_wdata_bytes_log2:
   // m_rdata_bytes_log2:
   // m_verif_comp:
   // m_master_port_agents[]:
   // m_req_channel_names[]:
   // m_rsp_channel_names[]:
   // m_request_queues[]:
   // m_template_req_data:
   // m_template_line_desc:
   // m_template_trans_desc:
   // m_template_req_desc:
   //
   evl_tlink_rtl_port m_tlink_rtl_port;
   int                m_port_id                 = 0;
   int                m_wdata_bytes             = 16;
   int                m_rdata_bytes             = 16;
   int                m_wdata_bytes_log2        = 4;
   int                m_rdata_bytes_log2        = 4;
   bit                m_check_for_early_acquire = 1'b0;
   evl_verif_comp     m_verif_comp              = null;
   evl_verif_comp     m_master_port_agents[$];
   string             m_req_channel_names[int];
   string             m_rsp_channel_names[int];
   evl_req_desc_queue m_request_queues[int];
   evl_req_desc       m_pend_wb[evl_paddr_t];
   evl_line_data      m_template_req_data;
   evl_line_desc      m_template_line_desc;
   evl_trans_desc     m_template_trans_desc;
   evl_req_desc       m_template_req_desc;


   //-----------------------------------------------------------------------------------------------
   //
   // TLM Ports
   //
   // m_obs_req_port:
   // m_obs_rsp_port:
   //
   uvm_analysis_port #(evl_req_desc) m_obs_req_port;
   uvm_analysis_port #(evl_req_desc) m_obs_rsp_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_queue_tag
   //
   function int gen_bus_queue_tag(input evl_bus_req_id_t id, input int channel);
      return ((int'(channel) << `EVL_BUS_REQ_ID_WIDTH) | int'(id));
   endfunction : gen_bus_queue_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      if ((port_id >= 0) && (port_id < m_master_port_agents.size())) begin
         m_master_port_agents[port_id] = agent_in;
         if (m_master_port_agents[port_id] != null) begin
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("setting %0s as a master agent for %0s channel %0s", m_master_port_agents[port_id].get_abstract_name(), get_abstract_name(), m_req_channel_names[port_id]))
         end
      end
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      if ((port_id < 0) || (port_id >= m_master_port_agents.size())) begin
         return null;
      end
      return m_master_port_agents[port_id];
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_requests
   //
   // Get request objects from the interface, transfer the information into a request descriptor,
   // then send it on the observed request port.
   //
   task process_requests();
      evl_req_desc        req_desc;
      evl_req_desc        exp_req_desc;
      evl_line_data       line_data;
      evl_xword_t         xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_tlink_bus_req_s bus_req;
      int                 channel;
      int                 channel_port;
      int                 bus_queue_tag;
      evl_bus_req_id_t    bus_tag;

      forever begin
         m_tlink_rtl_port.get_obs_req(bus_req);

         exp_req_desc = null;
         channel      = int'(bus_req.opcode) & `EVL_TL_REQ_CHANNEL_MASK;
         channel_port = channel >> `EVL_TL_REQ_CHANNEL_SHIFT;
         $cast(req_desc, m_template_req_desc.clone());
         req_desc.mark_as_bus_child();
         req_desc.set_bus_cmd(bus_req.opcode);
         req_desc.set_paddr(bus_req.address);
         req_desc.set_bus_req_size(bus_req.cmd_size);
         req_desc.set_bus_req_id(bus_req.source);
         req_desc.set_bus_req_param(evl_bus_subopcode_t'(bus_req.param));
         req_desc.set_bus_rsp_id(~evl_bus_req_id_t'(0));
         bus_tag       = bus_req.source;
         bus_queue_tag = gen_bus_queue_tag(bus_req.source, channel_port);

         //
         // The following code fragment will link the observed request to a predicted request.  This
         // will cause the transaction ID to be set appropriately.  For TileLink requests, it is
         // possible to have a zero-time issue (i.e., a predicted request will arrive in the same
         // time tick, but it has not arrived yet).  To accommodate this, if a null descriptor is
         // returned, we will wait for zero time and try once more.
         //
         if (m_master_port_agents[channel_port] != null) begin
            exp_req_desc = m_master_port_agents[channel_port].find_observed_request(TLINK_BUS_AGENT, m_port_id, channel_port, req_desc);
            if (exp_req_desc == null) begin
               m_verif_comp.wait_zero_time(1);
               exp_req_desc = m_master_port_agents[channel_port].find_observed_request(TLINK_BUS_AGENT, m_port_id, channel_port, req_desc);
            end
            if (exp_req_desc == req_desc) begin
               exp_req_desc = null;
            end
            else begin
               req_desc.set_parent_check();
               if (exp_req_desc != null) begin
                  exp_req_desc.set_bus_req_desc(req_desc);
                  req_desc.set_core_id(exp_req_desc.get_core_id());
                  req_desc.set_correlated();
                  req_desc.reset_create_time();
               end
               else if ((req_desc.get_parent_req_desc() == null) && (!(req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }))) begin
                  req_desc.set_trans_id();
               end
            end
         end

         //
         // If the upstream component didn't predict anything for this request create a transaction
         // descriptor out of thin air to set it as the root
         //
         if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
            if (req_desc.get_root_trans_desc() == null) begin
               evl_cmd_t              cmd;
               evl_tlink_req_opcode_t bus_cmd;
               evl_trans_desc         trans_desc;

               bus_cmd = evl_tlink_req_opcode_t'(req_desc.get_bus_cmd());
               $cast(trans_desc, m_template_trans_desc.clone());
               if (req_desc.bus_cmd_is_atomic() == 1'b1) begin
                  cmd = ATOMIC;
               end
               else if (req_desc.bus_cmd_is_write() == 1'b1) begin
                  cmd = ST;
               end
               else begin
                  cmd = LD;
               end
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

         if (req_desc.bus_cmd_has_write_data() == 1'b1) begin
            $cast(line_data, m_template_req_data.clone());

            xword_data = evl_xword_t'(bus_req.data) << (m_wdata_bytes * 8 * (bus_req.address[5:0] >> m_wdata_bytes_log2));

            case (bus_req.cmd_size)
               3'b000:  xword_byte_en = 64'h1         << bus_req.address[5:0];
               3'b001:  xword_byte_en = 64'h3         << { bus_req.address[5:1], 1'b0 };
               3'b010:  xword_byte_en = 64'hF         << { bus_req.address[5:2], 2'b00 };
               3'b011:  xword_byte_en = 64'hFF        << { bus_req.address[5:3], 3'b000 };
               3'b100:  xword_byte_en = 64'hFFFF      << { bus_req.address[5:4], 4'b0000 };
               3'b101:  xword_byte_en = 64'hFFFF_FFFF << { bus_req.address[5],   5'b00000 };
               default: xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
            endcase

            line_data.set_xword(0, xword_data, xword_byte_en);
            req_desc.set_req_data(line_data);
         end // if (req_desc.bus_cmd_has_write_data() == 1'b1)

         m_obs_req_port.write(req_desc);
         req_desc.set_issued();

         //
         // Add the request to the appropriate request queue.
         //
         if (!m_request_queues.exists(bus_queue_tag)) begin
            m_request_queues[bus_queue_tag] = new($sformatf("%0s(REQQ-%04x)", get_root_abstract_name(), bus_queue_tag));
         end
         if ((get_int_param(pk_allow_unique_tags, 0) != 0) && (m_request_queues[bus_queue_tag].size() > 0) && (req_desc.bus_cmd_has_unique_tag() == 1'b1)) begin
            `dut_error(get_abstract_name(), $sformatf("received a request on channel %s (ID 0x%02x(0x%04x)), but there is already one in progress", m_req_channel_names[channel], bus_tag, bus_queue_tag), pk_tlink_error_intensity, { VMOD_TLINK_SB }, m_tlink_rtl_port.get_rtl_name())
         end
         m_request_queues[bus_queue_tag].push(req_desc);

         if (m_check_for_early_acquire == 1'b1) begin
            evl_paddr_t line_addr = req_desc.get_paddr() & const_line_paddr_mask;

            if (channel == `EVL_TL_REQ_CHANNEL_A) begin
               if (m_pend_wb.exists(line_addr)) begin
                  `dut_warning_mdesc(get_abstract_name(), req_desc, $sformatf("received a request for 0x%x on channel %s, but %0s is in progress", line_addr, m_req_channel_names[channel], m_pend_wb[line_addr].sprint_obj())) // , pk_tlink_error_intensity, { VMOD_TLINK_SB }, m_tlink_rtl_port.get_rtl_name())
               end
            end
            else if (channel == `EVL_TL_REQ_CHANNEL_C) begin
               if (m_pend_wb.exists(line_addr)) begin
                  `dut_warning_mdesc(get_abstract_name(), req_desc, $sformatf("received a request for 0x%x on channel %s, but %0s is in progress", line_addr, m_req_channel_names[channel], m_pend_wb[line_addr].sprint_obj())) // , pk_tlink_error_intensity, { VMOD_TLINK_SB }, m_tlink_rtl_port.get_rtl_name())
               end
               m_pend_wb[line_addr] = req_desc;
            end
         end

         //
         // Add the request to the list that is checked for timeouts.
         //
         m_verif_comp.add_active_mem_desc(req_desc);
         `evl_log_mdesc(UVM_HIGH, VMOD_TLINK_MON, get_abstract_name(), req_desc, req_desc.sprint_obj(1))
      end
      return;
   endtask : process_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get response objects from the interface, transfer the information into a request descriptor,
   // and then send it on the obs_rsp_port to be processed by the Verification Component.  Note that
   // we only send a response when all beats have been received (so the response will be printed
   // when the last beat is captured).
   //
   task process_responses();
      evl_req_desc        snt_req_desc;
      evl_xword_byte_en_t xword_byte_en;
      evl_xword_t         xword_data;
      evl_paddr_t         req_addr;
      evl_line_data       line_data;
      evl_tlink_bus_rsp_s bus_rsp;
      bit                 found_request;
      int                 channel;
      evl_bus_req_id_t    bus_tag;
      int                 bus_queue_tag;
      string              err_messages[$];

      forever begin
         m_tlink_rtl_port.get_obs_rsp(bus_rsp);

         err_messages  = {};
         found_request = 1'b0;

         //
         // For channel E responses, we need to use the sink to determine the bus queue tag.
         // TLD_ReleaseAck responses only occur for channel C requests, so the corresponding bus
         // queue tag will be based on the source and channel C.  The bus queue tag for all other
         // responses will be base on the source and the channel bits.
         //
         channel = int'(bus_rsp.opcode) & `EVL_TL_RSP_CHANNEL_MASK;
         if (channel == `EVL_TL_RSP_CHANNEL_E) begin
            bus_tag = bus_rsp.sink;
         end
         else begin
            bus_tag = bus_rsp.source;
         end
         if (bus_rsp.opcode == TLD_ReleaseAck) begin
            bus_queue_tag = gen_bus_queue_tag(bus_tag, (`EVL_TL_REQ_CHANNEL_C >> `EVL_TL_RSP_CHANNEL_SHIFT));
         end
         else begin
            bus_queue_tag = gen_bus_queue_tag(bus_tag, (channel >> `EVL_TL_RSP_CHANNEL_SHIFT));
         end

         if (!m_request_queues.exists(bus_queue_tag)) begin
            m_request_queues[bus_queue_tag] = new($sformatf("%0s(REQQ-%04x)", get_root_abstract_name(), bus_queue_tag));
         end

         //
         // Pop the predicted request descriptor from the appopriate queue.  The downstream agent
         // should have filled in the response information appropriately.
         //
         snt_req_desc = m_request_queues[bus_queue_tag].pop();

         //
         // If we did not find the request, create a request descriptor (this prevents any
         // subsequent null descriptor issues).  If we have a valid descriptor, clean things up as
         // necessary.
         //
         if (snt_req_desc == null) begin
            err_messages.push_back($sformatf("received a response for ID 0x%02x(0x%04x) on channel %s, but there is no matching pending request", bus_tag, bus_queue_tag, m_rsp_channel_names[channel]));
            $cast(snt_req_desc, m_template_req_desc.clone());
            case (bus_rsp.opcode)
               TLD_AccessAck:     snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_PutFull));
               TLD_AccessAckData: snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_Get));
               TLD_HintAck:       snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_Intent));
               TLD_Grant:         snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_AcquirePerm));
               TLD_GrantData:     snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_AcquireBlock));
               TLD_ReleaseAck:    snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLC_ReleaseData));
               TLC_AccessAck:     snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_PutFull));
               TLC_AccessAckData: snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_Get));
               TLC_HintAck:       snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLB_Intent));
               TLC_ProbeAck:      snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLB_ProbeBlock));
               TLC_ProbeAckData:  snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLB_ProbeBlock));
               TLE_GrantAck:      snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TLA_AcquireBlock));
               default:           snt_req_desc.set_bus_cmd(evl_bus_cmd_t'(TL_ReqUnknown));
            endcase
            snt_req_desc.set_paddr(~(evl_paddr_t'(0)) & const_line_paddr_mask);
            if (bus_rsp.denied == 1'b1) begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode) | evl_bus_rsp_type_t'(`EVL_TL_RSP_DENIED));
            end
            else if (bus_rsp.corrupt == 1'b1) begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode) | evl_bus_rsp_type_t'(`EVL_TL_RSP_CORRUPT));
            end
            else begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode));
            end
            snt_req_desc.set_bus_req_size(bus_rsp.data_size);
            snt_req_desc.set_bus_req_id(bus_tag);
            snt_req_desc.set_bus_rsp_valid(0, m_tlink_rtl_port.get_cycle_count());
            snt_req_desc.reset_create_time();
         end
         else begin
            //
            // Remove the request from the m_active_mem_descs queue that checks for timeouts if
            // there will not be an additional response for it.  Note that acquire requests require
            // a final grant acknowledge on channel E.
            //
            `evl_log_mdesc(UVM_HIGH, VMOD_TLINK_MON, get_abstract_name(), snt_req_desc, $sformatf("found the request matching the response just observed on the channel %0s (%0s)", m_rsp_channel_names[channel], snt_req_desc.sprint_obj()))
            if ((channel == `EVL_TL_RSP_CHANNEL_E) || (snt_req_desc.bus_cmd_is_acquire() == 1'b0)) begin
               m_verif_comp.delete_active_mem_desc(snt_req_desc);
            end
            found_request = 1'b1;
         end

         //
         // If the original request is an acquire and this is not a grant acknowledge, push the
         // request descriptor on a channel E queue ... this will allow us to correlate the channel
         // E response when it occurs.
         //
         if (bus_rsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
            int ack_bus_tag;

            ack_bus_tag = gen_bus_queue_tag(bus_rsp.sink, (`EVL_TL_RSP_CHANNEL_E >> `EVL_TL_RSP_CHANNEL_SHIFT));
            if (!m_request_queues.exists(ack_bus_tag)) begin
               m_request_queues[ack_bus_tag] = new($sformatf("%0s(REQQ-%04x)", get_root_abstract_name(), ack_bus_tag));
            end
            m_request_queues[ack_bus_tag].push(snt_req_desc);
         end

         //
         // If this is a channel E response, then there will be no data or additional response
         // information associated with it ... make sure we can accept a channel E response, though.
         //
         if (channel == `EVL_TL_RSP_CHANNEL_E) begin
            if (snt_req_desc.get_int_param(pk_bus_rsp_count, 0) != 1) begin
               err_messages.push_back($sformatf("received an unexpected channel E response for %0s", snt_req_desc.sprint_obj()));
            end
            if (!(evl_tlink_rsp_opcode_t'(snt_req_desc.get_bus_rsp_type()) inside { TLD_Grant, TLD_GrantData })) begin
               err_messages.push_back($sformatf("unpredicted channel E response received (%s)", snt_req_desc.sprint_obj()));
            end
            snt_req_desc.set_bus_rsp_id(bus_tag, 1);
            snt_req_desc.incr_int_param(pk_bus_rsp_count);
            snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode), 1);
            snt_req_desc.set_bus_rsp_valid(1, m_tlink_rtl_port.get_cycle_count());
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), snt_req_desc, $sformatf("%p", bus_rsp.opcode))
         end
         else begin
            //
            // This is not a channel E response, so check for data.
            //
            line_data = null;
            if (snt_req_desc.bus_rsp_has_data(evl_bus_rsp_type_t'(bus_rsp.opcode)) == 1'b1) begin
               req_addr   = snt_req_desc.get_paddr();
               if (channel == `EVL_TL_RSP_CHANNEL_C) begin
                  xword_data    = evl_xword_t'(bus_rsp.data);
                  xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
               end
               else begin
                  xword_data = evl_xword_t'(bus_rsp.data) << (m_rdata_bytes * 8 * (req_addr[5:0] >> m_rdata_bytes_log2));
                  case (bus_rsp.data_size[2:0])
                     3'b000:  xword_byte_en = 64'h1         <<   req_addr[5:0];
                     3'b001:  xword_byte_en = 64'h3         << { req_addr[5:1], 1'b0 };
                     3'b010:  xword_byte_en = 64'hF         << { req_addr[5:2], 2'b00 };
                     3'b011:  xword_byte_en = 64'hFF        << { req_addr[5:3], 3'b000 };
                     3'b100:  xword_byte_en = 64'hFFFF      << { req_addr[5:4], 4'b0000 };
                     3'b101:  xword_byte_en = 64'hFFFF_FFFF << { req_addr[5],   5'b00000 };
                     default: xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
                  endcase
               end

               $cast(line_data, m_template_req_data.clone());
               line_data.set_xword(0, xword_data, xword_byte_en);
            end

            //
            // If the request has been observed by a downstream object, perform bus checks.
            //
            if (snt_req_desc.has_been_observed() == 1'b1) begin
               evl_line_data      rsp_data;
               evl_bus_rsp_type_t rsp_type;

               rsp_type = evl_bus_rsp_type_t'(bus_rsp.opcode);
               if (bus_rsp.denied == 1'b1) begin
                  rsp_type |= evl_bus_rsp_type_t'(`EVL_TL_RSP_DENIED);
               end
               else if (bus_rsp.corrupt == 1'b1) begin
                  rsp_type |= evl_bus_rsp_type_t'(`EVL_TL_RSP_CORRUPT);
               end
               if (snt_req_desc.get_bus_rsp_type() == `EVL_Rsp_Null) begin
                  err_messages.push_back($sformatf("unpredicted response received (%s)", snt_req_desc.sprint_obj()));
               end
               else if (snt_req_desc.get_bus_rsp_type() != rsp_type) begin
                  err_messages.push_back($sformatf("expected the response type to be %s, not %s (%0s)", snt_req_desc.get_bus_rsp_name(), snt_req_desc.get_bus_rsp_name(rsp_type), snt_req_desc.sprint_obj()));
               end

               rsp_data = snt_req_desc.get_rsp_data();
               if ((rsp_data == null) && (snt_req_desc.bus_rsp_has_data(evl_bus_rsp_type_t'(bus_rsp.opcode)) == 1'b1)) begin
                  err_messages.push_back($sformatf("not expecting response data to be null (%s)", snt_req_desc.sprint_obj()));
               end

               if (evl_tlink_param_t'(snt_req_desc.get_bus_rsp_param()) != bus_rsp.param) begin
                  evl_tlink_param_t tlink_param;

                  tlink_param = evl_tlink_param_t'(snt_req_desc.get_bus_rsp_param());
                  if (tlink_param == TL_param_none) begin
                     err_messages.push_back($sformatf("a response parameter of %p is illegal for %s", bus_rsp.param, snt_req_desc.sprint_obj()));
                  end
                  else begin
                     err_messages.push_back($sformatf("expected a response parameter of %p, not %p (%s)", tlink_param, bus_rsp.param, snt_req_desc.sprint_obj()));
                  end
               end

               if ((line_data != null) && (rsp_data != null)) begin
                  string line_err_messages[];

                  if (channel == `EVL_TL_RSP_CHANNEL_C) begin
                     rsp_data.validate_line();
                     line_data.validate_line();
                  end
                  if ((bus_rsp.corrupt == 1'b0) && (bus_rsp.denied == 1'b0) && (line_data.compare_data_and_byte_ens(rsp_data, line_err_messages) == 1'b1)) begin
                     foreach (line_err_messages[ii]) begin
                        err_messages.push_back($sformatf("%0s (%0s)", line_err_messages[ii], snt_req_desc.sprint_obj()));
                     end
                  end
               end
            end

            //
            // Regardless of whether this has been observed or not, make sure we only get a single C
            // channel or D channel response.
            //
            if (snt_req_desc.get_int_param(pk_bus_rsp_count, 0) != 0) begin
               err_messages.push_back($sformatf("received an unexpected additional response for %0s", snt_req_desc.sprint_obj()));
            end

            if (line_data != null) begin
               snt_req_desc.set_rsp_data(line_data);
            end
            if (bus_rsp.opcode inside { TLD_Grant, TLD_GrantData }) begin
               snt_req_desc.set_bus_rsp_id(bus_rsp.sink);
            end
            else begin
               snt_req_desc.set_bus_rsp_id(bus_tag);
            end
            if (bus_rsp.denied == 1'b1) begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode) | evl_bus_rsp_type_t'(`EVL_TL_RSP_DENIED));
            end
            else if (bus_rsp.corrupt == 1'b1) begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode) | evl_bus_rsp_type_t'(`EVL_TL_RSP_CORRUPT));
            end
            else begin
               snt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(bus_rsp.opcode));
            end
            snt_req_desc.set_bus_rsp_size(bus_rsp.data_size);
            snt_req_desc.set_bus_rsp_param(evl_bus_subopcode_t'(bus_rsp.param));
            snt_req_desc.incr_int_param(pk_bus_rsp_count);
            snt_req_desc.set_bus_rsp_valid(0, m_tlink_rtl_port.get_cycle_count());
            `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, m_verif_comp.get_abstract_name(), snt_req_desc, snt_req_desc.sprint_rsp())
         end
         foreach (err_messages[ii]) begin
            `dut_error_mdesc(m_verif_comp.get_abstract_name(), snt_req_desc, err_messages[ii], pk_tlink_error_intensity, { VMOD_TLINK_SB }, m_tlink_rtl_port.get_rtl_name())
         end
         err_messages = {};

         //
         // Remove the request descriptor from the request queue.
         //
         void'(m_request_queues[bus_queue_tag].pop(snt_req_desc));
         if ((m_check_for_early_acquire == 1'b1) && (bus_rsp.opcode == TLD_ReleaseAck)) begin
            evl_paddr_t line_addr = snt_req_desc.get_paddr() & const_line_paddr_mask;

            m_pend_wb.delete(line_addr);
         end

         m_obs_rsp_port.write(snt_req_desc);
      end
      return;
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_resets
   //
   task watch_resets();
      if (m_rtl_port == null) begin
         return;
      end
      m_check_for_early_acquire = (m_rtl_port.get_int_param(pk_check_for_early_acquire, 0) == 0) ? 1'b0 : 1'b1;
      forever begin
         m_tlink_rtl_port.wait_for_reset_assertion();
         m_tlink_rtl_port.wait_cycles(1);
         foreach (m_request_queues[ii]) begin
            if (m_request_queues[ii] != null) begin
               m_request_queues[ii].delete_all_descs();
            end
         end
      end
   endtask : watch_resets


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(TLINK)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(TLINK)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(TLINK)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(TLINK)", get_root_abstract_name()));

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
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_TLINK);

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
      m_wdata_bytes      = m_tlink_rtl_port.get_int_param(pk_wdata_bytes);
      m_rdata_bytes      = m_tlink_rtl_port.get_int_param(pk_rdata_bytes);
      m_wdata_bytes_log2 = $clog2(m_wdata_bytes);
      m_rdata_bytes_log2 = $clog2(m_rdata_bytes);
      fork
         process_requests();
         process_responses();
      join_none;
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_phase
   //
   virtual function void check_phase(uvm_phase phase);
      super.check_phase(phase);
      foreach (m_request_queues[ii]) begin
         if (m_request_queues[ii].size() > 0) begin
            m_request_queues[ii].print_obj(get_abstract_name());
         end
      end
   endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tlink_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_req_port = new("m_obs_req_port", this);
      m_obs_rsp_port = new("m_obs_rsp_port", this);
      if (`et_test_plusargs("disableTLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_warning_on_unmatched_tlink_response, UVM_HIGH)
         `evl_set_error_intensity(pk_tlink_error_intensity, UVM_HIGH)
      end
      if (`et_test_plusargs("enableTLinkChecks") != 0) begin
         `evl_set_error_intensity(pk_warning_on_unmatched_tlink_response, -1)
         `evl_set_error_intensity(pk_tlink_error_intensity, -1)
      end

      for (int ii = 0; ii <= `EVL_TL_REQ_CHANNEL_MASK; ii++) begin
         m_req_channel_names[ii & `EVL_TL_REQ_CHANNEL_MASK] = "?";
      end
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_A] = "A";
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_B] = "B";
      m_req_channel_names[`EVL_TL_REQ_CHANNEL_C] = "C";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_A >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "A";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_B >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "B";
      m_req_channel_names[(`EVL_TL_REQ_CHANNEL_C >> `EVL_TL_REQ_CHANNEL_SHIFT)] = "C";

      for (int ii = 0; ii <= `EVL_TL_RSP_CHANNEL_MASK; ii++) begin
         m_rsp_channel_names[ii & `EVL_TL_RSP_CHANNEL_MASK] = "?";
      end
      m_rsp_channel_names[`EVL_TL_RSP_CHANNEL_C] = "C";
      m_rsp_channel_names[`EVL_TL_RSP_CHANNEL_D] = "D";
      m_rsp_channel_names[`EVL_TL_RSP_CHANNEL_E] = "E";
      m_rsp_channel_names[(`EVL_TL_RSP_CHANNEL_C >> `EVL_TL_RSP_CHANNEL_SHIFT)] = "C";
      m_rsp_channel_names[(`EVL_TL_RSP_CHANNEL_D >> `EVL_TL_RSP_CHANNEL_SHIFT)] = "D";
      m_rsp_channel_names[(`EVL_TL_RSP_CHANNEL_E >> `EVL_TL_RSP_CHANNEL_SHIFT)] = "E";

      m_master_port_agents.push_back(null);
      m_master_port_agents.push_back(null);
      m_master_port_agents.push_back(null);
   endfunction : new

endclass: evl_tlink_monitor

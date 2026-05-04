//--------------------------------------------------------------------------------------------------
//
// Class: evl_neigh_scoreboard
//
//
class evl_neigh_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_neigh_scoreboard)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_core_req_port)
   `evl_param_key(pk_core_req_port_count)
   `evl_param_key(pk_neighsb_error_intensity)
   `evl_param_key(pk_coop_parents)

   `include "dv/common/base/evl_mem_map_pk_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_neigh_rtl_port m_neigh_rtl_port;

   // port for receiving observed requests from the core dcaches
   // minions have a miss port and an evict port, which are both connected to this export through
   // their respective bus interfaces
   uvm_analysis_export   #(evl_req_desc) m_core_miss_req_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_core_miss_req_fifo[];

   uvm_analysis_export   #(evl_req_desc) m_core_evict_req_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_core_evict_req_fifo[];

   // port for receiving observed responses to the core dcaches
   uvm_analysis_export   #(evl_req_desc) m_core_miss_rsp_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_core_miss_rsp_fifo[];

   uvm_analysis_export   #(evl_req_desc) m_core_evict_rsp_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_core_evict_rsp_fifo[];

   // port for receiving observed requests to the shire
   uvm_analysis_export   #(evl_req_desc) m_shire_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_shire_req_fifo;

   // port for receiving observed responses from the shire
   uvm_analysis_export   #(evl_req_desc) m_shire_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_shire_rsp_fifo;

   // port for receiving observed requests from the core PMUs
   uvm_analysis_export   #(evl_pmu_desc) m_core_pmu_req_export[];
   uvm_tlm_analysis_fifo #(evl_pmu_desc) m_core_pmu_req_fifo[];


   evl_dword_t [8:3][1:0]                m_pmu_counters;


   evl_req_desc                          m_pending_shire_reqs[$];
   evl_req_desc                          m_slowpair_pend_msg_q[$];
   evl_req_desc_queue                    m_core_pend_msg_q[];
   evl_req_desc                          m_store_coop_q[int];

   typedef struct {
      evl_paddr_t                 ctl_paddr;
      evl_bus_neigh_coop_mask_t   neigh_mask;
      evl_bus_minion_coop_mask_t  minion_mask;
      evl_bus_minion_coop_mask_t  minion_obs_mask;
      evl_bus_minion_coop_mask_t  minion_pend_mask;
      evl_req_desc_queue          ctl_q;
   } ctl_operation_t;
   ctl_operation_t                m_read_coop_q[int];

   parameter MAX_LINES_PER_TENSOR_LOAD = 16;
   parameter UC_BOX_BANK               = 4;

   evl_req_desc                          m_active_shire_reqs[$];

   //Neigh Chicken bits (NCB)
   bit [1:0] m_ncb_agent_forced;
   bit       m_ncb_force_all_agents;
   bit [1:0] m_ncb_dest_fifo;
   bit       m_ncb_force_dest_fifo;

   parameter NCB_SOURCE_DCACHE = 0;
   parameter NCB_SOURCE_ICACHE = 1;
   parameter NCB_SOURCE_TBOX   = 2;
   parameter NCB_SOURCE_PTW    = 3;

   parameter NCB_DEST_BANKS    = 0;
   parameter NCB_DEST_UCBLOCK  = 1;
   parameter NCB_DEST_TBOX     = 2;


   function void ncb_set_bus_bank(input evl_req_desc req_desc, input evl_sc_bank_id_t bank, input int req_source_agent);
      if (m_ncb_force_dest_fifo == 1) begin
         evl_sc_bank_id_t tmp_bank;
         // ncb_dest_fifo translation:
         if (m_ncb_dest_fifo == NCB_DEST_BANKS) begin
            evl_paddr_t paddr = req_desc.get_paddr();
            evl_sc_bank_id_t bank_id = m_global_attrs.get_bank_from_address(paddr);
            tmp_bank = bank_id & 4'h3;
         end
         else if (m_ncb_dest_fifo == NCB_DEST_UCBLOCK) begin
            tmp_bank = evl_sc_bank_id_t'(UC_BOX_BANK);
         end

         if (m_ncb_force_all_agents == 1) begin
            bank = tmp_bank;
         end
         else if (req_source_agent == m_ncb_agent_forced) begin
            // 0 : source agent is DCACHE
            // 1 : source agent is ICACHE
            // 2 : source agent is TBOX
            // 3 : source agent is PTW
            bank = tmp_bank;
         end
         else begin
         end
      end

      req_desc.set_bus_bank(bank);
   endfunction : ncb_set_bus_bank


   virtual function void add_active_mem_desc(input evl_mem_desc mem_desc_in);
      int dflt_timeout = mem_desc_in.m_timeout_value;
      evl_req_desc req_desc;
      $cast(req_desc, mem_desc_in);
      if (req_desc.get_bus_cmd() inside {ET_LINK_REQ_ReadCoop, ET_LINK_REQ_WriteAround}) begin
         mem_desc_in.set_timeout_value(dflt_timeout*2);
      end
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), mem_desc_in, $sformatf("Add Active Mem Desc: %s", mem_desc_in.sprint_obj()))
      super.add_active_mem_desc(mem_desc_in);
   endfunction : add_active_mem_desc

   virtual function void delete_active_mem_desc(input evl_mem_desc mem_desc_in);
      `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), mem_desc_in, $sformatf("Delete Active Mem Desc: %s", mem_desc_in.sprint_obj()))
      super.delete_active_mem_desc(mem_desc_in);
   endfunction : delete_active_mem_desc

   virtual function bit compare_msg_rsp_attrs(input  evl_req_desc req_desc,
                                          input  evl_req_desc exp_req_desc,
                                          output string       err_messages_out[]);
      string err_messages[$];

      if (req_desc.get_bus_cmd() != exp_req_desc.get_bus_cmd()) begin
         err_messages.push_back($sformatf("bus_cmd mismatch: have %0p, expected %0p (%s)", req_desc.get_bus_cmd(), exp_req_desc.get_bus_cmd(), req_desc.sprint_obj()));
      end
      if ((exp_req_desc.get_bus_bank() != ~evl_sc_bank_id_t'(0)) && (req_desc.get_bus_bank() != ~evl_sc_bank_id_t'(0)) && (req_desc.get_bus_bank() != exp_req_desc.get_bus_bank())) begin
         err_messages.push_back($sformatf("bus_bank mismatch: have %0d, expected %0d (%s)", req_desc.get_bus_bank(), exp_req_desc.get_bus_bank(), req_desc.sprint_obj()));
      end
      if (req_desc.get_bus_rsp_size() != exp_req_desc.get_bus_rsp_size()) begin
         err_messages.push_back($sformatf("bus_rsp_size mismatch: have %0p, expected %0p (%s)", req_desc.get_bus_rsp_size(), exp_req_desc.get_bus_rsp_size(), req_desc.sprint_obj()));
      end

      if (err_messages.size() > 0) begin
          err_messages_out = new[err_messages.size()];

         foreach(err_messages[ii]) begin
            err_messages_out[ii] = err_messages[ii];
         end
      end

      return (err_messages.size() > 0);

   endfunction : compare_msg_rsp_attrs

   function void process_predicted_request(input  evl_req_desc     obs_req_desc,
                                           ref    evl_req_desc     pending_reqs[$],
                                           ref    evl_req_desc     active_reqs[$],
                                           input  bit              ignore_id     = 1'b0,
                                           input  bit              relaxed_order = 1'b0);
      int pre_pending_size_q, post_pending_size_q;
      int pre_active_size_q, post_active_size_q;

      pre_pending_size_q = pending_reqs.size();
      pre_active_size_q  = active_reqs.size();
      super.process_predicted_request(obs_req_desc, pending_reqs, active_reqs, ignore_id, relaxed_order);

      if (obs_req_desc.get_bus_cmd() inside {ET_LINK_REQ_MsgSendData}) begin
         bit found = 0;
         // For Messages, the neigh expects one request going out but no response comming back
         // so we have to delete it from the m_active_shire_reqs queue
         foreach (m_active_shire_reqs[ii]) begin
            evl_req_desc req_desc = m_active_shire_reqs[ii];
            if ((req_desc.get_paddr()          == obs_req_desc.get_paddr()           ) &&
                (req_desc.get_bus_req_id()     == obs_req_desc.get_bus_req_id()      ) &&
                (req_desc.get_bus_req_source() == obs_req_desc.get_bus_req_source()  ) &&
                (req_desc.get_req_data().get_xword(0) == obs_req_desc.get_req_data().get_xword(0))
            ) begin
               m_active_shire_reqs[ii].set_done();
               m_active_shire_reqs.delete(ii);
               found = 1;
               break;
            end
         end
         if (found == 1'b0) begin
            sb_error("(Cannot find Message in the 'm_active_shire_reqs' queue): Messages that goes to the shire cache must be deleted from the queue", obs_req_desc);
         end
      end

      post_pending_size_q = pending_reqs.size();
      post_active_size_q  = active_reqs.size();
      if (pre_pending_size_q > post_pending_size_q) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), obs_req_desc, $sformatf("Deleting from the m_pending_shire_reqs queue: %s", obs_req_desc.sprint_obj()))
      end
      if (pre_active_size_q < post_active_size_q) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), obs_req_desc, $sformatf("Adding to the m_active_shire_reqs queue: %s", obs_req_desc.sprint_obj()))
      end
   endfunction : process_predicted_request


   //*********************************************************************************************//
   // Message Requests
   //*********************************************************************************************//

   function void process_core_request_messages(inout evl_req_desc req_desc, inout bit is_l2_req, inout bit is_local_msg, int src_core_id);
      bit is_fast_pair = 0;
      bit unknown_msg_type = 0;
       // Extract destination from the physical adddress of the request
      evl_paddr_t dst_p_addr = req_desc.get_paddr();
      int dst_neigh_id = dst_p_addr[`EVL_BUS_MSG_DST_NEIGH_MSB:`EVL_BUS_MSG_DST_NEIGH_LSB];
      int dst_core_id  = dst_p_addr[`EVL_BUS_MSG_DST_MINION_MSB:`EVL_BUS_MSG_DST_MINION_LSB];
      int dst_hart_id  = dst_p_addr[`EVL_BUS_MSG_DST_HART_MSB];
      //int dst_shire_id = dst_p_addr[`EVL_BUS_MSG_SHIRE_ID_MSB:`EVL_BUS_MSG_SHIRE_ID_LSB];
      int dst_shire_id = 0;
      int port_id      = dst_p_addr[`EVL_BUS_MSG_PORT_MSB:`EVL_BUS_MSG_PORT_LSB];
      string obs_msg, pred_msg;
      // 0xFF refers to the local shire id
      dst_shire_id     = (dst_shire_id == 8'hff) ? get_shire_id() : dst_shire_id;

      obs_msg = $sformatf("Core to Neigh ( Observed ): %s (addr 0x%x) request from Neigh:%0d Minion:%0d", req_desc.get_bus_cmd_name(), dst_p_addr, get_neighborhood_id(), src_core_id);

      // Sanity check for destination queue in case the destination queue does not exist (TBOX may not exist..)
      if (m_core_pend_msg_q[dst_core_id] == null) begin
         sb_error("neigh_scoreboard: m_core_pend_msg_q is null");
      end
      // Sanity check just in case the address is not compliant with the spec
      // PRM-19, 2.4.1 Minion ESR Map
      if ((dst_p_addr[39:30] == 11'b00000000100) && (dst_p_addr[21:20] == 2'b00) && (dst_p_addr[11:8] == 4'b1000) && (dst_p_addr[5:0] == 6'b000000)) begin
         obs_msg = $sformatf("%s is a Port Message type", obs_msg);
      end
      else if ((dst_p_addr[31] == 1'b1) && (dst_p_addr[23:22] == 2'b10)) begin
         // Special Messages for non-message operations
         if (dst_p_addr[2:0] == 3'b000) begin
            // Reduce Messages
            if ((dst_p_addr[21:20] == 2'b00) && (dst_p_addr[11:4] == 8'b11100000)) begin
               if (dst_p_addr[3] == 1) begin
                  obs_msg = $sformatf("%s is a ReduceReady message", obs_msg);
               end
               else begin
                  obs_msg = $sformatf("%s is a ReduceData message", obs_msg);
               end
            end
         end
         else if ((dst_p_addr[21:16] == 6'b000000) && (dst_p_addr[11:6] == 6'b110000)) begin
            obs_msg = $sformatf("%s is a TBOS Reply message", obs_msg);
         end
         else if ((dst_p_addr[21:16] == 6'b000000) && (dst_p_addr[11:3] == 9'b111000010)) begin
            obs_msg = $sformatf("%s is a TBOX Pull message", obs_msg);
         end
         else if ((dst_p_addr[21:7] ==? 15'b10000100000x000) && (dst_p_addr[11:3] == 9'b111000010)) begin
            if (dst_p_addr[10] == 0) begin
               obs_msg = $sformatf("%s is a TBOX Minion Req message", obs_msg);
            end
            else begin
               obs_msg = $sformatf("%s is a TBOX Minion Data message", obs_msg);
            end
         end
         else if (dst_p_addr[21:6] == 16'b1111100000000000) begin
            obs_msg = $sformatf("%s is an Atomic response", obs_msg);
         end
         else begin
            unknown_msg_type = 1;
         end
      end
      else begin
         unknown_msg_type = 1;
      end

      if (unknown_msg_type == 1) begin
         sb_error($sformatf("neigh_scoreboard: %s, encoding error, unknown message type", obs_msg));
      end
      else begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
      end

      // According to the PRM-19 messages between certain minions in a neighborhood bypass the L2 path
      // and go straight to the evict ff of the destination minion using a fast path.
      // Adjacent minions within a neighbourhood don't send a request to the L2
      is_fast_pair = ((src_core_id[3:1] == dst_core_id[3:1])  &&
                      (src_core_id      != dst_core_id     )) ||
                     ((src_core_id == 0) && (dst_core_id inside {2, 4})) ||
                     ((src_core_id == 2) && (dst_core_id inside {0   })) ||
                     ((src_core_id == 4) && (dst_core_id inside {0, 6})) ||
                     ((src_core_id == 6) && (dst_core_id inside {4   }));


      // Source and destination neighbourhood and shire id have to be the same in order to use the fast lane or go to the L2
      is_local_msg = (get_neighborhood_id() == dst_neigh_id) && (get_shire_id() == dst_shire_id);
      is_l2_req = !is_local_msg || !is_fast_pair;

      // If it is not an L2 req, we have to enqueue the request into the local pending queue
      if (!is_l2_req) begin
         evl_req_desc fastpair_req_desc;
         $cast(fastpair_req_desc, req_desc.clone());
         fastpair_req_desc.set_bus_rsp_type(ET_LINK_RSP_MsgRcvData);
         fastpair_req_desc.set_bus_rsp_valid(0, get_cycle_count());
         fastpair_req_desc.set_rsp_data(req_desc.get_req_data());
         fastpair_req_desc.set_bus_rsp_id(dst_p_addr[`EVL_BUS_MESSAGE_CONF_MSB:`EVL_BUS_MESSAGE_CONF_LSB]);
         fastpair_req_desc.set_bus_rsp_size(req_desc.get_bus_req_size);
         print_req_desc("Core to Neigh (Enqueued REQ)", fastpair_req_desc);
         // Since the message is destined to the dst_core_id, push the request descriptor in the respective queue.
         // Once the response (ET_LINK_RSP_MsgRcvData) is received from a minion core we are going to match it with this request.
         m_core_pend_msg_q[dst_core_id].push(fastpair_req_desc);
         add_active_mem_desc(fastpair_req_desc);
         void'(req_desc.add_child_desc(fastpair_req_desc));
      end
      pred_msg = $sformatf("Core to Neigh (Prediction): %s (addr 0x%x) destination is", req_desc.get_bus_cmd_name(), req_desc.get_paddr());
      if (is_local_msg) begin
         pred_msg = $sformatf("%s a local %s Minion", pred_msg, (is_fast_pair == 1) ? "fast pair" : "slow pair");
      end
      else begin
         pred_msg = $sformatf("%s a remote Minion", pred_msg);
      end
      pred_msg = $sformatf("%s (Shire:%0d Neigh:%0d Minion:%0d Hart:%0d Port:%0d)", pred_msg, dst_shire_id, dst_neigh_id, dst_core_id, dst_hart_id, port_id);
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, pred_msg);

   endfunction : process_core_request_messages

   function void process_shire_request_messages(inout evl_req_desc rcv_req_desc, inout string obs_msg, inout string pred_msg);
      bit is_slowpair = 0;
      evl_req_desc slowpair_req_desc;
      evl_paddr_t dst_p_addr = rcv_req_desc.get_paddr();
      int dst_core_id  = dst_p_addr[`EVL_BUS_MSG_DST_MINION_MSB:`EVL_BUS_MSG_DST_MINION_LSB];
      int dst_neigh_id = dst_p_addr[`EVL_BUS_MSG_DST_NEIGH_MSB:`EVL_BUS_MSG_DST_NEIGH_LSB];
      //int dst_shire_id = dst_p_addr[`EVL_BUS_MSG_SHIRE_ID_MSB:`EVL_BUS_MSG_SHIRE_ID_LSB];
      int dst_shire_id = 0;
      dst_shire_id     = (dst_shire_id == 8'hff) ? get_shire_id() : dst_shire_id;
      is_slowpair = (dst_neigh_id == get_neighborhood_id()) && (dst_shire_id == get_shire_id());
      pred_msg = $sformatf("Neigh to Shire-Cache (Prediction): ET_LINK_REQ_MsgSendData (addr 0x%x)", dst_p_addr);
      if (is_slowpair == 1'b1) begin
         pred_msg = $sformatf("%s destination is local Minion %0d (slow-pair)", pred_msg, dst_core_id);
         $cast(slowpair_req_desc, rcv_req_desc.clone());
         slowpair_req_desc.set_rsp_data(rcv_req_desc.get_req_data());
         m_slowpair_pend_msg_q.push_back(slowpair_req_desc);
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), slowpair_req_desc, $sformatf("Pushing back request to m_slowpair_pend_msg_q: %s", slowpair_req_desc.sprint_obj()));
         print_req_desc("Neigh to Shire-Cache (Enqueued REQ)", slowpair_req_desc);
      end
      else begin
         pred_msg = $sformatf("%s destination is remote Minion", pred_msg);
      end
   endfunction : process_shire_request_messages

   function void process_shire_response_messages (inout evl_req_desc rcv_req_desc, inout string obs_msg, inout string pred_msg);

      bit is_slow_pair = 0;
      evl_paddr_t dst_p_addr = rcv_req_desc.get_paddr();
      int dst_core_id  = dst_p_addr[`EVL_BUS_MSG_DST_MINION_MSB:`EVL_BUS_MSG_DST_MINION_LSB];
      pred_msg = $sformatf("%s destination is Minion %0d", pred_msg, dst_core_id);
      foreach (m_slowpair_pend_msg_q[jj]) begin
         if ((m_slowpair_pend_msg_q[jj].get_paddr()                 == rcv_req_desc.get_paddr()                ) &&
             (m_slowpair_pend_msg_q[jj].get_rsp_data().get_xword(0) == rcv_req_desc.get_rsp_data().get_xword(0))
         ) begin
            m_slowpair_pend_msg_q[jj].set_done();
            m_slowpair_pend_msg_q.delete(jj);
            pred_msg = $sformatf("%s and matched with a previous observed slowpair request.", pred_msg);
            is_slow_pair = 1;
            break;
         end
      end
      if (!is_slow_pair) begin
         pred_msg = $sformatf("%s and comes from a remote Core.", pred_msg);
      end

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, obs_msg)
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, pred_msg)
      print_req_desc("Shire-Cache to Neigh (Enqueued REQ)", rcv_req_desc);
      m_core_pend_msg_q[dst_core_id].push(rcv_req_desc);
      add_active_mem_desc(rcv_req_desc);
   endfunction : process_shire_response_messages

   //*********************************************************************************************//
   //*********************************************************************************************//


   //*********************************************************************************************//
   // Cooperative Tensor Load (CTL)
   //*********************************************************************************************//

   function void add_ctl_request (inout evl_req_desc req_desc, inout string obs_msg, inout string pred_msg, int src_core_id);
      evl_bus_coop_id_t coop_id = req_desc.get_bus_coop_id();

      if ((m_read_coop_q[coop_id].ctl_q == null) || ((m_read_coop_q[coop_id].ctl_q.m_descs.size() == 0) && (m_read_coop_q[coop_id].minion_pend_mask == 0))) begin
         m_read_coop_q[coop_id].ctl_q = new;
         m_read_coop_q[coop_id].neigh_mask       = req_desc.get_bus_neigh_coop_mask();
         m_read_coop_q[coop_id].minion_mask      = req_desc.get_bus_minion_coop_mask();
         m_read_coop_q[coop_id].minion_obs_mask  = '0;
         m_read_coop_q[coop_id].minion_pend_mask = req_desc.get_bus_minion_coop_mask();
         m_read_coop_q[coop_id].ctl_paddr        = req_desc.get_paddr();
      end
      else begin
         // Sanity Checks
         if (m_read_coop_q[coop_id].neigh_mask != req_desc.get_bus_neigh_coop_mask()) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
            sb_error($sformatf("CTL: Neigh mask missmatch. First time seen was 0x%x, now 0x%x", m_read_coop_q[coop_id].neigh_mask, req_desc.get_bus_neigh_coop_mask()), req_desc);
            print_ctl_requests(m_read_coop_q[coop_id], "Previous CTL");
         end
         if (m_read_coop_q[coop_id].minion_mask != req_desc.get_bus_minion_coop_mask()) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
            sb_error($sformatf("CTL: Minion mask missmatch. First time seen was 0x%x, now 0x%x", m_read_coop_q[coop_id].minion_mask, req_desc.get_bus_minion_coop_mask()), req_desc);
            print_ctl_requests(m_read_coop_q[coop_id], "Previous CTL");
         end
         if (m_read_coop_q[coop_id].ctl_paddr != req_desc.get_paddr()) begin
            sb_error($sformatf("CTL: Address missmatch. For coop_id:0x%x, previous address was 0x%x, now is 0x%x", coop_id, m_read_coop_q[coop_id].ctl_paddr, req_desc.get_paddr()), req_desc);
            print_ctl_requests(m_read_coop_q[coop_id], "Previous CTL");
         end
      end
      obs_msg = $sformatf("%s (Neigh Mask 0x%x, Minion Mask 0x%x)", obs_msg, req_desc.get_bus_neigh_coop_mask(), req_desc.get_bus_minion_coop_mask());

      if (m_read_coop_q[coop_id].minion_obs_mask[src_core_id] == 1) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
         sb_error($sformatf("CTL: Unexpected request. Request from Minion %0d with coop_id %0d has already been observed in the past.", src_core_id, coop_id), req_desc);
      end
      else begin
         // Mark it as observed
         m_read_coop_q[coop_id].minion_obs_mask[src_core_id]  = 1'b1;
         // Delete it from the pending Minions
         m_read_coop_q[coop_id].minion_pend_mask[src_core_id] = 1'b0;
         // Add it to the queue
         m_read_coop_q[coop_id].ctl_q.push(req_desc);
         add_active_mem_desc(req_desc);
      end
      pred_msg = $sformatf("%s coop_id: %0d, %0d/%0d cooperating requests have been seen (%0d pending).", pred_msg, coop_id, $countones(m_read_coop_q[coop_id].minion_obs_mask), $countones(m_read_coop_q[coop_id].minion_mask), $countones(m_read_coop_q[coop_id].minion_pend_mask));
   endfunction : add_ctl_request

   function bit is_last_ctl_req (evl_bus_coop_id_t coop_id, inout string pred_msg);
      if (m_read_coop_q[coop_id].minion_pend_mask == '0) begin
         pred_msg = $sformatf("%s All cooperating request have been seen.", pred_msg);
         // As it is the last one, we reset all the values
         m_read_coop_q[coop_id].neigh_mask       = '0;
         m_read_coop_q[coop_id].minion_mask      = '0;
         m_read_coop_q[coop_id].minion_obs_mask  = '0;
         m_read_coop_q[coop_id].minion_pend_mask = '0;
         m_read_coop_q[coop_id].ctl_paddr        = '0;
         return 1;
      end
      return 0;
   endfunction : is_last_ctl_req

   function byte is_neigh_master(evl_neighborhood_id_t nid, evl_paddr_t p_addr, evl_bus_neigh_coop_mask_t neigh_mask);
      bit [1:0] pa_bits = p_addr[7:6];
      if ($countones(neigh_mask) == 1) begin
         // If it is not an Inter-Neigh CTL, the Neigh is always the master
         if (neigh_mask[nid] == 0) begin
            sb_error($sformatf("CTL: Request from Neigh-%0d but it is not set in the Neigh Coop Mask (0x%x)", nid, neigh_mask));
         end
         return 1;
      end

      if (pa_bits == 2'b00) begin
         if (neigh_mask[0]   == 1'b1   )      return (nid == 0);
         else if (neigh_mask[3:0] ==? 4'b0x10) return (nid == 1);
         else if (neigh_mask[3:0] ==  4'b0100) return (nid == 2);
         else if (neigh_mask[3:0] ==? 4'b1xx0) return (nid == 3);
      end
      else if (pa_bits == 2'b01) begin
         if (neigh_mask[1:0] == 2'b01  )      return (nid == 0);
         else if (neigh_mask[1]   == 1'b1   ) return (nid == 1);
         else if (neigh_mask[2:0] == 3'b100 ) return (nid == 2);
         else if (neigh_mask[3:0] == 4'b1000) return (nid == 3);
      end
      else if (pa_bits == 2'b10) begin
         if (neigh_mask[3:0] == 4'b0001)      return (nid == 0);
         else if (neigh_mask[2:1] == 2'b01  ) return (nid == 1);
         else if (neigh_mask[2]   == 1'b1   ) return (nid == 2);
         else if (neigh_mask[3:1] == 3'b100 ) return (nid == 3);
      end
      else if (pa_bits == 2'b11) begin
         if (neigh_mask[3:0] ==? 4'b00x1)      return (nid == 0);
         else if (neigh_mask[3:0] == 4'b0010) return (nid == 1);
         else if (neigh_mask[3:2] == 2'b01  ) return (nid == 2);
         else if (neigh_mask[3]   == 1'b1   ) return (nid == 3);
      end
      sb_error($sformatf("Unexpected encoding. CTL Neigh Master is Unknown (PA[7:6]=0x%x, CNM=0x%x)", pa_bits, neigh_mask));
      return 0;
   endfunction : is_neigh_master

   function void coop_link_reqs_to_parent(bit is_ctl, inout evl_req_desc merged_req_desc, input int num_req, evl_bus_coop_id_t coop_id, int base_req_src, inout string coop_pred_msg);
      evl_req_desc_queue coop_parents;
      string log_msg;
      // Pre sanity checks
      if (is_ctl == 1'b1) begin
         int parent_num_req = $countones(merged_req_desc.get_bus_minion_coop_mask());
         if (num_req != parent_num_req) begin
            sb_error("CTL: Request going from Neigh to SC (Parent Req) has the wrong Minion Coop Mask.", merged_req_desc);
         end
      end

      // coop_parents is used to link the original request descriptors from the Minions to the final request that goes to
      // the Shire-Cache. This can be seen in the process_shire_response_coop() function called from watch_for_shire_responses().
      coop_parents = new();
      for (int j = 0; j < num_req; j++) begin
         evl_req_desc minion_coop_req;
         if (is_ctl == 1'b1) begin
            minion_coop_req = m_read_coop_q[coop_id].ctl_q.pop();
         end
         else begin
            minion_coop_req = m_store_coop_q[j+base_req_src];
         end
         minion_coop_req.add_child_desc(merged_req_desc);
         coop_parents.push(minion_coop_req);
         add_active_mem_desc(minion_coop_req);
         log_msg = $sformatf("%s%s Minion-%0d(TrID:0x%x)", log_msg, (j>0)?",":"", minion_coop_req.get_bus_req_source(), minion_coop_req.get_trans_id());
      end
      coop_pred_msg = $sformatf("Parents are %s", log_msg);

      // Post Sanity check
      if (is_ctl == 1'b1) begin
         if (m_read_coop_q[coop_id].ctl_q.size() != 0) begin
            // At this point all the elements in the queue should be poped out and linked to the merged_req_desc
            sb_error("CTL: after linking Minion request to SC request, there still are elements pending in the queue");
            for (int i = 0; i < m_read_coop_q[coop_id].ctl_q.size(); i++) begin
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), merged_req_desc, $sformatf("m_read_coop_q[%0d].ctl_q[%0d] = %0s", coop_id, i, m_read_coop_q[coop_id].ctl_q.get(i).sprint_obj()))
            end
         end
         m_read_coop_q.delete(coop_id);
      end
      merged_req_desc.set_object_param(pk_coop_parents, coop_parents);
   endfunction : coop_link_reqs_to_parent

   function void process_core_request_ctl (inout evl_req_desc req_desc, inout evl_req_desc coop_ld_req_desc, inout bit is_l2_req, int src_core_id);
      bit is_last_req, is_master;
      string obs_msg, pred_msg, coop_pred_msg;
      longint unsigned cl_set = 0;
      int minion_mask_count = 0;
      evl_neighborhood_id_t nid = get_neighborhood_id();
      evl_bus_coop_id_t           coop_id     = req_desc.get_bus_coop_id();
      evl_bus_neigh_coop_mask_t   neigh_mask  = req_desc.get_bus_neigh_coop_mask();
      evl_bus_minion_coop_mask_t  minion_mask = req_desc.get_bus_minion_coop_mask();
      evl_paddr_t                 dst_p_addr  = req_desc.get_paddr();
      is_master = 0;
      is_l2_req = 0;

      obs_msg =  $sformatf("Core to Neigh ( Observed ): %s (addr 0x%x) request from Neigh:%0d Minion:%0d coop_id:%0d", req_desc.get_bus_cmd_name(), dst_p_addr, nid, src_core_id, coop_id);
      pred_msg = $sformatf("Core to Neigh (Prediction): %s (addr 0x%x)", req_desc.get_bus_cmd_name(), dst_p_addr);

      // Sanity Checks
      // The Minion Cooperative Mask must include the Minion who is doing the request
      if (neigh_mask[nid] == 1'b0) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
         sb_error($sformatf("CTL: Request from Neigh-%0d but it is not set in the Neigh Coop Mask (0x%x)", nid, neigh_mask), req_desc);
      end
      // The Neigh Cooperative Mask must include the Neigh who is doing the request
      if (minion_mask[src_core_id] == 1'b0) begin
         `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
         sb_error($sformatf("CTL: Request from Minion-%0d but it is not set in the Minion Coop Mask (0x%x)", src_core_id, minion_mask), req_desc);
      end

      // Add it to the queue
      add_ctl_request (req_desc, obs_msg, pred_msg, src_core_id);
      // Check if it is the last cooperating request
      is_last_req = is_last_ctl_req (coop_id, pred_msg);

      if (is_last_req == 1'b1) begin
         $cast(coop_ld_req_desc, req_desc.clone());
         // Request id is always the same as the coop id sent to the neigh
         coop_ld_req_desc.set_bus_req_id(coop_ld_req_desc.get_bus_coop_id());
         // If the cooperative read is inter neigh then the L2 request will be just a Read
         // otherwise it will be a ReadCoop. It is safe to just count 1's as we already
         // check at the begining that our Neigh ID is set on the mask
         if ($countones(neigh_mask) == 1) begin
            coop_ld_req_desc.set_bus_cmd(ET_LINK_REQ_Read);
            coop_ld_req_desc.set_bus_coop_id(0); // Coop id for Read operations should be 0
         end
         else begin
            coop_ld_req_desc.set_bus_cmd(ET_LINK_REQ_ReadCoop);
         end
         // According to ET-Link spec the Reads that are generated due to a
         // coooperative load should have the source field set to ET_LINK_NEIGH_SRC_Coop
         coop_ld_req_desc.set_bus_req_source(ET_LINK_NEIGH_SRC_Coop);
         // Once the ReadCoop is sent link it to all the upstream descriptors to be
         // able to cascade the response later.
         // for now just replicate the queue in an object param. We may want to modify
         // our base library classes to support multiple parents.
         coop_link_reqs_to_parent(1, coop_ld_req_desc, $countones(minion_mask), coop_id, 0, coop_pred_msg);

         is_master      = is_neigh_master(nid, dst_p_addr, neigh_mask);
         is_l2_req      = is_master;
         if (is_master == 1'b1) begin
            pred_msg = $sformatf("%s Neigh-%0d is the Coop Master (PA[7:6]=0x%x, CNM=0x%x)", pred_msg, nid, dst_p_addr[7:6], neigh_mask);
            pred_msg = $sformatf("%s request goes to the Shire-Cache. %s.", pred_msg, coop_pred_msg);
         end
         else begin
            // Coop Slave Neighs will receive a Response with Data without having seen a request, therefore
            // We need to add it to the m_active_shire_reqs queue in order to match it once the request comes back
            pred_msg = $sformatf("%s Neigh-%0d is a Coop Slave (PA[7:6]=0x%x, CNM=0x%x)", pred_msg, nid, dst_p_addr[7:6], neigh_mask);
            ncb_set_bus_bank(coop_ld_req_desc, evl_sc_bank_id_t'(get_bank_from_address(dst_p_addr)), NCB_SOURCE_DCACHE);
            void'(req_desc.add_child_desc(coop_ld_req_desc));
            coop_ld_req_desc.set_req_data(req_desc.get_req_data());
            m_active_shire_reqs.push_back(coop_ld_req_desc);
         end
      end
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, pred_msg)
   endfunction : process_core_request_ctl;

   function void process_shire_response_coop(inout evl_req_desc rcv_req_desc, inout string obs_msg, inout string pred_msg);
      evl_req_desc req_desc;
      bit found = 1'b0;
      bit is_ctl = 0;
      foreach (m_active_shire_reqs[ii]) begin
         req_desc = m_active_shire_reqs[ii];
         print_req_desc($sformatf("Shire-Cache to Neigh (m_active_shire_reqs[%0d])", ii), m_active_shire_reqs[ii]);
         // Matches with a CTL?
         if ((rcv_req_desc.get_bus_req_source() == ET_LINK_NEIGH_SRC_Coop       ) &&
             (rcv_req_desc.get_bus_req_source() == req_desc.get_bus_req_source()) &&
             (rcv_req_desc.get_bus_req_id()     == req_desc.get_bus_req_id()    )
         ) begin
            m_active_shire_reqs.delete(ii);
            found  = 1'b1;
            is_ctl = 1'b1;
            break;
         end
         // Matches with a CTS?
         else if (rcv_req_desc == req_desc) begin
            m_active_shire_reqs.delete(ii);
            found  = 1'b1;
            is_ctl = 1'b0;
            break;
         end
      end

      if (found == 1'b1) begin
         evl_req_desc_queue coop_parents;
         pred_msg = $sformatf("%s matched with a 'Core to Neigh' request. Forwarding request to ", pred_msg);
         $cast(coop_parents, req_desc.get_object_param(pk_coop_parents));
         if (coop_parents != null) begin
            while (coop_parents.size() > 0) begin
               int req_source;
               evl_req_desc coop_src_desc = coop_parents.pop();
               coop_src_desc.replicate_rsp_info(rcv_req_desc);
               coop_src_desc.set_bus_rsp_valid(0, get_cycle_count());
               req_source = coop_src_desc.get_bus_req_source();
               pred_msg = $sformatf("%s Minion-%0d(TrID:0x%x)%s", pred_msg, req_source, coop_src_desc.get_trans_id(), (coop_parents.size() != 0) ? "," : "");
               `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("forwarding shire response to coop parent (Minion %0d): %0s", req_source, coop_src_desc.sprint_obj()))
            end
            pred_msg = $sformatf("%s.", pred_msg);
         end
         else begin
            // coop_parents contains the original request descriptor of each cooperating Minion
            sb_error($sformatf("%s Response coop_parents is null", (is_ctl == 1'b1) ? "CTL ReadCoop" : "CTS WriteAround"), rcv_req_desc);
         end
      end
      else begin
         sb_error($sformatf("%s has no response with ID 0x%0x expected (%0s)", (is_ctl == 1'b1) ? "CTL" : "CTS", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_rsp()), rcv_req_desc);
      end
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, obs_msg)
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, pred_msg)
   endfunction : process_shire_response_coop

   //*********************************************************************************************//
   //*********************************************************************************************//


   //*********************************************************************************************//
   // Cooperative Tensor Store (CTS)
   //*********************************************************************************************//

   function evl_bus_req_source_t get_base_req_src_cts(input evl_req_desc req_desc, input evl_bus_coop_way_t bus_coop_way);
      evl_bus_req_source_t base_req_src;
      // Get the base source of the transaction (i.e. the closest even Minion)
      if (bus_coop_way == 0) return req_desc.get_bus_req_source();
      base_req_src = (req_desc.get_bus_req_source() % 8);
      if (bus_coop_way == 2) begin
         base_req_src[1:0] = '0;
      end
      else begin
         base_req_src[0]   = '0;
      end
      return base_req_src;
   endfunction : get_base_req_src_cts

   function void cts_check_paddr(input evl_bus_coop_way_t n_way, input evl_bus_req_source_t base_req_src);
      int i;
      evl_paddr_t paddr_cl, paddr_cl_tmp;

      paddr_cl = m_store_coop_q[base_req_src].get_paddr();
      paddr_cl[5:0] = '0;

      for (i = 1; i < n_way; i++) begin
         paddr_cl_tmp = m_store_coop_q[i+base_req_src].get_paddr();
         paddr_cl_tmp[5:0] = '0;
         if (paddr_cl != paddr_cl_tmp) begin
            sb_error($sformatf("Cooperative Tensor Store requests write to different memory lines. Expected Cache Line aligned addr: 0x%x, observed Cache Line aligned addr: 0x%x", paddr_cl, paddr_cl_tmp));
         end
      end
    endfunction : cts_check_paddr

   //-----------------------------------------------------------------------------------------------
   //
   // Function: merge_coop_store_data
   // req_desc     : Request descriptor that will be used as the expected L2 request
   // bus_coop_way : Cooperative way (0=No coop, 1=2-way, 2=4-way )
   // paddr        : Cooperative store address (discarding always bits [5:0])
   function void merge_coop_store_data(inout evl_req_desc req_desc, input evl_bus_coop_way_t bus_coop_way, input evl_paddr_t paddr);
      evl_req_desc core_req[4];
      evl_oword_t oword[2]; // oword = 32 bytes
      evl_qword_t qword[4]; // qword = 16 bytes
      evl_paddr_t paddr_masked;

      int merge_word_offset;
      evl_req_desc       even_minion_req[2];
      evl_req_desc       odd_minion_req[2];
      evl_paddr_t        even_minion_addr;
      evl_paddr_t        odd_minion_addr;

      int i;
      evl_bus_req_source_t base_req_src;
      evl_bus_req_source_t tmp_req_source;
      evl_bus_req_size_t coop_store_size;
      evl_bus_coop_way_t n_way;

      if (bus_coop_way == 0) begin
         sb_error($sformatf("neigh_scoreboard: Should not be here. Non-cooperative directly goes to L2"));
      end
      else if (bus_coop_way > 2) begin
         sb_error($sformatf("neigh_scoreboard: Un-expected subopcode %0d for WriteAround", bus_coop_way));
      end
      n_way = 2**(bus_coop_way);
      base_req_src = get_base_req_src_cts(req_desc, bus_coop_way);

      cts_check_paddr(n_way, base_req_src);

      // With the base source of the transaction (e.g. Minion0) we can
      // compute the range of Minion ids that are part of the transaction
      // as a cooperative transaction is done by consecutive Minions
      for (i = 0; i < n_way; i++) begin
         core_req[i] = m_store_coop_q[i+base_req_src];
         m_store_coop_q.delete(i+base_req_src);
      end

      // Classify the transactions (Cache Line)
      //
      // * If it is pair-128 (index [1] is ignored)
      // --------------------------------------------------------------------
      // | even_minion[0] | odd_minion[0] | /////////////////////////////// |
      // --------------------------------------------------------------------
      //    OR
      // --------------------------------------------------------------------
      // | ////////////////////////////// | even_minion[0] | odd_minion[0]  |
      // --------------------------------------------------------------------
      //
      // * If it is pair-128 (index [1] is ignored)
      // --------------------------------------------------------------------
      // |          even_minion[0]        |          odd_minion[0]          |
      // --------------------------------------------------------------------
      //
      // * If it is quad-128
      // --------------------------------------------------------------------
      // | even_minion[0] | odd_minion[0] | even_minion[1] |  odd_minion[1] |
      // --------------------------------------------------------------------

      for (int i = 0; i < n_way; i++) begin
         tmp_req_source = core_req[i].get_bus_req_source() % 8;
         if ((tmp_req_source % 2) == 0) begin
            if (tmp_req_source == base_req_src) begin
               even_minion_req[0] = core_req[i];
            end
            else begin
               even_minion_req[1] = core_req[i];
            end
         end
         else begin
            if ((tmp_req_source-1) == base_req_src) begin
               odd_minion_req[0] = core_req[i];
            end
            else begin
               odd_minion_req[1] = core_req[i];
            end
         end
      end

      even_minion_addr = even_minion_req[0].get_paddr();
      odd_minion_addr  =  odd_minion_req[0].get_paddr();
      if (odd_minion_req[0].get_bus_req_size() == 5) begin
         // If it is pair-256, req_size() is in bytes, ((2^5)*8) == 256 bits
         merge_word_offset = even_minion_addr[5];
         // Get even/odd minion request owords
         oword[0+merge_word_offset] = even_minion_req[0].get_req_oword(0 + merge_word_offset);
         oword[1+merge_word_offset] =  odd_minion_req[0].get_req_oword(1 + merge_word_offset);

         // Merge data from odd/even minions, and set req_desc
         req_desc.invalidate_req_data();
         req_desc.set_req_oword(0+merge_word_offset, oword[0+merge_word_offset]);
         req_desc.set_req_oword(1+merge_word_offset, oword[1+merge_word_offset]);

         // Set quad word enables depending on the even minion address
         req_desc.set_bus_req_qword_en(4'b1111);

         // Merged request contains a full cache line
         coop_store_size = ET_LINK_Line;
      end
      else begin
         // If it is pair-128 or quad-128
         merge_word_offset = even_minion_addr[5:4];
         req_desc.invalidate_req_data();
         for (i = 0; i < n_way; i += 2) begin
            // Get even/odd minion request qwords
            qword[i+merge_word_offset  ] = even_minion_req[i/2].get_req_qword(i  +merge_word_offset);
            qword[i+1+merge_word_offset] =  odd_minion_req[i/2].get_req_qword(i+1+merge_word_offset);
            // Merge data from odd/even minions, and set req_desc
            req_desc.set_req_qword(i  +merge_word_offset, qword[i  +merge_word_offset]);
            req_desc.set_req_qword(i+1+merge_word_offset, qword[i+1+merge_word_offset]);
         end

         // Set quad word enables depending on the even minion address
         if (n_way == 4) begin
            // The merge writes the full cache line: [*|*|*|*]
            req_desc.set_bus_req_qword_en(4'b1111);
            coop_store_size = ET_LINK_Line;
         end
         else begin
            // addr[5:4] == 00, the merge writes to these positions of the cache line: [ | |*|*]
            // addr[5:4] == 10, the merge writes to these positions of the cache line: [*|*| | ]
            req_desc.set_bus_req_qword_en(4'b0011 << even_minion_addr[5:4]);
            coop_store_size = ET_LINK_HLine;
         end
      end

      // Change the source to the Minion ID with lowest id (even)
      req_desc.set_bus_req_source(even_minion_req[0].get_bus_req_source());

      // Change the address to the base addr (i.e. ignoring lower bits)
      paddr_masked      = req_desc.get_paddr();
      odd_minion_addr   = odd_minion_req[0].get_paddr();

      // Set the Physical Address to be the base one
      req_desc.set_paddr(even_minion_addr);

      // Assign the correct size to the WriteAround going to the L2
      req_desc.set_bus_req_size(coop_store_size);

   endfunction : merge_coop_store_data

   //-----------------------------------------------------------------------------------------------
   //
   // Function : store_coop_size
   // req_desc : Request descriptor from the Minion
   // Return   : Number of requests that have been observed from the same cooperation
   function byte store_coop_size(evl_req_desc req_desc);
      evl_bus_req_source_t base_req_src;
      evl_bus_coop_way_t   bus_coop_way;
      byte ret;

      bus_coop_way = req_desc.get_bus_coop_way();

      // Get the base source of the transaction (i.e. the closest even Minion)
      base_req_src = (req_desc.get_bus_req_source() % 8);
      if (bus_coop_way == 2) begin
         base_req_src[1:0] = '0;
      end
      else begin
         base_req_src[0]   = '0;
      end
      ret = 0;
      for (int j = 0; j < (2**bus_coop_way); j++) begin
         if (m_store_coop_q[j+base_req_src] != null) begin
            ret++;
         end
      end
      return ret;
   endfunction : store_coop_size

   function void cts_offset_check(inout evl_req_desc req_desc);
      evl_bus_coop_way_t   coop_way     = req_desc.get_bus_coop_way();
      int                  src_core_id  = req_desc.get_bus_req_source()%8;
      evl_paddr_t          dst_p_addr   = req_desc.get_paddr();
      evl_bus_req_size_t   req_size     = req_desc.get_bus_req_size();
      evl_bus_req_source_t base_req_src = get_base_req_src_cts(req_desc, coop_way);

      int coop_grp_id = src_core_id - base_req_src;
      int expected_offset = (coop_grp_id << req_size);
      int observed_offset = (req_size == 4) ? dst_p_addr[4:0] : dst_p_addr[5:0];

      if ((coop_way == 2) && (req_size == 4)) begin
         observed_offset = dst_p_addr[5:0];
      end

      if (expected_offset != observed_offset) begin
          sb_error($sformatf("Core %0d sent a TensorStore Coop with an invalid offset. Expected offset: 0x%x, observed offset: 0x%x (most probably a SW issue)", src_core_id, expected_offset, observed_offset));
      end

   endfunction : cts_offset_check

   function void process_core_request_cts(inout evl_req_desc req_desc, inout evl_req_desc coop_st_req_desc, inout bit is_l2_req, int src_core_id);
      string obs_msg, pred_msg, coop_pred_msg;
      byte n_store_coop_seen = 0;
      evl_bus_coop_way_t bus_coop_way = req_desc.get_bus_coop_way();
      byte n_store_coop = 2**(bus_coop_way);
      evl_neighborhood_id_t       nid         = get_neighborhood_id();
      evl_bus_coop_id_t           coop_id     = req_desc.get_bus_coop_id();
      evl_bus_minion_coop_mask_t  minion_mask = req_desc.get_bus_minion_coop_mask();
      evl_paddr_t                 paddr       = req_desc.get_paddr();
      // Strip dword offset
      longint unsigned            cl_set      = paddr >> 6;
      evl_paddr_t                 dst_p_addr  = req_desc.get_paddr();

      int base_req_src = get_base_req_src_cts(req_desc, bus_coop_way);

      obs_msg  = $sformatf("Core to Neigh ( Observed ): %s (addr 0x%x) request from Neigh:%0d Minion:%0d (Minion Mask 0x%x)", req_desc.get_bus_cmd_name(), paddr, nid, src_core_id, minion_mask);
      pred_msg = $sformatf("Core to Neigh (Prediction): %s (addr 0x%x)", req_desc.get_bus_cmd_name(), paddr);

      // If it does not exist on the queue, add it
      if (m_store_coop_q[src_core_id % 8] == null) begin
         m_store_coop_q[src_core_id % 8] = req_desc;
      end
      else begin
         sb_error($sformatf("Core %0d sent a TensorStore Coop before its previous TensorStore Coop finished. Previous paddr: 0x%x and trId: 0x%x", src_core_id, m_store_coop_q[src_core_id % 8].get_paddr(), m_store_coop_q[src_core_id % 8].get_trans_id()), req_desc);
      end

      // Tensor Store without cooperation
      // There is no need to add it into the queue
      if (bus_coop_way == 0) begin
         // For non-cooperative requests we only see one
         n_store_coop_seen = 1;
         is_l2_req = 1;
      end
      else begin
         // Check if the CTS request contains the correct offset
         cts_offset_check(req_desc);

         n_store_coop_seen = store_coop_size(req_desc);
         // Enqueue the request to the L2 pending queue only if all the cooperative stores (according to the coop-way bits) have been seen)
         if (n_store_coop_seen == n_store_coop) begin
            is_l2_req = 1;
         end
         else begin
            is_l2_req = 0;
         end
      end

      if (is_l2_req == 1'b1) begin
         $cast(coop_st_req_desc, req_desc.clone());
         coop_st_req_desc.replicate_req_info(req_desc);
         coop_link_reqs_to_parent(0, coop_st_req_desc, n_store_coop, 0, base_req_src, coop_pred_msg);
         // Once the 2/4 WriteArounds are merged together, change the size and enqueue only one
         // This applies only for 2/4 coop request. CTS with coop_way == 0 does not need to merge data
         if (bus_coop_way != 0) merge_coop_store_data(coop_st_req_desc, bus_coop_way, dst_p_addr);
         else m_store_coop_q.delete(src_core_id);
      end

      pred_msg = $sformatf("%s coop_id: %0d, %0d/%0d cooperating requests have been seen", pred_msg, coop_id, n_store_coop_seen, n_store_coop);

      if (is_l2_req == 1'b1) begin
         pred_msg = $sformatf("%s, request goes to the Shire-Cache. %s.", pred_msg, coop_pred_msg);
      end
      else begin
         pred_msg = $sformatf("%s, coop store still pending", pred_msg);
      end

      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, obs_msg)
      `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), req_desc, pred_msg)
   endfunction : process_core_request_cts

   function void process_shire_request_cts (inout evl_req_desc req_desc, inout string obs_msg);
      obs_msg = $sformatf("%s. To L2: WriteAround request.", obs_msg);
   endfunction : process_shire_request_cts

   //*********************************************************************************************//
   //*********************************************************************************************//


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_bus_requests
   //
   // This task starts sub tasks that monitor all the busses attached to this scoreboard
   //
   task watch_for_bus_requests();
      watch_for_core_requests();
      watch_for_shire_requests();
      watch_for_shire_responses();
      watch_for_core_resp();
   endtask: watch_for_bus_requests


   function void print_req_desc(string origin, evl_req_desc req_desc);
      if (req_desc != null) begin
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_cmd           = 0x%x", origin, req_desc.get_bus_cmd()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: paddr             = 0x%x", origin, req_desc.get_paddr()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_bank          = 0x%x", origin, req_desc.get_bus_bank()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_req_id        = 0x%x", origin, req_desc.get_bus_req_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_req_size      = 0x%x", origin, req_desc.get_bus_req_size()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_req_len       = 0x%x", origin, req_desc.get_bus_req_len()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_req_source    = 0x%x", origin, req_desc.get_bus_req_source()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_rsp_type      = 0x%x", origin, req_desc.get_bus_rsp_type()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_rsp_id        = 0x%x", origin, req_desc.get_bus_rsp_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_rsp_size      = 0x%x", origin, req_desc.get_bus_rsp_size()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_rsp_dest      = 0x%x", origin, req_desc.get_bus_rsp_dest()))

         if (req_desc.get_req_data() != null) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: req_data = 0x%x", origin, req_desc.get_req_data().get_xword(0)))
         end
         else begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: req_data = null", origin))
         end
         if (req_desc.get_rsp_data() != null) begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: rsp_data = 0x%x", origin, req_desc.get_rsp_data().get_xword(0)))
         end
         else begin
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: rsp_data = null", origin))
         end

         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_type          = %p",   origin, req_desc.get_bus_type()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_cmd_name      = %s",   origin, req_desc.get_bus_cmd_name()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_rsp_name      = %s",   origin, req_desc.get_bus_rsp_name()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: tag_id            = 0x%x", origin, req_desc.get_tag_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: req_tag_id        = 0x%x", origin, req_desc.get_req_tag_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: rsp_tag_id        = 0x%x", origin, req_desc.get_rsp_tag_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_coop_id       = 0x%x", origin, req_desc.get_bus_coop_id()))
         `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("%s: bus_scp_fill_addr = 0x%x", origin, req_desc.get_bus_scp_fill_addr()))
      end
   endfunction : print_req_desc

   function void print_ctl_requests(ctl_operation_t ctl_struct, string orig);
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("%s: ctl_paddr        = 0x%x", orig, ctl_struct.ctl_paddr))
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("%s: neigh_mask       = 0x%x", orig, ctl_struct.neigh_mask))
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("%s: minion_mask      = 0x%x", orig, ctl_struct.minion_mask))
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("%s: minion_obs_mask  = 0x%x", orig, ctl_struct.minion_obs_mask))
         `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("%s: minion_pend_mask = 0x%x", orig, ctl_struct.minion_pend_mask))
         for (int ii = 0; ii < ctl_struct.ctl_q.size(); ii++) begin
            evl_req_desc ctl_req_desc = ctl_struct.ctl_q.get(ii);
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), ctl_req_desc, $sformatf("%s: ctl_q[%0d] = Minion-%0d", orig, ii, ctl_req_desc.get_bus_req_source()));
         end
   endfunction : print_ctl_requests



   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_shire_requests
   //
   // This task monitors the requests observed by the ETLink interface between neigh and shire-cache
   // and checks that they were expected and correctly formed
   //
   task watch_for_shire_requests();
      fork
         forever begin
            evl_req_desc rcv_req_desc;
            string obs_msg, pred_msg;

            m_shire_req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               obs_msg = $sformatf("Neigh to Shire-Cache ( Observed ): %s (addr 0x%x)", rcv_req_desc.sprint_obj(), rcv_req_desc.get_paddr());
               print_req_desc("Neigh to Shire-Cache (Observed REQ)", rcv_req_desc);

               // LATER: ignore stuff coming from Icache / PTW, maybe we'll model these later
               if (rcv_req_desc.get_bus_req_source() inside {ET_LINK_NEIGH_SRC_Icache, ET_LINK_NEIGH_SRC_Ptw0, ET_LINK_NEIGH_SRC_Ptw1}) begin
                  continue;
               end

               if (rcv_req_desc.get_bus_cmd() inside {ET_LINK_REQ_WriteAround}) begin
                  process_shire_request_cts(rcv_req_desc, obs_msg);
               end
               else if (rcv_req_desc.get_bus_cmd() == ET_LINK_REQ_MsgSendData) begin
                  process_shire_request_messages(rcv_req_desc, obs_msg, pred_msg);
               end

               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, obs_msg)
               if (pred_msg != "") begin
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, pred_msg)
               end

               //----------------------- obs_req_desc,      pending_reqs[$],      active_reqs[$], ignore_id, relaxed_order
               process_predicted_request(rcv_req_desc, m_pending_shire_reqs, m_active_shire_reqs,      1'b0,          1'b1);
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("Neigh to Shire-Cache (Predicted ): Found a matching request for %s", rcv_req_desc.sprint_obj()))
            end
         end
      join_none
   endtask: watch_for_shire_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_core_resp
   //
   // This task monitors the fills observed by the ETLink interface between neigh and the core
   //
   task watch_for_core_resp();

         foreach (m_core_evict_rsp_fifo[ii]) begin
         fork
            automatic int jj = ii;
            automatic evl_req_desc rcv_req_desc = null;
            automatic evl_req_desc snd_req_desc = null;

            forever begin
               string obs_msg, pred_msg;

               m_core_evict_rsp_fifo[jj].get(rcv_req_desc);
               if (rcv_req_desc != null) begin
                  print_req_desc("Neigh to Core (Observed REQ)", rcv_req_desc);

                  // Although it seems awkward here to receive an ET_LINK_REQ_MsgSendData
                  // is because the et_link_mon changes the opcode to send when a recv
                  // appears on the respone channel
                  obs_msg  = $sformatf("Neigh to Core ( Observed ): %s (%s) destination is Minion %0d", rcv_req_desc.get_bus_cmd_name(), rcv_req_desc.sprint_obj(), jj);
                  pred_msg = "Neigh to Core (Predicted ):";

                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("Found miss response %s", rcv_req_desc.sprint_obj() ))
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc,  obs_msg)
               end
            end

         join_none
       end
   endtask: watch_for_core_resp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_shire_responses
   //
   // This task monitors the responses observed by the ETLink interface between neigh and shire-cache
   // and predicts where they should be forwarded.
   //
   task watch_for_shire_responses();

      fork
         forever begin
            evl_req_desc rcv_req_desc;
            evl_req_desc src_req_desc;
            evl_req_desc snd_req_desc = null;
            string obs_msg, pred_msg;
            bit found;

            m_shire_rsp_fifo.get(rcv_req_desc);

            if (rcv_req_desc != null) begin
               evl_req_desc_queue coop_parents;
               print_req_desc("Shire-Cache to Neigh (Observed REQ)", rcv_req_desc);
               obs_msg  = $sformatf("Shire-Cache to Neigh ( Observed ): %s (addr 0x%x)", rcv_req_desc.sprint_obj(), rcv_req_desc.get_paddr());
               pred_msg = $sformatf("Shire-Cache to Neigh (Predicted ): %s (addr 0x%x)", rcv_req_desc.get_bus_cmd_name(), rcv_req_desc.get_paddr());

               // LATER: ignore stuff coming from Icache / PTW, maybe we'll model these later
               rcv_req_desc.set_done();
               if (rcv_req_desc.get_bus_req_source() inside {ET_LINK_NEIGH_SRC_Icache, ET_LINK_NEIGH_SRC_Ptw0, ET_LINK_NEIGH_SRC_Ptw1}) begin
                  rcv_req_desc.set_trans_done();
                  continue;
               end

               $cast(coop_parents, rcv_req_desc.get_object_param(pk_coop_parents));

               src_req_desc = rcv_req_desc.get_parent_req_desc();
               if (src_req_desc == null) begin
                  src_req_desc = rcv_req_desc;
               end

               if (rcv_req_desc.get_bus_cmd() inside {ET_LINK_REQ_MsgSendData}) begin
                  // You can receive a message without ever seeing a request before
                  // therefore we need to treat them in a special way
                  process_shire_response_messages (rcv_req_desc, obs_msg, pred_msg);
               end
               else if (rcv_req_desc.get_bus_req_source() == ET_LINK_NEIGH_SRC_Coop) begin
                  // You can receive a CTL response without ever seeing a request before (e.g. Slave Neigh
                  //  in a Inter-Neigh CTL) therefore we need to treat them in a special way
                  process_shire_response_coop(rcv_req_desc, obs_msg, pred_msg);
               end
               else if ((coop_parents != null) &&
                        (rcv_req_desc.get_bus_cmd() inside {ET_LINK_REQ_WriteAround, ET_LINK_REQ_Write})
               ) begin
                  // In a cooperative operation, you see X request coming from the minions but only 1 going to the L2
                  // For this reason, we have to treat the response in a special way. An operation that goes to the L2 SCP
                  // is transformed into a normal WRITE, therefore we need to consider that case too
                  process_shire_response_coop(rcv_req_desc, obs_msg, pred_msg);
               end
               else begin
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("Shire-Cache to Neigh ( Observed ): %s", rcv_req_desc.sprint_obj()))
                  found = 1'b0;
                  //
                  // m_active_shire_reqs[] contains handles to the request descriptors that we "sent"
                  // to the shire cache (the "sent" request descriptor is obtained when the ET link
                  // VC performs a find_observed_request() call to this scoreboard ... it then marks
                  // the request descriptor that it created as a child of the "sent" request
                  // descriptor).  When we receive a response, we need to find a matching request ...
                  // the response should be *the same request descriptor that we sent*.  If no match
                  // is found, we have received a bonus response from the ether.
                  //
                  // Note that the flow for sending requests to the shire cache is as follows:
                  // - process_core_request() is called when a new core request is received ... if
                  //   a shire cache request will be spawned, a new request descriptor (l2_req) is
                  //   created as a child of the original request and it is added to the
                  //   m_pending_shire_reqs[] queue.
                  // - When a shire cache request is observed, find_observed_request() will be called
                  //   by the ET link VC ... this will cause the spawned descriptor to be moved from
                  //   the m_pending_shire_reqs[] queue to the m_active_shire_reqs[] queue.  The ET
                  //   link VC will also now have a handle to this spawned descriptor.
                  // - When a shire cache response is observed, we end up here.  This incoming
                  //   ought to be the *same* spawned descriptor.
                  //
                  foreach (m_active_shire_reqs[ii]) begin
                     if (m_active_shire_reqs[ii] == rcv_req_desc) begin
                        m_active_shire_reqs.delete(ii);
                        found = 1'b1;
                        src_req_desc.replicate_rsp_info(rcv_req_desc);
                        src_req_desc.set_bus_rsp_valid(0, get_cycle_count());
                        `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("forwarding shire response to parent: %0s", src_req_desc.sprint_obj()))
                        break;
                     end
                  end
                  if (!found) begin
                     sb_error($sformatf("no response with ID 0x%0x expected (%0s)", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_rsp()), rcv_req_desc);
                     foreach (m_active_shire_reqs[ii]) begin
                        `evl_log_mdesc(UVM_NONE, m_vmod, get_abstract_name(), m_active_shire_reqs[ii], $sformatf("This was pending: %0s", m_active_shire_reqs[ii].sprint_obj()))
                     end
                  end
               end
            end
            else begin
               sb_error("Shire-Cache to Neigh ( Observed ): received a null request desc from Shire-Cache :(");
            end
         end
      join_none
   endtask: watch_for_shire_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_core_requests
   //
   // This task monitors the requests observed by the ETLink interface between the cores and neigh
   // and predicts where they should be forwarded. Normal cache requests will go to the shire cache
   // while messages could be routed to another core's response channel
   // FUTURE: Dimitris Tsaliagos Fix race condition here in case a miss request and a evict happen on the same time (probably its ok, but looks dodgy anyway)
   task watch_for_core_requests();
      foreach (m_core_miss_req_fifo[ii]) begin
         fork
            automatic int jj = ii;
            forever begin
               evl_req_desc rcv_req_desc;
               m_core_miss_req_fifo[jj].get(rcv_req_desc);
               if (rcv_req_desc != null) begin
                  rcv_req_desc.set_int_param(pk_core_req_port, jj);
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("Core to Neigh ( Observed ): Core %0d (MISS) found %0s", jj, rcv_req_desc.sprint_obj()))
                  process_core_request(rcv_req_desc, jj);
               end
            end
         join_none
      end

      foreach (m_core_evict_req_fifo[ii]) begin
         fork
            automatic int jj = ii;
            forever begin
               evl_req_desc rcv_req_desc;
               m_core_evict_req_fifo[jj].get(rcv_req_desc);
               if (rcv_req_desc != null) begin
                  rcv_req_desc.set_int_param(pk_core_req_port, jj);
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_req_desc, $sformatf("Core to Neigh ( Observed ): Core %0d (EVICT) found %0s", jj, rcv_req_desc.sprint_obj()))
                  process_core_request(rcv_req_desc, jj);
               end
            end
         join_none
      end

      foreach (m_core_pmu_req_fifo[ii]) begin
         fork
            automatic int jj = ii;
            forever begin
               evl_pmu_desc rcv_pmu_desc;
               m_core_pmu_req_fifo[jj].get(rcv_pmu_desc);
               if (rcv_pmu_desc != null) begin
                  rcv_pmu_desc.set_int_param(pk_core_req_port, jj);
                  `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), rcv_pmu_desc, $sformatf("Core to Neigh ( Observed ): Core %0d (PMU) found %0s", jj, rcv_pmu_desc.sprint_obj()))
                  process_pmu_request(rcv_pmu_desc, jj);
               end
            end
         join_none
      end

   endtask : watch_for_core_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_rtl_events
   //
   // This task monitors meaningful RTL events picked up by the neighbourhood interface that might
   // cause internal state change or bus traffic. Currently this isn't used
   //
   task watch_for_rtl_events();
      evl_neigh_packet rtl_event;

      forever begin
         evl_neigh_packet_u packet_info;

         m_neigh_rtl_port.get_event_neigh(rtl_event);
         packet_info = rtl_event.get_info();
         if (rtl_event.get_packet_type() == NEIGH_ICACHE_REQ) begin
            `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Neighbourhood SB received an ICache request event"))
         end
         else if (rtl_event.get_packet_type() == NEIGH_PTW_REQ) begin
            `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Neighbourhood SB received a PTW request event"))
         end
         else if (rtl_event.get_packet_type() inside {NEIGH_FLB_RSP, NEIGH_FLB_REQ})
            `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Neighbourhood SB received an FLB transaction"))
         else begin
            `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("Neighbourhood SB received an unknown RTL event!"))
         end

      end
   endtask : watch_for_rtl_events


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_pmu_events
   //
   // This task monitors PMU events picked up by the neighbourhood interface
   //
   task watch_for_pmu_events();
      evl_pmu_desc pmu_desc;

      forever begin
         evl_neigh_packet_u packet_info;
         m_neigh_rtl_port.get_pmu_event(pmu_desc);
         if (pmu_desc != null) begin
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), pmu_desc, $sformatf("Neigh (PMU): saw %0s", pmu_desc.sprint_obj()))
            process_pmu_request(pmu_desc);
         end
      end
   endtask : watch_for_pmu_events


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bank_from_address
   //
   function int get_bank_from_address(input evl_paddr_t addr);
      // Get region corresponding to our addr
      // (regions are defined in evl_top_env class)

      // Check if the address is on a cacheable region
      if (addr_in_regions({pk_scp_region, pk_mram_region, pk_sram_region, pk_bootrom_region}, addr)) begin
          return m_global_attrs.get_bank_from_address(addr);
      end

      // Non-cacheable bank
      return `SC_BANKS;

   endfunction : get_bank_from_address

   function bit is_l2scp_fill(input evl_req_desc req_desc);
      evl_paddr_t paddr = req_desc.get_paddr();
      bit is_scp_region = (addr_in_region(pk_scp_region, paddr));
      bit is_scp_cmd    = (req_desc.get_bus_cmd() == ET_LINK_REQ_ScpFill);

      // Sanity check
      if ((is_scp_cmd == 1'b1) && (is_scp_region == 1'b0)) begin
         sb_error("Request not accessing SCP Region but bus_cmd is set to ET_LINK_REQ_ScpFill", req_desc);
      end
      return is_scp_cmd;
   endfunction : is_l2scp_fill

   function evl_paddr_t get_l2scp_addr(evl_paddr_t paddr);
      bit is_scp_region = (addr_in_region(pk_scp_region, paddr));
      // PRM-19: 2.3 The scratchpad region
      // SCP has two possible modes depending on the bit 30. If this bit is set, it is guaranteed the
      // utilization of different shire caches for consecutive scratchpad lines.
      if (is_scp_region == 1'b1) begin
         if (paddr[30] == 1'b1) begin
            evl_paddr_t scp_paddr = '0;
            scp_paddr[39:31] = 9'b1;
            scp_paddr[   30] = paddr[29];    //Specified in the PRM
            scp_paddr[29:28] = paddr[29:28]; //shire_id[6:5]
            scp_paddr[27:23] = paddr[10: 6]; //shire_id[4:0]
            scp_paddr[22: 6] = paddr[27:11]; //l2_scp_offset[22:6]
            scp_paddr[ 5: 0] = paddr[ 5: 0]; //l2_scp_offset[5:0]
            return scp_paddr;
         end
         else begin
            // bit 30 is a copy of bit 29 even if bit30 is not set
            paddr[30] = paddr[29]; //Specified in the PRM
         end
      end
      // Otherwise, the address is the same
      return paddr;
   endfunction : get_l2scp_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_core_request
   // This function performs the necessary actions when a request is received from a core
   //
   function void process_core_request(input evl_req_desc req_desc, int src_core_id);
      evl_req_desc l2_req;
      evl_req_desc coop_req_desc;
      bit is_local_msg = 0;

      // All opcodes generate an L2 Request apart from Messages (fast-pair) and ReadCoop (slave) which are handled inside
      // the neigh. By default we set it to 1 as those cases will properly set 'is_l2_req' if it is not the case.
      bit is_l2_req = 1;
      bit is_l2_coop_req = 0;

      print_req_desc("Core to Neigh (Observed REQ)", req_desc);

      // Since the minions do not set the source field on ET-LINK (they are a final agent with a dedicated ET-LINK)
      // we need to set it based on the core agent id.
      req_desc.set_bus_req_source(src_core_id);

      if ( m_enabled == 1'b1) req_desc.set_observed();

      // If the address is not in SCP Region, get_l2scp_addr() returns the same address
      // Else if the bit 30 is set, returns the correct translated SCP address
      req_desc.set_paddr(get_l2scp_addr(req_desc.get_paddr()));
      if (is_l2scp_fill(req_desc) == 1'b1) begin
         // ET_LINK_REQ_ScpFill requests also contain an address in the ET-Link Req Data field.
         // If the address also falls into the SCP region, we must translate it if the bit 30 is set.
         req_desc.set_bus_scp_fill_addr(get_l2scp_addr(req_desc.get_bus_scp_fill_addr()));
      end

      case (req_desc.get_bus_cmd())
         ET_LINK_REQ_ReadCoop : begin
            // Tensor loads (Coop)
            process_core_request_ctl (req_desc, coop_req_desc, is_l2_req, src_core_id);
            is_l2_coop_req = is_l2_req;
         end
         ET_LINK_REQ_WriteAround : begin
            // Tensor stores (Coop)
            process_core_request_cts (req_desc, coop_req_desc, is_l2_req, src_core_id);
            is_l2_coop_req = is_l2_req;
         end
         ET_LINK_REQ_MsgSendData : begin
            // Messages handling (due to port or tensor Reduce)
            process_core_request_messages(req_desc, is_l2_req, is_local_msg, src_core_id);
         end
         default: begin
            // do nothing
         end
      endcase

      // Enqueue request descriptor only if an L2 request should be generated according to the specs.
      // Last ReadCoop      -> L2 Req
      // Last WriteAround   -> L2 Req
      // Slow lane messages -> L2 Req
      // Normal Load/Stores -> L2 Req
      // I-cache Prefetch   -> L2 Req
      if (is_l2_req) begin
         if (!$cast(l2_req, (is_l2_coop_req ? coop_req_desc : req_desc.clone()))) begin
            sb_error("neigh_scoreboard: l2_req cast failed");
         end
         else begin
            if ((req_desc.get_bus_cmd() == ET_LINK_REQ_MsgSendData))  begin
               // If goes to L2 but goes to a Neigh in the same shire (slowpairs included)
               // the bus bank id is decided by bits [19:16] instead of the UC Bank (4)
               // If it goes to a remote shire, then bank 4 is set (UC).
               // get_bank_from_message() handles all this logic automatically
               ncb_set_bus_bank(l2_req, evl_sc_bank_id_t'(m_global_attrs.get_bank_from_message(get_shire_id(), req_desc.get_paddr())), NCB_SOURCE_DCACHE);
               l2_req.set_rsp_data(req_desc.get_req_data());
               l2_req.set_bus_rsp_type(ET_LINK_RSP_MsgRcvData);
               l2_req.set_bus_rsp_id(req_desc.get_bus_rsp_id());
               l2_req.set_bus_rsp_valid(0, get_cycle_count());
            end
            else if (l2_req.get_bus_cmd() == ET_LINK_REQ_Atomic) begin
               //Get atomic configuration (L2/L3
               evl_bus_atomic_conf_t atomic_conf = l2_req.get_bus_atomic_conf();
               evl_atomic_op_t       atomic_op   = evl_atomic_op_t'(atomic_conf[3:0]);
               `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), l2_req, $sformatf("%s %s Atomic for address 0x%h", atomic_op.name(), atomic_conf[6] ? "L3" : "L2", l2_req.get_paddr()))

               if (atomic_conf[6]) begin
                  // L3 Atomic goes to UC Box
                  ncb_set_bus_bank(l2_req, evl_sc_bank_id_t'(UC_BOX_BANK), NCB_SOURCE_DCACHE);
               end
               else begin
                  // L2 Atomic goes to normal L2 bank
                  ncb_set_bus_bank(l2_req, evl_sc_bank_id_t'(get_bank_from_address(l2_req.get_paddr())), NCB_SOURCE_DCACHE);
               end
            end
            else begin
               ncb_set_bus_bank(l2_req, evl_sc_bank_id_t'(get_bank_from_address(l2_req.get_paddr())), NCB_SOURCE_DCACHE);
            end

            // Cooperative Tensor Stores always translate to Write
            if (l2_req.get_bus_cmd() == ET_LINK_REQ_WriteAround) begin
               l2_req.set_bus_cmd(ET_LINK_REQ_Write);
            end

            void'(req_desc.add_child_desc(l2_req));
            l2_req.set_req_data(req_desc.get_req_data());
            m_pending_shire_reqs.push_back(l2_req);
            l2_req.set_abstract_name(get_abstract_name());
            is_l2_coop_req = 0;
            `evl_log_mdesc(UVM_MEDIUM, m_vmod, get_abstract_name(), l2_req, $sformatf("Core to Neigh (Prediction): L2 pending shire request %s", l2_req.sprint_obj()))
            print_req_desc("Core to Neigh (l2_req)", l2_req);
         end
      end
      if ( m_enabled == 1'b1) req_desc.set_processed();

   endfunction: process_core_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_pmu_request
   // This function performs the necessary actions when a PMU event is received from a core
   //
   function void process_pmu_request(input evl_pmu_desc pmu_desc, int src_core_id = -1);
      evl_pmu_cntr_id_t       pmu_cntr_id;
      evl_pmu_event_type_t    pmu_event_type;
      evl_pmu_txn_type_t      pmu_txn_type;
      evl_dword_t             pmu_cntr_data;
      evl_thread_id_t         thread_id;
      evl_dword_t             pmu_expected_data;
      evl_dword_t             pmu_overflow_data;


      pmu_cntr_id    = pmu_desc.get_cntr_id();
      pmu_event_type = pmu_desc.get_event_type();
      pmu_txn_type   = pmu_desc.get_txn_type();
      pmu_cntr_data  = pmu_desc.get_cntr_data();
      thread_id      = pmu_desc.get_thread_id();

      case (pmu_txn_type)
         PMU_TXN_CNTR_WR   : begin
                              `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x Writing 0x%0x to mhpmcounter%0d", 
                                          src_core_id, thread_id, pmu_cntr_data, pmu_cntr_id))
                              m_pmu_counters[pmu_cntr_id][thread_id] = pmu_cntr_data;
                              `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x: mhpmcounter%0d = 0x%0x",
                                          src_core_id, thread_id, pmu_cntr_id, m_pmu_counters[pmu_cntr_id][thread_id]))
                             end
         PMU_TXN_CNTR_RD   : begin
                              `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x Reading from mhpmcounter%0d", 
                                          src_core_id, thread_id, pmu_cntr_id))
                              `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("%p: Expected value: %0d, Observed value: %0d", 
                                          pmu_txn_type, m_pmu_counters[pmu_cntr_id][thread_id], pmu_cntr_data))
                              pmu_expected_data = m_pmu_counters[pmu_cntr_id][thread_id];
                              pmu_overflow_data = pmu_expected_data - 64'h80;
                              if (!((pmu_cntr_data == pmu_expected_data) || (pmu_cntr_data == (pmu_expected_data - 1)) || (pmu_cntr_data == pmu_overflow_data) || (pmu_cntr_data == (pmu_overflow_data - 1)))) begin
                                 sb_error($sformatf("PMU Counter %0d: Value read by RTL 0x%0x doesn't match expected value 0x%0x or expected overflow value 0x%0x", pmu_cntr_id, pmu_cntr_data, pmu_expected_data, pmu_overflow_data));
                              end
                             end
         PMU_TXN_CNTR_INCR : begin
                              m_pmu_counters[pmu_cntr_id][thread_id]++;
                              if (src_core_id != -1) begin
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x saw %p. Incrementing mhpmcounter%0d", 
                                             src_core_id, thread_id, pmu_event_type, pmu_cntr_id))
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x: mhpmcounter%0d = 0x%0x",
                                             src_core_id, thread_id, pmu_cntr_id, m_pmu_counters[pmu_cntr_id][thread_id]))
                              end
                              else begin
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Saw %p. Incrementing mhpmcounter%0d (T%0x)", 
                                             pmu_event_type, pmu_cntr_id, thread_id))
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("mhpmcounter%0d (T%0x) = 0x%0x",
                                             pmu_cntr_id, thread_id, m_pmu_counters[pmu_cntr_id][thread_id]))
                              end
                             end
         PMU_TXN_EVENT_WR  : begin
                              if (src_core_id != -1) begin
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Core %0d, Thread %0x Writing 0x%0x to mhpmevent%0d", 
                                             src_core_id, thread_id, pmu_event_type, pmu_cntr_id))
                              end
                              else begin
                                 `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Writing 0x%0x to mhpmevent%0d (T%0x)", 
                                             pmu_event_type, pmu_cntr_id, thread_id))
                              end
                             end
      endcase
   endfunction: process_pmu_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   //    The ETLink bus VC will ask if observed requests are to be checked.
   //    The neigh scoreboard doesn't model icache/ptw so those should be ignored.
   //    Similarly if the scoreboard if disabled, no request should cause an error.
   //
   function bit is_req_checked(input evl_req_desc req_desc_in);
      return ((m_enabled == 1'b1) && (req_desc_in.get_bus_req_source() < 8)) ? 1'b1 : 1'b0;
   endfunction : is_req_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      evl_req_desc         first_req_desc;
      evl_req_desc         match_req_desc;
      evl_bus_cmd_t        bus_cmd;
      evl_paddr_t          address;
      evl_bus_req_id_t     req_id;
      evl_bus_req_source_t source;
      bit                  ignore_id;

      if (paired_req_desc == null) begin
         bus_cmd  = bus_cmd_in;
         req_id   = req_id_in;
         address  = address_in;
         source   = port_in;
      end
      else begin
         bus_cmd  = (bus_cmd_in  != `EVL_Bus_Idle)         ? bus_cmd_in  : paired_req_desc.get_bus_cmd();
         req_id   = (req_id_in   != ~evl_bus_req_id_t'(0)) ? req_id_in   : paired_req_desc.get_bus_req_id();
         address  = (address_in  != ~evl_paddr_t'(0))      ? address_in  : paired_req_desc.get_paddr();
         source   = paired_req_desc.get_bus_req_source();
      end

      ignore_id = 1'b0;
      find_pending_request(m_pending_shire_reqs, first_req_desc, match_req_desc, req_id, bus_cmd, address, source, ignore_id);
      if (match_req_desc != null) begin
         if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc) && (paired_req_desc.get_bus_req_source() != ET_LINK_NEIGH_SRC_Icache) ) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_response
   //
   virtual function evl_req_desc find_observed_response(input int                 port_in,
                                                        input evl_req_desc        paired_req_desc,
                                                        input bit                 compare_fields = 1'b0,
                                                        input byte                qualifier      = 0,
                                                        input evl_bus_rsp_type_t  rsp_type_in    = `EVL_Rsp_Null,
                                                        input evl_bus_req_id_t    req_id_in      = ~evl_bus_req_id_t'(0),
                                                        input evl_bus_rsp_dest_t  dest_id_in     = ~evl_bus_rsp_dest_t'(0),
                                                        input evl_xword_byte_en_t rsp_byte_en_in = evl_xword_byte_en_t'(0),
                                                        input evl_xword_t         rsp_data_in    = evl_xword_t'(0));
      evl_req_desc req_desc       = null;
      evl_req_desc first_req_desc = null;
      evl_paddr_t p_addr;
      evl_req_desc_queue pend_msg_q;

      int minion_id = port_in >> 1;
      `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Neigh to Core (Predicted ): find_observed_response() Response going to Minion-%0d", minion_id))

      if (rsp_type_in == RspMsgData) begin
         pend_msg_q = m_core_pend_msg_q[minion_id];
         for (int jj = 0; jj < pend_msg_q.size(); jj++) begin
            req_desc = pend_msg_q.get(jj);
            if (first_req_desc == null) begin
               first_req_desc = req_desc;
            end
            if ((req_desc.compare_rsp_xword(rsp_byte_en_in, rsp_data_in) ==      1'b0) &&
                (req_desc.get_bus_rsp_id()                               == req_id_in) ) begin
               print_req_desc("Neigh to Core (Predicted REQ)", req_desc);
               // We delete it from the pending ones
               req_desc.set_done();
               m_core_pend_msg_q[minion_id].pop(req_desc);
               // Found a match! :)
               break;
            end
            req_desc = null;
         end

         if (req_desc != null) begin
            // If we matched the message we print it
            p_addr = req_desc.get_paddr();
            `evl_log_mdesc(UVM_DEBUG, m_vmod, get_abstract_name(), req_desc, $sformatf("Neigh to Core (Predicted ): find_observed_response() ID:%0d, found matching request for ET_LINK_RSP_MsgRcvData (addr 0x%x)", req_desc.get_bus_req_id(), p_addr))
            if (paired_req_desc != null) begin
               req_desc.add_child_desc(paired_req_desc);
            end
         end
         else begin
            // If we do not find it but the queue is not empty, we print the most recent one (useful for debug)
            print_req_desc("Neigh to Core (Most recent REQ)", first_req_desc);
            sb_error($sformatf("Neigh to Core: Matching request not found! Core %0d spurious ET_LINK_RSP_MsgRcvData response without a matching ET_LINK_REQ_MsgSendData", minion_id));
         end
      end
      return req_desc;
   endfunction : find_observed_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_core_miss_req_export") || (port_name == "core_miss_req_export")) begin
         if (port_number < m_core_miss_req_export.size()) begin
            return m_core_miss_req_export[port_number];
         end
      end
      if ((port_name == "m_core_miss_rsp_export") || (port_name == "core_miss_rsp_export")) begin
         if (port_number < m_core_miss_rsp_export.size()) begin
            return m_core_miss_rsp_export[port_number];
         end
      end

      if ((port_name == "m_core_evict_req_export") || (port_name == "core_evict_req_export")) begin
         if (port_number < m_core_evict_req_export.size()) begin
            return m_core_evict_req_export[port_number];
         end
      end
      if ((port_name == "m_core_evict_rsp_export") || (port_name == "core_evict_rsp_export")) begin
         if (port_number < m_core_evict_rsp_export.size()) begin
            return m_core_evict_rsp_export[port_number];
         end
      end
      if ((port_name == "m_shire_req_export") || (port_name == "shire_req_export")) begin
         return m_shire_req_export;
      end
      if ((port_name == "m_shire_rsp_export") || (port_name == "shire_rsp_export")) begin
         return m_shire_rsp_export;
      end

      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pmu_desc_export
   //
   virtual function uvm_analysis_export#(evl_pmu_desc) get_pmu_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_core_pmu_req_export") || (port_name == "core_pmu_req_export")) begin
         if (port_number < m_core_pmu_req_export.size()) begin
            return m_core_pmu_req_export[port_number];
         end
      end

      return null;
   endfunction : get_pmu_desc_export


   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_neigh_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         sb_error("neigh_scoreboard: m_rtl_port is null, SAD!");
      end
      if (m_neigh_rtl_port == null) begin
         sb_error("neigh_scoreboard: evl_neigh_rtl_port is null, SAD!");
      end

      if (get_int_param(pk_core_req_port_count) > 0) begin
         m_core_miss_req_export  = new[get_int_param(pk_core_req_port_count)];
         m_core_miss_req_fifo    = new[get_int_param(pk_core_req_port_count)];
         m_core_miss_rsp_export  = new[get_int_param(pk_core_req_port_count)];
         m_core_miss_rsp_fifo    = new[get_int_param(pk_core_req_port_count)];

         m_core_evict_req_export = new[get_int_param(pk_core_req_port_count)];
         m_core_evict_req_fifo   = new[get_int_param(pk_core_req_port_count)];
         m_core_evict_rsp_export = new[get_int_param(pk_core_req_port_count)];
         m_core_evict_rsp_fifo   = new[get_int_param(pk_core_req_port_count)];
         m_core_pend_msg_q       = new[get_int_param(pk_core_req_port_count)];

         m_core_pmu_req_export   = new[get_int_param(pk_core_req_port_count)];
         m_core_pmu_req_fifo     = new[get_int_param(pk_core_req_port_count)];
      end
      for (int ii = 0; ii < get_int_param(pk_core_req_port_count); ii++) begin
         m_core_miss_req_export[ii]  = new($sformatf("m_core_miss_req_export[%0d]", ii), this);
         m_core_miss_req_fifo[ii]    = new($sformatf("m_core_miss_req_fifo[%0d]", ii), this);
         m_core_miss_rsp_export[ii]  = new($sformatf("m_core_miss_rsp_export[%0d]", ii), this);
         m_core_miss_rsp_fifo[ii]    = new($sformatf("m_core_miss_rsp_fifo[%0d]", ii), this);

         m_core_evict_req_export[ii] = new($sformatf("m_core_evict_req_export[%0d]", ii), this);
         m_core_evict_req_fifo[ii]   = new($sformatf("m_core_evict_req_fifo[%0d]", ii), this);
         m_core_evict_rsp_export[ii] = new($sformatf("m_core_evict_rsp_export[%0d]", ii), this);
         m_core_evict_rsp_fifo[ii]   = new($sformatf("m_core_evict_rsp_fifo[%0d]", ii), this);
         m_core_pend_msg_q[ii]       = new($sformatf("%0s(MSG-%0d)", get_root_abstract_name(), ii));

         m_core_pmu_req_export[ii]   = new($sformatf("m_core_pmu_req_export[%0d]", ii), this);
         m_core_pmu_req_fifo[ii]     = new($sformatf("m_core_pmu_req_fifo[%0d]", ii), this);
      end
      m_shire_req_export = new("m_shire_req_export", this);
      m_shire_req_fifo   = new("m_shire_req_fifo", this);
      m_shire_rsp_export = new("m_shire_rsp_export", this);
      m_shire_rsp_fifo   = new("m_shire_rsp_fifo", this);



   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      for (int ii = 0; ii < m_core_miss_req_export.size(); ii++) begin
         m_core_miss_req_export[ii].connect(m_core_miss_req_fifo[ii].analysis_export);
         m_core_miss_rsp_export[ii].connect(m_core_miss_rsp_fifo[ii].analysis_export);
      end
      for (int ii = 0; ii < m_core_evict_req_export.size(); ii++) begin
         m_core_evict_req_export[ii].connect(m_core_evict_req_fifo[ii].analysis_export);
         m_core_evict_rsp_export[ii].connect(m_core_evict_rsp_fifo[ii].analysis_export);
      end
      for (int ii = 0; ii < m_core_pmu_req_export.size(); ii++) begin
         m_core_pmu_req_export[ii].connect(m_core_pmu_req_fifo[ii].analysis_export);
      end

      m_shire_req_export.connect(m_shire_req_fifo.analysis_export);
      m_shire_rsp_export.connect(m_shire_rsp_fifo.analysis_export);
   endfunction: connect_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      if (m_enabled == 1'b0) begin
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("Neighbourhood Scoreboard is disabled"))
      end
      else begin
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("Neighbourhood Scoreboard is enabled"))
      end

      if (m_ncb_force_dest_fifo != 0) begin
         string ncb_log_msg = "Neigh Chicken Bits: Banks redirection enabled.";
         if (m_ncb_force_all_agents != 0) begin
            ncb_log_msg = $sformatf("%s All agents are", ncb_log_msg);
         end
         else begin
            ncb_log_msg = $sformatf("%s Agent %d is", ncb_log_msg, m_ncb_agent_forced);
         end
         ncb_log_msg = $sformatf("%s redirected to bank %d", ncb_log_msg, m_ncb_dest_fifo);
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), ncb_log_msg);
      end
      else begin
         `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("Neigh Chicken Bits: Banks redirection disabled."))
      end

      fork
         super.run_phase(phase);
         fork
            watch_for_timeouts();
            watch_for_bus_requests();
            watch_for_rtl_events();
            watch_for_pmu_events();
         join_none;
      join_none;
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in);

      m_vmod   = VMOD_NEIGH_SB;
      m_err_pk = pk_neighsb_error_intensity;

      if (!`et_value_plusargs("neigh_sb_en=%d", m_enabled)) begin
         m_enabled = 1'b1;
      end


      foreach(m_pmu_counters[ii]) begin
         m_pmu_counters[ii] = '{evl_dword_t'(0), evl_dword_t'(0)};
      end

      // Neigh Chicken Bits (NCB)
      if (!`et_value_plusargs("agent_forced=%d", m_ncb_agent_forced)) m_ncb_agent_forced = 0;
      if (!`et_value_plusargs("force_all_agents=%d", m_ncb_force_all_agents)) m_ncb_force_all_agents = 0;
      if (!`et_value_plusargs("dest_fifo=%d", m_ncb_dest_fifo)) m_ncb_dest_fifo = 0;
      if (!`et_value_plusargs("force_dest_fifo=%d", m_ncb_force_dest_fifo)) m_ncb_force_dest_fifo = 0;

      `evl_set_error_intensity(m_err_pk, m_enabled ? -1 : UVM_HIGH);
   endfunction : new

   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      super.shutdown_phase(phase);

      m_neigh_rtl_port.shutdown_started();
   endtask : shutdown_phase

   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);
      if (m_pending_shire_reqs.size() != 0) begin
         foreach (m_pending_shire_reqs[ii]) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), m_pending_shire_reqs[ii], $sformatf("m_pending_shire_reqs[%0d] = %0s", ii, m_pending_shire_reqs[ii].sprint_obj()))
         end
         sb_error("Pending Shire Requests queue is not empty");
      end
      if (m_slowpair_pend_msg_q.size() != 0) begin
         foreach (m_slowpair_pend_msg_q[ii]) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), m_slowpair_pend_msg_q[ii], $sformatf("m_slowpair_pend_msg_q[%0d] = %0s", ii, m_slowpair_pend_msg_q[ii].sprint_obj()))
         end
         sb_error("Slow-Pair Pending Messages queue is not empty");
      end
      foreach (m_core_pend_msg_q[ii]) begin
         evl_req_desc_queue pend_msg_q = m_core_pend_msg_q[ii];
         if (pend_msg_q.size() != 0) begin
            for (int i = 0; i < pend_msg_q.size(); i++) begin
               evl_req_desc req_desc = pend_msg_q.get_desc(i);
               `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), req_desc, $sformatf("m_core_pend_msg_q[%0d][%0d] = %0s", ii, i, req_desc.sprint_obj()))
            end
            sb_error("To-Minions Pending Message queue is not empty");
         end
      end
      if (m_store_coop_q.size() != 0) begin
         foreach (m_store_coop_q[ii]) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), m_store_coop_q[ii], $sformatf("m_store_coop_q[%0d] = %0s", ii, m_store_coop_q[ii].sprint_obj()))
         end
         sb_error("Cooperative Tensor Store Pending queue is not empty");
      end
      foreach (m_read_coop_q[ii]) begin
         if (m_read_coop_q[ii].ctl_q.size() != 0) begin
            for (int i = 0; i < m_read_coop_q[ii].ctl_q.size(); i++) begin
               `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), m_read_coop_q[ii].ctl_q.get(i), $sformatf("m_read_coop_q[%0d].ctl_q[%0d] = %0s", ii, i, m_read_coop_q[ii].ctl_q.get(i).sprint_obj()))
            end
            sb_error("Cooperative Tensor Load Pending queue is not empty");
         end
      end
      if (m_active_shire_reqs.size() != 0) begin
         foreach (m_active_shire_reqs[ii]) begin
            `evl_log_mdesc(UVM_LOW, m_vmod, get_abstract_name(), m_active_shire_reqs[ii], $sformatf("m_active_shire_reqs[%0d] = %0s", ii, m_active_shire_reqs[ii].sprint_obj()))
         end
         sb_error("Active Shire Requests queue is not empty");
      end
   endtask : post_shutdown_phase

endclass : evl_neigh_scoreboard

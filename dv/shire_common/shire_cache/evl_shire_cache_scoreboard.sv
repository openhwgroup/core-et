//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_scoreboard
//
//
class evl_shire_cache_scoreboard #(type CACHE_TYPE = evl_shire_cache_scoreboard) extends evl_scoreboard_mem;

   `uvm_component_param_utils(evl_shire_cache_scoreboard #(CACHE_TYPE))

   typedef evl_req_desc evl_sc_req_desc_q[$];
   typedef evl_paddr_t  evl_paddr_q[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_after_tc_req)
   `evl_param_key(pk_bypass_for_parent)
   `evl_param_key(pk_etlink_cmd)
   `evl_param_key(pk_idx_cache_op)
   `evl_param_key(pk_issue_late_read)
   `evl_param_key(pk_l2_opcode)
   `evl_param_key(pk_paired_reqq_id)
   `evl_param_key(pk_parent_done_on_rsp)
   `evl_param_key(pk_pass_count)
   `evl_param_key(pk_random_sc_test)
   `evl_param_key(pk_remote_shire)
   `evl_param_key(pk_reqq_id)
   `evl_param_key(pk_sc_addr_space)
   `evl_param_key(pk_sc_desc_number)
   `evl_param_key(pk_sc_downstream_port)
   `evl_param_key(pk_sc_sb_error_intensity)
   `evl_param_key(pk_upstream_req_port)

   static semaphore serialize_print = new(1);


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_shire_cache_reg_block m_regs[evl_agent_id_t];
   evl_shire_cache_rtl_port  m_shire_cache_rtl_port       = null;
   evl_shire_cache_reqq      m_shire_cache_reqq           = null;
   evl_shire_cache_axi_meshq m_shire_cache_axi_meshq      = null;
   int                       m_sb_type                    = 0;
   evl_paddr_t               m_set_base_addr              = evl_paddr_t'(0);
   evl_paddr_t               m_cache_model_base_addr      = evl_paddr_t'(0);
   evl_paddr_t               m_index_mask                 = evl_paddr_t'(0);
   evl_paddr_t               m_stim_index_mask            = evl_paddr_t'(0);
   evl_paddr_t               m_alias_mask                 = evl_paddr_t'(0);
   int                       m_index_msb                  = 0;
   int                       m_raw_set_size               = 0;
   int                       m_set_size                   = 0;
   int                       m_mem_size                   = 0;
   int                       m_num_ways                   = 0;
   int                       m_bank_lsb                   = 0;
   bit                       m_perform_qword_en_check     = 1'b0;
   bit                       m_mask_with_base_and_limit   = 1'b0;
   bit                       m_extract_etlink_from_awuser = 1'b0;
   int                       m_base_port_num              = 0;
   bit                       m_remote_enable              = 1'b1;
   bit                       m_remote_l3_enable           = 1'b1;
   bit                       m_bypass                     = 1'b0;
   bit                       m_err_rsp_enable             = 1'b1;
   bit [7:0]                 m_err_interrupt_enable       = 0;
   bit                       m_two_shire_aliasing_use_lsb = 1'b0;
   bit                       m_all_shire_aliasing         = 1'b0;
   evl_verbosity_mod_t       m_verbosity_mod              = VMOD_SC_SB_WARN;
   int                       m_neigh_id[8];
   int                       m_coalesce_size[`SC_BANKS << 1];
   bit [`SC_BANKS-1:0]       m_l2_rbuf_enable;
   bit [`SC_BANKS-1:0]       m_scp_rbuf_enable;
   bit [`SC_BANKS-1:0]       m_zero_st_enable;
   bit [`SC_BANKS-1:0]       m_cbuf_enable;
   bit [2:0]                 m_ram_delay[`SC_BANKS << 1];
   evl_req_desc              m_req_queues[`SC_BANKS << 1][`SC_REQQ_DEPTH];
   evl_req_desc              m_pending_evicts[`SC_BANKS << 1][`SC_REQQ_DEPTH];
   evl_sc_req_desc_q         m_pending_fills[`SC_BANKS << 1];
   evl_sc_req_desc_q         m_pending_cbuf_evicts[`SC_BANKS << 1];
   evl_sc_req_desc_q         m_pending_atomics[`SC_BANKS << 1];
   evl_sc_req_desc_q         m_pending_downstream_rsp[`SC_BANKS << 1];
   evl_req_desc_queue        m_pending_message_queues[16];
   evl_req_desc_queue        m_pending_coop_rsp[`NUM_NEIGH];
   evl_req_desc              m_tag_rsp_queue[$];
   evl_line_desc             m_victim_line_queue[$];
   evl_req_desc_queue        m_coalescing_buffer[int];
   evl_sc_pipe_info_t        m_pipe_info_tc[int];
   evl_bus_parser            m_bus_parser_et;
   evl_line_data             m_template_req_data_et[16];
   evl_line_desc             m_template_line_desc_et[16];
   evl_trans_desc            m_template_trans_desc_et[16];
   evl_req_desc              m_template_req_desc_et[16];
   evl_line_data             m_template_req_data_axi;
   evl_line_desc             m_template_line_desc_axi;
   evl_trans_desc            m_template_trans_desc_axi;
   evl_req_desc              m_template_req_desc_axi;
   int                       m_delayed_inv_cycle_counts[$];
   int                       m_delayed_inv_cycle_banks[$];
   evl_line_desc             m_delayed_inv_line_descs[$];
   evl_sc_reqq_opcode_t      m_merge_partial_opcodes[$];
   evl_sc_reqq_opcode_t      m_check_partial_opcodes[$];
   evl_sc_reqq_opcode_t      m_index_cacheop_opcodes[$];
   evl_paddr_q               m_rtl_cbuf_addr_queues[`SC_BANKS << 1];
   evl_paddr_q               m_mdl_cbuf_addr_queues[`SC_BANKS << 1];

   evl_sc_scoreboard_cache #(CACHE_TYPE) m_cache;
   evl_shire_cache_config_cover          m_sc_config_cover;
   evl_shire_cache_scoreboard_cover      m_shire_cache_scoreboard_cover;

   //For checking RTLMIN-5405 stronger sync on cb invalidates.
   bit                       m_cb_inv_sync_check_in_progress[`SC_BANKS << 1];
   evl_req_desc_queue        m_reqs_ahead_of_cb_inv[`SC_BANKS << 1];
   evl_req_desc_queue        m_coalescing_buffer_snapshot[int];
   evl_req_desc_queue        m_rsp_queues[`SC_BANKS];


   //-----------------------------------------------------------------------------------------------
   function string get_way_name(input evl_way_t way_in);
      case (way_in)
         EVL_WAY_0: return "0";
         EVL_WAY_1: return "1";
         EVL_WAY_2: return "2";
         EVL_WAY_3: return "3";
         default:   return "none";
      endcase
   endfunction : get_way_name


   //-----------------------------------------------------------------------------------------------
   function set_shire_cache_reg_block(input evl_shire_cache_reg_block reg_block_in[evl_agent_id_t]);
      m_regs = reg_block_in;
   endfunction: set_shire_cache_reg_block


   //-----------------------------------------------------------------------------------------------
   function int gen_bus_req_id(input int bank_num, input evl_sc_reqq_id_t reqq_id);
      return (int'(reqq_id) | (bank_num << `SC_REQQ_ID_SIZE));
   endfunction : gen_bus_req_id


   //-----------------------------------------------------------------------------------------------
   function void set_shire_cache_reqq(input evl_shire_cache_reqq shire_cache_reqq_in);
      m_shire_cache_reqq = shire_cache_reqq_in;
   endfunction : set_shire_cache_reqq


   //-----------------------------------------------------------------------------------------------
   function void set_shire_cache_axi_meshq(input evl_shire_cache_axi_meshq shire_cache_axi_meshq_in);
      m_shire_cache_axi_meshq = shire_cache_axi_meshq_in;
   endfunction : set_shire_cache_axi_meshq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_reqq_req_act
   //
   function void add_reqq_req_act(input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                  input evl_sc_reqq_opcode_t    opcode_in,
                                  input evl_cache_st_t          obs_st_in,
                                  input evl_cache_st_t          model_st_list_in[],
                                  input evl_sc_qual_t           qualifier_in,
                                  input evl_cache_st_t          tag_action_in,
                                  input evl_data_action_t       data_action_in,
                                  input evl_abstract_bus_cmd_t  bus_cmd_in,
                                  input evl_abstract_rsp_type_t bus_rsp_in,
                                  input int                     err_code_in,
                                  input int                     no_cover_in = 1'b0);
      if (m_cache != null) begin
         m_cache.add_int_req_act(SC_TABLE_REQQ_REQ, etlink_bus_cmd_in, opcode_in, obs_st_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, int'(bus_rsp_in), err_code_in, no_cover_in);
      end
   endfunction : add_reqq_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_rbuf_req_act
   //
   function void add_rbuf_req_act(input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                  input evl_sc_reqq_opcode_t    opcode_in,
                                  input evl_cache_st_t          obs_st_in,
                                  input evl_cache_st_t          model_st_list_in[],
                                  input evl_sc_qual_t           qualifier_in,
                                  input evl_cache_st_t          tag_action_in,
                                  input evl_data_action_t       data_action_in,
                                  input evl_abstract_bus_cmd_t  bus_cmd_in,
                                  input evl_abstract_rsp_type_t bus_rsp_in,
                                  input int                     err_code_in,
                                  input int                     no_cover_in = 1'b0);
      if (m_cache != null) begin
         m_cache.add_int_req_act(SC_TABLE_RBUF_REQ, etlink_bus_cmd_in, opcode_in, obs_st_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, int'(bus_rsp_in), err_code_in, no_cover_in);
      end
   endfunction : add_rbuf_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tc_req_act
   //
   function void add_tc_req_act(input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                input evl_sc_reqq_opcode_t    opcode_in,
                                input evl_cache_st_t          obs_st_in,
                                input evl_cache_st_t          model_st_list_in[],
                                input evl_sc_qual_t           qualifier_in,
                                input evl_cache_st_t          tag_action_in,
                                input evl_data_action_t       data_action_in,
                                input evl_abstract_bus_cmd_t  bus_cmd_in,
                                input evl_abstract_rsp_type_t bus_rsp_in,
                                input int                     err_code_in,
                                input int                     no_cover_in = 1'b0);
      if (m_cache != null) begin
         m_cache.add_int_req_act(SC_TABLE_TC_REQ, etlink_bus_cmd_in, opcode_in, obs_st_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, int'(bus_rsp_in), err_code_in, no_cover_in);
      end
   endfunction : add_tc_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_tag_rsp_act
   //
   function void add_tag_rsp_act(input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                 input evl_sc_reqq_opcode_t    opcode_in,
                                 input evl_cache_st_t          obs_st_in,
                                 input evl_cache_st_t          model_st_list_in[],
                                 input evl_sc_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_bus_cmd_t  bus_cmd_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in,
                                 input int                     err_code_in,
                                 input int                     no_cover_in = 1'b0);
      if (m_cache != null) begin
         m_cache.add_int_req_act(SC_TABLE_TAG_RSP, etlink_bus_cmd_in, opcode_in, obs_st_in, model_st_list_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in, int'(bus_rsp_in), err_code_in, no_cover_in);
      end
   endfunction : add_tag_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                 input evl_axi_req_type_t      axi_bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_resp_t          bus_rsp_in,
                                 input evl_sc_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t new_bus_rsp_in,
                                 input evl_abstract_bus_cmd_t  new_bus_cmd_in,
                                 input int                     err_code_in,
                                 input int                     no_cover_in = 1'b0);
      if (m_cache != null) begin
         m_cache.add_bus_rsp_act(etlink_bus_cmd_in, axi_bus_cmd_in, model_st_in, bus_rsp_in, qualifier_in, tag_action_in, data_action_in, evl_bus_rsp_type_t'(new_bus_rsp_in), evl_bus_cmd_t'(new_bus_cmd_in), err_code_in, no_cover_in);
      end
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_pending_fill
   //
   function void enqueue_pending_fill(input evl_req_desc req_desc);
      evl_sc_bank_id_t bank_id;
      evl_sc_reqq_id_t reqq_id;

      bank_id = req_desc.get_bus_bank();
      reqq_id = evl_sc_reqq_id_t'(req_desc.get_int_param(pk_reqq_id));

      for (int ii = m_pending_fills[bank_id].size() - 1; ii >= 0; ii--) begin
         if ((m_pending_fills[bank_id][ii].get_int_param(pk_reqq_id)            == int'(reqq_id)) ||
             (m_pending_fills[bank_id][ii].get_int_param(pk_paired_reqq_id, -1) == int'(reqq_id))) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_pending_fills[%0d][%0d] contains an entry for reqq_id 0x%x (%0s, TrID 0x%x, id %0d) (file %0s, line %0d)", bank_id, ii, reqq_id, m_pending_fills[bank_id][ii].sprint_obj(), m_pending_fills[bank_id][ii].get_trans_id(), m_pending_fills[bank_id][ii].get_inst_id(), `__FILE__, `__LINE__), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            m_pending_fills[bank_id].delete(ii);
         end
      end

      for (int ii = m_pending_cbuf_evicts[bank_id].size() - 1; ii >= 0; ii--) begin
         if ((m_pending_cbuf_evicts[bank_id][ii].get_int_param(pk_reqq_id)            == int'(reqq_id)) ||
             (m_pending_cbuf_evicts[bank_id][ii].get_int_param(pk_paired_reqq_id, -1) == int'(reqq_id))) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_pending_cbuf_evicts[%0d][%0d] contains an entry for reqq_id 0x%x (%0s, TrID 0x%x, id %0d) (file %0s, line %0d)", bank_id, ii, reqq_id, m_pending_cbuf_evicts[bank_id][ii].sprint_obj(), m_pending_cbuf_evicts[bank_id][ii].get_trans_id(), m_pending_cbuf_evicts[bank_id][ii].get_inst_id(), `__FILE__, `__LINE__), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            m_pending_cbuf_evicts[bank_id].delete(ii);
         end
      end

      m_pending_fills[bank_id].push_back(req_desc);
   endfunction : enqueue_pending_fill


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_pending_downstream_rsp
   //
   function void enqueue_pending_downstream_rsp(input evl_req_desc req_desc);
      evl_sc_bank_id_t bank_id;
      evl_sc_reqq_id_t reqq_id;

      bank_id = req_desc.get_bus_bank();
      reqq_id = evl_sc_reqq_id_t'(req_desc.get_int_param(pk_reqq_id));
      
      `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("enqueue_pending_downstream_rsp for bank %0d, reqq ID 0x%x: %0s", bank_id, reqq_id, req_desc.sprint_obj()))

      m_pending_downstream_rsp[bank_id].push_back(req_desc);
   endfunction : enqueue_pending_downstream_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dequeue_pending_downstream_rsp
   //
   function void dequeue_pending_downstream_rsp(input evl_req_desc req_desc);
      evl_sc_bank_id_t bank_id;
      evl_sc_reqq_id_t reqq_id;

      bank_id = req_desc.get_bus_bank();
      reqq_id = evl_sc_reqq_id_t'(req_desc.get_int_param(pk_reqq_id));

      for (int ii = m_pending_downstream_rsp[bank_id].size() - 1; ii >= 0; ii--) begin
         if (m_pending_downstream_rsp[bank_id][ii] == req_desc) begin
            //found a match! remove it!
            m_pending_downstream_rsp[bank_id].delete(ii);
            return;
         end
      end
      //didn't find a match. boo
      `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("TB_FAIL: dequeue_pending_downstream_rsp couldn't find a match for this req_desc"))
   endfunction : dequeue_pending_downstream_rsp
   

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
      evl_req_desc req_desc;

      if (rsp_type_in == RspMsgData) begin
         int limit = m_pending_message_queues[port_in].size();

         if (limit == 0) begin
            return null;
         end

         //
         // All requests to a particular destination ought to be ordered (the real rule is all
         // source-to-destination requests must be ordered).
         //
         for (int ii = 0; ii < limit; ii++) begin
            req_desc = m_pending_message_queues[port_in].get_desc(ii);
            if (req_desc.get_bus_rsp_dest() == dest_id_in) begin
               if (compare_fields == 1'b0) begin
                  break;
               end
               if ( (req_desc.get_bus_req_id() != req_id_in) ||
                    (req_desc.compare_rsp_xword(rsp_byte_en_in, rsp_data_in) == 1'b1) ) begin
                  return null;
               end
            end
            req_desc = null;
         end

         req_desc = m_pending_message_queues[port_in].pop(req_desc);
         if ((paired_req_desc != null) && (req_desc != null)) begin
            req_desc.add_child_desc(paired_req_desc);
         end
         return req_desc;
      end // if (rsp_type_in == RspMsgData)

      // Deal with ReadCoop responses
      else if ((rsp_type_in inside {RspWData,RspErr}) && (dest_id_in == 12)) begin //12 is cooperative
         evl_req_desc_queue filter_queue;
         `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("find_observed_response: etlink monitor is asking about a (possible) coop rsp for port %0d",port_in))
         filter_queue = m_pending_coop_rsp[port_in].filter_bus_req_id(req_id_in);
         if (filter_queue == null) begin
            return null;
         end
         
         if (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_SB) != 0) begin
            filter_queue.print_obj("filter_queue");
         end

         // This code relies on the assumption that a neighborhood isn't going to have more than one ReadCoop outstanding
         // with the same reqid. If the neighborhood does that, the requests can get out of order, causing trouble.
         if (filter_queue.size() == 1) begin
            req_desc = filter_queue.pop();
            req_desc = m_pending_coop_rsp[port_in].pop(req_desc);
            return req_desc;
         end
         else begin
            `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("find_observed_response: expected filter queue size to be 1. not returning a req_desc."))
         end
         return null;
         
      end
      return null;
   endfunction : find_observed_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_idx_cop_request
   //
   function void enqueue_idx_cop_request(input evl_trans_id_t trid, input evl_sc_bank_id_t bank_id, input evl_sc_idx_cop_req_t rtl_info);
      `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("calling enqueue_idx_cop_request for trid %x bank_id %x", trid, bank_id))
      m_shire_cache_reqq.enqueue_idx_cop_request(trid, bank_id, rtl_info);
      if (rtl_info.opcode == SC_Sync) begin
         cb_inv_end_checks(bank_id);
      end
   endfunction : enqueue_idx_cop_request


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_mesh_response
   //
   function void process_mesh_response(input evl_req_desc rcv_req_desc);
      evl_req_desc req_desc;

      req_desc = m_shire_cache_axi_meshq.dequeue_active_mesh_request(rcv_req_desc);
      if (req_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), rcv_req_desc, $sformatf("no AXI response with ID 0x%0x expected (%0s)", rcv_req_desc.get_bus_req_id(), rcv_req_desc.sprint_rsp()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
      else begin
         process_bus_response(req_desc);
      end
   endfunction : process_mesh_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_qword_en_check
   //
   function void perform_qword_en_check(input evl_line_desc line_desc);
      if ( (line_desc.get_modified_qword_en() != evl_line_qword_en_t'(0)) &&
           (!(line_desc.get_cache_st() inside { `EVL_LINE_DIRTY_STATES, `EVL_LINE_WA_DIRTY_STATES})) ) begin //CLC WI_IE_Bsy was here??
         `dut_error(get_abstract_name(), $sformatf("cache state for 0x%x is %p, modified quadwords: %b", line_desc.get_paddr(), line_desc.get_cache_st(), line_desc.get_modified_qword_en()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
   endfunction : perform_qword_en_check


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_mask
   //
   virtual function evl_paddr_t get_index_mask();
      return ((evl_paddr_t'(1) << (m_index_msb - `EVL_LINE_ADDR_LSB)) - 1) << m_bank_lsb;
   endfunction : get_index_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_alias_mask
   //
   virtual function evl_paddr_t get_index_alias_mask();
      return (evl_paddr_t'(1) << (m_index_msb - `EVL_LINE_ADDR_LSB - 1)) << m_bank_lsb;
   endfunction : get_index_alias_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_shire_config
   //
   virtual function void process_shire_config(input evl_shire_config_t rtl_info);
      m_neigh_id[0] = int'(rtl_info.neigh_id_0);
      m_neigh_id[1] = int'(rtl_info.neigh_id_1);
      m_neigh_id[2] = int'(rtl_info.neigh_id_2);
      m_neigh_id[3] = int'(rtl_info.neigh_id_3);
   endfunction : process_shire_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_bank_config
   //
   virtual function void process_sc_bank_config(input evl_sc_bank_config_t rtl_info);
      m_raw_set_size     = rtl_info.set_size;
      m_set_size         = `EVL_LINE_BYTES * int'(rtl_info.set_size) * `SC_BANKS * `SC_SUB_BANKS;
      m_num_ways         = rtl_info.num_ways;
      m_bank_lsb         = rtl_info.bank_lsb;
      m_bypass           = rtl_info.bypass;
      m_remote_enable    = rtl_info.remote_enable;
      m_remote_l3_enable = rtl_info.remote_l3_enable;
      m_set_base_addr    = rtl_info.set_base;
      m_err_rsp_enable   = rtl_info.err_rsp_enable;
      m_err_interrupt_enable = rtl_info.err_interrupt_enable;
      m_two_shire_aliasing_use_lsb = rtl_info.two_shire_aliasing_use_lsb;
      m_all_shire_aliasing = rtl_info.all_shire_aliasing;
    
      if (m_set_size == 0) begin
         m_mem_size              = 0;
         m_index_msb             = 0;
         m_cache_model_base_addr = evl_paddr_t'(0);
         m_index_mask            = evl_paddr_t'(0);
         m_stim_index_mask       = evl_paddr_t'(0);
         m_alias_mask            = ~(`EVL_LINE_BYTES - 1);
         m_cache.set_cache_config(rtl_info.num_ways, evl_paddr_t'(0));
      end
      else begin
         evl_paddr_t set_base;
         evl_paddr_t set_top;

         m_mem_size   = (m_num_ways <= 0) ? m_set_size : (m_num_ways * m_set_size);
         m_index_msb  = $clog2(m_set_size);
         m_index_mask = get_index_mask();
         if ((1 << m_index_msb) == m_set_size) begin
            m_cache_model_base_addr = evl_paddr_t'(0);
            m_stim_index_mask       = ((evl_paddr_t'(1) << (m_index_msb + m_bank_lsb - `EVL_LINE_ADDR_LSB)) - 1) & m_index_mask;
            m_alias_mask            = ~((evl_paddr_t'(1) << (m_bank_lsb + m_index_msb)) - 1);
            m_cache.set_cache_config(rtl_info.num_ways, m_index_mask);
         end
         else begin
            evl_paddr_t index_alias;

            set_base                = `EVL_LINE_BYTES * int'(rtl_info.set_base) * `SC_BANKS * `SC_SUB_BANKS;
            set_top                 = m_index_mask & ((set_base + m_set_size) << (m_bank_lsb - `EVL_LINE_ADDR_LSB));
            m_cache_model_base_addr = m_index_mask & (set_base << (m_bank_lsb - `EVL_LINE_ADDR_LSB));
            m_stim_index_mask       = ((evl_paddr_t'(1) << (m_index_msb + m_bank_lsb - `EVL_LINE_ADDR_LSB - 1)) - 1) & m_index_mask;
            index_alias             = get_index_alias_mask();
            m_alias_mask            = ~((evl_paddr_t'(1) << (m_bank_lsb + m_index_msb)) - 1);
            m_cache.set_cache_config(rtl_info.num_ways, m_index_mask, m_cache_model_base_addr, set_top, index_alias);
         end
         m_index_msb = (m_index_msb + m_bank_lsb) - (`EVL_LINE_ADDR_LSB + 1);
      end
      if ((m_mask_with_base_and_limit == 1'b1) && (m_cache != null)) begin
         m_cache.set_base_and_top(m_cache_model_base_addr, m_set_size);
      end
      if (m_shire_cache_reqq != null) begin
         m_shire_cache_reqq.set_bank_info(`SC_BANKS, m_template_req_desc_et[15]);
      end
   endfunction : process_sc_bank_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_pipe_config
   //
   virtual function void process_sc_pipe_config(input evl_sc_pipe_config_t rtl_info);
      if (m_coalesce_size[rtl_info.bank] != rtl_info.num_entries) begin
         `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("bank %0d coalescing buffer size: %0d", rtl_info.bank, rtl_info.num_entries))
      end
      m_l2_rbuf_enable[rtl_info.bank]  = rtl_info.l2_rbuf_enable;
      m_scp_rbuf_enable[rtl_info.bank] = rtl_info.scp_rbuf_enable;
      m_zero_st_enable[rtl_info.bank]  = rtl_info.zero_state_enable;
      m_cbuf_enable[rtl_info.bank]     = rtl_info.cbuf_enable;
      m_coalesce_size[rtl_info.bank]   = rtl_info.num_entries;
      m_ram_delay[rtl_info.bank]       = rtl_info.ram_delay;
   endfunction : process_sc_pipe_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enqueue_pending_evict
   //
   function void enqueue_pending_evict(input evl_sc_bank_id_t bank_id,
                                       input evl_sc_reqq_id_t reqq_id,
                                       input evl_req_desc     req_desc,
                                       input evl_req_desc     prev_req_desc = null);
      if (req_desc != null) begin
         `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("pending evict for bank %0d, reqq ID 0x%x: %0s", bank_id, reqq_id, req_desc.sprint_obj()))
      end
      else begin
         `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), ~evl_trans_id_t'(0), reqq_id, $sformatf("pending evict for bank %0d, reqq ID 0x%x", bank_id, reqq_id))
      end
      if (m_pending_evicts[bank_id][reqq_id] != null) begin
         evl_paddr_t   addr;
         evl_line_desc line_desc;

         line_desc = m_pending_evicts[bank_id][reqq_id].get_line_desc();
         addr      = line_desc.get_paddr();
         `dut_error_mdesc(get_abstract_name(), m_pending_evicts[bank_id][reqq_id], $sformatf("victim (0x%x, bank %0d, reqq ID 0x%x) forced with %0s has not been written back", addr, bank_id, reqq_id, m_pending_evicts[bank_id][reqq_id].sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
      m_pending_evicts[bank_id][reqq_id] = req_desc;
   endfunction : enqueue_pending_evict


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_reqq_alloc
   //
   function void process_reqq_alloc(input evl_trans_id_t      trans_id,
                                    input evl_sc_bank_id_t    bank_id,
                                    input bit                 use_remote_shire_id,
                                    input evl_shire_id_t      remote_shire_id,
                                    input evl_sc_reqq_alloc_t rtl_info);
      evl_req_desc       req_desc     = null;
      evl_req_desc       tgt_req_desc = null;
      evl_table_desc     table_desc   = null;
      evl_line_desc      line_desc    = null;
      evl_bus_req_user_t aw_user      = evl_bus_req_user_t'(ET_LINK_REQ_Write);
      evl_sc_reqq_id_t   reqq_id      = rtl_info.reqq_id;
      evl_bus_cmd_t      bus_cmd      = `EVL_Bus_Idle;
      evl_bus_req_id_t   bus_req_id;
      evl_paddr_t        addr;
      
      `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("process_reqq_alloc calling perform_reqq_alloc for bank_id:%0d, reqq_id:0x%x, addr:0x%x, use_remote_shire_id:%0d, remote_shire_id:0x%x, get_shire_id:0x%x, orig_opcode:%p", bank_id, reqq_id, rtl_info.address, use_remote_shire_id, remote_shire_id, get_shire_id(), rtl_info.orig_opcode))
      //
      // Find a request descriptor that matches the allocation information.
      //
      req_desc = m_shire_cache_reqq.perform_reqq_alloc(trans_id, bank_id, rtl_info.port, rtl_info.tag_id, reqq_id, rtl_info.second_alloc, rtl_info.paired_id, rtl_info.address, rtl_info.address_space, (rtl_info.port == `EVL_BASE_PORT_NUM_AXI) | m_extract_etlink_from_awuser, ((rtl_info.orig_opcode inside { SC_Reqq_Read, SC_Reqq_ScpRead }) ? 1'b1 : 1'b0), m_verbosity_mod);

      //
      // If no real request descriptor has been allocated, finish.
      //
      if (req_desc == null) begin
         return;
      end

      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("process_reqq_alloc for %0s", req_desc.sprint_obj()))
      
      //
      // Extract specific information from the selected request descriptor and add the ET link
      // command to it.  If the incoming request is from an AXI bus, the ET link command is encoded
      // in the AWUSER bits for writes (for AXI reads, the ET link command is simply a read).
      //
      if (req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
         bus_cmd = req_desc.get_bus_cmd();
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end
      else if (req_desc.have_int_param(pk_etlink_cmd) == 1'b1) begin
         bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
      end
      else begin
         bus_cmd = (req_desc.bus_cmd_is_write() == 1'b1) ? evl_bus_cmd_t'(req_desc.get_bus_req_user()) : evl_bus_cmd_t'(ET_LINK_REQ_Read);
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end
      addr       = req_desc.get_paddr();
      bus_req_id = gen_bus_req_id(bank_id, reqq_id);

      if (use_remote_shire_id == 1'b1 && (remote_shire_id != get_shire_id())) begin
         req_desc.set_int_param(pk_remote_shire, 1);
      end

      // Some quick coverage on remote shire ids:
      if ((use_remote_shire_id == 1'b1) && (remote_shire_id != get_shire_id()) && (req_desc.get_bus_type() != EVL_BUS_TYPE_AXI)) begin
         if (m_shire_cache_scoreboard_cover != null) begin
            m_shire_cache_scoreboard_cover.sample_sc_remote_shire_id(remote_shire_id,m_remote_enable);
         end
      end
      
      //
      // Make sure there is nothing in the allocated request queue.
      //
      if (m_req_queues[bank_id][reqq_id] != null) begin
         evl_req_desc prev_req_desc = m_req_queues[bank_id][reqq_id];

         `dut_error_mdesc(get_abstract_name(), prev_req_desc, $sformatf("bank %0d, reqq_id 0x%x has been reallocated to %0s, but %0s has not been deallocated", bank_id, reqq_id, req_desc.sprint_obj(), prev_req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
      m_req_queues[bank_id][reqq_id] = req_desc;

      //
      // Message send (local)
      //
      if (bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData)) begin
         evl_shire_id_t        shire_id = evl_shire_id_t'(addr >> `EVL_MSG_SHIRE_ID_LSB);
         evl_neighborhood_id_t tmp_dst_port;
         evl_sc_qual_t         qualifier;

         if (shire_id == ~evl_shire_id_t'(0)) begin
            shire_id = get_shire_id();
         end
         tmp_dst_port = evl_neighborhood_id_t'(addr[`SC_MSG_NEIGH_ID_ADDR_RANGE]);
         if (tmp_dst_port >= (`NUM_NEIGH)) begin // only the neighs are legal destinations
            qualifier = `SCQ_DecErr;
         end
         if (m_bypass == 1'b1) begin //this really makes no difference but it's good to have a separate table entry for coverage
            qualifier = qualifier | `SCQ_Bypass;
         end
         // Perform the cache lookup
         m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_MsgSendData, I, req_desc, qualifier, 0, "REQQ stage");
         req_desc.set_root_processed();
         
         // Process the bus action as necessary ... note that ONLY requests local to this shire will
         // get added to the expected-response list
         if ((shire_id == get_shire_id()) && (table_desc.get_bus_cmd() != `EVL_Bus_Idle)) begin
            evl_neighborhood_id_t src_port;
            evl_neighborhood_id_t dst_port;
            int                   num_same_src_dst;
            evl_req_desc          tmp_desc;
            
            dst_port   = evl_neighborhood_id_t'(addr[`SC_MSG_NEIGH_ID_ADDR_RANGE]);
            bus_req_id = addr[`SC_MSG_ID_ADDR_RANGE];

            $cast(tgt_req_desc, m_template_req_desc_et[dst_port].clone());
            tgt_req_desc.set_bus_cmd(evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData));
            tgt_req_desc.set_paddr(addr);
            tgt_req_desc.set_bus_req_size(req_desc.get_bus_req_size());
            tgt_req_desc.set_bus_rsp_dest(addr[`SC_MSG_SOURCE_ADDR_RANGE]);
            tgt_req_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(ET_LINK_RSP_MsgRcvData));
            tgt_req_desc.set_bus_rsp_valid(0);
            tgt_req_desc.set_int_param(pk_upstream_req_port,req_desc.get_int_param(pk_upstream_req_port));
            m_pending_message_queues[dst_port].push(tgt_req_desc);

            //collect some message coverage
            src_port = req_desc.get_int_param(pk_upstream_req_port);
            if (`evl_log_check_verbosity(UVM_HIGH, VMOD_SC_SB) != 0) begin
               m_pending_message_queues[dst_port].print_obj();
            end
            for (int ii = 0; ii < m_pending_message_queues[dst_port].size(); ii++) begin
               tmp_desc = m_pending_message_queues[dst_port].get(ii);
               if (tmp_desc.get_int_param(pk_upstream_req_port) == src_port) begin
                  num_same_src_dst++;
               end
            end
            if (m_shire_cache_scoreboard_cover != null) begin
               m_shire_cache_scoreboard_cover.sample_sc_messages(req_desc.get_bus_req_size,src_port,dst_port,num_same_src_dst);
            end
         end
      end
      else if (m_bypass == 1'b0) begin

         //
         // Remote scratchpad reads and writes get processed here (but not remote scratchpad writearound)
         // (Reqq_Idx is how CP_Inv shows up)
         if ((use_remote_shire_id == 1'b1) && (remote_shire_id != get_shire_id()) && (!(rtl_info.orig_opcode inside {SC_Reqq_WriteAround,SC_Reqq_Idx}))) begin
            evl_sc_qual_t qualifier;
            evl_sc_reqq_opcode_t opcode;
            // Perform the cache lookup
            if (m_remote_enable == 1) begin
               qualifier = `SCQ_Remote;
            end
            else begin
               qualifier  = `SCQ_Remote | `SCQ_DownDis; //
            end
            if (req_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
               qualifier = qualifier | `SCQ_FromMesh;
            end
            case (rtl_info.orig_opcode)
               SC_Reqq_ScpRead: opcode = SC_Scp_Read;
               SC_Reqq_ScpReadCoop: opcode = SC_Scp_Read;
               SC_Reqq_ScpWrite: opcode = SC_Scp_Write;
               SC_Reqq_ScpWritePartial: opcode = SC_Scp_WritePartial;
               SC_Reqq_Atomic: opcode = SC_Scp_Atomic;
               SC_Reqq_CopFlush: opcode = SC_L2_Flush;
               SC_Reqq_CopEvict: opcode = SC_L2_Evict;
               SC_Reqq_CopPrefetch: opcode = SC_L2_Prefetch;
               SC_Reqq_CopScpFill: opcode = SC_Scp_Fill;
               default: opcode = SC_Reqq_AllocErr; //if this shows up it's a testbench error.
            endcase
            if (rtl_info.orig_opcode inside {SC_Reqq_CopEvict}) begin
               if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 2)) begin
                  qualifier = qualifier | `SCQ_L1_L3;
               end
               else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 2)) begin
                  qualifier = qualifier | `SCQ_L2_L3;
               end
               else if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = qualifier | `SCQ_L1_Mem;
               end
               else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = qualifier | `SCQ_L2_Mem;
               end
               else if ((req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = qualifier | `SCQ_L3_Mem;
               end
            end
            
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, opcode, I, req_desc, qualifier, 0, "REQQ stage");
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end

            if (table_desc.get_data_action() == Data_ZeroRsp) begin
               evl_line_data line_data;
               evl_line_data rsp_data;
               line_data = new();
               line_data.set_xword(0,evl_xword_t'(0));
               rsp_data = copy_appropriate_amount_of_rsp_data(line_data,req_desc.get_bus_req_size(),req_desc.get_paddr());
               req_desc.set_rsp_data(rsp_data);
               req_desc.set_root_processed();
            end

            // Set observed so that the response will be checked whenever it shows up
            req_desc.set_observed();
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end

            // If this is a ReadCoop, check to see if we need to respond to more than one neigh.
            if ((evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) == RspWData) && (bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_ReadCoop))) begin
               `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("process_reqq_alloc calling predict_read_coop_rsp"))
               predict_read_coop_rsp(req_desc);
            end
         end
         
         //
         // Scratchpad fill
         //
         else if (bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_ScpFill)) begin 
            // Perform the cache lookup
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_Scp_Fill, I, req_desc, `SCQ_None, 0, "REQQ stage");
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               evl_line_data req_data = req_desc.get_req_data();
               
               addr = evl_paddr_t'(req_data.get_dword(0)) & const_line_paddr_mask;
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_ScpFill);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end

         //
         // Flush with start level L3 and destination level Mem
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Flush)) && (req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
            evl_sc_qual_t qualifier;
            if (m_remote_l3_enable == 0) begin
               qualifier = `SCQ_L3_Mem | `SCQ_DownDis;
            end
            else begin
               qualifier = `SCQ_L3_Mem;
            end
            // Perform the cache lookup
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Flush, I, req_desc, qualifier, 0, "REQQ stage");
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Flush);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end
         //
         // Remote Scratchpad Evicts
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Evict)) && (use_remote_shire_id == 1'b1) && (remote_shire_id != get_shire_id())) begin //clc: pretty sure this code is redundant and this case is covered above, but too scared to comment it out right now. 
            evl_sc_qual_t qualifier;
            qualifier = `SCQ_Remote;
            if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 2)) begin
               qualifier = qualifier | `SCQ_L1_L3;
            end
            else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 2)) begin
               qualifier = qualifier | `SCQ_L2_L3;
            end
            else if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 3)) begin
               qualifier = qualifier | `SCQ_L1_Mem;
            end
            else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 3)) begin
               qualifier = qualifier | `SCQ_L2_Mem;
            end
            else if ((req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
               qualifier = qualifier | `SCQ_L3_Mem;
            end
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Evict, I, req_desc, qualifier, 0, "REQQ stage");
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
            
         end
         //
         // Evict with start level L3 and destination level Mem
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Evict)) && (req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
           // if ((use_remote_shire_id == 1'b1) && (remote_shire_id != get_shire_id())) begin // illegal remote scp evict
           //    m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Evict, I, req_desc, (`SCQ_Remote|`SCQ_L3_Mem), 0, "REQQ stage");
           // end
           // else begin
               evl_sc_qual_t qualifier;
               if (m_remote_l3_enable == 0) begin
                  qualifier = `SCQ_L3_Mem | `SCQ_DownDis;
               end
               else begin
                  qualifier = `SCQ_L3_Mem;
               end
               // Perform the cache lookup
               m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Evict, I, req_desc, qualifier, 0, "REQQ stage");
           // end
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Evict);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end
         
         //
         // Prefetch with start level L3 or Mem
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Prefetch)) && (req_desc.get_bus_cop_dest_level() inside { 2, 3 })) begin
            evl_sc_qual_t qualifier;
            // Perform the cache lookup
            if (req_desc.get_bus_cop_dest_level() == 2) begin
               if (m_remote_l3_enable == 0) begin
                  qualifier = `SCQ_L3 | `SCQ_DownDis;
               end
               else begin
                  qualifier = `SCQ_L3;
               end
               m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Prefetch, I, req_desc, qualifier, 0, "REQQ stage");
            end
            else begin
               m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Prefetch, I, req_desc, `SCQ_Mem, 0, "REQQ stage");
            end
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Prefetch);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end // if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Prefetch)) && (req_desc.get_bus_cop_dest_level() inside { 2, 3 }
         //
         // local scratchpad writearound
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_WriteAround)) && (m_sb_type == int'(SCP_SCOREBOARD)) && (req_desc.get_int_param(pk_remote_shire) != 1)) begin
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_WriteAround, I, req_desc, `SCQ_None, 0, "REQQ stage");
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end
         //
         // WriteAround when coalescing buffer is disabled
         //
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_WriteAround)) && (m_cbuf_enable[bank_id] == 0)) begin
           
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_WriteAround, I, req_desc, `SCQ_None, 0, "REQQ stage");
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Write);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
         end
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Write)) && (rtl_info.orig_opcode inside {SC_Reqq_WriteForward,SC_Reqq_WriteForwardPartial}) && (req_desc.get_bus_subopcode_dest() == dest_l3)) begin
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Write, I, req_desc, `SCQ_L3, 0, "REQQ stage");
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Write);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
         end
         else if ((bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_Read)) && (rtl_info.orig_opcode == SC_Reqq_ReadForward) && (req_desc.get_bus_subopcode_dest() == dest_l3)) begin
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L2_Read, I, req_desc, `SCQ_L3, 0, "REQQ stage");
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Read);
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end
         end
         // Illegal (maybe) requests to local scratchpad
         else if ((bus_cmd inside {evl_bus_cmd_t'(ET_LINK_REQ_Evict), evl_bus_cmd_t'(ET_LINK_REQ_Flush), evl_bus_cmd_t'(ET_LINK_REQ_Lock), 
                                   evl_bus_cmd_t'(ET_LINK_REQ_Unlock), evl_bus_cmd_t'(ET_LINK_REQ_Prefetch), evl_bus_cmd_t'(ET_LINK_REQ_Atomic)}) && (m_sb_type == int'(SCP_SCOREBOARD)) && (req_desc.get_int_param(pk_remote_shire) != 1)) begin
            evl_sc_reqq_opcode_t pipe_opcode;
            evl_sc_qual_t qualifier;
            case (bus_cmd)
               evl_bus_cmd_t'(ET_LINK_REQ_Evict): pipe_opcode = SC_L2_Evict;
               evl_bus_cmd_t'(ET_LINK_REQ_Flush): pipe_opcode = SC_L2_Flush;
               evl_bus_cmd_t'(ET_LINK_REQ_Lock): pipe_opcode = SC_L2_Lock;
               evl_bus_cmd_t'(ET_LINK_REQ_Unlock): pipe_opcode = SC_L2_Unlock;
               evl_bus_cmd_t'(ET_LINK_REQ_Prefetch): pipe_opcode = SC_L2_Prefetch;
               evl_bus_cmd_t'(ET_LINK_REQ_Atomic): pipe_opcode = SC_L2_Atomic;
            endcase // case (bus_cmd)
            qualifier = fix_qualifier_at_reqq_alloc(qualifier,req_desc);
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, pipe_opcode, I, req_desc, qualifier, 0, "REQQ stage");
            if (table_desc.get_data_action() == Data_ZeroRsp) begin
               evl_line_data line_data;
               evl_line_data rsp_data;
               line_data = new();
               line_data.set_xword(0,evl_xword_t'(0));
               rsp_data = copy_appropriate_amount_of_rsp_data(line_data,req_desc.get_bus_req_size(),req_desc.get_paddr());
               req_desc.set_rsp_data(rsp_data);
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end // if ((bus_cmd inside {evl_bus_cmd_t'(ET_LINK_REQ_Evict), evl_bus_cmd_t'(ET_LINK_REQ_Flush), evl_bus_cmd_t'(ET_LINK_REQ_Lock),...
         // The RspAck gets sent on AXI right away so we need to predict that now.
         else if ((bus_cmd inside {evl_bus_cmd_t'(ET_LINK_REQ_Atomic)}) && (m_sb_type == int'(L3_SCOREBOARD))) begin
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_L3_Atomic, I, req_desc, `SCQ_None, 0, "REQQ stage");
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
         end // if ((bus_cmd inside {evl_bus_cmd_t'(ET_LINK_REQ_Atomic)}) && (m_sb_type == int'(L3_SCOREBOARD)))

         

      end // if (m_bypass == 1'b0)
      else begin // m_bypass == 1'b1
         if (!(rtl_info.orig_opcode inside {SC_Reqq_Idx,SC_Reqq_Sync})) begin //Index cachops aren't affected by Bypass therefore don't need to be processed here.
            evl_sc_qual_t qualifier;
            if (bus_cmd inside { ET_LINK_REQ_Evict, ET_LINK_REQ_Flush }) begin
               if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 2)) begin
                  qualifier = `SCQ_L1_L3;
               end
               else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 2)) begin
                  qualifier = `SCQ_L2_L3;
               end
               else if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = `SCQ_L1_Mem;
               end
               else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = `SCQ_L2_Mem;
               end
               else if ((req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
                  qualifier = `SCQ_L3_Mem;
               end
               if ((use_remote_shire_id == 1'b1) && (remote_shire_id != get_shire_id())) begin
                  qualifier = qualifier | `SCQ_Remote;
               end
            end // if (opcode inside { SC_L2_Evict, SC_L2_Flush })
            else if (bus_cmd inside { ET_LINK_REQ_Prefetch }) begin
               if (req_desc.get_bus_cop_dest_level() == 1) begin
                  qualifier = `SCQ_L2;
               end
               else if (req_desc.get_bus_cop_dest_level() == 2) begin
                  qualifier = `SCQ_L3;
               end
               else begin
                  qualifier = `SCQ_Mem;
               end
            end // if (bus_cmd inside { ET_LINK_REQ_Prefetch })
          
            qualifier |= `SCQ_Bypass;
            
            qualifier = fix_qualifier_at_reqq_alloc(qualifier,req_desc);
          
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_REQQ_REQ, bus_cmd, SC_Reqq_Initial, I, req_desc, qualifier, 0, "REQQ stage");
            
            // Process the bus action as necessary
            if (table_desc.get_bus_cmd() != `EVL_Bus_Idle) begin
               $cast(tgt_req_desc, m_template_req_desc_axi.clone());
               if (bus_cmd inside { ET_LINK_REQ_WriteAround, ET_LINK_REQ_EvictToMem, ET_LINK_REQ_FlushToMem}) begin
                  aw_user = evl_bus_req_user_t'(ET_LINK_REQ_Write);
               end
               else begin
                  aw_user = evl_bus_req_user_t'(bus_cmd);
               end
               tgt_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               addr = addr & const_line_paddr_mask;
               req_desc.set_root_processed();
            end // if (table_desc.get_bus_cmd() != `EVL_Bus_Idle)
            if (table_desc.get_data_action() == Data_ZeroRsp) begin
               evl_line_data line_data;
               evl_line_data rsp_data;
               line_data = new();
               line_data.set_xword(0,evl_xword_t'(0));
               rsp_data = copy_appropriate_amount_of_rsp_data(line_data,req_desc.get_bus_req_size(),req_desc.get_paddr());
               req_desc.set_rsp_data(rsp_data);
               req_desc.set_root_processed();
            end
            if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
               if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
                  req_desc.set_abstract_rsp_type(RspErr);
               end
               else begin
                  req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               end
               req_desc.set_bus_rsp_valid(0);
               req_desc.set_root_processed();
            end
            //end
         end
      end

      //
      // If a new requests descriptor has been created, fill it out.
      //
      if (tgt_req_desc != null) begin
         evl_data_action_t data_action = table_desc.get_data_action();
         evl_cache_st_t    tag_action  = table_desc.get_tag_action();

         tgt_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
         tgt_req_desc.set_paddr(addr);
         tgt_req_desc.set_bus_bank(bank_id);
         tgt_req_desc.set_bus_req_id(bus_req_id);
         tgt_req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
         tgt_req_desc.set_int_param(pk_reqq_id, reqq_id);

         //
         // Process data action as appropriate.
         //
         if (data_action inside { Data_FwdWr }) begin
            tgt_req_desc.set_req_data(req_desc.get_req_data());
         end
         else if (data_action inside { Data_ForwardMsg }) begin
            tgt_req_desc.set_rsp_data(req_desc.get_req_data());
         end
         else if (data_action inside { Data_ReqWrNull }) begin
            tgt_req_desc.invalidate_req_data();
         end

         //
         // Process tag action as appropriate.
         //
         if (table_desc.get_tag_action() inside { Stale }) begin
            tgt_req_desc.set_ignore_st_on_rsp();
         end
         else if (!(tag_action inside { St_Ignore })) begin
            if (line_desc.get_cache_st() != tag_action) begin
               `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("REQQ model (bank %0d): modifying cache state for 0x%x from %p to %p", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), tag_action))
            end
            line_desc.set_cache_st(tag_action);
         end

         //
         // Process the miscellaneous action as appropriate.
         //
         if (evl_error_code'(table_desc.get_err_code()) inside { EC_DeallocReqq }) begin
            m_req_queues[bank_id][reqq_id] = null;
         end

         //
         // Add this descriptor as a child of the original request descriptor.
         //
         void'(req_desc.add_child_desc(tgt_req_desc));

         //
         // If the target descriptor is headed to AXI, add it to the mesh queue.  In addition, set
         // the AWUSER bits appropriately.
         //
         if (tgt_req_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
            if (tgt_req_desc.bus_cmd_is_write() == 1'b1) begin
               tgt_req_desc.set_bus_req_user(aw_user);
            end
            else begin
               tgt_req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
            end

            final_axi_req_fixup(tgt_req_desc,evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
            m_shire_cache_axi_meshq.add_pending_axi_req_desc(tgt_req_desc, m_sb_type);
            
            // Also add to our own queue to keep track of who is waiting for a response
            enqueue_pending_downstream_rsp(tgt_req_desc);
         end
      end
   endfunction : process_reqq_alloc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dequeue_reqq_entry
   //
   function void dequeue_reqq_entry(input evl_sc_bank_id_t bank_id, input evl_sc_reqq_id_t reqq_id);
      m_req_queues[bank_id][reqq_id] = null;
   endfunction : dequeue_reqq_entry


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rsp_reqq_id
   //
   function void process_rsp_reqq_id(input evl_sc_bank_id_t bank_id, input evl_sc_reqq_id_t reqq_id, input int cycle);
      evl_req_desc req_desc;

      req_desc = m_shire_cache_reqq.get_reqq_entry(bank_id, reqq_id);
      if (req_desc != null) begin
         m_rsp_queues[bank_id].push(req_desc);
         //req_desc.set_rsp_cycle(cycle);
      end
   endfunction : process_rsp_reqq_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rsp_dstq_id
   //
   function void process_rsp_dstq_id(input evl_sc_bank_id_t bank_id, input evl_neighborhood_id_t neigh_id, input int cycle);
      evl_req_desc req_desc;

      req_desc = m_rsp_queues[bank_id].pop();
      if (req_desc != null) begin
         req_desc.set_rsp_cycle(cycle);
      end
   endfunction : process_rsp_dstq_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_reqq_dealloc
   //
   function void process_reqq_dealloc(input evl_trans_id_t trid, input evl_sc_bank_id_t bank_id, input evl_sc_reqq_alloc_t rtl_info);
      evl_sc_reqq_id_t reqq_id = 0;

      while (rtl_info.reqq_nhot != 0) begin
         if (rtl_info.reqq_nhot[0:0] == 1'b1) begin
            m_shire_cache_reqq.dequeue_reqq_entry(bank_id, reqq_id);
            dequeue_reqq_entry(bank_id, reqq_id);
         end
         reqq_id++;
         rtl_info.reqq_nhot >>= 1;
      end
   endfunction : process_reqq_dealloc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rbuf_request
   //
   function void process_rbuf_request(input evl_trans_id_t trid, input evl_sc_bank_id_t bank_id, input evl_sc_pipe_rbuf_req_t rtl_info);
      evl_sc_reqq_id_t     reqq_id;
      evl_bus_cmd_t        bus_cmd;
      evl_sc_reqq_opcode_t opcode;    // opcode of the request
      evl_paddr_t          address;   // address of the request
      evl_req_desc         req_desc;
      evl_table_desc       table_desc;
      evl_line_desc        line_desc;
      evl_sc_qual_t        qualifier;
      qualifier = `SCQ_None;
      reqq_id  = rtl_info.reqq_id;
      opcode   = (rtl_info.scp == 1'b1) ? SC_Scp_Read : SC_L2_Read;
      address  = rtl_info.address;
      req_desc = m_req_queues[bank_id][reqq_id];
      if (req_desc == null) begin
         $cast(req_desc, m_template_req_desc_et[15].clone());
         req_desc.set_bus_bank(bank_id);
         req_desc.set_int_param(pk_reqq_id, ~evl_sc_reqq_id_t'(0));
         req_desc.set_paddr(address);
         `dut_error(get_abstract_name(), $sformatf("bank %0d could not find a request (%p 0x%x, TrID 0x%x) ... creating one from scratch for (%0s)", bank_id, opcode, address, trid, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
      else begin
         //Make sure the address matches
         if (req_desc.get_paddr() != address) begin
            evl_paddr_t req_desc_addr;
            evl_paddr_t modified_addr;
            req_desc_addr = req_desc.get_paddr();
            // If this is a remote SCP access and the shire id is 0xff, that really means we're accessing this shire.
            // The RTL has changed the shire id bits of the address in that case, so a mismatch on those bits is expected.
            if ((rtl_info.scp == 1'b1) && (int'(address[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) &&
                (req_desc_addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE] == ~(`SC_SCP_SHIRE_ID_SIZE'(1'b0)))) begin
               modified_addr = req_desc.get_paddr();
               modified_addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE] = `SC_SCP_SHIRE_ID_SIZE'(get_shire_id());
               if (address != modified_addr) begin
                  `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_rbuf_request(): address mismatch for this scp access with 0xff shire id ... expected 0x%x, but received 0x%x (original req_desc paddr = 0x%x)", modified_addr, address, req_desc.get_paddr()))
               end
            end
            else begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_rbuf_request got this request for bank_id %x reqq_id %x but the rtl says the address is %x while the req_desc addr is %x",bank_id,reqq_id,address,req_desc.get_paddr()))
            end
         end
      end
      `evl_log_mdesc_pk(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("RBUF Rd Req (bank %0d): scp=%0b, rbuf_id:0x%0x (%0s)", bank_id, rtl_info.scp, rtl_info.rbuf_id, req_desc.sprint_obj()))

      if ((m_sb_type == L2_SCOREBOARD) && (m_l2_rbuf_enable[bank_id] == 0)) begin
         `dut_error(get_abstract_name(), $sformatf("(bank %0d): Did not expect to see a RBUF hit in L2 Scoreboard when l2_rbuf_enable = 0",bank_id))
      end
      if ((m_sb_type == SCP_SCOREBOARD) && (m_scp_rbuf_enable[bank_id] == 0)) begin
         `dut_error(get_abstract_name(), $sformatf("(bank %0d): Did not expect to see a RBUF hit in SCP Scoreboard when scp_rbuf_enable = 0",bank_id))
      end
      
      //
      // Do some checking here (like size, etc.)
      //

      if (req_desc.have_int_param(pk_etlink_cmd) == 1'b1) begin
         bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
      end
      else if (req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
         bus_cmd = req_desc.get_bus_cmd();
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end
      else begin
         bus_cmd = (req_desc.bus_cmd_is_write() == 1'b1) ? evl_bus_cmd_t'(req_desc.get_bus_req_user()) : evl_bus_cmd_t'(ET_LINK_REQ_Read);
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end

      if (req_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
         qualifier  = qualifier | `SCQ_FromMesh;
      end

      m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_RBUF_REQ, bus_cmd, opcode, E, req_desc, qualifier, 0, "RBuf stage");
      if (m_perform_qword_en_check == 1'b1) begin
         perform_qword_en_check(line_desc);
      end
      // Always set observed so that the response will be checked whenever it shows up
      req_desc.set_observed();
      req_desc.set_root_processed();
      if (table_desc.get_bus_rsp() != evl_bus_rsp_type_t'(RspNull)) begin
         req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
         if (table_desc.get_data_action() == Data_MemRd) begin
            evl_line_data rsp_data;
            rsp_data = copy_appropriate_amount_of_rsp_data(line_desc.get_line_data(),req_desc.get_bus_req_size(),req_desc.get_paddr());
            req_desc.set_rsp_data(rsp_data);
         end
         req_desc.set_bus_rsp_valid(0);
      end
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_SB) != 0) begin
         line_desc.print_obj(get_abstract_name());
         if (table_desc.get_bus_rsp() != evl_bus_rsp_type_t'(RspNull)) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("predicted response: %0s", req_desc.sprint_rsp()))
         end
      end
      // If this is a ReadCoop, check to see if we need to respond to more than one neigh.
      if ((evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) == RspWData) && (bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_ReadCoop))) begin
         `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("process_rbuf_request: calling predict_read_coop_rsp"))
         predict_read_coop_rsp(req_desc);
         
      end
      
      
   endfunction : process_rbuf_request

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_reqq_alloc
   //
   virtual function evl_sc_qual_t fix_qualifier_at_reqq_alloc(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      return qualifier_in;
   endfunction : fix_qualifier_at_reqq_alloc
   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tc_req
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tc_req(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      return qualifier_in;
   endfunction : fix_qualifier_at_tc_req

   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_qualifier_at_tag_rsp
   //
   virtual function evl_sc_qual_t fix_qualifier_at_tag_rsp(input evl_sc_qual_t qualifier_in, input evl_req_desc req_desc);
      return qualifier_in;
   endfunction : fix_qualifier_at_tag_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_error_code
   //
   virtual function void process_error_code(input evl_req_desc req_desc, input evl_error_code error_code_in);
      return;
   endfunction : process_error_code

   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: convert_index_cacheop_address
   //
   virtual function evl_paddr_t convert_index_cacheop_address(input evl_paddr_t cacheop_address);
      return cacheop_address;
   endfunction : convert_index_cacheop_address

   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_cacheop_way
   //
   virtual function evl_way_t get_index_cacheop_way(input evl_paddr_t cacheop_address);
      evl_paddr_t addr;
      evl_way_t way_out;
      addr = cacheop_address >> `EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE;
      way_out = evl_way_t'(addr[`SC_WAY_ID_SIZE-1:0]);
      return way_out;
   endfunction : get_index_cacheop_way
   

   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tc_request
   //
   function void process_tc_request(input evl_trans_id_t trans_id, input evl_sc_bank_id_t bank_id, input int cycle_count, input evl_sc_reqq_pipe_req_t rtl_info);
      evl_sc_reqq_id_t        reqq_id;
      evl_bus_cmd_t           bus_cmd;
      evl_etlink_req_opcode_t etlink_cmd;
      evl_sc_reqq_opcode_t    opcode;    // opcode of the request
      evl_paddr_t             address;   // address of the request
      bit                     zero_data; // indicates that all data being written is zero
      evl_line_qword_en_t     qwen;      // qword enable
      int                     sc_size;   // size of the transaction (in bytes)
      evl_req_desc            req_desc;
      evl_paddr_t             pipe_set;
      int                     pipe_sub_bank;
      evl_sc_qual_t           qualifier;
      evl_cache_st_t          obs_st;
      evl_way_t               obs_way;
      evl_table_desc          table_desc;
      evl_line_desc           line_desc;
      evl_line_desc           prev_line_desc;
      evl_req_desc            victim_req_desc;
      evl_line_data           atomic_response;   // response of the atomic operation, needs to be here for axi_req to use
      evl_line_data           saved_victim_req_data; //used in some victim cases

      reqq_id         = rtl_info.reqq_id;
      opcode          = rtl_info.opcode;
      address         = rtl_info.address;
      zero_data       = rtl_info.zero_data;
      qwen            = rtl_info.qwen;
      sc_size         = rtl_info.sc_size;
      qualifier       = `SCQ_None;
      prev_line_desc  = null;
      victim_req_desc = null;
      if (opcode inside { SC_Scp_Fill }) begin
         int limit = m_pending_fills[bank_id].size();

         for (int ii = 0; ii < limit; ii++) begin
            if (m_pending_fills[bank_id][ii].get_int_param(pk_reqq_id) == reqq_id) begin
               req_desc = m_pending_fills[bank_id][ii];
               m_pending_fills[bank_id].delete(ii);
               break;
            end
         end
      end
      else if (opcode inside { SC_L2_Fill, SC_L3_Fill }) begin
         int limit = m_pending_fills[bank_id].size();

         for (int ii = 0; ii < limit; ii++) begin
            if ( (m_pending_fills[bank_id][ii].line_addr_match(address) == 1'b1) &&
                 ( (m_pending_fills[bank_id][ii].get_int_param(pk_reqq_id) == int'(reqq_id)) ||
                   (m_pending_fills[bank_id][ii].get_int_param(pk_paired_reqq_id, -1) == int'(reqq_id)) ) ) begin
               req_desc = m_pending_fills[bank_id][ii];
               m_pending_fills[bank_id].delete(ii);
               break;
            end
         end
      end
      else if (opcode inside { SC_L2_Evict }) begin
         int limit = m_pending_cbuf_evicts[bank_id].size();

         for (int ii = 0; ii < limit; ii++) begin
            if ( (m_pending_cbuf_evicts[bank_id][ii].line_addr_match(address) == 1'b1) &&
                 ( (m_pending_cbuf_evicts[bank_id][ii].get_int_param(pk_reqq_id) == int'(reqq_id)) ||
                   (m_pending_cbuf_evicts[bank_id][ii].get_int_param(pk_paired_reqq_id, -1) == int'(reqq_id)) ) ) begin
               req_desc = m_pending_cbuf_evicts[bank_id][ii];
               m_pending_cbuf_evicts[bank_id].delete(ii);
               break;
            end
         end
      end
      else if (opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic }) begin
         int limit = m_pending_atomics[bank_id].size();

         for (int ii = 0; ii < limit; ii++) begin
            if ( (m_pending_atomics[bank_id][ii].get_paddr() == address) &&
                 ( (m_pending_atomics[bank_id][ii].get_int_param(pk_reqq_id) == int'(reqq_id)) ||
                   (m_pending_atomics[bank_id][ii].get_int_param(pk_paired_reqq_id, -1) == int'(reqq_id)) ) ) begin
               req_desc = m_pending_atomics[bank_id][ii];
               m_pending_atomics[bank_id].delete(ii);
               break;
            end
         end
      end
      if (req_desc == null) begin
         req_desc = m_req_queues[bank_id][reqq_id];
      end

      //
      // If after checking all of the possible sources, look in the request queue itself ... either
      // grab an existing entry there OR create one from scratch.
      //
      if (req_desc != null) begin
      end
      else begin
         req_desc = m_shire_cache_reqq.get_reqq_entry(bank_id, reqq_id);
         if (req_desc != null) begin
            `dut_error(get_abstract_name(), $sformatf("could not find a request for bank %0d, reqq ID 0x%0x (%p 0x%x, TrID 0x%x) ... using current entry (%0s)", bank_id, reqq_id, opcode, address, trans_id, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
         end
         else begin
            $cast(req_desc, m_template_req_desc_et[15].clone());
            req_desc.set_bus_bank(bank_id);
            req_desc.set_int_param(pk_reqq_id, reqq_id);
            req_desc.set_paddr(address);
            if (!(trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
               req_desc.set_trans_id(trans_id);
            end
            `dut_error(get_abstract_name(), $sformatf("could not find a request for bank %0d, reqq ID 0x%0x (%p 0x%x, TrID 0x%x) ... creating one from scratch (%0s)", bank_id, reqq_id, opcode, address, trans_id, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
         end
      end // else: !if(req_desc != null)

      // this int param helps the cb inv checks
      req_desc.set_int_param(pk_after_tc_req, 1);
      req_desc.set_root_processed();

      // Determine if this is an Atomic second pass
      if (opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic }) begin
         req_desc.set_int_param(pk_pass_count, req_desc.get_int_param(pk_pass_count, 0) + 1);
      end


      //
      // Check the qwen that the RTL gave us.
      //
      if (req_desc.bus_cmd_is_write() && (req_desc.get_int_param(pk_idx_cache_op) == 0)) begin
         if (opcode inside {SC_L2_Prefetch,SC_L3_Prefetch,SC_L2_Flush,SC_L3_Flush,SC_L2_Evict,SC_L3_Evict,SC_L2_Lock,SC_L2_Unlock,SC_L2_UnlockInv}) begin
            //expect qwen to be 1's
            if (qwen != 4'hf) begin
               `dut_error(get_abstract_name(), $sformatf("expected qwen from rtl to be 0xf for opcode %p but it is 0x%x (%s)", opcode, qwen, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
         else if (opcode inside {SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic}) begin
            if (req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
            // Request is from Neigh (4'b0001 or 4'b0011) and when it goes down the pipe it is 4'b0101 or 4'b0111.  
              if ((qwen != 4'h7) & (qwen != 4'h5)) begin
                 `dut_error(get_abstract_name(), $sformatf("expected qwen from Neigh to be 0x7 or 0x5 for opcode %p but it is 0x%x (%s)", opcode, qwen, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
              end
            end
            else begin
              // If request is from L3slave, it is always 4'b0111.
              if (qwen != 4'h7) begin
               `dut_error(get_abstract_name(), $sformatf("expected qwen from l3slave to be 0x7 for opcode %p but it is 0x%x (%s)", opcode, qwen, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
               end
            end
         end
         else begin
            evl_line_data req_data;
            req_data = req_desc.get_req_data();
            if (req_data == null) begin
               `dut_error(get_abstract_name(), $sformatf("didn't expect req_data to be null %s", req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
            else if (qwen != req_data.get_qword_en()) begin
               `dut_error(get_abstract_name(), $sformatf("qwen (0x%x) from the RTL does not match req_desc.get_qword_en() (0x%x) %s", qwen, req_data.get_qword_en(), req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
      end // if (req_desc.bus_cmd_is_write())

      //
      // Check zero_data to see if it matches up with what we expect
      //
      if ((opcode inside {SC_L2_Fill,SC_L3_Fill,SC_Scp_Fill}) && (req_desc.get_rsp_xword(address >> `EVL_XWORD_ADDR_LSB) == 0)) begin
         if (zero_data != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("expecting zero_data to be set for this %p (%s)", opcode, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
         end
      end
      else if ((opcode inside {SC_L2_Write,SC_L3_Write,SC_Scp_Write,SC_L2_WriteAround}) && (req_desc.get_req_xword(address >> `EVL_XWORD_ADDR_LSB) == 0)) begin
         if (zero_data != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("expecting zero_data to be set for this %p (%s)", opcode, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
         end
      end
      else if (zero_data != 0) begin
         `dut_error(get_abstract_name(), $sformatf("not expecting zero_data to be set for this %p (%s)", opcode, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end

      //
      // Pipeline requests are processed in order ... queue up a request to be processed in the tag
      // response stage.
      //
      m_tag_rsp_queue.push_back(req_desc);
      if (m_pipe_info_tc.exists(bank_id)) begin
         obs_st        = (m_pipe_info_tc[bank_id].tag_hit == 1'b1) ? E : I;
         obs_way       = evl_way_t'(m_pipe_info_tc[bank_id].tag_ram_hit_way);
         pipe_set      = evl_paddr_t'(m_pipe_info_tc[bank_id].pipe_set);
         pipe_sub_bank = int'(m_pipe_info_tc[bank_id].pipe_sub_bank);
      end
      else begin
         obs_st        = I;
         obs_way       = EVL_WAY_NONE;
         pipe_set      = evl_paddr_t'(address);
         pipe_sub_bank = int'(0);
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_pipe_info_tc[%0d] does not exist (file %0s, line %0d)", bank_id, `__FILE__, `__LINE__))
      end

      if (!req_desc.have_int_param(pk_l2_opcode)) begin
         req_desc.set_int_param(pk_l2_opcode, int'(opcode));
      end

      req_desc.set_bus_bank(bank_id);
      if (req_desc.have_int_param(pk_etlink_cmd) == 1'b1) begin
         bus_cmd = evl_bus_cmd_t'(req_desc.get_int_param(pk_etlink_cmd));
      end
      else if (req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
         bus_cmd = req_desc.get_bus_cmd();
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end
      else begin
         bus_cmd = (req_desc.bus_cmd_is_write() == 1'b1) ? evl_bus_cmd_t'(req_desc.get_bus_req_user()) : evl_bus_cmd_t'(ET_LINK_REQ_Read);
         req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
      end
      etlink_cmd = evl_etlink_req_opcode_t'(bus_cmd);


      //
      // For SC_L2_WriteAround commands, we need to distinguish between partial and full.  Because of
      // this, for L2_Read commands, the cache lookup will need to determine this.
      //
      if (opcode inside { m_merge_partial_opcodes }) begin
         qualifier = `SCQ_MergePartial;
      end
      else if ((opcode inside { m_check_partial_opcodes }) && (obs_st != I)) begin
         qualifier = `SCQ_CheckPartial;
      end
      else if (opcode inside { SC_L2_Evict, SC_L2_Flush }) begin
         if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 2)) begin
            qualifier = `SCQ_L1_L3;
         end
         else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 2)) begin
            qualifier = `SCQ_L2_L3;
         end
         else if ((req_desc.get_bus_cop_start_level() == 0) && (req_desc.get_bus_cop_dest_level() == 3)) begin
            qualifier = `SCQ_L1_Mem;
         end
         else if ((req_desc.get_bus_cop_start_level() == 1) && (req_desc.get_bus_cop_dest_level() == 3)) begin
            qualifier = `SCQ_L2_Mem;
         end
         else if ((req_desc.get_bus_cop_start_level() == 2) && (req_desc.get_bus_cop_dest_level() == 3)) begin
            qualifier = `SCQ_L3_Mem;
         end
      end // if (opcode inside { SC_L2_Evict, SC_L2_Flush })
      else if (opcode inside { SC_Scp_Fill }) begin
         // For a scratchpad fill, tell the cache model that the fill goes to the address of the original
         // request.
         qualifier = `SCQ_UseSrcAddr;
      end
      else if (opcode inside { SC_L2_Lock }) begin
         qualifier = `SCQ_CheckLock;
      end

      if ((opcode inside { m_index_cacheop_opcodes })) begin
         qualifier |= `SCQ_IdxAddr;
      end

      if (opcode inside { SC_L3_Atomic, SC_Scp_Atomic }) begin
         if (req_desc.get_int_param(pk_pass_count) > 1) begin
            qualifier = `SCQ_SecondPass;
         end
      end

      if (opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic }) begin
         if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), address, req_desc.get_trans_id(), reqq_id, $sformatf("TC req (bank %0d): %p, %p, zero=%b, qwen=%b, size=%0d, set=0x%04x, subbank=%0d, %0s, way %0s (%0s, pass %0d)", bank_id, etlink_cmd, opcode, zero_data, qwen, sc_size, pipe_set, pipe_sub_bank, ((obs_st == E) ? "hit" : "miss"), get_way_name(obs_way), req_desc.sprint_obj(), req_desc.get_int_param(pk_pass_count)))
         end
         else begin
            `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), address, trans_id, reqq_id, $sformatf("TC req (bank %0d): %p, %p, zero=%b, qwen=%b, size=%0d, set=0x%04x, subbank=%0d, %0s, way %0s (%0s, pass %0d)", bank_id, etlink_cmd, opcode, zero_data, qwen, sc_size, pipe_set, pipe_sub_bank, ((obs_st == E) ? "hit" : "miss"), get_way_name(obs_way), req_desc.sprint_obj(), req_desc.get_int_param(pk_pass_count)))
         end
      end
      else begin
         if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), address, req_desc.get_trans_id(), reqq_id, $sformatf("TC req (bank %0d): %p, %p, zero=%b, qwen=%b, size=%0d, set=0x%04x, subbank=%0d, %0s, way %0s (%0s)", bank_id, etlink_cmd, opcode, zero_data, qwen, sc_size, pipe_set, pipe_sub_bank, ((obs_st == E) ? "hit" : "miss"), get_way_name(obs_way), req_desc.sprint_obj()))
         end
         else begin
            `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), address, trans_id, reqq_id, $sformatf("TC req (bank %0d): %p, %p, zero=%b, qwen=%b, size=%0d, set=0x%04x, subbank=%0d, %0s, way %0s (%0s)", bank_id, etlink_cmd, opcode, zero_data, qwen, sc_size, pipe_set, pipe_sub_bank, ((obs_st == E) ? "hit" : "miss"), get_way_name(obs_way), req_desc.sprint_obj()))
         end
      end // else: !if(opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic })

      // If the set size is 0, then there is no cache space allocated for this L2/L3/Scp
      if (m_bypass == 1) begin
         qualifier = qualifier | `SCQ_Bypass;
      end
      else if (m_set_size == 0) begin
         qualifier = qualifier | `SCQ_NoRegion;
      end
      

      qualifier = fix_qualifier_at_tc_req(qualifier, req_desc);

      //
      // Do some checking here (like size, etc.)
      //

      //
      // Perform the cache lookup.
      //
      m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_TC_REQ, bus_cmd, opcode, obs_st, req_desc, qualifier, qwen, "TC req stage");

      if ((obs_st inside { S, E, M }) && (m_num_ways > 0) && (obs_way != line_desc.get_cache_way())) begin
         if (obs_way == EVL_WAY_NONE) begin
            `evl_log_mdesc_pk(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("expected 0x%x to hit in way %0s", address, get_way_name(line_desc.get_cache_way())))
         end
         else if (line_desc.get_cache_way() == EVL_WAY_NONE) begin
            `evl_log_mdesc_pk(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("did not expect 0x%x to hit in way %0s", address, get_way_name(obs_way)))
         end
         else begin
            `evl_log_mdesc_pk(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("expected 0x%x to hit in way %0s, not way %0d", address, get_way_name(line_desc.get_cache_way()), get_way_name(obs_way)))
         end
      end

      if (m_perform_qword_en_check == 1'b1) begin
         perform_qword_en_check(line_desc);
      end

      if (table_desc.get_err_code() != EC_None) begin
         process_error_code(req_desc, table_desc.get_err_code());
      end

      if (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_SB) != 0) begin
         line_desc.print_obj(get_abstract_name());
         if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("predicted response: %0s", req_desc.sprint_rsp()))
         end
      end

      //
      // Process the tag action as necessary.
      //
      if (table_desc.get_tag_action() != St_Ignore) begin
         if (table_desc.get_tag_action() inside { Stale_M }) begin
            if (!(line_desc.get_cache_st() inside { M })) begin
               evl_cache_st_t new_state = M;

               `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to %p", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), new_state))
               line_desc.set_cache_st(new_state);
            end
         end
         else if (table_desc.get_tag_action() inside { A1_Stale_M_Bsy }) begin
            if (!(line_desc.get_cache_st() inside { A1_M_Bsy })) begin
               evl_cache_st_t new_state = A1_M_Bsy;

               `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to %p", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), new_state))
               line_desc.set_cache_st(new_state);
            end
         end
         else if (!(table_desc.get_tag_action() inside { Stale })) begin
            if (line_desc.get_cache_st() != table_desc.get_tag_action()) begin
               `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to %p", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), table_desc.get_tag_action()))
            end
            line_desc.set_cache_st(table_desc.get_tag_action());
         end
      end

      // In some cases we need to save the line data before it gets modified to be used in the victim later in this function
      if (table_desc.get_data_action() inside { Data_ReqWrBack_MemWr, Data_ReqWrBack_ZeroFill }) begin
         saved_victim_req_data = line_desc.get_line_data_copy();
         saved_victim_req_data.copy_modified_to_valid();
      end


      // Coverage:
      if (m_shire_cache_scoreboard_cover != null) begin
         if (table_desc.get_data_action() inside { Data_MemWr}) begin
            m_shire_cache_scoreboard_cover.sample_sc_cache_wr(line_desc.get_line_data(),req_desc.get_req_data(),m_zero_st_enable);
         end
         else if (table_desc.get_data_action() inside { Data_CacheFill}) begin
            m_shire_cache_scoreboard_cover.sample_sc_cache_fill(line_desc.get_line_data(),req_desc.get_rsp_data(),m_zero_st_enable);
         end
         else if (table_desc.get_data_action() inside { Data_MemRd}) begin
            m_shire_cache_scoreboard_cover.sample_sc_cache_rd(line_desc.get_line_data(),m_zero_st_enable);
         end
      end
      
      
      //
      // Process the data action as necessary.
      //
      if (table_desc.get_data_action() inside { Data_MemWr, Data_MrgWrBack, Data_MemWrAll, Data_MemWrRd, Data_MemWrRdNorm, Data_ReqWr, Data_ReqWrNorm, Data_ReqWrBack_MemWr }) begin
         if (req_desc.get_req_data() != null) begin
            if (opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic }) begin
               evl_bus_atomic_conf_t atomic_conf;
               // need to set the atomic operation for L3 (etlink monitor sets it for L2)
               if (req_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
                  evl_qword_t qword;

                  qword = req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB - 1));
                  req_desc.set_bus_atomic_conf(qword[`EVL_BUS_ATOMIC_CONF_WIDTH - 2:0]);
                  req_desc.set_bus_atomic_offset(qword >> (`EVL_BUS_ATOMIC_CONF_WIDTH - 1));
               end
               // this is as good a place as any to check conf[6] which is the dest bit. Brandon wanted
               // us to check to make sure the neigh/stimulus is sending the right thing.
               atomic_conf = req_desc.get_bus_atomic_conf();
               if (opcode inside { SC_L2_Atomic }) begin
                  if (atomic_conf[6] == 1'b1) begin
                     `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("Expected atomic_conf[6] to be 0 for this %p but atomic_conf = %x",opcode,atomic_conf))
                  end
               end
               else if (opcode inside { SC_L3_Atomic, SC_Scp_Atomic }) begin
                  if (atomic_conf[6] == 1'b0) begin
                     `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("Expected atomic_conf[6] to be 1 for this %p but atomic_conf = %x",opcode,atomic_conf))
                  end
               end

               //sample coverage:
               if (m_shire_cache_scoreboard_cover != null) begin
                  m_shire_cache_scoreboard_cover.sample_sc_atomics(req_desc,line_desc);
               end
               
               if (table_desc.get_data_action() inside { Data_MemWrRdNorm, Data_ReqWrNorm }) begin
                  atomic_response = line_desc.process_atomic(req_desc, 1'b1);
               end
               else begin
                  atomic_response = line_desc.process_atomic(req_desc, 1'b0);
               end
               line_desc.set_modified_qword_en();
               if (table_desc.get_data_action() inside { Data_MemWrRd, Data_MemWrRdNorm }) begin
                  if ((req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_VEC_OP) == 0) begin
                     if ((req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_DW_OP) == 0) begin
                        req_desc.set_bus_rsp_size(`EVL_WORD_ADDR_LSB);
                     end
                     else begin
                        req_desc.set_bus_rsp_size(`EVL_DWORD_ADDR_LSB);
                     end
                  end
                  else begin
                     req_desc.set_bus_rsp_size(`EVL_OWORD_ADDR_LSB);
                  end
                  req_desc.set_rsp_data(atomic_response);
               end
            end
            else begin
               line_desc.merge_data_from_line(req_desc.get_req_data(), 1'b1);
               if (table_desc.get_data_action() inside { Data_MemWrAll, Data_MemWrRdNorm }) begin
                  line_desc.set_modified_qword_en();
               end
            end
         end
      end
      else if (table_desc.get_data_action() == Data_MemWrInst) begin
         // MemWrInst is a hint that the previous state is invalid, so there might be a victim
         if (req_desc.get_req_data() != null) begin
            line_desc.merge_data_from_line(req_desc.get_req_data(), 1'b1);
         end
         prev_line_desc = m_cache.allocate_way(obs_way, line_desc);
         if (prev_line_desc == line_desc) begin
            prev_line_desc = null;
         end
         if (prev_line_desc != null) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_MemWrInst for 0x%x (replaced 0x%x)", bank_id, get_way_name(obs_way), line_desc.get_paddr(), prev_line_desc.get_paddr()))
         end
         else begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_MemWrInst for 0x%x", bank_id, get_way_name(obs_way), line_desc.get_paddr()))
         end
      end
      else if (table_desc.get_data_action() == Data_MemRd) begin
         evl_line_data rsp_data;
         rsp_data = copy_appropriate_amount_of_rsp_data(line_desc.get_line_data(),req_desc.get_bus_req_size(),req_desc.get_paddr());
         req_desc.set_rsp_data(rsp_data);
      end
      else if (table_desc.get_data_action() == Data_ZeroRsp) begin
         evl_line_data line_data;
         evl_line_data rsp_data;
         line_data = new();
         line_data.set_xword(0,evl_xword_t'(0));
         rsp_data = copy_appropriate_amount_of_rsp_data(line_data,req_desc.get_bus_req_size(),req_desc.get_paddr());
         req_desc.set_rsp_data(rsp_data);
      end
      else if (table_desc.get_data_action() == Data_CacheFill) begin
         prev_line_desc = m_cache.allocate_way(obs_way, line_desc);
         if (prev_line_desc == line_desc) begin
            prev_line_desc = null;
         end
         line_desc.set_xword(address >> `EVL_XWORD_ADDR_LSB, req_desc.get_rsp_xword(address >> `EVL_XWORD_ADDR_LSB));
         if (prev_line_desc != null) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_CacheFill for 0x%x (replaced 0x%x)", bank_id, get_way_name(obs_way), line_desc.get_paddr(), prev_line_desc.get_paddr()))
         end
         else begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_CacheFill for 0x%x", bank_id, get_way_name(obs_way), line_desc.get_paddr()))
         end
      end // if (table_desc.get_data_action() == Data_CacheFill)
      else if (table_desc.get_data_action() inside { Data_ZeroFill, Data_ReqWrBack_ZeroFill }) begin
         // Set the data to all zeros
         line_desc.set_xword(address >> `EVL_XWORD_ADDR_LSB, evl_xword_t'(0));
         // Lock state is also modified, so set to modified
         line_desc.set_modified_qword_en();
      end
      else if (table_desc.get_data_action() == Data_ZeroFillAlloc) begin
         prev_line_desc = m_cache.allocate_way(obs_way, line_desc);
         if (prev_line_desc == line_desc) begin
            prev_line_desc = null;
         end
         //set the data to all zeros
         line_desc.set_xword(address >> `EVL_XWORD_ADDR_LSB, evl_xword_t'(0));
         // Lock state is also modified, so set modified flags
         line_desc.set_modified_qword_en();
         if (prev_line_desc != null) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_ZeroFillAlloc for 0x%x (replaced 0x%x)", bank_id, get_way_name(obs_way), line_desc.get_paddr(), prev_line_desc.get_paddr()))
         end
         else begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) allocated way %0s for Data_ZeroFillAlloc for 0x%x", bank_id, get_way_name(obs_way), line_desc.get_paddr()))
         end
      end

      //
      // Process the set_modified_qword_en() actions here; this is necessary because we need to use
      // the modified bits to be set properly for any BusWrite operation.
      //
      if (evl_error_code'(table_desc.get_err_code()) inside { EC_SetQW, EC_SetQW_DeqWA, EC_Victim_SetQW }) begin
         line_desc.set_modified_qword_en();
      end

      //
      // Process the response action as necessary.
      //
      //Always set observed so that the response will be checked whenever it shows up
      req_desc.set_observed();
      if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
         // If this is a fill and we need to mark an error, we need to do it in the src_req_desc because req_desc
         // is the request/response to/from downstream.
         if (opcode inside {SC_L2_Fill,SC_L3_Fill,SC_Scp_Fill}) begin
            if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
               evl_req_desc   src_req_desc;
               src_req_desc = req_desc.get_parent_req_desc();
               src_req_desc.set_abstract_rsp_type(RspErr);
               src_req_desc.set_bus_rsp_valid(0);
            end
            else begin
               evl_req_desc   src_req_desc;
               src_req_desc = req_desc.get_parent_req_desc();
               src_req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
               src_req_desc.set_bus_rsp_valid(0);
            end // else: !if((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1))
         end
         else begin
            if ((evl_error_code'(table_desc.get_err_code()) inside { EC_MaybeErrRsp }) && (m_err_rsp_enable == 1'b1)) begin
               req_desc.set_abstract_rsp_type(RspErr);
            end
            else begin
               req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
            end
            req_desc.set_bus_rsp_valid(0);
         end
         req_desc.set_done();
      end


      // If this is a ReadCoop, check to see if we need to respond to more than one neigh.
      
      if ((evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) == RspWData) && (bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_ReadCoop))) begin
         
         `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("process_tc_req calling predict_read_coop_rsp"))
         predict_read_coop_rsp(req_desc);
      end

      //
      // Process the bus action as necessary.
      //
      if (!(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) inside { BusIdle, BusVictim })) begin
         evl_bus_req_id_t req_id;
         evl_req_desc     axi_req_desc;

         req_id = gen_bus_req_id(bank_id, reqq_id);
         $cast(axi_req_desc, m_template_req_desc_axi.clone());
         axi_req_desc.set_trans_id(req_desc.get_trans_id());
         axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
         if (table_desc.get_bus_cmd() inside { BusLineRd }) begin
            axi_req_desc.set_paddr(req_desc.get_paddr() & const_line_paddr_mask);
            axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
         end
         else begin
            axi_req_desc.set_paddr(req_desc.get_paddr());
            axi_req_desc.set_bus_req_size(req_desc.get_bus_req_size());
         end
         axi_req_desc.set_bus_bank(bank_id);
         axi_req_desc.set_bus_req_id(req_id);
         axi_req_desc.set_int_param(pk_etlink_cmd, int'(etlink_cmd));
         axi_req_desc.set_int_param(pk_reqq_id, reqq_id);
         if (req_desc.have_int_param(pk_paired_reqq_id) == 1'b1) begin
            axi_req_desc.set_int_param(pk_paired_reqq_id, req_desc.get_int_param(pk_paired_reqq_id));
         end

         // AXUSER
         if (table_desc.get_bus_cmd() inside { BusRead }) begin
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
         end
         else if (table_desc.get_bus_cmd() inside { BusLineRd }) begin
            axi_req_desc.set_paddr(req_desc.get_paddr() & const_line_paddr_mask);
            axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
         end
         else if (table_desc.get_bus_cmd() == BusFlush) begin
            axi_req_desc.set_paddr(req_desc.get_paddr() & const_line_paddr_mask);
            axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Flush));
         end
         else if (table_desc.get_bus_cmd() == BusEvict) begin
            axi_req_desc.set_paddr(req_desc.get_paddr() & const_line_paddr_mask);
            axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Evict));
         end
         else if (table_desc.get_bus_cmd() == BusWriteAtomic) begin
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_AtomicRsp));
         end
         else begin
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Write));
         end

         // AXQOS
         if (table_desc.get_bus_cmd() == BusWriteAtomic) begin
            axi_req_desc.set_bus_qos(`AXI_QOS_ATOMIC_RSP);
         end

         if (table_desc.get_data_action() inside { Data_ReqWrBack, Data_MrgWrBack }) begin
            evl_line_data req_data;

            req_data = line_desc.get_line_data_copy();
            req_data.copy_modified_to_valid();
            axi_req_desc.set_req_data(req_data);
         end
         else if (table_desc.get_data_action() inside { Data_FwdWr }) begin
            evl_line_data req_data;

            if (opcode inside { SC_L3_Atomic, SC_Scp_Atomic }) begin // L3 atomic and there's no L3:
               evl_bus_atomic_conf_t atomic_conf;
               // need to set the atomic operation for L3 (etlink monitor sets it for L2)
               if (req_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
                  evl_qword_t qword;
                  evl_paddr_t axi_lower_addr;
                  int offset;
                  
                  evl_paddr_t paddr;
                  qword = req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB - 1));
                  axi_lower_addr = evl_paddr_t'(qword >> `EVL_AXI_ATOMIC_ADDR_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_ADDR_MASK);
                  req_desc.set_bus_atomic_conf(qword[`EVL_BUS_ATOMIC_CONF_WIDTH - 2:0]);
                  req_desc.set_bus_atomic_offset(qword >> (`EVL_BUS_ATOMIC_CONF_WIDTH - 1));
                  //luckily we've already gotten the addr offset from the data and put it in this req_desc
                  atomic_response = new();
                  atomic_conf = req_desc.get_bus_atomic_conf();
                  //Need to set the appropriate byte enables based on the size
                  if (atomic_conf[4] == 1'b1) begin //vector/simd: 256bit response
                     offset = axi_lower_addr[5];
                     atomic_response.set_oword((offset),req_desc.get_req_oword(0));
                  end
                  else begin //scalar
                     if (atomic_conf[5] == 1'b1) begin // 64b
                        offset = axi_lower_addr[5:3];//paddr[5:3];
                        // there are only 4 dwords of data in the original request
                        atomic_response.set_dword(offset,req_desc.get_req_dword(offset[1:0]));
                     end
                     else begin //32b
                        offset = axi_lower_addr[5:2];
                        // There are only 8 words of data in the original request
                        atomic_response.set_word(offset,req_desc.get_req_word(offset[2:0]));
                     end
                  end
               end
            end // if (opcode inside { SC_L3_Atomic })
            else begin
               
               req_data = req_desc.get_req_data();
               // make a copy of the req data for the axi req desc rather than giving it a pointer to the same data
               axi_req_desc.set_req_data(req_data.clone_line_data());
            end
         end
         else if (table_desc.get_data_action() inside { Data_ReqWrNull }) begin
            axi_req_desc.invalidate_req_data();
         end

         
         // L3 Atomic response
         // (which looks like a request to a register)
         if (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) inside { BusWriteAtomic }) begin
            evl_qword_t axi_info_qword;
            evl_paddr_t axi_lower_addr;
            evl_paddr_t axi_shire_id;
            evl_qword_t axi_wuser_id;

            axi_info_qword = req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB + 1));
            axi_lower_addr = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_ADDR_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_ADDR_MASK);
            axi_shire_id   = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_SOURCE_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_SOURCE_MASK);
            axi_wuser_id   = (axi_info_qword >> `EVL_AXI_ATOMIC_TRANSID_SHIFT) & evl_qword_t'(`EVL_AXI_ATOMIC_TRANSID_MASK);
            axi_req_desc.set_paddr(`SC_L3_ATOMIC_RSP_ADDR_OFFSET | evl_paddr_t'(axi_lower_addr) | (evl_paddr_t'(axi_shire_id) << `SC_L3_ATOMIC_RSP_SHIRE_ID_SHIFT));
            if ((req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_VEC_OP) == 0) begin
               if ((req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_DW_OP) == 0) begin
                  axi_req_desc.set_bus_req_size(`EVL_WORD_ADDR_LSB);
               end
               else begin
                  axi_req_desc.set_bus_req_size(`EVL_DWORD_ADDR_LSB);
               end
            end
            else begin
               axi_req_desc.set_bus_req_size(`EVL_OWORD_ADDR_LSB);
            end
            axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(axi_wuser_id));
            axi_req_desc.set_req_data(atomic_response);
         end

         if (table_desc.get_tag_action() inside { Stale, Stale_M, A1_Stale_M_Bsy }) begin
            axi_req_desc.set_ignore_st_on_rsp();
         end


         void'(req_desc.add_child_desc(axi_req_desc));
         final_axi_req_fixup(axi_req_desc,evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
         m_shire_cache_axi_meshq.add_pending_axi_req_desc(axi_req_desc, m_sb_type);

         // Also add to our own queue to keep track of who is waiting for a response
         enqueue_pending_downstream_rsp(axi_req_desc);
      end

      //
      // Enqueue the victim line.  It is OK if this is a null handle.
      //
      if (evl_error_code'(table_desc.get_err_code()) inside { EC_Victim, EC_Victim_ClrQW, EC_Victim_SetQW }) begin
         m_victim_line_queue.push_back(line_desc);
      end
      else begin
         m_victim_line_queue.push_back(prev_line_desc);
      end

      //
      // If we have replaced a cache line, enqueue an eviction.
      //
      if ((m_perform_qword_en_check == 1'b1) && (prev_line_desc != null)) begin
         perform_qword_en_check(prev_line_desc);
      end

      if (prev_line_desc != null) begin
         if (m_coalescing_buffer.exists(bank_id)) begin
            m_coalescing_buffer[bank_id].find_and_remove_by_addr(prev_line_desc.get_paddr());
         end

         if (prev_line_desc.get_cache_st() inside { A1_E_Bsy }) begin
            // example: L2 has sent a Flush downstream, and we're waiting for a response but the line gets replaced in the meantime.
            `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), prev_line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to A1_I_Bsy ", bank_id, prev_line_desc.get_paddr(), prev_line_desc.get_cache_st()))
            prev_line_desc.set_cache_st(A1_I_Bsy);
            m_cache.deallocate_way(prev_line_desc);
         end
         else if (prev_line_desc.get_cache_st() inside { A2_E_Bsy }) begin
            // example: L2 has sent a Flush downstream, and we're waiting for a response but the line gets replaced in the meantime.
            `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), prev_line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to A2_I_Bsy ", bank_id, prev_line_desc.get_paddr(), prev_line_desc.get_cache_st()))
            prev_line_desc.set_cache_st(A2_I_Bsy);
            m_cache.deallocate_way(prev_line_desc);
         end
         else if (prev_line_desc.get_cache_st() inside { E }) begin
            int new_cycle_count;

            //
            // Delay the final change of this cache line to the tag response stage ... this allows RBuf hits to occur.
            //
            if (m_ram_delay[bank_id] == 3) begin
               new_cycle_count = cycle_count + 4;
            end
            else begin
               new_cycle_count = cycle_count + 3;
            end
            `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), prev_line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to EI_Pend (final invalidate delayed until cycle %0d)", bank_id, prev_line_desc.get_paddr(), prev_line_desc.get_cache_st(), new_cycle_count))
            prev_line_desc.set_cache_st(EI_Pend);
            m_delayed_inv_cycle_counts.push_back(new_cycle_count);
            m_delayed_inv_cycle_banks.push_back(bank_id);
            m_delayed_inv_line_descs.push_back(prev_line_desc);
            m_cache.deallocate_way(prev_line_desc);
         end
         else if (prev_line_desc.get_cache_st() inside { `EVL_LINE_DIRTY_STATES, `EVL_LINE_WA_DIRTY_STATES }) begin
            evl_sc_reqq_id_t victim_reqq_id;
            evl_line_data    req_data;

            if ((table_desc.get_bus_cmd() == evl_bus_cmd_t'(BusVictim)) && (req_desc.have_int_param(pk_paired_reqq_id) == 1'b1)) begin
               victim_reqq_id = evl_sc_reqq_id_t'(req_desc.get_int_param(pk_paired_reqq_id));
            end
            else begin
               victim_reqq_id = reqq_id;
            end

            //
            // Create a new request descriptor for the victim.  This should use the current request
            // descriptor as a template (we will see this descriptor used in the tag response stage).
            //
            $cast(victim_req_desc, req_desc.clone());
            victim_req_desc.set_bus_cmd(req_desc.get_bus_cmd());
            victim_req_desc.set_paddr(prev_line_desc.get_paddr());
            victim_req_desc.set_line_desc(prev_line_desc);
            victim_req_desc.set_int_param(pk_l2_opcode, int'(opcode));
            victim_req_desc.set_int_param(pk_etlink_cmd, int'(etlink_cmd));
            victim_req_desc.set_int_param(pk_reqq_id, int'(victim_reqq_id));

            //
            // Because data is dirty, we need to copy the cache data into the victim request
            // descriptor (it will be used in the tag response stage).
            //
            req_data = prev_line_desc.get_line_data_copy();
            req_data.copy_modified_to_valid();
            victim_req_desc.set_req_data(req_data);

            //
            // Add the victim request descriptor as a child of the current request descriptor.
            //
            void'(req_desc.add_child_desc(victim_req_desc));

            //
            // Enqueue the victim as a pending eviction, allocate the request queue entry to this
            // new descriptor, and deallocate the victim line.
            //
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("TC req model (bank %0d) 0x%x selected for replacement. cache_st = %0p", bank_id, prev_line_desc.get_paddr(),prev_line_desc.get_cache_st()))
//          m_shire_cache_reqq.enqueue_reqq_entry(bank_id, victim_reqq_id, victim_req_desc, req_desc, 1'b0, m_verbosity_mod);
            enqueue_pending_evict(bank_id, victim_reqq_id, victim_req_desc);
            if (prev_line_desc.get_cache_st() inside { `EVL_LINE_WA_DIRTY_STATES }) begin
               cb_inv_process_cb_evict(victim_req_desc);
            end
            m_cache.deallocate_way(prev_line_desc);
         end
         else if (prev_line_desc.get_cache_st() inside { `EVL_LINE_VALID_STATES }) begin
            m_cache.deallocate_way(prev_line_desc);
         end
         else begin
            // If you get this error but in the rtl world there's nothing being replaced, it's probably because the line didn't get
            // deallocated in the scoreboard because a state is not in EVL_LINE_DEALLOC_STATES
            `dut_error(get_abstract_name(), $sformatf("unexpected cache state for line selected for replacement ... cache_st:%p, addr:0x%x, way:%s", prev_line_desc.get_cache_st(), prev_line_desc.get_paddr(), prev_line_desc.get_cache_way_name()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            m_cache.deallocate_way(prev_line_desc);
         end
      end // if ((prev_line_desc != null)

      // This applies if the current req_desc is a victim (ex: an evict)
      if ((table_desc.get_bus_cmd() == evl_bus_cmd_t'(BusVictim)) && (victim_req_desc == null)) begin
         $cast(victim_req_desc, req_desc.clone());
         victim_req_desc.set_int_param(pk_l2_opcode, int'(opcode));
         victim_req_desc.set_int_param(pk_etlink_cmd, int'(bus_cmd));
         victim_req_desc.set_int_param(pk_reqq_id, int'(reqq_id));
          if (req_desc.have_int_param(pk_paired_reqq_id) == 1'b1) begin
             victim_req_desc.set_int_param(pk_paired_reqq_id, req_desc.get_int_param(pk_paired_reqq_id));
          end
         victim_req_desc.set_line_desc(line_desc);
         if (table_desc.get_data_action() inside { Data_ReqWrBack_MemWr, Data_ReqWrBack_ZeroFill}) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("using saved_victim_req_data"))
            victim_req_desc.set_req_data(saved_victim_req_data);
         end
         else if (table_desc.get_data_action() inside { Data_ReqWrBack, Data_MrgWrBack, Data_MemWrInst }) begin
            evl_line_data req_data;

            req_data = line_desc.get_line_data_copy();
            req_data.copy_modified_to_valid();
            victim_req_desc.set_req_data(req_data);
         end
         victim_req_desc.set_int_param(pk_bypass_for_parent, 1);
         void'(req_desc.add_child_desc(victim_req_desc));
         enqueue_pending_evict(bank_id, reqq_id, victim_req_desc);

         // If we're evicting something from the coalescing buffer, that might need to be tracked for a cb inv.
         if (line_desc.get_cache_st() inside {WI_Bsy, A1_WI_Bsy, A2_WI_Bsy, WIE_Bsy}) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("calling cb_inv_process_cb_evict. cache state is %0p",line_desc.get_cache_st()))
            cb_inv_process_cb_evict(victim_req_desc);
         end
         
      end

      //
      // If we are supposed to enqueue the write-around address, do it.
      //
      if (evl_error_code'(table_desc.get_err_code()) inside { EC_EnqWA, EC_UpdWAMru }) begin
         evl_req_desc wa_req_desc;

         if (!m_coalescing_buffer.exists(bank_id)) begin
            m_coalescing_buffer[bank_id] = new($sformatf("%0s(CBUF-%0d)", get_root_abstract_name(), bank_id));
         end
         wa_req_desc = m_coalescing_buffer[bank_id].find_and_set_mru(address);
         if ((wa_req_desc == null) && (evl_error_code'(table_desc.get_err_code()) inside { EC_EnqWA })) begin
            if (m_coalescing_buffer[bank_id].size() >= m_coalesce_size[bank_id]) begin
               evl_req_desc     evict_req_desc;
               evl_sc_reqq_id_t victim_reqq_id;

               //
               // If we have replaced a coalescing buffer entry, queue up a victim.
               //
               wa_req_desc = m_coalescing_buffer[bank_id].pop();
               victim_reqq_id = evl_sc_reqq_id_t'(req_desc.get_int_param(pk_paired_reqq_id));
               $cast(evict_req_desc, wa_req_desc.clone());
               evict_req_desc.set_int_param(pk_reqq_id, victim_reqq_id);
               evict_req_desc.set_int_param(pk_etlink_cmd, wa_req_desc.get_int_param(pk_etlink_cmd));
               evict_req_desc.set_int_param(pk_l2_opcode, int'(SC_L2_Evict));
               evict_req_desc.set_req_data(wa_req_desc.get_req_data());
               void'(req_desc.add_child_desc(evict_req_desc));
               m_pending_cbuf_evicts[bank_id].push_back(evict_req_desc);
               m_shire_cache_reqq.enqueue_reqq_entry(bank_id, victim_reqq_id, evict_req_desc, null, 1'b0, m_verbosity_mod);
            end
            m_coalescing_buffer[bank_id].push(req_desc);
         end
      end

      // If it's an Atomic 1st-pass that is going to require a 2nd-pass, set up the req_desc for the second pass
      // cases: atomic 1) miss, 2) hit partial
      if (((opcode inside { SC_L2_Atomic, SC_L3_Atomic, SC_Scp_Atomic }) && 
           !(m_cache.has_sub_qualifier(qualifier,`SCQ_NoRegion)) && !(m_cache.has_sub_qualifier(qualifier,`SCQ_DecErr))) && 
          ((obs_st == I) || (table_desc.get_data_action() == Data_ReqWrBack))) begin
         `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("setting up the second atomic ... reqq_id=0x%0x", reqq_id))
         m_pending_atomics[bank_id].push_back(req_desc);
      end

      if (table_desc.get_tag_action() inside { I }) begin
         m_cache.deallocate_way(line_desc);
         line_desc.invalidate_line_data();
      end
      else if (table_desc.get_tag_action() inside { EI_Pend }) begin
         int new_cycle_count;

         //
         // Delay the final change of this cache line to the tag response stage ... this allows RBuf hits to occur.
         //
         if (m_ram_delay[bank_id] == 3) begin
            new_cycle_count = cycle_count + 4;
         end
         else begin
            new_cycle_count = cycle_count + 3;
         end
         `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), req_desc.get_trans_id(), reqq_id, $sformatf("TC req model (bank %0d) modifying cache state for 0x%x from %p to EI_Pend (final invalidate delayed until cycle %0d)", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), new_cycle_count))
         m_delayed_inv_cycle_counts.push_back(new_cycle_count);
         m_delayed_inv_cycle_banks.push_back(bank_id);
         m_delayed_inv_line_descs.push_back(line_desc);
         m_cache.deallocate_way(line_desc);
      end
      else if (table_desc.get_tag_action() inside { `EVL_LINE_DEALLOC_STATES }) begin
         m_cache.deallocate_way(line_desc);
      end

      //
      // Process the remaining miscellaneous actions as necessary.
      //
      if (evl_error_code'(table_desc.get_err_code()) inside { EC_DeqWA, EC_Victim, EC_Victim_ClrQW, EC_SetQW_DeqWA, EC_Victim_SetQW }) begin
         if (m_coalescing_buffer.exists(bank_id)) begin
            evl_req_desc descs_out[];
            m_coalescing_buffer[bank_id].find_all_by_paddr(descs_out,line_desc.get_paddr());
            //if there is a victim and the address exists in the coalescing buffer, then call the function that
            // keeps track of cb invalidates so it can update its snapshot of the coalescing buffer if necessary.
            if ((victim_req_desc != null) && (descs_out.size() > 0)) begin
               cb_inv_process_cb_evict(victim_req_desc);
            end
            
            m_coalescing_buffer[bank_id].find_and_remove_by_addr(line_desc.get_paddr());
         end
      end
      if (evl_error_code'(table_desc.get_err_code()) inside { EC_ClrQW, EC_DeqWA, EC_Victim_ClrQW }) begin
         line_desc.clr_modified_qword_en();
      end

      // Another case for cb inv tracking: If this is just a WriteAround and there's no victim
      // then the request is basically done so remove it from m_reqs_ahead_of_cb_inv
      if ((m_cb_inv_sync_check_in_progress[bank_id] == 1'b1) && (etlink_cmd == ET_LINK_REQ_WriteAround) && (evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()) inside { BusIdle })) begin
         m_reqs_ahead_of_cb_inv[bank_id].pop(req_desc);
      end

      if (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_SB) != 0) begin
         line_desc.print_obj(get_abstract_name());
         if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, pk_reqq_id, $sformatf("predicted response: %0s", req_desc.sprint_rsp()))
         end
      end
      m_pipe_info_tc.delete(bank_id);
   endfunction : process_tc_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tc_tag_rsp
   //
   function void process_tc_tag_rsp(input evl_trans_id_t trans_id, input evl_sc_bank_id_t bank_id, input evl_sc_pipe_tag_rsp_t rtl_info);
      evl_sc_reqq_id_t    reqq_id;
      evl_paddr_t         victim_address;
      bit                 victim_write_around;
      evl_line_qword_en_t victim_qwen;
      evl_bus_req_id_t    req_id;
      evl_req_desc        req_desc;
      evl_req_desc        victim_req_desc;
      evl_line_desc       victim_line_desc;
      evl_sc_qual_t       qualifier;
      
      reqq_id             = rtl_info.reqq_id;
      victim_address      = rtl_info.victim_address;
      victim_write_around = rtl_info.victim_write_around;
      victim_qwen         = rtl_info.victim_qwen;
      req_id              = gen_bus_req_id(bank_id, reqq_id);

      if (m_tag_rsp_queue.size() == 0) begin
         req_desc         = null;
         victim_line_desc = null;
      end
      else if ((trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) || (m_tag_rsp_queue[0].get_trans_id() == trans_id)) begin
         req_desc         = m_tag_rsp_queue.pop_front();
         victim_line_desc = m_victim_line_queue.pop_front();
      end
      else begin
         if (m_tag_rsp_queue[0].get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            req_desc         = m_tag_rsp_queue.pop_front();
            victim_line_desc = m_victim_line_queue.pop_front();
         end
         else begin
            int limit = m_tag_rsp_queue.size();
            int index = 0;

            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("trans_id mismatch ...  %0s, victim:%0b,%0b, victim_address:0x%x, reqq_id:0x%x, trans_id:0x%x", m_tag_rsp_queue[0].sprint_obj(), rtl_info.victim, rtl_info.victim_silent, victim_address, reqq_id, trans_id), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            while (index < limit) begin
               if (m_tag_rsp_queue[0].get_trans_id() == trans_id) begin
                  break;
               end
               index++;
            end
            if (index < limit) begin
               for (int ii = index - 1; ii >= 0; ii--) begin
                  m_tag_rsp_queue.delete(ii);
                  m_victim_line_queue.delete(ii);
               end
            end
            req_desc         = m_tag_rsp_queue.pop_front();
            victim_line_desc = m_victim_line_queue.pop_front();
         end
      end

      //
      // Find any pending victims for this bank/reqq ID combination.
      //
      if (m_pending_evicts[bank_id][reqq_id] == null) begin
         victim_req_desc = null;
      end
      else begin
         victim_req_desc = m_pending_evicts[bank_id][reqq_id];
         m_pending_evicts[bank_id][reqq_id] = null;
      end

      if (req_desc != null) begin
         if (victim_line_desc == null) begin
            if ((rtl_info.victim == 1'b1) || (rtl_info.victim_silent == 1'b1)) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("unexpected %0s victim (0x%x) forced by %0s", ((rtl_info.victim == 1'b1) ? "modified" : "silent"), victim_address, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
         else begin
            if ((rtl_info.victim == 1'b0) && (rtl_info.victim_silent == 1'b0)) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("expected 0x%x to be a victim forced by %0s", victim_line_desc.get_paddr(), req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
            else if (victim_line_desc.get_paddr() != victim_address) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("expected the victim address to be 0x%x, not 0x%x (forced by %0s)", victim_line_desc.get_paddr(), victim_address, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
      end

      if (rtl_info.victim == 1'b1) begin
         if (victim_req_desc == null) begin
            if (req_desc != null) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("unexpected victim (0x%x) forced by %0s", victim_address, req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
            else begin
               `dut_error(get_abstract_name(), $sformatf("unexpected victim (0x%x)", victim_address), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
         else begin
            evl_line_desc  victim_line_desc;
            evl_line_desc  line_desc;
            evl_table_desc table_desc;
            evl_bus_cmd_t  victim_bus_cmd;

            victim_line_desc = victim_req_desc.get_line_desc();
            if (victim_line_desc.get_paddr() != victim_address) begin
               `dut_error_mdesc(get_abstract_name(), victim_req_desc, $sformatf("expected the victim address to be 0x%x, not 0x%x (%0s, id %0d)", victim_line_desc.get_paddr(), victim_address, victim_req_desc.sprint_obj(), victim_req_desc.get_inst_id()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
            victim_req_desc.set_paddr(victim_address);

            if (victim_req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
               victim_bus_cmd = evl_bus_cmd_t'(victim_req_desc.get_bus_cmd());
            end
            else if (victim_req_desc.bus_cmd_is_write() == 1'b0) begin
               victim_bus_cmd = evl_bus_cmd_t'(ET_LINK_REQ_Read);
            end
            else if (m_extract_etlink_from_awuser == 1'b0) begin
               victim_bus_cmd = evl_bus_cmd_t'(ET_LINK_REQ_Write);
            end
            else begin
               victim_bus_cmd = evl_bus_cmd_t'(victim_req_desc.get_bus_req_user());
            end

            qualifier = `SCQ_Victim;
            
            qualifier = fix_qualifier_at_tag_rsp(qualifier, req_desc);
            
            //
            // Check victim address here
            //
            m_cache.int_req_lookup(table_desc, line_desc, SC_TABLE_TAG_RSP, victim_bus_cmd, evl_sc_reqq_opcode_t'(victim_req_desc.get_int_param(pk_l2_opcode)), M, victim_req_desc, qualifier, 0, "TC tag rsp stage");

            if (m_perform_qword_en_check == 1'b1) begin
               perform_qword_en_check(line_desc);
            end

            if (line_desc != victim_line_desc) begin
               `dut_error_mdesc(get_abstract_name(), victim_req_desc, $sformatf("TB_FAIL: line descriptor and victim line descriptor do not match (file %0s, line %0d)", `__FILE__, `__LINE__), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end

            //
            // Process the tag action as necessary.
            //
            if (!(table_desc.get_tag_action() inside { St_Ignore })) begin
               if (line_desc.get_cache_st() != table_desc.get_tag_action()) begin
                  `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), victim_req_desc.get_trans_id(), reqq_id, $sformatf("TC tag rsp model (bank %0d): modifying cache state for 0x%x from %p to %p", bank_id, line_desc.get_paddr(), line_desc.get_cache_st(), table_desc.get_tag_action()))
               end
               line_desc.set_cache_st(table_desc.get_tag_action());
            end

            //
            // Perform the bus action as necessary.
            //
            if (table_desc.get_bus_cmd() != evl_bus_cmd_t'(BusIdle)) begin
               evl_req_desc axi_req_desc;

               //
               // Create a new AXI request descriptor.  This new AXI request descriptor will become
               // a child of the victim request descriptor.
               //
               $cast(axi_req_desc, m_template_req_desc_axi.clone());
               if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  axi_req_desc.set_trans_id(victim_req_desc.get_trans_id());
               end
               else begin
                  axi_req_desc.set_trans_id(trans_id);
               end
               axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
               axi_req_desc.set_paddr(victim_address & const_line_paddr_mask);
               axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
               axi_req_desc.set_bus_bank(bank_id);
               axi_req_desc.set_bus_req_id(req_id);
               axi_req_desc.set_int_param(pk_etlink_cmd, req_desc.get_int_param(pk_etlink_cmd));
               axi_req_desc.set_int_param(pk_reqq_id, reqq_id);
               if (victim_req_desc.have_int_param(pk_paired_reqq_id) == 1'b1) begin
                  axi_req_desc.set_int_param(pk_paired_reqq_id, victim_req_desc.get_int_param(pk_paired_reqq_id));
               end
               //
               // Set the AWUSER bits to the appropriate value.
               //
               if (table_desc.get_bus_cmd() == evl_bus_cmd_t'(BusFlushToMem)) begin
                  axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_FlushToMem));
               end
               else if (table_desc.get_bus_cmd() == evl_bus_cmd_t'(BusEvictToMem)) begin
                  axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_EvictToMem));
               end
               else begin
                  axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(ET_LINK_REQ_Write));
               end

               //
               // Make the AXI request descriptor a child of the victim request descriptor.
               //
               void'(victim_req_desc.add_child_desc(axi_req_desc));
               axi_req_desc.set_int_param(pk_parent_done_on_rsp, 1);

               //
               // Set AXI request data as appopriate.  Note that we use the modified bits to
               // determine the AXI data byte enables.
               //
               if (table_desc.get_data_action() inside { Data_ReqWrBack, Data_MrgWrBack }) begin
                  evl_line_data req_data;

                  req_data = victim_req_desc.get_req_data();
                  if (req_data == null) begin
                     `dut_error_mdesc(get_abstract_name(), victim_req_desc, $sformatf("TB_FAIL: victim request descriptor (%0s) should have request data, but it doesn't (file %0s, line %0d)", victim_req_desc.sprint_obj(), `__FILE__, `__LINE__), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
                     req_data = line_desc.get_line_data_copy();
                     req_data.copy_modified_to_valid();
                  end
                  axi_req_desc.set_req_data(req_data);
                  if (rtl_info.cbuf_match == 1'b1) begin
                     bit found_match = 1'b0;

                     for (int ii = m_pending_cbuf_evicts[bank_id].size() - 1; ii >= 0; ii--) begin
                        if (m_pending_cbuf_evicts[bank_id][ii].line_addr_match(victim_address) == 1'b1) begin
                           `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), m_pending_cbuf_evicts[bank_id][ii], pk_reqq_id, $sformatf("removing %0s from the pending cbuf eviction queue (bank %0d)", m_pending_cbuf_evicts[bank_id][ii].sprint_obj(), bank_id))
                           m_pending_cbuf_evicts[bank_id].delete(ii);
                           found_match = 1'b1;
                           break;
                        end
                     end
                     if (found_match == 1'b0) begin
                        `dut_error_mdesc(get_abstract_name(), victim_req_desc, $sformatf("TB_FAIL (bank %0d): victim request descriptor (%0s) should match a coalescing buffer eviction, but it doesn't (file %0s, line %0d)", bank_id, victim_req_desc.sprint_obj(), `__FILE__, `__LINE__), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
                     end
                  end
               end // if (table_desc.get_data_action() inside { Data_ReqWrBack, Data_MrgWrBack })
               else if (table_desc.get_data_action() inside { Data_FwdWr }) begin
                  evl_line_data req_data;

                  req_data = req_desc.get_req_data();
                  // make a copy of the req data for the axi req desc rather than giving it a pointer to the same data
                  axi_req_desc.set_req_data(req_data.clone_line_data());
               end
               else if (table_desc.get_data_action() inside { Data_ReqWrNull }) begin
                  axi_req_desc.invalidate_req_data();
               end

               if (evl_error_code'(table_desc.get_err_code()) inside { EC_LateRead }) begin
                  axi_req_desc.set_int_param(pk_issue_late_read, 1);
               end

               final_axi_req_fixup(axi_req_desc,evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));

               //
               // In full verbosity, print the descriptor hierarchy.
               //
               axi_req_desc.print_from_root(UVM_FULL, VMOD_SC_SB);

               //
               // Send the AXI request descriptor to the AXI bus agent.
               //
               m_shire_cache_axi_meshq.add_pending_axi_req_desc(axi_req_desc, m_sb_type);

               // Also add to our own queue to keep track of who is waiting for a response
               enqueue_pending_downstream_rsp(axi_req_desc);
            end

            //
            // The victim line may contain partial data.  If it does, we need to remove the victim
            // address from the coalescing buffer.
            //
            if (evl_error_code'(table_desc.get_err_code()) inside { EC_DeqWA, EC_SetQW_DeqWA }) begin
               if (m_coalescing_buffer.exists(bank_id)) begin
                  m_coalescing_buffer[bank_id].find_and_remove_by_addr(victim_req_desc.get_paddr());
               end
            end

            //
            // If we are supposed to wipe out the modified byte indicators, do it.
            //
            if (evl_error_code'(table_desc.get_err_code()) inside { EC_ClrQW }) begin
               line_desc.clr_modified_qword_en();
            end

            //
            // Deallocate the cache line and invalidate its data if it is headed to an invalid state.
            //
            if (table_desc.get_tag_action() inside { `EVL_LINE_DEALLOC_STATES }) begin
               m_cache.deallocate_way(line_desc);
               line_desc.invalidate_line_data();
            end
         end

         if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_SC_SB) != 0) begin
            if (req_desc == null) begin
               if (rtl_info.err_rsp != SC_Err_None) begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b, err_rsp:%p (did not find req_desc)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen, rtl_info.err_rsp))
               end
               else begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b (did not find req_desc)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen))
               end
            end
            else begin
               if (rtl_info.err_rsp != SC_Err_None) begin
                  if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                     `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), req_desc.get_trans_id(), reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b, err_rsp:%p (%0s)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen, rtl_info.err_rsp, req_desc.sprint_obj()))
                  end
                  else begin
                     `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b, err_rsp:%p (%0s)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen, rtl_info.err_rsp, req_desc.sprint_obj()))
                  end
               end
               else begin
                  if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                     `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), req_desc.get_trans_id(), reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b (%0s)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen, req_desc.sprint_obj()))
                  end
                  else begin
                     `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, victim=0x%x, victim_wa:%0b, victim_qwen:%0b (%0s)", bank_id, rtl_info.hit, victim_address, victim_write_around, victim_qwen, req_desc.sprint_obj()))
                  end
               end
            end
         end
      end
      else if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_SC_SB) != 0) begin // victim == 0
         if (req_desc == null) begin
            if (rtl_info.err_rsp != SC_Err_None) begin
               `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, err_rsp:%p (did not find req_desc)", bank_id, rtl_info.hit, rtl_info.err_rsp))
            end
            else begin
               `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b (did not find req_desc)", bank_id, rtl_info.hit))
            end
         end
         else begin
            if (rtl_info.err_rsp != SC_Err_None) begin
               if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), req_desc.get_trans_id(), reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, err_rsp:%p (%0s)", bank_id, rtl_info.hit, rtl_info.err_rsp, req_desc.sprint_obj()))
               end
               else begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b, err_rsp:%p (%0s)", bank_id, rtl_info.hit, rtl_info.err_rsp, req_desc.sprint_obj()))
               end
            end
            else begin
               if (trans_id inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), req_desc.get_trans_id(), reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b (%0s)", bank_id, rtl_info.hit, req_desc.sprint_obj()))
               end
               else begin
                  `evl_log_addr_trid_id(UVM_MEDIUM, VMOD_SC_SB, get_abstract_name(), ~evl_paddr_t'(0), trans_id, reqq_id, $sformatf("TC tag rsp (bank %0d): hit=%0b (%0s)", bank_id, rtl_info.hit, req_desc.sprint_obj()))
               end
            end
         end
      end
   endfunction : process_tc_tag_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_cbuf_status
   //
   function void process_cbuf_status(input evl_trans_id_t trans_id, input evl_sc_bank_id_t bank_id, input evl_dv_cbuf_status_t rtl_info);
      if (rtl_info.valid == 1'b1) begin
         m_rtl_cbuf_addr_queues[bank_id].push_back(rtl_info.address);
      end
   endfunction : process_cbuf_status;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bus_response
   //
   function void process_bus_response(input evl_req_desc req_desc);
      evl_table_desc table_desc;
      evl_line_desc  line_desc;
      evl_req_desc   src_req_desc;
      evl_bus_cmd_t  src_bus_cmd;
      evl_axi_resp_t axi_resp_type;

      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("processing AXI bus response for %0s", req_desc.sprint_obj()))
      dequeue_pending_downstream_rsp(req_desc);
      cb_inv_process_response(req_desc);
      src_req_desc = req_desc.get_parent_req_desc();
      if (src_req_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("no source request could be found for %0s", req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end
      else begin
         if (src_req_desc.have_int_param(pk_etlink_cmd) == 1'b1) begin
            src_bus_cmd = evl_bus_cmd_t'(src_req_desc.get_int_param(pk_etlink_cmd));
         end
         else if (src_req_desc.get_bus_type() != EVL_BUS_TYPE_AXI) begin
            src_bus_cmd = src_req_desc.get_bus_cmd();
         end
         else begin
            src_bus_cmd = (src_req_desc.bus_cmd_is_write() == 1'b1) ? evl_bus_cmd_t'(src_req_desc.get_bus_req_user()) : evl_bus_cmd_t'(ET_LINK_REQ_Read);
         end

         //This is a temporary check to make error messages more obvious. Remove once the scoreboard supports
         // error responses.
         axi_resp_type = evl_axi_resp_t'(req_desc.get_bus_rsp_type());
         if (axi_resp_type != AXI_RESP_OKAY) begin
            if (src_bus_cmd != ET_LINK_REQ_Read) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("shire cache scoreboard received an AXI response of type %0p, but it does not yet support error responses on AXI.",axi_resp_type), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end

         // If the set size is 0, then there is no cache space allocated for this L2/L3/Scp
         if (m_bypass == 1'b1) begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, src_bus_cmd, req_desc.get_bus_cmd(), req_desc.get_bus_rsp_type(), `SCQ_Bypass);
         end
         else if (req_desc.ignore_st_on_rsp() == 1'b1) begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, src_bus_cmd, req_desc.get_bus_cmd(), req_desc.get_bus_rsp_type(), `SCQ_Stale);
         end
         else if (m_set_size == 0) begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, src_bus_cmd, req_desc.get_bus_cmd(), req_desc.get_bus_rsp_type(), `SCQ_NoRegion);
         end
         else if (req_desc.get_int_param(pk_issue_late_read, 0) != 0) begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, src_bus_cmd, req_desc.get_bus_cmd(), req_desc.get_bus_rsp_type(), `SCQ_LateRead);
         end
         else begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, src_bus_cmd, req_desc.get_bus_cmd(), req_desc.get_bus_rsp_type(), `SCQ_None);
         end

         if (m_perform_qword_en_check == 1'b1) begin
            perform_qword_en_check(line_desc);
         end

         if (table_desc.get_tag_action() != St_Ignore) begin
            if (line_desc.get_cache_st() != table_desc.get_tag_action()) begin
               `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), src_req_desc.get_trans_id(), req_desc.get_bus_req_id(), $sformatf("Rsp model modifying cache state for 0x%x from %p to %p", line_desc.get_paddr(), line_desc.get_cache_st(), table_desc.get_tag_action()))
            end
            line_desc.set_cache_st(table_desc.get_tag_action());
         end

         if (table_desc.get_data_action() == Data_CascadeRsp) begin
            evl_line_data downstream_rsp_data;
            evl_line_data upstream_rsp_data;

            downstream_rsp_data = req_desc.get_rsp_data();
            upstream_rsp_data = copy_appropriate_amount_of_rsp_data(downstream_rsp_data,src_req_desc.get_bus_req_size(),src_req_desc.get_paddr());
            src_req_desc.set_rsp_data(upstream_rsp_data);
         end
         else if (table_desc.get_data_action() == Data_CascadeFill) begin
            evl_line_data downstream_rsp_data;
            evl_line_data upstream_rsp_data;

            downstream_rsp_data = req_desc.get_rsp_data();
            upstream_rsp_data = copy_appropriate_amount_of_rsp_data(downstream_rsp_data,src_req_desc.get_bus_req_size(),src_req_desc.get_paddr());
            src_req_desc.set_rsp_data(upstream_rsp_data);
            enqueue_pending_fill(req_desc);
         end
         else if (table_desc.get_data_action() == Data_CascadeFillZero) begin
            evl_line_data upstream_rsp_data;

            upstream_rsp_data = new();
            upstream_rsp_data.set_xword(0,evl_xword_t'(0));
            src_req_desc.set_rsp_data(upstream_rsp_data);
            enqueue_pending_fill(req_desc);
         end
         else if (table_desc.get_data_action() == Data_EnqueueFill) begin
            enqueue_pending_fill(req_desc);
         end

         if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("forwarding bus response to %0s", src_req_desc.sprint_obj()))
            src_req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
            src_req_desc.set_bus_rsp_valid(0);
            src_req_desc.set_done();
         end


         // If this is a ReadCoop response, check to see if it's responding to more than one neigh.
         if ((evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) == RspWData) && (src_bus_cmd == evl_bus_cmd_t'(ET_LINK_REQ_ReadCoop))) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("process_bus_rsp calling predict_read_coop_rsp"))
            predict_read_coop_rsp(src_req_desc);
         end

         
         if (table_desc.get_bus_cmd() != evl_bus_cmd_t'(BusIdle)) begin
            evl_bus_req_id_t req_id;
            evl_req_desc     axi_req_desc;

            if ((table_desc.get_bus_cmd() == evl_bus_cmd_t'(BusReadParent)) && (src_req_desc.get_int_param(pk_bypass_for_parent, 0) != 0)) begin
               if (src_req_desc.get_parent_req_desc() == null) begin
                  `dut_error_mdesc(get_abstract_name(), src_req_desc, $sformatf("no source request could be found for %0s", src_req_desc.sprint_obj()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
               end
               else begin
                  src_req_desc = src_req_desc.get_parent_req_desc();
               end
            end
            req_id = req_desc.get_bus_req_id();
            $cast(axi_req_desc, m_template_req_desc_axi.clone());
            axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
            axi_req_desc.set_paddr(src_req_desc.get_paddr() & const_line_paddr_mask);
            axi_req_desc.set_bus_bank(src_req_desc.get_bus_bank());
            axi_req_desc.set_bus_req_id(req_id);
            axi_req_desc.set_bus_req_size(`EVL_LINE_ADDR_LSB);
            axi_req_desc.set_int_param(pk_etlink_cmd, src_req_desc.get_int_param(pk_etlink_cmd));
            axi_req_desc.set_int_param(pk_reqq_id, src_req_desc.get_int_param(pk_reqq_id));
            if (src_req_desc.have_int_param(pk_paired_reqq_id) == 1'b1) begin
               axi_req_desc.set_int_param(pk_paired_reqq_id, src_req_desc.get_int_param(pk_paired_reqq_id));
            end

            void'(src_req_desc.add_child_desc(axi_req_desc));
            
            final_axi_req_fixup(axi_req_desc,evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
            
            m_shire_cache_axi_meshq.add_pending_axi_req_desc(axi_req_desc, m_sb_type);
            axi_req_desc.print_from_root(UVM_FULL, VMOD_SC_SB);
            
            // Also add to our own queue to keep track of who is waiting for a response
            enqueue_pending_downstream_rsp(axi_req_desc);
         end
         if (table_desc.get_tag_action() == I) begin
            line_desc.invalidate_line_data();
         end
         req_desc.set_done();
         if ((src_req_desc != null) && (req_desc.get_int_param(pk_parent_done_on_rsp) == 1)) begin
            src_req_desc.set_done();
         end
      end
   endfunction : process_bus_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_appropriate_amount_of_rsp_data
   //
   function evl_line_data copy_appropriate_amount_of_rsp_data(input evl_line_data rsp_data_in, input evl_bus_req_size_t req_size_in, input evl_paddr_t addr_in);
      evl_xword_t xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_line_data rsp_data_out;
      evl_paddr_t addr_mask;
      int shift_value;
      
      addr_mask = ((1<<(`EVL_LINE_ADDR_LSB))-1) & ~((1<<req_size_in)-1);
      shift_value = addr_mask & addr_in;
     
      xword_data = rsp_data_in.get_xword(0);
      case (req_size_in)
         'd0 : xword_byte_en = 64'h1;
         'd1 : xword_byte_en = 64'h3;
         'd2 : xword_byte_en = 64'hF;
         'd3 : xword_byte_en = 64'hFF;
         'd4 : xword_byte_en = 64'hFFFF;
         'd5 : xword_byte_en = 64'hFFFF_FFFF;
         'd6 : xword_byte_en = 64'hFFFF_FFFF_FFFF_FFFF;
      endcase // case
      xword_byte_en = xword_byte_en << shift_value;
     
      rsp_data_out = new();
      rsp_data_out.set_xword(0,xword_data,xword_byte_en);
      return rsp_data_out;
   endfunction : copy_appropriate_amount_of_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_all_inv
   //
   function void process_all_inv(input evl_sc_bank_id_t bank_id);
      evl_paddr_t bank_mask;
      evl_paddr_t shifted_bank_id;

      bank_mask = (`SC_BANKS - 1) << m_bank_lsb;
      shifted_bank_id = bank_id << m_bank_lsb;
      `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("process_all_inv: Going to invalidate cache model with mask 0x%0x and shifted bank id 0x%0x.", bank_mask, shifted_bank_id))
      m_cache.reset_matching_lines(bank_mask, shifted_bank_id);
      //also clear out the coalescing buffer
      if (m_coalescing_buffer.exists(bank_id)) begin
         m_coalescing_buffer[bank_id] = new($sformatf("%0s(CBUF-%0d)", get_root_abstract_name(), bank_id));
      end
   endfunction : process_all_inv


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_cb_inv_start
   //
   // We want to check that all of the appropriate requests are complete before the shire cache signals that it is
   // done with a cb inv cacheop. This function gets called when a cb inv is first started and it's job is to
   // gather up all of the requests that will need to eventually be finished before the cb inv can be
   // considered "done".
   //
   function void process_cb_inv_start(input evl_sc_bank_id_t bank_id);
      evl_req_desc tmp_req_desc;
      evl_bus_cmd_t bus_cmd;
      evl_etlink_req_opcode_t etlink_cmd;
      if (m_reqs_ahead_of_cb_inv[bank_id] == null) begin
         m_reqs_ahead_of_cb_inv[bank_id] = new($sformatf("%0s(CBINVREQ-%0d)",get_root_abstract_name(),bank_id));
      end
      
      if (m_cb_inv_sync_check_in_progress[bank_id] == 1'b1) begin
         `dut_error(get_abstract_name(), $sformatf("TB ERROR: process_cb_inv_start called for bank %d but cb_inv_sync_check_in_progress is already set.",bank_id))
      end
      m_cb_inv_sync_check_in_progress[bank_id] = 1'b1;

      //if there's no l2 then don't bother
      if (m_set_size == 0) begin
         return;
      end
      
      // Gather up all of the request descriptor that need to be done before the RTL sends back the sync ack.
      // Including...

      //... any WriteArounds in m_req_queues (that aren't already done)
      for (int ii = 0; ii < `SC_REQQ_DEPTH; ii++) begin
         tmp_req_desc = m_req_queues[bank_id][ii];
         if (tmp_req_desc != null) begin
            if (tmp_req_desc.have_int_param(pk_etlink_cmd) == 1'b1) begin
               bus_cmd = evl_bus_cmd_t'(tmp_req_desc.get_int_param(pk_etlink_cmd));
               etlink_cmd = evl_etlink_req_opcode_t'(bus_cmd);
               if (etlink_cmd == ET_LINK_REQ_WriteAround) begin
                  bit has_axi_progeny;
                  bit waiting_for_progeny_response;
                  int child_index;
                  evl_req_desc tmp_child_desc;
                  `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d thinking about adding this WriteAround to m_reqs_ahead_of_cb_inv", bank_id))
                  //if this req_desc already has a child desc of type AXI, and that already has a response, then we don't
                  // need to add it to m_reqs_ahead_of_cb_inv.
                  has_axi_progeny = 1'b0;
                  waiting_for_progeny_response = 1'b0;
                  
                  child_index = 0;
                  do begin
                     tmp_child_desc = tmp_req_desc.get_req_desc(child_index);
                     if (tmp_child_desc != null) begin
                        `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_child_desc, $sformatf("process_cb_inv_start found this child desc. bus_type = %0p rsp_valid = %0p %0s",tmp_child_desc.get_bus_type(),tmp_child_desc.bus_rsp_is_valid(0),tmp_child_desc.sprint_obj()))
                        child_index++;
                        if (tmp_child_desc.get_bus_type() == EVL_BUS_TYPE_AXI) begin
                           has_axi_progeny = 1'b1;
                           if (tmp_child_desc.bus_rsp_is_valid(0) == 0) begin
                              waiting_for_progeny_response = 1'b1;
                           end
                        end
                        // Another case: The original request may have a "victim" request descriptor child which is also ETLINK, and an AXI grandchild.
                        else if (tmp_child_desc.get_bus_type() == EVL_BUS_TYPE_ETLINK) begin
                           evl_req_desc tmp_grandchild_desc;
                           
                           tmp_grandchild_desc = tmp_child_desc.get_req_desc(0);
                           if (tmp_grandchild_desc != null) begin
                              `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_grandchild_desc, $sformatf("process_cb_inv_start found this grandchild desc. bus_type = %0p rsp_valid = %0p",tmp_grandchild_desc.get_bus_type(),tmp_grandchild_desc.bus_rsp_is_valid(0)))
                              has_axi_progeny = 1'b1;
                              if (tmp_grandchild_desc.bus_rsp_is_valid(0) == 0) begin
                                 waiting_for_progeny_response = 1'b1;
                              end
                           end
                        end
                     end
                  end
                  while (tmp_child_desc != null);
                  `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv start. has_axi_progeny=%x waiting_for_progeny_response = %x",has_axi_progeny,waiting_for_progeny_response))
                  // if we found a child (or children) and nobody is waiting for a response, then skip putting it in the list
                  if ((has_axi_progeny == 1'b1) && (waiting_for_progeny_response == 1'b0)) begin
                     `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d not adding this WriteAround to m_reqs_ahead_of_cb_inv because it's downstream request and response have already been observed",bank_id))
                  end
                  // If it has no children and it has been though process_tc_req, then it's basically done and doesn't need to be on the list
                  else if ((has_axi_progeny == 1'b0) && (tmp_req_desc.get_int_param(pk_after_tc_req) == 1)) begin
                     `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d not adding this WriteAround to m_reqs_ahead_of_cb_inv because it has been through tc req and no downstream requests were necessary",bank_id))
                  end
                  // If it has no children and has not been through process_tc_req then add it to the list
                  else begin
                     `evl_log_mdesc(UVM_FULL, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d adding this WriteAround to m_reqs_ahead_of_cb_inv. paddr = %x",bank_id,tmp_req_desc.get_paddr()))
                     m_reqs_ahead_of_cb_inv[bank_id].push(tmp_req_desc);
                  end
               end
            end
         end
      end
      //... everything in m_pending_evicts
      for (int jj = 0; jj < `SC_REQQ_DEPTH; jj++) begin
         tmp_req_desc = m_pending_evicts[bank_id][jj];
         if (tmp_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d adding this pending evict to m_reqs_ahead_of_cb_inv",bank_id))
            m_reqs_ahead_of_cb_inv[bank_id].push(tmp_req_desc);
         end
      end

      //... everything in m_pending_cbuf_evicts.
      for (int jj = 0; jj < `SC_REQQ_DEPTH; jj++) begin
         tmp_req_desc = m_pending_cbuf_evicts[bank_id][jj];
         if (tmp_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), tmp_req_desc, $sformatf("process_cb_inv_start bank %d adding this pending evict to m_reqs_ahead_of_cb_inv",bank_id))
            m_reqs_ahead_of_cb_inv[bank_id].push(tmp_req_desc);
         end
      end

      // What about the coalescing buffer? Take a snapshot of it, and when those
      // get evicted add the new evicts to the list.
      if (m_coalescing_buffer.exists(bank_id)) begin
         if (!m_coalescing_buffer_snapshot.exists(bank_id)) begin
            m_coalescing_buffer_snapshot[bank_id] = new($sformatf("%0s(CBUF_S=%0d)", get_root_abstract_name(), bank_id));
         end
         for (int ii = 0; ii < m_coalescing_buffer[bank_id].size(); ii++) begin
            m_coalescing_buffer_snapshot[bank_id].push(m_coalescing_buffer[bank_id].get(ii));
         end
         `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("process_cb_inv_start bank %d created this snapshot of the coalescing buffer:",bank_id))
         if (`evl_log_check_verbosity(UVM_HIGH, VMOD_SC_SB) != 0) begin
            m_coalescing_buffer_snapshot[bank_id].print_obj("coalescing_buffer_snapshot");
         end
      end
      
   endfunction: process_cb_inv_start

   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: cb_inv_process_cb_evict
   //
   //
   // When a coalescing buffer entry that's in our snapshot gets evicted,
   // we need to add the new eviction req_desc to the list of requests that
   // need to finish before the sync goes back to the idxcopsm.
   function void cb_inv_process_cb_evict(input evl_req_desc req_desc);
      evl_sc_bank_id_t bank_id;

      bank_id = req_desc.get_bus_bank();
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("cb_inv_process_cb_evict bank %d called with this req_desc",bank_id))
      if (m_cb_inv_sync_check_in_progress[bank_id] == 1'b1) begin
         if (m_coalescing_buffer_snapshot.exists(bank_id)) begin
            evl_req_desc descs_out[];
            m_coalescing_buffer_snapshot[bank_id].find_all_by_paddr(descs_out,req_desc.get_paddr());
            if (descs_out.size() == 1) begin
               //we found 1 match as expected. remove it.
               m_coalescing_buffer_snapshot[bank_id].pop(descs_out[0]);
               //make sure this new victim is observed before the end of the cb inv sync
               `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("cb_inv_process_cb_evict: adding this to m_reqs_ahead_of_cb_inv"))
               m_reqs_ahead_of_cb_inv[bank_id].push(req_desc);
            end
            else begin
               // there might be no matches and that's okay.
               //`dut_error(get_abstract_name(), $sformatf("cb_inv_process_cb_evict: m_coalescing_buffer_snapshot[%d] has %d items that match addr %x. That's confusing",bank_id,descs_out.size(),req_desc.get_paddr()))
            end
         end // if (m_coalescing_buffer_snapshot.exists(bank_id))
      end
   endfunction: cb_inv_process_cb_evict
   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: cb_inv_process_response
   //
   // When the shire cache gets a response from downstream, remove it from the list
   // that we created in process_cb_inv_start().
   function void cb_inv_process_response(input evl_req_desc req_desc);
      evl_sc_bank_id_t bank_id;
      evl_req_desc tmp_req_desc;
      evl_req_desc parent_req_desc;
      evl_req_desc grand_req_desc;
      bank_id = req_desc.get_bus_bank();
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("cb_inv_process_response (bank %x) called with this req_desc",bank_id))
      if (m_cb_inv_sync_check_in_progress[bank_id] == 1'b1) begin
         m_reqs_ahead_of_cb_inv[bank_id].find_and_remove_by_addr(req_desc.get_paddr());
         // Also, we may have added a WriteAround request to m_reqs_ahead_of_cb_inv and now it's child (actually grandchild)
         // (which was a capacity eviction, so different address) is the response, so use the (grand)parent desc.
         parent_req_desc = req_desc.get_parent_req_desc();
         if (parent_req_desc != null) begin
            grand_req_desc = parent_req_desc.get_parent_req_desc();
            if (grand_req_desc != null) begin
               m_reqs_ahead_of_cb_inv[bank_id].find_and_remove_by_addr(grand_req_desc.get_paddr());
               //m_reqs_ahead_of_cb_inv[bank_id].print_obj("reqs_ahead_of_cb_inv");
            end
         end
      end
   endfunction: cb_inv_process_response
   
   //-----------------------------------------------------------------------------------------------
   //
   // Function: cb_inv_end_checks
   //
   // Checks to to at the end of a cb inv to make sure the rtl waiting for
   // everything that needed to finish.
   //
   function void cb_inv_end_checks(input evl_sc_bank_id_t bank_id);
      `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("cb_inv_end_checks is running for bank %d.",bank_id))
      
      //Make sure reqs_ahead_of_cb_inv is empty.
      if (m_reqs_ahead_of_cb_inv[bank_id] != null) begin
         if (m_reqs_ahead_of_cb_inv[bank_id].size() != 0) begin
            `dut_error(get_abstract_name(), $sformatf("cb_inv_end_checks is expecting m_reqs_ahead_of_cb_inv[%d] to be empty, but it's not:",bank_id))
            m_reqs_ahead_of_cb_inv[bank_id].print_obj("reqs_ahead_of_cb_inv");
         end
      end
      
      //Make sure the coalescing buffer snapshot is empty.
      if (m_coalescing_buffer_snapshot.exists(bank_id)) begin
         if (m_coalescing_buffer_snapshot[bank_id].size() != 0) begin
            `dut_error(get_abstract_name(), $sformatf("cb_inv_end_checks is expecting m_coalescing_buffer_snapshot[%d] to be empty, but it's not:",bank_id))
             m_coalescing_buffer_snapshot[bank_id].print_obj("coalescing_buffer_snapshot");  
         end
      end
      
      m_cb_inv_sync_check_in_progress[bank_id] = 1'b0;
   endfunction: cb_inv_end_checks


   //-----------------------------------------------------------------------------------------------
   //
   // Function: predict_read_coop_rsp
   //
   // This function is called to predict responses for read coop. It is assumed that the input
   // req_desc already has response data in it as if this were a regular read response.
   function void predict_read_coop_rsp(input evl_req_desc req_desc_in);
      evl_bus_neigh_coop_mask_t coop_mask;
      int upstream_req_port;
      evl_req_desc extra_rsp_desc;
      coop_mask = req_desc_in.get_bus_neigh_coop_mask();
      upstream_req_port = req_desc_in.get_int_param(pk_upstream_req_port);
      `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc_in, $sformatf("predict_read_coop_rsp: get_bus_neigh_coop_mask = %x get_bus_req_source = %x upstream_req_port = %x", req_desc_in.get_bus_neigh_coop_mask(),req_desc_in.get_bus_req_source(),req_desc_in.get_int_param(pk_upstream_req_port)))
      for (int ii = 0; ii < `SC_PORTS; ii++) begin
         if (coop_mask[ii] == 1'b1) begin
            if (ii == upstream_req_port) begin
               // Might as well use the req_desc that already exists
               m_pending_coop_rsp[ii].push(req_desc_in);
            end
            else begin
               // The etlink monitor is going to be confused when this one shows up
               // and ask this scoreboard for a req_desc.
               $cast(extra_rsp_desc, m_template_req_desc_et[ii].clone());
               extra_rsp_desc.set_paddr(req_desc_in.get_paddr());
               extra_rsp_desc.set_bus_req_size(req_desc_in.get_bus_req_size());
               extra_rsp_desc.set_bus_cmd(req_desc_in.get_bus_cmd());
               extra_rsp_desc.set_bus_req_source(req_desc_in.get_bus_req_source());
               extra_rsp_desc.set_bus_rsp_dest(req_desc_in.get_bus_req_source());
               extra_rsp_desc.set_bus_rsp_type(req_desc_in.get_bus_rsp_type());
               extra_rsp_desc.set_bus_rsp_valid(0);
               extra_rsp_desc.set_rsp_data(req_desc_in.get_rsp_data());
               extra_rsp_desc.set_bus_req_id(req_desc_in.get_bus_req_id());
               void'(req_desc_in.add_child_desc(extra_rsp_desc));
               //set observed so it will be checked in the etlink monitor
               extra_rsp_desc.set_observed();
               m_pending_coop_rsp[ii].push(extra_rsp_desc);
            end
         end // if (coop_mask[ii] == 1'b1)
         else begin
            // if the mask is 0 and this is the originating port, then mark the req_desc to not expect a response.
            if (ii == upstream_req_port) begin
               req_desc_in.set_abstract_rsp_type(RspNull);
            end
         end // else: !if(coop_mask[ii] == 1'b1)
      end
   endfunction : predict_read_coop_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rtl_event
   //
   function process_rtl_event(evl_shire_cache_packet rtl_event);
      evl_trans_id_t   trid;
      evl_sc_bank_id_t bank_id;
      int              cycle_count;
      evl_sc_packet_u  packet_info;
      evl_sc_event_t   packet_type;

      trid        = rtl_event.get_trans_id();
      bank_id     = rtl_event.get_bank_id();
      packet_info = rtl_event.get_info();
      cycle_count = rtl_event.get_cycle_count();
      packet_type = evl_sc_event_t'(rtl_event.get_packet_type());

      //
      // The EI_Pend state is used solely to allow late hits in the RBuf stage.  An RBuf event is
      // the first event to be observed, so we will transition to I one cycle later for RBuf checks
      // than TC req (or other) checks.
      //
      if (m_delayed_inv_cycle_counts.size() > 0) begin
         int cycle_limit = (packet_type inside { SHIRE_CACHE_RBUF_REQ }) ? cycle_count : cycle_count + 1;
     
         for (int ii = m_delayed_inv_cycle_counts.size() - 1; ii >= 0; ii--) begin
            if ((cycle_count >= m_delayed_inv_cycle_counts[ii]) || ((cycle_limit >= m_delayed_inv_cycle_counts[ii]) && (bank_id == m_delayed_inv_cycle_banks[ii]))) begin
               evl_line_desc line_desc;

               line_desc = m_delayed_inv_line_descs[ii];
               m_delayed_inv_cycle_banks.delete(ii);
               m_delayed_inv_cycle_counts.delete(ii);
               m_delayed_inv_line_descs.delete(ii);
               if (line_desc.get_cache_st() inside { EI_Pend }) begin
                  //
                  // Process the final change of this cache line.
                  //
                  `evl_log_addr_trid_id(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), line_desc.get_paddr(), ~evl_trans_id_t'(0), -1, $sformatf("RBUF model modifying cache state for 0x%x from %p to I (bank %0d)", line_desc.get_paddr(), line_desc.get_cache_st(), bank_id))
                  line_desc.set_cache_st(I);
                  line_desc.invalidate_line_data();
               end
            end
         end
      end

      if (rtl_event.get_packet_type() == SHIRE_CONFIG) begin
         process_shire_config(packet_info.shire_config_info.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_CONFIG) begin
         process_sc_bank_config(packet_info.bank_config_info.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_PIPE_CONFIG) begin
         process_sc_pipe_config(packet_info.pipe_config_info.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_IDX_COP_REQ) begin
         enqueue_idx_cop_request(trid, bank_id, packet_info.idx_cop_req.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_REQQ_ALLOC) begin
         process_reqq_alloc(trid, bank_id, rtl_event.use_remote_shire_id(), rtl_event.get_remote_shire_id(), packet_info.reqq_alloc.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_REQQ_DEALLOC) begin
         process_reqq_dealloc(trid, bank_id, packet_info.reqq_alloc.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_RBUF_REQ) begin
         process_rbuf_request(trid, bank_id, packet_info.pipe_rbuf_req.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_TC_INFO) begin
         m_pipe_info_tc[bank_id] = packet_info.pipe_info_tc.info;
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_TC_REQ) begin
         process_tc_request(trid, bank_id, cycle_count, packet_info.pipe_req_tc.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_TC_TAG_RSP) begin
         process_tc_tag_rsp(trid, bank_id, packet_info.pipe_tag_rsp_tc.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_CBUF_STATUS) begin
         process_cbuf_status(trid, bank_id, packet_info.cbuf_status.info);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_ALL_INV) begin
         process_all_inv(bank_id);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_CB_INV_START) begin
         process_cb_inv_start(bank_id);
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_RSP_REQQ) begin
         process_rsp_reqq_id(bank_id, packet_info.rsp_reqq.info.reqq_id, rtl_event.get_cycle_count());
      end
      else if (rtl_event.get_packet_type() == SHIRE_CACHE_RSP_DSTQ) begin
         process_rsp_dstq_id(bank_id, packet_info.rsp_reqq.info.neigh_id, rtl_event.get_cycle_count());
      end
      else begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: received an unknown RTL event (0x%0x)!", int'(rtl_event.get_packet_type())))
      end
   endfunction : process_rtl_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: final_axi_req_fixup
   //
   virtual function void final_axi_req_fixup(input evl_req_desc axi_req_desc, input evl_abstract_bus_cmd_t abstract_bus_cmd);
      return;
   endfunction: final_axi_req_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: initialize_regs
   //
   virtual function void initialize_regs();
      string reg_field_names_no_check[$];

      // reset each reg model to the initial values
      foreach (m_regs[ii]) begin
         m_regs[ii].reset();
      end

      // cherry-pick some fields we don't want to compare
      // FUTURE: eventually we'll want to model these so we can compare
      reg_field_names_no_check.push_back("SC_ERR_LOG_INFO");
      reg_field_names_no_check.push_back("SC_ERR_LOG_ADDRESS");
//    reg_field_names_no_check.push_back("IDX_COP_SM_STATE");  // this is handled in ral_sc_bank_regs.sv
      reg_field_names_no_check.push_back("SC_PERFMON_CYC_CNTR");
      reg_field_names_no_check.push_back("SC_PERFMON_P0_CNTR");
      reg_field_names_no_check.push_back("SC_PERFMON_P1_CNTR");
      foreach (m_regs[ii]) begin
         foreach (reg_field_names_no_check[jj]) begin
            uvm_reg_field reg_field;

            reg_field = m_regs[ii].get_field_by_name(reg_field_names_no_check[jj]);
            reg_field.set_compare(UVM_NO_CHECK);
         end
      end
      
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_SB) != 0) begin
         foreach (m_regs[ii]) begin
            m_regs[ii].print();
         end
      end

      //test_reg_accesses();
   endfunction: initialize_regs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: test_req_accesses
   //
   virtual function void test_reg_accesses();
      string        reg_name;
      evl_dword_t   reg_inst_data;
      uvm_reg       reg_inst;
      string        reg_field_name;
      uvm_reg_field reg_field;
      evl_dword_t   reg_field_data;
      ral_reg_sc_l3_shire_swizzle_ctl l3_swizzle_reg;

      reg_name = "sc_pipe_ctl";
      reg_inst  = m_regs[0].get_reg_by_name(reg_name);
      reg_inst_data = reg_inst.get();

      reg_field_name = "ESR_SC_NUM_L3_REQQ_ENTRIES";
      reg_field = m_regs[0].get_field_by_name(reg_field_name);
      reg_field_data = reg_field.get();
      reg_inst_data  = m_regs[0].sc_l3_shire_swizzle_ctl.get();
      reg_field_data = m_regs[0].ESR_SC_SHIRE_SEL_B5.get();
   endfunction: test_reg_accesses


   //-----------------------------------------------------------------------------------------------
   //
   // Function: capture_configuration
   //
   function void capture_configuration();
      string        reg_name;
      evl_dword_t   reg_inst_data;
      uvm_reg       reg_inst;
      string        reg_field_name;
      uvm_reg_field reg_field;
      int           scp_size;
      int           l2_size;
      int           l3_size;

      scp_size = -1;
      l2_size  = -1;
      l3_size  = -1;
      `ifdef ET_NO_SC_COVERAGE
         if (`et_test_plusargs("sc_cover_on=1") != 0) begin
            m_sc_config_cover = new();
         end
      `else // ifndef ET_NO_SC_COVERAGE
         m_sc_config_cover = new();
      `endif // !ifndef ET_NO_SC_COVERAGE

      if (m_global_attrs.get_int_param(pk_random_sc_test, 0) <= 0) begin
         return;
      end

      if ((m_regs.size() > 0) && m_regs.exists(0)) begin
         reg_inst = m_regs[0].get_reg_by_name("sc_scp_cache_ctl");
         if (reg_inst != null) begin
            reg_field = reg_inst.get_field_by_name("SET_SIZE");
            if (reg_field != null) begin
               scp_size = int'(reg_field.get());
            end
         end

         reg_inst = m_regs[0].get_reg_by_name("sc_l2_cache_ctl");
         if (reg_inst != null) begin
            reg_field = reg_inst.get_field_by_name("SET_SIZE");
            if (reg_field != null) begin
               l2_size = int'(reg_field.get());
            end
         end

         reg_inst = m_regs[0].get_reg_by_name("sc_l3_cache_ctl");
         if (reg_inst != null) begin
            reg_field = reg_inst.get_field_by_name("SET_SIZE");
            if (reg_field != null) begin
               l3_size = int'(reg_field.get());
            end
         end

         if (m_sc_config_cover != null) begin
            if ((scp_size >= 0) && (l2_size >= 0) && (l3_size >= 0)) begin
               m_sc_config_cover.sample_sc_mode(scp_size, l2_size, l3_size);
            end

            reg_inst = m_regs[0].get_reg_by_name("sc_l3_shire_swizzle_ctl");
            if (reg_inst != null) begin
               reg_inst_data = evl_dword_t'(reg_inst.get());
               m_sc_config_cover.sample_sc_swizzle_mode(reg_inst_data);
            end

            foreach (m_regs[ii]) begin
               reg_inst = m_regs[ii].get_reg_by_name("sc_reqq_ctl");
               reg_inst = m_regs[0].get_reg_by_name("sc_reqq_ctl");
               if (reg_inst != null) begin
                  reg_inst_data = evl_dword_t'(reg_inst.get());
                  m_sc_config_cover.sample_sc_reqq_ctl(reg_inst_data);
               end
            end

            foreach (m_regs[ii]) begin
               reg_inst = m_regs[ii].get_reg_by_name("sc_pipe_ctl");
               reg_inst = m_regs[0].get_reg_by_name("sc_pipe_ctl");
               if (reg_inst != null) begin
                  reg_inst_data = evl_dword_t'(reg_inst.get());
                  m_sc_config_cover.sample_sc_pipe_ctl(reg_inst_data);
               end
            end
         end
      end
      return;
   endfunction : capture_configuration


   //-----------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_shire_cache_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "TB_FAIL: m_rtl_port is null, SAD!")
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      foreach (m_template_req_data_et[ii]) begin
         m_template_req_data_et[ii]   = evl_line_data::type_id::create($sformatf("S%0d:NEIGH-%0d(ETLINK)", get_shire_id(), ii));
         m_template_line_desc_et[ii]  = evl_line_desc::type_id::create($sformatf("S%0d:NEIGH-%0d(ETLINK)", get_shire_id(), ii));
         m_template_trans_desc_et[ii] = evl_trans_desc::type_id::create($sformatf("S%0d:NEIGH-%0d(ETLINK)", get_shire_id(), ii));
         m_template_req_desc_et[ii]   = evl_req_desc::type_id::create($sformatf("S%0d:NEIGH-%0d(ETLINK)", get_shire_id(), ii));

         m_template_trans_desc_et[ii].set_shire_id(get_shire_id());
         m_template_trans_desc_et[ii].set_neighborhood_id(ii);

         m_template_req_desc_et[ii].set_shire_id(get_shire_id());
         m_template_req_desc_et[ii].set_neighborhood_id(ii);

         m_template_line_desc_et[ii].set_data_template(m_template_req_data_et[ii]);
         m_template_trans_desc_et[ii].set_data_template(m_template_req_data_et[ii]);
         m_template_req_desc_et[ii].set_data_template(m_template_req_data_et[ii]);
         m_template_req_desc_et[ii].set_bus_type(EVL_BUS_TYPE_ETLINK);

         m_template_trans_desc_et[ii].set_line_template(m_template_line_desc_et[ii]);
         m_template_req_desc_et[ii].set_line_template(m_template_line_desc_et[ii]);
      end

      m_template_req_data_axi   = evl_line_data::type_id::create($sformatf("S%0d:L3_MESH(AXI)", get_shire_id()));
      m_template_line_desc_axi  = evl_line_desc::type_id::create($sformatf("S%0d:L3_MESH(AXI)", get_shire_id()));
      m_template_trans_desc_axi = evl_trans_desc::type_id::create($sformatf("S%0d:L3_MESH(AXI)", get_shire_id()));
      m_template_req_desc_axi = evl_req_desc::type_id::create($sformatf("S%0d:L3_MESH(AXI)", get_shire_id()));

      m_template_trans_desc_axi.set_shire_id(get_shire_id());
      m_template_req_desc_axi.set_shire_id(get_shire_id());

      m_template_line_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_trans_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_req_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_req_desc_axi.set_bus_type(EVL_BUS_TYPE_AXI);

      m_template_trans_desc_axi.set_line_template(m_template_line_desc_axi);
      m_template_req_desc_axi.set_line_template(m_template_line_desc_axi);

      m_bus_parser_et = evl_bus_parser::get_bus_parser(EVL_BUS_TYPE_ETLINK);

      foreach (m_pending_message_queues[ii]) begin
         m_pending_message_queues[ii] = new($sformatf("%0s(MSG-%0d)", get_root_abstract_name(), ii));
      end
      foreach (m_pending_coop_rsp[ii]) begin
         m_pending_coop_rsp[ii] = new($sformatf("%0s(COOP-%0d)", get_root_abstract_name(), ii));
      end

      m_cache.m_shire_cache_scoreboard = this;
      
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         // this is a task from the base evl_verif_comp that checks anything on the
         // m_active_mem_descs list for timeouts. The Scoreboard is going to add and remove
         // things from that list.
         super.run_phase(phase);
         watch_for_timeouts();
      join_none;
      initialize_regs();
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      fork
         begin
            super.post_configure_phase(phase);
         end
         begin
            serialize_print.get(1);
            if (m_bypass == 1'b1) begin
               `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("bypass:          1"))
            end
            if (m_num_ways > 0) begin
               `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("number of ways:  %0d", m_num_ways))
            end
            `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("set size:        %0dK", m_set_size / 1024))
            `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("memory size:     %0dK", m_mem_size / 1024))
            `evl_log(UVM_LOW, VMOD_SC_SB, get_abstract_name(), $sformatf("index mask:      0x%x", m_index_mask))
            `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("remote_enable:   0x%x", m_remote_enable))
            `evl_log(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), $sformatf("remote_l3_enable:   0x%x", m_remote_l3_enable))
            serialize_print.put(1);
         end
      join
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.post_shutdown_phase(phase);
         end
         begin
            if (m_shire_cache_rtl_port != null) begin
               m_shire_cache_rtl_port.send_final_state();
               wait_cycles(2);
            end
         end
      join
      phase.drop_objection(this);
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: check_phase
   //
   virtual function void check_phase(uvm_phase phase);
      int            line_list_size;
      evl_line_desc  line_list[];
      evl_req_desc   cbuf_req_descs[$];
      evl_paddr_t    addr;
      evl_cache_st_t cache_st;

      super.check_phase(phase);
      m_shire_cache_reqq.print_active_reqq_entries();
      if (m_global_attrs.get_error_count() > 0) begin
         return;
      end
      line_list_size = m_cache.get_valid_line_list(line_list);

      //
      // Gather all of the coalescing buffer request descriptors and addresses that are currently
      // modeled in the scoreboard.
      //
      foreach (m_coalescing_buffer[bank_id]) begin
         evl_req_desc req_desc;

         req_desc = m_coalescing_buffer[bank_id].pop();
         while (req_desc != null) begin
            cbuf_req_descs.push_back(req_desc);
            m_mdl_cbuf_addr_queues[bank_id].push_back((req_desc.get_paddr() & const_line_paddr_mask));
            req_desc = m_coalescing_buffer[bank_id].pop();
         end
      end

      //
      // First, compare RTL coalescing buffer addresses to those in that are modeled in the
      // scoreboard.  If there is an address in RTL that is not modeled, flag an error.  If there is
      // a match, delete the model address (so we can see if there are any left over when we are
      // done).
      //
      for (int bank_id = 0; bank_id < `SC_BANKS; bank_id++) begin
         bit have_data;

         for (int ii = m_rtl_cbuf_addr_queues[bank_id].size() - 1; ii >= 0; ii--) begin
            have_data = 1'b0;
            for (int jj = m_mdl_cbuf_addr_queues[bank_id].size() - 1; jj >= 0; jj--) begin
               if (m_rtl_cbuf_addr_queues[bank_id][ii] == m_mdl_cbuf_addr_queues[bank_id][jj]) begin
                  have_data = 1'b1;
                  m_mdl_cbuf_addr_queues[bank_id].delete(jj);
                  break;
               end
            end
            if (have_data == 1'b0) begin
               `dut_error(get_abstract_name(), $sformatf("RTL has an unexpected address in the coalescing buffer for bank %0d (0x%x)", bank_id, m_rtl_cbuf_addr_queues[bank_id][ii]), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
      end

      //
      // Now see if we have any modeled coalescing buffer addresses that are not found in RTL.
      //
      for (int bank_id = 0; bank_id < `SC_BANKS; bank_id++) begin
         for (int ii = m_mdl_cbuf_addr_queues[bank_id].size() - 1; ii >= 0; ii--) begin
            `dut_error(get_abstract_name(), $sformatf("RTL does not have an expected address in the coalescing buffer for bank %0d (0x%x)", bank_id, m_mdl_cbuf_addr_queues[bank_id][ii]), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
         end
      end

      for (int ii = 0; ii < line_list_size; ii++) begin
         addr     = line_list[ii].get_paddr();
         cache_st = line_list[ii].get_cache_st();
         if (cache_st inside { `EVL_LINE_WA_DIRTY_STATES }) begin
            bit found_match = 1'b0;

            for (int jj = cbuf_req_descs.size() - 1; jj >= 0; jj--) begin
               if (cbuf_req_descs[jj].line_addr_match(addr) == 1'b1) begin
                  found_match = 1'b1;
                  cbuf_req_descs.delete(jj);
                  break;
               end
            end
            if (found_match == 1'b0) begin
               `dut_error(get_abstract_name(), $sformatf("cache line for 0x%x has a state of %p, but there is no coalescing buffer entry for it", addr, cache_st), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
            end
         end
      end
      for (int jj = cbuf_req_descs.size() - 1; jj >= 0; jj--) begin
         `dut_error_mdesc(get_abstract_name(), cbuf_req_descs[jj], $sformatf("%0s is in the coalescing buffer, but its state is %p", cbuf_req_descs[jj].sprint_obj(), cbuf_req_descs[jj].get_cache_st()), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_mod })
      end

      // The pending ReadCoop response queues should be empty
      foreach (m_pending_coop_rsp[ii]) begin
         if (m_pending_coop_rsp[ii].size() != 0) begin
            `dut_error(get_abstract_name(), $sformatf("expecting m_pending_coop_rsp to be empty but there are responses waiting to go to port %x",ii))
            m_pending_coop_rsp[ii].print_obj("pending_coop_rsp");
         end
      end

      // check for empty: m_req_queues[bank_id][reqq_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         for (int jj = 0; jj < `SC_REQQ_DEPTH; jj++) begin
            if (m_req_queues[ii][jj] != null) begin
               `dut_error_mdesc(get_abstract_name(), m_req_queues[ii][jj], $sformatf(" %s is still left in m_req_queues[%0d][%0d] during check_phase()",m_req_queues[ii][jj].sprint_obj(),ii,jj))
            end
         end
      end

      // check for empty: m_pending_evicts[bank_id][reqq_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         for (int jj = 0; jj < `SC_REQQ_DEPTH; jj++) begin
            if (m_pending_evicts[ii][jj] != null) begin
               `dut_error_mdesc(get_abstract_name(), m_pending_evicts[ii][jj], $sformatf(" %s is still left in m_pending_evicts[%0d][%0d] during check_phase()",m_pending_evicts[ii][jj].sprint_obj(),ii,jj))
            end
         end
      end

      // check for empty: m_pending_fills[bank_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         foreach (m_pending_fills[ii][jj]) begin
            `dut_error_mdesc(get_abstract_name(), m_pending_fills[ii][jj], $sformatf(" %s is still left in m_pending_fills[%0d][%0d] during check_phase()",m_pending_fills[ii][jj].sprint_obj(),ii,jj))
         end
      end

      // check for empty: m_pending_downstream_rsp[bank_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         foreach (m_pending_downstream_rsp[ii][jj]) begin
            `dut_error_mdesc(get_abstract_name(), m_pending_downstream_rsp[ii][jj], $sformatf(" %s is still left in m_pending_downstream_rsp[%0d][%0d] during check_phase()",m_pending_downstream_rsp[ii][jj].sprint_obj(),ii,jj))
         end
      end

      // check for empty: m_pending_cbuf_evicts[bank_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         foreach (m_pending_cbuf_evicts[ii][jj]) begin
            `dut_error_mdesc(get_abstract_name(), m_pending_cbuf_evicts[ii][jj], $sformatf(" %s is still left in m_pending_cbuf_evicts[%0d][%0d] during check_phase()",m_pending_cbuf_evicts[ii][jj].sprint_obj(),ii,jj))
         end
      end

      // check for empty: m_pending_atomics[bank_id]
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         foreach (m_pending_atomics[ii][jj]) begin
            `dut_error_mdesc(get_abstract_name(), m_pending_atomics[ii][jj], $sformatf(" %s is still left in m_pending_atomics[%0d][%0d] during check_phase()",m_pending_atomics[ii][jj].sprint_obj(),ii,jj))
         end
      end

      // The pending message queues should be empty
      foreach (m_pending_message_queues[ii]) begin
         if (m_pending_message_queues[ii].size() != 0) begin
            `dut_error(get_abstract_name(), $sformatf("expecting m_pending_message_queues to be empty but there are responses waiting to go in queue %0d",ii))
            m_pending_message_queues[ii].print_obj();
         end
      end

      // The m_tag_rsp_queue should be empty
      foreach (m_tag_rsp_queue[ii]) begin
         `dut_error_mdesc(get_abstract_name(), m_tag_rsp_queue[ii], $sformatf(" %s is still left in m_tag_rsp_queue during check_phase()",m_tag_rsp_queue[ii].sprint_obj()))
      end

      m_shire_cache_axi_meshq.check_for_empty();
   endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("SHIRE_CACHE(SB)");
      if (`et_test_plusargs("disableSCSBChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_sb_error_intensity, UVM_HIGH);
      end
      if (`et_test_plusargs("enableSCSBChecks") != 0) begin
         `evl_set_error_intensity(pk_sc_sb_error_intensity, -1);
      end
      for (int ii = 0; ii < (`SC_BANKS << 1); ii++) begin
         m_coalesce_size[ii] = 0;
      end
      m_cbuf_enable = {`SC_BANKS{1'b1}};
      for (int ii = 0; ii < 8; ii++) begin
         m_neigh_id[ii] = ii;
      end
      for (int ii = 0; ii < `SC_BANKS; ii++) begin
         m_rsp_queues[ii] = new($sformatf("%0s(RSPQ-%0d)", get_root_abstract_name(), ii));
      end

      `ifdef ET_NO_SC_COVERAGE
         if (`et_test_plusargs("sc_cover_on=1") != 0) begin
            m_shire_cache_scoreboard_cover = new();
         end
      `else // ifdef ET_NO_SC_COVERAGE
         m_shire_cache_scoreboard_cover = new();
      `endif // !ifdef ET_NO_SC_COVERAGE
   endfunction : new

endclass : evl_shire_cache_scoreboard

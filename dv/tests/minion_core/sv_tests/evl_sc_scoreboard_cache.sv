//--------------------------------------------------------------------------------------------------
//
// Class: evl_sc_scoreboard_cache
//
class evl_sc_scoreboard_cache #(type SCOREBOARD_TYPE = evl_shire_cache_scoreboard) extends evl_cache;

   `uvm_component_param_utils(evl_sc_scoreboard_cache #(SCOREBOARD_TYPE))


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_reqq_index)
   `evl_param_key(pk_sc_sb_error_intensity)

   // Pointer back to its parent shire cache scoreboard
   evl_shire_cache_scoreboard  #(SCOREBOARD_TYPE) m_shire_cache_scoreboard;

   // Coverage
   evl_sc_scoreboard_cache_cover m_sc_scoreboard_cache_cover;
   
   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_table_wrapper #(SCOREBOARD_TYPE, "IntReq") m_int_req_table;
   evl_table_wrapper #(SCOREBOARD_TYPE, "BusRsp") m_bus_rsp_table;
   evl_table_desc                                 m_table_desc_null;
   evl_req_desc                                   m_etlink_req_desc;
   evl_req_desc                                   m_axi_req_desc;
   evl_line_desc                                  m_almost_null_line_desc;
   evl_verbosity_mod_t                            m_verbosity_mod      = VMOD_SC_SB;
   evl_verbosity_mod_t                            m_verbosity_warn_mod = VMOD_SC_SB_WARN;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Personalization
   //
   evl_paddr_t m_way_index_mask  = 0;
   evl_paddr_t m_way_select_mask = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_verbosity_mods
   //
   function void set_verbosity_mods(input evl_verbosity_mod_t verbosity_mod_in, input evl_verbosity_mod_t verbosity_warn_mod_in);
      m_verbosity_mod      = verbosity_mod_in;
      m_verbosity_warn_mod = verbosity_warn_mod_in;
   endfunction : set_verbosity_mods


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_coverage
   //
   function void enable_coverage();
      if (m_int_req_table == null) begin
         m_int_req_table = new($sformatf("%0s(%0s-IntReq)", get_root_abstract_name(), m_cache_type));
      end
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new($sformatf("%0s(%0s-BusRsp)", get_root_abstract_name(), m_cache_type));
      end
      `ifndef ET_NO_SC_COVERAGE
         m_int_req_table.enable_coverage();
         m_bus_rsp_table.enable_coverage();
      `endif
   endfunction : enable_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_coverage
   //
   function void build_coverage();
      m_int_req_table.build_coverage();
      m_bus_rsp_table.build_coverage();
   endfunction : build_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_int_req_index
   //
   function longint gen_int_req_index(input evl_sc_sub_table_t   sub_table_in,
                                      input evl_bus_cmd_t        bus_cmd_in,
                                      input evl_sc_reqq_opcode_t opcode_in,
                                      input evl_cache_st_t       obs_st_in,
                                      input evl_cache_st_t       model_st_in,
                                      input evl_sc_qual_t        qualifier_in);
      return longint'({qualifier_in, model_st_in, obs_st_in, opcode_in, bus_cmd_in, sub_table_in});
   endfunction : gen_int_req_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_int_req_name
   //
   function string gen_int_req_name(input longint index_in);
      bit [($bits(evl_sc_sub_table_t)-1):0]   raw_sub_table;
      evl_bus_cmd_t                           raw_bus_cmd;
      bit [($bits(evl_sc_reqq_opcode_t)-1):0] raw_opcode;
      bit [($bits(evl_cache_st_t)-1):0]       raw_obs_st;
      bit [($bits(evl_cache_st_t)-1):0]       raw_model_st;
      evl_sc_qual_t                           qualifier;
      evl_sc_sub_table_t                      sub_table;
      evl_etlink_req_opcode_t                 bus_cmd;
      evl_sc_reqq_opcode_t                    opcode;
      evl_cache_st_t                          obs_st;
      evl_cache_st_t                          model_st;

      { qualifier, raw_model_st, raw_obs_st, raw_opcode, raw_bus_cmd, raw_sub_table } = index_in;
      sub_table = evl_sc_sub_table_t'(raw_sub_table);
      bus_cmd   = evl_etlink_req_opcode_t'(raw_bus_cmd);
      opcode    = evl_sc_reqq_opcode_t'(raw_opcode);
      obs_st    = evl_cache_st_t'(raw_obs_st);
      model_st  = evl_cache_st_t'(raw_model_st);
      return $sformatf("IntReq:%0s:%p:%p:%p:%p:%p:0x%x", m_cache_type, sub_table, bus_cmd, opcode, obs_st, model_st, qualifier);
   endfunction : gen_int_req_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_int_req_act
   //
   function void add_int_req_act(input evl_sc_sub_table_t      sub_table,
                                 input evl_etlink_req_opcode_t etlink_cmd_in,
                                 input evl_sc_reqq_opcode_t    opcode_in,
                                 input evl_cache_st_t          obs_st_in,
                                 input evl_cache_st_t          model_st_list_in[],
                                 input evl_sc_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_bus_cmd_t           bus_cmd_in,
                                 input evl_bus_rsp_type_t      bus_rsp_in,
                                 input int                     err_code_in,
                                 input bit                     no_cover_in = 1'b0);
      longint        index;
      evl_bus_cmd_t  etlink_cmd = evl_bus_cmd_t'(etlink_cmd_in);
      evl_cache_st_t model_st_list[$];
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_cmd(bus_cmd_in);
      table_desc.set_bus_rsp(bus_rsp_in);
      table_desc.set_err_code(err_code_in);

      if (model_st_list_in.size() == 0) begin
         model_st_list.push_back(St_Ignore);
      end
      else begin
         foreach (model_st_list_in[ii]) begin
            model_st_list.push_back(model_st_list_in[ii]);
         end
      end

      if (m_int_req_table == null) begin
         m_int_req_table = new($sformatf("%0s(%0s-IntReq)", get_root_abstract_name(), m_cache_type));
      end

      foreach (model_st_list[ii]) begin
         index = gen_int_req_index(sub_table, etlink_cmd, opcode_in, obs_st_in, model_st_list[ii], qualifier_in);
         m_int_req_table.add_table_entry(index, table_desc, gen_int_req_name(index), ~no_cover_in);
      end
   endfunction : add_int_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_index
   //
   function longint gen_bus_rsp_index(input evl_bus_cmd_t      etlink_cmd_in,
                                      input evl_bus_cmd_t      axi_bus_cmd_in,
                                      input evl_cache_st_t     model_st_in,
                                      input evl_bus_rsp_type_t axi_bus_rsp_in,
                                      input evl_sc_qual_t      qualifier_in);
      return longint'({ qualifier_in, evl_bus_rsp_type_t'(axi_bus_rsp_in), model_st_in, axi_bus_cmd_in, etlink_cmd_in });
   endfunction : gen_bus_rsp_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_name
   //
   function string gen_bus_rsp_name(input longint index_in);
      evl_bus_cmd_t                     raw_etlink_cmd;
      evl_bus_cmd_t                     raw_axi_bus_cmd;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      evl_bus_rsp_type_t                raw_bus_rsp;
      evl_sc_qual_t                     qualifier;
      evl_etlink_req_opcode_t           etlink_cmd;
      evl_axi_req_type_t                axi_bus_cmd;
      evl_cache_st_t                    model_st;
      evl_axi_resp_t                    axi_bus_rsp;

      { qualifier, raw_bus_rsp, raw_model_st, raw_axi_bus_cmd, raw_etlink_cmd } = index_in;
      etlink_cmd  = evl_etlink_req_opcode_t'(raw_etlink_cmd);
      axi_bus_cmd = evl_axi_req_type_t'(raw_axi_bus_cmd);
      model_st    = evl_cache_st_t'(raw_model_st);
      axi_bus_rsp = evl_axi_resp_t'(raw_bus_rsp);
      return $sformatf("BusRsp:%0s:%p:%p:%p:%p:0x%x", m_cache_type, etlink_cmd, axi_bus_cmd, model_st, axi_bus_rsp, qualifier);
   endfunction : gen_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_etlink_req_opcode_t etlink_cmd_in,
                                 input evl_axi_req_type_t      axi_bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_resp_t          axi_bus_rsp_in,
                                 input evl_sc_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_bus_rsp_type_t      bus_rsp_in,
                                 input evl_bus_cmd_t           bus_cmd_in,
                                 input int                     err_code_in,
                                 input bit                     no_cover_in = 1'b0);
      longint        index;
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_rsp(bus_rsp_in);
      table_desc.set_bus_cmd(bus_cmd_in);
      table_desc.set_err_code(err_code_in);

      index = gen_bus_rsp_index(evl_bus_cmd_t'(etlink_cmd_in), evl_bus_cmd_t'(axi_bus_cmd_in), model_st_in, evl_bus_rsp_type_t'(axi_bus_rsp_in), qualifier_in);
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new($sformatf("%0s(%0s-BusRsp)", get_root_abstract_name(), m_cache_type));
      end
      m_bus_rsp_table.add_table_entry(index, table_desc, gen_bus_rsp_name(index), ~no_cover_in);
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: int_req_lookup
   //
   // Hierarchy of lookups:
   //
   // - { bus_cmd,       l2_tc_opcode, obs_st, model_st,  qualifier }
   // - { bus_cmd,       l2_tc_opcode, obs_st, St_Ignore, qualifier }
   // - { `EVL_Bus_Idle, l2_tc_opcode, obs_st, model_st,  qualifier }
   //
   virtual function void int_req_lookup(output evl_table_desc       table_desc,
                                        output evl_line_desc        line_desc,
                                        input  evl_sc_sub_table_t   sub_table,
                                        input  evl_bus_cmd_t        bus_cmd_in,
                                        input  evl_sc_reqq_opcode_t opcode_in,
                                        input  evl_cache_st_t       obs_st_in,
                                        input  evl_req_desc         req_desc_in,
                                        input  evl_sc_qual_t        qualifier_in,
                                        input  evl_line_qword_en_t  qwen_in,
                                        input  string               debug_msg     = "TC stage");
      evl_bus_cmd_t bus_cmd = bus_cmd_in;
      evl_sc_qual_t qualifier;
      int           child_index;
      evl_paddr_t   paddr;
      qualifier = qualifier_in;

      paddr = req_desc_in.get_paddr();
      if (has_sub_qualifier(qualifier, `SCQ_UseSrcAddr)) begin
         evl_req_desc src_req_desc;

         src_req_desc = req_desc_in.get_parent_req_desc();
         if (src_req_desc == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("Expecting src_req_desc to not be null for this request with qualifier SCQ_UseSrcAddr"))
            line_desc = get_line_desc(req_desc_in.get_paddr());
         end
         else begin
            paddr = src_req_desc.get_paddr();
            line_desc = get_line_desc(src_req_desc.get_paddr());
         end
      end // if (has_sub_qualifier(qualifier, `SCQ_UseSrcAddr))
      else if (has_sub_qualifier(qualifier, `SCQ_IdxAddr)) begin
         line_desc = get_line_desc_by_set_way(m_shire_cache_scoreboard.convert_index_cacheop_address(req_desc_in.get_paddr()), m_shire_cache_scoreboard.get_index_cacheop_way(req_desc_in.get_paddr()),0,0,1);
         qualifier = qualifier & (~`SCQ_IdxAddr);
         // If no line_desc was found, use an empty line_desc that has cache_st=I to keep the rest of the code happy.
         if (line_desc == null) begin
            line_desc = m_almost_null_line_desc;
         end
      end
      else begin
         line_desc = get_line_desc(req_desc_in.get_paddr());
      end
      req_desc_in.set_line_desc(line_desc);
      if (m_int_req_table == null) begin
         table_desc = m_table_desc_null;
         return;
      end      
      
      if (has_sub_qualifier(qualifier_in, `SCQ_CheckPartial)) begin
         qualifier = qualifier & (~`SCQ_CheckPartial);
         if ((line_desc.get_cache_st() inside { `EVL_LINE_DIRTY_STATES, `EVL_LINE_WA_DIRTY_STATES }) && (line_desc.get_modified_qword_en() != ~evl_line_qword_en_t'(0))) begin
            qualifier = qualifier | `SCQ_Partial;
         end
      end
      else if (has_sub_qualifier(qualifier_in, `SCQ_MergePartial)) begin
         if (m_sc_scoreboard_cache_cover != null) begin
            m_sc_scoreboard_cache_cover.sample_writearound_partial_qwens(qwen_in,line_desc.get_modified_qword_en());
         end
         if ((qwen_in | line_desc.get_modified_qword_en()) == ~evl_line_qword_en_t'(0)) begin
            qualifier = qualifier & (~`SCQ_MergePartial);
         end
         else begin
            qualifier = qualifier & (~`SCQ_MergePartial) | `SCQ_Partial;
         end
      end
      if (has_sub_qualifier(qualifier_in, `SCQ_CheckSize)) begin
         evl_paddr_t max_size_mask = (1'b1 << (`SC_SCP_SET_MAX_MSB + 1)) - 1;
         evl_paddr_t addr = paddr & const_line_paddr_mask & max_size_mask;

         qualifier = qualifier & (~`SCQ_CheckSize);
         // Is this an out of bounds address?
         if ((m_use_base_and_top == 1'b1) && ((addr < m_base_addr) || (addr > m_top_addr))) begin
            qualifier = qualifier | `SCQ_DecErr;
         end
      end

      // If Locking this way would cause all ways to be locked, let the table know.
      if (has_sub_qualifier(qualifier_in, `SCQ_CheckLock)) begin
         evl_line_desc tmp_line_desc;
         int           num_ways_locked;

         num_ways_locked = 0;
         //remove the CheckPartial qualifier
         qualifier = qualifier & (~`SCQ_CheckLock);
         //Only bother checking if the current state of this line is not a lock state
         if (!(line_desc.get_cache_st() inside {`EVL_LINE_LOCK_STATES})) begin
            for (int ii = 0; ii < m_num_ways; ii++) begin
               tmp_line_desc = get_line_desc_by_set_way(req_desc_in.get_paddr(), evl_way_t'(ii),1'b0,1'b0);
               if (tmp_line_desc != null) begin
                  if (tmp_line_desc.get_cache_st() inside {`EVL_LINE_LOCK_STATES}) begin
                     num_ways_locked++;
                  end
               end
            end // for (int ii = 0; ii < m_num_ways; ii++)
            if (num_ways_locked == (m_num_ways - 1)) begin
               qualifier = qualifier | `SCQ_AllLocked;
            end
         end // if (!(line_desc.get_cache_st() inside {`EVL_LINE_LOCK_STATES}))
      end // if (has_sub_qualifier(qualifier_in, `SCQ_CheckLock))

      table_desc = m_int_req_table.get_table_entry(gen_int_req_index(sub_table, bus_cmd, opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier));
                   
      if (table_desc == null) begin
         table_desc = m_int_req_table.get_table_entry(gen_int_req_index(sub_table, bus_cmd, opcode_in, obs_st_in, St_Ignore, qualifier));
         if ((table_desc == null) && (bus_cmd != `EVL_Bus_Idle)) begin
            table_desc = m_int_req_table.get_table_entry(gen_int_req_index(sub_table, `EVL_Bus_Idle, opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier));
         end
      end

      if (`evl_log_check_verbosity(UVM_HIGH, VMOD_SC_SB) != 0) begin
         if (table_desc != null) begin
            evl_abstract_bus_cmd_t  new_bus_cmd;
            evl_abstract_rsp_type_t new_bus_rsp;
            evl_error_code          err_code;

            new_bus_cmd = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
            new_bus_rsp = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
            err_code    = evl_error_code'(table_desc.get_err_code());
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc_in, pk_reqq_index, $sformatf("internal lookup for 0x%x from %0s: %0s, %p, %p, %p, %s ... returned %p, %p, %p, %p, %p %s", req_desc_in.get_paddr(), debug_msg, m_etlink_req_desc.get_bus_cmd_name(bus_cmd), opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier_string(qualifier), table_desc.get_tag_action(), table_desc.get_data_action(), new_bus_cmd, new_bus_rsp, err_code, req_desc_in.sprint_obj()))
         end
         else begin
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc_in, pk_reqq_index, $sformatf("internal lookup for 0x%x from %0s: %0s, %p, %p, %p, %s ... no table entry found %s", req_desc_in.get_paddr(), debug_msg, m_etlink_req_desc.get_bus_cmd_name(bus_cmd), opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier_string(qualifier), req_desc_in.sprint_obj()))
         end
      end

      if (table_desc != null) begin
         if (m_global_attrs.coverage_enabled() == 1'b1) begin
            table_desc.sample();
         end
      end
      else begin
         if (bus_cmd == `EVL_Bus_Idle) begin
            `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("internal lookup for 0x%x from %0s: EVL_Bus_Idle, %p, %p, %p, %s ... no table entry", req_desc_in.get_paddr(), debug_msg, opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier_string(qualifier)), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_warn_mod })
         end
         else begin
            `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("internal lookup for 0x%x from %0s: %0s, %p, %p, %p, %s ... no table entry", req_desc_in.get_paddr(), debug_msg, m_etlink_req_desc.get_bus_cmd_name(bus_cmd), opcode_in, obs_st_in, line_desc.get_cache_st(), qualifier_string(qualifier)), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_warn_mod })
         end
         table_desc = m_table_desc_null;
      end
   endfunction : int_req_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_lookup
   //
   virtual function void bus_rsp_lookup(output evl_table_desc      table_desc,
                                        output evl_line_desc       line_desc,
                                        input  evl_req_desc        req_desc_in,
                                        input  evl_bus_cmd_t       etlink_cmd_in,
                                        input  evl_bus_cmd_t       axi_bus_cmd_in,
                                        input  evl_bus_rsp_type_t  bus_rsp_in,
                                        input  evl_sc_qual_t       qualifier_in);
      evl_cache_st_t cache_st;

      line_desc = req_desc_in.get_line_desc();
      if (line_desc == null) begin
         line_desc = get_line_desc(req_desc_in.get_paddr());
      end
      if (line_desc == null) begin
         return;
      end

      if (has_sub_qualifier(qualifier_in, `SCQ_Stale)) begin
         cache_st = Stale;
      end
      else begin
         cache_st = line_desc.get_cache_st();
      end

      table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(etlink_cmd_in, axi_bus_cmd_in, cache_st, bus_rsp_in, qualifier_in));
      if (table_desc != null) begin
         if (`evl_log_check_verbosity(UVM_HIGH, VMOD_SC_SB) != 0) begin
            evl_abstract_rsp_type_t new_bus_rsp;
            evl_abstract_bus_cmd_t  new_bus_cmd;
            evl_error_code          err_code;

            new_bus_rsp = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
            new_bus_cmd = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
            err_code    = evl_error_code'(table_desc.get_err_code());
            `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc_in, pk_reqq_index, $sformatf("bus response lookup for 0x%x: %s, %s, %p, %s, %s ... returned %p, %p, %p, %p, %p", line_desc.get_paddr(), m_etlink_req_desc.get_bus_cmd_name(etlink_cmd_in), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_string(qualifier_in), table_desc.get_tag_action(), table_desc.get_data_action(), new_bus_rsp, new_bus_cmd, err_code))
         end
         if (m_global_attrs.coverage_enabled() == 1'b1) begin
            table_desc.sample();
         end
         return;
      end
      `evl_log_mdesc_pk(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc_in, pk_reqq_index, $sformatf("bus response lookup for 0x%x: %s, %s, %p, %s, %s ... no table entry found", line_desc.get_paddr(), m_etlink_req_desc.get_bus_cmd_name(etlink_cmd_in), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_string(qualifier_in)))
      `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("bus response lookup for 0x%x: %0s, %0s, %p, %s, %s ... no table entry", line_desc.get_paddr(), m_etlink_req_desc.get_bus_cmd_name(etlink_cmd_in), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_string(qualifier_in)), pk_sc_sb_error_intensity, { VMOD_SC_SB_WARN, m_verbosity_warn_mod })
      table_desc = m_table_desc_null;
   endfunction : bus_rsp_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_sub_qualifier
   //
   function bit has_sub_qualifier(input evl_sc_qual_t qualifier_in, input evl_sc_qual_t sub_qualifier_in);
      if ((`SCQ_SrcDstMask & sub_qualifier_in) != 0) begin
         if ((`SCQ_SrcDstMask & qualifier_in) == sub_qualifier_in) begin
            return 1'b1;
         end
         return 1'b0;
      end
      if ((~`SCQ_SrcDstMask & qualifier_in & sub_qualifier_in) == (~`SCQ_SrcDstMask & sub_qualifier_in)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : has_sub_qualifier


   //-----------------------------------------------------------------------------------------------
   //
   // Function: qualifier_string
   //
   function string qualifier_string(input evl_sc_qual_t qualifier_in);
      string string_out;
      string space;

      if (qualifier_in == `SCQ_None) begin
         string_out = "SCQ_None";
      end
      else begin
         space = "";
         if (has_sub_qualifier(qualifier_in, `SCQ_Victim)) begin
            string_out = $sformatf("%0s%0sSCQ_Victim", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Stale)) begin
            string_out = $sformatf("%0s%0sSCQ_Stale", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Local)) begin
            string_out = $sformatf("%0s%0sSCQ_Local", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Remote)) begin
            string_out = $sformatf("%0s%0sSCQ_Remote", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Partial)) begin
            string_out = $sformatf("%0s%0sSCQ_Partial", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_CheckPartial)) begin
            string_out = $sformatf("%0s%0sSCQ_CheckPartial", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_MergePartial)) begin
            string_out = $sformatf("%0s%0sSCQ_MergePartial", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_LateRead)) begin
            string_out = $sformatf("%0s%0sSCQ_LateRead", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_SecondPass)) begin
            string_out = $sformatf("%0s%0sSCQ_SecondPass", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_UseSrcAddr)) begin
            string_out = $sformatf("%0s%0sSCQ_UseSrcAddr", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_NoRegion)) begin
            string_out = $sformatf("%0s%0sSCQ_NoRegion", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_CheckLock)) begin
            string_out = $sformatf("%0s%0sSCQ_CheckLock", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_AllLocked)) begin
            string_out = $sformatf("%0s%0sSCQ_AllLocked", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L1)) begin
            string_out = $sformatf("%0s%0sSCQ_L1", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L1_L2)) begin
            string_out = $sformatf("%0s%0sSCQ_L1_L2", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L1_L3)) begin
            string_out = $sformatf("%0s%0sSCQ_L1_L3", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L1_Mem)) begin
            string_out = $sformatf("%0s%0sSCQ_L1_Mem", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L2)) begin
            string_out = $sformatf("%0s%0sSCQ_L2", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L2_L3)) begin
            string_out = $sformatf("%0s%0sSCQ_L2_L3", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L2_Mem)) begin
            string_out = $sformatf("%0s%0sSCQ_L2_Mem", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L3)) begin
            string_out = $sformatf("%0s%0sSCQ_L3", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_L3_Mem)) begin
            string_out = $sformatf("%0s%0sSCQ_L3_Mem", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Mem)) begin
            string_out = $sformatf("%0s%0sSCQ_Mem", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_IdxAddr)) begin
            string_out = $sformatf("%0s%0sSCQ_IdxAddr", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Bypass)) begin
            string_out = $sformatf("%0s%0sSCQ_Bypass", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_DownDis)) begin
            string_out = $sformatf("%0s%0sSCQ_DownDis", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_FromMesh)) begin
            string_out = $sformatf("%0s%0sSCQ_FromMesh", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_DecErr)) begin
            string_out = $sformatf("%0s%0sSCQ_DecErr", string_out, space);
            space = "|";
         end
         if (has_sub_qualifier(qualifier_in, `SCQ_Undefined3)) begin
            string_out = $sformatf("%0s%0sSCQ_Undefined3", string_out, space);
            space = "|";
         end
      end
      return string_out;
   endfunction : qualifier_string

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_table_desc_null = new();
      m_etlink_req_desc = new();
      m_axi_req_desc    = new();
      m_etlink_req_desc.set_bus_type(EVL_BUS_TYPE_ETLINK);
      m_axi_req_desc.set_bus_type(EVL_BUS_TYPE_AXI);
      m_almost_null_line_desc = new();
      `ifdef ET_NO_SC_COVERAGE
         if (`et_test_plusargs("sc_cover_on=1") != 0) begin
            m_sc_scoreboard_cache_cover = new();
         end
      `else // ifdef ET_NO_SC_COVERAGE
         m_sc_scoreboard_cache_cover = new();
      `endif // !ifdef ET_NO_SC_COVERAGE
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sc_scoreboard_cache", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_sc_scoreboard_cache

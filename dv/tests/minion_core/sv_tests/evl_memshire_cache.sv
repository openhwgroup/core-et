//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_cache
//
class evl_memshire_cache extends evl_cache;

   `uvm_component_utils(evl_memshire_cache)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_ms_sb_error_intensity)
   `evl_param_key(pk_lo_pending_count)
   `evl_param_key(pk_hi_pending_count)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_table_wrapper #(evl_memshire_scoreboard, "BusReq") m_bus_req_table;
   evl_table_wrapper #(evl_memshire_scoreboard, "BusRsp") m_bus_rsp_table;
   evl_table_desc                                         m_table_desc_null;
   evl_req_desc                                           m_etlink_req_desc;
   evl_req_desc                                           m_axi_req_desc;
   evl_verbosity_mod_t                                    m_verbosity_mod      = VMOD_MS_SB;
   evl_verbosity_mod_t                                    m_verbosity_warn_mod = VMOD_MS_SB_WARN;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Personalization
   //
// evl_paddr_t m_way_index_mask  = 0;


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
      if (m_bus_req_table == null) begin
         m_bus_req_table = new($sformatf("%0s(%0s-BusReq)", get_root_abstract_name(), m_cache_type));
      end
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new($sformatf("%0s(%0s-BusRsp)", get_root_abstract_name(), m_cache_type));
      end
      m_bus_req_table.enable_coverage();
      m_bus_rsp_table.enable_coverage();
   endfunction : enable_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_coverage
   //
   function void build_coverage();
      m_bus_req_table.build_coverage();
      m_bus_rsp_table.build_coverage();
   endfunction : build_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_req_index
   //
   function longint gen_bus_req_index(input evl_ms_sub_table_t sub_table_in,
                                      input evl_bus_cmd_t      axi_bus_cmd_in,
                                      input evl_bus_cmd_t      etlink_bus_cmd_in,
                                      input evl_cache_st_t     model_st_in,
                                      input evl_ms_qual_t      qualifier_in);
      return longint'({sub_table_in, qualifier_in, model_st_in, etlink_bus_cmd_in, axi_bus_cmd_in});
   endfunction : gen_bus_req_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_req_name
   //
   function string gen_bus_req_name(input longint index_in);
      evl_bus_cmd_t                     raw_axi_bus_cmd;
      evl_bus_cmd_t                     raw_etlink_bus_cmd;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      bit [($bits(evl_ms_qual_t)-1):0]  raw_qualifier;
      bit [($bits(evl_ms_sub_table_t)-1):0] raw_sub_table;
      evl_axi_req_type_t                axi_bus_cmd;
      evl_etlink_req_opcode_t           etlink_bus_cmd;
      evl_cache_st_t                    model_st;
      evl_ms_qual_t                     qualifier;
      evl_ms_sub_table_t                sub_table;

      {raw_sub_table, raw_qualifier, raw_model_st, raw_etlink_bus_cmd, raw_axi_bus_cmd } = index_in;
      axi_bus_cmd    = evl_axi_req_type_t'(raw_axi_bus_cmd);
      etlink_bus_cmd = evl_etlink_req_opcode_t'(raw_etlink_bus_cmd);
      model_st       = evl_cache_st_t'(raw_model_st);
      qualifier      = evl_ms_qual_t'(raw_qualifier);
      sub_table      = evl_ms_sub_table_t'(raw_sub_table);
      return $sformatf("BusReq:%0p:%0p:%0p:%p:%p", sub_table, axi_bus_cmd, etlink_bus_cmd, model_st, qualifier);
   endfunction : gen_bus_req_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act
   //
   function void add_bus_req_act(input evl_ms_sub_table_t sub_table_in,
                                 input evl_bus_cmd_t      axi_bus_cmd_in,
                                 input evl_bus_cmd_t      etlink_bus_cmd_in,
                                 input evl_cache_st_t     model_st_in,
                                 input evl_ms_qual_t      qualifier_in,
                                 input evl_cache_st_t     tag_action_in,
                                 input evl_data_action_t  data_action_in,
                                 input evl_bus_cmd_t      bus_cmd_in,
                                 input evl_bus_rsp_type_t bus_rsp_in,
                                 input int                err_code_in,
                                 input bit                no_cover_in = 1'b0);
      longint        index;
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_cmd(bus_cmd_in);
      table_desc.set_bus_rsp(bus_rsp_in);
      table_desc.set_err_code(err_code_in);

      if (m_bus_req_table == null) begin
         m_bus_req_table = new($sformatf("%0s(%0s-BusReq)", get_root_abstract_name(), m_cache_type));
      end

      index = gen_bus_req_index(sub_table_in, axi_bus_cmd_in, etlink_bus_cmd_in, model_st_in, qualifier_in);

      table_desc.set_index(index);

      m_bus_req_table.add_table_entry(index, table_desc, gen_bus_req_name(index), ~no_cover_in);
   endfunction : add_bus_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_index
   //
   function longint gen_bus_rsp_index(input evl_bus_cmd_t      axi_bus_cmd_in,
                                      input evl_bus_cmd_t      etlink_bus_cmd_in,
                                      input evl_cache_st_t     model_st_in,
                                      input evl_bus_rsp_type_t axi_bus_rsp_in,
                                      input evl_ms_qual_t      qualifier_in);
      return longint'({ qualifier_in, axi_bus_rsp_in, model_st_in, etlink_bus_cmd_in, axi_bus_cmd_in });
   endfunction : gen_bus_rsp_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_name
   //
   function string gen_bus_rsp_name(input longint index_in);
      evl_bus_cmd_t                     raw_axi_bus_cmd;
      evl_bus_cmd_t                     raw_etlink_bus_cmd;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      bit [($bits(evl_ms_qual_t)-1):0]  raw_qualifier;
      evl_bus_rsp_type_t                raw_bus_rsp;
      evl_axi_req_type_t                axi_bus_cmd;
      evl_etlink_req_opcode_t           etlink_bus_cmd;
      evl_cache_st_t                    model_st;
      evl_axi_resp_t                    axi_bus_rsp;
      evl_ms_qual_t                     qualifier;

      { raw_qualifier, raw_bus_rsp, raw_model_st, raw_etlink_bus_cmd, raw_axi_bus_cmd } = index_in;
      etlink_bus_cmd = evl_etlink_req_opcode_t'(raw_etlink_bus_cmd);
      axi_bus_cmd    = evl_axi_req_type_t'(raw_axi_bus_cmd);
      model_st       = evl_cache_st_t'(raw_model_st);
      axi_bus_rsp    = evl_axi_resp_t'(raw_bus_rsp);
      qualifier      = evl_ms_qual_t'(raw_qualifier);
      return $sformatf("BusRsp:%p:%p:%p:%p:%p", axi_bus_cmd, etlink_bus_cmd, model_st, axi_bus_rsp, qualifier);
   endfunction : gen_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_bus_cmd_t      axi_bus_cmd_in,
                                 input evl_bus_cmd_t      etlink_bus_cmd_in,
                                 input evl_cache_st_t     model_st_in,
                                 input evl_axi_resp_t     axi_bus_rsp_in,
                                 input evl_ms_qual_t      qualifier_in,
                                 input evl_cache_st_t     tag_action_in,
                                 input evl_data_action_t  data_action_in,
                                 input evl_bus_rsp_type_t bus_rsp_in,
                                 input evl_bus_cmd_t      bus_cmd_in,
                                 input int                err_code_in,
                                 input bit                no_cover_in = 1'b0);
      longint        index;
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_rsp(bus_rsp_in);
      table_desc.set_bus_cmd(bus_cmd_in);
      table_desc.set_err_code(err_code_in);

      index = gen_bus_rsp_index(axi_bus_cmd_in, etlink_bus_cmd_in, model_st_in, axi_bus_rsp_in, qualifier_in);
      table_desc.set_index(index);
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new($sformatf("%0s(%0s-BusRsp)", get_root_abstract_name(), m_cache_type));
      end
      m_bus_rsp_table.add_table_entry(index, table_desc, gen_bus_rsp_name(index), ~no_cover_in);
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_req_lookup
   //
   virtual function void bus_req_lookup(output evl_table_desc     table_desc,
                                        output evl_line_desc      line_desc,
                                        input  evl_ms_sub_table_t sub_table_in,
                                        input  evl_bus_cmd_t      axi_bus_cmd_in,
                                        input  evl_bus_cmd_t      etlink_bus_cmd_in,
                                        input  evl_req_desc       req_desc_in,
                                        input  evl_ms_qual_t      qualifier_in);
      evl_ms_qual_t qualifier;
      int           child_index;

      line_desc = get_line_desc(req_desc_in.get_paddr());
      req_desc_in.set_line_desc(line_desc);
      if (m_bus_req_table == null) begin
         table_desc = m_table_desc_null;
         return;
      end
      qualifier = qualifier_in;

      table_desc = m_bus_req_table.get_table_entry(gen_bus_req_index(sub_table_in, axi_bus_cmd_in, etlink_bus_cmd_in, line_desc.get_cache_st(), qualifier));

      if (`evl_log_check_verbosity(UVM_HIGH, VMOD_MS_SB) != 0) begin
         if (table_desc != null) begin
            evl_abstract_bus_cmd_t  new_bus_cmd;
            evl_abstract_rsp_type_t new_bus_rsp;
            evl_error_code          err_code;

            new_bus_cmd = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
            new_bus_rsp = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
            err_code    = evl_error_code'(table_desc.get_err_code());
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc_in, $sformatf("bus request lookup for 0x%x: %0s, %0s, %p, %p ... returned %p, %p, %p, %p, %p", req_desc_in.get_paddr(), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), line_desc.get_cache_st(), qualifier, table_desc.get_tag_action(), table_desc.get_data_action(), new_bus_cmd, new_bus_rsp, err_code))
         end
         else begin
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc_in, $sformatf("bus request lookup for 0x%x: %0s, %0s, %p, %p ... no table entry found", req_desc_in.get_paddr(), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), line_desc.get_cache_st(), qualifier))
         end
      end

      if (table_desc != null) begin
         table_desc.sample();
      end
      else begin
         `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("bus request lookup for 0x%x: %0s, %0s, %p, %p ... no table entry", req_desc_in.get_paddr(), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), line_desc.get_cache_st(), qualifier), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_warn_mod })
         table_desc = m_table_desc_null;
      end
   endfunction : bus_req_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_lookup
   //
   virtual function void bus_rsp_lookup(output evl_table_desc      table_desc,
                                        output evl_line_desc       line_desc,
                                        input  evl_req_desc        req_desc_in,
                                        input  evl_bus_cmd_t       axi_bus_cmd_in,
                                        input  evl_bus_cmd_t       etlink_bus_cmd_in,
                                        input  evl_bus_rsp_type_t  bus_rsp_in,
                                        input  evl_ms_qual_t       qualifier_in);
      evl_cache_st_t cache_st;

      line_desc = req_desc_in.get_line_desc();
      if (line_desc == null) begin
         line_desc = get_line_desc(req_desc_in.get_paddr());
      end
      if (line_desc == null) begin
         return;
      end

//    if (has_sub_qualifier(qualifier_in, `SCQ_Stale)) begin
//       cache_st = Stale;
//    end
//    else begin
         cache_st = line_desc.get_cache_st();
//    end

      table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(axi_bus_cmd_in, etlink_bus_cmd_in, cache_st, bus_rsp_in, qualifier_in));
      if (table_desc != null) begin
         if (`evl_log_check_verbosity(UVM_HIGH, VMOD_MS_SB) != 0) begin
            evl_abstract_rsp_type_t new_bus_rsp;
            evl_abstract_bus_cmd_t  new_bus_cmd;
            evl_error_code          err_code;

            new_bus_rsp = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
            new_bus_cmd = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
            err_code    = evl_error_code'(table_desc.get_err_code());
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc_in, $sformatf("bus response lookup for 0x%x: %s, %s, %p, %s, %p ... returned %p, %p, %p, %p, %p", line_desc.get_paddr(), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_in, table_desc.get_tag_action(), table_desc.get_data_action(), new_bus_rsp, new_bus_cmd, err_code))
         end
         table_desc.sample();
         return;
      end
      `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc_in, $sformatf("bus response lookup for 0x%x: %s, %s, %p, %s, %p ... no table entry found", line_desc.get_paddr(), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_in))
      `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("bus response lookup for 0x%x: %0s, %0s, %p, %s, %p ... no table entry", line_desc.get_paddr(), m_axi_req_desc.get_bus_cmd_name(axi_bus_cmd_in), m_etlink_req_desc.get_bus_cmd_name(etlink_bus_cmd_in), cache_st, req_desc_in.get_bus_rsp_name(bus_rsp_in), qualifier_in), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_warn_mod })
      table_desc = m_table_desc_null;
   endfunction : bus_rsp_lookup


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
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_memshire_cache", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_memshire_cache

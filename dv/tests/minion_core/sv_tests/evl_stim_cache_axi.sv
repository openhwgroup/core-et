//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_cache_axi
//
class evl_stim_cache_axi extends evl_cache;

   `uvm_component_utils(evl_stim_cache_axi)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   evl_table_wrapper #(evl_l2_master_agent_axi, "IntReq") m_int_req_table;
   evl_table_wrapper #(evl_l2_master_agent_axi, "BusReq") m_bus_req_table;
   evl_table_wrapper #(evl_l2_master_agent_axi, "BusRsp") m_bus_rsp_table;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_table_desc m_table_desc_null;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Personalization
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_int_req_index
   //
   function longint gen_int_req_index(input evl_cmd_t      cmd_in,
                                      input bit            cache_attr_in,
                                      input evl_cache_st_t model_st_in,
                                      input byte           qualifier_in);
      return longint'({ qualifier_in, model_st_in, cache_attr_in, cmd_in });
   endfunction : gen_int_req_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_int_req_name
   //
   function string gen_int_req_name(input longint index_in);
      bit [($bits(evl_cmd_t)-1):0]      raw_cmd;
      bit                               raw_cache_attr;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      byte                              qualifier;
      evl_cmd_t                         cmd;
      evl_cache_st_t                    model_st;
      string                            cache_attr;

      { qualifier, raw_model_st, raw_cache_attr, raw_cmd } = index_in[$bits(qualifier) + $bits(raw_model_st) + $bits(raw_cache_attr) + $bits(raw_cmd) - 1:0];
      cmd        = evl_cmd_t'(raw_cmd);
      model_st   = evl_cache_st_t'(raw_model_st);
      cache_attr = (raw_cache_attr == 1'b1) ? "cached" : "uncached";
      return $sformatf("IntReq:%p:%p:%0s:0x%x", cmd, model_st, cache_attr, qualifier);
   endfunction : gen_int_req_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_int_req_act
   //
   function void add_int_req_act(input evl_cmd_t              cmd_in,
                                 input bit                    cache_attr_in,
                                 input evl_cache_st_t         model_st_in,
                                 input evl_axi_qual_t         qualifier_in,
                                 input evl_cache_st_t         tag_action_in,
                                 input evl_data_action_t      data_action_in,
                                 input evl_abstract_bus_cmd_t bus_cmd_in);
      longint        index;
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_cmd(evl_bus_cmd_t'(bus_cmd_in));

      index = gen_int_req_index(cmd_in, cache_attr_in, model_st_in, qualifier_in);
      table_desc.set_index(index);
      if (m_int_req_table == null) begin
         m_int_req_table = new();
         m_int_req_table.set_abstract_name($sformatf("%0s(IntReq)", get_root_abstract_name()));
      end
      m_int_req_table.add_table_entry(index, table_desc, gen_int_req_name(index));
   endfunction : add_int_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_req_index
   //
   function longint gen_bus_req_index(input evl_bus_cmd_t    bus_cmd_in,
                                      input evl_cache_st_t   model_st_in,
                                      input evl_axi_qual_t   qualifier_in,
                                      input evl_axi_source_t source_in);
      return longint'({ byte'(source_in), byte'(qualifier_in), model_st_in, bus_cmd_in });
   endfunction : gen_bus_req_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_req_name
   //
   function string gen_bus_req_name(input longint index_in);
      evl_bus_cmd_t                     raw_bus_cmd;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      byte                              raw_qualifier;
      byte                              raw_source;
      evl_axi_qual_t                    qualifier;
      evl_axi_source_t                  source;
      string                            bus_cmd;
      evl_cache_st_t                    model_st;

      { source, qualifier, raw_model_st, raw_bus_cmd } = index_in[$bits(source) + $bits(qualifier) + $bits(raw_model_st) + $bits(raw_bus_cmd) - 1:0];
      bus_cmd   = (raw_bus_cmd == `EVL_AXI_REQ_READ) ? "EVL_AXI_REQ_READ" : "EVL_AXI_REQ_WRITE";
      model_st  = evl_cache_st_t'(raw_model_st);
      qualifier = evl_axi_qual_t'(raw_qualifier);
      source    = evl_axi_source_t'(raw_source);
      return $sformatf("BusReq:%0s:%p:%p:%p", bus_cmd, model_st, qualifier, source);
   endfunction : gen_bus_req_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act
   //
   function void add_bus_req_act(input evl_bus_cmd_t           bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_qual_t          qualifier_in,
                                 input evl_axi_source_t        source_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in,
                                 input evl_abstract_bus_cmd_t  new_bus_cmd_in,
                                 input int                     err_code_in = 0);
      longint        index;
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_cmd(evl_bus_cmd_t'(new_bus_cmd_in));
      table_desc.set_bus_rsp(evl_bus_rsp_type_t'(bus_rsp_in));
      table_desc.set_err_code(err_code_in);

      index = gen_bus_req_index(bus_cmd_in, model_st_in, qualifier_in, source_in);
      table_desc.set_index(index);
      if (m_bus_req_table == null) begin
         m_bus_req_table = new();
         m_bus_req_table.set_abstract_name($sformatf("%0s(BusReq)", get_root_abstract_name()));
      end
      m_bus_req_table.add_table_entry(index, table_desc, gen_bus_req_name(index));
   endfunction : add_bus_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_index
   //
   function longint gen_bus_rsp_index(input evl_bus_cmd_t  bus_cmd_in,
                                      input evl_cache_st_t model_st_in,
                                      input byte           qualifier_in);
      return longint'({ qualifier_in, model_st_in, bus_cmd_in });
   endfunction : gen_bus_rsp_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_name
   //
   function string gen_bus_rsp_name(input longint index_in);
      evl_bus_cmd_t                     raw_bus_cmd;
      bit [($bits(evl_cache_st_t)-1):0] raw_model_st;
      byte                              qualifier;
      string                            bus_cmd;
      evl_cache_st_t                    model_st;

      { qualifier, raw_model_st, raw_bus_cmd } = index_in[$bits(qualifier) + $bits(raw_model_st) + $bits(raw_bus_cmd) - 1:0];
      bus_cmd    = (raw_bus_cmd == `EVL_AXI_REQ_READ) ? "EVL_AXI_REQ_READ" : "EVL_AXI_REQ_WRITE";
      model_st   = evl_cache_st_t'(raw_model_st);
      return $sformatf("BusRsp:%0s:%p:0x%x", bus_cmd, model_st, qualifier);
   endfunction : gen_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_bus_cmd_t           bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_qual_t          qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in);
      longint         index;
      evl_table_desc  table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_rsp(evl_bus_rsp_type_t'(bus_rsp_in));

      index = gen_bus_rsp_index(bus_cmd_in, model_st_in, qualifier_in);
      table_desc.set_index(index);
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new();
         m_bus_rsp_table.set_abstract_name($sformatf("%0s(BusRsp)", get_root_abstract_name()));
      end
      m_bus_rsp_table.add_table_entry(index, table_desc, gen_bus_rsp_name(index));
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: int_req_lookup
   //
   virtual function evl_req_desc int_req_lookup(output evl_table_desc table_desc,
                                                output evl_line_desc  line_desc,
                                                input  evl_trans_desc trans_desc_in,
                                                input  evl_req_desc   req_desc_in,
                                                input  evl_axi_qual_t qualifier_in);
      evl_req_desc req_desc = req_desc_in;
      int          child_index;

      line_desc = get_line_desc(trans_desc_in.get_paddr());
      if (m_int_req_table == null) begin
         table_desc = m_table_desc_null;
         if (req_desc != null) begin
            req_desc.clr_bus_cmd();
         end
         return req_desc;
      end

      table_desc = m_int_req_table.get_table_entry(gen_int_req_index(trans_desc_in.get_cmd(), trans_desc_in.is_cached(), line_desc.get_cache_st(), byte'(qualifier_in)));
      if (table_desc == null) begin
         table_desc = m_int_req_table.get_table_entry(gen_int_req_index(trans_desc_in.get_cmd(), trans_desc_in.is_cached(), St_Ignore, byte'(qualifier_in)));
      end
      if (req_desc == null) begin
         $cast(req_desc, m_template_req_desc.clone());
      end
      trans_desc_in.copy_to_desc(req_desc);
      child_index = trans_desc_in.add_child_desc(req_desc);
      if (table_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), trans_desc_in, $sformatf("TB_ISSUE: didn't find a table match for %0s ... %0p, %0p, %0p, %0p", trans_desc_in.sprint_obj(), trans_desc_in.get_cmd(), trans_desc_in.is_cached(), line_desc.get_cache_st(), qualifier_in));
         table_desc = m_table_desc_null;
         req_desc.clr_bus_cmd();
         return req_desc;
      end
      if (table_desc.pending_bus_cmd() == 1'b0) begin
         req_desc.clr_bus_cmd();
         return req_desc;
      end

      req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
      req_desc.set_paddr(trans_desc_in.get_paddr());
      req_desc.set_bus_req_size($clog2(trans_desc_in.get_cmd_length()));

      return req_desc;
   endfunction : int_req_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_lookup
   //
   virtual function void bus_rsp_lookup(output evl_table_desc table_desc,
                                        output evl_line_desc  line_desc,
                                        input  evl_req_desc   req_desc_in,
                                        input  evl_axi_qual_t qualifier_in);
      line_desc = get_line_desc(req_desc_in.get_paddr());
      if (m_bus_rsp_table == null) begin
         table_desc = m_table_desc_null;
         return;
      end
      `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus rsp lookup: %0s, %p, %p", req_desc_in.get_bus_cmd_name(), line_desc.get_cache_st(), qualifier_in))
      table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(req_desc_in.get_bus_cmd(), line_desc.get_cache_st(), byte'(qualifier_in)));
      if (table_desc == null) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus rsp lookup: %0s, St_Ignore, %p", req_desc_in.get_bus_cmd_name(), qualifier_in))
         table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(req_desc_in.get_bus_cmd(), St_Ignore, byte'(qualifier_in)));
         if (table_desc == null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus rsp lookup: EVL_Bus_Idle, St_Ignore, AXQ_None"))
            table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(evl_bus_cmd_t'(`EVL_Bus_Idle), St_Ignore, byte'(AXQ_None)));
         end
      end
      if (table_desc == null) begin
         table_desc = m_table_desc_null;
         return;
      end
      return;

   endfunction : bus_rsp_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_req_lookup
   //
   virtual function void bus_req_lookup(output evl_table_desc   table_desc,
                                        output evl_line_desc    line_desc,
                                        input  evl_req_desc     req_desc_in,
                                        input  evl_axi_qual_t   qualifier_in,
                                        input  evl_axi_source_t source_in);
      evl_axi_req_type_t bus_cmd = evl_axi_req_type_t'(req_desc_in.get_bus_cmd());

      line_desc = get_line_desc(req_desc_in.get_paddr(), 1); // create data if it doesn't exist
      if (m_bus_req_table == null) begin
         table_desc = m_table_desc_null;
         return;
      end
      `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus req lookup: %0s, %p, %p, %p", req_desc_in.get_bus_cmd_name(), line_desc.get_cache_st(), qualifier_in, source_in))
      table_desc = m_bus_req_table.get_table_entry(gen_bus_req_index(req_desc_in.get_bus_cmd(), line_desc.get_cache_st(), qualifier_in, source_in));
      if (table_desc == null) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus req lookup: %0s, St_Ignore, %p, %p", req_desc_in.get_bus_cmd_name(), qualifier_in, source_in))
         table_desc = m_bus_req_table.get_table_entry(gen_bus_req_index(req_desc_in.get_bus_cmd(), St_Ignore, qualifier_in, source_in));
         if (table_desc == null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc_in, $sformatf("performing bus req lookup: EVL_Bus_Idle, St_Ignore, AXQ_None, AXI_NoSrc"))
            table_desc = m_bus_req_table.get_table_entry(gen_bus_req_index(evl_bus_cmd_t'(`EVL_Bus_Idle), St_Ignore, AXQ_None, AXI_NoSrc));
         end
      end
      if (table_desc == null) begin
         table_desc = m_table_desc_null;
      end
      if (table_desc.get_err_code() != 0) begin 
         `dut_error_mdesc(get_abstract_name(), req_desc_in, $sformatf("TB_ISSUE: didn't find a table match for %0s ... %0s, %0p, %0p, %0p", req_desc_in.sprint_obj(), req_desc_in.get_bus_cmd_name(), line_desc.get_cache_st(), qualifier_in, source_in))
      end
   endfunction : bus_req_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      m_table_desc_null = new();
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_stim_cache_axi", input uvm_component parent_in = null);
      int value;

      super.new(name_in, parent_in);
      set_random_data_use();
      if (`et_value_plusargs("forceZeroLine=%d", value) != 0) begin      
         set_force_zero_weight(value);
      end
      else begin
         set_force_zero_weight(10);
      end
   endfunction : new

endclass : evl_stim_cache_axi

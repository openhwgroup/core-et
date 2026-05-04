//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_dcache
//
//
class evl_minion_dcache  #(type SCOREBOARD_TYPE = evl_minion_dcache_scoreboard) extends evl_cache;

   // `uvm_component_utils(evl_minion_dcache)
   `uvm_component_param_utils(evl_minion_dcache #(SCOREBOARD_TYPE))

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_table_wrapper #(SCOREBOARD_TYPE, "CoreReq") m_req_table;
   evl_table_wrapper #(SCOREBOARD_TYPE, "BusRsp")  m_bus_rsp_table;
   evl_table_desc                                  m_table_desc_null;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_coverage
   //
   function void enable_coverage();
      if (m_req_table == null) begin
         m_req_table = new($sformatf("%0s(%0s-CoreReq)", get_root_abstract_name(), m_cache_type));
      end
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table = new($sformatf("%0s(%0s-BusRsp)", get_root_abstract_name(), m_cache_type));
      end
      m_req_table.enable_coverage();
      m_bus_rsp_table.enable_coverage();
   endfunction : enable_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dump_tables
   //
   virtual function void dump_tables();
      if (m_req_table != null) begin
         m_req_table.dump_tables();
      end
      if (m_bus_rsp_table == null) begin
         m_bus_rsp_table.dump_tables();
      end
   endfunction : dump_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_coverage
   //
   function void build_coverage();
      m_req_table.build_coverage();
      m_bus_rsp_table.build_coverage();
   endfunction : build_coverage


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_req_index
   //
   function longint gen_req_index(input evl_cmd_t         cmd_in,
                                  input evl_cache_st_t    model_st_in,
                                  input evl_dcache_qual_t qualifier_in);
      return longint'({ qualifier_in, model_st_in, cmd_in });
   endfunction : gen_req_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_req_name
   //
   function string gen_req_name(input longint index_in);
      evl_cmd_t         raw_cmd;
      evl_cache_st_t    raw_model_st;
      evl_dcache_qual_t qualifier;

      { qualifier, raw_model_st, raw_cmd } = index_in;
      return $sformatf("CoreReq:%0s:%p:%p:%p", m_cache_type, raw_cmd, raw_model_st, qualifier);
   endfunction : gen_req_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_req_act
   //
   function void add_req_act(input evl_cmd_t          cmd_list_in[],
                             input evl_cache_st_t     model_st_list_in[],
                             input evl_dcache_qual_t  qualifier_in,
                             input evl_cache_st_t     tag_action_in,
                             input evl_data_action_t  data_action_in,
                             input evl_bus_cmd_t      bus_cmd_in,
                             input evl_bus_rsp_type_t bus_rsp_in,
                             input int                err_code_in,
                             input bit                no_cover_in = 1'b0);
      longint        index;
      evl_cmd_t      cmd_list[$];
      evl_cache_st_t model_st_list[$];
      evl_table_desc table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_bus_cmd(bus_cmd_in);
      table_desc.set_bus_rsp(bus_rsp_in);
      table_desc.set_err_code(err_code_in);

      if (cmd_list_in.size() == 0) begin
         cmd_list.push_back(evl_cmd_t'(`EVL_Bus_Idle));
      end
      else begin
         foreach (cmd_list_in[ii]) begin
            cmd_list.push_back(cmd_list_in[ii]);
         end
      end

      if (model_st_list_in.size() == 0) begin
         model_st_list.push_back(St_Ignore);
      end
      else begin
         foreach (model_st_list_in[ii]) begin
            model_st_list.push_back(model_st_list_in[ii]);
         end
      end

      foreach (cmd_list[ii]) begin
         foreach (model_st_list[jj]) begin
            index = gen_req_index(cmd_list[ii], model_st_list[jj], qualifier_in);
            if (m_req_table == null) begin
               m_req_table = new();
            end
            `evl_log(UVM_HIGH, VMOD_MINION_SB, get_abstract_name(), $sformatf("add_req_act %0s", gen_req_name(index)))
            //`evl_log(UVM_HIGH, VMOD_L1_SB, get_abstract_name(), $sformatf("add_req_act %0s", gen_req_name(index)))
            m_req_table.add_table_entry(index, table_desc, gen_req_name(index), ~no_cover_in);
         end
      end
   endfunction : add_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: req_lookup
   //
   // Hierarchy of lookups:
   //
   // - { cmd, model_st,  qualifier }
   // - { cmd, St_Ignore, qualifier }
   //
   virtual function void req_lookup(output evl_table_desc    table_desc,
                                    input  evl_cmd_t         cmd_in,
                                    input  evl_cache_st_t    state_in,
                                    input  evl_dcache_qual_t qualifier_in,
                                    input  string            debug_msg = "dcache");
      int child_index;

      if (m_req_table == null) begin
         table_desc = m_table_desc_null;
         return;
      end

      table_desc = m_req_table.get_table_entry(gen_req_index(cmd_in, state_in, qualifier_in));

      if (table_desc == null) begin
         table_desc = m_req_table.get_table_entry(gen_req_index(cmd_in, St_Ignore, qualifier_in));
      end

      if (table_desc != null) begin
         if (m_global_attrs.coverage_enabled() == 1'b1) begin
            table_desc.sample();
         end
      end
      else begin
         sb_error($sformatf("internal lookup for %0s: %p, %p, %p ... no table entry", debug_msg, cmd_in, state_in, qualifier_in), null, `__FILE__, `__LINE__);
         table_desc = m_table_desc_null;
      end
   endfunction : req_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_index
   //
   function longint gen_bus_rsp_index(input evl_cmd_t          cmd_in,
                                      input evl_bus_cmd_t      bus_cmd_in,
                                      input evl_cache_st_t     model_st_in,
                                      input evl_bus_rsp_type_t bus_rsp_in,
                                      input evl_dcache_qual_t  qualifier_in);
      return longint'({ qualifier_in, bus_rsp_in, model_st_in, bus_cmd_in, cmd_in });
   endfunction : gen_bus_rsp_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_bus_rsp_name
   //
   function string gen_bus_rsp_name(input longint index_in);
      evl_cmd_t               raw_cmd;
      evl_bus_cmd_t           raw_etlink_cmd;
      evl_cache_st_t          raw_model_st;
      evl_bus_rsp_type_t      raw_bus_rsp;
      bit [3:0]               raw_qualifier;
      evl_dcache_qual_t       qualifier;
      evl_abstract_rsp_type_t bus_rsp;
      evl_etlink_req_opcode_t etlink_cmd;

      { raw_qualifier, raw_bus_rsp, raw_model_st, raw_etlink_cmd, raw_cmd } = index_in;
      qualifier  = evl_dcache_qual_t'(raw_qualifier);
      bus_rsp    = evl_abstract_rsp_type_t'(raw_bus_rsp);
      etlink_cmd = evl_etlink_req_opcode_t'(raw_etlink_cmd);
      return $sformatf("BusRsp:%0s:%p:%p:%p:%p:%p", m_cache_type, raw_cmd, etlink_cmd, raw_model_st, bus_rsp, qualifier);
   endfunction : gen_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   function void add_bus_rsp_act(input evl_cmd_t          cmd_list_in[],
                                 input evl_bus_cmd_t      bus_cmd_list_in[],
                                 input evl_cache_st_t     model_st_list_in[],
                                 input evl_bus_rsp_type_t bus_rsp_list_in[],
                                 input evl_dcache_qual_t  qualifier_in,
                                 input evl_cache_st_t     tag_action_in,
                                 input evl_data_action_t  data_action_in,
                                 input int                err_code_in,
                                 input bit                no_cover_in = 1'b0);
      longint            index;
      evl_cmd_t          cmd_list[$];
      evl_bus_cmd_t      bus_cmd_list[$];
      evl_cache_st_t     model_st_list[$];
      evl_bus_rsp_type_t bus_rsp_list[$];
      evl_table_desc     table_desc;

      table_desc = new();
      table_desc.set_tag_action(tag_action_in);
      table_desc.set_data_action(data_action_in);
      table_desc.set_err_code(err_code_in);

      if (cmd_list_in.size() == 0) begin
         cmd_list.push_back(NOCMD);
      end
      else begin
         foreach (cmd_list_in[ii]) begin
            cmd_list.push_back(cmd_list_in[ii]);
         end
      end

      if (bus_cmd_list_in.size() == 0) begin
         bus_cmd_list.push_back(`EVL_Bus_Idle);
      end
      else begin
         foreach (bus_cmd_list_in[ii]) begin
            bus_cmd_list.push_back(bus_cmd_list_in[ii]);
         end
      end

      if (model_st_list_in.size() == 0) begin
         model_st_list.push_back(St_Ignore);
      end
      else begin
         foreach (model_st_list_in[ii]) begin
            model_st_list.push_back(model_st_list_in[ii]);
         end
      end

      if (bus_rsp_list_in.size() == 0) begin
         bus_rsp_list.push_back(`EVL_Rsp_Null);
      end
      else begin
         foreach (bus_rsp_list_in[ii]) begin
            bus_rsp_list.push_back(bus_rsp_list_in[ii]);
         end
      end

      foreach (cmd_list[ii]) begin
         foreach (bus_cmd_list[jj]) begin
            foreach (model_st_list[kk]) begin
               foreach (bus_rsp_list[ll]) begin
                  index = gen_bus_rsp_index(cmd_list[ii], bus_cmd_list[jj], model_st_list[kk], bus_rsp_list[ll], qualifier_in);
                  if (m_bus_rsp_table == null) begin
                     m_bus_rsp_table = new();
                  end
                  `evl_log(UVM_HIGH, VMOD_MINION_SB, get_abstract_name(), $sformatf("add_bus_rsp_act %0s", gen_bus_rsp_name(index)))
                  //`evl_log(UVM_HIGH, VMOD_L1_SB, get_abstract_name(), $sformatf("add_bus_rsp_act %0s", gen_bus_rsp_name(index)))
                  m_bus_rsp_table.add_table_entry(index, table_desc, gen_bus_rsp_name(index), ~no_cover_in);
               end
            end
         end
      end
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_lookup
   //
   // - { root_cmd, bus_cmd, model_st,  bus_rsp_type }
   // - { root_cmd, bus_cmd, St_Ignore, bus_rsp_type }
   //
   virtual function void bus_rsp_lookup(output evl_table_desc      table_desc,
                                        output evl_line_desc       line_desc,
                                        input  evl_req_desc        req_desc_in,
                                        input  evl_dcache_qual_t   qualifier_in,
                                        input  evl_bus_rsp_type_t  bus_rsp_in);
      evl_cache_st_t cache_st;
      evl_cmd_t      root_cmd;
      evl_bus_cmd_t  bus_cmd;
      evl_trans_desc root_desc;

      root_desc = req_desc_in.get_root_trans_desc();
      root_cmd  = (root_desc) ? root_desc.get_cmd() : NOCMD;
      bus_cmd   = req_desc_in.get_bus_cmd();
      line_desc = req_desc_in.get_line_desc();

    /****
      //if (line_desc != null) begin
      //  `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) bus_rsp_lookup: req_desc_in.get_line_desc (first try): %s", line_desc.sprint_obj())) 
      //  if (line_desc.m_line_data == null) begin
      //     `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) for above line_desc: but m_line_data is null"))
      //  end else begin
      //     `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) for above line_desc: m_line_data is following:"))
      //     if (`evl_log_check_verbosity(UVM_DEBUG, VMOD_L1_SB) != 0) begin
      //        line_desc.m_line_data.print_obj(get_abstract_name(), req_desc_in.get_paddr());
      //     end
      //  end
      //end
      //else begin
      //  `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) bus_rsp_lookup: req_desc_in.get_line_desc (first try) returned null" )) 
      //end
    ****/
      
      if (line_desc == null) begin
         line_desc = get_line_desc(req_desc_in.get_paddr());
       /****
         //line_desc = get_line_desc(req_desc_in.get_paddr(), 1);    // CYE: create_data=1
         //if (line_desc != null) begin
         //   `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) bus_rsp_lookup: get_line_desc (second try): %s", line_desc.sprint_obj()))
         //   if (line_desc.m_line_data == null) begin
         //      `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) for above line_desc: but m_line_data is null"))
         //   end else begin
         //      `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) for above line_desc: m_line_data is following:"))
         //      if (`evl_log_check_verbosity(UVM_DEBUG, VMOD_L1_SB) != 0) begin
         //         line_desc.m_line_data.print_obj(get_abstract_name(), req_desc_in.get_paddr());
         //      end
         //   end
         //end 
         //else begin
         //   `evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) bus_rsp_lookup: get_line_desc (second try): returned null" ))
         //end
       ****/
      end

      if (line_desc == null) begin
         //`evl_log_mdesc(UVM_DEBUG, VMOD_L1_SB, get_abstract_name(), req_desc_in, $sformatf("(CYE DEBUG) bus_rsp_lookup: final line_desc is still null: will return (out of function bus_rsp_lookup)" ))
         return;
      end

      cache_st = line_desc.get_cache_st();

      table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(root_cmd, bus_cmd, cache_st, bus_rsp_in, qualifier_in));
      if (table_desc == null) begin
         table_desc = m_bus_rsp_table.get_table_entry(gen_bus_rsp_index(root_cmd, bus_cmd, St_Ignore, bus_rsp_in, qualifier_in));
      end

      if (table_desc != null) begin
         if (m_global_attrs.coverage_enabled() == 1'b1) begin
            table_desc.sample();
         end
      end
      else begin
         sb_error($sformatf("bus response lookup for 0x%x: %0p %0p %0p %0p... no table entry", line_desc.get_paddr(), root_cmd, bus_cmd, cache_st, qualifier_in), req_desc_in, `__FILE__, `__LINE__);
         table_desc = m_table_desc_null;
      end
   endfunction : bus_rsp_lookup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_locked_way_count
   //
   // This function returns the number of way that are currently locked at the provided index
   //
   function int get_locked_way_count(input evl_paddr_t paddr_in);
      evl_paddr_t                  paddr;
      evl_way_desc#(evl_line_desc) way_desc;
      evl_line_desc                prev_line;
      evl_paddr_t                  way_cache_index;
      int                          count;

      paddr = paddr_in & const_line_paddr_mask;
      count = 0;

      for (int w = 0; w < m_num_ways; w++) begin
         way_cache_index = gen_way_cache_index(w, paddr);
         if (m_mdl_ways.exists(way_cache_index)) begin
            way_desc  = m_mdl_ways[way_cache_index];
            if (way_desc != null) begin
               $cast(prev_line, way_desc.get_line_desc());
               if ((prev_line != null) && (prev_line.get_cache_st() inside {L, D})) begin
                  count++;
               end
            end
         end
      end

      return count;
   endfunction : get_locked_way_count


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
   function new(input string name_in = "evl_minion_dcache", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("MINION(D$)");
   endfunction : new

endclass: evl_minion_dcache

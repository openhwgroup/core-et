//--------------------------------------------------------------------------------------------------
//
// Class: evl_cache_base
//
class evl_cache_base #(type TAG_T = evl_paddr_t, type LINE_T = evl_line_desc_base) extends evl_component;

   `uvm_component_param_utils(evl_cache_base#(TAG_T, LINE_T))


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit                   m_memory_loaded       = 1'b0;
   string                m_cache_type          = "$";
   evl_line_data         m_template_cache_data = null;
   LINE_T                m_template_line_desc  = null;
   evl_way_desc#(LINE_T) m_template_way_desc   = null;
   evl_req_desc          m_template_req_desc   = null;
   string                m_file_name           = "";
   evl_scoreboard        m_parent_sb           = null;
   LINE_T                m_mdl_lines[TAG_T];
   evl_way_desc#(LINE_T) m_mdl_ways[TAG_T];
   LINE_T                m_rtl_lines[TAG_T];
   evl_rand_gen          m_rand_gen;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Personalization
   //
   int   m_num_ways         = 0;
   bit   m_random_data      = 0;
   bit   m_use_base_and_top = 0;
   int   m_force_zero_wt    = 0;
   TAG_T m_base_addr        = 0;
   TAG_T m_top_addr         = 0;
   TAG_T m_size_mask        = 0;
   TAG_T m_tag_mask         = ~TAG_T'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Common Code for Calculating Indices into a Cache
   //
   evl_cache_index_config#(TAG_T) cache_index_config;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_parent_sb
   //
   virtual function void set_parent_sb(input evl_scoreboard sb_in);
      m_parent_sb = sb_in;
   endfunction : set_parent_sb


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_type
   //
   virtual function void set_cache_type(input string cache_type_in);
      m_cache_type = cache_type_in;
   endfunction : set_cache_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   virtual function void set_rand_gen(input evl_rand_gen rand_gen_in);
      m_rand_gen = rand_gen_in;
   endfunction : set_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_template
   //
   virtual function void set_data_template(input evl_line_data data_template_in);
      m_template_cache_data = data_template_in;
   endfunction : set_data_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_line_template
   //
   virtual function void set_line_template(input LINE_T line_template_in);
      m_template_line_desc = line_template_in;
   endfunction : set_line_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_way_template
   //
   virtual function void set_way_template(input evl_way_desc#(LINE_T) way_template_in);
      m_template_way_desc = way_template_in;
   endfunction : set_way_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_desc_template
   //
   virtual function void set_req_desc_template(input evl_req_desc req_desc_template_in);
      m_template_req_desc = req_desc_template_in;
   endfunction : set_req_desc_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_base_and_top
   //
   function void set_base_and_top(input TAG_T base_addr_in, input TAG_T size_in);
      m_size_mask        = (TAG_T'(1) << $clog2(size_in)) - TAG_T'(1);
      m_base_addr        = base_addr_in;
      m_top_addr         = base_addr_in + size_in - TAG_T'(1);
      m_use_base_and_top = 1'b1;
   endfunction : set_base_and_top


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tag_mask
   //
   virtual function void set_tag_mask(input TAG_T addr_mask_in);
      m_tag_mask = addr_mask_in;
   endfunction : set_tag_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_mask
   //
   virtual function TAG_T get_tag_mask();
      return m_tag_mask;
   endfunction : get_tag_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_desc
   //
   virtual function LINE_T get_line_desc(input TAG_T tag_in,
                                         input bit   create_data = 1'b0);
      TAG_T tag;

      if (m_memory_loaded == 1'b0) begin
         m_memory_loaded = 1'b1;
         if (m_file_name != "") begin
            perform_load_mem();
         end
      end
      if (m_use_base_and_top == 1'b1) begin
         if (((tag_in - m_base_addr) & m_size_mask) > m_top_addr) begin
            tag = (m_top_addr + TAG_T'(1)) & m_tag_mask & m_size_mask;
            `dut_warning(get_abstract_name(), $sformatf("address is out of range (0x%x -> 0x%x is outside 0x%x-0x%x)", tag_in, (tag_in - m_base_addr) & m_size_mask, m_base_addr, m_top_addr))
         end
         else begin
            tag = tag_in & m_tag_mask & m_size_mask;
            if ((tag_in & m_tag_mask) != tag) begin
               `evl_log(UVM_MEDIUM, VMOD_CACHE, get_abstract_name(), $sformatf("mapping address 0x%x to 0x%x", tag_in, tag))
            end
         end
      end
      else begin
         tag = tag_in & m_tag_mask;
      end
      if (!m_mdl_lines.exists(tag)) begin
         m_mdl_lines[tag] = new(get_abstract_name());
         m_mdl_lines[tag].set_rand_gen(m_rand_gen);
         m_mdl_lines[tag].set_tag_mask(m_tag_mask);
         m_mdl_lines[tag].set_tag(tag_in);
         if (create_data == 1'b1) begin
            if ((m_random_data == 1'b0) || (m_force_zero_wt == 0)) begin
               m_mdl_lines[tag].create_data(m_random_data);
            end
            else begin
               if (m_rand_gen.get_int(100) <= m_force_zero_wt) begin
                  m_mdl_lines[tag].create_data(1'b0);
               end
               else begin
                  m_mdl_lines[tag].create_data(m_random_data);
               end
            end
         end
      end
      return m_mdl_lines[tag];
   endfunction : get_line_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_desc_by_set_way
   //
   virtual function LINE_T get_line_desc_by_set_way(input TAG_T     tag_in,
                                                    input evl_way_t way_in,
                                                    input bit       create_data = 1'b0,
                                                    input bit       warning_if_not_found = 1'b1,
                                                    input bit       return_null_if_not_found = 1'b0);
      TAG_T        tag;
      evl_way_desc#(LINE_T) way_desc;
      LINE_T       prev_line;
      TAG_T        way_cache_index;

      tag = tag_in & m_tag_mask;

      if (int'(way_in) >= m_num_ways) begin
         return null;
      end
      way_cache_index = gen_way_cache_index(way_in, tag);
      if (!m_mdl_ways.exists(way_cache_index)) begin
         $cast(way_desc, m_template_way_desc.clone());
         way_desc.set_cache_way(way_in);
         m_mdl_ways[way_cache_index] = way_desc;
      end
      way_desc  = m_mdl_ways[way_cache_index];
      $cast(prev_line, way_desc.get_line_desc());
      if (prev_line == null) begin
         if (warning_if_not_found == 1'b1) begin
            `dut_warning(get_abstract_name(), $sformatf("Access to set %0x way %0d - line has not been initialized)", tag & get_index_mask(), int'(way_in)))
         end
         if (return_null_if_not_found == 1'b1) begin
            return prev_line;
         end
         prev_line = get_line_desc(tag, create_data);
      end
      return prev_line;
   endfunction : get_line_desc_by_set_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_random_data_use
   //
   virtual function void set_random_data_use();
      m_random_data = 1'b1;
   endfunction : set_random_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_zero_data_use
   //
   virtual function void set_zero_data_use();
      m_random_data = 1'b0;
   endfunction : set_zero_data_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_force_zero_weight
   //
   virtual function void set_force_zero_weight(input int weight_in);
      m_force_zero_wt = (weight_in <= 0) ? 0 : ((weight_in >= 100) ? 100 : weight_in);
   endfunction : set_force_zero_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_config
   //
   function void set_cache_config(input int   num_ways_in,
                                  input TAG_T index_mask_in,
                                  input TAG_T base_index_in  = TAG_T'(0),
                                  input TAG_T top_index_in   = TAG_T'(0),
                                  input TAG_T index_alias_in = TAG_T'(0));
      m_num_ways = ((num_ways_in > 0) && (num_ways_in < 9)) ? num_ways_in : 0;
      cache_index_config.set_cache_config(num_ways_in, index_mask_in, base_index_in, top_index_in, index_alias_in);
   endfunction : set_cache_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_num_ways
   //
   function int get_num_ways();
      return m_num_ways;
   endfunction : get_num_ways


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_way_cache_index
   //
   function TAG_T gen_way_cache_index(input evl_way_t way_in,
                                      input TAG_T     addr_in);
      return cache_index_config.gen_way_cache_index(way_in, addr_in);
   endfunction : gen_way_cache_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_set_bits
   //
   function TAG_T get_set_bits(input TAG_T addr_in);
      return cache_index_config.get_set_bits(addr_in);
   endfunction : get_set_bits


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_mask
   //
   function TAG_T get_index_mask();
      return cache_index_config.get_index_mask();
   endfunction : get_index_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: allocate_way
   //
   virtual function LINE_T allocate_way(input evl_way_t way_in,
                                        input LINE_T    line_desc_in);
      evl_way_desc#(LINE_T) way_desc;
      LINE_T                prev_line;
      TAG_T                 way_cache_index;

      if ((m_num_ways <= 0) || (int'(way_in) >= m_num_ways)) begin
         return null;
      end
      way_cache_index = gen_way_cache_index(way_in, line_desc_in.get_tag());
      if (!m_mdl_ways.exists(way_cache_index)) begin
         $cast(way_desc, m_template_way_desc.clone());
         way_desc.set_cache_way(way_in);
         m_mdl_ways[way_cache_index] = way_desc;
      end
      way_desc = m_mdl_ways[way_cache_index];
      $cast(prev_line, way_desc.get_line_desc());
      if (line_desc_in.get_way_desc() != null) begin
         if ((prev_line == line_desc_in) && (line_desc_in.get_way_desc() == way_desc)) begin
            `dut_warning(get_abstract_name(), $sformatf("reallocating way %0d to the same line (0x%x)", int'(way_in), line_desc_in.get_tag()))
         end
         else if (line_desc_in.get_way_desc() == way_desc) begin
            `dut_warning(get_abstract_name(), $sformatf("reallocating way %0d to the same line (0x%x ... way_desc points to null)", int'(way_in), line_desc_in.get_tag()))
         end
         else begin
            `dut_warning(get_abstract_name(), $sformatf("reallocating line 0x%x from way %0d to way %0d", line_desc_in.get_tag(), int'(line_desc_in.get_cache_way()), int'(way_in)))
         end
      end
      if (prev_line != null) begin
         prev_line.set_way_desc(null);
      end
      way_desc.set_line_desc(line_desc_in, 1'b1);
      return prev_line;
   endfunction : allocate_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: deallocate_way
   //
   virtual function LINE_T deallocate_way(input LINE_T line_desc_in);
      evl_way_desc#(LINE_T) way_desc;

      if (m_num_ways <= 0) begin
         return null;
      end
      way_desc = line_desc_in.get_way_desc();
      if (way_desc != null) begin
         way_desc.set_line_desc(null);
      end
      line_desc_in.set_way_desc(null);
      line_desc_in.set_cache_way(EVL_WAY_NONE);
   endfunction : deallocate_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_valid_line_list
   //
   virtual function int get_valid_line_list(output LINE_T line_list[]);
      LINE_T line_q[$];

      foreach (m_mdl_lines[addr]) begin
         if (m_mdl_lines[addr].get_cache_st() != I) begin
            line_q.push_back(m_mdl_lines[addr]);
         end
      end
      if (line_q.size() <= 0) begin
         return 0;
      end
      line_list = new[line_q.size()](line_q);
      return line_q.size();
   endfunction : get_valid_line_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset
   //
   virtual function void reset();
      LINE_T line_q[$];

      if (get_valid_line_list(line_q) > 0) begin
         foreach (line_q[ii]) begin
            void'(deallocate_way(line_q[ii]));
            line_q[ii].zero_all_line_data();
            line_q[ii].set_cache_st(I);
         end
      end
   endfunction : reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_matching_lines
   //
   virtual function void reset_matching_lines(input TAG_T mask_in, input TAG_T value_to_match_in);
      LINE_T line_q[$];

      if (get_valid_line_list(line_q) > 0) begin
         foreach (line_q[addr]) begin
            if ((line_q[addr].get_tag() & mask_in) == (value_to_match_in & mask_in)) begin
               void'(deallocate_way(line_q[addr]));
               line_q[addr].zero_all_line_data();
               line_q[addr].set_cache_st(I);
            end
         end
      end
   endfunction : reset_matching_lines


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dump_tables
   //
   virtual function void dump_tables();
      return;
   endfunction: dump_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem();
      return;
   endfunction: perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: load_mem
   //
   virtual function void load_mem(input string file_name);
      m_file_name = file_name;
   endfunction: load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sb_error
   //
   //   This function is used to throw DUT errors that will be qualified with this cache's parent
   //   scoreboard error intensity key, if it has been defined.
   //
   virtual function void sb_error(input string       msg_in,
                                  input evl_mem_desc mem_desc_in = null,
                                  input string       file_in     = "",
                                  input int          line_in     = 0);
      if (m_parent_sb != null) begin
         m_parent_sb.sb_error(msg_in, mem_desc_in, {}, get_abstract_name(), file_in, line_in);
      end
      else begin
         if (mem_desc_in != null) begin
            `dut_error_mdesc(get_abstract_name(), mem_desc_in, msg_in)
         end
         else begin
            `dut_error(get_abstract_name(), msg_in)
         end
      end
   endfunction : sb_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      if (m_global_attrs.use_random_data() == 1'b1) begin
         set_random_data_use();
      end
      else begin
         set_zero_data_use();
      end
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if (m_template_cache_data == null) begin
         m_template_cache_data = evl_line_data::type_id::create(get_abstract_name());
      end
      if (m_template_line_desc == null) begin
         m_template_line_desc = LINE_T::type_id::create(get_abstract_name());
         m_template_line_desc.set_data_template(m_template_cache_data);
      end
      if (m_template_way_desc == null) begin
         m_template_way_desc = evl_way_desc#(LINE_T)::type_id::create(get_abstract_name());
      end
      if (m_template_req_desc == null) begin
         m_template_req_desc = evl_req_desc::type_id::create(get_abstract_name());
         m_template_req_desc.set_data_template(m_template_cache_data);
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      super.reset_phase(phase);
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      super.post_reset_phase(phase);
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      super.pre_configure_phase(phase);
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      super.configure_phase(phase);
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      super.post_configure_phase(phase);
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      super.pre_main_phase(phase);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      super.post_main_phase(phase);
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      super.pre_shutdown_phase(phase);
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      super.shutdown_phase(phase);
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_cache_base", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      cache_index_config = new();
   endfunction : new

endclass : evl_cache_base

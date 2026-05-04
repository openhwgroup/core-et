//--------------------------------------------------------------------------------------------------
//
// Class: evl_sc_config_test
//
class evl_sc_config_test extends evl_stimulus;

   `uvm_object_utils(evl_sc_config_test)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_sc_config_test m_sc_config_test = new();


   //-----------------------------------------------------------------------------------------------
   typedef enum int {
         EVL_SC_l3_shire_swizzle_ctl      = 0,
         EVL_SC_reqq_ctl                  = 1,
         EVL_SC_pipe_ctl                  = 2,
         EVL_SC_l2_cache_ctl              = 3,
         EVL_SC_l3_cache_ctl              = 4,
         EVL_SC_scp_cache_ctl             = 5,
         EVL_SC_idx_cop_sm_ctl            = 6,
         EVL_SC_idx_cop_sm_physical_index = 7,
         EVL_SC_idx_cop_sm_data0          = 8,
         EVL_SC_idx_cop_sm_data1          = 9,
         EVL_SC_idx_cop_sm_ecc            = 10,
         EVL_SC_err_log_ctl               = 11,
         EVL_SC_err_log_info              = 12,
         EVL_SC_err_log_address           = 13,
         EVL_SC_sbe_dbe_counts            = 14,
         EVL_SC_reqq_debug_ctl            = 15,
         EVL_SC_reqq_debug0               = 16,
         EVL_SC_reqq_debug1               = 17,
         EVL_SC_reqq_debug2               = 18,
         EVL_SC_trace_address_enable      = 19,
         EVL_SC_trace_address_value       = 20,
         EVL_SC_trace_ctl                 = 21,
         EVL_SC_perfmon_ctl_status        = 22,
         EVL_SC_perfmon_cyc_cntr          = 23,
         EVL_SC_perfmon_p0_cntr           = 24,
         EVL_SC_perfmon_p1_cntr           = 25,
         EVL_SC_perfmon_p0_qual           = 26,
         EVL_SC_perfmon_p1_qual           = 27
      } evl_sc_reg_type_t;


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_sc_config_reg
   //
   class evl_sc_config_reg extends evl_object;
      evl_sc_reg_type_t m_reg_type;
      evl_paddr_t       m_addr;
      evl_dword_t       m_default;
      evl_dword_t       m_read_only;
      evl_dword_t       m_force_zero;
      evl_dword_t       m_force_one;
      evl_dword_t       m_value;


      //--------------------------------------------------------------------------------------------
      function evl_sc_reg_type_t get_type();
         return m_reg_type;
      endfunction : get_type


      //--------------------------------------------------------------------------------------------
      function evl_paddr_t get_paddr();
         return m_addr;
      endfunction : get_paddr


      //--------------------------------------------------------------------------------------------
      function void set_default(input evl_dword_t dword_in);
         m_default = (dword_in & (~m_force_zero)) | m_force_one;
      endfunction : set_default


      //--------------------------------------------------------------------------------------------
      function evl_dword_t get_default();
         return m_default;
      endfunction : get_default


      //--------------------------------------------------------------------------------------------
      function void set_value(input evl_dword_t dword_in);
         m_value = (dword_in & (~m_force_zero)) | m_force_one;
      endfunction : set_value


      //--------------------------------------------------------------------------------------------
      function evl_dword_t get_value();
         return m_value;
      endfunction : get_value


      //--------------------------------------------------------------------------------------------
      function new(input string            name_in       = "",
                   input evl_sc_reg_type_t reg_type_in   = EVL_SC_l3_shire_swizzle_ctl,
                   input evl_paddr_t       addr_in       = evl_paddr_t'(0),
                   input evl_dword_t       default_in    = evl_dword_t'(0),
                   input evl_dword_t       read_only_in  = evl_dword_t'(0),
                   input evl_dword_t       force_zero_in = evl_dword_t'(0),
                   input evl_dword_t       force_one_in  = evl_dword_t'(0));
         super.new(name_in);
         set_abstract_name(name_in);
         m_reg_type   = reg_type_in;
         m_addr       = addr_in;
         m_read_only  = read_only_in;
         m_force_zero = force_zero_in;
         m_force_one  = force_one_in;
         set_default(default_in);
         set_value(default_in);
      endfunction : new
   endclass : evl_sc_config_reg


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_sc_rand_config_object
   //
   class evl_sc_rand_config_object extends evl_object;

      localparam ESR_SC_SCP_SET_BASE_DEFAULT = 0;

      localparam ESR_SC_SCP_SET_SIZE_DEFAULT = (`SC_CACHE_SIZE_IN_MB == 1) ? (`SC_SETS_PER_SUB_BANK * 12) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 2) ? (`SC_SETS_PER_SUB_BANK * 16) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 3) ? (`SC_SETS_PER_SUB_BANK * 16) / 32 : (`SC_SETS_PER_SUB_BANK * 20) / 32;

      localparam ESR_SC_L2_SET_BASE_DEFAULT  = (`SC_CACHE_SIZE_IN_MB == 1) ? (`SC_SETS_PER_SUB_BANK * 12) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 2) ? (`SC_SETS_PER_SUB_BANK * 16) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 3) ? (`SC_SETS_PER_SUB_BANK * 16) / 32 : (`SC_SETS_PER_SUB_BANK * 20) / 32;

      localparam ESR_SC_L2_SET_SIZE_DEFAULT  = (`SC_CACHE_SIZE_IN_MB == 1) ? (`SC_SETS_PER_SUB_BANK * 16) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 2) ? (`SC_SETS_PER_SUB_BANK * 8)  / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 3) ? (`SC_SETS_PER_SUB_BANK * 8)  / 32 : (`SC_SETS_PER_SUB_BANK * 4) / 32;

      localparam ESR_SC_L3_SET_BASE_DEFAULT  = (`SC_CACHE_SIZE_IN_MB == 1) ? (`SC_SETS_PER_SUB_BANK * 28) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 2) ? (`SC_SETS_PER_SUB_BANK * 24) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 3) ? (`SC_SETS_PER_SUB_BANK * 24) / 32 : (`SC_SETS_PER_SUB_BANK * 24) / 32;

      localparam ESR_SC_L3_SET_SIZE_DEFAULT  = (`SC_CACHE_SIZE_IN_MB == 1) ? (`SC_SETS_PER_SUB_BANK * 4) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 2) ? (`SC_SETS_PER_SUB_BANK * 8) / 32 :
                                               (`SC_CACHE_SIZE_IN_MB == 3) ? (`SC_SETS_PER_SUB_BANK * 8) / 32 : (`SC_SETS_PER_SUB_BANK * 8) / 32;


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      static semaphore m_print_info = new(1);


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      rand bit [3:0]  r_sc_cache_alloc_mode                 = 4'h0;
      rand bit [1:0]  r_sc_l3_shire_swizzle_mode            = 2'h0;

      rand bit        r_sc_all_shire_aliasing               = 1'b0;
      rand bit        r_sc_two_shire_aliasing_use_shire_lsb = 1'b0;

      rand bit [3:0]  r_sc_axi_qos                          = `AXI_QOS_MEM_LOW_PRIORITY;
      rand bit        r_sc_cbuf_enable                      = 1'b1;
      rand bit        r_sc_remote_l3_enable                 = 1'b1;
      rand bit        r_sc_remote_scp_enable                = 1'b1;
      rand bit        r_sc_l2_bypass                        = 1'b0;
      rand bit        r_sc_l3_bypass                        = 1'b0;
      rand bit [5:0]  r_sc_num_l3_reqq_entries              = `SC_REQQ_DEPTH/3;
      rand bit        r_sc_reqq_no_link_list                = 1'b0;
      rand bit        r_sc_ecc_scrub_enable                 = 1'b0;
      rand bit [4:0]  r_sc_l3_yield_priority                = 5'h00;

      rand bit [2:0]  r_sc_ram_delay                        = 3'd`SC_RAM_DELAY_DEFAULT;
      rand bit        r_sc_l2_rbuf_enable                   = 1'b1;
      rand bit        r_sc_scp_rbuf_enable                  = 1'b1;
      rand bit        r_sc_zero_state_enable                = 1'b1;
      rand bit        r_sc_allow_only_1_req_per_sub_bank    = 1'b0;
      rand bit        r_sc_allow_only_1_req_per_bank        = 1'b0;
      rand bit [31:0] r_sc_cbuf_entry_enable                = 32'hffffffff;

      rand bit [11:0] r_sc_scp_set_base                     = ESR_SC_SCP_SET_BASE_DEFAULT[11:0];
      rand bit [12:0] r_sc_scp_set_size                     = ESR_SC_SCP_SET_SIZE_DEFAULT[12:0];

      rand bit [11:0] r_sc_l2_set_base                      = ESR_SC_L2_SET_BASE_DEFAULT[11:0];
      rand bit [12:0] r_sc_l2_set_size                      = ESR_SC_L2_SET_SIZE_DEFAULT[12:0];

      rand bit [11:0] r_sc_l3_set_base                      = ESR_SC_L3_SET_BASE_DEFAULT[11:0];
      rand bit [12:0] r_sc_l3_set_size                      = ESR_SC_L3_SET_SIZE_DEFAULT[12:0];

      rand bit        r_sc_err_rsp_enable                   = 1'b0;
      rand bit [7:0]  r_sc_err_interrupt_enable             = 8'hfe;

      rand bit        r_sc_trace_filter_reqq_id_enable      = 1'b0;
      rand bit [5:0]  r_sc_trace_filter_reqq_id             = {6{1'b0}};
      rand bit        r_sc_trace_filter_port_enable         = 1'b0;
      rand bit [2:0]  r_sc_trace_filter_port                = {3{1'b0}};
      rand bit        r_sc_trace_filter_source_enable       = 1'b0;
      rand bit [7:0]  r_sc_trace_filter_source              = {8{1'b0}};
      rand bit        r_sc_trace_filter_l2_enable           = 1'b0;
      rand bit        r_sc_trace_filter_l3_enable           = 1'b0;
      rand bit        r_sc_trace_filter_fsm_enable          = 1'b0;
      rand bit [11:0] r_sc_trace_type_hot_enable            = {12{1'b0}};

           bit        m_core_present                        = 1'b0;
           bit        m_noc_present                         = 1'b0;

           int        m_sc_scp_set_base                     = -1;
           int        m_sc_scp_set_size                     = -1;

           int        m_sc_l2_set_base                      = -1;
           int        m_sc_l2_set_size                      = -1;

           int        m_sc_l3_set_base                      = -1;
           int        m_sc_l3_set_size                      = -1;

           int        m_sc_l3_shire_swizzle_mode            = -1;

           bit        m_global_config;
           bit        m_use_rand_mode;


      //--------------------------------------------------------------------------------------------
      //
      // Register Storage (for fixing configurations)
      //
      evl_esr_sc_l3_shire_swizzle_ctl_t m_sc_l3_shire_swizzle_ctl;
      evl_esr_sc_reqq_ctl_t             m_sc_reqq_ctl;
      evl_esr_sc_pipe_ctl_t             m_sc_pipe_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_scp_pipe_cache_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_l2_pipe_cache_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_l3_pipe_cache_ctl;
      evl_esr_sc_err_log_ctl_t          m_sc_err_log_ctl;
      evl_esr_sc_trace_ctl_t            m_sc_trace_ctl;


      //--------------------------------------------------------------------------------------------
      //
      // Hard Constraints
      //
      constraint c_sc_ram_delay_hard           { r_sc_ram_delay inside { [2:4] }; }

      constraint c_sc_num_l3_reqq_entries_hard { r_sc_num_l3_reqq_entries <= (`SC_REQQ_DEPTH - 2); }

      constraint c_sc_mode_order_1_hard { solve r_sc_cache_alloc_mode before r_sc_scp_set_base; }
      constraint c_sc_mode_order_2_hard { solve r_sc_cache_alloc_mode before r_sc_scp_set_size; }
      constraint c_sc_mode_order_3_hard { solve r_sc_cache_alloc_mode before r_sc_l2_set_base; }
      constraint c_sc_mode_order_4_hard { solve r_sc_cache_alloc_mode before r_sc_l2_set_size; }
      constraint c_sc_mode_order_5_hard { solve r_sc_cache_alloc_mode before r_sc_l3_set_base; }
      constraint c_sc_mode_order_6_hard { solve r_sc_cache_alloc_mode before r_sc_l3_set_size; }

      constraint c_sc_scp_order_1_hard  { solve r_sc_scp_set_base before r_sc_scp_set_size; }
      constraint c_sc_scp_order_2_hard  { solve r_sc_scp_set_base before r_sc_l2_set_base;  }
      constraint c_sc_scp_order_3_hard  { solve r_sc_scp_set_base before r_sc_l2_set_size;  }
      constraint c_sc_scp_order_4_hard  { solve r_sc_scp_set_size before r_sc_l2_set_base;  }
      constraint c_sc_scp_order_5_hard  { solve r_sc_scp_set_size before r_sc_l2_set_size;  }

      constraint c_sc_scp_set_base_hard { r_sc_scp_set_base < `SC_SETS_PER_SUB_BANK; }
      constraint c_sc_scp_set_size_hard { (r_sc_scp_set_base + r_sc_scp_set_size) <= `SC_SETS_PER_SUB_BANK; }
      constraint c_sc_l2_set_base_hard  { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l2_set_base == 12'h000;
                                          else
                                             (r_sc_l2_set_base >= (r_sc_scp_set_base + r_sc_scp_set_size)) && (r_sc_l2_set_base < `SC_SETS_PER_SUB_BANK); }
      constraint c_sc_l2_set_size_hard  { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l2_set_size == 13'h000;
                                          else
                                             (r_sc_l2_set_base + r_sc_l2_set_size) <= `SC_SETS_PER_SUB_BANK; }
      constraint c_sc_l3_set_base_hard  { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l3_set_base == 12'h000;
                                          else if ((r_sc_l2_set_base  + r_sc_l2_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l3_set_base == 12'h000;
                                          else if (r_sc_l2_set_size == 13'h000)
                                             (r_sc_l3_set_base >= (r_sc_scp_set_base + r_sc_scp_set_size)) && (r_sc_l3_set_base < `SC_SETS_PER_SUB_BANK);
                                          else
                                             (r_sc_l3_set_base >= (r_sc_l2_set_base + r_sc_l2_set_size)) && (r_sc_l3_set_base < `SC_SETS_PER_SUB_BANK); }
      constraint c_sc_l3_set_size_hard  { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l3_set_size == 13'h000;
                                          else if ((r_sc_l2_set_base  + r_sc_l2_set_size) >= `SC_SETS_PER_SUB_BANK)
                                             r_sc_l3_set_size == 13'h000;
                                          else
                                             (r_sc_l3_set_base + r_sc_l3_set_size) <= `SC_SETS_PER_SUB_BANK; }


      //--------------------------------------------------------------------------------------------
      //
      // Normal Constraints
      //
      constraint c_sc_cache_alloc_mode                 { r_sc_cache_alloc_mode                 inside { [0:6] }; }

      constraint c_sc_l3_shire_swizzle_mode            { r_sc_l3_shire_swizzle_mode            inside { [0:2] }; }

      constraint c_sc_all_shire_aliasing               { r_sc_all_shire_aliasing               inside { [0:1] }; }
      constraint c_sc_two_shire_aliasing_use_shire_lsb { r_sc_two_shire_aliasing_use_shire_lsb inside { [0:0] }; }

      constraint c_sc_axi_qos                          { r_sc_axi_qos                          dist { `AXI_QOS_MEM_LOW_PRIORITY := 4, `AXI_QOS_MEM_HIGH_PRIORITY := 4, [0:15] :/ 2 }; }
      constraint c_sc_cbuf_enable                      { r_sc_cbuf_enable                      dist { 1'b1 := 3, 1'b0 := 1}; }
      constraint c_sc_remote_l3_enable                 { r_sc_remote_l3_enable                 dist { 1'b1 := 9, 1'b0 := 1}; }
      constraint c_sc_remote_scp_enable                { r_sc_remote_scp_enable                dist { 1'b1 := 9, 1'b0 := 1}; }
      constraint c_sc_l2_bypass                        { r_sc_l2_bypass                        dist { 1'b0 := 9, 1'b1 := 1}; }
      constraint c_sc_l3_bypass                        { r_sc_l3_bypass                        dist { 1'b0 := 9, 1'b1 := 1}; }
      constraint c_sc_num_l3_reqq_entries              { r_sc_num_l3_reqq_entries              inside { [6'h02:6'h3f] }; }
      constraint c_sc_reqq_no_link_list                { r_sc_reqq_no_link_list                == 1'b0; }
      constraint c_sc_ecc_scrub_enable                 { r_sc_ecc_scrub_enable                 inside { [0:1] }; }
      constraint c_sc_l3_yield_priority                { r_sc_l3_yield_priority                inside { [0:31] }; }

      constraint c_sc_ram_delay                        { r_sc_ram_delay                        inside { [2:4] }; }
      constraint c_sc_l2_rbuf_enable                   { r_sc_l2_rbuf_enable                   inside { [0:1] }; }
      constraint c_sc_scp_rbuf_enable                  { r_sc_scp_rbuf_enable                  inside { [0:1] }; }
      constraint c_sc_zero_state_enable                { r_sc_zero_state_enable                inside { [0:1] }; }
      constraint c_sc_allow_only_1_req_per_sub_bank    { r_sc_allow_only_1_req_per_sub_bank    dist { 1'b0 := 9, 1'b1 := 1 }; }
      constraint c_sc_allow_only_1_req_per_bank        { r_sc_allow_only_1_req_per_bank        dist { 1'b0 := 9, 1'b1 := 1 }; }
      constraint c_sc_cbuf_entry_enable                { r_sc_cbuf_entry_enable                dist { 32'h00000001:=1, 32'h00000003:=1, 32'h00000007:=1, 32'h0000000f:=1,
                                                                                                      32'h0000001f:=1, 32'h0000003f:=1, 32'h0000007f:=1, 32'h000000ff:=1,
                                                                                                      32'h000001ff:=1, 32'h000003ff:=1, 32'h000007ff:=1, 32'h00000fff:=1,
                                                                                                      32'h00001fff:=1, 32'h00003fff:=1, 32'h00007fff:=1, 32'h0000ffff:=1,
                                                                                                      32'h0001ffff:=1, 32'h0003ffff:=1, 32'h0007ffff:=1, 32'h000fffff:=1,
                                                                                                      32'h001fffff:=1, 32'h003fffff:=1, 32'h007fffff:=1, 32'h00ffffff:=1,
                                                                                                      32'h01ffffff:=1, 32'h03ffffff:=1, 32'h07ffffff:=1, 32'h0fffffff:=1,
                                                                                                      32'h1fffffff:=1, 32'h3fffffff:=1, 32'h7fffffff:=1, 32'hffffffff:=1,
                                                                                                      [32'h00000001:32'hffffffff]:/32, [32'h0001:32'h00ff]:/32 }; }

      constraint c_sc_err_rsp_enable                   { r_sc_err_rsp_enable                   inside { [0:1] }; }
      constraint c_sc_err_interrupt_enable             { r_sc_err_interrupt_enable == 8'hfe; } // inside { [8'h00:8'hff] }; }

      constraint c_sc_trace_filter_reqq_id_enable      { r_sc_trace_filter_reqq_id_enable      inside { [0:1] }; }
      constraint c_sc_trace_filter_reqq_id             { r_sc_trace_filter_reqq_id             inside { [6'h00:6'h3f] }; }
      constraint c_sc_trace_filter_port_enable         { r_sc_trace_filter_port_enable         inside { [0:1] }; }
      constraint c_sc_trace_filter_port                { r_sc_trace_filter_port                inside { [3'b000:3'b111] }; }
      constraint c_sc_trace_filter_source_enable       { r_sc_trace_filter_source_enable       inside { [0:1] }; }
      constraint c_sc_trace_filter_source              { r_sc_trace_filter_source              inside { [8'h00:8'hff] }; }
      constraint c_sc_trace_filter_l2_enable           { r_sc_trace_filter_l2_enable           inside { [0:1] }; }
      constraint c_sc_trace_filter_l3_enable           { r_sc_trace_filter_l3_enable           inside { [0:1] }; }
      constraint c_sc_trace_filter_fsm_enable          { r_sc_trace_filter_fsm_enable          inside { [0:1] }; }
      constraint c_sc_trace_type_hot_enable            { r_sc_trace_type_hot_enable            inside { [12'h000:12'hfff] }; }

      constraint c_sc_scp_set_base                     { if (r_sc_cache_alloc_mode < 6)
                                                            r_sc_scp_set_base == 12'h000;
                                                         else
                                                            r_sc_scp_set_base inside { [12'h000:12'hfff] }; }
      constraint c_sc_scp_set_size                     { if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 1))
                                                            r_sc_scp_set_size == (`SC_SETS_PER_SUB_BANK * 12) / 32;
                                                         else if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 2))
                                                            r_sc_scp_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 3))
                                                            r_sc_scp_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else if (r_sc_cache_alloc_mode == 0)
                                                            r_sc_scp_set_size == (`SC_SETS_PER_SUB_BANK * 20) / 32;
                                                         else if (r_sc_cache_alloc_mode < 4)
                                                            r_sc_scp_set_size == 13'h000;
                                                         else if (r_sc_cache_alloc_mode == 4)
                                                            r_sc_scp_set_size == `SC_SETS_PER_SUB_BANK;
                                                         else if (r_sc_cache_alloc_mode == 5)
                                                            r_sc_scp_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else
                                                            r_sc_scp_set_size inside { [13'h000:13'hfff] }; }

      constraint c_sc_l2_set_base                      { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                                            r_sc_l2_set_base == 12'h000;
                                                         else if (r_sc_cache_alloc_mode < 6)
                                                            r_sc_l2_set_base == (r_sc_scp_set_base + r_sc_scp_set_size);
                                                         else
                                                            r_sc_l2_set_base inside { [12'h000:12'hfff] }; }
      constraint c_sc_l2_set_size                      { if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 1))
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 2))
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 4;
                                                         else if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 3))
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 4;
                                                         else if (r_sc_cache_alloc_mode == 0)
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 8;
                                                         else if (r_sc_cache_alloc_mode == 1)
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else if (r_sc_cache_alloc_mode == 2)
                                                            r_sc_l2_set_size == (`SC_SETS_PER_SUB_BANK * 31) / 32;
                                                         else if (r_sc_cache_alloc_mode == 3)
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK;
                                                         else if (r_sc_cache_alloc_mode == 4)
                                                            r_sc_l2_set_size == 13'h000;
                                                         else if (r_sc_cache_alloc_mode == 5)
                                                            r_sc_l2_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else
                                                            r_sc_l2_set_size inside { [13'h000:13'hfff] }; }

      constraint c_sc_l3_set_base                      { if ((r_sc_scp_set_base + r_sc_scp_set_size) >= `SC_SETS_PER_SUB_BANK)
                                                            r_sc_l3_set_base == 12'h000;
                                                         else if ((r_sc_l2_set_base  + r_sc_l2_set_size) >= `SC_SETS_PER_SUB_BANK)
                                                            r_sc_l3_set_base == 12'h000;
                                                         else if (r_sc_cache_alloc_mode < 6)
                                                            r_sc_l3_set_base == (r_sc_l2_set_base + r_sc_l2_set_size);
                                                         else
                                                            r_sc_l3_set_base inside { [12'h000:12'hfff] }; }
      constraint c_sc_l3_set_size                      { if ((r_sc_cache_alloc_mode == 0) && (`SC_CACHE_SIZE_IN_MB == 1))
                                                            r_sc_l3_set_size == `SC_SETS_PER_SUB_BANK / 8;
                                                         else if (r_sc_cache_alloc_mode == 0)
                                                            r_sc_l3_set_size == `SC_SETS_PER_SUB_BANK / 4;
                                                         else if (r_sc_cache_alloc_mode == 1)
                                                            r_sc_l3_set_size == `SC_SETS_PER_SUB_BANK / 2;
                                                         else if (r_sc_cache_alloc_mode == 2)
                                                            r_sc_l3_set_size == `SC_SETS_PER_SUB_BANK / 32;
                                                         else if (r_sc_cache_alloc_mode == 3)
                                                            r_sc_l3_set_size == 13'h000;
                                                         else if (r_sc_cache_alloc_mode == 4)
                                                            r_sc_l3_set_size == 13'h000;
                                                         else if (r_sc_cache_alloc_mode == 5)
                                                            r_sc_l3_set_size == 13'h000;
                                                         else
                                                            r_sc_l3_set_size inside { [13'h000:13'hfff] }; }


      //--------------------------------------------------------------------------------------------
      function void assign_swizzle(ref evl_esr_sc_l3_shire_swizzle_ctl_t sc_l3_shire_swizzle_ctl);
         sc_l3_shire_swizzle_ctl = evl_esr_sc_l3_shire_swizzle_ctl_t'(0);
         sc_l3_shire_swizzle_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = 1'b0;
         sc_l3_shire_swizzle_ctl.esr_sc_all_shire_aliasing               = 1'b0;
         sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2                  = (`SC_BANKS > 4) ? 4'd10 : 4'd9;
         sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1                  = (`SC_BANKS > 4) ? 4'd9  : 4'd8;
         sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0                  = (`SC_BANKS > 4) ? 4'd8  : 4'd7;
         sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2                      = 4'd7;
         sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1                      = 4'd6;
         sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0                      = 4'd5;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5                     = 4'd5;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4                     = 4'd4;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3                     = 4'd3;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2                     = 4'd2;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1                     = 4'd1;
         sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0                     = 4'd0;
      endfunction : assign_swizzle


      //--------------------------------------------------------------------------------------------
      function void fix_masks(ref evl_esr_sc_pipe_cache_ctl_t scp_pipe_cache_ctl,
                              ref evl_esr_sc_pipe_cache_ctl_t l2_pipe_cache_ctl,
                              ref evl_esr_sc_pipe_cache_ctl_t l3_pipe_cache_ctl);
         if (scp_pipe_cache_ctl.set_size == 13'h000) begin
            scp_pipe_cache_ctl.set_base = 12'h000;
         end
         if (l2_pipe_cache_ctl.set_size == 13'h000) begin
            l2_pipe_cache_ctl.set_base = 12'h000;
         end
         if (l3_pipe_cache_ctl.set_size == 13'h000) begin
            l3_pipe_cache_ctl.set_base = 12'h000;
         end
         scp_pipe_cache_ctl.set_mask = (1 << $clog2(scp_pipe_cache_ctl.set_size)) - 1;
         l2_pipe_cache_ctl.set_mask  = (1 << $clog2(l2_pipe_cache_ctl.set_size))  - 1;
         l3_pipe_cache_ctl.set_mask  = (1 << $clog2(l3_pipe_cache_ctl.set_size))  - 1;
         if (scp_pipe_cache_ctl.set_size == (scp_pipe_cache_ctl.set_mask + 1)) begin
            scp_pipe_cache_ctl.tag_mask = scp_pipe_cache_ctl.set_mask;
         end
         else begin
            scp_pipe_cache_ctl.tag_mask = scp_pipe_cache_ctl.set_mask >> 1;
         end
         if (l2_pipe_cache_ctl.set_size == (l2_pipe_cache_ctl.set_mask + 1)) begin
            l2_pipe_cache_ctl.tag_mask = l2_pipe_cache_ctl.set_mask;
         end
         else begin
            l2_pipe_cache_ctl.tag_mask = l2_pipe_cache_ctl.set_mask >> 1;
         end
         if (l3_pipe_cache_ctl.set_size == (l3_pipe_cache_ctl.set_mask + 1)) begin
            l3_pipe_cache_ctl.tag_mask = l3_pipe_cache_ctl.set_mask;
         end
         else begin
            l3_pipe_cache_ctl.tag_mask = l3_pipe_cache_ctl.set_mask >> 1;
         end
      endfunction : fix_masks


      //--------------------------------------------------------------------------------------------
      function void post_randomize();
         m_sc_reqq_ctl.esr_sc_axi_qos             = r_sc_axi_qos;
         m_sc_reqq_ctl.esr_sc_cbuf_enable         = r_sc_cbuf_enable;
         m_sc_reqq_ctl.esr_sc_remote_l3_enable    = r_sc_remote_l3_enable;
         m_sc_reqq_ctl.esr_sc_remote_scp_enable   = r_sc_remote_scp_enable;
         m_sc_reqq_ctl.esr_sc_l2_bypass           = r_sc_l2_bypass;
         m_sc_reqq_ctl.esr_sc_l3_bypass           = r_sc_l3_bypass;
         m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries = r_sc_num_l3_reqq_entries;
         m_sc_reqq_ctl.esr_sc_reqq_no_link_list   = r_sc_reqq_no_link_list;
         m_sc_reqq_ctl.esr_sc_ecc_scrub_enable    = r_sc_ecc_scrub_enable;
         m_sc_reqq_ctl.esr_sc_l3_yield_priority   = r_sc_l3_yield_priority;

         m_sc_pipe_ctl.esr_sc_idx_cop_sm_ctl_user_en        = 1'b1;
         m_sc_pipe_ctl.esr_sc_ram_deep_sleep                = 1'b0;
         m_sc_pipe_ctl.esr_sc_ram_shut_down                 = 1'b0;
         m_sc_pipe_ctl.esr_sc_ram_delay                     = r_sc_ram_delay;
         m_sc_pipe_ctl.esr_sc_l2_rbuf_enable                = r_sc_l2_rbuf_enable;
         m_sc_pipe_ctl.esr_sc_scp_rbuf_enable               = r_sc_scp_rbuf_enable;
         m_sc_pipe_ctl.esr_sc_zero_state_enable             = r_sc_zero_state_enable;
         m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_sub_bank = r_sc_allow_only_1_req_per_sub_bank;
         m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_bank     = r_sc_allow_only_1_req_per_bank;
         m_sc_pipe_ctl.esr_sc_cbuf_entry_enable             = r_sc_cbuf_entry_enable;

         // When there's only 1 req per bank/subbank and the l3 is busy the l2 can get totally starved.
         // (Also if there are very few l2 reqq entries)
         // To prevent this make sure l3_yield_priority is yielding to the L2 often.
         if (((m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_sub_bank == 1) ||
             (m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_bank) || ((`SC_REQQ_DEPTH - r_sc_num_l3_reqq_entries) < 10))
             && ((m_sc_reqq_ctl.esr_sc_l3_yield_priority == 0) || (m_sc_reqq_ctl.esr_sc_l3_yield_priority > 10))) begin
            m_sc_reqq_ctl.esr_sc_l3_yield_priority = 2;
         end


         m_sc_err_log_ctl.esr_sc_err_rsp_enable             = r_sc_err_rsp_enable;
         m_sc_err_log_ctl.esr_sc_err_interrupt_enable       = r_sc_err_interrupt_enable;

         m_sc_trace_ctl.esr_sc_trace_filter_reqq_id_enable  = r_sc_trace_filter_reqq_id_enable;
         m_sc_trace_ctl.esr_sc_trace_filter_reqq_id         = r_sc_trace_filter_reqq_id;
         m_sc_trace_ctl.esr_sc_trace_filter_port_enable     = r_sc_trace_filter_port_enable;
         m_sc_trace_ctl.esr_sc_trace_filter_port            = r_sc_trace_filter_port;
         m_sc_trace_ctl.esr_sc_trace_filter_source_enable   = r_sc_trace_filter_source_enable;
         m_sc_trace_ctl.esr_sc_trace_filter_source          = r_sc_trace_filter_source;
         m_sc_trace_ctl.esr_sc_trace_filter_l2_enable       = r_sc_trace_filter_l2_enable;
         m_sc_trace_ctl.esr_sc_trace_filter_l3_enable       = r_sc_trace_filter_l3_enable;
         m_sc_trace_ctl.esr_sc_trace_filter_fsm_enable      = r_sc_trace_filter_fsm_enable;
         m_sc_trace_ctl.esr_sc_trace_type_hot_enable        = r_sc_trace_type_hot_enable;

         if (r_sc_l3_shire_swizzle_mode == 0) begin
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0    = 4'h0;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0     = (`SC_L3_SHIRES <= 32) ? m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 + 4'h1 :
                                                                                       m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 = (`SC_BANKS <= 4) ? m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1 :
                                                                                  m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 + 4'h1;
         end
         else if (r_sc_l3_shire_swizzle_mode == 1) begin
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0     = 4'd0;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0    = (`SC_BANKS <= 4) ? m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1 :
                                                                                  m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 = (`SC_L3_SHIRES <= 32) ? m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 + 4'h1 :
                                                                                       m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 + 4'h1;
         end
         else if (r_sc_l3_shire_swizzle_mode == 2) begin
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0     = 4'd0;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2     = m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 = (`SC_BANKS <= 4) ? m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 + 4'h1 :
                                                                                  m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2 = m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0    = (`SC_SUB_BANKS <= 4) ? m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 + 4'h1 :
                                                                                      m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3 + 4'h1;
            m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5    = m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 + 4'h1;
         end
         m_sc_l3_shire_swizzle_ctl.esr_sc_all_shire_aliasing               = r_sc_all_shire_aliasing;
         m_sc_l3_shire_swizzle_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = r_sc_two_shire_aliasing_use_shire_lsb;
         if (m_sc_l3_shire_swizzle_mode < 0) begin
            longint value;

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b0");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b1");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b2");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b0");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b1");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b2");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b0");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b1");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b2");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b3");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b4");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4 = value[3:0];
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b5");
            if (value != -1) begin
               m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5 = value[3:0];
            end
         end

         //
         // Assign the cache configuration fields
         //
         if ((m_sc_l3_shire_swizzle_ctl.esr_sc_all_shire_aliasing == 1'b1) && (r_sc_l3_set_size > 0)) begin
            int l3_subbanks_clog2;
            int tag_size;

            l3_subbanks_clog2 = $clog2(r_sc_l3_set_size);
            if ((1 << l3_subbanks_clog2) > r_sc_l3_set_size) begin
               l3_subbanks_clog2--;
            end
            tag_size = `EVL_PADDR_MSB + 1 - (`EVL_LINE_ADDR_LSB + $clog2(`SC_BANKS) + $clog2(`SC_SUB_BANKS) + l3_subbanks_clog2);
            if (tag_size > `SC_TAG_MAX_SIZE) begin
               int set_modification;

               set_modification = (1 << (tag_size - `SC_TAG_MAX_SIZE)) * (1 << l3_subbanks_clog2);
               r_sc_l3_set_size = r_sc_l3_set_size + set_modification;
               r_sc_l3_set_base = r_sc_l3_set_base - set_modification;
               if (r_sc_l2_set_size > 0) begin
                  if (r_sc_l2_set_base >= set_modification) begin
                     r_sc_l2_set_base  = r_sc_l2_set_base - set_modification;
                     r_sc_scp_set_size = r_sc_l2_set_base;
                  end
                  else begin
                     r_sc_l2_set_base  = 0;
                     r_sc_l2_set_size  = r_sc_l3_set_base;
                     r_sc_scp_set_base = 0;
                     r_sc_scp_set_size = 0;
                  end
               end
               else begin
                  r_sc_scp_set_size = r_sc_l3_set_base;
               end
            end
         end
         m_scp_pipe_cache_ctl.set_base = r_sc_scp_set_base;
         m_scp_pipe_cache_ctl.set_size = r_sc_scp_set_size;
         m_l2_pipe_cache_ctl.set_base  = r_sc_l2_set_base;
         m_l2_pipe_cache_ctl.set_size  = r_sc_l2_set_size;
         m_l3_pipe_cache_ctl.set_base  = r_sc_l3_set_base;
         m_l3_pipe_cache_ctl.set_size  = r_sc_l3_set_size;
         fix_masks(m_scp_pipe_cache_ctl, m_l2_pipe_cache_ctl, m_l3_pipe_cache_ctl);

         if ((m_scp_pipe_cache_ctl.set_size != 0) || (m_l3_pipe_cache_ctl.set_size != 0) || (m_sc_reqq_ctl.esr_sc_remote_l3_enable  == 1'b1) || (m_sc_reqq_ctl.esr_sc_remote_scp_enable == 1'b1)) begin
            if (m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries > (`SC_REQQ_DEPTH - 3)) begin
               m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries = `SC_REQQ_DEPTH - 3;
            end
            if (m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries < 3) begin
               m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries = 3;
            end
         end
      endfunction : post_randomize


      //--------------------------------------------------------------------------------------------
      task print_config();
         if (m_global_config == 1'b1) begin
            `evl_log_no_time(UVM_LOW, VMOD_CONFIG, "")
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("Global Shire Cache Config:"))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   shire cache mode:    %0d", r_sc_cache_alloc_mode))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_scp_set_base: 0x%x", m_scp_pipe_cache_ctl.set_base))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_scp_set_size: 0x%x", m_scp_pipe_cache_ctl.set_size))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_scp_set_mask: 0x%x", m_scp_pipe_cache_ctl.set_mask))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_scp_tag_mask: 0x%x", m_scp_pipe_cache_ctl.tag_mask))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_set_base:  0x%x", m_l2_pipe_cache_ctl.set_base))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_set_size:  0x%x", m_l2_pipe_cache_ctl.set_size))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_set_mask:  0x%x", m_l2_pipe_cache_ctl.set_mask))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_tag_mask:  0x%x", m_l2_pipe_cache_ctl.tag_mask))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_set_base:  0x%x", m_l3_pipe_cache_ctl.set_base))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_set_size:  0x%x", m_l3_pipe_cache_ctl.set_size))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_set_mask:  0x%x", m_l3_pipe_cache_ctl.set_mask))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_tag_mask:  0x%x", m_l3_pipe_cache_ctl.tag_mask))
            `evl_log_no_time(UVM_LOW, VMOD_CONFIG, "")
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   shire L3 swizzle mode:                   %0d", r_sc_l3_shire_swizzle_mode))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_two_shire_aliasing_use_shire_lsb: %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_two_shire_aliasing_use_shire_lsb))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_all_shire_aliasing:               %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_all_shire_aliasing))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_sub_bank_sel_b2:                  %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b2))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_sub_bank_sel_b1:                  %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b1))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_sub_bank_sel_b0:                  %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_sub_bank_sel_b0))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_bank_sel_b2:                      %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b2))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_bank_sel_b1:                      %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b1))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_bank_sel_b0:                      %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_bank_sel_b0))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b5:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b5))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b4:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b4))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b3:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b3))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b2:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b2))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b1:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b1))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_shire_sel_b0:                     %0d", m_sc_l3_shire_swizzle_ctl.esr_sc_shire_sel_b0))
            `evl_log_no_time(UVM_LOW, VMOD_CONFIG, "")
         end
         else begin
            m_print_info.get();
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("Local Shire Cache Config:"))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_axi_qos:                       0x%x", m_sc_reqq_ctl.esr_sc_axi_qos))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_cbuf_enable:                   %0d", m_sc_reqq_ctl.esr_sc_cbuf_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_remote_l3_enable:              %0d", m_sc_reqq_ctl.esr_sc_remote_l3_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_remote_scp_enable:             %0d", m_sc_reqq_ctl.esr_sc_remote_scp_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_bypass:                     %0d", m_sc_reqq_ctl.esr_sc_l2_bypass))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_bypass:                     %0d", m_sc_reqq_ctl.esr_sc_l3_bypass))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_num_l3_reqq_entries:           0x%x", m_sc_reqq_ctl.esr_sc_num_l3_reqq_entries))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_reqq_no_link_list:             %0d", m_sc_reqq_ctl.esr_sc_reqq_no_link_list))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_ecc_scrub_enable:              %0d", m_sc_reqq_ctl.esr_sc_ecc_scrub_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l3_yield_priority:             %0d", m_sc_reqq_ctl.esr_sc_l3_yield_priority))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_ram_delay:                     0x%x", m_sc_pipe_ctl.esr_sc_ram_delay))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_l2_rbuf_enable:                %0d", m_sc_pipe_ctl.esr_sc_l2_rbuf_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_scp_rbuf_enable:               %0d", m_sc_pipe_ctl.esr_sc_scp_rbuf_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_zero_state_enable:             %0d", m_sc_pipe_ctl.esr_sc_zero_state_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_allow_only_1_req_per_sub_bank: %0d", m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_sub_bank))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_allow_only_1_req_per_bank:     %0d", m_sc_pipe_ctl.esr_sc_allow_only_1_req_per_bank))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_cbuf_entry_enable:             0x%x", m_sc_pipe_ctl.esr_sc_cbuf_entry_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_err_rsp_enable:                %0d", m_sc_err_log_ctl.esr_sc_err_rsp_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_err_interrupt_enable:          0x%x", m_sc_err_log_ctl.esr_sc_err_interrupt_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_reqq_id_enable:   %0d", m_sc_trace_ctl.esr_sc_trace_filter_reqq_id_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_reqq_id:          0x%x", m_sc_trace_ctl.esr_sc_trace_filter_reqq_id))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_port_enable:      %0d", m_sc_trace_ctl.esr_sc_trace_filter_port_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_port:             0x%x", m_sc_trace_ctl.esr_sc_trace_filter_port))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_source_enable:    %0d", m_sc_trace_ctl.esr_sc_trace_filter_source_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_source:           0x%x", m_sc_trace_ctl.esr_sc_trace_filter_source))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_l2_enable:        %0d", m_sc_trace_ctl.esr_sc_trace_filter_l2_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_l3_enable:        %0d", m_sc_trace_ctl.esr_sc_trace_filter_l3_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_filter_fsm_enable:       %0d", m_sc_trace_ctl.esr_sc_trace_filter_fsm_enable))
            `evl_log(UVM_LOW, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_sc_trace_type_hot_enable:         0x%x", m_sc_trace_ctl.esr_sc_trace_type_hot_enable))
            `evl_log_no_time(UVM_LOW, VMOD_CONFIG, "")
         end
         m_print_info.put();
      endtask : print_config


      //--------------------------------------------------------------------------------------------
      function new(input string name_in = "evl_sc_rand_config_object", input bit global_config_in = 1'b0);
         longint value;

         super.new(name_in);
         set_abstract_name(name_in);
         m_global_config = global_config_in;

         if (evl_verif_comp_map::get_verif_comp(CORE_RTL_AGENT) != null) begin
            m_core_present = 1'b1;
         end

         if (evl_verif_comp_map::get_verif_comp(NOC_RTL_AGENT) != null) begin
            m_noc_present = 1'b1;
         end

         assign_swizzle(m_sc_l3_shire_swizzle_ctl);

         if (`et_test_plusargs("scConfig=fixed") != 0) begin
            m_use_rand_mode = 1'b0;
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("sc_rand_config");
            if (value == -1) begin
               m_use_rand_mode = 1'b1;
            end
            else begin
               m_use_rand_mode = value[0:0];
            end
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_axi_qos");
         if (value != -1) begin
            r_sc_axi_qos = value[0:0];
            r_sc_axi_qos.rand_mode(0);
            c_sc_axi_qos.constraint_mode(0);
         end

         value = (m_core_present == 1'b1) ? 1 : m_global_attrs.get_plus_arg_value_as_int("esr_sc_cbuf_enable");
         if (value != -1) begin
            r_sc_cbuf_enable = value[0:0];
            r_sc_cbuf_enable.rand_mode(0);
            c_sc_cbuf_enable.constraint_mode(0);
         end

         value = (m_core_present == 1'b1) ? 1 : m_global_attrs.get_plus_arg_value_as_int("esr_sc_remote_l3_enable");
         if (value != -1) begin
            r_sc_remote_l3_enable = value[0:0];
            r_sc_remote_l3_enable.rand_mode(0);
            c_sc_remote_l3_enable.constraint_mode(0);
         end

         value = (m_core_present == 1'b1) ? 1 : m_global_attrs.get_plus_arg_value_as_int("esr_sc_remote_scp_enable");
         if (value != -1) begin
            r_sc_remote_scp_enable = value[0:0];
            r_sc_remote_scp_enable.rand_mode(0);
            c_sc_remote_scp_enable.constraint_mode(0);
         end

         value = (m_core_present == 1'b1) ? 0 : m_global_attrs.get_plus_arg_value_as_int("esr_sc_l2_bypass");
         if (value != -1) begin
            r_sc_l2_bypass = value[0:0];
            r_sc_l2_bypass.rand_mode(0);
            c_sc_l2_bypass.constraint_mode(0);
         end

         value = (m_core_present == 1'b1) ? 0 : m_global_attrs.get_plus_arg_value_as_int("esr_sc_l3_bypass");
         if (value != -1) begin
            r_sc_l3_bypass = value[0:0];
            r_sc_l3_bypass.rand_mode(0);
            c_sc_l3_bypass.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_num_l3_reqq_entries");
         if (value != -1) begin
            r_sc_num_l3_reqq_entries = value[5:0];
            r_sc_num_l3_reqq_entries.rand_mode(0);
            c_sc_num_l3_reqq_entries.constraint_mode(0);
            if (r_sc_num_l3_reqq_entries >= (`SC_REQQ_DEPTH - 2)) begin
               r_sc_num_l3_reqq_entries = (`SC_REQQ_DEPTH - 2);
            end
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_reqq_no_link_list");
         if (value != -1) begin
            r_sc_reqq_no_link_list = value[0:0];
            r_sc_reqq_no_link_list.rand_mode(0);
            c_sc_reqq_no_link_list.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_ecc_scrub_enable");
         if (value != -1) begin
            r_sc_ecc_scrub_enable = value[0:0];
            r_sc_ecc_scrub_enable.rand_mode(0);
            c_sc_ecc_scrub_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l3_yield_priority");
         if (value != -1) begin
            r_sc_l3_yield_priority = value[4:0];
            r_sc_l3_yield_priority.rand_mode(0);
            c_sc_l3_yield_priority.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_ram_delay");
         if (value != -1) begin
            r_sc_ram_delay = value[2:0];
            r_sc_ram_delay.rand_mode(0);
            c_sc_ram_delay.constraint_mode(0);
            if (r_sc_ram_delay < 3'h2) begin
               r_sc_ram_delay = 3'h2;
            end
            else if (r_sc_ram_delay > 3'h4) begin
               r_sc_ram_delay = 3'h4;
            end
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l2_rbuf_enable");
         if (value != -1) begin
            r_sc_l2_rbuf_enable = value[0:0];
            r_sc_l2_rbuf_enable.rand_mode(0);
            c_sc_l2_rbuf_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_scp_rbuf_enable");
         if (value != -1) begin
            r_sc_scp_rbuf_enable = value[0:0];
            r_sc_scp_rbuf_enable.rand_mode(0);
            c_sc_scp_rbuf_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_zero_state_enable");
         if (value != -1) begin
            r_sc_zero_state_enable = value[0:0];
            r_sc_zero_state_enable.rand_mode(0);
            c_sc_zero_state_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_allow_only_1_req_per_sub_bank");
         if (value != -1) begin
            r_sc_allow_only_1_req_per_sub_bank = value[0:0];
            r_sc_allow_only_1_req_per_sub_bank.rand_mode(0);
            c_sc_allow_only_1_req_per_sub_bank.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_allow_only_1_req_per_bank");
         if (value != -1) begin
            r_sc_allow_only_1_req_per_bank = value[0:0];
            r_sc_allow_only_1_req_per_bank.rand_mode(0);
            c_sc_allow_only_1_req_per_bank.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_cbuf_entry_enable");
         if (value != -1) begin
            r_sc_cbuf_entry_enable = value;
            r_sc_cbuf_entry_enable.rand_mode(0);
            c_sc_cbuf_entry_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_err_rsp_enable");
         if (value != -1) begin
            r_sc_err_rsp_enable = value[0:0];
            r_sc_err_rsp_enable.rand_mode(0);
            c_sc_err_rsp_enable.constraint_mode(0);
         end
         else if (m_core_present == 1'b1) begin
            r_sc_err_rsp_enable.rand_mode(0);
            c_sc_err_rsp_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_err_interrupt_enable");
         if (value != -1) begin
            r_sc_err_interrupt_enable = value[7:0];
            r_sc_err_interrupt_enable.rand_mode(0);
            c_sc_err_interrupt_enable.constraint_mode(0);
         end
         else if (m_core_present == 1'b1) begin
            r_sc_err_interrupt_enable.rand_mode(0);
            c_sc_err_interrupt_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_reqq_id_enable");
         if (value != -1) begin
            r_sc_trace_filter_reqq_id_enable = value[0:0];
            r_sc_trace_filter_reqq_id_enable.rand_mode(0);
            c_sc_trace_filter_reqq_id_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_reqq_id");
         if (value != -1) begin
            r_sc_trace_filter_reqq_id = value[5:0];
            r_sc_trace_filter_reqq_id.rand_mode(0);
            c_sc_trace_filter_reqq_id.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_port_enable");
         if (value != -1) begin
            r_sc_trace_filter_port_enable = value[0:0];
            r_sc_trace_filter_port_enable.rand_mode(0);
            c_sc_trace_filter_port_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_port");
         if (value != -1) begin
            r_sc_trace_filter_port = value[2:0];
            r_sc_trace_filter_port.rand_mode(0);
            c_sc_trace_filter_port.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_source_enable");
         if (value != -1) begin
            r_sc_trace_filter_source_enable = value[0:0];
            r_sc_trace_filter_source_enable.rand_mode(0);
            c_sc_trace_filter_source_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_source");
         if (value != -1) begin
            r_sc_trace_filter_source = value[7:0];
            r_sc_trace_filter_source.rand_mode(0);
            c_sc_trace_filter_source.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_l2_enable");
         if (value != -1) begin
            r_sc_trace_filter_l2_enable = value[0:0];
            r_sc_trace_filter_l2_enable.rand_mode(0);
            c_sc_trace_filter_l2_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_l3_enable");
         if (value != -1) begin
            r_sc_trace_filter_l3_enable = value[0:0];
            r_sc_trace_filter_l3_enable.rand_mode(0);
            c_sc_trace_filter_l3_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_filter_fsm_enable");
         if (value != -1) begin
            r_sc_trace_filter_fsm_enable = value[0:0];
            r_sc_trace_filter_fsm_enable.rand_mode(0);
            c_sc_trace_filter_fsm_enable.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_trace_type_hot_enable");
         if (value != -1) begin
            r_sc_trace_type_hot_enable = value[11:0];
            r_sc_trace_type_hot_enable.rand_mode(0);
            c_sc_trace_type_hot_enable.constraint_mode(0);
         end

         if (m_use_rand_mode == 1'b0) begin
            r_sc_axi_qos.rand_mode(0);
            c_sc_axi_qos.constraint_mode(0);
            r_sc_cbuf_enable.rand_mode(0);
            c_sc_cbuf_enable.constraint_mode(0);
            r_sc_remote_l3_enable.rand_mode(0);
            c_sc_remote_l3_enable.constraint_mode(0);
            r_sc_remote_scp_enable.rand_mode(0);
            c_sc_remote_scp_enable.constraint_mode(0);
            r_sc_l2_bypass.rand_mode(0);
            c_sc_l2_bypass.constraint_mode(0);
            r_sc_l3_bypass.rand_mode(0);
            c_sc_l3_bypass.constraint_mode(0);
            r_sc_num_l3_reqq_entries.rand_mode(0);
            c_sc_num_l3_reqq_entries.constraint_mode(0);
            r_sc_reqq_no_link_list.rand_mode(0);
            c_sc_reqq_no_link_list.constraint_mode(0);
            r_sc_ecc_scrub_enable.rand_mode(0);
            c_sc_ecc_scrub_enable.constraint_mode(0);
            r_sc_l3_yield_priority.rand_mode(0);
            c_sc_l3_yield_priority.constraint_mode(0);
            r_sc_ram_delay.rand_mode(0);
            c_sc_ram_delay.constraint_mode(0);
            r_sc_l2_rbuf_enable.rand_mode(0);
            c_sc_l2_rbuf_enable.constraint_mode(0);
            r_sc_scp_rbuf_enable.rand_mode(0);
            c_sc_scp_rbuf_enable.constraint_mode(0);
            r_sc_zero_state_enable.rand_mode(0);
            c_sc_zero_state_enable.constraint_mode(0);
            r_sc_allow_only_1_req_per_sub_bank.rand_mode(0);
            c_sc_allow_only_1_req_per_sub_bank.constraint_mode(0);
            r_sc_allow_only_1_req_per_bank.rand_mode(0);
            c_sc_allow_only_1_req_per_bank.constraint_mode(0);
            r_sc_cbuf_entry_enable.rand_mode(0);
            c_sc_cbuf_entry_enable.constraint_mode(0);
            r_sc_err_rsp_enable.rand_mode(0);
            c_sc_err_rsp_enable.constraint_mode(0);
            r_sc_err_interrupt_enable.rand_mode(0);
            c_sc_err_interrupt_enable.constraint_mode(0);
            r_sc_trace_filter_reqq_id_enable.rand_mode(0);
            c_sc_trace_filter_reqq_id_enable.constraint_mode(0);
            r_sc_trace_filter_reqq_id.rand_mode(0);
            c_sc_trace_filter_reqq_id.constraint_mode(0);
            r_sc_trace_filter_port_enable.rand_mode(0);
            c_sc_trace_filter_port_enable.constraint_mode(0);
            r_sc_trace_filter_port.rand_mode(0);
            c_sc_trace_filter_port.constraint_mode(0);
            r_sc_trace_filter_source_enable.rand_mode(0);
            c_sc_trace_filter_source_enable.constraint_mode(0);
            r_sc_trace_filter_source.rand_mode(0);
            c_sc_trace_filter_source.constraint_mode(0);
            r_sc_trace_filter_l2_enable.rand_mode(0);
            c_sc_trace_filter_l2_enable.constraint_mode(0);
            r_sc_trace_filter_l3_enable.rand_mode(0);
            c_sc_trace_filter_l3_enable.constraint_mode(0);
            r_sc_trace_filter_fsm_enable.rand_mode(0);
            c_sc_trace_filter_fsm_enable.constraint_mode(0);
            r_sc_trace_type_hot_enable.rand_mode(0);
            c_sc_trace_type_hot_enable.constraint_mode(0);
            r_sc_l3_shire_swizzle_mode.rand_mode(0);
            c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            r_sc_all_shire_aliasing.rand_mode(0);
            c_sc_all_shire_aliasing.constraint_mode(0);
            r_sc_two_shire_aliasing_use_shire_lsb.rand_mode(0);
            c_sc_two_shire_aliasing_use_shire_lsb.constraint_mode(0);
         end

         //
         // Any non-global configuration block can essentially quit at this point.
         //
         if (m_global_config == 1'b0) begin
            r_sc_cache_alloc_mode = 0;
            r_sc_cache_alloc_mode.rand_mode(0);
            c_sc_cache_alloc_mode.constraint_mode(0);
            r_sc_l3_shire_swizzle_mode = 0;
            r_sc_l3_shire_swizzle_mode.rand_mode(0);
            c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            r_sc_all_shire_aliasing = 1'b0;
            r_sc_all_shire_aliasing.rand_mode(0);
            c_sc_all_shire_aliasing.constraint_mode(0);
            r_sc_two_shire_aliasing_use_shire_lsb = 1'b0;
            r_sc_two_shire_aliasing_use_shire_lsb.rand_mode(0);
            c_sc_two_shire_aliasing_use_shire_lsb.constraint_mode(0);
            return;
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_all_shire_aliasing");
         if (value != -1) begin
            r_sc_all_shire_aliasing = value[0:0];
            r_sc_all_shire_aliasing.rand_mode(0);
            c_sc_all_shire_aliasing.constraint_mode(0);
         end
         else if ((m_core_present == 1'b1) || (m_use_rand_mode == 1'b0)) begin
            r_sc_all_shire_aliasing.rand_mode(0);
            c_sc_all_shire_aliasing.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_two_shire_aliasing_use_shire_lsb");
         if (value != -1) begin
            r_sc_two_shire_aliasing_use_shire_lsb = value[0:0];
            r_sc_two_shire_aliasing_use_shire_lsb.rand_mode(0);
            c_sc_two_shire_aliasing_use_shire_lsb.constraint_mode(0);
         end
         else if ((m_core_present == 1'b1) || (m_use_rand_mode == 1'b0)) begin
            r_sc_two_shire_aliasing_use_shire_lsb.rand_mode(0);
            c_sc_two_shire_aliasing_use_shire_lsb.constraint_mode(0);
         end

         //
         // If we have real memory shires, force the swizzle mode to be 0 for now.
         //
         if ((evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT) != null) || (m_core_present == 1'b1)) begin
            value = 0;
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l3_shire_swizzle_mode");
         end
         if (value != -1) begin
            m_sc_l3_shire_swizzle_mode = int'(value[1:0]);
            if (m_sc_l3_shire_swizzle_mode > 2) begin
               m_sc_l3_shire_swizzle_mode = 0;
            end
            r_sc_l3_shire_swizzle_mode = m_sc_l3_shire_swizzle_mode[1:0];
            r_sc_l3_shire_swizzle_mode.rand_mode(0);
            c_sc_l3_shire_swizzle_mode.constraint_mode(0);
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b0");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b1");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_bank_sel_b2");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b0");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b1");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_sub_bank_sel_b2");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b0");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b1");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b2");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b3");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b4");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_shire_sel_b5");
            if (value != -1) begin
               r_sc_l3_shire_swizzle_mode = 2'b00;
               r_sc_l3_shire_swizzle_mode.rand_mode(0);
               c_sc_l3_shire_swizzle_mode.constraint_mode(0);
            end
         end
         if (m_use_rand_mode == 1'b0) begin
            r_sc_l3_shire_swizzle_mode.rand_mode(0);
            c_sc_l3_shire_swizzle_mode.constraint_mode(0);
         end

         //
         // If a specific mode is selected, use it.
         //
         value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_cache_alloc_mode");
         if (value == -1) begin
            if ((m_core_present == 1'b1) || (m_noc_present == 1'b1)) begin
               value = 0;
            end
            else if (m_use_rand_mode == 1'b0) begin
               value = 16;
            end
            else begin
               value = $urandom() % 6;
            end
         end
         else if (value <= 0) begin
            value = 0;
         end
         else if (value >= 6) begin
            value = 6;
         end
         r_sc_cache_alloc_mode = value[3:0];
         r_sc_cache_alloc_mode.rand_mode(0);
         c_sc_cache_alloc_mode.constraint_mode(0);

         //
         // If no specific mode was selected or a random mode was selected, then allow randomization
         // of the particulars.
         //
         if (value >= 6) begin
            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_scp_set_base");
            if (value == -1) begin
               m_sc_scp_set_base = value;
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_scp_set_size");
            if (value == -1) begin
               m_sc_scp_set_size = value;
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l2_set_base");
            if (value == -1) begin
               m_sc_l2_set_base = value;
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l2_set_size");
            if (value == -1) begin
               m_sc_l2_set_size = value;
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l3_set_base");
            if (value == -1) begin
               m_sc_l3_set_base = value;
            end

            value = m_global_attrs.get_plus_arg_value_as_int("esr_sc_l3_set_size");
            if (value == -1) begin
               m_sc_l3_set_size = value;
            end
         end

         if (m_sc_scp_set_size == 0) begin
            r_sc_cache_alloc_mode = 4'h6;
            m_sc_scp_set_base     = 0;
            r_sc_scp_set_base     = 12'h000;
            r_sc_scp_set_size     = 13'h000;
            r_sc_scp_set_base.rand_mode(0);
            r_sc_scp_set_size.rand_mode(0);
            c_sc_scp_set_base.constraint_mode(0);
            c_sc_scp_set_size.constraint_mode(0);
         end

         if (m_sc_l2_set_size == 0) begin
            r_sc_cache_alloc_mode = 4'h6;
            m_sc_l2_set_base      = 0;
            r_sc_l2_set_base      = 12'h000;
            r_sc_l2_set_size      = 13'h000;
            r_sc_l2_set_base.rand_mode(0);
            r_sc_l2_set_size.rand_mode(0);
            c_sc_l2_set_base.constraint_mode(0);
            c_sc_l2_set_size.constraint_mode(0);
         end

         if (m_sc_l3_set_size == 0) begin
            r_sc_cache_alloc_mode = 4'h6;
            m_sc_l3_set_base      = 0;
            r_sc_l3_set_base      = 12'h000;
            r_sc_l3_set_size      = 13'h000;
            r_sc_l3_set_base.rand_mode(0);
            r_sc_l3_set_size.rand_mode(0);
            c_sc_l3_set_base.constraint_mode(0);
            c_sc_l3_set_size.constraint_mode(0);
         end

         //
         // If we have specified the L3 set base or size, we *may* need to specify the L2 and scratchpad
         // sizes and bases.
         //
         if ((m_sc_l3_set_base >= 0) || (m_sc_l3_set_size >= 0)) begin
            r_sc_cache_alloc_mode = 4'h6;
            if ((m_sc_l3_set_base >= 0) && (m_sc_l3_set_size >= 0)) begin
               if ((m_sc_l3_set_base + m_sc_l3_set_size) > `SC_SETS_PER_SUB_BANK) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l3_set_base=0x%0x with +esr_sc_l3_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l3_set_base, m_sc_l3_set_size, `SC_SETS_PER_SUB_BANK))
                  if (m_sc_l3_set_base >= `SC_SETS_PER_SUB_BANK) begin
                     m_sc_l3_set_base = `SC_SETS_PER_SUB_BANK - 1;
                  end
                  m_sc_l3_set_size = `SC_SETS_PER_SUB_BANK - m_sc_l3_set_base;
               end
            end
            else if (m_sc_l3_set_base >= `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l3_set_base=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l3_set_base, `SC_SETS_PER_SUB_BANK - 1))
               m_sc_l3_set_base = `SC_SETS_PER_SUB_BANK - 1;
               m_sc_l3_set_size = `SC_SETS_PER_SUB_BANK - m_sc_l3_set_base;
            end
            else if (m_sc_l3_set_size > `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l3_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l3_set_size, `SC_SETS_PER_SUB_BANK))
               m_sc_l3_set_size = `SC_SETS_PER_SUB_BANK;
               m_sc_l3_set_base = `SC_SETS_PER_SUB_BANK - m_sc_l3_set_size;
            end
            if ((m_sc_l2_set_base >= 0) || (m_sc_l2_set_size >= 0)) begin
               if ((m_sc_l2_set_base >= 0) && (m_sc_l2_set_size >= 0)) begin
                  if ((m_sc_l2_set_base + m_sc_l2_set_size) > m_sc_l3_set_base) begin
                     `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_base=0x%0x with +esr_sc_l2_set_size=0x%0x is inconsistent with L3 base (0x%0x ... cannot exceed 0x%0x)", m_sc_l2_set_base, m_sc_l2_set_size, m_sc_l3_set_base, `SC_SETS_PER_SUB_BANK))
                     if (m_sc_l2_set_size > m_sc_l2_set_base) begin
                        m_sc_l2_set_size = m_sc_l2_set_base;
                     end
                     m_sc_l2_set_base = m_sc_l3_set_base - m_sc_l2_set_size;
                  end
               end
               else if (m_sc_l2_set_size > m_sc_l3_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_size=0x%0x is inconsistent with L3 base (0x%0x ... cannot exceed 0x%0x)", m_sc_l2_set_size, m_sc_l3_set_base, `SC_SETS_PER_SUB_BANK))
                  m_sc_l2_set_size = m_sc_l3_set_base;
                  m_sc_l2_set_base = m_sc_l3_set_base - m_sc_l2_set_size;
               end
               else if (m_sc_l2_set_base > m_sc_l3_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_base=0x%0x is inconsistent with L3 base (0x%0x ... cannot exceed 0x%0x)", m_sc_l2_set_base, m_sc_l3_set_base, `SC_SETS_PER_SUB_BANK))
                  m_sc_l2_set_base = m_sc_l3_set_base - 1;
                  m_sc_l2_set_size = 1;
               end
               if ((m_sc_scp_set_base >= 0) || (m_sc_scp_set_size >= 0)) begin
                  if ((m_sc_scp_set_base >= 0) && (m_sc_scp_set_size >= 0)) begin
                     if ((m_sc_scp_set_base + m_sc_scp_set_size) > m_sc_l2_set_base) begin
                        `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x with +esr_sc_scp_set_size=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_scp_set_size, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                        if (m_sc_scp_set_size > m_sc_scp_set_base) begin
                           m_sc_scp_set_size = m_sc_scp_set_base;
                        end
                        m_sc_scp_set_base = m_sc_l2_set_base - m_sc_scp_set_size;
                     end
                  end
                  else if (m_sc_scp_set_size > m_sc_l2_set_base) begin
                     `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_size=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_size, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                     m_sc_scp_set_size = m_sc_l2_set_base;
                     m_sc_scp_set_base = m_sc_l2_set_base - m_sc_scp_set_size;
                  end
                  else if (m_sc_scp_set_base > m_sc_l2_set_base) begin
                     `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                     m_sc_scp_set_base = m_sc_l2_set_base - 1;
                     m_sc_scp_set_size = 1;
                  end
               end
            end
            else if ((m_sc_scp_set_base >= 0) || (m_sc_scp_set_size >= 0)) begin
               if ((m_sc_scp_set_base >= 0) && (m_sc_scp_set_size >= 0)) begin
                  if ((m_sc_scp_set_base + m_sc_scp_set_size) > m_sc_l3_set_base) begin
                     `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x with +esr_sc_scp_set_size=0x%0x is inconsistent with L3 base + size (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_scp_set_size, m_sc_l3_set_base + m_sc_l3_set_size, `SC_SETS_PER_SUB_BANK))
                     if (m_sc_scp_set_size > m_sc_scp_set_base) begin
                        m_sc_scp_set_size = m_sc_scp_set_base;
                     end
                     m_sc_scp_set_base = m_sc_l3_set_base - m_sc_scp_set_size;
                  end
               end
               else if (m_sc_scp_set_size > m_sc_l3_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_size=0x%0x is inconsistent with L3 base + size (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_size, m_sc_l3_set_base + m_sc_l3_set_size, `SC_SETS_PER_SUB_BANK))
                  m_sc_scp_set_size = m_sc_l3_set_base;
                  m_sc_scp_set_base = m_sc_l3_set_base - m_sc_scp_set_size;
               end
               else if (m_sc_scp_set_base > m_sc_l3_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x is inconsistent with L3 base + size (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_l3_set_base + m_sc_l3_set_size, `SC_SETS_PER_SUB_BANK))
                  m_sc_scp_set_base = m_sc_l3_set_base - 1;
                  m_sc_scp_set_size = 1;
               end
            end
            else begin
               m_sc_l2_set_size  = m_sc_l3_set_base / 2;
               m_sc_l2_set_base  = m_sc_l3_set_base - m_sc_l2_set_size;
               m_sc_scp_set_size = m_sc_l3_set_base - m_sc_l2_set_base;
               m_sc_scp_set_base = m_sc_l2_set_base - m_sc_scp_set_size;
            end
            if (m_sc_l2_set_size == 0) begin
               m_sc_l2_set_base = 0;
            end
            if (m_sc_scp_set_size == 0) begin
               m_sc_scp_set_base = 0;
            end
            r_sc_scp_set_base = m_sc_scp_set_base[11:0];
            r_sc_scp_set_size = m_sc_scp_set_size[11:0];
            r_sc_l2_set_base  = m_sc_l2_set_base[11:0];
            r_sc_l2_set_size  = m_sc_l2_set_size[11:0];
            r_sc_l3_set_base  = m_sc_l3_set_base[11:0];
            r_sc_l3_set_size  = m_sc_l3_set_size[11:0];
            r_sc_scp_set_base.rand_mode(0);
            r_sc_scp_set_size.rand_mode(0);
            c_sc_scp_set_base.constraint_mode(0);
            c_sc_scp_set_size.constraint_mode(0);
            r_sc_l2_set_base.rand_mode(0);
            r_sc_l2_set_size.rand_mode(0);
            c_sc_l2_set_base.constraint_mode(0);
            c_sc_l2_set_size.constraint_mode(0);
            r_sc_l3_set_base.rand_mode(0);
            r_sc_l3_set_size.rand_mode(0);
            c_sc_l3_set_base.constraint_mode(0);
            c_sc_l3_set_size.constraint_mode(0);
         end
         else if ((m_sc_l2_set_base >= 0) || (m_sc_l2_set_size >= 0)) begin
            r_sc_cache_alloc_mode = 4'h6;
            if ((m_sc_l2_set_base >= 0) && (m_sc_l2_set_size >= 0)) begin
               if ((m_sc_l2_set_base + m_sc_l2_set_size) > `SC_SETS_PER_SUB_BANK) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_base=0x%0x with +esr_sc_l2_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l2_set_base, m_sc_l2_set_size, `SC_SETS_PER_SUB_BANK))
                  if (m_sc_l2_set_base >= `SC_SETS_PER_SUB_BANK) begin
                     m_sc_l2_set_base = `SC_SETS_PER_SUB_BANK - 1;
                  end
                  m_sc_l2_set_size = `SC_SETS_PER_SUB_BANK - m_sc_l2_set_base;
               end
            end
            else if (m_sc_l2_set_base >= `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_base=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK - 1))
               m_sc_l2_set_base = `SC_SETS_PER_SUB_BANK - 1;
               m_sc_l2_set_size = `SC_SETS_PER_SUB_BANK - m_sc_l2_set_base;
            end
            else if (m_sc_l2_set_size > `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_l2_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_l2_set_size, `SC_SETS_PER_SUB_BANK))
               m_sc_l2_set_size = `SC_SETS_PER_SUB_BANK;
               m_sc_l2_set_base = `SC_SETS_PER_SUB_BANK - m_sc_l2_set_size;
            end
            if ((m_sc_scp_set_base >= 0) || (m_sc_scp_set_size >= 0)) begin
               if ((m_sc_scp_set_base >= 0) && (m_sc_scp_set_size >= 0)) begin
                  if ((m_sc_scp_set_base + m_sc_scp_set_size) > m_sc_l2_set_base) begin
                     `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x with +esr_sc_scp_set_size=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_scp_set_size, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                     if (m_sc_scp_set_size > m_sc_scp_set_base) begin
                        m_sc_scp_set_size = m_sc_scp_set_base;
                     end
                     m_sc_scp_set_base = m_sc_l2_set_base - m_sc_scp_set_size;
                  end
               end
               else if (m_sc_scp_set_size > m_sc_l2_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_size=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_size, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                  m_sc_scp_set_size = m_sc_l2_set_base;
                  m_sc_scp_set_base = m_sc_l2_set_base - m_sc_scp_set_size;
               end
               else if (m_sc_scp_set_base > m_sc_l2_set_base) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x is inconsistent with L2 base (0x%0x ... cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_l2_set_base, `SC_SETS_PER_SUB_BANK))
                  m_sc_scp_set_base = m_sc_l2_set_base - 1;
                  m_sc_scp_set_size = 1;
               end
            end
            r_sc_scp_set_base = m_sc_scp_set_base[11:0];
            r_sc_scp_set_size = m_sc_scp_set_size[12:0];
            r_sc_l2_set_base  = m_sc_l2_set_base[11:0];
            r_sc_l2_set_size  = m_sc_l2_set_size[12:0];
            r_sc_scp_set_base.rand_mode(0);
            r_sc_scp_set_size.rand_mode(0);
            c_sc_scp_set_base.constraint_mode(0);
            c_sc_scp_set_size.constraint_mode(0);
            r_sc_l2_set_base.rand_mode(0);
            r_sc_l2_set_size.rand_mode(0);
            c_sc_l2_set_base.constraint_mode(0);
            c_sc_l2_set_size.constraint_mode(0);
         end
         else if ((m_sc_scp_set_base >= 0) || (m_sc_scp_set_size >= 0)) begin
            r_sc_cache_alloc_mode = 4'h6;
            if ((m_sc_scp_set_base >= 0) && (m_sc_scp_set_size >= 0)) begin
               if ((m_sc_scp_set_base + m_sc_scp_set_size) > `SC_SETS_PER_SUB_BANK) begin
                  `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x with +esr_sc_scp_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_scp_set_base, m_sc_scp_set_size, `SC_SETS_PER_SUB_BANK))
                  if (m_sc_scp_set_base >= `SC_SETS_PER_SUB_BANK) begin
                     m_sc_scp_set_base = `SC_SETS_PER_SUB_BANK - 1;
                  end
                  m_sc_scp_set_size = `SC_SETS_PER_SUB_BANK - m_sc_scp_set_base;
               end
            end
            else if (m_sc_scp_set_base >= `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_base=0x%0x is too large (cannot exceed 0x%0x)", m_sc_scp_set_base, `SC_SETS_PER_SUB_BANK - 1))
               m_sc_scp_set_base = `SC_SETS_PER_SUB_BANK - 1;
               m_sc_scp_set_size = `SC_SETS_PER_SUB_BANK - m_sc_scp_set_base;
            end
            else if (m_sc_scp_set_size > `SC_SETS_PER_SUB_BANK) begin
               `dut_warning(get_abstract_name(), $sformatf("configuration error ... +esr_sc_scp_set_size=0x%0x is too large (cannot exceed 0x%0x)", m_sc_scp_set_size, `SC_SETS_PER_SUB_BANK))
               m_sc_scp_set_size = `SC_SETS_PER_SUB_BANK;
               m_sc_scp_set_base = `SC_SETS_PER_SUB_BANK - m_sc_scp_set_size;
            end
            r_sc_scp_set_base = m_sc_scp_set_base[11:0];
            r_sc_scp_set_size = m_sc_scp_set_size[12:0];
            r_sc_scp_set_base.rand_mode(0);
            r_sc_scp_set_size.rand_mode(0);
            c_sc_scp_set_base.constraint_mode(0);
            c_sc_scp_set_size.constraint_mode(0);
         end
      endfunction : new

   endclass : evl_sc_rand_config_object


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_sc_config_test_seq
   //
   class evl_sc_config_test_seq extends evl_sequence_mdir;

      //--------------------------------------------------------------------------------------------
      //
      // Static Member Variables and Objects
      //
      static int                       m_global_config_randomized = 0;
      static evl_sc_rand_config_object m_global_config_object     = new("SC_Rand", 1'b1);


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      bit                               m_use_rand_mode;
      evl_verif_comp                    m_shire_agent;
      evl_sc_config_reg                 m_config_regs[$];
      evl_sc_rand_config_object         m_config_object;
      evl_esr_sc_l3_shire_swizzle_ctl_t m_sc_l3_shire_swizzle_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_scp_pipe_cache_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_l2_pipe_cache_ctl;
      evl_esr_sc_pipe_cache_ctl_t       m_l3_pipe_cache_ctl;


      //--------------------------------------------------------------------------------------------
      function void create_sc_swizzle_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_sc_l3_shire_swizzle_ctl_t read_only_val  = evl_esr_sc_l3_shire_swizzle_ctl_t'(0);
         evl_esr_sc_l3_shire_swizzle_ctl_t force_zero_val = evl_esr_sc_l3_shire_swizzle_ctl_t'(0);
         evl_esr_sc_l3_shire_swizzle_ctl_t force_one_val  = evl_esr_sc_l3_shire_swizzle_ctl_t'(0);
         evl_sc_config_reg                 config_reg;

         m_global_config_object.assign_swizzle(m_sc_l3_shire_swizzle_ctl);

         config_reg = new(name_in,
                          EVL_SC_l3_shire_swizzle_ctl,
                          addr,
                          evl_dword_t'(m_sc_l3_shire_swizzle_ctl),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_swizzle_ctl


      //--------------------------------------------------------------------------------------------
      function void create_sc_reqq_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_sc_reqq_ctl_t default_val    = evl_esr_sc_reqq_ctl_t'(0);
         evl_esr_sc_reqq_ctl_t read_only_val  = evl_esr_sc_reqq_ctl_t'(0);
         evl_esr_sc_reqq_ctl_t force_zero_val = evl_esr_sc_reqq_ctl_t'(0);
         evl_esr_sc_reqq_ctl_t force_one_val  = evl_esr_sc_reqq_ctl_t'(0);
         evl_sc_config_reg     config_reg;

         default_val.esr_sc_axi_qos             = 1'b1;
         default_val.esr_sc_cbuf_enable         = 1'b1;
         default_val.esr_sc_remote_l3_enable    = 1'b1;
         default_val.esr_sc_remote_scp_enable   = 1'b1;
         default_val.esr_sc_l2_bypass           = 1'b0;
         default_val.esr_sc_l3_bypass           = 1'b0;
         default_val.esr_sc_num_l3_reqq_entries = `SC_REQQ_DEPTH/3;
         default_val.esr_sc_reqq_no_link_list   = 1'b0;
         default_val.esr_sc_ecc_scrub_enable    = 1'b0;
         default_val.esr_sc_l3_yield_priority   = 5'h00;

         config_reg = new(name_in,
                          EVL_SC_reqq_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_reqq_ctl


      //--------------------------------------------------------------------------------------------
      function void create_sc_pipe_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_sc_pipe_ctl_t default_val    = evl_esr_sc_pipe_ctl_t'(0);
         evl_esr_sc_pipe_ctl_t read_only_val  = evl_esr_sc_pipe_ctl_t'(0);
         evl_esr_sc_pipe_ctl_t force_zero_val = evl_esr_sc_pipe_ctl_t'(0);
         evl_esr_sc_pipe_ctl_t force_one_val  = evl_esr_sc_pipe_ctl_t'(0);
         evl_sc_config_reg     config_reg;

         default_val.esr_sc_ram_deep_sleep                = 1'b0;
         default_val.esr_sc_ram_shut_down                 = 1'b0;
         default_val.esr_sc_ram_delay                     = 3'h2;
         default_val.esr_sc_l2_rbuf_enable                = 1'b1;
         default_val.esr_sc_scp_rbuf_enable               = 1'b1;
         default_val.esr_sc_zero_state_enable             = 1'b1;
         default_val.esr_sc_allow_only_1_req_per_sub_bank = 1'b0;
         default_val.esr_sc_allow_only_1_req_per_bank     = 1'b0;
         default_val.esr_sc_cbuf_entry_enable             = 32'hffffffff;

         force_zero_val.esr_sc_ram_deep_sleep = 1'b0;
         force_zero_val.esr_sc_ram_shut_down  = 1'b0;

         config_reg = new(name_in,
                          EVL_SC_pipe_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_pipe_ctl


      //--------------------------------------------------------------------------------------------
      function void create_sc_cache_ctl(input evl_paddr_t       addr,
                                        input string            name_in,
                                        input evl_sc_reg_type_t reg_type_in,
                                        input evl_dword_t       default_in);
         evl_esr_sc_pipe_cache_ctl_t read_only_val  = evl_esr_sc_pipe_cache_ctl_t'(0);
         evl_esr_sc_pipe_cache_ctl_t force_zero_val = evl_esr_sc_pipe_cache_ctl_t'(0);
         evl_esr_sc_pipe_cache_ctl_t force_one_val  = evl_esr_sc_pipe_cache_ctl_t'(0);
         evl_sc_config_reg           config_reg;


         read_only_val.set_base_rsvd = 4'hf;
         read_only_val.set_size_rsvd = 3'h7;
         read_only_val.set_mask_rsvd = 4'hf;
         read_only_val.tag_mask_rsvd = 4'hf;

         force_zero_val.set_base_rsvd = 4'hf;
         force_zero_val.set_size_rsvd = 3'h7;
         force_zero_val.set_mask_rsvd = 4'hf;
         force_zero_val.tag_mask_rsvd = 4'hf;

         config_reg = new(name_in,
                          reg_type_in,
                          addr,
                          default_in,
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_cache_ctl


      //--------------------------------------------------------------------------------------------
      function void create_sc_err_log_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_sc_err_log_ctl_t default_val    = evl_esr_sc_err_log_ctl_t'(0);
         evl_esr_sc_err_log_ctl_t read_only_val  = evl_esr_sc_err_log_ctl_t'(0);
         evl_esr_sc_err_log_ctl_t force_zero_val = evl_esr_sc_err_log_ctl_t'(0);
         evl_esr_sc_err_log_ctl_t force_one_val  = evl_esr_sc_err_log_ctl_t'(0);
         evl_sc_config_reg        config_reg;

         default_val.esr_sc_err_rsp_enable       = 1'b1;
         default_val.esr_sc_err_interrupt_enable = 8'hfe;

         config_reg = new(name_in,
                          EVL_SC_err_log_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_err_log_ctl


      //--------------------------------------------------------------------------------------------
      function void create_sc_trace_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_sc_trace_ctl_t default_val    = evl_esr_sc_trace_ctl_t'(0);
         evl_esr_sc_trace_ctl_t read_only_val  = evl_esr_sc_trace_ctl_t'(0);
         evl_esr_sc_trace_ctl_t force_zero_val = evl_esr_sc_trace_ctl_t'(0);
         evl_esr_sc_trace_ctl_t force_one_val  = evl_esr_sc_trace_ctl_t'(0);
         evl_sc_config_reg      config_reg;

         default_val.esr_sc_trace_filter_reqq_id_enable = 1'b0;
         default_val.esr_sc_trace_filter_reqq_id        = {6{1'b0}};
         default_val.esr_sc_trace_filter_port_enable    = 1'b0;
         default_val.esr_sc_trace_filter_port           = {3{1'b0}};
         default_val.esr_sc_trace_filter_source_enable  = 1'b0;
         default_val.esr_sc_trace_filter_source         = {8{1'b0}};
         default_val.esr_sc_trace_filter_l2_enable      = 1'b0;
         default_val.esr_sc_trace_filter_l3_enable      = 1'b0;
         default_val.esr_sc_trace_filter_fsm_enable     = 1'b0;
         default_val.esr_sc_trace_type_hot_enable       = {12{1'b0}};

         config_reg = new(name_in,
                          EVL_SC_trace_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_sc_trace_ctl


      //--------------------------------------------------------------------------------------------
      //
      // Task: build_registers
      //
      function bit build_registers(input evl_shire_id_t shire_id_in);
         uvm_reg        register;
         evl_verif_comp sc_agent;
         evl_reg_block  sc_reg_block;
         uvm_reg_block  reg_blocks[$];

         m_shire_agent = evl_verif_comp_map::get_verif_comp(SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id_in);
         if (m_shire_agent == null) begin
            return 1'b0;
         end

         sc_agent = evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id_in);
         if (sc_agent == null) begin
            return 1'b0;
         end

         sc_reg_block = sc_agent.get_evl_reg_block();
         if (sc_reg_block == null) begin
            return 1'b0;
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            uvm_reg_block reg_block;

            reg_block = sc_reg_block.get_block_by_name($sformatf("bank_%0d", ii));
            if (reg_block == null) begin
               return 1'b0;
            end
            reg_blocks.push_back(reg_block);
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_l3_shire_swizzle_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_swizzle_ctl(register.get_address(), register.get_full_name());
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_reqq_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_reqq_ctl(register.get_address(), register.get_full_name());
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_pipe_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_pipe_ctl(register.get_address(), register.get_full_name());
         end

         m_scp_pipe_cache_ctl = evl_esr_sc_pipe_cache_ctl_t'(0);
         if ((`SC_CACHE_SIZE_IN_MB == 3) && (`SC_BANKS == 4) && (`SC_SUB_BANKS == 4)) begin
            m_scp_pipe_cache_ctl.set_size = `SC_SETS_PER_SUB_BANK / 2;
         end
         else begin
            m_scp_pipe_cache_ctl.set_size = (`SC_SETS_PER_SUB_BANK * 20) / 32;
         end

         m_l2_pipe_cache_ctl = evl_esr_sc_pipe_cache_ctl_t'(0);
         if ((`SC_CACHE_SIZE_IN_MB == 3) && (`SC_BANKS == 4) && (`SC_SUB_BANKS == 4)) begin
            m_l2_pipe_cache_ctl.set_size = `SC_SETS_PER_SUB_BANK / 4;
         end
         else begin
            m_l2_pipe_cache_ctl.set_size = `SC_SETS_PER_SUB_BANK / 8;
         end
         m_l2_pipe_cache_ctl.set_base = m_scp_pipe_cache_ctl.set_size;

         m_l3_pipe_cache_ctl = evl_esr_sc_pipe_cache_ctl_t'(0);
         m_l3_pipe_cache_ctl.set_size = `SC_SETS_PER_SUB_BANK / 4;
         m_l3_pipe_cache_ctl.set_base = m_scp_pipe_cache_ctl.set_size + m_l2_pipe_cache_ctl.set_size;

         m_global_config_object.fix_masks(m_scp_pipe_cache_ctl, m_l2_pipe_cache_ctl, m_l3_pipe_cache_ctl);

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_l2_cache_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_cache_ctl(register.get_address(), register.get_full_name(), EVL_SC_l2_cache_ctl,  evl_dword_t'(m_l2_pipe_cache_ctl));
         end
         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_l3_cache_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_cache_ctl(register.get_address(), register.get_full_name(), EVL_SC_l3_cache_ctl,  evl_dword_t'(m_l3_pipe_cache_ctl));
         end
         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_scp_cache_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_cache_ctl(register.get_address(), register.get_full_name(), EVL_SC_scp_cache_ctl, evl_dword_t'(m_scp_pipe_cache_ctl));
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_err_log_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_err_log_ctl(register.get_address(), register.get_full_name());
         end

         for (int ii = 0; ii < `SC_BANKS; ii++) begin
            register = reg_blocks[ii].get_reg_by_name("sc_trace_ctl");
            if (register == null) begin
               return 1'b0;
            end
            create_sc_trace_ctl(register.get_address(), register.get_full_name());
         end
         return 1'b1;
      endfunction : build_registers


      //--------------------------------------------------------------------------------------------
      //
      // Task: configure_me
      //
      virtual task configure_me(output bit result);
         evl_paddr_t    addr;
         evl_trans_desc trans_desc;
         evl_trans_desc trans_descs[$];
         evl_dword_t    dword;
         int            dmactive_drv_value;

         if (build_registers(m_agent.get_shire_id()) == 1'b0) begin
            result = 1'b0;
            return;
         end
         result = 1'b1;
         if (m_global_config_randomized++ == 0) begin
            m_global_config_object.randomize();
            if (m_use_rand_mode == 1'b0) begin
               m_global_config_object.post_randomize();
            end
         end

         //
         // Force certain values to match across all configurations.
         //
         m_config_object.r_sc_remote_l3_enable  = m_global_config_object.r_sc_remote_l3_enable;
         m_config_object.r_sc_remote_scp_enable = m_global_config_object.r_sc_remote_scp_enable;
         m_config_object.r_sc_l3_bypass         = m_global_config_object.r_sc_l3_bypass;
         m_config_object.r_sc_remote_l3_enable.rand_mode(0);
         m_config_object.r_sc_remote_scp_enable.rand_mode(0);
         m_config_object.r_sc_l3_bypass.rand_mode(0);
         m_config_object.c_sc_remote_l3_enable.constraint_mode(0);
         m_config_object.c_sc_remote_scp_enable.constraint_mode(0);
         m_config_object.c_sc_l3_bypass.constraint_mode(0);
         #0 if (m_use_rand_mode == 1'b1) begin
            m_config_object.randomize();
         end
         else begin
            m_config_object.post_randomize();
         end
         foreach (m_config_regs[ii]) begin
            evl_sc_reg_type_t reg_type = m_config_regs[ii].get_type();

            if (reg_type == EVL_SC_l3_shire_swizzle_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_global_config_object.m_sc_l3_shire_swizzle_ctl));
            end
            else if (reg_type == EVL_SC_reqq_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_config_object.m_sc_reqq_ctl));
            end
            else if (reg_type == EVL_SC_pipe_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_config_object.m_sc_pipe_ctl));
            end
            else if (reg_type == EVL_SC_err_log_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_config_object.m_sc_err_log_ctl));
            end
            else if (reg_type == EVL_SC_scp_cache_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_global_config_object.m_scp_pipe_cache_ctl));
            end
            else if (reg_type == EVL_SC_l2_cache_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_global_config_object.m_l2_pipe_cache_ctl));
            end
            else if (reg_type == EVL_SC_l3_cache_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_global_config_object.m_l3_pipe_cache_ctl));
            end
         end
      endtask : configure_me


      //--------------------------------------------------------------------------------------------
      //
      // Task: write_configuration
      //
      task write_configuration();
         evl_trans_desc    trans_desc;
         evl_trans_desc    trans_descs[$];
         evl_dword_t       dword;
         int               dmactive_drv_value;
         evl_sc_config_reg config_reg;
         evl_sc_reg_type_t reg_type;

         dmactive_drv_value = m_shire_agent.get_rtl_int_param_by_name("pk_dv_force_dmactive_drv", 0);
         m_shire_agent.set_rtl_int_param_by_name("pk_dv_force_dmactive_drv", 32'h88880001);

         //
         // Write the sc_reqq_ctl registers first.
         //
         foreach (m_config_regs[ii]) begin
            config_reg = m_config_regs[ii];
            reg_type   = config_reg.get_type();
            if (reg_type inside { EVL_SC_reqq_ctl }) begin
               dword = config_reg.get_value();
               `evl_log(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("Writing 0x%x to %0s (0x%x)", dword, config_reg.get_abstract_name(), config_reg.get_paddr()))
               issue(trans_desc, ST, config_reg.get_paddr(), 8, evl_bus_req_source_t'(0), { dword, dword, dword, dword, dword, dword, dword, dword }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
               trans_descs.push_back(trans_desc);
            end
         end

         //
         // Write the sc_pipe_ctl registers next.
         //
         foreach (m_config_regs[ii]) begin
            config_reg = m_config_regs[ii];
            reg_type   = config_reg.get_type();
            if (reg_type inside { EVL_SC_pipe_ctl }) begin
               dword = config_reg.get_value();
               `evl_log(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("Writing 0x%x to %0s (0x%x)", dword, config_reg.get_abstract_name(), config_reg.get_paddr()))
               issue(trans_desc, ST, config_reg.get_paddr(), 8, evl_bus_req_source_t'(0), { dword, dword, dword, dword, dword, dword, dword, dword }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
               trans_descs.push_back(trans_desc);
            end
         end

         //
         // Now write the remaining registers.
         //
         foreach (m_config_regs[ii]) begin
            config_reg = m_config_regs[ii];
            reg_type   = config_reg.get_type();
            if (reg_type inside { EVL_SC_l3_shire_swizzle_ctl, EVL_SC_scp_cache_ctl, EVL_SC_l2_cache_ctl, EVL_SC_l3_cache_ctl, EVL_SC_err_log_ctl }) begin
               dword = config_reg.get_value();
               `evl_log(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("Writing 0x%x to %0s (0x%x)", dword, config_reg.get_abstract_name(), config_reg.get_paddr()))
               issue(trans_desc, ST, config_reg.get_paddr(), 8, evl_bus_req_source_t'(0), { dword, dword, dword, dword, dword, dword, dword, dword }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
               trans_descs.push_back(trans_desc);
            end
         end

         //
         // Wait for all of the writes to complete.
         //
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end

         //
         // Now read the configuration registers.
         //
         trans_descs = {};
         foreach (m_config_regs[ii]) begin
            issue(trans_desc, LD, m_config_regs[ii].get_paddr(), 8, evl_bus_req_source_t'(0), { }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
            trans_descs.push_back(trans_desc);
         end
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end
         foreach (m_config_regs[ii]) begin
            dword = trans_descs[ii].get_read_dword(-1);
            `evl_log_mdesc(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), trans_descs[ii], $sformatf("Read 0x%x for %0s", dword, m_config_regs[ii].get_abstract_name()))
         end
         blocking_issue(trans_desc, BARRIER, 0);

         if (m_shire_agent.get_rtl_int_param_by_name("pk_dv_force_dmactive_drv", 0) == 32'h88880001) begin
            m_shire_agent.set_rtl_int_param_by_name("pk_dv_force_dmactive_drv", dmactive_drv_value);
         end
         else if ((m_shire_agent.get_rtl_int_param_by_name("pk_dv_force_dmactive_drv", 0) & 32'h00000001) == 0) begin
            `dut_error(get_abstract_name(), $sformatf("dv_force_dmactive was deasserted during the shire cache configuration"))
         end
      endtask : write_configuration


      //--------------------------------------------------------------------------------------------
      //
      // Task: configure_phase
      //
      virtual task configure_phase(input uvm_phase phase);
         int value;

         phase.raise_objection(phase);
         if (`et_test_plusargs("scConfig=fixed") != 0) begin
            m_use_rand_mode = 1'b0;
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("sc_rand_config");
            if (value == -1) begin
               m_use_rand_mode = 1'b1;
            end
            else begin
               m_use_rand_mode = value[0:0];
            end
         end
         fork
            begin
               super.configure_phase(phase);
            end
            begin
               bit result;

               configure_me(result);
               if (result == 1'b1) begin
                  write_configuration();
               end
            end
         join
         phase.drop_objection(phase);
      endtask : configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Task: post_configure_phase
      //
      virtual task post_configure_phase(input uvm_phase phase);
         disable_stimulus(`EVL_ON_ICE_DISABLE);
         m_global_config_object.print_config();
         m_config_object.print_config();
         super.post_configure_phase(phase);
      endtask : post_configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_sc_config_test_seq", input evl_verif_comp agent_in = null);
         super.new(name_in);
         m_config_object   = new(name_in);
         m_allow_unaligned = 1'b1;
      endfunction

   endclass : evl_sc_config_test_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      bit            no_config = 1'b0;
      evl_verif_comp verif_comp_list[];

      `ifdef ET_MAXION_STANDALONE
         no_config = 1'b1;
      `else // ET_MAXION_STANDALONE
         if (evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT) == null) begin
            no_config = 1'b1;
         end
         else if ((evl_verif_comp_map::get_verif_comp(CORE_RTL_AGENT) != null) || (evl_verif_comp_map::get_verif_comp(NOC_RTL_AGENT) != null)) begin
            string value;

            if (`et_value_plusargs("scConfig=%s", value) == 0) begin
               value = "none";
            end
            else if (`et_test_plusargs("noSCConfig") != 0) begin
               value = "none";
            end
            if (value == "none") begin
               no_config = 1'b1;
            end
         end
         else if ((`et_test_plusargs("noSCConfig") != 0) || (`et_test_plusargs("scConfig=none") != 0)) begin
            no_config = 1'b1;
         end
         if ((evl_verif_comp_map::get_verif_comp(FROMSYS_MASTER_STIM_AGENT) == null) && (evl_verif_comp_map::get_verif_comp(NEIGH_STIM_AGENT) == null)) begin
            no_config = 1'b1;
         end
      `endif // !ET_MAXION_STANDALONE

      if (no_config == 1'b0) begin
         if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, FROMSYS_MASTER_STIM_AGENT) != 0) begin
            foreach (verif_comp_list[ii]) begin
               evl_shire_id_t shire_id = verif_comp_list[ii].get_shire_id();

               if (evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id) != null) begin
                  evl_sc_config_test_seq sc_config_test_seq;

                  sc_config_test_seq = new($sformatf("SC_Rand-%0d", shire_id), verif_comp_list[ii]);
                  sc_config_test_seq.set_abstract_name("Shire Cache Configuration");
                  verif_comp_list[ii].add_sequence_wrapper(sc_config_test_seq.get_wrapper());
               end
            end
         end
         else if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, NEIGH_STIM_AGENT) != 0) begin
            bit agent_started[evl_shire_id_t];

            foreach (verif_comp_list[ii]) begin
               evl_shire_id_t shire_id = verif_comp_list[ii].get_shire_id();

               if ((evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id) != null) && (!agent_started.exists(shire_id))) begin
                  evl_sc_config_test_seq sc_config_test_seq;

                  agent_started[shire_id] = 1'b1;
                  sc_config_test_seq = new($sformatf("SC_Rand-%0d", verif_comp_list[ii].get_shire_id()), verif_comp_list[ii]);
                  sc_config_test_seq.set_abstract_name("Shire Cache Configuration");
                  verif_comp_list[ii].add_sequence_wrapper(sc_config_test_seq.get_wrapper());
               end
            end
         end
      end
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sc_config_test");
      super.new(name_in);
   endfunction

endclass : evl_sc_config_test

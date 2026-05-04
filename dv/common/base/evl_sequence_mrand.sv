//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_mrand
//
class evl_sequence_mrand extends evl_sequence_mem;

   `uvm_object_utils(evl_sequence_mrand)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   static semaphore m_print_contents = new(1);


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Randomization
   //
   rand int                       r_wt_nop;
   rand int                       r_wt_ld;
   rand int                       r_wt_ld_coop;
   rand int                       r_wt_st;
   rand int                       r_wt_st_wa;
   rand int                       r_wt_evict;
   rand int                       r_wt_cache_lock;
   rand int                       r_wt_cache_unlock;
   rand int                       r_wt_cache_evict;
   rand int                       r_wt_cache_evict_to_mem;
   rand int                       r_wt_cache_flush;
   rand int                       r_wt_cache_flush_to_mem;
   rand int                       r_wt_cache_prefetch;
   rand int                       r_wt_cache_scp_fill;
   rand int                       r_wt_message;
   rand int                       r_wt_atomic;

   rand int                       r_cmd_count;
   rand int                       r_paddr_count;
   rand int                       r_scp_addr_count;
   rand int                       r_fill_addr_count;
   rand int                       r_cache_attr_count;
   rand int                       r_cmd_length_count;
   rand int                       r_addr_sp_count;
   rand int                       r_dword_count;
   rand int                       r_qword_en_count;
   rand int                       r_trans_attr_count;
   rand int                       r_req_source_count;
   rand int                       r_cline_zero_count;
   rand int                       r_qos_count;
   rand int                       r_vc_count;

   rand evl_cmd_t                 r_cmd;
   rand evl_paddr_t               r_paddr;
   rand evl_paddr_t               r_fill_addr;
   rand evl_paddr_t               r_scp_addr;
   rand bit                       r_cache_attr;
   rand evl_cmd_length_t          r_cmd_length;
   rand evl_addr_sp_t             r_addr_sp;
   rand evl_dword_t               r_dword;
   rand evl_line_qword_en_t       r_qword_en;
   rand evl_trans_attr_s          r_trans_attr;
   rand evl_bus_req_source_t      r_req_source;
   rand evl_bus_cop_start_level_t r_cop_start_level;
   rand evl_bus_cop_dest_level_t  r_cop_dest_level;
   rand bit                       r_cop_final_state;
   rand evl_bus_atomic_conf_t     r_atomic_conf;
   rand evl_bus_atomic_transid_t  r_atomic_transid;
   rand evl_subopcode_dest_t      r_subopcode_dest;
   rand evl_bus_message_conf_t    r_message_conf;
   rand evl_bus_coop_target_t     r_coop_target;
   rand bit                       r_cline_zero;
   rand evl_bus_qos_t             r_qos;
   rand evl_bus_req_vc_t          r_vc;

        evl_cmd_t                 m_cmd_list[$];
        evl_paddr_t               m_paddr_list[$];
        evl_paddr_t               m_fill_addr_list[$];
        evl_paddr_t               m_scp_addr_list[$];
        bit                       m_cache_attr_list[$];
        evl_cmd_length_t          m_cmd_length_list[$];
        evl_addr_sp_t             m_addr_sp_list[$];
        evl_dword_t               m_dword_list[$];
        evl_line_qword_en_t       m_qword_en_list[$];
        evl_bus_req_source_t      m_req_source_list[$];
        bit                       m_cline_zero_list[$];
        evl_bus_cop_start_level_t m_cop_start_level_list[$];
        evl_bus_cop_dest_level_t  m_cop_dest_level_list[$];
        bit                       m_cop_final_state_list[$];
        evl_bus_atomic_conf_t     m_atomic_conf_list[$];
        evl_bus_atomic_transid_t  m_atomic_transid_list[$];
        evl_subopcode_dest_t      m_subopcode_dest_list[$];
        evl_bus_message_conf_t    m_message_conf_list[$];
        evl_bus_coop_target_t     m_coop_target_list[$];
        evl_bus_qos_t             m_qos_list[$];
        evl_bus_req_vc_t          m_vc_list[$];

        string                    m_test_message = "";


   //-----------------------------------------------------------------------------------------------
   //
   // Base Constraints
   //
   constraint c_cmd_count             { r_cmd_count        inside { 1 }; }
   constraint c_paddr_count           { r_paddr_count      inside { 1 }; }
   constraint c_scp_addr_count        { r_scp_addr_count   inside { 1 }; }
   constraint c_fill_addr_count       { r_fill_addr_count  inside { 1 }; }
   constraint c_cache_attr_count      { r_cache_attr_count inside { 1 }; }
   constraint c_cmd_length_count      { r_cmd_length_count inside { 1 }; }
   constraint c_addr_sp_count         { r_addr_sp_count    inside { 1 }; }
   constraint c_dword_count           { r_dword_count      inside { 8 }; }
   constraint c_qword_en_count        { r_qword_en_count   inside { 1 }; }
   constraint c_trans_attr_count      { r_trans_attr_count inside { 1 }; }
   constraint c_req_source_count      { r_req_source_count inside { 1 }; }
   constraint c_cline_zero_count      { r_cline_zero_count inside { 0 }; }
   constraint c_qos_count             { r_qos_count        inside { 0 }; }
   constraint c_vc_count              { r_vc_count         inside { 0 }; }

   constraint c_wt_nop                { r_wt_nop                == 0; }
   constraint c_wt_ld                 { r_wt_ld                 == 0; }
   constraint c_wt_ld_coop            { r_wt_ld_coop            == 0; }
   constraint c_wt_st                 { r_wt_st                 == 0; }
   constraint c_wt_st_wa              { r_wt_st_wa              == 0; }
   constraint c_wt_evict              { r_wt_evict              == 0; }
   constraint c_wt_cache_lock         { r_wt_cache_lock         == 0; }
   constraint c_wt_cache_unlock       { r_wt_cache_unlock       == 0; }
   constraint c_wt_cache_evict        { r_wt_cache_evict        == 0; }
   constraint c_wt_cache_evict_to_mem { r_wt_cache_evict_to_mem == 0; }
   constraint c_wt_cache_flush        { r_wt_cache_flush        == 0; }
   constraint c_wt_cache_flush_to_mem { r_wt_cache_flush_to_mem == 0; }
   constraint c_wt_cache_prefetch     { r_wt_cache_prefetch     == 0; }
   constraint c_wt_cache_scp_fill     { r_wt_cache_scp_fill     == 0; }
   constraint c_wt_message            { r_wt_message            == 0; }
   constraint c_wt_atomic             { r_wt_atomic             == 0; }

   constraint c_cmd                   { r_cmd inside { NOP, LD, ST, EVICT }; }
   constraint c_paddr                 { r_paddr != ~evl_paddr_t'(0);   }
   constraint c_cache_attr            { r_cache_attr inside { 1'b1 };  }
   constraint c_cmd_length            { r_cmd_length inside { [1:8] }; }
   constraint c_addr_sp               { r_addr_sp    inside { Mem };   }
   constraint c_cop_start_level_lcl   { r_cop_start_level inside { [0:2] }; }
   constraint c_cop_dest_level_lcl    { r_cop_dest_level  inside { [1:3] }; }
   constraint c_atomic_conf           { r_atomic_conf[3:0] dist { [0:11] := 5,
                                                                 [12:15] :/ 5 }; }
   constraint c_subopcode_dest        { r_subopcode_dest  dist { dest_l2 := 19, dest_l3 := 1 }; }
   constraint c_message_conf          { r_message_conf    inside { 0, 8, 16, 24, 192, 196 }; }
   constraint c_coop_target           { r_coop_target     inside { [1:255] }; }
   constraint c_coop_target_hard      { r_coop_target     <= ((evl_bus_coop_target_t'(1) << `NUM_NEIGH) - 1); }
   constraint c_cline_zero            { r_cline_zero == 1'b0; }
   constraint c_req_source            { r_req_source      inside { [0:11] }; } //no random coop source


   //-----------------------------------------------------------------------------------------------
   virtual function void randomize_me();
      int count;

      pre_randomize_me();

      void'(randomize());

      r_wt_nop.rand_mode(0);
      r_wt_ld.rand_mode(0);
      r_wt_ld_coop.rand_mode(0);
      r_wt_st.rand_mode(0);
      r_wt_st_wa.rand_mode(0);
      r_wt_evict.rand_mode(0);
      r_wt_cache_lock.rand_mode(0);
      r_wt_cache_unlock.rand_mode(0);
      r_wt_cache_evict.rand_mode(0);
      r_wt_cache_evict_to_mem.rand_mode(0);
      r_wt_cache_flush.rand_mode(0);
      r_wt_cache_flush_to_mem.rand_mode(0);
      r_wt_cache_prefetch.rand_mode(0);
      r_wt_cache_scp_fill.rand_mode(0);
      r_wt_message.rand_mode(0);
      r_wt_atomic.rand_mode(0);

      repeat (r_wt_nop) begin
         m_cmd_list.push_back(NOP);
      end
      repeat (r_wt_ld) begin
         m_cmd_list.push_back(LD);
      end
      repeat (r_wt_ld_coop) begin
         m_cmd_list.push_back(LD_COOP);
      end
      repeat (r_wt_st) begin
         m_cmd_list.push_back(ST);
      end
      repeat (r_wt_st_wa) begin
         m_cmd_list.push_back(ST_WA);
      end
      repeat (r_wt_evict) begin
         m_cmd_list.push_back(EVICT);
      end
      repeat (r_wt_cache_lock) begin
         m_cmd_list.push_back(CACHE_LOCK);
      end
      repeat (r_wt_cache_unlock) begin
         m_cmd_list.push_back(CACHE_UNLOCK);
      end
      repeat (r_wt_cache_evict) begin
         m_cmd_list.push_back(CACHE_EVICT);
      end
      repeat (r_wt_cache_evict_to_mem) begin
         m_cmd_list.push_back(CACHE_EVICT_TO_MEM);
      end
      repeat (r_wt_cache_flush) begin
         m_cmd_list.push_back(CACHE_FLUSH);
      end
      repeat (r_wt_cache_flush_to_mem) begin
         m_cmd_list.push_back(CACHE_FLUSH_TO_MEM);
      end
      repeat (r_wt_cache_prefetch) begin
         m_cmd_list.push_back(CACHE_PREFETCH);
      end
      repeat (r_wt_cache_scp_fill) begin
         m_cmd_list.push_back(CACHE_SCP_FILL);
      end
      repeat (r_wt_message) begin
         m_cmd_list.push_back(MESSAGE);
      end
      repeat (r_wt_atomic) begin
         m_cmd_list.push_back(ATOMIC);
      end

      m_cmds = new[r_cmd_count];
      count  = r_cmd_count - 1;
      if (m_cmd_list.size() > 0) begin
         int limit;

         limit = m_cmd_list.size();
         while (count >= 0) begin
            m_cmd_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_cmds[count--] = m_cmd_list[ii];
            end
         end
         m_cmds.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_cmd);
            m_cmds[count--] = r_cmd;
         end
      end

      m_paddrs = new[r_paddr_count];
      count    = r_paddr_count - 1;
      if (m_paddr_list.size() > 0) begin
         int limit;

         limit = m_paddr_list.size();
         while (count >= 0) begin
            m_paddr_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_paddrs[count--] = m_paddr_list[ii];
            end
         end
         m_paddrs.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_paddr);
            m_paddrs[count--] = r_paddr;
         end
      end

      m_scp_addrs = new[r_scp_addr_count];
      count       = r_scp_addr_count - 1;
      if (m_scp_addr_list.size() > 0) begin
         int limit;

         limit = m_scp_addr_list.size();
         while (count >= 0) begin
            m_scp_addr_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_scp_addrs[count--] = m_scp_addr_list[ii];
            end
         end
         m_scp_addrs.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_scp_addr);
            m_scp_addrs[count--] = r_scp_addr;
         end
      end

      m_fill_addrs = new[r_fill_addr_count];
      count        = r_fill_addr_count - 1;
      if (m_fill_addr_list.size() > 0) begin
         int limit;

         limit = m_fill_addr_list.size();
         while (count >= 0) begin
            m_fill_addr_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_fill_addrs[count--] = m_fill_addr_list[ii];
            end
         end
         m_fill_addrs.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_fill_addr);
            m_fill_addrs[count--] = r_fill_addr;
         end
      end

      m_cache_attrs = new[r_cache_attr_count];
      count         = r_cache_attr_count - 1;
      if (m_cache_attr_list.size() > 0) begin
         int limit;

         limit = m_cache_attr_list.size();
         while (count >= 0) begin
            m_cache_attr_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_cache_attrs[count--] = m_cache_attr_list[ii];
            end
         end
         m_cache_attrs.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_cache_attr);
            m_cache_attrs[count--] = r_cache_attr;
         end
      end

      m_cmd_lengths = new[r_cmd_length_count];
      count         = r_cmd_length_count - 1;
      if (m_cmd_length_list.size() > 0) begin
         int limit;

         limit = m_cmd_length_list.size();
         while (count >= 0) begin
            m_cmd_length_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_cmd_lengths[count--] = m_cmd_length_list[ii];
            end
         end
         m_cmd_lengths.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_cmd_length);
            m_cmd_lengths[count--] = r_cmd_length;
         end
      end

      m_dwords = new[r_dword_count];
      count    = r_dword_count - 1;
      if (m_dword_list.size() > 0) begin
         int limit;

         limit = m_dword_list.size();
         while (count >= 0) begin
            m_dword_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_dwords[count--] = m_dword_list[ii];
            end
         end
         m_dwords.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_dword);
            m_dwords[count--] = r_dword;
         end
      end

      m_qword_ens = new[r_qword_en_count];
      count       = r_qword_en_count - 1;
      if (m_qword_en_list.size() > 0) begin
         int limit;

         limit = m_qword_en_list.size();
         while (count >= 0) begin
            m_qword_en_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_qword_ens[count--] = m_qword_en_list[ii];
            end
         end
         m_qword_ens.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_qword_en);
            m_qword_ens[count--] = r_qword_en;
         end
      end

      m_trans_attrs = new[r_trans_attr_count];
      count         = r_trans_attr_count - 1;
      while (count >= 0) begin
         evl_trans_attr_s trans_attr;

         randomize(r_cop_start_level);
         randomize(r_cop_dest_level);
         randomize(r_cop_final_state);
         randomize(r_atomic_conf);
         randomize(r_atomic_transid);
         randomize(r_subopcode_dest);
         randomize(r_message_conf);
         randomize(r_coop_target);
         trans_attr.cop_start_level = r_cop_start_level;
         trans_attr.cop_dest_level  = (r_cop_start_level > r_cop_dest_level) ? r_cop_start_level : r_cop_dest_level;
         trans_attr.cop_final_state = r_cop_final_state;
         trans_attr.atomic_conf     = r_atomic_conf;
         trans_attr.atomic_transid  = r_atomic_transid;
         trans_attr.subopcode_dest  = r_subopcode_dest;
         trans_attr.message_conf    = r_message_conf;
         trans_attr.coop_target     = r_coop_target;
         m_trans_attrs[count--] = trans_attr;
      end

      if (m_cop_start_level_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_cop_start_level_list.size();
         while (count >= 0) begin
            m_cop_start_level_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].cop_start_level = m_cop_start_level_list[ii];
               if (m_trans_attrs[count].cop_start_level > m_trans_attrs[count].cop_dest_level) begin
                  m_trans_attrs[count].cop_dest_level = m_trans_attrs[count].cop_start_level;
               end
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_cop_dest_level_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_cop_dest_level_list.size();
         while (count >= 0) begin
            m_cop_dest_level_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].cop_dest_level = m_cop_dest_level_list[ii];
               if (m_trans_attrs[count].cop_start_level > m_trans_attrs[count].cop_dest_level) begin
                  m_trans_attrs[count].cop_start_level = m_trans_attrs[count].cop_dest_level;
               end
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_cop_final_state_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_cop_final_state_list.size();
         while (count >= 0) begin
            m_cop_final_state_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].cop_final_state = m_cop_final_state_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_atomic_conf_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_atomic_conf_list.size();
         while (count >= 0) begin
            m_atomic_conf_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].atomic_conf = m_atomic_conf_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_atomic_transid_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_atomic_transid_list.size();
         while (count >= 0) begin
            m_atomic_transid_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].atomic_transid = m_atomic_transid_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_subopcode_dest_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_subopcode_dest_list.size();
         while (count >= 0) begin
            m_subopcode_dest_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].subopcode_dest = m_subopcode_dest_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_message_conf_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_message_conf_list.size();
         while (count >= 0) begin
            m_message_conf_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].message_conf = m_message_conf_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      if (m_coop_target_list.size() > 0) begin
         int limit;

         count = m_trans_attrs.size() - 1;
         limit = m_coop_target_list.size();
         while (count >= 0) begin
            m_coop_target_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_trans_attrs[count].coop_target = m_coop_target_list[ii];
               count--;
            end
         end
         m_trans_attrs.shuffle();
      end

      m_addr_spaces = new[r_addr_sp_count];
      count         = r_addr_sp_count - 1;
      if (m_addr_sp_list.size() > 0) begin
         int limit;

         limit = m_addr_sp_list.size();
         while (count >= 0) begin
            m_addr_sp_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_addr_spaces[count--] = m_addr_sp_list[ii];
            end
         end
         m_addr_spaces.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_addr_sp);
            m_addr_spaces[count--] = r_addr_sp;
         end
      end

      m_req_sources = new[r_req_source_count];
      count         = r_req_source_count - 1;
      if (m_req_source_list.size() > 0) begin
         int limit;

         limit = m_req_source_list.size();
         while (count >= 0) begin
            m_req_source_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_req_sources[count--] = m_req_source_list[ii];
            end
         end
         m_req_sources.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_req_source);
            m_req_sources[count--] = r_req_source;
         end
      end

      m_cline_zeros = new[r_cline_zero_count];
      count         = r_cline_zero_count - 1;
      if (m_cline_zero_list.size() > 0) begin
         int limit;

         limit = m_cline_zero_list.size();
         while (count >= 0) begin
            m_cline_zero_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_cline_zeros[count--] = m_cline_zero_list[ii];
            end
         end
         m_cline_zeros.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_cline_zero);
            m_cline_zeros[count--] = r_cline_zero;
         end
      end

      m_qoses = new[r_qos_count];
      count = r_qos_count - 1;
      if (m_qos_list.size() > 0) begin
         int limit;

         limit = m_qos_list.size();
         while (count >= 0) begin
            m_qos_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_qoses[count--] = m_qos_list[ii];
            end
         end
         m_qoses.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_qos);
            m_qoses[count--] = r_qos;
         end
      end

      m_vcs = new[r_vc_count];
      count = r_vc_count - 1;
      if (m_vc_list.size() > 0) begin
         int limit;

         limit = m_vc_list.size();
         while (count >= 0) begin
            m_vc_list.shuffle();
            for (int ii = 0; ((ii < limit) && (count >= 0)); ii++) begin
               m_vcs[count--] = m_vc_list[ii];
            end
         end
         m_vcs.shuffle();
      end
      else begin
         while (count >= 0) begin
            randomize(r_vc);
            m_vcs[count--] = r_vc;
         end
      end
      post_randomize_me();
   endfunction : randomize_me


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_trans_attr
   //
   virtual function evl_cmd_t fix_trans_attr(input evl_cmd_t      cmd_in,
                                           ref   evl_trans_attr_s trans_attr);
      if (cmd_in inside { CACHE_EVICT, CACHE_FLUSH } ) begin
         if (trans_attr.cop_dest_level <= trans_attr.cop_start_level) begin
            if (trans_attr.cop_start_level == 3) begin
               trans_attr.cop_start_level = 2;
               trans_attr.cop_dest_level  = 3;
            end
            else begin
               trans_attr.cop_dest_level = trans_attr.cop_start_level + 1;
            end
         end
         if (trans_attr.cop_dest_level < 2) begin
            return NOP;
         end
      end
      else if (cmd_in inside { CACHE_EVICT_TO_MEM, CACHE_FLUSH_TO_MEM } ) begin
         trans_attr.cop_dest_level = 3;
      end
      else if (cmd_in inside { CACHE_PREFETCH } ) begin
         if (trans_attr.cop_dest_level < 1) begin
            return NOP;
         end
      end
      else if (cmd_in inside { CACHE_SCP_FILL } ) begin
         void'(get_next_fill_addr(trans_attr.scp_fill_addr));
      end
      return cmd_in;
   endfunction : fix_trans_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      disable_stimulus();
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: add_test_message_info
   //
   function void add_test_message_info(input string message);
      if (m_test_message.len() >= 100) begin
         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("%s,", m_test_message))
         m_test_message = "";
      end
      if (m_test_message == "") begin
         m_test_message = message;
      end
      else begin
         m_test_message = $sformatf("%s, %s", m_test_message, message);
      end
   endfunction : add_test_message_info


   //-----------------------------------------------------------------------------------------------
   //
   // Task: flush_test_message_info
   //
   function void flush_test_message_info();
      if (m_test_message.len() != 0) begin
         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), m_test_message)
      end
      m_test_message = "";
      `evl_log_no_time(UVM_FULL, VMOD_TEST, "")
   endfunction : flush_test_message_info


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_init
   //
   virtual task pre_main_init(input uvm_phase phase);
      return;
   endtask : pre_main_init


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.pre_main_phase(phase);
         end
         begin
            #0 randomize_me();
            #0 pre_main_init(phase);
         end
      join
      phase.drop_objection(this);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      if (`evl_log_check_verbosity(UVM_FULL, VMOD_TEST) != 0) begin
         m_print_contents.get();
         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("%0s contents:", get_abstract_name()))
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_cmds[] size: %0d", m_cmds.size()))
         foreach (m_cmds[ii]) begin
            add_test_message_info($sformatf("%p", m_cmds[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_paddrs[] size: %0d", m_paddrs.size()))
         foreach (m_paddrs[ii]) begin
            add_test_message_info($sformatf("0x%x", m_paddrs[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_scp_addrs[] size: %0d", m_scp_addrs.size()))
         foreach (m_scp_addrs[ii]) begin
            add_test_message_info($sformatf("0x%x", m_scp_addrs[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_fill_addrs[] size: %0d", m_fill_addrs.size()))
         foreach (m_fill_addrs[ii]) begin
            add_test_message_info($sformatf("0x%x", m_fill_addrs[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_cache_attrs[] size: %0d", m_cache_attrs.size()))
         foreach (m_cache_attrs[ii]) begin
            add_test_message_info($sformatf("%0d", m_cache_attrs[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_cmd_lengths[] size: %0d", m_cmd_lengths.size()))
         foreach (m_cmd_lengths[ii]) begin
            add_test_message_info($sformatf("%0d", m_cmd_lengths[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_addr_spaces[] size: %0d", m_addr_spaces.size()))
         foreach (m_addr_spaces[ii]) begin
            add_test_message_info($sformatf("%p", m_addr_spaces[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_dwords[] size: %0d", m_dwords.size()))
         foreach (m_dwords[ii]) begin
            add_test_message_info($sformatf("0x%x", m_dwords[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_qword_ens[] size: %0d", m_qword_ens.size()))
         foreach (m_qword_ens[ii]) begin
            add_test_message_info($sformatf("%b", m_qword_ens[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_req_sources[] size: %0d", m_req_sources.size()))
         foreach (m_req_sources[ii]) begin
            add_test_message_info($sformatf("0x%0x", m_req_sources[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_trans_attrs[] size: %0d", m_trans_attrs.size()))
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_vcs[] size: %0d", m_vcs.size()))
         foreach (m_vcs[ii]) begin
            add_test_message_info($sformatf("%0d", m_vcs[ii]));
         end
         flush_test_message_info();

         `evl_log_no_addr(UVM_FULL, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("m_qoses[] size: %0d", m_qoses.size()))
         foreach (m_qoses[ii]) begin
            add_test_message_info($sformatf("0x%0x", m_qoses[ii]));
         end
         flush_test_message_info();

         m_print_contents.put();
      end
      enable_stimulus();
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      disable_stimulus();
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_mrand");
      super.new(name_in);
   endfunction : new

endclass : evl_sequence_mrand

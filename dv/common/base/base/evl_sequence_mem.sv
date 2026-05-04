//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_mem
//
class evl_sequence_mem extends evl_sequence #(evl_mem_desc);

   `uvm_object_utils(evl_sequence_mem)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_delay_wrapper    m_delay_wrapper = null;
   semaphore            m_issue_lock;

   evl_cmd_t            m_cmd;
   evl_paddr_t          m_paddr;
   evl_vaddr_t          m_vaddr;
   bit                  m_cache_attr;
   evl_cmd_length_t     m_cmd_length;
   evl_addr_sp_t        m_addr_space;
   evl_dword_t          m_dword;
   evl_line_qword_en_t  m_qword_en;
   evl_bus_req_source_t m_req_source;
   evl_trans_attr_s     m_trans_attr;
   evl_bus_qos_t        m_qos;
   evl_bus_req_vc_t     m_vc;
   evl_thread_id_t      m_thread_id;
   evl_trans_desc       m_template_trans_desc;
   int                  m_allow_unaligned;
   int                  m_allow_et_unaligned;
   bit                  m_fixup_message;

   evl_cmd_t            m_cmds[];
   evl_paddr_t          m_paddrs[];
   evl_paddr_t          m_scp_addrs[];
   evl_paddr_t          m_fill_addrs[];
   evl_vaddr_t          m_vaddrs[];
   bit                  m_cache_attrs[];
   evl_cmd_length_t     m_cmd_lengths[];
   evl_addr_sp_t        m_addr_spaces[];
   evl_dword_t          m_dwords[];
   evl_line_qword_en_t  m_qword_ens[];
   evl_bus_req_source_t m_req_sources[];
   evl_trans_attr_s     m_trans_attrs[];
   evl_bus_qos_t        m_qoses[];
   evl_bus_req_vc_t     m_vcs[];
   evl_thread_id_t      m_thread_ids[];
   bit                  m_cline_zeros[];

   evl_cmd_t            m_cmd_dir;
   evl_paddr_t          m_paddr_dir;
   evl_paddr_t          m_scp_addr_dir;
   evl_paddr_t          m_fill_addr_dir;
   evl_vaddr_t          m_vaddr_dir;
   bit                  m_cache_attr_dir;
   evl_cmd_length_t     m_cmd_length_dir;
   evl_addr_sp_t        m_addr_space_dir;
   evl_dword_t          m_dwords_dir[`EVL_LINE_DWORDS];
   evl_line_qword_en_t  m_qword_en_dir;
   evl_bus_req_source_t m_req_source_dir;
   evl_trans_attr_s     m_trans_attr_dir;
   evl_bus_qos_t        m_qos_dir;
   evl_bus_req_vc_t     m_vc_dir;
   evl_thread_id_t      m_thread_id_dir;
   bit                  m_cline_zero_dir;

   int                  m_cmds_index         = 0;
   int                  m_paddrs_index       = 0;
   int                  m_scp_addrs_index    = 0;
   int                  m_fill_addrs_index   = 0;
   int                  m_vaddrs_index       = 0;
   int                  m_cache_attrs_index  = 0;
   int                  m_cmd_lengths_index  = 0;
   int                  m_addr_spaces_index  = 0;
   int                  m_dwords_index       = 0;
   int                  m_dwords_index_dir   = 0;
   int                  m_qword_ens_index    = 0;
   int                  m_req_sources_index  = 0;
   int                  m_trans_attrs_index  = 0;
   int                  m_qos_index          = 0;
   int                  m_vcs_index          = 0;
   int                  m_thread_ids_index   = 0;
   int                  m_cline_zeros_index  = 0;
   int                  m_dword_start_index  = 0;

   bit                  m_dir_trans_pend   = 1'b0;
   bit                  m_dir_trans_avail  = 1'b0;
   bit                  m_trans_issued     = 1'b0;
   bit                  m_shift_dwords     = 1'b0;
   bit                  m_drop_on_conflict = 1'b0;
   evl_trans_desc       m_last_trans_desc  = null;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_drop_on_conflict
   //
   virtual function void set_drop_on_conflict(input bit value_in);
      m_drop_on_conflict = value_in;
   endfunction : set_drop_on_conflict


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_stimulus
   //
   virtual function bit have_stimulus();
      if (m_dir_trans_pend == 1'b1) begin
         return 1'b1;
      end
      if ( (m_cmds.size()        < 1) ||
           (m_paddrs.size()      < 1) ||
           (m_cache_attrs.size() < 1) ||
           (m_cmd_lengths.size() < 1) ||
           (m_addr_spaces.size() < 1) ) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : have_stimulus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_cmd
   //
   virtual function bit get_next_cmd(output evl_cmd_t cmd);
      if (m_dir_trans_avail == 1'b1) begin
         cmd = m_cmd_dir;
      end
      else begin
         int array_size = m_cmds.size();

         if (array_size < 1) begin
            cmd = NOCMD;
            return 1'b0;
         end
         if (array_size <= m_cmds_index) begin
            m_cmds_index = 0;
         end
         cmd = m_cmds[m_cmds_index++];
      end
      return 1'b1;
   endfunction : get_next_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_cache_attr
   //
   virtual function bit get_next_cache_attr(output bit cache_attr);
      if (m_dir_trans_avail == 1'b1) begin
         cache_attr = m_cache_attr_dir;
      end
      else begin
         int array_size = m_cache_attrs.size();

         if (array_size < 1) begin
            cache_attr = 1'b1;
            return 1'b0;
         end
         if (array_size <= m_cache_attrs_index) begin
            m_cache_attrs_index = 0;
         end
         cache_attr = m_cache_attrs[m_cache_attrs_index++];
      end
      return 1'b1;
   endfunction : get_next_cache_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_cmd_length
   //
   virtual function bit get_next_cmd_length(output evl_cmd_length_t cmd_length);
      if (m_dir_trans_avail == 1'b1) begin
         cmd_length = m_cmd_length_dir;
      end
      else begin
         int array_size = m_cmd_lengths.size();

         if (array_size < 1) begin
            cmd_length = evl_cmd_length_t'(0);
            return 1'b0;
         end
         if (array_size <= m_cmd_lengths_index) begin
            m_cmd_lengths_index = 0;
         end
         cmd_length = m_cmd_lengths[m_cmd_lengths_index++];
      end
      return 1'b1;
   endfunction : get_next_cmd_length


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_addr_space
   //
   virtual function bit get_next_addr_sp(output evl_addr_sp_t addr_sp);
      if (m_dir_trans_avail == 1'b1) begin
         addr_sp = m_addr_space_dir;
      end
      else begin
         int array_size = m_addr_spaces.size();

         if (array_size < 1) begin
            addr_sp = Mem;
            return 1'b0;
         end
         if (array_size <= m_addr_spaces_index) begin
            m_addr_spaces_index = 0;
         end
         addr_sp = m_addr_spaces[m_addr_spaces_index++];
      end
      return 1'b1;
   endfunction : get_next_addr_sp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_paddr
   //
   virtual function bit get_next_paddr(output evl_paddr_t paddr);
      if (m_dir_trans_avail == 1'b1) begin
         paddr = m_paddr_dir;
      end
      else begin
         int array_size = m_paddrs.size();

         if (array_size < 1) begin
            paddr = ~evl_paddr_t'(0);
            return 1'b0;
         end
         if (array_size <= m_paddrs_index) begin
            m_paddrs_index = 0;
         end
         paddr = m_paddrs[m_paddrs_index++];
      end
      return 1'b1;
   endfunction : get_next_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_scp_addr
   //
   virtual function bit get_next_scp_addr(output evl_paddr_t paddr);
      if (m_dir_trans_avail == 1'b1) begin
         paddr = m_scp_addr_dir;
      end
      else begin
         int array_size = m_scp_addrs.size();

         if (array_size < 1) begin
            paddr = ~evl_paddr_t'(0);
            return 1'b0;
         end
         if (array_size <= m_scp_addrs_index) begin
            m_scp_addrs_index = 0;
         end
         paddr = m_scp_addrs[m_scp_addrs_index++];
      end
      return 1'b1;
   endfunction : get_next_scp_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_fill_addr
   //
   virtual function bit get_next_fill_addr(output evl_paddr_t paddr);
      if (m_dir_trans_avail == 1'b1) begin
         paddr = m_fill_addr_dir;
      end
      else begin
         int array_size = m_fill_addrs.size();

         if (array_size < 1) begin
            paddr = ~evl_paddr_t'(0);
            return 1'b0;
         end
         if (array_size <= m_fill_addrs_index) begin
            m_fill_addrs_index = 0;
         end
         paddr = m_fill_addrs[m_fill_addrs_index++];
      end
      return 1'b1;
   endfunction : get_next_fill_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_vaddr
   //
   virtual function bit get_next_vaddr(output evl_vaddr_t vaddr);
      if (m_dir_trans_avail == 1'b1) begin
         vaddr = m_vaddr_dir;
      end
      else begin
         int array_size = m_vaddrs.size();

         if (array_size < 1) begin
            vaddr = ~evl_vaddr_t'(0);
            return 1'b0;
         end
         if (array_size <= m_vaddrs_index) begin
            m_vaddrs_index = 0;
         end
         vaddr = m_vaddrs[m_vaddrs_index++];
      end
      return 1'b1;
   endfunction : get_next_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_dword
   //
   virtual function bit get_next_dword(output evl_dword_t dword);
      if (m_dir_trans_avail == 1'b1) begin
         if (m_dwords_index_dir >= `EVL_LINE_DWORDS) begin
            m_dwords_index_dir = 0;
         end
         if (m_shift_dwords == 1'b0) begin
            dword = m_dwords_dir[m_dwords_index_dir++];
         end
         else begin
            dword = m_dwords_dir[m_dwords_index_dir];
            m_dwords_dir[m_dwords_index_dir++] = { dword[0:0], dword[`EVL_DWORD_MSB:1] };
         end
      end
      else begin
         int array_size = m_dwords.size();

         if (array_size < 1) begin
            dword = ~evl_dword_t'(0);
            return 1'b0;
         end
         if (m_shift_dwords == 1'b0) begin
            if (array_size <= m_dwords_index) begin
               m_dwords_index = 0;
            end
            dword = m_dwords[m_dwords_index++];
         end
         else begin
            if (array_size <= m_dwords_index) begin
               m_dword_start_index = (m_dword_start_index + 1) & 7;
               m_dwords_index      = m_dword_start_index;
               if (array_size <= m_dwords_index) begin
                  m_dwords_index = 0;
               end
            end
            dword = m_dwords[m_dwords_index];
            m_dwords[m_dwords_index++] = { dword[0:0], dword[`EVL_DWORD_MSB:1] };
         end
      end
      return 1'b1;
   endfunction : get_next_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_qword_en
   //
   virtual function bit get_next_qword_en(output evl_line_qword_en_t qword_en);
      if (m_dir_trans_avail == 1'b1) begin
         qword_en = m_qword_en_dir;
      end
      else begin
         int array_size = m_qword_ens.size();

         if (array_size < 1) begin
            qword_en = ~evl_line_qword_en_t'(0);
            return 1'b0;
         end
         if (array_size <= m_qword_ens_index) begin
            m_qword_ens_index = 0;
         end
         qword_en = m_qword_ens[m_qword_ens_index++];
      end
      return 1'b1;
   endfunction : get_next_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_req_source
   //
   virtual function bit get_next_req_source(output evl_bus_req_source_t req_source);
      if (m_dir_trans_avail == 1'b1) begin
         req_source = m_req_source_dir;
      end
      else begin
         int array_size = m_req_sources.size();

         if (array_size < 1) begin
            req_source = evl_bus_req_source_t'(0);
            return 1'b0;
         end
         if (array_size <= m_req_sources_index) begin
            m_req_sources_index = 0;
         end
         req_source = m_req_sources[m_req_sources_index++];
      end
      return 1'b1;
   endfunction : get_next_req_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_trans_attr
   //
   virtual function bit get_next_trans_attr(output evl_trans_attr_s trans_attr);
      if (m_dir_trans_avail == 1'b1) begin
         trans_attr = m_trans_attr_dir;
      end
      else begin
         int array_size = m_trans_attrs.size();

         if (array_size < 1) begin
            trans_attr = evl_trans_attr_s'(0);
            return 1'b0;
         end
         if (array_size <= m_trans_attrs_index) begin
            m_trans_attrs_index = 0;
         end
         trans_attr = m_trans_attrs[m_trans_attrs_index++];
      end
      return 1'b1;
   endfunction : get_next_trans_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_qos
   //
   virtual function bit get_next_qos(output evl_bus_qos_t qos);
      if (m_dir_trans_avail == 1'b1) begin
         qos = m_qos_dir;
      end
      else begin
         int array_size = m_qoses.size();

         if (array_size < 1) begin
            qos = evl_bus_qos_t'(0);
            return 1'b0;
         end
         if (array_size <= m_qos_index) begin
            m_qos_index = 0;
         end
         qos = m_qoses[m_qos_index++];
      end
      return 1'b1;
   endfunction : get_next_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_vc
   //
   virtual function bit get_next_vc(output evl_bus_req_vc_t vc);
      if (m_dir_trans_avail == 1'b1) begin
         vc = m_vc_dir;
      end
      else begin
         int array_size = m_vcs.size();

         if (array_size < 1) begin
            vc = ~evl_bus_req_vc_t'(0);
            return 1'b0;
         end
         if (array_size <= m_vcs_index) begin
            m_vcs_index = 0;
         end
         vc = m_vcs[m_vcs_index++];
      end
      return 1'b1;
   endfunction : get_next_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_thread_id
   //
   virtual function bit get_next_thread_id(output evl_thread_id_t thread_id);
      if (m_dir_trans_avail == 1'b1) begin
         thread_id = m_thread_id_dir;
      end
      else begin
         int array_size = m_thread_ids.size();

         if (array_size < 1) begin
            thread_id = evl_thread_id_t'(0);
            return 1'b0;
         end
         if (array_size <= m_thread_ids_index) begin
            m_thread_ids_index = 0;
         end
         thread_id = m_thread_ids[m_thread_ids_index++];
      end
      return 1'b1;
   endfunction : get_next_thread_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_cline_zero
   //
   virtual function bit get_next_cline_zero(output bit cline_zero);
      if (m_dir_trans_avail == 1'b1) begin
         cline_zero = m_cline_zero_dir;
      end
      else begin
         int array_size = m_cline_zeros.size();

         if (array_size < 1) begin
            cline_zero = 1'b0;
            return 1'b0;
         end
         if (array_size <= m_cline_zeros_index) begin
            m_cline_zeros_index = 0;
         end
         cline_zero = m_cline_zeros[m_cline_zeros_index++];
      end
      return 1'b1;
   endfunction : get_next_cline_zero


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_addr_sp
   //
   virtual function evl_addr_sp_t fix_addr_sp(input evl_cmd_t     cmd_in,
                                              input evl_addr_sp_t addr_sp_in);
      if (cmd_in == NOP) begin
         return Mem;
      end
      return addr_sp_in;
   endfunction : fix_addr_sp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_cache_attr
   //
   virtual function bit fix_cache_attr(input evl_cmd_t     cmd_in,
                                       input evl_addr_sp_t addr_sp_in,
                                       input bit           cache_attr_in);
      if (cmd_in == NOP) begin
         return 1'b0;
      end
      else if ((cmd_in inside { `EVL_COP_CMDS }) && (!(cmd_in inside { CACHE_EVICT, CACHE_EVICT_TO_MEM, CACHE_FLUSH, CACHE_FLUSH_TO_MEM, CACHE_PREFETCH }))) begin
         return 1'b1;
      end
      return cache_attr_in;
   endfunction : fix_cache_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_cmd_length
   //
   virtual function evl_cmd_length_t fix_cmd_length(input evl_cmd_t        cmd_in,
                                                    input evl_addr_sp_t    addr_sp_in,
                                                    input bit              cache_attr_in,
                                                    input evl_cmd_length_t cmd_length_in,
                                                    input evl_trans_attr_s trans_attrs_in,
                                                    input evl_paddr_t      addr_in);
      if (cmd_in == NOP) begin
         return 0;
      end
      if (cmd_in inside { ATOMIC }) begin
         if ((trans_attrs_in.atomic_conf & `EVL_BUS_ATOMIC_CONF_VEC_OP) != 0) begin
            return `EVL_OWORD_BYTES;
         end
         else begin
            if ((trans_attrs_in.atomic_conf & `EVL_BUS_ATOMIC_CONF_DW_OP) != 0) begin
               return `EVL_DWORD_BYTES;
            end
            else begin
               return `EVL_WORD_BYTES;
            end
         end
      end
      else if (cmd_in inside { CACHE_EVICT_TO_MEM, CACHE_FLUSH_TO_MEM }) begin
         return `EVL_LINE_BYTES;
      end
      else if (cmd_in inside { ST_WA }) begin
         if (int'(cmd_length_in) > `EVL_OWORD_BYTES) begin
            return `EVL_LINE_BYTES;
         end
         else if (int'(cmd_length_in) > `EVL_QWORD_BYTES) begin
            return `EVL_OWORD_BYTES;
         end
         else begin
            return `EVL_QWORD_BYTES;
         end
      end
      else if (cmd_in inside { CACHE_LOCK, CACHE_LOCK_SW, CACHE_UNLOCK, CACHE_UNLOCK_SW, CACHE_EVICT, CACHE_EVICT_SW, CACHE_FLUSH, CACHE_FLUSH_SW, CACHE_PREFETCH }) begin
         return 1;
      end
      else if (cmd_in inside { CACHE_SCP_FILL }) begin
         return 8;
      end
      else if (cmd_in inside { LD, LD_COOP, ST, LD_NO_MOD, ST_NO_MOD, ST_DBG_NOC, MESSAGE }) begin
         if (int'(cmd_length_in) > `EVL_OWORD_BYTES) begin
            return `EVL_LINE_BYTES;
         end
         else if (int'(cmd_length_in) > `EVL_QWORD_BYTES) begin
            return `EVL_OWORD_BYTES;
         end
         else if (int'(cmd_length_in) > `EVL_DWORD_BYTES) begin
            return `EVL_QWORD_BYTES;
         end
         else if (int'(cmd_length_in) > `EVL_WORD_BYTES) begin
            return `EVL_DWORD_BYTES;
         end
         else if (int'(cmd_length_in) > `EVL_HWORD_BYTES) begin
            return `EVL_WORD_BYTES;
         end
         return cmd_length_in;
      end
      else if ((!(cmd_in inside { ATOMIC, MESSAGE, IFETCH, REFETCH, CANCEL })) && (cache_attr_in == 1'b1)) begin
         return `EVL_LINE_BYTES;
      end
      return cmd_length_in;
   endfunction : fix_cmd_length


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_paddr
   //
   virtual function evl_paddr_t fix_paddr(input evl_cmd_t        cmd_in,
                                          input evl_addr_sp_t    addr_sp_in,
                                          input bit              cache_attr_in,
                                          input evl_cmd_length_t cmd_length_in,
                                          input evl_trans_attr_s trans_attrs_in,
                                          input evl_paddr_t      paddr_in,
                                          input evl_verif_comp   agent_in);
      evl_paddr_t paddr = paddr_in;

      if (cmd_in == NOP) begin
         return evl_paddr_t'(0);
      end
      else if (cmd_in inside { MESSAGE }) begin
         if (m_fixup_message == 1'b1) begin
            if (cache_attr_in == 1'b0) begin
               paddr = (paddr_in & `EVL_MSG_ADDR_AND_MASK) | (`EVL_MSG_ADDR_OR_MASK);
            end
            else if (agent_in != null) begin
               paddr = (paddr_in & `EVL_MSG_ADDR_AND_MASK & `EVL_MSG_SHIRE_AND_MASK) | (`EVL_MSG_ADDR_OR_MASK) | (agent_in.get_shire_id() << `EVL_MSG_SHIRE_ID_LSB);
            end
            else begin
               paddr = (paddr_in & `EVL_MSG_ADDR_AND_MASK & `EVL_MSG_SHIRE_AND_MASK) | (`EVL_MSG_ADDR_OR_MASK) | (`EVL_MSG_LOCAL_SHIRE_ID << `EVL_MSG_SHIRE_ID_LSB);
            end
         end
      end
      else if (cmd_in inside { ATOMIC }) begin
         if ((cache_attr_in == 1'b1) && (m_allow_unaligned == 0)) begin
            paddr[5:0] = {6{1'b0}};
         end
         else if ((trans_attrs_in.atomic_conf & `EVL_BUS_ATOMIC_CONF_VEC_OP) != 0) begin
            paddr[4:0] = {5{1'b0}};
         end
         else if ((trans_attrs_in.atomic_conf & `EVL_BUS_ATOMIC_CONF_DW_OP) != 0) begin
            paddr[2:0] = {3{1'b0}};
         end
         else begin
            paddr[1:0] = {2{1'b0}};
         end
      end
      else if (cmd_in inside { CACHE_LOCK, CACHE_LOCK_SW, CACHE_UNLOCK, CACHE_UNLOCK_SW, CACHE_EVICT, CACHE_EVICT_SW, CACHE_EVICT_TO_MEM, CACHE_FLUSH, CACHE_FLUSH_SW, CACHE_FLUSH_TO_MEM, CACHE_PREFETCH, CACHE_SCP_FILL }) begin
         paddr[5:0] = {6{1'b0}};
      end
      else if (cmd_in inside { ST_DBG_NOC }) begin
         return paddr;
      end
      else if ((cache_attr_in == 1'b1) && (m_allow_unaligned == 0)) begin
         paddr[5:0] = {6{1'b0}};
      end
      else if (cmd_length_in > 32) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[5:0] = {6{1'b0}};
         end
      end
      else if (cmd_length_in > 16) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[4:0] = {5{1'b0}};
         end
      end
      else if (cmd_length_in > 8) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[3:0] = {4{1'b0}};
         end
      end
      else if (cmd_length_in > 4) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[2:0] = {3{1'b0}};
         end
      end
      else if (cmd_length_in > 2) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[1:0] = {2{1'b0}};
         end
      end
      else if (cmd_length_in > 1) begin
         if ((cache_attr_in == 1'b0) || (m_allow_et_unaligned == 1'b0) || ((cmd_length_in + paddr[`EVL_LINE_ADDR_LSB-1:0]) > `EVL_LINE_BYTES)) begin
            paddr[0:0] = {1{1'b0}};
         end
      end
      return paddr;
   endfunction : fix_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_trans_attr
   //
   virtual function evl_cmd_t fix_trans_attr(input evl_cmd_t        cmd_in,
                                             ref   evl_trans_attr_s trans_attr);
      if (cmd_in inside { CACHE_EVICT_TO_MEM, CACHE_FLUSH_TO_MEM } ) begin
         trans_attr.cop_dest_level = 3;
      end
      return cmd_in;
   endfunction : fix_trans_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Task: issue
   //
   virtual task issue(output evl_trans_desc       trans_desc,
                      input  evl_cmd_t            cmd_in,
                      input  evl_paddr_t          paddr_in,
                      input  evl_cmd_length_t     length_in        = evl_cmd_length_t'(1),
                      input  evl_bus_req_source_t req_source_in    = evl_bus_req_source_t'(0),
                      input  evl_dword_t          dwords_in[]      = {},
                      input  evl_delay_wrapper    delay_wrapper_in = null,
                      input  evl_line_qword_en_t  qword_en_in      = ~evl_line_qword_en_t'(0),
                      input  evl_trans_attr_s     trans_attr_in    = evl_trans_attr_s'(0),
                      input  bit                  cache_attr_in    = 1'b1,
                      input  evl_bus_qos_t        qos_in           = evl_bus_qos_t'(0),
                      input  evl_bus_req_vc_t     vc_in            = ~evl_bus_req_vc_t'(0),
                      input  evl_thread_id_t      thread_id_in     = evl_thread_id_t'(0));
      int index;
      int limit;

      if ((stimulus_is_enabled() == 1'b0) && (cmd_in != BARRIER)) begin
         wait_for_stimulus_enable();
      end
      m_issue_lock.get(1);
      wait (m_dir_trans_pend == 1'b0);
      m_cmd_dir        = cmd_in;
      m_paddr_dir      = paddr_in;
      m_scp_addr_dir   = paddr_in;
      m_fill_addr_dir  = trans_attr_in.scp_fill_addr;
      m_cmd_length_dir = length_in;
      m_cache_attr_dir = cache_attr_in;
      m_addr_space_dir = Mem;
      m_qword_en_dir   = qword_en_in;
      m_req_source_dir = req_source_in;
      m_trans_attr_dir = trans_attr_in;
      m_qos_dir        = qos_in;
      m_vc_dir         = vc_in;
      m_thread_id_dir  = thread_id_in;
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_dwords_dir[ii] = ~evl_dword_t'(0);
      end
      if (cmd_in inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         limit = dwords_in.size();
         if (!(cmd_in inside { `EVL_JTAG_CMDS })) begin
            if (length_in <= 8) begin
               limit = (limit > 1) ? 1 : limit;
            end
            else if (length_in <= 16) begin
               limit = (limit > 2) ? 2 : limit;
            end
            else if (length_in <= 32) begin
               limit = (limit > 4) ? 4 : limit;
            end
            else begin
               limit = (limit > 8) ? 8 : limit;
            end
         end
         index = 0;
         for (int ii = limit - 1; ii >= 0; ii--) begin
            m_dwords_dir[index++] = dwords_in[ii];
         end
      end
      if (delay_wrapper_in != null) begin
         m_delay_wrapper = delay_wrapper_in.clone();
      end
      else begin
         m_delay_wrapper = null;
      end
      m_trans_issued = 1'b0;
      #0 m_dir_trans_pend = 1'b1;
      wait (m_trans_issued == 1'b1);
      trans_desc = m_last_trans_desc;
      m_issue_lock.put(1);
      if ((cmd_in == BARRIER) && (trans_desc != null)) begin
         trans_desc.wait_for_done();
      end
   endtask : issue


   //-----------------------------------------------------------------------------------------------
   //
   // Task: blocking_issue
   //
   virtual task blocking_issue(output evl_trans_desc       trans_desc,
                               input  evl_cmd_t            cmd_in,
                               input  evl_paddr_t          paddr_in,
                               input  evl_cmd_length_t     length_in        = evl_cmd_length_t'(1),
                               input  evl_bus_req_source_t req_source_in    = evl_bus_req_source_t'(0),
                               input  evl_dword_t          dwords_in[]      = {},
                               input  evl_delay_wrapper    delay_wrapper_in = null,
                               input  evl_line_qword_en_t  qword_en_in      = ~evl_line_qword_en_t'(0),
                               input  evl_trans_attr_s     trans_attr_in    = evl_trans_attr_s'(0),
                               input  bit                  cache_attr_in    = 1'b1,
                               input  evl_bus_qos_t        qos_in           = evl_bus_qos_t'(0),
                               input  evl_bus_req_vc_t     vc_in            = ~evl_bus_req_vc_t'(0),
                               input  evl_thread_id_t      thread_id_in     = evl_thread_id_t'(0));
      issue(trans_desc, cmd_in, paddr_in, length_in, req_source_in, dwords_in, delay_wrapper_in, qword_en_in, trans_attr_in, cache_attr_in, qos_in, vc_in, thread_id_in);
      if (trans_desc != null) begin
         trans_desc.wait_for_done();
      end
   endtask : blocking_issue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_desc
   //
   virtual function evl_trans_desc get_trans_desc(input evl_sequencer_mem sequencer_in,
                                                  input evl_verif_comp    agent_in,
                                                  input evl_trans_desc    template_in);
      evl_trans_desc       trans_desc;
      evl_cmd_t            cmd;
      evl_paddr_t          paddr;
      bit                  cache_attr;
      evl_cmd_length_t     cmd_length;
      evl_addr_sp_t        addr_sp;
      evl_bus_req_source_t req_source;
      evl_trans_attr_s     trans_attrs;
      evl_bus_qos_t        qos;
      evl_bus_req_vc_t     vc;
      evl_thread_id_t      thread_id;

      m_trans_issued = 1'b0;

      if ((stimulus_is_disabled() == 1'b1) || (have_stimulus() == 1'b0)) begin
         if ((m_dir_trans_pend == 1'b0) || (m_cmd_dir != BARRIER)) begin
            return null;
         end
      end
      if (m_dir_trans_pend == 1'b1) begin
         m_dir_trans_avail  = 1'b1;
         m_dwords_index_dir = 0;
      end
      else begin
         m_dir_trans_avail = 1'b0;
      end
      if (template_in != null) begin
         trans_desc = template_in.clone_desc(agent_in);
      end
      else if (m_template_trans_desc != null) begin
         trans_desc = m_template_trans_desc.clone_desc(agent_in);
      end
      else begin
         trans_desc = new(agent_in.get_abstract_name());
         trans_desc.set_master_agent(agent_in);
      end
      if (agent_in != null) begin
         trans_desc.set_shire_id(agent_in.get_shire_id());
         trans_desc.set_neighborhood_id(agent_in.get_neighborhood_id());
      end
      trans_desc.set_trans_id();

      void'(get_next_cmd(cmd));
      if (cmd inside { `EVL_JTAG_CMDS_WITH_READ_DATA }) begin
         cmd_length = `EVL_LINE_DWORDS;
         void'(get_next_paddr(paddr));
      end
      else if (cmd inside { `EVL_JTAG_CMDS }) begin
         cmd_length = 0;
         paddr      = evl_paddr_t'(0);
      end
      else begin
         if (cmd inside { CACHE_SCP_FILL }) begin
            void'(get_next_scp_addr(paddr));
         end
         else begin
            void'(get_next_paddr(paddr));
         end
         void'(get_next_cache_attr(cache_attr));
         void'(get_next_cmd_length(cmd_length));
         void'(get_next_addr_sp(addr_sp));
         if (cmd inside { LD_COOP }) begin
            req_source = 12; //Cooperative
         end
         else begin
            void'(get_next_req_source(req_source));
         end
         // note: ST_WA has a subopcode inside the neighborhood, but it'll be 0 to the shire cache
         if (cmd inside { `EVL_COP_CMDS, LD_COOP, MESSAGE, ST_WA }) begin
            void'(get_next_trans_attr(trans_attrs));
            trans_attrs.subopcode_dest = evl_bus_subopcode_t'(0);
            `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("%p subopcode %0d", cmd, trans_attrs.subopcode_dest))
         end
         else if (cmd inside {LD, ST, LD_NO_MOD, ST_NO_MOD, ST_DBG_NOC}) begin
            void'(get_next_trans_attr(trans_attrs));
            `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("%p subopcode %0d", cmd, trans_attrs.subopcode_dest))
         end
         else if (cmd inside { ATOMIC }) begin
            void'(get_next_trans_attr(trans_attrs));
            trans_attrs.subopcode_dest = evl_bus_subopcode_t'(trans_attrs.atomic_conf);
            `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("%p subopcode %0d", cmd, trans_attrs.subopcode_dest))
         end
         else begin
            trans_attrs = evl_trans_attr_s'(0);
         end
         void'(get_next_qos(qos));
         void'(get_next_vc(vc));
         void'(get_next_thread_id(thread_id));
      end

      if (cmd == BARRIER) begin
         addr_sp    = Mem;
         cache_attr = 1'b0;
         cmd_length = evl_cmd_length_t'(1);
      end
      else if (!(cmd inside { `EVL_JTAG_CMDS })) begin
         cmd        = fix_trans_attr(cmd, trans_attrs);
         addr_sp    = fix_addr_sp(cmd, addr_sp);
         cache_attr = fix_cache_attr(cmd, addr_sp, cache_attr);
         cmd_length = fix_cmd_length(cmd, addr_sp, cache_attr, cmd_length, trans_attrs, paddr);
         paddr      = fix_paddr(cmd, addr_sp, cache_attr, cmd_length, trans_attrs, paddr, agent_in);
      end

      trans_desc.set_cmd_attrs(cmd, addr_sp, cache_attr, cmd_length, paddr, req_source);
      if (cmd inside { ATOMIC }) begin
         trans_attrs.atomic_offset = evl_line_offset_t'(paddr);
      end
      trans_desc.set_bus_trans_attr(trans_attrs);
      trans_desc.set_bus_qos(qos);
      trans_desc.set_vc(vc);
      trans_desc.set_thread_id(thread_id);
      trans_desc.set_delay_wrapper(m_delay_wrapper);
      m_delay_wrapper = null;
      if (agent_in != null) begin
         agent_in.fix_stimulus_trans_desc_early(trans_desc);
      end

      if (cmd inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         bit           cline_zero = 1'b0;
         evl_dword_t   dword0     = evl_dword_t'(0);
         evl_dword_t   dword1     = evl_dword_t'(0);
         evl_dword_t   dword2     = evl_dword_t'(0);
         evl_dword_t   dword3     = evl_dword_t'(0);
         evl_dword_t   dword4     = evl_dword_t'(0);
         evl_dword_t   dword5     = evl_dword_t'(0);
         evl_dword_t   dword6     = evl_dword_t'(0);
         evl_dword_t   dword7     = evl_dword_t'(0);
         evl_line_data write_data;

         write_data = trans_desc.get_write_data(1'b1);
         write_data.invalidate_line();
         void'(get_next_cline_zero(cline_zero));

         if (cmd inside { `EVL_JTAG_CMDS_WITH_WRITE_DATA }) begin
            void'(get_next_dword(dword7));
            void'(get_next_dword(dword6));
            void'(get_next_dword(dword5));
            void'(get_next_dword(dword4));
            void'(get_next_dword(dword3));
            void'(get_next_dword(dword2));
            void'(get_next_dword(dword1));
            void'(get_next_dword(dword0));
            trans_desc.set_write_xword(0, { dword7, dword6, dword5, dword4, dword3, dword2, dword1, dword0 });
         end
         else if (cmd inside { ST_DBG_NOC }) begin
            if (cmd_length <= 1) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_byte(0, byte'(dword0));
            end
            else if (cmd_length <= 2) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_hword(0, evl_hword_t'(dword0));
            end
            else if (cmd_length <= 4) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_word(0, evl_word_t'(dword0));
            end
            else if (cmd_length <= 8) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_dword(0, dword0);
            end
            else if (cmd_length <= 16) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_qword(0, { dword1, dword0 });
            end
            else if (cmd_length <= 32) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword3));
                  void'(get_next_dword(dword2));
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_oword(0, { dword3, dword2, dword1, dword0 });
            end
            else if (cmd_length <= 64) begin
               if (cline_zero == 1'b0) begin
                  void'(get_next_dword(dword7));
                  void'(get_next_dword(dword6));
                  void'(get_next_dword(dword5));
                  void'(get_next_dword(dword4));
                  void'(get_next_dword(dword3));
                  void'(get_next_dword(dword2));
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
               end
               trans_desc.set_write_xword(0, { dword7, dword6, dword5, dword4, dword3, dword2, dword1, dword0 });
            end // if (cmd_length <= 64)
         end
         else if (cmd inside { ATOMIC }) begin
            // Atomics are 32 bytes and the data needs to be in bits 256:0 (not shifted based on address)
            if (cline_zero == 1'b0) begin
               void'(get_next_dword(dword3));
               void'(get_next_dword(dword2));
               void'(get_next_dword(dword1));
               void'(get_next_dword(dword0));
            end
            if ((trans_attrs.atomic_conf & `EVL_BUS_ATOMIC_CONF_VEC_OP) != 0) begin
               trans_desc.set_write_qword(0, { dword1, dword0 });
               trans_desc.set_write_qword(1, { dword3, dword2 });
            end
            else begin
               evl_bus_atomic_conf_t atomic_conf_op;

               atomic_conf_op = (trans_attrs.atomic_conf & `EVL_BUS_ATOMIC_OP_MASK);
               if ((trans_attrs.atomic_conf & `EVL_BUS_ATOMIC_CONF_DW_OP) != 0) begin
                  trans_desc.set_write_dword(0, dword0);
                  // atomic opcode COMPEXCH is a special operation with two operands
                  if (atomic_conf_op == SC_AMOCOMPEXCH) begin
                     trans_desc.set_write_dword(1, dword1);
                  end
               end
               else begin
                  trans_desc.set_write_word(0, evl_word_t'(dword0));
                  // atomic opcode COMPEXCH is a special operation with two operands
                  if (atomic_conf_op == SC_AMOCOMPEXCH) begin
                     trans_desc.set_write_word(2, evl_word_t'(dword1));
                  end
               end
            end
         end
         else begin
            if (cmd_length <= 1) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_byte(0, byte'(dword0));
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_byte(-1, byte'(dword0));
               end
            end
            else if (cmd_length <= 2) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_hword(0, evl_hword_t'(dword0));
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_hword(-1, evl_hword_t'(dword0));
               end
            end
            else if (cmd_length <= 4) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_word(0, evl_word_t'(dword0));
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_word(-1, evl_word_t'(dword0));
               end
            end
            else if (cmd_length <= 8) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_dword(0, dword0);
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_dword(-1, dword0);
               end
            end
            else if (cmd_length <= 16) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_qword(0, { dword1, dword0 });
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword1));
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_qword(-1, { dword1, dword0 });
               end
            end
            else if (cmd_length <= 32) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword3));
                  void'(get_next_dword(dword2));
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_oword(0, { dword3, dword2, dword1, dword0 });
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword3));
                     void'(get_next_dword(dword2));
                     void'(get_next_dword(dword1));
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_oword(-1, { dword3, dword2, dword1, dword0 });
               end
            end
            else if (cmd_length <= 64) begin
               if (cmd inside { MESSAGE }) begin
                  void'(get_next_dword(dword7));
                  void'(get_next_dword(dword6));
                  void'(get_next_dword(dword5));
                  void'(get_next_dword(dword4));
                  void'(get_next_dword(dword3));
                  void'(get_next_dword(dword2));
                  void'(get_next_dword(dword1));
                  void'(get_next_dword(dword0));
                  trans_desc.set_write_xword(0, { dword7, dword6, dword5, dword4, dword3, dword2, dword1, dword0 });
               end
               else begin
                  if (cline_zero == 1'b0) begin
                     void'(get_next_dword(dword7));
                     void'(get_next_dword(dword6));
                     void'(get_next_dword(dword5));
                     void'(get_next_dword(dword4));
                     void'(get_next_dword(dword3));
                     void'(get_next_dword(dword2));
                     void'(get_next_dword(dword1));
                     void'(get_next_dword(dword0));
                  end
                  trans_desc.set_write_xword(-1, { dword7, dword6, dword5, dword4, dword3, dword2, dword1, dword0 });
               end
            end // if (cmd_length <= 64)
         end

         if (cmd inside { ST_WA, CACHE_EVICT_TO_MEM, CACHE_FLUSH_TO_MEM } && (cmd_length == `EVL_LINE_BYTES)) begin
            evl_line_qword_en_t qword_en;

            void'(get_next_qword_en(qword_en));
            write_data.invalidate_line();
            for (int ii = 0; ii < `EVL_LINE_QWORDS; ii++) begin

               if (qword_en[0:0] == 1'b1) begin
                  evl_qword_t qword;

                  qword = trans_desc.get_write_qword(ii);
                  trans_desc.set_write_qword(ii, qword);
               end
               qword_en = qword_en >> 1;
            end
         end
      end

      if ((m_drop_on_conflict == 1'b1) && ((m_dir_trans_avail == 1'b0) || (cmd != BARRIER))) begin
         trans_desc.set_can_drop();
      end

      if (cmd inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_TEST, agent_in.get_abstract_name(), trans_desc, $sformatf("issuing %p 0x%x <- 0x%x", trans_desc.get_cmd(), trans_desc.get_paddr(), trans_desc.get_write_xword(0)))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_TEST, agent_in.get_abstract_name(), trans_desc, $sformatf("issuing %p 0x%x", trans_desc.get_cmd(), trans_desc.get_paddr()))
      end

      m_last_trans_desc = trans_desc;
      if (m_dir_trans_avail == 1'b1) begin
         m_cmd_dir        = NOCMD;
         m_dir_trans_pend = 1'b0;
         m_trans_issued   = 1'b1;
      end
      return trans_desc;
   endfunction : get_trans_desc


   //-----------------------------------------------------------------------------------------------
   virtual function void pre_randomize_me();
      return;
   endfunction : pre_randomize_me


   //-----------------------------------------------------------------------------------------------
   virtual function void randomize_me();
      pre_randomize_me();
      void'(randomize());
      post_randomize_me();
   endfunction : randomize_me


   //-----------------------------------------------------------------------------------------------
   virtual function void post_randomize_me();
      return;
   endfunction : post_randomize_me


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_mem");
      super.new(name_in);
      m_cmd_dir    = NOCMD;
      m_issue_lock = new(1);
      if (`et_value_plusargs("allowUnaligned=%d", m_allow_unaligned) == 0) begin
         m_allow_unaligned = 1;
      end
      if (`et_value_plusargs("allowETUnaligned=%d", m_allow_et_unaligned) == 0) begin
         m_allow_et_unaligned = 0;
      end
      if (`et_value_plusargs("fixupMessage=%d", m_fixup_message) == 0) begin
         m_fixup_message = 1'b1;
      end
   endfunction : new

endclass : evl_sequence_mem

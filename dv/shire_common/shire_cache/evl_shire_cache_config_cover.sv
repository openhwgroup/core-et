//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_config_cover
//
class evl_shire_cache_config_cover extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_mode
   //
   int m_sc_mode = 15;

   covergroup cg_sc_mode ();
      coverpoint m_sc_mode {
         bins m_sc_mode[] = { [0:7] };
      }
   endgroup : cg_sc_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_swizzle_mode
   //
   int m_sc_swizzle_mode = 15;

   covergroup cg_sc_swizzle_mode ();
      coverpoint m_sc_swizzle_mode {
         bins m_sc_swizzle_mode[] = { [0:2] };
      }
   endgroup : cg_sc_swizzle_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_reqq_ctl
   //
   bit       m_sc_axi_qos;
   bit       m_sc_cbuf_enable;
   bit       m_sc_remote_l3_enable;
   bit       m_sc_remote_scp_enable;
   bit       m_sc_l2_bypass;
   bit       m_sc_l3_bypass;
   bit [5:0] m_sc_num_l3_reqq_entries;
   bit       m_sc_reqq_no_link_list;
   bit       m_sc_ecc_scrub_enable;
   bit [4:0] m_sc_l3_yield_priority;

   covergroup cg_sc_reqq_ctl ();
      coverpoint m_sc_axi_qos {
         bins m_sc_axi_qos[] = { [0:1] };
      }
      coverpoint m_sc_cbuf_enable {
         bins m_sc_cbuf_enable[] = { [0:1] };
      }
      coverpoint m_sc_remote_l3_enable {
         bins m_sc_remote_l3_enable[] = { [0:1] };
      }
      coverpoint m_sc_remote_scp_enable {
         bins m_sc_remote_scp_enable[] = { [0:1] };
      }
      coverpoint m_sc_l2_bypass {
         bins m_sc_l2_bypass[] = { [0:1] };
      }
      coverpoint m_sc_l3_bypass {
         bins m_sc_l3_bypass[] = { [0:1] };
      }
      coverpoint m_sc_num_l3_reqq_entries {
         bins l3_reqq_num_0_to_3   = { [0:3] };
         bins l3_reqq_num_4_to_7   = { [4:7] };
         bins l3_reqq_num_8_to_15  = { [8:15] };
         bins l3_reqq_num_16_to_23 = { [16:23] };
         bins l3_reqq_num_24_to_39 = { [24:39] };
         bins l3_reqq_num_40_to_47 = { [40:47] };
         bins l3_reqq_num_48_to_55 = { [48:55] };
         bins l3_reqq_num_56_to_59 = { [56:59] };
         bins l3_reqq_num_60_to_63 = { [60:63] };
      }
//    coverpoint m_sc_reqq_no_link_list {
//       bins m_sc_reqq_no_link_list[] = { [0:1] };
//    }
//    coverpoint m_sc_ecc_scrub_enable {
//       bins m_sc_ecc_scrub_enable[] = { [0:1] };
//    }
      coverpoint m_sc_l3_yield_priority {
         bins l3_yield_0_to_3   = { [0:3] };
         bins l3_yield_4_to_11  = { [4:11] };
         bins l3_yield_12_to_19 = { [12:19] };
         bins l3_yield_20_to_27 = { [20:27] };
         bins l3_yield_28_to_31 = { [28:31] };
      }
   endgroup : cg_sc_reqq_ctl


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_pipe_ctl
   //
   bit       m_sc_ram_deep_sleep;
   bit       m_sc_ram_shut_down;
   bit [2:0] m_sc_ram_delay;
   bit       m_sc_l2_rbuf_enable;
   bit       m_sc_scp_rbuf_enable;
   bit       m_sc_zero_state_enable;
   bit       m_sc_allow_only_1_req_per_sub_bank;
   bit       m_sc_allow_only_1_req_per_bank;
   bit [5:0] m_sc_cbuf_entry_enable;

   covergroup cg_sc_pipe_ctl ();
//    sc_ram_delay : coverpoint m_sc_ram_deep_sleep {
//       bins m_sc_ram_deep_sleep = { [0:1] };
//    }
//    sc_l2_rbuf_enable : coverpoint m_sc_ram_shut_down {
//       bins m_sc_ram_shut_down = { [0:1] };
//    }
      sc_ram_delay : coverpoint m_sc_ram_delay {
         bins        m_sc_ram_delay[] = { [2:4] };
         ignore_bins ignore_values    = { [0:1], [5:7] };
      }
      sc_l2_rbuf_enable : coverpoint m_sc_l2_rbuf_enable {
         bins m_sc_l2_rbuf_enable[] = { [0:1] };
      }
      sc_scp_rbuf_enable : coverpoint m_sc_scp_rbuf_enable {
         bins m_sc_scp_rbuf_enable[] = { [0:1] };
      }
      sc_zero_state_enable : coverpoint m_sc_zero_state_enable {
         bins m_sc_zero_state_enable[] = { [0:1] };
      }
      sc_allow_only_1_req_per_sub_bank : coverpoint m_sc_allow_only_1_req_per_sub_bank {
         bins m_sc_allow_only_1_req_per_sub_bank[] = { [0:1] };
      }
      sc_allow_only_1_req_per_bank : coverpoint m_sc_allow_only_1_req_per_bank {
         bins m_sc_allow_only_1_req_per_bank[] = { [0:1] };
      }
      sc_cbuf_entry_enable : coverpoint m_sc_cbuf_entry_enable {
         bins        m_sc_cbuf_entry_enable[] = { [1:32] };
         ignore_bins ignore_values            = { 0, [33:63] };
      }
   endgroup : cg_sc_pipe_ctl


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_mode
   //
   function void sample_sc_mode(input int scp_size, input int l2_size, input int l3_size);
      int total_sets;

      total_sets = int'(`SC_SETS_PER_SUB_BANK);
      m_sc_mode = 15;
      if (scp_size == 0) begin
         if ((l2_size == (total_sets / 2)) && (l3_size == (total_sets / 2))) begin
            m_sc_mode = 1;
         end
         else if ((l2_size == ((total_sets * 31) / 32)) && (l3_size == (total_sets / 32))) begin
            m_sc_mode = 2;
         end
         else if ((l2_size == total_sets) && (l3_size == 0)) begin
            m_sc_mode = 3;
         end
      end
      else if ((scp_size == total_sets) && (l2_size == 0) && (l3_size == 0)) begin
         m_sc_mode = 4;
      end
      else if ((scp_size == (total_sets / 2)) && (l2_size == (total_sets / 2)) && (l3_size == 0)) begin
         m_sc_mode = 5;
      end
      else begin
         if (`SC_CACHE_SIZE_IN_MB == 1) begin
            if ((scp_size == ((total_sets * 12) / 32)) && (l2_size == (total_sets / 2)) && (l3_size == (total_sets / 8))) begin
               m_sc_mode = 0;
            end
         end
         else if (`SC_CACHE_SIZE_IN_MB == 2) begin
            if ((scp_size == (total_sets / 2)) && (l2_size == (total_sets / 4)) && (l3_size == (total_sets / 4))) begin
               m_sc_mode = 0;
            end
         end
         else if (`SC_CACHE_SIZE_IN_MB == 3) begin
            if ((scp_size == (total_sets / 2)) && (l2_size == (total_sets / 4)) && (l3_size == (total_sets / 4))) begin
               m_sc_mode = 0;
            end
         end
         else begin
            if ((scp_size == ((total_sets * 20) / 32)) && (l2_size == (total_sets / 8)) && (l3_size == (total_sets / 4))) begin
               m_sc_mode = 0;
            end
         end
      end

      //
      // The following code is for capturing random modes.  We specifically want to capture whether
      // there is a power of 2 or not:
      //
      // 6: SCP is not a power of 2, L2 is not a power of 2, and L3 is not a power of 2
      // 7: All others
      //
      if ((m_sc_mode == 15) && (scp_size > 0) && (l2_size > 0) && (l3_size > 0)) begin
         if ( (scp_size != (1 << $clog2(scp_size))) && (l2_size != (1 << $clog2(l2_size))) && (l3_size != (1 << $clog2(l3_size))) ) begin
            m_sc_mode = 6;
         end
         else begin
            m_sc_mode = 7;
         end
      end
      if (m_sc_mode != 15) begin
         cg_sc_mode.sample();
      end
   endfunction : sample_sc_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_swizzle_mode
   //
   function void sample_sc_swizzle_mode(input evl_dword_t dword_in);
      evl_esr_sc_l3_shire_swizzle_ctl_t swizzle_ctl;

      swizzle_ctl = evl_esr_sc_l3_shire_swizzle_ctl_t'(dword_in);
      m_sc_swizzle_mode = 15;
      if (swizzle_ctl.esr_sc_bank_sel_b1 != (swizzle_ctl.esr_sc_bank_sel_b0 + 1)) begin
         return;
      end
      if (swizzle_ctl.esr_sc_sub_bank_sel_b1 != (swizzle_ctl.esr_sc_sub_bank_sel_b0 + 1)) begin
         return;
      end
      if (swizzle_ctl.esr_sc_shire_sel_b4 != (swizzle_ctl.esr_sc_shire_sel_b3 + 1)) begin
         return;
      end
      if (swizzle_ctl.esr_sc_shire_sel_b3 != (swizzle_ctl.esr_sc_shire_sel_b2 + 1)) begin
         return;
      end
      if (swizzle_ctl.esr_sc_shire_sel_b2 != (swizzle_ctl.esr_sc_shire_sel_b1 + 1)) begin
         return;
      end
      if (swizzle_ctl.esr_sc_shire_sel_b1 != (swizzle_ctl.esr_sc_shire_sel_b0 + 1)) begin
         return;
      end
      if (`SC_BANKS > 4) begin
         if (swizzle_ctl.esr_sc_bank_sel_b2 != (swizzle_ctl.esr_sc_bank_sel_b1 + 1)) begin
            return;
         end
         if (`SC_SUB_BANKS > 4) begin
            if (swizzle_ctl.esr_sc_sub_bank_sel_b2 != (swizzle_ctl.esr_sc_sub_bank_sel_b1 + 1)) begin
               return;
            end
            if ((swizzle_ctl.esr_sc_bank_sel_b0 == 5) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 8)) begin
               m_sc_swizzle_mode = 0;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 8)) begin
               m_sc_swizzle_mode = 1;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 3)) begin
               m_sc_swizzle_mode = 2;
            end
         end
         else begin
            if ((swizzle_ctl.esr_sc_bank_sel_b0 == 5) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 8)) begin
               m_sc_swizzle_mode = 0;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 8)) begin
               m_sc_swizzle_mode = 1;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 3)) begin
               m_sc_swizzle_mode = 2;
            end
         end
      end
      else begin
         if (`SC_SUB_BANKS > 4) begin
            if (swizzle_ctl.esr_sc_sub_bank_sel_b2 != (swizzle_ctl.esr_sc_sub_bank_sel_b1 + 1)) begin
               return;
            end
            if ((swizzle_ctl.esr_sc_bank_sel_b0 == 5) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 7)) begin
               m_sc_swizzle_mode = 0;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 7)) begin
               m_sc_swizzle_mode = 1;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 2)) begin
               m_sc_swizzle_mode = 2;
            end
         end
         else begin
            if ((swizzle_ctl.esr_sc_bank_sel_b0 == 5) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 7)) begin
               m_sc_swizzle_mode = 0;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 7)) begin
               m_sc_swizzle_mode = 1;
            end
            else if ((swizzle_ctl.esr_sc_bank_sel_b0 == 0) && (swizzle_ctl.esr_sc_sub_bank_sel_b0 == 2)) begin
               m_sc_swizzle_mode = 2;
            end
         end
      end
      if (m_sc_swizzle_mode != 15) begin
         cg_sc_swizzle_mode.sample();
      end
   endfunction : sample_sc_swizzle_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_reqq_ctl
   //
   function void sample_sc_reqq_ctl(input evl_dword_t dword_in);
      evl_esr_sc_reqq_ctl_t reqq_ctl;

      reqq_ctl = evl_esr_sc_reqq_ctl_t'(dword_in);
      m_sc_axi_qos             = reqq_ctl.esr_sc_axi_qos[0];
      m_sc_cbuf_enable         = reqq_ctl.esr_sc_cbuf_enable;
      m_sc_remote_l3_enable    = reqq_ctl.esr_sc_remote_l3_enable;
      m_sc_remote_scp_enable   = reqq_ctl.esr_sc_remote_scp_enable;
      m_sc_l2_bypass           = reqq_ctl.esr_sc_l2_bypass;
      m_sc_l3_bypass           = reqq_ctl.esr_sc_l3_bypass;
      m_sc_num_l3_reqq_entries = reqq_ctl.esr_sc_num_l3_reqq_entries;
      m_sc_reqq_no_link_list   = reqq_ctl.esr_sc_reqq_no_link_list;
      m_sc_ecc_scrub_enable    = reqq_ctl.esr_sc_ecc_scrub_enable;
      m_sc_l3_yield_priority   = reqq_ctl.esr_sc_l3_yield_priority;
      cg_sc_reqq_ctl.sample();
   endfunction : sample_sc_reqq_ctl


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_pipe_ctl
   //
   function void sample_sc_pipe_ctl(input evl_dword_t dword_in);
      evl_esr_sc_pipe_ctl_t pipe_ctl;

      pipe_ctl = evl_esr_sc_pipe_ctl_t'(dword_in);
      m_sc_ram_deep_sleep                = pipe_ctl.esr_sc_ram_deep_sleep;
      m_sc_ram_shut_down                 = pipe_ctl.esr_sc_ram_shut_down;
      m_sc_ram_delay                     = pipe_ctl.esr_sc_ram_delay;
      m_sc_l2_rbuf_enable                = pipe_ctl.esr_sc_l2_rbuf_enable;
      m_sc_scp_rbuf_enable               = pipe_ctl.esr_sc_scp_rbuf_enable;
      m_sc_zero_state_enable             = pipe_ctl.esr_sc_zero_state_enable;
      m_sc_allow_only_1_req_per_sub_bank = pipe_ctl.esr_sc_allow_only_1_req_per_sub_bank;
      m_sc_allow_only_1_req_per_bank     = pipe_ctl.esr_sc_allow_only_1_req_per_bank;
      m_sc_cbuf_entry_enable             = $countones(pipe_ctl.esr_sc_cbuf_entry_enable);
      cg_sc_pipe_ctl.sample();
   endfunction : sample_sc_pipe_ctl


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      cg_sc_mode         = new();
      cg_sc_swizzle_mode = new();
      cg_sc_reqq_ctl     = new();
      cg_sc_pipe_ctl     = new();
   endfunction : new

endclass : evl_shire_cache_config_cover

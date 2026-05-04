//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_agent_axi
//
// This class contains all of the ports and tasks necessary to be a master
// or a slave. It also includes a cache model that can be used as a memory model
// for a slave or a cache model for a master.
//
// This class tries to be generic and not define exactly what will happen when
// it receives a transaction. Instead it makes a call to a function which returns
// the action it should take. (ex: I'm a master and I received a LD from the sequencer.
// What request should I drive on the bus, and what should I do to my cache model?)
//
// This class is used by first extending it, and in the extension
// creating only the ports that are needed (ie: for a slave or a master) and
// making function calls to fill out a table of what actions to take when.
// Please see evl_l2_master_agent_axi.sv for an example of a master agent or
// evl_mesh_slave_agent_axi.sv for an example of a slave agent.
//
virtual class evl_stim_agent_axi extends evl_verif_comp;

   localparam CONCAT_AXI_REQID_SIZE       = $bits(evl_axi_reqq_id_t) + $bits(evl_axi_bank_id_t) + $bits(evl_axi_bridge_id_t);
   localparam CONCAT_AXI_SHORT_REQID_SIZE = $bits(evl_axi_reqq_id_t) + $bits(evl_axi_bridge_id_t);

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `include "dv/common/base/evl_mem_map_pk_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Delay Keys
   //
   // pk_req_delay:          This is the delay before processing the transaction in a master
   //                        stimulus agent.
   // pk_bus_req_delay:      This is the delay before sending a bus request (processed in the
   //                        driver).
   // pk_bus_req_data_delay: This is the delay before sending a bus data (processed in the driver).
   // pk_mem_req_delay:      This is the delay before processing an incoming slave request.
   // pk_mem_bus_rsp_delay:  This is the delay before sending a bus response (processed in the
   //                        driver).
   // pk_req_on_port_zero:   This is not really a delay, but it is a way to force an AXI request
   //                        to use AXI port 0.
   //
   `evl_param_key(pk_allow_loopback)
   `evl_param_key(pk_bus_req_data_delay)
   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_forwarded_req)
   `evl_param_key(pk_from_axi_stim_agent)
   `evl_param_key(pk_l3_set_size)
   `evl_param_key(pk_loopback_region)
   `evl_param_key(pk_mem_bus_rsp_delay)
   `evl_param_key(pk_mem_req_delay)
   `evl_param_key(pk_noc_l3_miss)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_req_delay)
   `evl_param_key(pk_req_on_port_zero)
   `evl_param_key(pk_serialize_on_addr_done)
   `evl_param_key(pk_serialize_on_addr_issue)
   `evl_param_key(pk_terminate_on_rsp)
   `evl_param_key(pk_tlm_req_port)


   //-----------------------------------------------------------------------------------------------
   //
   // Member TLM Ports
   //
   uvm_analysis_port     #(evl_req_desc) m_snd_req_ports[$];
   uvm_analysis_port     #(evl_req_desc) m_snd_rsp_ports[$];
   uvm_analysis_export   #(evl_req_desc) m_obs_req_exports[$];
   uvm_analysis_export   #(evl_req_desc) m_rcv_rsp_exports[$];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_obs_req_fifos[$];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rcv_rsp_fifos[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int                     m_tables_built             = 0;
   bit                     m_count_stimulus           = 1'b0;
   int                     m_stimulus_busy            = 0;
   int                     m_active_limit             = 256;  // maximum number of active (waiting for response) descs
   int                     m_issue_limit              = 64;   // maximum number of not-yet-issued descs
   int                     m_bus_req_id_limit         = 64;
   int                     m_port_count               = 0;
   int                     m_priority_process_started = 0;
   int                     m_new_pending_req_count    = 0;
   int                     m_trans_count              = 0;
   int                     m_req_sent_count           = 0;
   int                     m_req_rcv_count            = 0;
   int                     m_data_width               = 0;
   int                     m_data_width_clog2         = 0;
   int                     m_num_l3_ports             = 1;
   int                     m_num_l3_port_mask         = 0;
   evl_cmd_length_t        m_max_cmd_length           = `EVL_LINE_BYTES;
   bit                     m_random_req_id            = 1'b1;
   bit                     m_use_source               = 1'b0;
   bit                     m_axi_mem_map_err          = 1'b0;
   bit                     m_bank_is_lsb_of_id        = 1'b0;
   bit                     m_allow_duplicate_sent_ids = 1'b0;
   evl_axi_bank_id_t       m_bank_mask                = evl_axi_bank_id_t'((1 << `SC_BANK_ID_SIZE) - 1);
   int                     m_bank_shift               = `EVL_LINE_ADDR_LSB;
   int                     m_l3_port                  = -1;
   int                     m_uc_port                  = -1;
   int                     m_sys_port                 = -1;
   int                     m_observe_descs            = 1;
   int                     m_check_l3_ways            = 0;
   semaphore               m_get_trans_desc_busy      = new(1);
   semaphore               m_rcv_req_semaphore        = new(1);
   evl_bus_req_id_t        m_bus_req_fwd_id           = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_bus_req_fwd_id_limit     = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_next_bus_req_fwd_id      = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_atomic_rsp_bus_req_mask  = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_atomic_rsp_bus_req_base  = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_atomic_rsp_bus_req_incr  = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_atomic_rsp_bus_req_limit = evl_bus_req_id_t'(0);
   int                     m_atomic_rsp_bus_req_shift = 0;
   evl_bus_cache_attr_t    m_rd_cache_attr            = evl_bus_cache_attr_t'(0);
   evl_bus_cache_attr_t    m_wr_cache_attr            = evl_bus_cache_attr_t'(0);
   evl_bus_cache_attr_t    m_rd_uc_cache_attr         = evl_bus_cache_attr_t'(0);
   evl_bus_cache_attr_t    m_wr_uc_cache_attr         = evl_bus_cache_attr_t'(0);
   int                     m_hi_priority_ports[$];
   evl_req_desc            m_hi_priority_queue[$];
   evl_req_desc            m_lo_priority_queue[$];
   evl_trans_desc          m_active_bus_reqs[$];
   evl_bus_req_id_queue    m_bus_req_id_queues[int];
   evl_stim_cache_axi      m_cache;
   evl_req_desc            m_pending_reqs[$];
   evl_line_data           m_template_cache_data;
   evl_line_data           m_template_req_data;
   evl_line_desc           m_template_line_desc;
   evl_trans_desc          m_template_trans_desc;
   evl_req_desc            m_template_req_desc;
   evl_bus_req_id_t        m_bus_req_id;
   evl_verif_comp          m_sc_agent;
   evl_line_queue          m_l3_lines[evl_paddr_t];
   byte                    m_active_bus_req_ids[] [evl_bus_req_id_t];
   evl_sequencer_mem       m_mem_sequencers[$];
   int                     m_obs_req_mb_map[$];
   mailbox #(evl_req_desc) m_obs_req_mbs[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_trans_desc_early
   //
   virtual function void fix_stimulus_trans_desc_early(input evl_trans_desc trans_desc_in);
      if (trans_desc_in.get_cmd() inside { `EVL_LD_ST_CMDS }) begin
         evl_cmd_length_t cmd_length;
         evl_paddr_t      paddr;

         cmd_length = trans_desc_in.get_cmd_length();
         paddr      = trans_desc_in.get_paddr();
         if (cmd_length > m_max_cmd_length) begin
            cmd_length = m_max_cmd_length;
            trans_desc_in.set_cmd_length(cmd_length);
         end
         if (cmd_length > 32) begin
            paddr[5:0] = {6{1'b0}};
         end
         else if (cmd_length > 16) begin
            paddr[4:0] = {5{1'b0}};
         end
         else if (cmd_length > 8) begin
            paddr[3:0] = {4{1'b0}};
         end
         else if (cmd_length > 4) begin
            paddr[2:0] = {3{1'b0}};
         end
         else if (cmd_length > 2) begin
            paddr[1:0] = {2{1'b0}};
         end
         else if (cmd_length > 1) begin
            paddr[0:0] = {1{1'b0}};
         end
         trans_desc_in.set_paddr(paddr);
      end
   endfunction : fix_stimulus_trans_desc_early


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_info
   //
   virtual function string format_req_info(input evl_trans_desc trans_desc);
      evl_cmd_t cmd;
      string    message;

      cmd     = trans_desc.get_cmd();
      message = "";
      if (cmd inside { `EVL_COP_CMDS }) begin
         if (cmd inside { CACHE_EVICT, CACHE_FLUSH }) begin
            case (trans_desc.get_bus_cop_start_level())
               1:       message = " l2";
               2:       message = " l3";
               default: message = " l1";
            endcase
            case (trans_desc.get_bus_cop_dest_level())
               1:       message = $sformatf("%s l2", message);
               2:       message = $sformatf("%s l3", message);
               default: message = $sformatf("%s mem", message);
            endcase
         end
         else if (cmd inside { CACHE_PREFETCH }) begin
            case (trans_desc.get_bus_cop_dest_level())
               1:       message = " l2";
               2:       message = " l3";
               default: message = " mem";
            endcase
         end
         else if (cmd inside { CACHE_SCP_FILL }) begin
            message = $sformatf(" 0x%x", trans_desc.get_bus_scp_fill_addr());
         end
         else if (cmd inside { CACHE_UNLOCK }) begin
            message = $sformatf(" %0d", trans_desc.get_bus_cop_final_state());
         end
         else if (cmd inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
            evl_line_data wr_data;

            wr_data = trans_desc.get_write_data();
            if (wr_data != null) begin
               int                 dword_length;
               int                 beg_dword;
               int                 end_dword;
               evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

               dword_length = (trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES;
               if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
                  byte_ens = (evl_dword_byte_en_t'(1) << `EVL_WORD_BYTES) - evl_dword_byte_en_t'(1);
                  if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                     byte_ens = byte_ens << `EVL_WORD_BYTES;
                  end
               end
               beg_dword = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
               end_dword = dword_length - 1 + beg_dword;
               message   = $sformatf("%0s <- 0x%s", message, wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
               for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
                  message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
               end
            end
         end
         return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
      end
      else if (cmd == MESSAGE) begin
         evl_line_data wr_data;

         wr_data = trans_desc.get_write_data();
         message = $sformatf(" %0d", int'(trans_desc.get_bus_message_conf()));
         if (wr_data != null) begin
            int                 dword_length;
            int                 beg_dword;
            int                 end_dword;
            evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

            dword_length = (trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES;
            if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
               byte_ens = (evl_dword_byte_en_t'(1) << `EVL_WORD_BYTES) - evl_dword_byte_en_t'(1);
               if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                  byte_ens = byte_ens << `EVL_WORD_BYTES;
               end
            end
            beg_dword = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
            end_dword = dword_length - 1 + beg_dword;
            message   = $sformatf("%0s <- 0x%s", message, wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
            for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
               message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
            end
         end
         return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
      end
      else if (cmd == ATOMIC) begin
         evl_line_data wr_data;

         wr_data = trans_desc.get_write_data();
         message = $sformatf(" conf:0x%2x", int'(trans_desc.get_bus_atomic_conf()));
         if (wr_data != null) begin
            int                 dword_length;
            int                 beg_dword;
            int                 end_dword;
            evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

            dword_length = (trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES;
            if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
               byte_ens = (evl_dword_byte_en_t'(1) << `EVL_WORD_BYTES) - evl_dword_byte_en_t'(1);
               if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                  byte_ens = byte_ens << `EVL_WORD_BYTES;
               end
            end
            beg_dword = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
            end_dword = dword_length - 1 + beg_dword;
            message   = $sformatf("%0s <- 0x%s", message, wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
            for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
               message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
            end
         end
         return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
      end
      else if ((cmd inside { `EVL_CMDS_WITH_WRITE_DATA }) && (cmd != LD_COOP)) begin
         evl_line_data wr_data;

         wr_data = trans_desc.get_write_data();
         if (wr_data != null) begin
            int                 dword_length;
            int                 beg_dword;
            int                 end_dword;
            evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0);

            dword_length = (trans_desc.get_cmd_length() + (`EVL_DWORD_BYTES - 1)) / `EVL_DWORD_BYTES;
            if (trans_desc.get_cmd_length() < evl_cmd_length_t'(`EVL_DWORD_BYTES)) begin
               byte_ens = (evl_dword_byte_en_t'(1) << `EVL_WORD_BYTES) - evl_dword_byte_en_t'(1);
               if ((trans_desc.get_paddr() & (1 << `EVL_WORD_ADDR_LSB)) != 0) begin
                  byte_ens = byte_ens << `EVL_WORD_BYTES;
               end
            end
            beg_dword = int'(trans_desc.get_paddr() & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
            end_dword = dword_length - 1 + beg_dword;
            message   = $sformatf(" <- 0x%s", wr_data.format_invalid_and_unknown_by_index(end_dword, byte_ens));
            for (int ii = (end_dword - 1); ii >= beg_dword; ii--) begin
               message = $sformatf("%s 0x%s", message, wr_data.format_invalid_and_unknown_by_index(ii));
            end
         end
         return $sformatf("%0p 0x%x%0s", cmd, trans_desc.get_paddr(), message);
      end
      else begin
         return $sformatf("%0p 0x%x", cmd, trans_desc.get_paddr());
      end
   endfunction : format_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req
   //
   virtual function void print_req(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s (%0d)", format_req_info(trans_desc), trans_desc.get_cmd_length()))
      if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_REPLAY) != 1'b0) begin
         print_replay_info(trans_desc);
      end
   endfunction : print_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req_done
   //
   virtual function void print_req_done(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s complete", format_req_info(trans_desc)))
   endfunction : print_req_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_replay_info
   //
   virtual function void print_replay_info(input evl_trans_desc trans_desc);
      string message;

      message = trans_desc.sprint_delays();
      if (trans_desc.get_cmd() inside { `EVL_CMDS_WITH_WRITE_DATA }) begin
         if (message == "") begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p set_qword_en=0x%x", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), trans_desc.get_write_qword_en()))
         end
         else begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p set_qword_en=0x%x %0s", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), trans_desc.get_write_qword_en(), message))
         end
      end
      else begin
         if (message == "") begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source()))
         end
         else begin
            `evl_log_no_time(UVM_MEDIUM, VMOD_REPLAY, $sformatf("REPLAY %0t %0s %0s set_length=%0d set_mem_space=%p set_cache_attr=%0d set_req_source=%p %0s", $time, get_abstract_name(), format_req_info(trans_desc), trans_desc.get_cmd_length(), trans_desc.get_addr_sp(), trans_desc.is_cached(), trans_desc.get_bus_req_source(), message))
         end
      end
   endfunction : print_replay_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_template
   //
   virtual function void add_addr_template(input evl_paddr_t addr_mask_in,
                                           input evl_paddr_t addr_value_in,
                                           input bit         addr_ignore,
                                           input bit         clear_first = 1'b0);
      //
      // If this is not a memory agent, do not add an address template.
      //
      if (m_memory_agent == 1'b0) begin
         return;
      end
      m_cache.add_addr_template(addr_mask_in, addr_value_in, addr_ignore, clear_first);
   endfunction : add_addr_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem(input string file_name);
      //
      // If this is not a memory agent, do not load the memory from a file.
      //
      if (m_memory_agent == 1'b0) begin
         return;
      end
      if (file_name != "") begin
         m_cache.load_mem(file_name);
      end
   endfunction : perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_active_mem_desc
   //
   virtual function void delete_active_mem_desc(input evl_mem_desc mem_desc_in);
      super.delete_active_mem_desc(mem_desc_in);
      foreach (m_active_bus_reqs[ii]) begin
         if (mem_desc_in == m_active_bus_reqs[ii]) begin
            m_active_bus_reqs.delete(ii);
            return;
         end
      end
   endfunction : delete_active_mem_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_port
   //
   virtual function uvm_analysis_port#(evl_req_desc) get_req_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0,
                                                                       input int    subport_number = 0);
      if ((port_name == "m_snd_req_port") || (port_name == "sa_snd_req") || (port_name == "snd_req")) begin
         if (port_number < m_snd_req_ports.size()) begin
            return m_snd_req_ports[port_number];
         end
      end
      else if ((port_name == "m_snd_rsp_port") || (port_name == "sa_snd_rsp") || (port_name == "snd_rsp")) begin
         if (port_number < m_snd_rsp_ports.size()) begin
            return m_snd_rsp_ports[port_number];
         end
      end
      else if (port_name == "m_snd_l3_req_port") begin
         if ((m_l3_port >= 0) && (m_l3_port < m_snd_req_ports.size())) begin
            return m_snd_req_ports[m_l3_port];
         end
      end
      else if (port_name == "m_snd_uc_req_port") begin
         if ((m_uc_port >= 0) && (m_uc_port < m_snd_req_ports.size())) begin
            return m_snd_req_ports[m_uc_port];
         end
      end
      else if (port_name == "m_snd_sys_req_port") begin
         if ((m_sys_port >= 0) && (m_sys_port < m_snd_req_ports.size())) begin
            return m_snd_req_ports[m_sys_port];
         end
      end
      else if (port_name == "m_snd_l3_rsp_port") begin
         if ((m_l3_port >= 0) && (m_l3_port < m_snd_rsp_ports.size())) begin
            return m_snd_rsp_ports[m_l3_port];
         end
      end
      else if (port_name == "m_snd_uc_rsp_port") begin
         if ((m_uc_port >= 0) && (m_uc_port < m_snd_rsp_ports.size())) begin
            return m_snd_rsp_ports[m_uc_port];
         end
      end
      else if (port_name == "m_snd_sys_rsp_port") begin
         if ((m_sys_port >= 0) && (m_sys_port < m_snd_rsp_ports.size())) begin
            return m_snd_rsp_ports[m_sys_port];
         end
      end
      return null;
   endfunction : get_req_desc_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc_export
   //
   virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name      = "unknown",
                                                                           input int    port_number    = 0,
                                                                           input int    subport_number = 0);
      if ((port_name == "m_obs_req_export") || (port_name == "obs_req")) begin
         if (port_number < m_obs_req_exports.size()) begin
            return m_obs_req_exports[port_number];
         end
      end
      else if ((port_name == "m_rcv_rsp_export") || (port_name == "rcv_rsp")) begin
         if (port_number < m_rcv_rsp_exports.size()) begin
            return m_rcv_rsp_exports[port_number];
         end
      end
      else if (port_name == "m_obs_l3_req_export") begin
         if ((m_l3_port >= 0) && (m_l3_port < m_obs_req_exports.size())) begin
            return m_obs_req_exports[m_l3_port];
         end
      end
      else if (port_name == "m_obs_uc_req_export") begin
         if ((m_uc_port >= 0) && (m_uc_port < m_obs_req_exports.size())) begin
            return m_obs_req_exports[m_uc_port];
         end
      end
      else if (port_name == "m_obs_sys_req_export") begin
         if ((m_sys_port >= 0) && (m_sys_port < m_obs_req_exports.size())) begin
            return m_obs_req_exports[m_sys_port];
         end
      end
      else if (port_name == "m_rcv_l3_rsp_export") begin
         if ((m_l3_port >= 0) && (m_l3_port < m_rcv_rsp_exports.size())) begin
            return m_rcv_rsp_exports[m_l3_port];
         end
      end
      else if (port_name == "m_rcv_uc_rsp_export") begin
         if ((m_uc_port >= 0) && (m_uc_port < m_rcv_rsp_exports.size())) begin
            return m_rcv_rsp_exports[m_uc_port];
         end
      end
      else if (port_name == "m_rcv_sys_rsp_export") begin
         if ((m_sys_port >= 0) && (m_sys_port < m_rcv_rsp_exports.size())) begin
            return m_rcv_rsp_exports[m_sys_port];
         end
      end
      return null;
   endfunction : get_req_desc_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tables
   //
   // This virtual function will be used by classes extended from this one to create
   // tables to describe the specific actions of a master or slave agent.
   //
   virtual function void build_tables();
      return;
   endfunction : build_tables


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_tag_list
   //
   virtual function void build_req_id_list();
      evl_axi_bank_id_t bank_id = evl_axi_bank_id_t'(0);

      do begin
         m_bus_req_id_queues[int'(bank_id)] = new();
         for (int ii = 0; ii < m_bus_req_id_limit; ii++) begin
            m_bus_req_id_queues[int'(bank_id)].push(evl_bus_req_id_t'(ii));
         end
         bank_id += evl_axi_bank_id_t'(1);
      end while (bank_id != evl_axi_bank_id_t'(0));
   endfunction : build_req_id_list


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop_task
   //
   virtual task fetch_and_process_loop_task(input evl_sequencer_mem mem_sequencer, input evl_bus_req_vc_t vc);
      forever begin
         wait_cycles(1);
         while (stimulus_is_enabled() == 1'b1) begin
            fetch_and_process_trans_desc(mem_sequencer, vc);
         end
         wait_for_stimulus_enable();
      end
   endtask : fetch_and_process_loop_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop
   //
   virtual function void fetch_and_process_loop(input evl_sequencer_mem mem_sequencer, input evl_bus_req_vc_t vc);
      fork
         fetch_and_process_loop_task(mem_sequencer, vc);
      join_none
   endfunction : fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Task: master_fetch_and_process_loop
   //
   virtual task master_fetch_and_process_loop();
      int sequencer_count = 0;

      wait_for_stimulus_enable();
      foreach (m_mem_sequencers[ii]) begin
         if (m_mem_sequencers[ii] != null) begin
            sequencer_count++;
         end
      end
      if (sequencer_count > 0) begin
         build_req_id_list();
         foreach (m_mem_sequencers[ii]) begin
            if (m_mem_sequencers[ii] != null) begin
               fetch_and_process_loop(m_mem_sequencers[ii], evl_bus_req_vc_t'(ii));
            end
         end
      end
      wait (sequencer_count == 0);
   endtask : master_fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   virtual function int select_req_port(input evl_req_desc req_desc, input evl_axi_source_t axi_source);
      return 0;
   endfunction : select_req_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_rsp_port
   //
   virtual function int select_rsp_port(input evl_req_desc req_desc);
      return req_desc.get_int_param(pk_tlm_req_port, 0);
   endfunction : select_rsp_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fetch_and_process_trans_desc
   //
   virtual task fetch_and_process_trans_desc(input evl_sequencer_mem mem_sequencer, input int vc_in);
      evl_trans_desc    trans_desc;
      evl_sequence_mem  sequence_mem;
      evl_req_desc      req_desc;
      evl_table_desc    table_desc;
      evl_line_desc     line_desc;
      evl_cmd_t         cmd;
      int               delay_value;
      evl_bus_req_id_t  bus_req_id;
      evl_axi_reqq_id_t reqq_id;
      evl_axi_bank_id_t bank_id;
      evl_paddr_t       bus_addr;

      req_desc = null;
      mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      if (trans_desc == null) begin
         wait_cycles(1);
         return;
      end
      cmd = trans_desc.get_cmd();
      `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("fetch_and_process got this cmd: %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
      add_active_mem_desc(trans_desc);
      if (cmd == BARRIER) begin
         int barrier_limit;

         m_stimulus_busy++;
         print_req(trans_desc);
         barrier_limit = int'(trans_desc.get_paddr());
         if (barrier_limit >= 0) begin
            barrier_limit++;
            wait (m_active_desc_count <= barrier_limit);
         end
         print_req_done(trans_desc);
         `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG fetch_and_process dropping this cmd (0): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         m_stimulus_busy--;
         wait_cycles(1);
         return;
      end
      else if ((stimulus_is_enabled() == 1'b0) && (trans_desc.can_drop_trans() == 1'b1)) begin
         `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG fetch_and_process dropping this cmd (1): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         m_stimulus_busy = 1'b0;
         return;
      end

      //
      // Wait for the request delay time before moving on.
      //
      m_stimulus_busy++;
      if (get_next_delay(delay_value, pk_req_delay, trans_desc) == 1'b1) begin
         if (delay_value > 1) begin
            wait_cycles_with_release(delay_value - 1);
         end
      end

      if (cmd == NOP) begin
         `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG fetch_and_process dropping this cmd (2): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         m_stimulus_busy--;
         wait_cycles(1);
         return;
      end

      forever begin
         bit resource_conflict = 1'b0;

         //
         // Serialize accesses between sequencers.
         //
         m_get_trans_desc_busy.get(1);

         //
         // Now perform a lookup.  This line of code does a lot.  It takes in a transaction
         // descriptor from the sequencer side of the world, looks it up in a table and based on the
         // output creates a request descriptor to describe a bus request.  It also gives back
         // table descriptor which includes what needs to be done with the cache model for this
         // transaction.
         //
         req_desc = m_cache.int_req_lookup(table_desc, line_desc, trans_desc, req_desc, AXQ_None);
         bus_addr = evl_paddr_t'(req_desc.get_paddr());

         if (bus_addr[39] == 1'b1) begin
            bank_id = evl_axi_bank_id_t'(bus_addr >> 11) & m_bank_mask;
         end
         else begin
            bank_id = evl_axi_bank_id_t'(bus_addr >> m_bank_shift) & m_bank_mask;
         end

         if (table_desc.pending_conflict() == 1'b1) begin
            `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (0): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
            resource_conflict = 1'b1;
         end
         else if (table_desc.pending_bus_cmd() == 1'b1) begin
            if (m_active_desc_count > m_active_limit) begin
               `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (1): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
               resource_conflict = 1'b1;
            end
            else if (non_issued_mem_desc_count() > m_issue_limit) begin
               `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (2): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
               resource_conflict = 1'b1;
            end
            else begin
               evl_bus_req_id_queue req_id_queue;

               req_id_queue = trans_desc.get_bus_req_id_queue();
               if ((req_id_queue == null) || (req_id_queue.size() == 0)) begin
                  req_id_queue = m_bus_req_id_queues[bank_id];
               end
               if (req_id_queue.get_next(bus_req_id, 1'b1, m_random_req_id) == 1'b0) begin
                  `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (3): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
                  resource_conflict = 1'b1;
               end
               else begin
                  bus_req_id = add_bank_to_req_id(bus_req_id, bank_id);
                  while (check_active_req_ids(bus_req_id, bank_id) == 1'b1) begin
                     if (req_id_queue.get_next(bus_req_id) == 1'b0) begin
                        `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (4): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
                        resource_conflict = 1'b1;
                        break;
                     end
                     else begin
                        bus_req_id = add_bank_to_req_id(bus_req_id, bank_id);
                     end
                  end
               end
            end
         end

         if ((resource_conflict == 1'b0) && (req_desc != null)) begin
            if (get_int_param(pk_serialize_on_addr_done, 0) > 0) begin
               evl_paddr_t addr = trans_desc.get_paddr();

               if (req_desc.has_dependency(1'b1) == 1'b1) begin
                  `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (5): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
                  resource_conflict = 1'b1;
               end
               else if (req_desc.depend_size(1'b0) == 0) begin
                  if (req_desc.bus_cmd_is_write() == 1'b1) begin
                     for (int ii = m_active_bus_reqs.size() - 1; ii >= 0; ii--) begin
                        if ( (m_active_bus_reqs[ii] != trans_desc) &&
                             (m_active_bus_reqs[ii].line_addr_match(addr) == 1'b1) &&
                             (m_active_bus_reqs[ii].is_done()             == 1'b0) ) begin
                           `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("%0s found a resource conflict against %0s (TrID 0x%x)", req_desc.sprint_obj(), m_active_bus_reqs[ii].sprint_obj(), m_active_bus_reqs[ii].get_trans_id()))
                           void'(req_desc.add_dep_desc(m_active_bus_reqs[ii]));
                           `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (6): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
                           resource_conflict = 1'b1;
                           //
                           // We should only need to track the latest write to a particular address.
                           //
                           if (m_active_bus_reqs[ii].bus_cmd_is_write() == 1'b1) begin
                              m_active_bus_reqs.delete(ii);
                           end
                        end
                     end
                  end
                  else if (req_desc.bus_cmd_is_read() == 1'b1) begin
                     for (int ii = m_active_bus_reqs.size() - 1; ii >= 0; ii--) begin
                        if ( (m_active_bus_reqs[ii] != trans_desc) &&
                             (m_active_bus_reqs[ii].bus_cmd_is_write()    == 1'b1) &&
                             (m_active_bus_reqs[ii].line_addr_match(addr) == 1'b1) &&
                             (m_active_bus_reqs[ii].is_done()             == 1'b0) ) begin
                           `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("%0s found a resource conflict against %0s (TrID 0x%x)", req_desc.sprint_obj(), m_active_bus_reqs[ii].sprint_obj(), m_active_bus_reqs[ii].get_trans_id()))
                           void'(req_desc.add_dep_desc(m_active_bus_reqs[ii]));
                           `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG resource_conflict (7): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
                           resource_conflict = 1'b1;
                           break;
                        end
                     end
                  end
               end
            end
         end

         //
         // Do we need to drop the request?
         //
         if (((stimulus_is_enabled() == 1'b0) || (resource_conflict == 1'b1)) && (trans_desc.can_drop_trans() == 1'b1)) begin
            `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG fetch_and_process dropping this cmd (3): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
            trans_desc.set_done();
            delete_active_mem_desc(trans_desc);
            m_stimulus_busy--;

            //
            // Allow another sequencer to proceed.
            //
            wait_cycles(1);
            m_get_trans_desc_busy.put(1);
            return;
         end

         if ((req_desc != null) && (get_int_param(pk_serialize_on_addr_done, 0) > 0) && (!(trans_desc inside { m_active_bus_reqs }))) begin
            m_active_bus_reqs.push_back(trans_desc);
         end

         if (resource_conflict == 1'b0) begin
            break;
         end

         //
         // Allow another sequencer to proceed and wait one cycle to see if the resource conflict
         // has been resolved.
         //
         m_get_trans_desc_busy.put(1);
         wait_cycles(1);
      end // forever begin

      print_req(trans_desc);
      if (m_count_stimulus == 1'b1) begin
         m_trans_count++;
      end

      if (req_desc == null) begin
         `evl_log_addr(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("BFLOOD_DEBUG fetch_and_process dropping this cmd (4): %0p (from %0s, id %0d)", cmd, mem_sequencer.get_abstract_name(), mem_sequencer.get_inst_id()))
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
      else begin
         // Copy write data from trans_desc to req_desc when necessary
         if (table_desc.get_data_action() inside { Data_ReqWr }) begin
            evl_line_data line_data;

            line_data = new();
            if (trans_desc.get_write_data() == null) begin
               `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: data_action is Data_ReqWr but trans_desc has null write data"))
            end
            line_data.copy_data_from_line(trans_desc.get_write_data());
            req_desc.set_req_data(line_data);
         end
         else if (table_desc.get_data_action() inside { Data_ReqWrNorm }) begin
            evl_line_data wr_data;
            evl_line_data line_data;

            line_data = new();
            wr_data   = trans_desc.get_write_data();
            if (wr_data == null) begin
               `dut_error(get_abstract_name(), $sformatf("TB_ISSUE: data_action is Data_ReqWr but trans_desc has null write data"))
            end
            else begin
               evl_line_offset_t offset     = evl_line_offset_t'(trans_desc.get_paddr());
               int               cmd_length = int'(trans_desc.get_cmd_length());

               if (cmd_length > `EVL_OWORD_ADDR_LSB) begin
                  line_data.copy_data_from_line(wr_data);
               end
               else if (cmd_length > `EVL_QWORD_ADDR_LSB) begin
                  line_data.set_oword(0, wr_data.get_oword(offset >> `EVL_OWORD_ADDR_LSB));
               end
               else if (cmd_length > `EVL_DWORD_ADDR_LSB) begin
                  line_data.set_qword(0, wr_data.get_qword(offset >> `EVL_QWORD_ADDR_LSB));
               end
               else if (cmd_length > `EVL_WORD_ADDR_LSB) begin
                  line_data.set_dword(0, wr_data.get_dword(offset >> `EVL_DWORD_ADDR_LSB));
               end
               else if (cmd_length > `EVL_HWORD_ADDR_LSB) begin
                  line_data.set_word(0, wr_data.get_word(offset >> `EVL_WORD_ADDR_LSB));
               end
               else begin
                  line_data.set_byte(0, wr_data.get_byte(offset));
               end
               req_desc.set_req_data(line_data);
            end
         end
         else if (table_desc.get_data_action() == Data_Create) begin
            evl_line_data line_data;

            line_data = m_template_req_data.clone_line_data();
            line_data.create_data(null);
            req_desc.set_req_data(line_data);
         end

         set_bus_req_id_bit(bus_req_id, bank_id);
         req_desc.set_bus_req_id(bus_req_id);

         //
         // Add in appropriate bus delays.
         //
         if (get_next_delay(delay_value, pk_bus_req_delay, trans_desc) == 1'b1) begin
            req_desc.add_delay(pk_bus_req_delay, delay_value);
         end
         if (get_next_delay(delay_value, pk_bus_req_data_delay, trans_desc) == 1'b1) begin
            req_desc.add_delay(pk_bus_req_data_delay, delay_value);
         end

         // Give the specific master agent a chance to modify the request before it's driven.
         if (trans_desc.get_vc() == ~evl_bus_req_vc_t'(0)) begin
            trans_desc.set_vc(vc_in);
         end

         if (trans_desc.is_cached() == 1'b1) begin
            if (req_desc.bus_cmd_is_read() == 1'b1) begin
               req_desc.set_bus_cache_attr(m_rd_cache_attr);
            end
            else begin
               req_desc.set_bus_cache_attr(m_wr_cache_attr);
            end
         end
         else begin
            if (req_desc.bus_cmd_is_read() == 1'b1) begin
               req_desc.set_bus_cache_attr(m_rd_uc_cache_attr);
            end
            else begin
               req_desc.set_bus_cache_attr(m_wr_uc_cache_attr);
            end
         end

         final_fixup(trans_desc, req_desc);

         if ((1 << req_desc.get_bus_req_size()) > m_data_width) begin
            int burst_len;

            burst_len = (((1 << req_desc.get_bus_req_size()) + m_data_width - 1) / m_data_width) - 1;
            req_desc.set_bus_req_len(burst_len);
            req_desc.set_bus_req_size(m_data_width_clog2);
         end
         else begin
            req_desc.set_bus_req_len(0);
         end

         req_desc.set_as_original_req();

         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("sending this request descriptor"))
         m_pending_reqs.push_back(req_desc);
         if (m_count_stimulus == 1'b1) begin
            m_req_sent_count++;
         end
         trans_desc.set_int_param(pk_from_axi_stim_agent, 1);
         if (get_next_delay(delay_value, pk_req_on_port_zero, trans_desc) == 1'b1) begin
            if (delay_value != 0) begin
               req_desc.set_int_param(pk_req_on_port_zero, 1);
            end
         end
         m_snd_req_ports[select_req_port(req_desc, AXI_NoSrc)].write(req_desc);
      end

      //
      // Now that we have processed the request, allow another sequencer to proceed.
      //
      m_stimulus_busy--;
      wait_cycles(1);
      m_get_trans_desc_busy.put(1);
   endtask : fetch_and_process_trans_desc


   //----------------------------------------------------------------------------------------------
   //
   // Function: final_fixup
   //
   // A specific agent may want to use this function to modify the request before it is driven.
   //
   virtual function void final_fixup(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      if (req_desc.get_vc() == ~evl_bus_req_vc_t'(0)) begin
         req_desc.set_vc(trans_desc.get_vc());
      end
   endfunction : final_fixup


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_responses()
   //
   task watch_for_received_responses_task(input uvm_tlm_analysis_fifo #(evl_req_desc) rcv_rsp_fifo, input int port_number);
      evl_req_desc rcv_rsp_desc;

      forever begin
         rcv_rsp_fifo.get(rcv_rsp_desc);
         process_received_response(rcv_rsp_desc);
      end
   endtask : watch_for_received_responses_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: watch_for_received_responses()
   //
   function void watch_for_received_responses(input uvm_tlm_analysis_fifo #(evl_req_desc) rcv_rsp_fifo, input int port_number);
      if (rcv_rsp_fifo != null) begin
         fork
            watch_for_received_responses_task(rcv_rsp_fifo, port_number);
         join_none
      end
   endfunction : watch_for_received_responses


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_req_priorities()
   //
   task process_req_priorities(input int port_number);
      if (m_priority_process_started++ > 0) begin
         m_priority_process_started = 1;
         return;
      end
      forever begin
         #0 wait (m_new_pending_req_count > 0);
         #0 m_new_pending_req_count--;
         if (m_hi_priority_queue.size() > 0) begin
            process_received_request(m_hi_priority_queue.pop_front(), port_number);
         end
         else if (m_lo_priority_queue.size() > 0) begin
            process_received_request(m_lo_priority_queue.pop_front(), port_number);
         end
      end
   endtask : process_req_priorities


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_obs_req_fifo_task()
   //
   task process_obs_req_fifo_task(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo,
                                  input int                                   port_number,
                                  input mailbox #(evl_req_desc)               obs_req_mb,
                                  input bit                                   start_mb);
      evl_req_desc obs_req_desc;
      evl_req_desc rcv_req_desc;

      if (start_mb == 1'b1) begin
         fork
            forever begin
               obs_req_mb.get(rcv_req_desc);
               process_received_request(rcv_req_desc, port_number);
            end
         join_none
      end
      forever begin
         obs_req_fifo.get(obs_req_desc);
         obs_req_desc.set_int_param(pk_tlm_req_port, port_number);
         obs_req_mb.put(obs_req_desc);
      end
   endtask : process_obs_req_fifo_task


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_obs_req_fifo()
   //
   function void process_obs_req_fifo(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo,
                                      input int                                   port_number,
                                      input mailbox #(evl_req_desc)               obs_req_mb,
                                      input bit                                   start_mb);
      fork
         process_obs_req_fifo_task(obs_req_fifo, port_number, obs_req_mb, start_mb);
      join_none
   endfunction : process_obs_req_fifo


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_obs_req_fifo()
   //
   function void start_obs_req_fifo(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo,
                                    input int                                   port_number,
                                    input int                                   mb_number);
      if (!m_obs_req_mbs.exists(mb_number)) begin
         m_obs_req_mbs[mb_number] = new();
         process_obs_req_fifo(obs_req_fifo, port_number, m_obs_req_mbs[mb_number], 1'b1);
      end
      else begin
         process_obs_req_fifo(obs_req_fifo, port_number, m_obs_req_mbs[mb_number], 1'b0);
      end
   endfunction : start_obs_req_fifo


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_requests_task()
   //
   task watch_for_received_requests_task(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo, input int port_number, input bit hi_priority = 1'b0);
      evl_req_desc rcv_req_desc;

      if (m_obs_req_fifos.size() > 1) begin
         if (obs_req_fifo != null) begin
            fork
               process_req_priorities(port_number);
            join_none
            forever begin
               obs_req_fifo.get(rcv_req_desc);
               rcv_req_desc.set_int_param(pk_tlm_req_port, port_number);
               if (hi_priority == 1'b1) begin
                  m_hi_priority_queue.push_back(rcv_req_desc);
               end
               else begin
                  m_lo_priority_queue.push_back(rcv_req_desc);
               end
               m_new_pending_req_count++;
            end
         end
      end
      else begin
         if (obs_req_fifo != null) begin
            forever begin
               obs_req_fifo.get(rcv_req_desc);
               rcv_req_desc.set_int_param(pk_tlm_req_port, port_number);
               process_received_request(rcv_req_desc, port_number);
            end
         end
      end
   endtask : watch_for_received_requests_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_requests()
   //
   function void watch_for_received_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) obs_req_fifo, input int port_number, input bit hi_priority = 1'b0);
      if (obs_req_fifo != null) begin
         //
         // If m_obs_req_mb_map.size() is greater than 0, then this particular stimulus agent has
         // multiple incoming observed request ports that should (potentially) be processed in
         // parallel.  The start_obs_req_fifo() function call will create a new request descriptor
         // mailbox (if necessary) based on the m_obs_req_mb_map[] value associated with this
         // observed request port.  Thus, we can have multiple observed request ports wick down to a
         // single mailbox, or we can have them go to different mailboxes.
         //
         if (m_obs_req_mb_map.size() > 0) begin
            if ((port_number >= 0) && (port_number < m_obs_req_mb_map.size())) begin
               start_obs_req_fifo(obs_req_fifo, port_number, m_obs_req_mb_map[port_number]);
            end
         end
         else begin
            fork
               watch_for_received_requests_task(obs_req_fifo, port_number, hi_priority);
            join_none
         end
      end
   endfunction : watch_for_received_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_received_response()
   //
   // This function will get called for Master Agents when a response is
   // observed on the bus. The evl_req_desc sent is actually the same
   // one that was originally sent on sa_snd_req_port.
   //
   function void process_received_response(input evl_req_desc rcv_rsp_desc);
      evl_table_desc    table_desc;
      evl_req_desc      req_desc;
      evl_line_desc     line_desc;
      evl_trans_desc    trans_desc;
      evl_axi_bank_id_t bank_id;
      evl_paddr_t       bus_addr;
      evl_data_action_t data_action;

      trans_desc = rcv_rsp_desc.get_root_trans_desc();
      `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), rcv_rsp_desc, $sformatf("response received for %0s", rcv_rsp_desc.sprint_obj()))

      foreach (m_pending_reqs[ii]) begin
         if (rcv_rsp_desc == m_pending_reqs[ii]) begin
            m_pending_reqs.delete(ii);
            break;
         end
      end
      req_desc = rcv_rsp_desc;
      if (is_active_master() == 1'b1) begin
         if (req_desc.get_int_param(pk_forwarded_req, 0) == 0) begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, req_desc, AXQ_Local);
         end
         else begin
            m_cache.bus_rsp_lookup(table_desc, line_desc, req_desc, AXQ_Remote);
         end
      end
      else begin
         //
         // This is the loopback response case ... we have received a response from an AXI master
         // agent that has been linked to this agent.
         //
         m_cache.bus_rsp_lookup(table_desc, line_desc, rcv_rsp_desc, AXQ_Remote);
      end

      req_desc.set_done();
      data_action = table_desc.get_data_action();
      if (data_action inside { Data_ReqRd }) begin
         evl_line_data line_data;

         line_data = new();
         if (req_desc.get_rsp_data() == null) begin
            `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_response not expecting response data to be null"))
         end
         else begin
            line_data.copy_data_from_line(req_desc.get_rsp_data());
         end
         if (trans_desc != null) begin
            trans_desc.set_read_data(line_data);
         end
      end
      else if ((data_action inside { Data_FwdRd, Data_FwdWr }) && (req_desc.get_int_param(pk_terminate_on_rsp, 0) == 0)) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), rcv_rsp_desc, $sformatf("loopback response for %0s", rcv_rsp_desc.sprint_obj()))
         //
         // This is the loopback response case ... we have received a response from an AXI master
         // agent that has been linked to this agent.
         //
         req_desc = rcv_rsp_desc.get_parent_req_desc();
         req_desc.replicate_rsp_info(rcv_rsp_desc);
         req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
         req_desc.set_bus_rsp_size(`EVL_LINE_ADDR_LSB);
         req_desc.set_bus_rsp_type(rcv_rsp_desc.get_bus_rsp_type());
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
         req_desc.set_done();
         m_snd_rsp_ports[select_rsp_port(req_desc)].write(req_desc);
         return;
      end
      // Mark the transaction as done.
      if (trans_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_received_response not expecting trans_desc to be null"))
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), req_desc, $sformatf("process_received_response marking this transaction done"))
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end

      bus_addr = evl_paddr_t'(req_desc.get_paddr());

      if (bus_addr[39] == 1'b1) begin
         bank_id = evl_axi_bank_id_t'(bus_addr >> 11) & m_bank_mask;
      end
      else begin
         bank_id = evl_axi_bank_id_t'(bus_addr >> m_bank_shift) & m_bank_mask;
      end
      unset_bus_req_id_bit(req_desc.get_bus_req_id(), bank_id);
   endfunction : process_received_response


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function evl_axi_source_t get_axi_source(input int req_port);
      return AXI_NoSrc;
   endfunction : get_axi_source


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function bit address_is_mine(input evl_paddr_t addr_in, input evl_axi_source_t axi_source, input evl_etlink_req_opcode_t opcode_in);
      return 1'b1;
   endfunction : address_is_mine


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function bit check_active_req_ids(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         return (m_active_bus_req_ids[0].exists(bus_req_id) && (&m_active_bus_req_ids[0][bus_req_id] == 1'b1));
      end
      else begin
         return (m_active_bus_req_ids[0].exists(bus_req_id) && (m_active_bus_req_ids[0][bus_req_id][0] == 1'b1));
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function evl_bus_req_id_t add_bank_to_req_id(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      evl_axi_reqq_id_t   reqq_id;
      evl_axi_bridge_id_t bridge_id;

      { bridge_id, reqq_id } = bus_req_id[CONCAT_AXI_SHORT_REQID_SIZE - 1:0];
      if (m_bank_is_lsb_of_id == 1'b0) begin
         return evl_bus_req_id_t'({ bridge_id, bank_id, reqq_id });
      end
      return evl_bus_req_id_t'({ bridge_id, reqq_id, bank_id });
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function void set_bus_req_id_bit(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         m_active_bus_req_ids[0][bus_req_id] = {m_active_bus_req_ids[0][bus_req_id][6:0], 1'b1};
      end
      else begin
         m_active_bus_req_ids[0][bus_req_id][0] = 1'b1;
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function void unset_bus_req_id_bit(input evl_bus_req_id_t bus_req_id, input evl_axi_bank_id_t bank_id);
      if (m_allow_duplicate_sent_ids == 1'b1) begin
         m_active_bus_req_ids[0][bus_req_id] = {1'b0, m_active_bus_req_ids[0][bus_req_id][7:1]};
      end
      else begin
         m_active_bus_req_ids[0][bus_req_id][0] = 1'b0;
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   virtual function evl_axi_qual_t preprocess_qualifier(input evl_req_desc req_desc, input evl_axi_qual_t axi_qual_in, input evl_axi_source_t axi_source);
      evl_trans_desc trans_desc;

      trans_desc = req_desc.get_root_trans_desc();
      if ((trans_desc != null) && (trans_desc.get_int_param(pk_from_axi_stim_agent, 0) > 0)) begin
         return axi_qual_in;
      end
      if (req_desc.get_bus_qos() inside { `AXI_QOS_ATOMIC_RSP, `AXI_QOS_MESSAGE }) begin
         if ((req_desc.get_paddr() & `EVL_ATRSP_ADDR_CHECK_MASK) == `EVL_ATRSP_ADDR_CHECK_VALUE) begin
            evl_shire_id_t shire_id = evl_shire_id_t'(req_desc.get_paddr() >> `EVL_ATRSP_SHIRE_ID_LSB);

            if (shire_id <= `EVL_ATRSP_MAX_SHIRE_ID) begin
               return AXQ_AtomicComplete;
            end
         end
         if (evl_etlink_req_opcode_t'(req_desc.get_bus_req_user()) == ET_LINK_REQ_MsgSendData) begin
            if (axi_qual_in == AXQ_Local) begin
               return AXQ_MsgLocal;
            end
            else if (axi_qual_in == AXQ_Remote) begin
               return AXQ_MsgRemote;
            end
         end
      end
      else if (evl_etlink_req_opcode_t'(req_desc.get_bus_req_user()) == ET_LINK_REQ_Atomic) begin
         if (axi_qual_in == AXQ_Local) begin
            return AXQ_AtomicLocal;
         end
         else if (axi_qual_in == AXQ_Remote) begin
            return AXQ_AtomicRemote;
         end
      end
      return axi_qual_in;
   endfunction : preprocess_qualifier


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_process_received_request()
   //
   // This task will get called from process_received_request() in order to post-process the request
   // descriptor (e.g. bus_error).
   //
   virtual function void post_process_received_request(input evl_req_desc req_desc);
      if (m_axi_mem_map_err == 1'b1) begin
         bit         is_bus_err = 1'b0;
         evl_paddr_t paddr      = req_desc.get_paddr();

         if (addr_in_region(pk_scp_region, paddr)) begin
            if (paddr[22:0] >= 23'h280000) begin
               is_bus_err = 1'b1;
            end
            if ((paddr[30:23] > 34) && (paddr[30:23] != 254)) begin
               is_bus_err = 1'b1;
            end
         end
         else if (addr_in_region(pk_esr_region, paddr)) begin
            // PRM-19: 2.4 The ESR Region (4G to 8G)
            if ((paddr[29:22] >= 0) && (paddr[29:22] <= 34)) is_bus_err = 1'b0;
            else if ((paddr[29:22] >= 232) && (paddr[29:22] <= 239)) is_bus_err = 1'b0;
            else if  (paddr[29:22] == 253) is_bus_err = 1'b0;
            else if  (paddr[29:22] == 254) is_bus_err = 1'b0;
            else if  (paddr[29:22] == 255) is_bus_err = 1'b0;
            else is_bus_err = 1'b1;
         end
         else if (addr_in_region(pk_mram_region, paddr)) begin
            is_bus_err = (paddr >= 64'h08800000000) ? 1 : 0;
         end
         else if (addr_in_regions({pk_io_region, pk_reserved_region}, paddr)) begin
            is_bus_err = 1'b1;
         end

         if (is_bus_err == 1'b1) begin
            req_desc.set_abstract_rsp_type(RspErr);
            req_desc.set_bus_rsp_qword_en(4'b0000);
         end
      end
   endfunction : post_process_received_request


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_received_request()
   //
   // This task will get called for agents when an incoming request is observed on the bus.
   //
   virtual task process_received_request(input evl_req_desc rcv_req_desc, input int port_num);
      evl_trans_desc          trans_desc;
      evl_table_desc          table_desc;
      evl_line_desc           line_desc;
      evl_line_data           line_data;
      evl_req_desc            req_desc;
      evl_axi_qual_t          axi_qual;
      evl_axi_source_t        axi_source;
      evl_cache_st_t          tag_action;
      evl_data_action_t       data_action;
      evl_abstract_bus_cmd_t  bus_cmd;
      evl_abstract_rsp_type_t rsp_type;
      int                     last_line;
      int                     delay_value;

      line_data = new();
      if (m_count_stimulus == 1'b1) begin
         m_req_rcv_count++;
      end

      // First, wait for the time specified by mem_req_delay.
      if (get_next_delay(delay_value, pk_mem_req_delay, null) == 1'b1) begin
         if (delay_value > 1) begin
            wait_cycles_with_release(delay_value - 1);
         end
      end

      // If the request is a write and the write data arrived during the mem_req_delay, great!
      // If it is a write and the write data has not arrived yet, just wait longer!
      // If it's not a write there's no need for more waiting, advance to GO and collect $200!
      if ((rcv_req_desc.bus_cmd_is_write() == 1'b1) && (rcv_req_desc.have_all_req_data() == 1'b0)) begin
         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), rcv_req_desc, $sformatf("waiting for request data (%0s)", rcv_req_desc.sprint_obj()))
         rcv_req_desc.wait_for_all_data();
      end

      #0;
      m_rcv_req_semaphore.get(1);
      `evl_log_mdesc(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), rcv_req_desc, $sformatf("processing %0s", rcv_req_desc.sprint_obj()))
      trans_desc = rcv_req_desc.get_root_trans_desc();

      axi_source = get_axi_source(port_num);
      if ((is_active_master() == 1'b0) && (address_is_mine(rcv_req_desc.get_paddr(), axi_source, evl_etlink_req_opcode_t'(rcv_req_desc.get_bus_req_user())) == 1'b1)) begin
         axi_qual = preprocess_qualifier(rcv_req_desc, AXQ_Local, axi_source);
      end
      else begin
         axi_qual = preprocess_qualifier(rcv_req_desc, AXQ_Remote, axi_source);
      end

      last_line = rcv_req_desc.get_max_line_burst_idx();

      m_cache.bus_req_lookup(table_desc, line_desc, rcv_req_desc, axi_qual, axi_source);
      tag_action  = table_desc.get_tag_action();
      data_action = table_desc.get_data_action();
      rsp_type    = evl_abstract_rsp_type_t'(table_desc.get_bus_rsp());
      bus_cmd     = evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd());
      if (m_observe_descs == 1'b1) begin
         rcv_req_desc.set_observed();
         rcv_req_desc.set_parent_processed();
      end
      if ((m_check_l3_ways > 0) && (m_check_l3_ways <= 4) && (m_sc_agent != null)) begin
         if (tag_action inside { St_SysChkI, St_SysChkE, St_SysChkM }) begin
            `dut_error_mdesc(get_abstract_name(), rcv_req_desc, $sformatf("%0s will cause a memory access", rcv_req_desc.sprint_obj()))
            case (tag_action)
               St_SysChkI: line_desc.set_cache_st(I);
               St_SysChkE: line_desc.set_cache_st(E);
               St_SysChkM: line_desc.set_cache_st(M);
            endcase
         end
         else if (tag_action != St_Ignore) begin
            check_for_sys_bus(tag_action, line_desc, rcv_req_desc);
         end
      end
      if (rsp_type != RspNull) begin
         evl_paddr_t   cur_addr;
         evl_line_desc cur_line_desc;

         cur_addr = rcv_req_desc.get_paddr();
         if (data_action == Data_MemRd) begin
            for (int line_idx = 0; line_idx <= last_line; line_idx++) begin
               cur_line_desc = m_cache.get_line_desc(cur_addr, 1);
               rcv_req_desc.set_rsp_data(cur_line_desc.get_line_data_copy(), line_idx);
               cur_addr += evl_paddr_t'(`EVL_LINE_BYTES);
            end
         end
         else if (data_action == Data_MemWr) begin
            for (int line_idx = 0; line_idx <= last_line; line_idx++) begin
               cur_line_desc = m_cache.get_line_desc(cur_addr, 1);
               line_data = rcv_req_desc.get_req_data(line_idx);
               cur_line_desc.set_xword(0, line_data.get_xword(0), line_data.get_xword_valids(0));
               cur_addr += evl_paddr_t'(`EVL_LINE_BYTES);
            end
         end

         rcv_req_desc.set_bus_rsp_id(rcv_req_desc.get_bus_req_id());
         rcv_req_desc.set_bus_rsp_size(`EVL_LINE_ADDR_LSB);
         rcv_req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));

         //
         // Add in appropriate bus delays to send out.
         //
         if (get_next_delay(delay_value, pk_mem_bus_rsp_delay, trans_desc) == 1'b1) begin
            rcv_req_desc.add_delay(pk_mem_bus_rsp_delay, delay_value);
         end
         post_process_received_request(rcv_req_desc);
         rcv_req_desc.set_bus_rsp_valid(0, get_cycle_count());
         rcv_req_desc.set_done();
         m_snd_rsp_ports[select_rsp_port(rcv_req_desc)].write(rcv_req_desc);
      end
      if (bus_cmd != BusIdle) begin
         evl_req_desc snd_req_desc;

         $cast(snd_req_desc, rcv_req_desc.clone());
         void'(rcv_req_desc.add_child_desc(snd_req_desc));
         snd_req_desc.replicate_req_info(rcv_req_desc);
         if (m_bus_req_fwd_id == evl_bus_req_id_t'(0)) begin
            snd_req_desc.set_bus_req_id(m_bus_req_fwd_id | rcv_req_desc.get_bus_req_id());
         end
         else begin
            if ((m_next_bus_req_fwd_id == evl_bus_req_id_t'(0)) || (m_next_bus_req_fwd_id > m_bus_req_fwd_id_limit)) begin
               m_next_bus_req_fwd_id = m_bus_req_fwd_id;
            end
            snd_req_desc.set_bus_req_id(m_next_bus_req_fwd_id);
            m_next_bus_req_fwd_id++;
         end
         snd_req_desc.set_abstract_bus_cmd(bus_cmd);
         snd_req_desc.set_int_param(pk_forwarded_req, 1);
         if (rsp_type != RspNull) begin
            snd_req_desc.set_int_param(pk_terminate_on_rsp, 1);
         end
         if (bus_cmd == BusWriteAtomic) begin
            evl_line_data atomic_response;
            evl_qword_t   axi_info_qword;
            evl_paddr_t   axi_lower_addr;
            evl_paddr_t   axi_shire_id;
            evl_qword_t   axi_wuser_id;

            axi_info_qword  = rcv_req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB + 1));
            axi_lower_addr  = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_ADDR_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_ADDR_MASK);
            axi_shire_id    = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_SOURCE_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_SOURCE_MASK);
            axi_wuser_id    = (axi_info_qword >> `EVL_AXI_ATOMIC_TRANSID_SHIFT) & evl_qword_t'(`EVL_AXI_ATOMIC_TRANSID_MASK);
            rcv_req_desc.set_bus_atomic_conf(axi_info_qword[`EVL_BUS_ATOMIC_CONF_WIDTH - 2:0]);
            rcv_req_desc.set_bus_atomic_offset(axi_info_qword >> (`EVL_BUS_ATOMIC_CONF_WIDTH - 1));
            atomic_response = line_desc.process_atomic(rcv_req_desc, 1'b0);
            snd_req_desc.set_paddr(`SC_L3_ATOMIC_RSP_ADDR_OFFSET | evl_paddr_t'(axi_lower_addr) | (evl_paddr_t'(axi_shire_id) << `SC_L3_ATOMIC_RSP_SHIRE_ID_SHIFT));
            snd_req_desc.set_req_data(atomic_response);
            if ((rcv_req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_VEC_OP) == 0) begin
               if ((rcv_req_desc.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_DW_OP) == 0) begin
                  snd_req_desc.set_bus_req_size(`EVL_WORD_ADDR_LSB);
                  atomic_response.invalidate_line();
                  snd_req_desc.set_req_word(-1, snd_req_desc.get_req_word(-1));
               end
               else begin
                  snd_req_desc.set_bus_req_size(`EVL_DWORD_ADDR_LSB);
                  atomic_response.invalidate_line();
                  snd_req_desc.set_req_dword(-1, snd_req_desc.get_req_dword(-1));
               end
            end
            else begin
               snd_req_desc.set_bus_req_size(`EVL_OWORD_ADDR_LSB);
               atomic_response.invalidate_line();
               snd_req_desc.set_req_oword(-1, snd_req_desc.get_req_oword(-1));
            end
            snd_req_desc.set_bus_req_user(axi_wuser_id);
            snd_req_desc.set_bus_qos(2);
            if (m_atomic_rsp_bus_req_mask != evl_bus_req_id_t'(0)) begin
               snd_req_desc.set_bus_req_id((rcv_req_desc.get_bus_req_id() & m_atomic_rsp_bus_req_mask) | m_atomic_rsp_bus_req_base + (m_atomic_rsp_bus_req_incr << m_atomic_rsp_bus_req_shift));
               m_atomic_rsp_bus_req_incr++;
               if (m_atomic_rsp_bus_req_incr > m_atomic_rsp_bus_req_limit) begin
                  m_atomic_rsp_bus_req_incr = evl_bus_req_id_t'(0);
               end
            end
         end
         post_process_loopback_req(trans_desc, snd_req_desc);
         if (m_count_stimulus == 1'b1) begin
            m_req_sent_count++;
         end
         m_pending_reqs.push_back(snd_req_desc);
         if (snd_req_desc.bus_cmd_is_write() == 1'b1) begin
            snd_req_desc.set_have_all_data();
         end
         if (m_observe_descs == 1'b1) begin
            snd_req_desc.set_issued();
         end
         m_snd_req_ports[select_req_port(snd_req_desc, axi_source)].write(snd_req_desc);
      end
      m_rcv_req_semaphore.put(1);
   endtask : process_received_request


   //----------------------------------------------------------------------------------------------
   //
   // Function: post_process_loopback_req
   //
   // A specific agent may want to use this function to modify the request before it is driven.
   //
   virtual function void post_process_loopback_req(input evl_trans_desc trans_desc, input evl_req_desc req_desc);
      return;
   endfunction : post_process_loopback_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_pending_request
   //
   function void find_pending_request(output evl_req_desc     first_req_desc,
                                      output evl_req_desc     match_req_desc,
                                      input  evl_bus_req_id_t req_id,
                                      input  evl_bus_cmd_t    bus_cmd,
                                      input  evl_paddr_t      address);
      evl_req_desc req_desc;

      first_req_desc = null;
      match_req_desc = null;
      foreach (m_pending_reqs[ii]) begin
         req_desc = m_pending_reqs[ii];
         if (req_desc.get_bus_req_id() == req_id) begin
            if (first_req_desc == null) begin
               first_req_desc = req_desc;
            end
            if ((req_desc.get_bus_cmd() == bus_cmd) && (req_desc.get_paddr() == address)) begin
               match_req_desc = req_desc;
               m_pending_reqs.delete(ii);
               return;
            end
            else begin
               m_pending_reqs.delete(ii);
            end
         end
      end
      return;
   endfunction : find_pending_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      evl_req_desc     first_req_desc;
      evl_req_desc     match_req_desc;
      evl_bus_cmd_t    bus_cmd;
      evl_paddr_t      address;
      evl_bus_req_id_t req_id;

      if (paired_req_desc == null) begin
         bus_cmd = bus_cmd_in;
         req_id  = req_id_in;
         address = address_in;
      end
      else begin
         bus_cmd = (bus_cmd_in != `EVL_Bus_Idle)         ? bus_cmd_in : paired_req_desc.get_bus_cmd();
         req_id  = (req_id_in  != ~evl_bus_req_id_t'(0)) ? req_id_in  : paired_req_desc.get_bus_req_id();
         address = (address_in != ~evl_paddr_t'(0))      ? address_in : paired_req_desc.get_paddr();
      end
      find_pending_request(first_req_desc, match_req_desc, req_id, bus_cmd, address);
      if (match_req_desc != null) begin
         if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc)) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_int_req_act
   //
   // This function is used to define a table of actions to take when a transaction descriptor
   // is received from the stimulus world. The first 4 inputs to this function are table inputs
   // and the last 3 are outputs of the table.
   // Lists are used as a way to collapse the table to save typing (and sanity) but you can
   // think of the inputs in terms of a single entry:
   // cmd: The command from the stimulus (ex: LD, ST). These are generic and bus agnostic.
   // cache_attr: Is it cached or uncached?
   // model_st: What is the current state of this agent's cache model? (Invalid, Exclusive, Modified)
   // qualifier: This is an optional extra input that you can use for whatever you want.
   // tag_action: How should we modify the cache state of our model based on the transaction?
   // data_action: What should we do with the data based on the transaction?
   // bus_cmd: What (if any) command should we drive on the bus?
   //
   function void add_int_req_act(input evl_cmd_t              cmd_in,
                                 input bit                    cache_attr_in,
                                 input evl_cache_st_t         model_st_in,
                                 input evl_axi_qual_t         qualifier_in,
                                 input evl_cache_st_t         tag_action_in,
                                 input evl_data_action_t      data_action_in,
                                 input evl_abstract_bus_cmd_t bus_cmd_in);
      m_cache.add_int_req_act(cmd_in, cache_attr_in, model_st_in, qualifier_in, tag_action_in, data_action_in, bus_cmd_in);
   endfunction : add_int_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act
   //
   // This function is used to define a table of actions to take when a request is observed on the
   // bus.  The first 3 inputs will be inputs to the table and the last 4 inputs will be the outputs
   // of the table.  Lists are used as a way to collapse the table to save typing but you can think
   // of the inputs in terms of a single entry:
   //
   //   bus_cmd_in:     The command observed on the bus
   //   model_st_in:    The current state of the agent's cache model
   //   qualifier_in:   This is an optional extra input
   //   tag_action_in:  How should we modifiy the cache state of our model based on the request?
   //   data_action_in: What should we do with the data based on the request?
   //   bus_rsp_in:     What (if any) response we should drive on the bus?
   //   new_bus_cmd:    What (if any) command we should drive on the bus?
   //   err_code_in:    What (if any) error should be logged
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
      m_cache.add_bus_req_act(bus_cmd_in, model_st_in, qualifier_in, source_in, tag_action_in, data_action_in, bus_rsp_in, new_bus_cmd_in, err_code_in);
   endfunction : add_bus_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   // This function is used to define a table of actions to take when a response is observed on the
   // bus.  The first 3 inputs will be inputs to the table and the last 2 inputs will be outputs of
   // the table.  Lists are used as a way to collapse the table to save typing but you can think of
   // the inputs in terms of a single entry:
   //
   //   bus_cmd_in:     The (response) command observed on the bus
   //   model_st_in:    The current state of the agent's cache model
   //   qualifier_in:   This is an option extra input
   //   tag_action_in:  How should we modify the cache state of our model based on the response?
   //   data_action_in: What should we do with the data based on the response?
   //   bus_rsp_in:     What (if any) response we should drive on the bus?
   //
   function void add_bus_rsp_act(input evl_bus_cmd_t           bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_qual_t          qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in);
      m_cache.add_bus_rsp_act(bus_cmd_in, model_st_in, qualifier_in, tag_action_in, data_action_in, bus_rsp_in);
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_for_sys_bus
   //
   // This function is used to check for system bus activity.  It is intended to be used in a single
   // shire build in which the L3 and system AXI interfaces are connected to a slave agent.  The
   // reason for using this is to flag any requests that will potentially lead to system bus
   // activity, allowing us to determine which post-silicon bring-up tests require memory shire
   // access.
   //
   virtual function void check_for_sys_bus(input evl_cache_st_t tag_action, input evl_line_desc line_desc, input evl_req_desc req_desc);
      int         l3_way_count;
      evl_paddr_t addr = req_desc.get_paddr();

      if ( (m_check_l3_ways < 1) || (m_check_l3_ways > 4) || (m_sc_agent == null) ||
           (int'(addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) ||
           ((addr >= evl_paddr_t'(40'h01_0000_0000)) && (addr <= evl_paddr_t'(40'h01_ffff_ffff))) ) begin
         return;
      end
      if (tag_action == St_L3ChkI) begin
         line_desc.set_cache_st(I);
         return;
      end
      else if (tag_action == St_L3ChkE) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("%0s will cause a memory access", req_desc.sprint_obj()))
         line_desc.set_cache_st(E);
      end
      else if (tag_action == St_L3ChkEM) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("%0s will cause a memory access", req_desc.sprint_obj()))
         line_desc.set_cache_st(M);
      end
      else if (tag_action == St_L3ChkM) begin
         line_desc.set_cache_st(M);
      end
      else if (tag_action != St_Ignore) begin
         line_desc.set_cache_st(tag_action);
         return;
      end
      addr &= (((evl_paddr_t'(m_sc_agent.get_int_param(pk_l3_set_size)) << (`SC_L3_SHIRE_ID_SIZE)) - 1) & ~(evl_paddr_t'(`EVL_LINE_BYTES - 1)));
      if (!m_l3_lines.exists(addr)) begin
         m_l3_lines[addr] = new();
      end
      l3_way_count = m_l3_lines[addr].push_desc(line_desc);
      if (l3_way_count > m_check_l3_ways) begin
         while (m_l3_lines[addr].size() > 1) begin
            evl_line_desc evict_line;

            evict_line = m_l3_lines[addr].pop_desc();
            if (evict_line.get_cache_st() == M) begin
               `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("%0s may cause 0x%x to be evicted", req_desc.sprint_obj(), evict_line.get_paddr()))
            end
         end
      end
   endfunction : check_for_sys_bus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_my_bus_ports
   //
   // This function is used to connect all of the ports that go between this stimulus agent and its
   // bus agent.
   //
   virtual function void connect_my_bus_ports(input evl_verif_comp bus_verif_comp,
                                              input string         port_name      = "",
                                              input int            port_number    = 0,
                                              input int            subport_number = 0);
      uvm_analysis_port   #(evl_req_desc) bus_port;
      uvm_analysis_export #(evl_req_desc) bus_export;

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_rsp", port_number);
      if ((port_number < m_snd_rsp_ports.size()) && (m_snd_rsp_ports[port_number] != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_rsp_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_rsp_ports[port_number].connect(bus_export);
      end

      bus_export = bus_verif_comp.get_req_desc_export("sa_snd_req", port_number);
      if ((port_number < m_snd_req_ports.size()) && (m_snd_req_ports[port_number] != null) && (bus_export != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", m_snd_req_ports[port_number].get_full_name(), bus_export.get_full_name()))
         m_snd_req_ports[port_number].connect(bus_export);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_obs_req_port", port_number);
      if ((port_number < m_obs_req_exports.size()) && (m_obs_req_exports[port_number] != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_obs_req_exports[port_number].get_full_name()))
         bus_port.connect(m_obs_req_exports[port_number]);
      end

      bus_port = bus_verif_comp.get_req_desc_port("m_rcv_rsp_port", port_number);
      if ((port_number < m_rcv_rsp_exports.size()) && (m_rcv_rsp_exports[port_number] != null) && (bus_port != null)) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", bus_port.get_full_name(), m_rcv_rsp_exports[port_number].get_full_name()))
         bus_port.connect(m_rcv_rsp_exports[port_number]);
      end
   endfunction : connect_my_bus_ports


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      evl_sequencer_mem mem_sequencer;

      super.build_phase(phase);

      if (get_int_param_by_name("pk_observe_descs", 1) != 1) begin
         `evl_log(UVM_HIGH, VMOD_AXI_STIM, get_abstract_name(), $sformatf("pk_observe_descs int param seen in rtl_port. Setting m_observe_descs to %0x", get_int_param_by_name("pk_observe_descs", 1)))
         m_observe_descs = get_int_param_by_name("pk_observe_descs", 1);
      end
      if (m_rtl_port != null) begin
         m_data_width = m_rtl_port.get_int_param_by_name("pk_data_width", `EVL_LINE_BYTES);
      end
      if (m_data_width < 1) begin
         m_data_width = `EVL_LINE_BYTES;
      end
      m_data_width_clog2 = $clog2(m_data_width);
      m_max_cmd_length   = m_data_width;

      m_num_l3_ports     = 1 << $clog2(get_int_param(pk_num_l3_ports, 1));
      m_num_l3_port_mask = m_num_l3_ports - 1;

      m_active_bus_req_ids = new[m_num_l3_ports];
      if (m_sequencer_create_count > 1) begin
         for (int ii = 0; ii < m_sequencer_create_count; ii++) begin
            mem_sequencer = evl_sequencer_mem::type_id::create($sformatf("sequencer_%0d", ii), this);
            set_sequencer_wrapper(mem_sequencer.get_wrapper());
         end
      end
      else begin
         mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
         set_sequencer_wrapper(mem_sequencer.get_wrapper());
      end

      m_cache = evl_stim_cache_axi::type_id::create("cache", this);
      m_cache.set_abstract_name($sformatf("%0s($)", get_root_abstract_name()));
      m_cache.set_rand_gen(m_rand_gen);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach (m_obs_req_exports[ii]) begin
         if (m_obs_req_exports[ii] != null) begin
            m_obs_req_exports[ii].connect(m_obs_req_fifos[ii].analysis_export);
         end
      end
      foreach (m_rcv_rsp_exports[ii]) begin
         if (m_rcv_rsp_exports[ii] != null) begin
            m_rcv_rsp_exports[ii].connect(m_rcv_rsp_fifos[ii].analysis_export);
         end
      end
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_cache_data = evl_line_data::type_id::create(get_abstract_name());
      m_template_req_data   = evl_line_data::type_id::create(get_abstract_name());
      m_template_line_desc  = evl_line_desc::type_id::create(get_abstract_name());
      m_template_trans_desc = evl_trans_desc::type_id::create(get_abstract_name());
      m_template_req_desc   = evl_req_desc::type_id::create(get_abstract_name());

      m_template_trans_desc.set_shire_id(m_shire_id);
      m_template_trans_desc.set_neighborhood_id(m_neighborhood_id);
      m_template_trans_desc.set_core_id(m_core_id);

      m_template_req_desc.set_shire_id(m_shire_id);
      m_template_req_desc.set_neighborhood_id(m_neighborhood_id);
      m_template_req_desc.set_core_id(m_core_id);

      m_template_line_desc.set_rand_gen(m_rand_gen);
      m_template_trans_desc.set_rand_gen(m_rand_gen);
      m_template_req_desc.set_rand_gen(m_rand_gen);

      m_template_line_desc.set_master_agent(this);
      m_template_trans_desc.set_master_agent(this);
      m_template_req_desc.set_master_agent(this);

      m_template_line_desc.set_data_template(m_template_cache_data);
      m_template_trans_desc.set_data_template(m_template_cache_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_AXI);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

      m_cache.set_req_desc_template(m_template_req_desc);
      m_cache.set_line_template(m_template_line_desc);

      m_sc_agent = evl_verif_comp_map::get_verif_comp(SHIRE_CACHE_RTL_AGENT);

      foreach (m_sequencer_wrappers[ii]) begin
         if ((m_sequencer_wrappers[ii] != null) && (m_sequencer_wrappers[ii].get_object() != null)) begin
            evl_sequencer_mem mem_sequencer;

            $cast(mem_sequencer, m_sequencer_wrappers[ii].get_object());
            mem_sequencer.set_trans_desc_template(m_template_trans_desc);
            m_mem_sequencers.push_back(mem_sequencer);
         end
      end

      if (m_tables_built++ == 0) begin
         build_tables();
      end
      m_tables_built = 1;

      foreach (m_rcv_rsp_fifos[ii]) begin
         watch_for_received_responses(m_rcv_rsp_fifos[ii], ii);
      end
      foreach (m_obs_req_fifos[ii]) begin
         if (ii inside { m_hi_priority_ports }) begin
            watch_for_received_requests(m_obs_req_fifos[ii], ii, 1'b1);
         end
         else begin
            watch_for_received_requests(m_obs_req_fifos[ii], ii, 1'b0);
         end
      end
      fork
         master_fetch_and_process_loop();
         watch_for_timeouts();
      join_none
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      fork
         begin
            super.post_reset_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostResetPhase), 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      fork
         begin
            super.post_configure_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostConfigurePhase), 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      m_count_stimulus = 1'b1;
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      fork
         begin
            super.post_main_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(PostMainPhase), 1'b1, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      fork
         begin
            super.shutdown_phase(phase);
         end
         begin
            phase.raise_objection(this);
            wait_for_active_mem_desc(m_global_attrs.get_timeout_value(ShutdownPhase), 1'b1, 1'b0, 1'b1);
            phase.drop_objection(this);
         end
      join
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: report_phase
   //
   virtual function void report_phase(uvm_phase phase);
      super.report_phase(phase);
      if (m_trans_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of transactions performed during test: %0d", m_trans_count))
      end
      if (m_req_sent_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of bus requests sent during test:      %0d", m_req_sent_count))
      end
      if (m_req_rcv_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of bus requests received during test:  %0d", m_req_rcv_count))
      end
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_verif_comp",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = NO_AGENT);
      int value;

      super.new(name_in, parent_in, agent_type_in);
      m_allow_mem_load = 1'b0;
      if (`et_test_plusargs("serialize_on_addr_done=") != 0) begin
         if (`et_value_plusargs("serialize_on_addr_done=%d", value) != 0) begin
            if (value != 0) begin
               set_int_param(pk_serialize_on_addr_done, 1);
            end
         end
      end
      else if (`et_test_plusargs("serialize_on_addr_done") != 0) begin
         set_int_param(pk_serialize_on_addr_done, 1);
      end
      if (`et_value_plusargs("random_req_id=%d", value) != 0) begin
         if (value == 0) begin
            m_random_req_id = 1'b0;
         end
      end
      if (`et_value_plusargs("axi_active_limit=%d", value) != 0) begin
         if (value < 1) begin
            m_active_limit = 1;
         end
         else if (value > 2048) begin
            m_active_limit = 2048;
         end
         else begin
            m_active_limit = value;
         end
      end // if (`et_value_plusargs("axi_active_limit=%d", value) != 0)
      if (`et_value_plusargs("axi_issue_limit=%d", value) != 0) begin
         if (value < 1) begin
            m_issue_limit = 1;
         end
         else if (value > 2048) begin
            m_issue_limit = 2048;
         end
         else begin
            m_issue_limit = value;
         end
      end // if (`et_value_plusargs("axi_issue_limit=%d", value) != 0)
      if (`et_test_plusargs("axi_mem_map_err=") != 0) begin
         if (`et_value_plusargs("axi_mem_map_err=%d", value) != 0) begin
            m_axi_mem_map_err = (value == 0) ? 1'b0 : 1'b1;
         end
      end
      else if (`et_test_plusargs("axi_mem_map_err") != 0) begin
         m_axi_mem_map_err = 1'b1;
      end
      if (`et_test_plusargs("axi_allow_duplicate_sent_ids") != 0) begin
         m_allow_duplicate_sent_ids = 1'b1;
      end
   endfunction : new

endclass : evl_stim_agent_axi

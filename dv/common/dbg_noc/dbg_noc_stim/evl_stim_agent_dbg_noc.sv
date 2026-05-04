//--------------------------------------------------------------------------------------------------
//
// Class: evl_stim_agent_dbg_noc
//
class evl_stim_agent_dbg_noc extends evl_verif_comp;

   `uvm_component_utils(evl_stim_agent_dbg_noc)

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
   // pk_req_on_port_zero:   This is not really a delay, but it is a way to force an DBG_NOC request
   //                        to use DBG_NOC port 0.
   //
   `evl_param_key(pk_allow_loopback)
   `evl_param_key(pk_bus_req_delay)
   `evl_param_key(pk_forwarded_req)
   `evl_param_key(pk_loopback_region)
   `evl_param_key(pk_mem_bus_rsp_delay)
   `evl_param_key(pk_mem_req_delay)
   `evl_param_key(pk_req_on_port_zero)
   `evl_param_key(pk_num_l3_ports)
   `evl_param_key(pk_from_dbg_noc_stim_agent)
   `evl_param_key(pk_terminate_on_rsp)
   `evl_param_key(pk_req_delay)
   `evl_param_key(pk_serialize_on_addr_done)
   `evl_param_key(pk_serialize_on_addr_issue)
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
   bit                     m_dbg_noc_mem_map_err          = 1'b0;
   bit                     m_bank_is_lsb_of_id        = 1'b0;
   int                     m_bank_shift               = `EVL_LINE_ADDR_LSB;
   int                     m_l3_port                  = -1;
   int                     m_uc_port                  = -1;
   int                     m_sys_port                 = -1;
   semaphore               m_get_trans_desc_busy      = new(1);
   semaphore               m_rcv_req_semaphore        = new(1);
   evl_bus_req_id_t        m_bus_req_fwd_id           = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_bus_req_fwd_id_limit     = evl_bus_req_id_t'(0);
   evl_bus_req_id_t        m_next_bus_req_fwd_id      = evl_bus_req_id_t'(0);
   int                     m_hi_priority_ports[$];
   evl_req_desc            m_hi_priority_queue[$];
   evl_req_desc            m_lo_priority_queue[$];
   evl_trans_desc          m_active_bus_reqs[$];
   evl_bus_req_id_queue    m_bus_req_id_queues[int];
   evl_req_desc            m_pending_reqs[$];
   evl_line_data           m_template_cache_data;
   evl_line_data           m_template_req_data;
   evl_line_desc           m_template_line_desc;
   evl_trans_desc          m_template_trans_desc;
   evl_req_desc            m_template_req_desc;
   evl_bus_req_id_t        m_bus_req_id;
   evl_sequencer_mem       m_mem_sequencer;
   int                     m_obs_req_mb_map[$];
   mailbox #(evl_req_desc) m_obs_req_mbs[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: fix_stimulus_trans_desc_early
   //
   virtual function void fix_stimulus_trans_desc_early(input evl_trans_desc trans_desc_in);
   endfunction : fix_stimulus_trans_desc_early


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_info
   //
   virtual function string format_req_info(input evl_trans_desc trans_desc);
      evl_cmd_t      cmd;
      string         message;
      evl_line_data  wr_data;

      cmd     = trans_desc.get_cmd();
      message = "";

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
   endfunction : format_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_req
   //
   virtual function void print_req(input evl_trans_desc trans_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("%0s (%0d)", format_req_info(trans_desc), trans_desc.get_cmd_length()))
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
   // Task: fetch_and_process_loop_task
   //
   virtual task fetch_and_process_loop_task(input evl_sequencer_mem mem_sequencer);
      forever begin
         wait_cycles(1);
         while (stimulus_is_enabled() == 1'b1) begin
            fetch_and_process_trans_desc(mem_sequencer);
         end
         wait_for_stimulus_enable();
      end
   endtask : fetch_and_process_loop_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: fetch_and_process_loop
   //
   virtual function void fetch_and_process_loop(input evl_sequencer_mem mem_sequencer);
      fork
         fetch_and_process_loop_task(mem_sequencer);
      join_none
   endfunction : fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Task: master_fetch_and_process_loop
   //
   virtual task master_fetch_and_process_loop();
      int sequencer_count = 0;

      wait_for_stimulus_enable();
      fetch_and_process_loop(m_mem_sequencer);
      wait (sequencer_count == 0);
   endtask : master_fetch_and_process_loop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: select_req_port
   //
   virtual function int select_req_port(input evl_req_desc req_desc);
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
   virtual task fetch_and_process_trans_desc(input evl_sequencer_mem mem_sequencer);
      evl_trans_desc          trans_desc;
      evl_sequence_mem        sequence_mem;
      evl_req_desc            req_desc;
      evl_table_desc          table_desc;
      evl_line_desc           line_desc;
      evl_cmd_t               cmd;
      int                     delay_value;
      evl_bus_req_id_t        bus_req_id;
      evl_paddr_t             bus_addr;

      req_desc = null;
      mem_sequencer.get_trans_desc(trans_desc, sequence_mem);
      if (trans_desc == null) begin
         wait_cycles(1);
         return;
      end
      cmd = trans_desc.get_cmd();
      `evl_log_addr(UVM_HIGH, VMOD_DBG_NOC_STIM, get_abstract_name(), trans_desc.get_paddr(), $sformatf("fetch_and_process got this cmd: %0p", cmd))
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
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
         if (m_count_stimulus == 1'b1) begin
            m_trans_count++;
         end
         m_stimulus_busy--;
         wait_cycles(1);
         return;
      end

      if (!(cmd inside { ST_DBG_NOC })) begin
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
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

         $cast(req_desc, m_template_req_desc.clone());
         trans_desc.copy_to_desc(req_desc);
         trans_desc.add_child_desc(req_desc);

         req_desc.set_paddr(trans_desc.get_paddr());
         if (cmd inside { ST_DBG_NOC }) begin
            req_desc.set_req_xword(0, trans_desc.get_write_xword(0));
         end

         //
         // Do we need to drop the request?
         //
         if (((stimulus_is_enabled() == 1'b0) || (resource_conflict == 1'b1)) && (trans_desc.can_drop_trans() == 1'b1)) begin
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
         trans_desc.set_done();
         delete_active_mem_desc(trans_desc);
      end
      else begin
         req_desc.set_as_original_req();

         `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_STIM, get_abstract_name(), req_desc, $sformatf("sending this request descriptor"))
         m_pending_reqs.push_back(req_desc);
         if (m_count_stimulus == 1'b1) begin
            m_req_sent_count++;
         end
         trans_desc.set_int_param(pk_from_dbg_noc_stim_agent, 1);
         if (get_next_delay(delay_value, pk_req_on_port_zero, trans_desc) == 1'b1) begin
            if (delay_value != 0) begin
               req_desc.set_int_param(pk_req_on_port_zero, 1);
            end
         end
         m_snd_req_ports[0].write(req_desc);
      end

      //
      // Now that we have processed the request, allow another sequencer to proceed.
      //
      trans_desc.set_done();
      delete_active_mem_desc(trans_desc);      
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

   endfunction : process_received_response


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_process_received_request()
   //
   // This task will get called from process_received_request() in order to post-process the request
   // descriptor (e.g. bus_error).
   //
   virtual function void post_process_received_request(input evl_req_desc req_desc);
      if (m_dbg_noc_mem_map_err == 1'b1) begin
         bit         is_bus_err = 1'b0;
         evl_paddr_t paddr      = req_desc.get_paddr();

         if (addr_in_region(pk_scp_region, paddr)) begin
            if (paddr[22:0] >= 23'h280000) begin
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
         if (req_desc.get_paddr() == address) begin
            match_req_desc = req_desc;
            m_pending_reqs.delete(ii);
            return;
         end
      end
      return;
   endfunction : find_pending_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
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

      bus_port = bus_verif_comp.get_req_desc_port("m_obs_tx_req_port", port_number);
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

      m_snd_req_ports.push_back(null);
      m_rcv_rsp_exports.push_back(null);
      m_rcv_rsp_fifos.push_back(null);
      m_snd_req_ports[0]   = new($sformatf("m_snd_req_port"),   this);
      m_rcv_rsp_exports[0] = new($sformatf("m_rcv_rsp_export"), this);
      m_rcv_rsp_fifos[0]   = new($sformatf("m_rcv_rsp_fifo"),   this);

      m_mem_sequencer = evl_sequencer_mem::type_id::create("sequencer", this);
      set_sequencer_wrapper(m_mem_sequencer.get_wrapper());

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
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_DBG_NOC);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);

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
   function new(input string           name_in       = "evl_stim_agent_dbg_noc",
                input uvm_component    parent_in     = null,
                input evl_agent_type_t agent_type_in = DBG_NOC_MASTER_STIM_AGENT);
      int value;

      super.new(name_in, parent_in, agent_type_in);
      m_active_limit = 8;
      m_issue_limit = 8;
      set_abstract_name("DBG_NOC_MASTER(STIM)");
      m_allow_mem_load = 1'b0;
      set_active_master();
   endfunction : new

endclass : evl_stim_agent_dbg_noc

//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_fe_scoreboard
//
//  
class evl_minion_fe_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_minion_fe_scoreboard)


   //
   // Member Variables and Objects
   //
   evl_minion_rtl_port m_minion_rtl_port;

   evl_trans_desc_queue                  m_replay_queue;
   bit [1:0]                             m_fence = 2'b00; // Fence ongoing
   bit                                   m_scp_en = 1'b0; // State of the scratchpad
   mailbox #(bit)                        m_scp_ctrl_q;
   int                                   consecutive_stall_cycles = 0;
   bit                                   m_sb_en = 1'b1;  // whether this FrontEnd_SB is enabled
   bit                                   m_sb_access_fault_protection_disable = 1'b0; //choose whether we throw DUT error on Access Fault

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_rtl_events
   //
   //    This task gets events coming from the RTL monitor and calls the appropriate routine.
   //
   task watch_for_rtl_events();
      evl_minion_packet rtl_event;

      forever begin
         evl_minion_packet_u packet_info;

         m_minion_rtl_port.get_event_fe(rtl_event);
         packet_info = rtl_event.get_info();

         case (rtl_event.get_packet_type())
            MINION_STALL_FE_INT : begin
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_hazard === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_hazard"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_hazard_vpu === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_hazard_vpu"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_int_sboard_hazard === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_int_sboard_hazard"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_div === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_div"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_trans === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_trans"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_do_fence === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_do_fence"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_dcache === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_dcache"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_ctrl_stall_csr === 1 ) begin
                                        // Commenting out since this is causing alot of noise in log
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_ctrl_stall_csr"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.csr_fe_stall[0] === 1 ) begin
                                        // Commenting out since this is causing alot of noise in log
					//`evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  csr_fe_stall TID0"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.csr_fe_stall[1] === 1 ) begin
                                        // Commenting out since this is causing alot of noise in log
                                        //`evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  csr_fe_stall TID1"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.ex_gsc_busy === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  ex_gsc_busy"))
                                  end
                                  if (packet_info.stall_packet.info.id_ctrl_stall === 1 && packet_info.stall_packet.info.id_stall_fp2int === 1 ) begin
                                        `evl_log(UVM_DEBUG, m_vmod, get_abstract_name(), $sformatf("Core stalled this cycle due to  id_stall_fp2int"))
                                  end
                                  end


            MINION_OPC_FE_INT   : `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("FE-->IntPipe Instruction Packet::TID %0d::PC 0x%0x::OPC 0x%0x ",
                                           packet_info.opcode_packet.info.fe_thread_id,packet_info.opcode_packet.info.fe_inst_data.pc,packet_info.opcode_packet.info.fe_inst_data.inst_bits ))

            MINION_FE_ACCESS_FAULT : begin
                                     if (packet_info.opcode_packet.info.fe_inst_data.af0 === 1) begin
                                      `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("FE AF0 Occured::TID %d::PC 0x%x",
                                           packet_info.opcode_packet.info.fe_thread_id, packet_info.opcode_packet.info.fe_inst_data.pc ))    
                                       if (m_sb_access_fault_protection_disable === 1'b0) begin
                                        `dut_error(get_abstract_name(), $sformatf("Minion tried to fetch from a forbidden memory region (check Memory Spec)"))
                                       end   
                                     end
                                     if (packet_info.opcode_packet.info.fe_inst_data.af1 === 1) begin
                                      `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("FE AF1 Occured::TID %d::PC 0x%x",
                                           packet_info.opcode_packet.info.fe_thread_id, packet_info.opcode_packet.info.fe_inst_data.pc ))       
                                       if (m_sb_access_fault_protection_disable === 1'b0) begin
                                        `dut_error(get_abstract_name(), $sformatf("Minion compressed instruction fetch access fault (check Memory Spec)"))
                                       end   
                                     end

                                     end

            MINION_ICACHE_REQ_VALID_FE : begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("ICACHE valid high for this many cycles without READY:: %0d", packet_info.redirect_packet.info.icache_req_valid_hi_cycles))
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("ICACHE valid+ready happened this many cycles ago without RESP:: %0d", packet_info.redirect_packet.info.icache_req_valid_to_resp_cycles))
                                            // RTLMIN-5705: This error will be disabled while we have the patch to avoid livelock situation in place
                                            //              as response time may be ocassionally quite high
                                            //if (packet_info.redirect_packet.info.icache_req_valid_hi_cycles >= 15) begin
                                            //   `dut_error(get_abstract_name(), $sformatf("ICACHE Valid high for 15 cycles and ready still not signaled"))
                                            //end
                                            if (packet_info.redirect_packet.info.icache_req_valid_to_resp_cycles > 3) begin
                                               `dut_error(get_abstract_name(), $sformatf("ICACHE Valid+Ready signalled 4 cycles ago  and resp still not signaled"))
                                            end
				         end

            MINION_UC_REPLAY : begin
                                            if (packet_info.redirect_packet.info.t0_replay_no_redir === 1) begin
                                               `dut_error(get_abstract_name(), $sformatf("TID0 marked an instruction as replay but no replay redirect received"))
                                            end
                                            if (packet_info.redirect_packet.info.t1_replay_no_redir === 1) begin
                                               `dut_error(get_abstract_name(), $sformatf("TID1 marked an instruction as replay but no replay redirect received"))
                                            end
			       end


            MINION_UNCACHEABLE_FE : begin
                                               `dut_error(get_abstract_name(), $sformatf("Instructions should NEVER be marked Uncacheable"))
			       end

            MINION_REDIR_INT_FE : begin

                                     if (packet_info.redirect_packet.info.fe_redirect_thread[0] == 1'b1) begin
                                        if ($isunknown(packet_info.redirect_packet.info.fe_redirect_info[0].pc)) begin
                                           `dut_error(get_abstract_name(), $sformatf("TID0 Redirect Address is X"))
                                        end
                                        if (packet_info.redirect_packet.info.fe_redirect_ipi[0] == 1'b1) begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Interrupt Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_xcpt == 1'b1) begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Exception Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_csr_eret == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE ERET Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_csr_replay == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE CSR REPLAY Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_reg_replay == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE MEM REPLAY Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                        else begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Branch Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 0,packet_info.redirect_packet.info.fe_redirect_info[0].pc))
                                        end
                                     end

                                     if (packet_info.redirect_packet.info.fe_redirect_thread[1] == 1'b1) begin
                                        if ($isunknown(packet_info.redirect_packet.info.fe_redirect_info[1].pc)) begin
                                           `dut_error(get_abstract_name(), $sformatf("TID1 Redirect Address is X"))
                                        end
                                        if (packet_info.redirect_packet.info.fe_redirect_ipi[1] == 1'b1) begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Interrupt Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_xcpt == 1'b1) begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Exception Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_csr_eret == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE ERET Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_csr_replay == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE CSR REPLAY Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                        else if (packet_info.redirect_packet.info.fe_redirect_wb_reg_replay == 1'b1) begin
                                           `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE MEM REPLAY Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                        else begin
                                           `evl_log(UVM_MEDIUM, m_vmod, get_abstract_name(), $sformatf("IntPipe-->FE Branch Redirect Packet::TID %0d::RedirPC 0x%0x",
                                                 1,packet_info.redirect_packet.info.fe_redirect_info[1].pc))
                                        end
                                     end

                                  end
            default             : `dut_error(get_abstract_name(), $sformatf("Minion SB received an unknown RTL event!"))
         endcase
      end
   endtask : watch_for_rtl_events



   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_idle
   //
   task wait_for_idle();
      if (m_active_desc_count <= 0) begin
         return;
      end
      wait (m_active_desc_count <= 0);
   endtask: wait_for_idle







   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_minion_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "minion_fe_scoreboard: m_rtl_port is null, SAD!")
      end

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction: connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);

   endtask : post_shutdown_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("FrontEnd Scoreboard is %0s", m_sb_en ? "enabled" : "disabled"))

      fork
         super.run_phase(phase);
         if (m_sb_en == 1'b1) begin
            fork
                  watch_for_timeouts();
                  watch_for_rtl_events();
            join_none;
         end
      join_none;
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("FE(SB)");
      m_replay_queue = new("FEReplayQ");
      m_scp_ctrl_q = new();
      m_vmod = VMOD_FE_SB;

      if (!(`et_value_plusargs("fe_sb_en=%d", m_sb_en))) begin
         m_sb_en = 1'b1;   // default is enabled 
      end

      if (`et_test_plusargs("FESB_FETCH_PROTECTION_DISABLE")) begin
        m_sb_access_fault_protection_disable = 1'b1; 
      end
   endfunction : new

endclass : evl_minion_fe_scoreboard

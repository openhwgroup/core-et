//-----------------------------------------------------------------------------------------------
//
// Class: evl_minion_rtl_port
//
class evl_minion_rtl_port extends evl_rtl_port;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_shire_id:             This is the shire ID for this component.  If this component is not
   //                         local to a shire, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_neighborhood_id:      This is the neighborhood ID for this component.  If this component is
   //                         not local to a neighborhood, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_core_id:              This is the core ID for this component.  If this component is not
   //                         local to a core, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_obs_event:            This is the mailbox in which events are stored for the associated
   //                         scoreboard to pop.
   //
   evl_shire_id_t             m_shire_id        = ~evl_shire_id_t'(0);
   evl_neighborhood_id_t      m_neighborhood_id = ~evl_neighborhood_id_t'(0);
   evl_core_id_t              m_core_id         = ~evl_core_id_t'(0);
   evl_dword_t                m_rand_mem_addr[1:0];
   evl_dword_t                m_val2[1:0];
   evl_rand_gen               m_rand_gen;
   evl_dword_t                m_rsp_delays[evl_dword_t];

   mailbox #(evl_minion_packet) m_obs_event;
   mailbox #(evl_minion_packet) m_obs_event_fe;
   mailbox #(evl_minion_packet) m_obs_event_pmu;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_minion_event
   //
   task get_event_minion(output evl_minion_packet packet);
      m_obs_event.get(packet);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_event_fe
   //
   task get_event_fe(output evl_minion_packet packet);
      m_obs_event_fe.get(packet);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_event_pmu
   //
   task get_event_pmu(output evl_minion_packet packet);
      m_obs_event_pmu.get(packet);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_event
   //
   function void send_event(input int packet_type, input evl_minion_packet_u info);
      evl_minion_packet minion_packet;
      minion_packet = new();
      minion_packet.set_packet_type (packet_type);
      minion_packet.set_info        (info);
      void'(m_obs_event.try_put(minion_packet));
   endfunction : send_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_event_fe
   //
   function void send_event_fe(input int packet_type, input evl_minion_packet_u info);
      evl_minion_packet minion_packet;
      //`evl_log(UVM_MEDIUM, VMOD_MINION_SB, get_abstract_name(), $sformatf("IWAHEED send_event_fe "))
      minion_packet = new();
      minion_packet.set_packet_type (packet_type);
      minion_packet.set_info        (info);
      void'(m_obs_event_fe.try_put(minion_packet));
   endfunction : send_event_fe

   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_event_pmu
   //
   function void send_event_pmu(input int packet_type, input evl_minion_packet_u info);
      evl_minion_packet minion_packet;
      minion_packet = new();
      minion_packet.set_packet_type (packet_type);
      minion_packet.set_info        (info);
      void'(m_obs_event_pmu.try_put(minion_packet));
   endfunction : send_event_pmu


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_val2
   //
   //   Set the shadowed value of VALIDATION2 CSR
   //
   function void set_val2(input bit thread_id, input evl_dword_t val);
      m_val2[thread_id] = val;
   endfunction: set_val2


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_delay
   //
   function evl_dword_t get_rsp_delay(input evl_dword_t addr);
      if (m_rsp_delays.exists(addr) == 1'b1) begin
         return m_rsp_delays[addr];
      end else begin
         return 0;
      end
   endfunction : get_rsp_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_diag_rand
   //
   //   Get a random value from the SystemVerilog PRNG
   //   The value of the payload can modify the behaviour of this function:
   //     - 0: get a 32-bit random value
   //     - 1: get a 32-bit value between the lower and upper boundaries specified in VALIDATION2
   //          VALIDATION2[31:0]  = lower limit
   //          VALIDATION2[63:32] = upper limit
   //     - 2: get a 64-bit random value
   //
   function evl_dword_t get_diag_rand(input evl_dword_t payload, input bit thread_id);
      evl_dword_t rand_value;
      evl_dword_t rand_val;

      case (payload)
         0: begin
            rand_val = m_rand_gen.get_word();
         end
         1: begin
            evl_word_t upper, lower;
            {upper, lower} = m_val2[thread_id];
            `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("calling urandom lower %h upper %h", lower, upper));
            rand_val = m_rand_gen.get_word_range(upper, lower);
         end
         2: begin
            rand_val = m_rand_gen.get_dword();
         end
      endcase

      `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Forcing validation1 register for urandom %h", rand_val));
      return rand_val;
   endfunction: get_diag_rand


   //-----------------------------------------------------------------------------------------------
   //
   // Function: diag_rand_mem
   //
   //   Randomizes the memory starting from (addr) until adrr+sub_cmd[16:0]
   //   Uses the preloading agent to actually preload the addresses into the RTL and informs BEMU for
   //   the new values;
   //
   function diag_rand_mem(logic [55:0] addr, bit is_upper, bit thread_id);
      evl_verif_comp_map   verif_comp_map;
      evl_verif_comp       verif_comp_list[];
      int size = 0;

      verif_comp_map = evl_verif_comp_map::get_inst();
      verif_comp_map.get_verif_comp_list(verif_comp_list, MEM_STIM_AGENT);

      m_rand_mem_addr[thread_id] |=  addr[31:0] << is_upper*32;
      size = !(is_upper) ? addr[55:32] : 0;

      `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("diag_rand_mem: addr %h, size %0d", m_rand_mem_addr[thread_id], size));

      // Find preloading agent handle
      foreach (verif_comp_list[i]) begin
          if (verif_comp_list[i].is_stimulus_vc() &&  verif_comp_list[i].m_memory_agent == 1 && verif_comp_list[i].m_allow_mem_load == 1) begin
              `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Found preloading agent %s",verif_comp_list[i].get_abstract_name() ));
               verif_comp_list[i].set_random_data_use();
          end
      end

//   for (int i=0; i<size; i++) begin
//      evl_line_desc line_desc;
//      evl_cache m_cache;
//      evl_paddr_t new_paddr = addr + i;
//      //$cast(m_cache, verif_comp_list[i]);
//      //line_desc = m_cache.get_line_desc(new_paddr);
//      //line_desc.set_word(new_paddr >> `EVL_WORD_ADDR_LSB, $urandom());
//      `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Randomizding MEM[%h] %h",new_paddr, 'h0));
//  end

   endfunction: diag_rand_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: diag_inject_interrupt
   //
   function void diag_inject_interrupt(input evl_dword_t payload, input bit thread_id);
      evl_interrupt_t interrupt_type;

      interrupt_type = evl_interrupt_t'(payload[54:53]);

      `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Thread %0d injecting %0p interrupt (payload: 0x%0x)", thread_id, interrupt_type, payload))
      m_global_attrs.send_message("interrupt_mailbox", 0, interrupt_type, payload);
   endfunction: diag_inject_interrupt


   //-----------------------------------------------------------------------------------------------
   //
   // Function: diag_delay_response
   //
   function void diag_delay_response(input evl_dword_t payload);
      evl_dword_t delay = payload[55:40];
      evl_dword_t paddr = {payload[39:6], 6'b0};
      `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Set response delay for 0x%0x to %0d cycles", paddr, delay));
      m_rsp_delays[paddr] = delay;
   endfunction: diag_delay_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_diag_cmd
   //
   //   This function decodes the command written to the CSR VALIDATION1 and calls the appropriate
   //   action
   //
   function evl_dword_t process_diag_cmd(input bit thread_id, input evl_dword_t val);
      evl_diag_cmd_t cmd;
      evl_dword_t    payload;

      cmd     = evl_diag_cmd_t'(val[63:56]);
      payload = evl_dword_t'(val[55:0]);

      if (cmd inside { DIAG_RAND_MEM_UPPER, DIAG_RAND_MEM_LOWER, DIAG_UEI }) begin
         `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("Received DIAG_CMD T%0d cmd=%s payload 0x%h", thread_id, cmd.name(), payload))
      end
      else begin
         `evl_log(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), $sformatf("Received DIAG_CMD T%0d cmd=%s payload 0x%h", thread_id, cmd.name(), payload))
      end

      case (cmd)
         DIAG_PUTCHAR        : m_global_attrs.putchar(m_shire_id, m_neighborhood_id, m_core_id, thread_id, byte'(payload));
         DIAG_RAND           : return get_diag_rand(payload, thread_id);
         DIAG_CYCLE          : return get_cycle_count();
         DIAG_RAND_MEM_UPPER : diag_rand_mem(payload, thread_id, 1);
         DIAG_RAND_MEM_LOWER : diag_rand_mem(payload, thread_id, 0);
         DIAG_UEI            : diag_inject_interrupt(payload, thread_id);
         DIAG_ECC_INJ        : `evl_log(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), $sformatf("ECC Injection diagnostic command %0d", cmd))
         DIAG_RSP_DELAY      : diag_delay_response(payload);
         DIAG_NOP            : ;
         default             : `dut_error(get_abstract_name(), $sformatf("Unkown diagnostic command %0d", cmd))
      endcase

      return 0;

   endfunction: process_diag_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_thread
   //
   function void enable_thread(input bit thread_id);
      m_global_attrs.enable_thread(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: enable_thread


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_thread
   //
   function void disable_thread(input bit thread_id);
      m_global_attrs.disable_thread(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: disable_thread


   //-----------------------------------------------------------------------------------------------
   //
   // Function: thread_begin
   //
   function void thread_begin(input bit thread_id);
      m_global_attrs.incr_thread_started(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: thread_begin


   //-----------------------------------------------------------------------------------------------
   //
   // Function: thread_boot_begin
   //
   function void thread_boot_begin(input bit thread_id);
      m_global_attrs.incr_thread_boot_started(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: thread_boot_begin


   //-----------------------------------------------------------------------------------------------
   //
   // Function: thread_boot_end
   //
   function void thread_boot_end(input bit thread_id);
      m_global_attrs.incr_thread_boot_done(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: thread_boot_end


   //-----------------------------------------------------------------------------------------------
   //
   // Function: thread_pass
   //
   function void thread_pass(input bit thread_id);
      m_global_attrs.incr_thread_done(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: thread_pass


   //-----------------------------------------------------------------------------------------------
   //
   // Function: thread_fail
   //
   function void thread_fail(input bit thread_id);
      m_global_attrs.incr_thread_fail(m_shire_id, m_neighborhood_id, m_core_id, thread_id);
   endfunction: thread_fail


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   function void set_shire_id(input evl_shire_id_t shire_id_in);
      m_shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_neighborhood_id
   //
   function void set_neighborhood_id(input evl_neighborhood_id_t neighborhood_id_in);
      m_neighborhood_id = neighborhood_id_in;
   endfunction : set_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_id
   //
   function void set_core_id(input evl_core_id_t core_id_in);
      m_core_id = core_id_in;
   endfunction : set_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   function evl_shire_id_t get_shire_id();
      return m_global_attrs.get_current_shire_id(m_shire_id);
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_neighborhood_id
   //
   function evl_neighborhood_id_t get_neighborhood_id();
      return m_neighborhood_id;
   endfunction : get_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_id
   //
   function evl_core_id_t get_core_id();
      return m_core_id;
   endfunction : get_core_id


   //--------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string         full_name_in = "minion_rtl_port",
                input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0),
                input int            skip_levels_in[] = {});
      bit sb_en = 0;
      super.new(full_name_in, CORE_RTL_AGENT, agent_pid_in, "evl_minion_env", "", "", "", skip_levels_in);
      m_obs_event       = new();
      m_obs_event_fe    = new();
      m_obs_event_pmu   = new();
      m_rand_gen = evl_rand_gen::get_inst();
   endfunction : new

endclass : evl_minion_rtl_port

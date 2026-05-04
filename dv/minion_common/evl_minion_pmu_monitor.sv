//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_pmu_monitor
//
//  
class evl_minion_pmu_monitor extends evl_monitor;

   `uvm_component_utils(evl_minion_pmu_monitor)

   //-----------------------------------------------------------------------------------------------
   //
   // analysis port, to send the transactions to scoreboard/monitor/etc...
   //
   uvm_analysis_port #(evl_pmu_desc) m_obs_req_port;
   //uvm_analysis_port #(evl_pmu_desc) m_obs_rsp_port;

   // This is the queue to hold requests so we can match up the responses
   evl_pmu_desc      m_request_queue[$];

   evl_pmu_desc      m_template_pmu_desc;

   //
   // Member Variables and Objects
   //
   evl_minion_rtl_port m_minion_rtl_port;

   bit                  m_mon_en = 1'b1;  // whether this monitor is enabled
   evl_verbosity_mod_t  m_vmod   = VMOD_PMU_MON;

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_rtl_events
   //
   //    This task gets events coming from the RTL monitor and calls the appropriate routine.
   //
   task watch_for_rtl_events();
      evl_minion_packet rtl_event;
      evl_pmu_desc pmu_desc;

      forever begin
         evl_minion_packet_u packet_info;

         m_minion_rtl_port.get_event_pmu(rtl_event);
         packet_info = rtl_event.get_info();

         $cast(pmu_desc, m_template_pmu_desc.clone());
         pmu_desc.set_thread_id(packet_info.pmu_packet.info.pmu_thread_id);
         pmu_desc.set_cntr_id(packet_info.pmu_packet.info.pmu_cntr_id);

         case (rtl_event.get_packet_type())
            MINION_PMU_CNTR_WR   : begin
                                    `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Thread %0x Writing 0x%0x to mhpmcounter%0d", 
                                                packet_info.pmu_packet.info.pmu_thread_id, packet_info.pmu_packet.info.pmu_cntr_data, packet_info.pmu_packet.info.pmu_cntr_id))
                                    pmu_desc.set_cntr_data(packet_info.pmu_packet.info.pmu_cntr_data);
                                    pmu_desc.set_txn_type(PMU_TXN_CNTR_WR);
                                   end
            MINION_PMU_CNTR_RD   : begin
                                    `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Thread %0x Reading from mhpmcounter%0d", 
                                                packet_info.pmu_packet.info.pmu_thread_id, packet_info.pmu_packet.info.pmu_cntr_id))
                                    pmu_desc.set_cntr_data(packet_info.pmu_packet.info.pmu_cntr_data);
                                    pmu_desc.set_txn_type(PMU_TXN_CNTR_RD);
                                   end
            MINION_PMU_CNTR_INCR : begin
                                    evl_pmu_event_type_t incr_event;
                                    incr_event = evl_pmu_event_type_t'(packet_info.pmu_packet.info.pmu_event_type);
                                    `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Saw %p. Incrementing thread %0x mhpmcounter%0d", 
                                                evl_pmu_event_type_t'(incr_event), packet_info.pmu_packet.info.pmu_thread_id, packet_info.pmu_packet.info.pmu_cntr_id))
                                    pmu_desc.set_txn_type(PMU_TXN_CNTR_INCR);
                                   end
            MINION_PMU_EVENT_WR  : begin
                                    `evl_log(UVM_HIGH, m_vmod, get_abstract_name(), $sformatf("Thread %0x Writing 0x%0x to mhpmevent%0d", 
                                                packet_info.pmu_packet.info.pmu_thread_id, int'(packet_info.pmu_packet.info.pmu_event_type), packet_info.pmu_packet.info.pmu_cntr_id))
                                    pmu_desc.set_event_type(evl_pmu_event_type_t'(packet_info.pmu_packet.info.pmu_event_type));
                                    pmu_desc.set_txn_type(PMU_TXN_EVENT_WR);
                                   end
            default             : `dut_error(get_abstract_name(), $sformatf("PMU Monitor received an unknown RTL event!"))
         endcase
         m_obs_req_port.write(pmu_desc);
      end
   endtask : watch_for_rtl_events



   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pmu_desc_port
   //
   virtual function uvm_analysis_port#(evl_pmu_desc) get_pmu_desc_port(input string port_name      = "unknown",
                                                                       input int    port_number    = 0);
      if ((port_name == "m_obs_req_port") || (port_name == "obs_req_port") || (port_name == "obs_req")) begin
         return m_obs_req_port;
      end

      return null;
   endfunction : get_pmu_desc_port



   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);

      $cast(m_minion_rtl_port, m_rtl_port);
      if (m_rtl_port == null) begin
         `dut_error(get_abstract_name(), "minion_pmu_montior: m_rtl_port is null, SAD!")
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

      m_template_pmu_desc   = evl_pmu_desc::type_id::create($sformatf("%s(PMU)", get_root_abstract_name()));

      m_template_pmu_desc.set_shire_id(m_minion_rtl_port.get_shire_id());
      m_template_pmu_desc.set_neighborhood_id(m_minion_rtl_port.get_neighborhood_id());
      m_template_pmu_desc.set_core_id(m_minion_rtl_port.get_core_id());

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
      `evl_log(UVM_NONE, m_vmod, get_abstract_name(), $sformatf("Minion PMU Monitor is %0s", m_mon_en ? "enabled" : "disabled"))

      fork
         super.run_phase(phase);
         if (m_mon_en == 1'b1) begin
            fork
                  //watch_for_timeouts();
                  watch_for_rtl_events();
            join_none;
         end
      join_none;
   endtask: run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_pmu_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("PMU(MON)");

      m_obs_req_port = new("m_obs_req_port", this);

      if (!(`et_value_plusargs("min_pmu_mon_en=%d", m_mon_en))) begin
         m_mon_en = 1'b0;   // default is disabled 
      end

   endfunction : new

endclass : evl_minion_pmu_monitor

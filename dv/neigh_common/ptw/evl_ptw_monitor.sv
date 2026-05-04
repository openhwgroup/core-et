//--------------------------------------------------------------------------------------------------
//
// Class: evl_ptw_monitor
//
class evl_ptw_monitor extends evl_monitor;

   `uvm_component_utils(evl_ptw_monitor)

   // This is the handle to the RTL port
   evl_ptw_rtl_port_base m_ptw_rtl_port;

   // This is a handle back to the verification component
   evl_verif_comp m_verif_comp = null;

   //-----------------------------------------------------------------------------------------------
   //
   // analysis port, to send the transactions to scoreboard/monitor/etc...
   //
   uvm_analysis_port #(evl_ptw_desc) m_obs_req_port;
   uvm_analysis_port #(evl_ptw_desc) m_obs_rsp_port;

   // This is a queue of outstanding requests. There might only be one outstanding at all times
   evl_ptw_desc                      m_req_q[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_requests
   //
   task process_requests();
      evl_ptw_desc req_desc;
      evl_ptw_req  bus_req;

      forever begin
         m_ptw_rtl_port.get_obs_req(bus_req);
         req_desc = new();

         // Populate descriptor from bus info
         req_desc.set_vaddr(bus_req.addr << `EVL_PAGE_ADDR_LSB);
         req_desc.set_vm_mode(evl_vm_mode_t'(bus_req.satp_mode));
         req_desc.set_root_ppn(bus_req.satp_ppn << `EVL_PAGE_ADDR_LSB);
         req_desc.set_privilege(evl_privilege_t'(bus_req.prv));

         // Push it on pending queue so we can match the response and ship it to observers
         m_req_q.push_back(req_desc);
         m_obs_req_port.write(req_desc);
      end
   endtask : process_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   task process_responses();
      evl_ptw_desc         req_desc;
      evl_ptw_rsp          bus_rsp;
      evl_page_table_entry pte_rsp;

      forever begin
         m_ptw_rtl_port.get_obs_rsp(bus_rsp);

         if (m_req_q.size() == 0) begin
            `dut_error(get_abstract_name(), $sformatf("received a PTW response but no requests was outstanding"))
            req_desc = new();
         end
         else begin
            req_desc = m_req_q.pop_front();
         end

         req_desc.set_paddr(bus_rsp.ppn << `EVL_PAGE_ADDR_LSB);
         pte_rsp = new();
         pte_rsp.set_paddr(bus_rsp.ppn << `EVL_PAGE_ADDR_LSB);
         pte_rsp.set_cache_st(E);  // FUTURE: Emmanuel Marie A/D/V bits
         pte_rsp.set_attr(evl_base_pkg::evl_pte_attr_t'({bus_rsp.x, bus_rsp.w, bus_rsp.r}));
         if (bus_rsp.g == 1'b1) pte_rsp.set_global();
         if (bus_rsp.u == 1'b1) pte_rsp.set_user_mode();
         req_desc.set_rsp_pte(pte_rsp);
         if (bus_rsp.access_fault == 1'b1) req_desc.set_access_fault();

         m_obs_rsp_port.write(req_desc);
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_ptw_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
      fork
         process_requests();
         process_responses();
      join_none;
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_ptw_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_req_port  = new("m_obs_req_port", this);
      m_obs_rsp_port  = new("m_obs_rsp_port", this);
   endfunction : new

endclass: evl_ptw_monitor


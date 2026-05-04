//--------------------------------------------------------------------------------------------------
//
// Interface: evl_ptw_interface
//
`include "rtl/inc/soc.vh"

module evl_ptw_interface #(parameter int  AGENT_ID            = 0,
                           parameter int  SKIP_LEVELS[]       = {0}) (
      input wire                  isolate,
      input wire                  reset,
      input wire                  clock,

      // request signals
      inout wire                  req_valid,
      inout wire                  req_ready,
      inout wire minion_ptw_req   req_info,

      // response signals
      inout wire                  rsp_valid,
      inout wire minion_ptw_pte   rsp_info
   );

   typedef class evl_ptw_rtl_port;

   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_neigh_vc_pkg::*;

// clocking monitor_req_cb @(posedge clock);
// FUTURE: Emmanuel Marie input/output delays
//    default input #0.1ns output #0.1ns;
//    input req_valid;
//    input req_ready;
//    input req_info;
// endclocking


// clocking monitor_rsp_cb @(posedge clock);
// FUTURE: Emmanuel Marie input/output delays
//    default input #0.1ns output #0.1ns;
//    input rsp_valid;
//    input rsp_info;
// endclocking


   class evl_ptw_rtl_port extends evl_ptw_rtl_port_base;
      mailbox m_obs_req_mb;
      mailbox m_obs_rsp_mb;

      //-----------------------------------------------------------------------------------------------
      //
      // Function: get_obs_rsp
      // This function retrieves a bus_rsp object from the mailbox to send to
      // the monitor
      //
      virtual task get_obs_rsp(output evl_ptw_rsp bus_rsp);
         m_obs_rsp_mb.get(bus_rsp);
      endtask : get_obs_rsp


      //-----------------------------------------------------------------------------------------------
      //
      // Function: get_obs_req
      // This function retrieves a bus_req object from the mailbox to send to
      // the monitor
      //
      virtual task get_obs_req(output evl_ptw_req bus_req);
         m_obs_req_mb.get(bus_req);
      endtask : get_obs_req


      //-----------------------------------------------------------------------------------------------
      //
      // Task: monitor_req
      //
      task monitor_req_rsp();
         evl_ptw_req bus_req;
         evl_ptw_rsp bus_rsp;

         forever begin
            @(posedge clock);
            if ((rsp_valid === 1'b1) && (isolate !== 1'b1)) begin
               bus_rsp = rsp_info;
               `evl_log(UVM_HIGH, VMOD_PTW_IFC, $sformatf("%0s(PTW-IFC)", get_root_abstract_name()), $sformatf("PTW Response to agent %0d", AGENT_ID))
               m_obs_rsp_mb.put(bus_rsp);
            end
            if ((req_valid === 1'b1) && (req_ready === 1'b1) && (isolate !== 1'b1)) begin
               bus_req = req_info;
               `evl_log(UVM_HIGH, VMOD_PTW_IFC, $sformatf("%0s(PTW-IFC)", get_root_abstract_name()), $sformatf("PTW Request from agent %0d", AGENT_ID))
               m_obs_req_mb.put(bus_req);
            end
            ping_clock();
         end
      endtask : monitor_req_rsp


      //-----------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string         full_name_in     = "ptw_rtl_port",
                   input evl_agent_id_t agent_pid_in     = ~evl_agent_id_t'(0),
                   input int            skip_levels_in[] = {});
         super.new(full_name_in, PTW_BUS_AGENT, agent_pid_in, "evl_ptw_verif_comp", "", "", "", skip_levels_in);
         m_obs_req_mb = new();
         m_obs_rsp_mb = new();
      endfunction
   endclass

   bit              m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), PTW_BUS_AGENT, AGENT_ID, "evl_ptw_verif_comp", "", "", "", { SKIP_LEVELS });
   evl_ptw_rtl_port m_rtl_port    = new($sformatf("%m"), AGENT_ID, { SKIP_LEVELS });
   bit              m_power_off   = 1'b0;
   bit              m_power_on    = 1'b0;

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b1) && (m_power_off === 1'b0));
         m_rtl_port.power_off();
         m_power_off = 1'b1;
         m_power_on  = 1'b0;
         #0 wait (isolate !== 1'b1);
      end
   end

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b0) && (m_power_on === 1'b0));
         m_rtl_port.power_on();
         m_power_on  = 1'b1;
         m_power_off = 1'b0;
         #0 wait (isolate !== 1'b0);
      end
   end

   initial begin
      #0;
      fork
         forever begin
            wait (reset === 1'b1);
            m_rtl_port.assert_reset();
            wait (reset === 1'b0);
            m_rtl_port.deassert_reset();
         end
         m_rtl_port.monitor_req_rsp();
      join
   end

endmodule

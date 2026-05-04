   //-----------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the Shire top-level module.  It includes DV-specific
   // objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;

   //-----------------------------------------------------------------------------------------------
   //
   // Shire-specific RTL Port Class Definition
   //
   class shire_rtl_port extends evl_rtl_port;

      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string         full_name_in = "shire_rtl_port",
                   input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0));
         super.new(full_name_in, SHIRE_RTL_AGENT, agent_pid_in, "evl_shire_example_env");
      endfunction : new

   endclass : shire_rtl_port


   //
   // Create a Shire RTL port instance.
   //
   bit            m_register_me = new($sformatf("%m"), SHIRE_RTL_AGENT, SHIRE_ID, "evl_shire_example_env");
   shire_rtl_port m_rtl_port    = new($sformatf("%m"), SHIRE_ID);


   //
   // Create neighborhood ETLink bus agent interfaces for each neighborhood.
   //
   genvar neighborhood_idx;
   generate
      for (neighborhood_idx = 0; neighborhood_idx < NUM_NEIGH; neighborhood_idx = neighborhood_idx + 1) begin : m_neighborhood
         evl_etlink_interface #(.AGENT_ID (neighborhood_idx)) m_neighborhood (
                                                                             .reset     ( reset                                 ), 
                                                                             .clk       ( clk                                   ),
                                                                             .req_valid ( neigh_sc_req_valid [neighborhood_idx] ),
                                                                             .req_ready ( neigh_sc_req_ready [neighborhood_idx] ),
                                                                             .req_info  ( neigh_sc_req_info  [neighborhood_idx] ),
                                                                             .rsp_valid ( neigh_sc_rsp_valid [neighborhood_idx] ),
                                                                             .rsp_ready ( neigh_sc_rsp_ready [neighborhood_idx] ),
                                                                             .rsp_info  ( neigh_sc_rsp_info  [neighborhood_idx] )                                                                                     
                                                                             );
      end
   endgenerate


   //
   // Watch clocks, resets, etc., and transfer the information the RTL port.
   //
   initial begin
      #0;
      fork
         begin
            forever begin
               wait (reset === 1'b1);
               m_rtl_port.assert_reset();
               wait (reset === 1'b0);
               m_rtl_port.deassert_reset();
            end
         end
         begin
            wait (clk === 1'b0);
            forever begin
               @(posedge clk);
               // Capture pin values here
               m_rtl_port.m_cycle_count++;
               -> m_rtl_port.m_clock_event;
            end
         end
      join
   end

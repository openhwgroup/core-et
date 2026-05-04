`include "rtl/inc/soc.vh"

`ifndef __EVL_NOC_PKG
   `include "dv/noc/evl_noc_pkg.sv"
`endif

module evl_noc_interface #(parameter bit                                      NOC_STUB           = 1'b0,
                           parameter int                                      MINION_SHIRE_COUNT = 35,
                           parameter bit [63:0]                               MINION_SHIRE_STUB  = {63{1'b0}},
                           parameter bit [(2*`SOCTOP_MEMSHIRES_PER_SIDE)-1:0] MEMSHIRE_STUB      = {(2*`SOCTOP_MEMSHIRES_PER_SIDE){1'b0}}) (
      input wire reset,
      input wire clock
   );


   //-----------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the SoC top-level module.  It includes
   // DV-specific objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_soc_pkg::*;
   import evl_noc_pkg::*;


   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me   = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                 SOC_RTL_AGENT,
                                                                 ~evl_agent_id_t'(0),
                                                                 "evl_noc_env", "", "", "", { 1 });


   //-----------------------------------------------------------------------------------------------
   //
   // Create a SoC RTL port instance and drive it appropriately.
   //
   bit              m_start_monitor = 1'b0;
   bit              m_reset_on      = 1'b0;
   bit              m_reset_off     = 1'b0;
   evl_soc_rtl_port m_rtl_port      = new($sformatf("%m"), { 1 });

   initial begin
      m_rtl_port.set_int_param_by_name("pk_noc_stub", NOC_STUB);
      m_rtl_port.set_int_param_by_name("pk_minion_shire_count", MINION_SHIRE_COUNT);
      m_rtl_port.set_int_param_by_name("pk_minion_shire_stub_hi", MINION_SHIRE_STUB[63:32]);
      m_rtl_port.set_int_param_by_name("pk_minion_shire_stub_lo", MINION_SHIRE_STUB[31:0]);
      m_rtl_port.set_int_param_by_name("pk_memshire_stub", MEMSHIRE_STUB);
      #1ns m_start_monitor = 1'b1;
   end

   always @(posedge clock) begin
      if (m_start_monitor === 1'b1) begin
         if ((reset === 1'b1) && (m_reset_on === 1'b0)) begin
            m_rtl_port.assert_reset();
         end
         else if ((reset === 1'b0) && (m_reset_off === 1'b0)) begin
            m_rtl_port.deassert_reset();
         end
         m_reset_on  <= ~reset;
         m_reset_off <= reset;
         m_rtl_port.ping_clock();
      end
   end

endmodule

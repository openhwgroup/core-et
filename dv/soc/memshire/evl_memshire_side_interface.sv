`include "rtl/inc/soc.vh"

`ifndef DV_MEMSHIRE_USE_PLL_BEH
   `define DV_MEMSHIRE_USE_PLL_BEH 0
`endif // ifndef DV_MEMSHIRE_USE_PLL_BEH

module evl_memshire_side_interface #(parameter string MEMSHIRE_SIDE = "west") (
      input wire reset,
      input wire clock
   );


   //-----------------------------------------------------------------------------------------------
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_memshire_pkg::*;

   localparam MEMSHIRE_SIDE_ID = (MEMSHIRE_SIDE == "west") ? 0 : 1;


   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                               MEM_SHIRE_SIDE_RTL_AGENT,
                                                               MEMSHIRE_SIDE_ID,
                                                               "evl_memshire_side_env",
                                                               $sformatf("pk_memshire_side_id=%0d,pk_memshire_use_pll_beh=%0d", MEMSHIRE_SIDE_ID, `DV_MEMSHIRE_USE_PLL_BEH), "", "", { 1 });


   //-----------------------------------------------------------------------------------------------
   //
   // Create an RTL port instance and drive it appropriately.
   //
   int                   m_shire_inst_id;
   bit                   m_start_monitor = 1'b0;
   bit                   m_reset_on      = 1'b0;
   bit                   m_reset_off     = 1'b0;
   evl_memshire_rtl_port m_rtl_port      = new($sformatf("%m"), MEMSHIRE_SIDE_ID, $sformatf("pk_memshire_side_id=%0d,pk_memshire_use_pll_beh=%0d", MEMSHIRE_SIDE_ID, `DV_MEMSHIRE_USE_PLL_BEH), { 1 });

   initial begin
      m_rtl_port.set_int_param_by_name("pk_memshire_use_pll_beh", `DV_MEMSHIRE_USE_PLL_BEH);
      #1ns m_start_monitor = 1'b1;
   end

   always @(posedge clock) begin
      if (m_start_monitor === 1'b1) begin
         if (reset === 1'b1) begin
            if (m_reset_on === 1'b0) begin
               m_rtl_port.assert_reset();
            end
         end
         else if (reset === 1'b0) begin
            if (m_reset_off === 1'b0) begin
               m_rtl_port.deassert_reset();
            end
         end
         m_reset_on  <= reset;
         m_reset_off <= ~reset;

         m_rtl_port.ping_clock();
      end
   end
endmodule

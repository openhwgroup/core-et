`include "rtl/inc/soc.vh"

`ifndef __EVL_SOC_PKG
   `include "dv/soc/evl_soc_pkg.sv"
`endif

`ifdef ET_USE_BIST_RTL
   `define DV_USE_BIST_RTL 1
`else
   `define DV_USE_BIST_RTL 0
`endif // ifdef ET_USE_BIST_RTL

module evl_soc_interface #(parameter bit                                      NOC_STUB             = 1'b0,
                           parameter int                                      MINION_SHIRE_COUNT   = 35,
                           parameter bit [36:0]                               MINION_SHIRE_DISABLE = {36{1'b0}},
                           parameter bit [(2*`SOCTOP_MEMSHIRES_PER_SIDE)-1:0] DV_MEMSHIRE_DISABLE     = {(2*`SOCTOP_MEMSHIRES_PER_SIDE){1'b0}},
                           parameter int                                      NOC_SB_ERR_INTENSITY = 1) (
      input  wire reset,
      input  wire clock,
      input  wire clk_ref_min,
      input  wire clk_step_min
   );


   //-----------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the SoC top-level module.  It includes
   // DV-specific objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_soc_pkg::*;


   //-----------------------------------------------------------------------------------------------
   //
   // Create an instance identifier.
   //
   bit m_register_me   = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                 SOC_RTL_AGENT,
                                                                 ~evl_agent_id_t'(0),
                                                                 "evl_soc_env",
                                                                 $sformatf("pk_noc_stub=%0d,pk_minion_shire_count=%0d,pk_minion_shire_stub_hi=%0d,pk_minion_shire_stub_lo=%0d,pk_memshire_stub=%0d", NOC_STUB, MINION_SHIRE_COUNT, MINION_SHIRE_DISABLE[35:32], MINION_SHIRE_DISABLE[31:0], DV_MEMSHIRE_DISABLE),
                                                                 "", "", { 1 });


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
      m_rtl_port.set_int_param_by_name("pk_minion_shire_stub_hi", MINION_SHIRE_DISABLE[35:32]);
      m_rtl_port.set_int_param_by_name("pk_minion_shire_stub_lo", MINION_SHIRE_DISABLE[31:0]);
      m_rtl_port.set_int_param_by_name("pk_memshire_stub", DV_MEMSHIRE_DISABLE);
      m_rtl_port.set_int_param_by_name("pk_noc_sb_err_intensity", NOC_SB_ERR_INTENSITY);
      m_rtl_port.set_int_param_by_name("pk_use_bist_rtl", `DV_USE_BIST_RTL);
      #100ps m_start_monitor = 1'b1;
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
         m_rtl_port.ping_clock(0);
      end
   end

   always @(posedge clk_ref_min) begin
      if (m_start_monitor === 1'b1) begin
         m_rtl_port.ping_clock(1);
      end
   end

   always @(posedge clk_step_min) begin
      if (m_start_monitor === 1'b1) begin
         m_rtl_port.ping_clock(2);
      end
   end

   `ifdef EVL_SIMULATION
      logic [MINION_SHIRE_COUNT-1:0] mtips = '0;
      logic [MINION_SHIRE_COUNT-1:0] meips = '0;
      logic [MINION_SHIRE_COUNT-1:0] seips = '0;

      initial begin
         evl_message_packet packet;
         evl_dword_t        payload;
         evl_interrupt_t    packet_type;
         bit                irq_val;

         forever begin
            m_rtl_port.get_interrupt_packet(packet);
            if (packet != null) begin
               payload      = packet.get_packet_value();
               packet_type  = evl_interrupt_t'(packet.get_packet_type());
               irq_val = payload[55];

               if ((NOC_STUB == 1) && (packet_type inside {EVL_MTIP, EVL_MEIP, EVL_SEIP})) begin
                  for (int shire = 0; shire < MINION_SHIRE_COUNT; ++shire) begin
                     // Shire mask in lower bits of payload
                     if (payload[shire] == 1'b1) begin
                        `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "IRQ-INJECT", $sformatf("Forcing Shire %0d interrupt %0p -> %0s", shire, packet_type, (irq_val) ? "Set" : "Clear"))
                        case (packet_type)
                           EVL_MTIP : mtips[shire] = irq_val;
                           EVL_MEIP : meips[shire] = irq_val;
                           EVL_SEIP : seips[shire] = irq_val;
                           default  : `dut_error("IRQ-INJECT", "SoC interface received an illegal interrupt injection command")
                        endcase
                     end
                  end
               end
            end
         end
      end

      genvar gen_minion_shire;
      generate
         if (NOC_STUB == 1) begin
            for (gen_minion_shire = 0; gen_minion_shire < MINION_SHIRE_COUNT; ++gen_minion_shire) begin: SHIRE
               if (MINION_SHIRE_DISABLE[gen_minion_shire] == 0) begin
                  initial begin
                     #0;
                     `evl_log(UVM_LOW, VMOD_GLOBAL, "IRQ-INJECT", $sformatf("this simulation will force plic_mtip, plic_meips, and plic_seip for shire %0d", gen_minion_shire))
                  end
                  always @(mtips[gen_minion_shire]) begin
                     force SHIRE[gen_minion_shire].shire_top.plic_mtip = mtips[gen_minion_shire];
                  end
                  always @(meips[gen_minion_shire]) begin
                     force SHIRE[gen_minion_shire].shire_top.plic_meip = meips[gen_minion_shire];
                  end
                  always @(seips[gen_minion_shire]) begin
                     force SHIRE[gen_minion_shire].shire_top.plic_seip = seips[gen_minion_shire];
                  end
               end
            end
         end
      endgenerate

      `ifndef SKIP_FAKE_ESR_INIT
         initial begin
            bit [7:0] minion_freq_mode[`EVL_MAX_MINION_SHIRE_ID];
            bit [7:0] lvdpll_mode;
            int       int_mode;
            int       freq_mode_mhz;
            string    lvdpll_mode_string;
            string    strap_lvdpll;
            int       ii;

            ii = 0;
            if (`et_value_plusargs("LVDPLL_MODE=%s", lvdpll_mode_string) == 1) begin
               if (lvdpll_mode_string == "RAND") begin
                  for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
                     std::randomize(lvdpll_mode) with { lvdpll_mode >= 1; lvdpll_mode <=45; };
                     `ifdef DV_IO_SHIRE_STUB
                        if(`DV_IO_SHIRE_STUB == 0) begin
                           if(`et_value_plusargs("STRAP_LVDPLL=%s", strap_lvdpll) == 1) begin
                              if (strap_lvdpll == "24")  begin
                                 lvdpll_mode += 45;
                              end
                              if (strap_lvdpll == "EXT") begin
                                 lvdpll_mode += 90;
                              end
                           end
                        end
                     `endif
                     minion_freq_mode[ii] = lvdpll_mode;
                  end
               end
               else begin
                  string mode_suffix = lvdpll_mode_string.tolower();

                  for (int ii = 0; ii < lvdpll_mode_string.len(); ii++) begin
                     if ((lvdpll_mode_string.getc(ii) < "0") || (lvdpll_mode_string.getc(ii) > "9")) begin
                        lvdpll_mode_string = lvdpll_mode_string.substr(0, ii - 1);
                        break;
                     end
                  end
                  int_mode    = lvdpll_mode_string.atoi();
                  int_mode    = ((int_mode - 300) / 25) + 1;
                  lvdpll_mode = int_mode;
                  `ifdef DV_IO_SHIRE_STUB
                     if(`DV_IO_SHIRE_STUB == 0) begin
                        if(`et_value_plusargs("STRAP_LVDPLL=%s", strap_lvdpll) == 1) begin
                           if (strap_lvdpll == "24")  begin
                              lvdpll_mode += 45;
                           end
                           if (strap_lvdpll == "EXT") begin
                              lvdpll_mode += 90;
                           end
                        end
                     end
                  `endif
                  for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
                     minion_freq_mode[ii] = lvdpll_mode;
                  end
               end
            end
            else begin
               for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
                  minion_freq_mode[ii] = 19; // 750 MHz
               end
            end

            for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
               uvm_config_db #(bit[7:0])::set(null, "*", $sformatf("minion_freq_mode_%0d", ii), minion_freq_mode[ii]);
               freq_mode_mhz = ((minion_freq_mode[ii] - 1) * 25) + 300;
               `evl_log(UVM_LOW, VMOD_CONFIG, "SOC(RTL)", $sformatf("LVDPLL clock: Shire %0d, freq_mode %0d (%0dMHz)", ii, minion_freq_mode[ii],freq_mode_mhz))
            end
         end

         initial begin
            int       minion_primary_clock;
            string    minion_primary_clock_string;
            bit [3:0] clock_sel;

            if (`et_value_plusargs("MINION_PRIMARY_CLOCK=%s", minion_primary_clock_string) == 1) begin

               case(minion_primary_clock_string)
               	  "REF_CLOCK":  begin
                                   std::randomize(minion_primary_clock) with {
                                      minion_primary_clock inside {0, 1};
                                   };
               	                end
               	  "STEP_CLOCK": begin
                                   std::randomize(minion_primary_clock) with {
                                      minion_primary_clock inside {2, 3};
                                   };
               	                end
               	  "LVDPLL_0":   minion_primary_clock = 4;
               	  "LVDPLL_1":   minion_primary_clock = 5;
               	  "LVDPLL_2":   minion_primary_clock = 6;
               	  "LVDPLL_3":   minion_primary_clock = 7;
               	  "RAND":       begin
                                   std::randomize(minion_primary_clock) with {
                                      minion_primary_clock >= 0;
                                      minion_primary_clock <= 7;
                     				  minion_primary_clock dist {
                      					0 := 5,
                                        1 := 5,
                                        2 := 5,
                                        3 := 5,
                                        4 := 20,
                                        5 := 20,
                                        6 := 20,
                                        7 := 20
                                      };
                                   };
               	                end
               	  default:      minion_primary_clock = 2;
               endcase

               case(minion_primary_clock)
               	  0:   clock_sel = 4'b1000;
               	  1:   clock_sel = 4'b1001;
               	  2:   clock_sel = 4'b1010;
               	  3:   clock_sel = 4'b1011;
               	  4:   clock_sel = 4'b1100;
               	  5:   clock_sel = 4'b1101;
               	  6:   clock_sel = 4'b1110;
               	  7:   clock_sel = 4'b1111;
               	  default:      clock_sel = 4'b1100;
               endcase

               uvm_config_db #(bit[3:0])::set(null, "*", "minion_shire_clock_sel", clock_sel);
               `evl_log(UVM_LOW, VMOD_CONFIG, "SOC(RTL)", $sformatf("Clock sel set to: %0h", clock_sel))
            end
         end
      `endif

      initial begin
         int  neigh_feedback_delay;
         int  feedback_delay_diff;
         real minion_shire_dllfeedback_delay[`EVL_MAX_MINION_SHIRE_ID];
         real minion_neigh_dllfeedback_delay[`EVL_MAX_MINION_SHIRE_ID];
         int  ii;

         ii = 0;
         if (`et_test_plusargs("DLLFEEDBACK_DELAY_RAND") == 1) begin
            for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
               std::randomize(neigh_feedback_delay) with {
                     neigh_feedback_delay >= 0;
                     neigh_feedback_delay <= 1000;
                     (neigh_feedback_delay % 50) == 0;
                  };
               minion_neigh_dllfeedback_delay[ii] = (real'(neigh_feedback_delay)) / 1000;

               std::randomize(feedback_delay_diff) with {
                     feedback_delay_diff dist {
                        6 := 1,
                        36 := 1,
                        [7:35] :/ 1
                     };
                  };
               minion_shire_dllfeedback_delay[ii] = (real'(neigh_feedback_delay + (feedback_delay_diff * 50))) / 1000;
            end

            for (ii = 0; ii < `EVL_MAX_MINION_SHIRE_ID; ii++) begin
               uvm_config_db #(real)::set(null, "*", $sformatf("minion_shire_dllfeedback_delay_%0d", ii), minion_shire_dllfeedback_delay[ii]);
               `evl_log(UVM_LOW, VMOD_CONFIG, "SOC(RTL)", $sformatf("DLL Shire clock feedback delay: Shire %0d, minion_shire_dllfeedback_delay = %f", ii, minion_shire_dllfeedback_delay[ii]))

               uvm_config_db #(real)::set(null, "*", $sformatf("minion_neigh_dllfeedback_delay_%0d", ii), minion_neigh_dllfeedback_delay[ii]);
               `evl_log(UVM_LOW, VMOD_CONFIG, "SOC(RTL)", $sformatf("DLL Neigh clock feedback delay: Shire %0d, minion_neigh_dllfeedback_delay = %f", ii, minion_neigh_dllfeedback_delay[ii]))
            end
         end
      end

      initial begin
        bit[35:0] clk_dly_ctl_rand;
        bit[63:0] clk_dly_ctl[`EVL_MAX_MINION_SHIRE_ID];
        string    clk_dly_ctl_string;
        int       i;
        i=0;

        if(`et_value_plusargs("CLK_DLY_CTL=%s", clk_dly_ctl_string) == 1) begin
          if(clk_dly_ctl_string == "RAND") begin
            for(i=0; i<`EVL_MAX_MINION_SHIRE_ID; i++) begin
              clk_dly_ctl[i] = 0;
              std::randomize(clk_dly_ctl_rand) with {
                clk_dly_ctl_rand[5:0] == clk_dly_ctl_rand[11:6];
                clk_dly_ctl_rand[11:6] == clk_dly_ctl_rand[17:12];
                clk_dly_ctl_rand[17:12] == clk_dly_ctl_rand[23:18];
              };
              clk_dly_ctl[i][35:0] = clk_dly_ctl_rand;
            end
          end
          else begin
            clk_dly_ctl_rand = clk_dly_ctl_string.atoi();
            for(i=0; i<`EVL_MAX_MINION_SHIRE_ID; i++) begin
            	clk_dly_ctl[i] = 0;
            	clk_dly_ctl[i][35:0] = clk_dly_ctl_rand;
            end
          end
        end
        else begin
          for(i=0; i<`EVL_MAX_MINION_SHIRE_ID; i++) clk_dly_ctl[i] = 0;
        end

        for(i=0; i<`EVL_MAX_MINION_SHIRE_ID; i++) begin
          uvm_config_db #(bit[63:0])::set(null, "*", $sformatf("clk_dly_ctl_%0d", i), clk_dly_ctl[i]);
          $display("CLK_DLY_CTL : Shire %d, neigh0_tap = %d", i, clk_dly_ctl[i][5:0]);
          $display("CLK_DLY_CTL : Shire %d, neigh1_tap = %d", i, clk_dly_ctl[i][11:6]);
          $display("CLK_DLY_CTL : Shire %d, neigh2_tap = %d", i, clk_dly_ctl[i][17:12]);
          $display("CLK_DLY_CTL : Shire %d, neigh3_tap = %d", i, clk_dly_ctl[i][23:18]);
          $display("CLK_DLY_CTL : Shire %d, neigh_feedback_tap = %d", i, clk_dly_ctl[i][29:24]);
          $display("CLK_DLY_CTL : Shire %d, shire_feedback_tap = %d", i, clk_dly_ctl[i][35:30]);
        end
      end
   
   `include "dv/soc/minion_gatesim_upf_assert.sv"

   `endif
endmodule

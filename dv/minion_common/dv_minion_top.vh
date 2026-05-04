   //
   // This file is included in rtl/shire/minion/minion_top.v for simulations
   // only.  It obviously assumes the presence of certain parameters and
   // wires.
   //
   `ifdef ET_SIMULATION
      `ifdef ENABLE_BEMU
         localparam bit PREVENT_DPI = 1'b0;
      `else
         localparam bit PREVENT_DPI = 1'b1;
      `endif
      `ifdef EVL_SIMULATION
         wire [63:0] soc_cycle;

         testend_monitor #(.SHIRE_ID(SHIRE_ID), .NEIGH_ID(NEIGH_ID), .MINION_ID(MINION_ID), .PREVENT_DPI(PREVENT_DPI)) testend_mon (.*);
         evl_minion_interface #(.MINION_ID(MINION_ID)) minion_ifc( .reset (reset_non_debug | reset ), .isolate (iso_enable), .clock(clock));
         `ifdef ENABLE_BEMU
            arch_state_monitor arch_state_mon (.*);
            perf_event_monitor perf_event_mon (.*);
            dreams_event_monitor dreams_event_mon (.*);
            // Temporary disabling this Port Monitor for Eagle based benches due
            // to JIRA -> RTLMIN-1937
            // port_monitor port_mon (.*);
         `endif // ENABLE_BEMU
         `ifdef ENABLE_ZSIM
            zsim_monitor zsim_mon (.*);
         `endif // ENABLE_ZSIM
      `else // ifdef EVL_SIMULATION
         `ifndef ET_NO_TESTEND_MONITOR
            wire [63:0] soc_cycle;
            testend_monitor #(.SHIRE_ID(SHIRE_ID), .NEIGH_ID(NEIGH_ID), .MINION_ID(MINION_ID), .PREVENT_DPI(PREVENT_DPI)) testend_mon (.*);
         `endif // ifndef ET_NO_TESTEND_MONITOR

         `ifdef ENABLE_BEMU
             arch_state_monitor arch_state_mon (.*);
         `endif
        `endif
   `endif // !ifdef ET_SIMULATION

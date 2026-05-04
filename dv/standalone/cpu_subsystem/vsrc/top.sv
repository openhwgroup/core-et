`include "dv/inc/et_tb_defines.vh"
`include "rtl/inc/soc.vh"

`define TIMING

`define MIN_IFC_INTPIPE cpu_subsystem_top.i_neigh.gen_minions[0].min_inst.minion.core.intpipe

`ifdef USE_REAL_DDRC
   `ifndef ET_DEFAULT_NET_IS_WIRE
      `define ET_DEFAULT_NET_IS_WIRE 1
   `endif // ifndef ET_DEFAULT_NET_IS_WIRE
`endif // ifdef USE_REAL_DDRC

`ifdef ET_DEFAULT_NET_IS_WIRE
   `default_nettype wire
`else // ifdef ET_DEFAULT_NET_IS_WIRE
   // cause error if a single bit wire is attempted to be assigned but has not been declared
   `default_nettype none
`endif // !ifdef ET_DEFAULT_NET_IS_WIRE

`include "rtl/inc/timescale.vh"


//--------------------------------------------------------------------------------------------------
module top
(
 input wire tb_clk
);

   // System signals
   logic         clock;
   logic         clock_ext;   
   logic         reset_cold;
   logic         reset_warm;

   // Power control
   logic         cpu_pd_req;
   logic         cpu_pd_ack;
   logic [7:0]   minion_pd_req;
   logic [7:0]   minion_pd_ack;

   // Interrupt requests
   logic [`PLIC_NEXTINT_SOURCES-1:0]  plic_irq;

   // AXI MASTER INTERFACE
   // Read address channel
   logic [7:0]   axi_ARID;
   logic [31:0]  axi_ARADDR;
   logic [7:0]   axi_ARLEN;
   logic [2:0]   axi_ARSIZE;
   logic [1:0]   axi_ARBURST;
   logic         axi_ARLOCK;
   logic [3:0]   axi_ARCACHE;
   logic [2:0]   axi_ARPROT;
   logic         axi_ARVALID;
   logic         axi_ARREADY;
   // Read data channel
   logic [7:0]   axi_RID;
   logic [511:0] axi_RDATA;
   logic [1:0]   axi_RRESP;
   logic         axi_RLAST;
   logic         axi_RVALID;
   logic         axi_RREADY;
   // Write address channel
   logic [7:0]   axi_AWID;
   logic [31:0]  axi_AWADDR;
   logic [7:0]   axi_AWLEN;
   logic [2:0]   axi_AWSIZE;
   logic [1:0]   axi_AWBURST;
   logic         axi_AWLOCK;
   logic [3:0]   axi_AWCACHE;
   logic [2:0]   axi_AWPROT;
   logic         axi_AWVALID;
   logic         axi_AWREADY;
   // Write data channel
   logic [511:0] axi_WDATA;
   logic [63:0]  axi_WSTRB;
   logic         axi_WLAST;
   logic         axi_WVALID;
   logic         axi_WREADY;
   // Write response channel
   logic [7:0]   axi_BID;
   logic [1:0]   axi_BRESP;
   logic         axi_BVALID;
   logic         axi_BREADY;
   // APB SLAVE INTERFACE
   logic         apb_psel;
   logic         apb_penable;
   logic         apb_pwrite;
   logic [`CPU_APB_ADDR_WIDTH-1:0]  apb_paddr;
   logic [`CPU_APB_DATA_WIDTH-1:0]  apb_pwdata;
   logic [`CPU_APB_DATA_WIDTH-1:0]  apb_prdata;
   logic         apb_pready;
   logic         apb_pslverr;

   // APB SLAVE SUBSYSTEM
   logic         cpu_apb_psel;
   logic         cpu_apb_penable;
   logic         cpu_apb_pwrite;
   logic [`CPU_APB_ADDR_WIDTH-1:0]  cpu_apb_paddr;
   logic [`CPU_APB_DATA_WIDTH-1:0]  cpu_apb_pwdata;
   logic [`CPU_APB_DATA_WIDTH-1:0]  cpu_apb_prdata;
   logic         cpu_apb_pready;
   logic         cpu_apb_pslverr;


   // APB INJECTOR
   logic         injector_enabled;
   logic         injector_apb_psel;
   logic         injector_apb_penable;
   logic         injector_apb_pwrite;
   logic [`CPU_APB_ADDR_WIDTH-1:0]  injector_apb_paddr;
   logic [`CPU_APB_DATA_WIDTH-1:0]  injector_apb_pwdata;
   logic [`CPU_APB_DATA_WIDTH-1:0]  injector_apb_prdata;
   logic         injector_apb_pready;
   logic         injector_apb_pslverr;

   //CSR monitor signals
   wire [11:0]                           mon_csr_mem_addr;
   reg  [11:0]                           mon_csr_wb_addr;
   wire [2:0]                            mon_csr_wb_cmd;
   wire [`XREG_RANGE]                    mon_csr_wb_wdata;
   wire                                  mon_csr_wb_thread_id;

   cpu_subsystem_top
   cpu_subsystem_top
   (
    .clock ( clock ),
    .clock_ext ( clock_ext ),    
    .reset_cold ( reset_cold ),
    .reset_warm ( reset_warm ),
    .cpu_pd_req ( cpu_pd_req ),
    .cpu_pd_ack ( cpu_pd_ack ),
    .minion_pd_req ( minion_pd_req ),
    .minion_pd_ack ( minion_pd_ack ),
    .plic_irq ( plic_irq ),
    .axi_ARID ( axi_ARID ),
    .axi_ARADDR ( axi_ARADDR ),
    .axi_ARLEN ( axi_ARLEN ),
    .axi_ARSIZE ( axi_ARSIZE ),
    .axi_ARBURST ( axi_ARBURST ),
    .axi_ARLOCK ( axi_ARLOCK ),
    .axi_ARCACHE ( axi_ARCACHE ),
    .axi_ARPROT ( axi_ARPROT ),
    .axi_ARVALID ( axi_ARVALID ),
    .axi_ARREADY ( axi_ARREADY ),
    .axi_RID ( axi_RID ),
    .axi_RDATA ( axi_RDATA ),
    .axi_RRESP ( axi_RRESP ),
    .axi_RLAST ( axi_RLAST ),
    .axi_RVALID ( axi_RVALID ),
    .axi_RREADY ( axi_RREADY ),
    .axi_AWID ( axi_AWID ),
    .axi_AWADDR ( axi_AWADDR ),
    .axi_AWLEN ( axi_AWLEN ),
    .axi_AWSIZE ( axi_AWSIZE ),
    .axi_AWBURST ( axi_AWBURST ),
    .axi_AWLOCK ( axi_AWLOCK ),
    .axi_AWCACHE ( axi_AWCACHE ),
    .axi_AWPROT ( axi_AWPROT ),
    .axi_AWVALID ( axi_AWVALID ),
    .axi_AWREADY ( axi_AWREADY ),
    .axi_WDATA ( axi_WDATA ),
    .axi_WSTRB ( axi_WSTRB ),
    .axi_WLAST ( axi_WLAST ),
    .axi_WVALID ( axi_WVALID ),
    .axi_WREADY ( axi_WREADY ),
    .axi_BID ( axi_BID ),
    .axi_BRESP ( axi_BRESP ),
    .axi_BVALID ( axi_BVALID ),
    .axi_BREADY ( axi_BREADY ),
    .apb_psel ( cpu_apb_psel ),
    .apb_penable ( cpu_apb_penable ),
    .apb_pwrite ( cpu_apb_pwrite ),
    .apb_paddr ( cpu_apb_paddr ),
    .apb_pwdata ( cpu_apb_pwdata ),
    .apb_prdata ( cpu_apb_prdata ),
    .apb_pready ( cpu_apb_pready ),
    .apb_pslverr ( cpu_apb_pslverr )
    );




`ifdef TIMING

 `ifdef VERILATOR
   initial begin
      clock = 1'b0;
      forever #0.5ns clock = !clock; // 1 GHz
   end
   initial begin
      clock_ext = 1'b0;
      forever #2.5ns clock_ext = !clock_ext; // 200 MHz
   end
 `else
   cpu_subsystem_clock_gen
   cpu_subsystem_clock_gen      
   (
    .clock_1ghz ( clock ),
    .clock_200mhz ( clock_ext )
   );
 `endif
   
`else
   assign clock = tb_clk;
   int clk_div = 0;
   // Dive clock by 5 -> 200Mhz
   always_ff @(posedge clock) begin
     clk_div <= (clk_div + 1)%5;
   end
   always_ff @(posedge clock) begin
      if (clk_div == 0)
        clock_ext <= !clock_ext;
   end
`endif

 // System signals
   assign cpu_pd_req= 0;
   assign minion_pd_req= 0;
   // assign plic_irq= 0;


   // End
////////////////
// Erbium STUB
////////////////
   erbium_ip_stub erbium_ip_stub (.reset(reset_warm), .*);
////////////////
// END Erbium STUB
////////////////
   // Mux between both apb buses (injector vs noc)
   always_comb begin
      if (injector_enabled) begin
         cpu_apb_psel         = injector_apb_psel;
         cpu_apb_penable      = injector_apb_penable;
         cpu_apb_pwrite       = injector_apb_pwrite;
         cpu_apb_paddr        = injector_apb_paddr;
         cpu_apb_pwdata       = injector_apb_pwdata;
         injector_apb_prdata  = cpu_apb_prdata;
         injector_apb_pready  = cpu_apb_pready;
         injector_apb_pslverr = cpu_apb_pslverr;

         // Default value for unconnected signals
         apb_pready = '0;
         apb_pslverr = '0;
         apb_prdata = '0;
      end else begin
         cpu_apb_psel         = apb_psel;
         cpu_apb_penable      = apb_penable;
         cpu_apb_pwrite       = apb_pwrite;
         cpu_apb_paddr        = apb_paddr;
         cpu_apb_pwdata       = apb_pwdata;
         apb_prdata  = cpu_apb_prdata;
         apb_pready  = cpu_apb_pready;
         apb_pslverr = cpu_apb_pslverr;

         // Default value for unconnected signals
         injector_apb_pready  = '0;
         injector_apb_pslverr = '0;
         injector_apb_prdata = '0;
      end
   end


   cpu_subsystem_apb_injector
     apb_injector
   (
    .clock       ( clock ),
    .reset       ( reset_warm ),
    .enabled     ( injector_enabled ),
    .apb_psel    ( injector_apb_psel ),
    .apb_penable ( injector_apb_penable ),
    .apb_paddr   ( injector_apb_paddr ),
    .apb_pwrite  ( injector_apb_pwrite ),
    .apb_pwdata  ( injector_apb_pwdata ),
    .apb_prdata  ( injector_apb_prdata ),
    .apb_pready  ( injector_apb_pready ),
    .apb_pslverr ( injector_apb_pslverr )
    );


   cpu_subsystem_irq_injector
   irq_injector
   (.*);


   //      CK      RST    DOUT                   DIN                 DEF
   `RST_FF(clock,  reset_warm, mon_csr_wb_addr,       mon_csr_mem_addr,   1'b0)

   // CSR accesses
   assign mon_csr_mem_addr           = `MIN_IFC_INTPIPE.csr_file.io_rw_mem_addr;
   assign mon_csr_wb_cmd             = `MIN_IFC_INTPIPE.csr_file.io_rw_wb_cmd;
   assign mon_csr_wb_wdata           = `MIN_IFC_INTPIPE.csr_file.io_rw_wb_wdata;
   assign mon_csr_wb_thread_id       = `MIN_IFC_INTPIPE.csr_file.wb_thread_id;

`ifdef TIMING
   reg  no_reset_deassert;
   initial begin
      int delay_value;

      delay_value = 0;
      #0;
      if (`et_test_plusargs("no_reset_deassert") != 0) begin
         no_reset_deassert = 1;
      end
      else begin
         no_reset_deassert = 0;
      end
      if (`et_value_plusargs("reset_delay=%d", delay_value) == 0) begin
         `ifdef UPF
            delay_value = 0;
         `else // ifdef UPF
            delay_value = 1;
         `endif // !ifdef UPF
      end
      if (delay_value < 0) begin
         delay_value = 0;
      end
      if (delay_value > 0) begin
         repeat (delay_value) begin
            @(posedge clock);
         end
      end
      reset_cold <= 1'b1;
      reset_warm <= 1'b1;
      repeat (20) @(posedge clock);
      if (no_reset_deassert == 0) begin
         reset_cold <= 1'b0;
         reset_warm <= 1'b0;
      end
   end // initial begin
 `endif //  `ifdef TIMING

   // Debugging shutdown
   initial begin
      if ($test$plusargs("emergency_shutdown")) begin
         repeat (5000) @(posedge clock);
         $finish;
      end
   end


   `ifdef ET_SIMULATION
    `ifdef VCS
        import "DPI-C" context function void vcs_startOfSim();  // Constructs the Checker TB
        import "DPI-C" context function void vcs_endOfReset();  // Indicates end of reset to start executing code
        initial begin
           $display("SYSTEM: Start Simulation");
           vcs_startOfSim();
           @(negedge reset_cold);
           $display("SYSTEM: Reset done");
           vcs_endOfReset();
           $display("SYSTEM: end of reset");
        end

        // Test end functionality
        // When the C-world request a test end it does it thought the endSimAt DPI
        // The simulation will wait the request cycles + 1 to avoid killing the simulation
        // inside the DPI.
        bit end_sim_enabled;
        int unsigned end_sim_time;
        bit end_sim, end_sim_next;
        export "DPI-C" function endSimAt_DPI;
        function void endSimAt_DPI(input int unsigned t);
        begin
           end_sim_enabled = 1'b1;
           end_sim_time = t + 1;
        end
        endfunction

        always_ff @(posedge clock) begin
           if (end_sim_enabled) end_sim_time <= end_sim_time-1;
        end

        assign end_sim_next = end_sim_enabled && end_sim_time == 0;
        always_ff @(posedge clock) begin
           end_sim <= end_sim_next;
        end
   
        always_ff @(posedge clock) begin
           if (end_sim) $finish;
        end
   
    `endif //  `ifdef VCS


       //////////////////////////////////////////////////////////////////
       // Tracing
       ////////////////////////////////////////////////////////////////
        bit dump_enable;
        bit trace_enable;
        initial begin
           if ($value$plusargs("dump=%d", dump_enable) || $value$plusargs("traceEnable=%d", trace_enable)) begin
              $display("[%0t] Tracing enabled\n", $time);
              `ifdef VERILATOR
                $dumpfile("trace.fst");
                $dumpvars();
              `else
                $vcdplusfile("trace.vpd");
                $vcdpluson;
              `endif
           end else begin
              $display("[%0t] Tracing disabled\n", $time);
           end
        end // initial begin

        final begin
           if (dump_enable || trace_enable) begin
                   `ifdef VERILATOR
              $dumpflush;
                   `else
              $fflushall;
                   `endif
           end
        end
   

         function void CoSim_dut_error(input string message_in);
            $error("EMU(C): ", message_in);
         endfunction : CoSim_dut_error

         export "DPI-C" function CoSim_dut_error;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_dut_warning
         //
         function void CoSim_dut_warning(input string message_in);
            $warning("EMU(C): ", message_in);
         endfunction : CoSim_dut_warning

         export "DPI-C" function CoSim_dut_warning;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_print
         //
         function void CoSim_agent_print(input string type_in,
                                         input string message_in);
            $info("EMU(C): ", message_in);
         endfunction : CoSim_agent_print

         export "DPI-C" function CoSim_agent_print;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_initialized
         //
         function void CoSim_agent_initialized();
           // TODO: Do something
            /// EVL does:
            //           static function void cosim_start();
            //    m_cosim_started = ~m_cosim_cleared;
            //   endfunction : cosim_start

         endfunction : CoSim_agent_initialized

         export "DPI-C" function CoSim_agent_initialized;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_drop_objection
         //
           function void CoSim_agent_drop_objection(input int result, input int minion_id, input int thread_id);
               //   static function void cosim_clear();
               //      m_cosim_cleared = 1'b1;
               //      m_cosim_started = 1'b0;
               //   endfunction : cosim_clear
               //
         endfunction : CoSim_agent_drop_objection
     
         export "DPI-C" function CoSim_agent_drop_objection;
   `endif //  `ifdef ET_SIMULATION
   
endmodule

